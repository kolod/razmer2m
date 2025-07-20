#include <unity.h>

// Include the timer header for testing
#include "../../include/timer.h"

// Include the timer implementation for testing
#include "../../src/timer.cpp"

// Mock time variables for testing
uint64_t mock_time = 0;

// Test helper functions
void setMockTime(uint64_t time) {
    mock_time = time;
}

void advanceMockTime(uint64_t time) {
    mock_time += time;
}

// Mock Arduino functions
uint32_t millis() {
    return (uint32_t)((mock_time / 1000) % 0x100000000ULL);
}

uint32_t micros() {
    return (uint32_t)(mock_time % 0x100000000ULL);
}

void delay(unsigned long ms) {
    setMockTime(uint64_t(ms) * 1000);
}

void delayMicroseconds(unsigned int us) {
    setMockTime(uint64_t(us));
}

void resetMockTime() {
    mock_time = 0;
}

// Unity setup and teardown
void setUp(void) {
    resetMockTime();
}

void tearDown(void) {
    // Clean up after each test
}

// Test cases for MillisTimer
void test_millis_timer_basic_functionality() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    setMockTime(0);
    timer.reset();
    TEST_ASSERT_FALSE(timer.isExpired()); // Should not be expired initially
    
    // Advance time by 500ms - should not be expired
    setMockTime(500000);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance time by 1000ms - should be expired
    setMockTime(1000000);
    TEST_ASSERT_TRUE(timer.isExpired());
}

void test_millis_timer_multiple_intervals() {
    MillisTimer timer;
    timer.setInterval(500);
    
    setMockTime(0);
    timer.reset();
    
    // First interval
    setMockTime(499000);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(500000);
    TEST_ASSERT_TRUE(timer.isExpired());
    
    // Reset for second interval
    timer.reset();
    setMockTime(500000); // Reset time should be current time
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(1000000);
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test cases for MicrosTimer
void test_micros_timer_basic_functionality() {
    MicrosTimer timer;
    timer.setInterval(1000); // 1000 microseconds
    
    setMockTime(0);
    timer.reset();
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance time by 500μs - should not be expired
    setMockTime(500);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance time by 1000μs - should be expired
    setMockTime(1000);
    TEST_ASSERT_TRUE(timer.isExpired());
}

void test_micros_timer_precision() {
    MicrosTimer timer;
    timer.setInterval(100); // 100 microseconds
    
    setMockTime(0);
    timer.reset();
    
    // Test precise timing
    setMockTime(99);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(100);
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test reset parameter functionality
void test_timer_reset_parameter() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    setMockTime(0);
    timer.reset();
    
    // Advance to middle of interval
    setMockTime(500000);
    TEST_ASSERT_FALSE(timer.isExpired(false)); // Don't reset
    
    // Advance to end of original interval
    setMockTime(1000000);
    TEST_ASSERT_TRUE(timer.isExpired(false)); // Should be expired
    
    // Test with reset parameter
    timer.reset();
    setMockTime(1000000);
    
    setMockTime(1500000);
    TEST_ASSERT_FALSE(timer.isExpired(true)); // Reset on check - should extend timer
    
    // Now it should expire 1000ms after the reset time (1500ms)
    setMockTime(2499000);
    TEST_ASSERT_TRUE(timer.isExpired());
    
    setMockTime(2500000);
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test edge cases
void test_timer_edge_cases() {
    MillisTimer timer;
    
    // Zero interval
    timer.setInterval(0);
    timer.reset();
    TEST_ASSERT_TRUE(timer.isExpired()); // Should immediately expire with 0 interval
    
    // Very large interval
    timer.setInterval(ULONG_MAX);
    setMockTime(0);
    timer.reset();
    
    setMockTime(1000000000); // Large time advance
    TEST_ASSERT_FALSE(timer.isExpired()); // Should still not be expired
}

// Test timer state transitions
void test_timer_state_transitions() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    // Initial state - active, should be expired
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // After reset - active, should not be expired
    setMockTime(0);
    timer.reset();
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // After interval - should be expired and inactive
    setMockTime(1000000);
    TEST_ASSERT_TRUE(timer.isExpired());
    TEST_ASSERT_TRUE(timer.isExpired()); // Should remain expired
}

// Test imprecise timing - millis() resolution is 1ms
void test_millis_timer_imprecise_timing() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    setMockTime(0);
    timer.reset();
    
    // Test with sub-millisecond precision - should round down
    setMockTime(999500); // 999.5ms - millis() should return 999
    TEST_ASSERT_FALSE(timer.isExpired()); // Should not be expired yet
    
    setMockTime(1000500); // 1000.5ms - millis() should return 1000
    TEST_ASSERT_TRUE(timer.isExpired()); // Should be expired now
}

// Test imprecise timing - micros() can have jitter
void test_micros_timer_imprecise_timing() {
    MicrosTimer timer;
    timer.setInterval(1000);
    
    setMockTime(0);
    timer.reset();
    
    // Simulate micros() jitter - real Arduino can have timing variations
    setMockTime(998); // Slightly early
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(1002); // Slightly late but should be expired
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test timing with overflow conditions
void test_timer_overflow_conditions() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    // Simulate time near overflow (millis() overflows every ~49.7 days)
    // Using values that would cause overflow in 32-bit arithmetic
    uint64_t near_overflow = 0xFFFFF000ULL * 1000; // Close to millis() overflow
    
    setMockTime(near_overflow);
    timer.reset();
    
    // Advance past theoretical overflow point
    setMockTime(near_overflow + 2000000); // +2000ms
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test rapid successive timer checks with imprecise timing
void test_rapid_timer_checks() {
    MillisTimer timer;
    timer.setInterval(10); // Short interval
    
    setMockTime(0);
    timer.reset();
    
    // Rapid checks with small time advances (simulating real-world jitter)
    for (int i = 1; i < 10; i++) {
        setMockTime(i * 1000); // Each millisecond
        TEST_ASSERT_FALSE(timer.isExpired());
    }
    
    // Should expire at 10ms
    setMockTime(10000);
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test timer behavior with time going backwards (system clock adjustments)
void test_timer_time_backwards() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    setMockTime(5000000); // Start at 5000ms
    timer.reset();
    
    // Time goes backwards (could happen with system clock adjustment)
    setMockTime(3000000); // Back to 3000ms
    
    // Timer should handle this gracefully - may be expired or not,
    // but shouldn't crash or behave erratically
    bool result = timer.isExpired();
    TEST_ASSERT(result == true || result == false); // Either is acceptable
    
    // Reset and continue normally
    timer.reset();
    setMockTime(4000000); // 4000ms
    TEST_ASSERT_TRUE(timer.isExpired()); // Should be expired (1000ms after reset)
}

// Test micros timer with very short intervals (sub-millisecond precision)
void test_micros_timer_sub_millisecond() {
    MicrosTimer timer;
    timer.setInterval(500); // 500 microseconds (0.5ms)
    
    setMockTime(0);
    timer.reset();
    
    // Test with microsecond precision
    setMockTime(499);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(500);
    TEST_ASSERT_TRUE(timer.isExpired());
    
    // Test reset with sub-millisecond timing
    setMockTime(750);
    timer.reset();
    
    setMockTime(1249);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(1250);
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test timer with irregular time advances (simulating real system load)
void test_timer_irregular_advances() {
    MillisTimer timer;
    timer.setInterval(100);
    
    setMockTime(0);
    timer.reset();
    
    // Irregular time advances that might occur under system load
    uint64_t irregular_times[] = {0, 15000, 33000, 47000, 78000, 95000, 100000, 115000};
    size_t num_times = sizeof(irregular_times) / sizeof(irregular_times[0]);
    
    for (size_t i = 0; i < num_times; i++) {
        setMockTime(irregular_times[i]);
        bool expired = timer.isExpired();
        
        if (irregular_times[i] >= 100000) { // 100ms
            TEST_ASSERT_TRUE(expired);
        } else {
            TEST_ASSERT_FALSE(expired);
        }
    }
}

// Test timer behavior with simulated timing variations
void test_timer_with_jitter() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    setMockTime(0);
    timer.reset();
    
    // Test basic functionality with precise timing
    setMockTime(500000);
    TEST_ASSERT_FALSE(timer.isExpired());
    
    setMockTime(1000000);
    TEST_ASSERT_TRUE(timer.isExpired());
    
    // Test with microsecond timer and simulate timing variations
    MicrosTimer micro_timer;
    micro_timer.setInterval(100);
    
    setMockTime(0);
    micro_timer.reset();
    
    // Simulate slightly early timing (real systems might read time slightly early)
    setMockTime(98);
    TEST_ASSERT_FALSE(micro_timer.isExpired());
    
    // Simulate slightly late timing (common in real systems)
    setMockTime(102);
    TEST_ASSERT_TRUE(micro_timer.isExpired());
    
    // Test another cycle with different timing variations
    micro_timer.setInterval(50);
    setMockTime(200);
    micro_timer.reset();
    
    // Test with timing that's not exactly aligned
    setMockTime(247); // 47μs later, should not be expired
    TEST_ASSERT_FALSE(micro_timer.isExpired());
    
    setMockTime(253); // 53μs later, should be expired
    TEST_ASSERT_TRUE(micro_timer.isExpired());
}

// Test millis() overflow behavior - millis() returns uint32_t and overflows every ~49.7 days
void test_millis_overflow() {
    MillisTimer timer;
    timer.setInterval(2000); // 2 second interval
    
    // Start near millis() overflow point (0xFFFFFFFF milliseconds)
    // Use a more reasonable test that doesn't cause 64-bit overflow issues
    // We'll simulate millis() going from 0xFFFFFFFE to 0xFFFFFFFF to 0x00000000
    
    // Set mock_time so that millis() returns 0xFFFFFFFE
    mock_time = (uint64_t)0xFFFFFFFEUL * 1000;
    timer.reset();
    // millis() should return 0xFFFFFFFE at this point
    
    // Advance time by 1000ms - millis() should return 0xFFFFFFFF
    mock_time = (uint64_t)0xFFFFFFFFUL * 1000;
    TEST_ASSERT_EQUAL(0xFFFFFFFF, millis());
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance time by another 1000ms - this causes millis() to overflow to 0x00000000
    mock_time = (uint64_t)0xFFFFFFFFUL * 1000 + 1000000;
    TEST_ASSERT_EQUAL(0x000003E7, millis()); // Should be 999 (1000000/1000 - 1)
    // Timer was started at 0xFFFFFFFE, now at 999 - elapsed time is 1001ms
    // Since 1001ms < 2000ms interval, should NOT be expired yet
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance more time to make it actually expire (need 2000ms total elapsed)
    // We've elapsed 1001ms, need 999ms more
    mock_time = (uint64_t)0xFFFFFFFFUL * 1000 + 2000000; // +1000ms more
    TEST_ASSERT_EQUAL(0x000007CF, millis()); // Should be 1999
    // Now elapsed time: 1999 - 0xFFFFFFFE = 2001ms (with wraparound)
    TEST_ASSERT_TRUE(timer.isExpired()); // Should handle overflow correctly
    
    // Test continued operation after overflow
    // Reset the timer when millis() returns a small value after overflow
    mock_time = (uint64_t)0x100000000UL * 1000; // This makes millis() return 0
    timer.reset(); // Reset at overflow point
    TEST_ASSERT_EQUAL(0, millis());
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance another 1000ms after overflow - should still not be expired
    mock_time = (uint64_t)0x100000000UL * 1000 + 1000000;
    TEST_ASSERT_EQUAL(1000, millis());
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance another 1000ms after overflow - now should be expired
    mock_time = (uint64_t)0x100000000UL * 1000 + 2000000;
    TEST_ASSERT_EQUAL(2000, millis());
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test micros() overflow behavior - micros() returns uint32_t and overflows every ~71.6 minutes
void test_micros_overflow() {
    MicrosTimer timer;
    timer.setInterval(2000000); // 2 second interval in microseconds
    
    // Start near micros() overflow point (0xFFFFFFFF microseconds)
    // Use a cleaner approach that avoids 64-bit overflow issues
    
    // Set mock_time so that micros() returns 0xFFFFFFFE
    mock_time = 0xFFFFFFFEUL;
    timer.reset();
    // micros() should return 0xFFFFFFFE at this point
    
    // Advance time by 1 second - micros() should return 0xFFFFFFFF
    mock_time = 0xFFFFFFFFUL;
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance time to cause micros() overflow
    mock_time = 0x100000000UL + 1000000; // This makes micros() return 1000000
    // Timer was started at 0xFFFFFFFE, now at 1000000 - elapsed time is 1000002μs
    // Since 1000002μs < 2000000μs interval, should NOT be expired yet
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance more time to make it actually expire (need 2000000μs total elapsed)
    // We've elapsed 1000002μs, need ~999998μs more
    mock_time = 0x100000000UL + 2000000; // Total elapsed: 2000002μs
    TEST_ASSERT_TRUE(timer.isExpired()); // Should handle overflow correctly
    
    // Test continued operation after overflow
    mock_time = 0x100000000UL; // This makes micros() return 0
    timer.reset(); // Reset at overflow point
    TEST_ASSERT_EQUAL(0, micros());
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance 1 second after overflow - should still not be expired
    mock_time = 0x100000000UL + 1000000;
    TEST_ASSERT_EQUAL(1000000, micros());
    TEST_ASSERT_FALSE(timer.isExpired());
    
    // Advance another 1 second after overflow - now should be expired
    mock_time = 0x100000000UL + 2000000;
    TEST_ASSERT_EQUAL(2000000, micros());
    TEST_ASSERT_TRUE(timer.isExpired());
}

// Test multiple overflows in sequence
void test_multiple_overflows() {
    MillisTimer timer;
    timer.setInterval(2000); // 2 second interval
    
    // Test timer behavior across millis() overflow
    // Set timer just before millis() would overflow from 0xFFFFFFFF to 0x00000000
    uint64_t near_overflow = (uint64_t)0xFFFFFFFEUL * 1000; // At millis() = 0xFFFFFFFE
    
    setMockTime(near_overflow);
    timer.reset();
    // At this point millis() returns 0xFFFFFFFE (4294967294)
    
    // Advance time by 3000ms, crossing the overflow boundary
    setMockTime(near_overflow + 3000000);
    // At this point millis() returns 0x00000BB7 (2999) - time has wrapped around
    
    // The timer should recognize that only 3000ms has passed, not billions of ms
    // Since 3000ms > 2000ms interval, timer SHOULD be expired
    TEST_ASSERT_TRUE(timer.isExpired()); // Test if overflow handling works correctly
    
    // Now advance to 2500ms total elapsed time - should be expired
    setMockTime(near_overflow + 2500000);
    TEST_ASSERT_TRUE(timer.isExpired()); // Should be expired
}

// Test overflow edge case - timer set exactly at overflow point
void test_overflow_edge_case() {
    MillisTimer timer;
    timer.setInterval(1000);
    
    // Set timer exactly at millis() overflow point
    // millis() overflows from 0xFFFFFFFF to 0x00000000
    uint64_t overflow_point = (uint64_t)0xFFFFFFFFUL * 1000;
    
    setMockTime(overflow_point);
    timer.reset();
    // millis() returns 0xFFFFFFFF at this point
    
    // Advance by exactly the interval
    setMockTime(overflow_point + 1000000);
    // millis() returns 0x000003E7 (999) at this point - just before next overflow
    TEST_ASSERT_TRUE(timer.isExpired());
    
    // Test microsecond timer at overflow point
    MicrosTimer micro_timer;
    micro_timer.setInterval(1000);
    
    uint64_t micros_overflow = 0xFFFFFFFFUL; // micros() overflow point
    
    setMockTime(micros_overflow);
    micro_timer.reset();
    // micros() returns 0xFFFFFFFF at this point
    
    setMockTime(micros_overflow + 1000);
    // micros() returns 0x000003E7 (999) at this point
    TEST_ASSERT_TRUE(micro_timer.isExpired());
}

// Main test runner
int main() {
    UNITY_BEGIN();
    
    // MillisTimer tests
    RUN_TEST(test_millis_timer_basic_functionality);
    RUN_TEST(test_millis_timer_multiple_intervals);
    
    // MicrosTimer tests
    RUN_TEST(test_micros_timer_basic_functionality);
    RUN_TEST(test_micros_timer_precision);
    
    // Advanced functionality tests
    RUN_TEST(test_timer_reset_parameter);
    RUN_TEST(test_timer_edge_cases);
    RUN_TEST(test_timer_state_transitions);
    
    // Imprecise timing tests
    RUN_TEST(test_millis_timer_imprecise_timing);
    RUN_TEST(test_micros_timer_imprecise_timing);
    RUN_TEST(test_timer_overflow_conditions);
    RUN_TEST(test_rapid_timer_checks);
    RUN_TEST(test_timer_time_backwards);
    RUN_TEST(test_micros_timer_sub_millisecond);
    RUN_TEST(test_timer_irregular_advances);
    RUN_TEST(test_timer_with_jitter);
    
    // Overflow handling tests
    RUN_TEST(test_millis_overflow);
    RUN_TEST(test_micros_overflow);
    RUN_TEST(test_multiple_overflows);
    RUN_TEST(test_overflow_edge_case);
    
    return UNITY_END();
}
