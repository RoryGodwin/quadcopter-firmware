#include "TelemetryManager.h"
#include <stdio.h>
#include <math.h>

TelemetryManager::TelemetryManager(SensorFusion* fusionSystem)
    : fusion(fusionSystem) {}

void TelemetryManager::update() {
    TelemetryData data;

    auto orientation = fusion->getOrientation();
    data.roll = orientation.roll;
    data.pitch = orientation.pitch;
    data.yaw = orientation.yaw;

    data.altitude = fusion->getAltitude();
    data.speed = fusion->getSpeed();
    data.distanceFromHome = fusion->getDistanceFromHome();

    if (lastTelemetry.has_value()) {
        data.batteryVoltage = lastTelemetry->batteryVoltage;
        data.batteryRawADC = lastTelemetry->batteryRawADC;
    } else {
        data.batteryVoltage = 0.0f;
        data.batteryRawADC = 0;
    }

    lastTelemetry = data;
}

void TelemetryManager::setBatteryRawADC(uint16_t raw) {
    if (lastTelemetry.has_value()) {
        lastTelemetry->batteryRawADC = raw;
    }
}

void TelemetryManager::send() {
    if (!lastTelemetry.has_value()) return;

    const auto& t = *lastTelemetry;

    printf("TLM RPY:%.2f,%.2f,%.2f ALT:%.2fm SPD:%.2fm/s DIST:%.2fm BATT_ADC:%u\n",
           t.roll, t.pitch, t.yaw,
           t.altitude,
           t.speed,
           t.distanceFromHome,
           t.batteryRawADC);
}
