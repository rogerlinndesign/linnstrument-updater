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

//[Headers] You can add your own extra header files here...
#include "UpdaterApplication.h"
//[/Headers]

#include "ProjectsComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ProjectsComponent::ProjectsComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (projectNumberCombo_ = new ComboBox ("project number combo"));
    projectNumberCombo_->setEditableText (false);
    projectNumberCombo_->setJustificationType (Justification::centred);
    projectNumberCombo_->setTextWhenNothingSelected (String());
    projectNumberCombo_->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    projectNumberCombo_->addItem (TRANS("1"), 1);
    projectNumberCombo_->addItem (TRANS("2"), 2);
    projectNumberCombo_->addItem (TRANS("3"), 3);
    projectNumberCombo_->addItem (TRANS("4"), 4);
    projectNumberCombo_->addItem (TRANS("5"), 5);
    projectNumberCombo_->addItem (TRANS("6"), 6);
    projectNumberCombo_->addItem (TRANS("7"), 7);
    projectNumberCombo_->addItem (TRANS("8"), 8);
    projectNumberCombo_->addItem (TRANS("9"), 9);
    projectNumberCombo_->addItem (TRANS("10"), 10);
    projectNumberCombo_->addItem (TRANS("11"), 11);
    projectNumberCombo_->addItem (TRANS("12"), 12);
    projectNumberCombo_->addItem (TRANS("13"), 13);
    projectNumberCombo_->addItem (TRANS("14"), 14);
    projectNumberCombo_->addItem (TRANS("15"), 15);
    projectNumberCombo_->addItem (TRANS("16"), 16);
    projectNumberCombo_->addListener (this);

    addAndMakeVisible (receiveButton_ = new TextButton ("receive button"));
    receiveButton_->setButtonText (TRANS("Receive From LinnStrument"));
    receiveButton_->addListener (this);

    addAndMakeVisible (sendButton_ = new TextButton ("send button"));
    sendButton_->setButtonText (TRANS("Send To LinnStrument"));
    sendButton_->addListener (this);

    addAndMakeVisible (introLabel_ = new Label ("intro label",
                                                TRANS("Please select which sequencer project number\n"
                                                "to transfer between LinnStrument and your computer.")));
    introLabel_->setFont (Font (15.00f, Font::plain));
    introLabel_->setJustificationType (Justification::centred);
    introLabel_->setEditable (false, false, false);
    introLabel_->setColour (TextEditor::textColourId, Colours::black);
    introLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (504, 150);


    //[Constructor] You can add your own custom stuff here..
    receiveButton_->setEnabled(false);
    sendButton_->setEnabled(false);
    //[/Constructor]
}

ProjectsComponent::~ProjectsComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    projectNumberCombo_ = nullptr;
    receiveButton_ = nullptr;
    sendButton_ = nullptr;
    introLabel_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ProjectsComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ProjectsComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    projectNumberCombo_->setBounds ((getWidth() / 2) + 4 - (176 / 2), 64, 176, 24);
    receiveButton_->setBounds (48, 104, 200, 24);
    sendButton_->setBounds (272, 104, 184, 24);
    introLabel_->setBounds ((getWidth() / 2) - (488 / 2), 8, 488, 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ProjectsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == projectNumberCombo_)
    {
        //[UserComboBoxCode_projectNumberCombo_] -- add your combo box handling code here..
        checkTransferConditions();
        //[/UserComboBoxCode_projectNumberCombo_]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void ProjectsComponent::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == receiveButton_)
    {
        //[UserButtonCode_receiveButton_] -- add your button handler code here..
        FileChooser fc("Choose the project file to save to...",
                       File::getCurrentWorkingDirectory(),
                       "*.lpr",
                       true);
        if (fc.browseForFileToSave(true))
        {
            setBusy(true);
            UpdaterApplication::getApp().saveProject(projectNumberCombo_->getSelectedItemIndex(), fc.getResult());
            setBusy(false);
        }
        //[/UserButtonCode_receiveButton_]
    }
    else if (buttonThatWasClicked == sendButton_)
    {
        //[UserButtonCode_sendButton_] -- add your button handler code here..
        FileChooser fc("Choose the project file to load...",
                       File::getCurrentWorkingDirectory(),
                       "*.lpr",
                       true);
        if (fc.browseForFileToOpen())
        {
        }
        //[/UserButtonCode_sendButton_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ProjectsComponent::setBusy(bool state)
{
    projectNumberCombo_->setVisible(!state);
    receiveButton_->setVisible(!state);
    sendButton_->setVisible(!state);
}

void ProjectsComponent::checkTransferConditions()
{
    if (projectNumberCombo_->getSelectedItemIndex() >= 0 && projectNumberCombo_->getSelectedItemIndex() < 16)
    {
        receiveButton_->setEnabled(true);
        sendButton_->setEnabled(true);
    }
    else {
        receiveButton_->setEnabled(false);
        sendButton_->setEnabled(false);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ProjectsComponent" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="504" initialHeight="150">
  <BACKGROUND backgroundColour="ffffffff"/>
  <COMBOBOX name="project number combo" id="24fa3c431c67a0ea" memberName="projectNumberCombo_"
            virtualName="" explicitFocusOrder="0" pos="4Cc 64 176 24" editable="0"
            layout="36" items="1&#10;2&#10;3&#10;4&#10;5&#10;6&#10;7&#10;8&#10;9&#10;10&#10;11&#10;12&#10;13&#10;14&#10;15&#10;16"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="receive button" id="1d669813a9b2c000" memberName="receiveButton_"
              virtualName="" explicitFocusOrder="0" pos="48 104 200 24" buttonText="Receive From LinnStrument"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="send button" id="a89f7b810849aabc" memberName="sendButton_"
              virtualName="" explicitFocusOrder="0" pos="272 104 184 24" buttonText="Send To LinnStrument"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="intro label" id="2c00dca23c82e0a6" memberName="introLabel_"
         virtualName="" explicitFocusOrder="0" pos="0Cc 8 488 48" edTextCol="ff000000"
         edBkgCol="0" labelText="Please select which sequencer project number&#10;to transfer between LinnStrument and your computer."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
