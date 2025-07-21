#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro Wiring\n \
	name\tAVR\tArduino\tmode\n \
	ER\tPH1\t16\tinput\n \
	W1\tPH3\t6\tinput\n \
	W2\tPH4\t7\tinput\n \
	W4\tPH5\t8\tinput\n \
	W8\tPH6\t9\tinput\n \
	B0\tPA0\t22\tinput\n \
	B1\tPA1\t23\tinput\n \
	B2\tPA2\t24\tinput\n \
	B3\tPA3\t25\tinput\n \
	B4\tPA4\t26\tinput\n \
	A7\tPA5\t27\tinput\n \
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
#include "../../connection.h"

void gpio_begin(){
	pinMode(W1_PIN, INPUT);
	pinMode(W2_PIN, INPUT);
	pinMode(W4_PIN, INPUT);
	pinMode(W8_PIN, INPUT);
	pinMode(ER_PIN, INPUT);
	pinMode(B0_PIN, INPUT);
	pinMode(B1_PIN, INPUT);
	pinMode(B2_PIN, INPUT);
	pinMode(B3_PIN, INPUT);
	pinMode(B4_PIN, INPUT);
	pinMode(A7_PIN, INPUT);
	pinMode(DLED_PIN, OUTPUT);
}

#ifdef GPIO_INVERT_B_SIGNALS
#include "arduino_mega_pro_wiring_b_inverted.c"
#else
#include "arduino_mega_pro_wiring_b_direct.c"
#endif

yn_t gpio_A7isActive(){
	if(digitalRead(A7_PIN) == HIGH){
		return YES;
	}
	return NO;
}

uint8_t gpio_read(){
	uint8_t out = 0;
	if(digitalRead(W1_PIN) == HIGH){
		out |= 1;
	}
	if(digitalRead(W2_PIN) == HIGH){
		out |= 2;
	}
	if(digitalRead(W4_PIN) == HIGH){
		out |= 4;
	}
	if(digitalRead(W8_PIN) == HIGH){
		out |= 8;
	}
	if(digitalRead(W8_PIN) == HIGH){
		out |= CONNECTION_ERROR_MASK_1;
	}
	return out;
}

void gpio_blinkLed(){
	digitalWrite(DLED_PIN, HIGH);
	delayMicroseconds(1);
	digitalWrite(DLED_PIN, LOW);
}
void gpio_ledOn(){
	digitalWrite(DLED_PIN, HIGH);
}
void gpio_ledOff(){
	digitalWrite(DLED_PIN, LOW);
}
