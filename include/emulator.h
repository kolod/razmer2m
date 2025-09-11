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

#include "config.h"
#include "display.h"
#include "format.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

#define MODULE emulator

namespace emulator {

enum class algorithm_t : uint8_t { RANDOM, INCREMENTING, DECREMENTING, COUNT };

// Current algorithm
algorithm_t algorithm = algorithm_t::RANDOM;
volatile uint16_t frame_counter = 0;
volatile uint8_t sub_cycle_counter = 0;
char* volatile msg = nullptr;

// Axis values
int64_t axis[AXIS_COUNT] = {0};
int64_t next_axis[AXIS_COUNT] = {0};

// Boolean flags
volatile bool axis_updated = false;
volatile bool axis_ready = false;

// Next algorithm
void next_algorithm() {
    uint8_t next_algorithm = static_cast<uint8_t>(algorithm) + 1;
    if (next_algorithm >= static_cast<uint8_t>(algorithm_t::COUNT)) next_algorithm = 0;
    algorithm = static_cast<algorithm_t>(next_algorithm);
}

// Function must be called 50 times per second in interrupt routine
void update_50_hz() {
    gpio::set_debug_1(1);

    // Emulate sending message from transmitter
    uart::transmit(msg);

    // Update axis values
    if (axis_ready) {
        for (uint8_t i = 0; i < AXIS_COUNT; ++i) {
            axis[i] = next_axis[i];
        }
    }

    // Mark axis as updated
    axis_updated = true;

    // Increment frame_counter
    frame_counter++;

    gpio::set_debug_1(0);
}

// Function must be called 2000 times per second in interrupt routine
void update_2000_hz() {
    if (++sub_cycle_counter >= 5 * 8) {
        sub_cycle_counter = 0;
        update_50_hz();
    }
}

int64_t random_axis() {
    // Combine multiple rand() calls to get better distribution
    // rand() on AVR returns 0-32767, so we need multiple calls for larger ranges
    uint64_t r = 0;
    for (uint8_t i = 0; i < 8; i++) {  // 8 calls gives us 64 bits of randomness
        r = (r << 8) | (rand() & 0xFF);
    }

    // Scale to our desired range
    uint64_t range = static_cast<uint64_t>(MAX_AXIS_ABS);
    int64_t v = static_cast<int64_t>(r % range);

    if (rand() & 0x01) v = -v;

    return v;
}

// Slow function, must be called in main loop
void update() {
    // When next_axis copied to axis we can prepare next_axis
    if (axis_updated) {
        switch (algorithm) {
            // Random algorithm
            case algorithm_t::RANDOM:
                for (uint8_t i = 0; i < AXIS_COUNT; ++i) next_axis[i] = random_axis();
                break;

            // Incrementing algorithm
            case algorithm_t::INCREMENTING:
                for (uint8_t i = 0; i < AXIS_COUNT; ++i) next_axis[i] = (axis[i] < MAX_AXIS) ? axis[i] + 1 : MAX_AXIS;
                axis_ready = true;
                break;

            // Decrementing algorithm
            case algorithm_t::DECREMENTING:
                for (uint8_t i = 0; i < AXIS_COUNT; ++i) next_axis[i] = (axis[i] > MIN_AXIS) ? axis[i] - 1 : MIN_AXIS;
                axis_ready = true;
                break;

            default:
                break;
        }

        // Prepare the message for transmission
        gpio::set_debug_0(1);
        msg = format(next_axis);
        gpio::set_debug_0(0);

        // Reset axis_updated flag
        axis_updated = false;
    }

    // Put message directly on display
    // display::write(msg);

    // Change algorithm every 5 seconds
    if (frame_counter >= 50 * 5) {
        frame_counter = 0;
        axis_ready = true;
        next_algorithm();
    }
}

inline void init() {
    gpio::init();
    uart::init();
    spi::init();
    // display::init();
    timer::init(update_2000_hz);

    // Enable interrupts
    sei();
}

}  // namespace emulator