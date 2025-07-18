#ifndef COMMAND_RECEIVER_H
#define COMMAND_RECEIVER_H

#include <string>

// Class that handles parsing and interpreting low-bandwidth commands
class CommandReceiver {
public:
    CommandReceiver();

    // Feed one character at a time (e.g., from UART)
    void inputChar(char c);

private:
    std::string buffer; // Build command from incoming characters

    // Decode and apply a complete command
    void processCommand(const std::string& cmd);

    // Helper to parse movement strings like M+10-20
    void handleMovementCommand(const std::string& cmd);
};

#endif // COMMAND_RECEIVER_H
