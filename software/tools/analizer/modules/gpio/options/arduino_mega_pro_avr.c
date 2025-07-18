#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro AVR\n \
	name\tAVR\tArduino\tmode\n \
	B0\tPA0\t22\tinput\n \
	B1\tPA1\t23\tinput\n \
	B2\tPA2\t24\tinput\n \
	B3\tPA3\t25\tinput\n \
	B4\tPA4\t26\tinput\n \
	A7\tPA5\t27\tinput\n \
	LED\tPB7\t13\toutput\n \
" )

#include <avr/io.h>
#include <util/delay.h>
#include "../../avr/interface.h"

void gpio_begin(){
	DDRA = 0xC0; //input(0), output(1)-low
	AVR_PORT_CONFIG(B, 0x80u);
	AVR_PORT_CONFIG(C, 0);//input pullup
	AVR_PORT_CONFIG(D, 0);
	AVR_PORT_CONFIG(E, 0);
	AVR_PORT_CONFIG(F, 0);
	AVR_PORT_CONFIG(G, 0);
	AVR_PORT_CONFIG(H, 0);
	AVR_PORT_CONFIG(J, 0);
	AVR_PORT_CONFIG(K, 0);
	AVR_PORT_CONFIG(L, 0);
}

uint8_t gpio_readSignals(){
	uint8_t v = PINA;
	return v & 0x3F;
}

logicLevel_t gpio_readA7(){
	if(PINA & 32){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB0(){
	if(PINA & 1){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB1(){
	if(PINA & 2){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB2(){
	if(PINA & 4){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB3(){
	if(PINA & 8){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB4(){
	if(PINA & 16){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

void gpio_blinkLed(){
	AVR_PIN_HIGH(B, 7);
	_delay_us(1.0);
	AVR_PIN_LOW(B, 7);
}
void gpio_ledOn(){
	AVR_PIN_HIGH(B, 7);
}
void gpio_ledOff(){
	AVR_PIN_LOW(B, 7);
}
