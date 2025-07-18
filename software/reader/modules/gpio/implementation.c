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
