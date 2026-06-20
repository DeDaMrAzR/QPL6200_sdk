/**
 * @file libs/base/include/esec_features.h
 * @brief This file summarizes the main functions to enable/disable for the build.
 * @copyright Copyright (c) 2016-2021 Silex Insight. All Rights reserved
 */

#ifndef ESEC_FEATURES_H
#define ESEC_FEATURES_H

/**
  Features definitions
 **/

#ifndef SECUREBOOT
#define SECUREBOOT 1
#endif

#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 0
#endif

#ifndef TEST_ENABLED
#define TEST_ENABLED 0
#endif

#ifndef CRYPTOLIB_TEST_ENABLED
#define CRYPTOLIB_TEST_ENABLED 0
#endif

#ifndef UART_DEBUG_ENABLED
#define UART_DEBUG_ENABLED 0
#endif

#define AUTH_ALGO_ED25519         2
#define AUTH_ALGO_ECDSA_P256      3
#define AUTH_ALGO_ECDSA_P384      4
#define AUTH_ALGO_ECDSA_P521      5
#define AUTH_ALGO_ECDSA_P521_SHA3 6
#define AUTH_ALGO_RSA_4K          7

#define SECCFG_STOR_OTP 100

#define EK_SRC_SECCFG 202

#include <stdint.h>
#include "esec_config.h"

#define VOLATILE_ENABLED ((VOLATILE_MAX_ID) > 0)

// Round ECC_MAX_KEY_SIZE_BITS to next multiple of 32 and convert in bytes
#define ECC_MAX_KEY_SIZE ((ECC_MAX_KEY_SIZE_BITS + 31) / 32 * 4)
/**
  Max key size computation, depends on enabled features and mex key sizes defined before.
 **/

struct sym_key_struct {
    uint8_t key[SYM_MAX_KEY_SIZE];
};

struct ecc_key_struct {
    uint8_t p[ECC_MAX_KEY_SIZE];
    uint8_t n[ECC_MAX_KEY_SIZE];
    uint8_t Gx[ECC_MAX_KEY_SIZE];
    uint8_t Gy[ECC_MAX_KEY_SIZE];
    uint8_t a[ECC_MAX_KEY_SIZE];
    uint8_t b[ECC_MAX_KEY_SIZE];
    uint8_t Px[ECC_MAX_KEY_SIZE];
    uint8_t Py[ECC_MAX_KEY_SIZE];
    uint8_t priv[ECC_MAX_KEY_SIZE];
};

struct dh_modp_key_struct {
    uint8_t p[DH_MAX_KEY_SIZE];
    uint8_t G[DH_MAX_KEY_SIZE];
    uint8_t pub[DH_MAX_KEY_SIZE];
    uint8_t priv[DH_MAX_KEY_SIZE];
};

struct dsa_key_struct {
    uint8_t p[DSA_MAX_SIZE_P];
    uint8_t q[DSA_MAX_SIZE_Q];
    uint8_t G[DSA_MAX_SIZE_P];
    uint8_t pub[DSA_MAX_SIZE_P];
    uint8_t priv[DSA_MAX_SIZE_Q];
};

struct rsa_nocrt_key_struct {
    uint8_t n[RSA_MAX_SIZE];
    uint8_t e[RSA_MAX_SIZE];
    uint8_t d[RSA_MAX_SIZE];
    uint8_t lambda[RSA_MAX_SIZE];
};

struct rsa_crt_key_struct {
    uint8_t n[RSA_MAX_SIZE];
    uint8_t e[RSA_MAX_SIZE];
    uint8_t p[RSA_MAX_SIZE / 2];
    uint8_t q[RSA_MAX_SIZE / 2];
    uint8_t dp[RSA_MAX_SIZE / 2];
    uint8_t dq[RSA_MAX_SIZE / 2];
    uint8_t qinv[RSA_MAX_SIZE / 2];
};

extern union Max_key_union {
    struct sym_key_struct sym;
    struct ecc_key_struct ecc;

#if DH_MODP_ENABLED
    struct dh_modp_key_struct dh;
#endif

#if DSA_ENABLED
    struct dsa_key_struct dsa;
#endif

#if RSA_ENABLED
    struct rsa_nocrt_key_struct rsa_nocrt;
    struct rsa_crt_key_struct rsa_crt;
#endif
} max_key_union;

#define MAX_KEY_SIZE (sizeof(max_key_union))

#define APB_REGION1_PROTBITS                                                                                           \
    ((!APB_REGION1_SECURE_ACCESS) << 31) | (APB_REGION1_PRIVILIGED_ACCESS << 30) | (APB_REGION1_USER_BITS)
#define APB_REGION2_PROTBITS                                                                                           \
    ((!APB_REGION2_SECURE_ACCESS) << 31) | (APB_REGION2_PRIVILIGED_ACCESS << 30) | (APB_REGION2_USER_BITS)
#define APB_REGION3_PROTBITS                                                                                           \
    ((!APB_REGION3_SECURE_ACCESS) << 31) | (APB_REGION3_PRIVILIGED_ACCESS << 30) | (APB_REGION3_USER_BITS)
#define APB_REGION4_PROTBITS                                                                                           \
    ((!APB_REGION4_SECURE_ACCESS) << 31) | (APB_REGION4_PRIVILIGED_ACCESS << 30) | (APB_REGION4_USER_BITS)

// Maximal Attestation size (in bytes) => Must be bigger than the biggest key size
#define MAX_ATTEST_SIZE roundup_32(MAX_KEY_SIZE + 104)

// Number of random bytes used for counter-measures
#if PK_CM_ENABLED
#define PK_CM_RAND_SIZE 8
#endif

// Disable counter-measures
#if !PK_CM_ENABLED
#undef PK_CM_RAND_PROJ
#undef PK_CM_RAND_SCALAR
#undef PK_CM_RAND_MODULUS
#undef PK_CM_RAND_EXPONENT
#endif

#endif
