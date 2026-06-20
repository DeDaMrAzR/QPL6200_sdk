/**
 * @file libs/common/eSecure_metadata.h
 * @brief Defines metadata and bit masks for messages and key handling
 * @copyright Copyright (c) 2016-2021 Silex Insight. All Rights reserved
 */

#ifndef ESECURE_METADATA_H
#define ESECURE_METADATA_H

#include <stdint.h>
#include "sx_eddsa_alg.h"
#include "esec_features.h"

// Metadata size in bytes
#define METADATA_SIZE       4

// Custom metadata for special case
#define METADATA_CUSTOM     0xFFFFFFFF
#define KEY_HEADER_AUTH_DATA_LENGTH_MASK    0x0000007F
#define KEY_HEADER_AUTH_DATA_LENGTH_LSB     0
#define KEY_HEADER_AUTH_DATA_PRESENT        7

// Key type (bits [31:28])
#define KEY_TYPE_MASK      0xF0000000
#define KEY_TYPE_OFFSET    28
#define KEY_TYPE_SYM       0x0
#define KEY_TYPE_RSA       0x4
#define KEY_TYPE_DSA       0x6
#define KEY_TYPE_DH_ELG    0x7
#define KEY_TYPE_ECC_W     0x8
#define KEY_TYPE_ECC_MON   0xB
#define KEY_TYPE_EDDSA     0xC
#define KEY_TYPE_ECKCDSA   0xD

// Key mode (bits [27:26])
#define KEY_MODE_OFFSET    26
#define KEY_MODE_MASK      0x3
#define KEY_MODE_UNPROTECT 0x0
#define KEY_MODE_VOLATILE  0x1
#define KEY_MODE_WRAP      0x2
#define KEY_MODE_WRAP_AR   0x3

// Key restriction (bits [25:24])
#define KEY_RESTR_MASK     0x03000000
#define KEY_RESTR_UNLOCKED 0x00000000
#define KEY_RESTR_LOCKED   0x01000000
#define KEY_RESTR_INTERNAL 0x02000000
#define KEY_RESTR_RESTRICT 0x03000000

// Key index (bits [23:16])
#define KEY_INDEX_MASK     0x00FF0000
#define KEY_INDEX_OFFSET   16
#define KEY_INDEX_EK       0xFF

// Key NoProt (bit [15])
#define KEY_NOPROT         15

// Key attributes (bits [14:0])
#define KEY_ATTR_PRIVATE   14
#define KEY_ATTR_PUBLIC    13
#define KEY_ATTR_DOMAIN    12
#define KEY_ATTR_SHORT     11
#define KEY_ATTR_SIGN      10
#define KEY_ATTR_SPECIFIC_MASK 0x00000FFF
#define KEY_ATTR_MASK      0x00007FFF

// - Symmetric key type
#define KEY_SYM_SIZE       0x00003FFF
#define KEY_RSA_SIZE       0x000000FF
#define KEY_DH_ECC_SIZE    0x0000007F
#define KEY_AES_WRAP_MASK  0x00004000
// - RSA key type
#define KEY_RSA_CRT        9
#define KEY_RSA_RESERVED   0x00000100
// - DSA key type
#define KEY_DSA_SIZE_Q     0x00000780
#define KEY_DSA_SIZE_Q_OFF 7
#define KEY_DSA_SIZE_P     0x0000007F
#define KEY_DSA_RESERVED   0x00000800
// - DH Key type
#define KEY_DH_RESERVED    0x00000780

// - ECC key type
#define KEY_ECC_FIELD      11
#define KEY_ECC_A_0        9
#define KEY_ECC_A_3        8
#define KEY_ECC_SEL_CURV   0x00000380
#define KEY_ECC_SEL_CURV_OFFSET   7
#define KEY_ECC_MNG_448    0
#define KEY_ECC_MNG_25519  0x00000080
#define KEY_ECC_P160       0x13
#define KEY_ECC_P192       0x17
#define KEY_ECC_P224       0x1B
#define KEY_ECC_P256       0x1F
#define KEY_ECC_P320       0x27
#define KEY_ECC_P384       0x2F
#define KEY_ECC_P512       0x3F
#define KEY_ECC_P521       0x41
#define KEY_ECC_B163       0x14
#define KEY_ECC_B233       0x1D
#define KEY_ECC_B283       0x23
#define KEY_ECC_B409       0x33
#define KEY_ECC_B571       0x47
#define KEY_ECC_CURV_25519 0x1F
#define KEY_ECC_CURV_448   0x37

#define KEY_ECKCDSA_RESERVED    0x400

#define KEY_ECC_CURVE_NIST 0x00
#define KEY_ECC_CURVE_SM2_P256_V1 0x01
#define KEY_ECC_CURVE_SM2_P256_V2 0x02
#define KEY_ECC_CURVE_BRAINPOOL 0x03

// - ECC Montgomery key type
#define KEY_ECC_MON_RESERVED 0x00001C00

// - ECC EdDSA key type
#define KEY_ECC_EDDSA_RESERVED  0x00001F80
#define KEY_ECC_EDDSA_SIZE      0x0000007F
#define KEY_ECC_EDDSA_CURV_25519 0x1F
#define KEY_ECC_EDDSA_CURV_448   0x38

// Macro's to check the key types
#define IS_LOCKED_KEY(metadata)     ((metadata) & (KEY_RESTR_LOCKED | KEY_RESTR_INTERNAL))
#define IS_INTERNAL_KEY(metadata)   ((metadata) & KEY_RESTR_INTERNAL)
#define IS_RESTRICTED_KEY(metadata) (((metadata) & KEY_RESTR_LOCKED) && \
                                    ((metadata) & KEY_RESTR_INTERNAL))
#define IS_PROTECTED_KEY(metadata)  ((((metadata) >> KEY_MODE_OFFSET) & KEY_MODE_MASK) \
                                    != KEY_MODE_UNPROTECT)
#define IS_VOLATILE_KEY(metadata)   ((((metadata) >> KEY_MODE_OFFSET) & KEY_MODE_MASK) \
                                    == KEY_MODE_VOLATILE)
#define IS_ENDORSEMENT_KEY(metadata) (IS_VOLATILE_KEY(metadata) && \
                                     ((((metadata) & KEY_INDEX_MASK) >> KEY_INDEX_OFFSET) \
                                     == KEY_INDEX_EK))

#define IS_AES_KEYWRAP_KEY(metadata) ((metadata) & KEY_AES_WRAP_MASK)

// Macro's to check the key curve types
#define IS_BRAINPOOL_CURVE(n) ((n & KEY_ECC_SEL_CURV) >> KEY_ECC_SEL_CURV_OFFSET) == KEY_ECC_CURVE_BRAINPOOL
#define IS_NISTP_CURVE(n) ((n & KEY_ECC_SEL_CURV) >> KEY_ECC_SEL_CURV_OFFSET) == KEY_ECC_CURVE_NIST

// Certificate
#define CERT_MAGIC_NUMBER  0xE5ECCE01
#define CERT_CHIP_MAGIC_NUMBER 0xE5ECCC01

// Attestation
#define ATT_KEY_MAGIC_NUMBER  0xE5ECA101
#define ATT_RTC_MAGIC_NUMBER  0xE5ECA201

#if (CFG_AUTH_ALGO == AUTH_ALGO_ED25519) && ED25519_ENABLED
#define ESEC_EK_METADATA (uint32_t)(((KEY_TYPE_EDDSA) << (KEY_TYPE_OFFSET)) | \
                         (ED25519_KEY_SIZE - 1) | \
                         ((KEY_MODE_VOLATILE) << (KEY_MODE_OFFSET)) | \
                         (KEY_RESTR_RESTRICT) | \
                         ((KEY_INDEX_EK) << (KEY_INDEX_OFFSET)) | \
                         (0 << KEY_NOPROT) | \
                         (1 << KEY_ATTR_PRIVATE) | \
                         (0 << KEY_ATTR_PUBLIC))
#elif (CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P256)
#define ESEC_EK_METADATA (uint32_t)(((KEY_TYPE_ECC_W) << (KEY_TYPE_OFFSET)) | \
                         ((KEY_MODE_VOLATILE) << (KEY_MODE_OFFSET)) | \
                         (KEY_RESTR_RESTRICT) | \
                         ((KEY_INDEX_EK) << (KEY_INDEX_OFFSET)) | \
                         (0 << KEY_NOPROT) | \
                         (1 << KEY_ATTR_PRIVATE) | \
                         (0 << KEY_ATTR_PUBLIC) | \
                         (0 << KEY_ATTR_DOMAIN) | \
                         (0 << KEY_ECC_FIELD) | \
                         (1 << KEY_ATTR_SIGN) | \
                         (0 << (KEY_ECC_SEL_CURV_OFFSET)) | \
                         KEY_ECC_P256)
#elif (CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P384)
#define ESEC_EK_METADATA (uint32_t)(((KEY_TYPE_ECC_W) << (KEY_TYPE_OFFSET)) | \
                         ((KEY_MODE_VOLATILE) << (KEY_MODE_OFFSET)) | \
                         (KEY_RESTR_RESTRICT) | \
                         ((KEY_INDEX_EK) << (KEY_INDEX_OFFSET)) | \
                         (0 << KEY_NOPROT) | \
                         (1 << KEY_ATTR_PRIVATE) | \
                         (0 << KEY_ATTR_PUBLIC) | \
                         (0 << KEY_ATTR_DOMAIN) | \
                         (0 << KEY_ECC_FIELD) | \
                         (1 << KEY_ATTR_SIGN) | \
                         (0 << (KEY_ECC_SEL_CURV_OFFSET)) | \
                         KEY_ECC_P384)
#elif (CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P521) || (CFG_AUTH_ALGO == AUTH_ALGO_ECDSA_P521_SHA3)
#define ESEC_EK_METADATA (uint32_t)(((KEY_TYPE_ECC_W) << (KEY_TYPE_OFFSET)) | \
                         ((KEY_MODE_VOLATILE) << (KEY_MODE_OFFSET)) | \
                         (KEY_RESTR_RESTRICT) | \
                         ((KEY_INDEX_EK) << (KEY_INDEX_OFFSET)) | \
                         (0 << KEY_NOPROT) | \
                         (1 << KEY_ATTR_PRIVATE) | \
                         (0 << KEY_ATTR_PUBLIC) | \
                         (0 << KEY_ATTR_DOMAIN) | \
                         (0 << KEY_ECC_FIELD) | \
                         (1 << KEY_ATTR_SIGN) | \
                         (0 << (KEY_ECC_SEL_CURV_OFFSET)) | \
                         KEY_ECC_P521)
#elif (CFG_AUTH_ALGO == AUTH_ALGO_RSA_4K)
#define ESEC_EK_METADATA (uint32_t)(((KEY_TYPE_RSA) << (KEY_TYPE_OFFSET)) | \
                         ((KEY_MODE_VOLATILE) << (KEY_MODE_OFFSET)) | \
                         (KEY_RESTR_RESTRICT) | \
                         ((KEY_INDEX_EK) << (KEY_INDEX_OFFSET)) | \
                         (0 << KEY_NOPROT) | \
                         (1 << KEY_ATTR_PRIVATE) | \
                         (0 << KEY_ATTR_PUBLIC) | \
                         (1 << KEY_ATTR_DOMAIN) | \
                         (1 << KEY_ATTR_SHORT) | \
                         (1 << KEY_ATTR_SIGN) | \
                         (1 << KEY_RSA_CRT) | \
                         (AUTH_ALG_RSA_SIZE/4 - 1))
#else
#error Invalid authentication algorithm
#endif

#endif
