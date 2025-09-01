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

# Get GoogleTest
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
	GIT_SUBMODULES ""
	UPDATE_DISCONNECTED ON
)

# If cross-compiling for AVR
if(CMAKE_CROSSCOMPILING AND CMAKE_C_COMPILER MATCHES "avr-gcc")
    # AVR test application (for QEMU)
    add_executable(test_format_avr test_format_avr.cpp)
    target_compile_definitions(test_format_avr PRIVATE __AVR_ATmega328P__ F_CPU=16000000UL)
    target_compile_options(test_format_avr PRIVATE -mmcu=atmega328p -Os)
    target_link_options(test_format_avr PRIVATE -mmcu=atmega328p)
endif()

# If running with GCC
if(NOT CMAKE_CROSSCOMPILING AND CMAKE_C_COMPILER MATCHES "gcc")
    FetchContent_MakeAvailable(googletest)

    # Enforce C++17 for native tests
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Native tests
    add_executable(test_format_native test_format_native.cpp)
    target_link_libraries(test_format_native gtest_main)
    add_test(NAME FormatNativeTest COMMAND test_format_native)
    
    # AVR tests under QEMU
    if(WIN32)
        add_executable(qemu_test_runner qemu_test_runner_windows.cpp test_format_runner.cpp)
        target_link_libraries(qemu_test_runner ws2_32 gtest_main)
    else()
        add_executable(qemu_test_runner qemu_test_runner_linux.cpp test_format_runner.cpp)
        target_link_libraries(qemu_test_runner pthread gtest_main)
    endif()

    target_include_directories(qemu_test_runner PRIVATE ".")
    add_test(NAME FormatAVRTest COMMAND test_format_runner)
    set_tests_properties(FormatAVRTest PROPERTIES 
        TIMEOUT 180 # 3 minutes
        LABELS "avr;slow"
    )

    # For faster development, allow skipping slow tests
    if(DEFINED ENV{SKIP_SLOW_TESTS})
        set_tests_properties(FormatAVRTest PROPERTIES DISABLED TRUE)
    endif()
endif()
