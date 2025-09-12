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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

namespace spi {

volatile uint8_t* buffer = nullptr;
volatile uint8_t buffer_pos = 0;
volatile uint8_t buffer_size = 0;
volatile bool transmitting = false;

// Initialize SPI
// Set spi mode 0, MSB first, 125 kHz clock (assuming 16MHz system clock), master mode
inline void init() {
    // Set SPI pins direction for master mode:
    // - PB2 (CS): output, initially high (deselected)
    // - PB3 (MOSI): output
    // - PB4 (MISO): input with pull-up
    // - PB5 (SCK): output
    DDRB |= static_cast<uint8_t>(_BV(PB2) | _BV(PB3) | _BV(PB5));
    DDRB &= static_cast<uint8_t>(~_BV(PB4));
    PORTB |= static_cast<uint8_t>(_BV(PB2) | _BV(PB4));  // CS high, MISO pull-up
    _delay_us(10);

    // Enable SPI, Set as Master, Mode 0 (SPE=1, MSTR=1, CPOL=0, CPHA=0)
    // Prescaler: F_CPU / 128 = 125 kHz (SPR1=1, SPR0=1)
    // Most significant bit first (DORD=0)
    SPCR = static_cast<uint8_t>((_BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0)));
}

// Enable chip select
inline void start() { PORTB &= static_cast<uint8_t>(~_BV(PB2)); }

// Disable chip select
inline void stop() { PORTB |= static_cast<uint8_t>(_BV(PB2)); }

// Check if SPI is busy transmitting data
// Returns true if transmission is ongoing
inline bool is_busy() {
    uint8_t sreg = SREG;
    cli();
    bool busy = transmitting;
    SREG = sreg;
    return busy;
}

// Wait until SPI transmission is complete
inline void wait_until_done() {
    while (is_busy()) {
        _delay_us(10);
    }
}

// Transmit 16-bit data to all connected devices
template <size_t size>
void transmit(volatile uint8_t* data) {
    wait_until_done();

    // Save buffer pointers
    buffer = data;
    buffer_size = size;
    buffer_pos = 0;
    transmitting = true;

    // Enable chip select
    start();

    // Start transmission by sending the first byte
    SPDR = buffer[buffer_pos++];

    // Enable SPI interrupt AFTER starting the first byte transmission
    SPCR |= static_cast<uint8_t>(_BV(SPIE));
}

/**
 * @brief SPI Serial Transfer Complete Interrupt Service Routine.
 *
 * This ISR is triggered when an SPI transfer is complete (SPI_STC_vect).
 * It performs the following actions:
 * - Sets a debug GPIO pin high at the start and low at the end for timing analysis.
 * - Checks if the buffer position has reached the buffer size:
 *   - If equal, calls stop() to terminate the transfer.
 *   - If greater, disables the SPI interrupt by clearing the SPIE bit in SPCR.
 *   - Otherwise, loads the next byte from the buffer into the SPI Data Register (SPDR) and increments the buffer
 * position.
 */
ISR(SPI_STC_vect) {
    gpio::debug<0>(1);

    if (buffer_pos >= buffer_size) {
        stop();
        SPCR &= static_cast<uint8_t>(~_BV(SPIE));
        transmitting = false;
    } else {
        SPDR = buffer[buffer_pos++];
    }

    gpio::debug<0>(0);
}

}  // namespace spi
