#ifndef AVR_UTILITY_H
#define AVR_UTILITY_H

#define AVR_PORT_CONFIG(name, val) {uint8_t v = val; DDR##name = v; PORT##name = ~v;} //output(1)-low input(0)-pullup
#define AVR_PIN_HIGH(port, index) PORT##port |= 1<<P##port##index;
#define AVR_PIN_LOW(port, index) PORT##port &= (~(1<<P##port##index));

#endif
