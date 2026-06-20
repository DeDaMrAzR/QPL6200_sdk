/**
 * @brief eSecure Library: SHA HMAC acceleration
 * @copyright Copyright (c) 2021 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_HMAC_HEADER
#define ESEC_HMAC_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_hash_basics.h"

/** Compute the HMAC signature of the message. */
uint32_t esec_hmac(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len, uint8_t* digest);

uint32_t esec_hmac_begin(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len,
                         uint8_t* state);

uint32_t esec_hmac_update(const uint16_t algo, const uint8_t* message, size_t len, const uint8_t* state,
                          uint8_t* state_new);

uint32_t esec_hmac_finish(const uint16_t algo, struct esec_skey skey, const uint8_t* message, size_t len,
                          const uint8_t* state, uint8_t* digest);

#endif
