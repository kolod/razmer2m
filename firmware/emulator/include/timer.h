#pragma once

#include <Arduino.h>
#include <stdint.h>

class Timer {
public:
    void setInterval(uint32_t interval_value);
    uint32_t interval();
    bool isExpired(bool reset = false);
    void reset();

protected:
    uint32_t mInterval;
    uint32_t mStart;
    virtual uint32_t now() = 0; // Pure virtual function to get current time
};

class MillisTimer : public Timer {
private:
    uint32_t now() override;
public:
    MillisTimer();
};

class MicrosTimer : public Timer {
private:
    uint32_t now() override;
public:
    MicrosTimer();
};
