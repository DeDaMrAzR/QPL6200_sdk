/**
 * @brief eSecure Library: ARIA crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ARIA_HEADER
#define ESEC_ARIA_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_aria_basics.h"

/** Encrypt plaintext with ECB into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ecb_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext into the plaintext with ECB.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ecb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CTR cipher mode into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ctr_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext into the plaintext with CTR cipher mode.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ctr_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CBC cipher mode into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_cbc_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with CBC cipher mode into a plaintext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_cbc_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with CFB cipher mode into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_cfb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with CFB cipher mode into a plaintext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_cfb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len);

/** Encrypt the plaintext with OFB cipher mode into a ciphertext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ofb_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* ciphertext, size_t len);

/** Decrypt the ciphertext with OFB cipher mode into a plaintext.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 **/
uint32_t esec_aria_ofb_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[ESEC_ARIA_IV_SIZE],
                               uint8_t* plaintext, size_t len);

/** Encrypt and generate an authentication tag with ARIA GCM cipher mode.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The initialization vector (IV) has a size of 12 bytes.
 *
 * The generated authentication tag can have a size of 4 up to 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 * The additional authentication data(AAD) must be multiple of 16 bytes.
 **/
uint32_t esec_aria_gcm_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE],
                               uint8_t* ciphertext, size_t len, const uint8_t* aad, size_t aadlen, uint8_t* tag,
                               size_t tag_len);

/** Decrypt and verify the message authentication tag with ARIA GCM.
 *
 * The allowed ARIA key sizes are 16, 24 and 32 bytes.
 *
 * The initialization vector (IV) has 12 bytes are used.
 *
 * The generated authentication tag can have a size of 4 up to 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * As ARIA works per block, the size must be a multiple of 16 bytes.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 * The additional authentication data(AAD) must be multiple of 16 bytes.
 **/
uint32_t esec_aria_gcm_decrypt(struct esec_skey skey, const uint8_t* ciphertext,
                               const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* plaintext, size_t len,
                               const uint8_t* aad, size_t aadlen, const uint8_t* tag, size_t tag_len);

/** Encrypt and generate an authentication tag with ARIA CCM cipher mode.
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
 **/
uint32_t esec_aria_ccm_encrypt(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                               const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                               uint8_t* tag, size_t tagsize);

/** Decrypt and verify the message authentication tag with ARIA CCM.
 *
 * See also esec_aria_ccm_encrypt().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 **/
uint32_t esec_aria_ccm_decrypt(struct esec_skey skey, const uint8_t* ciphertext, uint8_t* plaintext, size_t len,
                               const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                               const uint8_t* tag, size_t tagsize);

/** Compute the ARIA-CMAC signature of the message.
 *
 * Tag size must be 16 bytes
 * */
uint32_t esec_aria_cmac_generate(struct esec_skey skey, const uint8_t* message, size_t len,
                                 uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC]);

/** Verify a message integrity against the passed CMAC signature.
 *
 * Tag size must be 16 bytes
 *
 * Returns ESEC_OKAY for a valid tag or
 * ESEC_INVALID_SIGNATURE when the tag does not match the computed tag.
 **/
uint32_t esec_aria_cmac_verify(struct esec_skey skey, const uint8_t* message, size_t len,
                               const uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC]);

/**
 * Start split plaintext ARIA-CBC encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cbc_enc_update() and esec_aria_cbc_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cbc_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start split plaintext ARIA-CTR encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ctr_enc_update() and esec_aria_ctr_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ctr_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start split plaintext ARIA-CFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cfb_enc_update() and esec_aria_cfb_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cfb_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start split plaintext ARIA-OFB encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ofb_enc_update() and esec_aria_ofb_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ofb_enc_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* plaintext, uint8_t* ciphertext, size_t len);

/**
 * Start split ciphertext ARIA-CBC decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cbc_dec_update_go() and esec_aria_cbc_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cbc_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start split ciphertext ARIA-CTR decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ctr_dec_update_go() and esec_aria_ctr_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ctr_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start split ciphertext ARIA-CFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_cfb_dec_update_go() and esec_aria_cfb_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cfb_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Start split ciphertext ARIA-OFB decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ofb_dec_update_go() and esec_aria_ofb_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ofb_dec_start(struct esec_aria_state* state, struct esec_skey skey, const uint8_t iv[16],
                                 const uint8_t* ciphertext, uint8_t* plaintext, size_t len);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cbc_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ctr_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cfb_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ofb_enc_update(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cbc_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ctr_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_cfb_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ofb_dec_update(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * End split plaintext ARIA encryption.
 *.
 */
uint32_t esec_aria_cbc_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * End split plaintext ARIA encryption.
 *
 */
uint32_t esec_aria_ctr_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * End split plaintext ARIA encryption.
 *
 */
uint32_t esec_aria_cfb_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * End split plaintext ARIA encryption.
 *
 */
uint32_t esec_aria_ofb_enc_finish(struct esec_aria_state* state, struct esec_skey skey, const uint8_t* plaintext,
                                  uint8_t* ciphertext, size_t len);

/**
 * End multi-message ARIA decryption.
 *
 */
uint32_t esec_aria_cbc_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * End multi-message ARIA decryption.
 *
 */
uint32_t esec_aria_ctr_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * End multi-message ARIA decryption.
 *
 */
uint32_t esec_aria_cfb_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * End multi-message ARIA decryption.
 *
 */
uint32_t esec_aria_ofb_dec_finish(struct esec_aria_state* state, struct esec_skey skey, uint8_t* plaintext,
                                  const uint8_t* ciphertext, size_t len);

/**
 * Start split plaintext ARIA-GCM encryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_gcm_enc_update() and esec_aria_gcm_enc_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_gcm_encrypt_start(struct esec_skey skey, const uint8_t* plaintext,
                                     const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE], uint8_t* ciphertext, size_t len,
                                     const uint8_t* header, size_t headerlen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_gcm_encrypt_update(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * End split plaintext ARIA encryption.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 */
uint32_t esec_aria_gcm_encrypt_finish(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tag_len,
                                      uint8_t lenAlenC[16]);

/**
 * Start split ciphertext ARIA-GCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_gcm_dec_update_go() and esec_aria_gcm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_gcm_decrypt_start(struct esec_skey skey, uint8_t* plaintext, const uint8_t iv[ESEC_ARIA_GCM_IV_SIZE],
                                     const uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_gcm_decrypt_update(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE]);

/**
 * End multi-message ARIA decryption.
 * The argument tag_len is the tag length which can be
 * between 4 and 16 bytes
 *
 */
uint32_t esec_aria_gcm_decrypt_finish(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag, size_t tag_len,
                                      uint8_t lenAlenC[16]);

/**
 * Start split ciphertext ARIA-CCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ccm_dec_update_go() and esec_aria_ccm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ccm_encrypt_start(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                     const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize, size_t total_len);

/**
 * Continue split plaintext ARIA encryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ccm_encrypt_update(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize);

/**
 * End split plaintext ARIA encryption.
 *
 */
uint32_t esec_aria_ccm_encrypt_finish(struct esec_skey skey, const uint8_t* plaintext, uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag,
                                      size_t tagsize);

/**
 * Start split ciphertext ARIA-CCM decryption. The key and mode
 * are stored inside the state and are used in the subsequent calls to
 * esec_aria_ccm_dec_update_go() and esec_aria_ccm_dec_finish_go().
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ccm_decrypt_start(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                     const uint8_t* nonce, size_t noncelen, const uint8_t* header, size_t headerlen,
                                     uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize, size_t total_len);

/**
 * Continue split plaintext ARIA decryption.
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 */
uint32_t esec_aria_ccm_decrypt_update(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], size_t tagsize);

/**
 * End multi-message ARIA decryption.
 *
 */
uint32_t esec_aria_ccm_decrypt_finish(struct esec_skey skey, uint8_t* plaintext, const uint8_t* ciphertext, size_t len,
                                      size_t noncelen, uint8_t ctx[ESEC_ARIA_CTX_xCM_SIZE], uint8_t* tag,
                                      size_t tagsize);

/** Start ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 * */
uint32_t esec_aria_cmac_generate_start(struct esec_skey skey, const uint8_t* message, size_t len,
                                       uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Continue ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 * */
uint32_t esec_aria_cmac_generate_update(struct esec_skey skey, const uint8_t* message, size_t len,
                                        uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Finish ARIA-CMAC signature of the message.
 *
 * Ctx size must be 16 bytes
 * Tag size must be 16 bytes
 * */
uint32_t esec_aria_cmac_generate_finish(struct esec_skey skey, const uint8_t* message, size_t len,
                                        uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t digest[ESEC_TAG_SIZE_ARIA_CMAC]);

/** Start ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 * */
uint32_t esec_aria_cmac_verify_start(struct esec_skey skey, const uint8_t* message, size_t len,
                                     uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Continue ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * The length of the data (plaintext, ciphertxt) must be a multiple
 * of 16.
 * */
uint32_t esec_aria_cmac_verify_update(struct esec_skey skey, const uint8_t* message, size_t len,
                                      uint8_t ctx[ESEC_ARIA_IV_SIZE]);

/** Finish ARIA-CMAC verifying of the message.
 *
 * Ctx size must be 16 bytes
 * Tag size must be 16 bytes
 * */
uint32_t esec_aria_cmac_verify_finish(struct esec_skey skey, const uint8_t* message, size_t len,
                                      uint8_t ctx[ESEC_ARIA_IV_SIZE], uint8_t tag[ESEC_TAG_SIZE_ARIA_CMAC]);

#endif
