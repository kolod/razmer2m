#include "interface.h"

#include "../rmc/interface.h"
#include "../rsc/interface.h"
#include "../biState_t/interface.h"
#include "../gpio/interface.h"
#include "../printer/interface.h"

#define ANALIZER_TIMEOUT_MS					10
#define ANALIZER_CYCLE_TIMEOUT_MS			50

static void arun_START();
static void arun_A7();
static void arun_B0();
static void arun_B1();
static void arun_B2();
static void arun_B3();
static void arun_B4();
static void arun_ALL();
static void arun_IDLE();

static void (*arun)();

void anl_begin(){
	arun = arun_START;
}

static void arun_START(){
	pr_printStr("\nA7 test:\n");
	rsc_begin("A7", gpio_readA7, ANALIZER_CYCLE_TIMEOUT_MS);
	arun = arun_A7;
}

static void arun_A7(){
	if(rsc_run() == IDLE){
		pr_printStr("\nB0 test:\n");
		rsc_begin("B0", gpio_readB0, ANALIZER_TIMEOUT_MS);
		arun = arun_B0;
	}
}

static void arun_B0(){
	if(rsc_run() == IDLE){
		pr_printStr("\nB1 test:\n");
		rsc_begin("B1", gpio_readB1, ANALIZER_TIMEOUT_MS);
		arun = arun_B1;
	}
}

static void arun_B1(){
	if(rsc_run() == IDLE){
		pr_printStr("\nB2 test:\n");
		rsc_begin("B2", gpio_readB2, ANALIZER_TIMEOUT_MS);
		arun = arun_B2;
	}
}

static void arun_B2(){
	if(rsc_run() == IDLE){
		pr_printStr("\nB3 test:\n");
		rsc_begin("B3", gpio_readB3, ANALIZER_TIMEOUT_MS);
		arun = arun_B3;
	}
}

static void arun_B3(){
	if(rsc_run() == IDLE){
		pr_printStr("\nB4 test:\n");
		rsc_begin("B4", gpio_readB4, ANALIZER_TIMEOUT_MS);
		arun = arun_B4;
	}
}

static void arun_B4(){
	if(rsc_run() == IDLE){
		pr_printStr("\nAll signal test:\n");
		rmc_begin(ANALIZER_TIMEOUT_MS);
		arun = arun_ALL;
	}
}

static void arun_ALL(){
	if(rmc_run() == IDLE){
		pr_printStr("\nDone!\n");
		arun = arun_IDLE;
	}
}

static void arun_IDLE(){
	;
}

void anl_run(){
	arun();
}
