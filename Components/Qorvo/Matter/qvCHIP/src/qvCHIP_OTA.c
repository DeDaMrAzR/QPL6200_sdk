/*
 * Copyright (c) 2021, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 */

/** @file "qvCHIP_OTA.c"
 *
 *  OTA Upgrade functionality
 *
 *  Implementation of qvCHIP OTA API
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
/* <CodeGenerator Placeholder> Includes */

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVCHIP

// #define GP_LOCAL_LOG

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "qvCHIP.h"
#include "gpLog.h"

#include "gpAssert.h"
#include "gpUpgrade.h"

#include "esec_features.h"
#include "esecure.h"
#include "eSecure_metadata.h"
#include "mbedtls/sha256.h"
#include "q_firmware_header.h"

/* </CodeGenerator Placeholder> Includes */

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> Macro */
#define PRIMARY_HEADER_SIGNATURE_SIZE (256)
/* </CodeGenerator Placeholder> Macro */

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticData */
qvCHIP_OtaHeaderValidationCback_t pOtaHeaderValidationCb = NULL;
qvCHIP_OtaUpgradeHandledCback_t pOtaUpgradeHandledCb = NULL;

static uint32_t OtaLastWriteAddress = 0;

#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
static uint32_t gOtaBlWriteAddress = 0;
static uint32_t gOtaStartWriteAddress = 0;
static qvCHIP_OtaImageType_t gCurrentImageType = qvCHIP_OtaImageTypeInvalid;

extern uint32_t bootloader_ota_section_start_address__;
static const uint32_t bootloaderOtaSectionStartAddr __attribute__((used)) =
    (uint32_t)&bootloader_ota_section_start_address__;
#endif // GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET

static mbedtls_sha256_context payload_sha256_ctx;

#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
extern const UInt32 upgrade_image_user_license_start__;
const UInt32 UserLicenseAddr = (UInt32)&upgrade_image_user_license_start__;
#endif

#include "global.h"
#include "gpSecureBoot_PublicKey.c"
/* </CodeGenerator Placeholder> StaticData */

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticFunctionDefinitions */
static qvCHIP_OtaStatus_t OtaMapFileOffsetToMemoryMap(uint32_t chunk_offset, uint32_t length,
                                                      uint32_t* memorymap_offset);
#if !defined(GP_UPGRADE_DIVERSITY_COMPRESSION) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)
static bool OtaChunkExceedsJumpTableBoundary(uint32_t chunk_offset, uint32_t length, uint32_t* jump_table_data_size,
                                             uint32_t* ota_data_size);
static qvCHIP_OtaStatus_t OtaWritePartialData(uint32_t offset, uint16_t length, uint8_t* dataChunk);
#endif
/* </CodeGenerator Placeholder> StaticFunctionDefinitions */

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticFunctionDefinitions */
static qvCHIP_OtaStatus_t OtaMapFileOffsetToMemoryMap(uint32_t chunk_offset, uint32_t length,
                                                      uint32_t* memorymap_offset)
{
#if !defined(GP_DIVERSITY_GPHAL_XP4002)
    uint32_t jumptable_length = gpUpgrade_GetJumptableOtaAreaSize();
#endif

    if(!memorymap_offset)
    {
        GP_ASSERT_SYSTEM(memorymap_offset);
        return qvCHIP_OtaStatusInvalidParam;
    }
#if defined(GP_DIVERSITY_GPHAL_XP4002)
#if defined(GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET)
    (*memorymap_offset) = chunk_offset + gOtaStartWriteAddress;
#else
    (*memorymap_offset) = chunk_offset + gpUpgrade_GetOtaAreaStartAddress();
#endif
    return qvCHIP_OtaStatusSuccess;
#else

#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    if(chunk_offset + length > jumptable_length + gpUpgrade_GetOtaAreaSize())
    {
        return qvCHIP_OtaStatusInvalidAddress;
    }

    (*memorymap_offset) = chunk_offset + gpUpgrade_GetOtaAreaStartAddress() - jumptable_length;
    return qvCHIP_OtaStatusSuccess;
#else
    if(chunk_offset < jumptable_length)
    {
        /* Bridging the boundary is not accounted for in this function, so assert. */
        if(chunk_offset + length > jumptable_length)
        {
            return qvCHIP_OtaStatusInvalidAddress;
        }
        GP_LOG_PRINTF("jt chunk sa %lx", 0, gpUpgrade_GetJumptableOtaAreaStartAddress());
        (*memorymap_offset) = chunk_offset + gpUpgrade_GetJumptableOtaAreaStartAddress();
        return qvCHIP_OtaStatusSuccess;
    }
    else
    {
        (*memorymap_offset) = chunk_offset + gpUpgrade_GetOtaAreaStartAddress() - jumptable_length;
        return qvCHIP_OtaStatusSuccess;
    }
#endif
#endif
}

#if !defined(GP_UPGRADE_DIVERSITY_COMPRESSION) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)
static bool OtaChunkExceedsJumpTableBoundary(uint32_t chunk_offset, uint32_t length, uint32_t* jump_table_data_size,
                                             uint32_t* ota_data_size)
{
    uint32_t jumptable_length = gpUpgrade_GetJumptableOtaAreaSize();
    bool retVal = false;

    if(chunk_offset < jumptable_length)
    {
        if(chunk_offset + length > jumptable_length)
        {
            (*jump_table_data_size) = jumptable_length - chunk_offset;
            (*ota_data_size) = (chunk_offset + length) - jumptable_length;
            retVal = true;
        }
    }

    return retVal;
}

static qvCHIP_OtaStatus_t OtaWritePartialData(uint32_t offset, uint16_t length, uint8_t* dataChunk)
{
    qvCHIP_OtaStatus_t result = qvCHIP_OtaStatusSuccess;
    uint32_t memorymap_offset = 0;

    result = OtaMapFileOffsetToMemoryMap(offset, length, &memorymap_offset);

    if(result != qvCHIP_OtaStatusSuccess)
    {
        return result;
    }

    OtaLastWriteAddress = memorymap_offset + length;
    GP_LOG_PRINTF("gpUpgrade_WriteChunkaddr:%lx l:%u", 0, (unsigned long)memorymap_offset, length);
    return (qvCHIP_OtaStatus_t)gpUpgrade_WriteChunk(memorymap_offset, length, dataChunk);
}

#endif

static bool ParseSecureElementVersionString(const uint8_t* buffer, size_t buffer_len, uint32_t* version_out)
{
    const char* cursor = (const char*)buffer;
    const char* limit = (const char*)(buffer + buffer_len);

    *version_out = 0;

    // Skip non-digit characters
    while(cursor < limit && !isdigit((unsigned char)*cursor))
    {
        cursor++;
    }
    if(cursor == limit)
    {
        return false;
    }

    char* endptr = NULL;
    uint32_t major = strtoul(cursor, &endptr, 10);
    if(endptr == cursor || major > 0xFFUL)
    {
        return false;
    }
    cursor = endptr;

    if(cursor >= limit || *cursor != '.')
    {
        return false;
    }
    cursor++;

    uint32_t minor = strtoul(cursor, &endptr, 10);
    if(endptr == cursor || minor > 0xFFUL)
    {
        return false;
    }
    cursor = endptr;

    if(cursor >= limit || *cursor != '.')
    {
        return false;
    }
    cursor++;

    uint32_t patch = strtoul(cursor, &endptr, 10);
    if(patch > 0xFFUL)
    {
        return false;
    }

    *version_out = ((uint32_t)major << 16) | ((uint32_t)minor << 8) | (uint32_t)patch;
    return true;
}

static qvCHIP_OtaStatus_t OtaWriteProgressInfo(qvCHIP_OtaProgressStorage_t* progressStorage)
{
    qvCHIP_OtaStatus_t status;
    if(progressStorage == NULL)
    {
        return qvCHIP_OtaStatusInvalidAddress;
    }
    uint32_t ota_end_address = gpUpgrade_GetOtaAreaStartAddress() + gpUpgrade_GetOtaAreaSize();
    status = (qvCHIP_OtaStatus_t)gpUpgrade_WriteChunk(ota_end_address - sizeof(qvCHIP_OtaProgressStorage_t),
                                                      sizeof(qvCHIP_OtaProgressStorage_t), (uint8_t*)progressStorage);

    GP_LOG_PRINTF("OtaWriteProgressInfo at %" PRIu32 ", status: %d", 0,
                  ota_end_address - sizeof(qvCHIP_OtaProgressStorage_t), status);
    // gpLog_PrintBuffer(sizeof(qvCHIP_OtaProgressStorage_t), (uint8_t *)progressStorage);

    return status;
}

static qvCHIP_OtaStatus_t OtaReadProgressInfo(qvCHIP_OtaProgressStorage_t* progressStorage)
{
    qvCHIP_OtaStatus_t status;
    if(progressStorage == NULL)
    {
        return qvCHIP_OtaStatusInvalidAddress;
    }
    uint32_t ota_end_address = gpUpgrade_GetOtaAreaStartAddress() + gpUpgrade_GetOtaAreaSize();
    status = gpUpgrade_ReadChunk(ota_end_address - sizeof(qvCHIP_OtaProgressStorage_t),
                                 sizeof(qvCHIP_OtaProgressStorage_t), (uint8_t*)progressStorage);

    GP_LOG_PRINTF("OtaReadProgressInfo from %" PRIu32 ", status: %d", 0,
                  ota_end_address - sizeof(qvCHIP_OtaProgressStorage_t), status);
    // gpLog_PrintBuffer(sizeof(qvCHIP_OtaProgressStorage_t), (uint8_t *)progressStorage);

    return status;
}

/* </CodeGenerator Placeholder> StaticFunctionDefinitions */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

bool qvCHIP_OtaValidateImage(qvCHIP_Ota_ImageHeader_t imageHeader)
{
    GP_LOG_PRINTF("qvCHIP_OtaValidateImage", 0);
    if(pOtaHeaderValidationCb == NULL)
    {
        return true;
    }

    return pOtaHeaderValidationCb(imageHeader);
}

void qvCHIP_OtaEraseArea(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaEraseArea", 0);
    gpUpgrade_EraseOtaArea();
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    gOtaStartWriteAddress = gpUpgrade_GetOtaAreaStartAddress();
#endif // GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
}

void qvCHIP_OtaStartEraseArea(qvCHIP_OtaEraseCompleteCback_t cb)
{
    GP_LOG_PRINTF("qvCHIP_OtaStartEraseArea", 0);
    gpUpgrade_StartEraseOtaArea(cb);
}

uint32_t qvCHIP_OtaGetAreaSize(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaGetAreaSize", 0);
    return gpUpgrade_GetOtaAreaSize();
}

uint32_t qvCHIP_OtaGetAreaSizeForBootloader(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaGetAreaSizeForBootloader", 0);
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    // Bootloader OTA area will be inside the unused part of bootloader section
    // It starts from bootloaderOtaSectionStartAddr to upgImageUserLicenseStartAddr
    // bootloaderOtaSectionStartAddr is defined in the linker file
    GP_ASSERT_SYSTEM(bootloaderOtaSectionStartAddr < UserLicenseAddr);
    return (uint32_t)(UserLicenseAddr - bootloaderOtaSectionStartAddr);
#else
    // Bootloader OTA image will be stored in the same OTA area as application image
    return gpUpgrade_GetOtaAreaSize();
#endif
}

bool qvCHIP_OtaCheckRemainingAreaSize(uint32_t imageSize)
{
    GP_LOG_PRINTF("qvCHIP_OtaCheckRemainingAreaSize", 0);
    // Get total OTA area size
    uint32_t totalSize = gpUpgrade_GetOtaAreaSize();

#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    // in this case, OtaLastWriteAddress is the trully memory address in flash, so it should be compared with the end
    // address of the OTA area directly
    if((OtaLastWriteAddress + imageSize) > (gpUpgrade_GetOtaAreaStartAddress() + totalSize))
    {
        return false;
    }
#else
    // in this case, OtaLastWriteAddress is the offset in the OTA area, so it should be compared with totalSize directly
    if((OtaLastWriteAddress + imageSize) > totalSize)
    {
        return false;
    }
#endif // GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET

    return true;
}

void qvCHIP_OtaStartWrite(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaStartWrite", 0);
    // Initialize the SHA-256 context
    mbedtls_sha256_init(&payload_sha256_ctx);
    mbedtls_sha256_starts(&payload_sha256_ctx, 0); // 0 indicates SHA-256 (not SHA-224)

    gpUpgrade_StartWrite();
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    if(gCurrentImageType != qvCHIP_OtaImageTypeBootloader)
    {
        if(OtaLastWriteAddress == 0)
        {
            // This is the first time to write application image, start from the beginning of OTA area
            OtaLastWriteAddress = gpUpgrade_GetOtaAreaStartAddress();
        }
        gOtaStartWriteAddress = OtaLastWriteAddress + 1;
        GP_LOG_PRINTF("gOtaStartWriteAddress %lx", 0, gOtaStartWriteAddress);
    }
    else
    {
        gOtaStartWriteAddress = bootloaderOtaSectionStartAddr;
        GP_LOG_PRINTF("gOtaStartWriteAddress %lx", 0, gOtaStartWriteAddress);
    }
#endif // GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
}

qvCHIP_OtaStatus_t qvCHIP_OtaWriteChunk(uint32_t offset, uint16_t length, uint8_t* dataChunk)
{
    qvCHIP_OtaStatus_t result = qvCHIP_OtaStatusSuccess;
    uint32_t memorymap_offset = 0;
    GP_LOG_PRINTF("qvCHIP_OtaWriteChunk addr:%lx l:%u", 0, (unsigned long)offset, length);

    mbedtls_sha256_update(&payload_sha256_ctx, dataChunk, length);
    if(NULL == dataChunk || 0 == length)
    {
        return qvCHIP_OtaStatusInvalidParam;
    }

#if !defined(GP_UPGRADE_DIVERSITY_COMPRESSION) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)
    uint32_t jump_table_data_size = 0;
    uint32_t ota_data_size = 0;

    if(OtaChunkExceedsJumpTableBoundary(offset, length, &jump_table_data_size, &ota_data_size))
    {
        // We received a block that contains data for Jumptable and data for upgrade application. This needs to be split
        // and stored in seperate sections in flash. See Figure 2.2 of the document below to understand how the
        // jumptables and the application are stored in flash (similar layout for external OTA and internal
        // non-compressed OTA)
        // Components/Qorvo/Bootloader/vlatest/apps/AppBootloader/doc/pdf/SW30236_AN_Vol_2_Secure_User_Mode_Bootloader_Implementation.pdf

        GP_ASSERT_SYSTEM(length == (jump_table_data_size + ota_data_size));

        result = OtaWritePartialData(offset, jump_table_data_size, dataChunk);
        if(result != qvCHIP_OtaStatusSuccess)
        {
            return result;
        }

        result = OtaWritePartialData(offset + jump_table_data_size, ota_data_size, dataChunk + jump_table_data_size);
        if(result != qvCHIP_OtaStatusSuccess)
        {
            return result;
        }

        return qvCHIP_OtaStatusSuccess;
    }
#endif //! defined(GP_UPGRADE_DIVERSITY_COMPRESSION) && !defined(GP_UPGRADE_DIVERSITY_USE_INTSTORAGE)

    result = OtaMapFileOffsetToMemoryMap(offset, length, &memorymap_offset);
    if(result != qvCHIP_OtaStatusSuccess)
    {
        return result;
    }
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    if(gCurrentImageType != qvCHIP_OtaImageTypeBootloader)
    {
        OtaLastWriteAddress = memorymap_offset + length;
    }
    else
    {
        gOtaBlWriteAddress = memorymap_offset + length;
    }
#else
    OtaLastWriteAddress = offset + length;
#endif

    GP_LOG_PRINTF("gpUpgrade_WriteChunkaddr:%lx l:%u", 0, (unsigned long)memorymap_offset, length);
    return (qvCHIP_OtaStatus_t)gpUpgrade_WriteChunk(memorymap_offset, length, dataChunk);
}

qvCHIP_OtaStatus_t qvCHIP_OtaReadChunk(uint32_t offset, uint16_t length, uint8_t* dataChunk)
{
    qvCHIP_OtaStatus_t result;
    uint32_t memorymap_offset;
    GP_LOG_PRINTF("qvCHIP_OtaReadChunk addr:%lx l:%u", 0, (unsigned long)offset, length);
    if(NULL == dataChunk || 0 == length)
    {
        return qvCHIP_OtaStatusInvalidParam;
    }

    result = OtaMapFileOffsetToMemoryMap(offset, length, &memorymap_offset);

    if(result != qvCHIP_OtaStatusSuccess)
    {
        return result;
    }

    return (qvCHIP_OtaStatus_t)gpUpgrade_ReadChunk(memorymap_offset, length, dataChunk);
}

uint32_t qvCHIP_OtaGetCrc(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaGetCrc", 0);
    return gpUpgrade_GetCrc();
}

void qvCHIP_OtaSetCrc(uint32_t crcValue)
{
    GP_LOG_PRINTF("qvCHIP_OtaSetCrc", 0);
    gpUpgrade_SetCrc(crcValue);
}
qvCHIP_OtaStatus_t qvCHIP_OtaGetHash(uint8_t* calculated_hash, uint32_t length)
{
    if(length != 32)
    {
        return qvCHIP_OtaStatusInvalidParam;
    }
    // Finalize the hash computation
    mbedtls_sha256_finish(&payload_sha256_ctx, calculated_hash);

    // Free the SHA-256 context
    mbedtls_sha256_free(&payload_sha256_ctx);

    return qvCHIP_OtaStatusSuccess;
}

qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingImage(void)
{
    uint32_t swVer = 0;
    uint32_t hwVer = 0;

#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    uint32_t startAddr = gOtaStartWriteAddress;
    uint32_t imgSz = OtaLastWriteAddress - startAddr;
#else
    uint32_t startAddr = gpUpgrade_GetOtaAreaStartAddress();
    uint32_t imgSz = OtaLastWriteAddress;
#endif

    GP_LOG_PRINTF("qvCHIP_OtaSetPendingImage %lx len %ld", 0, startAddr, imgSz);

    // NOTE : In this implemenation the arguments provided in this API are unused.
    // Instead the License area based approach is used where the versions are stored
    // a dedicated programmable memory area during the factory flow.
    return (qvCHIP_OtaStatus_t)gpUpgrade_SetPendingImage(swVer, hwVer, startAddr, imgSz);
}

qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingBootloader(void)
{
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    uint32_t startAddr = bootloaderOtaSectionStartAddr;
    uint32_t imgSz = gOtaBlWriteAddress - startAddr;
#else
    uint32_t startAddr = gpUpgrade_GetOtaAreaStartAddress();
    uint32_t imgSz = OtaLastWriteAddress;
#endif
    GP_LOG_PRINTF("qvCHIP_OtaSetPendingBootloader %lx len %ld", 0, startAddr, imgSz);
    if(imgSz == 0)
    {
        return (qvCHIP_OtaStatus_t)qvCHIP_OtaStatusInvalidImage;
    }

    gpUpgrade_Status_t result;
    result = gpUpgrade_SetPendingBootloaderImage(startAddr, imgSz);

    if(gpUpgrade_StatusSuccess == result)
    {
        return qvCHIP_OtaStatusSuccess;
    }
    return (qvCHIP_OtaStatus_t)qvCHIP_OtaStatusInvalidImage;
}
qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingSecureElement(void)
{
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    uint32_t startAddr = gOtaStartWriteAddress;
    uint32_t imgSz = OtaLastWriteAddress - startAddr;
#else
    uint32_t startAddr = gpUpgrade_GetOtaAreaStartAddress();
    uint32_t imgSz = OtaLastWriteAddress;
#endif

    gpUpgrade_Status_t result;
    GP_LOG_PRINTF("qvCHIP_OtaSetPendingSecureElement %lx len %ld", 0, startAddr, imgSz);
    if(imgSz == 0)
    {
        return (qvCHIP_OtaStatus_t)qvCHIP_OtaStatusInvalidImage;
    }

    result = gpUpgrade_SetPendingSecureElementImage(startAddr, imgSz);

    if(gpUpgrade_StatusSuccess == result)
    {
        return qvCHIP_OtaStatusSuccess;
    }
    return (qvCHIP_OtaStatus_t)qvCHIP_OtaStatusInvalidImage;
}

void qvCHIP_OtaReset(void)
{
    GP_LOG_PRINTF("qvCHIP_OtaReset", 0);
    gpLog_Flush();
    gpUpgrade_Reset();
}

void gpUpgrade_cbUpgradeHandled(bool upgradeHandled, qvCHIP_OtaStatus_t upgradeStatus)
{
    GP_LOG_PRINTF("gpUpgrade_cbUpgradeHandled", 0);
    if(pOtaUpgradeHandledCb != NULL)
    {
        pOtaUpgradeHandledCb(upgradeHandled, upgradeStatus);
    }
}

void qvCHIP_OtaSetUpgradeHandledCb(qvCHIP_OtaUpgradeHandledCback_t upgradeHandledCb)
{
    GP_LOG_PRINTF("qvCHIP_OtaSetUpgradeHandledCb", 0);
    pOtaUpgradeHandledCb = upgradeHandledCb;
}

void qvCHIP_OtaSetHeaderValidationCb(qvCHIP_OtaHeaderValidationCback_t headerValidationCb)
{
    GP_LOG_PRINTF("qvCHIP_OtaSetHeaderValidationCb", 0);
    pOtaHeaderValidationCb = headerValidationCb;
}
#if defined(GP_DIVERSITY_GPHAL_XP4002)
int qvCHIP_CompareBootloaderVersion(const uint32_t version_a, const uint32_t app_changelist_specifier_a,
                                    const uint32_t version_b, const uint32_t app_changelist_specifier_b)
{
    return version_a - version_b;
}

bool qvCHIP_GetBootloaderVersion(uint32_t* version, uint32_t* app_changelist_specifier, const void* bootloader_header)
{
    return gpUpgrade_GetBootloaderVersion(version, app_changelist_specifier, bootloader_header);
}

bool qvCHIP_GetSecureElementVersion(uint32_t* version)
{
    uint8_t buffer[128];
    bool result = false;

    *version = 0;

    result = gpUpgrade_GetSecureElementVersion(buffer, 128, NULL);
    if(true == result)
    {
        result = ParseSecureElementVersionString(buffer, sizeof(buffer), version);
    }
    return result;
}

bool qvCHIP_GetApplicationVersionFromHeader(qvCHIP_OtaImageType_t app_type, uint32_t* version_word)
{
    if(qvCHIP_OtaImageTypePrimaryApplication == app_type)
    {
        /* In case of primary application in dual boot or standalone application,
         * we let the gpUpgrade API take care of finding the correct header. */
        return gpUpgrade_GetApplicationVersionFromHeader(version_word, NULL);
    }
    else if(qvCHIP_OtaImageTypeSecondaryApplication == app_type)
    {
#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
        /* If the application does not support dual boot it should not accept
         * qvCHIP_OtaImageTypeSecondaryApplication, return false in this case. */
        return gpUpgrade_GetSecondaryApplicationVersionFromHeader(version_word, NULL);
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
    }

    return false;
}

#endif

bool qvCHIP_OtaVerifyContentHash()
{
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);

    uint8_t index = otaProgressHeader.currentImageIndex;
    qvCHIP_Ota_SubImageHeader_t header = otaProgressHeader.imageHeaders[index];

    uint8_t calculated_hash[32]; // Buffer to hold the calculated hash

    qvCHIP_OtaGetHash(calculated_hash, 32);
    // Compare the calculated hash with the expected hash
    if(memcmp(calculated_hash, header.contentHash, 32) == 0)
    {
        GP_LOG_SYSTEM_PRINTF("secondary header hash OK", 0);
        return true; // Hashes match
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("secondary hash mismatch", 0);
        return false; // Hashes do not match
    }
}

bool verify_signature(uint8_t* header, uint16_t header_size, uint8_t* signature)
{
    uint32_t status;
    uint8_t sha256sum[ESEC_HASH_STATE_SIZE_SHA256];

    // GP_LOG_SYSTEM_PRINTF("Header:", 0);
    // gpLog_PrintBuffer(header_size, header);

    mbedtls_sha256_init(&payload_sha256_ctx);

    if(mbedtls_sha256_starts(&payload_sha256_ctx, 0) != 0)
    {
        return false;
    }

    if(mbedtls_sha256_update(&payload_sha256_ctx, header, header_size))
    {
        return false;
    }

    /* Finish up */
    if(mbedtls_sha256_finish(&payload_sha256_ctx, sha256sum) != 0)
    {
        return false;
    }

    // GP_LOG_SYSTEM_PRINTF("Hash OK digest:", 0);
    // gpLog_PrintBuffer(sizeof(sha256sum), sha256sum);

    struct esec_ecc_key ecc_key;

    /* public key is of type ECC_P256 and used only for signature verification. */
    ecc_key.keyspec = ESEC_KEY_ECC_P256 | ESEC_KEY_PUB | ESEC_KEY_PERMS_SIGN;
    /* key mode is unprotected */
    ecc_key.storage.cfg = KEY_MODE_UNPROTECT << KEY_MODE_OFFSET;
    ecc_key.storage.auth = NULL;
    /* Location of key */
    extern const appuc_fw_header_t appuc_fw_header_content;
    appuc_fw_header_t* fw_header = (appuc_fw_header_t*)&appuc_fw_header_content;
    ecc_key.storage.material = (UInt8*)&fw_header->certificate.content.pubkey[0];

    // GP_LOG_SYSTEM_PRINTF("Key:", 0);
    // gpLog_PrintBuffer(64, ecc_key.storage.material);
    // GP_LOG_SYSTEM_PRINTF("Signature:", 0);
    // gpLog_PrintBuffer(256, signature);

    status = esec_ecdsa_verify_digest(ecc_key, sha256sum, ESEC_HASH_STATE_SIZE_SHA256, (void*)signature);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Header signature verify fail: %lx", 0, status);
        return false;
    }

    return true;
}

qvCHIP_OtaStatus_t qvCHIP_OtaValidateandStoreHeader(const uint8_t* header, uint16_t data_size,
                                                    uint16_t* header_size_out, const uint8_t* public_key,
                                                    size_t public_key_size)
{
    uint16_t header_size;
    if(header == NULL || header_size_out == NULL)
    {
        return qvCHIP_OtaStatusInvalidParam;
    }
    uint8_t nbr_of_images = header[2];
    header_size = 4 + nbr_of_images * sizeof(qvCHIP_Ota_SubImageHeader_t);

    uint8_t* signature = (uint8_t*)&header[header_size]; // signature follows header, header_size excludes signature
    if(true != verify_signature((uint8_t*)header, header_size, signature))
    {
        return qvCHIP_OtaStatusInvalidImage;
    }
    GP_LOG_SYSTEM_PRINTF("Header signature verify succeeded !", 0);

    // Initialize structure to hold header information and upgrade progress, and store to NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader = {0};
    uint8_t offset = 4; // after header version and header size
    uint8_t imageIndex = 0;
    while((offset + sizeof(qvCHIP_Ota_SubImageHeader_t) <= data_size) && (imageIndex < nbr_of_images))
    {
        MEMCPY(&otaProgressHeader.imageHeaders[imageIndex], &header[offset], sizeof(qvCHIP_Ota_SubImageHeader_t));
        GP_LOG_PRINTF("qvCHIP_OtaValidateandStoreHeader: imageType: %d", 0,
                      (uint8_t)otaProgressHeader.imageHeaders[imageIndex].imageIdentifier);
        imageIndex += 1;
        offset += sizeof(qvCHIP_Ota_SubImageHeader_t);
    }
    // If we have fewer images than the maximum we can accommodate, set next image to invalid type
    if(imageIndex < QVCHIP_OTA_MAX_IMAGES)
    {
        otaProgressHeader.imageHeaders[imageIndex].imageIdentifier = qvCHIP_OtaImageTypeInvalid;
    }
    (*header_size_out) = header_size + PRIMARY_HEADER_SIGNATURE_SIZE;
    otaProgressHeader.currentImageIndex = 0;

    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);

    return qvCHIP_OtaStatusSuccess;
}

uint8_t qvCHIP_OtaGetCurrentImage(qvCHIP_Ota_SubImageHeader_t* imageHeader, qvCHIP_OtaImageDownloadState_t* imageState)
{
    qvCHIP_OtaProgressStorage_t otaProgressHeader;

    // Invalid index if provided buffers are null
    if((imageHeader == NULL) || (imageState == NULL))
    {
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
        gCurrentImageType = qvCHIP_OtaImageTypeInvalid;
#endif
        return 0xFF;
    }

    // Read header from NVM
    qvCHIP_OtaStatus_t res = OtaReadProgressInfo(&otaProgressHeader);
    if(res != qvCHIP_OtaStatusSuccess)
    {
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
        gCurrentImageType = qvCHIP_OtaImageTypeInvalid;
#endif
        return 0xFF;
    }
    // If block size is 0, this means the structure is not initialized
    if(otaProgressHeader.blockSize == 0)
    {
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
        gCurrentImageType = qvCHIP_OtaImageTypeInvalid;
#endif
        return 0xFF;
    }

    uint8_t index = otaProgressHeader.currentImageIndex;

    if((index >= QVCHIP_OTA_MAX_IMAGES) ||
       (otaProgressHeader.imageHeaders[index].imageIdentifier == qvCHIP_OtaImageTypeInvalid))
    {
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
        gCurrentImageType = qvCHIP_OtaImageTypeInvalid;
#endif
        return 0xFF;
    }

    MEMCPY(imageHeader, &otaProgressHeader.imageHeaders[index], sizeof(qvCHIP_Ota_SubImageHeader_t));
    *imageState = otaProgressHeader.imageState[index];
    GP_LOG_PRINTF("qvCHIP_OtaGetCurrentImage: %d id: %x", 0, index,
                  (uint8_t)otaProgressHeader.imageHeaders[index].imageIdentifier);
#ifdef GP_UPGRADE_DIVERSITY_BOOTLOADER_OTA_SECTION_OFFSET
    gCurrentImageType = otaProgressHeader.imageHeaders[index].imageIdentifier;
#endif

    return index;
}

void qvCHIP_OtaResetProgressInfo(void)
{
    qvCHIP_OtaProgressStorage_t otaProgressHeader = {0};
    GP_LOG_PRINTF("qvCHIP_OtaResetProgressInfo", 0);
    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);
}

bool qvCHIP_OtaImageDownloadInProgress(void)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);
    GP_LOG_PRINTF("qvCHIP_OtaImageDownloadInProgress: current image index is %d, block size is %lu", 0,
                  otaProgressHeader.currentImageIndex, otaProgressHeader.blockSize);

    if((0xFF != otaProgressHeader.currentImageIndex) && (otaProgressHeader.currentImageIndex < QVCHIP_OTA_MAX_IMAGES) &&
       (0 != otaProgressHeader.blockSize))
    {
        return true;
    }

    return false;
}

void qvCHIP_OtaSetImageState(qvCHIP_OtaImageDownloadState_t newState)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);

    uint8_t index = otaProgressHeader.currentImageIndex;
    otaProgressHeader.imageState[index] = newState;
    //    GP_LOG_PRINTF("qvCHIP_OtaSetImageState: %d", 0, newState);

    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);
}

void qvCHIP_OtaSetBlockSize(uint32_t blockSize)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);

    otaProgressHeader.blockSize = blockSize;
    //   GP_LOG_PRINTF("qvCHIP_OtaSetBlockSize: %lu", 0, blockSize);

    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);
}

uint32_t qvCHIP_OtaGetBlockSize(void)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);
    //    GP_LOG_PRINTF("qvCHIP_OtaGetBlockSize: %lu", 0, otaProgressHeader.blockSize);

    return otaProgressHeader.blockSize;
}

void qvCHIP_OtaSetLastProvider(uint64_t nodeId, uint16_t endpoint, uint8_t fabricIndex)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);

    otaProgressHeader.lastUsedProviderNodeID = nodeId;
    otaProgressHeader.lastUsedProviderEndpoint = endpoint;
    otaProgressHeader.lastUsedProviderFabricIndex = fabricIndex;

    GP_LOG_PRINTF("qvCHIP_OtaSetLastProvider: endpoint - %i, fabric - %u", 0,
                  otaProgressHeader.lastUsedProviderEndpoint, otaProgressHeader.lastUsedProviderFabricIndex);

    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);
}

bool qvCHIP_OtaGetLastProvider(uint64_t* nodeId, uint16_t* endpoint, uint8_t* fabricIndex)
{
    if((nodeId == NULL) || (endpoint == NULL) || (fabricIndex == NULL))
    {
        return false;
    }
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);
    *nodeId = otaProgressHeader.lastUsedProviderNodeID;
    *endpoint = otaProgressHeader.lastUsedProviderEndpoint;
    *fabricIndex = otaProgressHeader.lastUsedProviderFabricIndex;
    GP_LOG_PRINTF("qvCHIP_OtaGetLastProvider: endpoint - %i, fabric - %u", 0,
                  otaProgressHeader.lastUsedProviderEndpoint, otaProgressHeader.lastUsedProviderFabricIndex);

    return true;
}

uint8_t qvCHIP_OtaGoNextImage(void)
{
    // Read header from NVM
    qvCHIP_OtaProgressStorage_t otaProgressHeader;
    OtaReadProgressInfo(&otaProgressHeader);

    otaProgressHeader.currentImageIndex += 1;
    if(otaProgressHeader.currentImageIndex > QVCHIP_OTA_MAX_IMAGES ||
       otaProgressHeader.imageHeaders[otaProgressHeader.currentImageIndex].imageIdentifier ==
           qvCHIP_OtaImageTypeInvalid)
    {
        otaProgressHeader.currentImageIndex = 0xFF;
    }

    GP_LOG_PRINTF("qvCHIP_OtaGoNextImage: %d", 0, otaProgressHeader.currentImageIndex);
    // Write header to NVM
    OtaWriteProgressInfo(&otaProgressHeader);

    return (otaProgressHeader.currentImageIndex);
}
