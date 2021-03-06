/*
 Copyright 2014 Roger Linn Design (www.rogerlinndesign.com)
 
 Written by Geert Bevin (http://gbevin.com).
 
 This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
 To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/
 or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
#ifndef LINNSTRUMENTSERIALMAC_H_INCLUDED
#define LINNSTRUMENTSERIALMAC_H_INCLUDED

#include "JuceHeader.h"

#include "LinnStrumentSerial.h"

class LinnStrumentSerialMac : public LinnStrumentSerial,
                              public Timer
{
public:
    LinnStrumentSerialMac();
    virtual ~LinnStrumentSerialMac();
    
    void timerCallback() override;
    
    String getFullLinnStrumentDevice() override;
    bool detect() override;
    bool isDetected() override;
    bool prepareDevice() override;
    bool performUpgrade() override;
    
private:
    ChildProcess upgradeChild;
    String upgradeOutput;
    bool upgradeVerificationPhase;
    bool upgradeSuccessful;
};

#endif  // LINNSTRUMENTSERIALMAC_H_INCLUDED
