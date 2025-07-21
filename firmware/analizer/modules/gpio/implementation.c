#include "interface.h"

#ifdef GPIO_OPTIONS_ARDUINO_MEGA_PRO_AVR
#include "options/arduino_mega_pro_avr.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_MEGA_PRO_WIRING
#include "options/arduino_mega_pro_wiring.c"
#else
#include "options/dummy.c"
#endif
#endif

logicLevel_t gpio_getB0Level(uint8_t v){
	if(v & 1){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_getB1Level(uint8_t v){
	if(v & 2){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_getB2Level(uint8_t v){
	if(v & 4){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_getB3Level(uint8_t v){
	if(v & 8){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_getB4Level(uint8_t v){
	if(v & 16){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}

logicLevel_t gpio_getA7Level(uint8_t v){
	if(v & 32){
		return LOGIC_LEVEL_HIGH;
	}
	return LOGIC_LEVEL_LOW;
}
