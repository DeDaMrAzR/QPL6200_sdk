# OTA multi-image Application Note

## Introduction

This document provides an overview of the multi-image Over-The-Air (OTA) update process for Qorvo devices. Multi-image OTA updates allow for updating multiple components of the firmware, such as the Secure Element (SE), bootloader, primary application, and secondary application [optional].

## OTA image format

For preparing OTA, it is necessary to create subimages for all partial images that need to be updated. Subimages must align with the OTA sub-image structure.
- Create the OTA file with the desired subimages.
- Sign the header and subimages.
- Distribute the OTA file to the devices.

| **Subimage descriptors**         | **Size** | **Description**                                                                 |
|----------------------------------|----------|---------------------------------------------------------------------------------|
| `imageIdentifier`                | 4 bytes  | The type of subimage (e.g., SEFW, Bootloader SW, Application SW).               |
| `imageSize`                      | 4 bytes  | The size of the subimage payload.                                               |
| `imageVersion`                   | 4 bytes  | The version of the subimage.                                                    |
| `dataOffset`                     | 4 bytes  | The offset where the subimage starts in the OTA file.                 |
| `contentHash`                    | 32 bytes | The hash of the subimage content for integrity verification.                  |

How to prepare the OTA file and sign it can be found in the Matter OTA firmware update guide.
![Matter OTA firmware update](https://github.com/Qorvo/QMatter/blob/v1.0.0.0/Documents/Guides/ota_device_firmware_upgrade.md)

## File Authentication
To protect the OTA file against tampering, we authenticate the image by verifying the contents via a cryptographic signature. Due to the nature of the OTA process, where subimages are fetched one by one and the device may reboot during the process, we cannot add a signature that covers the entirety of the OTA file contents.
Instead, we authenticate the header only, embedding a hash of each segment payload in the header. Any mutation in the segment payload will cause a header mismatch, allowing the OTA process to abort if a mismatch is detected.
## OTA Collecting Images Process
When the OTA process starts, the application version is different from the active version, ensuring that at least the application update needs to be applied.
1. **First Iteration**
   - Keep the rolling hash of each segment and check it against the descriptor’s value.
   - Download the whole file to verify all segment hashes without prematurely stopping.
2. **Other Iterations**
   - Do not save the contents if not needed, but check the segment’s hash before applying a needed update.

![Flowchart of update Secure Element, Bootloader, secondary application and primary application](./Images/flowchart_OTA_update.png)

## OTA Update Flow

The OTA upgrade process is performed sequentially for each sub image, each sequence requires:
- verifying the sub image hash,
- verifying the sub image version,
- placing the sub image in the OTA area (all sub images are stored in the OTA section, except for the OTA area of the application bootloader, which is located in the bootloader section),
- triggering the upgrade process

![OTA Update Flow](./Images/multi_image_OTA_flow.png)

- **Subimage Verification and Placement**

   Regardless of the transport protocol, the collected subimages should be verified and placed in the OTA area. The Secure Element Firmware (SEFW), Bootloader Software (Bootloader SW), and Application Software (Application SW) blocks must contain signatures to allow for separate authentication of each block. Existing Qorvo tooling should be used to sign these blocks.
- **Version Dependencies and Maintenance**

  For every release, it is essential to clearly define the dependencies between the Application, SEFW, and Bootloader. This includes specifying the minimum version fields. Maintaining version numbers is crucial to ensure compatibility and proper functioning. It may be beneficial to create a separate guide on versioning all components for our customers.
- **Version Checking**

   After placing the upgrade image in the OTA upgrade area and ensuring the version is higher than the current one, use the following functions to check the versions:
- **Triggering the Upgrade Process**

   If the version is correct, the upgrade process must be triggered. For upgrading the Secure Element and bootloader, the application must call the esec_start_upgrade function to initiate the upgrade process.
The secondary and primary application upgrades are handled by the application bootloader, which runs after a reset.
## Matter OTA Update example
In the following diagram example, we will update the Matter application. The OTA file contains two subimages: the bootloader and the Matter application. The bootloader is not updated, so only the Matter application will be updated.

![Matter OTA Update](./Images/multi_image_ota_example.png)
