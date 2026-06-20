.. include:: <isonum.txt>

.. _shared_button_handler:

Button Handler
==============

The Button Handler module file is responsible for managing button inputs.
It provides the implementation for handling button presses and releases, and triggering appropriate application callbacks based on button actions.
It embeds button debouncing mechanism. This allows the application to respond to user interactions through physical buttons.

Implementation details
----------------------

The module uses the Qorvo\ |reg| GPIO driver to handle buttons.
The embedded debouncing mechanism is based on a ``gpScheduler`` and freeRTOS timer task.
Once, the Qorvo\ |reg| GPIO driver detects a ping change, the module defers the processing function to a timer task.
This is done to avoid prolonged processing of interrupt.
Once the timer task gets processing time, the ``gpScheduler`` is used to further delay pin state change.
This is done to the button pin is settled before the module checks the pin state.
The delay of the ``gpScheduler`` is set to 20ms by default.
You can be change the debouncing time by changing the `BUTTON_DEBOUNCE_PERIOD_MS` macro in the ``ButtonHandler.c`` file.

Usage
-----

To use the module, complete the following steps:

1. Include the Button Handler module in the application.
#. Define application callback function that will be called when a button is pressed or released.
#. Initialize the Button Handler module.


An example code snippet is shown below:

.. code-block:: c

    #include "ButtonHandler.h"

    #define BUTTON_0 0
    #define BUTTON_1 5

    static const ButtonConfig_t buttons[] = {BUTTON_0, BUTTON_1};

    void ButtonHandler_callback(uint8_t btnIdx, bool btnPressed)
    {
        // process buttons press and release
    }

    int main(void)
    {
        ButtonHandler_Init(buttons, Q_ARRAY_SIZE(buttons), ButtonHandler_callback);

        return 0;
    }
