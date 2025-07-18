#ifndef TELEMETRY_MANAGER_H
#define TELEMETRY_MANAGER_H

#include "SensorFusion.h"
#include <optional>
#include <string>

struct TelemetryData {
    float roll, pitch, yaw;
    float altitude;
    float speed;
    float distanceFromHome;
    float batteryVoltage;
    uint16_t batteryRawADC;  // Raw ADC reading (0–4095)
};

class TelemetryManager {
private:
    SensorFusion* fusion;
    std::optional<TelemetryData> lastTelemetry;

public:
    TelemetryManager(SensorFusion* fusionSystem);

    void update();
    void send();

    void setBatteryRawADC(uint16_t raw);
};

#endif
