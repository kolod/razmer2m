#pragma message ( "\n\n \
	GPIO: Arduino Nano Wiring\n \
	name\tAVR\tArduino\tmode\n \
	D1\tPD2\t2\toutput\n \
	D2\tPD3\t3\toutput\n \
	D3\tPD4\t4\toutput\n \
	D4\tPD5\t5\toutput\n \
	CLK\tPD6\t6\toutput\n \
	CS\tPD7\t7\toutput\n \
" )

#define D1_PIN		2
#define D2_PIN		3
#define D3_PIN		4
#define D4_PIN		5
#define CS_PIN		6
#define CLK_PIN		7


#include "../Arduino.h"

void gpio_begin(){
	pinMode(D1_PIN, OUTPUT);
	pinMode(CLK_PIN, OUTPUT);
	pinMode(CS_PIN, OUTPUT);
	digitalWrite(D1_PIN, LOW);
	digitalWrite(CLK_PIN, LOW);
	digitalWrite(CS_PIN, HIGH);
}

void gpio_clkHigh(){
	digitalWrite(CLK_PIN, HIGH);
}

void gpio_clkLow(){
	digitalWrite(CLK_PIN, LOW);
}

void gpio_csHigh(){
	digitalWrite(CS_PIN, HIGH);
}

void gpio_csLow(){
	digitalWrite(CS_PIN, LOW);
}

void gpio_dataWriteForEach(uint8_t v){
	if(bitRead(v, 0)){
		digitalWrite(D1_PIN, HIGH);
	} else {
		digitalWrite(D1_PIN, LOW);
	}
	if(bitRead(v, 1)){
		digitalWrite(D2_PIN, HIGH);
	} else {
		digitalWrite(D2_PIN, LOW);
	}
	if(bitRead(v, 2)){
		digitalWrite(D3_PIN, HIGH);
	} else {
		digitalWrite(D3_PIN, LOW);
	}
	if(bitRead(v, 3)){
		digitalWrite(D4_PIN, HIGH);
	} else {
		digitalWrite(D4_PIN, LOW);
	}
}

void gpio_dataWriteForAll(int v){
	if(v){
		digitalWrite(D1_PIN, HIGH);
		digitalWrite(D2_PIN, HIGH);
		digitalWrite(D3_PIN, HIGH);
		digitalWrite(D4_PIN, HIGH);
	} else {
		digitalWrite(D1_PIN, LOW);
		digitalWrite(D2_PIN, LOW);
		digitalWrite(D3_PIN, LOW);
		digitalWrite(D4_PIN, LOW);
	}
}

void gpio_blinkLed(){
	digitalWrite(13, HIGH);
	delayMicroseconds(100);
	digitalWrite(13, LOW);
}
