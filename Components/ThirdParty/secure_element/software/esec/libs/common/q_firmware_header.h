

#ifndef Q_FIRMWARE_HEADER_H
#define Q_FIRMWARE_HEADER_H

#include "q_certificate_definition.h"
#if defined(GP_COMP_VERSION) || defined(GP_DIVERSITY_BOOTLOADER_BUILD)
#include "gpVersion.h"
#endif

/************************************************
 *  Macro definitions
 ***********************************************/
#define APPUC_FW_FIRST_STAGE_BL                   (0xC960730D)
#define APPUC_FW_LOAD_COMPLETE_MW                 (0x693A5C81UL)
#define APPUC_FW_HEADER_NON_SECURE_BOOT_MW        (0x5733416B)
#define APPUC_FW_HEADER_VERSION                   (0)

/************************************************
 *  Structs
 ***********************************************/

typedef struct {
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} se_fw_version_t;

/** @brief SE Firmware authenticated header structure */
typedef struct fw_auth_header_s {
    uint32_t fw_size;            /**< firmware size*/
    uint32_t anti_rollback_version;   /**< Anti-rollback firmware version*/
    uint8_t encrypt_flag;        /**< 0: firmware unencrypted, 1: firmware encrypted*/
    uint8_t key_select;          /**< firmware root key index*/
    se_fw_version_t fw_version; /**< Firmware version */
    uint8_t fw_hdr_version;      /**< Firmware header version */
    uint8_t reserved[18];        /**< reserved*/
    uint8_t encrypted_key[ESEC_ROOT_KEYS_SIZE]; /**< software encryption pub key*/
} fw_auth_header_t;

/** @brief SE Firmware header structure */
typedef struct fw_header_s {
    certificate_t certificate;         /**< certificate with developer signing pub key*/
    uint8_t signature[AUTH_SIGN_SIZE]; /**< firmware signature*/
    fw_auth_header_t auth;             /**< authenticated header section*/
} fw_header_t;

/** @brief APPUC Firmware version info structure */
typedef struct appuc_fw_version_info_s {
    uint32_t app_version;
    uint32_t app_descriptor;
    uint32_t app_changelist_specifier;
    uint32_t reserved;
} appuc_fw_version_info_t;

/** @brief APPUC Firmware authenticated header structure */
typedef struct appuc_fw_auth_header_s {
    uint32_t fw_size;     /**< firmware size*/
    uint32_t version;     /**< firmware version*/
    uint8_t encrypt_flag; /**< 0: firmware unencrypted, 1: firmware encrypted*/
    uint8_t key_select;   /**< firmware root key index*/
    uint8_t reserved1[2];
    uint32_t vpp;
    uint32_t image_type;
    uint32_t load_completed_magic_word;
    uint8_t header_version;
    uint8_t reserved[3];
    uint32_t image_destination_address;
    uint8_t encrypted_key[ESEC_ROOT_KEYS_SIZE]; /**< software encryption pub key*/
} appuc_fw_auth_header_t;

/** @brief APPUC Firmware header structure */
typedef struct appuc_fw_header_s {
    certificate_t certificate;         /**< certificate with developer signing pub key*/
    uint8_t signature[AUTH_SIGN_SIZE]; /**< firmware signature*/
    appuc_fw_auth_header_t auth;       /**< authenticated header section*/
    appuc_fw_version_info_t appuc_fw_version_info;
    uint8_t reserved[112];
    uint8_t padding[12];
} appuc_fw_header_t;

/*****************************************************************************
 *                    Functions
 *****************************************************************************/
const appuc_fw_header_t* hal_get_appuc_header(void);

#if defined(GP_COMP_VERSION) || defined(GP_DIVERSITY_BOOTLOADER_BUILD)
const gpVersion_SoftwareInfo_t* get_sw_info(void);
#endif

#endif /* Q_FIRMWARE_HEADER_H */
