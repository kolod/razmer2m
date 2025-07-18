//					implementation dependencies
#include "interface.h"

int timer_done(Timer *self) {
	stime_t now = getCurrentTime();
	if(stime_compareM(&now, &self->end) == YES) {
		return 1;
	}
	return 0;
}

int timer_doneReset(Timer *self) {
	stime_t now = getCurrentTime();
	if(stime_compareM(&now, &self->end) == YES) {
		self->end = stime_add(&now, &self->interval);
		return 1;
	}
	return 0;
}

void timer_setInterval(Timer *self, stime_t interval) {
	self->interval = interval;
}

stime_t timer_getInterval(Timer *self){
	return self->interval;
}

void timer_reset(Timer *self) {
	stime_t now = getCurrentTime();
	self->end = stime_add(&now, &self->interval);
}

stime_t timer_getTimePassed(Timer *self) {
	stime_t now = getCurrentTime();
	stime_t start = stime_subtract(&self->end, &self->interval);
	stime_t out = stime_subtract(&now, &start);
	return out;
}

stime_t timer_getTimeRest(Timer *self) {
	stime_t now = getCurrentTime();
	stime_t out;
	if(stime_compareM(&self->end, &now) == YES) {
		out = stime_subtract(&self->end, &now);
	} else {
		stime_reset(&out);
	}
	return out;
}

