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

#pragma once
#include <stdbool.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

// Interface for QEMU test runner
class QEMUTestRunner {
   public:
    // Default constructor (defined per-platform in corresponding .cpp)
    QEMUTestRunner();
    // Destructor for safe polymorphic deletion
    ~QEMUTestRunner();

    // Start the QEMU process with the expected AVR image and serial port.
    // Returns true if QEMU was launched (not necessarily connected yet).
    // Implementations typically wait a short period for the VM to boot.
    bool startQEMU(std::string avr_binary);

    // Create listening socket on port 1234 and wait for QEMU to connect.
    // Should be called before startQEMU().
    // Returns true if socket is ready to accept connections.
    bool createListeningSocket();

    // Accept connection from QEMU and perform hello handshake.
    // Should be called after startQEMU().
    // Returns true on successful connection and handshake.
    bool acceptConnection();

    // Send a single-line command to the target (a trailing newline will be added if needed)
    // and return the first line of the response (up to and including the newline),
    // or an empty string on timeout/connection error. Not thread-safe.
    // param command: Command text without the trailing newline.
    // return: Response line (may contain a trailing '\n'), or empty string on failure.
    std::string sendCommand(const std::string& command);

    // Receive a line from the target within the given timeout.
    // Reads until a '\n' is received or the timeout elapses.
    // param timeout_ms: Maximum time to wait in milliseconds.
    // return: Collected data (may contain a trailing '\n'), or empty string on timeout.
    std::string receiveResponse(int timeout_ms = 1000);

    // Close sockets, terminate QEMU if running, and release resources.
    // Safe to call multiple times; subsequent calls are no-ops.
    void cleanup();

   private:
#ifdef _WIN32
    SOCKET listen_sock;  // Listening socket
    SOCKET sock;         // Connected socket
    HANDLE qemu_process;
#else
    int listen_sock;  // Listening socket
    int sock;         // Connected socket
    pid_t qemu_process;
#endif
    bool connected;
};
