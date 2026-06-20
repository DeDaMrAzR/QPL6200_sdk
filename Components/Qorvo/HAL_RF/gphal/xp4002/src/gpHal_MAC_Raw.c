/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * gpHal_MAC_Raw.c
 *   This file contains the implementation of the MAC functions for Raw mode.
 *
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
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpPd.h"
#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_MAC.h"
// GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

// Debug
#include "hal.h"
#include "gpAssert.h"
#include "gpLog.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION

#define GP_HAL_NONCE_104 13              // bytes

#define GP_HAL_ZB_RAW_NONCE_SIZE    0x10 // round nonce size (13) to next multiple of GP_WB_MAX_MEMBER_SIZE (8)
#define GP_HAL_ZB_RAW_KEY_INFO_SIZE 0x18 // round key info size (17) to next multiple of GP_WB_MAX_MEMBER_SIZE (8)

#define GPHAL_ZB_FRAMECOUNTER_POOLSIZE 2
#define GPHAL_ZB_KEYINFO_POOLSIZE      2

/* compile time verification of info structures */
GP_COMPILE_TIME_VERIFY((GP_HAL_ZB_RAW_NONCE_SIZE % GP_WB_MAX_MEMBER_SIZE) == 0);
GP_COMPILE_TIME_VERIFY(GP_HAL_ZB_RAW_NONCE_SIZE >= GP_WB_ZB_RAWMODE_NONCE_SIZE);
GP_COMPILE_TIME_VERIFY((GP_HAL_ZB_RAW_KEY_INFO_SIZE % GP_WB_MAX_MEMBER_SIZE) == 0);
GP_COMPILE_TIME_VERIFY(GP_HAL_ZB_RAW_KEY_INFO_SIZE >= GP_WB_ZB_RAWMODE_KEY_INFO_SIZE);

#ifndef GP_COMP_CHIPEMU
COMPILER_ALIGNED(GP_WB_MAX_MEMBER_SIZE)
UInt8 gpHal_ZB_RawMode_FrameCounter_Pool[GP_HAL_ZB_RAW_NONCE_SIZE * GPHAL_ZB_FRAMECOUNTER_POOLSIZE] LINKER_SECTION(
    ".rt_fast_ram");
COMPILER_ALIGNED(GP_WB_MAX_MEMBER_SIZE)
UInt8 gpHal_ZB_RawMode_KeyInfo_Pool[GP_HAL_ZB_RAW_KEY_INFO_SIZE * GPHAL_ZB_KEYINFO_POOLSIZE] LINKER_SECTION(
    ".rt_fast_ram");
#endif // GP_COMP_CHIPEMU

#ifdef GP_COMP_CHIPEMU
extern UInt32 gpChipEmu_GetGpMicroStructZBRawFrameCounterPoolStart(UInt32 gp_mm_ram_linear_start);
extern UInt32 gpChipEmu_GetGpMicroStructZBRawKeyInfoPoolStart(UInt32 gp_mm_ram_linear_start);
#define GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_START                                                                          \
    gpChipEmu_GetGpMicroStructZBRawFrameCounterPoolStart(GP_MM_RAM_LINEAR_START)
#define GP_HAL_ZB_RAW_KEYINFO_POOL_START gpChipEmu_GetGpMicroStructZBRawKeyInfoPoolStart(GP_MM_RAM_LINEAR_START)
#else
#define GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_START ((UIntPtr)&gpHal_ZB_RawMode_FrameCounter_Pool[0])
#define GP_HAL_ZB_RAW_KEYINFO_POOL_START      ((UIntPtr)&gpHal_ZB_RawMode_KeyInfo_Pool[0])
#endif // GP_COMP_CHIPEMU

#define GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_INDEX_TO_ADDRESS(index)                                                        \
    (GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_START + index * GP_HAL_ZB_RAW_NONCE_SIZE)
#define GP_HAL_ZB_RAW_KEYINFO_POOL_INDEX_TO_ADDRESS(index)                                                             \
    (GP_HAL_ZB_RAW_KEYINFO_POOL_START + index * GP_HAL_ZB_RAW_KEY_INFO_SIZE)

static UInt8 gpHal_activeRawKeyInfoIndex;
static UInt8 gpHal_activeRawFrameCounterIndex;
#endif // def GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  MAC FUNCTIONS
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
//  MAC GENERAL FUNCTIONS
//-------------------------------------------------------------------------------------------------------

#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
Bool gpHal_GetTxEncryptionDone(UInt8 PBMentry)
{
    Bool encryptionDone = false;
    if(gpPd_BufferTypeZigBee == gpPd_GetPdType(gpPd_GetPdFromPBM(PBMentry)))
    {
        gpHal_Address_t pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
        encryptionDone = GP_WB_READ_ZB_FORMAT_T_ENC_DONE(pbmOptAddress);
    }
    return encryptionDone;
}

void gpHalMac_RawModeInit(void)
{
    // initialize to a non-existing entry.
    GP_WB_WRITE_ZBFILT_RAWMODE_KEY_INFO_PTR((gpHal_Address_t)0xFFFF);
    GP_WB_WRITE_ZBFILT_RAWMODE_NONCE_PTR((gpHal_Address_t)0xFFFF);

    gpHal_activeRawKeyInfoIndex = 0xff;
    gpHal_activeRawFrameCounterIndex = 0xff;
}

#define KEY_INFO_CURRKEY_LEN 16
void gpHal_SetRawModeEncryptionKeys(UInt8 encryptionKeyId, UInt8* pCurrKey)
{
    UInt8 nonActiveKI = gpHal_activeRawKeyInfoIndex == 0 ? 1 : 0;
    gpHal_Address_t ptr = (gpHal_Address_t)GP_HAL_ZB_RAW_KEYINFO_POOL_INDEX_TO_ADDRESS(nonActiveKI);

    GP_WB_WRITE_ZB_RAWMODE_KEY_INFO_KEYID(ptr, encryptionKeyId);
    gpHal_WriteRegs(ptr + GP_WB_ZB_RAWMODE_KEY_INFO_CURRKEY_ADDRESS, pCurrKey, KEY_INFO_CURRKEY_LEN);

    // make the updated KeyInfo active by updating the pointer used by the RT.
    GP_WB_WRITE_ZBFILT_RAWMODE_KEY_INFO_PTR(ak_rap_rt_to_ptr(ptr));
    gpHal_activeRawKeyInfoIndex = nonActiveKI;

#ifdef GP_LOCAL_LOG
    GP_LOG_PRINTF("gpHal_Sec: poolId: %u, keyId: %u, key:", 0, nonActiveKI, GP_WB_READ_ZB_RAWMODE_KEY_INFO_KEYID(ptr));
    UInt8 key[KEY_INFO_CURRKEY_LEN];
    gpHal_ReadRegs(ptr + GP_WB_ZB_RAWMODE_KEY_INFO_CURRKEY_ADDRESS, (void*)key, KEY_INFO_CURRKEY_LEN);
    gpLog_PrintBuffer(KEY_INFO_CURRKEY_LEN, (UInt8*)key);
#endif // GP_LOCAL_LOG
}

void gpHal_SetRawModeNonceFields(UInt32 frameCounter, MACAddress_t* pExtendedAddress, UInt8 seclevel)
{
    UInt8 nonce[GP_HAL_NONCE_104 + 3];
    UInt8 nonActiveFC = gpHal_activeRawFrameCounterIndex == 0 ? 1 : 0;
    gpHal_Address_t old_ptr =
        (gpHal_Address_t)GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_INDEX_TO_ADDRESS(gpHal_activeRawFrameCounterIndex);
    gpHal_Address_t new_ptr = (gpHal_Address_t)GP_HAL_ZB_RAW_FRAMECOUNTER_POOL_INDEX_TO_ADDRESS(nonActiveFC);

    if((frameCounter == 0xFFFFFFFF) && (pExtendedAddress == NULL))
    {
        GP_ASSERT_DEV_INT(0);
        return;
    }

    // Read the old nonce first, so we can update it further on
    gpHal_ReadRegs(old_ptr, (void*)nonce, GP_HAL_NONCE_104);

    UInt8 idx = 0;
    if(pExtendedAddress != NULL)
    {
        MEMCPY(&nonce[idx], pExtendedAddress, GP_WB_ZB_RAWMODE_NONCE_MACADDRESS_LEN);
        HOST_TO_BIG_UINT64(&nonce[idx]);
    }
    idx += GP_WB_ZB_RAWMODE_NONCE_MACADDRESS_LEN;
    if(frameCounter != 0xFFFFFFFF)
    {
        HOST_TO_BIG_UINT32(&frameCounter);
        MEMCPY(&nonce[idx], &frameCounter, sizeof(frameCounter));
    }
    idx += sizeof(frameCounter);
    nonce[idx] = seclevel;
    idx++;
    nonce[idx] = 0;
    idx++;
    nonce[idx] = 0;
    idx++;
    nonce[idx] = 0;
    gpHal_WriteRegs(new_ptr, nonce, GP_HAL_NONCE_104);

    // make the updated FrameCounter active by updating the pointer used by the RT.
    GP_WB_WRITE_ZBFILT_RAWMODE_NONCE_PTR(ak_rap_rt_to_ptr(new_ptr));
    gpHal_activeRawFrameCounterIndex = nonActiveFC;

#ifdef GP_LOCAL_LOG
    GP_LOG_PRINTF("gpHal_Sec: nonceptr: %u", 0, ak_rap_rt_to_ptr(new_ptr));
    GP_LOG_PRINTF("gpHal_Sec: poolId: %u, nonce:", 0, nonActiveFC);
    gpHal_ReadRegs(new_ptr, (void*)nonce, GP_HAL_NONCE_104);
    gpLog_PrintBuffer(GP_HAL_NONCE_104, nonce);
#endif // GP_LOCAL_LOG
}
#endif // def GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION
