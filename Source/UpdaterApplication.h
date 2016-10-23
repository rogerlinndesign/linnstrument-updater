/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#ifndef UPDATERAPPLICATION_H_INCLUDED
#define UPDATERAPPLICATION_H_INCLUDED

#include <JuceHeader.h>

#include "MainWindow.h"
#include "LinnStrumentSerial.h"

class MainComponent;
class UpgradeComponent;

class UpdaterApplication : public JUCEApplication,
                           public MessageListener,
                           public Timer
{
public:
    UpdaterApplication();
    
    inline static UpdaterApplication &getApp()
    {
        UpdaterApplication* const app = dynamic_cast<UpdaterApplication*>(JUCEApplication::getInstance());
        jassert (app != nullptr);
        return *app;
    }
    
    const String getApplicationName() override;
    const String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;

    void initialise(const String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void anotherInstanceStarted (const String& commandLine) override;
    
    MainComponent* getMainComponent();
    UpgradeComponent* getUpgradeComponent();
    
    void handleMessage(const juce::Message &message) override;
    void timerCallback() override;
    
    LinnStrumentSerial &getLinnStrumentSerial();
    
    void home();
    void findFirmware();
    void continueWithFirmwareFile(const File& file);
    void connectionWarning();
    void detectLinnStrument();
    void prepareDevice();
    void upgradeLinnStrument();
    void restoreSettings();
    void retry();
    
    void setLabelText(const String& text);
    void setProgressText(const String& text);
    void showPrepareDevice(bool flag);
    void setUpgradeFailed();
    void showRetry(bool flag);
    
    bool saveProject(uint8_t number, const File& file);
    

private:

    ScopedPointer<MainWindow> mainWindow;
    ScopedPointer<LinnStrumentSerial> linnStrumentSerial;
};

#endif  // UPDATERAPPLICATION_H_INCLUDED
