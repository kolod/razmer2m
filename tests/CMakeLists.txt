#    This is a part of the Razmer2M project
#    Copyright (C) 2025-... Oleksandr Kolodkin <oleksandr.kolodkin@ukr.net>
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

# If running with GCC
if(NOT CMAKE_CROSSCOMPILING)
    FetchContent_MakeAvailable(googletest)

    # Enforce C++17 for native tests
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Native tests
    add_executable(test_format_native test_format_native.cpp)
    target_link_libraries(test_format_native gtest_main)
    add_test(NAME FormatNativeTest COMMAND test_format_native)
endif()
