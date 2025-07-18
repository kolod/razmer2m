#include <stdio.h>
#include "../../Arduino.h"

#pragma message ( "\n\n \
	GPIO: stdout\n \
" )

static int A7, B0, B1, B2, B3, B4, W1, W2, W4, W8, ER;

void gpio_begin(){
	A7 = B0 = B1 = B2 = B3 = B4 = W1 = W2, W4 = W8 = ER = 0;
	printf("A7 B0 B1 B2 B3 B4 W1 W2 W4 W8 ER\n");
}

void gpio_B0(){
	B0 = 1;
	B1 = 0;
	B2 = 0;
	B3 = 0;
	B4 = 0;
	A7 = 0;
}

void gpio_B1(){
	B0 = 0;
	B1 = 1;
	B2 = 0;
	B3 = 0;
	B4 = 0;
	A7 = 0;
}

void gpio_B2(){
	B0 = 0;
	B1 = 0;
	B2 = 1;
	B3 = 0;
	B4 = 0;
	A7 = 0;
}

void gpio_B3(){
	B0 = 0;
	B1 = 0;
	B2 = 0;
	B3 = 1;
	B4 = 0;
	A7 = 0;
}

void gpio_B4(){
	B0 = 0;
	B1 = 0;
	B2 = 0;
	B3 = 0;
	B4 = 1;
	A7 = 0;
}

void gpio_A7B0(){
	B0 = 1;
	B1 = 0;
	B2 = 0;
	B3 = 0;
	B4 = 0;
	A7 = 1;
}

void gpio_A7B1(){
	B0 = 0;
	B1 = 1;
	B2 = 0;
	B3 = 0;
	B4 = 0;
	A7 = 1;
}

void gpio_A7B2(){
	B0 = 0;
	B1 = 0;
	B2 = 1;
	B3 = 0;
	B4 = 0;
	A7 = 1;
}

void gpio_A7B3(){
	B0 = 0;
	B1 = 0;
	B2 = 0;
	B3 = 1;
	B4 = 0;
	A7 = 1;
}

void gpio_A7B4(){
	B0 = 0;
	B1 = 0;
	B2 = 0;
	B3 = 0;
	B4 = 1;
	A7 = 1;
}

void gpio_writeData(uint8_t sign, yn_t error){
	if(bitRead(sign, 0)){
		W1 = 1;
	} else {
		W1 = 0;
	}
	if(bitRead(sign, 1)){
		W2 = 1;
	} else {
		W2 = 0;
	}
	if(bitRead(sign, 2)){
		W4 = 1;
	} else {
		W4 = 0;
	}
	if(bitRead(sign, 3)){
		W8 = 1;
	} else {
		W8 = 0;
	}
	if(error == YES){
		ER = 1;
	} else {
		ER = 0;
	}
}

void gpio_blinkLed(){
	;
}

void gpio_print(){
	printf("A7:%d  B:%d %d %d %d %d  W:%d %d %d %d  E:%d\n", A7, B0, B1, B2, B3, B4, W1, W2, W4, W8, ER);
}



