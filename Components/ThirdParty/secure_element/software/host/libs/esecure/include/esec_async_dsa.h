/**
 * @brief eSecure Library: Asynchronous DSA keys and cryptographic functions
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_DSA_HEADER
#define ESEC_ASYNC_DSA_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_dsa_basics.h"
#include "esec_hash_basics.h"

/** Create a DSA public and private key pair randomly.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* domain,
                                struct esec_key_storage storage, int flags, struct esec_dsa_key* key);

/** Import a DSA public, private or key pair into protected eSecure storage.
 *
 * The storage must be one of the eSecure protected storages: volatile, wrapped
 * or wrapped with anti-replay.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * The flags argument accepts ESEC_KEY_EXPORTABLE.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_import_key_go(struct esec_mb* mb, uint32_t domainspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_dsa_key* key);

/** Export key material from an eSecure (protected) DSA key.
 *
 * The key material will be exported into the material buffer. That buffer
 * must be large enough to hold all material (domain, private, public) that
 * the key has.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_export_key_go(struct esec_mb* mb, struct esec_dsa_key key, uint8_t* material);

/** Read the public key from the eSecure DSA key.
 *
 * The domain parameters will be included in pubkey.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_read_pub_key_go(struct esec_mb* mb, struct esec_dsa_key key, uint8_t* pubkey);

/** Transform a DSA key into a new version with another protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_dsa_transfer_key_go(struct esec_mb* mb, struct esec_dsa_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_dsa_key* dst);

/** Delete a (protected) key from eSecure.
 *
 * Mostly useful for volatile keys and wrapped keys with anti replay.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_delete_key_go(struct esec_mb* mb, struct esec_dsa_key key);

/** Generate a DSA signature for the message.
 *
 * The signature is a pair of big numbers modulo q. The signature buffer
 * must be large enough to hold twice the key size of q.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_sign_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_dsa_key key, const uint8_t* message,
                          size_t len, uint8_t* signature);

/** Verify the DSA signature for the given message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_verify_go(struct esec_mb* mb, const uint16_t hash_alg, struct esec_dsa_key key,
                            const uint8_t* message, size_t len, const uint8_t* signature);

/** Generate a DSA signature for the message digest.
 *
 * The signature is a pair of big numbers modulo q. The signature buffer
 * must be large enough to hold twice the key size of q.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_sign_digest_go(struct esec_mb* mb, struct esec_dsa_key key, const uint8_t* digest, size_t len,
                                 uint8_t* signature);

/** Verify the DSA signature for the given message digest.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dsa_verify_digest_go(struct esec_mb* mb, struct esec_dsa_key key, const uint8_t* digest, size_t len,
                                   const uint8_t* signature);

#endif
