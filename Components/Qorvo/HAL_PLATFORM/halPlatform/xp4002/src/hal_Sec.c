/*
 * Copyright (c) 2025, Qorvo Inc
 *
 * hal_Sec.c
 *
 * Contains all security functionality of the HAL
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
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "hal.h"
#include "gpHal.h"
#include "gpAssert.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_HALPLATFORM

#define GP_HAL_KEY_128   16 // bytes
#define GP_HAL_DATA_LEN  16 // bytes
#define GP_HAL_NONCE_104 13 // bytes

#define Hal_AESKeyLen128 (128 >> 3)
#define Hal_AESKeyLen192 (192 >> 3)
#define Hal_AESKeyLen256 (256 >> 3)
#define Hal_AESKeyLenInv 0xFF

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    UInt8 key[GP_HAL_KEY_128];
    UInt8 nonce[16];
} gpHal_CCMBuffer_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Key buffer must be 32-byte aligned.
COMPILER_ALIGNED(32) static gpHal_CCMBuffer_t gpHal_SecRamBuffer LINKER_SECTION(".lower_ram_retain");

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_HAL_SEC_DUMP_SSP_STATE
static void gpHalSec_Dump(void)
{
    GP_LOG_SYSTEM_PRINTF("mode:%i", 0, GP_WB_READ_SSP_MODE());
    GP_LOG_SYSTEM_PRINTF("msg:%lx - %i", 0, GP_WB_READ_SSP_MSG_PTR(), GP_WB_READ_SSP_MSG_LEN());
    GP_LOG_SYSTEM_PRINTF("key:%lx - %i", 0, GP_WB_READ_SSP_KEY_PTR(), GP_WB_READ_SSP_KEY_LEN());
    gpLog_Flush();
    GP_LOG_SYSTEM_PRINTF("mic:%lx - %i", 0, GP_WB_READ_SSP_MIC_PTR(), GP_WB_READ_SSP_MIC_LEN());
    GP_LOG_SYSTEM_PRINTF("aux:%lx - %i", 0, GP_WB_READ_SSP_A_PTR(), GP_WB_READ_SSP_A_LEN());
    GP_LOG_SYSTEM_PRINTF("non:%lx - %i", 0, GP_WB_READ_SSP_NONCE_PTR(), GP_WB_READ_SSP_MIC_LEN());
    gpLog_Flush();
}

#else
#define gpHalSec_Dump()
#endif // GP_DIVERSITY_HAL_SEC_DUMP_SSP_STATE

/** @brief Perform AES encryption in SSP engine.
 *
 *  @param dataPtr   Pointer to 16-byte data buffer for input and output, non-compressed RAM address.
 *  @param keyPtr    Pointer to key buffer, compressed RAM address.
 *  @param keylen    Key length.
 *  @param hardened  True to use internal secret key.
 */
hal_Result_t gpHalSec_SspAesEncrypt(UInt32 dataPtr, UInt32 keyPtr, UInt8 keylen, Bool hardened)
{
    // Set Data input and output Ptr
    GP_WB_WRITE_SSP_MSG_PTR(dataPtr);
    GP_WB_WRITE_SSP_MSG_OUT_PTR(dataPtr);

    // Set encryption key
    GP_WB_WRITE_SSP_KEY_PTR(keyPtr);

    // Harden with secret key
    if(hardened)
    {
        GP_WB_WRITE_SSP_USE_SKEY(1);
    }
    else
    {
        GP_WB_WRITE_SSP_USE_SKEY(0);
    }

    // Set the length
    GP_WB_WRITE_SSP_KEY_LEN((keylen == Hal_AESKeyLen128)   ? GP_WB_ENUM_SSP_KEY_LEN_KEY_128
                            : (keylen == Hal_AESKeyLen256) ? GP_WB_ENUM_SSP_KEY_LEN_KEY_256
                                                           : GP_WB_ENUM_SSP_KEY_LEN_KEY_192);
    GP_WB_WRITE_SSP_MSG_LEN(GP_HAL_DATA_LEN);

    // Set the mode
    GP_WB_WRITE_SSP_MODE(GP_WB_ENUM_SSP_MODE_AES);

    // Start encryption and poll for finish
    GP_WB_SSP_START_ENCRYPT();
    __DSB();

    // On high clock speeds, it is still possible we read out the status before the hardware was able to update.
    // Add an additional wait
    HAL_WAIT_US(1);

    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_SSP_BUSY(), GP_HAL_DEFAULT_TIMEOUT);

    // Assert when memory error occured
    GP_ASSERT_DEV_EXT(0 == GP_WB_READ_SSP_MEM_ERR());

    return gpHal_ResultSuccess;
}

/** @brief Perform CCM encryption/decryption in SSP engine.
 *
 *  During encryption, MIC will be written immediately after encrypted data.
 *  During decryption, MIC check bytes will be written immediately after decrypted data.
 *  Caller must check that these are all-zero bytes.
 *
 *  @param pKey            Pointer to 16-byte key.
 *  @param pNonce          Pointer to 13-byte nonce.
 *  @param ptrBuf          Pointer to buffer for input and output (non-compressed RAM address).
 *  @param ptrBufAux       Pointer to buffer for additional authentication data (non-compressed RAM address).
 *  @param dataLen         Data length in bytes.
 *  @param micLen          MIC length in bytes.
 *  @param auxLen          Additional authentication data length in bytes.
 *  @param decrypt         True to decrypt, false to encrypt.
 *  @param windowStartPtr  The starting location of the cyclical window, in case a wrappable buffer is used.
 *  @param windowLen       The size of the cyclical window, in case a wrappable buffer is used.
 */
hal_Result_t gpHalSec_SspCcmOperation(const UInt8* pKey, const UInt8* pNonce, UIntPtr ptrBuf, UIntPtr ptrBufAux,
                                      UInt8 dataLen, UInt8 micLen, UInt8 auxLen, Bool modeDecrypt,
                                      UIntPtr windowStartPtr, UInt16 windowLen)
{
    GP_WB_WRITE_SSP_MSG_PTR(ptrBuf);
    GP_WB_WRITE_SSP_MSG_OUT_PTR(ptrBuf);
    GP_WB_WRITE_SSP_MSG_LEN(dataLen);

    GP_WB_WRITE_SSP_CIRC_WD_START_PTR(windowStartPtr);
    GP_WB_WRITE_SSP_CIRC_WD_LEN(windowLen);

    // Write micPtr and length, mic will be placed after data
    GP_WB_WRITE_SSP_MIC_PTR(ptrBuf + dataLen);
    GP_WB_WRITE_SSP_MIC_LEN(micLen >> 1); // MIC length divide by 2!

    // Set the auxPtr and length, if needed linearize aux data
    // aux is only input for security engine so no need to delinearize and restore data later on
    GP_WB_WRITE_SSP_A_PTR(ptrBufAux);
    GP_WB_WRITE_SSP_A_LEN(auxLen);

    // Write key + keyPtr
    // Write Nonce + noncePtr
    MEMCPY(gpHal_SecRamBuffer.key, pKey, GP_HAL_KEY_128);
    MEMCPY(gpHal_SecRamBuffer.nonce, pNonce, GP_HAL_NONCE_104);
    GP_WB_WRITE_SSP_KEY_PTR((UIntPtr)gpHal_SecRamBuffer.key);
    GP_WB_WRITE_SSP_NONCE_PTR((UIntPtr)gpHal_SecRamBuffer.nonce);

    GP_WB_WRITE_SSP_KEY_LEN(GP_WB_ENUM_SSP_KEY_LEN_KEY_128);

    // Set the mode & start encryption/decryption
    GP_WB_WRITE_SSP_USE_SKEY(0);
    GP_WB_WRITE_SSP_MODE(modeDecrypt ? GP_WB_ENUM_SSP_MODE_DECRYPT : GP_WB_ENUM_SSP_MODE_ENCRYPT);
#ifdef GP_DIVERSITY_HAL_SEC_DUMP_SSP_STATE
    gpHalSec_Dump();
#endif // FIXME
    GP_WB_SSP_START_ENCRYPT();
    __DSB();

    // On high clock speeds, it is still possible we read out the status before the hardware was able to update.
    // Add an additional wait
    HAL_WAIT_US(1);

    // Polled wait for encryption to finish & assert when memory error occured
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_SSP_BUSY(), GP_HAL_DEFAULT_TIMEOUT);
    GP_ASSERT_DEV_EXT(0 == GP_WB_READ_SSP_MEM_ERR());

    // restore defaults
    GP_WB_WRITE_SSP_CIRC_WD_START_PTR(0);
    GP_WB_WRITE_SSP_CIRC_WD_LEN(0);

    return gpHal_ResultSuccess;
}

/*****************************************************************************
 *                    Public functions
 *****************************************************************************/
