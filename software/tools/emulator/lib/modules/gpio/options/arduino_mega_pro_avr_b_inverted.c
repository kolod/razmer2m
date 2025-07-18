#pragma message ( "\n\n \
	inverted B signals \
" )

void gpio_B0(){
	PORTA = 0x1E;
}

void gpio_B1(){
	PORTA = 0x1D;
}

void gpio_B2(){
	PORTA = 0x1B;
}

void gpio_B3(){
	PORTA = 0x17;
}

void gpio_B4(){
	PORTA = 0xF;
}

void gpio_A7B0(){
	PORTA = 0x3E;
}

void gpio_A7B1(){
	PORTA = 0x3D;
}

void gpio_A7B2(){
	PORTA = 0x3B;
}

void gpio_A7B3(){
	PORTA = 0x37;
}

void gpio_A7B4(){
	PORTA = 0x2F;
}
