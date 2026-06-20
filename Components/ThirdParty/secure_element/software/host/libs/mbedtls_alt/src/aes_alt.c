/*
 *  FIPS-197 compliant AES implementation
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
 * Copyright (C) 2018 Silex Inside: hardware offload with CryptoSoc.
 */
/*
 *  The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
 *
 *  http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
 *  http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 */

#if !defined(MBEDTLS_CONFIG_FILE)

#include <mbedtls/build_info.h>
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_AES_C)

#include <string.h>

#include <stdint.h>
#include <mbedtls/aes.h>
#include "mbedtls/error.h"

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_printf printf
#endif /* MBEDTLS_PLATFORM_C */

#if defined(MBEDTLS_AES_ALT)
#include "esec_aes.h"
#include "esec_aes_basics.h"
#include "esecure_async.h"
#include "gpHal_SEC.h"

/* Implementation that should never be optimized out by the compiler */
static void mbedtls_zeroize(void* v, size_t n)
{
    volatile unsigned char* p = (unsigned char*)v;
    while(n--)
        *p++ = 0;
}

void mbedtls_aes_init(mbedtls_aes_context* ctx)
{
    memset(ctx, 0, sizeof(mbedtls_aes_context));
}

void mbedtls_aes_free(mbedtls_aes_context* ctx)
{
    if(ctx == NULL)
        return;

    mbedtls_zeroize(ctx, sizeof(mbedtls_aes_context));
}

/*
 * AES key schedule (encryption)
 */
int mbedtls_aes_setkey_enc(mbedtls_aes_context* ctx, const unsigned char* key, unsigned int keybits)
{
    if(!(keybits == 128 || keybits == 192 || keybits == 256))
    {
        return (MBEDTLS_ERR_AES_INVALID_KEY_LENGTH);
    }
    uint32_t keylen = keybits/8;
    ctx->keysize = keylen;
    memcpy(ctx->key, key, keylen);
    return 0;
}

/*
 * AES key schedule (decryption)
 */
int mbedtls_aes_setkey_dec(mbedtls_aes_context* ctx, const unsigned char* key, unsigned int keybits)
{
    return mbedtls_aes_setkey_enc(ctx, key, keybits);
}

/*
 * AES-ECB block encryption/decryption
 */
int mbedtls_aes_crypt_ecb(mbedtls_aes_context* ctx,
                          int mode,
                          const unsigned char input[16],
                          unsigned char output[16])
{
    uint32_t status = 0;
    gpEncryption_AESOptions_t aes_options;
    struct esec_skey skey = {.storage = ESEC_STORAGE_HOST(ctx->key), .keyspec = ctx->keysize};

    if( mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT )
    {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    if (mode == MBEDTLS_AES_ENCRYPT)
    {
        aes_options.keylen = ctx->keysize;
        aes_options.options = gpEncryption_KeyIdKeyPtr;
        memcpy(output, input, 16);
        status = gpHal_AESEncrypt(output, ctx->key, aes_options);
    }
    else
    {
        status = esec_aes_ecb_decrypt(skey, input, output, 16);
    }

    return (status == 0) ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
}

#if defined(MBEDTLS_CIPHER_MODE_CBC)
/*
 * AES-CBC buffer encryption/decryption
 */

int mbedtls_aes_crypt_cbc(mbedtls_aes_context* ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char* input,
                          unsigned char* output)
{
    int status = 0;
    struct esec_mb mb = {};
    struct esec_aes_state state = {};
    struct esec_skey skey = {.storage = ESEC_STORAGE_HOST(ctx->key), .keyspec = ctx->keysize};

    if(mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT)
    {
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    if(length % 16)
    {
        return (MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH);
    }

    if(mode == MBEDTLS_AES_ENCRYPT)
    {
        status = esec_aes_cbc_enc_start_go(&mb, &state, skey, iv, input, output, length);
    }
    else
    {
        status = esec_aes_cbc_dec_start_go(&mb, &state, skey, iv, input, output, length);
    }

    if(status != 0)
    {
        /* sending mailbox command to start enc/dec failed */
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    }

    status = esec_pop_cmd_response();
    if(status == 0)
    {
        /* mailbox command completed successfully */
        memcpy(iv, state.context, 16);
    }

    return (status == 0) ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
}
#endif /* MBEDTLS_CIPHER_MODE_CBC */

#if defined(MBEDTLS_CIPHER_MODE_CFB)
/*
 * AES-CFB128 buffer encryption/decryption
 */

static inline void _handle_cfb128_partial_block(int32_t mode, const uint8_t* input, uint8_t* output, uint8_t* iv,
                                                size_t* iv_offset, uint8_t length)
{
    uint8_t c;
    uint8_t n = *iv_offset;
    const uint8_t aes_block_size = 16;

    while(length--)
    {
        if(mode == MBEDTLS_AES_DECRYPT)
        {
            c = *input++;
            *output++ = (uint8_t)(c ^ iv[n]);
            iv[n] = (uint8_t)c;
        }
        else
        {
            iv[n] = *output++ = (uint8_t)(iv[n] ^ *input++);
        }
        n++;
    }
    *iv_offset = n % aes_block_size;
}

int mbedtls_aes_crypt_cfb128(mbedtls_aes_context* ctx, int mode, size_t length, size_t* iv_off, unsigned char iv[16],
                             const unsigned char* input, unsigned char* output)
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n;
    struct esec_mb mb = {};
    struct esec_aes_state state = {};
    struct esec_skey skey = {.storage = ESEC_STORAGE_HOST(ctx->key), .keyspec = ctx->keysize};
    uint32_t len_blocks;
    uint32_t status;

    if(mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT)
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;

    /* handle bytes in first block if iv_off is not zero */
    n = *iv_off;

    if(n > 15)
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;

    if(n)
    {
        uint8_t rem = 16 - n;
        _handle_cfb128_partial_block(mode, input, output, iv, iv_off, rem);
        input += rem;
        output += rem;
        length -= rem;
    }

    len_blocks = (length / 16) * 16;
    /* handle remaining 16-byte blocks */
    if(len_blocks)
    {
        if(mode == MBEDTLS_AES_DECRYPT)
        {
            status = esec_aes_cfb_dec_start_go(&mb, &state, skey, iv, input, output, len_blocks);
        }
        else
        {
            status = esec_aes_cfb_enc_start_go(&mb, &state, skey, iv, input, output, len_blocks);
        }
        if(status != 0)
        {
            /* sending mailbox command to start enc/dec failed */
            return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
        }
        status = esec_pop_cmd_response();
        if(status == 0)
        {
            /* mailbox command completed successfully */
            memcpy(iv, state.context, 16);
            input += len_blocks;
            output += len_blocks;
        }
        else
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
        length -= len_blocks;
    }

    // handle last block if bytes remaining
    if(length)
    {
        ret = mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, iv, iv);
        if(ret != 0)
            return ret;

        _handle_cfb128_partial_block(mode, input, output, iv, iv_off, length);
    }

    return 0;
}

/*
 * AES-CFB8 buffer encryption/decryption
 */
int mbedtls_aes_crypt_cfb8( mbedtls_aes_context *ctx,
                            int mode,
                            size_t length,
                            unsigned char iv[16],
                            const unsigned char *input,
                            unsigned char *output )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    unsigned char c;
    unsigned char ov[17];

    if( mode != MBEDTLS_AES_ENCRYPT && mode != MBEDTLS_AES_DECRYPT )
        return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
    while( length-- )
    {
        memcpy( ov, iv, 16 );
        ret = mbedtls_aes_crypt_ecb( ctx, MBEDTLS_AES_ENCRYPT, iv, iv );
        if( ret != 0 )
            goto exit;

        if( mode == MBEDTLS_AES_DECRYPT )
            ov[16] = *input;

        c = *output++ = (unsigned char)( iv[0] ^ *input++ );

        if( mode == MBEDTLS_AES_ENCRYPT )
            ov[16] = c;

        memcpy( iv, ov + 1, 16 );
    }
    ret = 0;

exit:
    return( ret );
}
#endif /*MBEDTLS_CIPHER_MODE_CFB */

#if defined(MBEDTLS_CIPHER_MODE_CTR)
/*
 * AES-CTR buffer encryption/decryption
 */
int mbedtls_aes_crypt_ctr( mbedtls_aes_context *ctx,
                       size_t length,
                       size_t *nc_off,
                       unsigned char nonce_counter[16],
                       unsigned char stream_block[16],
                       const unsigned char *input,
                       unsigned char *output )
{
    int c, i;
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t n;
    uint32_t status = 0;

    n = *nc_off;

    if(n > 0x0F)
        return (MBEDTLS_ERR_AES_BAD_INPUT_DATA);

    if(n != 0)
    {
        for(uint32_t offset = n; offset < 16; offset++)
        {
            c = *input++;
            *output++ = (unsigned char)(c ^ stream_block[offset]);
            length--;
        }
    }

    struct esec_mb mb = {};
    struct esec_aes_state state = {};
    struct esec_skey skey = {.storage = ESEC_STORAGE_HOST(ctx->key), .keyspec = ctx->keysize};
    uint32_t len_blocks = (length / 16) * 16;

    if(len_blocks)
    {
        status = esec_aes_ctr_enc_start_go(&mb, &state, skey, nonce_counter, input, output, len_blocks);
        if(status != 0)
        {
            /* sending mailbox command to start enc/dec failed */
            return MBEDTLS_ERR_AES_BAD_INPUT_DATA;
        }
        status = esec_pop_cmd_response();
        if(status == 0)
        {
            /* mailbox command completed successfully */
            memcpy(nonce_counter, state.context, 16);
            input += len_blocks;
            output += len_blocks;
        }
        else
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
    }

    uint32_t rem = length % 16;
    n = 0;
    if(rem)
    {
        ret = mbedtls_aes_crypt_ecb(ctx, MBEDTLS_AES_ENCRYPT, nonce_counter, stream_block);
        if(ret != 0)
        {
            return MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
        }
        for(i = 16; i > 0; i--)
            if(++nonce_counter[i - 1] != 0)
                break;
        while(rem--)
        {
            c = *input++;
            *output++ = (unsigned char)(c ^ stream_block[n++]);
        }
    }

    *nc_off = n;
    return (status == 0) ? 0 : MBEDTLS_ERR_PLATFORM_HW_ACCEL_FAILED;
}
#endif /* MBEDTLS_CIPHER_MODE_CTR */

#endif /* MBEDTLS_AES_ALT */

#endif /* MBEDTLS_AES_C */
