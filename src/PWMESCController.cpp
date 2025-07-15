#include "PWMESCController.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Controller stores the GPIO pins to control each ESC/motor
// Pass the pin value (copies into the class) for simplicity.
PWMESCController::PWMESCController(const std::vector<int>&pins)
{
    motorPins = pins
}

// Initilise the PWM output on each pin and arm the ESCs.
// This function is called once during startup.
void PWMESCController::init()
{
    for (int pin : motorPins)
    {
        // Set the pin function to PPWM (default is digital I/O)
        gpio_set_function(pin, GPIO_FUNC_PWM);

        // Get PWM slice number for the pin (PWM is grouped in slices)
        uint slice = pwm_gpio_to_slice_num(pim);

        // Set PWM frequancy to 50Hz which  the ESC expects
        // 125MHz / 125.0 = 1.25MHz PWM clock -> wrap at 25000 ticks gives 50Hz
        pwm_set_wrap(slice, 24999);
        pwm_set_clkdiv(slice, 125.0f);

        // Enable the PWM slice to start outputting
        pwm_set_enabled(slice, true)

        // Send 1ms pulse to let ESC initilise (arming)
        remESC(pin);

    }
}

// Arming the is a safty step to prepare it to receive throttle.
// send a "neutral" 1ms pulse (standard arming signal.)
void PWMESCController::armESC(int pin)
{
    // minimum throttke  = 0.0 (1ms pulse)
    writePWM(pin, 0.0f);
    // Give ESC time to arm
    sleep_ms(1000);
}

// Set throttle of a spesific motor
// this func ensures throttle is within safe bounds and passes it to writePWM.
void PWMESCController::setThrottle(int motorIndex, float throttle)
{
    if (motorIndex < 0 || motorIndex >= (int)motorPins.size()) return;

        // Clamp throttle to safe range (0.0 to 1.0)
        if (throttle < 0.0f) throttle = 0.0f;
        if (throttle > 1.0f) throttle = 1.0f;

        writePWM(motorPins[motorIndex], throttle);
}

// Convert throttle to PWM signal (1ms to 2ms pulse) and set duty cycle
void PWMESSCController::writePWM(int pin, float throttle)
{
    uint slice = pwm_gpio_to_slice_num(pin);
    uint channel = pwm_gpio_to_channel(pin);

    // ESC 1-2ms (min-max) 
    float minDuty = 0.05f; // 5%
    float maxDuty = 0.1f // 10%
    float duty = minDuty + (maxDuty - minDuty) * throttle;

    // Convert duty cycle to 0 - 25000 (PWM wrap value)
    uint level = static_cast<uint>(duty * 25000);

    // Apply the calculated duty cycle to the approptiate PWM channel
    pwm_set_chan_level(slice, channel, level);

}