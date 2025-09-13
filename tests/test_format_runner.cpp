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

#include <gtest/gtest.h>

#include "qemu_test_runner.h"

class AVRFormatTest : public ::testing::Test {
   protected:
    static QEMUTestRunner* runner;

    // Set up QEMU test environment
    static void SetUpTestSuite() {
        // Instantiate platform-specific runner
        runner = new QEMUTestRunner();

        std::string avr_binary = "../../tests-avr/tests/test_format_avr";

        ASSERT_TRUE(runner->createListeningSockets()) << "Failed to create listening sockets";
        ASSERT_TRUE(runner->startQEMU(avr_binary)) << "Failed to start QEMU";
        ASSERT_TRUE(runner->acceptConnections()) << "Failed to accept QEMU connections";
    }

    // Clean up QEMU test environment
    static void TearDownTestSuite() {
        if (runner) {
            runner->cleanup();
            delete runner;
            runner = nullptr;
        }
    }

    // Format test command
    // FORMAT:<axis_count>,<digit_count>,<dot_pos>,<value1>,<value2>,...
    // <axis_count>: Number of axes (1-4)
    // <digit_count>: Total number of digits (including sign and decimal point)
    // <dot_pos>: Position of the decimal point (0-5)
    // <value1>,<value2>,...: Values to format (up to 4)
    std::string formatTest(int axis_count, int digit_count, int dot_pos, const std::vector<int64_t>& values) {
        std::string cmd =
            "FORMAT:" + std::to_string(axis_count) + "," + std::to_string(digit_count) + "," + std::to_string(dot_pos);

        for (size_t i = 0; i < values.size(); i++) {
            cmd += "," + std::to_string(values[i]);
        }

        std::string response = runner->sendCommand(cmd);

        // Extract result from response
        size_t result_pos = response.find("RESULT:");
        if (result_pos != std::string::npos) {
            return response.substr(result_pos + 7);
        }

        return "";
    }
};

// Static member initialization
QEMUTestRunner* AVRFormatTest::runner = nullptr;

TEST_F(AVRFormatTest, FormatsFourAxisValues) {
    std::vector<int64_t> values = {123456, -123456, 0, 42};
    std::string result = formatTest(4, 6, 4, values);

    ASSERT_FALSE(result.empty()) << "No result received from AVR";
    EXPECT_EQ(result, "  1234.56: -1234.56:  0000.00:  0000.42\n");
}

TEST_F(AVRFormatTest, FormatsNonDefaultTemplateParameters) {
    std::vector<int64_t> values = {1234567, -7654321, 42};
    std::string result = formatTest(3, 7, 4, values);

    ASSERT_FALSE(result.empty()) << "No result received from AVR";
    EXPECT_EQ(result, " 1234.567:-7654.321: 0000.042\n");
}

TEST_F(AVRFormatTest, HandlesZeroValues) {
    std::vector<int64_t> values = {0, 0, 0, 0};
    std::string result = formatTest(4, 6, 4, values);

    ASSERT_FALSE(result.empty()) << "No result received from AVR";
    EXPECT_EQ(result, "  0000.00:  0000.00:  0000.00:  0000.00\n");
}

TEST_F(AVRFormatTest, HandlesMaxNegativeValues) {
    std::vector<int64_t> values = {-999999, -1};
    std::string result = formatTest(2, 6, 4, values);

    ASSERT_FALSE(result.empty()) << "No result received from AVR";
    EXPECT_EQ(result, " -9999.99: -0000.01\n");
}

TEST_F(AVRFormatTest, HandlesMaxPositiveValues) {
    std::vector<int64_t> values = {999999, 1};
    std::string result = formatTest(2, 6, 4, values);

    ASSERT_FALSE(result.empty()) << "No result received from AVR";
    EXPECT_EQ(result, "  9999.99:  0000.01\n");
}
