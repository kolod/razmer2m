#ifndef CONFIG_H
#define CONFIG_H

#define START_DELAY_MS			5000

//select hardware your want to use:
#define GPIO_OPTIONS_ARDUINO_NANO_AVR
//#define GPIO_OPTIONS_ARDUINO_NANO_WIRING

//if no data is available over UART after this time, error shall be displayed:
#define COMMUNICATION_TIMEOUT_MS		1000

#endif
