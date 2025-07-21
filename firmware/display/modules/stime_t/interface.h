#ifndef STIME_H
#define STIME_H

#include <time.h>
#include "../yn_t/interface.h"

typedef struct timespec stime_t;

extern void stime_reset(stime_t *self);
extern stime_t getCurrentTime();
extern stime_t usToStime(long int us);
extern stime_t msToStime (long int ms) ;
extern stime_t sToStime (long int s);
extern unsigned long stimeToMs(stime_t v);
extern unsigned long stimeToUs(stime_t v);

extern yn_t stime_compareE(stime_t *a, stime_t *b);
extern yn_t stime_compareNE(stime_t *a, stime_t *b);
extern yn_t stime_compareL(stime_t *a, stime_t *b);
extern yn_t stime_compareM(stime_t *a, stime_t *b);
extern yn_t stime_compareLE(stime_t *a, stime_t *b);
extern yn_t stime_compareME(stime_t *a, stime_t *b);

extern stime_t stime_add(stime_t *a, stime_t *b);
extern stime_t stime_subtract(stime_t *a, stime_t *b);

extern stime_t getTimePassed (stime_t t);
extern stime_t getTimeRest (stime_t interval, stime_t start);
extern yn_t timeHasPassed(stime_t interval, stime_t start, stime_t now);
extern long stime_getSeconds(stime_t self);
extern long stime_getNanoseconds(stime_t self);

extern void stime_sleep(stime_t self);
extern void stime_delayIdleRest(stime_t interval, stime_t start);

#endif
