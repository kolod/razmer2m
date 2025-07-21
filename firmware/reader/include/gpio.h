#pragma once

#include <Arduino.h>
#include "config.h"

class GPIO {
public:
    static void begin();
    
    static bool B0();
    static bool B1();
    static bool B2();
    static bool B3();
    static bool B4();
    static bool A7();

    static uint8_t read();

    static void blinkLed();
    static void ledOn();
    static void ledOff();
};

extern GPIO gpio;
