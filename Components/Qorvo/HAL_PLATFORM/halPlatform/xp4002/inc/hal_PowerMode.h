
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

#ifndef _HAL_POWERMODE_H_
#define _HAL_POWERMODE_H_

/**
 * @defgroup HAL_PLATFORM low power module
 * @{
 *
 * @brief This module defines low power mode APIs
 *
 * @file hal_PowerMode.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @typedef hal_StandbyPowerMode_t
 *  @brief The hal_ClockAccuracy_t type defines the hal clock accuracy values in PPM.
 */
typedef uint16_t hal_ClockAccuracy_t;

/** @name hal_PowerModeResult_t */
//@{
/** @brief The function returned successful. */
#define hal_PowerModeResultSuccess 0x0
/** @brief An invalid parameter was given as a parameter to this function. */
#define hal_PowerModeResultInvalidParameter 0x1
/** @brief Generic error*/
#define hal_PowerModeResultError 0x2
/** @brief The GP chip is busy. */
#define hal_PowerModeResultBusy 0x7
/** @brief The request was invalid */
#define hal_PowerModeInvalidRequest 0xc2
/** @typedef hal_PowerModeResult_t
 *  @brief The hal_PowerModeResult_t  type defines the result of hal power modes functions.
 */
typedef uint8_t hal_PowerModeResult_t;
//@}

/** @name hal_StandbyPowerMode_t*/
//@{
/** @brief This is the LPS power mode, it uses the low power regulator and switches off the digital core */
#define hal_StandbyPowerModeLPS GP_WB_ENUM_STANDBY_POWER_MODE_LPS
/** @brief This is the LPS power mode, with DCDC enabled it uses the DCDCregulator and switches off the digital core */
#define hal_StandbyPowerModeELPS GP_WB_ENUM_STANDBY_POWER_MODE_ELPS
/** @brief  In this mode, both analog as digital supplies are on, in order to support the 32MHz based sleep clock */
#define hal_StandbyPowerModeHPS_RET GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET
/** @brief  No change in standby power mode */
#define hal_StandbyPowerModeNoChange GP_WB_ENUM_STANDBY_POWER_MODE_NOCHANGE

/** @typedef hal_StandbyPowerMode_t
 *  @brief The hal_StandbyPowerMode_t type defines the hal standby power modes.
 */
typedef uint16_t hal_StandbyPowerMode_t;
//@}

/** @name hal_ActivePowerMode_t*/
//@{
/** @brief This is the High power Mode, uses the 192MHz BBL clock, locked to the xo32m and has analog voltages enabled
 */
#define hal_ActivePowerModeHPS GP_WB_ENUM_ACTIVE_POWER_MODE_HPS
/** @brief This is the High Power Mode with DCDC convertor enabled */
#define hal_ActivePowerModeEHPS GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS
/** @brief No change in active power mode */
#define hal_ActivePowerModeNoChange GP_WB_ENUM_ACTIVE_POWER_MODE_NOCHANGE

/** @typedef hal_ActivePowerMode_t
 *  @brief The hal_ActivePowerMode_t type defines the hal active power modes.
 */
typedef uint16_t hal_ActivePowerMode_t;
//@}

/** @name hal_SleepMode_t*/
//@{
/* sleep_mode  */
/** @brief The GP chip will run a RC timer during standby.*/
#define hal_SleepModeRC GPHAL_ENUM_SLEEP_MODE_RC
/** @brief The GP chip will run a 32kHz crystal clock during standby.*/
#define hal_SleepMode32kHz GPHAL_ENUM_SLEEP_MODE_32KHZ
/** @brief The GP chip will run a 32MHz clock during standby.*/
#define hal_SleepMode32MHz GPHAL_ENUM_SLEEP_MODE_32MHZ
/** @typedef hal_SleepMode_t
 *  @brief The hal_SleepMode_t type defines the hal sleep modes.
 */
typedef uint8_t hal_SleepMode_t;
//@}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/**
 * @brief Sets the power modes for the device.
 *
 * This function sets the power modes of the device to the specified modes.
 *
 * @param[in] standbyPowerMode The standby power mode to be set. see @c hal_StandbyPowerMode_t
 * @param[in] activePowerMode The active power mode to be set. see @c hal_ActivePowerMode_t
 * @param[in] sleepMode The sleep mode to be set, which controls the RTC_TIMER_SRC. see @c hal_SleepMode_t
 *
 * @return Returns a status code indicating the success or failure of the operation. see @c hal_PowerModeResult_t
 */
hal_PowerModeResult_t hal_SetPowerMode(const hal_StandbyPowerMode_t standbyPowerMode,
                                       const hal_ActivePowerMode_t activePowerMode, hal_SleepMode_t sleepMode);

/**
 * @brief Get the power mode for the device.
 *
 * This function reads the power mode of the device.
 *
 * @param[out] pStandbyPowerMode Pointer to hold the standby power mode of the device. see @c hal_StandbyPowerMode_t
 * @param[out] pActivePowerMode Pointer to hold the active power mode of the device. see @c hal_ActivePowerMode_t
 * @param[out] pSleepMode Pointer to hold the sleep mode of the device (The RTC_TIMER_SRC). see @c hal_SleepMode_t
 *
 * @return Returns a status code indicating the success or failure of the operation. see @c hal_PowerModeResult_t
 */
hal_PowerModeResult_t hal_GetPowerMode(hal_StandbyPowerMode_t* pStandbyPowerMode,
                                       hal_ActivePowerMode_t* pActivePowerMode, hal_SleepMode_t* pSleepMode);

/**
 * @brief Set the clock accuracy for a sleep mode.
 *
 * This function sets the clock accuracy for the RC clock
 *
 * @param[in] clockAccuracy The clock accuracy for the clock to be set. see @c hal_ClockAccuracy_t
 * @param[in] sleepMode The sleep mode of which the clock accuracy value need to be set. see @c hal_SleepMode_t
 */

void hal_setClockAccuracy(hal_SleepMode_t sleepMode, hal_ClockAccuracy_t clockAccuracy);

/**
 * @brief Reads the clock accuracy of the device.
 *
 * This function reads the clock accuracy depending on the current sleep mode of the device.
 *
 * @return Returns the clock accuracy. see @c hal_ClockAccuracy_t
 */

hal_ClockAccuracy_t hal_GetSleepClockAccuracy(void);

/**
 * @brief Reads the worst clock accuracy of the device.
 *
 * This function reads the worst clock accuracy depending on the current sleep mode of the device.
 *
 * @return Returns the worst clock accuracy. see @c hal_ClockAccuracy_t
 */

hal_ClockAccuracy_t hal_GetWorstSleepClockAccuracy(void);

/**
 * @brief Reads the clock accuracy of the device.
 *
 * This function reads the worst clock accuracy depending on the current sleep mode of the device.
 *
 * @param[in] nextSleepMode The sleep mode for which the clock accuracy is requested. see @c hal_SleepMode_t
 *
 * @return Returns the clock accuracy. see @c hal_ClockAccuracy_t
 */

hal_ClockAccuracy_t hal_GetAverageSleepClockAccuracy(hal_SleepMode_t nextSleepMode);

/**
 * @brief Reads the Allan deviation of the RC clock in ppm of the device.
 *
 * This function reads the Allan deviation of the RC clock in ppm of the device.
 *
 * @param[in] currentTemperature The temperature for which the clock accuracy is requested.
 *
 * @return Returns the Allan deviation.
 */

uint8_t hal_AllanDeviationClockAccuracy(Int16 currentTemperature);

/**
 * @brief Calibrate the clock accuracy based on the temperature.
 *
 * This function measures the chip temperature and adjusts the clock accuracies based on the measured results.
 *
 * @param[in] sleepMode The sleep mode for which the clock accuracy is recalibrated. see @c hal_SleepMode_t
 */

void hal_TemperatureCalibrateClockAccuracy(hal_SleepMode_t sleepMode);

#ifdef __cplusplus
}
#endif

/** @} */

#endif //_HAL_POWERMODE_H_
