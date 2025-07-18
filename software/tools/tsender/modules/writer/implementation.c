#include "interface.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
 #include <unistd.h>
#include "../connection.h"
#include "../Timer/interface.h"
#include "../stime_t/interface.h"
#include "../debug/interface.h"
#include "../userInput/interface.h"
#include "../esbuffer/interface.h"
#include "../appSerial/interface.h"

static void run_DUMMY();
static void run_START();
static void run_BUSY();
static void run_REPEAT();

static void (*run)() = run_DUMMY;

Timer send_timern;
Timer *send_timer = &send_timern;

Timer change_timern;
Timer *change_timer = &change_timern;

Timer work_timern;
Timer *work_timer = &work_timern;

long int send_interval_ms = 20;
long int change_interval_ms = 300;
long int work_time_ms = 5000;

static void selectOptions(){
	while(1){
		printf("sendInterval changeInterval workTime (ms): ");
		int r = fscanf(stdin, "%ld %ld %ld", &send_interval_ms, &change_interval_ms, &work_time_ms);
		if(r == 3){
			break;
		} else {
			printf("fscanf = %d\n", r);
		}
	}
}

static void selectRepeat(){
	while(1){
		printf("type 's' to start sending data (%ld %ld %ld) or 'o' for more options before start(s | o): ", send_interval_ms, change_interval_ms, work_time_ms);
		char s = userInput_getFirstChar();
		if(s == 's'){
			run = run_START;
			break;
		} else if(s =='o') {
			selectOptions();
			run = run_START;
			break;
		} else {
			continue;
		}
	}
}

static void run_DUMMY(){
	stime_sleep(msToStime(100));
}

static void run_START(){
	esbuffer_reset();
	esbuffer_print();
	timer_setInterval(send_timer, msToStime(send_interval_ms));
	timer_reset(send_timer);
	timer_setInterval(change_timer, msToStime(change_interval_ms));
	timer_reset(change_timer);
	timer_setInterval(work_timer, msToStime(work_time_ms));
	timer_reset(work_timer);
	run = run_BUSY;
}

static void run_BUSY(){
	if(timer_done(send_timer)){
		appSerial_sendBuffer();
		timer_reset(send_timer);
	}
	if(timer_done(change_timer)){
		esbuffer_changeData();
		esbuffer_changeNextItemError();
		esbuffer_print();
		timer_reset(change_timer);
	}
	if(timer_done(work_timer)){
		run = run_REPEAT;
	}
}

static void run_REPEAT(){
	selectRepeat();
}

void writer_begin(){
	appSerial_begin();
	run = run_REPEAT;
}

void writer_run(){
	run();
}

void writer_end(){
	appSerial_end();
}
