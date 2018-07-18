/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "UpdaterApplication.h"

#include "CommandIDs.h"
#include "MainComponent.h"
#include "UpgradeComponent.h"
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
#ifdef LINNSTRUMENT_LOADER
    return "LinnStrument Loader";
#else
    return ProjectInfo::projectName;
#endif
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
    commandManager = new ApplicationCommandManager();
    commandManager->addListener(this);
    commandManager->registerAllCommandsForTarget(this);

    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel_);
    mainWindow = new MainWindow();
    detectLinnStrument();
}

void UpdaterApplication::shutdown()
{
    commandManager->removeListener(this);
    
    mainWindow = nullptr;
    
    deleteAndZero(commandManager);
}

void UpdaterApplication::systemRequestedQuit()
{
    quit();
}

void UpdaterApplication::anotherInstanceStarted(const String&)
{
}

MainComponent* UpdaterApplication::getMainComponent()
{
    return (MainComponent*)mainWindow->getContentComponent();
}

UpgradeComponent* UpdaterApplication::getUpgradeComponent()
{
    return getMainComponent()->getUpgradeComponent();
}


ApplicationCommandTarget* UpdaterApplication::getNextCommandTarget()
{
    return nullptr;
}

void UpdaterApplication::getAllCommands(Array <CommandID> &commands)
{
    const CommandID ids[] = {
        CommandIDs::version,
        CommandIDs::quit
    };
    commands.addArray(ids, numElementsInArray(ids));
}

void UpdaterApplication::getCommandInfo(const CommandID commandID, ApplicationCommandInfo &result)
{
    const int cmd = ModifierKeys::commandModifier;
    
    switch (commandID)
    {
        case CommandIDs::version:
            result.setInfo(getApplicationName() + " v" + String(ProjectInfo::versionString),
                           "The version of the " + getApplicationName() + ".",
                           CommandCategories::help, 0);
            result.setActive(true);
            break;
            
        case CommandIDs::quit:
            result.setInfo("Exit",
                           "Exit " + getApplicationName() + ".",
                           CommandCategories::help, 0);
            result.setActive(true);
            result.defaultKeypresses.add(KeyPress('q', cmd, 0));
            break;
            
        default:
            break;
    }
}

bool UpdaterApplication::perform(const InvocationInfo &info)
{
    switch (info.commandID)
    {
        case CommandIDs::version:
        {
            break;
        }
            
        case CommandIDs::quit:
        {
            this->systemRequestedQuit();
            break;
        }
        
        default:
            return false;
    }
    
    return true;
}

void UpdaterApplication::applicationCommandInvoked(const ApplicationCommandTarget::InvocationInfo &)
{
}

void UpdaterApplication::applicationCommandListChanged()
{
}

void UpdaterApplication::handleMessage(const juce::Message &message)
{
    ApplicationMessage *msg = (ApplicationMessage *)&message;
    switch(msg->type_)
    {
        case ApplicationMessageType::detectLinnStrument:
        {
            if (linnStrumentSerial->isDetected() || linnStrumentSerial->detect())
            {
                getMainComponent()->setIntroText("Found LinnStrument.\n\nPlease click on the action that you want to perform below.");
                getMainComponent()->setButtonsEnabled(true);
            }
            else
            {
                getMainComponent()->setIntroText("Please connect LinnStrument's USB cable. DO NOT use a USB hub!\n\nThen activate Update OS mode in Global Settings.");
                startTimer(300);
            }
            break;
        }
            
        case ApplicationMessageType::findFirmware:
        {
            if (linnStrumentSerial->findFirmwareFile())
            {
#ifdef LINNSTRUMENT_LOADER
                prepareDevice();
#else
                readSettings();
#endif
            }
            else
            {
                getUpgradeComponent()->setLabelText("Please select which firmware file to use.", false);
                getUpgradeComponent()->setProgressText("");
                getUpgradeComponent()->showSelectFirmware(true);
            }
            break;
        }
            
        case ApplicationMessageType::readSettings:
        {
            getUpgradeComponent()->setLabelText("Retrieving LinnStrument's settings.", false);
            
            if (linnStrumentSerial->readSettings())
            {
                getUpgradeComponent()->setLabelText("LinnStrument's settings have been retrieved.", false);
                UpdaterApplication::getApp().setProgressText("");
                MessageManager::getInstance()->runDispatchLoopUntil(2000);
                postMessage(new ApplicationMessage(ApplicationMessageType::prepareDevice, (void *)nullptr));
            }
            else
            {
                getUpgradeComponent()->setLabelText("Couldn't retrieve LinnStrument's settings,\ninterrupting firmware upgrade.", false);
                getUpgradeComponent()->setProgressText("");
                UpdaterApplication::getApp().showPrepareDevice(true);
            }
            break;
        }
        
        case ApplicationMessageType::prepareDevice:
        {
            UpdaterApplication::getApp().showPrepareDevice(false);
            if (linnStrumentSerial->prepareDevice())
            {
                getUpgradeComponent()->setLabelText("LinnStrument has been prepared for OS Update.", false);
				MessageManager::getInstance()->runDispatchLoopUntil(2000);
				postMessage(new ApplicationMessage(ApplicationMessageType::updateDevice, (void *)nullptr));
            }
            else
            {
                getUpgradeComponent()->setLabelText("An unexpected error occurred.\nPlease reconnect LinnStrument, quit and restart the updater.", false);
            }
            break;
        }
            
        case ApplicationMessageType::updateDevice:
        {
            if (linnStrumentSerial->performUpgrade())
            {
                getUpgradeComponent()->setLabelText("Performing LinnStrument firmware update.\nDO NOT disconnect LinnStrument.\nDO NOT quit the updater.", false);
            }
            break;
        }
            
        case ApplicationMessageType::restoreSettings:
        {
            UpdaterApplication::getApp().showRetry(false);
            
            getUpgradeComponent()->setLabelText("Restoring LinnStrument's settings.", false);
            getUpgradeComponent()->setProgressText("");

            if (linnStrumentSerial->restoreSettings())
            {
                getUpgradeComponent()->setLabelText("The firmware update is finished!", false);
                getUpgradeComponent()->setProgressText("");
                getUpgradeComponent()->showQuit(true);
            }
            else
            {
                if (linnStrumentSerial->hasSettings()) {
                    getUpgradeComponent()->setLabelText("The LinnStrument firmware has been upgraded.\nThe settings couldn't be restored.\nYou can retry or perform the calibration manually.", false);
                    UpdaterApplication::getApp().showRetry(true);
                }
                else {
                    getUpgradeComponent()->setLabelText("The LinnStrument firmware has been upgraded.\n\nPlease perform the calibration by carefully sliding over the light guides.", false);
                    getUpgradeComponent()->showQuit(true);
                }
                UpdaterApplication::getApp().setProgressText("");
            }
            break;
        }
    }
}

void UpdaterApplication::timerCallback()
{
    detectLinnStrument();
    stopTimer();
}

LinnStrumentSerial &UpdaterApplication::getLinnStrumentSerial()
{
    return *linnStrumentSerial;
}

void UpdaterApplication::home()
{
    getMainComponent()->setButtonsEnabled(false);
 
    linnStrumentSerial->resetDetection();
    detectLinnStrument();
}

void UpdaterApplication::findFirmware()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::findFirmware, (void *)nullptr));
}

void UpdaterApplication::continueWithFirmwareFile(const File& file)
{
    linnStrumentSerial->setFirmwareFile(file);
    findFirmware();
}

void UpdaterApplication::detectLinnStrument()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::detectLinnStrument, (void *)nullptr));
}

void UpdaterApplication::readSettings()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::readSettings, (void *)nullptr));
}

void UpdaterApplication::restoreSettings()
{
    postMessage(new ApplicationMessage(ApplicationMessageType::restoreSettings, (void *)nullptr));
}

void UpdaterApplication::prepareDevice() {
    postMessage(new ApplicationMessage(ApplicationMessageType::prepareDevice, (void *)nullptr));
}

void UpdaterApplication::retry() {
    postMessage(new ApplicationMessage(ApplicationMessageType::restoreSettings, (void *)nullptr));
}

void UpdaterApplication::setLabelText(const String& text)
{
    if (getUpgradeComponent()->isVisible())
    {
        getUpgradeComponent()->setLabelText(text, false);
    }
}

void UpdaterApplication::setProgressText(const String& text)
{
    if (getUpgradeComponent()->isVisible())
    {
        getUpgradeComponent()->setProgressText(text);
    }
}

void UpdaterApplication::showPrepareDevice(bool flag)
{
    getUpgradeComponent()->showPrepareDevice(flag);
}

void UpdaterApplication::setUpgradeFailed()
{
    getUpgradeComponent()->setLabelText("The firmware upgrade failed.\nPlease reconnect LinnStrument, quit and restart the updater.", false);
    getUpgradeComponent()->setProgressText("");
}

void UpdaterApplication::showRetry(bool flag)
{
    getUpgradeComponent()->showRetry(flag);
}

bool UpdaterApplication::saveProject(uint8_t number, const File& file)
{
    return linnStrumentSerial->saveProject(number, file);
}

bool UpdaterApplication::loadProject(uint8_t number, const File& file)
{
    return linnStrumentSerial->loadProject(number, file);
}
