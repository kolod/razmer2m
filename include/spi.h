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

namespace spi {

void init() {
    // Set MOSI and SCK as Output
    // DDRB |= _BV(PB3) | _BV(PB5);
    // Enable SPI, Set as Master
    SPCR |= _BV(SPE) | _BV(MSTR);
    // Set SPI Clock Rate
    SPSR |= _BV(SPI2X);
}

// Transmit data as address and data
inline void transmit(uint8_t addr, uint8_t data) {
    loop_until_bit_is_set(SPSR, SPIF);  // Wait for transmission complete
    SPDR = addr;                        // Transmit address
    loop_until_bit_is_set(SPSR, SPIF);  // Wait for transmission complete
    SPDR = data;                        // Transmit data
}

// Transmit address and data as uint16_t
inline void transmit(uint16_t data) { transmit(data >> 8, data & 0xFF); }

// Enable chip select
inline void start() { PORTB &= static_cast<uint8_t>(~_BV(PB2)); }

// Disable chip select
inline void stop() { PORTB |= static_cast<uint8_t>(_BV(PB2)); }

// Transmit multiple 16-bit words
void transmit(uint16_t data[], uint8_t count) {
    start();
    for (uint8_t i = 0; i < count; ++i) transmit(data[i]);
    stop();
}

// Transmit a identical 16-bit word to multiple devices
void transmit(uint16_t data, uint8_t count) {
    start();
    for (uint8_t i = 0; i < count; ++i) transmit(data);
    stop();
}

// Transmit data as address and data to multiple devices
void transmit(uint8_t addr, uint8_t data[], uint8_t count) {
    start();
    for (uint8_t i = 0; i < count; ++i) transmit(addr, data[i]);
    stop();
}

}  // namespace spi
