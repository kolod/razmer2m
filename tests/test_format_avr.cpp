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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "config.h"
#include "format.h"

// UART configuration for ATmega328P
#define BAUD 9600
#define UBRR_VAL ((F_CPU / 16 / BAUD) - 1)

// Command buffer
#define CMD_BUFFER_SIZE 64
static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

// Initialize UART
void uart_init() {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)UBRR_VAL;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Send character via UART
void uart_putchar(char c) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Send data
    UDR0 = c;
}

// Receive character from UART (non-blocking)
int uart_getchar() {
    if (UCSR0A & (1 << RXC0)) {
        return UDR0;
    }
    return -1;
}

// Send string via UART
void uart_puts(const char* str) {
    while (*str) {
        uart_putchar(*str++);
    }
}

// Parse comma-separated values into array
int parse_values(const char* str, int64_t* values, size_t max_values) {
    size_t count = 0;
    const char* ptr = str;

    while (*ptr && count < max_values) {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t') ptr++;

        // Parse number
        int64_t value = 0;
        int negative = 0;

        if (*ptr == '-') {
            negative = 1;
            ptr++;
        } else if (*ptr == '+') {
            ptr++;
        }

        while (*ptr >= '0' && *ptr <= '9') {
            value = value * 10 + (*ptr - '0');
            ptr++;
        }

        if (negative) value = -value;
        values[count++] = value;

        // Skip to next comma or end
        while (*ptr && *ptr != ',') ptr++;
        if (*ptr == ',') ptr++;
    }

    return count;
}

// Process command
void process_command(const char* cmd) {
    if (strncmp(cmd, "FORMAT:", 7) == 0) {
        // Parse axis count, digit count, dot position, and values
        const char* params = cmd + 7;

        // Expected format: "FORMAT:axis_count,digit_count,dot_pos,val1,val2,..."
        int axis_count, digit_count, dot_pos;
        int64_t values[8];  // Max 8 axes

        // Parse parameters
        const char* ptr = params;
        axis_count = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            axis_count = axis_count * 10 + (*ptr - '0');
            ptr++;
        }
        if (*ptr == ',') ptr++;

        digit_count = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            digit_count = digit_count * 10 + (*ptr - '0');
            ptr++;
        }
        if (*ptr == ',') ptr++;

        dot_pos = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            dot_pos = dot_pos * 10 + (*ptr - '0');
            ptr++;
        }
        if (*ptr == ',') ptr++;

        // Parse values
        int value_count = parse_values(ptr, values, axis_count);

        if (value_count >= axis_count) {
            // Call format function based on parameters
            char* result = nullptr;

            // Handle different template parameter combinations
            if (axis_count == 4 && digit_count == 6 && dot_pos == 4) {
                result = format<4, 6, 4>(reinterpret_cast<const int64_t (&)[4]>(values));
            } else if (axis_count == 3 && digit_count == 7 && dot_pos == 4) {
                result = format<3, 7, 4>(reinterpret_cast<const int64_t (&)[3]>(values));
            } else if (axis_count == 2 && digit_count == 6 && dot_pos == 4) {
                result = format<2, 6, 4>(reinterpret_cast<const int64_t (&)[2]>(values));
            } else {
                // Default case
                result = format(reinterpret_cast<const int64_t (&)[4]>(values));
            }

            if (result) {
                uart_puts("RESULT:");
                uart_puts(result);
            } else {
                uart_puts("ERROR:Format failed\n");
            }
        } else {
            uart_puts("ERROR:Invalid parameter count\n");
        }
    } else if (strcmp(cmd, "PING") == 0) {
        uart_puts("PONG\n");
    } else if (strcmp(cmd, "READY") == 0) {
        uart_puts("AVR_TEST_READY\n");
    } else {
        uart_puts("ERROR:Unknown command\n");
    }
}

int main() {
    // Initialize UART
    uart_init();

    // Send ready message
    uart_puts("AVR_TEST_READY\n");

    // Main loop
    while (1) {
        int c = uart_getchar();

        if (c >= 0) {
            char ch = (char)c;

            if (ch == '\n' || ch == '\r') {
                // End of command
                if (cmd_index > 0) {
                    cmd_buffer[cmd_index] = '\0';
                    process_command(cmd_buffer);
                    cmd_index = 0;
                }
            } else if (cmd_index < CMD_BUFFER_SIZE - 1) {
                // Add character to buffer
                cmd_buffer[cmd_index++] = ch;
            } else {
                // Buffer overflow
                uart_puts("ERROR:Command too long\n");
                cmd_index = 0;
            }
        }

        _delay_ms(1);
    }

    return 0;
}
