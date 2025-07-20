# Timer Class Test Documentation

This directory contains unit tests for the Timer class hierarchy.

## Test Environment

- **Platform**: Native (runs on host computer)
- **Framework**: Unity testing framework
- **Mocked dependencies**: Arduino.h functions (millis, micros, delay)

## Test Structure

### Mock Implementation
- `test/mocks/Arduino.h` - Mock Arduino header with function declarations
- `test/mocks/Arduino.cpp` - Mock Arduino implementation placeholder
- Mock time functions implemented directly in test file for better control

### Test Cases

1. **Timer Base Class Tests**
   - Constructor initialization
   - Interval setting and getting
   - Reset and expire functionality

2. **MillisTimer Tests**
   - Basic timing functionality using milliseconds
   - Multiple interval testing
   - State transitions

3. **MicrosTimer Tests**
   - Microsecond precision timing
   - High-precision interval testing

4. **Advanced Tests**
   - Reset parameter functionality
   - Edge cases (zero interval, large intervals)
   - State transition validation

## Running Tests

To run the timer tests:

```bash
pio test -e native_test -f test_timer
```

To run all tests:

```bash
pio test -e native_test
```

## Mock Time Control

The test framework provides several functions to control time during testing:

- `setMockTime(millis, micros)` - Set absolute time values
- `advanceMockTime(millis, micros)` - Advance time by specified amounts
- `resetMockTime()` - Reset time to zero
- `delay(ms)` - Simulate delay and advance time
- `delayMicroseconds(us)` - Simulate microsecond delay

## Test Coverage

The tests cover:
- ✅ Timer initialization and configuration
- ✅ Interval timing accuracy
- ✅ State management (active/inactive)
- ✅ Reset functionality with and without time reset
- ✅ Edge cases and error conditions
- ✅ Both millisecond and microsecond precision
- ✅ Multiple timer cycles
