#include "interface.h"
#include <limits.h>
#include <Arduino.h>


#define STOPWATCH_COUNT_DEFAULT 10000UL

static unsigned long stopwatch_count = STOPWATCH_COUNT_DEFAULT;
static unsigned long stopwatch_min = ULONG_MAX;
static unsigned long stopwatch_max = 0UL;
static unsigned long stopwatch_c = 0UL;
static unsigned long stopwatch_t1;
static unsigned long stopwatch_t2;

static void stopwatch_onDoneFunctionDummy(unsigned long min_time_us, unsigned long max_time_us){
	;
}

static void (*stopwatch_onDoneFunction)(unsigned long min_time_us, unsigned long max_time_us) = stopwatch_onDoneFunctionDummy;

void stopwatch_begin(int count, void onDoneFunction(unsigned long min_time_us, unsigned long max_time_us)){
	stopwatch_count = count;
	stopwatch_onDoneFunction = onDoneFunction;
}

void stopwatch_start(){
	stopwatch_t1 = micros();
}

void stopwatch_stop(){
	stopwatch_t2 = micros();
	if(stopwatch_c < stopwatch_count){
		unsigned long td = stopwatch_t2 - stopwatch_t1;
		if(td < stopwatch_min) stopwatch_min = td;
		if(td > stopwatch_max) stopwatch_max = td;
		stopwatch_c++;
	}else{
		stopwatch_onDoneFunction(stopwatch_min, stopwatch_max);
		stopwatch_c = 0UL;
		stopwatch_min = ULONG_MAX;
		stopwatch_max = 0UL;
	} 
}
