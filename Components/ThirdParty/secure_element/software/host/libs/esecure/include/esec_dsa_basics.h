/**
 * @brief eSecure Library: DSA keys definition and helper
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_DSA_BASICS_HEADER
#define ESEC_DSA_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>
#include "esec_keys_basics.h"

/** DSA asymmetric key.
 *
 * Represents a DSA key. The key can be protected,
 * volatile or unprotected. It can contain a public, a private key
 * or a private public key pair.
 **/
struct esec_dsa_key {
    uint32_t keyspec;
    struct esec_key_storage storage;
};

/** A DSA key specification with given base sizes for p and q.
 *
 * The key base sizes must be multiple of 4 bytes. A DSA key
 * always contains the domain value (the modulus p, modulus q and generator G).
 **/
#define ESEC_KEY_DSA(PSIZE, QSIZE)                                                                                     \
    (ESEC_KEY_TYPE_DSA | ESEC_KEY_DOMAIN | ((((QSIZE)-4) << 5) & 0x780) | (((PSIZE)-4) >> 2))

#endif
