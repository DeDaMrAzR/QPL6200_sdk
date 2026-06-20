/**
 * @brief eSecure Library: asynchronous SHA hash acceleration
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_HASH_HEADER
#define ESEC_ASYNC_HASH_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_hash_basics.h"

/** Compute asynchronously the digest on the full message.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hash_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* message, size_t len, uint8_t* digest);

/** Update asynchronously the hash state with a chunk of the message.
 *
 * The size of the message message chunk (lgiven by the 'len' argument) must be
 * a multiple of 64 bytes for SHA1, SHA224 and SHA256. For SHA384 and SHA512,
 * len must be a multiple of 128.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hash_update_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* state, const uint8_t* message,
                             size_t len, uint8_t* new_state);

/** Compute asynchronously the final raw hash digest.
 *
 * The raw hash digest is the untruncated hash state. For SHA224 and SHA384, it
 * means it contains respectively 4 and 16 bytes more than the digest. The raw
 * hash needs to be truncated accordingly. For the other hash algorithms, the
 * no truncation is needed.
 *
 * total_len represents the total amount of bytes that were hashed, including
 * the bytes hashed with esec_hash_update(). len gives the amount of bytes to
 * hash now. As total_len is the sum of the previous hashed blocks and len,
 * the difference (total_len - len) must be a multiple of 64 bytes for SHA1,
 * SHA224 and SHA256. For SHA384 and SHA512, it must be a multiple of 128.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_hash_finish_go(struct esec_mb* mb, const uint16_t algo, const uint8_t* state, const uint8_t* message,
                             size_t len, size_t total_len, uint8_t* raw_digest);

#endif
