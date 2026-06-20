# Device Preparation Guide

Learn how to verify your QPG6200 device and prepare it for secure operation.


## Device secure acceptance checklist

First, verify the Hardware Development Kit (HDK) according to [Quickstart - Check the Hardware](../../../sphinx/docs/QUICKSTART.rst#check-the-hardware).


Second, verify if the device is an untampered genuine Qorvo™ product:

  1. [Check the device state](#checking-the-device-state). Then, examine its output:
     - Check if the returned **ROM Bootloader version** matches with the version in [SDK Release Notes](../../../RELEASE_NOTES.md)
     - Check if the **Secure Element Firmware version** matches with the version in [SDK Release Notes](../../../RELEASE_NOTES.md)
  2. Perform [Device Attestation](#device-attestation).

**IMPORTANT:**
Do not use the device if any of the checks fail. The device may be compromised and unsafe to use. [Contact Qorvo™](../../../sphinx/docs/QORVO.rst#contact-information) to resolve the problem.

If all the tests pass successfully, proceed to configure the device for operation.

## Device Security Features Overview

This guide prepares QPG6200 for secure operation and shows how to use its security features.

The QPG6200 provides 5 main security features:

    - Device Attestation - check the device's authenticity.
    - Secure Storage - encrypt data with a device-unique key.
    - Secure Debug - enable trusted developers to gain access to the application processor.
    - Secure Boot - execute trusted applications only.
    - Firmware Encryption - encrypt application binaries on update.

**IMPORTANT:**
* These features must be enabled separately and recommended to enable all the security features.
* Enabling the Secure Boot and Secure Storage features are mandatory to operate the device in accordance with the PSA/SESIP level 2 security certification.

## Developer keys, Certificates and Manufacturer Public Key Relationship

The following diagram shows a simplified schematic of the trust chain.
This trust structure is used in both Secure Debug and Secure Boot features.

![Secure Boot & Debug Trust Chain](./secure_debug_boot_chain.drawio.png)

The diagram is read as:

    * The device trusts the Manufacturer by means of its Public Key (programmed at OEM production)
    * The developer is trusted by the manufacturer by means of the signed Developer Certificate
    * The device trusts firmware files from a Manufacturer-trusted Developer

The Developer:

    * Has a Developer Private key
    * Has a Certificate (corresponding to the Private Key) that is signed by the Manufacturer
    * Uses both to sign firmware files

The Manufacturer:

    * Has a Private Key and a Public Key
    * Selectively signs Developer's Certificates

The devices:

    * Have a copy of the Manufacturer's Public Key
    * Trust any firmware which trust chain can be validated against the Manufacturer Public Key


## Checking the device state

The QPG6200 device can provide information on its state and configuration.
Qorvo Platform Tool's [read-identification](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#read-identification) subcommand
displays a summary of the most important device information.

```console
> qorvo-security-toolbox read-identification --device QPG6200
Emulator: Segger JLink device - 1200000204
Device serial: 0x00000000000000000000000000033d9c
MAC address: 74:05:7c:00:00:23:3d:9c
BLE address: 74:05:7c:23:3d:9c
ROMBL: (0, 0), SEFW: 'SEUCFW-4.0.0-inc5'
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
    por_from_tamper     : 25 (0x19)
```

The following fields are important to quickly identify a device and determine its state.
Details on the expected values are documented in [Product Identification](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/product_identification.html).

    - Device serial
    - MAC / BLE addresses
    - SEFW version
    - Device status - life_cycle_state
    - Device status - srk_source


## Device Attestation

The QPG6200 device is programmed in Qorvo's factories with cryptographic assets that allow any user to validate its identity.

The identity chain implemented in the QPG6200 is documented in [Qorvo Platform Tools - Product Identification](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/product_identification.html#device-attestation).
It contains information about the stored certificates, their location, and instructions on how to retrieve them.
Optionally, you can manually verify the chain of trust, using Qorvo's public authenticity certificates.
These certificates are published in [Qorvo Platform Tools - Public Qorvo Certificates](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/product_identification.html#public-qorvo-certificates).

Verify the device before you take any other action.
Use Qorvo Platform Tool's [attest-device](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#device-attestation) subcommand.
This verifies that you are using a genuine Qorvo device.


```console
qorvo-security-toolbox attest-device --device qpg6200
Device is OK!
```


## Secure Storage


[Secure Storage](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/secure_storage.html) mechanism allows an application to encrypt and store data in device memory.
Additional Private Keys and authentication certificates can be stored in application space without compromising their security.
The main items stored with this mechanism are Private Keys.

Example use case:
Store private keys for a Matter DAC certificate which identifies the device in a Matter network.

Once secured, items cannot be used directly.
They are accessed by the Secure Element upon request from the application.
Memory address identifies the item. It is used to generate the item and to call it during later use.


### Initialization

This feature must be initialized during production with Qorvo Platform Tools [init-secure-storage](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#init-secure-storage) subcommand.
Otherwise, it is disabled and cannot be used.

```console
> qorvo.security_toolbox init-secure-storage --device QPG6200
SRK initialized to RNG_OTP (0x01)
```


### Generate Items

Items can be provisioned using Secure Storage with [Provisioning Bundles](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/provisioning_bundles.html).
Bundles support wrapping of existing items (e.g. private keys) or their creation by the Secure Element.

"Wrapping" encrypts these items with a device-specific key ([SRK](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/glossary.html#term-SRK)), and stores the encrypted data in a specified memory location.
These items must exist when the bundle is generated. They will be embedded in the generated bundle binary.
See [Key Wrapping Item](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/bundle_json.html#key-wrapping-item) for more details.

Item creation is done completely by the Secure Element, based on the given parameters and constraints.
See [Key Creating Item](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/bundle_json.html#key-creating-item) for more details.


### Use Items

Items in Secure Storage can be used in cryptographic operations such as signature and verification of data.
These operations are invoked via Mailbox transactions, are carried out by the Secure Element; their result - returned to the application.
Documentation of the exact Mailbox transactions is not in the scope of this chapter.
Please check library-specific documentation.


## Secure Debug

To enable Secure Debug follow the guide in [Security Toolbox - Secure Debug Example Bundle](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/bundle_json.html#example-security-configuration-chunk-json).
It demonstrates how to switch the secure debug enable field.

Once the device is provisioned with the Secure Debug enable OTP bits, this mode needs to be turned on explicitly on the device. Use the security toolbox command [Secure Debug Enable](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#secure-debug-enabling).

**WARNING: Secure Debug cannot be disabled** Once enabled, it modifies the device permanently and irreversibly.


## Secure Boot

To enable Secure Boot on the device follow the guide in [Security Toolbox - Secure Boot Example Bundle](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/bundle_json.html#example-security-configuration-chunk-json).


In QPG6200, Secure Boot provides cryptographic authentication of an application before it is executed.
This ensures that only signed, valid and genuine applications are executed.

**WARNING: Secure boot cannot be disabled**. Once enabled, it modifies the device permanently and irreversibly.

Similar to the architecture in [Device Attestation](#device-attestation),
Secure Boot relies on a chain of trust that begins with the device's ROM bootloader
and goes down the hierarchy of trust to user level applications.
A detailed view of the startup procedure and trust forwarding can be found in the [Application Bootloader](../../../Applications/Bootloader/README.md#architecture) section.

A very simplified diagram of the startup sequence is shown below.
The diagram highlights the user-controlled trust forwarding operations (named Transitions) that happen during startup.

![Initialization Matrix](./iot_sdk_app_boot_signing.drawio.png)


### Insecure boot + Complete Image

A Complete Image is an uncompressed binary composed of the Application Bootloader, an Application and possibly other elements and dependencies.
It is the output of the build process of the SDK and is usually programmed in the device with QPT subcommands.

In this mode the SEFW does not perform any checks in the Complete Image.
The SDK build flow for a Complete Image includes the Application Bootloader and, by default, also includes a pre-generated Developer Certificate.

This Developer Certificate is embedded in the Application Header, enabling an integrity check during boot.

This is intended for development only - no actions or preparations for production builds are necessary.


### Insecure boot + OTA Image

An OTA Image is a compressed binary of a component (Application Bootloader, Application, others) that is used to update devices in the field.
It is an output of the build process of the SDK.
It is usually distributed to devices by the customer.

In this mode the SEFW does not perform any checks and starts the Application Bootloader.

The Application Bootloader uses the Developer Certificate embedded in the OTA image Application Header, similar to the Insecure Boot + Complete Image mode.

This is intended for development only - no actions or preparations for production builds are necessary.


### Secure Boot + Complete Image

In this mode the SEFW reads the Developer Certificate embedded in the Application Bootloader Header and validates it (i.e. checks its authenticity).
The Certificate is validated against the Manufacturer Public Key programmed into the device during production.

The Application Bootloader uses the Developer Certificate embedded in the Application image and authenticates this certificate against the Manufacturer Public Key programmed in the device.

This checks the authenticity of both Application Bootloader and the Application.

The Manufacturer Public Key is a prerequisite for production.


### Secure Boot + OTA Image

This mode has the same behavior and requirements as “Secure boot + ‘complete’ image”.


### Note on Accuracy and Simplifications

This chapter simplifies some concepts regarding storage of cryptographic keys and flow of information in a chain of trust.
This has no consequences to functionality from user's perspective.

The device stores a digest of the public key. QPT uses the public key directly and generates the digest on demand.

## Diagnostic Mode

Diagnostic mode offers a system analysis interface.

To read more about Diagnostic Mode follow the guide in [Security Toolbox](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/diagnostic.html).
To enable Diagnostic Mode follow the guide in [Security Toolbox - Secure Debug Example Bundle](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/bundles/bundle_json.html#example-security-configuration-chunk-json).
It demonstrates how to switch the diagnostic mode enable field.

## Programmer Mode

Programmer mode offers a interface to program and provision the device.
To read more about Programmer Mode and how to enable it, follow the guide in [Security Toolbox](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/programmer_mode.html).

## Firmware Encryption

Firmware OTA binaries can be encrypted, for example to protect the Intellectual Property (IP) during distribution to devices in the field.
This is the case with SEFW images, which are encrypted by Qorvo before release (see [Secure Element Firmware Upgrade](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/secure_upgrade.html#secure-element-firmware-upgrade)).

See [SDK Build Flow Notes](#sdk-buildflow-notes) for automatic encryption during build support.
See [Secure Upgrade](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/secure_upgrade.html) for steps on how to update the Application Bootloader or the main Application.

**Firmware encryption is only supported for OTA images and depends on Application Bootloader support.**


# Secure Boot Walkthrough

To enable [Secure Boot](#secure-boot) follow these steps:

    1. Adjust the example JSON to enable and trigger Secure Boot
    2. Generate the corresponding provisioning bundle
    3. Apply the generated bundle
    4. Reset the device for changes to take effect
    5. Test: Sign and program an application


## 1. Adjust the example Secure Boot JSON

The following code snippet contains the minimum required information to enable Secure Boot via provisioning.

```JSON
{
  "role": "SM",
  "key_id": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
  "customer_id": [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
  "otp_word_size": 16,
  "items": [],
  "security_configuration": {
    "trigger_transition_to_lcs_secured": true,
    "ksmpk_iterations": 1000,
    "secure_boot": {
      "trigger_secure_boot_enabled": true,
      "signing_public_key": {
        "data": "import:./secure_boot_singing_public_key.pk"
      }
    }
  }
}
```

The `security_configuration.signing_public_key.data` item points to the Manufacturer Public Key.
This Public Key must be the pair of the Private Key used to sign Developer Certificates.
This item's value **needs to be updated** accordingly.


## 2. Generate the corresponding provisioning bundle

Qorvo Platform Tool's [make-bundle](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#make-bundle) subcommand can be used to generate the bundle.

```console
> qorvo-security-toolbox make-bundle secure-boot-bundle.bin trigger-secure-boot.json --device QPG6200-EXT
Bundle with 238 bytes was saved to secure-boot-bundle.bin
```

Remember to replace `QPG6200-EXT` with the correct device! For example, `QPG6200`.


## 3. Apply the generated bundle

Qorvo Platform Tool's [sm-provision](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#sm-provision) subcommand can be used to program and activate the generated bundle.

```console
> qorvo-security-toolbox sm-provision --device QPG6200-EXT secure-boot-bundle.bin
Writing bundle secure-boot-bundle.bin to device on address 0x10020000.
Command device to start provisioning the bundle.
Done, restart needed for the data to be used.
```

If an application is programmed in the device's non-volatile memory, it may be corrupted and will need to be reprogrammed!


## 4. Reset the device for changes to take effect

This can be done through the [reset-target](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#reset-target) subcommand or physically with the device's `RESETn` pin.

```console
qorvo-security-toolbox reset-target --device QPG6200-EXT
Target reset.
```

## 5. Test: Sign an Application

After Secure Boot is enabled, applications need to be signed with the correct credentials to be executable in the device.
This can be achieved with the [QorvoPlatformTools](../../../Tools/QorvoPlatformTools/README.md#qorvo-platform-tools) utility.

The tool supports signing binary files with direct or indirect access to the signing private key.
Both approaches are shown below.


## Direct Signing

This approach is usually used in development environments, when the signing private key is stored as a plain file without any password protection.
Example: sign `application.hex` using `developer_private_key.der` and `developer_certificate_signed.cert`:


```console
appuc-firmware-packer --appuc 1 --version 1 \
    --input application.hex \
    --cert developer_certificate_signed.cert \
    --output application_signed.hex \
    --sign developer_private_key.der
Reading developer_key_private.der
Reading developer_certificate_signed.cert
hex written to application_signed.hex
```


## Indirect Signing

This approach is usually used in production environments, when the signing private key is stored in a protected manner.
Keys can be password-protected, stored in HSMs or part of a networked build infrastructure.
The procedure is separated into three steps:

  - generate the digest that need to be signed,
  - externally sign,
  - build the signed HEX file.

The binary digest is generated using SHA-256 and is expected to be ECDSA-256 signed.

```console
# Generate the digest
appuc-firmware-packer --appuc 1 --version 1 \
    --input application.hex \
    --cert developer_certificate_signed.cert \
    --output application_signed.hex
    --get_hash
Reading developer_certificate_signed.cert
firmware digest written to application_signed-sha256.hash
pre-packed firmware written to application_signed-prepacked.bin

# Sign the digest with OpenSSL; this could be replaced by an HSM request
openssl pkeyutl -sign -in application_signed-sha256.hash \
  -inkey developer_key_private.der \
  -keyform der \
  -out data.signature

# Build the signed HEX file
appuc-firmware-packer --appuc 1 --version 1 \
    --input application.hex \
    --cert developer_certificate_signed.cert \
    --output application_signed.hex \
    --inject_prepacked application_signed-prepacked.bin \
    --inject_signature data.signature \
    --inject_signature_format asn1

```

## 6. Test: Program a Signed Application

Correctly signed application will successfully validate in boot stage and pass on to execution.
In the below snippet, the Application Bootloader indicates that Secure Debug was successfully enabled.
If this was not the case, the line `Secure boot Enabled` would be omitted.

```console
Bootloader start...
Secure boot Enabled
OTA header pointer is NULL.
OTA image not pending
Application has been found at 0x1001f400 address
Address for wake up from sleep mode is set to 0x1001f400
Application validate correctly
Booting application at 0x1001f400 address
========================
Hello world
Hello world
Hello world
Hello world
```


# Secure Debug Walkthrough

To enable [Secure Debug](#secure-debug) follow these steps:

1. [Adjust the example JSON to enable and trigger Secure Debug](#adjust-the-example-secure-debug-json)
2. [Generate the corresponding provisioning bundle](#id1)
3. [Apply the generated bundle](#id2)
4. [Reset the device for changes to take effect](#id3)
5. [Test: Read the device's identification](#test-read-the-device-s-identification)
6. [Debugging a device](#debugging-an-application-on-a-device)


## 1. Adjust the example Secure Debug JSON

```JSON
{
  "role": "SM",
  "key_id": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
  "customer_id": [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
  "otp_word_size": 16,
  "items": [],
  "security_configuration": {
    "trigger_transition_to_lcs_secured": true,
    "ksmpk_iterations": 1000,
    "secure_debug": {
      "trigger_secure_debug_enabled": true,
      "signing_public_key": {
        "data": "import:./secure_debug_signing_public_key.pk"
      }
    }
  }
}
```

The `secure_boot_signing_public_key.pk` is the manufacturer's public key.
This key must match the signature of the developer's key in the embedded certificate.
Details on how to prepare a single JSON file for Secure Boot and Secure Debug can be found in chapter [single json file generation](#secure-boot-and-secure-debug-example).


## 2. Generate the corresponding provisioning bundle

Qorvo Platform Tool's [make-bundle](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#make-bundle) subcommand can be used to generate the bundle.

```console
> qorvo-security-toolbox make-bundle secure-debug-bundle.bin trigger-secure-debug.json --device QPG6200-EXT
Bundle with 238 bytes was saved to secure-debug-bundle.bin
```

Remember to replace `QPG6200-EXT` with the correct device! For example, `QPG6200`.

## 3. Apply the generated bundle

Qorvo Platform Tool's [sm-provision](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#sm-provision) subcommand can be used to program and activate the generated bundle.

```console
> qorvo-security-toolbox sm-provision --device QPG6200-EXT secure-debug-bundle.bin
Writing bundle secure-debug-bundle.bin to device on address 0x10020000.
Command device to start provisioning the bundle.
Done, restart needed for the data to be used.
```

## 4. Reset the device for changes to take effect

This can be done through the [reset-target](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#reset-target) subcommand or physically with the device's `RESETn` pin.

```console
qorvo-security-toolbox reset-target --device QPG6200-EXT
Target reset.
```


## 5. Test: Read the device's identification

After the successful device reset, verify if Secure Debug feature is enabled by reading the device's identification.
To read the device's identification use the [read-identification](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#read-identification) subcommand.

```console
qorvo-security-toolbox read-identification --device QPG6200-EXT
Emulator: Segger JLink device - 1200000288
The device is secured and the developer credentials are not provided.
Aborted!
```

Notice the message about credentials error.
It appears when Secure Debug is enabled - in this case, the error is correct and expected.
From this time onwards any QPT command requires two additional parameters:

- `developer-certificate-path` - Path to the signed developer certificate.
- `developer-privatekey-path` - Path to the developer private key in *DER* format

Try to read the device's identification again with credentials.

```console
qorvo-security-toolbox read-identification --device QPG6200-EXT --developer-certificate-path secure_debug_dev_cert_signed.cert --developer-privatekey-pat
h secure_debug_dev_private_key.der
Emulator: Segger JLink device - 1200000412
Device serial: 0x00000000000000000000000000033d9c
MAC address: 74:05:7c:00:00:23:3d:9c
BLE address: 74:05:7c:23:3d:9c
ROMBL: (1, 1) - d1635c36674c9bae9eee895c738c78922ab954d42e6caf3ab5ba23c2fe96de15
SEFW: 'SEUCFW-4.0.2-inc2' - 2a166a3b3ab5e4770f2f5451c4a7aafd8802377162d0ee4f30da427b5bd736c0
SEFW Rollback version: 0
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
    por_from_tamper     : 0 (0x00)
```
Now, the valid output with data is displayed.
Check if the device life cycle state has been changed to *SECURED* state.

**Important: From now on, every command REQUIRES valid attributes `developer-certificate-path` and `developer-privatekey-path`. It will fail otherwise.**

This is true for all commands that interact with the connected device.


## 6. Debugging an Application on a Device

Use the subcommand [debug-open-debug](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/commands.html#debug-open-debug) to enable debug access. This is required on all Secure Debug enabled devices. Debug access stays enabled until the device is power-on reset.

```console
qorvo-security-toolbox debug-open-debug --device Q
PG6200-EXT --developer-certificate-path secure_debug_dev_cert_signed.cert --developer-privatekey-path secure_debug_dev_private_key.der
Debug access granted until power-on reset.
```

To run the active application after enabling debug access, issue the above command with an additional `-r` parameter.

```console
qorvo-security-toolbox debug-open-debug --device QPG6200-EXT \
    --developer-certificate-path secure_debug_dev_cert_signed.cert \
    --developer-privatekey-path secure_debug_dev_private_key.der \
    -r
Debug access granted until power-on reset.
Application started.

```

The application is running. You can attach a debugger.
**Remember to attach debugger without doing device reset**.


# Secure Boot and Secure Debug Example

Secure Boot and Secure Debug can be activated in the same provisioning sequence.
An example of the required JSON file for that is shown below.
Similar steps to Secure Boot and Secure Debug must be followed to enable and use the features.

```JSON
{
  "role": "SM",
  "key_id": [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
  "customer_id": [15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
  "otp_word_size": 16,
  "items": [],
  "security_configuration": {
    "trigger_transition_to_lcs_secured": true,
    "ksmpk_iterations": 1000,
    "secure_boot": {
      "trigger_secure_boot_enabled": true,
      "signing_public_key": {
        "data": "import:./secure_boot_singing_public_key.pk"
      }
    },
    "secure_debug": {
      "trigger_secure_debug_enabled": true,
      "signing_public_key": {
        "data": "import:./secure_debug_signing_public_key.pk"
      }
    }
  }
}
```

# OTA image generation

OTA image is created in steps:

1. Compress the application binary using LZMA and generate a digest using a hash function - `compressFirmware.py`

2. Sign the digest with a private key. Depends on signing method - Direct or Indirect.

3. Generate a compressed image authenticated header and build a signed OTA file - `compressFirmware.py`

4. [Combo mode](../../../Applications/Bootloader/README.md#dual-mode-application-bootloader) only - repeat steps 1 to 3 for a secondary image. Results in a second signed OTA file.

5. Create the final OTA image by bundling outcomes of steps 1 to 4 into a single file, and add an OTA header - `bundle_ota_img.py`

Application Bootloader verifies if signatures are correct. On success, it unpacks the binaries into non-volatile memory of the device.

OTA image can be signed in two ways:
- Direct, where the public key is unprotected. It runs in a single script - `generate_ota_img.py`.
- Indirect, where the public key is secured against unauthorized access. This needs manual configuration.

Both methods are covered in detail below.




### Direct Signing

Insecure method, typically used in development environments.

The private key is accessed directly as a plain file stored on a local resource (e.g., without decrypting, password protection, elevated user privileges, etc).

All [steps for OTA image generation](#ota-image-generation) are performed in a single run by a dedicated script `generate_ota_img.py`. It calls the `compressFirmware.py` and `bundle_ota_img.py` and other necessary functions to deliver a complete OTA image.

```console
"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Ota/generate_ota_img.py
--bl_image_file "${SCRIPT_DIR}"/../../../Work/Bootloader_qpg6200_compr_secure_dualboot/Bootloader_qpg6200_compr_secure_dualboot_before_hexmerge.hex_
--chip_config_header "${SCRIPT_DIR}"/../../../Applications/Matter/Switch/include/CHIPProjectConfig.h
--chip_root "${SCRIPT_DIR}"/../../../Components/ThirdParty/Matter/repo
--compression lzma
--developer-key "${SCRIPT_DIR}"/../../../Tools/SecureBoot/developer_key_private.der
--factory_data_config "${SCRIPT_DIR}"/../../../Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config
--flash-secondary-start-offset 0x176800
--flash_app_start_offset 0xc400
--flash_start 0x10013000
--header_version 1
--in_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200_release_thread_certified/Switch_Combo_qpg6200_release_thread_certified.hex
--out_file "${SCRIPT_DIR}"/../../../Work/Switch_Combo_qpg6200_release_thread_certified/Switch_Combo_qpg6200_release_thread_certified.ota
--se_image_file "${SCRIPT_DIR}"/../../..//Binaries/SEFW/se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_[VERSION].hex
--secondary-input-file "${SCRIPT_DIR}"/../../../Work/Switch_Sec_Zigbee_qpg6200_release/Switch_Sec_Zigbee_qpg6200_release.hex
--xp400x

```

### Indirect Signing

Secure method, used in production environments.

The private key is protected from direct access (e.g., encrypted, password-protected, stored in HSMs, or part of a networked build infrastructure).

To sign with a protected key, follow the steps manually. Adapt the examples to your production environment and key storage methods.

1. Compress the application binary using LZMA and generate a digest using a hash function.

```console
python3 Tools/Ota/compressFirmware.py  \
--xp400x \
--compression=lzma \
--input Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.bin \
--license_offset 0x0 \
--page_size 0x200 \
--sector_size 0x400 \
--get_hash
```

Expected output: creates a new digest file `filename.hash.bin` in the directory specified by the `--input` argument, where  `filename` matches the value provided to it.

2. Sign the digest using the protected private key.

```console
openssl pkeyutl -sign -in Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.hash.bin   \
-inkey Tools/SecureBoot/developer_key_private.der   \
-keyform der   \
-out Work/Switch_Combo_qpg6200_thread_certified/data_primary.signature
```

3. Generate a compressed image authenticated header and build a signed OTA file.

```console
python3 Tools/Ota/compressFirmware.py  \
--xp400x \
--compression=lzma  \
--input Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.bin \
--license_offset 0x0 \
--output Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.compressed.bin \
--page_size 0x200 \
--sector_size 0x400 \
--inject_signature Work/Switch_Combo_qpg6200_thread_certified/data_primary.signature \
--inject_signature_format asn1
```

4. [Combo mode](../../../Applications/Bootloader/README.md#dual-mode-application-bootloader) only: repeat steps 1-3 for the secondary application binary. Make sure both files are available before proceeding to the next step.

5. Generate an OTA header and its digest using a hash function.

```console
python3 Tools/Ota/bundle_ota_img.py \
--chip_config_header Applications/Matter/Switch/include/CHIPProjectConfig.h \
--factory_data_config Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config \
--se_image_file Binaries/SEFW/se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_[VERSION].hex \
--bl_image_file Work/Bootloader_qpg6200_compr_secure_dualboot/Bootloader_qpg6200_compr_secure_dualboot_before_hexmerge.hex_ \
--chip_root Components/ThirdParty/Matter/repo \
--input_file Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.compressed.bin \
--secondary_input_file Work/Switch_Sec_Zigbee_qpg6200/Switch_Sec_Zigbee_qpg6200.compressed.bin \
--out_file Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.ota_header.hash.bin \
--test_update_image \
--get_hash
```

Expected output: creates a new digest file in the directory specified by the `--out_file` argument.

6. Sign the digest using the developer private key.

```console
openssl pkeyutl -sign -in Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.ota_header.hash.bin   \
-inkey Tools/SecureBoot/developer_key_private.der   \
-keyform der   \
-out Work/Switch_Combo_qpg6200_thread_certified/ota_header.signature
```

7. Create the final OTA image. Bundle outcomes of steps 1 to 6 into a single file.
Notice the `--secondary_input_file` argument - this needs to be passed only for Combo applications. It's the path to the secondary application prepared in step 4.

```console
python3 Tools/Ota/bundle_ota_img.py \
--chip_config_header Applications/Matter/Switch/include/CHIPProjectConfig.h \
--factory_data_config Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config \
--se_image_file Binaries/SEFW/se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_[VERSION].hex \
--bl_image_file Work/Bootloader_qpg6200_compr_secure_dualboot/Bootloader_qpg6200_compr_secure_dualboot_before_hexmerge.hex_ \
--chip_root Components/ThirdParty/Matter/repo \
--input_file Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_thread_certified.compressed.bin \
--secondary_input_file Work/Switch_Sec_Zigbee_qpg6200/Switch_Sec_Zigbee_qpg6200.compressed.bin \
--out_file Work/Switch_Combo_qpg6200_thread_certified/Switch_Combo_qpg6200_ota_thread_certified.ota \
--test_update_image \
--inject_signature Work/Switch_Combo_qpg6200_thread_certified/ota_header.signature \
--inject_signature_format asn1 \
--developer-key Tools/SecureBoot/developer_key_private.der
```

# SDK Buildflow Notes

The SDK build flow automatically signs the built applications and generates an OTA image in a post-build step.
This step can be customized to use the appropriate private key and certificate.

To use custom user-defined Manufacturer Public Key and Developer Signed Certificates, the developer needs to override the respective files in `Tools/SecureBoot`.
[Secure Boot](https://qorvo_sdk.gitlab.io/public/tools/packages/qorvo-platform-tools/security_toolbox/security.html#secure-boot)

The SDK build flow currently does not support automatic encryption of OTA images.
