/**
 * @brief eSecure Library: asynchronous RSA cryptographic functions
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_RSA_HEADER
#define ESEC_ASYNC_RSA_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"
#include "esec_hash.h" //TODO Change to async
#include "esec_rsa_basics.h"

/** Create asynchronously a RSA public/private key pair with true random bits.
 *
 * The argument domainspec specifies the RSA key size. If domainspec has
 * ESEC_KEY_SHORT_EXP, the public key has only 4 bytes. The generated key
 * always contains a key pair.
 *
 * The flags argument accepts ESEC_KEY_PERM_SIGN and ESEC_KEY_EXPORTABLE.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_create_key_go(struct esec_mb* mb, uint32_t domainspec, uint8_t* pub_key,
                                struct esec_key_storage storage, int flags, struct esec_rsa_key* key);

/** Derive asynchronously a RSA public/private key from seed.
 *
 * The salt provides random data used to enhance the security of the derivation.
 * The argument domainspec specifies the RSA key size. If domainspec has
 * ESEC_KEY_SHORT_EXP, the public key has only 4 bytes. The derived key
 * always contains a key pair.
 *
 * The flags argument accepts ESEC_KEY_PERM_SIGN and ESEC_KEY_EXPORTABLE and
 * ESEC_KEY_ONLY_ESECURE
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_derive_rsa_key_from_seed_go(struct esec_mb* mb, const uint8_t* salt, size_t salt_sz, uint32_t domainspec,
                                          uint8_t* pub_key, struct esec_key_storage storage, int flags,
                                          struct esec_rsa_key* key);

/** Asynchronous import of a RSA public, private or key pair into
 *  protected eSecure storage.
 *
 * The storage must be one of the eSecure protected storages: volatile, wrapped
 * or wrapped with anti-replay.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * The flags argument accepts ESEC_KEY_PERM_SIGN and ESEC_KEY_EXPORTABLE.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_import_key_go(struct esec_mb* mb, uint32_t keyspec, const uint8_t* material,
                                struct esec_key_storage storage, int flags, struct esec_rsa_key* key);

/** Asynchronous export of a key material from an eSecure (protected) RSA key.
 *
 * The argument domainspec tells if the wrapped key material contains a public
 * key, a private key or both (ESEC_KEY_PUB, ESEC_KEY_PRIV or
 * ESEC_KEY_PAIR). It may not be empty.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_export_key_go(struct esec_mb* mb, struct esec_rsa_key key, uint8_t* material);

/** Asynchronous read of the public key from the eSecure RSA key.
 *
 * The key domain parameters will be included in pubkey.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_read_pub_key_go(struct esec_mb* mb, struct esec_rsa_key key, uint8_t* pubkey);

/** Transform asynchronously a RSA key into a new version with another
 *  protected storage
 *
 * The material from the 'src' protected key is stored in a new protected key
 * called 'dst' with the given 'storage' and the new bus protection bits
 * (busprotbits).
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_rsa_transfer_key_go(struct esec_mb* mb, struct esec_rsa_key src, struct esec_key_storage storage,
                                  uint32_t busprotbits, struct esec_rsa_key* dst);

/** Delete asynchronously a (protected) key from eSecure.
 *
 * Mostly useful for volatile keys and wrapped keys with anti replay.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_delete_key_go(struct esec_mb* mb, struct esec_rsa_key key);

/** Generate asynchronously a RSA signature for the message.
 *
 * The signature is one big number value. The signature buffer should be large
 * enough to hold 1 big numbers as defined by the key base length.
 *
 * Padding can be any of ESEC_RSA_PADDING_NONE, ESEC_RSA_PADDING_EMSA_PKCS or
 * ESEC_RSA_PADDING_PSS.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_sign_go(struct esec_mb* mb, uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key,
                          const uint8_t* message, size_t len, uint8_t* signature, size_t salt_len);

/** Verify asynchronously the RSA signature for the given message.
 *
 * Padding can be any of ESEC_RSA_PADDING_NONE, ESEC_RSA_PADDING_EMSA_PKCS or
 * ESEC_RSA_PADDING_PSS.
 *
 * salt_len is a parameter used only in case of padding type ESEC_RSA_PADDING_PSS
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_verify_go(struct esec_mb* mb, uint8_t padding, uint8_t hash_alg, struct esec_rsa_key key,
                            const uint8_t* message, size_t len, const uint8_t* signature, size_t salt_len);

/** Encrypt asynchronously plaintext with RSA.
 *
 * Padding can be any of ESEC_RSA_PADDING_NONE, ESEC_RSA_PADDING_EME_PKCS or
 * ESEC_RSA_PADDING_OAEP.
 *
 * The plaintext is interpreted as a big endian value. It must be smaller
 * than the modulo from the RSA key. The argument len gives its size in bytes.
 *
 * The size of the ciphertext buffers must match the RSA key numbers size.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_encrypt_go(struct esec_mb* mb, uint8_t padding, struct esec_rsa_key key, const uint8_t* plaintext,
                             size_t len, uint8_t* ciphertext, uint8_t hash_algo);

/** Decrypt asynchronously a ciphertext with RSA.
 *
 * Padding can be any of ESEC_RSA_PADDING_NONE, ESEC_RSA_PADDING_EME_PKCS or
 * ESEC_RSA_PADDING_OAEP.
 *
 * The lenght of the decrypted message will be stored in decrypted_len.
 * The size of the ciphertext and plaintext buffers must match the
 * RSA key numbers size.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_decrypt_go(struct esec_mb* mb, uint8_t padding, struct esec_rsa_key key, const uint8_t* ciphertext,
                             uint8_t* plaintext, uint32_t* decrypted_len, uint8_t hash_algo);

/** Wrap a key asynchronously using RSA
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_rsa_key_wrap_go(struct esec_mb* mb, struct esec_skey target_key, struct esec_rsa_key wrapping_key,
                              uint32_t tmp_aes_key_len, uint8_t* output, uint32_t out_size);

/** Unwrap a key asynchronously using RSA
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 */
uint32_t esec_rsa_key_unwrap_go(struct esec_mb* mb, struct esec_rsa_key wrapping_key, uint8_t* wrapped_key,
                                uint32_t wrapped_key_len, uint32_t flags, uint16_t klen,
                                struct esec_key_storage storage, struct esec_skey* key);

/** Generate asynchronously a RSA signature for the message digest.
 *
 * The signature is one big number value. The signature buffer should be large
 * enough to hold 1 big numbers as defined by the key base length.
 *
 * If using a padding scheme, it should be applied on the digest before using this function
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_sign_digest_go(struct esec_mb* mb, struct esec_rsa_key key, const uint8_t* digest, size_t len,
                                 uint8_t* signature);

/** Verify asynchronously the RSA signature for the given message digest.
 *
 * If using a padding scheme, it should be applied on the digest before using this function
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_rsa_verify_digest_go(struct esec_mb* mb, struct esec_rsa_key key, const uint8_t* digest, size_t len,
                                   const uint8_t* signature);

#endif
