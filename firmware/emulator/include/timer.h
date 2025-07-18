#pragma once

#include <Arduino.h>

class Timer {
protected:
	unsigned long interval;
	unsigned long start;
	unsigned long end;
	int active;

	virtual unsigned long getCurrentTime() = 0;

public:
	void setInterval(unsigned long interval_value);
	unsigned long getInterval();
	void expire();
	unsigned long getRest();
	int check();
	int checkAndReset();
	
	// Pure virtual methods for subclasses to implement
	virtual void reset() = 0;
};

class MillisTimer : public Timer {
protected:
	unsigned long getCurrentTime() override;

public:
	void reset() override;
};

class MicrosTimer : public Timer {
protected:
	unsigned long getCurrentTime() override;

public:
	void reset() override;
};
