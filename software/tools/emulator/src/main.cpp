#include <Arduino.h>
#include "config.h"
#include "writer.h"
#include "timer.h"
#include "config.h"
#include "esbuffer.h"
#include "biState_t.h"


static Ton btimern;
static Ton *btimer = &btimern;
static int a_count = 0;
static int ind = 0;

static biState_t (*run)(void) = nullptr;

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

static biState_t run_START() {
	ton_setInterval(btimer, 500);
	tonu_reset(btimer);
	a_count = 0;
	ind = 0;
	run = run_B0;
	return BUSY;
}

static biState_t run_B0() {
	if (tonur(btimer)) {
		writeNextSign();
#if defined(BOARD_ATmega2560)
        PORTA = 0x02; // B1
#elif defined(BOARD_ATmega328)
        PORTC = 0x02; // B1
#endif
		run = run_B1;
	}
	return BUSY;
}

static biState_t run_B1() {
	if (tonur(btimer)) {
		writeNextSign();
#if defined(BOARD_ATmega2560)
        PORTA = 0x04; // B2
#elif defined(BOARD_ATmega328)
        PORTC = 0x04; // B2
#endif
		run = run_B2;
	}
	return BUSY;
}

static biState_t run_B2() {
	if (tonur(btimer)) {
		writeNextSign();
#if defined(BOARD_ATmega2560)
        PORTA = 0x08; // B3
#elif defined(BOARD_ATmega328)
        PORTC = 0x08; // B3
#endif
		run = run_B3;
	}
	return BUSY;
}

static biState_t run_B3() {
	if(tonur(btimer)){
		writeNextSign();
#if defined(BOARD_ATmega2560)
        PORTA = 0x16; // B4
#elif defined(BOARD_ATmega328)
        PORTC = 0x10; // B4
#endif
		run = run_B4;
	}
	return BUSY;
}

static biState_t run_B4() {
	if (tonur(btimer)) {
		a_count++;
		if (a_count == 7) {
#if defined(BOARD_ATmega2560)
            PORTA = 0x21; // A7B0
#elif defined(BOARD_ATmega328)
            PORTC = 0x21; // A7B0
#endif
			run = run_A7B0;
			return IDLE;
		}
#if defined(BOARD_ATmega2560)
        PORTA = 0x01; // B0
#elif defined(BOARD_ATmega328)
        PORTC = 0x01; // B0
#endif
		run = run_B0;
	}
	return BUSY;
}

static biState_t run_A7B0() {
	if (tonur(btimer)) {
#if defined(BOARD_ATmega2560)
        PORTA = 0x22; // A7B1
#elif defined(BOARD_ATmega328)
        PORTC = 0x22; // A7B1
#endif
		run = run_A7B1;
	}
	return IDLE;
}

static biState_t run_A7B1() {
	if (tonur(btimer)) {
#if defined(BOARD_ATmega2560)
        PORTA = 0x24; // A7B2
#elif defined(BOARD_ATmega328)
        PORTC = 0x24; // A7B2
#endif
		run = run_A7B2;
	}
	return IDLE;
}

static biState_t run_A7B2() {
	if (tonur(btimer)) {
#if defined(BOARD_ATmega2560)
        PORTA = 0x28; // A7B3
#elif defined(BOARD_ATmega328)
        PORTC = 0x28; // A7B3
#endif
		run = run_A7B3;
	}
	return IDLE;
}

static biState_t run_A7B3() {
	if (tonur(btimer)) {
#if defined(BOARD_ATmega2560)
        PORTA = 0x30; // A7B4
#elif defined(BOARD_ATmega328)
        PORTC = 0x30; // A7B4
#endif
		run = run_A7B4;
	}
	return IDLE;
}

static biState_t run_A7B4() {
	if (tonur(btimer)) {
#if defined(BOARD_ATmega2560)
        PORTA = 0x01; // B0
#elif defined(BOARD_ATmega328)
        PORTC = 0x01; // B0
#endif
		a_count = 0;
		ind = 0;
		run = run_B0;
		return BUSY;
	}
	return IDLE;
}

void setup() {	
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

	run = run_START;
}

void loop() {
	if (run != nullptr) run();
}
