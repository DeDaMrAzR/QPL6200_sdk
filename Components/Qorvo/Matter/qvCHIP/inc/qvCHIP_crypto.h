/*
 * Copyright (c) 2020, Qorvo Inc
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

/** @file "qvCHIP_crypto.h"
 *
 *  CHIP wrapper for crypto API
 *
 *  Declarations of the crypto specific public functions and enumerations of qvCHIP.
 */

#ifndef _QVCHIP_CRYPTO_H_
#define _QVCHIP_CRYPTO_H_

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    crypto API
 *****************************************************************************/
/**
 * @brief Checks if the DAC and private key are present in the info page.
 *
 * This function verifies whether the DAC Device Attestation Certificate) and
 * matching private key are present in the info page.
 *
 * @return true if the DAC and key are present in the info page, false otherwise.
 */
bool qvCHIP_Crypto_DacAndKeyInInfoPage(void);

/**
 * @brief Reads the device attestation certificate.
 *
 * This function reads the device attestation certificate and stores it in the provided buffer.
 *
 * @param[out] outBuffer     Pointer to the buffer where the certificate will be stored.
 * @param[in]  bufferSize    The size of the buffer.
 * @param[out] outDataLength Pointer to the variable where the length of the certificate
                             data will be stored.
 *
 * @return The status of the operation.
 */
qvStatus_t qvCHIP_Crypto_ReadDac(uint8_t* outBuffer, uint32_t bufferSize, uint32_t* outDataLength);

/** @brief Sign with private key associated with DAC
 *  @param msg             message to sign
 *  @param msg_size        size of the message
 *  @param signature_dst   pointer to signature destination data
 *  @return                QV_STATUS_INVALID_ARGUMENT if data is NULL,
 *                         QV_STATUS_INVALID_DATA if the factory data is missing,
 *                         QV_STATUS_NO_ERROR otherwise
 */
qvStatus_t qvCHIP_Crypto_SignWithDacPk(uint8_t* msg, uint32_t msg_size, uint8_t* signature_dst);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* _QVCHIP_CRYPTO_H_ */
