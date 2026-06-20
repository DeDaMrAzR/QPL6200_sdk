/**
 * @brief eSecure Library: RSA key representation and cryptographic functions
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_RSA_BASICS_HEADER
#define ESEC_RSA_BASICS_HEADER

/** RSA asymmetric key.
 *
 * Represents a RSA key. The key can be protected,
 * volatile or unprotected. It can contain a public, a private key
 * or a private public key pair.
 **/
struct esec_rsa_key {
    uint32_t keyspec;
    struct esec_key_storage storage;
};

/** RSA public key is a 4 byte short exponent. **/
#define ESEC_KEY_SHORT_EXP (1 << 11)

/** RSA private key contains CRT values for faster operations. **/
#define ESEC_KEY_PRIV_CRT (1 << 9)

/** RSA key specification with a fixed key base length.
 *
 * The key base length gives the size in bytes of all big numbers used in a
 * key. The key base length must be a multiple of 4 bytes. A RSA key
 * always contains the domain value (the modulo 'n').
 *
 * A public key, when included, provides the public exponend. That could
 * be a short exponent of 4 bytes if ESEC_KEY_SHORT_EXP is set.
 *
 * A private key can be included too. Without CRT, the private key has 2 big
 * numbers. With CRT, the private key has 5 big numbers which are half the size
 * in bytes of the key base length.
 **/
#define ESEC_KEY_RSA(SZ) (ESEC_KEY_TYPE_RSA | ESEC_KEY_DOMAIN | ((SZ >> 2) - 1))

/** No additional padding in RSA signature and encyption operations.
 *
 * See also esec_rsa_sign(), esec_rsa_verify(), esec_rsa_encypt() and
 * esec_rsa_decrypt().
 **/
#define ESEC_RSA_PADDING_NONE 0

/** Optimal asymetric encryption padding for RSA encryption operations.
 *
 * See also esec_rsa_encypt() and esec_rsa_decrypt().
 **/
#define ESEC_RSA_PADDING_OAEP 1

/** EME PKCS padding for RSA encryption operations.
 *
 * See also esec_rsa_encypt() and esec_rsa_decrypt().
 **/
#define ESEC_RSA_PADDING_EME_PKCS 2

/** EMSA PKCS padding for RSA signature operations.
 *
 * See also esec_rsa_sign() and esec_rsa_verify().
 **/
#define ESEC_RSA_PADDING_EMSA_PKCS 3

/** Probabilistic Signature Scheme (PSS) padding for RSA signature operations.
 *
 * See also esec_rsa_sign() and esec_rsa_verify().
 **/
#define ESEC_RSA_PADDING_PSS 4

#endif
