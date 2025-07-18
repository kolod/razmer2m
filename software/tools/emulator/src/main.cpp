#include <Arduino.h>
#include "config.h"
#include "timer.h"
#include "config.h"
#include "esbuffer.h"


static Ton btimern;
static Ton *btimer = &btimern;
static Ton change_timern;
static Ton *change_timer = &change_timern;
static Ton mode_timern;
static Ton *mode_timer = &mode_timern;


static int a_count = 0;
static int ind = 0;

// Helper functions to set B signals and A7
static void setBSignals(uint8_t value) {
#if defined(BOARD_ATmega2560)
#ifdef GPIO_INVERT_B_SIGNALS
	PORTA = ~value & 0x1F;
#else
	PORTA = value;
#endif
#elif defined(BOARD_ATmega328)
#ifdef GPIO_INVERT_B_SIGNALS
	PORTC = ~value & 0x3F;
#else
	PORTC = value;
#endif
#endif
}

static bool run_START();
static bool run_B0();
static bool run_B1();
static bool run_B2();
static bool run_B3();
static bool run_B4();
static bool run_A7B0();
static bool run_A7B1();
static bool run_A7B2();
static bool run_A7B3();
static bool run_A7B4();

static void changeData_START();
static void changeData_STEP1();
static void changeData_STEP2();
static void changeData_STEP3();


static bool (*run)() = run_START;
static void (*changeData)() = changeData_START;

void writeNextSign() {
#if defined(BOARD_ATmega2560)
    // For ATmega2560, use direct port manipulation
    PORTH = (PORTH & 0x85) | (0x04 * esbuffer_getItemSign(ind)) | (0x02 * (esbuffer_getItemError(ind) == YES));
#elif defined(BOARD_ATmega328)
    // For ATmega328, use direct port manipulation
    PORTB = (PORTB & 0xE0) | (0x01 * esbuffer_getItemSign(ind)) | (0x10 * (esbuffer_getItemError(ind) == YES));
#endif
	ind++;
}

static bool run_START() {
	ton_setInterval(btimer, 500);
	tonu_reset(btimer);
	a_count = 0;
	ind = 0;
	run = run_B0;
	return false; // busy
}

static bool run_B0() {
	if (tonur(btimer)) {
		writeNextSign();
		setBSignals(0x02); // B1
		run = run_B1;
	}
	return false; // busy
}

static bool run_B1() {
	if (tonur(btimer)) {
		writeNextSign();
		setBSignals(0x04); // B2
		run = run_B2;
	}
	return false; // busy
}

static bool run_B2() {
	if (tonur(btimer)) {
		writeNextSign();
		setBSignals(0x08); // B3
		run = run_B3;
	}
	return false; // busy
}

static bool run_B3() {
	if(tonur(btimer)){
		writeNextSign();
		setBSignals(0x10); // B4
		run = run_B4;
	}
	return false; // busy
}

static bool run_B4() {
	if (tonur(btimer)) {
		a_count++;
		if (a_count == 7) {
			setBSignals(0x21); // A7B0
			run = run_A7B0;
			return true; // ready for data change
		}
		setBSignals(0x01); // B0
		run = run_B0;
	}
	return false; // busy
}

static bool run_A7B0() {
	if (tonur(btimer)) {
		setBSignals(0x22); // A7B1
		run = run_A7B1;
	}
	return false; // busy
}

static bool run_A7B1() {
	if (tonur(btimer)) {
		setBSignals(0x24); // A7B2
		run = run_A7B2;
	}
	return false; // busy
}

static bool run_A7B2() {
	if (tonur(btimer)) {
		setBSignals(0x28); // A7B3
		run = run_A7B3;
	}
	return false; // busy
}

static bool run_A7B3() {
	if (tonur(btimer)) {
		setBSignals(0x30); // A7B4
		run = run_A7B4;
	}
	return false; // busy
}

static bool run_A7B4() {
	if (tonur(btimer)) {
		setBSignals(0x01); // B0
		a_count = 0;
		ind = 0;
		run = run_B0;
		return false; // busy
	}
	return false; // busy
}

static void changeData_START() {
	ton_setInterval(change_timer, 500);
	ton_setInterval(mode_timer, 10000);
	ton_reset(change_timer);
	ton_reset(mode_timer);
	changeData = changeData_STEP1;
}

static void changeData_STEP1(){
	if (ton(change_timer)) {
		ton_reset(change_timer);
		esbuffer_changeData();
	}

	if (ton(mode_timer)) {
		ton_reset(mode_timer);
		ton_reset(change_timer);
		changeData = changeData_STEP2;
		return;
	}
}

static void changeData_STEP2(){
	if (ton(change_timer)) {
		ton_reset(change_timer);
		esbuffer_changeNextItemError();
	} 
	
	if (ton(mode_timer)) {
		ton_setInterval(mode_timer, 3000);
		ton_reset(mode_timer);
		ton_reset(change_timer);
		changeData = changeData_STEP3;
		return;
	}
}

static void changeData_STEP3(){
	if (ton(mode_timer)) {
		ton_setInterval(mode_timer, 10000);
		ton_reset(mode_timer);
		ton_reset(change_timer);
		esbuffer_reset();
		changeData = changeData_STEP1;
		return;
	}
}

void loop() {
	if (run()) {
		if (changeData != nullptr) changeData();
	}
}

void setup() {
	Serial.begin(115200);	

	pinMode(W1_PIN, OUTPUT);
	pinMode(W2_PIN, OUTPUT);
	pinMode(W4_PIN, OUTPUT);
	pinMode(W8_PIN, OUTPUT);
	pinMode(ER_PIN, OUTPUT);
	pinMode(B0_PIN, OUTPUT);
	pinMode(B1_PIN, OUTPUT);
	pinMode(B2_PIN, OUTPUT);
	pinMode(B3_PIN, OUTPUT);
	pinMode(B4_PIN, OUTPUT);
	pinMode(A7_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	esbuffer_reset();
}
