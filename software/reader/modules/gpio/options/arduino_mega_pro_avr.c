#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro AVR\n \
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

#include <avr/io.h>
#include <util/delay.h>
#include "../../avr/interface.h"
#include "../../connection.h"

void gpio_begin(){
	DDRA = 0xC0; //input(0), output(1)-low
	AVR_PORT_CONFIG(B, 0x80u);
	AVR_PORT_CONFIG(C, 0);//input pullup
	AVR_PORT_CONFIG(D, 0);
	AVR_PORT_CONFIG(E, 0);
	AVR_PORT_CONFIG(F, 0);
	AVR_PORT_CONFIG(G, 0);
	DDRH = 0x85; //input(0), output(1)-low
	AVR_PORT_CONFIG(J, 0);
	AVR_PORT_CONFIG(K, 0);
	AVR_PORT_CONFIG(L, 0);
}

#ifdef GPIO_INVERT_B_SIGNALS
#include "arduino_mega_pro_avr_b_inverted.c"
#else
#include "arduino_mega_pro_avr_b_direct.c"
#endif

yn_t gpio_A7isActive(){
	if(PINA & 32){
		return YES;
	}
	return NO;
}

uint8_t gpio_read(){
	uint8_t v1 = PINH;
	uint8_t v2 = v1 >> 3;
	if(v1 & 2){
		v2 |= CONNECTION_ERROR_MASK_1;
	}
	return v2;
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
