#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "../yn_t/interface.h"

extern void gpio_begin();

extern yn_t gpio_B0isActive();
extern yn_t gpio_B1isActive();
extern yn_t gpio_B2isActive();
extern yn_t gpio_B3isActive();
extern yn_t gpio_B4isActive();
extern yn_t gpio_A7isActive();

extern uint8_t gpio_read();

extern void gpio_blinkLed();
extern void gpio_ledOn();
extern void gpio_ledOff();

#endif
