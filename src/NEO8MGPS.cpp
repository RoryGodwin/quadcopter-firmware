#include "NEO8MGPS.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include <cstring>
#include <cstdlib>

// Constructor sets UART parameters
NEO8MGPS::NEO8MGPS(uart_inst_t* uartPort, uint txPin, uint rxPin, uint32_t baud)
    : uart(uartPort), tx(txPin), rx(rxPin), baudRate(baud) {}

// Initialize UART and GPIO for GPS
void NEO8MGPS::init() {
    uart_init(uart, baudRate);
    gpio_set_function(tx, GPIO_FUNC_UART);
    gpio_set_function(rx, GPIO_FUNC_UART);
}

// Read GPS NMEA data and extract info from $GPGGA sentence
bool NEO8MGPS::read(GPSData& data) {
    int idx = 0;

    // Wait for start of sentence '$'
    while (uart_is_readable(uart)) {
        char c = uart_getc(uart);
        if (c == '$') {
            buffer[idx++] = c;
            break;
        }
    }

    // Read the rest of the sentence until newline or buffer full
    while (idx < BUFFER_SIZE - 1 && uart_is_readable(uart)) {
        char c = uart_getc(uart);
        buffer[idx++] = c;
        if (c == '\n') break;
    }

    buffer[idx] = '\0';  // Null-terminate

    // Only parse GPGGA (position fix) for now
    if (strstr(buffer, "GPGGA") != nullptr) {
        return parseGPGGA(buffer, data);
    }

    return false;
}

// Convert NMEA GPS coordinates to decimal degrees
float NEO8MGPS::convertToDecimalDegrees(const char* raw, const char hemisphere) {
    if (!raw || strlen(raw) < 6) return 0.0f;

    char degs[3] = {0};
    strncpy(degs, raw, 2);
    float degrees = atof(degs);

    float minutes = atof(raw + 2);
    float decimal = degrees + (minutes / 60.0f);

    if (hemisphere == 'S' || hemisphere == 'W') {
        decimal *= -1.0f;
    }

    return decimal;
}

// Parse $GPGGA sentence for latitude, longitude, altitude, satellites
bool NEO8MGPS::parseGPGGA(const char* sentence, GPSData& data) {
    char copy[BUFFER_SIZE];
    strncpy(copy, sentence, BUFFER_SIZE);

    // Tokenize the sentence by commas
    char* tokens[15] = { nullptr };
    int i = 0;
    char* token = strtok(copy, ",");

    while (token && i < 15) {
        tokens[i++] = token;
        token = strtok(nullptr, ",");
    }

    if (i < 10) return false;

    // Example:
    // $GPGGA,hhmmss.ss,lat,NS,long,EW,fix,satellites,hdop,altitude,M,...

    data.fix = atoi(tokens[6]) > 0;
    data.satellites = atoi(tokens[7]);
    data.altitude = atof(tokens[9]);
    data.latitude = convertToDecimalDegrees(tokens[2], tokens[3][0]);
    data.longitude = convertToDecimalDegrees(tokens[4], tokens[5][0]);

    return data.fix;
}
