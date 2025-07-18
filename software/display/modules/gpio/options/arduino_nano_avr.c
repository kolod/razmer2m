#pragma message ( "\n\n \
	GPIO: Arduino Nano AVR\n \
	name\tAVR\tArduino\tmode\n \
	D1\tPD2\t2\toutput\n \
	D2\tPD3\t3\toutput\n \
	D3\tPD4\t4\toutput\n \
	D4\tPD5\t5\toutput\n \
	CLK\tPD6\t6\toutput\n \
	CS\tPD7\t7\toutput\n \
" )

//D1_PIN		PD2
//D2_PIN		PD3
//D3_PIN		PD4
//D4_PIN		PD5
//CLK_PIN		PD6
//CS_PIN		PD7

#include <avr/io.h>
#include <util/delay.h>
#include "../../avr/interface.h"

void gpio_begin(){
	AVR_PORT_CONFIG(B, 0x20);
	AVR_PORT_CONFIG(C, 0x00);
	AVR_PORT_CONFIG(D, 0xFC);
	AVR_PIN_HIGH(D, 7);
}

void gpio_csHigh(){
	AVR_PIN_HIGH(D, 6);
}

void gpio_csLow(){
	AVR_PIN_LOW(D, 6);
}

void gpio_clkHigh(){
	AVR_PIN_HIGH(D, 7);
}

void gpio_clkLow(){
	AVR_PIN_LOW(D, 7);
}


void gpio_dataWriteForEach(uint8_t v){
	volatile uint8_t regval = PORTD;
	regval &= 0xC3;
	regval |= v<<2;
	PORTD = regval;
}

void gpio_dataWriteForAll(int v){
	if(v){
		PORTD |= 0xF<<PD2;
	} else {
		PORTD &= (~(0xF<<PD2));
	}
}

void gpio_blinkLed(){
	AVR_PIN_HIGH(B, 5);
	_delay_us(100.0);
	AVR_PIN_LOW(B, 5);
}
