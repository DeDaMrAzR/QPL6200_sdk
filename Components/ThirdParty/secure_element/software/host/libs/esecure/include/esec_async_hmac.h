/**
 * @brief eSecure Library: asynchronous SHA HMAC acceleration
 * @copyright Copyright (c) 2021 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_HMAC_HEADER
#define ESEC_ASYNC_HMAC_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_hash_basics.h"

/** Start asynchronously the partial HMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hmac_begin_go(struct esec_mb* mb, const uint16_t algo, struct esec_skey skey, const uint8_t* message,
                            size_t len, uint8_t* state);

/** Update asynchronously the partial HMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hmac_update_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* message, size_t len,
                             const uint8_t* state, uint8_t* state_new);

/** Complete asynchronously the partial HMAC signature of the message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hmac_finish_go(struct esec_mb* mb, const uint16_t algo, struct esec_skey skey, const uint8_t* message,
                             size_t len, const uint8_t* state, uint8_t* digest);

#endif
