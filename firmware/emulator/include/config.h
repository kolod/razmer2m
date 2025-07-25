#pragma once

#if !defined(START_DELAY_MS)
    #define START_DELAY_MS 5000
#endif

#define GPIO_INVERT_B_SIGNALS

#define RAZMER2M_ROWS_COUNT				4
#define RAZMER2M_COLUMNS_COUNT			7
#define RAZMER2M_SIGNS_COUNT			(RAZMER2M_ROWS_COUNT * RAZMER2M_COLUMNS_COUNT)
#define RAZMER2M_CODE_PLUS				0
#define RAZMER2M_CODE_MINUS				9

#define CONNECTION_UART_BAUD_RATE				38400
#define CONNECTION_UART_DPS						"8n1"
#define CONNECTION_PACKAGE_DELIMITER_START		'<'
#define CONNECTION_PACKAGE_DELIMITER_STOP		'>'
#define CONNECTION_ERROR_MASK_1					0x40
#define CONNECTION_ERROR_MASK_0					0xBF

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
