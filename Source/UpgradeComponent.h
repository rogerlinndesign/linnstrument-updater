/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.4

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_98D955BBF337B26__
#define __JUCE_HEADER_98D955BBF337B26__

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
                          public ButtonListener
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
    void showGoBack(bool flag);
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
    ScopedPointer<Label> progressLabel_;
    ScopedPointer<TextButton> updateButton_;
    ScopedPointer<TextButton> retryButton_;
    ScopedPointer<TextButton> goAheadDefaultSettingsButton_;
    ScopedPointer<Label> linnstrumentLabel_;
    ScopedPointer<TextButton> goBackButton_;
    ScopedPointer<TextButton> goAheadButton_;
    ScopedPointer<TextButton> selectFirmwareButton_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UpgradeComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_98D955BBF337B26__
