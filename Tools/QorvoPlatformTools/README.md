# Qorvo Platform Tools

> **WARNING**: QorvoPlatformTools version `1.3.8` is incompatible with JLink versions `8.12e` and later. Please use JLink version `8.12d` or earlier until this issue is resolved.

Qorvo Platform Tools (QPT) is a tool suite used during development:
- [Read QPT Documentation](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/)

QPT is composed of:
- Security Toolbox - individual tools used during application development
- Server and Client application references for production lines

#### Programming

You can first erase the target device with the following command:

```shell
qorvo-security-toolbox erase-target --device QPG6200
```

Then do a programming with the following command:

```shell
qorvo-security-toolbox program-hex --device QPG6200 <hex_file>
```

Where:
- ``<hex_file>`` - the application you want to write to your device.

### Upgrading Secure Element firmware

Before you start upgrading the Secure Element firmware, you can check what is the current version running on your device. You can do it with the following command:

```shell
qorvo-security-toolbox read-identification --device QPG6200
```

You will receive an output with the information of the current version of the Secure Element firmware running on your device. The output will be similar to the one below:

```shell
user@pc:~/iot_sdk $ qorvo-security-toolbox read-identification --device QPG6200
Emulator: Segger JLink device - 1200000188
Device serial: 0x00000000000000000000000000033d9e
MAC address: 74:05:7c:00:00:23:3d:9e
BLE address: 74:05:7c:23:3d:9e
ROMBL: (0, 0), SEFW: 'SEUCFW-4.0.2-inc2'
Device status:
    boot_step           : QSPI_INIT (0x00)
    host_image          : FLASH_A (0x00)
    host_upgrade_flag   : False
    esec_version        : 0 (0x00)
    host_version        : 0 (0x00)
    last_tamper_status  : 0 (0x00)
    last_tamper_status_timestamp: 0 (0x00)
    raw_tamper_status   : 0 (0x00)
    current_time        : 0 (0x00)
-----
    life_cycle_state    : INIT (0x06)
    srk_source          : RNG_OTP (0x01)
    provisioning_result : ERROR_OK (0x01)
    provisioning_role   : 0 (0x00)
    provisioning_last_chunk: 0 (0x00)
    por_from_tamper     : 212 (0xd4)
```

To upgrade the secure element firmware, complete the following steps:

1. Download the image from the `Binaries/SEFW/` directory of the SDK.
2. Run the following commands:

```shell
qorvo-security-toolbox upgrade-sefw --device QPG6200 <sefw_upgrade_file>
qorvo-security-toolbox reset-target --device QPG6200
```

Where:
- ``<sefw_upgrade_file>`` is the secure element upgrade file you want to write to your device.
