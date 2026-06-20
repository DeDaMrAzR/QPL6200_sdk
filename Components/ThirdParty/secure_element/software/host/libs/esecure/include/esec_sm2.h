/**
 * @brief eSecure Library: SM2 crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SM2_HEADER
#define ESEC_SM2_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_sm2_basics.h"

/** Generate an SM2 elliptic curve signature for the message.
 *
 * The signature is a coordinate pair. The signature buffer should be large
 * enough to hold 2 big numbers. Each big number must be 4 byte aligned.

 * The domain should be either Weierstrass prime field or binary field,
 * can be predefined for Prime and can be provided with the key for Prime or
 * for binary field.
 **/
uint32_t esec_sm2_sign(struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len, const uint8_t* message,
                       size_t message_len, uint8_t* signature);

/** Verify the SM2 elliptic curve signature for the given message.
 *
 * The domain should be either Weierstrass prime field or binary field,
 * can be predefined for Prime and can be provided with the key for Prime or
 * for binary field.
 **/
uint32_t esec_sm2_verify(struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                         const uint8_t* message, size_t message_len, uint8_t* signature);

/** Compute the SM2 random number and random point.
 *
 * The domain should be either Weierstrass prime field or binary field,
 * can be predefined for Prime and can be provided with the key for Prime or
 * for binary field.
 **/
uint32_t esec_sm2_key_exch_rand(struct esec_ecc_key key, uint8_t* random_number, uint8_t* random_point);

/** Generate a shared secret key with SM2 from a pair of ECC keys.
 *
 * The SM2 key exchange process has 2 steps:
 * 1. Each party generates a random number and based on this random number,
 *    a random point.
 * 2. In order to generate a shared secret, each party combines its private
 *    key with the other public key, also using the random number/point
 *    computed at step 1 and the received random point.
 *
 * The first key (key_private) contains the private key of the local user.
 * random_number_cal and random_point_cal are step 1 data computed locally
 *
 * The second key (key_public_rcv) contains the public key received from
 * the remote party.
 * random_number_rcv is part of step 1 computed on the remote party side

 * That public key must be of the same type and domain as the first key
 * (key_private). The second key (key_public_rcv) must be stored unprotected
 * in the memory in the host.
 *
 * The generated shared key is stored in a buffer whose size must be twice
 * the size of the public keys. For SM2 P256_V1/V2, the shared key buffer needs
 * 64 bytes, for SM2 F257, the shared key buffer needs 66 bytes.

 * The domain should be either Weierstrass prime field or binary field,
 * can be predefined for Prime and can be provided with the key for Prime or
 * for binary field.
 **/
uint32_t esec_sm2_key_exch_gen(struct esec_ecc_key key_private, const uint8_t* key_public_rcv, const uint8_t* cofactor,
                               uint8_t* random_point_rcv, uint8_t* random_point_cal, uint8_t* random_number_cal,
                               struct esec_key_storage storage, int flags, struct esec_skey* sharedkey);

#endif
