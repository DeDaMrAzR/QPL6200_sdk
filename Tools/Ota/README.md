# OTA image generation tool

This document describes the process of creating an image file for the over-the-air firmware upgrade process.
Such files can be used to perform a device firmware upgrade.

When calling `generate_ota_img.py`, you need to specify a signed .hex file produced during the build process.

## Usage

```
usage: generate_ota_img.py [-h] [--chip_config_header CHIP_CONFIG_HEADER] [--factory_data_config FACTORY_DATA_CONFIG] [--se_image_file SE_IMAGE_FILE]
                           [--bl_image_file BL_IMAGE_FILE] [-vn VERSION] [-mi MIN_VERSION] [-ma MAX_VERSION] [-vs VERSION_STR] [-tui] [-vid VENDOR_ID]
                           [-pid PRODUCT_ID] [--header_version HEADER_VERSION]
                           [--ota_offset OTA_OFFSET] [--flash_app_start_offset FLASH_APP_START_OFFSET]
                           [--flash-secondary-start-offset FLASH_SECONDARY_START_OFFSET] [--flash_start FLASH_START] [--compression {none,lzma}]
                           [--prune_only] [--jumptables] [--xp400x] [--developer-key FILE.der]
                           [--secondary-input-file SECONDARY_INPUT_FILE] --out_file OUT_FILE --in_file IN_FILE --chip_root CHIP_ROOT
```

### Example

```
"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Ota/generate_ota_img.py
    --bl_image_file "${SCRIPT_DIR}"/../../../Work/Bootloader_qpg6200_compr_signed/Bootloader_qpg6200_compr_signed_before_hexmerge.hex_
    --chip_config_header "${SCRIPT_DIR}"/../../../Applications/Matter/Switch/include/CHIPProjectConfig.h
    --chip_root "${SCRIPT_DIR}"/../../../Components/ThirdParty/Matter/repo
    --compression lzma
    --developer-key "${SCRIPT_DIR}"/../../../Tools/SecureBoot/developer_key_private.der
    --factory_data_config "${SCRIPT_DIR}"/../../../Tools/QorvoPlatformTools/Matter/Credentials/test_switch_qpg6200.factory_data_config
    --flash_app_start_offset 0xc400
    --flash_start 0x10013000
    --header_version 1
    --in_file "${SCRIPT_DIR}"/../../../Work//switch_qpg6200_development_thread_certified/switch_qpg6200_development_thread_certified.hex
    --out_file "${SCRIPT_DIR}"/../../../Work//switch_qpg6200_development_thread_certified/switch_qpg6200_development_thread_certified.ota
    --se_image_file "${SCRIPT_DIR}"/../../..//Binaries/SEFW/se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_408.hex
    --xp400x
```
