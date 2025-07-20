#include "timer.h"

// Timer base class implementation
void Timer::setInterval(uint32_t interval) {
    this->mInterval = interval;
}

uint32_t Timer::interval() {
    return mInterval;
}

// returns 1 when time is over
// returns 0 while waiting for an interval  
bool Timer::isExpired(bool reset) {
    uint32_t current_time = now();

    if ((current_time - mStart) >= mInterval) {
        // Reset the timer for next interval
        if (reset) mStart = current_time;
        return true;
    }

    // Timer has not expired - no reset
    return false;
}

void Timer::reset() {
    mStart = now();
}

// MillisTimer implementation
MillisTimer::MillisTimer() {
    mInterval = 0;
    mStart = now();
}

uint32_t MillisTimer::now() {
    return millis();
}

// MicrosTimer implementation
MicrosTimer::MicrosTimer() {
    mInterval = 0;
    mStart = now();
}

uint32_t MicrosTimer::now() {
    return micros();
}
