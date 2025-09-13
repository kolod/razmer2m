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

#include <util/delay.h>

#include "config.h"
#include "display.h"
#include "format.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"

#define MODULE receiver

namespace receiver {

const char* const test_msg[] = {
    "8.        :8.        :8.        :8.        \n",  //
    " 8.       : 8.       : 8.       : 8.       \n",  //
    "  8.      :  8.      :  8.      :  8.      \n",  //
    "   8.     :   8.     :   8.     :   8.     \n",  //
    "    8.    :    8.    :    8.    :    8.    \n",  //
    "     8.   :     8.   :     8.   :     8.   \n",  //
    "      8.  :      8.  :      8.  :      8.  \n",  //
    "       8. :       8. :       8. :       8. \n",  //
    "        8.:        8.:        8.:        8.\n"   //
};

// Draw running dashes to indicate waiting for data
inline void check_display_mode() {
    for (uint8_t column = 0; column < 8; column++) {
        display::write(test_msg[column]);
        while (display::update());
        _delay_ms(100);
    }
}

void init() {
    gpio::init();
    display::init();
    uart::receiver::init();
    check_display_mode();
    display::clear();
}

void update() {
    // Check if a complete message has been received
    auto msg = uart::receiver::get_message();
    if (msg != nullptr) {
        // Start display update
        display::write(msg);
        while (display::update());
    }
}

}  // namespace receiver
