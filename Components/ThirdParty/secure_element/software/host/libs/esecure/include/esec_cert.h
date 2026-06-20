/**
 * @brief eSecure Library: eSecure certification
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_CERT_HEADER
#define ESEC_CERT_HEADER

#include <stdint.h>
#include <stddef.h>
#include "esec_cert_basics.h"

/** Create a certificate for a ECC key with a certifier signature.
 *
 * The size of the certificate is ESEC_CERT_KEY_BASE_SIZE plus the size
 * of the public key and when applicable domain parameters.
 **/
uint32_t esec_certify_ecc_key(esec_certifier_key signer, const uint8_t nonce[12], struct esec_ecc_key target_key,
                              uint8_t* cert);

/** Create a certificate for an EdDSA key with a certifier signature. **/
uint32_t esec_certify_eddsa_key(esec_certifier_key signer, const uint8_t nonce[12], struct esec_eddsa_key target_key,
                                struct esec_cert_ed25519_key* cert);

/** Create a signed certificate with the time read from eSecure RTC. **/
uint32_t esec_certify_time(esec_certifier_key key, const uint8_t nonce[12], struct esec_time_certificate* cert);

/** Read the chip manufaturer certificate.
 *
 * This certificate contains the public part of the chip unique endorsement key.
 **/
uint32_t esec_read_chip_certificate(struct esec_chip_certificate* cert);

/** Read the eSecure Entity Attestation Token.
 *
 **/
uint32_t esec_get_attestation_token(uint8_t* challenge, uint32_t challenge_size, uint8_t* token, uint32_t token_size,
                                    uint32_t* token_len, int32_t* attest_status);

/** Read the eSecure Entity Attestation Token Length
 *
 **/
uint32_t esec_get_attestation_token_len(uint32_t challenge_size, uint32_t* token_len, int32_t* attest_status);

#endif
