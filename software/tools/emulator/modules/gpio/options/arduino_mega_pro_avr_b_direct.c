#pragma message ( "\n\n \
	direct B signals \
" )

void gpio_B0(){
	PORTA = 1;
}

void gpio_B1(){
	PORTA = 2;
}

void gpio_B2(){
	PORTA = 4;
}

void gpio_B3(){
	PORTA = 8;
}

void gpio_B4(){
	PORTA = 16;
}

void gpio_A7B0(){
	PORTA = 0x21;
}

void gpio_A7B1(){
	PORTA = 0x22;
}

void gpio_A7B2(){
	PORTA = 0x24;
}

void gpio_A7B3(){
	PORTA = 0x28;
}

void gpio_A7B4(){
	PORTA = 0x30;
}
