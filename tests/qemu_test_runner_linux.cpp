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

// Linux implementation

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "qemu_test_runner.h"

QEMUTestRunner::QEMUTestRunner()
    : listen_sock(-1),
      sock(-1),
      monitor_listen_sock(-1),
      monitor_sock(-1),
      qemu_process(0),
      connected(false),
      monitor_port(0) {}

QEMUTestRunner::~QEMUTestRunner() { cleanup(); }

int QEMUTestRunner::findFreePort(int start_port, int end_port) {
    for (int port = start_port; port <= end_port; port++) {
        int test_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (test_sock < 0) continue;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(test_sock, (sockaddr*)&addr, sizeof(addr)) == 0) {
            close(test_sock);
            return port;
        }
        close(test_sock);
    }
    return -1;  // No free port found
}

bool QEMUTestRunner::createListeningSockets() {
    std::cout << "Creating listening socket on port 1234..." << std::endl;

    // Create serial listening socket
    listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock < 0) {
        std::cerr << "Failed to create listening socket" << std::endl;
        return false;
    }

    // Allow socket reuse
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
        std::cerr << "Failed to bind to port 1234" << std::endl;
        close(listen_sock);
        listen_sock = -1;
        return false;
    }

    if (listen(listen_sock, 1) != 0) {
        std::cerr << "Failed to listen on port 1234" << std::endl;
        close(listen_sock);
        listen_sock = -1;
        return false;
    }

    // Find free port for monitor
    monitor_port = findFreePort(1000, 65535);
    if (monitor_port == -1) {
        std::cerr << "Failed to find free port for monitor" << std::endl;
        close(listen_sock);
        listen_sock = -1;
        return false;
    }

    std::cout << "Creating monitor listening socket on port " << monitor_port << "..." << std::endl;

    // Create monitor listening socket
    monitor_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (monitor_listen_sock < 0) {
        std::cerr << "Failed to create monitor listening socket" << std::endl;
        close(listen_sock);
        listen_sock = -1;
        return false;
    }

    setsockopt(monitor_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in monitor_addr;
    monitor_addr.sin_family = AF_INET;
    monitor_addr.sin_port = htons(monitor_port);
    monitor_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(monitor_listen_sock, (sockaddr*)&monitor_addr, sizeof(monitor_addr)) != 0) {
        std::cerr << "Failed to bind monitor to port " << monitor_port << std::endl;
        close(listen_sock);
        close(monitor_listen_sock);
        listen_sock = -1;
        monitor_listen_sock = -1;
        return false;
    }

    if (listen(monitor_listen_sock, 1) != 0) {
        std::cerr << "Failed to listen on monitor port " << monitor_port << std::endl;
        close(listen_sock);
        close(monitor_listen_sock);
        listen_sock = -1;
        monitor_listen_sock = -1;
        return false;
    }

    std::cout << "Listening sockets ready on ports 1234 (serial) and " << monitor_port << " (monitor)" << std::endl;
    return true;
}

bool QEMUTestRunner::startQEMU(std::string avr_binary) {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!std::filesystem::exists(avr_binary)) {
        std::cerr << "AVR binary file not found: " << avr_binary << std::endl;
        return false;
    }

    std::string options = " -machine uno -nographic -monitor none";
    std::string serial = " -serial tcp:localhost:1234,server";
    std::string chardev = " -chardev socket,id=mon1,host=localhost,port=" + std::to_string(monitor_port);
    std::string monitor = " -mon chardev=mon1";
    std::string bios = " -bios " + avr_binary;
    std::string cmd = "qemu-system-avr" + options + serial + chardev + monitor + bios;

    std::cout << "Starting QEMU with command: " << cmd << std::endl;

    qemu_process = fork();
    if (qemu_process == 0) {
        execl("/bin/sh", "sh", "-c", cmd.c_str(), (char*)nullptr);
        _exit(1);
    } else if (qemu_process < 0) {
        std::cerr << "Failed to fork QEMU process" << std::endl;
        return false;
    }

    std::cout << "Waiting for QEMU to start..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return true;
}

bool QEMUTestRunner::acceptConnections() {
    std::cout << "Waiting for QEMU to connect..." << std::endl;

    if (listen_sock < 0 || monitor_listen_sock < 0) {
        std::cerr << "No listening sockets available" << std::endl;
        return false;
    }

    // Set sockets to non-blocking to check both
    int flags = fcntl(listen_sock, F_GETFL, 0);
    fcntl(listen_sock, F_SETFL, flags | O_NONBLOCK);
    int monitor_flags = fcntl(monitor_listen_sock, F_GETFL, 0);
    fcntl(monitor_listen_sock, F_SETFL, monitor_flags | O_NONBLOCK);

    bool serial_connected = false;
    bool monitor_connected = false;
    auto start_time = std::chrono::steady_clock::now();

    while ((!serial_connected || !monitor_connected) &&
           std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() <
               30) {
        if (!serial_connected) {
            sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            sock = accept(listen_sock, (sockaddr*)&client_addr, &client_len);
            if (sock >= 0) {
                std::cout << "Serial connection accepted!" << std::endl;
                serial_connected = true;
            }
        }

        if (!monitor_connected) {
            sockaddr_in monitor_addr;
            socklen_t monitor_len = sizeof(monitor_addr);
            monitor_sock = accept(monitor_listen_sock, (sockaddr*)&monitor_addr, &monitor_len);
            if (monitor_sock >= 0) {
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
    fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);

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
    send(sock, cmd_with_newline.c_str(), cmd_with_newline.length(), 0);

    return receiveResponse(2000);
}

std::string QEMUTestRunner::receiveResponse(int timeout_ms) {
    if (!connected) return "";

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    std::string response;
    auto start_time = std::chrono::steady_clock::now();

    while (
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() <
        timeout_ms) {
        char buffer[1024];
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

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

    fcntl(sock, F_SETFL, flags & ~O_NONBLOCK);

    return response;
}

void QEMUTestRunner::cleanup() {
    std::cout << "Cleaning up QEMU and connections..." << std::endl;

    // Send quit command through monitor if connected
    if (monitor_sock >= 0) {
        std::cout << "Sending 'quit' command to QEMU monitor..." << std::endl;
        send(monitor_sock, "quit\n", 5, 0);

        // Read monitor response
        char buffer[1024];
        int bytes = recv(monitor_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "Monitor response: " << buffer << std::endl;
        }

        close(monitor_sock);
        monitor_sock = -1;
    }

    if (connected && sock >= 0) {
        close(sock);
        connected = false;
        sock = -1;
    }

    if (listen_sock >= 0) {
        close(listen_sock);
        listen_sock = -1;
    }

    if (monitor_listen_sock >= 0) {
        close(monitor_listen_sock);
        monitor_listen_sock = -1;
    }

    if (qemu_process > 0) {
        std::cout << "Waiting for QEMU process " << qemu_process << " to exit..." << std::endl;

        // Wait for graceful shutdown after quit command
        int status;
        for (int i = 0; i < 50; i++) {
            if (waitpid(qemu_process, &status, WNOHANG) > 0) {
                std::cout << "QEMU process exited gracefully" << std::endl;
                qemu_process = 0;
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // If still running, force terminate
        std::cout << "QEMU didn't exit gracefully, sending SIGTERM..." << std::endl;
        kill(qemu_process, SIGTERM);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (waitpid(qemu_process, &status, WNOHANG) == 0) {
            std::cout << "QEMU didn't terminate gracefully, force killing..." << std::endl;
            kill(qemu_process, SIGKILL);
            waitpid(qemu_process, &status, 0);
        }

        qemu_process = 0;
        std::cout << "QEMU process terminated" << std::endl;
    }
}
