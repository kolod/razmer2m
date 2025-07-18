#ifndef STOPWATCH_H
#define STOPWATCH_H

extern void stopwatch_begin(int count, void onDoneFunction(unsigned long min_time_us, unsigned long max_time_us));
extern void stopwatch_start();
extern void stopwatch_stop();

#endif
