/**
 * @brief eSecure Library: ARIA asynchronous crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_ARIA_HEADER
#define ESEC_ASYNC_ARIA_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_aria_basics.h"

/** Encrypt asynchronously plaintext with ECB into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with ECB.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CBC into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CBC.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CTR into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ctr_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CTR.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ctr_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with CFB into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cfb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with CFB.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cfb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with OFB into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ofb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* ciphertext, size_t len);

/** Decrypt asynchronously the ciphertext into the plaintext with OFB.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size should be a multiple of 16 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ofb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_IV_SIZE], uint8_t* plaintext, size_t len);

/** Encrypt asynchronously plaintext with GCM into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The argument tag_len is the size of the tag and can be 4 bytes up to 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_gcm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                  const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tag_len);

/** Decrypt asynchronously the ciphertext into the plaintext with GCM.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * The argument tag_len is the size of the tag and can be 4 bytes up to 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_gcm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                                  const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tag_len);

/** Encrypt and generate asynchronously an authentication tag with ARIA CCM cipher mode.
 *
 * Encrypt and authenticate plaintext according to the ARIA CCM cipher mode
 * defined in RFC 3610.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes. The nonce has a size from
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
uint32_t esec_aria_ccm_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                  const uint8_t* header, size_t headerlen, uint8_t* tag, size_t tagsize);

/** Decrypt and verify asynchronously the message authentication tag with ARIA CCM.
 *
 * See also esec_aria_ccm_encrypt_go().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_ccm_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                  uint8_t* plaintext, size_t len, const uint8_t* nonce, size_t noncelen,
                                  const uint8_t* header, size_t headerlen, const uint8_t* tag, size_t tagsize);

/** Compute asynchronously the ARIA-CMAC signature of the message.
 *
 * Tag size must be 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cmac_generate_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                    uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC]);

/** Verify asynchronously a message integrity against the passed ARIA-CMAC signature.
 *
 * Tag size must be 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_aria_cmac_verify_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                  const uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC]);

/**
 * Start asynchronously split plaintext ARIA-CBC encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cbc_enc_start_update() and esec_aria_cbc_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext ARIA-CTR encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ctr_enc_start_update() and esec_aria_ctr_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext ARIA-CFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cfb_enc_start_update() and esec_aria_cfb_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext ARIA-OFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ofb_enc_start_update() and esec_aria_ofb_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_enc_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split ciphertext ARIA-CBC decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cbc_dec_update_go() and esec_aria_cbc_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext ARIA-CTR decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ctr_dec_update_go() and esec_aria_ctr_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext ARIA-CFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cfb_dec_update_go() and esec_aria_cfb_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start asynchronously split ciphertext ARIA-OFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ofb_dec_update_go() and esec_aria_ofb_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_dec_start_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                    const uint8_t iv[16], const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_enc_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_dec_update_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_enc_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cbc_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ctr_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cfb_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ofb_dec_finish_go(struct esec_mb* mb, struct esec_aria_state* state, struct esec_skey skey,
                                     uint8_t* plaintext, const uint8_t* ciphertext, size_t len);

/**
 * Start asynchronously split plaintext ARIA-GCM encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_gcm_enc_start_update() and esec_aria_gcm_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_encrypt_start_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                        const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_encrypt_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_encrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                         uint8_t* tag, size_t tag_len, uint8_t lenAlenC[16]);

/**
 * Start asynchronously split ciphertext ARIA-GCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_gcm_dec_update_go() and esec_aria_gcm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_decrypt_start_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                        const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], const uint8_t* ciphertext, size_t len,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_decrypt_update_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_gcm_decrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                         uint8_t* tag, size_t tag_len, uint8_t lenAlenC[16]);

/**
 * Start asynchronously split ciphertext ARIA-CCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ccm_dec_update_go() and esec_aria_ccm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_encrypt_start_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                        uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                        size_t tagsize, size_t total_len);

/**
 * Continue asynchronously split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_encrypt_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize);

/**
 * End asynchronously split plaintext ARIA encryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_encrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                         uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tagsize);

/**
 * Start asynchronously split ciphertext ARIA-CCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ccm_dec_update_go() and esec_aria_ccm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_decrypt_start_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                        const uint8_t* ciphertext, size_t len, const uint8_t* nonce, size_t noncelen,
                                        const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE],
                                        size_t tagsize, size_t total_len);

/**
 * Continue asynchronously split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_decrypt_update_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize);

/**
 * End asynchronously multi-message ARIA decryption.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_ccm_decrypt_finish_go(struct esec_mb* mb, struct esec_skey skey, uint8_t* plaintext,
                                         const uint8_t* ciphertext, size_t len, size_t noncelen,
                                         uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tagsize);

/** Start ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_generate_init_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Continue ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_generate_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message,
                                           size_t len, uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Finish ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * Tag size must be 16 bytes
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_generate_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message,
                                           size_t len, uint8_t ctx[ESEC_ARIA_IV_SIZE],
                                           uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC]);

/** Start ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_verify_init_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                       uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Continue ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_verify_update_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Finish ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * Tag size must be 16 bytes
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_aria_cmac_verify_finish_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* message, size_t len,
                                         uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC]);
#endif
