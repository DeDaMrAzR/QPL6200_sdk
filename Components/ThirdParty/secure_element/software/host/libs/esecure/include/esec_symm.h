/**
 * @brief eSecure Library: Symmetric keys
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SYMM_HEADER
#define ESEC_SYMM_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_symm_basics.h"

/** Create a symmetric key randomly.
 *
 * The len argument specifies size of the created key.
 *
 * For exportable keys, add the ESEC_KEY_EXPORTABLE. Otherwise leave it to 0.
 * No other flags are allowed.
 **/
uint32_t esec_create_skey(uint16_t len, struct esec_key_storage storage, int flags, struct esec_skey* skey);

/** Derive a symmetric key from seed
 *
 * The salt provides random data used to enhance the security of the derivation.
 * The len argument specifies size of the derived key.
 *
 * For exportable keys, add the ESEC_KEY_EXPORTABLE. Otherwise leave it to 0.
 * No other flags are allowed.
 *
 **/
uint32_t esec_derive_skey_from_seed(const uint8_t* salt, size_t salt_sz, uint16_t len, struct esec_key_storage storage,
                                    int flags, struct esec_skey* skey);

/** Derive a symmetric key from another symmetric key with HKDF
 *
 * ikm is the input key material.
 * hash_alg defines which hash algorithm should be used to derive the key.
 * salt provides random data used to enhance the security of the derivation.
 * info can be used to create multiple versions from the same input key.
 * len gives the desired size for the new derived output key stored in okm.
 * okm holds the newly derived key
 **/
uint32_t esec_derive_hkdf_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt, size_t salt_sz,
                               const uint8_t* info, size_t info_sz, struct esec_key_storage storage, uint32_t len,
                               struct esec_skey* okm);

/** Derive a symmetric key from another symmetric key with PBKDF2
 *
 * ikm is the input password
 * hash_alg defines which hash algorithm should be used to derive the key.
 * salt provides random data used to enhance the security of the derivation.
 * iterations defines how many derivation iterations should be done.
 * len gives the desired size for the new derived output key stored in okm.
 * okm holds the newly derived key
 **/
uint32_t esec_derive_pbkdf2_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* salt, size_t salt_sz,
                                 uint32_t iterations, struct esec_key_storage storage, uint32_t len,
                                 struct esec_skey* okm);

/**
 * KDF2 key derivation function defined by IEEE 1363a-2004 and ANSI-X9.63-KDF.
 *
 * ikm is the input key (shared secret).
 * hash_alg defines which hash algorithm should be used to derive the key.
 * param is the shared parameter string.
 * param_sz gives the size of the shared parameter string.
 * storage specifies the eSecure storage for okm.
 * len gives the desired size for the new derived output key stored in okm.
 * okm holds the newly derived key
 */
uint32_t esec_derive_kdf2_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param, size_t param_sz,
                               struct esec_key_storage storage, uint32_t len, struct esec_skey* okm);

/**
 * Key derivation function defined by NIST 800-108.
 *
 * ikm is the input key (shared secret).
 * hash_alg defines which hash algorithm should be used to derive the key.
 * param is the shared parameter string.
 * param_sz gives the size of the shared parameter string.
 * storage specifies the eSecure storage for okm.
 * len gives the desired size for the new derived output key stored in okm.
 * okm holds the newly derived key
 */
uint32_t esec_derive_nist_skey(struct esec_skey ikm, uint32_t hash_alg, const uint8_t* param, size_t param_sz,
                               struct esec_key_storage storage, uint32_t len, struct esec_skey* okm);

/** Import a symmetric key into a esec_skey.
 *
 * The storage can be volatile, wrapped or wrapped with anti-replay.
 *
 * The flags argument accepts ESEC_KEY_EXPORTABLE or 0.
 **/
uint32_t esec_import_skey(const uint8_t* material, uint16_t klen, struct esec_key_storage storage, int flags,
                          struct esec_skey* skey);

/** Export a symmetric key from a esec_skey. **/
uint32_t esec_export_skey(struct esec_skey skey, uint8_t* material);

/** Hardware export of esec_skey to external IP core (through APB master bus).
 *
 * The storage can be volatile, wrapped or wrapped with anti-replay. The key
 * size must be aligned on 32 bit and be least 16 bytes.
 *
 * The APB bus address offset must be aligned on the key size (or its upper
 * power of 2).
 **/
uint32_t esec_hw_export_skey(struct esec_skey skey, uint32_t offset);

/** Transform a symmetric key into a new version with another protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 */
uint32_t esec_transfer_skey(struct esec_skey src, struct esec_key_storage storage, uint32_t busprotbits,
                            struct esec_skey* dst);

/** Delete a symmetric key from eSecure
 *
 * Mostly useful for volatile and wrapped keys with anti replay protectetion.
 **/
uint32_t esec_delete_skey(struct esec_skey key);

#endif
