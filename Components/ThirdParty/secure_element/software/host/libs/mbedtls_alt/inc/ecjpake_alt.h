/**
 * \file ecjpake.h
 *
 * \brief Elliptic curve J-PAKE
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef MBEDTLS_ECJPAKE_ALT_H
#define MBEDTLS_ECJPAKE_ALT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * EC J-PAKE context structure.
 *
 * J-PAKE is a symmetric protocol, except for the identifiers used in
 * Zero-Knowledge Proofs, and the serialization of the second message
 * (KeyExchange) as defined by the Thread spec.
 *
 * In order to benefit from this symmetry, we choose a different naming
 * convention from the Thread v1.0 spec. Correspondence is indicated in the
 * description as a pair C: client name, S: server name
 */
typedef struct mbedtls_ecjpake_context
{
    mbedtls_md_type_t MBEDTLS_PRIVATE(md_type);          /**< Hash to use                    */
    mbedtls_ecp_group MBEDTLS_PRIVATE(grp);              /**< Elliptic curve                 */
    mbedtls_ecjpake_role MBEDTLS_PRIVATE(role);          /**< Are we client or server?       */
    int MBEDTLS_PRIVATE(point_format);                   /**< Format for point export        */

    mbedtls_ecp_point MBEDTLS_PRIVATE(Xm1);              /**< My public key 1   C: X1, S: X3 */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Xm2);              /**< My public key 2   C: X2, S: X4 */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Xp1);              /**< Peer public key 1 C: X3, S: X1 */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Xp2);              /**< Peer public key 2 C: X4, S: X2 */
    mbedtls_ecp_point MBEDTLS_PRIVATE(Xp);               /**< Peer public key   C: Xs, S: Xc */

    mbedtls_mpi MBEDTLS_PRIVATE(xm1);                    /**< My private key 1  C: x1, S: x3 */
    mbedtls_mpi MBEDTLS_PRIVATE(xm2);                    /**< My private key 2  C: x2, S: x4 */

    mbedtls_mpi MBEDTLS_PRIVATE(s);                      /**< Pre-shared secret (passphrase) */
} mbedtls_ecjpake_context;

#if defined(MBEDTLS_SELF_TEST)

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if a test failed
 */
int mbedtls_ecjpake_self_test( int verbose );

#endif /* MBEDTLS_SELF_TEST */

#ifdef __cplusplus
}
#endif


#endif /* ecjpake.h */
