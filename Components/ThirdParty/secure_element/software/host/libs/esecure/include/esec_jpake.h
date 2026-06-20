/**
 * @brief eSecure Library: Elliptic Curve JPAKE
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_JPAKE_HEADER
#define ESEC_JPAKE_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_jpake_basics.h"

/** Generate two points with proofs for round 1 of EC J-PAKE.
 *
 * Generate two points X1 and X2 for a given user and the corresponding
 * Zero-Knowledge Proofs (zkp1 & zkp2), as well as the random scalar x2.
 * The points like X or X2 contain 2 values, the zero knowledge proofs contains
 * 3 values. The random number is one value. The size of a value is 32 bytes
 * as the hardware implementation only supports ECC P256.
 *
 * The password, user_id and remote_user_id must be smaller than the domain.
 **/
uint32_t esec_ecjpake_generate_r1(const struct esec_jpake_cfg* cfg, uint8_t* x2, uint8_t* X1, uint8_t* zkp1,
                                  uint8_t* X2, uint8_t* zkp2);

/** Verify two points with their proofs for round 1 of EC J-PAKE.
 *
 * Verify the received two points X3 and X4 for a given user and the
 * corresponding Zero-Knowledge Proofs (zkp3 & zkp4).
 * The points like X3 contain 2 values, the zero knowledge proofs contains 3
 * values. The size of a value is 32 bytes as the hardware implementation
 * only supports ECC P256.
 *
 * The password, user_id and remote_user_id must be smaller than the domain.
 *
 * See also esec_ecjpake_generate_r1()
 **/
uint32_t esec_ecjpake_verify_r1(const struct esec_jpake_cfg* cfg, const uint8_t* X3, const uint8_t* zkp3,
                                const uint8_t* X4, const uint8_t* zkp4);

/** Generate a point with proof for round 2 of EC J-PAKE.
 *
 * Generate point A and the corresponding Zero-Knowledge Proofs (zkpA) based on
 * the random scalar and generated points and received points from round 1 of
 * EC J-PAKE.
 *
 * See also esec_ecjpake_generate_r1()
 *
 * The hardware implementation only supports ECC P256 curves for now.
 *
 * The password, user_id and remote_user_id must be smaller than the domain.
 **/
uint32_t esec_ecjpake_generate_r2(const struct esec_jpake_cfg* cfg, const uint8_t* pwd, size_t pwdlen,
                                  const uint8_t* x2, const uint8_t* X1, const uint8_t* X3, const uint8_t* X4,
                                  uint8_t* A, uint8_t* zkpA);

/** Verify a point with proof for round 2 of EC J-PAKE.
 *
 * Verify the received point B and the corresponding Zero-Knowledge Proofs
 * (zkpB) with the points and values from round 1.
 *
 * See also esec_ecjpake_generate_r1() and esec_ecjpake_generate_r2()
 *
 * The hardware implementation only supports ECC P256 curves for now.
 *
 * The password, user_id and remote_user_id must be smaller than the domain.
 **/
uint32_t esec_ecjpake_verify_r2(const struct esec_jpake_cfg* cfg, uint8_t* x2, const uint8_t* X1, const uint8_t* X2,
                                const uint8_t* X3, const uint8_t* B, const uint8_t* zkpB);

/** Generate a shared session key based on data from round 1 and 2 of EC J-PAKE
 *
 * The generated session key has the size of the output hash algorithm used.
 * In this case, it's 32 bytes.
 **/
uint32_t esec_ecjpake_generate_sessionkey(const struct esec_jpake_cfg* cfg, const uint8_t* pwd, size_t pwdlen,
                                          const uint8_t* x2, const uint8_t* X4, const uint8_t* B, uint8_t* session_key);

#endif
