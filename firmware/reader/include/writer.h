#pragma once

#include <Arduino.h>
#include "timer.h"
#include "config.h"
#include "buffer.h"

class Writer {
public:
    Writer();
    void begin();
    void run(Buffer *buffer);

private:
    enum WriteState {
        StateIdle,
        StateWaitData,
        StateGetNextSign,
        StateWriteStartSign,
        StateWriteSigns,
        StateWriteStopSign
    };

    uint8_t wSign;
    WriteState currentState;
    MicrosTimer busyTimer;

    // State machine methods
    bool writeDataWaitData(Buffer *buffer);
    bool writeDataGetNextSign(Buffer *buffer);
    bool writeDataWriteStartSign(Buffer *buffer);
    bool writeDataWriteSigns(Buffer *buffer);
    bool writeDataWriteStopSign(Buffer *buffer);
};
