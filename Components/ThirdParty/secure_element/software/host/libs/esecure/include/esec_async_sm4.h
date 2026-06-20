/**
 * @brief eSecure Library: SM4 asynchronous crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_SM4_HEADER
#define ESEC_ASYNC_SM4_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_sm4_basics.h"

/** Encrypt asynchronously plaintext with ECB into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with ECB.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CBC into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CBC.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CTR into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CTR.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CFB into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CFB.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with OFB into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with OFB.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with GCM into a ciphertext.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * The tag_len is the size of the tag and can be 4 bytes up to 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[ESEC_SM4_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                 const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tag_len);

/** Decrypt asynchronously the ciphertext into the plaintext with GCM.
 *
 * The allowed SM4 key size is 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As SM4 works per block, the size should be a multiple of 16 bytes.
 *
 * The tag_len is the size of the tag and can be 4 bytes up to 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm4_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[ESEC_SM4_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                                 const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tag_len);

#endif
