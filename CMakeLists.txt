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

cmake_minimum_required(VERSION 3.31)
project(razmer2m 
    VERSION 1.0 
    DESCRIPTION "A hardware and software system for replacing a cathode ray tube indicator with an LED display for machine tools equipped with a numerical control device (NCU) \"Razmer 2M 1104\"."
    LANGUAGES C CXX
)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_FIRMWARE "Build firmware" ON)

# Add cmake/ directory to CMAKE_MODULE_PATH for custom modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Add include directory for headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# User-configurable options for display parameters
set(AXIS_COUNT 4 CACHE STRING "Number of axes to display")
set(AXIS_DIGIT_COUNT 6 CACHE STRING "Total number of digits per axis (including decimal)")
set(AXIS_DOT_POSITION 4 CACHE STRING "Position of decimal point from left (0-based)")

# Add the firmware directory
if(BUILD_FIRMWARE)
    add_subdirectory(firmware)
else()
    message(STATUS "Firmware build is disabled. Skipping firmware directory.")
endif()

# Add the tests directory
if(BUILD_TESTS)
    enable_testing()
	add_subdirectory(tests)
else()
    message(STATUS "Tests build is disabled. Skipping tests directory.")
endif()
