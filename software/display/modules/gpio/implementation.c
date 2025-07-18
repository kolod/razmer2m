#include "interface.h"

#ifdef GPIO_OPTIONS_ARDUINO_NANO_AVR
#include "options/arduino_nano_avr.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_NANO_WIRING
#include "options/arduino_nano_wiring.c"
#else
#include "options/dummy.c"
#endif
#endif
