#include "FlightController.h"

FlightController::FlightController(SensorFusion* sensor, ESCController* esc)
    : sensorFusion(sensor), escController(esc),
      pidRoll(1.0f, 0.0f, 0.05f),
      pidPitch(1.0f, 0.0f, 0.05f),
      pidYaw(1.0f, 0.0f, 0.02f) {}

void FlightController::init() {
    // ESCs already initialized externally
    // Sensor fusion already initialized externally
    pidRoll.reset();
    pidPitch.reset();
    pidYaw.reset();
}

void FlightController::setTargetAngles(float roll, float pitch, float yaw) {
    targetRoll = roll;
    targetPitch = pitch;
    targetYaw = yaw;
}

void FlightController::update(float dt) {
    const FusionData& state = sensorFusion->getData();

    // Compute corrections from PID
    float rollCorrection  = pidRoll.update(targetRoll, state.roll, dt);
    float pitchCorrection = pidPitch.update(targetPitch, state.pitch, dt);
    float yawCorrection   = pidYaw.update(targetYaw, state.yaw, dt);

    // Compute motor outputs
    float motor[4] = {
        baseThrottle - rollCorrection + pitchCorrection - yawCorrection,  // Front Left
        baseThrottle + rollCorrection + pitchCorrection + yawCorrection,  // Front Right
        baseThrottle + rollCorrection - pitchCorrection - yawCorrection,  // Back Right
        baseThrottle - rollCorrection - pitchCorrection + yawCorrection   // Back Left
    };

    // Clip motor outputs to [0.0, 1.0]
    for (int i = 0; i < 4; ++i) {
        if (motor[i] < 0.0f) motor[i] = 0.0f;
        if (motor[i] > 1.0f) motor[i] = 1.0f;
        escController->setThrottle(i, motor[i]);
    }
}
