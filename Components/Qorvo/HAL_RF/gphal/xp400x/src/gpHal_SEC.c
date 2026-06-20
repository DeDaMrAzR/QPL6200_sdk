/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpHal_SEC.c
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

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_SEC.h"
#include "gpHal_Pbm.h"
#include "hal_kx_Sec.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

#include "gpAssert.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#define GP_HAL_KEY_128      16 // bytes
#define GP_HAL_DATA_LEN      16 // bytes
#define GP_HAL_KEY_256      32 // bytes
#define GP_HAL_MAXKEY_LEN   GP_HAL_KEY_256
#define GP_HAL_NONCE_104    13 // bytes

#define GP_HAL_SEC_USER_KEY_ID_TO_LOCATION(id)      (GP_WB_NVR_USER_KEY_0_LSB_ADDRESS    + (id                                )*(GP_WB_NVR_USER_KEY_0_LSB_LEN   +GP_WB_NVR_USER_KEY_0_MSB_LEN)   )
#define GP_HAL_SEC_PRODUCT_KEY_ID_TO_LOCATION(id)   (GP_WB_NVR_PRODUCT_KEY_0_LSB_ADDRESS + (id - gpEncryption_KeyIdProductKey0)*(GP_WB_NVR_PRODUCT_KEY_0_LSB_LEN+GP_WB_NVR_PRODUCT_KEY_0_MSB_LEN))

#define GP_HAL_SEC_PUF_AES_SECRET_KEY_1_RAM_ADDR (GP_MM_RAM_PKEY_START + 0x60)
#define GP_HAL_SEC_PUF_AES_SECRET_KEY_2_RAM_ADDR (GP_MM_RAM_PKEY_START + 0x70)

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct {
    UInt8 key[GP_HAL_MAXKEY_LEN];
    UInt8 data[GP_HAL_DATA_LEN];
} gpHal_AESBuffer_t;

typedef union {
    gpHal_AESBuffer_t aes;
} gpHal_SecurityRamBuffer_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Key buffer must be 32-byte aligned.
COMPILER_ALIGNED(32) static gpHal_SecurityRamBuffer_t gpHal_SecRamBuffer LINKER_SECTION(".lower_ram_retain");

HAL_CRITICAL_SECTION_DEF(aes_encrypt_lock);

/** Perform CCM encryption or decryption, depending on mode parameter. */
static gpHal_Result_t gpHalSec_CCMbase(gpEncryption_CCMOptions_t* pCCMOptions, Bool modeDecrypt)
{
    UInt8 PBMhandle;
    UInt8 dataWithMicLength;
    gpHal_Address_t tempPtrBuf;
    gpHal_Address_t tempPtrBufAux;
    UInt8 checkHandle;
    Bool wrappeablePbm;
    gpHal_Result_t result;

    UInt32 windowStartPtr = 0;
    UInt16 windowLen = 0;

    GP_ASSERT_DEV_EXT(NULL != pCCMOptions);
    GP_ASSERT_DEV_EXT(NULL != pCCMOptions->pKey);
    GP_ASSERT_DEV_EXT(NULL != pCCMOptions->pNonce);


    wrappeablePbm = (gpPd_GetPdType(pCCMOptions->pdHandle) == gpPd_BufferTypeZigBee)? true: false;



    if(wrappeablePbm)
    {
        if ((pCCMOptions->dataLength + pCCMOptions->auxLength + pCCMOptions->micLength) > 0x80)
        {
            // return Invalid Parameter(payload size excess limit) to upper layer
            return gpHal_ResultInvalidParameter;
        }
        pCCMOptions->auxOffset &= 0x7f;
        pCCMOptions->dataOffset &= 0x7f;
    }

    dataWithMicLength = pCCMOptions->dataLength + pCCMOptions->micLength;

    //Copy data and aux from Pd to Pbm in case of pdram
    PBMhandle = gpPd_SecRequest(pCCMOptions->pdHandle,
                                pCCMOptions->dataOffset,
                                //For decryption we also need to copy mic from pd to pbm !
                                (modeDecrypt) ? dataWithMicLength : pCCMOptions->dataLength,
                                pCCMOptions->auxOffset,
                                pCCMOptions->auxLength);
    if (!GP_HAL_CHECK_PBM_VALID(PBMhandle))
    {
        return gpHal_ResultBusy;
    }

    if (wrappeablePbm)
    {
        UInt32 pbmStartAddress = GP_HAL_PBM_ENTRY2ADDR(PBMhandle);
        // Wrappable pbm, use base address => config SSP circ window
        windowStartPtr = pbmStartAddress;
        windowLen = GP_PD_BUFFER_SIZE_ZIGBEE;
        /* change the pointer such that the SSP passes through the CIRC_WD */
        tempPtrBuf = GP_MM_SSP_REGMAP_CIRC_WD_START + pCCMOptions->dataOffset;
        tempPtrBufAux = GP_MM_SSP_REGMAP_CIRC_WD_START + pCCMOptions->auxOffset;
    }
    else
    {
        UInt32 pbmStartAddress = GP_HAL_PBM_ENTRY2ADDR(PBMhandle);
        // Non-wrappable pbm, use base address
        tempPtrBuf = pbmStartAddress + pCCMOptions->dataOffset;
        tempPtrBufAux = pbmStartAddress + pCCMOptions->auxOffset;
    }

    result = gpHalSec_SspCcmOperation(
        pCCMOptions->pKey,
        pCCMOptions->pNonce,
        tempPtrBuf,
        tempPtrBufAux,
        pCCMOptions->dataLength,
        pCCMOptions->micLength,
        pCCMOptions->auxLength,
        modeDecrypt,
        windowStartPtr,
        windowLen
    );

    //returned pdHandle should be te same, copy mic aswell for decryption !
    checkHandle = gpPd_cbSecConfirm(PBMhandle, pCCMOptions->dataOffset, dataWithMicLength);
    GP_ASSERT_DEV_EXT(checkHandle == pCCMOptions->pdHandle);

    if (result != gpHal_ResultSuccess)
    {
        return result;
    }

    //If decryption was succesfull, check MIC (should be all zero)
    if (modeDecrypt)
    {
        UInt8 micData[16] = {0}; //Maximum MIC length
        UIntLoop i;
        //Mic will be after data, check if all 0 after decryption
        gpPd_ReadByteStream(pCCMOptions->pdHandle, pCCMOptions->dataOffset + pCCMOptions->dataLength, pCCMOptions->micLength, micData);

        for (i = 0; i < pCCMOptions->micLength; ++i)
        {
            if (0 != micData[i])
            {
                //Decryption failed
                return gpHal_ResultInvalidParameter;
            }
        }
    }

    return gpHal_ResultSuccess;
}

/*****************************************************************************
 *                    Public functions
 *****************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  SYNCHRONOUS SECURITY FUNCTIONS
//-------------------------------------------------------------------------------------------------------

void gpHal_SEC_Init(void)
{
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_SecRamBuffer.aes.key);
    GP_HAL_REGISTER_AKRAM_REGION(gpHal_SecRamBuffer.aes.data);
    HAL_CREATE_MUTEX(&aes_encrypt_lock);
}

gpHal_Result_t gpHal_AESEncrypt(UInt8* pInplaceBuffer, UInt8* pAESKey, gpEncryption_AESOptions_t AESOptions)
{
    UInt8* pPatchedAesKey;
    gpEncryption_KeyId_t keyId;
    Bool hardened;
    //24-bit address range
    UInt32 dataPtr;
    UInt32 keyPtr;
    UInt32 compressedKeyPtr;
    gpHal_Result_t result;

    HAL_ACQUIRE_MUTEX(aes_encrypt_lock);

    if(AESOptions.keylen != gpEncryption_AESKeyLen128 &&
        AESOptions.keylen != gpEncryption_AESKeyLen256 &&
        AESOptions.keylen != gpEncryption_AESKeyLen192)
    {
        result = gpHal_ResultInvalidParameter;
        goto exit;
    }

    keyId = GP_ENCRYPTION_OPTIONS_GET_KEYID(AESOptions.options);
    hardened = GP_ENCRYPTION_OPTIONS_IS_HARDENED(AESOptions.options);

    MEMCPY(gpHal_SecRamBuffer.aes.data, pInplaceBuffer, GP_HAL_DATA_LEN);

    dataPtr = GP_HAL_PHYSICAL_TO_ARM(gpHal_SecRamBuffer.aes.data);


    GP_ASSERT_DEV_EXT(GP_ENCRYPTION_KEYID_IS_KEYPTR(keyId));

    //resolve potential NULL key pointer
    if(NULL == pAESKey)
    {
        MEMSET(gpHal_SecRamBuffer.aes.key, 0, AESOptions.keylen);
    }
    else
    {
        MEMCPY(gpHal_SecRamBuffer.aes.key, pAESKey, AESOptions.keylen);
    }
    pPatchedAesKey = gpHal_SecRamBuffer.aes.key;

    keyPtr = GP_HAL_PHYSICAL_TO_ARM(pPatchedAesKey);

    compressedKeyPtr = keyPtr;

    result = gpHalSec_SspAesEncrypt(dataPtr, compressedKeyPtr, AESOptions.keylen, hardened);
    if (result != gpHal_ResultSuccess)
        goto exit;

    // Copy data back to the in place buffer
    MEMCPY(pInplaceBuffer, (UInt8*)GP_HAL_ARM_TO_PHYSICAL(dataPtr), GP_HAL_DATA_LEN);
    result = gpHal_ResultSuccess;
exit:
    HAL_RELEASE_MUTEX(aes_encrypt_lock);
    return result;
}

gpHal_Result_t gpHal_CCMEncrypt(gpEncryption_CCMOptions_t* pCCMOptions)
{
    //Call CCM base function
    return gpHalSec_CCMbase(pCCMOptions, false);
}
gpHal_Result_t gpHal_CCMEncrypt_RAM(UInt16 dataLength, UInt16 auxLength, UInt8 micLength, UInt8* dataPtr, UInt8* auxPtr, UInt8* micPtr, UInt8* pKey, UInt8* pNonce, UInt8* dataOutPtr)
{
    gpHal_Result_t result = gpHal_ResultInvalidRequest;

    gpEncryption_CCMOptions_t CCMOptions;
    gpPd_Handle_t pdHandle;

    if (dataLength + micLength + auxLength > GP_PD_BUFFER_SIZE_ZIGBEE)
    {
        return result;
    }

    pdHandle = gpPd_GetPd();
    if(gpPd_ResultValidHandle != gpPd_CheckPdValid(pdHandle))
    {
        return gpHal_ResultInvalidRequest;
    }

    CCMOptions.dataLength = dataLength;
    CCMOptions.auxLength = auxLength;
    CCMOptions.micLength = micLength;
    CCMOptions.pKey = pKey;
    CCMOptions.pNonce = pNonce;
    CCMOptions.pdHandle = pdHandle;
    CCMOptions.dataOffset = 0;
    CCMOptions.auxOffset = dataLength+micLength;
    gpPd_WriteByteStream(pdHandle, 0, dataLength + micLength, dataPtr);
    gpPd_WriteByteStream(pdHandle, dataLength+micLength, auxLength, auxPtr);

    //Call CCM base function
    result = gpHalSec_CCMbase(&CCMOptions,GP_WB_ENUM_SSP_MODE_ENCRYPT);
    gpPd_ReadByteStream(pdHandle, 0, dataLength + micLength, dataOutPtr);

    gpPd_FreePd(pdHandle);

    return result;
}


gpHal_Result_t gpHal_CCMDecrypt_RAM(UInt16 dataLength, UInt16 auxLength, UInt8 micLength, UInt8* dataPtr, UInt8* auxPtr, UInt8* micPtr, UInt8* pKey, UInt8* pNonce, UInt8* dataOutPtr)
{
    gpHal_Result_t result = gpHal_ResultInvalidRequest;

    gpEncryption_CCMOptions_t CCMOptions;
    gpPd_Handle_t pdHandle;

    if (dataLength + micLength + auxLength > GP_PD_BUFFER_SIZE_ZIGBEE)
    {
        return result;
    }
    pdHandle = gpPd_GetPd();
    if(gpPd_ResultValidHandle != gpPd_CheckPdValid(pdHandle))
    {
        return gpHal_ResultInvalidRequest;
    }

    CCMOptions.pdHandle = pdHandle;

    CCMOptions.dataLength = dataLength;
    CCMOptions.auxLength = auxLength;
    CCMOptions.micLength = micLength;
    CCMOptions.pKey = pKey;
    CCMOptions.pNonce = pNonce;
    CCMOptions.dataOffset = 0;
    CCMOptions.auxOffset = dataLength+micLength;

    if (dataLength)
    {
        gpPd_WriteByteStream(pdHandle, 0, dataLength, dataPtr);
    }
    gpPd_WriteByteStream(pdHandle, dataLength, micLength, micPtr);
    gpPd_WriteByteStream(pdHandle, dataLength+micLength, auxLength, auxPtr);

    //Call CCM base function
    result = gpHalSec_CCMbase(&CCMOptions,GP_WB_ENUM_SSP_MODE_DECRYPT);
    if (dataLength > 0)
    {
        gpPd_ReadByteStream(pdHandle, 0, dataLength, dataOutPtr);
    }

    gpPd_FreePd(pdHandle);
    return result;
}


gpHal_Result_t gpHal_CCMDecrypt(gpEncryption_CCMOptions_t* pCCMOptions)
{
    //Call CCM base function
    return gpHalSec_CCMbase(pCCMOptions, true);
}
