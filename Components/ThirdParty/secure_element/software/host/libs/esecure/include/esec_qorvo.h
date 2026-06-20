/**
 * @brief eSecure Library: miscellaneous
 * Copyright (c) 2022, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 * CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 * IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 * LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * QORVO INC. SHALL NOT, IN ANY
 * CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 * FOR ANY REASON WHATSOEVER.
 *
 * @file
 */

#ifndef ESEC_QORVO_HEADER
#define ESEC_QORVO_HEADER

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "esec_errors.h"
#include "esec_hash_basics.h"
#include "esec_cert_basics.h"
#include "q_certificate_definition.h"

#define power_mode_standby_mask  0x000F
#define power_mode_active_offset 4
#define power_mode_active_mask   (power_mode_standby_mask << power_mode_active_offset)
#define power_mode_sleep_offset  8
#define power_mode_sleep_mask    (power_mode_standby_mask << power_mode_sleep_offset)

/** @typedef power_mode_t
 */
typedef uint16_t power_mode_t;
//@}

/** @enum ESEC_LCS_t */
/** @brief Life Cycle States.
 */
//@{
/** @brief Invalid (unable to determine LCS). */
#define ESEC_LCS_INVALID (0x00)
/** @brief VIRGIN. */
#define ESEC_LCS_VIRGIN (0x03)
/** @brief Chip Manufacturer Initialisation. */
#define ESEC_LCS_CM_INIT (0x05)
/** @brief System Manufacturer Initialisation. */
#define ESEC_LCS_SM_INIT (0x06)
/** @brief Retest. */
#define ESEC_LCS_RETEST (0x09)
/** @brief Secured. */
#define ESEC_LCS_SECURED (0x0a)
/** @brief RMA test. */
#define ESEC_LCS_RMA_TEST (0x0c)
/** @brief RMA. */
#define ESEC_LCS_RMA (0x0f)
/** @brief Decommissioned. */
#define ESEC_LCS_DECOMMISSIONED (0x11)
/** @typedef ESEC_LCS_t
    @brief Life Cycle State.
*/
typedef uint8_t ESEC_LCS_t;
//@}

/** @enum srk_source_t */
/** @brief
 * Possible SRK source return values
 */
//@{
#define srk_source_otp     (0x01)
#define srk_source_puf     (0x02)
#define srk_source_invalid (0x03)
#define srk_source_none    (0x04)

/** @typedef srk_source_t
 */
typedef uint8_t srk_source_t;
//@}

/**
 * @brief Challenge/response key.
 */
typedef enum {
    CR_KEY_CM_FW = 0,     /**< CM firmware key. */
    CR_KEY_CM_CMD = 3,    /**< CM command key. */
    CR_KEY_CM_RETEST = 5, /**< CM retest key. */
    CR_KEY_CM_RMA = 6,    /**< CM rma key. */
    CR_KEY_SM_FW = 9,     /**< SM firmware key. */
    CR_KEY_SM_CMD = 10,   /**< SM command key. */
} cr_key_t;

/** @typedef platform_state_t
 */
typedef struct {
    ESEC_LCS_t lcs;
    srk_source_t srk_source;
    uint8_t rom_version_minor;
    uint8_t rom_version_major;
    uint8_t rombl_sha256[ESEC_HASH_SIZE_SHA256];
    uint8_t sefw_sha256[ESEC_HASH_SIZE_SHA256];
    uint32_t sefw_rollback_version;
    uint8_t sefw_verbose_version[128];
    uint8_t signature[ESEC_MFCT_SIG_SZ];
} platform_state_t;
//@}

/** @enum core_enum_t */
/** @brief
 * Possible SRK source return values
 */
//@{
#define core_enum_akuma (0x01)
#define core_enum_appuc (0x02)

/** @typedef core_enum_t
 */
typedef uint8_t core_enum_t;
//@}

#define Q_POWER_MODE(pm_sb, pm_act, pm_sleep)                                                                          \
    (pm_sb + (pm_act << power_mode_active_offset) + (pm_sleep << power_mode_sleep_offset))
#define Q_POWER_MODE_ACTIVE(q_pm)  ((q_pm & power_mode_active_mask) >> power_mode_active_offset)
#define Q_POWER_MODE_STANDBY(q_pm) (q_pm & power_mode_standby_mask)
#define Q_POWER_MODE_SLEEP(q_pm)   ((q_pm & power_mode_sleep_mask) >> power_mode_sleep_offset)

#define Q_POWER_MODE_SET_MODE(pm, mode, mask, offset) ((pm & (~mask)) | ((mode << offset) & mask))
#define Q_POWER_MODE_SET_STANDBY_MODE(pm, pm_sb)      Q_POWER_MODE_SET_MODE(pm, pm_sb, power_mode_standby_mask, 0)
#define Q_POWER_MODE_SET_ACTIVE_MODE(pm, pm_act)                                                                       \
    Q_POWER_MODE_SET_MODE(pm, pm_act, power_mode_active_mask, power_mode_active_offset)
#define Q_POWER_MODE_SET_SLEEP_MODE(pm, pm_sleep)                                                                      \
    Q_POWER_MODE_SET_MODE(pm, pm_sleep, power_mode_sleep_mask, power_mode_sleep_offset)

#define GP_WB_ENUM_STANDBY_POWER_MODE_NOCHANGE 0xF
#define GP_WB_ENUM_ACTIVE_POWER_MODE_NOCHANGE  0xF

#define OTP_QORVO_SM_APPUC_SECURE_BOOT_ENABLE_MASK        (0b111 << 0)
#define OTP_QORVO_SM_APPUC_SECURE_DEBUG_ENABLE_MASK       (0b111 << 6)
#define OTP_QORVO_SM_APPUC_SECURE_DEBUG_ANY_ENABLE_MASK   (0b1 << 8)
#define OTP_QORVO_SM_APPUC_SECURE_DEBUG_NIDEN_ENABLE_MASK (0b1 << 7)
#define OTP_QORVO_SM_APPUC_SECURE_DEBUG_DBGEN_ENABLE_MASK (0b1 << 6)
#define OTP_QORVO_SM_APPUC_DEBUG_DISABLE_MASK             (0b111 << 3)
#define OTP_QORVO_SM_APPUC_PROGRAMMING_DISABLE_MASK       (0b111 << 9)

#define PROVISIONING_RESULT_ROLE_MASK 0x80
#define PROVISIONING_RESULT_ROLE_CM   0x00
#define PROVISIONING_RESULT_ROLE_SM   0x80

#define PROVISIONING_RESULT_ERROR_MASK 0x7f
/*! No provisioning has taken place yet */
#define PROVISIONING_RESULT_ERROR_NONE 0x00
/*! Provisioning was succesful */
#define PROVISIONING_RESULT_ERROR_OK 0x01
/*! Too many chunks in provisioning bundle */
#define PROVISIONING_RESULT_ERROR_TOO_MANY_CHUNKS 0x02
/*! Unable to verify chunk; last processed chunk field contains index (0-based) of failed chunk */
#define PROVISIONING_RESULT_ERROR_CHUNK_VERIFICATION 0x03
/*! Malformed chunk found; last processed chunk field contains index (0-based) of failed chunk */
#define PROVISIONING_RESULT_ERROR_MALFORMED_CHUNK 0x04
/*! Other error */
#define PROVISIONING_RESULT_OTHER 0x05
/*! No bundle present at specified address (only for SM provisioning) */
#define PROVISIONING_RESULT_NO_BUNDLE 0x06

/** @brief  Read SEUC firmware version.
 *
 *  @param  version - pointer to location for version
 *  @return ESEC_OKAY if success, error code otherwise (see esec_errors.h)
 **/
uint32_t esec_get_firmware_version(uint32_t* version);

/** @brief  Write data to NVM.
 *
 *  Disallows writes to SEUC firmware region and CM infopage area.
 *
 *  @param  destination_address destination address (must be in NVM)
 *  @param  source_address source address (can not be in NVM)
 *  @param  length number of bytes
 *  @return ESEC_OKAY if success, error code otherwise (see esec_errors.h)
 **/
uint32_t esec_nvm_write(uint32_t destination_address, uint32_t source_address, uint32_t length);

/** @brief  Write 16 bytes of aligned data to NVM, verify ECC errors in the process
 *
 *  Disallows writes to SEUC firmware region and CM infopage area.
 *
 *  @param  destination_address destination address (must be in NVM and NVM_WORD_SIZE_BYTES (=16) aligned)
 *  @param  source_address source address (can not be in NVM)
 *  @return ESEC_OKAY if success, TEST_FAILED when ECC errors occured, or another error code otherwise (see
 *esec_errors.h)
 **/
uint32_t esec_nvm_write_with_error_check(uint32_t destination_address, uint32_t source_address);

/** @brief  Erase from NVM.
 *
 *  Disallows erasing parts of SEUC firmware region and CM infopage area.
 *
 *  @param  address destination address (must be in NVM)
 *  @param  length number of bytes
 *  @return ESEC_OKAY if success, error code otherwise (see esec_errors.h)
 **/
uint32_t esec_nvm_erase(uint32_t address, uint32_t length);

/** @brief  Request a reset (softpor).
 *
 *  Requests a reset of the device.
 *  @param soft_por_trigger The soft por trigger to use for the reset, see PMUD_SOFT_POR_TRIGGERS register
 *  @return Will not return if succesful, error code otherwise (see esec_errors.h)
 **/
uint32_t esec_reset_request(uint8_t soft_por_trigger);

/** @brief  Change the clock frequency divider of a core
 *
 *  Changes the clock divider in the STANDBY regmap to obtain a different instruction clock on APPUC or AKUMA
 *  @param  clk_freq_enum the value to set the divider to, from enum.h
 *  @param  core_enum the core for which to change the frequency, either
 *  @return ESEC_OKAY if no error
 **/
uint32_t esec_set_clk_freq(uint8_t clk_freq_enum, core_enum_t core_enum);

/** @brief  Power up the 32k Crystal Oscillator
 *
 *  Powers up the oscillator if the bool is true, turns it off otherwise
 *  @param  power_up bool to indicate power up or not
 *  @return ESEC_OKAY if no error
 **/
uint32_t esec_xt32k_power_up(bool power_up);

/** @brief ECC point multiplication (result = scalar * point) for secp256r1 curve
 *  @param scalar integer value to be multiplied by the point [input]
 *  @param point point on the curve to be multiplied with [input]
 *  @param result output of the multiplication operation [output]
 *  @return ESEC_OKAY if successful, error code otherwise
 */
uint32_t esec_ecc_secp256r1_point_mul(const uint8_t* scalar, const uint8_t* point, uint8_t* result);

/**
 * @brief Multiply and add ECC Weierstrass points on secp256r1 curve
 *        \p result = \p scalar_m * \p point1 + \p scalar_n * \p point2
 *
 * @param scalar_m    will multiply the first point
 * @param point1      the first point to multiply and add
 * @param scalar_n    will multiply the second point
 * @param point2      the second point to multiply and add
 * @param result      the result of the mutliply/add operation
 * @return  ESEC_OKAY if no error
 */

uint32_t esec_ecc_secp256r1_point_muladd(const uint8_t* scalar_m, const uint8_t* point1, const uint8_t* scalar_n,
                                         const uint8_t* point2, uint8_t* result);

/**
 * @brief Transition the SEUC to sleep
 *
 */
void esec_go_to_sleep(void);

/**
 * @brief Get the symbolic SEUC FW version as 0-terminated ASCII string
 *
 * @param version  Pointer to a 128-byte buffer
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_get_seucfw_version(uint8_t* version);

/**
 * Checks certificate validity
 *
 * @param certificate Pointer to the certificate structure to be validated
 * @param auth_misc   Miscellaneous authorization bits to check
 * @param cr_key      Key type for verification
 *
 * @return ESEC_OKAY if certificate valid, else certificate invalid.
 *
 */
uint32_t esec_validate_certificate(certificate_t* certificate, uint32_t auth_misc, cr_key_t cr_key);

/**
 * @brief Finalize the wake up in the SEUC
 * @return  ESEC_OKAY if no error
 *
 */
uint32_t esec_finalize_wakeup(void);

/**
 * @brief Configure the GPIO wakeup
 * @param wakeup_pin_modes_0  Wakeup config for gpio 0-15, see PMUD_WAKEUP_PIN_MODES_0 register description
 * @param wakeup_pin_modes_1  Wakeup config for gpio 16-29, see PMUD_WAKEUP_PIN_MODES_1 register description
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_config_gpio_wakeup(uint32_t wakeup_pin_modes_0, uint32_t wakeup_pin_modes_1);

/**
 * @brief Configure the APPUC RAM retain setting
 * @param ram_retain  the requested RAM retain setting, see PMUD_RAM_RETAIN register description (only APPUC parts)
 * @return  ESEC_OKAY if no error
 *
 */
uint32_t esec_config_appuc_ram_retain(uint16_t ram_retain);

/**
 * @brief Change the temperature settings, the temperature is passed via CAL_RAM regmap
 *
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_change_temperature(void);

/**
 * @brief Write the PMUD register RC64K_USE_VCORE
 * @param value The value to be written either 0 or 1
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_write_rc64k_use_vcore(uint8_t value);

/**
 * @brief Change the battery voltage settings, the battery voltage is passed via CAL_RAM regmap
 *
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_change_voltage(void);

/**
 * @brief Set a power mode
 *
 * @param power_mode  the power mode to set
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_set_power_mode(const power_mode_t power_mode);

/**
 * @brief Get the current power mode
 *
 * @param power_mode  Pointer to location for the power mode
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_get_power_mode(power_mode_t* power_mode);

/** @brief  Read SEUC SM qorvo_config.
 *
 *  @param  qorvo_config - pointer to location to store configuration in.
 *          use the OTP_QORVO_SM_APPUC_* masks to extract the configuration.
 *
 *  @return ESEC_OKAY if success, error code otherwise (see esec_errors.h)
 **/
uint32_t esec_get_sm_qorvo_config(uint32_t* qorvo_config);

/**
 * @brief Perpare the chip to go to sleep and obtain the power mode in the process
 *
 * @param power_mode  Pointer to location for the power mode
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_prepare_sleep(power_mode_t* power_mode);

/**
 * @brief Write to the STANDBY_RESET registers to trigger reset of a peripheral
 *
 * @param reset_index  The index of the standby register (0, 1, 2 or 3)
 * @param reset_value  Value to be written to STANDBY_RESET_<reset_index>
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_standby_reset_write(uint8_t reset_index, uint32_t reset_value);

/**
 * @brief Retrieve platform state
 *
 * @param nonce           A 64-bit number that will be included in the signature to prevent response replay attacks
 * @param platform_state  Pointer to location for the platform state
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_get_platform_state(uint64_t nonce, platform_state_t* platform_state);

/**
 * @brief Start upgrade procedure
 *
 * @param sefw_or_appuc   1 = sefw, 0 = appuc fw
 * @param offset          Offset in RRAM where the upgrade image is located
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_start_upgrade(bool sefw_or_appuc, uint32_t offset);

/**
 * @brief Retrieve tamper levels
 *
 * @param tamper_levels   Pointer to a uint32_t array to store the tamper levels in
 * @param number_of_words Size of the array in words (should be at least 8)
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_get_tamper_levels(uint32_t* tamper_levels, uint8_t number_of_words);

/**
 * @brief Apply an sm provisioning bundle
 *
 * @param bundle_address  Location where the bundle is stored
 * @return  ESEC_OKAY if no error
 */
uint32_t esec_apply_sm_bundle(uint32_t bundle_address, uint32_t* result);

/**
 * @brief Acquires the mutex for communication with the SE.
 */
void esec_acquire_mutex(void);

/**
 * @brief Releases the mutex for communication with the SE.
 */
void esec_release_mutex(void);

#endif
