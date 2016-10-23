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

#include <stdint.h>

class LinnStrumentSerial
{
public:
    LinnStrumentSerial() : projectCount(0), projectSize(0) {};
    virtual ~LinnStrumentSerial() {};

    virtual String getFullLinnStrumentDevice() = 0;
    virtual bool detect() = 0;
    virtual bool isDetected() = 0;
    virtual bool prepareDevice() = 0;
    virtual bool performUpgrade() = 0;
    
    bool findFirmwareFile();
    void setFirmwareFile(const File& file);
    bool hasFirmwareFile();
    void resetDetection();
    bool readSettings();
    bool restoreSettings();
    bool hasSettings();

    bool saveProject(uint8_t number, const File& file);

protected:
    String firmwareFile;
    String linnstrumentDevice;
 
private:
	MemoryBlock settings;
    MemoryBlock projects;
    uint8_t projectCount;
    uint32_t projectSize;
};

#endif  // LINNSTRUMENTSERIAL_H_INCLUDED
