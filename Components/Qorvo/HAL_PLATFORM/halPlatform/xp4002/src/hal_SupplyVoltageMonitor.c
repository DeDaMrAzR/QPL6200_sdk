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
// #define GP_LOCAL_LOG
#define GP_COMPONENT_ID GP_COMPONENT_ID_HALPLATFORM

#include "hal.h"
#include "gpLog.h"
#include "hal_SupplyVoltageMonitor.h"
#include "qDrvBistAdc.h"
#include "gpHal_Calibration.h"
#include "gpSched.h"

#ifndef GP_COMP_CHIPEMU
#include "hal_SEMailbox.h"
#endif // GP_COMP_CHIPEMU

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUIC.h"
#include "qDrvAkumaTriggers.h"
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** @brief Minimum supply voltage in microvolt. */
#define HAL_BATTERY_VOLTAGE_MIN_UV  2100000 // 2.1v

/** @brief supply voltage monitoring period in microseconds. */
#define HAL_SUPPLY_VOLTAGE_MONITORING_PERIOD_US MS_TO_US(10 * 60000) // 10 minutes

/** Maximal duration BIST ADC is enabled during TX (before we consider it as failed) */
#define HAL_SUPPLY_VOLTAGE_MONITORING_TX_TIMEOUT_US MS_TO_US(60000) // 1 minute

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/** @brief supply voltage monitoring state structure. */
typedef struct {
    qDrvBistAdc_t bistAdc; /**< Bist ADC instance.*/
    UInt8 taskHandle;      /**< Calibration task handle.*/
    Int32 voltage;         /**< Periodic measurement result.*/
    Bool initialized;      /**< Initialization flag.*/
    Bool bTxBusy;          /**< TX measurement busy flag.*/
    Bool bMeasureTx;       /**< Time to measure supply voltage during TX.*/
    Int32 voltageTx;       /**< Supply voltage measurement result during TX activity.*/
} hal_SupplyVoltageMonitorState_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
/** @brief supply voltage monitoring state. */
static hal_SupplyVoltageMonitorState_t supplyVoltState;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/** @brief Reads the supply voltage monitoring initialization state.*/
static inline Bool hal_supplyVoltageMonitorInitialized(void)
{
    return supplyVoltState.initialized;
}

/** @brief Reads the TX measurement busy flag state.*/
static inline Bool hal_supplyVoltageMonitorIsTxBusy(void)
{
    return supplyVoltState.bTxBusy;
}

/** @brief Reads the TX measurement time flag state.*/
static inline Bool hal_supplyVoltageMonitorIsTimeToMeasureTx(void)
{
    return supplyVoltState.bMeasureTx;
}
/** @brief Enables BIST ADC measurement.*/
static void hal_SupplyVoltageMonitorEnable(void)
{
    qDrvBistAdc_Enable(&supplyVoltState.bistAdc);
}

/** @brief Disables BIST ADC measurement.*/
static void hal_SupplyVoltageMonitorDisable(void)
{
    qDrvBistAdc_Disable(&supplyVoltState.bistAdc);
}

/** @brief Starts BIST ADC measurement.*/
static void hal_SupplyVoltageMonitorStart(void)
{
    qDrvBistAdc_Trigger(&supplyVoltState.bistAdc);
}

/**
 * @brief Indicated if the voltage measurement has completed.
 *
 * @return True if there the measurement has completed.
 */
static Bool hal_IsSupplyVoltageMonitorResultValid(void)
{
    return qDrvBistAdc_ReadyCheck(&supplyVoltState.bistAdc);
}

/**
 * @brief Reads out the value of the voltage measurement.
 *
 *  @return The voltage value in uV.
 */
static Int32 hal_SupplyVoltageMonitorResultGet(void)
{
    return qDrvBistAdc_ResultGet(&supplyVoltState.bistAdc);
}

/** @brief Supply voltage monitoring calibration task callback. */
static void supplyVoltageMonitoringTask(const gpHal_CalibrationTask_t* task)
{
    NOT_USED(task);
    Int32 voltageTx;
    Int32 voltageMin;

    if(hal_supplyVoltageMonitorIsTxBusy())
    {
        GP_LOG_PRINTF("skip periodic ", 0);

        /*
         * Bist ADC is enabled for supply voltage measurement on tx activities.
         * If we continue periodic measurement it will disable Bist ADC. that's not what we want.
         */
        return;
    }

    supplyVoltState.voltage = hal_SupplyVoltageMonitorResultGetBlocking();
    voltageTx = hal_SupplyVoltageMonitorResultGetTx();
    voltageMin = hal_SupplyVoltageMonitorResultGetMin();

    GP_LOG_PRINTF("Periodic %ld uV, tx %ld uV, min %ld uV", 0, supplyVoltState.voltage, voltageTx, voltageMin);
    // gpLog_Flush();

#ifndef GP_COMP_CHIPEMU
    hal_SEMailbox_ChangeBattVoltage(voltageMin);
#endif // GP_COMP_CHIPEMU

    /* start supply voltage measurement on tx activities */
    supplyVoltState.bMeasureTx = true;

    NOT_USED(voltageTx);
    NOT_USED(voltageMin);
}

static void hal_SupplyVoltageMonitorTxTimeout(void)
{
    if(hal_supplyVoltageMonitorIsTxBusy())
    {
        hal_SupplyVoltageMonitorDisable();
        supplyVoltState.bTxBusy = false;
    }

    GP_LOG_PRINTF("TxTimeout", 0);
}
/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/**
 * @brief Initialize supply voltage monitoring.
 *
 */
void hal_SupplyVoltageMonitorInit(void)
{
    qResult_t result;
    gpHal_CalibrationTask_t calTask;
    GP_LOG_PRINTF("SupplyVoltageMonitorInit", 0);

    supplyVoltState.voltage = HAL_BATTERY_VOLTAGE_INVALID;
    supplyVoltState.voltageTx = HAL_BATTERY_VOLTAGE_INVALID;
    supplyVoltState.bTxBusy = false;
    supplyVoltState.bMeasureTx = true;

    result = qDrvBistAdc_Init(&supplyVoltState.bistAdc);
    Q_ASSERT(result == Q_OK);

    qDrvBistAdc_Config_t bistAdcConfig = {.configNrAvgExp = 5,
                                          .muxPSettingVBat = 7,
                                          .muxNSettingVBat = 7,
                                          .diffMode = true,
                                          .lowVoltageRange = false, // high voltage range
                                          .configTSampleUs = 1};

    qDrvBistAdc_ConfigSet(&supplyVoltState.bistAdc, &bistAdcConfig);

    MEMSET(&calTask, 0, sizeof(gpHal_CalibrationTask_t));
    calTask.flags = GP_HAL_CALIBRATION_FLAG_PERIODIC;
    calTask.calibrationPeriod = HAL_SUPPLY_VOLTAGE_MONITORING_PERIOD_US;

    supplyVoltState.taskHandle = gpHal_CalibrationCreateTask(&calTask, supplyVoltageMonitoringTask);

    /* configure QUIC to trigger bist adc measurement upon Tx */
    qDrvQUIC_Channel_t quicChannelTxOn = qDrvQUIC_Alloc(false);
    Q_ASSERT(quicChannelTxOn != Q_NOCHANNEL);

    result = qDrvQUIC_EdgeSet(quicChannelTxOn, qRegQUIC_EdgeRising);
    Q_ASSERT(result == Q_OK);

    qDrvAkumaTriggers_t akTriggers;
    qDrvAkumaTriggers_Init(&akTriggers);

    qDrvBistAdc_QuicActionConfig_t pActionConfig = {
        .actionIdx = 0,
        .action = qRegBistAdc_QuicActionStart,
    };

    qDrvAkumaTriggers_QuicTriggerConfig_t quicSrcConfig = {
        .ipcChannel = 0, /* dummy value */
        .srcTrigger = qDrvAkumaTriggers_QuicSrcTriggerPaOn,
    };

    /* Connect timers with provided configuration. */
    result = qDrvQUIC_Connect(quicChannelTxOn, &akTriggers, &quicSrcConfig, &supplyVoltState.bistAdc, &pActionConfig);
    Q_ASSERT(result == Q_OK);

#ifndef GP_COMP_CHIPEMU
    supplyVoltState.initialized = true;
    supplyVoltState.voltage = hal_SupplyVoltageMonitorResultGetBlocking();
    hal_SEMailbox_ChangeBattVoltage(supplyVoltState.voltage);
#endif // GP_COMP_CHIPEMU
}

/**
 * @brief Read current supply voltage.
 * This is a blocking call which takes around 90us with 192MHz main clock
 *
 * @return Current supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetBlocking(void)
{
    Bool timeout;

    if(!hal_supplyVoltageMonitorInitialized())
    {
        return HAL_BATTERY_VOLTAGE_INVALID;
    }

    if(!hal_supplyVoltageMonitorIsTxBusy())
    {
        hal_SupplyVoltageMonitorEnable();
        hal_SupplyVoltageMonitorStart();
        GP_DO_WHILE_TIMEOUT((hal_IsSupplyVoltageMonitorResultValid() != 1), 64000, &timeout);
        hal_SupplyVoltageMonitorDisable();
        if(timeout)
        {
            return HAL_BATTERY_VOLTAGE_INVALID;
        }

        supplyVoltState.voltage = hal_SupplyVoltageMonitorResultGet();
    }

    return supplyVoltState.voltage;
}

/**
 * @brief Read the last measured supply voltage during the radio transmit (TX) activity.
 *
 * @return  Supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetTx(void)
{
    return supplyVoltState.voltageTx;
}

/**
 * @brief Read the worst case supply voltage measured during TX or periodic.
 *
 * @return Minimum supply voltage in micro volt.
 */
Int32 hal_SupplyVoltageMonitorResultGetMin(void)
{
    Int32 minVolt;
    Int32 voltage = supplyVoltState.voltage;
    Int32 voltageTx = hal_SupplyVoltageMonitorResultGetTx();

    if((voltage != HAL_BATTERY_VOLTAGE_INVALID) && (voltageTx != HAL_BATTERY_VOLTAGE_INVALID))
    {
        minVolt = min(voltageTx, voltage);
    }
    else
    {
        minVolt = voltageTx != HAL_BATTERY_VOLTAGE_INVALID ? voltageTx : voltage;
    }

    return minVolt;
}

/**
 * @brief Enables supply voltage measurement to be tirggered using QUIC during TX activity.
 *
 */
void hal_SupplyVoltageMonitorAnnounceTxStart(void)
{
    if(hal_supplyVoltageMonitorIsTimeToMeasureTx())
    {
        if(!hal_supplyVoltageMonitorInitialized())
        {
            return;
        }

        if(hal_supplyVoltageMonitorIsTxBusy())
        {
            return;
        }

        /* Use QUIC to trigger bist adc measurement upon Tx. so no need to start. */
        hal_SupplyVoltageMonitorEnable();
        supplyVoltState.bTxBusy = true;
        supplyVoltState.bMeasureTx = false;

        /*
         * Schedule timeout in case something is wrong in transmitting data packet
         * or there is no confirm indication.
         */
        gpSched_ScheduleEvent(HAL_SUPPLY_VOLTAGE_MONITORING_TX_TIMEOUT_US, hal_SupplyVoltageMonitorTxTimeout);
        GP_LOG_PRINTF("TxStart", 0);
    }
}

/**
 * @brief Read the supply voltage measured during TX activity and disables BIST ADC.
 *
 */
void hal_SupplyVoltageMonitorAnnounceTxFinished(void)
{
    if(!hal_supplyVoltageMonitorInitialized())
    {
        return;
    }

    if(!hal_supplyVoltageMonitorIsTxBusy())
    {
        return;
    }

    Bool timeout;
    GP_DO_WHILE_TIMEOUT((hal_IsSupplyVoltageMonitorResultValid() != 1), 64000, &timeout);
    if(!timeout)
    {
        supplyVoltState.voltageTx = hal_SupplyVoltageMonitorResultGet();
    }
    else
    {
        supplyVoltState.voltageTx = HAL_BATTERY_VOLTAGE_INVALID;
    }

    hal_SupplyVoltageMonitorDisable();
    supplyVoltState.bTxBusy = false;

    /*  Unschedule timeout that was registered to handle failure
     *  when something is wrong with in transmitting data packet
     */
    gpSched_UnscheduleEvent(hal_SupplyVoltageMonitorTxTimeout);
    GP_LOG_PRINTF("TxFin %ld uV ", 0, supplyVoltState.voltageTx);
}

/**
 * @brief Check whether the BIST ADC to be re-enabled after sleep for TX supply voltage measurement.
 * This callback is called from the reset_handler, worst case execution time ~70 us
 */
void hal_SupplyVoltageMonitorTxCheck(void)
{
    if(!hal_supplyVoltageMonitorInitialized())
    {
        return;
    }

    if(hal_supplyVoltageMonitorIsTxBusy())
    {
        hal_SupplyVoltageMonitorDisable();
        /* Use QUIC to trigger bist adc measurement upon Tx. so no need to start. */
        hal_SupplyVoltageMonitorEnable();
    }
}

/**
 * @brief Update calibration task periodic interval.
 *
 *  @param interval Calibration task interval in microseconds.
 *                  Calibration framework granularity is 10ms,
 *                  so interval should be in multiples of 10ms.
 *                  This function must be called after @c void hal_SupplyVoltageMonitorInit(void);
 */
void hal_SupplyVoltageMonitorSetInterval(UInt32 interval)
{
    gpHal_CalibrationSetInterval(supplyVoltState.taskHandle, interval);
    GP_LOG_PRINTF("setInterval %ld us", 0, interval);
}
