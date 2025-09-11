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
#include <assert.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "config.h"
#include "gpio.h"
#include "spi.h"

namespace display {

// Only receiver has display
#if defined(RECEIVER) || defined(EMULATOR)

// Buffer for display content
// Because screen updating is done column by column
// addressing is done by column first
uint16_t buffer[8][AXIS_COUNT];

// Transmission buffer for SPI
volatile uint8_t tx_buffer[AXIS_COUNT * 2];

// Convert ASCII character to segment representation
uint8_t segment_from_ascii(char c) {
    switch (c) {
        case '0':
            return 0x7E;  // 0
        case '1':
            return 0x30;  // 1
        case '2':
            return 0x6D;  // 2
        case '3':
            return 0x79;  // 3
        case '4':
            return 0x33;  // 4
        case '5':
            return 0x5B;  // 5
        case '6':
            return 0x5F;  // 6
        case '7':
            return 0x70;  // 7
        case '8':
            return 0x7F;  // 8
        case '9':
            return 0x7B;  // 9
        case 'a':
        case 'A':
            return 0x77;  // A
        case 'b':
        case 'B':
            return 0x1F;  // B
        case 'c':
        case 'C':
            return 0x4E;  // C
        case 'd':
        case 'D':
            return 0x3D;  // D
        case 'e':
        case 'E':
            return 0x4F;  // E
        case 'f':
        case 'F':
            return 0x47;  // F
        case 'r':
        case 'R':
            return 0x05;  // R
        case 'o':
        case 'O':
            return 0x1D;  // O
        case '-':
            return 0x01;  // Minus
        case '.':
            return 0x80;  // Dot
        case ',':
            return 0x80;  // Alternate dot
        default:
            return 0x00;  // Blank for unsupported characters and spaces
    }
}

// Clear transmission buffer
inline void clear_tx_buffer() {
    for (uint8_t i = AXIS_COUNT * 2; i-- > 0;) tx_buffer[i] = 0;
}

// Send identical command to all connected devices
void send_command(uint8_t addr, uint8_t data) {
    //_delay_us(10);  // Small delay to allow the display to process the command
    clear_tx_buffer();
    for (uint8_t i = 0; i < AXIS_COUNT; ++i) {
        tx_buffer[i * 2] = addr;
        tx_buffer[i * 2 + 1] = data;
    }
    spi::transmit<sizeof(tx_buffer)>(tx_buffer);
}

// Send command to a specific device
// To all other device we must send no-op command (0x00)
void send_command(uint8_t addr, uint8_t data, uint8_t device) {
    //_delay_us(10);  // Small delay to allow the display to process the command
    clear_tx_buffer();
    tx_buffer[device * 2] = addr;
    tx_buffer[device * 2 + 1] = data;
    spi::transmit<sizeof(tx_buffer)>(tx_buffer);
}

inline void init() {
    // Initialize SPI
    spi::init();

    // Initialize MAX7219
    send_command(0x09, 0x00);  // Decode mode: BCD
    send_command(0x0A, 0x0F);  // Intensity: 0 (min)
    send_command(0x0B, 0x0B);  // Scan limit: all 8 digits
    send_command(0x0C, 0x0C);  // Shutdown register: normal operation
}

// Update display content
void update() {
    for (uint8_t col = 0; col < 8; ++col) {
        // Prepare buffer for this column across all devices
        for (uint8_t i = 0; i < AXIS_COUNT; ++i) {
            tx_buffer[i * 2] = col + 1;             // Address (1-8)
            tx_buffer[i * 2 + 1] = buffer[col][i];  // Data
        }
        spi::transmit<sizeof(tx_buffer)>(tx_buffer);
    }
}

void write(const char* string) {
    uint8_t column = 0;
    uint8_t row = 0;

    while (*string) {
        // Ignore ':'
        if (*string == ':') {
            ++string;
            continue;
        }

        // Convert each character to segments and store in buffer
        buffer[column][row] = segment_from_ascii(*string++);

        // If next char is dot
        if (*string == '.') {
            buffer[column][row] |= 0x80;  // Set the dot bit
            ++string;
        }

        // Move to next column
        if (++column < 7) continue;
        column = 0;

        // Next row
        row++;

        // Assert on row overflow
        assert(row < AXIS_COUNT);
    }

    update();
}

#endif

}  // namespace display
