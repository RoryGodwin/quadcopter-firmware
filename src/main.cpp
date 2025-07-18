#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "UltrasonicSensor.h"
#include "CommandReceiver.h"
#include "EscController.h"
#include "GPSModule.h"
#include "IMUSensor.h"
#include "SensorFusion.h"
#include "FlightController.h"
#include "TelemetryManager.h"

#include <stdio.h>
#include <optional>

// === Global Components ===
UltrasonicSensor* ultrasonic = nullptr;
CommandReceiver commandReceiver;
GPSModule gps;
IMUSensor imu;
SensorFusion sensorFusion;
FlightController flightController;
TelemetryManager telemetry(&sensorFusion);
EscController escController;

// === Setup Function ===
void setup() {
    stdio_init_all();
    sleep_ms(2000);
    printf("=== Drone System Booting ===\n");

    // --- ADC Setup for Battery Monitoring ---
    adc_init();
    adc_gpio_init(26);     // GPIO 26 = ADC0
    adc_select_input(0);   // Select ADC0

    // --- Optional Ultrasonic ---
    UltrasonicSensor* us = new UltrasonicSensor(10, 11);
    us->init();
    auto test = us->readDistanceCM();
    if (test.has_value()) {
        ultrasonic = us;
        printf("[INFO] Ultrasonic sensor detected: %.2f cm\n", *test);
    } else {
        delete us;
        ultrasonic = nullptr;
        printf("[INFO] No ultrasonic sensor found.\n");
    }

    // --- ESC Setup ---
    escController.attachMotor(0, 2);
    escController.attachMotor(1, 3);
    escController.attachMotor(2, 4);
    escController.attachMotor(3, 5);
    escController.armAll();
    printf("[INFO] ESCs armed.\n");

    // --- IMU ---
    if (!imu.init()) {
        printf("[ERROR] IMU init failed.\n");
        while (true) sleep_ms(1000);
    }
    printf("[INFO] IMU ready.\n");

    // --- GPS ---
    if (!gps.init()) {
        printf("[WARN] GPS not found. Using IMU only.\n");
    } else {
        printf("[INFO] GPS detected.\n");
    }

    // --- Flight Controller ---
    flightController.setSensorFusion(&sensorFusion);
    flightController.setESCController(&escController);
}

// === Main Loop ===
int main() {
    setup();

    while (true) {
        // --- Read Commands ---
        if (stdin_is_readable()) {
            char c = getchar();
            commandReceiver.inputChar(c);
        }

        // --- IMU and GPS ---
        imu.update();
        if (gps.isAvailable()) gps.update();

        // --- Sensor Fusion ---
        sensorFusion.setIMUData(imu.getOrientation());
        if (gps.isAvailable()) sensorFusion.setGPSData(gps.getLocation());
        sensorFusion.update();

        // --- Ultrasonic ---
        if (ultrasonic) {
            auto dist = ultrasonic->readDistanceCM();
            if (!dist.has_value()) {
                printf("[WARN] Disabling ultrasonic.\n");
                delete ultrasonic;
                ultrasonic = nullptr;
            }
        }

        // --- Battery ADC ---
        uint16_t raw = adc_read();
        telemetry.setBatteryRawADC(raw);

        // --- Telemetry ---
        telemetry.update();
        telemetry.send();

        // --- Control ---
        flightController.update();

        sleep_ms(50);
    }

    return 0;
}
