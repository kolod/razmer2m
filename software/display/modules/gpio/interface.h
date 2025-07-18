#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

extern void gpio_begin();

extern void gpio_clkHigh();
extern void gpio_clkLow();

extern void gpio_csLow();
extern void gpio_csHigh();

extern void gpio_dataWriteForEach(uint8_t v);
extern void gpio_dataWriteForAll(int v);
extern void gpio_blinkLed();

#endif
