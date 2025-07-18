#include "CommandReceiver.h"
#include <stdio.h>
#include <stdlib.h>
#include <cctype>

// Constructor
CommandReceiver::CommandReceiver() {
    buffer.reserve(16);
}

// This function should be called on each incoming byte from serial
void CommandReceiver::inputChar(char c) {
    if (c == '\n' || c == '\r') {
        // End of command
        if (!buffer.empty()) {
            processCommand(buffer);
            buffer.clear();
        }
    } else {
        // Accumulate characters (with a limit)
        if (buffer.length() < 15) {
            buffer += c;
        } else {
            buffer.clear(); // Drop malformed long command
        }
    }
}

void CommandReceiver::processCommand(const std::string& cmd) {
    if (cmd.empty()) return;

    char type = toupper(cmd[0]);

    switch (type) {
        case 'M':
            handleMovementCommand(cmd);
            break;
        case 'A':
            printf("[CMD] Arm Motors\n");
            // Insert arming logic here
            break;
        case 'D':
            printf("[CMD] Disarm Motors\n");
            // Insert disarming logic here
            break;
        case 'H':
            printf("[CMD] Hold Position\n");
            // Trigger GPS/IMU lock
            break;
        case 'F':
            printf("[CMD] Switch to Auto Mode\n");
            // Switch to autonomous control
            break;
        default:
            printf("[CMD] Unknown command: %s\n", cmd.c_str());
    }
}

void CommandReceiver::handleMovementCommand(const std::string& cmd) {
    // Expect format like M+10-20
    if (cmd.length() < 4) return;

    int x = 0, y = 0;
    sscanf(cmd.c_str() + 1, "%d%d", &x, &y);

    printf("[CMD] Manual Move: X=%d Y=%d\n", x, y);
    // Apply manual control to the flight controller here
}
