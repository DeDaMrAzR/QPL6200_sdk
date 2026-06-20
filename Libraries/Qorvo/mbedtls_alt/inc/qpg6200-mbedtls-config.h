/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 *
 */

#pragma once

#include "common-mbedtls-config.h"

#define MBEDTLS_CCM_ALT
#define MBEDTLS_ECDSA_VERIFY_ALT
#define MBEDTLS_ECDSA_SIGN_ALT
#define MBEDTLS_ECJPAKE_ALT
#define MBEDTLS_ECP_ALT
#define MBEDTLS_SHA256_ALT
#define MBEDTLS_AES_ALT

#define MBEDTLS_ENTROPY_HARDWARE_ALT

#ifdef MBEDTLS_MPI_MAX_SIZE
#undef MBEDTLS_MPI_MAX_SIZE
#endif
#define MBEDTLS_MPI_MAX_SIZE 64 /**< Maximum number of bytes for usable MPIs. */
#undef MBEDTLS_ECP_WINDOW_SIZE
#undef MBEDTLS_ECP_FIXED_POINT_OPTIM

// Enable TLS 1.3
#if QORVO_MBEDTLS_TLS1_3_ENABLE
#define MBEDTLS_SSL_PROTO_TLS1_3
#define MBEDTLS_SSL_TLS1_3_COMPATIBILITY_MODE
#define MBEDTLS_PSA_CRYPTO_C
#define MBEDTLS_PSA_CRYPTO_CLIENT
#define MBEDTLS_SSL_KEEP_PEER_CERTIFICATE
#define MBEDTLS_SSL_SESSION_TICKETS
#define MBEDTLS_SSL_TLS1_3_DEFAULT_NEW_SESSION_TICKETS 1
#define MBEDTLS_SSL_TLS1_3_TICKET_NONCE_LENGTH         32
#define PSA_WANT_ALG_HKDF_EXTRACT                      1
#define PSA_WANT_ALG_HKDF_EXPAND                       1
#define PSA_WANT_ALG_SHA_256                           1
#endif // QORVO_MBEDTLS_TLS1_3_ENABLE

#ifdef PSA_CRYPTO_IMPLEMENTED
#define MBEDTLS_PSA_CRYPTO_C
#endif

#include "qorvo-mbedtls-config-check.h"
