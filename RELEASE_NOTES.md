# Release Notes

**2.0.2**

March 04, 2026

## Bug Fixes


**Resolve Unexpected fault occurred during the device zigbee touchlink process.** APPSCS-7415
- Added return statement in ConnectTouchlink member function. This fix resolve the assertion during Zigbee touchlink process.
- **Severity:** Critical


**Fixed device cannot be added to Google Home.** APPSCS-7484
- Increased the maximum number of reports in flight CHIP_IM_MAX_REPORTS_IN_FLIGHT from 2 to 4 in the CHIPProjectConfig.h file. This change prevents the issue that devices using SDK 2.0.1 could not be added to Google, with the app stuck at ‘Connecting’ or ‘Adding device’ stages, despite Bluetooth connection being established.
- **Severity:** Medium



## Changes


- (IoT SDK) Removed these examples: HelloWorld, Concurrent, Peripherals, Sleep, Bluetooth LE, Matter Light, Matter Thermostatic Radiator Valve (TRV).
- (Qorvo component) Added GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET flag for new OTA update feature.
- (Matter) Added OTA_SINGLE_REBOOT_MULTI_APPLY flag for new OTA update feature.
- (Qorvo component) Removed the script that merges the bootloader metadata to new bootloader image in UserModeBootloader_XP4002_secure.py_.
- (Qorvo component) Added bootloader_ota_section_start_address__ in linker script. This address is the start address that the new bootloader image will be downloaded to.
- (Qorvo component) Added qvCHIP_OtaGetAreaSizeForBootloader and qvCHIP_OtaGetRemainingAreaSize functions to serve the new bootloader download space logic and minimize reboot count logic.
- (Qorvo component) Modified the processing method for handling the remaining data in the last OTA block of each sub-image.



## New Features


**New OTA upgrade mechanism.** QPGFPT-34
- Modified the code logic for single reboot when the next OTA update package contains images for: SEFW, application bootloader, Matter/Zigbee application. Additional reboot is required for the other application image.
- New application bootloader image will now be downloaded to unused space in bootloader section. Bootloader metadata is no longer existed in bootloader sub image.





# Certified components

| Certification                              | Date         | Certificate                                                                                                                                                | Link                                                                                                                   |
| ------------------------------------------ | ------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------- |
| Bluetooth LE 5.4                           | Jun 12, 2024 | **Declaration ID**: D068807<br>**QDID**: 203579                                                                                                            | [https://qualification.bluetooth.com/ListingDetails/172011](https://qualification.bluetooth.com/ListingDetails/172011) |
| Bluetooth LE 5.4 - Design Reassessed       | Dec 19, 2024 | **DN**: Q338888                                                                                                                                            | [https://qualification.bluetooth.com/ListingDetails/243332](https://qualification.bluetooth.com/ListingDetails/243332) |
| Thread 1.4 - FTD Optimized                 | Sep 30, 2025 | [certificate](_static/14A153.0_2025_09_30.pdf)                                                                                                             | [https://www.threadgroup.org/Certified-Products](https://www.threadgroup.org/Certified-Products)                       |
| Thread 1.4 - MTD Optimized                 | Sep 30, 2025 | [certificate](_static/14A154.0_2025_09_30.pdf)                                                                                                             | [https://www.threadgroup.org/Certified-Products](https://www.threadgroup.org/Certified-Products)                       |
| Zigbee R23 Platform Certification          | Oct 11, 2024 | [certificate](https://api.knack.com/v1/applications/54e658034b4f44e42fb18201/download/asset/66ffb729e2b14202fbb2e6f4/zigbeeprocorexmlpicsr23_at_least.xml) | [https://csa-iot.org/csa_product/qpg6200/](https://csa-iot.org/csa_product/qpg6200/)                                   |
| Matter 1.3 - Combo Switch                  | Sep 17, 2024 | [certificate](https://api.knack.com/v1/applications/54e658034b4f44e42fb18201/download/asset/66e7f7cd581bea02ca954e95/pics_v1.3_color_dimmer_switch.zip)    | [https://csa-iot.org/csa_product/qorvo-qpg6200-dk-switch/](https://csa-iot.org/csa_product/qorvo-qpg6200-dk-switch/)   |
| Matter 1.4.0 - Thermostatic Radiator Valve | Aug 22, 2025 | [certificate](https://api.knack.com/v1/applications/54e658034b4f44e42fb18201/download/asset/681b2a0f6e247603129369ab/pics_trv_20250401_3002a46508.zip)     | [https://csa-iot.org/csa_product/qorvo-qpg6200-dk-trv/](https://csa-iot.org/csa_product/qorvo-qpg6200-dk-trv/)         |
| PSA Certified Level 2                      | May 21, 2025 | [certificate](https://trustcb.com/download/?wpdmdl=5166)                                                                                                   | [https://products.psacertified.org/products/qpg6200-with-secure-element](https://products.psacertified.org/products/qpg6200-with-secure-element) <br> [https://trustcb.com/download/?wpdmdl=5167](https://trustcb.com/download/?wpdmdl=5167)  |
