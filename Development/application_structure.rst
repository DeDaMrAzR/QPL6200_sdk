Firmware structure
******************

The firmware that is an output of the build stage can be composed of either one or multiple applications. These applications are merged into the final firmware in the postbuild script file using the `Hexmerge tool <../Tools/Hex/README.md#hexmerge>`_.

The most common use case to build a composed firmware is when an application requires a bootloader to boot or when the application is composed of more than one image. For example, Combo applications are composed of Bootloader, Matter\ :superscript:`TM`, and Zigbee applications that are merged together in the postbuild script file.

Furthermore, if an application is composed of additional components, the `Hexmerge tool <../Tools/Hex/README.md#hexmerge>`_ is used to merge those components into the application. For example, the Matter\ :superscript:`TM` application requires Factory Data. Once the Factory Data is generated, it is merged into the application.

The postbuild script is called directly from the makefile at the end of the build process. This ensures that all necessary components are correctly merged and the final firmware is properly structured.

An example of the firmware structure is shown in the image below:

.. image:: ./.images/firmware_structure.png
   :alt: Firmware structure

An example of the firmware structure for Combo applications is shown in the image below:

.. image:: ./.images/combo_firmware_structure.png
   :alt: Combo application firmware structure

Firmware signing
****************

Image signing is one of the steps of the application build process. Each application that composes a firmware is signed. When the application is booted, the `application bootloader <../Applications/Bootloader/README.md#application-bootloader>`_ authenticates the image so it can be safely booted. To read more about the authentication process during boot, refer to `Application Launch Process <../Applications/Bootloader/README.md#application-launch-process>`_.

The signing of the application is done before application components are merged to the final firmware. This is done, because some application components that are required for the application to run are changed when the device is running. For example, signing of the Matter\ :superscript:`TM` application is done before Factory Data block is merged to the final firmware.

To sign an application, use the `QorvoPlatformTools <../Tools/QorvoPlatformTools/README.md#qorvo-platform-tools>`_. You can follow the
`Security Tutorial - Sign an Application <../Documents/User_Journey/Security/Security_User_Journey.md#test-sign-an-application>`_.
