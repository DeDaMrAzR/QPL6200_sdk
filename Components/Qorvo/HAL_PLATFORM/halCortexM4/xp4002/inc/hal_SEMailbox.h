/*
 * Copyright (c) 2023, Qorvo Inc
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
 *
 */


#ifndef _HAL_SEMAILBOX_H_
#define _HAL_SEMAILBOX_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include <stdbool.h>
#include "esec_errors.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#define HAL_SEMAILBOX_RESULT_SUCCESS (ESEC_OKAY)

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the mailbox of the secure element.
 */
void hal_SEMailboxInit(void);

/**
 * @brief Writes to the RRAM via the SE Mailbox.
 *
 * @param address       target address where the data needs to be written
 * @param data          pointer to data to write
 * @param sizeInBytes   length in bytes of the data
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_NvmWrite(UInt32 address, UInt32* data, UInt32 sizeInBytes);

/**
 * @brief Request a soft reset with specific reason
 *
 * @param reset_reason  Specific reset reason to be used for the trigger, supported reasons are
 *                          - GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP
 *                          - GP_WB_ENUM_POR_REASON_SOFT_POR_RRAM_BL_CRC_INVALID
 *                          - GP_WB_ENUM_POR_REASON_SOFT_POR_BOOTLOADER
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_ResetRequest(UInt32 reset_reason);

/**
 * @brief Perform a write to a STANDBY_RESET register
 *
 * @param reset_index   The reset register index
 * @param reset_value   The value to be written to that register
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_ResetWrite(uint8_t reset_index, uint32_t reset_value);

/**
 * @brief Configure pin wakeup via the SE Mailbox.
 *
 * @param wakeup_pin_modes_0    value to be written to PMUD_WAKEUP_PIN_MODE_0-15
 * @param wakeup_pin_modes_1    value to be written to PMUD_WAKEUP_PIN_MODE_16+
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_ConfigWakeupPins(UInt32 wakeup_pin_modes_0, UInt32 wakeup_pin_modes_1);

/**
 * @brief Power on or off the 32k crystal oscillator
 *
 * @param start     true for power on, false for power off
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_XT32kPowerUp(bool start);

/**
 * @brief Configure the APPUC clock frequency
 *
 * @param clk_freq_enum     Value to be set, supported values are:
 *                              - GP_WB_ENUM_CLOCK_SPEED_M32
 *                              - GP_WB_ENUM_CLOCK_SPEED_M64
 *                              - GP_WB_ENUM_CLOCK_SPEED_M96
 *                              - GP_WB_ENUM_CLOCK_SPEED_M192
 *
 * @return Mailbox command status
 */
UInt32 hal_SEMailbox_SetAppucClkFreq(uint8_t clk_freq_enum);

/**
 * @brief Change the temperature settings
 *
 * @param temp_deg      Temperature in degrees C
 *
 * @return Mailbox command status
*/
UInt32 hal_SEMailbox_ChangeTemperature(int32_t temp_deg);

/**
 * @brief Change the battery voltage settings
 *
 * @param vbatt_uv      Battery voltage in micro-Volt
 *
 * @return Mailbox command status
*/
UInt32 hal_SEMailbox_ChangeBattVoltage(uint32_t vbatt_uv);

/**
 * @brief Configure the AKUMA clock frequency
 *
 * @param clk_freq_enum     Value to be set, supported values are:
 *                              - GP_WB_ENUM_CLOCK_SPEED_M32
 *                              - GP_WB_ENUM_CLOCK_SPEED_M64
 */
UInt32 hal_SEMailbox_SetAkumaClkFreq(uint8_t clk_freq_enum);

/**
 * @brief Generate a MAC with AES CMAC mode as described in RFC4493.
 *
 * @param pMac          16 octet (output) parameter containing the message authentication code
 * @param pKey          16 octet key
 * @param dataLength    length of pData
 * @param pData         Message to be authenticated
 *
 * @return Mailbox command status
 */
UInt32 hal_SeMailboxAesCmacGenerate(UInt8* pMac, UInt8* pKey, UInt8 dataLength, UInt8* pData);

/**
 * @brief Acquires the mutex for communication with the SE.
 */
void hal_SEMailbox_MutexAcquire(void);

/**
 * @brief Releases the mutex for communication with the SE.
 */
void hal_SEMailbox_MutexRelease(void);

#ifdef __cplusplus
}
#endif

#endif // _HAL_SEMAILBOX_H_
