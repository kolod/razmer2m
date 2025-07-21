#pragma once

#include <Arduino.h>
#include "config.h"

class GPIO {
public:
    static void begin();
    
    static bool B0isActive();
    static bool B1isActive();
    static bool B2isActive();
    static bool B3isActive();
    static bool B4isActive();
    static bool A7isActive();

    static uint8_t read();

    static void blinkLed();
    static void ledOn();
    static void ledOff();
};

extern GPIO gpio;
