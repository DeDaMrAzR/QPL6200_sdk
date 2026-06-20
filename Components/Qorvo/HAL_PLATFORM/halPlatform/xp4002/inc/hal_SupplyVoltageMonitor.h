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

#ifndef _HAL_SUPPLY_VOLTAGE_MONITOR_H_
#define _HAL_SUPPLY_VOLTAGE_MONITOR_H_

#include "global.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief Invalid supply voltage. */
#define HAL_BATTERY_VOLTAGE_INVALID 0xFFFFFFF

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize supply voltage monitoring.
 *
 */
void hal_SupplyVoltageMonitorInit(void);

/**
 * @brief Enables supply voltage measurement to be tirggered using QUIC during TX activity.
 *
 */
void hal_SupplyVoltageMonitorAnnounceTxStart(void);

/**
 * @brief Read the supply voltage measured during TX activity and disables BIST ADC.
 *
 */
void hal_SupplyVoltageMonitorAnnounceTxFinished(void);

/**
 * @brief Check whether the BIST ADC to be re-enabled after sleep for TX supply voltage measurement.
 *
 */
void hal_SupplyVoltageMonitorTxCheck(void);

/**
 * @brief Read current supply voltage.
 * This is a blocking call which takes around 90us with 192MHz main clock
 *
 * @return Current supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetBlocking(void);

/**
 * @brief Read the last measured supply voltage during the radio transmit (TX) activity.
 *
 * @return  Supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetTx(void);

/**
 * @brief Read the worst case supply voltage measured during TX or periodic.
 *
 * @return Minimum supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetMin(void);

/**
 * @brief Update calibration task periodic interval.
 *        Calibration framework granularity is 10ms, so interval should be in multiples of 10ms.
 *        This function must be called after @c void hal_SupplyVoltageMonitorInit()
 *
 *  @param interval Calibration task interval in microseconds.
 *
 */
void hal_SupplyVoltageMonitorSetInterval(UInt32 interval);
#ifdef __cplusplus
}
#endif

#endif //_HAL_SUPPLY_VOLTAGE_MONITOR_H_
