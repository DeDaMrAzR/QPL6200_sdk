/**
 * @brief eSecure Library: SM2 Ellyptic Curve crypto key definition
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SM2_BASICS_HEADER
#define ESEC_SM2_BASICS_HEADER

#include <stdint.h>

#include "esec_keys_basics.h"
#include "esec_ecc_basics.h"

#define ESEC_KEY_SM2_P256_BYTE_SIZE 32
#define ESEC_KEY_SM2_F257_BYTE_SIZE 33

/** Weierstrass Prime SM2 P256 v1 predefined curve. */
#define ESEC_KEY_SM2_P256_V1 (ESEC_KEY_ECP(32) | ESEC_KEY_SELCURVE(1))

/** Weierstrass Prime SM2 P256 v2 predefined curve. */
#define ESEC_KEY_SM2_P256_V2 (ESEC_KEY_ECP(32) | ESEC_KEY_SELCURVE(2))

#endif
