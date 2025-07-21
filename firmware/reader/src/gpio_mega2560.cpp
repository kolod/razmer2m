#ifdef BOARD_ATmega2560

#include "gpio.h"

bool GPIO::B0isActive() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINA & (1 << 0);
#else
    return PINA & (1 << 0);
#endif
}

bool GPIO::B1isActive() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINA & (1 << 1);
#else
    return PINA & (1 << 1);
#endif
}

bool GPIO::B2isActive() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINA & (1 << 2);
#else
    return PINA & (1 << 2);
#endif
}

bool GPIO::B3isActive() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINA & (1 << 3);
#else
    return PINA & (1 << 3);
#endif
}

bool GPIO::B4isActive() {
#ifdef GPIO_INVERT_B_SIGNALS
    return !PINA & (1 << 4);
#else
    return PINA & (1 << 4);
#endif
}

bool GPIO::A7isActive() {
    return PINA & (1 << 5); // A7 is not inverted
}

// Read W1, W2, W4, W8, ER pins
// Returns a 5-bit value where each bit corresponds to a pin state
// W1 = bit 0, W2 = bit 1, W4 = bit 2, W8 = bit 3, ER = bit 4
uint8_t GPIO::read() {
    uint8_t temp = PINH; // Read W1, W2, W4, W8, ER pins
    return ((temp >> 3) & 0x0F) | ((temp << 4) & 0x10);
}

GPIO gpio;

#endif
