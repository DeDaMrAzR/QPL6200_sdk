.. include:: <isonum.txt>

.. _shared_app_battery:

App Battery
===========

The AppBattery module is responsible for handling internal battery measurement.
The module provides an API to start/stop the battery measurements and to get the battery level.
The module also handles battery updates to the Zigbee endpoint attribute.
Additionally it provided a reference implementation for ZDO Power Descriptor request.

Implementation details
----------------------

The module utilizes Qorvo\ |reg| drivers to perform battery measurements and ``gpScheduler`` for periodic battery level updates.
Once started, the module will periodically measure the battery level and update the Zigbee endpoint attribute.
The time interval for battery measurement and update is set to 5 minutes by default and can be changed by modifying the `APP_BATTERY_UPDATE_PERIOD` macro in the ``AppBattery.cpp`` file.

Usage
-----

To use the module, complete the following steps:

1. Include the AppBattery module in the application.
#. Start the battery measurement.

An example code snippet is shown below:

.. code-block:: c

    #include "AppBattery.h"

    int main(void)
    {
        Application_StartBatteryMeasurement();

        return 0;
    }
