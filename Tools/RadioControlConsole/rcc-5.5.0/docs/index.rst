.. Radio Control Console documentation master file, created by
   sphinx-quickstart on Wed Sep  6 12:42:14 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Radio Control Console's documentation!
=================================================


The Radio Control Console is an application that provides control over a Qorvo
device running a PTC or CTC application.


Quick-Start - CLI
=================

To control the device via the CLI interface, execute `python -m qorvo.rcc -c COM10 -b 57600`.
The result should be a message giving the connected DUT's information, like below.
Check the complete command set in :doc:`cli_commands/summary`.


.. image:: assets/sample_rcc_session.png


Jadelogger - Log Watcher
========================

The package also includes a "logwatcher" CLI application.
It is a simple and limited implementation of Qorvo's Jadelogger tool, that is packaged for user convinience.

The tool can be invoked by:

   * The package-installed `rcc-log-watch` executable
   * The package directy with `python3 -m qorvo.rcc.cli.logwatch`

The tool takes the same connection arguemnts as the main executable, namely: `-c` and `-b` or `-e` and `-p`.
It can also take a `-d` option with an integer value corresponding to the desired debug level (0 is the most verbose).
Lastly the `--ignore-crc-errors` option flag can be used on corner-cases where the firmware sends logging messages with corrupted CRC values.

Example usage and output:

.. code-block:: text

   python3 -m qorvo.rcc.cli.logwatch -c com7 -b 115200
   INFO:qorvo.rcc.transport.jadelogger:== Connecting to PEAK - logfile: None ==
   2024-05-23-12:19:14.656  01 10.005472 hb !!
   2024-05-23-12:19:24.611  01 20.005600 hb !!
   2024-05-23-12:19:34.620  01 30.005728 hb !!
   2024-05-23-12:19:44.600  01 40.005824 hb !!
   2024-05-23-12:19:54.641  01 50.005952 hb !!
   2024-05-23-12:20:04.601  01 60.006080 hb !!


Quick-Start - Scripting
=======================

The Radio Control Console application also exposes an API for direct Python scritping.

.. code-block:: python

   # Import the package
   from qorvo.rcc.ptc_versions.manager import connect_to_ptc_driver
   from qorvo.rcc.transport.peakconnect import ConenctAndStart
   # Then connect to a DUT with an object
   ptc = connect_to_ptc_driver(ConnectAndStart('COM10:57600'))
   # Use the connection! Get DUT information
   print(ptc.get_device_info())
   # Set a continuous wave
   ptc.start_tx_carrier_wave()


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   cli_commands/summary
   scripting_api/qorvo.rcc


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
