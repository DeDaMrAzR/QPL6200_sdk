# Zigbee applications

## Software architecture

The Zigbee application software architecture is divided into software modules. Each software module is a separate C/C++ file. A software module has limited scope and features and can use additional libraries and drivers. This software architecture allows to encapsulate features into separate application building blocks that are combined to create an application.

Software modules are exchanging information between each other with use of ``AppEvent`` or with direct API calls. Events are processed and handled by [AppTask](#apptask) module, that is started at application boot.

A simplified diagram of software architecture for Zigbee application is shown below:

![Zigbee Software Architecture](.images/zigbee_applications_software_architecture.png)

In the above software architecture diagram, buttons and LEDs are handled by a separate software modules, `AppButtons` and `StatusLed` respectively.

The `AppButtons` module processes buttons with use of [Button Handler](../shared/modules/buttonHandler.rst) module. Based on the callback function provided to the ``ButtonHandler_Init`` function it schedules and unschedules events that are send to other application modules with use of [AppTask](#apptask) module.

The `StatusLed` module is responsible for handling LEDs in the application. It defines an API for other software modules to easily update LEDs to present application state to the user. It handles LEDs with use of [Status LED](../shared/modules/statusLed.rst) module.

Both `AppButtons` and `StatusLed` modules use GPIO pins defined in the [qPinCfg](../shared/qPinCfg/README.md) configuration.

Zigbee applications also use other modules that are shared between applications, those are [Reset Count](../shared/modules/resetCount.rst) and [App Battery](../shared/modules/appBattery.rst) modules.

### AppTask

The AppTask module is responsible managing event exchange between software modules in the application. At boot, the AppTask sets up a dedicated event queue so that other software modules can send events between each other.

### AppManager

The AppManager is responsible for managing the core logic and state of the application. The module is responsible for reacting to events send by other software modules, reacting to button events, handling LEDs state, and updating the Application specific Zigbee cluster state.

### Zigbee task manager

The Zigbee task manager module is responsible for handling Zigbee network. The module exposes an API that allows to open commissioning and start factory reset of the Zigbee device. The module implements set of callbacks that allow other application modules to react to the Zigbee network state.

### Zigbee endpoint manager

The Zigbee endpoint manager module is responsible for defining the Zigbee cluster used in the device. The module exposes an API to interact with the Zigbee cluster within an application. For example, in the Zigbee Switch application, the module exposes an API to send On/Off toggle command.

## Touchlink and distribute security key
This sections describes how to add Touchlink and Distribute Security key to the Concurrent Light reference application. As per Zigbee Alliance regulation, the Touchlink master key and Distributed security key are provided by the Zigbee Alliance after having performed a successful product certification.

### Adding Touchlink Master Key
In the reference application, the Touchlink master key is not enabled by default. To apply the Touchlink Master key, you can edit "/IoT_SDK/Applications/Zigbee/Shared/src/zigbee/ZigBeeTouchLink.cpp"

- Update the Touchlink supported scheme variable, "CZigBeeClusterTouchlink::`nSupportedSchemes`", to enable supporting certified product. Be sure to disable the development and pre-certification ones in an end-product.<br>
    - For example, from default setting, in which `supportsCertifiedProduct` bit is not set for supporting development and pre-certification: <br>

```C++
      // Supported keys
      const uint16_t CZigBeeClusterTouchlink::nSupportedSchemes =
      // for Development phase
                                                                  CZigBeeClusterTouchlink::supportsDevelopment
                                                                  |
      // for a Certification phase
                                                                  CZigBeeClusterTouchlink::supportsPrecertification
                                                                  // |
      // for a Product
                                                                  // CZigBeeClusterTouchlink::supportsCertifiedProduct
                                                                  ;
```
- To enable the bit `supportsPrecertification` for pre-certification only: <br>

```C++
      // Supported keys
      const uint16_t CZigBeeClusterTouchlink::nSupportedSchemes =
      // for Development phase
                                                                  // CZigBeeClusterTouchlink::supportsDevelopment
                                                                  // |
      // for a Certification phase
                                                                  CZigBeeClusterTouchlink::supportsPrecertification
                                                                  // |
      // for a Product
                                                                  // CZigBeeClusterTouchlink::supportsCertifiedProduct
                                                                  ;
```
- To enable the bit `supportsCertifiedProduct` for the certified product: <br>

```C++
      // Supported keys
      const uint16_t CZigBeeClusterTouchlink::nSupportedSchemes =
      // for Development phase
                                                                  // CZigBeeClusterTouchlink::supportsDevelopment
                                                                  // |
      // for a Certification phase
                                                                  // CZigBeeClusterTouchlink::supportsPrecertification
                                                                  // |
      // for a Product
                                                                  CZigBeeClusterTouchlink::supportsCertifiedProduct
                                                                  ;
```

- Then, add the Touchlink Master key to "CQorvoClusterTouchLinkCommissioningSecurity::`abMasterKey`, which is by default filled with all `0x0`:
```C++
      //Touchlink Master key - to be set by users
      const unsigned char CQorvoClusterTouchLinkCommissioningSecurity::
          abMasterKey[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                            0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
```

### Adding Distributed Security Key
Some systems will require the official Zigbee 3.0 security key to be used. To apply the Distributed security key, you can edit "/IoT_SDK/Applications/Zigbee/Shared/src/zigbee/ZigBeeStackConfiguration.cpp"
- Add the Distributed security key to variable CZigBeeApplicationSecurityMaterial::`abDistributedSecurityLinkKey`, which is commented out by default: <br>
```C++
      //const unsigned char CZigBeeApplicationSecurityMaterial::
      //    abDistributedSecurityLinkKey[16] = { 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5,
      //    0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf };
```

- For example:
```C++
      //Distributed Security Key - to be set by user
      const unsigned char CZigBeeApplicationSecurityMaterial::
         abDistributedSecurityLinkKey[16] = { 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5,
         0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf };
```


### Factory Block and Persistent Memory Block
Factory Block and Persistent memory block resides in application non-volatile memory. The configuration of factory block and persistent memory block are set in /IoT_SDK/Libraries/Qorvo/FactoryBlock/{projectname}.fb.xml and /IoT_SDK/Libraries/Qorvo/PersistentStorageBlock/{projectname}.psb.xml respectively. <br>
For example, <br>
1. The manufacturer name is configured as Qorvo by default in <b>DoorWindow.fb.xml</b>. You may customize the manufacturer name there.
2. The Install Code is defined in <b>Switch.psb.xml</b>. You may customize the Install Code there.<br><br>

For more information, please refer to __`GP_P334_AN_15396_FB_PSB_Mechanism.pdf`__


## Zigbee rejoin cycle information
In the situation that the parent is out of reach, e.g. it was powered off, the end device will implement rejoin attempt to look for the parent. <br>
There are few diversities for configuring the number of rejoin attempt and the duration in between in {SDK}/Applications/Zigbee/Shared/src/zigbee/ZigBeeCommissioning.cpp <br>
  - APP_REJOIN_ATTEMPTS
    - define the number of rejoin attempt in every rejoin attempt cycle <b>(default value is 3)</b>
  - APP_REJOIN_RETRY_DELAY
    - define the duration between each rejoin attempt within one rejoin attempt cycle <b>(default value is 3s)</b>
  - APP_REJOIN_BASE_BACKOFF_DELAY
    - define the backoff duration between each rejoin cycle.
    - Please be reminded that the backoff duration will be doubled for each rejoin attempt cycle, e.g. if APP_REJOIN_BASE_BACKOFF_DELAY = 20s, the duration will be set to 20s, then 40s, then 80s, etc. <b>(default value is 60s)</b>
  - APP_REJOIN_MAX_BACKOFF_DELAY
    - define the maximum backoff duration between rejoin attempt cycle. <b>(default value is 30min)</b>
<br>

<b>Remarks:</b>
1. There will be some overhead for each rejoin attempt, e.g. the APP_REJOIN_RETRY_DELAY  is set as 3s, the measured duration will be a bit longer (same to APP_REJOIN_BASE_BACKOFF_DELAY ).
2. The customers can edit the default configuration. But, please be reminded that the device will wake up at each rejoin attempt, which means that more frequent the rejoin attempt is triggered, the more energy will be consumed.
