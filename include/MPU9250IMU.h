#ifndef MPU9250_IMU_H
#define MPU9250_IMU_H

#include "IMUManager.h"

// MPU9250IMU provides support for reading sensor data from the MPU-9250 IMU.
// It implements the IMUManager interface and uses I2C to communicate with the sensor.
class MPU9250IMU : public IMUManager {
public:
    // Constructor: takes I2C port and address (default is 0x68)
    MPU9250IMU(i2c_inst_t* i2cPort, uint sdaPin, uint sclPin, uint addr = 0x68);

    // Set up I2C and initialize sensor registers
    void init() override;

    // Read raw accelerometer, gyroscope, and magnetometer data
    bool read(IMUData& data) override;

private:
    i2c_inst_t* i2c;     // I2C bus (i2c0 or i2c1)
    uint8_t address;     // I2C address of the sensor
    uint sda, scl;       // GPIO pins for I2C

    // Read a block of bytes from a sensor register
    bool readBytes(uint8_t reg, uint8_t* buffer, size_t length);

    // Write a byte to a sensor register
    bool writeByte(uint8_t reg, uint8_t value);

    // Helpers for parsing raw data
    int16_t read16(uint8_t high, uint8_t low);
};

#endif // MPU9250_IMU_H
