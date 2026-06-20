# Secure Element firmware

## Version Information

**Important**: Always upgrade the Secure Element firmware to the most recent version released in the SDK. The Secure Element firmware requirements may vary between SDK versions.

### How to identify firmware version:
The firmware version can be retrieved using a [Qorvo Platform Tools command](#how-to-check-secure-element-firmware-version). It is also indicated in version field of the filename:

**Filename format:**

- ``QPG6200``: *se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_[VERSION].hex*

### How to upgrade Secure Element firmware:
Qorvo Platform Tools can be used to upgrade the Secure Element firmware through below command:
- `qorvo-security-toolbox.exe upgrade-sefw --device QPG6200 <SEFW_FILE>`

**Example:**
- `qorvo-security-toolbox.exe upgrade-sefw --device QPG6200 ./se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_[VERSION].hex`

For more information, please refer to [secure-element-firmware-upgrade](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/secure_upgrade.html#secure-element-firmware-upgrade)


### How to check Secure Element firmware version
Qorvo Platform Tools can be used to check the Secure Element firmware version through below command:
- `qorvo-security-toolbox read-identification --device QPG6200`

The Secure Element firmware version can be found from the output of command:

**Examples:**
- `SEFW: 'SEUCFW-[VERSION]-inc1' - <VERSION COMMIT HASH>`

For more information, please refer to [device-information-query](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#device-information-query)


## Chip revision

Chip revision is a part of the package information printed on the top of package in the following format:

**QPG6200**
**X**

For example:
 ``QPG6200
 L``
