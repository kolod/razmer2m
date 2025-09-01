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

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Native tests
add_executable(test_format_native test_format_native.cpp)
target_link_libraries(test_format_native gtest_main)
add_test(NAME FormatNativeTest COMMAND test_format_native)

# AVR test application (for QEMU)
if(CMAKE_CROSSCOMPILING AND CMAKE_C_COMPILER MATCHES "avr-gcc")
    add_executable(test_format_avr test_format_avr.cpp)
    target_compile_definitions(test_format_avr PRIVATE __AVR_ATmega328P__ F_CPU=16000000UL)
    target_compile_options(test_format_avr PRIVATE -mmcu=atmega328p)
    target_link_options(test_format_avr PRIVATE -mmcu=atmega328p)
endif()

# Test runner (runs AVR tests in QEMU)
if(NOT CMAKE_CROSSCOMPILING)
    add_executable(test_format_runner test_format_runner.cpp)
    target_link_libraries(test_format_runner gtest_main)
    if(WIN32)
        target_link_libraries(test_format_runner ws2_32)
    endif()
    add_test(NAME FormatAVRTest COMMAND test_format_runner)
endif()
