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

#ifndef __JUCE_HEADER_21BE87E8D8D11C1E__
#define __JUCE_HEADER_21BE87E8D8D11C1E__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ProjectsComponent  : public Component,
                           public ComboBoxListener,
                           public ButtonListener
{
public:
    //==============================================================================
    ProjectsComponent ();
    ~ProjectsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setBusy(bool state);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void checkTransferConditions();
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> projectNumberCombo_;
    ScopedPointer<TextButton> receiveButton_;
    ScopedPointer<TextButton> sendButton_;
    ScopedPointer<Label> introLabel_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_21BE87E8D8D11C1E__
