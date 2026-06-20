

#ifndef Q_CERTIFICATE_DEFINITION_H
#define Q_CERTIFICATE_DEFINITION_H

#include "esec_features.h"
/************************************************
 *  Definitions
 ***********************************************/

/** @brief Salt length for the RSA PSS signature done with the RSA authentication scheme */
#define AUTH_ALG_RSA_4K_SALT_LENGTH 64
/** @brief Short exponent used with RSA authentication scheme */
static const uint8_t AUTH_ALG_RSA_PUBKEY_EXP[] = {0x00, 0x01, 0x00, 0x01}; /* 65537u as big-endian byte string */
#define AUTH_ALG_RSA_PUBKEY_EXP_BLK (block_t_convert(AUTH_ALG_RSA_PUBKEY_EXP, 4))

#if CFG_AUTH_ALGO == AUTH_ALGO_ED25519 && ED25519_ENABLED
#define AUTH_PRIVKEY_SIZE ED25519_KEY_SIZE                                /**< Authentication private key size*/
#define AUTH_PUBKEY_SIZE  ED25519_KEY_SIZE                                /**< Authentication public key size*/
#define AUTH_SIGN_SIZE    ED25519_SIGN_SIZE                               /**< Authentication signature size*/
#elif CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P256
#define AUTH_PRIVKEY_SIZE 32                                              /**< Authentication private key size*/
#define AUTH_PUBKEY_SIZE  64                                              /**< Authentication public key size*/
#define AUTH_SIGN_SIZE    64                                              /**< Authentication signature size*/
#elif CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P384
#define AUTH_PRIVKEY_SIZE 48                                              /**< Authentication private key size*/
#define AUTH_PUBKEY_SIZE  96                                              /**< Authentication public key size*/
#define AUTH_SIGN_SIZE    96                                              /**< Authentication signature size*/
#elif CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P521 || CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P521_SHA3
#define AUTH_PRIVKEY_SIZE 66                                              /**< Authentication private key size*/
#define AUTH_PUBKEY_SIZE  132                                             /**< Authentication public key size*/
#define AUTH_SIGN_SIZE    132                                             /**< Authentication signature size*/
#elif CFG_AUTH_ALGO == AUTH_ALGO_RSA_4K
#define AUTH_PRIVKEY_SIZE (AUTH_ALG_RSA_SIZE + AUTH_ALG_RSA_SIZE / 2 * 5) /**< Authentication private key size*/
#define AUTH_PUBKEY_SIZE  (AUTH_ALG_RSA_SIZE + 4)                         /**< Authentication public key size*/
#define AUTH_SIGN_SIZE    AUTH_ALG_RSA_SIZE                               /**< Authentication signature size*/
#else
#error Invalid authentication algorithm
#endif

/** @brief eSecure firmware signing authorization */
#define AUTH_MISC_ESEC_SIGN 0x01
/** @brief Host firmware signing authorization */
#define AUTH_MISC_HOST_SIGN 0x02
/** @brief Manufacturer key revokation authorization */
#define AUTH_MISC_REVOKE_KEY 0x04
/** @brief Set Virtual Time authorization */
#define AUTH_MISC_SET_VIRTUAL_TIME 0x10

/************************************************
 *  Structs
 ***********************************************/

/** @brief Certificate structure */
typedef struct certificate_content_s {
    uint32_t magic_number;              /**< certificate magic number (0xE5ECCE01) */
    uint8_t auth_debug[DBG_GRANT_SIZE]; /**< debug authorization bits*/
#if(DBG_GRANT_SIZE % 4)
    uint8_t reserved0[4 - (DBG_GRANT_SIZE % 4)];
#endif
    uint8_t auth_misc0;                   /**< miscellaneous authorization bits
                                           * (sign eSecure or host firmware, revoke keys)*/
    uint8_t key_index;                    /**< manufacturer key index*/
    uint8_t auth_misc1;                   /**< miscellaneous authorization bits */
    uint8_t auth_misc2;                   /**< miscellaneous authorization bits */
    uint32_t auth_tamper;                 /**< tamper authorization bits*/
    uint32_t expiration_time_val;         /**< Certification (Virtual) Expiration Time */
    uint8_t serialno[16];                 /**< serial number*/
    uint8_t serialno_mask[16];            /**< serial number mask*/
    uint8_t pubkey[AUTH_PUBKEY_SIZE];     /**< pub signing key*/
    uint8_t man_pubkey[AUTH_PUBKEY_SIZE]; /**< manufacturer public key*/
} certificate_content_t;

/** @brief Certificate structure */
typedef struct certificate_s {
    certificate_content_t content;     /**< certificate content*/
    uint8_t signature[AUTH_SIGN_SIZE]; /**< certificate signature*/
} certificate_t;

#define CERT_CONTENT_SIZE sizeof(certificate_content_t) /**< Certificate content size */
#define CERT_SIZE         sizeof(certificate_t)         /**< Certificate size */

#endif                                                  /* Q_CERTIFICATE_DEFINITION_H */
