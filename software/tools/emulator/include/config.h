#ifndef CONFIG_H
#define CONFIG_H

#define START_DELAY_MS			5000

#define GPIO_OPTIONS_ARDUINO_MEGA_PRO_AVR
//#define GPIO_OPTIONS_ARDUINO_MEGA_PRO_WIRING

#define GPIO_INVERT_B_SIGNALS


#ifdef BOARD_ATmega2560

#define W1_PIN		6
#define W2_PIN		7
#define W4_PIN		8
#define W8_PIN		9
#define ER_PIN		16

#define B0_PIN		22
#define B1_PIN		23
#define B2_PIN		24
#define B3_PIN		25
#define B4_PIN		26

#define A7_PIN		27

#define DLED_PIN	13
#endif

#endif
