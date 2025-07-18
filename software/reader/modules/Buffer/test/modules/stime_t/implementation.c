#include "interface.h"

#define usec2timespec(ts, us)             \
  (ts)->tv_sec = (time_t) (us / 1000000);            \
  (ts)->tv_nsec = (long) (us % 1000000) * 1000;
  
#define msec2timespec(ts, ms)             \
  (ts)->tv_sec = (time_t) (ms / 1000);            \
  (ts)->tv_nsec = (long) (ms % 1000) * 1000000;

#define timespeccmp(a, b, CMP)             \
  (((a)->tv_sec == (b)->tv_sec) ?            \
   ((a)->tv_nsec CMP (b)->tv_nsec) :            \
   ((a)->tv_sec CMP (b)->tv_sec))

#define timespecadd(a, b, result)            \
  do {               \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;         \
    (result)->tv_nsec = (a)->tv_nsec + (b)->tv_nsec;         \
    if ((result)->tv_nsec >= 1000000000)           \
      {               \
        ++(result)->tv_sec;            \
        (result)->tv_nsec -= 1000000000;           \
      }               \
  } while (0)

#define timespecsub(a, b, result)            \
  do {               \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;         \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;         \
    if ((result)->tv_nsec < 0) {           \
      --(result)->tv_sec;            \
      (result)->tv_nsec += 1000000000;           \
    }               \
  } while (0)
#define timespecclear(tvp) ((tvp)->tv_sec = (tvp)->tv_nsec = 0L)

#define MY_CLOCK CLOCK_REALTIME

#define GET_CURRENT_TIME(V) struct timespec V; clock_gettime(MY_CLOCK, &V)

stime_t getCurrentTime() {
	GET_CURRENT_TIME(out);
	return out;
}

stime_t usToStime (long int us) {
	stime_t t = {0, 0};
	if ( us <= 0 ) {
		return t;
	}
	t.tv_sec = us / 1000000;
	t.tv_nsec = ( us % 1000000 ) * 1000;
	return t;
}

stime_t sToStime (long int s) {
	stime_t t = {0, 0};
	if (s <= 0 ) {
		return t;
	}
	t.tv_sec = s;
	t.tv_nsec = 0;
	return t;
}

yn_t stime_compareE(stime_t *a, stime_t *b){
	if(a->tv_sec == b->tv_sec && a->tv_nsec == b->tv_nsec){
		return YES;
	}
	return NO;
}

yn_t stime_compareNE(stime_t *a, stime_t *b){
	if(a->tv_sec != b->tv_sec || a->tv_nsec != b->tv_nsec){
		return YES;
	}
	return NO;
}

yn_t stime_compareL(stime_t *a, stime_t *b){
	if(timespeccmp(a, b, <)){
		return YES;
	}
	return NO;
}

yn_t stime_compareM(stime_t *a, stime_t *b){
	if(timespeccmp(a, b, >)){
		return YES;
	}
	return NO;
}

yn_t stime_compareLE(stime_t *a, stime_t *b){
	if(timespeccmp(a, b, <=)){
		return YES;
	}
	return NO;
}

yn_t stime_compareME(stime_t *a, stime_t *b){
	if(timespeccmp(a, b, >=)){
		return YES;
	}
	return NO;
}

stime_t stime_add(stime_t *a, stime_t *b) {
	stime_t out;
	timespecadd(a, b, &out);
	return out;
}

stime_t stime_subtract(stime_t *a, stime_t *b) {
	stime_t out;
	timespecsub(a, b, &out);
	return out;
}

void stime_reset(stime_t *self){
	self->tv_sec = 0;
	self->tv_nsec = 0;
}

stime_t getTimePassed (stime_t t) {
	GET_CURRENT_TIME(now);
	stime_t dif;
	timespecsub(&now, &t, &dif);
	return dif;
}

stime_t getTimeRest (stime_t interval, stime_t start) {
	GET_CURRENT_TIME(now);
	stime_t out, sum;
	timespecadd (&interval, &start, &sum);
	timespecsub (&sum, &now, &out);
	return out;
}

yn_t timeHasPassed(stime_t interval, stime_t start, stime_t now) {
	stime_t dif;
	timespecsub (&now, &start, &dif);
	if (timespeccmp(&interval, &dif, <)) {
		return YES;
	}
	return NO;
}

long stime_getSeconds(stime_t self){
	return (long) self.tv_sec;
}

long stime_getNanoseconds(stime_t self){
	return (long) self.tv_nsec;
}

void stime_sleep(stime_t self){
	nanosleep(&self, NULL);
}


#undef usec2timespec
#undef msec2timespec
#undef timespeccmp
#undef timespecadd
#undef timespecsub
#undef timespecclear
#undef MY_CLOCK 
#undef GET_CURRENT_TIME
