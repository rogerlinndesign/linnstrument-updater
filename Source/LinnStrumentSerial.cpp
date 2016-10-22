/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "LinnStrumentSerialMac.h"

#include "UpdaterApplication.h"

#include "Serial/serial/serial.h"

#include <algorithm>

namespace {
    static uint32_t crc_table[16] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };
    
	uint32_t crc_update(uint32_t crc, uint8_t data) {
        uint8_t tbl_idx;
		tbl_idx = (uint8_t)(crc ^ (data >> (0 * 4)));
        crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
		tbl_idx = (uint8_t)(crc ^ (data >> (1 * 4)));
        crc = crc_table[tbl_idx & 0x0f] ^ (crc >> 4);
        return crc;
    }
    
	uint32_t crc_byte_array(uint8_t* s, uint8_t size) {
		uint32_t crc = ~(uint32_t)0;
        for (uint8_t i = 0; i < size; ++i) {
            crc = crc_update(crc, *s++);
        }
        crc = ~crc;
        return crc;
    }

    bool handshake(const juce::String& fullDevice, serial::Serial& linnSerial)
    {
        MessageManager::getInstance()->runDispatchLoopUntil(1000);

        std::cout << "Opening serial device " << linnSerial.getPort() << " for settings retrieval with baud rate 115200" << std::endl;
        for (int i = 1; i <= 3; ++i) {
            try {
                linnSerial.open();
                break;
            }
            catch (serial::SerialException e) {
                std::cerr << "Try " << i << " : wasn't able to open serial device " << fullDevice << ": " << e.what() << std::endl;
                MessageManager::getInstance()->runDispatchLoopUntil(500);
            }
        }
        
        if (!linnSerial.isOpen()) {
            std::cerr << "Wasn't able to open serial device " << fullDevice << std::endl;
            return false;
        }
        
        MessageManager::getInstance()->runDispatchLoopUntil(1000);
        
        for (int i = 1; i <= 2; ++i) {
            if (linnSerial.write("5, 4, 3, 2, 1 ...\n") != 18) {
                std::cerr << "Couldn't write the complete handshake message to serial device " << fullDevice << std::endl;
                return false;
            }
            
            std::string linnGoCode = linnSerial.readline();
            if (linnGoCode == "LinnStruments are go!\n") {
                return true;
            }
            else {
                std::cerr << "Didn't receive the correct go code from serial device " << fullDevice << " (" << linnGoCode << ")" << std::endl;
                MessageManager::getInstance()->runDispatchLoopUntil(500);
            }
        }
        
        return false;
    }
    
	int negotiateIncomingCRC(serial::Serial& linnSerial, juce::String& fullDevice, uint8_t* buffer, uint8_t size) {
        if (linnSerial.write("c") != 1) {
            std::cerr << "Couldn't to write the check command to serial device " << fullDevice << std::endl;
            return -1;
        }
        
        uint32_t remote_crc = 0;
        size_t actual_crc = linnSerial.read((uint8_t*)&remote_crc, sizeof(uint32_t));
        if (actual_crc != sizeof(uint32_t)) {
            std::cerr << "Couldn't retrieve data from device " << fullDevice << " (wrong CRC length)" << std::endl;
            return -1;
        }
        
        uint32_t local_crc = crc_byte_array(buffer, size);
        if (local_crc != remote_crc) {
            std::cerr << "Couldn't retrieve data from device " << fullDevice << " (wrong CRC " << local_crc << " != " << remote_crc << "), retrying" << std::endl;
            if (linnSerial.write("w") != 1) {
                std::cerr << "Couldn't to write the CRC wrong command to serial device " << fullDevice << std::endl;
                return -1;
            }
            return 0;
        }

        if (linnSerial.write("o") != 1) {
            std::cerr << "Couldn't to write the CRC ok command to serial device " << fullDevice << std::endl;
            return -1;
        }
        
        return 1;
    }
    
	int negotiateOutgoingCRC(serial::Serial& linnSerial, juce::String& fullDevice, uint8_t* buffer, uint8_t size) {
        uint8_t serialCheck;
        if (linnSerial.read(&serialCheck, 1) != 1 || serialCheck != 'c') {
            std::cerr << "Didn't receive the CRC check code from device " << fullDevice << std::endl;
            return -1;
        }
        
        uint32_t crc = crc_byte_array(buffer, size);
        if (linnSerial.write((uint8_t*)&crc, sizeof(uint32_t)) != sizeof(uint32_t)) {
            std::cerr << "Couldn't write the CRC to device " << fullDevice << std::endl;
            return -1;
        }
        
        uint8_t serialCRCState;
        if (linnSerial.read(&serialCRCState, 1) != 1) {
            std::cerr << "Didn't receive the CRC state from device " << fullDevice << std::endl;
            return -1;
        }
        if (serialCRCState == 'w') {
            std::cerr << "CRC code " << crc << " didn't correspond for device " << fullDevice << std::endl;
            return 0;
        }
        
        return 1;
    }
}

bool LinnStrumentSerial::readSettings()
{
    if (!isDetected()) return false;
    
    settings.reset();
    
    try {
        juce::String fullDevice = getFullLinnStrumentDevice();
        const char* devicePort = fullDevice.toRawUTF8();
        const std::string devicePortString(devicePort);
        serial::Timeout timeout = serial::Timeout::simpleTimeout(3000);
        serial::Serial linnSerial(devicePortString, 115200, timeout);
        
        if (!handshake(fullDevice, linnSerial)) {
            return false;
        }
        
        // read settings
        {
            std::cout << "Reading settings" << std::endl;
            UpdaterApplication::getApp().setProgressText("Transferring instrument settings...");
            
            if (linnSerial.write("s") != 1) {
                std::cerr << "Couldn't to give the read settings command to serial device " << fullDevice << std::endl;
                return false;
            }
            
            std::string ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive the ACK code from serial device " << fullDevice << std::endl;
                return false;
            }
            
            uint8_t sizeBuffer[4];
            if (linnSerial.read(sizeBuffer, 4) != 4) {
                std::cerr << "Couldn't retrieve the size of the settings from device " << fullDevice << std::endl;
                return false;
            }
            
            int32_t settingsSize;
            std::memcpy(&settingsSize, sizeBuffer, sizeof(int32_t));
            
            const int32_t batchsize = 96;
            int32_t remaining = settingsSize;
            settings.ensureSize(remaining);
            uint8_t* dest = (uint8_t*)settings.getData();
            while (remaining > 0) {
                MessageManager::getInstance()->runDispatchLoopUntil(20);
				uint8_t requested = (uint8_t)std::min(remaining, batchsize);
                size_t actual = linnSerial.read(dest, requested);
                if (actual != requested) {
                    std::cerr << "Couldn't retrieve the settings from device " << fullDevice << " (wrong batch length)" << std::endl;
                    return false;
                }
                
                // version 2.0.0-beta1 and 2.0.0-beta2
                if (settings[0] == 9) {
                    if (linnSerial.write("a") != 1) {
                        std::cerr << "Couldn't to write the ack command to serial device " << fullDevice << std::endl;
                        return false;
                    }
                }
                // version 2.0.0-beta3 and later
                else if (settings[0] >= 10) {
					int crc = negotiateIncomingCRC(linnSerial, fullDevice, dest, requested);
                    if (crc == -1)      return false;
                    else if (crc == 0)  continue;
                }

                remaining -= actual;
                dest += actual;
            }
            
            ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive read settings finish ACK code from serial device " << fullDevice << std::endl;
                return false;
            }

            std::cout << "Read settings with size " << settingsSize << std::endl;
        }
        
        // check if this is version 2.0 or later, also read the stored projects
        if (settings[0] >= 9) {
            projects.reset();
            
            std::cout << "Reading projects" << std::endl;

            if (linnSerial.write("p") != 1) {
                std::cerr << "Couldn't to give the read projects command to serial device " << fullDevice << std::endl;
                return false;
            }
            
            std::string ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive the ACK code from serial device " << fullDevice << std::endl;
                return false;
            }
            
            if (linnSerial.read(&projectCount, 1) != 1) {
                std::cerr << "Couldn't retrieve the project count from device " << fullDevice << std::endl;
                return false;
            }
            
            if (linnSerial.read((uint8_t*)&projectSize, sizeof(int32_t)) != sizeof(int32_t)) {
                std::cerr << "Couldn't retrieve the size of the project from device " << fullDevice << std::endl;
                return false;
            }
            
            projects.ensureSize(projectSize*projectCount);
            uint8_t* dest = (uint8_t*)projects.getData();
            
            const int32_t batchsize = 96;
            for (int p = 0; p < projectCount; ++p) {
                std::cout << "Reading project " << (p+1) << std::endl;
                UpdaterApplication::getApp().setProgressText(String::formatted("Transferring project %d of %d...", (p+1), projectCount));
                int32_t remaining = projectSize;
                while (remaining > 0) {
                    MessageManager::getInstance()->runDispatchLoopUntil(20);
					uint8_t requested = (uint8_t)std::min(remaining, batchsize);
                    size_t actual = linnSerial.read(dest, requested);
                    if (actual != requested) {
                        std::cerr << "Couldn't retrieve the settings from device " << fullDevice << " (wrong batch length)" << std::endl;
                        return false;
                    }
                    
                    // version 2.0.0-beta1 and 2.0.0-beta2
                    if (settings[0] == 9) {
                        if (linnSerial.write("a") != 1) {
                            std::cerr << "Couldn't to write the ack command to serial device " << fullDevice << std::endl;
                            return false;
                        }
                    }
                    // version 2.0.0-beta3 and later
                    else if (settings[0] >= 10) {
						int crc = negotiateIncomingCRC(linnSerial, fullDevice, dest, requested);
                        if (crc == -1)      return false;
                        else if (crc == 0)  continue;
                    }
                    
                    remaining -= actual;
                    dest += actual;
                }
            }
            
            ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive read projects finish ACK code from serial device " << fullDevice << std::endl;
                return false;
            }

            std::cout << "Read " << (int)projectCount << " projects from " << fullDevice << " with each a size of " << projectSize << std::endl;
        }
    }
    catch (serial::SerialException e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    catch (serial::IOException e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    catch (serial::PortNotOpenedException e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool LinnStrumentSerial::restoreSettings()
{
    if (!isDetected() || !hasSettings()) return false;
    
    try {
        juce::String fullDevice = getFullLinnStrumentDevice();
        const char* devicePort = fullDevice.toRawUTF8();
        const std::string devicePortString(devicePort);
        serial::Timeout timeout = serial::Timeout::simpleTimeout(1500);
        serial::Serial linnSerial(devicePortString, 115200, timeout);
        
        if (!handshake(fullDevice, linnSerial)) {
            return false;
        }
        
        if (projectCount > 0) {
            std::cout << "Restoring projects" << std::endl;
            
            for (uint8_t projectIndex = 0; projectIndex < projectCount; ++projectIndex) {
            
                std::cout << "Restoring project " << (projectIndex+1) << std::endl;
                UpdaterApplication::getApp().setProgressText(String::formatted("Transferring project %d of %d...", (projectIndex+1), projectCount));
                
                if (linnSerial.write("q") != 1) {
                    std::cerr << "Couldn't give the restore project command to serial device " << fullDevice << std::endl;
                    return false;
                }
                
                MessageManager::getInstance()->runDispatchLoopUntil(20);
                std::string ackCode = linnSerial.readline();
                if (ackCode != "ACK\n") {
                    std::cerr << "Didn't receive the restore project ACK code from serial device " << fullDevice << std::endl;
                    return false;
                }
                
                if (linnSerial.write((uint8_t*)&projectSize, 4) != 4) {
                    std::cerr << "Couldn't write the size of the project to device " << fullDevice << std::endl;
                    return false;
                }
                
                ackCode = linnSerial.readline();
                if (ackCode != "ACK\n") {
                    std::cerr << "Didn't receive restore project size ACK code from serial device " << fullDevice << std::endl;
                    return false;
                }
                
                if (linnSerial.write(&projectIndex, 1) != 1) {
                    std::cerr << "Couldn't write the index of the project to device " << fullDevice << std::endl;
                    return false;
                }
                
                ackCode = linnSerial.readline();
                if (ackCode != "ACK\n") {
                    std::cerr << "Didn't receive restore project index ACK code from serial device " << fullDevice << std::endl;
                    return false;
                }
                
                const uint32_t batchsize = 96;
                uint8_t* source = ((uint8_t*)projects.getData()) + (projectIndex * projectSize);
                uint32_t remaining = projectSize;
                while (remaining > 0) {
                    MessageManager::getInstance()->runDispatchLoopUntil(20);
                    
					uint8_t actual = (uint8_t)std::min(remaining, batchsize);
                    if (linnSerial.write(source, actual) != actual) {
                        std::cerr << "Couldn't write the project to device " << fullDevice << std::endl;
                        return false;
                    }
                    
                    int crc = negotiateOutgoingCRC(linnSerial, fullDevice, source, actual);
                    if (crc == -1)      return false;
                    else if (crc == 0)  continue;

                    remaining -= actual;
                    source += actual;
                }
                
                ackCode = linnSerial.readline();
                if (ackCode != "ACK\n") {
                    std::cerr << "Didn't receive restore project finish ACK code from serial device " << fullDevice << std::endl;
                    return false;
                }
            }
        }
        
        // restore settings
        {
            std::cout << "Restoring settings" << std::endl;
            UpdaterApplication::getApp().setProgressText("Transferring instrument settings...");
            
            if (linnSerial.write("r") != 1) {
                std::cerr << "Couldn't give the restore settings command to serial device " << fullDevice << std::endl;
                return false;
            }
            
            MessageManager::getInstance()->runDispatchLoopUntil(20);
            std::string ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive the restore settings ACK code from serial device " << fullDevice << std::endl;
                return false;
            }
            
            int32_t settingsSize = settings.getSize();
            if (linnSerial.write((uint8_t*)&settingsSize, sizeof(int32_t)) != sizeof(int32_t)) {
                std::cerr << "Couldn't write the size of the settings to device " << fullDevice << std::endl;
                return false;
            }
            
            ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive restore settings progress ACK code from serial device " << fullDevice << std::endl;
                return false;
            }

            const uint32_t batchsize = 96;
            uint8_t* source = (uint8_t*)settings.getData();
            uint32_t remaining = settingsSize;
            while (remaining > 0) {
                MessageManager::getInstance()->runDispatchLoopUntil(20);
                
				uint8_t actual = (uint8_t)std::min(remaining, batchsize);
                if (linnSerial.write(source, actual) != actual) {
                    std::cerr << "Couldn't write the settings to device " << fullDevice << std::endl;
                    return false;
                }
                
                int crc = negotiateOutgoingCRC(linnSerial, fullDevice, source, actual);
                if (crc == -1)      return false;
                else if (crc == 0)  continue;
                
                remaining -= actual;
                source += actual;
            }
            
            ackCode = linnSerial.readline();
            if (ackCode != "ACK\n") {
                std::cerr << "Didn't receive restore settings finish ACK code from serial device " << fullDevice << std::endl;
                return false;
            }
        }
    }
    catch (serial::SerialException e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    
    return true;
}

bool LinnStrumentSerial::hasSettings() {
    return settings.getSize() > 0;
}
