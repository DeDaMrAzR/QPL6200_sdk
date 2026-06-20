# Matter applications

## Software architecture

Matter applications software architecture is divided into software modules, each software module is a separate C/C++ file. A software module has limited scope and features and can use additional libraries and drivers. This software architecture allows to encapsulate features into separate application building blocks that are combined to create an application.

Software modules are exchanging information between each other with use of ``AppEvent`` or with direct API calls. Events are processed and handled by [AppTask](#apptask) module, that is started at application boot.

A simplified diagram of software architecture for Matter Switch application is shown below:

![Matter Software Architecture](.images/matter_applications_software_architecture.png)

### AppTask

The AppTask module contains the main application logic. The module is responsible managing event exchange between software modules in the application. At boot, the `AppTask` module sets up a dedicated event queue so that other software modules can send events between each other. The module also controls the device behavior by handing button presses, and controlling LEDs with use of [qvIO](../shared/modules/qvIO.rst) module.

### Manager

The Manager module is responsible for handling Matter stack and cluster attribute values for easy access by other application modules. The module exposes an API to interact with the Matter clusters. For example, in the [Matter Switch application](./Switch/README.md), the module exposes an API to send `On/Off toggle` command, in the [Matter Motion sensor](./MotionSensor/README.md) application, the module exposes an API to check if the sensor is occupied, and to get `Occupied to Unoccupied Delay` attribute value. In the [Matter DoorWindow application](./DoorWindow/README.md), the module exposes an API to check if the Door/Window is closed.

### ZCL Callbacks

The ZCL callbacks module encapsulates Matter stack callbacks to one place. Whenever cluster attribute changes, this is processed by the stack and at signalled to the application with callback functions. Thanks to this the device can to react to changes that affect it in the network. The modules uses the API exposed by the `Manager` module to save cluster attribute values.


## Secure Storage

Matter applications use [Secure Storage](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/secure_storage.rst#secure-storage).
The Storage Root Key must be initialized __once__ using our [Qorvo Platform Tools](../../Tools/QorvoPlatformTools/README.md) package, for Matter devices to be fully operational.

After [initialization](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#init-secure-storage), be sure to [reset](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.rst#reset-target) the device.

> __WARNING__: Be sure to configure the correct device: `QPG6200`
