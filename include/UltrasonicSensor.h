#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include "pico/stdlib.h"
#include <optional>

// This class represents an HC-SR04 ultrasonic sensor.
// It allows optional use — safe to leave disconnected.
class UltrasonicSensor {
public:
    UltrasonicSensor(uint trigPin, uint echoPin);
    void init();

    // Attempts to read distance in centimeters.
    // Returns std::nullopt if the sensor is not connected or fails to respond.
    std::optional<float> readDistanceCM();

private:
    uint trigPin;
    uint echoPin;

    // Measures echo time in microseconds, or -1 if failed
    int64_t measureEchoTime();
};

#endif // ULTRASONIC_SENSOR_H
