#ifdef BOARD_ATmega328

#include "gpio.h"

bool GPIO::B0() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINC & (1 << 0);
#else
    return PINC & (1 << 0);
#endif
}

bool GPIO::B1() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINC & (1 << 1);
#else
    return PINC & (1 << 1);
#endif
}

bool GPIO::B2() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINC & (1 << 2);
#else
    return PINC & (1 << 2);
#endif
}

bool GPIO::B3() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINC & (1 << 3);
#else
    return PINC & (1 << 3);
#endif
}

bool GPIO::B4() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINC & (1 << 4);
#else
    return PINC & (1 << 4);
#endif
}

bool GPIO::A7() {
    return PINC & (1 << 5); // A7 is not inverted
}

uint8_t GPIO::read() {
    return PINB & 0x1F; // Read W1, W2, W4, W8, ER pins
}

GPIO gpio;

#endif