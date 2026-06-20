/**
 * @brief eSecure Library: Triple DES asynchronous crypto acceleration
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_3DES_HEADER
#define ESEC_ASYNC_3DES_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_symm_basics.h"

/** Encrypt asynchronously the plaintext with triple DES in ECB cipher mode
 *
 * The symmetric key must have 24 bytes (3 times 8 bytes).
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_3des_ecb_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* plaintext, size_t len,
                                  uint8_t* ciphertext);

/** Decrypt asynchronously the plaintext with triple DES in ECB cipher mode
 *
 * The symmetric key must have 24 bytes (3 times 8 bytes)
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_3des_ecb_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t* ciphertext, size_t len,
                                  uint8_t* plaintext);

/** Encrypt asynchronously the plaintext with triple DES in CBC cipher mode
 *
 * The symmetric key must have 24 bytes (3 times 8 bytes).
 * The IV has 8 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_3des_cbc_encrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8],
                                  const uint8_t* plaintext, size_t len, uint8_t* ciphertext);

/** Decrypt asynchronously the plaintext with triple DES in CBC cipher mode
 *
 * The symmetric key must have 24 bytes (3 times 8 bytes).
 * The IV has 8 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_3des_cbc_decrypt_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8],
                                  const uint8_t* ciphertext, size_t len, uint8_t* plaintext);

/** Compute asynchronously the CBC-MAC with triple DES of a message
 *
 * The symmetric key must have 24 bytes (3 times 8 bytes).
 * The IV has 8 bytes. The computed MAC will have 8 bytes.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_3des_cbc_mac_go(struct esec_mb* mb, struct esec_skey skey, const uint8_t iv[8], const uint8_t* message,
                              size_t len, uint8_t mac[8]);

#endif
