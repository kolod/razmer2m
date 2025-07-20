#include "timer.h"

// Timer base class implementation
void Timer::setInterval(unsigned long interval) {
    this->mInterval = interval;
}

unsigned long Timer::interval() {
    return mInterval;
}

// returns 1 when time is over
// returns 0 while waiting for an interval  
bool Timer::isExpired(bool reset) {
    unsigned long current_time = now();

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

unsigned long MillisTimer::now() {
    return millis();
}

// MicrosTimer implementation
MicrosTimer::MicrosTimer() {
    mInterval = 0;
    mStart = now();
}

unsigned long MicrosTimer::now() {
    return micros();
}
