/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "LinnStrumentSerialWindows.h"

#include "UpdaterApplication.h"

LinnStrumentSerialWindows::LinnStrumentSerialWindows() : upgradeVerificationPhase(false), upgradeSuccessful(false)
{
}

LinnStrumentSerialWindows::~LinnStrumentSerialWindows()
{
}

bool LinnStrumentSerialWindows::findFirmwareFile()
{
    File current_app = File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile);
    File parent_dir = current_app.getParentDirectory();
    Array<File> firmware_files;
    parent_dir.findChildFiles(firmware_files, File::TypesOfFileToFind::findFiles, false, "*.bin");
    if (firmware_files.size() > 0)
    {
        firmwareFile = firmware_files[0].getFullPathName();
    }
    
    return hasFirmwareFile();
}

bool LinnStrumentSerialWindows::hasFirmwareFile()
{
    return firmwareFile.isNotEmpty();
}

bool LinnStrumentSerialWindows::detect()
{
    if (!hasFirmwareFile()) return false;

    linnstrumentDevice = String::empty;
    
    return isDetected();
}

bool LinnStrumentSerialWindows::isDetected()
{
    return linnstrumentDevice.isNotEmpty();
}

bool LinnStrumentSerialWindows::prepareDevice()
{
    if (!hasFirmwareFile() || !isDetected()) return false;

	return true;
}

bool LinnStrumentSerialWindows::performUpgrade()
{
    if (!hasFirmwareFile() || !isDetected()) return false;
    
    upgradeOutput.clear();
    upgradeVerificationPhase = false;
    upgradeSuccessful = false;

	return false;
}

void LinnStrumentSerialWindows::timerCallback()
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

