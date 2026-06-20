# Application configurations for release builds

## Introduction

This document describes how to configure your application for a release build.

Release build requires optimizations, so your application runs efficiently and securely in the field. These include:

* Disabling logging and removing unneeded sources from the build
* Modifying assert behavior

### Table of contents
- [Application configurations for release builds](#application-configurations-for-release-builds)
  - [Introduction](#introduction)
    - [Table of contents](#table-of-contents)
    - [Introduction to SDK Diversities (prerequisite)](#introduction-to-sdk-diversities-prerequisite)
  - [How to move from a debug to a release build](#how-to-move-from-a-debug-to-a-release-build)
    - [Step 1: Disable logging](#step-1-disable-logging)
      - [Disable the application logging](#disable-the-application-logging)
      - [Disable the Matter stack logging (only relevant for Matter applications)](#disable-the-matter-stack-logging-only-relevant-for-matter-applications)
      - [Disable the bootloader logging (only relevant for applications with bootloader)](#disable-the-bootloader-logging-only-relevant-for-applications-with-bootloader)
      - [Remove unneeded sources from the build](#remove-unneeded-sources-from-the-build)
    - [Step 2: Modify the assert behavior](#step-2-modify-the-assert-behavior)
  - [Detailed description of configurations](#detailed-description-of-configurations)
    - [Logging feature and how it is configured](#logging-feature-and-how-it-is-configured)
      - [Qorvo specific logging statements](#qorvo-specific-logging-statements)
      - [Matter specific logging statements](#matter-specific-logging-statements)
    - [Assert handling feature and how it is configured](#assert-handling-feature-and-how-it-is-configured)


### Introduction to SDK Diversities (prerequisite)

The build system of this SDK utilizes a set of macro definitions, referred to as **`Diversities`**, to manage the following:

- **Code management**: Enable or disable specific parts of the code during compilation.
- **Configuration**: Provide compile-time parameters to the application build.

## How to move from a debug to a release build

Applications in this SDK support a set of features used for software development and testing.

When enabled, these features have a negative impact on:

* Performance
* Flash size
* RAM usage
* Power consumption
* etc.

Use the Makefiles that has *_release* in the filename as reference.

Follow these steps to configure your application for a release build:

### Step 1: Disable logging

#### Disable the application logging

Remove the diversity `GP_DIVERSITY_LOG` from the `qorvo_internal.h` header files of the application and libraries on which the application depends.

> **Note:** Refer to the Makefile of the application to get a list of the libraries on which the application depends.

#### Disable the Matter stack logging (only relevant for Matter applications)

Add the following arguments to `--args` for the Matter stack library GN build command:

- chip_error_logging=false
- chip_progress_logging=false
- chip_detail_logging=false
- chip_automation_logging=false

Example:

Before

```
--args=" qpg_target_ic...
```

After

```
--args=" chip_error_logging=false chip_progress_logging=false chip_detail_logging=false chip_automation_logging=false qpg_target_ic...
```

#### Disable the bootloader logging (only relevant for applications with bootloader)

Remove the diversity `GP_DIVERSITY_LOG` from the `qorvo_internal.h` header file of the bootloader.

#### Remove unneeded sources from the build

Remove the following source file from the Makefile of the application, the bootloader (if applicable), and the libraries, as it is not needed in a release build where logging is disabled:

* `Components/Qorvo/HAL_PLATFORM/halCortexM4/src/default_log_handlers.c`

### Step 2: Modify the assert behavior

Set the following diversities in the Makefile of the application and the libraries to modify the assert behavior **to trigger a reset instead of an assertion**.

* `GP_DIVERSITY_ASSERT_ACTION_RESET`
* `GP_DIVERSITY_ASSERT_REPORTING_NOTHING`

## Detailed description of configurations

### Logging feature and how it is configured

The following components are used to allow logging to work:

* **Matter stack logging** (`Components/ThirdParty/Matter/repo/src/lib/support/logging/CHIPLogging.h`): Can be used in matter source files to enable/disable the inclusion of the logging strings towards the compiler using preprocessor MACRO definitions. It also controls where these strings are sent towards and how they are buffered and flushed at runtime. This block also handles the string formatting of the parameters, etc.
* **gpLog** (`Components/Qorvo/OS/gpLog`): This block has the same functionality as the logging used in the Matter stack but for Qorvo specific source files. The MACRO definitions are also dedicated to this block.
* **gpCom** (`Components/Qorvo/BaseUtils/gpCom`): gpCom provides inter device data encapsulation and routing for serial links transporting serialized remote procedure call messages (e.g. encode Bluetooth HCI messages send it using the one UART and ChipLog/gpLog messages to a different UART).
* **HAL_PLATFORM** (`Components/Qorvo/HAL_PLATFORM`): This component defines the hardware specific dedicated parts such as **qDrvUART.c** (`Components/Qorvo/HAL_PLATFORM/gpPeripheral/src/qDrvUART.c`). This part will specify how the hardware block is configured and how for example logging strings are buffered at hardware level (RAM / DMA / ...).
* **qPinCfg** (`Applications/shared/qPinCfg/`): The qPinCfg component defines the functionality of each pin (`Applications/shared/qPinCfg/inc/boards/qPinCfg_IoTCarrierBoard_qpg6200.h`) and what peripheral they map to for that application (`Applications/Matter/switch/include/qPinCfg.h`).

By default the application supports logging over UART. The logging strings are stored as ASCII values in the hex after compilation, they are provided to the compiler through different MACRO definitions.

In this application build, logging strings are directed from 2 parts into gpCom:

- Qorvo specific logging

```
  GP_LOG_PRINTF("test string %u", 0, parameter);
  GP_LOG_SYSTEM_PRINTF("test string %u", 0, parameter);
```

- Application and Matter stack logging

```
  ChipLogProgress(NotSpecified, "test string %u", parameter);
  ChipLogError(NotSpecified, "test string %u", parameter);
```

#### Qorvo specific logging statements

In the Qorvo specific files, the `GP_LOG_SYSTEM_PRINTF` and `GP_LOG_PRINTF` macro definitions are used to encapsulate the logging strings. `GP_LOG_PRINTF` macro definitions can be disabled at file level by adding or removing `#define GP_LOCAL_LOG` at top level of the source file.

These macro definitions can be used by including the following header in your source files.

```
#include "gpLog.h"
```

When all DIVERSITY settings are set to enable logging for specific Qorvo source files in your application build, the following declaration is made for each of these string MACRO definitions:

```
#define GP_LOG_PRINTF(s, ...) gpLog_Printf(GP_COMPONENT_ID, true, GP_LOG_PSTR(s), __VA_ARGS__)
#define GP_LOG_SYSTEM_PRINTF(s, ...) gpLog_Printf(GP_COMPONENT_ID, true, GP_LOG_PSTR(s), __VA_ARGS__)
```

When the logging is disabled for a specific Qorvo source file in the application build, these macro's will become an empty definition and a such will be ignored by the compiler:

```
#define GP_LOG_PRINTF(s, ...)
#define GP_LOG_SYSTEM_PRINTF(s, ...)
```

The decision tree for this mapping can be found in **gpLog.h** (`Components/Qorvo/OS/gpLog/inc/gpLog.h`).

These MACRO definitions will be converted to `gpLog_Printf` statements and are forwarded from gpLog to gpCom via:

```
gpCom_DataRequest(GP_COMPONENT_ID_LOG, nbr_chars+1, (UInt8*)buf, GP_LOG_COMMUNICATION_ID);
```

in **gpLog_vsnprintf.c** (`Components/Qorvo/OS/gpLog/src/gpLog_vsnprintf.c`)

#### Matter specific logging statements

In the Matter specific files, the `ChipLogError`, `ChipLogProgress`, `ChipLogDetail`, `ChipLogAutomation` MACRO definitions encapsulate the logging strings. These logging levels can be specified globally for the Matter stack at compile time.

Include the following header in your source files to use these MACRO definitions.

```
#include <lib/support/logging/CHIPLogging.h>
```

These MACRO definitions will be converted to qvCHIP_Printf which map to qvIO_UartTxData. This call hands over the logging strings to gpCom via:

```
gpCom_DataRequest(GP_COMPONENT_ID_QVIO, length, (UInt8*)txBuffer, GP_COM_DEFAULT_COMMUNICATION_ID);
```

in **qvIO.c** (`Components/Qorvo/BSP/qvIO/src/qvIO.c`)

The logging levels can get enabled in the gn build structure via:

```
chip_error_logging = true
chip_progress_logging =  true
chip_detail_logging =  true
chip_automation_logging = true
```

These configurations can be added as arguments in the GN build command for the Matter stack library build.

```diff
-    --args=" qpg_target_ic=\"$(QPG_TARGET_IC)\" qpg_sdk_root=\"$(QPG_SDK_ROOT)\" qpg_sdk_lib_dir=\"$(QPG_SDK_LIB_DIR)\" openthread_qpg_root=\"$(QPG_OPENTHREAD_DIR)\" qpg_sdk_include_platform_libs=$(QPG_SDK_INCLUDE_PLATFORM_LIBS) chip_project_config_include_dirs=[\"$(QPG_APPLICATION_INCLUDE_PATH)\"] freertos_root=\"$(FREERTOS_ROOT)\" thread_certified=$(THREAD_CERTIFIED) chip_openthread_target=\"$(THREAD_CERTIFIED_TARGET)\" $${MATTER_ARGS[*]}" \
+    --args=" chip_error_logging=true chip_progress_logging=true chip_detail_logging=true chip_automation_logging=true qpg_target_ic=\"$(QPG_TARGET_IC)\" qpg_sdk_root=\"$(QPG_SDK_ROOT)\" qpg_sdk_lib_dir=\"$(QPG_SDK_LIB_DIR)\" openthread_qpg_root=\"$(QPG_OPENTHREAD_DIR)\" qpg_sdk_include_platform_libs=$(QPG_SDK_INCLUDE_PLATFORM_LIBS) chip_project_config_include_dirs=[\"$(QPG_APPLICATION_INCLUDE_PATH)\"] freertos_root=\"$(FREERTOS_ROOT)\" thread_certified=$(THREAD_CERTIFIED) chip_openthread_target=\"$(THREAD_CERTIFIED_TARGET)\" $${MATTER_ARGS[*]}" \
```

### Assert handling feature and how it is configured

The build system uses an assert mechanism to decrease the size of error handling code. This is used
to highlight and identify issues on debug builds. Use this mechanism to identify and resolve issues during the R&D
phase of the product before going into release stage. When the testing stage passes and no asserts are triggered
they can be converted to any default behavior for release.

The default behavior of any subcomponent ASSERT implementation redirects to the gpAssert component. This is an assumed behavior for this SDK.
This component can be controlled through the `GP_DIVERSITY_ASSERT_REPORTING` setting which configures how the assertions are reported (e.g. print the file name and line of the assert triggered).

- The list of settings for `GP_DIVERSITY_ASSERT_REPORTING`

```
// define reporting behavior
#if defined(GP_DIVERSITY_ASSERT_REPORTING_CALLBACK)
#define GP_ASSERT_REPORT(info, compId, file, line)          gpAssert_CallCbAssertIndication(info,compId,file,line)
#elif defined(GP_DIVERSITY_ASSERT_REPORTING_LOG)
#define GP_ASSERT_REPORT(info, compId, file, line)          gpAssert_PrintLogString(compId,file,line)
#elif defined(GP_DIVERSITY_ASSERT_REPORTING_LED_DEBUG)
#define GP_ASSERT_REPORT(info, compId, file, line)          gpAssert_LedDebug(line)
#elif defined(GP_DIVERSITY_ASSERT_REPORTING_NOTHING)
#define GP_ASSERT_REPORT(info, compId, file, line)          do {} while(false)
#endif //GP_DIVERSITY_ASSERT_REPORTING_CALLBACK
```

- The list of settings for `GP_DIVERSITY_ASSERT_ACTION`:

```
// define action behavior
#if defined(GP_DIVERSITY_ASSERT_ACTION_BLINK_LED)
#define GP_ASSERT_FOLLOW_UP_ACTION()                    gpAssert_BlinkLed()
#elif defined(GP_DIVERSITY_ASSERT_ACTION_EXIT)
#define GP_ASSERT_FOLLOW_UP_ACTION()                    gpAssert_Exit()
#elif defined(GP_DIVERSITY_ASSERT_ACTION_RESET)
#define GP_ASSERT_FOLLOW_UP_ACTION()                    gpAssert_ResetSystem()
#elif defined(GP_DIVERSITY_ASSERT_ACTION_NOTHING)
#define GP_ASSERT_FOLLOW_UP_ACTION()                    do {} while(false)
#endif
```
