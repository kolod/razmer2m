#pragma once

#include <stdint.h>

// Mock Arduino functions for testing
extern uint32_t millis();
extern uint32_t micros();

// Common Arduino types
typedef bool boolean;
typedef uint8_t byte;

// Arduino constants
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

// Mock delay functions
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
