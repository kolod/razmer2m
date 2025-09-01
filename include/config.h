//    This is a part of the Razmer2M project
//    Copyright (C) 2025-... Oleksandr kolodkin <oleksandr.kolodkin@ukr.net>
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
#include <stdlib.h>

// Default configuration values
// These can be overridden by user-defined values
#ifndef AXIS_COUNT
#define AXIS_COUNT (4)  // Number of axes
#endif

#ifndef AXIS_DIGIT_COUNT
#define AXIS_DIGIT_COUNT (6)  // Number of digits per axis
#endif

#ifndef AXIS_DOT_POSITION
#define AXIS_DOT_POSITION (4)  // Position of the decimal point (0-based index)
#endif

#ifndef BAUDRATE
#define BAUDRATE (38400)  // Default baud rate
#endif

// Compile-time configuration validation
#if (AXIS_COUNT < 1) || (AXIS_COUNT > 5)
#error "AXIS_COUNT must be between 1 and 5 inclusive"
#endif

#if (AXIS_DIGIT_COUNT <= 0) || (AXIS_DIGIT_COUNT > 7)
#error "AXIS_DIGIT_COUNT must be between 1 and 7 inclusive"
#endif

#if (AXIS_DOT_POSITION < 0) || (AXIS_DOT_POSITION > AXIS_DIGIT_COUNT)
#error "AXIS_DOT_POSITION must be between 0 and AXIS_DIGIT_COUNT inclusive"
#endif

typedef void (*callback_t)();

constexpr int64_t kInt64Max = 9223372036854775807LL;
constexpr int64_t compute_max_abs() {
    int64_t max = 1;
    for (uint8_t i = 0; i < AXIS_DIGIT_COUNT; ++i) {
        if (max > kInt64Max / 10) return kInt64Max;
        max *= 10;
    }
    return max - 1;
}
constexpr int64_t MAX_AXIS_ABS = compute_max_abs();
constexpr int64_t MAX_AXIS = MAX_AXIS_ABS;
constexpr int64_t MIN_AXIS = -MAX_AXIS_ABS;
