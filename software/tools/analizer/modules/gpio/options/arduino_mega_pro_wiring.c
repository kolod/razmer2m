#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro Wiring\n \
	name\tAVR\tArduino\tmode\n \
	B0\tPA0\t22\tinput\n \
	B1\tPA1\t23\tinput\n \
	B2\tPA2\t24\tinput\n \
	B3\tPA3\t25\tinput\n \
	B4\tPA4\t26\tinput\n \
	A7\tPA5\t27\tinput\n \
	LED\tPB7\t13\toutput\n \
" )

#define B0_PIN		22
#define B1_PIN		23
#define B2_PIN		24
#define B3_PIN		25
#define B4_PIN		26

#define A7_PIN		27

#define DLED_PIN	13

#include "../../Arduino.h"

void gpio_begin(){
	pinMode(B0_PIN, INPUT);
	pinMode(B1_PIN, INPUT);
	pinMode(B2_PIN, INPUT);
	pinMode(B3_PIN, INPUT);
	pinMode(B4_PIN, INPUT);
	pinMode(A7_PIN, INPUT);
	pinMode(DLED_PIN, OUTPUT);
}

logicLevel_t gpio_readA7(){
	if(digitalRead(A7_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB0(){
	if(digitalRead(B0_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB1(){
	if(digitalRead(B1_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB2(){
	if(digitalRead(B2_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB3(){
	if(digitalRead(B3_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB4(){
	if(digitalRead(B4_PIN) == HIGH){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

uint8_t gpio_readSignals(){
	uint8_t out = 0;
	if(digitalRead(B0_PIN) == HIGH){
		out |= 1;
	}
	if(digitalRead(B1_PIN) == HIGH){
		out |= 2;
	}
	if(digitalRead(B2_PIN) == HIGH){
		out |= 4;
	}
	if(digitalRead(B3_PIN) == HIGH){
		out |= 8;
	}
	if(digitalRead(B4_PIN) == HIGH){
		out |= 8;
	}
	if(digitalRead(A7_PIN) == HIGH){
		out |= 32;
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
