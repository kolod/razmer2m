#pragma message ( "\n\n \
	GPIO: dummy\n \
" )

void gpio_begin(){
	;
}

logicLevel_t gpio_readA7(){
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB0(){
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB1(){
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB2(){
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB3(){
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_readB4(){
	return LOGIC_LEVEL_LOW;
}

uint8_t gpio_readSignals(){
	uint8_t out = 0;
	return out;
}

void gpio_blinkLed(){
	;
}
void gpio_ledOn(){
	;
}
void gpio_ledOff(){
	;
}
