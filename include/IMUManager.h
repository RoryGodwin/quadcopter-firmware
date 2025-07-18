#ifndef IMU_MANAGER_H
#define IMU_MANAGER_H

// Structure to hold IMU sensor readings
struct IMUData {
    float accelX, accelY, accelZ;   // Acceleration (g)
    float gyroX, gyroY, gyroZ;      // Angular velocity (deg/s)
    float magX, magY, magZ;         // Magnetic field (uT)
};

// Abstract base class representing any IMU device
class IMUManager {
public:
    // Initialize the sensor (e.g., I2C setup, configure registers)
    virtual void init() = 0;

    // Read current sensor data into the provided structure
    virtual bool read(IMUData& data) = 0;

    // Virtual destructor for cleanup
    virtual ~IMUManager() = default;
};

#endif // IMU_MANAGER_H
