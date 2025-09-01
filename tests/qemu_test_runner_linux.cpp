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

QEMUTestRunner::QEMUTestRunner() : listen_sock(-1), sock(-1), qemu_process(0), connected(false) {}

QEMUTestRunner::~QEMUTestRunner() { cleanup(); }

bool QEMUTestRunner::createListeningSocket() {
    std::cout << "Creating listening socket on port 1234..." << std::endl;

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

    std::cout << "Listening socket ready on port 1234" << std::endl;
    return true;
}

bool QEMUTestRunner::startQEMU(std::string avr_binary) {
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    if (!std::filesystem::exists(avr_binary)) {
        std::cerr << "AVR binary file not found: " << avr_binary << std::endl;
        return false;
    }

    std::string cmd = "qemu-system-avr -machine uno -nographic -serial tcp:localhost:1234 -bios " + avr_binary;

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

bool QEMUTestRunner::acceptConnection() {
    std::cout << "Waiting for QEMU to connect..." << std::endl;

    if (listen_sock < 0) {
        std::cerr << "No listening socket available" << std::endl;
        return false;
    }

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    sock = accept(listen_sock, (sockaddr*)&client_addr, &client_len);

    if (sock < 0) {
        std::cerr << "Failed to accept connection" << std::endl;
        return false;
    }

    connected = true;
    std::cout << "QEMU connected successfully!" << std::endl;

    // Wait for hello from AVR
    std::cout << "Waiting for hello from AVR..." << std::endl;
    std::string response = receiveResponse(10000);
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

    if (connected && sock >= 0) {
        close(sock);
        connected = false;
        sock = -1;
    }

    if (listen_sock >= 0) {
        close(listen_sock);
        listen_sock = -1;
    }

    if (qemu_process > 0) {
        std::cout << "Waiting for QEMU process " << qemu_process << " to exit..." << std::endl;

        // Wait briefly for graceful shutdown first
        int status;
        for (int i = 0; i < 20; i++) {
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
