/**
 * @brief eSecure Library: Asynchronous SHA hash acceleration
 * @copyright Copyright (c) 2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_SHA3_HEADER
#define ESEC_ASYNC_SHA3_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_shake_basics.h"

/**
 * @brief Compute asynchronously shake operation which is a SHA3 operation with
 *        variable length specififed by the user
 * @param algo which algo to use ESEC_HASH_ALGO_SHAKE_128 or ESEC_HASH_ALGO_SHAKE_256
 * @param msg message input to be hashed
 * @param len length of input data
 * @param digest output of hashing the message
 * @param digest_len desired output length
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_shake_hash_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len, uint8_t* digest,
                            size_t digest_len);

/**
 * @brief Start asynchronously a shake operation and output intermediate state
 *        for context switching
 * @param algo which algo to use ESEC_HASH_ALGO_SHAKE_128 or ESEC_HASH_ALGO_SHAKE_256
 * @param msg message input to start with
 * @param len length of input data
 * @param state_out intermediate state after absorbing the input , length will always
 *        be ESEC_SHA3_HASH_STATE_SIZE
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_shake_hash_begin_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len,
                                  uint8_t* state_out);

/**
 * @brief Update asynchronously a shake operation using a previous state with new
 *        data and output intermediate state for context switching
 * @param algo which algo to use ESEC_HASH_ALGO_SHAKE_128 or ESEC_HASH_ALGO_SHAKE_256
 * @param msg message input to be updated
 * @param len length of input data
 * @param state_in previous state to be updated length must always be
 *        ESEC_SHA3_HASH_STATE_SIZE
 * @param state_out new intermediate state after absorbing the new input, length
 *        will always be ESEC_SHA3_HASH_STATE_SIZE
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_shake_hash_update_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len,
                                   const uint8_t* state_in, uint8_t* state_out);

/**
 * @brief Finish asynchronously a shake operation using a previous state with
 *        new data and the final digest of the hash
 * @param algo which algo to use ESEC_HASH_ALGO_SHAKE_128 or ESEC_HASH_ALGO_SHAKE_256
 * @param msg message input to be updated
 * @param len length of input data
 * @param state_in previous state to be updated length must always be
 *        ESEC_SHA3_HASH_STATE_SIZE
 * @param digest final digest of the hash , length will be digest_len
 * @param digest_len desired output length to be squeezed
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_shake_hash_finish_go(struct esec_mb* mb, uint16_t algo, const uint8_t* msg, size_t len,
                                   const uint8_t* state_in, uint8_t* digest, size_t digest_len);

#endif
