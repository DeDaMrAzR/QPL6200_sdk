/**
 * @brief eSecure Library: EdDSA keys and crypto acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_EDDSA_HEADER
#define ESEC_EDDSA_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_ed25519_basics.h"

/** Create an EdDSA public and private key pair randomly.
 *
 * A key pair consists of 2 times the 32 bytes for Ed25519, and 2 times 57 bytes
 * for Ed448.
 **/
uint32_t esec_eddsa_create_key(uint32_t domainspec, struct esec_key_storage storage, int flags,
                               struct esec_eddsa_key* key);

/** Import an EdDSA public, private or key pair into protected eSecure storage.
 *
 * The storage must be one of the eSecure protected storages: volatile, wrapped
 * or wrapped with anti-replay.
 *
 * The argument pubpriv tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or ESEC_KEY_PAIR).
 * It may not be empty.
 *
 * The flags argument accepts ESEC_KEY_EXPORTABLE or 0.
 **/
uint32_t esec_eddsa_import_key(uint32_t domainspec, uint32_t pubpriv, const uint8_t* material,
                               struct esec_key_storage storage, int flags, struct esec_eddsa_key* key);

/** Export key material from a protected EdDSA key. **/
uint32_t esec_eddsa_export_key(struct esec_eddsa_key key, uint8_t* material);

/** Read the public key from the eSecure EdDSA key.
 *
 * If the eSecure key does not contain the public key, it's recomputed.
 **/
uint32_t esec_eddsa_read_pub_key(struct esec_eddsa_key key, uint8_t* pubkey);

/** Transform a EdDSA key into a new version with another protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 */
uint32_t esec_eddsa_transfer_key(struct esec_eddsa_key src, struct esec_key_storage storage, uint32_t busprotbits,
                                 struct esec_eddsa_key* dst);

/** Delete a (protected) EdDSA key from eSecure **/
uint32_t esec_eddsa_delete_key(struct esec_eddsa_key key);

/** Generate an Ed25519 EdDSA signature for the message.
 *
 * The signature is a coordinate pair stored as the concatenation of two
 * times 32 bytes.
 **/
uint32_t esec_ed25519_sign(struct esec_eddsa_key key, const uint8_t* message, size_t len, uint8_t* signature);

/** Verify an Ed25519 EdDSA signature for the message.
 *
 * The signature is a coordinate pair stored as the concatenation of two
 * times 32 bytes.
 **/
uint32_t esec_ed25519_verify(struct esec_eddsa_key key, const uint8_t* message, size_t len, const uint8_t* signature);

/** Generate an Ed448 EdDSA signature for the message.
 *
 * The context is shared by the signer and verifier and has a length of
 * maximum of 255 bytes.
 *
 * The signature is a coordinate pair stored as the concatenation of two
 * times 57 bytes.
 **/
uint32_t esec_ed448_sign(struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len, const uint8_t* message,
                         size_t len, uint8_t* signature);

/** Verify an Ed448 EdDSA signature for the message.
 *
 * The context is shared by the signer and verifier and has a length of
 * maximum of 255 bytes.

 * The signature is a coordinate pair stored as the concatenation of two
 * times 57 bytes.
 **/
uint32_t esec_ed448_verify(struct esec_eddsa_key key, const uint8_t* ctx, size_t ctx_len, const uint8_t* message,
                           size_t len, const uint8_t* signature);

#endif
