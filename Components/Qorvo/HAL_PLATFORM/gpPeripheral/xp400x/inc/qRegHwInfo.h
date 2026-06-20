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

#ifndef _Q_REG_HWINFO_H_
#define _Q_REG_HWINFO_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @enum qRegHwInfo_Package_t */
//@{
#define qRegHwInfo_PackageIDDefault         0
#define qRegHwInfo_PackageIDDefault1        1
#define qRegHwInfo_PackageID40Pin10dbm      2
#define qRegHwInfo_PackageID40Pin20dbm      3
#define qRegHwInfo_PackageID32Pin10dbm2Anio 4
#define qRegHwInfo_PackageID32Pin20dbm2Anio 5
#define qRegHwInfo_PackageID32Pin10dbm3Anio 6
#define qRegHwInfo_PackageID32Pin20dbm3Anio 7
/** @typedef qRegHwInfo_Package_t
 *  @brief HW info package.
 */
typedef UInt8 qRegHwInfo_Package_t;
//@}

/** @enum qRegHwInfo_OutPower_t */
//@{
#define qRegHwInfo_OutPower10dbm 0
#define qRegHwInfo_OutPower20dbm 1
/** @typedef qRegHwInfo_OutPower_t
 *  @brief HW info package output power.
 */
typedef UInt8 qRegHwInfo_OutPower_t;
//@}

/** @enum qRegHwInfo_PinCount_t */
//@{
#define qRegHwInfo_PinCount32 32
#define qRegHwInfo_PinCount40 40
/** @typedef qRegHwInfo_PinCount_t
 *  @brief HW info package pin count.
 */
typedef UInt8 qRegHwInfo_PinCount_t;
//@}

/** @brief Get HW info package. */
qRegHwInfo_Package_t qRegHwInfo_PackageGet(void);

/** @brief Get the debug pin mask. */
uint32_t qRegHwInfo_DebugPinMaskGet(void);

/** @brief Get HW info package output power. */
static inline qRegHwInfo_OutPower_t qRegHwInfo_OutPowerGet(qRegHwInfo_Package_t package)
{
    switch(package)
    {
        case qRegHwInfo_PackageID40Pin10dbm:
        case qRegHwInfo_PackageID32Pin10dbm2Anio:
        case qRegHwInfo_PackageID32Pin10dbm3Anio:
            return qRegHwInfo_OutPower10dbm;
        case qRegHwInfo_PackageID40Pin20dbm:
        case qRegHwInfo_PackageID32Pin20dbm2Anio:
        case qRegHwInfo_PackageID32Pin20dbm3Anio:
            return qRegHwInfo_OutPower20dbm;
        default:
            return qRegHwInfo_OutPower10dbm;
    }
}

/** @brief Get HW info package ANIO mask. */
static inline UInt8 qRegHwInfo_AnioMaskGet(qRegHwInfo_Package_t package)
{
    switch(package)
    {
        case qRegHwInfo_PackageIDDefault:
        case qRegHwInfo_PackageIDDefault1:
        case qRegHwInfo_PackageID40Pin10dbm:
        case qRegHwInfo_PackageID40Pin20dbm:
            return 0b1111;
        case qRegHwInfo_PackageID32Pin10dbm2Anio:
        case qRegHwInfo_PackageID32Pin20dbm2Anio:
            return 0b0011;
        case qRegHwInfo_PackageID32Pin10dbm3Anio:
        case qRegHwInfo_PackageID32Pin20dbm3Anio:
            return 0b0111;
        default:
            return 0b1111;
    }
}

/** @brief Get HW info package pin count. */
static inline qRegHwInfo_PinCount_t qRegHwInfo_PinCountGet(qRegHwInfo_Package_t package)
{
    switch(package)
    {
        case qRegHwInfo_PackageIDDefault:
        case qRegHwInfo_PackageIDDefault1:
        case qRegHwInfo_PackageID40Pin10dbm:
        case qRegHwInfo_PackageID40Pin20dbm:
            return qRegHwInfo_PinCount40;
        case qRegHwInfo_PackageID32Pin10dbm2Anio:
        case qRegHwInfo_PackageID32Pin20dbm2Anio:
        case qRegHwInfo_PackageID32Pin10dbm3Anio:
        case qRegHwInfo_PackageID32Pin20dbm3Anio:
            return qRegHwInfo_PinCount32;
        default:
            return qRegHwInfo_PinCount40;
    }
}

/** @brief Get HW info package input mask. */
static inline UInt32 qRegHwInfo_InputMaskGet(qRegHwInfo_Package_t package)
{
    switch(package)
    {
        case qRegHwInfo_PackageIDDefault:
        case qRegHwInfo_PackageIDDefault1:
        case qRegHwInfo_PackageID40Pin10dbm:
            return 0x3FFFFFFF; /* All available pins */
        case qRegHwInfo_PackageID40Pin20dbm:
            return 0x3FFBFFFF; /* Without: 18 */
        case qRegHwInfo_PackageID32Pin10dbm2Anio:
        case qRegHwInfo_PackageID32Pin10dbm3Anio:
            return 0x363C1FFF; /* Without: 13-17, 22-24, 27 */
        case qRegHwInfo_PackageID32Pin20dbm2Anio:
        case qRegHwInfo_PackageID32Pin20dbm3Anio:
            return 0x36381FFF; /* Without: 13-18, 22-24, 27 */
        default:
            return 0x00000000;
    }
}

/** @brief Get HW info package output mask. */
static inline UInt32 qRegHwInfo_OutputMaskGet(qRegHwInfo_Package_t package)
{
    switch(package)
    {
        case qRegHwInfo_PackageIDDefault:
        case qRegHwInfo_PackageIDDefault1:
        case qRegHwInfo_PackageID40Pin10dbm:
            return 0x01FFFFFF; /* Pins: 0-24 */
        case qRegHwInfo_PackageID40Pin20dbm:
            return 0x01FBFFFF; /* Without: 18. */
        case qRegHwInfo_PackageID32Pin10dbm2Anio:
        case qRegHwInfo_PackageID32Pin10dbm3Anio:
            return 0x003C1FFF; /* Without: 13-17, 22-24 */
        case qRegHwInfo_PackageID32Pin20dbm2Anio:
        case qRegHwInfo_PackageID32Pin20dbm3Anio:
            return 0x00381FFF; /* Without: 13-18, 22-24 */
        default:
            return 0x00000000;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_HWINFO_H_ */
