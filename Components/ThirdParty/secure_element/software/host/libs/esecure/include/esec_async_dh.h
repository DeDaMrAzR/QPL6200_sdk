/**
 * @brief eSecure Library: Diffie-Hellman asynchronous crypto operation
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_DH_HEADER
#define ESEC_ASYNC_DH_HEADER

#include "esec_errors.h"
#include "esec_dh_basics.h"
#include "esec_symm.h"

/** Create asynchronously a Diffie Hellman public and private key pair randomly.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dh_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* domain,
                               struct esec_key_storage storage, int flags, struct esec_dh_key* key);

/** Import asynchronously a Diffie Hellman key into protected eSecure storage.
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
uint32_t esec_dh_import_key_go(struct esec_mb* mb, uint32_t domainspec, const uint8_t* material,
                               struct esec_key_storage storage, int flags, struct esec_dh_key* key);

/** Export asynchronously aa key material from an eSecure (protected) DSA key.
 *
 * The key material will be exported into the material buffer. That buffer
 * must be large enough to hold all material (domain, private, public) that
 * the key has.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dh_export_key_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* material);

/** Read asynchronously the public key from the eSecure DSA key.
 *
 * The domain parameters will be included in pubkey.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dh_read_pub_key_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* pubkey);

/** Transform asynchronously a DH key into a new version with another
 *  protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_dh_transfer_key_go(struct esec_mb* mb, struct esec_dh_key src, struct esec_key_storage storage,
                                 uint32_t busprotbits, struct esec_dh_key* dst);

/** Delete asynchronously a (protected) key from eSecure.
 *
 * Mostly useful for volatile keys and wrapped keys with anti replay.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dh_delete_key_go(struct esec_mb* mb, struct esec_dh_key key);

/** Compute asynchronously a shared key with Diffie Hellman.
 *
 * The "remote_pubkey" and "sharedkey" buffers must have the same size as
 * the base length of "key".
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_dh_genkey_go(struct esec_mb* mb, struct esec_dh_key key, uint8_t* remote_pubkey,
                           struct esec_key_storage storage, int flags, struct esec_skey* sharedkey);

#endif
