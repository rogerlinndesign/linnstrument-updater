/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)

 Written by Geert Bevin (http://gbevin.com).

 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UpgradeComponent  : public Component,
                          public Button::Listener
{
public:
    //==============================================================================
    UpgradeComponent ();
    ~UpgradeComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setLabelText(const String& text, bool enableButton);
    void setProgressText(const String& text);
    void showGoAhead(bool flag);
    void showSelectFirmware(bool flag);
    void showPrepareDevice(bool flag);
    void showRetry(bool flag);
    void showQuit(bool flag);
    void resetUIState();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;

    // Binary resources:
    static const char* rogerlinndesign_png;
    static const int rogerlinndesign_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Label> progressLabel_;
    std::unique_ptr<TextButton> goAheadButton_;
    std::unique_ptr<TextButton> selectFirmwareButton_;
    std::unique_ptr<TextButton> updateButton_;
    std::unique_ptr<TextButton> retryButton_;
    std::unique_ptr<TextButton> goAheadDefaultSettingsButton_;
    std::unique_ptr<Label> linnstrumentLabel_;
    std::unique_ptr<TextButton> quitButton_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UpgradeComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

