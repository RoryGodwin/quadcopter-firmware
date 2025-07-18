#include "SensorFusion.h"
#include "pico/time.h"
#include <cmath>

// Constructor accepts external IMU and GPS pointers (dependency injection)
SensorFusion::SensorFusion(IMUManager* imu, GPSManager* gps)
    : imuSensor(imu), gpsSensor(gps) {}

// Call once during system setup
void SensorFusion::init() {
    imuSensor->init();
    gpsSensor->init();
    lastUpdateTime = get_absolute_time();
}

// Call repeatedly in main loop to update the state
bool SensorFusion::update() {
    // Read IMU data (always required)
    if (!imuSensor->read(imuData)) {
        return false;  // IMU failure is critical
    }

    // Try to read GPS data (may fail if blocked or no fix)
    fused.gpsFix = gpsSensor->read(gpsData) && gpsData.fix;

// Compute dt for filter update
absolute_time_t now = get_absolute_time();
float dt = absolute_time_diff_us(lastUpdateTime, now) / 1e6f;
lastUpdateTime = now;

// Update filter with current gyro/accel
orientationFilter.update(
    imuData.gyroX, imuData.gyroY, imuData.gyroZ,
    imuData.accelX, imuData.accelY, imuData.accelZ,
    dt
);

// Extract orientation from filter
fused.roll = orientationFilter.getRoll();
fused.pitch = orientationFilter.getPitch();
fused.yaw = orientationFilter.getYaw();



    // If GPS available, update position
    if (fused.gpsFix) {
        fused.latitude  = gpsData.latitude;
        fused.longitude = gpsData.longitude;
        fused.altitude  = gpsData.altitude;

        // Velocity from IMU (basic integration for now)
        // Get time since last update
        absolute_time_t now = get_absolute_time();
        float dt = absolute_time_diff_us(lastUpdateTime, now) / 1e6f;
        lastUpdateTime = now;

        // Integrate acceleration to estimate velocity (very basic)
        fused.velocityX += imuData.accelX * dt;
        fused.velocityY += imuData.accelY * dt;
        fused.velocityZ += imuData.accelZ * dt;
    }

    return true;
}

// Access fused sensor state
const FusionData& SensorFusion::getData() {
    return fused;
}
