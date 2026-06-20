# Application configurations for production builds

## Introduction

In this document, we guide you through the necessary steps to configure your application for a production build starting from a release build. Refer to [creating_release_build.md](./creating_release_build.md#how-to-move-from-a-debug-to-a-release-build) for instructions on how to create a release build.

### Table of contents

- [Application configurations for production builds](#application-configurations-for-production-builds)
  - [Introduction](#introduction)
    - [Table of contents](#table-of-contents)
    - [Example production builds in the SDK](#example-production-builds-in-the-sdk)
    - [Postbuild scripts in the SDK](#postbuild-scripts-in-the-sdk)
  - [How to move from a release to a production build](#how-to-move-from-a-release-to-a-production-build)
    - [Matter applications](#matter-applications)
      - [Matter factory data](#matter-factory-data)
        - [Step 1: Modify the factory data config file](#step-1-modify-the-factory-data-config-file)
        - [Step 2: Modify the factory data Makefile](#step-2-modify-the-factory-data-makefile)
      - [Matter firmware data](#matter-firmware-data)
        - [Step 1: Provide vendor specific Certification Declaration](#step-1-provide-vendor-specific-certification-declaration)
        - [Step 2: Create vendor specific firmware data config file](#step-2-create-vendor-specific-firmware-data-config-file)
        - [Step 3: Modify the firmware data Makefile](#step-3-modify-the-firmware-data-makefile)
      - [Certification Declaration in Factory Data (Optional)](#certification-declaration-in-factory-data-optional)
        - [Step 1: Provide empty Certification Declaration in the application](#step-1-provide-empty-certification-declaration-in-the-application)
        - [Step 2: Add Certification Declaration using `Qorvo Platform Tools`](#step-2-add-certification-declaration-using-qorvo-platform-tools)
      - [Over-the-Air Production file](#over-the-air-production-file)
    - [ZigBee applications](#zigbee-applications)
      - [ZigBee Persistent Storage Block (PSB)](#zigbee-persistent-storage-block-psb)
        - [Step 1: Modify the PSB XML file](#step-1-modify-the-psb-xml-file)
        - [Step 2: Modify the PSB Makefile](#step-2-modify-the-psb-makefile)
      - [ZigBee Factory Block (FB)](#zigbee-factory-block-fb)
        - [Step 1: Modify the FB XML file](#step-1-modify-the-fb-xml-file)
        - [Step 2: Modify the FB Makefile](#step-2-modify-the-fb-makefile)


### Example production builds in the SDK

This SDK contains example production builds for some applications that utilize some of the instructions mentioned in this document, and can be used as reference example or starting point to create your own production build.

### Postbuild scripts in the SDK

For more details about postbuild script check [Firmware structure chapter of this documentation](../../../Development/application_structure.rst#firmware-structure).

## How to move from a release to a production build

The difference between the release and production builds is the configuration of some data blocks for Matter and ZigBee applications.

### Matter applications

#### Matter factory data

Matter applications in this SDK use example factory data configurations for testing and development purposes. These configurations are sufficient for development and release builds. For production, you need to generate your own factory data specific to your product. Follow these steps to do so:

##### Step 1: Modify the factory data config file

* Modify the values in the factory data config file: `Tools/QorvoPlatformTools/Matter/Credentials/test_{app}_qpg6200.factory_data_config` according to your product use case.

##### Step 2: Modify the factory data Makefile

* Review and modify (if needed) the following parameters in the factory data Makefile: `Libraries/Qorvo/FactoryData/Makefile.FactoryData_{app}`

| Parameter                                   | Description                                                                                                                           |
|---------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| `WORKDIR`                                   | Reflects the application name                                                                                                         |
| `APP_TARGET`                                | Reflects the application name                                                                                                         |
| `GP_DATA_SECTION_START_FACTORYDATA`         | The address of factory data in NMV                                                                                                    |
| `DATA_CONF_FILE`                            | The modified factory data config file from [Step 1: Modify the factory data config file](#step-1-modify-the-factory-data-config-file) |

* The factory data Makefile is configured as a prerequisite to the Matter application Makefile. When the application is built, the factory data hex file is generated and merged to the application in one final hex file. Refer to one of the Matter applications Makefiles and Postbuild scripts.

* The factory data hex file can be built separately and programmed at the factory for containing some device specific configuration, please follow these steps if you decide to go with this option:

  1. Remove or wipe the factory data.

        You can either remove the factory data from the application Makefile or wipe it by providing an empty factory data.

     * Remove the factory data from the application Makefile.

        * Remove every line that contains `FactoryData_{app}` from the Application Makefile.
        * Remove the factory data entry from the `hexmerge` command in the Application postbuild script.

     * Wipe the factory data rejoin in the application hex file.

        * `FactoryData_empty` is provided in the SDK to wipe the factory data rejoin.
        * Replace all instances of `FactoryData_[app]` with `FactoryData_empty` in the application Makefile and postbuild script.

  2. Build and flash the factory data hex file separately

     * Execute the following make command with the modified factory data Makefile

        ```
        make -f Libraries/Qorvo/FactoryData/Makefile.FactoryData_{app}
        ```
     * Flash the hex file separately in the factory


#### Matter firmware data

Matter applications in this SDK use example firmware data which contains a test **Certification Declaration** for the Matter solutions for testing purposes. this is sufficient for development and release builds. For production, you need to generate your own firmware data containing your own vendor specific Certification Declaration. Follow these steps to do so:

##### Step 1: Provide vendor specific Certification Declaration

* Store your Certification Declaration `[vendor]_[app]_cd.bin` in `Tools/QorvoPlatformTools/Matter/Credentials/`.

##### Step 2: Create vendor specific firmware data config file

* Create `[vendor]_[app].firmware_data_config` file in `Tools/QorvoPlatformTools/Matter/Credentials/`

    ```
    --certification-declaration=${QPG_SDK_ROOT}/Tools/QorvoPlatformTools/Matter/Credentials/[vendor]_[app]_cd.bin
    ```

##### Step 3: Modify the firmware data Makefile

* Review and modify (if needed) the following parameters in the firmware data Makefile: `Libraries/Qorvo/FactoryData/Makefile.FirmwareData_{app}`

| Parameter                  | Description                                                                                                                                                             |
|----------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `WORKDIR`                  | Reflects the application name                                                                                                                                           |
| `LIBFIRMWAREDATA_SWITCH_A` | Reflects the application name                                                                                                                                           |
| `DATA_CONF_FILE`           | The vendor specific firmware data config file from [Step 2: Create vendor specific firmware data config file](#step-2-create-vendor-specific-firmware-data-config-file) |

* The firmware data Makefile is configured as a prerequisite to the Matter application Makefile. When the application is built, the firmware data library is generated and linked to the application. Refer to one of the Matter applications Makefiles.


#### Certification Declaration in Factory Data (Optional)

Certification Declaration can be added to the factory data using `Qorvo Platform Tools` in the factory. Follow these steps to do so:

##### Step 1: Provide empty Certification Declaration in the application

* Follow the steps defined in [Matter firmware data](#matter-firmware-data), but use `qorvo_empty_cd.bin` instead of `[vendor]_[app]_cd.bin`.

##### Step 2: Add Certification Declaration using `Qorvo Platform Tools`

* Refer to [Qorvo Platform Tools Documentation](../../../Tools/QorvoPlatformTools/README.md) for instructions.

#### Over-the-Air Production file

Once in the field, the Certification Declaration is updatable via the OTA image and thus becomes part of the Production Application.

To include the Certification Declaration in the OTA image, follow the same steps in [Matter firmware data](#matter-firmware-data)



### ZigBee applications

#### ZigBee Persistent Storage Block (PSB)

ZigBee applications in this SDK use example PSB configurations for testing and development purposes. These configurations are sufficient for development and release builds. For production, you need to generate your own PSB specific to your product. Follow these steps to do so:

##### Step 1: Modify the PSB XML file

* Modify the values in the PSB XML file: `Libraries/Qorvo/PersistentStorageBlock/{APP}.psb.xml` according to your product use case.

* Refer to `Documents/User Manuals/Zigbee/GP_P334_AN_15396_FB_PSB_Mechanism.pdf` application note section **3 Persistent Storage Block** for description of the fields in the PSB XML file.

##### Step 2: Modify the PSB Makefile

* Review and modify (if needed) the following parameters in the PSB Makefile: `Libraries/Qorvo/PersistentStorageBlock/Makefile.PersistentStorageBlock_qpg6200_{APP}`

| Parameter                  | Description                                                                                       |
|----------------------------|---------------------------------------------------------------------------------------------------|
| `WORKDIR`                  | Reflects the application name                                                                     |
| `XML_FILE`                 | The modified PSB XML file from [Step 1: Modify the PSB XML file](#step-1-modify-the-psb-xml-file) |
| `NBR_OF_POOLS`             | Modify according to your product use case `*`                                                     |
| `NBR_OF_SECTORS`           | Modify according to your product use case `*`                                                     |
| `GP_DATA_SECTION_START_NVM`| Provide the start address of the NVM section                                                      |
| `APP_TARGET`               | Reflects the application name                                                                     |

* `*` Refer to `Documents/User Manuals/Zigbee/GP_P334_AN_15396_FB_PSB_Mechanism.pdf` application note section **5 NVM Persistent Storage Block (PSB) configuration** for description of these fields.

* The PSB Makefile is configured as a prerequisite to the ZigBee application Makefile. When the application is built, the PSB hex file is generated and merged to the application in one final hex file. Refer to one of the ZigBee applications Makefiles and Postbuild scripts.

#### ZigBee Factory Block (FB)

ZigBee applications in this SDK use example FB configurations for testing and development purposes. These configurations are sufficient for development and release builds. For production, you need to generate your own FB specific to your product. Follow these steps to do so:

##### Step 1: Modify the FB XML file

* Modify the values in the FB XML file: `Libraries/Qorvo/FactoryBlock/{APP}.fb.xml` according to your product use case.
*  Refer to `Documents/User Manuals/Zigbee/GP_P334_AN_15396_FB_PSB_Mechanism.pdf` application note section **2 Factory Block** for description of the fields in the FB XML file.

##### Step 2: Modify the FB Makefile

* Review and modify (if needed) the following parameters in the FB Makefile: `Libraries/Qorvo/FactoryBlock/Makefile.FactoryBlock_qpg6200_{APP}`

| Parameter                  | Description                                                                                               |
|----------------------------|-----------------------------------------------------------------------------------------------------------|
| `WORKDIR`                  | Reflects the application name                                                                             |
| `XML_FILE`                 | The modified **FB** XML file from [Step 1: Modify the FB XML file](#step-1-modify-the-fb-xml-file)        |
| `INSTALL_CODE_FILE`        | The modified **PSB** XML file from [Step 1: Modify the PSB XML file](#step-1-modify-the-psb-xml-file) `*` |
| `APP_TARGET`               | Reflects the application name                                                                             |

* `*` The modified Persistent Storage Block (PSB) XML file is provided to extract the install code and inject it to the FactoryBlock.

* The FB Makefile is configured as a prerequisite to the ZigBee application Makefile. When the application is built, the FB hex file is generated and merged to the application in one final hex file. Refer to one of the ZigBee applications Makefiles and Postbuild scripts.

* The FB hex file could be built separately and programmed at the factory for containing some device specific configuration like the "injected" `install-code` and `eui64`. Use the following steps to build it separately:


  1. Remove FB from the application Makefile and postbuild script

     * Remove every line that contains `FactoryBlock_qpg6200_{App}` from the Application Makefile
     * Remove the factory block entry from the `hexmerge` command in the Application postbuild script

  2. Build and flash the FB hex file separately

     * Execute the following make command with the modified FB Makefile

        ```
        make -f Libraries/Qorvo/FactoryBlock/Makefile.FactoryBlock_qpg6200_{APP}
        ```

     * Flash the hex file separately in the factory
