#include "timer.h"

// Timer base class implementation
void Timer::setInterval(unsigned long interval_value) {
	this->interval = interval_value;
}

unsigned long Timer::getInterval() {
	return interval;
}

void Timer::expire() {
	active = 0;
}

unsigned long Timer::getRest() {
	unsigned long r = end - getCurrentTime();
	if (r > interval) {
		return 0;
	}
	return r;
}

// returns 1 when time is over
// returns 0 while waiting for an interval  
int Timer::check() {
	if (!active) return 1;
	unsigned long now = getCurrentTime();
	if (start < end) {
		if (now >= start && now < end) {
			return 0;
		}
		active = 0;
		return 1;
	}
	if (now >= end && now < start) {
		active = 0;
		return 1;
	}
	return 0;
}

// reset on timeout
int Timer::checkAndReset() {
	unsigned long now = getCurrentTime();
	if (start < end) {
		if (now >= start && now < end) {
			return 0;
		}
		reset();
		return 1;
	}
	if (now >= end && now < start) {
		reset();
		return 1;
	}
	return 0;
}

// MillisTimer implementation
unsigned long MillisTimer::getCurrentTime() {
	return millis();
}

void MillisTimer::reset() {
	unsigned long now = millis();
	start = now;
	end = start + interval;
	active = 1;
}

// MicrosTimer implementation
unsigned long MicrosTimer::getCurrentTime() {
	return micros();
}

void MicrosTimer::reset() {
	unsigned long now = micros();
	start = now;
	end = start + interval;
	active = 1;
}
