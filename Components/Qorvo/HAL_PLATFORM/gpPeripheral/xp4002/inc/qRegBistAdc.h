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

#ifndef _Q_REG_BISTADC_H_
#define _Q_REG_BISTADC_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @enum qRegBistAdc_QuicActionType_t */
//@{
#define qRegBistAdc_QuicActionNone      GP_WB_ENUM_BIST_ACTION_NONE
#define qRegBistAdc_QuicActionStart     GP_WB_ENUM_BIST_ACTION_ADC_START
#define qRegBistAdc_QuicActionContStart GP_WB_ENUM_BIST_ACTION_ADC_CONT_START
#define qRegBistAdc_QuicActionContStop  GP_WB_ENUM_BIST_ACTION_ADC_CONT_STOP
/** @typedef qRegBistAdc_QuicActionType_t
 *  @brief BistAdc QUIC action type.
 */
typedef UInt8 qRegBistAdc_QuicActionType_t;
//@}

/** @brief Get the retain bit position for a given peripheral.*/
#define Q_REG_BIST_QUIC_ACTION_CHANNEL_ADDRES(actionIdx)                                                               \
    ((GP_WB_BIST_QUIC_ACTION_CHANNEL_1_ADDRESS - GP_WB_BIST_QUIC_ACTION_CHANNEL_0_ADDRESS) * actionIdx)

static inline void qRegBistAdc_QuicChannelSet(UInt32 actionIdx, UInt32 quicChannel)
{
    size_t addr = GP_WB_BIST_QUIC_ACTION_CHANNEL_0_ADDRESS + Q_REG_BIST_QUIC_ACTION_CHANNEL_ADDRES(actionIdx);
    GP_WB_MWRITE_U32(addr, GP_WB_BIST_QUIC_ACTION_CHANNEL_0_MASK, quicChannel << GP_WB_BIST_QUIC_ACTION_CHANNEL_0_LSB);
}

static inline UInt32 qRegBistAdc_QuicChannelGet(UInt32 actionIdx)
{
    size_t addr = GP_WB_BIST_QUIC_ACTION_CHANNEL_0_ADDRESS + Q_REG_BIST_QUIC_ACTION_CHANNEL_ADDRES(actionIdx);
    return (GP_WB_READ_U32(addr) & GP_WB_BIST_QUIC_ACTION_CHANNEL_0_MASK) >> GP_WB_BIST_QUIC_ACTION_CHANNEL_0_LSB;
}

static inline void qRegBistAdc_QuicActionTypeSet(UInt32 actionIdx, qRegBistAdc_QuicActionType_t actionType)
{
    size_t addr = GP_WB_BIST_QUIC_ACTION_CHANNEL_0_ADDRESS + Q_REG_BIST_QUIC_ACTION_CHANNEL_ADDRES(actionIdx);
    GP_WB_MWRITE_U32(addr, GP_WB_BIST_QUIC_ACTION_TYPE_0_MASK, actionType << GP_WB_BIST_QUIC_ACTION_TYPE_0_LSB);
}

static inline void qRegBistAdc_Trigger(void)
{
    // start the measurement from software.
    GP_WB_BIST_ADC_START();
}

static inline Bool qRegBistAdc_ReadyCheck(void)
{
    return GP_WB_READ_BIST_ADC_ACCU_RESULT_VALID();
}

static inline UInt32 qRegBistAdc_ResultGet(void)
{
    return GP_WB_READ_BIST_ADC_ACCU_RESULT();
}

#ifdef __cplusplus
}
#endif

#endif //_Q_REG_BISTADC_H_
