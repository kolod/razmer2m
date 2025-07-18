#include "interface.h"

#include "../Arduino.h"
#include "../Ton/interface.h"
#include "../esbuffer/interface.h"
#include "../mr1/interface.h"

static Ton change_timern;
static Ton *change_timer = &change_timern;
static Ton mode_timern;
static Ton *mode_timer = &mode_timern;

static void changeData_DUMMY();
static void changeData_START();
static void changeData_STEP1();
static void changeData_STEP2();
static void changeData_STEP3();
static void (*changeData)() = changeData_DUMMY;

static void changeData_DUMMY(){
	;
}

static void changeData_START(){
	ton_setInterval(change_timer, 500);
	ton_setInterval(mode_timer, 10000);
	ton_reset(change_timer);
	ton_reset(mode_timer);
	changeData = changeData_STEP1;
}

static void changeData_STEP1(){
	if(ton(change_timer)){
		ton_reset(change_timer);
		esbuffer_changeData();
	}
	if(ton(mode_timer)){
		ton_reset(mode_timer);
		ton_reset(change_timer);
		changeData = changeData_STEP2;
		return;
	}
}

static void changeData_STEP2(){
	if(ton(change_timer)){
		ton_reset(change_timer);
		esbuffer_changeNextItemError();
	}
	if(ton(mode_timer)){
		ton_setInterval(mode_timer, 3000);
		ton_reset(mode_timer);
		ton_reset(change_timer);
		changeData = changeData_STEP3;
		return;
	}
}

static void changeData_STEP3(){
	if(ton(mode_timer)){
		ton_setInterval(mode_timer, 10000);
		ton_reset(mode_timer);
		ton_reset(change_timer);
		esbuffer_reset();
		changeData = changeData_STEP1;
		return;
	}
}


void writer_begin(){
	esbuffer_reset();
	mr1_begin();
	changeData = changeData_START;
}

void writer_run(){
	biState_t s = mr1_write();
	if(s == IDLE){
		changeData();
	}
}
