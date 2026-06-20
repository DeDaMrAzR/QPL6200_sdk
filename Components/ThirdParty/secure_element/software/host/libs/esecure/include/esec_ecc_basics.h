/**
 * @brief eSecure Library: Ellyptic Curve crypto key definition
 * @copyright Copyright (c) 2017-2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ECC_BASICS_HEADER
#define ESEC_ECC_BASICS_HEADER

#include <stdint.h>

#include "esec_keys_basics.h"
#include "esec_ed25519.h"
#include "esec_symm.h"

/* internal flag for Weierstrass & EC-KCDSA keys */
#define ESEC_KEY_BINARY (1 << 11)
#define ESEC_KEY_PRIME  (0 << 11)

/* Used internally */
#define ESEC_KEY_SELCURVE(x) ((x) << 7)

/* Use acceleration with not predefined curve parameter */
#define ESEC_KEY_PARAM_A_IS_NULL    (1 << 9)
#define ESEC_KEY_PARAM_A_IS_MINUS_3 (1 << 8)

#define ESEC_KEY_ECP(SZ)  (ESEC_KEY_TYPE_ECC_W | ESEC_KEY_PRIME | (SZ - 1))
#define ESEC_KEY_ECB(SZ)  (ESEC_KEY_TYPE_ECC_W | ESEC_KEY_BINARY | (SZ - 1))
#define ESEC_KEY_ECMG(SZ) (ESEC_KEY_TYPE_ECC_MG | (SZ - 1))

#define ESEC_KEY_ECP_KCDSA(SZ) (ESEC_KEY_TYPE_ECKCDSA | ESEC_KEY_PRIME | (SZ - 1))
#define ESEC_KEY_ECB_KCDSA(SZ) (ESEC_KEY_TYPE_ECKCDSA | ESEC_KEY_BINARY | (SZ - 1))

#define MAX_KEY_ECC_SIZE 72

#define ESEC_KEY_ECC_P192_BYTE_SIZE   24
#define ESEC_KEY_ECC_P224_BYTE_SIZE   28
#define ESEC_KEY_ECC_P256_BYTE_SIZE   32
#define ESEC_KEY_ECC_P384_BYTE_SIZE   48
#define ESEC_KEY_ECC_P521_BYTE_SIZE   66
#define ESEC_KEY_ECC_B163_BYTE_SIZE   21
#define ESEC_KEY_ECC_B233_BYTE_SIZE   30
#define ESEC_KEY_ECC_B283_BYTE_SIZE   36
#define ESEC_KEY_ECC_B409_BYTE_SIZE   52
#define ESEC_KEY_ECC_B571_BYTE_SIZE   72
#define ESEC_KEY_ECC_X25519_BYTE_SIZE 32
#define ESEC_KEY_ECC_X448_BYTE_SIZE   56

#define ESEC_KEY_ECC_CURVE_NIST      0x00
#define ESEC_KEY_ECC_CURVE_BRAINPOOL 0x03

/** Weierstrass Prime P192 predefined curve. */
#define ESEC_KEY_ECC_P192 (ESEC_KEY_ECP(24) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Prime P224 predefined curve. */
#define ESEC_KEY_ECC_P224 (ESEC_KEY_ECP(28) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Prime P256 predefined curve. */
#define ESEC_KEY_ECC_P256 (ESEC_KEY_ECP(32) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Prime P384 predefined curve. */
#define ESEC_KEY_ECC_P384 (ESEC_KEY_ECP(48) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Prime P521 predefined curve. */
#define ESEC_KEY_ECC_P521 (ESEC_KEY_ECP(66) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Binary B163 predefined curve. */
#define ESEC_KEY_ECC_B163 (ESEC_KEY_ECB(21) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Binary B233 predefined curve. */
#define ESEC_KEY_ECC_B233 (ESEC_KEY_ECB(30) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Binary B283 predefined curve. */
#define ESEC_KEY_ECC_B283 (ESEC_KEY_ECB(36) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Binary B409 predefined curve. */
#define ESEC_KEY_ECC_B409 (ESEC_KEY_ECB(52) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Binary B571 predefined curve. */
#define ESEC_KEY_ECC_B571 (ESEC_KEY_ECB(72) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Montgomery Curve25519 predefined curve for DH key exchange. */
#define ESEC_KEY_ECC_X25519 (ESEC_KEY_ECMG(32) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Montgommery Curve448 predefined cure for DH key exchange. */
#define ESEC_KEY_ECC_X448 (ESEC_KEY_ECMG(56) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_NIST))

/** Weierstrass Prime Brainpool P160 predefined curve. */
#define ESEC_KEY_ECC_BP_P160 (ESEC_KEY_ECP(20) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P192 predefined curve. */
#define ESEC_KEY_ECC_BP_P192 (ESEC_KEY_ECP(24) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P224 predefined curve. */
#define ESEC_KEY_ECC_BP_P224 (ESEC_KEY_ECP(28) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P256 predefined curve. */
#define ESEC_KEY_ECC_BP_P256 (ESEC_KEY_ECP(32) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P384 predefined curve. */
#define ESEC_KEY_ECC_BP_P320 (ESEC_KEY_ECP(40) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P384 predefined curve. */
#define ESEC_KEY_ECC_BP_P384 (ESEC_KEY_ECP(48) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** Weierstrass Prime Brainpool P512 predefined curve. */
#define ESEC_KEY_ECC_BP_P512 (ESEC_KEY_ECP(64) | ESEC_KEY_SELCURVE(ESEC_KEY_ECC_CURVE_BRAINPOOL))

/** eSecure elliptic curve asymmetric key.
 *
 * Represents any kind of eSecure ECC key. The key can be protected,
 * volatile or unprotected.
 **/
struct esec_ecc_key {
    uint32_t keyspec;
    struct esec_key_storage storage;
};

#endif
