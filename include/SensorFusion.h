#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H

#include "IMUManager.h"
#include "GPSManager.h"
#include "MadgwickFilter.h"


// Structure for fused output: position, velocity, and orientation
struct FusionData {
    float latitude = 0.0f;
    float longitude = 0.0f;
    float altitude = 0.0f;

    float velocityX = 0.0f;  // Dead-reckoning estimate
    float velocityY = 0.0f;
    float velocityZ = 0.0f;

    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    bool gpsFix = false;     // True if GPS is currently valid
};

class SensorFusion {
public:
    SensorFusion(IMUManager* imu, GPSManager* gps);

    void init();
    bool update();                  // Read sensors and update internal estimates
    const FusionData& getData();    // Return current fused data

private:
    IMUManager* imuSensor;
    GPSManager* gpsSensor;

    FusionData fused;
    IMUData imuData;
    GPSData gpsData;

    MadgwickFilter orientationFilter;

    absolute_time_t lastUpdateTime;
};

#endif // SENSOR_FUSION_H
