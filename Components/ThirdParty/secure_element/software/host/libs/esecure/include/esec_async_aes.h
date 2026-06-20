/**
 * @brief eSecure Library: AES asynchronous crypto acceleration
 * @copyright Copyright (c) 2017-2021 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_AES_HEADER
#define ESEC_ASYNC_AES_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_aes_basics.h"

/**
 * Start asynchronously split plaintext AES encryption (mode as parameter).
 * The key and mode are stored inside the state and are used in the subsequent
 * calls to esec_aes_enc_go() and esec_aes_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                               enum esec_aes_mode mode, const uint8_t iv[16], const uint8_t* plaintext,
                               uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext AES-CBC encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_enc() and esec_aes_enc_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_cbc_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext AES-CTR encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_enc() and esec_aes_enc_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_ctr_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext AES-CFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_enc() and esec_aes_enc_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_cfb_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext AES-OFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_enc() and esec_aes_enc_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_ofb_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext AES-XTS encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_enc() and esec_aes_enc_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_xts_enc_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext AES encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_enc_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* plaintext,
                         uint8_t* ciphertext, size_t len);

/**
 * End asynchronously split plaintext AES encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_enc_finish_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* plaintext,
                                uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split ciphertext AES decryption (mode as parameter).
 * The key and mode are stored inside the state and are used in the subsequent
 * calls to esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                               enum esec_aes_mode mode, const uint8_t iv[16], const uint8_t* ciphertext,
                               uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext AES-CBC decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_cbc_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext AES-CTR decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_ctr_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext AES-CFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_cfb_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext AES-OFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_ofb_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext AES-XTS decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_xts_dec_start_go(struct esec_mb* mb, struct esec_aes_state* state, struct esec_skey skey,
                                   const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Continue asynchronously split ciphertext AES decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aes_dec() and esec_aes_dec_finish().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_dec_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* ciphertext,
                         uint8_t* plaintext, size_t len);

/**
 * End asynchronously multi-message AES decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aes_dec_finish_go(struct esec_mb* mb, struct esec_aes_state* state, const uint8_t* ciphertext,
                                uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with ECB into a ciphertext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ECB works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with ECB.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ECB works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len);

/** Encrypt asynchronously the plaintext with CTR cipher mode into a ciphertext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CTR cipher mode.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously the plaintext with XTS cipher mode into a ciphertext.
 *
 * XTS requires a pair of AES keys. The double key is stored as one big key in
 * skey. The allowed combined key sizes are 32, 48 and 64 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_xts_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext with XTS cipher mode into a plaintext.
 *
 * XTS requires a pair of AES keys. The double key is stored as one big key in
 * skey. The allowed combined key sizes are 32, 48 and 64 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_xts_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously the plaintext with CBC cipher mode into a ciphertext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length above 16 bytes and up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext with CBC cipher mode into a plaintext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length above 16 bytes and up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously the plaintext with CFB cipher mode into a ciphertext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext with CFB cipher mode into a plaintext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously the plaintext with OFB cipher mode into a ciphertext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[16], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext with OFB cipher mode into a plaintext.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The length must be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[16], uint8_t* plaintext, size_t len);

/** Encrypt and generate asynchronously an authentication tag with AES GCM cipher mode.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes.
 *
 * The initialization vector (IV) has a size of 12 bytes.
 *
 * The generated authentication tag can have a size between 4 and 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length up to 4GB is allowed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 const uint8_t iv[12], uint8_t* ciphertext, size_t len, const uint8_t* header,
                                 size_t headerlen, uint8_t* tag, size_t tag_len);

/** Decrypt and verify asynchronously the message authentication tag with AES GCM.
 *
 * See also esec_aes_gcm_encrypt_go().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 const uint8_t iv[12], uint8_t* plaintext, size_t len, const uint8_t* header,
                                 size_t headerlen, const uint8_t* tag, size_t tag_len);

/** Encrypt and generate asynchronously an authentication tag with AES CCM cipher mode.
 *
 * Encrypt and authenticate plaintext according to the AES CCM cipher mode
 * defined in RFC 3610.
 *
 * The allowed AES key sizes are 16, 24 and 32 bytes. The nonce has a size from
 * 2 to 8 bytes. The header, also called "additional authenticated data" or
 * AAD, can have any size up to 2^64 bytes. The generated tag can have the
 * sizes of 4, 6, 8, 10, 12, 14 or 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext. The
 * length should be smaller than 256^(15-noncelen).
 *
 * One can also achieve CCM* style encryption without authentication by
 * configuring a "tagsize" of 0. For CCM*, the caller has to format the nonce
 * according to the CCM* parameters from the protocol specification.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ccm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                 uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t* nonce, size_t noncelen, uint8_t* tag, size_t tagsize);

/** Decrypt and verify asynchronously the message authentication tag with AES CCM.
 *
 * See also esec_aes_ccm_encrypt_go().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_ccm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                 uint8_t* plaintext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t* nonce, size_t noncelen, const uint8_t* tag, size_t tagsize);

/** Compute asynchronously the CMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_cmac_generate_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                               uint8_t digest[ESEC_TAG_SIZE_AES_CMAC]);

/** Start the partial asynchronous computation of the CMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_cmac_generate_begin_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                     uint8_t* context_out);

/** Update the partial asynchronous computation of the CMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_cmac_generate_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                      const uint8_t* context_in, uint8_t* context_out);

/** Complete the partial asynchronous computation of the CMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_cmac_generate_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                      const uint8_t* context_in, uint8_t digest[ESEC_TAG_SIZE_AES_CMAC]);

/** Verify asynchronously a message integrity against the passed CMAC signature.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_cmac_verify_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                             const uint8_t digest[ESEC_TAG_SIZE_AES_CMAC]);

/** Wrap asynchronously a key using AES.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_skey wrapping_key,
                              uint8_t* output, uint32_t output_size);

/** Unwrap asynchronously a key using AES.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aes_key_unwrap_go(struct esec_mb* mb, struct esec_skey wrapping_key, uint8_t* wrapped_key,
                                uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                struct esec_key_storage storage, struct esec_skey* key);

#endif
