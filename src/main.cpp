#include "pico/stdlib.h"
#include "UltrasonicSensor.h"
#include <stdio.h>
#include <optional>

// Pointer to our optional ultrasonic sensor object
UltrasonicSensor* ultrasonic = nullptr;

int main() {
    // Initialize USB serial output (for printf)
    stdio_init_all();
    sleep_ms(2000);  // Give USB serial time to connect

    printf("Starting Drone System...\n");

    // Allocate and initialize the ultrasonic sensor (GPIO 10 = Trig, GPIO 11 = Echo)
    auto* us = new UltrasonicSensor(10, 11);
    us->init();

    // Try one test read to see if the sensor is connected
    auto initialReading = us->readDistanceCM();

    if (initialReading.has_value()) {
        // If sensor responded, keep it
        ultrasonic = us;
        printf("Ultrasonic sensor detected. Initial height = %.2f cm\n", *initialReading);
    } else {
        // If no response, discard sensor and continue without it
        delete us;
        ultrasonic = nullptr;
        printf("Ultrasonic sensor NOT detected. Running without it.\n");
    }

    // Main control loop (replace with your flight logic later)
    while (true) {
        // Only try to read from ultrasonic if it's connected
        if (ultrasonic) {
            auto distance = ultrasonic->readDistanceCM();

            if (distance.has_value()) {
                // Valid reading
                printf("Altitude: %.2f cm\n", *distance);
            } else {
                // Sensor was removed or failed
                printf("Ultrasonic sensor failed. Disabling further use.\n");
                delete ultrasonic;
                ultrasonic = nullptr;
            }
        }

        // Delay to limit loop frequency (adjust as needed)
        sleep_ms(200);
    }

    return 0;
}
