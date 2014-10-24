/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#ifndef LINNSTRUMENTSERIAL_H_INCLUDED
#define LINNSTRUMENTSERIAL_H_INCLUDED

#include "JuceHeader.h"

class LinnStrumentSerial : public Timer
{
public:
    LinnStrumentSerial();
    ~LinnStrumentSerial();
    
    void timerCallback() override;

    bool findFirmwareFile();
    bool hasFirmwareFile();
    bool detect();
    bool isDetected();
    bool prepareDevice();
    bool performUpgrade();
    
private:
    String firmwareFile;
    String linnstrumentDevice;
    ChildProcess detectionChild;
    ChildProcess preparationChild;
    ChildProcess upgradeChild;
    String upgradeOutput;
    bool upgradeVerificationPhase;
    bool upgradeSuccessful;
};



#endif  // LINNSTRUMENTSERIAL_H_INCLUDED
