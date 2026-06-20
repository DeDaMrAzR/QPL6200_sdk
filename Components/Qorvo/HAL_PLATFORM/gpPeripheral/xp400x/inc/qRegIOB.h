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

#ifndef _Q_REG_IOB_H_
#define _Q_REG_IOB_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define REG_IOB_PIN_ALT_FUNC(PIN, PERIPH, PERIPH_INDEX, FUNC)                                                          \
    Q_CONCAT(GP_WB_ENUM_GPIO_, PIN, _ALTERNATES_, PERIPH, _, PERIPH_INDEX, _, FUNC)

#define REG_IOB_PIN_ALT(PIN, PERIPH, PERIPH_INDEX)                                                                     \
    Q_CONCAT(GP_WB_ENUM_GPIO_, PIN, _ALTERNATES_, PERIPH, _, PERIPH_INDEX)

#define Q_REG_IOB_PULL_MASK    GP_WB_IOB_GPIO_PULL_ENABLE_MASK
#define Q_REG_IOB_SCHMITT_MASK GP_WB_IOB_GPIO_SCHMITT_TRIGGER_MASK

/** @enum qRegIOB_DriveStrength_t */
//@{
#define qRegIOB_DriveStrength2ma  GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_2MA
#define qRegIOB_DriveStrength4ma  GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_4MA
#define qRegIOB_DriveStrength8ma  GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_8MA
#define qRegIOB_DriveStrength12ma GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_12MA
/** @typedef qRegIOB_DriveStrength_t
 *  @brief IOB drive strength.
 */
typedef UInt8 qRegIOB_DriveStrength_t;
//@}

/** @enum qRegIOB_Pull_t */
//@{
#define qRegIOB_PullDown GP_WB_ENUM_GPIO_PULL_MODE_PULL_DOWN
#define qRegIOB_PullUp   GP_WB_ENUM_GPIO_PULL_MODE_PULL_UP
/** @typedef qRegIOB_Pull_t
 *  @brief IOB input pull mode.
 */
typedef UInt8 qRegIOB_Pull_t;
//@}

/** @enum qRegIOB_SlewRate_t */
//@{
#define qRegIOB_SlewRateFast GP_WB_ENUM_GPIO_SLEW_RATE_FAST
#define qRegIOB_SlewRateSlow GP_WB_ENUM_GPIO_SLEW_RATE_SLOW
/** @typedef qRegIOB_SlewRate_t
 *  @brief IOB output slew rate.
 */
typedef UInt8 qRegIOB_SlewRate_t;
//@}

/** @brief Set output drive strength. */
static inline void qRegIOB_DriveStrengthSet(UInt8 iobIndex, qRegIOB_DriveStrength_t drive)
{
    UInt32 bitPos = iobIndex * GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_NBITS;
    UInt32 regAddress = GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_ADDRESS + Q_BYTEOFFSET(bitPos, sizeof(UInt32));

    bitPos %= sizeof(UInt32) * 8;
    drive &= GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_MASK;

    GP_WB_MWRITE_U32(regAddress, (GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_MASK << bitPos), (drive << bitPos));
}

/** @brief Set output drive strength. */
static inline qRegIOB_DriveStrength_t qRegIOB_DriveStrengthGet(UInt8 iobIndex)
{
    UInt32 bitPos = iobIndex * GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_NBITS;
    UInt32 regAddress = GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_ADDRESS + Q_BYTEOFFSET(bitPos, sizeof(UInt32));

    bitPos %= sizeof(UInt32) * 8;

    return (qRegIOB_DriveStrength_t)((GP_WB_READ_U32(regAddress) >> bitPos) & GP_WB_IOB_GPIO_0_DRIVE_STRENGTH_MASK);
}

static inline void qRegIOB_InputBufferEnableSet(UInt8 iobIndex, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_ADDRESS, (1UL << iobIndex), (enable << iobIndex));
}

static inline void qRegIOB_InputBufferMaskEnableSet(UInt32 iobMask, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_ADDRESS, iobMask, (enable ? iobMask : 0));
}

static inline Bool qRegIOB_InputBufferEnableGet(UInt8 iobIndex)
{
    return (GP_WB_READ_U32(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_ADDRESS) & (1UL << iobIndex)) ? true : false;
}

static inline void qRegIOB_PullMaskEnableSet(UInt32 iobMask, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_PULL_ENABLE_ADDRESS, iobMask, (enable ? iobMask : 0));
}

static inline void qRegIOB_PullEnableSet(UInt8 iobIndex, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_PULL_ENABLE_ADDRESS, (1UL << iobIndex), (enable << iobIndex));
}

static inline Bool qRegIOB_PullEnableGet(UInt8 iobIndex)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_IOB_GPIO_PULL_ENABLE_ADDRESS) >> iobIndex) & 1UL);
}

static inline void qRegIOB_PullMaskModeSet(UInt32 iobMask, qRegIOB_Pull_t pull)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_PULL_MODE_ADDRESS, iobMask, (pull ? iobMask : 0));
}

static inline void qRegIOB_PullModeSet(UInt8 iobIndex, qRegIOB_Pull_t pull)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_PULL_MODE_ADDRESS, (1UL << iobIndex), ((pull & 1UL) << iobIndex));
}

static inline qRegIOB_Pull_t qRegIOB_PullModeGet(UInt8 iobIndex)
{
    return (qRegIOB_Pull_t)((GP_WB_READ_U32(GP_WB_IOB_GPIO_PULL_MODE_ADDRESS) >> iobIndex) & 1UL);
}

static inline void qRegIOB_SchmittTriggerEnableSet(UInt8 iobIndex, Bool enable)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_SCHMITT_TRIGGER_ADDRESS, (1UL << iobIndex), (enable << iobIndex));
}

static inline Bool qRegIOB_SchmittTriggerEnableGet(UInt8 iobIndex)
{
    return (Bool)((GP_WB_READ_U32(GP_WB_IOB_GPIO_SCHMITT_TRIGGER_ADDRESS) >> iobIndex) & 1UL);
}

static inline void qRegIOB_SlewRateSet(UInt8 iobIndex, qRegIOB_SlewRate_t slewRate)
{
    GP_WB_MWRITE_U32(GP_WB_IOB_GPIO_SLEW_RATE_ADDRESS, (GP_WB_IOB_GPIO_0_SLEW_RATE_MASK << iobIndex),
                     (slewRate << iobIndex));
}

static inline qRegIOB_SlewRate_t qRegIOB_SlewRateGet(UInt8 iobIndex)
{
    return (qRegIOB_SlewRate_t)((GP_WB_READ_U32(GP_WB_IOB_GPIO_SLEW_RATE_ADDRESS) >> iobIndex) &
                                GP_WB_IOB_GPIO_0_SLEW_RATE_MASK);
}

static inline void qRegIOB_AlternateEnableSet(UInt8 iobIndex, Bool enable)
{
    UInt32 addressOffset = iobIndex * sizeof(uint32_t);
    UInt32 alternateAddress = GP_WB_IOB_GPIO_0_ALTERNATE_CFG_ADDRESS + addressOffset;

    GP_WB_MWRITE_U32(alternateAddress, GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_MASK, enable);
}

static inline Bool qRegIOB_AlternateEnableGet(UInt8 iobIndex)
{
    UInt32 addressOffset = iobIndex * sizeof(uint32_t);
    UInt32 alternateAddress = GP_WB_IOB_GPIO_0_ALTERNATE_CFG_ADDRESS + addressOffset;

    return (Bool)(GP_WB_READ_U32(alternateAddress) & GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_MASK);
}

static inline void qRegIOB_AlternateSet(UInt8 iobIndex, UInt8 alternate)
{
    UInt32 addressOffset = iobIndex * sizeof(uint32_t);
    UInt32 alternateAddress = GP_WB_IOB_GPIO_0_ALTERNATE_CFG_ADDRESS + addressOffset;

    GP_WB_MWRITE_U32(alternateAddress, GP_WB_IOB_GPIO_0_ALTERNATE_MASK, alternate << GP_WB_IOB_GPIO_0_ALTERNATE_LSB);
}

static inline UInt8 qRegIOB_AlternateGet(UInt8 iobIndex)
{
    UInt32 addressOffset = iobIndex * sizeof(uint32_t);
    UInt32 alternateAddress = GP_WB_IOB_GPIO_0_ALTERNATE_CFG_ADDRESS + addressOffset;

    return ((GP_WB_READ_U32(alternateAddress) & GP_WB_IOB_GPIO_0_ALTERNATE_MASK) >> GP_WB_IOB_GPIO_0_ALTERNATE_LSB);
}

static inline void qRegIOB_IRLedPowerSet(Bool enable)
{
    GP_WB_WRITE_IOB_IRLED_PUP(enable);
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_IOB_H_ */
