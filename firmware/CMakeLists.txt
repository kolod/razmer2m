#    This is a part of the Razmer2M project
#    Copyright (C) 2025-... Oleksandr kolodkin <oleksandr.kolodkin@ukr.net>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

include(Platform/Generic-ELF)
include(CMakePrintHelpers)

option(BUILD_EMULATOR "Build emulator" ON)
option(BUILD_TRANSMITTER "Build transmitter" ON)
option(BUILD_RECEIVER "Build receiver" ON)

find_package(AVRDUDE)

# AVR specific compile definitions
add_compile_definitions(
    __AVR_ATmega328P__
    F_CPU=16000000UL
)

add_compile_options(-mmcu=atmega328p) # Set the target MCU
add_link_options(-mmcu=atmega328p)    # Set the target CPU

# Emulator target
if(BUILD_EMULATOR)
    add_executable(${PROJECT_NAME}_emulator main.cpp)
    set(_emulator_defs EMULATOR)
    if(AXIS_COUNT)
        list(APPEND _emulator_defs AXIS_COUNT=${AXIS_COUNT})
    endif()
    if(AXIS_DIGIT_COUNT)
        list(APPEND _emulator_defs AXIS_DIGIT_COUNT=${AXIS_DIGIT_COUNT})
    endif()
    if(AXIS_DOT_POSITION)
        list(APPEND _emulator_defs AXIS_DOT_POSITION=${AXIS_DOT_POSITION})
    endif()
    target_compile_definitions(${PROJECT_NAME}_emulator PRIVATE ${_emulator_defs})
    target_add_size(${PROJECT_NAME}_emulator)
    target_create_hex(${PROJECT_NAME}_emulator)
    target_create_binary(${PROJECT_NAME}_emulator)
    target_create_listing(${PROJECT_NAME}_emulator)
    avrdude_upload(upload_${PROJECT_NAME}_emulator
        TARGET ${PROJECT_NAME}_emulator
        MCU atmega328p
        PROGRAMMER arduino
        PORT COM6
        BAUDRATE 115200
        VERBOSE
        REPORT_FILE upload_report.txt
    )
endif()

# Transmitter target
if(BUILD_TRANSMITTER)
    add_executable(${PROJECT_NAME}_transmitter main.cpp)
    set(_transmitter_defs TRANSMITTER)
    if(AXIS_COUNT)
        list(APPEND _transmitter_defs AXIS_COUNT=${AXIS_COUNT})
    endif()
    if(AXIS_DIGIT_COUNT)
        list(APPEND _transmitter_defs AXIS_DIGIT_COUNT=${AXIS_DIGIT_COUNT})
    endif()
    if(AXIS_DOT_POSITION)
        list(APPEND _transmitter_defs AXIS_DOT_POSITION=${AXIS_DOT_POSITION})
    endif()
    target_compile_definitions(${PROJECT_NAME}_transmitter PRIVATE ${_transmitter_defs})
    target_add_size(${PROJECT_NAME}_transmitter)
    target_create_hex(${PROJECT_NAME}_transmitter)
    target_create_binary(${PROJECT_NAME}_transmitter)
    target_create_listing(${PROJECT_NAME}_transmitter)
    avrdude_upload("upload_${PROJECT_NAME}_transmitter"
        TARGET ${PROJECT_NAME}_transmitter
        MCU atmega328p
        PROGRAMMER arduino
        PORT /dev/ttyUSB0
        BAUDRATE 115200
        LFUSE 0xFF
        HFUSE 0xD9
        EFUSE 0xFF
        VERBOSE
        REPORT_FILE upload_report.txt
    )
endif()

# Receiver target
if(BUILD_RECEIVER)
    add_executable(${PROJECT_NAME}_receiver main.cpp)
    set(_receiver_defs RECEIVER)
    if(AXIS_COUNT)
        list(APPEND _receiver_defs AXIS_COUNT=${AXIS_COUNT})
    endif()
    if(AXIS_DIGIT_COUNT)
        list(APPEND _receiver_defs AXIS_DIGIT_COUNT=${AXIS_DIGIT_COUNT})
    endif()
    if(AXIS_DOT_POSITION)
        list(APPEND _receiver_defs AXIS_DOT_POSITION=${AXIS_DOT_POSITION})
    endif()
    target_compile_definitions(${PROJECT_NAME}_receiver PRIVATE ${_receiver_defs})
    target_add_size(${PROJECT_NAME}_receiver)
    target_create_hex(${PROJECT_NAME}_receiver)
    target_create_binary(${PROJECT_NAME}_receiver)
    target_create_listing(${PROJECT_NAME}_receiver)
    avrdude_upload(upload_${PROJECT_NAME}_receiver
        TARGET ${PROJECT_NAME}_receiver
        MCU atmega328p
        PROGRAMMER arduino
        PORT /dev/ttyUSB0
        BAUDRATE 115200
        LFUSE 0xFF
        HFUSE 0xD9
        EFUSE 0xFF
        VERBOSE
        REPORT_FILE upload_report.txt
    )
endif()
