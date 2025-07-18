#ifndef NEO8M_GPS_H
#define NEO8M_GPS_H

#include "GPSManager.h"

// Implements NEO-8M GPS via UART, parsing NMEA sentences (e.g. $GPGGA, $GPRMC)
class NEO8MGPS : public GPSManager {
public:
    // Constructor: UART instance, TX/RX pins, and optional baud rate
    NEO8MGPS(uart_inst_t* uartPort, uint txPin, uint rxPin, uint32_t baud = 9600);

    void init() override;
    bool read(GPSData& data) override;

private:
    uart_inst_t* uart;
    uint tx, rx;
    uint32_t baudRate;

    // Buffer for assembling GPS sentences
    static constexpr int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];

    // Helper functions
    bool parseGPGGA(const char* sentence, GPSData& data);
    float convertToDecimalDegrees(const char* raw, const char hemisphere);
};

#endif // NEO8M_GPS_H
