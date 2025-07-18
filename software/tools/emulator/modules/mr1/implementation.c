#include "interface.h"

#include "../Arduino.h"
#include "../gpio/interface.h"
#include "../Ton/interface.h"
#include "../esbuffer/interface.h"

static Ton btimern;
static Ton *btimer = &btimern;
static int a_count = 0;
static int ind = 0;

static biState_t run_DUMMY();
static biState_t run_START();
static biState_t run_B0();
static biState_t run_B1();
static biState_t run_B2();
static biState_t run_B3();
static biState_t run_B4();
static biState_t run_A7B0();
static biState_t run_A7B1();
static biState_t run_A7B2();
static biState_t run_A7B3();
static biState_t run_A7B4();

static biState_t (*run)() = run_DUMMY;

static void writeNextSign(){
	gpio_writeData(esbuffer_getItemSign(ind), esbuffer_getItemError(ind));
	ind++;
}

static biState_t run_DUMMY(){
	return IDLE;
}

static biState_t run_START(){
	ton_setInterval(btimer, 500);
	tonu_reset(btimer);
	a_count = 0;
	ind = 0;
	run = run_B0;
	return BUSY;
}

static biState_t run_B0(){
	if(tonur(btimer)){
		writeNextSign();
		gpio_B1();
		run = run_B1;
	}
	return BUSY;
}

static biState_t run_B1(){
	if(tonur(btimer)){
		writeNextSign();
		gpio_B2();
		run = run_B2;
	}
	return BUSY;
}

static biState_t run_B2(){
	if(tonur(btimer)){
		writeNextSign();
		gpio_B3();
		run = run_B3;
	}
	return BUSY;
}

static biState_t run_B3(){
	if(tonur(btimer)){
		writeNextSign();
		gpio_B4();
		run = run_B4;
	}
	return BUSY;
}

static biState_t run_B4(){
	if(tonur(btimer)){
		a_count++;
		if(a_count == 7){
			gpio_A7B0();
			run = run_A7B0;
			return IDLE;
		}
		gpio_B0();
		run = run_B0;
	}
	return BUSY;
}

static biState_t run_A7B0(){
	if(tonur(btimer)){
		gpio_A7B1();
		run = run_A7B1;
	}
	return IDLE;
}

static biState_t run_A7B1(){
	if(tonur(btimer)){
		gpio_A7B2();
		run = run_A7B2;
	}
	return IDLE;
}

static biState_t run_A7B2(){
	if(tonur(btimer)){
		gpio_A7B3();
		run = run_A7B3;
	}
	return IDLE;
}

static biState_t run_A7B3(){
	if(tonur(btimer)){
		gpio_A7B4();
		run = run_A7B4;
	}
	return IDLE;
}

static biState_t run_A7B4(){
	if(tonur(btimer)){
		gpio_B0();
		a_count = 0;
		ind = 0;
		run = run_B0;
		return BUSY;
	}
	return IDLE;
}

void mr1_begin(){
	gpio_begin();
	run = run_START;
}

biState_t mr1_write(){
#ifdef MODE_TEST
	delayMicroseconds(1);
	gpio_print();
#endif
	return run();
}
