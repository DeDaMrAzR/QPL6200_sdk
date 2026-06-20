# Matter Door Window

## Introduction

Qorvo&reg; QPG6200 Matter door/window example shows how to create door/window sensor, with Thread
connectivity, using the Matter protocol. It is using Bluetooth&reg; LE to perform Matter&trade; commissioning. This example can be used as reference for
creating your own Matter door/window sensor.

Buttons are used as input to trigger device state changes, LEDs are used as output to visualize the device states.

Features of this application are:
1. Simulate the contact sensor by using a button.
2. Bluetooth LE for Matter commissioning procedure.
3. Factory reset implementation based on long pressing button.

## Hardware Setup
![QPG6200 Carrier Board, Matter](.images/matter.png)

## QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset Button|
| GPIO0| Input (Pull-up)| SW9| Multiple functionalities: <br />- Trigger OTA - (released 0-3s) <br />- Factory Reset (released after 6s)|
| GPIO21| Input (Pull-up)|SW4| Door/Window sensor state input: <br />- Closed: Press and hold <br />- Open: Button released|
| GPIO2| Output| LD7| Matter status indicator <br />- **Short blink every 1s**: The device is in idle state (not commissioned yet and not Bluetooth LE advertising).<br />- **Very fast blinks**: Bluetooth LE advertising.<br />- **Fast blinks**: Bluetooth LE connected and subscribed but not yet commissioned.<br />- **On**: Full service connectivity|

## Usage


### Logging output

  > The Matter application does not use Jadelogger to retrieve its serial logging.

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
qvCHIP v0.0.0.0 ROMv0/0 (CL:0) r:3
[P][DL] BLEManagerImpl::Init() complete
[P][-] Initializing OpenThread stack
[P][DL] OpenThread started: OK
[P][DL] Setting OpenThread device type to SLEEPY END DEVICE
[P][-] Starting OpenThread task
[P][-] Starting Platform Manager Event Loop
[P][-] ============================
[P][-] Qorvo DoorWindow-Sensor-app Launching
[P][-] ============================

```

### Building and flashing

See [the installation guide](../../../sphinx/docs/QUICKSTART.rst#install-the-sdk) to setup a build environment.<br />
See [Building and flashing the example applications](../../../sphinx/docs/QUICKSTART.rst#working-with-examples) section to get instructions how to build and program the Matter door/window example application.

### Testing the example

The Matter door/window example will start Bluetooth LE advertising automatically at start-up if it is was not commissioned before
in a fabric. If it is advertising, it is discoverable for a Matter controller to start the Matter commissioning over Bluetooth LE.

The commissioning procedure is done over Bluetooth LE where a connection is setup between a Matter device and a Matter
controller. This Matter controller takes the role of a commissioner.
The commissioner needs to get information from the Matter device to start the commissioning. This information can be
obtained by a QR code or from the serial output of the Matter device.

The final phase in the commissioning procedure is Thread provisioning. This involves sending the Thread network
credentials over Bluetooth LE to the Matter device. Once this is done, the device joins the Thread network and
communication with other Thread devices in the network can be achieved.

#### POSIX CLI chip-tool

For a commissioning guide that makes use of the POSIX cli chip-tool, please refer to [Commissioning Qorvo Matter device with POSIX CLI chip-tool](../../../Documents/User%20Manuals/Matter/commissioning_posix_cli_chiptool.rst)

#### Google Ecosystem

For a commissioning guide that makes use of the Google ecosystem, please refer to
[Commissioning a Qorvo Matter™ end device in the Google ecosystem](../../../Documents/User%20Manuals/Matter/commissioning_with_google.rst).

#### Apple Ecosystem

For a commissioning guide that makes use of the Apple ecosystem, please refer to
[Commissioning a Qorvo Matter™ end device in the Apple ecosystem](../../../Documents/User%20Manuals/Matter/commissioning_with_apple.rst).
