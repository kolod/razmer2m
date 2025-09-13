//    This is a part of the Razmer2M project
//    Copyright (C) 2025-... Oleksandr Kolodkin <oleksandr.kolodkin@ukr.net>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include "format.h"

TEST(FormatTest, FormatsFourAxisValues) {
    int64_t arr[4] = {123456, -123456, 0, 42};
    // Call the format function
    char* result = format(arr);
    // Example expected output, adjust as per your format implementation
    // e.g., " 1234.56: -1234.56:    0.00:   0.42\n"
    // For now, just check that result is not null and buffer is not empty
    ASSERT_NE(result, nullptr);
    ASSERT_STRNE(result, "");
    // Check for expected substrings or format
    EXPECT_STREQ(result, "  1234.56: -1234.56:  0000.00:  0000.42\n");
}

TEST(FormatTest, FormatsNonDefaultTemplateParameters) {
    // 3 axes, 7 digits, dot at position 3
    int64_t arr[3] = {1234567, -7654321, 42};
    // format<AXIS_COUNT_T, AXIS_DIGIT_COUNT_T, AXIS_DOT_POSITION_T>
    char* result = format<3, 7, 4>(arr);
    ASSERT_NE(result, nullptr);
    ASSERT_STRNE(result, "");
    // Example expected output: " 1234.567:-7654.321: 0000.042\n"
    EXPECT_STREQ(result, " 1234.567:-7654.321: 0000.042\n");
}

TEST(FormatTest, HandlesZeroValues) {
    int64_t arr[4] = {0, 0, 0, 0};
    char* result = format(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "  0000.00:  0000.00:  0000.00:  0000.00\n");
}

TEST(FormatTest, HandlesMaxNegativeValues) {
    int64_t arr[2] = {-999999, -1};
    char* result = format<2>(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, " -9999.99: -0000.01\n");
}

TEST(FormatTest, HandlesMaxPositiveValues) {
    int64_t arr[2] = {999999, 1};
    char* result = format<2>(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "  9999.99:  0000.01\n");
}

TEST(FormatTest, SingleAxisFormat) {
    int64_t arr[1] = {12345};
    char* result = format<1>(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "  0123.45\n");
}

TEST(FormatTest, DifferentDecimalPositions) {
    // Test with dot at position 1 (1 digit before dot, 5 after)
    int64_t arr[2] = {123456, -789012};
    char* result = format<2, 6, 1>(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "  1.23456: -7.89012\n");
}

TEST(FormatTest, NoFractionalDigits) {
    // Test with dot at end (all digits before dot)
    int64_t arr[2] = {123, -456};
    char* result = format<2, 3, 3>(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "     123:    -456\n");
}

TEST(FormatTest, LargeAxisCount) {
    int64_t arr[6] = {111111, -222222, 333333, -444444, 555555, -666666};
    char* result = format<6>(arr);
    ASSERT_NE(result, nullptr);
    // Should contain all 6 axes with colons between them
    EXPECT_NE(strstr(result, "1111.11"), nullptr);
    EXPECT_NE(strstr(result, "-2222.22"), nullptr);
    EXPECT_NE(strstr(result, "3333.33"), nullptr);
    EXPECT_NE(strstr(result, "-4444.44"), nullptr);
    EXPECT_NE(strstr(result, "5555.55"), nullptr);
    EXPECT_NE(strstr(result, "-6666.66"), nullptr);
    // Count colons (should be 5 for 6 axes)
    int colon_count = 0;
    for (char* p = result; *p; p++) {
        if (*p == ':') colon_count++;
    }
    EXPECT_EQ(colon_count, 5);
}

TEST(FormatTest, BufferEndsWithNewlineAndNull) {
    int64_t arr[2] = {123, 456};
    char* result = format<2>(arr);
    ASSERT_NE(result, nullptr);
    size_t len = strlen(result);
    EXPECT_GT(len, 0);
    EXPECT_EQ(result[len - 1], '\n');  // Should end with newline
    EXPECT_EQ(result[len], '\0');      // Should be null-terminated
}

TEST(FormatTest, SpecialCase1) {
    int64_t arr[4] = {100246, 951015, -397891, -908626};
    char* result = format(arr);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "  1002.46:  9510.15: -3978.91: -9086.26\n");
}