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

#pragma once
#include <stdint.h>
#include <stdio.h>

#include "config.h"

// Formats the emulated data into a string
// The format is " 1234.56: -1234.56: 1234.56: -1234.56\n"
// Each axis value is formatted with leading spaces and a dot at the correct position
template <size_t AXIS_COUNT_T = AXIS_COUNT, int AXIS_DIGIT_COUNT_T = AXIS_DIGIT_COUNT,
          int AXIS_DOT_POSITION_T = AXIS_DOT_POSITION>
char* format(const int64_t (&axis)[AXIS_COUNT_T]) {
    // Buffer to hold the formatted string
    static char buffer[AXIS_COUNT_T * 10 + 1];

    // Compile-time calculation of format string components
    constexpr int integer_digits = AXIS_DOT_POSITION_T;
    constexpr int fractional_digits = AXIS_DIGIT_COUNT_T - AXIS_DOT_POSITION_T;

    static const char plus_str[] = "        ";
    static const char minus_str[] = "       -";

    // Compile-time divisor calculation
    constexpr auto calculate_divisor = []() constexpr {
        int div = 1;
        for (int j = 0; j < fractional_digits; j++) div *= 10;
        return div;
    };
    constexpr int divisor = calculate_divisor();

    char* buffer_ptr = buffer;

    // Format each axis value
    for (uint8_t i = 0; i < AXIS_COUNT_T; i++) {
        // Insert sign for negative values or space for positive values
        int64_t value = axis[i];
        const char* sign = &plus_str[AXIS_DIGIT_COUNT_T];
        if (value < 0) {
            value = -value;
            sign = &minus_str[AXIS_DIGIT_COUNT_T];
        }

        // Calculate integer and fractional parts
        int integer_part = value / divisor;
        int fractional_part = value % divisor;

        // Format based on whether we have fractional digits
        if constexpr (fractional_digits > 0) {
            // Build format string with decimal point and fractional part
            constexpr char format_str[] = {'%', 's',                                // sign character + extra spaces
                                           '%', '0', '0' + integer_digits,    'd',  // zero-padded integer part
                                           '.',                                     // decimal point
                                           '%', '0', '0' + fractional_digits, 'd',  // zero-padded fractional part
                                           ':',                                     // axis separator
                                           '\0'};
            buffer_ptr += sprintf(buffer_ptr, format_str, sign, integer_part, fractional_part);
        } else {
            // Build format string without decimal point
            constexpr char format_str[] = {'%', 's',                             // sign character + extra spaces
                                           '%', '0', '0' + integer_digits, 'd',  // zero-padded integer part
                                           ':',                                  // axis separator
                                           '\0'};
            buffer_ptr += sprintf(buffer_ptr, format_str, sign, integer_part);
        }
    }

    // Replace last colon with newline
    *(buffer_ptr - 1) = '\n';

    // Return the formatted string
    return buffer;
}
