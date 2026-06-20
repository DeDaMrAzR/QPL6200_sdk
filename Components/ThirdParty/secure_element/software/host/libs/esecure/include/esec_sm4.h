/**
 * @brief eSecure Library: SM4 crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SM4_HEADER
#define ESEC_SM4_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_sm4_basics.h"

/** Encrypt plaintext with ECB into a ciphertext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext into the plaintext with ECB.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CTR cipher mode into a ciphertext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 **/
uint32_t esec_sm4_ctr_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext into the plaintext with CTR cipher mode.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 **/
uint32_t esec_sm4_ctr_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CBC cipher mode into a ciphertext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_cbc_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with CBC cipher mode into a plaintext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_cbc_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CFB cipher mode into a ciphertext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_cfb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with CFB cipher mode into a plaintext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_cfb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with OFB cipher mode into a ciphertext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 **/
uint32_t esec_sm4_ofb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with OFB cipher mode into a plaintext.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 **/

uint32_t esec_sm4_ofb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_IV_SIZE],
                              uint8_t* plaintext, size_t len);

/** Encrypt and generate an authentication tag with SM4 GCM cipher mode.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The initialization vector (IV) has a size of 16 bytes,
 * only 12 bytes are used.
 *
 * The generated authentication tag can have a size of 4 up to 16 bytes.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * The additional authentication data(AAD) must be multiple of 16 bytes.
 **/
uint32_t esec_sm4_gcm_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_SM4_GCM_IV_SIZE],
                              uint8_t* ciphertext, size_t len, const uint8_t* aad, size_t aadlen, uint8_t* tag,
                              size_t tag_len);

/** Decrypt and verify the message authentication tag with SM4 GCM.
 *
 * The allowed SM4 key sizes is 16 bytes.
 *
 * The initialization vector (IV) has a size of 16 bytes,
 * only 12 bytes are used.
 *
 * The generated authentication tag can have a size of 4 up to 16 bytes.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * The additional authentication data(AAD) must be multiple of 16 bytes.
 **/
uint32_t esec_sm4_gcm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_SM4_GCM_IV_SIZE],
                              uint8_t* plaintext, size_t len, const uint8_t* aad, size_t aadlen, const uint8_t* tag,
                              size_t tag_len);
#endif
