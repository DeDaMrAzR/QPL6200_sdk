.. _shared_status_led:

Status LED
==========

The status LED module is responsible for managing the status LEDs directly connected to GPIO pins.
This includes controlling the LEDs to indicate various states or events within the application.
The module provides an API to set the LED on or off, or to blink the LED with a specified on and off time.

Implementation details
----------------------

The module uses the GPIO driver to control the status LEDs.
For LED effects like blinking, the module uses a gpScheduler timer to toggle the LED state at the specified interval.

Usage
-----

To use the module, complete the following steps:

1. Include the StatusLed module in the application.
#. Initialize the StatusLed module.
#. Use the provided API to control the status LEDs.

.. code-block:: c

    #include "StatusLed.h"

    #define STATUS_LED_0 10
    #define STATUS_LED_1 11

    static const uint8_t leds[] = {STATUS_LED_0, STATUS_LED_1};

    int main(void)
    {
        StatusLed_Init(leds, Q_ARRAY_SIZE(leds), false);

        StatusLed_SetLed(STATUS_LED_0, true);
        StatusLed_BlinkLed(STATUS_LED_1, 1000, 500);

        return 0;
    }
