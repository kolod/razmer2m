#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "../logicLevel_t/interface.h"

extern void gpio_begin();

extern uint8_t gpio_readSignals();

extern logicLevel_t gpio_readA7();
extern logicLevel_t gpio_readB0();
extern logicLevel_t gpio_readB1();
extern logicLevel_t gpio_readB2();
extern logicLevel_t gpio_readB3();
extern logicLevel_t gpio_readB4();

extern logicLevel_t gpio_getA7Level(uint8_t v);
extern logicLevel_t gpio_getB0Level(uint8_t v);
extern logicLevel_t gpio_getB1Level(uint8_t v);
extern logicLevel_t gpio_getB2Level(uint8_t v);
extern logicLevel_t gpio_getB3Level(uint8_t v);
extern logicLevel_t gpio_getB4Level(uint8_t v);

extern void gpio_blinkLed();
extern void gpio_ledOn();
extern void gpio_ledOff();

#endif
