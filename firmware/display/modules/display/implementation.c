#include "interface.h"
#include "../Arduino.h"
#include <string.h>
#include <stddef.h>
#include <stddef.h>
#include <inttypes.h>
#include "../gpio/interface.h"
#include "../result_t/interface.h"
#include "../yn_t/interface.h"
#include "../displayError_t/interface.h"
#include "../asciiBcd/interface.h"
#include "../segIndicator/interface.h"
#include "../razmer2m1104.h"
#include "../debug/interface.h"
#include "../tcode/interface.h"
#include "../Ton/interface.h"
#include "../DBuffer/interface.h"
#ifdef MODE_TEST
#include <stdio.h>
#endif

#define MAX7219_BUF_LENGTH					8
#define MAX7219_SIGNS_COUNT					8
#define MAX7219_REG_FIRST_DIGIT				0x01
#define MAX7219_REG_DECODE_MODE				0x09
#define MAX7219_REG_INTENSITY				0x0a
#define MAX7219_REG_SCAN_LIMIT				0x0b
#define MAX7219_REG_SHUTDOWN				0x0c
#define MAX7219_REG_DISPLAY_TEST			0x0f

#define MAX7219_INTENSITY_MIN				0x00
#define MAX7219_INTENSITY_MAX				0x0f
#define MAX7219_DECODE_MODE					0x00
#define MAX7219_SCAN_LIMIT					7
#define MAX7219_SHUTDOWN_STOP				1
#define MAX7219_DISPLAY_TEST_STOP			0
#define MAX7219_BRIGHTNESS 					MAX7219_INTENSITY_MAX

#define ERROR_SIGN_BLINK_INTERVAL_MS		200

//#define DISPLAY_BUF_LEN						16

//#define DISPLAY_FIRST_CHAR					' '
//#define DISPLAY_LAST_CHAR					'z'
//#define DOT_INDEX							2

//#define ERROR_BIT_INDEX						6


static Ton comm_timern;
static Ton *comm_timer = &comm_timern;
static Ton error_timern;
static Ton *error_timer = &error_timern;
static displayError_t error = DISPLAY_ERROR_UNKNOWN;
static DBuffer dbuffern;
static DBuffer *dbuffer = &dbuffern;

static void run_ERROR_COMMUNICATION(Buffer *buffer);
static void run_ERROR_FATAL(Buffer *buffer);
static void run_SUCCESS(Buffer *buffer);
static void run_IDLE(Buffer *buffer);

static void (*run)(Buffer *) = run_IDLE;


static void shiftOutMsbFirstForAll(uint8_t val){
	for (int i = 0; i < 8; i++)  {
		gpio_dataWriteForAll((val & 128) != 0);
		val <<= 1;
		gpio_clkHigh();
		gpio_clkLow();		
	}
}

static void shiftOutMsbFirstForEach(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4){
	for (int i = 0; i < 8; i++)  {
		uint8_t v = ((b1 & 0x80) >> 7) | ((b2 & 0x80) >> 6) | ((b3 & 0x80) >> 5) | ((b4 & 0x80) >> 4);//pack bit[i] of each parameter into single variable. 
		gpio_dataWriteForEach(v);
		b1 <<= 1; b2 <<= 1; b3 <<= 1; b4 <<= 1;
		gpio_clkHigh();
		gpio_clkLow();		
	}
}

static void writeByteForAll(uint8_t reg, uint8_t data) {
	gpio_csLow();
	shiftOutMsbFirstForAll(reg);
	shiftOutMsbFirstForAll(data);
	gpio_csHigh();
} 

static void writeByteForEach(volatile uint8_t reg, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
	gpio_csLow();
	shiftOutMsbFirstForAll(reg);
	shiftOutMsbFirstForEach(b1, b2, b3, b4);
	gpio_csHigh();
}

static void clear() {
	for(size_t i = MAX7219_REG_FIRST_DIGIT; i < MAX7219_REG_FIRST_DIGIT + MAX7219_SIGNS_COUNT; i++) {
		writeByteForAll(i, 0x00);
	}
}

static void setBrightness (uint8_t brightness){
	writeByteForAll(MAX7219_REG_INTENSITY, brightness & 0x0f);
}

static void printStr(const char *str){
#ifndef MODE_TEST
	size_t len = strlen(str);
	for(size_t i=0; i<MAX7219_BUF_LENGTH; i++){
		if(i < len){
			uint8_t sign7 = segIndicator_encodeASCIISwap(str[len - i - 1]);
			writeByteForAll(MAX7219_REG_FIRST_DIGIT + i, sign7);
		} else {
			writeByteForAll(MAX7219_REG_FIRST_DIGIT + i, 0x00);
		}
	}
#else
	printf("%s\n", str);
#endif
}

static void printStrForEach(const char *str){
#ifndef MODE_TEST
	size_t len = strlen(str);
	for(size_t i=0; i<MAX7219_BUF_LENGTH; i++){
		if(i < len){
			uint8_t sign7 = segIndicator_encodeASCIISwap(str[len - i - 1]);
			writeByteForEach(MAX7219_REG_FIRST_DIGIT + i, sign7, sign7, sign7, sign7);
		} else {
			writeByteForEach(MAX7219_REG_FIRST_DIGIT + i, 0x00, 0x00, 0x00, 0x00);
		}
	}
#else
	printf("%s\n", str);
#endif
}

static void printError(){
	printStr(displayError_toString(error));
}

static void writeToDisplay(){
#ifndef MODE_TEST
	for(size_t i=0; i<RAZMER2M_COLUMNS_COUNT; i++){
		uint8_t b1, b2, b3, b4;
		if(dbuffer_getSegmentFromColumn(dbuffer, i, &b1, &b2, &b3, &b4) == RESULT_SUCCESS){
			writeByteForEach(MAX7219_REG_FIRST_DIGIT + i, b1, b2, b3, b4);
		}
	}
#else
	dbuffer_print(dbuffer);
#endif
}

static result_t readFromExternalBuffer(Buffer *buffer){
	if(buffer_lockForReading(buffer) == RESULT_SUCCESS){
		ton_reset(comm_timer);
		for(size_t i=0; i<RAZMER2M_COLUMNS_COUNT; i++){
			uint8_t b1;
			if(buffer_readNext(buffer, &b1) != RESULT_SUCCESS){
				error = DISPLAY_ERROR_BUFFER_READ;
				buffer_unlockForReading(buffer);
				return RESULT_FAILURE;
			}
			uint8_t b2;
			if(buffer_readNext(buffer, &b2) != RESULT_SUCCESS){
				error = DISPLAY_ERROR_BUFFER_READ;
				buffer_unlockForReading(buffer);
				return RESULT_FAILURE;
			}
			uint8_t b3;
			if(buffer_readNext(buffer, &b3) != RESULT_SUCCESS){
				error = DISPLAY_ERROR_BUFFER_READ;
				buffer_unlockForReading(buffer);
				return RESULT_FAILURE;
			}
			uint8_t b4;
			if(buffer_readNext(buffer, &b4) != RESULT_SUCCESS){
				error = DISPLAY_ERROR_BUFFER_READ;
				buffer_unlockForReading(buffer);
				return RESULT_FAILURE;
			}
			if(dbuffer_setCodeToColumn(dbuffer, i, b1, b2, b3, b4) != RESULT_SUCCESS){
				error = DISPLAY_ERROR_BUFFER_READ;
				buffer_unlockForReading(buffer);
				return RESULT_FAILURE;
			}
		}
		buffer_unlockForReading(buffer);
		return RESULT_SUCCESS;
	} else {
		if(ton(comm_timer)){
			error = DISPLAY_ERROR_COMMUNICATION;
			return RESULT_FAILURE;
		}
	}
	return RESULT_SUCCESS;
}

static void printData(){
	if(tonr(error_timer)){
		dbuffer_changeError(dbuffer);
		
	}
	if(dbuffer_hasNewData(dbuffer) == YES){
		writeToDisplay();
		//debug_echo();
	}
}

static void run_IDLE(Buffer *buffer){
	;
}

static void run_ERROR_COMMUNICATION(Buffer *buffer){
	if(buffer_readyForReading(buffer) == YES){
		ton_reset(comm_timer);
		run = run_SUCCESS;
		return;
	}
}

static void run_ERROR_FATAL(Buffer *buffer){
	;
}

static void run_SUCCESS(Buffer *buffer){
	if(readFromExternalBuffer(buffer) != RESULT_SUCCESS){
		printError();
		if(error == DISPLAY_ERROR_COMMUNICATION){
			run = run_ERROR_COMMUNICATION;
		} else {
			run = run_ERROR_FATAL;
		}
		return;
	}
	printData();
}

void display_begin(unsigned int communication_timeout_ms){
	gpio_begin();
	dbuffer_begin(dbuffer);
	writeByteForAll(MAX7219_REG_SCAN_LIMIT, MAX7219_SCAN_LIMIT);
	writeByteForAll(MAX7219_REG_DECODE_MODE, MAX7219_DECODE_MODE);
	writeByteForAll(MAX7219_REG_SHUTDOWN, MAX7219_SHUTDOWN_STOP);
	writeByteForAll(MAX7219_REG_DISPLAY_TEST, MAX7219_DISPLAY_TEST_STOP);
	clear();
	setBrightness(MAX7219_BRIGHTNESS);
	ton_setInterval(comm_timer, communication_timeout_ms);
	ton_reset(comm_timer);
	ton_setInterval(error_timer, ERROR_SIGN_BLINK_INTERVAL_MS);
	ton_reset(error_timer);
	error = DISPLAY_ERROR_NONE;
	run = run_SUCCESS;
}

void display_run(Buffer *buffer){
	run(buffer);
}
