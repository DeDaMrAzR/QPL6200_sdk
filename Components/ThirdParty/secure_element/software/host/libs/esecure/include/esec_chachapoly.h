/**
 * @brief eSecure Library: ChaCha20 and Poly1305 crypto acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_CHACHAPOLY_HEADER
#define ESEC_CHACHAPOLY_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_symm_basics.h"

/** Encrypt and generate an authentication tag with Chacha20 & Poly1305
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
 **/
uint32_t esec_chachapoly_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t nonce[12],
                                 uint8_t* ciphertext, size_t len, const uint8_t* header, size_t headerlen,
                                 uint8_t tag[16]);

/** Decrypt and verify the message authentication tag with Chacha20 & Poly1305
 *
 * See also esec_chachapoly_encrypt().
 *
 * Returns ESEC_OKAY for a valid signature or
 * ESEC_INVALID_SIGNATURE when the signature does not match the message.
 **/
uint32_t esec_chachapoly_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t nonce[12],
                                 uint8_t* plaintext, size_t len, const uint8_t* header, size_t headerlen,
                                 const uint8_t tag[16]);

/** Encrypt the plaintext with ChaCha20
 *
 * The symmetric key must have 32 bytes.
 *
 * The initialization vector (IV) has a size of 4 bytes.
 **/
uint32_t esec_chacha_encrypt(struct esec_skey skey, const uint8_t* plaintext, const uint8_t iv[4],
                             const uint8_t nonce[12], size_t len, uint8_t* ciphertext);

/** Decrypt the ciphertext with ChaCha20
 *
 * The symmetric key must have 32 bytes.
 *
 * The initialization vector (IV) has a size of 4 bytes.
 *
 * See also esec_chacha_encrypt().
 **/
uint32_t esec_chacha_decrypt(struct esec_skey skey, const uint8_t* ciphertext, const uint8_t iv[4],
                             const uint8_t nonce[12], size_t len, uint8_t* plaintext);

#endif
