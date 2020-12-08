#if defined(__APPLE__)

/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include <AvailabilityMacros.h>
#include <CoreFoundation/CFNumber.h>
#include <sys/param.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <sys/ioctl.h>
#include <IOKit/serial/ioss.h>
#include <errno.h>

#include "LinnStrumentSerialMac.h"

#include "UpdaterApplication.h"

LinnStrumentSerialMac::LinnStrumentSerialMac() : upgradeVerificationPhase(false), upgradeSuccessful(false)
{
}

LinnStrumentSerialMac::~LinnStrumentSerialMac()
{
}

String LinnStrumentSerialMac::getFullLinnStrumentDevice()
{
    if (!isDetected()) return String();
    
    return "/dev/"+linnstrumentDevice;
}

bool LinnStrumentSerialMac::detect()
{
    resetDetection();
    
    io_iterator_t serialPortIterator;
    char bsdPath[MAXPATHLEN];
    
    CFMutableDictionaryRef classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch == NULL)
    {
        return false;
    }
    CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));
    
    kern_return_t success = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, &serialPortIterator);
    if (KERN_SUCCESS != success)
    {
        return false;
    }
    
    io_service_t modemService;
    
    while ((modemService = IOIteratorNext(serialPortIterator)) &&
           !isDetected())
    {
        CFTypeRef bsdPathAsCFString = IORegistryEntrySearchCFProperty(modemService, kIOServicePlane, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, kIORegistryIterateRecursively);
        
        if (bsdPathAsCFString)
        {
            // Initialize the returned path
            *bsdPath = '\0';
            
            // Convert the path from a CFString to a C (NUL-terminated)
            Boolean result = CFStringGetCString((CFStringRef) bsdPathAsCFString, bsdPath, sizeof(bsdPath), kCFStringEncodingUTF8);
            CFRelease(bsdPathAsCFString);
            
            if (result)
            {
                io_object_t parent = 0;
                io_object_t parents = modemService;
                while (KERN_SUCCESS == IORegistryEntryGetParentEntry(parents, kIOServicePlane, &parent) &&
                       !isDetected())
                {
                    CFMutableDictionaryRef dict = NULL;
                    result = IORegistryEntryCreateCFProperties(parent, &dict, kCFAllocatorDefault, 0);
                    if (!result)
                    {
                        if (CFDictionaryContainsKey(dict, CFSTR("USB Vendor Name")) &&
                            CFDictionaryContainsKey(dict, CFSTR("USB Product Name")) &&
                            CFDictionaryContainsKey(dict, CFSTR("idVendor")) &&
                            CFDictionaryContainsKey(dict, CFSTR("idProduct")))
                        {
                            CFStringRef vendorName = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("USB Vendor Name"));
                            CFStringRef productName = (CFStringRef)CFDictionaryGetValue(dict, CFSTR("USB Product Name"));
                            CFNumberRef idVendor = (CFNumberRef)CFDictionaryGetValue(dict, CFSTR("idVendor"));
                            CFNumberRef idProduct = (CFNumberRef)CFDictionaryGetValue(dict, CFSTR("idProduct"));
                            int64_t id_vendor = 0;
                            int64_t id_product = 0;
                            CFNumberGetValue(idVendor, kCFNumberSInt64Type, &id_vendor);
                            CFNumberGetValue(idProduct, kCFNumberSInt64Type, &id_product);
                            
                            if (linnstrumentDevice.isEmpty() &&
                                kCFCompareEqualTo == CFStringCompare(vendorName, CFSTR("Roger Linn Design"), 0) &&
                                kCFCompareEqualTo == CFStringCompare(productName, CFSTR("LinnStrument SERIAL"), 0) &&
                                id_vendor == 0xf055 &&
                                id_product == 0x0070)
                            {
                                String device = String(bsdPath);
                                if (device.startsWith("/dev/"))
                                {
                                    linnstrumentDevice = device.substring(5);
                                    std::cout << "Using LinnStrument device " << linnstrumentDevice << std::endl;
                                }
                            }
                        }
                        
                        CFRelease(dict);
                    }

                    if (parents != modemService)
                    {
                        IOObjectRelease(parents);
                    }
                    
                    parents = parent;
                }
            }
        }
        
        // Release the io_service_t now that we are done with it.
        (void) IOObjectRelease(modemService);
    }
    
    IOObjectRelease(serialPortIterator);  // Release the iterator.
    
    return isDetected();
}

bool LinnStrumentSerialMac::isDetected()
{
    return linnstrumentDevice.isNotEmpty();
}

bool LinnStrumentSerialMac::prepareDevice()
{
    if (!hasFirmwareFile() || !isDetected()) return false;

    int flags = (O_RDWR | O_NOCTTY | O_NONBLOCK | O_CLOEXEC | O_SYNC);
    int fd = open(getFullLinnStrumentDevice().toRawUTF8(), flags);
    
    if (fd == -1) {
        std::cerr << "Can't find serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        return false;
    }

    struct termios options;
    if (-1 == tcgetattr(fd, &options)) {
        std::cerr << "Impossible to get atttributes of serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        close(fd);
        return false;
    }
    
    if (-1 == cfsetispeed(&options, B1200)) {
        std::cerr << "Impossible to change the input baud rate of serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        close(fd);
        return false;
    }
    if (-1 == cfsetospeed(&options, B1200)) {
        std::cerr << "Impossible to change the output baud rate of serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        close(fd);
        return false;
    }
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~PARENB;
    
    if (-1 == tcflush(fd, TCIFLUSH)) {
        std::cerr << "Unexpected error while flushing serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        close(fd);
        return false;
    }
    if (-1 == tcsetattr(fd, TCSANOW, &options)) {
        std::cerr << "Unexpected error while set attributes of serial device /dev/" << linnstrumentDevice << " (" << errno << ")" << std::endl;
        close(fd);
        return false;
    }
    close(fd);

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
            UpdaterApplication::getApp().restoreSettings();
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

#endif
