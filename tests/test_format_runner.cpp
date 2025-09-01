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

#ifdef _WIN32
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#endif

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

class QEMUTestRunner {
   private:
#ifdef _WIN32
    SOCKET sock;
    HANDLE qemu_process;
#else
    int sock;
    pid_t qemu_process;
#endif
    bool connected;

   public:
    QEMUTestRunner() : sock(-1), qemu_process(0), connected(false) {
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        sock = INVALID_SOCKET;
        qemu_process = nullptr;
#endif
    }

    ~QEMUTestRunner() {
        cleanup();
#ifdef _WIN32
        WSACleanup();
#endif
    }

    bool startQEMU() {
        // Print ${CWD}
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

        // AVR binary file
        std::string avr_binary = "../../tests-avr/tests/test_format_avr";

        // Check if the AVR binary file exists
        if (!std::filesystem::exists(avr_binary)) {
            std::cerr << "AVR binary file not found: " << avr_binary << std::endl;
            return false;
        }

        // Start QEMU with the AVR test
        std::string cmd = "qemu-system-avr -machine arduino-uno -bios " + avr_binary +
                          " -nographic -no-shutdown -no-reboot -serial tcp:localhost:1234,server,nowait";

        std::cout << "Starting QEMU with command: " << cmd << std::endl;

#ifdef _WIN32
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
#else
        // Linux implementation using fork/exec
        qemu_process = fork();
        if (qemu_process == 0) {
            // Child process - exec QEMU
            execl("/bin/sh", "sh", "-c", cmd.c_str(), (char*)nullptr);
            exit(1);  // If exec fails
        } else if (qemu_process < 0) {
            std::cerr << "Failed to fork QEMU process" << std::endl;
            return false;
        }
#endif

        // Wait for QEMU to start
        std::cout << "Waiting for QEMU to start..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));

        return true;
    }

    bool connectToQEMU() {
        std::cout << "Attempting to connect to QEMU on localhost:1234..." << std::endl;

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef _WIN32
        if (sock == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
#else
        if (sock < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
#endif

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        // Try to connect with retries
        for (int i = 0; i < 20; i++) {
            std::cout << "Connection attempt " << (i + 1) << "/20..." << std::endl;
            if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
                connected = true;
                std::cout << "Successfully connected to QEMU!" << std::endl;

                // Perform handshake with AVR program
                std::cout << "Sending PING handshake..." << std::endl;
                send(sock, "PING\n", 5, 0);

                std::string response = receiveResponse(5000);
                std::cout << "Received handshake response: '" << response << "'" << std::endl;

                if (response.find("PONG") != std::string::npos) {
                    std::cout << "Handshake successful!" << std::endl;
                    return true;
                } else {
                    std::cerr << "Handshake failed - expected PONG, got: " << response << std::endl;
                    return false;
                }
            }
            std::cout << "Connection failed, waiting 1 second before retry..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cerr << "Failed to connect to QEMU after 20 attempts" << std::endl;
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

#ifdef _WIN32
        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);
#else
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

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

#ifdef _WIN32
        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);
#else
        fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);
#endif

        return response;
    }

    void cleanup() {
        std::cout << "Cleaning up QEMU and connections..." << std::endl;

#ifdef _WIN32
        if (connected && sock != INVALID_SOCKET) {
            closesocket(sock);
            connected = false;
        }

        if (qemu_process) {
            TerminateProcess(qemu_process, 0);
            CloseHandle(qemu_process);
            qemu_process = nullptr;
        }
#else
        if (connected && sock >= 0) {
            close(sock);
            connected = false;
            sock = -1;
        }

        if (qemu_process > 0) {
            std::cout << "Terminating QEMU process " << qemu_process << std::endl;
            kill(qemu_process, SIGTERM);

            // Wait a bit for graceful shutdown
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Force kill if still running
            int status;
            if (waitpid(qemu_process, &status, WNOHANG) == 0) {
                std::cout << "QEMU didn't terminate gracefully, force killing..." << std::endl;
                kill(qemu_process, SIGKILL);
                if (waitpid(qemu_process, &status, WNOHANG) == 0) {
                    // If still not cleaned up, just abandon it
                    std::cout << "Warning: QEMU process may still be running" << std::endl;
                    // Kill all qemu processes
                    system("pkill qemu-system-avr");
                    system("pkill qemu");
                }
            }

            qemu_process = 0;
            std::cout << "QEMU process terminated" << std::endl;
        }
#endif
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
