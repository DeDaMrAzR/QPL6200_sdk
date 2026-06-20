/*
 * Copyright (c) 2024, Qorvo Inc
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

#include "qRegHwInfo.h"
#include "qRegInfoPage.h"

/** @brief Debug pin mask in SWD/JTAG mode. */
#define Q_REG_HWINFO_DEBUG_PINS_ALL 0b1111UL

/** @brief Debug pin mask in SWD-only mode. */
#define Q_REG_HWINFO_DEBUG_PINS_SWDONLY 0b0011UL

qRegHwInfo_Package_t qRegHwInfo_PackageGet(void)
{
#if defined(REG_HWINFO_PACKAGE_ID)
    return REG_HWINFO_PACKAGE_ID;
#else
    return (qRegHwInfo_Package_t)gpHal_InfoPagePackageId();
#endif
}

uint32_t qRegHwInfo_DebugPinMaskGet(void)
{
    UInt8 pinsel = gpHal_InfoPageDebugPins();
    Bool swdonly = gpHal_InfoPageDebugPinsSWDOnly();

    uint32_t mask = swdonly ? Q_REG_HWINFO_DEBUG_PINS_SWDONLY : Q_REG_HWINFO_DEBUG_PINS_ALL;

    switch(pinsel)
    {
        case INFOPAGE_DEBUG_PINOUT_6_9:
            mask <<= 6;
            break;

        case INFOPAGE_DEBUG_PINOUT_12_15:
            mask <<= 12;
            break;

        case INFOPAGE_DEBUG_PINOUT_1_4:
            mask <<= 1;
            break;

        default:
            mask = 0;
    }

    return mask;
}
