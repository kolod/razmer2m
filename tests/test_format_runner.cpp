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

#include <gtest/gtest.h>
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

class QEMUTestRunner {
   private:
    SOCKET sock;
    HANDLE qemu_process;
    bool connected;

   public:
    QEMUTestRunner() : sock(INVALID_SOCKET), qemu_process(nullptr), connected(false) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~QEMUTestRunner() {
        cleanup();
        WSACleanup();
    }

    bool startQEMU() {
        // Start QEMU with the AVR test
        std::string cmd =
            "qemu-system-avr -machine arduino-uno -bios ../build/tests/test_format_avr.elf "
            "-nographic -serial telnet:localhost:1234,server,nowait";

        STARTUPINFOA si = {0};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi = {0};

        if (!CreateProcessA(nullptr, const_cast<char*>(cmd.c_str()), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr,
                            nullptr, &si, &pi)) {
            std::cerr << "Failed to start QEMU" << std::endl;
            return false;
        }

        qemu_process = pi.hProcess;
        CloseHandle(pi.hThread);

        // Wait for QEMU to start
        std::this_thread::sleep_for(std::chrono::seconds(3));

        return true;
    }

    bool connectToQEMU() {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            return false;
        }

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        // Try to connect with retries
        for (int i = 0; i < 10; i++) {
            if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
                connected = true;

                // Wait for ready message
                std::string response = receiveResponse(5000);
                if (response.find("AVR_TEST_READY") != std::string::npos) {
                    return true;
                }
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        return false;
    }

    std::string sendCommand(const std::string& command) {
        if (!connected) return "";

        std::string cmd_with_newline = command + "\n";
        send(sock, cmd_with_newline.c_str(), cmd_with_newline.length(), 0);

        return receiveResponse(2000);
    }

    std::string receiveResponse(int timeout_ms = 1000) {
        if (!connected) return "";

        // Set socket to non-blocking
        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);

        std::string response;
        auto start_time = std::chrono::steady_clock::now();

        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time)
                   .count() < timeout_ms) {
            char buffer[1024];
            int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

            if (bytes > 0) {
                buffer[bytes] = '\0';
                response += buffer;

                // Check if we have a complete response
                if (response.find('\n') != std::string::npos) {
                    break;
                }
            } else if (bytes == 0) {
                // Connection closed
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Set socket back to blocking
        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);

        return response;
    }

    void cleanup() {
        if (connected && sock != INVALID_SOCKET) {
            closesocket(sock);
            connected = false;
        }

        if (qemu_process) {
            TerminateProcess(qemu_process, 0);
            CloseHandle(qemu_process);
            qemu_process = nullptr;
        }
    }
};

class AVRFormatTest : public ::testing::Test {
   protected:
    static QEMUTestRunner* runner;

    static void SetUpTestSuite() {
        runner = new QEMUTestRunner();

        ASSERT_TRUE(runner->startQEMU()) << "Failed to start QEMU";
        ASSERT_TRUE(runner->connectToQEMU()) << "Failed to connect to QEMU";

        // Test connection with ping
        std::string response = runner->sendCommand("PING");
        ASSERT_TRUE(response.find("PONG") != std::string::npos) << "QEMU connection test failed";
    }

    static void TearDownTestSuite() {
        if (runner) {
            runner->cleanup();
            delete runner;
            runner = nullptr;
        }
    }

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
