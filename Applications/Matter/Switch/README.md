# Matter Switch

## Introduction

Qorvo&reg; QPG6200 Matter switch example shows how to create a remotely controlled dimmable color light bulb, with Thread
connectivity, using the Matter protocol. It is using Bluetooth&reg; LE to perform Matter&trade; commissioning. This example can be used as reference for
creating your own Matter light bulb.

Buttons are used as input to trigger device state changes, LEDs are used as output to visualize the device states.

Features of this application are:
1. Remote lighting control (dimming, color, on/off) using a Matter controller.
2. Bluetooth LE for Matter commissioning procedure.
3. Factory reset implementation based on number of reset cycles.

## Hardware Setup
![QPG6200 Carrier Board, Matter](.images/iot_carrier_board.png)

## QPG6200 GPIO Configurations

| GPIO Name| Direction| Connected To| Comments|
|:----------:|:----------:|:----------:|:---------|
| RESETN| Input| SW1| Reset Button|
| GPIO3| Input (Pull-up)| SW2| Toggle the light between two colors|
| GPIO29| Input (Pull-up)| SW3| Toggle the light between two levels|
| GPIO21| Input (Pull-up)| SW4| Switch Press/ Release control|
| GPIO5| Input (Pull-up)| SW5| Toggle the light on/off|
| GPIO0| Input (Pull-up)| SW9| Multiple functionalities:<br /> - Trigger OTA - (released in 0-3s)<br /> - Factory Reset (released after >6s)<br />|
| GPIO2| Output| LD8| Light status indicator:<br /> - **Short blink every 1s**: The device is in idle state (not commissioned yet and not Bluetooth LE advertising).<br /> - **Very fast blinks**: Bluetooth LE advertising.<br /> - **Fast blinks**: Bluetooth LE connected and subscribed but not yet commissioned.<br /> - **On**: Full service connectivity|

## Usage

### Logging output

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
qvCHIP v0.0.0.0 ROMv1/1 (CL:0) r:3
[P][DL] BLEManagerImpl::Init() complete
[P][-] Initializing OpenThread stack
[P][DL] OpenThread started: OK
[P][DL] Setting OpenThread device type to SLEEPY END DEVICE
[P][-] Starting OpenThread task
[P][-] Starting Platform Manager Event Loop
[P][-] ============================
[P][-] Qorvo Light-Switch-app Launching
[P][-] ============================

```

### Building and flashing

See [the installation guide](../../../sphinx/docs/QUICKSTART.rst#install-the-sdk) to setup a build environment.<br />
See [Building and flashing the example applications](../../../sphinx/docs/QUICKSTART.rst#working-with-examples) section to get instructions how to build and program the Matter light example application.

### Testing the example

The Matter switch will start Bluetooth LE advertising automatically at start-up if it is was not commissioned before
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

### Binding with Matter light device

Make sure both Light and Switch device are commissioned to the same network.

Assuming that the NodeID of the devices are set to the followings
- Matter Light device: NodeID = `#LightNodeId#`
- Matter Switch device: NodeID = `#SwitchNodeId#`

1. Write binding info to the Switch app
```
./chip-tool.elf binding write binding '[{"fabricIndex": 1, "node": 1, "endpoint": #LightNodeId#, "cluster": 6}, {"fabricIndex": 1, "node": 1, "endpoint": #LightNodeId#, "cluster": 8}]' #SwitchNodeId# 1
```

2. Confirm that the binding info is written successfully
```
./chip-tool.elf binding read binding #SwitchNodeId# 1
```
Output:
```
[1666874049.111186][11048:11053] CHIP:TOO: Endpoint: 1 Cluster: 0x0000_001E Attribute 0x0000_0000 DataVersion: 1511515605
[1666874049.111440][11048:11053] CHIP:TOO:   Binding: 2 entries
[1666874049.111458][11048:11053] CHIP:TOO:     [1]: {
[1666874049.111467][11048:11053] CHIP:TOO:       Node: #LightNodeId#
[1666874049.111472][11048:11053] CHIP:TOO:       Endpoint: 1
[1666874049.111475][11048:11053] CHIP:TOO:       Cluster: 6
[1666874049.111480][11048:11053] CHIP:TOO:       FabricIndex: 1
[1666874049.111484][11048:11053] CHIP:TOO:      }
[1666874049.111488][11048:11053] CHIP:TOO:     [2]: {
[1666874049.111492][11048:11053] CHIP:TOO:       Node: #LightNodeId#
[1666874049.111494][11048:11053] CHIP:TOO:       Endpoint: 1
[1666874049.111497][11048:11053] CHIP:TOO:       Cluster: 8
[1666874049.111502][11048:11053] CHIP:TOO:       FabricIndex: 1
[1666874049.111506][11048:11053] CHIP:TOO:      }
```

3. Write access control info to the Light app
```
./chip-tool.elf accesscontrol write acl '[{"fabricIndex": 1, "privilege": 5, "authMode": 2, "subjects": [112233], "targets": null}, {"fabricIndex": 1, "privilege": 3, "authMode": 2, "subjects": [#SwitchNodeId#], "targets": [{"cluster": 6, "endpoint": 1, "deviceType": null}, {"cluster": 8, "endpoint": 1, "deviceType": null}]}]' #LightNodeId# 0
```

4. Confirm that the access control is written successfully
```
./chip-tool.elf accesscontrol read acl #LightNodeId# 0
```
Output:
```
[1666874500.590545][11235:11240] CHIP:TOO: Endpoint: 0 Cluster: 0x0000_001F Attribute 0x0000_0000 DataVersion: 2344203767
[1666874500.590628][11235:11240] CHIP:TOO:   ACL: 2 entries
[1666874500.590658][11235:11240] CHIP:TOO:     [1]: {
[1666874500.590670][11235:11240] CHIP:TOO:       Privilege: 5
[1666874500.590680][11235:11240] CHIP:TOO:       AuthMode: 2
[1666874500.590692][11235:11240] CHIP:TOO:       Subjects: 1 entries
[1666874500.590710][11235:11240] CHIP:TOO:         [1]: 112233
[1666874500.590723][11235:11240] CHIP:TOO:       Targets: null
[1666874500.590733][11235:11240] CHIP:TOO:       FabricIndex: 1
[1666874500.590743][11235:11240] CHIP:TOO:      }
[1666874500.590773][11235:11240] CHIP:TOO:     [2]: {
[1666874500.590783][11235:11240] CHIP:TOO:       Privilege: 3
[1666874500.590792][11235:11240] CHIP:TOO:       AuthMode: 2
[1666874500.590803][11235:11240] CHIP:TOO:       Subjects: 1 entries
[1666874500.590815][11235:11240] CHIP:TOO:         [1]: #SwitchNodeId#
[1666874500.590833][11235:11240] CHIP:TOO:       Targets: 2 entries
[1666874500.590870][11235:11240] CHIP:TOO:         [1]: {
[1666874500.590879][11235:11240] CHIP:TOO:           Cluster: 6
[1666874500.590889][11235:11240] CHIP:TOO:           Endpoint: 1
[1666874500.590899][11235:11240] CHIP:TOO:           DeviceType: null
[1666874500.590908][11235:11240] CHIP:TOO:          }
[1666874500.590925][11235:11240] CHIP:TOO:         [2]: {
[1666874500.590933][11235:11240] CHIP:TOO:           Cluster: 8
[1666874500.590942][11235:11240] CHIP:TOO:           Endpoint: 1
[1666874500.590950][11235:11240] CHIP:TOO:           DeviceType: null
[1666874500.590958][11235:11240] CHIP:TOO:          }
[1666874500.590970][11235:11240] CHIP:TOO:       FabricIndex: 1
[1666874500.590993][11235:11240] CHIP:TOO:      }
```

5. Press `SW6` will toggle the LEDs on the Light device.
