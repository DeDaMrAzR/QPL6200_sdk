/**
 * @brief eSecure Library: EdDSA keys and crypto acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_EDDSA_BASICS_HEADER
#define ESEC_EDDSA_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"

#define ESEC_KEY_EDDSA(size)                (((size)-1) & 0x7F)
#define ESEC_KEY_EDDSA_SELCURVE(x)          (((x)&0x1) << 7)
#define ESEC_KEY_ECC_EDDSA_ED25519          (ESEC_KEY_EDDSA(32) | ESEC_KEY_EDDSA_SELCURVE(0))
#define ESEC_KEY_ECC_EDDSA_ED448            (ESEC_KEY_EDDSA(57) | ESEC_KEY_EDDSA_SELCURVE(0))
#define ESEC_KEY_ECC_EDDSA_TYPE_MASK        0x3FF
#define ESEC_KEY_ECC_EDDSA_ED25519_BYTESIZE 32
#define ESEC_KEY_ECC_EDDSA_ED448_BYTESIZE   57

/** eSecure EdDSA elliptic curve asymmetric key.
 *
 * Represents any kind of eSecure EdDSA key. The key can be protected,
 * volatile or unprotected. Those keys can be used with esec_ed25519_sign(),
 * esec_ed448_sign(), esec_ed25519_verify() and esec_ed448_verify().
 **/
struct esec_eddsa_key {
    struct esec_key_storage storage;
    uint32_t spec;
};

#endif
