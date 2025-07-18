#pragma message ( "\n\n \
	GPIO: Arduino Mega Pro AVR\n \
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../../avr/interface.h"

void gpio_begin(){
	AVR_PORT_CONFIG(A, 0x3F);
	AVR_PORT_CONFIG(B, 0x80);
	AVR_PORT_CONFIG(C, 0);
	AVR_PORT_CONFIG(D, 0);
	AVR_PORT_CONFIG(E, 0);
	AVR_PORT_CONFIG(F, 0);
	AVR_PORT_CONFIG(G, 0);
	AVR_PORT_CONFIG(H, 0x7A);
	AVR_PORT_CONFIG(J, 0);
	AVR_PORT_CONFIG(K, 0);
	AVR_PORT_CONFIG(L, 0);
}

#ifdef GPIO_INVERT_B_SIGNALS
#include "arduino_mega_pro_avr_b_inverted.c"
#else
#include "arduino_mega_pro_avr_b_direct.c"
#endif

void gpio_writeData(uint8_t sign, yn_t error){
	volatile uint8_t regval = PORTH;
	regval &= 0x85;
	regval |= sign<<3;
	if(error == YES){
		regval |= 0x02;
	}
	PORTH = regval;
}

void gpio_blinkLed(){
	AVR_PIN_HIGH(B, 7);
	_delay_us(100.0);
	AVR_PIN_LOW(B, 7);
}



