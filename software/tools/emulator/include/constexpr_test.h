#ifndef CONSTEXPR_TEST_H
#define CONSTEXPR_TEST_H

#include <Arduino.h>

/**
 * @file constexpr_test.h
 * @brief Test constexpr support in your PlatformIO/AVR GCC setup
 */

// Test 1: Simple constexpr function
constexpr int testConstexpr(int x) {
    return x * 2;
}

// Test 2: Constexpr variable
constexpr int TEST_VALUE = testConstexpr(21);

// Test 3: Constexpr with conditional
constexpr int conditionalConstexpr(int x) {
    return (x > 10) ? x * 2 : x + 5;
}

// Test 4: Constexpr array
constexpr int testArray[] = {1, 2, 3, 4, 5};

// Test 5: Template with constexpr
template<int N>
constexpr int templateConstexpr() {
    return N * N;
}

// Test functions to verify constexpr support
void printConstexprTests() {
    Serial.println("=== Constexpr Support Test ===");
    
    // Print compiler version
    Serial.print("GCC Version: ");
    Serial.println(__GNUC__);
    Serial.print("GCC Minor: ");
    Serial.println(__GNUC_MINOR__);
    Serial.print("GCC Patchlevel: ");
    Serial.println(__GNUC_PATCHLEVEL__);
    
    // Print C++ standard
#ifdef __cplusplus
    Serial.print("C++ Standard: ");
    Serial.println(__cplusplus);
#endif

    // Test constexpr values
    Serial.print("TEST_VALUE (should be 42): ");
    Serial.println(TEST_VALUE);
    
    Serial.print("conditionalConstexpr(15): ");
    Serial.println(conditionalConstexpr(15));
    
    Serial.print("templateConstexpr<5>(): ");
    Serial.println(templateConstexpr<5>());
    
    Serial.print("testArray[2]: ");
    Serial.println(testArray[2]);
    
    Serial.println("==============================");
}

#endif // CONSTEXPR_TEST_H
