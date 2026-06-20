/*
 * Copyright (c) 2024, Qorvo Inc
 *
 *   gpAppUcSubsystem_Calibrations
 *   Implementation of gpAppUcSubsystem_Calibrations
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
// #define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALPLATFORM

#include "gpLog.h"
#include "hal.h"
#include "gpHal_Phy.h"
#include "hal_PowerMode.h"
#include "gpHal_DEFS.h"
#include "gpHal_Calibration.h"
#include "qDrvTsens.h"
#include "gpAppUcSubsystem_Calibrations.h"
#ifndef GP_COMP_CHIPEMU
#include "esec_qorvo.h"
#endif // GP_COMP_CHIPEMU
#include "gpAppUcSubsystem_CalibrationDefaultsForEVB.h"
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_HAL_FLL_CAL_ENTRY_SIZE     (17)
#define GP_HAL_NR_OF_ZB_FLL_CHANNELS  (16)
#define GP_HAL_NR_OF_BLE_FLL_CHANNELS (40)

#define GP_HAL_RF_PRECALIBRATION_CHANNEL_TABLE_SIZE                                                                    \
    (GP_HAL_FLL_CAL_ENTRY_SIZE * ((GP_HAL_NR_OF_BLE_FLL_CHANNELS * 2) + GP_HAL_NR_OF_ZB_FLL_CHANNELS))

#define HAL_NUM_RT_CALIBRATIONS GP_WB_ENUM_CAL_TYPE_MAX
#define HAL_THRESHOLD_RT_CALIBRATIONS_US (1000 * 1000)
/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
#ifdef GP_COMP_GPHAL_RADIO
static UInt8 tempCalTaskHandle;

static UInt8 rfPreCalibrationChannelTable[GP_HAL_RF_PRECALIBRATION_CHANNEL_TABLE_SIZE] LINKER_SECTION(".rt_fast_ram");

static UInt8 rtCalibrationListInfo[GP_WB_CAL_ENTRY_SIZE * (HAL_NUM_RT_CALIBRATIONS)] LINKER_SECTION(".rt_fast_ram");

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
static void TemperatureCalibrationsTask(const gpHal_CalibrationTask_t* task);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void TemperatureCalibrationsTask(const gpHal_CalibrationTask_t* task)
{
    NOT_USED(task);

    // Retrieve CW settings
    gpHalPhy_CurrentCWModeSettings_t* settings = gpHal_GetCurrentContinuousWaveModeSettings();

    if(settings->mode != gpHal_CW_Off)
    {
        GP_LOG_PRINTF("Re-calibrate", 0);

        // back-up CW mode
        gpHal_ContinuousWaveMode_t mode = settings->mode;

        // Turn off CW
        gpHal_SetContinuousWaveMode(gpHal_CW_Off, 0, 0, 0);

        // Trigger calibration
        gpAppUcSubsystem_TriggerCalibrations();

        // Wait for calibration  done
        gpHal_WaitForCalibrationDone();

        GP_LOG_PRINTF("Retart CW", 0);
        // Restart CW
        gpHal_SetContinuousWaveMode(mode, settings->channel, settings->txpower, settings->antenna);
        return;
    }

    gpAppUcSubsystem_TriggerCalibrations();
}
#endif // GP_COMP_GPHAL_RADIO

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpAppUcSubsystem_CalibrationInit(void)
{
#ifdef GP_COMP_GPHAL_RADIO
    gpHal_CalibrationTask_t calTask;
    GP_LOG_PRINTF("CalInit", 0);

    GP_HAL_REGISTER_AKRAM_REGION(rfPreCalibrationChannelTable);
    GP_HAL_REGISTER_AKRAM_REGION(rtCalibrationListInfo);


    /* akuma calibration list */
    GP_WB_WRITE_RT_CAL_INFO_BASE_PTR(TO_RT_ADDR(GP_HAL_PHYSICAL_TO_ARM((UInt32)rtCalibrationListInfo)));

    /* Pre-calibration channel table  */
    GP_WB_WRITE_RT_RF_PRE_CALIBRATION_TABLE_PTR(
        TO_RT_ADDR(GP_HAL_PHYSICAL_TO_ARM((UInt32)rfPreCalibrationChannelTable)));

    // set Threshold to 0 to disable forcing calibration
    GP_WB_WRITE_RT_CAL_THRESHOLD(HAL_THRESHOLD_RT_CALIBRATIONS_US);

#ifndef GP_COMP_CHIPEMU
    gpAppUcSubsystem_CalibrationSetDcdcValues(&calRamDcdcData_EVB);
    gpAppUcSubsystem_CalibrationSetXo32mValues(&calRamXo32mData_EVB);
#endif // GP_COMP_CHIPEMU
    MEMSET(&calTask, 0, sizeof(gpHal_CalibrationTask_t));
    calTask.flags = GP_HAL_CALIBRATION_FLAG_TEMPERATURE_SENSITIVE;

    /* Trigger calibrations for every 10 C change */
    /* Temporary fix as recommnded by the digital team, see AD-7541 */
    calTask.temperatureThreshold = UQ_PRECISION_INCR8(5);

    /* Enable for periodic calibrations */
    tempCalTaskHandle = gpHal_CalibrationCreateTask(&calTask, TemperatureCalibrationsTask);
    GP_ASSERT_DEV_INT(tempCalTaskHandle != GP_HAL_CALIBRATION_INVALID_TASK_HANDLE);

    /* Trigger initail/precalibration */
    gpAppUcSubsystem_TriggerCalibrations();
#endif // GP_COMP_GPHAL_RADIO
}

void gpAppUcSubsystem_CalibrationSetDcdcValues(gpAppUcSubsystem_CalRamDcdc_t* pCalRamDcdcData)
{
    GP_WB_WRITE_CAL_RAM_DCDC_Y2_SKIP_EHPS(pCalRamDcdcData->y2_skip_ehps);
    GP_WB_WRITE_CAL_RAM_DCDC_Y2_SKIP_EMPS(pCalRamDcdcData->y2_skip_emps);
    GP_WB_WRITE_CAL_RAM_DCDC_Y2_MIN_EHPS(pCalRamDcdcData->y2_min_ehps);
    GP_WB_WRITE_CAL_RAM_DCDC_Y2_MIN_EMPS(pCalRamDcdcData->y2_min_emps);
    GP_WB_WRITE_CAL_RAM_DCDC_Y1_SKIP(pCalRamDcdcData->y1_skip);
    GP_WB_WRITE_CAL_RAM_DCDC_Y1_MIN(pCalRamDcdcData->y1_min);
}

void gpAppUcSubsystem_CalibrationSetXo32mValues(gpAppUcSubsystem_CalRamXo32m_t* pCalRamXo32mData)
{
    MEMCPY((void*)GP_WB_CAL_RAM_XO32M_CLOAD_COMP_0_ADDRESS, pCalRamXo32mData,
           GP_WB_CAL_RAM_XO32M_CLOAD_COMP_16_ADDRESS - GP_WB_CAL_RAM_XO32M_CLOAD_COMP_0_ADDRESS + 1);
}
/* Trigger calibrations */
void gpAppUcSubsystem_TriggerCalibrations(void)
{
#ifndef GP_COMP_CHIPEMU
#ifdef GP_COMP_GPHAL_RADIO
    Int16 currentTemperature;
    hal_Result_t result = gpHal_ResultSuccess;
    currentTemperature = qDrvTsens_ResultGetBlocking();
    GP_LOG_PRINTF("temp @ %dC", 0, Q_PRECISION_DECR8(currentTemperature));
    GP_WB_WRITE_CAL_RAM_TEMP_DEG(Q_PRECISION_DECR8(currentTemperature));

    esec_change_temperature();

    /* Following IPC command triggers the RT to do all types of radio calibrations  */
    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_TRIGGER_RADIO_CALIBRATIONS, 0, NULL);
    GP_ASSERT_DEV_EXT(result == gpHal_ResultSuccess);
#endif // GP_COMP_GPHAL_RADIO
#endif // GP_COMP_CHIPEMU
}

static Bool IsCalibrationBusy(void)
{
    return GP_WB_READ_RT_CAL_INFO_STATUS() == GP_WB_ENUM_CAL_STATUS_REQUESTED;
}

void gpHal_WaitForCalibrationDone(void)
{
    while(IsCalibrationBusy() == true)
    {
        HAL_WAIT_MS(1);
    }
}
