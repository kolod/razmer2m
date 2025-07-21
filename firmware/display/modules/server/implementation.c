#include "interface.h"

#include <stdint.h>
#include "../Arduino.h"
#include "../connection.h"
#include "../Ton/interface.h"
#include "../result_t/interface.h"
#include "../coop_t/interface.h"
#include "../razmer2m1104.h"
#include "../debug/interface.h"

#define BUSY_TIMEOUT_US				100
#define PACKAGE_TIMEOUT_MS			1000

static Ton busy_timern;
static Ton *busy_timer = &busy_timern;
static Ton package_timern;
static Ton *package_timer = &package_timern;

static coop_t read_IDLE(Buffer *buffer);
static coop_t read_START(Buffer *buffer);
static coop_t read_PACKAGE(Buffer *buffer);
static coop_t (*readPackage)(Buffer *) = read_IDLE;

void server_begin() {
#ifndef MODE_TEST
	Serial.begin(CONNECTION_UART_BAUD_RATE, CONNECTION_UART_DPS);
#endif
	ton_setInterval(busy_timer, BUSY_TIMEOUT_US);
	tonu_reset(busy_timer);
	ton_setInterval(package_timer, PACKAGE_TIMEOUT_MS);
	ton_reset(package_timer);
	readPackage = read_START;
}

static coop_t read_IDLE(Buffer *buffer){
	return YIELD;
}

static coop_t read_START(Buffer *buffer){
#ifndef MODE_TEST
	int c = Serial.read();
#else
	int c = -1;
#endif
	if(c == -1){//no data
		return YIELD;
	}
	if(c == CONNECTION_PACKAGE_DELIMITER_START){
		if(buffer_lockForWriting(buffer) != RESULT_SUCCESS){
			return YIELD;
		}
		ton_reset(package_timer);
		readPackage = read_PACKAGE;
		return PROCEED;
	}
	return YIELD;
}

static coop_t read_PACKAGE(Buffer *buffer){
	if(ton(package_timer)){
		buffer_unlockForWriting(buffer);
		readPackage = read_START;
		return YIELD;
	}
#ifndef MODE_TEST
	int c = Serial.read();
#else
	int c = -1;
#endif
	if(c == -1){//no data
		return YIELD;
	}
	if(c == CONNECTION_PACKAGE_DELIMITER_STOP){
		buffer_unlockForWriting(buffer);
		readPackage = read_START;
		return YIELD;
	}
	buffer_writeNext(buffer, (uint8_t) c);
	return PROCEED;
}

void server_run(Buffer *buffer){
	tonu_reset(busy_timer);
	while (1) {
		if (tonu(busy_timer)) {return;}
	    if(readPackage(buffer) == YIELD){
			return;
		}
	}
}
