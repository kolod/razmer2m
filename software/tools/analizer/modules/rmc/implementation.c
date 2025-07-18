#include "interface.h"

#include <stdint.h>
#include "../Arduino.h"
#include "../VDBuffer/interface.h"
#include "../Ton/interface.h"
#include "../gpio/interface.h"
#include "../printer/interface.h"

static uint8_t last_value;
static unsigned long mt1;

static Ton merror_timern;
static Ton *merror_timer = &merror_timern;

static VDBuffer mbuffern;
static VDBuffer *mbuffer = &mbuffern;

static biState_t mrun_START();
static biState_t mrun_WHILE_A7_HIGH();
static biState_t mrun_WHILE_A7_LOW();
static biState_t mrun_READ();
static biState_t mrun_DONE();
static biState_t mrun_IDLE();
static biState_t mrun_ERROR_A7_HIGH();
static biState_t mrun_ERROR_A7_LOW();
static biState_t mrun_ERROR_READ();

static biState_t mrun_DUMMY(){return IDLE;}

static biState_t (*mrun)() = mrun_DUMMY;

void rmc_begin(unsigned int timeout_ms){
	last_value = 0;
	ton_setInterval(merror_timer, timeout_ms);
	mrun = mrun_START;
}

static biState_t mrun_START(){
	last_value = 0;
	vdb_reset(mbuffer);
	ton_reset(merror_timer);
	mrun = mrun_WHILE_A7_HIGH;
	return BUSY;
}

static biState_t mrun_WHILE_A7_HIGH(){
	if(ton(merror_timer)){
		mrun = mrun_ERROR_A7_HIGH;
		return BUSY;
	}
	if(gpio_readA7() == LOGIC_LEVEL_LOW){
		ton_reset(merror_timer);
		mrun = mrun_WHILE_A7_LOW;
		return BUSY;
	}
	return BUSY;
}

static biState_t mrun_WHILE_A7_LOW(){
	if(ton(merror_timer)){
		mrun = mrun_ERROR_A7_LOW;
		return BUSY;
	}
	if(gpio_readA7() == LOGIC_LEVEL_HIGH){
		mt1 = micros();
		last_value = gpio_readSignals();
		ton_reset(merror_timer);
		mrun = mrun_READ;
		return BUSY;
	}
	return BUSY;
}

static biState_t mrun_READ(){
	if(ton(merror_timer)){
		mrun = mrun_ERROR_READ;
		return BUSY;
	}
	uint8_t v = gpio_readSignals();
	if(v != last_value){
		ton_reset(merror_timer);
		unsigned long t2 = micros();
		vdb_addItem(mbuffer, last_value, t2 - mt1);
		if(vdb_done(mbuffer) == YES){
			mrun = mrun_DONE;
			return BUSY;
		}
		mt1 = t2;
		last_value = v;
	}
	return BUSY;
}

static biState_t mrun_DONE(){
	vdb_print(mbuffer);
	mrun = mrun_IDLE;
	return BUSY;
}

static biState_t mrun_IDLE(){
	return IDLE;
}

static biState_t mrun_ERROR_A7_HIGH(){
	pr_printStr("All"); pr_printStr(": "); pr_printStr("timeout at A7 HIGH level. "); pr_printStr("Timeout, ms: "); pr_printULong(ton_getInterval(merror_timer));pr_printNl();
	mrun = mrun_IDLE;
	return BUSY;
}

static biState_t mrun_ERROR_A7_LOW(){
	pr_printStr("All"); pr_printStr(": "); pr_printStr("timeout at A7 LOW level. "); pr_printStr("Timeout, ms: "); pr_printULong(ton_getInterval(merror_timer));pr_printNl();
	mrun = mrun_IDLE;
	return BUSY;
}

static biState_t mrun_ERROR_READ(){
	pr_printStr("All"); pr_printStr(": "); pr_printStr("timeout while reading from all inputs. "); pr_printStr("Timeout, ms: "); pr_printULong(ton_getInterval(merror_timer));pr_printNl();
	mrun = mrun_IDLE;
	return BUSY;
}

biState_t rmc_run(){
	return mrun();
}
