#pragma once

#include <Arduino.h>

class Timer {
public:
	void setInterval(unsigned long interval_value);
	unsigned long interval();
	bool isExpired(bool reset = false);
	void reset();

protected:
	unsigned long mInterval;
	unsigned long mStart;
	virtual unsigned long now() = 0; // Pure virtual function to get current time
};

class MillisTimer : public Timer {
private:
	unsigned long now() override;
public:
	MillisTimer();
};

class MicrosTimer : public Timer {
private:
	unsigned long now() override;
public:
	MicrosTimer();
};
