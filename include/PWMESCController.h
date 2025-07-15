#ifndef PWM_ESC_CONTROLLER_H
#define PWM_ESC_CONTROLLER_H

#include "ESCCONTROLLER.h"
#include <vector>

// PWMESCController is a spesific implementation of the ESCController interface.
// It controls motors using standard PWM signals, 

class PWMESCController : public ESCController {
public:
    // constructor takes a list of GPIO pins connected to ESC signal wires
    PWMESCController(const std::vector<int>& Pins);


    // Implements the interface's init() method.
    // It sets up the RPi Pico's GPIOs to output PWM and arems the ESCs
    void init() override;

    // Implements the interface's setThrottle() method.
    // It sends a PWM signal corresponding to the requested throttle value.
    void setThrottle(int motorIndex, float throttle) override;

private:
    //motoRPins stores which GPIO pins control which motors.
    // This is private to encapsulate the internal logic
    std::vector<int> motorPins;

    // send an initial 1ms PWM signal to arm the ESC
    void armESC(int pin);
    
    // Converts throttle value to PWM duty cycle and send it)
    void writePWM(int pin, float throttle);

};

// PWM_ESC_CONTROLLER_H
#endif 
