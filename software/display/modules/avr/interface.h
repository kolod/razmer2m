#ifndef AVR_UTILITY_H
#define AVR_UTILITY_H

#define AVR_PORT(v) PORT##v
#define AVR_DDR(v) DDR##v
#define AVR_Pij(i,j) P##i##j
#define AVR_PORT_CONFIG(name, val) {uint8_t v = val; AVR_DDR(name) = v; AVR_PORT(name) = ~v;} //output(1)-low input(0)-pullup
#define AVR_PIN_HIGH(port, index) AVR_PORT(port) |= 1<<AVR_Pij(port, index);
#define AVR_PIN_LOW(port, index) AVR_PORT(port) &= (~(1<<AVR_Pij(port, index)));

#endif
