#ifndef TIMER_H
#define TIMER_H

//					interface dependencies
#include "../stime_t/interface.h"

/*
* do not access structure elements from other modules directly, 
* although they are declared public for simplification of memory allocation.
*/
typedef struct {
	stime_t interval;
	stime_t end;
} Timer;

extern int timer_done(Timer *self);
extern int timer_doneReset(Timer *self);
extern void timer_setInterval(Timer *self, stime_t interval);
extern stime_t timer_getInterval(Timer *self);
extern void timer_reset(Timer *self);
extern stime_t timer_getTimePassed(Timer *self);
extern stime_t timer_getTimeRest(Timer *self);

#endif
