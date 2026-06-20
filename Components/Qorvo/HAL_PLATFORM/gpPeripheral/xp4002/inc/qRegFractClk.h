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

#ifndef _Q_REG_FRACT_CLK_H_
#define _Q_REG_FRACT_CLK_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Fractional Clock Generator peripheral base address type. */
struct qRegFractClk_s {
    __IO UInt32 t_target;
    __IO UInt32 enable;
};

typedef struct qRegFractClk_s* qRegFractClk_t;

/** @brief Fractional Clock Generator 0 base address. */
#define GP_WB_FRACT_CLK_0_BASE_ADDRESS GP_WB_FRACT_CLK_GEN_T_TARGET_0_ADDRESS

/** @brief Fractional Clock Generator 1 base address. */
#define GP_WB_FRACT_CLK_1_BASE_ADDRESS GP_WB_FRACT_CLK_GEN_T_TARGET_1_ADDRESS

/** @brief Get the Fractional Clock Generator peripheral base address by ID. */
#define Q_REG_FRACT_CLK_BASEADDR_GET(ID) ((qRegFractClk_t)(Q_CONCAT(GP_WB_FRACT_CLK_, ID, _BASE_ADDRESS)))

/** @brief Set the Fractional Clock Generator t_target 0. */
static inline void qRegFractClk_TTargetSet(qRegFractClk_t baseAddr, UInt32 tTarget)
{
    baseAddr->t_target = tTarget;
}

/** @brief Get the Fractional Clock Generator t_target 0. */
static inline UInt32 qRegFractClk_TTargetGet(qRegFractClk_t baseAddr)
{
    return baseAddr->t_target;
}

/** @brief Enable or disable the Fractional Clock Generator t_target 0. */
static inline void qRegFractClk_TTargetEnableSet(qRegFractClk_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->enable |= GP_WB_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0_MASK;
    }
    else
    {
        baseAddr->enable &= ~GP_WB_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0_MASK;
    }
}

/** @brief Get the Fractional Clock Generator t_target 0 enable status. */
static inline Bool qRegFractClk_TTargetEnableGet(qRegFractClk_t baseAddr)
{
    return (baseAddr->enable & GP_WB_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0_MASK) ? true : false;
}

#ifdef __cplusplus
}
#endif

#endif // _Q_REG_FRACT_CLK_H_
