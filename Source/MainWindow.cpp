/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#include "MainWindow.h"

#include "CommandIDs.h"
#include "MainComponent.h"
#include "UpdaterApplication.h"

MainWindow::MainWindow() :
#ifdef LINNSTRUMENT_LOADER
DocumentWindow(String("LinnStrument Loader"),
#else
    DocumentWindow(String(ProjectInfo::projectName),
#endif
    Colours::white, DocumentWindow::closeButton)
{
#if JUCE_MAC
    extraMenu_.addCommandItem(commandManager, CommandIDs::version);
    setMacMainMenu(this, &extraMenu_);
#else
    setMenuBar(this);
#endif

    setResizable(false, false);
    setUsingNativeTitleBar(true);
    
    setOpaque(true);
    addToDesktop(getDesktopWindowStyleFlags());
    setContentOwned(new MainComponent(), true);
    
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
}

MainWindow::~MainWindow()
{
#if JUCE_MAC
    setMacMainMenu(0, 0);
#else
    setMenuBar(0);
#endif
    
    clearContentComponent();
}

void MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}


StringArray MainWindow::getMenuBarNames()
{
#ifdef JUCE_MAC
    return StringArray();
#else
    const char* const names[] = { CommandCategories::help, 0 };
    return StringArray(names);
#endif
}

PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const String &)
{
    PopupMenu menu;
    
    if (topLevelMenuIndex == 0)
    {
        // "Help" menu
        
#ifndef JUCE_MAC
        menu.addCommandItem(commandManager, CommandIDs::version);
        
        menu.addSeparator();
        
        menu.addCommandItem(commandManager, CommandIDs::quit);
#endif
    }
    
    return menu;
}

void MainWindow::menuItemSelected(int menuItemID, int)
{
}
