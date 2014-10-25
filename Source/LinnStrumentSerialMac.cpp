/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "LinnStrumentSerialMac.h"

#include "UpdaterApplication.h"

LinnStrumentSerialMac::LinnStrumentSerialMac() : upgradeVerificationPhase(false), upgradeSuccessful(false)
{
}

LinnStrumentSerialMac::~LinnStrumentSerialMac()
{
}

bool LinnStrumentSerialMac::findFirmwareFile()
{
    File current_app = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile);
    File parent_dir = current_app.getParentDirectory();
    Array<File> firmware_files;
    parent_dir.findChildFiles(firmware_files, File::TypesOfFileToFind::findFiles, false, "*.bin");
    if (firmware_files.size() > 0)
    {
        firmwareFile = firmware_files[0].getFullPathName();
    }
    
    return hasFirmwareFile();
}

bool LinnStrumentSerialMac::hasFirmwareFile()
{
    return firmwareFile.isNotEmpty();
}

bool LinnStrumentSerialMac::detect()
{
    if (!hasFirmwareFile()) return false;

    linnstrumentDevice = String::empty;
    
    detectionChild.start("/usr/sbin/system_profiler SPUSBDataType");
    if (!detectionChild.waitForProcessToFinish(1000))
    {
        detectionChild.kill();
        std::cerr << "Impossible to inspect USB devices" << std::endl;
    }
    else
    {
        String output = detectionChild.readAllProcessOutput();
        int indexBegin = output.indexOf("LinnStrument SERIAL");
        if (-1 == indexBegin)
        {
            std::cerr << "No LinnStrument serial device found" << std::endl;
        }
        else
        {
            String location = "Location ID: 0x";
            indexBegin = output.indexOf(indexBegin, location);
            if (-1 == indexBegin)
            {
                std::cerr << "Couldn't find the LinnStrument USB location" << std::endl;
            }
            else
            {
                int indexEnd = output.indexOf(indexBegin+location.length(), " ");
                if (-1 == indexEnd)
                {
                    std::cerr << "Couldn't find the LinnStrument USB location ID end" << std::endl;
                }
                else
                {
                    String locationId = output.substring(indexBegin+location.length(), indexEnd);
                    locationId = locationId.trimCharactersAtEnd("0");
                    String deviceName = "tty.usbmodem"+locationId+"1";
                    String devicePath = "/dev/"+deviceName;
                    File device(devicePath);
                    if (!device.exists())
                    {
                        std::cerr << "Expected LinnStrument device " << devicePath << " doesn't exist" << std::endl;
                    }
                    else
                    {
                        linnstrumentDevice = deviceName;
                    }
                }
            }
        }
    }
    
    return isDetected();
}

bool LinnStrumentSerialMac::isDetected()
{
    return linnstrumentDevice.isNotEmpty();
}

bool LinnStrumentSerialMac::prepareDevice()
{
    if (!hasFirmwareFile() || !isDetected()) return false;

    preparationChild.start("/bin/stty -f /dev/"+linnstrumentDevice+" 1200 cs8 -cstopb -parenb");
    if (!preparationChild.waitForProcessToFinish(1000) || preparationChild.getExitCode() > 0)
    {
        preparationChild.kill();
        std::cerr << "Impossible to prepare serial device /dev/" << linnstrumentDevice << std::endl;
        return false;
    }
    return true;
}

bool LinnStrumentSerialMac::performUpgrade()
{
    if (!hasFirmwareFile() || !isDetected()) return false;
    
    upgradeOutput.clear();
    upgradeVerificationPhase = false;
    upgradeSuccessful = false;
    
    File current_app = File::getSpecialLocation(File::SpecialLocationType::currentApplicationFile);
    File bossac_tool = current_app.getChildFile("Contents/Resources/bossac");
    if (bossac_tool.exists())
    {
        StringArray args;
        args.add(bossac_tool.getFullPathName());
        args.add("-i");
        args.add("--port="+linnstrumentDevice);
        args.add("-U");
        args.add("false");
        args.add("-e");
        args.add("-w");
        args.add("-v");
        args.add("-R");
        args.add("-b");
        args.add(firmwareFile);
        upgradeChild.start(args);
        startTimer(1);
        return true;
    }
    else
    {
        return false;
    }
}

void LinnStrumentSerialMac::timerCallback()
{
    if (!upgradeChild.isRunning())
    {
        stopTimer();
        
        if (upgradeSuccessful)
        {
            UpdaterApplication::getApp().setUpgradeDone();
        }
        else
        {
            UpdaterApplication::getApp().setUpgradeFailed();
        }
    }
    
    char buffer;
    if (upgradeChild.readProcessOutput(&buffer, 1) > 0)
    {
        upgradeOutput.append(String::charToString(buffer), 1);
        if (buffer == '%')
        {
            if (upgradeOutput.contains("Verify"))
            {
                upgradeVerificationPhase = true;
            }
            
            int index = upgradeOutput.lastIndexOf("] ");
            String progress = upgradeOutput.substring(index+2);
            if (upgradeVerificationPhase && progress == "100%")
            {
                upgradeSuccessful = true;
            }
            UpdaterApplication::getApp().setProgressText((upgradeVerificationPhase ? "Verifying... " : "Writing... ")+progress);
            upgradeOutput.clear();
        }
    }
}

