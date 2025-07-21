
#include "gpio.h"

// Initialization code for GPIO
void GPIO::begin() {
    pin_mode(W1_PIN, INPUT);
    pin_mode(W2_PIN, INPUT);
    pin_mode(W4_PIN, INPUT);
    pin_mode(W8_PIN, INPUT);
    pin_mode(ER_PIN, INPUT);
    pin_mode(B0_PIN, INPUT);
    pin_mode(B1_PIN, INPUT);
    pin_mode(B2_PIN, INPUT);
    pin_mode(B3_PIN, INPUT);
    pin_mode(B4_PIN, INPUT);
    pin_mode(A7_PIN, INPUT);
    pin_mode(LED_BUILTIN, OUTPUT);
}

void GPIO::blinkLed() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

void GPIO::ledOn() {
    digitalWrite(LED_BUILTIN, HIGH);
}

void GPIO::ledOff() {
    digitalWrite(LED_BUILTIN, LOW);
}
