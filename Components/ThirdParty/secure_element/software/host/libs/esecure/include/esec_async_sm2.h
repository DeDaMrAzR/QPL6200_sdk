/**
 * @brief eSecure Library: Asynchronous SM2 Ellyptic Curve crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 *
 * https://tools.ietf.org/html/draft-shen-sm2-ecdsa-02
 */

#ifndef ESEC_ASYNC_SM2_HEADER
#define ESEC_ASYNC_SM2_HEADER

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "esec_errors.h"
#include "esec_sm2_basics.h"

/** Generate asynchronously an SM2 elliptic curve signature for the message.
 *
 * The signature is a coordinate pair. The signature buffer should be large
 * enough to hold 2 big numbers. Each big number must be 4 bytes aligned.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm2_sign_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                          const uint8_t* message, size_t message_len, uint8_t* signature);

/** Verify asynchronously the SM2 elliptic curve signature for the given message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm2_verify_go(struct esec_mb* mb, struct esec_ecc_key key, const uint8_t* id_signer, size_t id_signer_len,
                            const uint8_t* message, size_t message_len, uint8_t* signature);

/** Generate asynchronously a random number and based on this random number,
 * a random point is computed.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm2_key_exch_rand_go(struct esec_mb* mb, struct esec_ecc_key key, uint8_t* random_number,
                                   uint8_t* random_point);

/** Generate asynchronously a shared secret key with SM2 from a pair of ECC keys.
 *
 * The SM2 key exchange process has 2 steps:
 * 1. Each party generates a random number and based on this random number,
 *    a random point.
 * 2. In order to generate a shared secret, each party combines its private
 *    key with the other public key, also using the random number/point
 *    computed at step 1 and the received random point.
 *
 * The first key (key1) contains the private key of the local user.
 * random_number_1 and random_point_1 are step 1 data computed locally
 *
 * The second key (key2) contains the public key received from the remote
 * party.
 * random_number_2 is part of step 1 computed remotely

 * That public key must be of the same type and domain as the first key
 * (key1). The second key (key2) must be stored unprotected in the memory
 * in the host.
 *
 * The generated shared key is stored in a buffer whose size must be twice
 * the size of the public keys. For SM2 P256_V1/V2, the shared key buffer needs
 * 64 bytes, for SM2 F257, the shared key buffer needs 66 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_sm2_key_exch_gen_go(struct esec_mb* mb, struct esec_ecc_key key1, const uint8_t* key2,
                                  const uint8_t* cofactor, uint8_t* random_point_2, uint8_t* random_point_1,
                                  uint8_t* random_number_1, struct esec_key_storage storage, int flags,
                                  struct esec_skey* sharedkey);

#endif
