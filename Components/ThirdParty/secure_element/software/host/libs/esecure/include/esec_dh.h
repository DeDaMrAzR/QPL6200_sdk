/**
 * @brief eSecure Library: Diffie-Hellman crypto operation
 * @copyright Copyright (c) 2017-2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_DH_HEADER
#define ESEC_DH_HEADER

#include "esec_errors.h"
#include "esec_dh_basics.h"
#include "esec_symm.h"

/** Create a Diffie Hellman public and private key pair randomly. **/
uint32_t esec_dh_create_key(uint32_t domainspec, uint8_t* domain, struct esec_key_storage storage, int flags,
                            struct esec_dh_key* key);

/** Import a Diffie Hellman key into protected eSecure storage.
 *
 * The storage must be one of the eSecure protected storages: volatile, wrapped
 * or wrapped with anti-replay.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * The flags argument accepts ESEC_KEY_EXPORTABLE.
 **/
uint32_t esec_dh_import_key(uint32_t domainspec, const uint8_t* material, struct esec_key_storage storage, int flags,
                            struct esec_dh_key* key);

/** Export key material from an eSecure (protected) DSA key.
 *
 * The key material will be exported into the material buffer. That buffer
 * must be large enough to hold all material (domain, private, public) that
 * the key has.
 **/
uint32_t esec_dh_export_key(struct esec_dh_key key, uint8_t* material);

/** Read the public key from the eSecure DSA key.
 *
 * The domain parameters will be included in pubkey.
 **/
uint32_t esec_dh_read_pub_key(struct esec_dh_key key, uint8_t* pubkey);

/** Transform a DH key into a new version with another protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 */
uint32_t esec_dh_transfer_key(struct esec_dh_key src, struct esec_key_storage storage, uint32_t busprotbits,
                              struct esec_dh_key* dst);

/** Delete a (protected) key from eSecure.
 *
 * Mostly useful for volatile keys and wrapped keys with anti replay.
 **/
uint32_t esec_dh_delete_key(struct esec_dh_key key);

/** Compute a shared key with Diffie Hellman.
 *
 * The "remote_pubkey" and "sharedkey" buffers must have the same size as
 * the base length of "key".
 **/
uint32_t esec_dh_genkey(struct esec_dh_key key, uint8_t* remote_pubkey, struct esec_key_storage storage, int flags,
                        struct esec_skey* sharedkey);

#endif
