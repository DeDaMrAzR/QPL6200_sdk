/**
 * @brief Asynchronous eSecure Library
 * @copyright Copyright (c) 2017-2021 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_HEADER
#define ESEC_ASYNC_HEADER

#include <stdbool.h>
#include <stdint.h>
#include "esec_mailbox_basics.h"
#include "../QPG62xx/platform.h" /* make sure the include doesn't accidentally take the platform.h from mbedtls */

#define UNUSED(x) (void)(x)

/** Command structure used in asynchronous command
 *
 * This structure should be used as an opaque structure and internal fields must
 * not be used directly.
 */
struct esec_mb {
    esec_chunk* pos;
    esec_chunk* output;
    esec_chunk descs[ESEC_MAX_DESCS];
};

#include "esec_async_hash.h"
#include "esec_async_symm.h"
#include "esec_async_aes.h"
#include "esec_async_chachapoly.h"
#include "esec_async_3des.h"
#include "esec_async_jpake.h"
#include "esec_async_ed25519.h"
#include "esec_async_ecc.h"
#include "esec_async_rsa.h"
#include "esec_async_dsa.h"
#include "esec_async_dh.h"
#include "esec_async_srp.h"
#include "esec_async_cert.h"
#include "esec_async_shake.h"
#include "esec_async_sm2.h"
#include "esec_async_sm4.h"
#include "esec_async_aria.h"
#include "esec_async_hmac.h"

/** Block until a response coming from the mailbox becomes available
 *
 * @return status of the issued command (like ESEC_OKAY or ESEC_INVALID_COMMAND)
 */
int esec_pop_cmd_response(void);

/**
 * Pops response from the mailbox if available or returns immediately
 *
 * @param response will contain the status code returned by esecure to a command
 * @return true if a response was available, false otherwise
 */
bool esec_pop_cmd_response_if_any(uint32_t* response);

/** Generate asynchronously random bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any*/
uint32_t esec_get_random_bytes_go(struct esec_mb* mb, uint8_t* bytes, size_t len);

/** Generate asynchronously a random prime number.
 *
 * The argument 'bits' gives the length in bits in the prime. The most
 * significant will be one. The 'rounds' argument specifies the number
 * of Miller-Rabin rounds to generate the prime number.
 *
 * The prime buffer size must be rounded up to a multiple of 32 bits.
 * Zero-padding is performed before the value.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_generate_prime_go(struct esec_mb* mb, uint8_t* prime, uint32_t bits, uint32_t rounds);

#endif
