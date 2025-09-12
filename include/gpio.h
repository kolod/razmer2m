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
#include <avr/io.h>

namespace gpio {

// GPIO Configuration:
//   System:
//     - PC6: Reset
//   Serial:
//     - PD0: Serial RX
//     - PD1: Serial TX
//   Display (used only on receiver):
//     - PB2: Display CS
//     - PB3: Display MOSI
//     - PB4: Display MISO
//     - PB5: Display SCK (Conflict with built-in LED)
//   Razmer2M (inputs for normal operation output on emulator, not used on receiver):
//     - PD2: W1
//     - PD3: W2
//     - PD4: W4
//     - PD5: W8
//     - PD6: ER
//     - PD7: A7
//     - PC0: B0
//     - PC1: B1
//     - PC2: B2
//     - PC3: B3
//     - PC4: B4
//     - PC5: B5
//   Debug:
//     - PB0: Debug 0
//     - PB1: Debug 1
//
//   Pin direction
//     for all:
//       - PD0..PD1: controlled by USART
//       - PB2..PB5: controlled by SPI
//       - PC6: reset by hardware
//       - PB0..PB1: output
//     for emulator:
//       - PD2..PD7: output
//       - PC0..PC5: output
//     for transmitter:
//       - PD2..PD7: input
//       - PC0..PC5: input
//     for receiver:
//       - PD2..PD7: not used
//       - PC0..PC5: not used

// Initialize GPIO pins based on mode
void init() {
    // Set debug pins as output
    DDRB = (1 << PB0) | (1 << PB1);

#ifdef EMULATOR
    // Set PD2..PD7 as output
    DDRD = (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
    // Set PC0..PC5 as output
    DDRC = (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
#endif
}

// Set debug pin 0
template <uint8_t pin>
inline void debug(uint8_t value) {
    if (value) {
        PORTB |= static_cast<uint8_t>(_BV(pin));
    } else {
        PORTB &= static_cast<uint8_t>(~_BV(pin));
    }
}

// Toggle debug pin 0
void toggle_debug_0() { PORTB ^= static_cast<uint8_t>(_BV(PB0)); }

// Toggle debug pin 1
void toggle_debug_1() { PORTB ^= static_cast<uint8_t>(_BV(PB1)); }

}  // namespace gpio
