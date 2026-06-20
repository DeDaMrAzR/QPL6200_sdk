.. include:: <isonum.txt>

.. _shared_reset_count:

Reset Count
===========

The Reset Count module is a simple module to keep track of the number of resets that have occurred since the device was powered on.
It provides the implementation for counting the number of resets and storing the count in non-volatile memory.
The module notifies the application with the reset count value with ``AppEvent``.
The event is scheduled with the `RESET_COUNT_TIMEOUT_US` delay since the initialization.

Implementation details
----------------------

The module uses the Qorvo\ |reg| NVM driver to store the reset count in non-volatile memory.

Usage
-----

To use the module, complete the following steps:

1. Include the Reset Count module in the application.
#. Initialize the Reset Count module.


An example code snippet is shown below:

.. code-block:: c

    #include "ResetCount.h"

    int main(void)
    {
        ResetCount_Init();

        return 0;
    }
