/*
 * Copyright (c) 2021-2022, Qorvo Inc
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

/** @file "qvCHIP_crypto.c"
 *
 *  CHIP wrapper for crypto API
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
/* <CodeGenerator Placeholder> Includes */

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVCHIP

#include "qvCHIP.h"
#include "gpHal.h"
#include "gpNvm.h"

#include "gpLog.h"

#include "esecure.h"

#include "esec_ecc.h"
#include "gpHal_kx_NVM.h"
#include "esec_keys_basics.h"
#include "eSecure_metadata.h"

#include "mbedtls/x509_crt.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* These locations are decided during provisioning - refer generate_sm_bundle_for_system_test.sh */
#define DAC_LOCATION 0x10200800
// note: obtained with wc -c < tmp/out/dac_pai_signed.der
#define DAC_MAX_SIZE 0x400
#define PK_LOCATION  0x10200C00

/*****************************************************************************
 *                    Static Component Function Implementations
 *****************************************************************************/

static void map_keymetadata_to_esec_ecc_key(struct esec_ecc_key* key, uint32_t key_metadata)
{
    key->keyspec = key_metadata & ~(KEY_MODE_MASK << KEY_MODE_OFFSET);
    key->storage.cfg = key_metadata & (KEY_MODE_MASK << KEY_MODE_OFFSET);
}

/**
 * @brief Populate esec_ecc_key struct given the location where
 * wrapped key is provisioned in NVM. Provisioned key contains
 * key metadata (4 bytes) + key data (60 bytes). Keydata described in Section 3.2.4,
 * BA470_eSecure_Commands.pdf
 *
 * @param ecc_key                           esec_ecc_key struct to be populated
 * @param provisioned_wrap_key_location     Location where the wrapped key is stored in NVM.
 * @param auth_data                         Authorization data used while provisioning.
 */
uint32_t populate_esec_ecc_key_for_wrapped_key(struct esec_ecc_key* ecc_key, NvmPtr wrap_key_location,
                                               uint8_t* auth_data)
{
    uint32_t key_metadata;
    const uint32_t wrapp_key_keydata_offset = 4;
    NvmPtr wrapped_key_metadata_addr = wrap_key_location;
    NvmPtr wrapped_key_keydata_addr = wrap_key_location + wrapp_key_keydata_offset;

    gpHal_NVMError_t nvm_error =
        gpHal_NVMRead(wrapped_key_metadata_addr, sizeof(key_metadata), (uint8_t*)&key_metadata);
    if(nvm_error)
    {
        return -1;
    }

    map_keymetadata_to_esec_ecc_key(ecc_key, key_metadata);
    ecc_key->storage.auth = auth_data;
    ecc_key->storage.material = (uint8_t*)wrapped_key_keydata_addr;
    return 0;
}

/*****************************************************************************
 *                    Public Function Implementations
 *****************************************************************************/

bool qvCHIP_Crypto_DacAndKeyInInfoPage(void)
{
    uint32_t PK_magic;
    uint32_t DAC_magic;
    gpHal_NVMError_t nvm_error;

    // read first 32bit word of wrapped key
    nvm_error = gpHal_NVMRead(PK_LOCATION, 4, (uint8_t*)&PK_magic);
    if(nvm_error != gpHal_NVMError_Success)
    {
        return false;
    }

    // read first 32bit word of DAC
    nvm_error = gpHal_NVMRead(DAC_LOCATION, 4, (uint8_t*)&DAC_magic);
    if(nvm_error != gpHal_NVMError_Success)
    {
        return false;
    }

    // Print out the magic values as hex
    // GP_LOG_SYSTEM_PRINTF("PK Magic: 0x%08x", 0, (unsigned int)PK_magic);
    // GP_LOG_SYSTEM_PRINTF("DAC Magic: 0x%08x", 0, (unsigned int)DAC_magic);

    // verify PK magic
    // signature (bit 10), domain (12), public (13) and private (14) bits are set in wrapped key metadata
    const uint16_t KEY_METADATA = 0xE400;
    const uint8_t KEY_TYPE = 0x8;
    const uint8_t KEY_MODE = 0x2;
    const uint8_t KEY_RESTRICTION = 0x1;
    const uint32_t PK_MAGIC_MASK = KEY_METADATA | KEY_RESTRICTION << 24 | KEY_MODE << 26 | KEY_TYPE << 28;
    bool is_pk = (PK_magic & PK_MAGIC_MASK);

    // GP_LOG_SYSTEM_PRINTF("is_pk: %d", 0, is_pk);

    // verify DAC magic
    const uint8_t ASN1_SEQUENCE_TAG = 0x30;
    const uint8_t ASN1_LENGTH_FLAG = 0x80;
    uint8_t length_bytes = (((DAC_magic >> 8) & 0xff) & ASN1_LENGTH_FLAG) ? (((DAC_magic >> 8) & 0xff) & 0x7F) : 0;
    // expect length bytes to be 2 (max is 3)
    uint16_t length = (((DAC_magic >> 16) & 0xFF) << 8) | ((DAC_magic >> 24) & 0xFF);
    bool is_dac = ((DAC_magic & 0xff) == ASN1_SEQUENCE_TAG) && length_bytes == 2 && (length <= DAC_MAX_SIZE);

    // GP_LOG_SYSTEM_PRINTF("is_dac: %d", 0, is_dac);

    if(!is_pk || !is_dac)
    {
        return false;
    }

    return true;
}

qvStatus_t qvCHIP_Crypto_ReadDac(uint8_t* outBuffer, uint32_t bufferSize, uint32_t* outDataLength)
{
    if(outBuffer == NULL || outDataLength == NULL)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    // note: Credentials::kMaxDERCertLength (bufferSize) may be smaller than DAC_MAX_SIZE;
    uint32_t dac_max_size = min(DAC_MAX_SIZE, bufferSize);

    gpHal_NVMError_t nvm_error = gpHal_NVMRead(DAC_LOCATION, dac_max_size, outBuffer);
    // TODO: fetch from info page first and only fallback to factory data afterwards
    // if (status != QV_STATUS_NO_ERROR)
    // {
    //     ChipLogProgress(NotSpecified, "tlvDataLength: %lu", tlvDataLength);
    //     status = qvCHIP_FactoryDataGetValue(TAG_ID_DEVICE_ATTESTATION_CERTIFICATE, attestationCertSpan.data(),
    //                                         attestationCertSpan.size(), &tlvDataLength);
    // }
    if(nvm_error != gpHal_NVMError_Success)
    {
        return QV_STATUS_NVM_ERROR;
    }

    // Parse the certificate to determine its length
    mbedtls_x509_crt crt;
    mbedtls_x509_crt_init(&crt);

    int ret = mbedtls_x509_crt_parse(&crt, outBuffer, dac_max_size);
    if(ret != 0)
    {
        GP_LOG_PRINTF("Failed to parse certificate: %d\n", 0, ret);
        mbedtls_x509_crt_free(&crt);
        return QV_STATUS_INVALID_DATA;
    }

    *outDataLength = crt.raw.len;

    mbedtls_x509_crt_free(&crt);

    return QV_STATUS_NO_ERROR;
}

qvStatus_t qvCHIP_Crypto_SignWithDacPk(uint8_t* msg, uint32_t msg_size, uint8_t* signature_dst)
{
    if((msg == NULL) || msg_size == 0 || signature_dst == NULL)
    {
        return QV_STATUS_INVALID_ARGUMENT;
    }

    uint32_t error;

    uint8_t auth_data[SEC_STOR_AUTH_SIZE] = {0};
    struct esec_ecc_key ecc_private_key;

    /* Sign message using provisioned (wrapped) user private key */
    populate_esec_ecc_key_for_wrapped_key(&ecc_private_key, PK_LOCATION, auth_data);
    error = esec_ecdsa_sign(ESEC_HASH_ALGO_SHA256, ecc_private_key, msg, msg_size, signature_dst);
    if(error)
    {
        return QV_STATUS_INVALID_DATA;
    }

    return QV_STATUS_NO_ERROR;
}
