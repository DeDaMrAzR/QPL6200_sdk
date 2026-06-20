/*
 *  NIST SP800-38C compliant CCM implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Copyright (C) 2018 Beerten Engineering
 * Copyright (C) 2018 Silex Inside
 *      hardware offload with CryptoSoc.
 */

/*
 * Definition of CCM:
 * http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf
 * RFC 3610 "Counter with CBC-MAC (CCM)"
 *
 * Related:
 * RFC 5116 "An Interface and Algorithms for Authenticated Encryption"
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include <mbedtls/build_info.h>
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_CCM_C)

#include "mbedtls/ccm.h"
#include "mbedtls/error.h"

#include <string.h>

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#if defined(MBEDTLS_CCM_ALT)
#include "esecure.h"
#include "esec_hash_basics.h"
#include "hal_SEMailbox.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void* v, size_t n)
{
    volatile unsigned char* p = (unsigned char*)v;
    while(n--)
        *p++ = 0;
}

#define CCM_ENCRYPT 0
#define CCM_DECRYPT 1

/*
 * Initialize context
 */
void mbedtls_ccm_init(mbedtls_ccm_context* ctx)
{
    memset(ctx, 0, sizeof(mbedtls_ccm_context));
}

int mbedtls_ccm_setkey(mbedtls_ccm_context* ctx, mbedtls_cipher_id_t cipher, const unsigned char* key,
                       unsigned int keybits)
{
    if(cipher != MBEDTLS_CIPHER_ID_AES)
        return (MBEDTLS_ERR_CCM_BAD_INPUT);

    if(keybits != 128 && keybits != 192 && keybits != 256)
        return (MBEDTLS_ERR_CCM_BAD_INPUT);

    memcpy(ctx->key, key, keybits / 8);
    ctx->keysize = keybits / 8;

    return (0);
}

/*
 * Free context
 */
void mbedtls_ccm_free(mbedtls_ccm_context* ctx)
{
    mbedtls_zeroize(ctx, sizeof(mbedtls_ccm_context));
}

/*
 * Authenticated encryption or decryption
 */
static int ccm_auth_crypt(mbedtls_ccm_context* ctx, int mode, size_t length, const unsigned char* iv, size_t iv_len,
                          const unsigned char* add, size_t add_len, const unsigned char* input, unsigned char* output,
                          unsigned char* tag, size_t tag_len)
{
    int ret;

    /*
     * Check length requirements: SP800-38C A.1
     * Additional requirement: a < 2^16 - 2^8 to simplify the code.
     * 'length' checked later (when writing it to the first block)
     */
    if(tag_len < 4 || tag_len > 16 || tag_len % 2 != 0)
        return (MBEDTLS_ERR_CCM_BAD_INPUT);

    /* Also implies q is within bounds */
    if(iv_len < 7 || iv_len > 13)
        return (MBEDTLS_ERR_CCM_BAD_INPUT);

    if(add_len > 0xFF00)
        return (MBEDTLS_ERR_CCM_BAD_INPUT);

    struct esec_skey skey = {.storage = ESEC_STORAGE_HOST(ctx->key), .keyspec = ctx->keysize};

    ret = esec_aes_ccm_encrypt(skey, input, output, length, add, add_len, iv, iv_len, tag, tag_len);
    if(ret)
    {
        ret = MBEDTLS_ERR_CCM_HW_ACCEL_FAILED;
    }

    return ret;
}

/*
 * Authenticated encryption
 */
int mbedtls_ccm_encrypt_and_tag(mbedtls_ccm_context* ctx, size_t length, const unsigned char* iv, size_t iv_len,
                                const unsigned char* add, size_t add_len, const unsigned char* input,
                                unsigned char* output, unsigned char* tag, size_t tag_len)
{
    return (ccm_auth_crypt(ctx, CCM_ENCRYPT, length, iv, iv_len, add, add_len, input, output, tag, tag_len));
}

/*
 * Authenticated decryption
 */
int mbedtls_ccm_auth_decrypt(mbedtls_ccm_context* ctx, size_t length, const unsigned char* iv, size_t iv_len,
                             const unsigned char* add, size_t add_len, const unsigned char* input,
                             unsigned char* output, const unsigned char* tag, size_t tag_len)
{
    int ret;

    ret =
        ccm_auth_crypt(ctx, CCM_DECRYPT, length, iv, iv_len, add, add_len, input, output, (unsigned char*)tag, tag_len);

    if(ret == MBEDTLS_ERR_CCM_AUTH_FAILED)
    {
        mbedtls_zeroize(output, length);
        ret = MBEDTLS_ERR_CCM_AUTH_FAILED;
    }
    else if(ret != 0)
    {
        ret = MBEDTLS_ERR_ERROR_GENERIC_ERROR;
    }

    return (ret);
}

int mbedtls_ccm_update(mbedtls_ccm_context* ctx, const unsigned char* input, size_t input_len, unsigned char* output,
                       size_t output_size, size_t* output_len)
{
    // link stub
    int ret = MBEDTLS_ERR_CCM_BAD_INPUT;
    return ret;
}
int mbedtls_ccm_set_lengths(mbedtls_ccm_context* ctx, size_t total_ad_len, size_t plaintext_len, size_t tag_len)
{
    // link stub
    int ret = MBEDTLS_ERR_CCM_BAD_INPUT;
    return ret;
}
int mbedtls_ccm_starts(mbedtls_ccm_context* ctx, int mode, const unsigned char* iv, size_t iv_len)
{
    // link stub
    int ret = MBEDTLS_ERR_CCM_BAD_INPUT;
    return ret;
}
#endif /* MBEDTLS_CCM_ALT */

#endif /* MBEDTLS_CCM_C */
