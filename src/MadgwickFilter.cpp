#include "MadgwickFilter.h"

MadgwickFilter::MadgwickFilter(float beta) : beta(beta) {
    reset();
}

void MadgwickFilter::reset() {
    q0 = 1.0f;
    q1 = q2 = q3 = 0.0f;
}

float MadgwickFilter::invSqrt(float x) {
    return 1.0f / sqrtf(x);  // Efficient approximation possible, but we use stdlib
}

void MadgwickFilter::update(float gx, float gy, float gz,
                            float ax, float ay, float az, float dt) {
    float qDot1, qDot2, qDot3, qDot4;
    float s0, s1, s2, s3;
    float norm;

    // Normalize accelerometer
    norm = sqrtf(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return;
    ax /= norm;
    ay /= norm;
    az /= norm;

    // Rate of change of quaternion from gyro
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Gradient descent corrective step
    s0 = q0 * q2 - q1 * q3;
    s1 = q0 * q3 + q1 * q2;
    s2 = q0 * q0 - 0.5f + q3 * q3;
    s3 = q1 * q1 + q2 * q2;

    // Normalize step magnitude
    norm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
    s0 *= norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;

    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;

    // Integrate to get quaternion
    q0 += qDot1 * dt;
    q1 += qDot2 * dt;
    q2 += qDot3 * dt;
    q3 += qDot4 * dt;

    // Normalize quaternion
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;
}

// Convert quaternion to Euler angles
float MadgwickFilter::getRoll() const {
    return atan2f(2.0f * (q0*q1 + q2*q3), 1.0f - 2.0f * (q1*q1 + q2*q2)) * 180.0f / M_PI;
}

float MadgwickFilter::getPitch() const {
    float sinp = 2.0f * (q0*q2 - q3*q1);
    if (fabsf(sinp) >= 1)
        return copysignf(90.0f, sinp);
    return asinf(sinp) * 180.0f / M_PI;
}

float MadgwickFilter::getYaw() const {
    return atan2f(2.0f * (q0*q3 + q1*q2), 1.0f - 2.0f * (q2*q2 + q3*q3)) * 180.0f / M_PI;
}
