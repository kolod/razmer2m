#include "interface.h"

#include "../Arduino.h"
#include "../LLBuffer/interface.h"
#include "../Ton/interface.h"
#include "../printer/interface.h"

static const char *descr = "?";
static unsigned long t1 = 0;
static Ton error_timern;
static Ton *error_timer = &error_timern;
static LLBuffer buffern;
static LLBuffer *buffer = &buffern;

static logicLevel_t getLevel_DUMMY(){return LOGIC_LEVEL_LOW;}
static logicLevel_t (*getLevel)() = getLevel_DUMMY;

static biState_t run_DUMMY(){return IDLE;}
static biState_t run_IDLE();
static biState_t run_START();
static biState_t run_WAIT_HIGH();
static biState_t run_WAIT_LOW();
static biState_t run_WHILE_HIGH();
static biState_t run_WHILE_LOW();
static biState_t run_RESULT_SUCCESS();
static biState_t run_ERROR_HIGH();
static biState_t run_ERROR_LOW();
static biState_t (*run)() = run_DUMMY;

void rsc_begin(const char *description, logicLevel_t (*getLevelFunction)(), unsigned int timeout_ms){
	getLevel = getLevelFunction;
	descr = description;
	ton_setInterval(error_timer, timeout_ms);
	run = run_START;
}

static biState_t run_IDLE(){
	return IDLE;
}

static biState_t run_START(){
	llb_reset(buffer);
	ton_reset(error_timer);
	if(getLevel() == LOGIC_LEVEL_LOW){
		run = run_WAIT_HIGH;
	} else {
		run = run_WAIT_LOW;
	}
	return BUSY;
}

static biState_t run_WAIT_HIGH(){
	if(ton(error_timer)){
		run = run_ERROR_LOW;
		return BUSY;
	}
	if(getLevel() == LOGIC_LEVEL_HIGH){
		ton_reset(error_timer);
		t1 = micros();
		run = run_WHILE_HIGH;
		return BUSY;
	}
	return BUSY;
}

static biState_t run_WAIT_LOW(){
	if(ton(error_timer)){
		run = run_ERROR_HIGH;
		return BUSY;
	}
	if(getLevel() == LOGIC_LEVEL_LOW){
		ton_reset(error_timer);
		t1 = micros();
		run = run_WHILE_LOW;
		return BUSY;
	}
	return BUSY;
}

static biState_t run_WHILE_HIGH(){
	if(ton(error_timer)){
		run = run_ERROR_HIGH;
		return BUSY;
	}
	if(getLevel() == LOGIC_LEVEL_LOW){
		llb_addHighTime(buffer, micros() - t1);
		if(llb_done(buffer) == YES){
			run = run_RESULT_SUCCESS;
			return BUSY;
		}
		ton_reset(error_timer);
		t1 = micros();
		run = run_WHILE_LOW;
		return BUSY;
	}
	return BUSY;
}

static biState_t run_WHILE_LOW(){
	if(ton(error_timer)){
		run = run_ERROR_LOW;
		return BUSY;
	}
	if(getLevel() == LOGIC_LEVEL_HIGH){
		llb_addLowTime(buffer, micros() - t1);
		if(llb_done(buffer) == YES){
			run = run_RESULT_SUCCESS;
			return BUSY;
		}
		ton_reset(error_timer);
		t1 = micros();
		run = run_WHILE_HIGH;
		return BUSY;
	}
	return BUSY;
}

static biState_t run_RESULT_SUCCESS(){
	unsigned long thmin = llb_getHighTimeMin(buffer);
	unsigned long thmax= llb_getHighTimeMax(buffer);
	unsigned long tlmin = llb_getLowTimeMin(buffer);
	unsigned long tlmax = llb_getLowTimeMax(buffer);
	pr_printStr(descr); pr_printStr(": "); pr_printStr("HIGH: "); pr_printULong(thmin); pr_printStr("..."); pr_printULong(thmax); pr_printStr(" (us)."); pr_printNl();
	pr_printStr(descr); pr_printStr(": "); pr_printStr("LOW: "); pr_printULong(tlmin); pr_printStr("..."); pr_printULong(tlmax); pr_printStr(" (us)."); pr_printNl();
	run = run_IDLE;
	return BUSY;
}

static biState_t run_ERROR_HIGH(){
	pr_printStr(descr); pr_printStr(": "); pr_printStr("timeout at HIGH level. "); pr_printStr("Timeout, ms: "); pr_printULong(ton_getInterval(error_timer)); pr_printStr(" count: "); pr_printULong(llb_getLength(buffer));pr_printNl();
	run = run_IDLE;
	return BUSY;
}

static biState_t run_ERROR_LOW(){
	pr_printStr(descr); pr_printStr(": "); pr_printStr("timeout at LOW level. "); pr_printStr("Timeout, ms: "); pr_printULong(ton_getInterval(error_timer)); pr_printStr(" count: "); pr_printULong(llb_getLength(buffer));pr_printNl();
	run = run_IDLE;
	return BUSY;
}

biState_t rsc_run(){
	return run();
}
