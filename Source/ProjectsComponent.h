/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

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
                           public ComboBox::Listener,
                           public Button::Listener
{
public:
    //==============================================================================
    ProjectsComponent ();
    ~ProjectsComponent();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void showBusy();
    void showDone();
    void showError();
    void checkTransferConditions();
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ComboBox> projectNumberCombo_;
    std::unique_ptr<TextButton> receiveButton_;
    std::unique_ptr<TextButton> sendButton_;
    std::unique_ptr<Label> introLabel_;
    std::unique_ptr<Label> busyLabel_;
    std::unique_ptr<Label> errorLabel_;
    std::unique_ptr<TextButton> okButton_;
    std::unique_ptr<Label> doneLabel_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectsComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
