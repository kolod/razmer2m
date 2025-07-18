#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro Wiring\n \
	name\tAVR\tArduino\tmode\n \
	ER\tPH1\t16\toutput\n \
	W1\tPH3\t6\toutput\n \
	W2\tPH4\t7\toutput\n \
	W4\tPH5\t8\toutput\n \
	W8\tPH6\t9\toutput\n \
	B0\tPA0\t22\toutput\n \
	B1\tPA1\t23\toutput\n \
	B2\tPA2\t24\toutput\n \
	B3\tPA3\t25\toutput\n \
	B4\tPA4\t26\toutput\n \
	A7\tPA5\t27\toutput\n \
	LED\tPB7\t13\toutput\n \
" )

#define W1_PIN		6
#define W2_PIN		7
#define W4_PIN		8
#define W8_PIN		9
#define ER_PIN		16

#define B0_PIN		22
#define B1_PIN		23
#define B2_PIN		24
#define B3_PIN		25
#define B4_PIN		26

#define A7_PIN		27

#define DLED_PIN	13

#include "../../Arduino.h"

void gpio_begin(){
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
	pinMode(DLED_PIN, OUTPUT);
}

#ifdef GPIO_INVERT_B_SIGNALS
#include "arduino_mega_pro_wiring_b_inverted.c"
#else
#include "arduino_mega_pro_wiring_b_direct.c"
#endif

void gpio_writeData(uint8_t sign, yn_t error){
	if(bitRead(sign, 0)){
		digitalWrite(W1_PIN, HIGH);
	} else {
		digitalWrite(W1_PIN, LOW);
	}
	if(bitRead(sign, 1)){
		digitalWrite(W2_PIN, HIGH);
	} else {
		digitalWrite(W2_PIN, LOW);
	}
	if(bitRead(sign, 2)){
		digitalWrite(W4_PIN, HIGH);
	} else {
		digitalWrite(W4_PIN, LOW);
	}
	if(bitRead(sign, 3)){
		digitalWrite(W8_PIN, HIGH);
	} else {
		digitalWrite(W8_PIN, LOW);
	}
	if(error == YES){
		digitalWrite(ER_PIN, HIGH);     
	} else {
		digitalWrite(ER_PIN, LOW);
	}
}

void gpio_blinkLed(){
	digitalWrite(DLED_PIN, HIGH);
	delayMicroseconds(100);
	digitalWrite(DLED_PIN, LOW);
}



