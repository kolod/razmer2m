#include "interface.h"

#ifdef MODE_TEST
#include "options/stdout.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_NANO_AVR
#include "options/dummy.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_NANO_WIRING
#include "options/dummy.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_MEGA_PRO_WIRING
#include "options/arduino_mega_pro_wiring.c"
#else
#ifdef GPIO_OPTIONS_ARDUINO_MEGA_PRO_AVR
#include "options/arduino_mega_pro_avr.c"
#else
#include "options/dummy.c"
#endif
#endif
#endif
#endif
#endif
