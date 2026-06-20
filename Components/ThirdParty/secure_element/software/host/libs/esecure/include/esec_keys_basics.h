/**
 * @brief eSecure Library: basic representations for keys
 * @copyright Copyright (c) 2017-2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_KEY_BASICS_HEADER
#define ESEC_KEY_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

/* Internal defines. Don't use them directly. */
#define ESEC_KEY_TYPE_RAW     0x00000000
#define ESEC_KEY_TYPE_RSA     0x40000000
#define ESEC_KEY_TYPE_DSA     0x60000000
#define ESEC_KEY_TYPE_DH      0x70000000
#define ESEC_KEY_TYPE_ECC_W   0x80000000
#define ESEC_KEY_TYPE_ECC_ED  0xA0000000
#define ESEC_KEY_TYPE_ECC_MG  0xB0000000
#define ESEC_KEY_TYPE_EDDSA   0xC0000000
#define ESEC_KEY_TYPE_ECKCDSA 0xD0000000
#define ESEC_KEY_TYPE_MASK    0xF0000000

/* Mask to set symmetric key as AES WrapKey */
#define ESEC_KEY_AES_WRAP_MASK 0x00004000

/* This macro calculates Wrapped Key Lengths of KeyWrap Functions */
#define ESEC_AES_KEYWRAP_WRAPPED_LEN(key_len)  (((((key_len) + 7) / 8) * 8) + 8)
#define ESEC_ECDH_KEYWRAP_WRAPPED_LEN(key_len) ((key_len) + 64 + 8)
#define ESEC_RSA_KEYWRAP_WRAPPED_LEN(key_len)  ((key_len) + 128 + 8)

/** Asymetric key with a short generator of 4 bytes. **/
#define ESEC_KEY_SHORT_GENERATOR (1 << 11)

#define ESEC_STORAGE_FLAG_HOST       0
#define ESEC_STORAGE_FLAG_VOLATILE   1
#define ESEC_STORAGE_FLAG_WRAPPED    2
#define ESEC_STORAGE_FLAG_WRAPPED_AR 3

#define ESEC_STORAGE_INDEX(idx) (((idx)&0xFF) << 16)

/** Created key can be exported.
 *
 * The key can be exported with functions like esec_export_skey(),
 * esec_eddsa_export_key() or esec_ecc_export_key().
 *
 * This flag can be used in the flags argument of the key creation functions
 * like esec_create_skey(), esec_ecc_create_key() or esec_eddsa_create_key().
 **/
#define ESEC_KEY_EXPORTABLE (1 << 0)

/** Created key can only sign content from eSecure.
 *
 * This flag can be used in the flags argument of the key creation functions
 * esec_ecc_create_key() or esec_eddsa_create_key().
 *
 * This flag automatically implies that the key cannot be exported. Its
 * storage must be protected (volatile or wrapped with optionally anti-replay).
 **/
#define ESEC_KEY_ONLY_ESECURE (1 << 1)

/** Created key can only sign content or verify content from eSecure.
 *
 * This flag can be used in the flags argument of the key creation function
 * esec_ecc_create_key().
 **/
#define ESEC_KEY_PERMS_SIGN (1 << 10)

#ifndef ESEC_STOR_AUTH_SIZE
/** The size of the authentication for access to protected keys in bytes. */
#define ESEC_STOR_AUTH_SIZE 8
#endif

/** Where and how eSecure keys are stored.
 *
 * Initialized with ESEC_STORAGE_HOST(), ESEC_STORAGE_VOLATILE(),
 * ESEC_STORAGE_WRAP() or ESEC_STORAGE_WRAP_AR(). Should be used
 * as an opaque object.
 **/
struct esec_key_storage {
    uint32_t cfg;
#if ESEC_STOR_AUTH_SIZE > 0
    const uint8_t* auth;
#endif
    uint8_t* material;
};

/** Key stored as is by the host */
#define ESEC_STORAGE_HOST(buffer)                                                                                      \
    {                                                                                                                  \
        (ESEC_STORAGE_FLAG_HOST << 26), NULL, (uint8_t*)buffer                                                         \
    }

/** Key stored inside eSecure at a given index.
 *
 * This constant is put in the "storage" member of the key structures.
 **/
#define ESEC_STORAGE_VOLATILE(idx, auth)                                                                               \
    {                                                                                                                  \
        ((ESEC_STORAGE_FLAG_VOLATILE << 26) | ESEC_STORAGE_INDEX(idx)), auth, NULL                                     \
    }

/** Key protected by eSecure and stored on the host.
 *
 * The host has to provide storage space for the protected key. As eSecure adds
 * data to protect a key, the storage has to provide room for
 * ESEC_STOR_PROT_SIZE extra bytes.
 *
 * This constant is put in the "storage" member of the key structures.
 **/
#define ESEC_STORAGE_WRAPPED(buffer, auth)                                                                             \
    {                                                                                                                  \
        (ESEC_STORAGE_FLAG_WRAPPED << 26), auth, buffer                                                                \
    }

/** Key protected by eSecure and stored on the host with anti replay protection.
 *
 * The host has to provide storage space for the protected key. As eSecure adds
 * data to protect a key, the storage has to provide room for
 * ESEC_STOR_PROT_SIZE extra bytes.
 *
 * This constant is put in the "storage" member of the key structures.
 **/
#define ESEC_STORAGE_WRAPPED_AR(buffer, idx, auth)                                                                     \
    {                                                                                                                  \
        ((ESEC_STORAGE_FLAG_WRAPPED_AR << 26) | ESEC_STORAGE_INDEX(idx)), auth, buffer                                 \
    }

/** Amount of extra bytes for the IV added to protected keys. */
#define ESEC_STOR_PROT_IV_SIZE (96 / 8)

/** Amount of extra bytes added to protected keys. */
#define ESEC_STOR_PROT_SIZE (ESEC_STOR_PROT_IV_SIZE + 128 / 8)

/** The key material contain the public key.
 *
 * An asymmetric key must contain at least a public key, a private key or both.
 **/
#define ESEC_KEY_PUB (1 << 13)

/** The key material contain the private key.
 *
 * An asymmetric key must contain at least a public key, a private key or both.
 **/
#define ESEC_KEY_PRIV (1 << 14)

/** The key material contains both public and private keys.
 *
 * An asymmetric key must contain at least a public key, a private key or both.
 **/
#define ESEC_KEY_PAIR (ESEC_KEY_PRIV | ESEC_KEY_PUB)

/** Flag for the presence of domain parameters in the key material.
 *
 * Absence of the key domain flag means a predefined domain is being used.
 **/
#define ESEC_KEY_DOMAIN (1 << 12)

#endif
