/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "UpdaterApplication.h"

#include "MainComponent.h"
#if JUCE_MAC
    #include "LinnStrumentSerialMac.h"
#elif JUCE_WINDOWS
	#include "LinnStrumentSerialWindows.h"
#endif

namespace
{
    enum ApplicationMessageType
    {
        findFirmware,
        detectLinnStrument,
        readSettings,
        prepareDevice,
        updateDevice,
        restoreSettings
    };
    
    struct ApplicationMessage: juce::Message
    {
        ApplicationMessage(const ApplicationMessageType type, void* const payload): type_ (type), payload_ (payload) {}
        ~ApplicationMessage() {}
        
        const ApplicationMessageType type_;
        void* const payload_;
    };
}


UpdaterApplication::UpdaterApplication() :
#if JUCE_MAC
    linnStrumentSerial(new LinnStrumentSerialMac())
#elif JUCE_WINDOWS
	linnStrumentSerial(new LinnStrumentSerialWindows())
#endif
{
};
    
const String UpdaterApplication::getApplicationName()
{
    return ProjectInfo::projectName;
}

const String UpdaterApplication::getApplicationVersion()
{
    return ProjectInfo::versionString;
}

bool UpdaterApplication::moreThanOneInstanceAllowed()
{
    return false;
}
    
void UpdaterApplication::initialise(const String&)
{
    mainWindow = new MainWindow();
    findFirmware();
}

void UpdaterApplication::shutdown()
{
    mainWindow = nullptr;
}

void UpdaterApplication::systemRequestedQuit()
{
    quit();
}

void UpdaterApplication::anotherInstanceStarted(const String&)
{
}

void UpdaterApplication::handleMessage(const juce::Message &message)
{
    ApplicationMessage *msg = (ApplicationMessage *)&message;
    switch(msg->type_)
    {
        case ApplicationMessageType::findFirmware:
        {
            if (linnStrumentSerial->findFirmwareFile())
            {
                detectLinnStrument();
            }
            else
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("No firmware file could be found in the same directory as the updater tool.", false);
                startTimer(300);
            }
            break;
        }
            
        case ApplicationMessageType::detectLinnStrument:
        {
            if (linnStrumentSerial->isDetected() || linnStrumentSerial->detect())
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("Found LinnStrument ready for OS Update.", true);
            }
            else
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("No LinnStrument found!\nPlease make sure Update OS mode is selected in Global Settings.", false);
                startTimer(300);
            }
            break;
        }
            
        case ApplicationMessageType::readSettings:
        {
            ((MainComponent *)mainWindow->getContentComponent())->setLabelText("Retrieving the LinnStrument's settings.", false);
            
            if (linnStrumentSerial->readSettings())
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("LinnStrument's settings have been retrieved.", false);
                postMessage(new ApplicationMessage(ApplicationMessageType::prepareDevice, (void *)nullptr));
            }
            else
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("The current settings couldn't be retrieved.\nPlease reconnect LinnStrument, possibly to another USB port, quit and restart the updater.", false);
            }
            break;
        }
        
        case ApplicationMessageType::prepareDevice:
        {
            if (linnStrumentSerial->prepareDevice())
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("LinnStrument has been prepared for OS Update.", false);
                postMessage(new ApplicationMessage(ApplicationMessageType::updateDevice, (void *)nullptr));
            }
            else
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("An unexpected error occurred.\nPlease reconnect LinnStrument, quit and restart the updater.", false);
            }
            break;
        }
            
        case ApplicationMessageType::updateDevice:
        {
            if (linnStrumentSerial->performUpgrade())
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("Performing LinnStrument firmware update.\nDO NOT disconnect LinnStrument.\nDO NOT quit the updater.", false);
            }
            break;
        }
            
        case ApplicationMessageType::restoreSettings:
        {
            ((MainComponent *)mainWindow->getContentComponent())->setLabelText("Restoring the LinnStrument's settings.", false);
            ((MainComponent *)mainWindow->getContentComponent())->setProgressText("");

            if (linnStrumentSerial->restoreSettings())
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("LinnStrument's settings have been restored.", false);
                setUpgradeDone();
            }
            else
            {
                ((MainComponent *)mainWindow->getContentComponent())->setLabelText("The settings couldn't be restored.\nThe LinnStrument firmware has been upgraded, but a manual calibration will have to be performed.", false);
            }
            break;
        }
    }
}

void UpdaterApplication::timerCallback()
{
    if (linnStrumentSerial->hasFirmwareFile())
    {
        detectLinnStrument();
    }
    else
    {
        findFirmware();
    }
    stopTimer();
}

LinnStrumentSerial &UpdaterApplication::getLinnStrumentSerial()
{
    return *linnStrumentSerial;
}

void UpdaterApplication::findFirmware()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::findFirmware, (void *)nullptr));
}

void UpdaterApplication::detectLinnStrument()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::detectLinnStrument, (void *)nullptr));
}

void UpdaterApplication::upgradeLinnStrument()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::readSettings, (void *)nullptr));
}

void UpdaterApplication::restoreSettings()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::restoreSettings, (void *)nullptr));
}

void UpdaterApplication::setUpgradeDone()
{
    ((MainComponent *)mainWindow->getContentComponent())->setLabelText("All done!", false);
    ((MainComponent *)mainWindow->getContentComponent())->setProgressText("");
}

void UpdaterApplication::setUpgradeFailed()
{
    ((MainComponent *)mainWindow->getContentComponent())->setLabelText("The firmware upgrade failed.\nPlease reconnect LinnStrument, quit and restart the updater.", false);
    ((MainComponent *)mainWindow->getContentComponent())->setProgressText("");
}

void UpdaterApplication::setProgressText(const String& text)
{
    ((MainComponent *)mainWindow->getContentComponent())->setProgressText(text);
}
