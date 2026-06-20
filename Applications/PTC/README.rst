Product Test Component
**********************

Introduction
============

This introduction manual provides an overview of the Qorvo® Product Test
Component(PTC) System and its intended usage. The PTC system targets to
enable RF validation, RF production test and PHY and MAC certification
measurements of the Qorvo IEEE802.15.4 and Bluetooth LE Silicon. After
reading this guide you will have a general understanding of the
different blocks in this system and will be aware of the references
towards detailed documentation of the individual blocks.

Bluetooth LE Direct Test Mode
=============================

To enable Direct Test Mode, please put following jumpers:

-  **J15** –> Configure UART pins for external logging
-  **J22** –> Decouple UART logging over USB

Apply power to the board via the USB C port **J35**. An FTDI cable can
now be connected to **J1**:

-  **FTDI** TX –> UART RX (``GPIO 8``)
-  **FTDI** RX –> UART TX (``GPIO 9``)
-  **FTDI GND**

.. raw:: html

   <link rel="stylesheet" href="../../_static/board_layout/rst_helpers/rst_board_layout.css">
   <script type="text/javascript" src="../../_static/board_layout/rst_helpers/rst_board_layout.js"></script>
   <script>
     // Title to display in the new window interactive view
     const newWindowTitle = "Product Test Component"
     // List of elements to show. Use on elements hidden in the SVG, like tags.
     const labelsToShow = [
						'J15',
						'J22',
						'J35',
						'J1',
						'GPIO8',
						'GPIO9',
						'FTDI'
						]
     // List of elements to hide. Use on visible elements, like Effects or Components.
     const labelsToHide = []
     // Execute JS after page loads completely
     document.addEventListener('DOMContentLoaded', function() {
         // Display and hide SVG elements
         modifySVG(labelsToShow, labelsToHide);
         // Modify New Window link to pass arguments that show/display elements
         newWindowUrl(labelsToShow, labelsToHide, newWindowTitle);
     });
   </script>
   <div class="svg-container">
        <object class="svg-content" type="image/svg+xml" data="../../_static/board_layout/gp_p346_hw_20344_iot_carrier_board_pcb11.svg" id="svgObject"></object>
        <div>
            <span class="svglink svglink-green">
               <a id="board-layout-new-window" class="svglink-button" href="../../_static/board_layout/interactive_view/board_layout.html" target="_blank"> Interactive view </a>
            </span>
            <span class="svglink svglink-blue">
               <a class="svglink-button" href="../../_static/board_layout/gp_p346_hw_20344_iot_carrier_board_pcb11.svg" download> Download </a>
            </span>
        </div>

   </div>

Documentation
=============

For more documentation, including a QuickStart Guide, please see :doc:`Radio
Control Console's documentation <../../Tools/RadioControlConsole/rcc-5.5.0/docs/index>`
