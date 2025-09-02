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

// Windows implementation

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "qemu_test_runner.h"

QEMUTestRunner::QEMUTestRunner()
    : serial_listen_socket(INVALID_SOCKET),
      serial_socket(INVALID_SOCKET),
      monitor_listen_socket(INVALID_SOCKET),
      monitor_socket(INVALID_SOCKET),
      qemu_process(nullptr),
      connected(false),
      serial_port(0),
      monitor_port(0) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

QEMUTestRunner::~QEMUTestRunner() {
    cleanup();
    WSACleanup();
}

int QEMUTestRunner::findFreePort(int start_port, int end_port) {
    for (int port = start_port; port <= end_port; port++) {
        SOCKET test_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (test_sock == INVALID_SOCKET) continue;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(test_sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
            closesocket(test_sock);
            return port;
        }
        closesocket(test_sock);
    }
    return -1;  // No free port found
}

bool QEMUTestRunner::createListeningSockets() {
    // Find free port for serial
    serial_port = findFreePort(1234, 65535);
    if (serial_port == -1) {
        std::cerr << "Failed to find free port for serial" << std::endl;
        return false;
    }

    std::cout << "Creating serial listening socket on port " << serial_port << "..." << std::endl;

    // Create serial listening socket
    serial_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serial_listen_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create serial listening socket" << std::endl;
        return false;
    }

    // Allow socket reuse
    int opt = 1;
    setsockopt(serial_listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serial_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serial_listen_socket, (sockaddr*)&addr, sizeof(addr)) != 0) {
        std::cerr << "Failed to bind to serial port " << serial_port << std::endl;
        closesocket(serial_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        return false;
    }

    if (listen(serial_listen_socket, 1) != 0) {
        std::cerr << "Failed to listen on serial port " << serial_port << std::endl;
        closesocket(serial_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        return false;
    }

    // Find free port for monitor
    monitor_port = findFreePort(1000, 65535);
    if (monitor_port == -1) {
        std::cerr << "Failed to find free port for monitor" << std::endl;
        closesocket(serial_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        return false;
    }

    std::cout << "Creating monitor listening socket on port " << monitor_port << "..." << std::endl;

    // Create monitor listening socket
    monitor_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (monitor_listen_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create monitor listening socket" << std::endl;
        closesocket(serial_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        return false;
    }

    setsockopt(monitor_listen_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in monitor_addr;
    monitor_addr.sin_family = AF_INET;
    monitor_addr.sin_port = htons(monitor_port);
    monitor_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(monitor_listen_socket, (sockaddr*)&monitor_addr, sizeof(monitor_addr)) != 0) {
        std::cerr << "Failed to bind monitor to port " << monitor_port << std::endl;
        closesocket(serial_listen_socket);
        closesocket(monitor_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        monitor_listen_socket = INVALID_SOCKET;
        return false;
    }

    if (listen(monitor_listen_socket, 1) != 0) {
        std::cerr << "Failed to listen on monitor port " << monitor_port << std::endl;
        closesocket(serial_listen_socket);
        closesocket(monitor_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
        monitor_listen_socket = INVALID_SOCKET;
        return false;
    }

    std::cout << "Listening sockets ready on ports " << serial_port << " (serial) and " << monitor_port << " (monitor)"
              << std::endl;
    return true;
}

bool QEMUTestRunner::startQEMU(std::string avr_binary) {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!std::filesystem::exists(avr_binary)) {
        std::cerr << "AVR binary file not found: " << avr_binary << std::endl;
        return false;
    }

    // See: https://www.qemu.org/docs/master/system/qemu-manpage.html
    std::string options = " -machine uno -nographic -monitor none";
    std::string serial = " -serial tcp:localhost:" + std::to_string(serial_port);
    std::string chardev = " -chardev socket,id=mon1,host=localhost,port=" + std::to_string(monitor_port);
    std::string monitor = " -mon chardev=mon1";
    std::string bios = " -bios " + avr_binary;
    std::string cmd = "qemu-system-avr" + options + serial + chardev + monitor + bios;

    std::cout << "Starting QEMU with command: " << cmd << std::endl;

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

    std::cout << "Waiting for QEMU to start..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return true;
}

bool QEMUTestRunner::acceptConnections() {
    std::cout << "Waiting for QEMU to connect..." << std::endl;

    if (serial_listen_socket == INVALID_SOCKET || monitor_listen_socket == INVALID_SOCKET) {
        std::cerr << "No listening sockets available" << std::endl;
        return false;
    }

    // Set sockets to non-blocking to check both
    u_long mode = 1;
    ioctlsocket(serial_listen_socket, FIONBIO, &mode);
    ioctlsocket(monitor_listen_socket, FIONBIO, &mode);

    bool serial_connected = false;
    bool monitor_connected = false;
    auto start_time = std::chrono::steady_clock::now();

    while ((!serial_connected || !monitor_connected) &&
           std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() <
               30) {
        if (!serial_connected) {
            sockaddr_in client_addr;
            int client_len = sizeof(client_addr);
            serial_socket = accept(serial_listen_socket, (sockaddr*)&client_addr, &client_len);
            if (serial_socket != INVALID_SOCKET) {
                std::cout << "Serial connection accepted!" << std::endl;
                serial_connected = true;
            }
        }

        if (!monitor_connected) {
            sockaddr_in monitor_addr;
            int monitor_len = sizeof(monitor_addr);
            monitor_socket = accept(monitor_listen_socket, (sockaddr*)&monitor_addr, &monitor_len);
            if (monitor_socket != INVALID_SOCKET) {
                std::cout << "Monitor connection accepted!" << std::endl;
                monitor_connected = true;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!serial_connected || !monitor_connected) {
        std::cerr << "Failed to accept all connections (serial: " << serial_connected
                  << ", monitor: " << monitor_connected << ")" << std::endl;
        return false;
    }

    // Set serial socket back to blocking
    mode = 0;
    ioctlsocket(serial_socket, FIONBIO, &mode);

    connected = true;

    // Wait for hello from AVR
    std::cout << "Waiting for hello from AVR..." << std::endl;
    std::string response = receiveResponse(10000);

    // Strip trailing CR/LF
    response.erase(response.find_last_not_of("\r\n") + 1);

    std::cout << "Received: '" << response << "'" << std::endl;

    if (response.find("hello") != std::string::npos) {
        std::cout << "Hello handshake successful!" << std::endl;
        return true;
    } else {
        std::cerr << "Hello handshake failed - expected hello, got: " << response << std::endl;
        return false;
    }
}

std::string QEMUTestRunner::sendCommand(const std::string& command) {
    if (!connected) return "";

    std::string cmd_with_newline = command + "\n";
    send(serial_socket, cmd_with_newline.c_str(), static_cast<int>(cmd_with_newline.length()), 0);

    return receiveResponse(2000);
}

std::string QEMUTestRunner::receiveResponse(int timeout_ms) {
    if (!connected) return "";

    u_long mode = 1;
    ioctlsocket(serial_socket, FIONBIO, &mode);

    std::string response;
    auto start_time = std::chrono::steady_clock::now();

    while (
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() <
        timeout_ms) {
        char buffer[1024];
        int bytes = recv(serial_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes > 0) {
            buffer[bytes] = '\0';
            response += buffer;

            if (response.find('\n') != std::string::npos) {
                break;
            }
        } else if (bytes == 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    mode = 0;
    ioctlsocket(serial_socket, FIONBIO, &mode);

    return response;
}

void QEMUTestRunner::cleanup() {
    // Prevent double cleanup
    if (qemu_process == nullptr) return;

    std::cout << "Cleaning up QEMU and connections..." << std::endl;

    // Send quit command through monitor if connected
    if (monitor_socket != INVALID_SOCKET) {
        std::cout << "Sending 'quit' command to QEMU monitor..." << std::endl;
        send(monitor_socket, "quit\n", 5, 0);

        // Read monitor response
        char buffer[1024];
        int bytes = recv(monitor_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "Monitor response: " << buffer << std::endl;
        }

        closesocket(monitor_socket);
        monitor_socket = INVALID_SOCKET;
    }

    if (connected && serial_socket != INVALID_SOCKET) {
        closesocket(serial_socket);
        connected = false;
        serial_socket = INVALID_SOCKET;
    }

    if (serial_listen_socket != INVALID_SOCKET) {
        closesocket(serial_listen_socket);
        serial_listen_socket = INVALID_SOCKET;
    }

    if (monitor_listen_socket != INVALID_SOCKET) {
        closesocket(monitor_listen_socket);
        monitor_listen_socket = INVALID_SOCKET;
    }

    if (qemu_process) {
        // Wait for graceful shutdown after quit command
        DWORD result = WaitForSingleObject(qemu_process, 5000);
        if (result == WAIT_TIMEOUT) {
            std::cout << "QEMU didn't exit gracefully, terminating process..." << std::endl;
            TerminateProcess(qemu_process, 0);
        } else {
            std::cout << "QEMU exited gracefully" << std::endl;
        }
        CloseHandle(qemu_process);
        qemu_process = nullptr;
    }
}