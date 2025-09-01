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

#include "config.h"

namespace uart {

// Zero-terminated buffer for UART transmission
//  - on emulator and transmitter mode, this buffer holds the data to be transmitted
//  - on receiver mode, this buffer holds the received data
char buffer[AXIS_COUNT * 10];

// Pointer to the current position in the buffer
char* buffer_ptr;

// Setup UART
void init() {
    // Set baud rate
    uint16_t ubrr = F_CPU / 16 / BAUDRATE - 1;
    UBRR0 = static_cast<uint16_t>(ubrr);  // Set baud rate

#if defined(EMULATOR) || defined(TRANSMITTER)
    // Enable transmitter only
    UCSR0B |= static_cast<uint8_t>(1 << TXEN0);
    // Enable data register empty interrupt
    UCSR0B |= static_cast<uint8_t>(1 << UDRIE0);
#elif defined(RECEIVER)
    // Initialize buffer pointer
    buffer_ptr = buffer;
    // Enable receiver only
    UCSR0B |= static_cast<uint8_t>(1 << RXEN0);
    // Enable receive complete interrupt
    UCSR0B |= static_cast<uint8_t>(1 << RXCIE0);
#endif

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = static_cast<uint8_t>((1 << UCSZ00) | (1 << UCSZ01));
}

// USART Data Register Empty interrupt
// Only for emulator and transmitter mode
#if defined(EMULATOR) || defined(TRANSMITTER)

ISR(USART_UDRE_vect) {
    gpio::set_debug_0(1);

    // Transmit next byte
    UDR0 = static_cast<uint8_t>(*buffer_ptr++);

    // Check if there are more bytes to transmit
    if (*buffer_ptr == '\0') {
        // Disable data register empty interrupt
        UCSR0B &= static_cast<uint8_t>(~_BV(UDRIE0));
    }

    gpio::set_debug_0(0);
}

#elif defined(RECEIVER)

// USART Receive Complete interrupt
ISR(USART_RX_vect) {
    // Receive next byte
    char received = static_cast<char>(UDR0);

    // Check if buffer is not full
    if (buffer_ptr < &buffer[sizeof(buffer)]) {
        // Store received byte in buffer
        *buffer_ptr++ = received;
        return;
    }

    // if received end of packet reset buffer pointer
    if (received == '\n') buffer_ptr = buffer;
}

#endif

// Start buffer transmit
//  - put first 2 bytes into the transmit register
//  - enable transmit register empty interrupt if more then 2 bytes in buffer
void transmit(char* buf) {
    // Initialize buffer pointer
    buffer_ptr = buf;

    // Transmit first byte
    if (*buffer_ptr == '\0') return;             // Ensure the buffer is not empty
    loop_until_bit_is_set(UCSR0A, UDRE0);        // Wait for transmit register to be empty
    UDR0 = static_cast<uint8_t>(*buffer_ptr++);  // Load first byte into transmit register

    // Transmit second byte
    if (*buffer_ptr == '\0') return;             // Ensure the buffer is not empty
    loop_until_bit_is_set(UCSR0A, UDRE0);        // Wait for transmit register to be empty
    UDR0 = static_cast<uint8_t>(*buffer_ptr++);  // Load second byte into transmit register

    // Transmit other bytes using interrupt
    if (*buffer_ptr == '\0') return;              // Ensure the buffer is not empty
    UCSR0B |= static_cast<uint8_t>(_BV(UDRIE0));  // Enable data register empty interrupt
}

}  // namespace uart
