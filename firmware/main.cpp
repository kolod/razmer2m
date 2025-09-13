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

#if defined(EMULATOR)
#include "emulator.h"
#elif defined(TRANSMITTER)
#include "transmitter.h"
#elif defined(RECEIVER)
#include "receiver.h"
#else
#error "EMULATOR, TRANSMITTER, or RECEIVER must be defined"
#endif

extern "C" int main() {
    sei();  // Enable global interrupts
    MODULE::init();
    while (true) {
        MODULE::update();
    }
}