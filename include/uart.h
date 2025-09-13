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
#include <avr/interrupt.h>
#include <avr/io.h>

#include "config.h"

namespace uart {

// Max digits per axis + separators + null terminator
constexpr size_t BUFFER_SIZE = AXIS_COUNT * 10 + 1;

// Define UART divider
constexpr uint32_t DIVIDER = static_cast<uint32_t>(F_CPU / 16 / BAUDRATE - 1);

namespace transmitter {

// Zero-terminated buffer for UART transmission
//  - on emulator and transmitter mode, this buffer holds the data to be transmitted
//  - on receiver mode, this buffer holds the received data
volatile char tx_buffer[BUFFER_SIZE];

// Pointer to the current position in the buffer
volatile char* buffer_ptr;

// Setup UART
void init() {
    // Initialize buffer pointer
    buffer_ptr = nullptr;
    // Set baud rate
    UBRR0 = DIVIDER;
    // Enable transmitter only
    UCSR0B |= static_cast<uint8_t>(1 << TXEN0);
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = static_cast<uint8_t>((1 << UCSZ00) | (1 << UCSZ01));
}

ISR(USART_UDRE_vect) {
    // Transmit next byte
    UDR0 = static_cast<uint8_t>(*buffer_ptr++);
    // If no more bytes to transmit, disable the data register empty interrupt and transmitter
    if (*buffer_ptr == '\0') UCSR0B &= static_cast<uint8_t>(~_BV(UDRIE0));
}

// Start buffer transmit
//  - copy data to internal buffer
//  - enable transmitter and data register empty interrupt
void transmit(char* buf) {
    // Ensure the buffer is not null
    if (buf == nullptr) return;

    // If buffer is empty, do nothing
    if (*buf == '\0') return;

    // Copy data to buffer with zero-termination
    for (size_t i = 0; i < sizeof(tx_buffer); i++) {
        tx_buffer[i] = *buf++;
        if (tx_buffer[i] == '\0') break;
    }

    // Initialize buffer pointer
    buffer_ptr = tx_buffer;

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

}  // namespace transmitter

namespace receiver {

// Zero-terminated buffer for UART reception
//  - on receiver mode, this buffer holds the received data
//  - double buffer to allow processing one buffer while receiving into another
volatile char buffer[2][BUFFER_SIZE];
volatile uint8_t current_buffer = 0;

// Flag indicating if a complete message has been received
volatile bool received_complete = false;

// Pointer to the current position in the buffer
volatile uint8_t buffer_pos;

// Setup UART
void init() {
    // Initialize buffer pointer
    current_buffer = 0;
    buffer_pos = 0;
    // Set baud rate
    UBRR0 = DIVIDER;
    // Enable receiver only & receive complete interrupt
    UCSR0B |= static_cast<uint8_t>(_BV(RXEN0) | _BV(RXCIE0));
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = static_cast<uint8_t>((1 << UCSZ00) | (1 << UCSZ01));
}

// USART Receive Complete interrupt
ISR(USART_RX_vect) {
    // Receive next byte
    char received = static_cast<char>(UDR0);

    // Check if buffer is not full
    if (buffer_pos < (BUFFER_SIZE - 1)) {
        // Store received byte in buffer
        buffer[current_buffer][buffer_pos++] = received;
        return;
    }

    // If buffer is newline received, finalize the message
    if (received == '\n') {
        buffer[current_buffer][buffer_pos] = '\0';  // Null-terminate the string
        current_buffer ^= 1;                        // Switch buffers
        buffer_pos = 0;                             // Reset buffer position
        received_complete = true;                   // Mark that a complete message has been received
    }
}

// Get the last received complete message
//  - returns nullptr if no complete message is available
//  - returns pointer to the message buffer otherwise
//  - returned pointer is valid until next call to get_message()
//  - resets the received_complete flag
inline char* get_message() {
    char* msg = nullptr;
    uint8_t sreg = SREG;
    cli();
    if (received_complete) {
        received_complete = false;
        msg = const_cast<char*>(buffer[current_buffer ^ 1]);
    }
    SREG = sreg;
    return msg;
}

}  // namespace receiver

}  // namespace uart
