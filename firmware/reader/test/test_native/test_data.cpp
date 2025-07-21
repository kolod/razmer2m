#include "unity.h"
#include "data.h"
#include "../../src/data.cpp"

// Test setup and teardown
void setUp(void) {}
void tearDown(void) {}

// Test cases for Data class
void testDataAppendAndRow(void) {
    Data d;
    d.clear();
    d.append('A');
    d.append('B');
    d.append('C');
    d.append('D');
    
    d.printDebug(); // Print the matrix for debugging

    // Assuming row(0) returns first row as string
    TEST_ASSERT_EQUAL_STRING("A", d.row(0).c_str());
    TEST_ASSERT_EQUAL_STRING("B", d.row(1).c_str());
    TEST_ASSERT_EQUAL_STRING("C", d.row(2).c_str());
    TEST_ASSERT_EQUAL_STRING("D", d.row(3).c_str());
}

void testDataFillAll(void) {
    Data d;
    d.clear();
    for (uint8_t i = 0; i < RAZMER2M_ROWS_COUNT * RAZMER2M_COLUMNS_COUNT; i++) {
        d.append('A' + (i % 26)); // Fill with letters A-Z    
        d.printDebug(); // Print the matrix for debugging
    }

    // Check if all rows are filled correctly
    TEST_ASSERT_EQUAL_STRING("AEIMQUY", d.row(0).c_str());
    TEST_ASSERT_EQUAL_STRING("BFJNRVZ", d.row(1).c_str());
    TEST_ASSERT_EQUAL_STRING("CGKOSWA", d.row(2).c_str());
    TEST_ASSERT_EQUAL_STRING("DHLPTXB", d.row(3).c_str());
}

void testDataClear(void) {
    Data d;
    d.append('Z');
    d.clear();
    TEST_ASSERT_EQUAL_STRING("", d.all().c_str()); // After clearing, all should be empty
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(testDataAppendAndRow);
    RUN_TEST(testDataFillAll);
    RUN_TEST(testDataClear);
    return UNITY_END();
}
