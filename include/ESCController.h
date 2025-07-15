#ifndef ESC_CONTROLLER_H
#define ESC_CONTROLLER_H

// ESCController os am abstract base class (also known as a interface).
// it defines a common set of methods that all ESC controllers must implement.
// this lets us create different types of ESC controllers (PWM, PPM, CAN, etc.)
// that can be used interchangably in the rest of the system.

class ESCController {
public:
    // Pure virtual method: must be implemented by derived classes
    // This method should handle all setup needed by derived classes
    // such as configuring GPIOs or arming the ESCs
    virtual void init() = 0;

    // pure virtual method to set throttle for a spesific motor
    // motorIndex = 0 - n depending on how many motors are used
    // throttle is normalised to values 0.0 - 1.0 (min-max)
    virtual void setThrottle(int motorIndex, float throttle) = 0;

    // Virtual destructor allows proper cleanup if derived class is deleted via a base class pointer.
    virtual ~ESCController() = default;

};
// ESC_CONTROLLER_H
#endif  