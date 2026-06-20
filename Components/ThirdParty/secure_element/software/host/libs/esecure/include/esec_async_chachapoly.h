/**
 * @brief eSecure Library: ChaCha20 and Poly1305 asynchronous crypto acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_CHACHAPOLY_HEADER
#define ESEC_ASYNC_CHACHAPOLY_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_symm_basics.h"

/** Encrypt and generate asynchronously an authentication tag with
 *  Chacha20 & Poly1305
 *
 * The symmetric key must have 32 bytes.
 *
 * The generated authentication tag has a size of 16 bytes.
 *
 * The argument len gives the size of the plaintext and of the ciphertext.
 * Any length up to 256MB is allowed.
 *
 * The algorithm follows the definition from RFC7539. As defined in the RFC in
 * section 2.8, the internal counter starts at 1.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_chachapoly_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                    const uint8_t nonce[12], uint8_t* ciphertext, size_t len, const uint8_t* header,
                                    size_t headerlen, uint8_t tag[16]);

/** Decrypt and verify asynchronously the message authentication tag with
 *  Chacha20 & Poly1305
 *
 * See also esec_chachapoly_encrypt().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_chachapoly_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                    const uint8_t nonce[12], uint8_t* plaintext, size_t len, const uint8_t* header,
                                    size_t headerlen, const uint8_t tag[16]);

/** Encrypt asynchronously the plaintext with ChaCha20
 *
 * The symmetric key must have 32 bytes.
 *
 * The initialization vector (IV) has a size of 4 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_chacha_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext,
                                const uint8_t iv[4], const uint8_t nonce[12], size_t len, uint8_t* ciphertext);

/** Decrypt asynchronously the ciphertext with ChaCha20
 *
 * The symmetric key must have 32 bytes.
 *
 * The initialization vector (IV) has a size of 4 bytes.
 *
 * See also esec_chacha_encrypt_go().
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_chacha_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext,
                                const uint8_t iv[4], const uint8_t nonce[12], size_t len, uint8_t* plaintext);

#endif
