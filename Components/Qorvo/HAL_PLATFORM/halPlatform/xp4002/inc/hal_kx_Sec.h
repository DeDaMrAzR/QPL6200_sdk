/*
 * Copyright (c) 2023, Qorvo Inc
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

#ifndef _HAL_KX_SEC_H_
#define _HAL_KX_SEC_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/** @brief Perform AES encryption in SSP engine.
 *
 *  @param dataPtr   Pointer to 16-byte data buffer for input and output, non-compressed RAM address.
 *  @param keyPtr    Pointer to key buffer, compressed RAM address.
 *  @param keylen    Key length.
 *  @param hardened  True to use internal secret key.
 */
hal_Result_t gpHalSec_SspAesEncrypt(UInt32 dataPtr, UInt32 keyPtr, UInt8 keylen, Bool hardened);

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
                                      UIntPtr windowStartPtr, UInt16 windowLen);

#ifdef __cplusplus
}
#endif

#endif // _HAL_KX_SEC_H_
