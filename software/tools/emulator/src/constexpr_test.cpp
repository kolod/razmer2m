#include <Arduino.h>
#include "constexpr_test.h"

void setup() {
    Serial.begin(9600);
    delay(2000); // Wait for serial connection
    
    Serial.println("Testing constexpr support...");
    printConstexprTests();
}

void loop() {
    // Nothing here
}
