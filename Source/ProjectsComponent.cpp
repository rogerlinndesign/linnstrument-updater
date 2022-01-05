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

    projectNumberCombo_.reset (new ComboBox ("project number combo"));
    addAndMakeVisible (projectNumberCombo_.get());
    projectNumberCombo_->setEditableText (false);
    projectNumberCombo_->setJustificationType (Justification::centredLeft);
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

    receiveButton_.reset (new TextButton ("receive button"));
    addAndMakeVisible (receiveButton_.get());
    receiveButton_->setButtonText (TRANS("Receive From LinnStrument"));
    receiveButton_->addListener (this);

    receiveButton_->setBounds (48, 104, 200, 24);

    sendButton_.reset (new TextButton ("send button"));
    addAndMakeVisible (sendButton_.get());
    sendButton_->setButtonText (TRANS("Send To LinnStrument"));
    sendButton_->addListener (this);

    sendButton_->setBounds (272, 104, 184, 24);

    introLabel_.reset (new Label ("intro label",
                                  TRANS("Please select which sequencer project number\n"
                                  "to transfer between LinnStrument and your computer.")));
    addAndMakeVisible (introLabel_.get());
    introLabel_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    introLabel_->setJustificationType (Justification::centred);
    introLabel_->setEditable (false, false, false);
    introLabel_->setColour (Label::textColourId, Colours::black);
    introLabel_->setColour (TextEditor::textColourId, Colours::black);
    introLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    busyLabel_.reset (new Label ("busy label",
                                 TRANS("Transferring project ...")));
    addAndMakeVisible (busyLabel_.get());
    busyLabel_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    busyLabel_->setJustificationType (Justification::centred);
    busyLabel_->setEditable (false, false, false);
    busyLabel_->setColour (TextEditor::textColourId, Colours::black);
    busyLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    errorLabel_.reset (new Label ("error label",
                                  TRANS("An unexpected error occurred during the transfer of the project.")));
    addAndMakeVisible (errorLabel_.get());
    errorLabel_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    errorLabel_->setJustificationType (Justification::centred);
    errorLabel_->setEditable (false, false, false);
    errorLabel_->setColour (TextEditor::textColourId, Colours::black);
    errorLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    okButton_.reset (new TextButton ("ok button"));
    addAndMakeVisible (okButton_.get());
    okButton_->setButtonText (TRANS("OK"));
    okButton_->addListener (this);

    doneLabel_.reset (new Label ("done label",
                                 TRANS("The sequencer project was transferred successfully.")));
    addAndMakeVisible (doneLabel_.get());
    doneLabel_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    doneLabel_->setJustificationType (Justification::centred);
    doneLabel_->setEditable (false, false, false);
    doneLabel_->setColour (Label::textColourId, Colours::black);
    doneLabel_->setColour (TextEditor::textColourId, Colours::black);
    doneLabel_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (504, 150);


    //[Constructor] You can add your own custom stuff here..
    receiveButton_->setEnabled(false);
    sendButton_->setEnabled(false);
    busyLabel_->setVisible(false);
    errorLabel_->setVisible(false);
    doneLabel_->setVisible(false);
    okButton_->setVisible(false);
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
    busyLabel_ = nullptr;
    errorLabel_ = nullptr;
    okButton_ = nullptr;
    doneLabel_ = nullptr;


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
    introLabel_->setBounds ((getWidth() / 2) - (488 / 2), 8, 488, 48);
    busyLabel_->setBounds ((getWidth() / 2) - (488 / 2), 84, 488, 24);
    errorLabel_->setBounds ((getWidth() / 2) - (488 / 2), 64, 488, 24);
    okButton_->setBounds ((getWidth() / 2) - (92 / 2), 104, 92, 24);
    doneLabel_->setBounds ((getWidth() / 2) - (488 / 2), 64, 488, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ProjectsComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == projectNumberCombo_.get())
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

    if (buttonThatWasClicked == receiveButton_.get())
    {
        //[UserButtonCode_receiveButton_] -- add your button handler code here..
        FileChooser fc("Choose the project file to save to...",
                       File::getSpecialLocation(File::userHomeDirectory),
                       "*.lpr",
                       true);
        if (fc.browseForFileToSave(true))
        {
            showBusy();
            if (!UpdaterApplication::getApp().saveProject((uint8_t)projectNumberCombo_->getSelectedItemIndex(), fc.getResult())) {
                showError();
            }
            else {
                showDone();
            }
        }
        //[/UserButtonCode_receiveButton_]
    }
    else if (buttonThatWasClicked == sendButton_.get())
    {
        //[UserButtonCode_sendButton_] -- add your button handler code here..
        FileChooser fc("Choose the project file to load...",
                       File::getSpecialLocation(File::userHomeDirectory),
                       "*.lpr",
                       true);
        if (fc.browseForFileToOpen())
        {
            showBusy();
            if (!UpdaterApplication::getApp().loadProject((uint8_t)projectNumberCombo_->getSelectedItemIndex(), fc.getResult())) {
                showError();
            }
            else {
                showDone();
            }
        }
        //[/UserButtonCode_sendButton_]
    }
    else if (buttonThatWasClicked == okButton_.get())
    {
        //[UserButtonCode_okButton_] -- add your button handler code here..
        projectNumberCombo_->setVisible(true);
        receiveButton_->setVisible(true);
        sendButton_->setVisible(true);
        busyLabel_->setVisible(false);
        errorLabel_->setVisible(false);
        doneLabel_->setVisible(false);
        okButton_->setVisible(false);
        //[/UserButtonCode_okButton_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ProjectsComponent::showBusy()
{
    projectNumberCombo_->setVisible(false);
    receiveButton_->setVisible(false);
    sendButton_->setVisible(false);
    busyLabel_->setVisible(true);
    errorLabel_->setVisible(false);
    doneLabel_->setVisible(false);
    okButton_->setVisible(false);
}

void ProjectsComponent::showDone()
{
    projectNumberCombo_->setVisible(false);
    receiveButton_->setVisible(false);
    sendButton_->setVisible(false);
    busyLabel_->setVisible(false);
    errorLabel_->setVisible(false);
    doneLabel_->setVisible(true);
    okButton_->setVisible(true);
}

void ProjectsComponent::showError()
{
    projectNumberCombo_->setVisible(false);
    receiveButton_->setVisible(false);
    sendButton_->setVisible(false);
    busyLabel_->setVisible(false);
    errorLabel_->setVisible(true);
    doneLabel_->setVisible(false);
    okButton_->setVisible(true);
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
            layout="33" items="1&#10;2&#10;3&#10;4&#10;5&#10;6&#10;7&#10;8&#10;9&#10;10&#10;11&#10;12&#10;13&#10;14&#10;15&#10;16"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="receive button" id="1d669813a9b2c000" memberName="receiveButton_"
              virtualName="" explicitFocusOrder="0" pos="48 104 200 24" buttonText="Receive From LinnStrument"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="send button" id="a89f7b810849aabc" memberName="sendButton_"
              virtualName="" explicitFocusOrder="0" pos="272 104 184 24" buttonText="Send To LinnStrument"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="intro label" id="2c00dca23c82e0a6" memberName="introLabel_"
         virtualName="" explicitFocusOrder="0" pos="0Cc 8 488 48" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="Please select which sequencer project number&#10;to transfer between LinnStrument and your computer."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="36"/>
  <LABEL name="busy label" id="b628914686c6be1c" memberName="busyLabel_"
         virtualName="" explicitFocusOrder="0" pos="0Cc 84 488 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Transferring project ..." editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="36"/>
  <LABEL name="error label" id="aef3e69726ed5da1" memberName="errorLabel_"
         virtualName="" explicitFocusOrder="0" pos="0Cc 64 488 24" edTextCol="ff000000"
         edBkgCol="0" labelText="An unexpected error occurred during the transfer of the project."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="ok button" id="fb0f571560d07e48" memberName="okButton_"
              virtualName="" explicitFocusOrder="0" pos="0Cc 104 92 24" buttonText="OK"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="done label" id="da43df7fc3d743ed" memberName="doneLabel_"
         virtualName="" explicitFocusOrder="0" pos="0Cc 64 488 24" textCol="ff000000"
         edTextCol="ff000000" edBkgCol="0" labelText="The sequencer project was transferred successfully."
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
