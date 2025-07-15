#include "pico/stdlib.h"
#include "ESCController.h"
#include "PWMESCController.h"

//#include "MotorController.h"

int main() {
    // Enable USB serial output (for debugging)
    stdio_init_all(); 

    // Create an instance of PWMESCController with GPIOs 2 - 5
    // These pins should connect to the ESC signal wires
    PWMESCController esc({2, 3, 4, 5})

    // Initialise all motors: configure PWM and arm ESCs
    esc.init();

    // Loop:ramp up and down motor 0 every 2 seconds 
    while (true)
    {
        // Set motor 0 to throttle to 50% 
        esc.setThrottle(0, 0.5);
        sleep_ms(2000);

        // Stop motor 0
        esc,setThrottle(0, 0.0);
        sleep_ms(2000);
    }
    return 0;
}    
