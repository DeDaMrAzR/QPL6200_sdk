# Combo Door Window Sensor

## Introduction

Qorvo&reg; QPG6200 Combo Door Window Sensor Reference Application shows how to create a switch to arm the system when door or window is opened, with either Thread connectivity, using Matter&trade; or Zigbee 3.0 protocols in 2 separated applications within 1 chip. It is using Bluetooth&reg; LE to perform Matter&trade; commissioning. This example can be used as reference for creating your own Matter&trade; or Zigbee 3.0 switch.

Buttons are used as input to trigger device state changes, while LEDs are used as output to visualize the device states.

Features of this application are:
1. Bluetooth LE for Matter&trade; commissioning procedure.
2. Factory reset implementation based on number of reset cycles.

## Hardware Setup

![QPG6200 Carrier Board, Zigbee and Matter&trade;](.images/iot_carrier_board.png)

## Matter Application (Primary application)

### QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset Button|
| GPIO21| Input (Pull-up)| SW4| Toggle Door/Window state.|
| GPIO0| Input (Pull-up)| SW9| Multiple functionalities:<br /> - Trigger OTA - (released in 0-3s)<br /> - Factory Reset (released in 6-9s)<br /> - Application Switch (Press for >12s)|
| GPIO2| Output| LD8| Light status indicator:<br /> - **Short blink every 1s**: The device is in idle state (not commissioned yet and not Bluetooth LE advertising).<br /> - **Very fast blinks**: Bluetooth LE advertising.<br /> - **Fast blinks**: Bluetooth LE connected and subscribed but not yet commissioned.<br /> - **On**: Full service connectivity|

## Zigbee Application (Secondary application)

### QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset Button|
| GPIO29| Input (Pull-up)| SW3| Tamper button.|
| GPIO0| Input (Pull-up)| SW9| Multiple functionalities:<br /> - Commissioning or binding (release in 0-2s)<br /> - Start/Stop Touchlink commissioning (release in 2-4s)<br /> - Start/Stop Touchlink commissioning with reset-to-factory-new request (release in 4-6s)<br /> - Finding and Binding (release in 6-8s)<br /> - Factory reset (release in 10-12s)<br /> - Application switch (Press for >12s)|
| GPIO4| Input (Pull-up)| SW10| Change contact switch state.|
| GPIO11| Output| D6| Dimmable Color LED (Green) as Light status indicator:<br /> - **Very fast blinks**: Commissioning active.<br /> - **Fast blinks**: Associate permit joining. <br /> - **Off**: commissioned|

## Firmware architecture

The Combo Door Window application is composed of:

1. [Application Bootloader](../../Bootloader/README.md).
2. [Matter Door Window application](../../Matter/DoorWindow/README.md) (primary application).
3. [Zigbee Door Window application](../../Zigbee/DoorWindow/README.md) (secondary application).

The application components are merged together in the postbuild script file. This process creates a firmware that is programmed to the device. You can read more about merging application components in [Firmware structure](../../../Development/application_structure.rst#firmware-structure) chapter of this documentation.

### Memory map

Combo application has fixed memory map that has to be supported throughout the lifecycle of the device.

![QPG6200 Combo memory map, Zigbee and Matter&trade;](../.images/combo_memory_map_simplified.png)

## Logging output

First make sure the development kit is connected to your computer using the USB-C port on the IoT Dev Kit for QPG6200.
As mentioned above, a virtual COM port (`/dev/ttyACMx`) will be available for use.

Next you can use a serial terminal application to connect to the COM port. In this example, we will be using Minicom.
This tool can be installed using following command:

```
sudo apt-get install minicom
```
After the installation, start Minicom using following options:

```
minicom -D /dev/ttyACM0 115200
```

Please note that the COM port number and device label **may differ** on your computer.

After resetting the programmed QPG6200 (press the button `SW1 RADIO RESET`), you will see similar output as below:

```
CalibrationCreateTask: hal_TempSensorInit()
[WARN] Get dev address - stubbed
qvCHIP v0.0.0.0 ROMv0/0 (CL:0) r:3
[00:00:00.120][P][-] ============================
[00:00:00.120][P][-] Qorvo Application_Init
[00:00:00.123][P][-] ============================
[00:00:00.126][P][-] Current Software Version: 1.1
```

## Building and flashing

Please refer to the section [Installation Guide](../../../sphinx/docs/QUICKSTART.rst#install-the-sdk) to setup a build environment.

Please refer to the section [Building and flashing the example applications](../../../sphinx/docs/QUICKSTART.rst#working-with-examples) to get instructions on
how to build and program the Combo Door Window example application.

## Testing the example

The Combo Door Window application will not start any advertisement or commissioning at start-up if it is was not commissioned before.
Please follow the above button usage to start a commissioning event from either of the protocol.

The Matter commissioning procedure is done over Bluetooth LE where a connection is setup between a Matter device and a Matter
controller. This Matter controller takes the role of a commissioner.
The commissioner needs to get information from the Matter device to start the commissioning. This information can be
obtained by a QR code or from the serial output of the Matter device.

The final phase in the commissioning procedure is Thread provisioning. This involves sending the Thread network
credentials over Bluetooth LE to the Matter device. Once this is done, the device joins the Thread network and
communication with other Thread devices in the network can be achieved.

The Zigbee commissioning procedure is done by the Zigbee stack.
It will first look for a Zigbee network with permit-joining enabled to try to commission with it.
If commissioning fails, the application will form distributed security network and allow other Zigbee device to join into it.

### POSIX CLI chip-tool

For a commissioning guide that makes use of the POSIX cli chip-tool, please refer to [Commissioning Qorvo Matter device with POSIX CLI chip-tool](../../../Documents/User%20Manuals/Matter/commissioning_posix_cli_chiptool.rst)

### Google Ecosystem

For a commissioning guide that makes use of the Google ecosystem, please refer to
[Commissioning a Qorvo Matter™ end device in the Google ecosystem](../../../Documents/User%20Manuals/Matter/commissioning_with_google.rst).

### Apple Ecosystem

For a commissioning guide that makes use of the Apple ecosystem, please refer to
[Commissioning a Qorvo Matter™ end device in the Apple ecosystem](../../../Documents/User%20Manuals/Matter/commissioning_with_apple.rst).
