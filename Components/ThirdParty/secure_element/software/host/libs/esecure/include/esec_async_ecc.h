/**
 * @brief eSecure Library: Asynchronous Ellyptic Curve crypto acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_ECC_HEADER
#define ESEC_ASYNC_ECC_HEADER

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "esec_errors.h"
#include "esec_ecc_basics.h"

/** Generate asynchronously an elliptic curve signature for the message.
 *
 * The signature is a coordinate pair. The signature buffer should be large
 * enough to hold 2 big numbers. Each big number must be 4 byte alligned.
 *
 * All ECC keys except ED25519 can be used here. For ED25519, see
 * esec_ed25519_sign() and esec_ed25519_verify().
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecdsa_sign_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_ecc_key key,
                            const uint8_t* message, size_t len, uint8_t* signature);

/** Verify asynchronously the elliptic curve signature for the given message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecdsa_verify_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_ecc_key key,
                              const uint8_t* message, size_t len, const uint8_t* signature);

/** Create a ECC public and private key pair randomly.
 *
 * The domain should be either Weierstrass prime field or binary field,
 * or Edwards or Montgommery curve.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecc_create_key_go(struct esec_mb* mb, uint32_t domainspec, const uint8_t* domain,
                                struct esec_key_storage storage, int flags, struct esec_ecc_key* key);

/** Derive asynchronously an ECC public and private key pair from seed.
 *
 * The salt provides random data used to enhance the security of the derivation.
 * The domain should be either Weierstrass prime field or binary field,
 * or Edwards or Montgommery curve.
 *
 * The flags argument accepts ESEC_KEY_PERM_SIGN, ESEC_KEY_EXPORTABLE and
 * ESEC_KEY_ONLY_ESECURE
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_derive_ecc_key_from_seed_go(struct esec_mb* mb, const uint8_t* salt, size_t salt_sz, uint32_t domainspec,
                                          const uint8_t* domain, struct esec_key_storage storage, int flags,
                                          struct esec_ecc_key* key);

/** Import a ECC public, private or key pair into protected eSecure storage.
 *
 * The storage must be one of the eSecure protected storages: volatile, wrapped
 * or wrapped with anti-replay.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * The flags argument accepts ESEC_KEY_PERM_SIGN and ESEC_KEY_EXPORTABLE.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecc_import_key_go(struct esec_mb* mb, uint32_t domainspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_ecc_key* key);

/** Export key material from an eSecure (protected) ECC key.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecc_export_key_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* material);

/** Transform an ECC key into a new version with another protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_ecc_transfer_key_go(struct esec_mb* mb, struct esec_ecc_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_ecc_key* dst);

/** Read the public key from the eSecure ECC key.
 *
 * If the eSecure key does not contain the public key, it's recomputed.
 * If the key has custom domain parameters, those domain parameters will
 * be included in pubkey.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecc_read_pub_key_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* pubkey);

/** Delete a (protected) key from eSecure.
 *
 * Mostly useful for volatile keys and wrapped keys with anti replay.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecc_delete_key_go(struct esec_mb* mb, struct esec_ecc_key key);

/** Generate a shared secret key with Diffie-Hellman from a pair of ECC keys.
 *
 * The Diffie-Hellman process has 2 steps:
 *   1. Each party creates a public/private key pair. This can be done with the
 *      CreateKey command.
 *   2. After public key exchange, each party combines its private key with the
 *      other public key to generate a shared secret. That's the purpose of
 *      this command.
 *
 * The first key (key1) contains the private key of the local user.
 *
 * The second key (key2) contains the public key received from the remote
 * party. Of course, that public key must be of the same type and domain
 * as the first key (key1). The second key (key2) must be stored unprotected
 * in the memory in the host.
 *
 * The generated shared key is stored in a buffer whose size must be as large
 * as the (maximum) size of the public keys. For example, with ECC P256,
 * the shared key buffer needs 64 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_genkey_ecdh_go(struct esec_mb* mb, struct esec_ecc_key key1, const uint8_t* key2,
                             struct esec_key_storage storage, int flags, struct esec_skey* sharedkey);

/* Check if the domain is included with the key. Returns false if a predefined
 * domain is used.
 */
bool esec_ecc_is_domain_included(const struct esec_ecc_key* key);

/** Wrap a key asynchronously using ECDH
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_ecdh_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_ecc_key wrapping_key,
                               uint8_t* shared_data, uint32_t shared_data_len, uint32_t tmp_aes_key_len,
                               uint8_t* output, uint32_t out_size);

/** Unwrap a key asynchronously using ECDH
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_ecdh_key_unwrap_go(struct esec_mb* mb, struct esec_ecc_key wrapping_key, uint8_t* shared_data,
                                 uint32_t shared_data_len, uint32_t tmp_aes_key_len, uint8_t* wrapped_key,
                                 uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                 struct esec_key_storage storage, struct esec_skey* key);

/** Generate asynchronously an elliptic curve signature for the message digest.
 *
 * The signature is a coordinate pair. The signature buffer should be large
 * enough to hold 2 big numbers. Each big number must be 4 byte alligned.
 *
 * All ECC keys except ED25519 can be used here. For ED25519, see
 * esec_ed25519_sign() and esec_ed25519_verify().
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecdsa_sign_digest_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* digest, size_t len,
                                   uint8_t* signature);

/** Verify asynchronously the elliptic curve signature for the given message digest.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_ecdsa_verify_digest_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* digest, size_t len,
                                     const uint8_t* signature);

#endif
