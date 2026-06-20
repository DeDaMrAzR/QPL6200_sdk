/*
 * Copyright (c) 2023, Qorvo Inc
 *
 *   Hardware Abstraction Layer for ARM-based devices.
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
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

#include "hal.h"
#include "gpHal.h"

static UInt8 gpHal_ChipVersion;

/*****************************************************************************
 *                    static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    HARDWARE VERSION
 *****************************************************************************/
UInt8 gpHal_GetChipVersion(void)
{
    return gpHal_ChipVersion;
}

UInt16 gpHal_GetHWVersionId(void)
{
/*
 *                   |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 * STANDBY_VERSION_0 |chipid(1:0)|             chipVersion           |
 * STANDBY_VERSION_1 | ext |     yieldid     | EMU |   chipid(4:2)   |
*/
    return GP_WB_READ_STANDBY_VERSION();
}

void hal_InitVersionInfo(void)
{
    //emulation         assert if build is not compatible with device
    //chipid            assert if build is not compatible with device
    //chipidext         assert if build is not compatible with device
    //chipversion       read from device; assert when < GPHAL_MIN_CHIPVERSION; ignored GP_HAL_EXPECTED_CHIP_VERSION
    //yieldid           no impact on SW
    //romblversion      read from device; ignored GP_HAL_EXPECTED_ROM_BL_VERSION
    //rtflashversion    read from device; assert when < GPHAL_MIN_RTSYSTEMVERSION; in case of flash assert when != GP_DIVERSITY_RT_SYSTEM_IN_FLASH_VERSION

    UInt16 HWversion = gpHal_GetHWVersionId();

    //>>> CHIP EMULATION (bit 11)
    GP_ASSERT_SYSTEM(false == BIT_TST(HWversion, 11));

    //>>> !EMULATED CHIP ID (bit 6..10)
    GP_ASSERT_SYSTEM(GPHAL_CHIP_ID_XP4002 == BF(HWversion, 0x07C1, 6));
    //>>> !EMULATED CHIP ID EXT (bit 15)
    GP_ASSERT_SYSTEM(false == BIT_TST(HWversion, 15));
    //>>> !EMULATED CHIP ID (bit 14..12)
    /* Bits 14 to 12 are -on purpose- not checked. */
    /* These bits keep track of the yield improvement */
    /* updates for which _no_ SW changes are required */

    {
        //>>> !EMULATED CHIP VERSION (bit 0..5)
        gpHal_ChipVersion = BF(HWversion, 0x003F, 0);
        switch(gpHal_ChipVersion) //convert HW encoding in SW encoding
        {
            // valid ChipVersions are:
            case 1: // ES3, 10dbm
            case 2: // ES3, 20dBm
            case 3: // ES4, 10dbm
            case 4: // ES4, 20dBm
            case 5: // ES5, 10dbm
            case 6: // ES5, 20dBm
                break;
            default:
                gpHal_ChipVersion = 0xFF;
                break;
        }
    }
    GP_ASSERT_SYSTEM(gpHal_ChipVersion >= GPHAL_MIN_CHIPVERSION);

#ifdef GP_HAL_DIVERSITY_INCLUDE_IPC
    gpHal_RtInitVersionInfo();
#endif
}
