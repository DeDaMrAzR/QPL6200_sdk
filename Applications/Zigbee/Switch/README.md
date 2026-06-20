# Zigbee Switch

## Introduction

The Sleepy light switch Reference Application demonstrates the usage of the QPG6200 Software Development Kit (SDK) to implement a Zigbee 3.0 light switch application. Buttons are used as input to trigger device state changes, while LEDs are used as output to visualize the device states. <br>
Low power supply (LPS) standby power mode is used in this sleepy application by default. The standby mode is configurable within the application.

## Hardware Setup

![QPG6200 Carrier Board, Zigbee](.images/iot_carrier_board.png)

## QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset Button|
| GPIO3| Input (Pull-up)| SW2| Level control button:<br /> - Toggle between 2 pre-set dimming level.|
| GPIO29| Input (Pull-up)| SW3| Color control button:<br /> - Toggle between 2 pre-set color.|
| GPIO5| Input (Pull-up)| SW5| Press/Release control button:<br /> - Toggle binding grouping mode.|
| GPIO0| Input (Pull-up)| SW9| Multiple functionalities:<br /> - Commissioning or binding (release in 0-2s)<br /> - Start/Stop Touchlink commissioning (release in 2-4s)<br /> - Start/Stop Touchlink commissioning with reset-to-factory-new request (release in 4-6s)<br /> - Finding and Binding (release in 6-8s)<br /> - Factory reset (release after >10s)|
| GPIO11| Output| D6| Dimmable Color LED (Green) as Light status indicator:<br /> - **Very fast blinks**: Commissioning active.<br /> - **Fast blinks**: Associate permit joining. <br /> - **Off**: Comissioned|


## Logging output

First make sure the development kit is connected to your computer using the USB-C port on the IoT Dev Kit for QPG6200.
As mentioned above, a virtual COM port (`/dev/ttyACMx`) will be available for use.

Next you can use a serial terminal application to connect to the COM port. In this example, we will be using Minicom.
This tool can be installed using following command:

```
sudo apt-get install minicom
```
After the installation, start minicom using following options:

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
how to build and program the Sleepy light switch example application.
