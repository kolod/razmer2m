#pragma once

#include <Arduino.h>
#include "gpio.h"
#include "timer.h"
#include "config.h"


class Reader {
public:
    Reader();
    void begin();
    void run(Buffer *buffer);

private:
    enum ReadState {
        A7NotActive,
        A7Active,
        A7NotActive2,
        B0,
        B1,
        B2,
        B3,
        B4,
        Error
    };

    MillisTimer errorTimer;
    MillisTimer sendErrorTimer;

    ReadState state;
    uint8_t count;


    void writeCode(Buffer *buffer);
    void setError(Buffer *buffer);
    void readCode(Buffer *buffer);
    void readErrorSignals(Buffer *buffer);
}

extern Reader reader;
