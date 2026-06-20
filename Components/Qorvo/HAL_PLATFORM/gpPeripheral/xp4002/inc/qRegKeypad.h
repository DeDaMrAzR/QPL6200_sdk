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

#ifndef _Q_REG_KEYPAD_H_
#define _Q_REG_KEYPAD_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Keypad peripheral base address type. */
typedef KEYPAD_REGMAP_T* qRegKeypad_t;

#define KEYPAD_0_BASE_ADDRESS GP_WB_KEYPAD_BASE_ADDRESS
#define KEYPAD_0_IRQn         KEYPAD_IRQn

/** @brief Get the Keypad peripheral base address by ID. */
#define Q_REG_KEYPAD_BASEADDR_GET(ID) ((qRegKeypad_t)Q_CONCAT(KEYPAD_, ID, _BASE_ADDRESS))

/** @brief Get the Keypad peripheral interrupt number. */
#define Q_REG_KEYPAD_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(KEYPAD_, ID, _IRQn))

/** @brief Active columns charge period selection disabled. */
#define Q_REG_KEYPAD_CHARGE_PERIOD_DISABLED 0xFF

/** @enum qRegKeypad_DetectedKeys_t */
//@{
#define qRegKeypad_DetectedKeysNone        0
#define qRegKeypad_DetectedKeysOne         1
#define qRegKeypad_DetectedKeysTwo         2
#define qRegKeypad_DetectedKeysMoreThenTwo 3
/** @typedef qRegKeypad_DetectedKeys_t
 *  @brief Number of detected keys.
 */
typedef UInt8 qRegKeypad_DetectedKeys_t;
//@}

/** @enum qRegKeypad_QuicActionType_t */
//@{
/** @brief Action none. */
#define qRegKeypad_QuicActionTypeNone GP_WB_ENUM_KEYPAD_ACTION_NONE
/** @brief Action trigger single scan. */
#define qRegKeypad_QuicActionTypeScan GP_WB_ENUM_KEYPAD_ACTION_DO_SCAN
/** @typedef qRegKeypad_QuicActionType_t
 *  @brief Keypad action.
 */
typedef UInt8 qRegKeypad_QuicActionType_t;
//@}

/** @enum qRegKeypad_Irq_t */
//@{
#define qRegKeypad_IrqScanDone GP_WB_INT_CTRL_MASK_APPUC_KEYPAD_SCAN_DONE_INTERRUPT_LSB
/** @typedef qRegKeypad_Irq_t
 *  @brief Keypad Interrupt.
 */
typedef UInt8 qRegKeypad_Irq_t;
//@}

/** @brief Set the Keypad enable mode. */
static inline void qRegKeypad_EnableSet(qRegKeypad_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->keypad_config.reg32[0] |= GP_WB_KEYPAD_KEYPAD_ENABLE_MASK;
    }
    else
    {
        baseAddr->keypad_config.reg32[0] &= ~GP_WB_KEYPAD_KEYPAD_ENABLE_MASK;
    }
}

/** @brief Get the Keypad enable mode. */
static inline Bool qRegKeypad_EnableGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_config.reg32[0] & GP_WB_KEYPAD_KEYPAD_ENABLE_MASK) ? true : false;
}

/** @brief When true the scan outputs are driven to 0 during active to allow new keypad press to be detected. */
static inline void qRegKeypad_SensitiveDuringActiveEnableSet(qRegKeypad_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->keypad_config.reg32[0] |= GP_WB_KEYPAD_SENSITIVE_DURING_ACTIVE_MASK;
    }
    else
    {
        baseAddr->keypad_config.reg32[0] &= ~GP_WB_KEYPAD_SENSITIVE_DURING_ACTIVE_MASK;
    }
}

/** @brief Get the sensitive during active status. */
static inline Bool qRegKeypad_SensitiveDuringActiveEnableGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_config.reg32[0] & GP_WB_KEYPAD_SENSITIVE_DURING_ACTIVE_MASK) ? true : false;
}

/** @brief When true the scan outputs are driven to 0 after the real measurement (debug). */
static inline void qRegKeypad_ProlongedRowDriveEnableSet(qRegKeypad_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->keypad_config.reg32[0] |= GP_WB_KEYPAD_PROLONGED_ROW_DRIVE_MASK;
    }
    else
    {
        baseAddr->keypad_config.reg32[0] &= ~GP_WB_KEYPAD_PROLONGED_ROW_DRIVE_MASK;
    }
}

/** @brief Get the prolonged row drive status. */
static inline Bool qRegKeypad_ProlongedRowDriveEnableGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_config.reg32[0] & GP_WB_KEYPAD_PROLONGED_ROW_DRIVE_MASK) ? true : false;
}

/** @brief Set the Keypad SSO spread enable mode. */
static inline void qRegKeypad_SsoSpreadEnableSet(qRegKeypad_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->keypad_config.reg32[0] |= GP_WB_KEYPAD_SSO_SPREAD_ENABLE_MASK;
    }
    else
    {
        baseAddr->keypad_config.reg32[0] &= ~GP_WB_KEYPAD_SSO_SPREAD_ENABLE_MASK;
    }
}

/** @brief Get the Keypad SSO spread enable mode. */
static inline Bool qRegKeypad_SsoSpreadEnableGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_config.reg32[0] & GP_WB_KEYPAD_SSO_SPREAD_ENABLE_MASK) ? true : false;
}

/** @brief Set the Keypad clock gating enable mode. */
static inline void qRegKeypad_ClkGatingEnableSet(qRegKeypad_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->keypad_config.reg32[0] |= GP_WB_KEYPAD_ENABLE_CLK_KEYPAD_BY_UC_MASK;
    }
    else
    {
        baseAddr->keypad_config.reg32[0] &= ~GP_WB_KEYPAD_ENABLE_CLK_KEYPAD_BY_UC_MASK;
    }
}

/** @brief Get the Keypad clock gating enable mode. */
static inline Bool qRegKeypad_ClkGatingEnableGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_config.reg32[0] & GP_WB_KEYPAD_ENABLE_CLK_KEYPAD_BY_UC_MASK) ? true : false;
}

/** @brief Set the Keypad active columns charge period. */
static inline void qRegKeypad_ActiveColumnsChargePeriodSet(qRegKeypad_t baseAddr, UInt8 activeColumnsChargePeriod)
{
    baseAddr->active_columns_charge_period.reg32[0] =
        (activeColumnsChargePeriod << GP_WB_KEYPAD_ACTIVE_COLUMNS_CHARGE_PERIOD_LSB) &
        GP_WB_KEYPAD_ACTIVE_COLUMNS_CHARGE_PERIOD_MASK;
}

/** @brief Get the Keypad active columns charge period. */
static inline UInt8 qRegKeypad_ActiveColumnsChargePeriodGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->active_columns_charge_period.reg32[0] & GP_WB_KEYPAD_ACTIVE_COLUMNS_CHARGE_PERIOD_MASK) >>
           GP_WB_KEYPAD_ACTIVE_COLUMNS_CHARGE_PERIOD_LSB;
}

/** @brief Set the Keypad passive columns charge period. */
static inline void qRegKeypad_PassiveColumnsChargePeriodSet(qRegKeypad_t baseAddr, UInt8 passiveColumnsChargePeriod)
{
    baseAddr->passive_columns_charge_period.reg32[0] =
        (passiveColumnsChargePeriod << GP_WB_KEYPAD_PASSIVE_COLUMNS_CHARGE_PERIOD_LSB) &
        GP_WB_KEYPAD_PASSIVE_COLUMNS_CHARGE_PERIOD_MASK;
}

/** @brief Get the Keypad passive columns charge period. */
static inline UInt8 qRegKeypad_PassiveColumnsChargePeriodGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->passive_columns_charge_period.reg32[0] & GP_WB_KEYPAD_PASSIVE_COLUMNS_CHARGE_PERIOD_MASK) >>
           GP_WB_KEYPAD_PASSIVE_COLUMNS_CHARGE_PERIOD_LSB;
}

/** @brief Set the Keypad row drive period. */
static inline void qRegKeypad_RowDrivePeriodSet(qRegKeypad_t baseAddr, UInt8 rowDrivePeriod)
{
    baseAddr->row_drive_period.reg32[0] =
        (rowDrivePeriod << GP_WB_KEYPAD_ROW_DRIVE_PERIOD_LSB) & GP_WB_KEYPAD_ROW_DRIVE_PERIOD_MASK;
}

/** @brief Get the Keypad row drive period. */
static inline UInt8 qRegKeypad_RowDrivePeriodGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->row_drive_period.reg32[0] & GP_WB_KEYPAD_ROW_DRIVE_PERIOD_MASK) >>
           GP_WB_KEYPAD_ROW_DRIVE_PERIOD_LSB;
}

/** @brief Get the Keypad scan busy status. */
static inline Bool qRegKeypad_ScanBusyGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_state.reg32[0] & GP_WB_KEYPAD_KEYPAD_SCAN_BUSY_MASK) ? true : false;
}

/** @brief Get the Keypad scan state status. */
static inline UInt8 qRegKeypad_ScanStateGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_state.reg32[0] & GP_WB_KEYPAD_KEYPAD_SCAN_STATE_MASK) >>
           GP_WB_KEYPAD_KEYPAD_SCAN_STATE_LSB;
}

/** @brief Get the Keypad scan counter status. */
static inline UInt8 qRegKeypad_ScanCounterGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_state.reg32[0] & GP_WB_KEYPAD_KEYPAD_SCAN_COUNTER_MASK) >>
           GP_WB_KEYPAD_KEYPAD_SCAN_COUNTER_LSB;
}

/** @brief Get the Keypad result updated status. */
static inline Bool qRegKeypad_ResultsUpdatedGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_status.reg32[0] & GP_WB_KEYPAD_RESULTS_UPDATED_MASK) ? true : false;
}

/** @brief Get the Keypad result changed status. */
static inline Bool qRegKeypad_ResultsChangedGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_status.reg32[0] & GP_WB_KEYPAD_RESULTS_CHANGED_MASK) ? true : false;
}

/** @brief Get the Keypad results valid status. */
static inline Bool qRegKeypad_ResultsValidGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_status.reg32[0] & GP_WB_KEYPAD_RESULTS_VALID_MASK) ? true : false;
}

/** @brief Get the Keypad scans results. */
static inline UInt8 qRegKeypad_ScansGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_scans.reg32[0] & GP_WB_KEYPAD_KEYPAD_SCANS_MASK) >> GP_WB_KEYPAD_KEYPAD_SCANS_LSB;
}

/** @brief Get the Keypad senses active charge results. */
static inline UInt8 qRegKeypad_SensesActiveChargeGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_senses_active_charge.reg32[0] & GP_WB_KEYPAD_KEYPAD_SENSES_ACTIVE_CHARGE_MASK) >>
           GP_WB_KEYPAD_KEYPAD_SENSES_ACTIVE_CHARGE_LSB;
}

/** @brief Get the Keypad senses results. */
static inline UInt8 qRegKeypad_SensesGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_senses.reg32[0] & GP_WB_KEYPAD_KEYPAD_SENSES_MASK) >> GP_WB_KEYPAD_KEYPAD_SENSES_LSB;
}

/** @brief Clear the Keypad results updated status. */
static inline void qRegKeypad_ClearResultsUpdated(qRegKeypad_t baseAddr)
{
    baseAddr->keypad_control.reg32[0] = GP_WB_KEYPAD_CLEAR_RESULTS_UPDATED_MASK;
}

/** @brief Clear the Keypad results changed status. */
static inline void qRegKeypad_ClearResultsChanged(qRegKeypad_t baseAddr)
{
    baseAddr->keypad_control.reg32[0] = GP_WB_KEYPAD_CLEAR_RESULTS_CHANGED_MASK;
}

/** @brief Clear the Keypad results valid status. */
static inline void qRegKeypad_ClearResultsValid(qRegKeypad_t baseAddr)
{
    baseAddr->keypad_control.reg32[0] = GP_WB_KEYPAD_CLEAR_RESULTS_VALID_MASK;
}

/** @brief Get the Keypad results stable counter. */
static inline UInt8 qRegKeypad_ResultsStableCounterGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->results_stable_counter.reg32[0] & GP_WB_KEYPAD_RESULTS_STABLE_COUNTER_MASK) >>
           GP_WB_KEYPAD_RESULTS_STABLE_COUNTER_LSB;
}

static inline qRegKeypad_DetectedKeys_t qRegKeypad_DetectedKeysGet(qRegKeypad_t baseAddr)
{
    return (qRegKeypad_DetectedKeys_t)((baseAddr->keypad_result_summary.reg32[0] & GP_WB_KEYPAD_NUMBER_OF_KEYS_MASK) >>
                                       GP_WB_KEYPAD_NUMBER_OF_KEYS_LSB);
}

/** @brief Get the Keypad single key scan (row) number (0...7). */
static inline UInt8 qRegKeypad_SingleKeyRowGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_result_summary.reg32[0] & GP_WB_KEYPAD_SINGLE_KEY_SCAN_MASK) >>
           GP_WB_KEYPAD_SINGLE_KEY_SCAN_LSB;
}

/** @brief Get the Keypad single key sense (column) number (0...7). */
static inline UInt8 qRegKeypad_SingleKeyColumnGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->keypad_result_summary.reg32[0] & GP_WB_KEYPAD_SINGLE_KEY_SENSE_MASK) >>
           GP_WB_KEYPAD_SINGLE_KEY_SENSE_LSB;
}

/** @brief Get the Keypad key vector. */
static inline UInt8 qRegKeypad_KeyVectorGet(qRegKeypad_t baseAddr, UInt8 index)
{
    UInt32* vector = (UInt32*)&baseAddr->key_vector_0;
    return vector[index] & GP_WB_KEYPAD_KEY_VECTOR_0_MASK;
}

/** @brief Check the Keypad scan done unmasked interrupt. */
static inline Bool qRegKeypad_IRQUnmaskedScanDoneCheck(qRegKeypad_t baseAddr)
{
    return baseAddr->unmasked_scan_done_interrupt.reg32[0] & GP_WB_KEYPAD_UNMASKED_SCAN_DONE_INTERRUPT_MASK ? true
                                                                                                            : false;
}

/** @brief Trigger the Keypad scan. */
static inline void qRegKeypad_ScanTrigger(qRegKeypad_t baseAddr)
{
    baseAddr->unnamed_reg_keypad_0x0054.reg32[0] = GP_WB_KEYPAD_DO_SCAN_MASK;
}

/** @brief Clear the Keypad scan done  interrupt. */
static inline void qRegKeypad_IRQScanDoneClear(qRegKeypad_t baseAddr)
{
    baseAddr->unnamed_reg_keypad_0x0054.reg32[0] = GP_WB_KEYPAD_CLR_SCAN_DONE_INTERRUPT_MASK;
}

/** @brief Set the Keypad QUIC action channel. */
static inline void qRegKeypad_QuicActionChannelSet(qRegKeypad_t baseAddr, UInt32 channel)
{
    baseAddr->quic_cfg_0.reg32[0] &= ~GP_WB_KEYPAD_QUIC_ACTION_CHANNEL_MASK;
    baseAddr->quic_cfg_0.reg32[0] |=
        (channel << GP_WB_KEYPAD_QUIC_ACTION_CHANNEL_LSB) & GP_WB_KEYPAD_QUIC_ACTION_CHANNEL_MASK;
}

/** @brief Get the Keypad QUIC action channel. */
static inline UInt32 qRegKeypad_QuicActionChannelGet(qRegKeypad_t baseAddr)
{
    return (baseAddr->quic_cfg_0.reg32[0] & GP_WB_KEYPAD_QUIC_ACTION_CHANNEL_MASK) >>
           GP_WB_KEYPAD_QUIC_ACTION_CHANNEL_LSB;
}

/** @brief Set the Keypad QUIC action type. */
static inline void qRegKeypad_QuicActionTypeSet(qRegKeypad_t baseAddr, qRegKeypad_QuicActionType_t quicActionType)
{
    baseAddr->quic_cfg_0.reg32[0] &= ~GP_WB_KEYPAD_QUIC_ACTION_TYPE_MASK;
    baseAddr->quic_cfg_0.reg32[0] |=
        (quicActionType << GP_WB_KEYPAD_QUIC_ACTION_TYPE_LSB) & GP_WB_KEYPAD_QUIC_ACTION_TYPE_MASK;
}

/** @brief Get the Keypad QUIC action type. */
static inline qRegKeypad_QuicActionType_t qRegKeypad_QuicActionTypeGet(qRegKeypad_t baseAddr)
{
    return (qRegKeypad_QuicActionType_t)((baseAddr->quic_cfg_0.reg32[0] & GP_WB_KEYPAD_QUIC_ACTION_TYPE_MASK) >>
                                         GP_WB_KEYPAD_QUIC_ACTION_TYPE_LSB);
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_KEYPAD_H_ */
