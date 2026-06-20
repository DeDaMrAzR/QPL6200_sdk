/*
 * Copyright (c) 2024-2025, Qorvo Inc
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

/** @file "App_Battery.h"
 *
 *  Application API
 *
 *  Declarations of the public functions and enumerations of App battery.
 */

#ifndef _APP_BATTERY_H_
#define _APP_BATTERY_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpBatteryMonitor.h"

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro Define for period of measurement update */
#define APP_BATTERY_UPDATE_PERIOD MS_TO_US(300000) // 5 mins

/** @brief Macro Define for an example battery voltage range */
#define APP_BATTERY_MEDIUM 66 //  Battery medium(66%)
#define APP_BATTERY_LOW    33 //  Battery low(33%)

/** @brief Macro Define for an example thresholds of battery voltage */
#define APP_BATTERY_FULL_THRESHOLD     320 // Battery full(100%)   in 10 mV - 3.2V
#define APP_BATTERY_MEDIUM_THRESHOLD   250 // Battery medium(66%)  in 10 mV - 2.5V
#define APP_BATTERY_LOW_THRESHOLD      230 // Battery low(33%)     in 10 mV - 2.3V
#define APP_BATTERY_CRITICAL_THRESHOLD 210 // Battery critical(0%) in 10 mV - 2.1V

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** @brief callback when device has updated its battery voltage and remaining percentage
 */
void Application_cbBatteryVoltageUpdated(UInt8 status);

/** @brief Function to retrieve Battery voltage value from system
 *
 *   @return volage                   low voltage threshold in 100mV/unit.
 */
UInt8 Application_GetLowBatteryVoltageThreshold(void);

/** @brief Function to retrieve Battery voltage value from system
 *
 *   @paramp pBatteryVoltage           measured battery voltage in 2.6 format
 *                                     - 2 bits V, 6 bits 1/64 volt (in 0.01 units).
 *   @return success                   True if valid temperature value is returned.
 */
Bool Application_GetBatteryVoltage(UQ2_6* pBatteryVoltage);

/** @brief example function to caluclate and update the battery remaining percentage
 *
 *   @param   batteryVoltage             Current battery voltage in 2.6 format
 *   @return  remainingPercentage        The battery remaining percentage in a 0.5% unit
 */
UInt8 Application_UpdateBatteryRemaining(UQ2_6 voltage);

/** @brief Function to update Power Confiugration cluster following attributes
 *       0x0020 BatteryVoltage attribute
 *       0x0021 BatteryPercentageRemainin attribute
 */
void Application_UpdateBattery(void);

/** @brief Function to start/restart periodic update battery voltage reading
 */
void Application_StartPeriodicBatteryUpdate(void);

/** @brief Function to stop periodic update battery voltage reading
 */
void Application_StopPeriodicBatteryUpdate(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_APP_H_
