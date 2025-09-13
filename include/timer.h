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

namespace timer {

callback_t callback = nullptr;

inline void init(callback_t _callback) {
    callback = _callback;

    // Reset
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;

    OCR0A = 124;                         // Set compare value for 2000 Hz
    TCCR0A = (1 << WGM01);               // Set CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00);  // Set prescaler to 64
    TIMSK0 = (1 << OCIE0A);              // Enable Timer0 compare interrupt
}

// Timer0 interrupt handler
ISR(TIMER0_COMPA_vect) {
    if (callback != nullptr) callback();
}

}  // namespace timer
