#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "../result_t/interface.h"
#include "../yn_t/interface.h"

extern void gpio_begin();
extern void gpio_B0();
extern void gpio_B1();
extern void gpio_B2();
extern void gpio_B3();
extern void gpio_B4();
extern void gpio_A7B0();
extern void gpio_A7B1();
extern void gpio_A7B2();
extern void gpio_A7B3();
extern void gpio_A7B4();
extern void gpio_writeData(uint8_t sign, yn_t error);
extern void gpio_blinkLed();
#ifdef MODE_TEST
extern void gpio_print();
#endif

#endif
