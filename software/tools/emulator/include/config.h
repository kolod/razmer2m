#pragma once

#define START_DELAY_MS			5000
#define GPIO_INVERT_B_SIGNALS

#if defined(BOARD_ATmega2560)
    #define W1_PIN	    PH3
    #define W2_PIN		PH4
    #define W4_PIN		PH5
    #define W8_PIN		PH6
    #define ER_PIN		PH1
    #define B0_PIN		PA0
    #define B1_PIN		PA1
    #define B2_PIN		PA2
    #define B3_PIN		PA3
    #define B4_PIN		PA4
    #define A7_PIN		PH0
#elif defined(BOARD_ATmega328)
    #define W1_PIN	    PB0
    #define W2_PIN		PB1
    #define W4_PIN		PB2
    #define W8_PIN		PB3
    #define ER_PIN		PB4
    #define B0_PIN		PC0
    #define B1_PIN		PC1
    #define B2_PIN		PC2
    #define B3_PIN		PC3
    #define B4_PIN		PC4
    #define A7_PIN		PC5
#endif
