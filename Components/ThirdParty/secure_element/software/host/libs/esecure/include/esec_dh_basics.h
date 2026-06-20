/**
 * @brief eSecure Library: Diffie-Hellman key definition and helper
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_DH_BASICS_HEADER
#define ESEC_DH_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>
#include "esec_keys_basics.h"

/** Diffie Hellman asymmetric key.
 *
 * Represents a Diffie Hellman key. The key can be protected,
 * volatile or unprotected. It can contain a public, private or
 * a private public key pair.
 **/
struct esec_dh_key {
    uint32_t keyspec;
    struct esec_key_storage storage;
};

/** A Diffie Hellman key specification with given base sizes
 *
 * The key base size must be multiple of 4 bytes. A Diffie Hellman key
 * always contains the domain values.
 **/
#define ESEC_KEY_DH(SIZE) (ESEC_KEY_TYPE_DH | ESEC_KEY_DOMAIN | (((SIZE)-4) >> 2))

#endif
