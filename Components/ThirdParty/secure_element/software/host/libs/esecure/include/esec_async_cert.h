/**
 * @brief eSecure Library: asynchronous eSecure certification
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_CERT_HEADER
#define ESEC_ASYNC_CERT_HEADER

#include <stdint.h>
#include <stddef.h>
#include "esec_cert_basics.h"

/** Create asynchronously a certificate for a ECC key with a certifier signature.
 *
 * The size of the certificate is ESEC_CERT_KEY_BASE_SIZE plus the size
 * of the public key and when applicable domain parameters.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_certify_ecc_key_go(struct esec_mb* mb, esec_certifier_key signer, const uint8_t nonce[12],
                                 struct esec_ecc_key target_key, uint8_t* cert);

/** Create asynchronously a certificate for an EdDSA key with a certifier signature.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_certify_eddsa_key_go(struct esec_mb* mb, esec_certifier_key signer, const uint8_t nonce[12],
                                   struct esec_eddsa_key target_key, struct esec_cert_ed25519_key* cert);

/** Create asynchronously a signed certificate with the time read from
 *  eSecure RTC.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_certify_time_go(struct esec_mb* mb, esec_certifier_key key, const uint8_t nonce[12],
                              struct esec_time_certificate* cert);

/** Read asynchronously the chip manufaturer certificate.
 *
 * This certificate contains the public part of the chip unique endorsement key.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_read_chip_certificate_go(struct esec_mb* mb, struct esec_chip_certificate* cert);

/** Read asynchronously the eSecure Entity Attestation Token.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_get_attestation_token_go(struct esec_mb* mb, uint8_t* challenge, uint32_t challenge_size, uint8_t* token,
                                       uint32_t token_size, uint32_t* token_len, int32_t* attest_status);

/** Read asynchronously the eSecure Entity Attestation Token LEngth
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_get_attestation_token_len_go(struct esec_mb* mb, uint32_t challenge_size, uint32_t* token_size,
                                           int32_t* attest_status);

#endif
