/*
 *   Copyright (c) 2017, Qorvo Inc
 *
 *   Upgrade functionality
 *   Implementation of gpUpgrade
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_UPGRADE

#include "gpHal.h"
#include "gpUtils.h"
#include "gpLog.h"
#include "gpUpgrade.h"
#include "gpUpgrade_defs.h"
#include "gpUpgrade_flash.h"
#include "hal_user_license.h"

#if defined(GP_UPGRADE_DIVERSITY_USE_EXTSTORAGE) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)
#include "gpExtStorage.h"
#endif // GP_COMP_EXTSTORAGE


#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "q_firmware_header.h"
#include "esec_qorvo.h"
#endif
#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "mm.h"
#include "hal_SEMailbox.h"
#define GP_MM_RAM_LINEAR_SIZE GP_MM_AKUMA_RAM_LINEAR_CODE_SIZE
#define GP_MM_FLASH_START               GP_MM_CMA_NVM_LINEAR_START
#define GP_MM_FLASH_ALT_START           GP_MM_CMA_NVM_LINEAR_START
#define FLASH_ALIGN_WRITE_UNIT(address) ((address) - ((address) % (FLASH_WRITE_UNIT)))
#endif
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

extern const UInt32 appImageLowerFlashStart;
#if defined(GP_UPGRADE_DIVERSITY_USE_EXTSTORAGE) || defined(GP_UPGRADE_DIVERSITY_COMPRESSION) ||                       \
    defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
const loaded_userlicense_t FLASH_PROGMEM upgrade_image_user_license LINKER_SECTION(
    ".upgrade_image_user_license_start") = {0};
extern const UInt32 upgImageUserLicenseStart;

#endif // _USE_EXTSTORAGE / _COMPRESSION / _DUAL_BOOT

#if defined(GP_DIVERSITY_GPHAL_XP4002)
extern const int bl_fw_header_Start;
extern const int appuc_fw_header_content;
#endif
/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
#if defined(GP_DIVERSITY_GPHAL_XP4002)
static gpHal_FlashError_t gpUpgrade_FlashUnitWrite(FlashPtr address, UInt8 length, UInt32* data);
#endif
/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpUpgrade_Init(void)
{
    gpHal_FlashError_t ret = gpHal_FlashError_Success;
    UInt32 loadCompleteMW = 0;
    (void)loadCompleteMW;

#if   defined(GP_DIVERSITY_APPUC_FW_HEADER)
    ret = gpUpgrade_FlashUnitWrite(upgImageUserLicenseStart + BOOTLOADER_MAILBOX_OTA_ADDRESS_OFFSET,
                                   sizeof(loadCompleteMW), &loadCompleteMW);
#elif defined(GP_UPGRADE_DIVERSITY_USE_EXTSTORAGE) || defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    /* Invalidate load completed magic word in preperation of downloading a new image */
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    ret = gpUpgrade_FlashUnitWrite(upgImageUserLicenseStart + USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD_OFFSET,
                                   sizeof(loadCompleteMW), &loadCompleteMW);
#else
    ret = gpHal_FlashProgramSector(upgImageUserLicenseStart + USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD_OFFSET,
                                   sizeof(loadCompleteMW), (UInt8*)&loadCompleteMW);
#endif
#endif
    GP_ASSERT_SYSTEM(ret == gpHal_FlashError_Success);

#if defined(GP_DIVERSITY_LOG) && (!defined(GP_DIVERSITY_APPUC_FW_HEADER))
    GP_LOG_SYSTEM_PRINTF("Upgrade Loaded MW remove", 0);
#endif
}

#if defined(GP_DIVERSITY_APPUC_FW_HEADER)
gpUpgrade_Status_t gpUpgrade_SetPendingImage(UInt32 swVer, UInt32 hwVer, UInt32 startAddr, UInt32 imgSz)
{
    gpHal_FlashError_t ret;

    UInt32* OtaFirmwareHeader = (UInt32*)startAddr;
    if(OtaFirmwareHeader[APPUC_FW_HEADER_IMAGE_TYPE_OFFSET / 4] != APPUC_FW_HEADER_IMAGE_TYPE_COMPRESSED_APP)
    {
        return gpUpgrade_StatusInvalidImage;
    }

    ret = gpUpgrade_FlashUnitWrite(upgImageUserLicenseStart + BOOTLOADER_MAILBOX_OTA_ADDRESS_OFFSET, sizeof(startAddr),
                                   &startAddr);
    if(gpHal_FlashError_Success != ret)
    {
        return gpUpgrade_StatusFailedVerify;
    }
    return gpUpgrade_StatusSuccess;
}
#else

gpUpgrade_Status_t gpUpgrade_SetPendingImage(UInt32 swVer, UInt32 hwVer, UInt32 startAddr, UInt32 imgSz)
{
    /* TODO : remove the input arguments */
    gpHal_FlashError_t ret;
    UInt32 progLoadMW;
    UInt8 activeImagefreshnessCounter, pendingImagefreshnessCounter;
    UInt32 activeImageAddress = 0, pendingImageAddress = 0;
#if   defined(GP_UPGRADE_DIVERSITY_USE_EXTSTORAGE) || defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    /* Copy the license of the external image to internal flash */
    UInt8 upgLicense[LOADED_USER_LICENSE_TOTAL_SIZE+EXTENDED_USER_LICENSE_TOTAL_SIZE];
#if defined(GP_UPGRADE_DIVERSITY_USE_EXTSTORAGE) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)
    gpExtStorage_ReadBlock(gpUpgrade_GetOtaAreaStartAddress(), LOADED_USER_LICENSE_TOTAL_SIZE+EXTENDED_USER_LICENSE_TOTAL_SIZE, upgLicense);
#elif defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    gpHal_FlashRead(gpUpgrade_GetOtaAreaStartAddress(), LOADED_USER_LICENSE_TOTAL_SIZE+EXTENDED_USER_LICENSE_TOTAL_SIZE, upgLicense);
#endif
#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
    //Back-up the boot address in the Upgrade Image User License.
    UInt32 bootAddr = 0;
    gpHal_FlashRead(upgImageUserLicenseStart + USER_LICENSE_BOOT_ADDRESS_OFFSET, sizeof(bootAddr), (UInt8*)&bootAddr);
    /* Mask 3 most significant bits, that are used by the customer for the test mode. APPSCS-4452, SDP013-1567. */
    bootAddr &= USER_LICENSE_BOOT_ADDRESS_BITMASK;
    MEMCPY(upgLicense + USER_LICENSE_BOOT_ADDRESS_OFFSET, (UInt8*)&bootAddr, sizeof(bootAddr));
#endif //GP_UPGRADE_DIVERSITY_DUAL_BOOT
    ret = gpHal_FlashProgramSector(upgImageUserLicenseStart , LOADED_USER_LICENSE_TOTAL_SIZE+EXTENDED_USER_LICENSE_TOTAL_SIZE, upgLicense);
    GP_ASSERT_SYSTEM(ret == gpHal_FlashError_Success);

    activeImageAddress = appImageLowerFlashStart;
    pendingImageAddress = upgImageUserLicenseStart;
#endif
    GP_LOG_PRINTF("Active_image_addr 0x%lx, Pending_image_addr 0x%lx", 0, activeImageAddress, pendingImageAddress);
    /* TODO: The OTA flow must call this function only after making sure that the image is valid
       This sanity check can be removed in the future.
       */
    gpHal_FlashRead(pendingImageAddress + USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD_OFFSET, sizeof(progLoadMW), (UInt8*)&progLoadMW);
    if (progLoadMW != USER_LICENSE_PROGRAM_LOADED_MAGIC_WORD)
    {
        GP_LOG_SYSTEM_PRINTF("gpUpgrade_SetPendingImage failed, pending image has invalid magic word:%lx", 0,
                             progLoadMW);
        return gpUpgrade_StatusInvalidImage;
    }


    gpHal_FlashRead(activeImageAddress + LOADED_USER_LICENSE_FRESHNESS_COUNTER_OFFSET, sizeof(activeImagefreshnessCounter), (UInt8*)&activeImagefreshnessCounter);

    /* Update the freshness counter in the pending image so that is is picked up by the UMB on next reboot */
    pendingImagefreshnessCounter = activeImagefreshnessCounter + 1;
    ret = gpHal_FlashProgramSector(pendingImageAddress + LOADED_USER_LICENSE_FRESHNESS_COUNTER_OFFSET, sizeof(pendingImagefreshnessCounter), (UInt8*)&pendingImagefreshnessCounter);
    GP_ASSERT_SYSTEM(ret == gpHal_FlashError_Success);
#if !defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    /* Write the load complete magic word to indicate that the pending image is ready to be installed */
    UInt32 loadCompleteMW = LOADED_USER_LICENSE_LOAD_COMPLETED_MAGIC_WORD;
    ret = gpHal_FlashProgramSector(pendingImageAddress + LOADED_USER_LICENSE_LOAD_COMPLETED_MAGIC_WORD_OFFSET, sizeof(loadCompleteMW), (UInt8*)&loadCompleteMW);
    GP_ASSERT_SYSTEM(ret == gpHal_FlashError_Success);
#endif
    GP_LOG_PRINTF("Freshness Counter (Active Image) %x, (Pending Image) %lx", 0, activeImagefreshnessCounter, pendingImagefreshnessCounter);
    return gpUpgrade_StatusSuccess;
}
#endif

#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
gpUpgrade_Status_t gpUpgrade_SetBootAddress(UInt32 bootAddr)
{
    gpHal_FlashError_t ret = gpHal_FlashError_VerifyFailure;
    gpUpgrade_Status_t retVal = gpUpgrade_StatusSuccess;
#if !defined(GP_DIVERSITY_APPUC_FW_HEADER)
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    ret = gpUpgrade_FlashUnitWrite(upgImageUserLicenseStart + USER_LICENSE_BOOT_ADDRESS_OFFSET, sizeof(bootAddr),
                                   &bootAddr);
#else
    ret = gpHal_FlashProgramSector(upgImageUserLicenseStart + USER_LICENSE_BOOT_ADDRESS_OFFSET, sizeof(bootAddr),
                                   (UInt8*)&bootAddr);
#endif
#else
    ret = gpUpgrade_FlashUnitWrite(upgImageUserLicenseStart + BOOTLOADER_MAILBOX_BOOT_ADDRESS_OFFSET, sizeof(bootAddr),
                                   &bootAddr);

#endif
    if(gpHal_FlashError_Success != ret)
    {
        //Failed to write the boot address in the upgrade image user license.
        retVal = gpUpgrade_StatusWriteError;
    }

    return retVal;
}
#endif //GP_UPGRADE_DIVERSITY_DUAL_BOOT

void gpUpgrade_Reset(void)
{
    gpUpgrade_ClrCrc();
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    hal_SEMailbox_ResetRequest(GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP);
#else
    UInt32* addr = (UInt32*)GP_MM_RAM_CRC_START;

    *addr = GP_UPGRADE_MAGIC_WORD;
    GP_WB_WRITE_PMUD_SOFT_POR_BOOTLOADER(1);
#endif
}
#if defined(GP_DIVERSITY_GPHAL_XP4002)
static gpHal_FlashError_t gpUpgrade_FlashUnitWrite(FlashPtr address, UInt8 length, UInt32* data)
{
    UInt32 writeAddressAligned = FLASH_ALIGN_WRITE_UNIT(address);
    UInt8 writeAlignOffset = address - writeAddressAligned;
    UInt32 buf[FLASH_WRITE_UNIT / sizeof(UInt32)];

    gpHal_FlashError_t ret = gpHal_FlashRead(writeAddressAligned, FLASH_WRITE_UNIT, (UInt8*)buf);
    if(gpHal_FlashError_Success == ret)
    {
        GP_ASSERT_SYSTEM(length <= FLASH_WRITE_UNIT - writeAlignOffset);
        memcpy((UInt8*)buf + writeAlignOffset, data, length);
        ret = gpHal_FlashWrite(writeAddressAligned, FLASH_WRITE_UNIT / sizeof(UInt32), (UInt32*)buf);
    }
    return ret;
}
#endif // defined(GP_DIVERSITY_GPHAL_XP4002)

#if defined(GP_DIVERSITY_GPHAL_XP4002)
Bool gpUpgrade_GetBootloaderVersion(uint32_t* version, uint32_t* app_changelist_specifier,
                                    const void* bootloader_header)
{
    appuc_fw_header_t* bootloader_fw_header_content = (appuc_fw_header_t*)&bl_fw_header_Start;
    if(bootloader_header)
    {
        bootloader_fw_header_content = (appuc_fw_header_t*)bootloader_header;
    }
    if(version == NULL || app_changelist_specifier == NULL)
    {
        return false;
    }

    /* convert from major/minor/patch/.. (big endian) to LE uint32 */
    (*version) = (((bootloader_fw_header_content->appuc_fw_version_info.app_version >> 0) & 0xff) << 24) |
                      (((bootloader_fw_header_content->appuc_fw_version_info.app_version >> 8) & 0xff) << 16) |
                      (((bootloader_fw_header_content->appuc_fw_version_info.app_version >> 16) & 0xff) << 8) |
                      (((bootloader_fw_header_content->appuc_fw_version_info.app_version >> 24) & 0xff) << 0);

    *app_changelist_specifier = bootloader_fw_header_content->appuc_fw_version_info.app_changelist_specifier;
    return true;
}

gpUpgrade_Status_t gpUpgrade_SetPendingBootloaderImage(UInt32 startAddr, UInt32 imgSz)
{
    uint32_t result;

    gpLog_PrintBuffer(16, (void*)startAddr);
    startAddr -= 0x10000000;

    result = esec_start_upgrade(0, startAddr);

    if(ESEC_OKAY == result)
    {
        return gpUpgrade_StatusSuccess;
    }
    return gpUpgrade_StatusInvalidImage;
}
Bool gpUpgrade_GetSecureElementVersion(uint8_t* version, int length, const void* upgrade_image_header)
{
    if(version == NULL)
    {
        return false;
    }
    if(upgrade_image_header)
    {
        GP_ASSERT_SYSTEM(false);
        /* not supported yet */
        return false;
    }
    else
    {
        uint32_t status;
        if(length < 128)
        {
            GP_ASSERT_SYSTEM(false);
            return false;
        }
        status = esec_get_seucfw_version(version);
        return (status == ESEC_OKAY);
    }
    return true;
}

gpUpgrade_Status_t gpUpgrade_SetPendingSecureElementImage(UInt32 startAddr, UInt32 imgSz)
{
    uint32_t result;

    startAddr -= 0x10000000;

    result = esec_start_upgrade(1, startAddr);
    if(ESEC_OKAY == result)
    {
        return gpUpgrade_StatusSuccess;
    }
    return gpUpgrade_StatusInvalidImage;
}

Bool gpUpgrade_GetApplicationVersionFromHeader(UInt32* version_word, const void* header)
{
    appuc_fw_header_t* input_data = (appuc_fw_header_t*)header;

    if(input_data == NULL)
    {
        // read from active application
        input_data = (appuc_fw_header_t*)&appuc_fw_header_content;
    }
    if(version_word == NULL)
    {
        return false;
    }
    /* convert from major/minor/patch/.. (big endian) to LE uint32 */
    (*version_word) = (((input_data->appuc_fw_version_info.app_version >> 0) & 0xff) << 24) |
                      (((input_data->appuc_fw_version_info.app_version >> 8) & 0xff) << 16) |
                      (((input_data->appuc_fw_version_info.app_version >> 16) & 0xff) << 8) |
                      (((input_data->appuc_fw_version_info.app_version >> 24) & 0xff) << 0);

    return true;
}

#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
/* Use the symbol from linker which is available only if GP_DIVERSITY_DUALBOOT_JUMP_ADDRESS is defined.
 * This is done to avoid errors when building standalone components, like libraries.
 */
extern const uint32_t __secondary_application_address;

Bool gpUpgrade_GetSecondaryApplicationVersionFromHeader(UInt32* version_word, const void* header)
{
    appuc_fw_header_t* input_data = (appuc_fw_header_t*)header;

    if(input_data == NULL)
    {
        // read from active application based on
        input_data = (appuc_fw_header_t*)&__secondary_application_address;
    }
    if(version_word == NULL)
    {
        return false;
    }
    /* convert from major/minor/patch/.. (big endian) to LE uint32 */
    (*version_word) = (((input_data->appuc_fw_version_info.app_version >> 0) & 0xff) << 24) |
                      (((input_data->appuc_fw_version_info.app_version >> 8) & 0xff) << 16) |
                      (((input_data->appuc_fw_version_info.app_version >> 16) & 0xff) << 8) |
                      (((input_data->appuc_fw_version_info.app_version >> 24) & 0xff) << 0);

    return true;
}
#endif //GP_UPGRADE_DIVERSITY_DUAL_BOOT

#endif
