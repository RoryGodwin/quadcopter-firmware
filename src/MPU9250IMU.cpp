#include "MPU9250IMU.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Constructor initializes I2C settings
MPU9250IMU::MPU9250IMU(i2c_inst_t* i2cPort, uint sdaPin, uint sclPin, uint addr)
    : i2c(i2cPort), address(addr), sda(sdaPin), scl(sclPin) {}

// Setup I2C, configure GPIOs and reset the sensor
void MPU9250IMU::init() {
    i2c_init(i2c, 400 * 1000); // 400 kHz
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    // Wake up MPU-9250 (exit sleep mode)
    writeByte(0x6B, 0x00); // PWR_MGMT_1 = 0
}

// Read 9-axis sensor data into the provided IMUData struct
bool MPU9250IMU::read(IMUData& data) {
    uint8_t buffer[14];

    // Read 14 bytes from register 0x3B (accel+gyro)
    if (!readBytes(0x3B, buffer, 14)) return false;

    // Parse accel and gyro
    data.accelX = (float)read16(buffer[0], buffer[1]) / 16384.0f;
    data.accelY = (float)read16(buffer[2], buffer[3]) / 16384.0f;
    data.accelZ = (float)read16(buffer[4], buffer[5]) / 16384.0f;

    data.gyroX = (float)read16(buffer[8], buffer[9]) / 131.0f;
    data.gyroY = (float)read16(buffer[10], buffer[11]) / 131.0f;
    data.gyroZ = (float)read16(buffer[12], buffer[13]) / 131.0f;

    // (Magnetometer requires separate setup — can add later)
    data.magX = data.magY = data.magZ = 0.0f;

    return true;
}

// Combine two bytes into a signed 16-bit value
int16_t MPU9250IMU::read16(uint8_t high, uint8_t low) {
    return (int16_t)((high << 8) | low);
}

// Write a single byte to the IMU
bool MPU9250IMU::writeByte(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = { reg, value };
    return i2c_write_blocking(i2c, address, buffer, 2, false) >= 0;
}

// Read multiple bytes starting from a register
bool MPU9250IMU::readBytes(uint8_t reg, uint8_t* buffer, size_t length) {
    if (i2c_write_blocking(i2c, address, &reg, 1, true) < 0) return false;
    return i2c_read_blocking(i2c, address, buffer, length, false) >= 0;
}
