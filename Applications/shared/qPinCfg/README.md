# qPinCfg

## Introduction

qPinCfg is a module to centralize pin management into one single place for each application individually.
It provides an overview of pin usages of the application, facilitates easy swap of pin usages and quick change of the hardware board.

## Architecture

![qPinCfg architecture](.images/qPinCfg.png)

- "qPinCfg.h" is an application-specific file.
- "qPinCfg_Common.h" is a generic and shared file for automated Board File selection.
- Board Files are header files which store the hardware information and describe the hardware pins.

## Usage

Include the file "qPinCfg.h" on the top of the files which require pin information.

```c
#include "qPinCfg.h"
```

## User customization

By default, this SDK uses the file `qPinCfg_IoTCarrierBoard_qpg6200.h`.
This file describes the IoT Carrier Board included in your SDK.

Each application can override this default individually and apply a custom board.
Please follow the below steps to override:

1. Create a new Board File file under `Applications/shared/qPinCfg/inc/boards`.
2. Uncomment ```QPINCFG_CUSTOM_BOARD_FILENAME``` in the "qPinCfg.h" of the application that needs overriding.
3. Define this ```QPINCFG_CUSTOM_BOARD_FILENAME``` as the new Board File.

![qPinCfg override](.images/override.png)
