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

#include "gpAssert.h"
#include "gpHal.h"
#include "gpUtils.h"
#include "gpUpgrade.h"
#include "gpUpgrade_flash.h"
#include "gpUpgrade_hash.h"
#include "gpUpgrade_defs.h"
#include "gpLog.h"

#include "esec_errors.h"
#include "esec_qorvo.h"
#include "esecure.h"

#include "hal_user_license.h"

#include "q_firmware_header.h"
#include "eSecure_metadata.h"

#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
#include "lzma.h"
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

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static const gpUpgrade_Metadata_t* bootloaderMetadata;
/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Extern Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void hal__AtomicOn(void)
{
}

void hal__AtomicOff(void)
{
}

static void Upgrade_PopulateEsecEccKeyForUnprotectedKey(struct esec_ecc_key* ecc_key, UInt8* ecc_pub_key)
{
    /* public key is of type ECC_P256 and used only for signature verification. */
    ecc_key->keyspec = ESEC_KEY_ECC_P256 | ESEC_KEY_PUB | ESEC_KEY_PERMS_SIGN;
    /* key mode is unprotected */
    ecc_key->storage.cfg = KEY_MODE_UNPROTECT << KEY_MODE_OFFSET;
    ecc_key->storage.auth = NULL;
    /* Location of key */
    ecc_key->storage.material = ecc_pub_key;
}

static UInt32 Upgrade_fwHashCalculate(UInt8* digest, const volatile appuc_fw_header_t* fwHeader)
{
    UInt8 state[ESEC_HASH_STATE_SIZE_SHA256] = {0};
    UInt32 status;
    const UInt32 appHeaderSize =
        sizeof(appuc_fw_auth_header_t) + sizeof(appuc_fw_version_info_t) + sizeof(fwHeader->reserved);

    status = esec_hash_update(ESEC_HASH_ALGO_SHA256, esec_sha256_initial_state, (uint8_t*)&(fwHeader->auth),
                              appHeaderSize, state);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Hash update failed, err: %lu", 0, status);
        return status;
    }

    status = esec_hash_finish(ESEC_HASH_ALGO_SHA256, state, (uint8_t*)(fwHeader) + sizeof(appuc_fw_header_t),
                              fwHeader->auth.fw_size, appHeaderSize + fwHeader->auth.fw_size, digest);

    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Hash finish failed, err: %lu", 0, status);
    }

    return status;
}

static const appuc_fw_header_t* Upgrade_GetOtaAreaHeader(void)
{
    const appuc_fw_header_t* otaHeader = NULL;

    if(!bootloaderMetadata)
    {
        GP_LOG_SYSTEM_PRINTF("Fatal error. Bootloader metadata is NULL", 0);
        GP_ASSERT_SYSTEM(false);
        return otaHeader;
    }

    /** OTA is not set */
    if(bootloaderMetadata->otaAddress == 0)
    {
        return otaHeader;
    }

    otaHeader = (const appuc_fw_header_t*)bootloaderMetadata->otaAddress;

    return otaHeader;
}

static UInt32* Upgrade_GetImageDestinationAddress(const appuc_fw_header_t* fw_header)
{
    return (UInt32*)(fw_header->auth.image_destination_address);
}

static gpUpgrade_Status_t Upgrade_ValidateCertificate(const appuc_fw_header_t* fwHeader)
{
    UInt32 status;

    /* validate certificate */
    status = esec_validate_certificate((certificate_t*)&fwHeader->certificate, AUTH_MISC_HOST_SIGN, CR_KEY_SM_FW);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Invalid certificate, err: %lu", 0, status);
        return gpUpgrade_StatusFailedVerify;
    }

    return gpUpgrade_StatusSuccess;
}

static gpUpgrade_Status_t Upgrade_SignatureValidate(const appuc_fw_header_t* fwHeader)
{
    UInt32 status;
    /* Compute SHA256 digest */
    UInt8 digest[ESEC_HASH_STATE_SIZE_SHA256];

    status = Upgrade_fwHashCalculate(digest, fwHeader);

    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Upgrade_fwHashCalculate() failed, err: %lu", 0, status);
        return gpUpgrade_StatusFailedVerify;
    }

    /* Verify signature */
    struct esec_ecc_key ecc_pub_key;
    UInt8* pubkey = (UInt8*)&fwHeader->certificate.content.pubkey[0];

    Upgrade_PopulateEsecEccKeyForUnprotectedKey(&ecc_pub_key, pubkey);

    status = esec_ecdsa_verify_digest(ecc_pub_key, digest, sizeof(digest), (const UInt8*)&fwHeader->signature[0]);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Invalid signature, err: %lu", 0, status);
        return gpUpgrade_StatusFailedVerify;
    }

    return gpUpgrade_StatusSuccess;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

gpUpgrade_Status_t gpUpgrade_Init(const gpUpgrade_Metadata_t* metadata)
{
    static Bool initialized;

    if(initialized)
    {
        return gpUpgrade_StatusFailedProgramError;
    }

    if(!metadata)
    {
        return gpUpgrade_StatusInvalidArgument;
    }

    bootloaderMetadata = metadata;

    return gpUpgrade_StatusSuccess;
}

const appuc_fw_header_t* gpUpgrade_GetActiveBootHeader(void)
{
    UInt32 activeBootAddr = 0;
    const appuc_fw_header_t* activeBootHeader = NULL;

    if(!bootloaderMetadata)
    {
        GP_LOG_SYSTEM_PRINTF("Fatal error. Bootloader metadata is NULL", 0);
        GP_ASSERT_SYSTEM(false);

        return NULL;
    }

    /* Mask 3 most significant bits, that are used by the customer for the test mode. APPSCS-4452, SDP013-1567. */
    activeBootAddr = bootloaderMetadata->bootAddress & USER_LICENSE_BOOT_ADDRESS_BITMASK;
    activeBootHeader = (const appuc_fw_header_t*)activeBootAddr;

    return activeBootHeader;
}

gpUpgrade_Status_t gpUpgrade_StoreWakeUpAddress(const appuc_fw_header_t* fwHeader)
{
    gpHal_FlashError_t status;

    if(!fwHeader)
    {
        return gpUpgrade_StatusInvalidArgument;
    }

    status = gpHal_FlashProgramSector((FlashPtr)&bootloaderMetadata->wakeUpAddress, sizeof(UIntPtr), (UInt8*)&fwHeader);
    if(status != gpHal_FlashError_Success)
    {
        GP_LOG_SYSTEM_PRINTF("Failed to store wake up address, err: %lu", 0, status);
        return gpUpgrade_StatusWriteError;
    }

    return gpUpgrade_StatusSuccess;
}

Bool gpUpgrade_IsSecureBootEnabled(void)
{
    UInt32 qorvo_config;
    // check secure boot enable
    esec_get_sm_qorvo_config(&qorvo_config);

    return ((qorvo_config & OTP_QORVO_SM_APPUC_SECURE_BOOT_ENABLE_MASK) == OTP_QORVO_SM_APPUC_SECURE_BOOT_ENABLE_MASK);
}

Bool gpUpgrade_ApplicationValidate(const appuc_fw_header_t* fwHeader)
{
    UInt32 status;

    status = Upgrade_SignatureValidate(fwHeader);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Invalid signature, err: %lu", 0, status);
        return false;
    }

    if(gpUpgrade_IsSecureBootEnabled())
    {
        status = Upgrade_ValidateCertificate(fwHeader);
        if(status != ESEC_OKAY)
        {
            GP_LOG_SYSTEM_PRINTF("Invalid certificate, err: %lu", 0, status);
        }
    }

    return (status == ESEC_OKAY);
}

/** @brief This function allows a bootloader implementation to install a pending image. Intended for bootloader
implementation.
- Check image integrity
*  - Clear application region for the new image
*  - Install the new application with retries
*   @return status
*/
gpUpgrade_Status_t gpUpgrade_InstallPendingImage(void)
{
    const appuc_fw_header_t* otaHeader = (const appuc_fw_header_t*)Upgrade_GetOtaAreaHeader();

    if(!otaHeader)
    {
        GP_ASSERT_SYSTEM(false);
        return gpUpgrade_StatusFailedProgramError;
    }

    UInt8* imageStartAddr = (UInt8*)otaHeader + sizeof(appuc_fw_header_t);
    UInt8* imageDstAddr = (UInt8*)Upgrade_GetImageDestinationAddress(otaHeader);
    UInt32 imageSize = otaHeader->auth.fw_size;
    UInt8 retries = GP_UPGRADE_UPGRADE_MAX_RETRIES;

    if(gpUpgrade_IsSecureBootEnabled())
    {
        GP_LOG_SYSTEM_PRINTF("Secure OTA Enabled", 0);
        if(Upgrade_ValidateCertificate(otaHeader) != ESEC_OKAY)
        {
            return gpUpgrade_StatusFailedVerify;
        }
    }

    // Verify signature of OTA image using the uncompress header
    if(ESEC_OKAY == Upgrade_SignatureValidate(otaHeader))
    {
        // Uncompress LZMA application image to proper location
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
        // Validate the lzma image header
        lzma_result result = lzma_IsValidInput(imageStartAddr, imageSize);
        if(result != lzma_ResultSuccess)
        {
            return gpUpgrade_StatusPreCheckFailed;
        }

        result = lzma_IsValidOutput(imageDstAddr);
        // Validate the output buffer is aligned
        if(result != lzma_ResultSuccess)
        {
            return gpUpgrade_StatusPreCheckFailed;
        }
#endif
    }
    else
    {
        return gpUpgrade_StatusFailedVerify;
    }
    /* Install application with retries*/
    while(retries--)
    {
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
        lzma_result result = lzma_Decode(imageStartAddr, imageSize, imageDstAddr);
        if(result != lzma_ResultSuccess)
        {
            GP_LOG_SYSTEM_PRINTF("LZMA decode error", 0);
        }
        else
        {
#else //! GP_UPGRADE_DIVERSITY_COMPRESSION
        if(gpUpgrade_FlashInstallImage(imageDstAddr, (UInt32)imageStartAddr, imageSize) == gpUpgrade_StatusSuccess)
        {
#endif
            GP_LOG_SYSTEM_PRINTF("Upgrade successful", 0);
            return gpUpgrade_StatusSuccess;
        }
    }
    /* Returning gpUpgrade_StatusLoadImageFailed means that the failure occurs during decompression (Non-recoverable state)*/
    GP_LOG_SYSTEM_PRINTF("Upgrade failed during decompression (Non-recoverable state)", 0);
    return gpUpgrade_StatusLoadImageFailed;
}

Bool gpUpgrade_IsImagePending(void)
{
    const appuc_fw_header_t* otaHeader = Upgrade_GetOtaAreaHeader();

    if(otaHeader == NULL)
    {
        GP_LOG_SYSTEM_PRINTF("OTA header pointer is NULL.", 0);
        return false;
    }

    /* Extract image destination address from OTA FW header. */
    const appuc_fw_header_t* appuc_fw_header_pt = (appuc_fw_header_t*)Upgrade_GetImageDestinationAddress(otaHeader);
    UInt32 certificateMagicNumber = otaHeader->certificate.content.magic_number;

    if(!appuc_fw_header_pt)
    {
        GP_LOG_SYSTEM_PRINTF("OTA destination address is NULL. Aborting.", 0);
        return false;
    }

    if(GP_UPGRADE_CERTIFICATE_MAGIC_NUMBER == certificateMagicNumber)
    {
        GP_LOG_SYSTEM_PRINTF("Certificate magic number found for pending image.", 0);

        UInt32 otaAppVersion = BIG_TO_HOST_UINT32(&(otaHeader->appuc_fw_version_info.app_version));
        UInt32 currentAppVersion = BIG_TO_HOST_UINT32(&(appuc_fw_header_pt->appuc_fw_version_info.app_version));

        GP_LOG_SYSTEM_PRINTF("Current app version: %lu.", 0, currentAppVersion);
        GP_LOG_SYSTEM_PRINTF("OTA app version: %lu.", 0, otaAppVersion);

        if(otaAppVersion > currentAppVersion)
        {
            GP_LOG_SYSTEM_PRINTF("App version of pending image is higher.", 0);
            return true;
        }
    }
    return false;
}
