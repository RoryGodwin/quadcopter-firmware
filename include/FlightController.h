#ifndef FLIGHT_CONTROLLER_H
#define FLIGHT_CONTROLLER_H

#include "SensorFusion.h"
#include "ESCController.h"

// Basic PID controller for one axis
struct PID {
    float kp, ki, kd;
    float integral = 0.0f;
    float previous_error = 0.0f;

    PID(float p = 1.0f, float i = 0.0f, float d = 0.1f)
        : kp(p), ki(i), kd(d) {}

    // Compute PID output
    float update(float target, float measured, float dt) {
        float error = target - measured;
        integral += error * dt;
        float derivative = (error - previous_error) / dt;
        previous_error = error;
        return kp * error + ki * integral + kd * derivative;
    }

    void reset() {
        integral = 0.0f;
        previous_error = 0.0f;
    }
};

class FlightController {
public:
    FlightController(SensorFusion* sensor, ESCController* esc);

    void init();
    void setTargetAngles(float roll, float pitch, float yaw);
    void update(float dt);

private:
    SensorFusion* sensorFusion;
    ESCController* escController;

    // Desired orientation
    float targetRoll = 0.0f;
    float targetPitch = 0.0f;
    float targetYaw = 0.0f;

    // PID controllers per axis
    PID pidRoll;
    PID pidPitch;
    PID pidYaw;

    // Throttle base level
    float baseThrottle = 0.5f;
};

#endif // FLIGHT_CONTROLLER_H
