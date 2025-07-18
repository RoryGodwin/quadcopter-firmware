#ifndef MADGWICK_FILTER_H
#define MADGWICK_FILTER_H

// Madgwick filter computes orientation (quaternion) using IMU data
// See: https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/

#include <cmath>

class MadgwickFilter {
public:
    MadgwickFilter(float beta = 0.1f);  // Beta controls responsiveness (0.1 = good balance)

    void reset();
    void update(float gx, float gy, float gz,
                float ax, float ay, float az, float dt);

    // Access roll/pitch/yaw in degrees
    float getRoll() const;
    float getPitch() const;
    float getYaw() const;

private:
    float beta;  // Algorithm gain
    float q0, q1, q2, q3;  // Quaternion state

    float invSqrt(float x);
};

#endif // MADGWICK_FILTER_H
