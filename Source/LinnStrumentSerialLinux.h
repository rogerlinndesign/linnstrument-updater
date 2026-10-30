#pragma once

#if defined(__linux__)

#include "JuceHeader.h"

#include "LinnStrumentSerial.h"

class LinnStrumentSerialLinux : public LinnStrumentSerial, public Timer
{
public:
    LinnStrumentSerialLinux();
    ~LinnStrumentSerialLinux() override;

    String getFullLinnStrumentDevice() override;
    bool detect() override;
    bool isDetected() override;
    bool prepareDevice() override;
    bool performUpgrade() override;

protected:
    void timerCallback() override;

private:
    ChildProcess upgradeChild;
    bool upgradeVerificationPhase;
    bool upgradeSuccessful;
    String upgradeOutput;
};

#endif
