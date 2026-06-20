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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL
#include "gpHal_DEFS.h"
#ifndef GP_COMP_CHIPEMU
#include "esec_common.h"
#include "esec_qorvo.h"
#endif
#include "hal_PowerMode.h"
#include "qDrvTsens.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
// Definitions for Sleep Clock Accuracies (SCA)
// Average accuracies
#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_AVERAGE
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_AVERAGE 500
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_AVERAGE

#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_AVERAGE
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_AVERAGE 100
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_AVERAGE

#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_AVERAGE
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_AVERAGE 20
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_AVERAGE

// Worst case accuracies (to be used for dimensioning maximum sleep time)
#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_WORST
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_WORST 500
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_WORST

#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_WORST
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_WORST 100
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_WORST

#ifndef GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_WORST
#define GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_WORST 20
#endif // GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_WORST

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

hal_ClockAccuracy_t halClockAccuracyRC = GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_AVERAGE;
hal_ClockAccuracy_t halClockAccuracy32K = GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_AVERAGE;
hal_ClockAccuracy_t halClockAccuracy32M = GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_AVERAGE;
Int8 hal_ClockAccuracyCalT32K = 25;
Int8 hal_ClockAccuracyCalTRC = 25;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void hal_UpdateStandbyMode(hal_SleepMode_t mode);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static void hal_UpdateStandbyMode(hal_SleepMode_t mode)
{
    if(mode == gpHal_SleepMode32kHz)
    {
        GP_ASSERT_SYSTEM(gpHal_32kHzCrystalAvailable());
    }

    // Disable continuous conversion from sleep counter to symbol counter.
    // TODO: This can be shifted to hal_ES
    GP_WB_WRITE_ES_ONLY_CONVERT_WHEN_NEEDED(1);

    // Set sleep modes and collaterals
    gpHal_SetDefaultStartupSymbolTimes(mode);

#ifdef GP_COMP_GPHAL_BLE
    // The RT system uses own CA in his calculations. Make sure we always update this when the sleep mode changes
    gpHal_BleSetClockAccuracy(gpHal_GetSleepClockAccuracy());
#endif
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void gpHal_InitSleep(void)
{
    hal_UpdateStandbyMode(gpHal_SleepMode16MHz);
}

hal_SleepMode_t hal_GetSleepMode(void)
{
    // TODO: move to hal_sleep.c ?
    return GP_WB_READ_PMUD_RTC_TIMER_SRC();
}

hal_PowerModeResult_t hal_SetPowerMode(const hal_StandbyPowerMode_t standbyPowerMode,
                                       const hal_ActivePowerMode_t activePowerMode, hal_SleepMode_t sleepMode)
{
#ifndef GP_COMP_CHIPEMU
    esec_status_t esec_status;

    /* If the 32kHz crystal is broken: fall back to RC sleep mode. */
    if(hal_SleepMode32kHz == sleepMode)
    {
        gpHal_SleepClockMeasurementStatus_t status = gpHalEs_Get32kHzBenchmarkStatus();
        if(gpHal_SleepClockMeasurementStatusNotStable == status)
        {
            GP_LOG_SYSTEM_PRINTF("Error: 32kHz XTAL broken!", 0);
            GP_ASSERT_DEV_EXT(false);
            return hal_PowerModeResultInvalidParameter;
        }

        /**
         * Look for 'gpHal_cb32kHzCalibrationDone' in gpHal_ES.c for the full story.
         * We disable this error-check here if the applications has not set
         * 'GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB' so that applications can still set gpHal_SleepMode32kHz before
         * the initial calibration has finished.
         */
#if defined(GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB)
        else if(gpHal_SleepClockMeasurementStatusStable != status)
        {
            GP_LOG_SYSTEM_PRINTF("Error: 32kHz XTAL not ready!", 0);
            GP_ASSERT_DEV_EXT(false);
            return hal_PowerModeResultBusy;
        }
#endif
    }

    esec_status = esec_set_power_mode(Q_POWER_MODE(standbyPowerMode, activePowerMode, sleepMode));
    if(ESEC_OKAY != esec_status)
    {
        GP_LOG_SYSTEM_PRINTF("Failed to set power mode, error: %lu", 0, esec_status);
        return hal_PowerModeResultError;
    }

    hal_UpdateStandbyMode(sleepMode);

    /* create task to handle sleep clock re-calibrations */
    gpHalES_setupSleepClockRecalibration(sleepMode);

#endif // GP_COMP_CHIPEMU
    return hal_PowerModeResultSuccess;
}

hal_PowerModeResult_t hal_GetPowerMode(hal_StandbyPowerMode_t* pStandbyPowerMode,
                                       hal_ActivePowerMode_t* pActivePowerMode, hal_SleepMode_t* pSleepMode)
{
#ifndef GP_COMP_CHIPEMU
    uint32_t esec_status = ESEC_OKAY;
    power_mode_t current_power_mode = 0;

    /* Validate input */
    if((NULL == pStandbyPowerMode) || (NULL == pActivePowerMode) || (NULL == pSleepMode))
    {
        return hal_PowerModeResultInvalidParameter;
    }

    /**
     * To keep the APIs consistent and the logic separate, we are going to read the active and standby power modes using
     * the esec_get_power_mode API and the sleep mode using the hal_GetSleepMode API
     */

    /* Step 1: Read active and standby power modes */
    esec_status = esec_get_power_mode(&current_power_mode);
    if(ESEC_OKAY != esec_status)
    {
        GP_LOG_SYSTEM_PRINTF("Failed to read power modes with error: %lu", 0, esec_status);
        return hal_PowerModeResultError;
    }

    *pStandbyPowerMode = (hal_StandbyPowerMode_t)Q_POWER_MODE_STANDBY(current_power_mode);
    *pActivePowerMode = (hal_ActivePowerMode_t)Q_POWER_MODE_ACTIVE(current_power_mode);

    /* Step 2: Read sleep mode */
    *pSleepMode = (hal_SleepMode_t)hal_GetSleepMode();
#endif // GP_COMP_CHIPEMU
    return hal_PowerModeResultSuccess;
}

void hal_setClockAccuracy(hal_SleepMode_t sleepmode, hal_ClockAccuracy_t clockAccuracy)
{
    switch(sleepmode)
    {
        case gpHal_SleepMode32kHz:
            halClockAccuracy32K = clockAccuracy;
            break;
        case gpHal_SleepModeRC:
            halClockAccuracyRC = clockAccuracy;
            break;
        case gpHal_SleepMode16MHz:
            halClockAccuracy32M = clockAccuracy;
            break;
        default:
            GP_ASSERT_DEV_INT(false);
            break;
    }
}

hal_ClockAccuracy_t hal_GetSleepClockAccuracy(void)
{
    const hal_ClockAccuracy_t averageScas[] = {halClockAccuracyRC, halClockAccuracy32K, halClockAccuracy32M};

    hal_SleepMode_t sleepMode = hal_GetSleepMode();

    if(sleepMode > gpHal_SleepMode16MHz)
    {
        GP_ASSERT_DEV_INT(false);
        return averageScas[gpHal_SleepMode16MHz];
    }

    return averageScas[sleepMode];
}

hal_ClockAccuracy_t hal_GetWorstSleepClockAccuracy(void)
{
    const hal_ClockAccuracy_t worstScas[] = {GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_RC_WORST,
                                             GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32K_WORST,
                                             GP_DIVERSITY_GPHAL_CLOCK_ACCURACY_PPM_32M_WORST};

    hal_SleepMode_t sleepMode = hal_GetSleepMode();

    if(sleepMode > gpHal_SleepMode16MHz)
    {
        GP_ASSERT_DEV_INT(false);
        return worstScas[gpHal_SleepMode16MHz];
    }

    return worstScas[sleepMode];
}

hal_ClockAccuracy_t hal_GetAverageSleepClockAccuracy(hal_SleepMode_t nextSleepMode)
{
    const hal_ClockAccuracy_t averageScas[] = {halClockAccuracyRC, halClockAccuracy32K, halClockAccuracy32M};

    if(nextSleepMode > gpHal_SleepMode16MHz)
    {
        GP_ASSERT_DEV_INT(false);
        return averageScas[gpHal_SleepMode16MHz];
    }

    return averageScas[nextSleepMode];
}

uint8_t hal_AllanDeviationClockAccuracy(Int16 currentTemperature)
{
    // 3 sigma allan deviation for RC clock. For x-tal clocks, the Allan deviation is negligible.
    if(currentTemperature < -10)
    {
        return 240; // ppm
    }
    return 120;     // ppm
}

void hal_TemperatureCalibrateClockAccuracy(hal_SleepMode_t sleepMode)
{
    if(sleepMode == gpHal_SleepMode16MHz)
    {
        return;
    }
    // get current temperature
    Int8 currentTemperature;
    currentTemperature = Q_PRECISION_DECR8(qDrvTsens_ResultGetBlocking());
    GP_LOG_SYSTEM_PRINTF("temp @ %dC", 0, currentTemperature);
    const Int8 temperatures[] = {-40, 25, 60, 100, 125};

    // get temperature of last calibration
    currentTemperature = 120;
    uint8_t i;
    Int16 totalCorr = 0;
    Int16 currentCorr;

    if(sleepMode == gpHal_SleepMode32kHz)
    {
        Int16 correctionFactors[5] = {3, 0, -2, -6, -15};
        currentCorr = correctionFactors[0];
        // find correction factor of temperature of last calibration
        for(i = 0; (i < 5) && (temperatures[i] < hal_ClockAccuracyCalT32K); i++)
        {
            currentCorr = correctionFactors[i];
        }

        if(currentTemperature > hal_ClockAccuracyCalT32K)
        {
            for(uint8_t j = hal_ClockAccuracyCalT32K; j < currentTemperature; j++)
            {
                totalCorr += currentCorr;
                if(j > temperatures[i]) // update correction factor if boundary is passed
                {
                    currentCorr = correctionFactors[i++];
                }
            }
            halClockAccuracy32K += totalCorr;
        }
        else
        {
            for(uint8_t j = hal_ClockAccuracyCalT32K; j > currentTemperature; j--)
            {
                totalCorr += currentCorr;
                if(j > temperatures[i]) // update correction factor if boundary is passed
                {
                    currentCorr = correctionFactors[i++];
                }
            }
            halClockAccuracy32K -= totalCorr;
        }
        GP_LOG_SYSTEM_PRINTF("Correction 32 kHz @ %dC > %d", 0, currentTemperature, totalCorr);
        hal_ClockAccuracyCalT32K = currentTemperature;
    }
    else
    {
        Int16 correctionFactors[5] = {-100, -100, -200, -600, -600};
        currentCorr = correctionFactors[0];
        // find correction factor of temperature of last calibration
        for(i = 0; (i < 5) && (temperatures[i] < hal_ClockAccuracyCalTRC); i++)
        {
            currentCorr = correctionFactors[i];
        }

        if(currentTemperature > hal_ClockAccuracyCalTRC)
        {
            // apply correction factor until currentTemperature
            for(uint8_t j = hal_ClockAccuracyCalTRC; j < currentTemperature; j++)
            {
                totalCorr += currentCorr;
                if(j > temperatures[i]) // update correction factor if boundary is passed
                {
                    currentCorr = correctionFactors[i++];
                }
            }
            halClockAccuracyRC += totalCorr;
        }
        else
        {
            for(uint8_t j = hal_ClockAccuracyCalTRC; j > currentTemperature; j--)
            {
                totalCorr += currentCorr;
                if(j > temperatures[i]) // update correction factor if boundary is passed
                {
                    currentCorr = correctionFactors[i++];
                }
            }
            halClockAccuracy32K -= totalCorr;
        }
        GP_LOG_SYSTEM_PRINTF("Correction RC @ %dC > %d", 0, currentTemperature, totalCorr);
        hal_ClockAccuracyCalTRC = currentTemperature;
    }
}

/*****************************************************************************
 *                    Public Function Definitions - for legacy code
 *****************************************************************************/
gpHal_SleepMode_t gpHal_GetSleepMode(void)
{
    return (gpHal_SleepMode_t)hal_GetSleepMode();
}

UInt16 gpHal_GetSleepClockAccuracy(void)
{
    return hal_GetSleepClockAccuracy();
}

UInt16 gpHal_GetWorstSleepClockAccuracy(void)
{
    return hal_GetWorstSleepClockAccuracy();
}

UInt16 gpHal_GetAverageSleepClockAccuracy(gpHal_SleepMode_t nextSleepMode)
{
    return hal_GetAverageSleepClockAccuracy(nextSleepMode);
}

gpHal_Result_t gpHal_SetSleepMode(gpHal_SleepMode_t mode)
{
#ifndef GP_COMP_CHIPEMU
    hal_StandbyPowerMode_t standbyPowerMode = hal_StandbyPowerModeNoChange;
    hal_ActivePowerMode_t activePowerMode = hal_ActivePowerModeNoChange;
    hal_PowerModeResult_t status;
    if(mode == hal_SleepMode32MHz)
    {
        standbyPowerMode = hal_StandbyPowerModeHPS_RET;
        activePowerMode = hal_ActivePowerModeHPS;
    }
    else
    {
        /* DCDC coil is not emulated on FPGA platform */
        standbyPowerMode = hal_StandbyPowerModeELPS;
        activePowerMode = hal_ActivePowerModeEHPS;
    }

    status = hal_SetPowerMode(standbyPowerMode, activePowerMode, mode);
    if(status != OKAY)
    {
        GP_LOG_SYSTEM_PRINTF("Error: set_power_mode!", 0);
        return gpHal_ResultInvalidParameter;
    }
#endif // GP_COMP_CHIPEMU

    return gpHal_ResultSuccess;
}

void gpHal_GoToSleepWhenIdle(Bool enable)
{
    NOT_USED(enable);
}
