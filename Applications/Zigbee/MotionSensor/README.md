# Zigbee Motion Sensor

## Introduction

The MotionSensor Reference Application demonstrates the usage of the QPG6200 Software Development Kit (SDK) to implement a Zigbee 3.0 motion sensor which is composed of the occupancy sensor, the On/Off sensor and the light sensor endpoints. Buttons are used as input to trigger device state changes, while LEDs are used as output to visualize the device states. <br>
Low power supply (LPS) standby power mode is used in this sleepy application by default. The standby mode is configurable within the application.

## Hardware Setup

![QPG6200 Carrier Board, Zigbee and Matter&trade;](.images/qpg6200dk.png)

## QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset button|
| GPI29| Input (Pull-up)| SW3| Occupancy button|
| GPIO3| Input (Pull-up)| SW2| Illuminance button|
| GPIO21| Input (Pull-up)| SW4| Control mode button|
| GPIO0| Input (Pull-up)| SW9| Time-sensitive multi-function button|
| GPIO11| Output | D6| Status LED (Green)|

## Usage

| Item                                   | Action |Result                                                                     |
| -------------------------------------- | ---------------------------------------------------------------------------------- |---------------------------------------------------------------------------------- |
| Start up | Power On Device |The application restarts.<br />Network information and NVM data will be restored.<br />Device will resume existing network if already commissioned. Otherwise, device will start Zigbee commissioning afterward.|
| Power Reset   | Press reset button 1 time| Power-On reset device.|
| Factory Reset |  Press reset button in succession for 10 times (around 1 ~ 2 second between each press)| Device performs a factory reset, network information and NVM data will be cleared.|
| Zigbee Touchlink Commissioning | Press reset button in succession for 7 times (around 1 ~ 2 second between each press) | Start a Zigbee Touchlink commissioning event.|
| Zigbee Touchlink Target | Press reset button in succession for 9 times (around 1 ~ 2 second between each press) | Enable Zigbee Touchlink target.|
| Zigbee Finding & Binding | Press reset button in succession for 8 times (around 1 ~ 2 second between each press) | Start Zigbee Finding & Binding.|
| Time-sensitive Multi-function | Long press (0s - 2s)| Start a Zigbee Commissioning event.|
|                               | Long press (2s - 4s)| Start a Zigbee TouchLink Commissioning event.|
|                               | Long press (4s - 6s)| Start a Zigbee TouchLink Commissioning event with reset-to-factory-new request.|
|                               | Long press (6s - 8s)| Start a Zigbee Finding & Binding.|
|                               | Long press (>10s)| Device performs a factory reset, network information and NVM data will be cleared.|
| Occupancy | Press the occupancy button | Toggle occupancy state (occupied/unoccupied).<br />If the state is changed from unoccupied to occupied, device will send OnWithTimedOff command and report illuminance measured value.|
| Illuminance | Press the illuminance button | Toggle illuminance measured value (MinMeasuredValue/MaxMeasuredValue). |
| Control mode | Press the control mode button | Toggle destination of commands (group/bind address). |

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
============================
Qorvo Application_Init
============================
[BTN] Added GPIO 0
[BTN] Added GPIO 29
[BTN] Added GPIO 3
[BTN] Added GPIO 21
[BTN] Polling GPIO...
GPIO 0 is now time sensitive.
Stack register: c154
Unknown stack, register 0! new stackId: 0x0
ZIGBEE: Device Type - Sleepy End Device
ZIGBEE: Device is NOT in network
============================
```

## Building and flashing

Please refer to the section [Installation Guide](../../../sphinx/docs/QUICKSTART.rst#install-the-sdk) to setup a build environment.

Please refer to the section [Building and flashing the example applications](../../../sphinx/docs/QUICKSTART.rst#working-with-examples) to get instructions on
how to build and program the Sleepy light switch example application.
