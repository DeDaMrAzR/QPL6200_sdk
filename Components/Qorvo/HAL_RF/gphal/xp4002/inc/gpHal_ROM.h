/*
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
 *
 */

#ifndef _GPHAL_ROM_H_
#define _GPHAL_ROM_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* ROM version */
#define ROMBL_VERSION_MAJOR 0x1UL
#define ROMBL_VERSION_MINOR 0x2UL
#define ROMBL_VERSION       ((ROMBL_VERSION_MAJOR << 24) | (ROMBL_VERSION_MINOR << 16) | (0x0000UL))

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef UIntPtr NvmPtr;

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    ROM function prototypes
 *****************************************************************************/

/**
 * @brief Function which returns the ROM version of the device
 *
 * @return MSB == Major, LSB == Minor
 */
UInt16 gpHal_ROM_version(void);

/**
 * @brief Function to write to NVM
 *
 * @param address 128-bit word address of NVM data within Kx memory map (not relative to start of NVM)
 * @param num_units Number of 128-bit words to write
 * @param pData  Pointer to input data buffer containing 'num_units' 128-bit words to write.
 *               Buffer must be located in RAM. Writing directly from one part of NVM to another is not supported.
 *
 */
void gpHal_ROM_NVM_write(UInt32 address, UInt16 num_units, const UInt8* pData);

/**
 * @brief Function to write to eOTP
 *
 * @param address 128-bit word address of eOTP data within Kx memory map (not relative to start of NVM)
 * @param num_units Number of 128-bit words to write
 * @param pData  Pointer to input data buffer containing 'num_units' 128-bit words to write.
 *               Buffer must be located in RAM. Writing directly from one part of (e)OTP or NVM is not supported.
 *
 * When a particular destination 128-bit word in eOTP is not all 0's, that word will not be written.
 */
void gpHal_ROM_eOTP_write(NvmPtr address, UInt16 num_units, const UInt8* pData);

/**
 * @brief Function to destroy eOTP
 *
 * @param address 128-bit word address of NVM data within Kx memory map (not relative to start of NVM)
 * @param num_units Number of 128-bit words to destroy
 */
void gpHal_ROM_eOTP_destroy(NvmPtr address, UInt16 num_units);

#ifdef __cplusplus
}
#endif

#endif // _GPHAL_ROM_H_
