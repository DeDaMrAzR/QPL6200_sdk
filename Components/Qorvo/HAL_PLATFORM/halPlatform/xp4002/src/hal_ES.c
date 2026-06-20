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
#define GP_COMPONENT_ID GP_COMPONENT_ID_HALPLATFORM
// #define GP_LOCAL_LOG

#include "global.h"
#include "hal.h"
#include "hal_ES.h"
#include "gpHal_DEFS.h"
#include "gpHal_HW.h"
#include "gpHal_reg.h"
#include "gpHal_Calibration.h"
#include "gpAssert.h"
#include "gpSched.h"
#include "gpStat.h"
#include "gpHal_OscillatorBenchmark.h"
#include "qDrvObm.h"
#include "qDrvBist.h"

#ifdef GP_DIVERSITY_FREERTOS
#include "FreeRTOS.h"
#include "timers.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
// #define GP_LOCAL_LOG
//  Benchmark factor:
//  ES_CALIBRATION_FACTOR = GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR * (nr of 64 MHz ticks / nr of sleep counter ticks)
#define GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR 0x100000

#define GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC 0x3E800000
/* (0x3E800000*5/100) Calibration factor should not deviates more than 5% from the nominal value.*/
#define GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC_5_PERC 0x3200000

#define GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K 0x3D090000
/* (0x3D090000*5/100) Calibration factor should not deviates more than 5% from the nominal value.*/
#define GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K_5_PERC 0x30D4000

// Nominal ES_CALIBRATION_FACTOR for 16 MHz sleep mode (31.25 kHz sleep counter).
#define GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR_16MHZ 0x40000000UL

// How many measurements to use for calculating the average benchmark value
#define GP_HAL_ES_MAX_NR_OF_BENCHMARK_MEASUREMENTS_HISTORY 32

#ifndef GP_HAL_ES_BENCHMARK_POWER_XTAL
#define GP_HAL_ES_BENCHMARK_POWER_XTAL 8 // 8 PPM
#endif
/* (1000000.0us/16000000Hz)*( 1000000.0us / ((1000000.0us/32000Hz)*pow(2,GP_HAL_ES_BENCHMARK_POWER_XTAL)) ) <= 8PPM */

#define GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_OTHER_US (UInt16)3000
#define GP_HAL_ES_DEFAULT_LONG_STARTUP_SYMBOL_TIME_OTHER_US                                                            \
    (GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_OTHER_US + GP_HAL_ES_BACKUP_DURATION_OTHER_US)
#define GP_HAL_ES_MIN_STARTUP_SYMBOL_TIME_OTHER_US (UInt16)2000
#define GP_HAL_ES_MAX_STARTUP_SYMBOL_TIME_OTHER_US (UInt16)5000

#define GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_16MHZ_US 1000
#define GP_HAL_ES_DEFAULT_LONG_STARTUP_SYMBOL_TIME_16MHZ_US                                                            \
    (GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_16MHZ_US + GP_HAL_ES_BACKUP_DURATION_OTHER_US)

#define GP_HAL_ES_STARTUP_SYMBOL_TIME_INCREMENT_WHEN_TOO_LATE_US 100
#define GP_HAL_ES_STARTUP_SYMBOL_TIME_DECREMENT_DEFAULT_US       5

#ifndef GP_DIVERSITY_GPHAL_32KHZ_INIT_CALIBRATION_MIN_BENCHMARKS
#define GP_DIVERSITY_GPHAL_32KHZ_INIT_CALIBRATION_MIN_BENCHMARKS 10
#endif // GP_DIVERSITY_GPHAL_32KHZ_INIT_CALIBRATION_MIN_BENCHMARKS
#ifndef GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_MIN_BENCHMARKS
#define GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_MIN_BENCHMARKS 4
#endif // GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_MIN_BENCHMARKS

// Maximal duration of a benchmark measurement (before we consider it as failed)
#define GPHAL_OSCILLATOR_BENCHMARK_TO_MS 5000

#define GPHAL_OBM_FREQUENCY_64MHZ 64000000

// Interval between triggering benchmark of the RC oscillator
#ifndef GP_HAL_RC_BENCHMARK_CALIBRATION_PERIOD_US
#define GP_HAL_RC_BENCHMARK_CALIBRATION_PERIOD_US 10000UL /* 10 ms */
#endif

// Interval between triggering benchmark of the xt32k oscillator
#ifndef GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_PERIOD_MS
#define GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_PERIOD_MS 900000UL /* 15 min */
#endif

/* Corresponding to 1ms benchmark time @64Khz */
#define DEFAULT_RC_SLEEP_CLOCK_NOF_TICKS 64

/* Maximum GP_HAL_ES_MAX_NR_OF_BENCHMARK_MEASUREMENTS_HISTORY */
#define DEFAULT_RC_SLEEP_CLOCK_BENCHMARK_QUEUE_DEPTH 8

/* Enable multiple measurements in each period chip is awake, at GP_HAL_RC_BENCHMARK_CALIBRATION_PERIOD_US interval */
#define DEFAULT_RC_SLEEP_CLOCK_BENCHMARK_ENABLE_MULTIPLE_MEASUREMENTS 1
/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/
#define IDX_TO_BASE(events_base, index) ((events_base) + 16 * (index))

#ifdef GP_HAL_CB_BENCHMARK_DONE
extern void GP_HAL_CB_BENCHMARK_DONE(UInt32 benchmark);
#endif
/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    Bool is_initialized;
    Bool EnableMultipleMeasurements;
    UInt8 QueueDepth;
    UInt16 NoOfLPTicks;
} gpHalEs_RCCalibProperties;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static gpHalEs_RCCalibProperties gpHalEs_RCCalibProp;
static UInt32 halEs_currentBenchmarkMeasurement;

#ifdef GP_COMP_GPHAL_ES_ABS_EVENT
static UInt16 gpHal_ES_AbsoluteEventsInUse;
#endif //GP_COMP_GPHAL_ES_ABS_EVENT

/* State to use for averaging benchmark calibration measurements */
static UInt8 gpHal_EsBenchmarkCurrentIndex;
static UInt32 gpHal_EsBenchmarkMeasurements[GP_HAL_ES_MAX_NR_OF_BENCHMARK_MEASUREMENTS_HISTORY];

/* Last timestamp when a benchmark calibration was performed */
static UInt32 gpHalES_SleepModeRC_TsLastBenchmark;
static UInt32 gpHal_SleepMode32kHz_stable_benchmark = GPHAL_ES_BENCHMARK_COUNTER_INVALID;
static UInt32 gpHal_SleepModeRC_stable_benchmark = GPHAL_ES_BENCHMARK_COUNTER_INVALID;
static gpHal_SleepMode_t gpHal_background_benchmark_mode;
static Bool gpHal_KeepAwakeDuringBenchmark = false;
static Bool gpHal_GotoSleepDisabled = false;

#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
static Bool gpHalES_blockedDCDC;
#endif // GP_DIVERSITY_GPHAL_DCDC_ENABLED
static Bool gpHalEs_IsQueueFull;
static gpHal_SleepClockMeasurementStatus_t gpHalEs_32kHz_benchmark_status;
static UInt8 calTaskHandleRC = GP_HAL_CALIBRATION_INVALID_TASK_HANDLE;
static UInt8 calTaskHandleXt32K = GP_HAL_CALIBRATION_INVALID_TASK_HANDLE;
#ifdef GP_COMP_CHIPEMU
static Bool gpHal_32kHzCrystalAvailableFlag = false;
#else
static Bool gpHal_32kHzCrystalAvailableFlag = true;
#endif // GP_COMP_CHIPEMU
/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static UInt32 gpHalES_GetBenchmarkAverage(UInt8 measurementHistory);
static void gpHalES_StartBackground_Benchmark(gpHal_SleepMode_t sleepMode, UInt32 nowTs);
static UInt32 gpHalES_ReadOscillatorBenchmark(void);
static UInt32 gpHalES_FilterRCBenchmarkMeasurement(UInt32 benchmark);
#ifdef GP_SCHED_DIVERSITY_SLEEP
static void gpHalEs_OscillatorBenchmarkAbort(void);
#endif
static void gpHal_OscillatorBenchmark_3Phase_Complete(gpHal_OscillatorBenchmark_Status_t status);
static void gpHalEs_TriggerOscillatorBenchmarkRcMeasurement(void);
static void gpHalES_OscillatorBenchmarkRestart(void);
/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
void gpHalES_EnableOscillatorBenchmark(Bool enable, Bool keepAwake)
{
    // Make sure to keep chip awake during a measurement
    gpHal_KeepAwakeDuringBenchmark = keepAwake;

    if(enable)
    {
        // Before starting, make sure we do not have an interrupt pending
        qDrvObm_TotalMeasDoneIRQClear();
    }

    // Enable circuit itself
    // GP_WB_WRITE_ES_ENABLE_OSCILLATOR_BENCHMARK(enable);
    // Enable clock to circuit
    // GP_WB_WRITE_ES_ENABLE_CLK_TIME_REFERENCE_OSCILLATOR_BENCHMARK_BY_UC(enable);
    qDrvBist_TotalMeasDoneIRQEnable(enable);
}

UInt32 gpHalES_GetBenchmarkAverage(UInt8 measurementHistory)
{
    UInt32 total = 0;
    UInt8 nrOfMeasurements = 0;
    UInt32 avg = 0;

    while(nrOfMeasurements < measurementHistory && (gpHal_EsBenchmarkMeasurements[nrOfMeasurements] != 0))
    {
        total += gpHal_EsBenchmarkMeasurements[nrOfMeasurements];
        nrOfMeasurements++;
    }

    gpHalEs_IsQueueFull = (nrOfMeasurements == measurementHistory);
    avg = (nrOfMeasurements > 0) ? total / nrOfMeasurements : GPHAL_ES_BENCHMARK_COUNTER_INVALID;
    return avg;
}

void gpHalEs_StartOscillatorBenchmark(void)
{
#ifdef GP_SCHED_DIVERSITY_SLEEP
    if(gpHal_32kHzCrystalAvailable())
    {
        // Schedule timeout in case something is wrong with the clock source
        // if keepawake is not true, that would mean chip could go to sleep after triggering a
        // benchmark measurement. So in that case, this abort function will not get unscheduled
        // on a valid benchmark done interrupt.So do not schedule this if keep awake is not set.
        if(gpHal_KeepAwakeDuringBenchmark)
        {
            gpSched_ScheduleEvent(MS_TO_US(GPHAL_OSCILLATOR_BENCHMARK_TO_MS), gpHalEs_OscillatorBenchmarkAbort);
        }
    }
#endif // GP_SCHED_DIVERSITY_SLEEP

#ifndef GP_COMP_CHIPEMU
    GP_ASSERT_DEV_INT(false == gpHal_GotoSleepDisabled);
    if(true == gpHal_KeepAwakeDuringBenchmark)
    {
        hal_SleepSetGotoSleepEnable(false);
        gpHal_GotoSleepDisabled = true;
    }

    // Perform benchmark measurement
    // GP_LOG_PRINTF("gts %d gfaw %d gtsc %ld", 0, gpHal_GotoSleepDisabled, gpHal_CalibrationGetFirstAfterWakeup(),
    qDrvObm_Start();
#endif // GP_COMP_CHIPEMU
    // Probably not needed anymore
    __DSB();
}

#ifdef GP_SCHED_DIVERSITY_SLEEP
void gpHalEs_OscillatorBenchmarkAbort(void)
{
    switch(gpHal_background_benchmark_mode)
    {
        case gpHal_SleepMode32kHz:
            if(gpHal_32kHzCrystalAvailable())
            {
                qDrvObm_Stop();
                gpHal_OscillatorBenchmark_3Phase_Complete(gpHal_OscillatorBenchmark_Result_Broken);
            }
            break;
        case gpHal_SleepModeRC:
            GP_LOG_SYSTEM_PRINTF("Warn! RC bm abort", 0);
            /* Disabling the OBM circuit and clearing mask, so that
               periodic re-calibrations can continue */
            gpHalES_EnableOscillatorBenchmark(false, false);
            break;
        default:
            GP_ASSERT_DEV_INT(false);
            break;
    }

    if(true == gpHal_GotoSleepDisabled)
    {
        hal_SleepSetGotoSleepEnable(true);
        gpHal_GotoSleepDisabled = false;
    }
}
#endif

void gpHalEs_TriggerOscillatorBenchmarkRcMeasurement(void)
{
    if(GPHAL_ES_BENCHMARK_COUNTER_INVALID == gpHal_SleepModeRC_stable_benchmark)
    {
        UInt32 tnow = GP_WB_READ_ES_SYSTEM_TIME_US();
        gpHalES_StartBackground_Benchmark(gpHal_SleepModeRC, tnow);
    }
}

/* Called regularly via calibration framework to re-benchmark the oscillator frequency. */
void gpHalEs_RCCalibrationHandler(const gpHal_CalibrationTask_t* pTask)
{
    NOT_USED(pTask);
    gpHal_SleepMode_t sleepMode = gpHal_GetSleepMode();
    Bool isFirstCalibAfterWakeup = gpHal_CalibrationGetFirstAfterWakeup();

    /* return if the current selected sleep mode is not RC*/
    if(sleepMode != gpHal_SleepModeRC)
    {
        return;
    }

    GP_LOG_PRINTF("LJRC recal", 0);
    /* Detect if chip went to sleep after triggering calibration (KEEP_AWAKE = 0) while calibration RC oscillator */
    if(isFirstCalibAfterWakeup && qDrvBist_TotalMeasDoneIRQEnabled() &&
       (gpHal_background_benchmark_mode == gpHal_SleepModeRC))
    {
        /* Chip can go to sleep after triggering measurement only if keep awake is zero */
        if(gpHal_KeepAwakeDuringBenchmark == 0)
        {
            gpHalES_EnableOscillatorBenchmark(false, false);
        }
    }

    if(isFirstCalibAfterWakeup || gpHalEs_RCCalibProp.EnableMultipleMeasurements)
    {
        // if multiple measurements in wake cycle is not defined then do recalibrations only
        // once after wake up
        gpHalES_TriggerOscillatorBenchmark();
    }
}

#ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
void gpHalEs_Xt32KCalibrationHandler(const gpHal_CalibrationTask_t* pTask)
{
    NOT_USED(pTask);
    gpHal_SleepMode_t sleepMode = gpHal_GetSleepMode();
    Bool isFirstCalibAfterWakeup = gpHal_CalibrationGetFirstAfterWakeup();

    /* return if the current selected sleep mode is not xt32k*/
    if(sleepMode != gpHal_SleepMode32kHz)
    {
        return;
    }

    GP_LOG_PRINTF("Xt32k recal", 0);
    /* Detect if chip went to sleep after triggering calibration (KEEP_AWAKE = 0) while calibration RC oscillator */
    if(isFirstCalibAfterWakeup && qDrvBist_TotalMeasDoneIRQEnabled() &&
       (gpHal_background_benchmark_mode == gpHal_SleepModeRC))
    {
        /* Chip can go to sleep after triggering measurement only if keep awake is zero */
        if(gpHal_KeepAwakeDuringBenchmark == 0)
        {
            gpHalES_EnableOscillatorBenchmark(false, false);
        }
    }

    gpHalES_TriggerOscillatorBenchmark();
}
#endif // #ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void gpHal_InitEs(void)
{
    UInt16 rcSleepClockNofTicks;
    UInt8 queueDepth;
    Bool enMultMeasurements;
    UIntLoop i;

    gpHalEs_IsQueueFull = false;
    gpHalES_SleepModeRC_TsLastBenchmark = 0;
    gpHal_EsBenchmarkCurrentIndex = 0;
    for(i = 0; i < GP_HAL_ES_MAX_NR_OF_BENCHMARK_MEASUREMENTS_HISTORY; i++)
    {
        gpHal_EsBenchmarkMeasurements[i] = 0;
    }

    rcSleepClockNofTicks = DEFAULT_RC_SLEEP_CLOCK_NOF_TICKS;
    queueDepth = DEFAULT_RC_SLEEP_CLOCK_BENCHMARK_QUEUE_DEPTH;
    enMultMeasurements = DEFAULT_RC_SLEEP_CLOCK_BENCHMARK_ENABLE_MULTIPLE_MEASUREMENTS;

    /* Default initialization of RC sleep clock calibration */
    gphalES_RCSleepClockCalibrationInit(rcSleepClockNofTicks, queueDepth, enMultMeasurements);

    calTaskHandleRC = GP_HAL_CALIBRATION_INVALID_TASK_HANDLE;
    calTaskHandleXt32K = GP_HAL_CALIBRATION_INVALID_TASK_HANDLE;

#ifdef GP_COMP_GPHAL_ES_ABS_EVENT
    gpHal_ES_AbsoluteEventsInUse = 0;
    //initialize events base pointer value
    GP_WB_WRITE_ES_EVENTS_BASE_PTR(SYSRAM_ES_EVENTS_START);
    //enable interrupts
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPT(true);
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPTS(0);
    NVIC_EnableIRQ((IRQn_Type)ES_IRQn);
    NVIC_SetPriority((IRQn_Type)ES_IRQn, 2);
#endif //GP_COMP_GPHAL_ES_ABS_EVENT
}

void gphalES_PeriodMeasDoneCb(void)
{
    // Do nothing
}

void gphalES_TotalMeasDoneCb(void)
{
    gpHalES_OscillatorBenchmarkDone_Handler();
}

void gphalES_RCSleepClockCalibrationInit(UInt16 nofRCSleepClockTicks, UInt8 QueueDepth, Bool EnableMultipleMeasurments)
{
    gpHalEs_RCCalibProperties* calibprop = &gpHalEs_RCCalibProp;
    GP_ASSERT_SYSTEM((nofRCSleepClockTicks > 0) & (nofRCSleepClockTicks <= 0x0FFF));
    GP_ASSERT_SYSTEM((QueueDepth > 0) && (QueueDepth <= GP_HAL_ES_MAX_NR_OF_BENCHMARK_MEASUREMENTS_HISTORY));

    calibprop->is_initialized = true;
    calibprop->NoOfLPTicks = nofRCSleepClockTicks;
    calibprop->QueueDepth = QueueDepth;
    calibprop->EnableMultipleMeasurements = EnableMultipleMeasurments;

    /* Flush the benchmark measurement queue */
    MEMSET(gpHal_EsBenchmarkMeasurements, 0, sizeof(gpHal_EsBenchmarkMeasurements));
    gpHal_EsBenchmarkCurrentIndex = 0;
    gpHalEs_IsQueueFull = false;
    /* if the sleep clock calibration parameters are changed in between an ongoing RC sleep clock calibration
     * then reset the oscillator benchmark and trigger new calibration*/
    if(gpHal_background_benchmark_mode == gpHal_SleepModeRC)
    {
        gpHal_SleepModeRC_stable_benchmark = GPHAL_ES_BENCHMARK_COUNTER_INVALID;
        if(qDrvBist_TotalMeasDoneIRQEnabled())
        {
            gpHalES_EnableOscillatorBenchmark(false, false);
            gpHalES_TriggerOscillatorBenchmark();
        }
    }
}

void gphalES_getRCSleepClockCalibrationParameters(UInt16* nofRCSleepClockTicks, UInt8* QueueDepth,
                                                  Bool* EnableMultipleMeasurments)
{
    gpHalEs_RCCalibProperties* calibprop = &gpHalEs_RCCalibProp;
    if(calibprop->is_initialized)
    {
        *nofRCSleepClockTicks = calibprop->NoOfLPTicks;
        *QueueDepth = calibprop->QueueDepth;
        *EnableMultipleMeasurments = calibprop->EnableMultipleMeasurements;
    }
    else
    {
        *nofRCSleepClockTicks = 0;
        *QueueDepth = 0;
        *EnableMultipleMeasurments = 0;
    }
}

void gpHal_StartInitialBenchmarkMeasurements(void)
{
    UInt32 tnow = GP_WB_READ_ES_SYSTEM_TIME_US();
    static qDrvObm_Config_t obmCfg = Q_DRV_OBM_CONFIG_DEFAULT(qDrvObm_ClkSrcRc64k, 0, DEFAULT_RC_SLEEP_CLOCK_NOF_TICKS);
    static const qDrvObm_Callbacks_t cbs = {
        .periodMeasDone = NULL,
        .totalMeasDone = gphalES_TotalMeasDoneCb
    };

    qResult_t result = qDrvObm_Init(&obmCfg, &cbs);
    GP_ASSERT_DEV_INT(result == Q_OK);
    if(gpHal_32kHzCrystalAvailable())
    {
        /* The DCDC should not start before calibration is done SW-6242 */
#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
        gpHalES_blockedDCDC = true;
        gpHalDCDC_BlockDCDC(true);
#endif // GP_DIVERSITY_GPHAL_DCDC_ENABLED

        gpHalEs_32kHz_benchmark_status = gpHal_SleepClockMeasurementStatusPending;
        gpHal_OscillatorBenchmark_3Phase_Init(GP_DIVERSITY_GPHAL_32KHZ_INIT_CALIBRATION_MIN_BENCHMARKS);
        gpHalES_StartBackground_Benchmark(gpHal_SleepMode32kHz, tnow);
    }
    else
    {
        gpHalES_StartBackground_Benchmark(gpHal_SleepModeRC, tnow);
    }
}

Bool gpHalES_AdvancedCalibrationAndReconversionBusy(void)
{
    UInt8 timeReferenceState = GP_WB_READ_ES_TIME_REFERENCE_STATE();
    return ((GP_WB_GET_ES_APPLY_ADVANCED_CALIBRATION_BUSY_FROM_TIME_REFERENCE_STATE(timeReferenceState) == 1) ||
            (GP_WB_GET_ES_SYSTEM_TIME_UPDATED_SINCE_LAST_CALIBRATION_FROM_TIME_REFERENCE_STATE(timeReferenceState) == 0) ||
            (GP_WB_GET_ES_RECONVERSION_BUSY_FROM_TIME_REFERENCE_STATE(timeReferenceState) == 1)
           );
}

static UInt32 gpHalES_GetCalibrationFactor(UInt8 sleepMode)
{
    UInt32 calibrationFactor;

    GP_LOG_PRINTF("BMV xtl %ld, rc %ld", 0, gpHal_SleepMode32kHz_stable_benchmark, gpHal_SleepModeRC_stable_benchmark);
    if(gpHal_SleepMode32kHz == sleepMode)
    {
        GP_ASSERT_DEV_INT(gpHal_SleepMode32kHz_stable_benchmark != GPHAL_ES_BENCHMARK_COUNTER_INVALID);
        /**
         * Divide 2, as 32KHz sampled with 64MHz OBM
         *  calibrationFactor should be within  +/- 5% range of 0x3D090000
         */
        calibrationFactor = (gpHal_SleepMode32kHz_stable_benchmark >> 1) *
                            (GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR >> GP_HAL_ES_BENCHMARK_POWER_XTAL);

        GP_LOG_PRINTF("64MHz ticks in 1 XT32K cycle %ld ", 0,
                      gpHal_SleepMode32kHz_stable_benchmark >> GP_HAL_ES_BENCHMARK_POWER_XTAL);

        GP_LOG_PRINTF("cf %ld", 0, calibrationFactor);
        if(!RANGE_CHECK(calibrationFactor,
                        GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K - GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K_5_PERC,
                        GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K + GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_XT32K_5_PERC))
        {
            GP_LOG_PRINTF("calibrationFactor deviates more than 5 percentage", 0);
        }
    }
    else if(gpHal_SleepModeRC == sleepMode)
    {
        GP_ASSERT_DEV_INT(gpHal_SleepModeRC_stable_benchmark != GPHAL_ES_BENCHMARK_COUNTER_INVALID);

        calibrationFactor = (GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR / gpHalEs_RCCalibProp.NoOfLPTicks) *
                            gpHal_SleepModeRC_stable_benchmark;

        GP_LOG_PRINTF("64MHz ticks in 1 RC64K cycle %ld ", 0,
                      (gpHal_SleepModeRC_stable_benchmark / gpHalEs_RCCalibProp.NoOfLPTicks));

        GP_LOG_PRINTF("cf %ld", 0, calibrationFactor);
        if(!RANGE_CHECK(calibrationFactor,
                        GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC - GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC_5_PERC,
                        GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC + GP_WB_ES_NOMINAL_CALIBRATION_FACTOR_LFRC_5_PERC))
        {
            GP_LOG_PRINTF("calibrationFactor deviates more than 5 percentage", 0);
        }
    }
    else
    {
        // 16 MHz sleep mode - sleep counter is derived from active time reference so always perfectly calibrated.
        GP_ASSERT_DEV_INT(gpHal_SleepMode16MHz == sleepMode);
        calibrationFactor = GP_WB_ES_BENCHMARK_CALIBRATION_FACTOR_16MHZ;
        GP_LOG_PRINTF("cf %ld", 0, calibrationFactor);
    }

    gpLog_Flush();

    return calibrationFactor;
}

UInt32 gpHalES_GetOscillatorBenchmark(gpHal_SleepMode_t sleepMode)
{
    UInt32 oscillatorBenchmark = 0;
    if(gpHal_SleepMode32kHz == sleepMode)
    {
        oscillatorBenchmark = gpHal_SleepMode32kHz_stable_benchmark;
    }
    else if(gpHal_SleepModeRC == sleepMode)
    {
        oscillatorBenchmark = gpHal_SleepModeRC_stable_benchmark;
    }
    else
    {
        /* No benchmark avaiable for this sleep mode */
        GP_ASSERT_DEV_INT(false);
    }

    return oscillatorBenchmark;
}

/* Apply sleep counter calibration factor and realign sleep counter to symbol counter.
 *
 * This function is called by the backup handler before entering sleep mode.
 * Must be called before halting the timer reference.
 */
void gpHal_RealignTimebase(void)
{
    UInt32 calibrationFactor;
    UInt8 sleepMode;

    // Determine which calibration factor to use (depends on sleep mode).
    sleepMode = gpHal_GetSleepMode();

    calibrationFactor = gpHalES_GetCalibrationFactor(sleepMode);

    // Write new calibration factor.
    GP_WB_WRITE_ES_CALIBRATION_FACTOR(calibrationFactor);
    GP_WB_WRITE_ES_PHASE_COMPENSATION(0);

    // trigger realignment calibration
    GP_WB_ES_APPLY_REALIGNMENT_CALIBRATION();
    // wait until not busy
    while(GP_WB_READ_ES_APPLY_REALIGNMENT_CALIBRATION_BUSY());

    // wait until symbol counter updated since last calibration
    while(!GP_WB_READ_ES_SYSTEM_TIME_UPDATED_SINCE_LAST_CALIBRATION());

    // there is a 1 es clock cycle gap between assertion of symbol counter updated since last calibration and
    // deassertion of reconversion busy
    HAL_WAIT_US(1);

    // wait until reconversion done
    while(GP_WB_READ_ES_RECONVERSION_BUSY());
}

//-------------------------------------------------------------------------------------------------------
//  CALIBRATION/BENCHMARK FUNCTIONS
//-------------------------------------------------------------------------------------------------------

void gpHalEs_PrepareOscillatorBenchmark(gpHal_SleepMode_t mode)
{
    UInt8 power;
    UInt16 nofLpTicks;
    Bool keepAwake = false;
    Bool isFirstMeasurement;

    if(gpHal_SleepModeRC == mode)
    {
        /*
         * Keep the chip awake for measurements if
         *  - Queue is not full
         *  - First measurement of a wake cycle
         *  - Benchmark data required for temperature correction
         * Chip can go to sleep if it is subsequent measurements of wake cycle
         */
        isFirstMeasurement = gpHal_CalibrationGetFirstAfterWakeup();
        keepAwake = (isFirstMeasurement) || (!gpHalEs_IsQueueFull);
    }
    else
    {
        keepAwake = true;
    }

    gpHalES_EnableOscillatorBenchmark(true, keepAwake);

    COMPILE_TIME_ASSERT(gpHal_SleepModeRC == gpHal_SleepModeEvent); // Warning optimization
    if(gpHal_SleepModeRC == mode)
    {
        nofLpTicks = gpHalEs_RCCalibProp.NoOfLPTicks;
        qDrvObm_ClkSrcSet(qDrvObm_ClkSrcRc64k);
        qDrvObm_EdgeCountThresholdSet(nofLpTicks);
        GP_LOG_PRINTF("nof_lp_ticks %d pAwake %d", 0, nofLpTicks, keepAwake);
        /* Log when the RC calibration is triggered */
        GP_STAT_SAMPLE_TIME();
    }
    else if(gpHal_SleepMode32kHz == mode)
    {
        power = GP_HAL_ES_BENCHMARK_POWER_XTAL;
        // Only needed for more accurate measurements
        // Value of 0 => 1 edge, i.e. 0 full clock cycles., 1 => 2 edges, i.e. 1 full clock cycle!
        qDrvObm_ClkSrcSet(qDrvObm_ClkSrcXt32k);
        qDrvObm_EdgeCountThresholdSet((0x1 << power));
        GP_LOG_PRINTF("edgecount %ld", 0, (unsigned long)(0x1 << power));
    }
    else
    {
        // Unsupported benchmark mode
        GP_ASSERT_DEV_INT(false);
        return;
    }

    gpHalEs_StartOscillatorBenchmark();
}

void gpHalES_setupSleepClockRecalibration(gpHal_SleepMode_t mode)
{
    Bool RCTaskInvalid = (calTaskHandleRC == GP_HAL_CALIBRATION_INVALID_TASK_HANDLE);
#ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
    Bool Xt32kTaskInvalid = (calTaskHandleXt32K == GP_HAL_CALIBRATION_INVALID_TASK_HANDLE);
#endif
    gpHal_CalibrationTask_t calTask;
    MEMSET(&calTask, 0, sizeof(gpHal_CalibrationTask_t));

    /* create calibration task to re-trigger calibrations based on the sleep mode */
    if((mode == gpHal_SleepModeRC && RCTaskInvalid)
#ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
       || (mode == gpHal_SleepMode32kHz && Xt32kTaskInvalid)
#endif
    )
    {
        if(mode == gpHal_SleepModeRC)
        {
            calTask.flags = GP_HAL_CALIBRATION_FLAG_PERIODIC | GP_HAL_CALIBRATION_FLAG_CALIBRATE_ON_CHIP_WAKEUP;
            calTask.calibrationPeriod = GP_HAL_RC_BENCHMARK_CALIBRATION_PERIOD_US;
            calTaskHandleRC = gpHal_CalibrationCreateTask(&calTask, gpHalEs_RCCalibrationHandler);
            GP_ASSERT_DEV_EXT(calTaskHandleRC != GP_HAL_CALIBRATION_INVALID_TASK_HANDLE);
        }
#ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
        else
        {
            calTask.flags = GP_HAL_CALIBRATION_FLAG_PERIODIC;
            calTask.calibrationPeriod = (GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_PERIOD_MS * 1000);
            calTaskHandleXt32K = gpHal_CalibrationCreateTask(&calTask, gpHalEs_Xt32KCalibrationHandler);
            GP_ASSERT_DEV_EXT(calTaskHandleXt32K != GP_HAL_CALIBRATION_INVALID_TASK_HANDLE);
        }
#endif
    }
}

void gpHalES_StartBackground_Benchmark(gpHal_SleepMode_t sleepMode, UInt32 tnow)
{
    GP_LOG_PRINTF("TriggerBackground mode %x", 0, sleepMode);
    qDrvObm_ClkSrc_t clkSrc;

    GP_ASSERT_DEV_INT(qDrvObm_StateGet() == qDrvObm_StateIdle);

    gpHal_background_benchmark_mode = sleepMode;
    if(sleepMode == gpHal_SleepModeRC)
    {
        clkSrc = qDrvObm_ClkSrcRc64k;
        /*
         * TODO - LastBenchmark time stamp is not necessary in the new approach
         * Check if this can be deleted.
         */
        gpHalES_SleepModeRC_TsLastBenchmark = tnow;
        GP_ASSERT_SYSTEM(gpHalEs_RCCalibProp.is_initialized == true);
    }
    else
    {
        clkSrc = qDrvObm_ClkSrcXt32k;
    }

    qDrvObm_ClkSrcSet(clkSrc);
    gpHalEs_PrepareOscillatorBenchmark(sleepMode);
}

gpHalES_BenchmarkResult_t gpHalES_TriggerOscillatorBenchmark(void)
{
    gpHal_SleepMode_t sleepMode;
    UInt32 tNow;
    if(qDrvBist_TotalMeasDoneIRQEnabled())
    {
        /* This calibration was started, no other calibration may interrupt it */
        return gpHalES_BenchmarkResult_Busy;
    }

    // Current time in us
    tNow = GP_WB_READ_ES_SYSTEM_TIME_US();

    sleepMode = gpHal_GetSleepMode();


    if(sleepMode == gpHal_SleepModeRC)
    {
        // gpHal_SleepMode32kHz_stable_benchmark stays valid forever, averaging filters just gets fed with new sample
        gpHalES_StartBackground_Benchmark(gpHal_SleepModeRC, tNow);
    }


#ifdef GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
    if(sleepMode == gpHal_SleepMode32kHz)
    {
        GP_ASSERT_DEV_EXT(gpHal_32kHzCrystalAvailable());

#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
        if(!gpHalES_blockedDCDC) /* block is reference counted so we only do it once */
        {
            gpHalDCDC_BlockDCDC(true);
            gpHalES_blockedDCDC = true;
        }
        if(gpHalDCDC_IsActive())
        {
            gpHal_SetCalibrationPendingOnWakeup(calTaskHandleXt32K);
            return gpHalES_BenchmarkResult_ActiveDCDC;
        }
        else
#endif // GP_DIVERSITY_GPHAL_DCDC_ENABLED
        {
            gpHal_OscillatorBenchmark_3Phase_Init(GP_DIVERSITY_GPHAL_32KHZ_REDO_CALIBRATION_MIN_BENCHMARKS);
            gpHalES_StartBackground_Benchmark(gpHal_SleepMode32kHz, tNow);
#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
            // clear pending flag if set earlier due to active DCDC
            gpHal_ClearCalibrationPendingOnWakeup(calTaskHandleXt32K);
#endif /*GP_DIVERSITY_GPHAL_DCDC_ENABLED */
        }
    }
#else // GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
    (void)gpHalES_blockedDCDC;
#endif
#endif // GP_DIVERSITY_GPHAL_32KHZ_ENABLE_RECALIBRATION
    return gpHalES_BenchmarkResult_Success;
}

UInt32 gpHalES_ReadOscillatorBenchmark(void)
{
    UInt32 benchmark;

    // Read-out benchmark result
    benchmark = qDrvObm_TotalMeasGet();

    return benchmark;
}

#ifndef GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB
#define GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB
/* This is the old behaviour where the stack falls back to RC mode automatically */
void gpHal_cb32kHzCalibrationDone(gpHal_SleepClockMeasurementStatus_t status, UInt32 mse)
{
    if(gpHal_SleepClockMeasurementStatusNotStable == status &&
       gpHal_SleepMode32kHz == gpHal_GetSleepMode() /* i.e. don't change if we're in 16Mhz mode! */)
    {
        GP_LOG_SYSTEM_PRINTF("xt32k unstable: switching to RC sleep mode", 0);
        gpHal_SetSleepMode(gpHal_SleepModeRC);
    }
}
#endif // GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB

void gpHal_OscillatorBenchmark_3Phase_Complete(gpHal_OscillatorBenchmark_Status_t status)
{
    GP_LOG_PRINTF("xtldone s=%x ", 0, status);
    gpHalES_EnableOscillatorBenchmark(false, false);

#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
    /* If nothing else is blocking it this will start the DCDC */
    gpHalDCDC_BlockDCDC(false);
    gpHalES_blockedDCDC = false;
#endif // GP_DIVERSITY_GPHAL_DCDC_ENABLED

    switch(status)
    {
        case gpHal_OscillatorBenchmark_Result_Stable:
            gpHalEs_32kHz_benchmark_status = gpHal_SleepClockMeasurementStatusStable;
            gpHal_SleepMode32kHz_stable_benchmark = gpHal_OscillatorBenchmark_MSE_GetStableValue();
            GP_LOG_PRINTF("bmv=%ld ", 0, gpHal_SleepMode32kHz_stable_benchmark);
            break;
        case gpHal_OscillatorBenchmark_Result_Unstable:
            gpHalEs_32kHz_benchmark_status = gpHal_SleepClockMeasurementStatusStable;
            gpHal_SleepMode32kHz_stable_benchmark = gpHal_OscillatorBenchmark_3Phase_GetAvg();
            GP_LOG_PRINTF("bmv2=%ld ", 0, gpHal_SleepMode32kHz_stable_benchmark);
            break;
        case gpHal_OscillatorBenchmark_Result_Broken:
        default:
        {
            if(true == gpHal_GotoSleepDisabled)
            {
                hal_SleepSetGotoSleepEnable(true);
                gpHal_GotoSleepDisabled = false;
            }
            GP_LOG_SYSTEM_PRINTF("HW error: 32kHz crystal benchmark timeout!", 0);
            gpHalEs_32kHz_benchmark_status = gpHal_SleepClockMeasurementStatusNotStable;
            gpHal_SleepMode32kHz_stable_benchmark = GPHAL_ES_BENCHMARK_COUNTER_INVALID;
            gpHal_Set32kHzCrystalAvailable(false);
        }
    }

#if defined(GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB)
    UInt32 mse = gpHal_OscillatorBenchmark_3Phase_GetMSE();
    gpHal_cb32kHzCalibrationDone(gpHalEs_32kHz_benchmark_status, mse);
#endif // GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB

    // Triggering initial RC calibration after initial 32kHz XT calibration
    gpHalEs_TriggerOscillatorBenchmarkRcMeasurement();
}

void gpHalES_OscillatorBenchmarkDone_Handler(void)
{
    halEs_currentBenchmarkMeasurement = gpHalES_ReadOscillatorBenchmark();
    gpSched_ScheduleEvent(0, gpHalES_OscillatorBenchmarkRestart);
}

void gpHalES_OscillatorBenchmarkRestart(void)
{
    gpHal_OscillatorBenchmark_Status_t status;
    UInt32 benchmark = halEs_currentBenchmarkMeasurement;

    if(true == gpHal_GotoSleepDisabled)
    {
        gpHal_GotoSleepDisabled = false;
        hal_SleepSetGotoSleepEnable(true);
    }

#ifdef GP_SCHED_DIVERSITY_SLEEP
    if(gpHal_32kHzCrystalAvailable())
    {
        /* Unschedule timeout that was registered to handle failure
         * when something is wrong with the clock source
         */
        gpSched_UnscheduleEvent(gpHalEs_OscillatorBenchmarkAbort);
    }
#endif // GP_SCHED_DIVERSITY_SLEEP

    if(benchmark != GPHAL_ES_BENCHMARK_COUNTER_INVALID)
    {
#ifdef GP_HAL_CB_BENCHMARK_DONE
        GP_HAL_CB_BENCHMARK_DONE(benchmark);
#endif
    }

    GP_LOG_PRINTF("bm_mode %d bm:%ld", 0, gpHal_background_benchmark_mode, benchmark);

    if(gpHal_SleepMode32kHz == gpHal_background_benchmark_mode)
    {
        status = gpHal_OscillatorBenchmark_3Phase_Add(benchmark);

        if(gpHal_OscillatorBenchmark_Result_NeedMoreSamples != status)
        {
            gpHal_OscillatorBenchmark_3Phase_Complete(status);
        }
        else
        {
            gpHalEs_StartOscillatorBenchmark();
        }
    }
    else if(gpHal_SleepModeRC == gpHal_background_benchmark_mode)
    {
        /* Log when the RC calibration interrupt is handled */
        GP_STAT_SAMPLE_TIME();
        if(benchmark != GPHAL_ES_BENCHMARK_COUNTER_INVALID)
        {
            gpHal_SleepModeRC_stable_benchmark = gpHalES_FilterRCBenchmarkMeasurement(benchmark);

            if(!gpHalEs_IsQueueFull)
            {
                // Trigger subsequent benchmarks till the queue is full
                gpHalEs_StartOscillatorBenchmark();
            }
            else
            {
                // Disable the benchmark circuit, will be re-enabled for
                // re-calibration (from calibration framework)
                gpHalES_EnableOscillatorBenchmark(false, false);
            }
        }
        else
        {
            // Re-trigger calibrations if benchmark result is invalid
            gpHalEs_StartOscillatorBenchmark();
        }
    }
    else
    {
        GP_ASSERT_DEV_INT(false);
    }
}

UInt32 gpHalES_FilterRCBenchmarkMeasurement(UInt32 benchmark)
{
    UInt32 filtered;
    UInt8 measurementHistory = gpHalEs_RCCalibProp.QueueDepth;
    UInt32 benchmarkAvg;

    GP_ASSERT_SYSTEM(measurementHistory != 0);
    gpHal_EsBenchmarkMeasurements[gpHal_EsBenchmarkCurrentIndex] = benchmark;
    // circular increment of index
    gpHal_EsBenchmarkCurrentIndex = (gpHal_EsBenchmarkCurrentIndex + 1) % measurementHistory;

    benchmarkAvg = gpHalES_GetBenchmarkAverage(measurementHistory);
    filtered = benchmarkAvg;

    return filtered;
}

void gpHal_SetDefaultStartupSymbolTimes(gpHal_SleepMode_t mode)
{
    UInt32 startupSymbolTime = 0;
    UInt32 longStartupSymbolTime = 0;

    switch(mode)
    {
        case gpHal_SleepModeRC:
        case gpHal_SleepMode32kHz:
        {
            startupSymbolTime = GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_OTHER_US;
            longStartupSymbolTime = GP_HAL_ES_DEFAULT_LONG_STARTUP_SYMBOL_TIME_OTHER_US;
            break;
        }
        case gpHal_SleepMode16MHz:
        {
            startupSymbolTime = GP_HAL_ES_DEFAULT_STARTUP_SYMBOL_TIME_16MHZ_US;
            longStartupSymbolTime = GP_HAL_ES_DEFAULT_LONG_STARTUP_SYMBOL_TIME_16MHZ_US;
            break;
        }
        default:
        {
            // Unknown sleep mode
            GP_ASSERT_DEV_INT(false);
            break;
        }
    }

    gpHalES_SetSleepAndWakeupTimes(startupSymbolTime, longStartupSymbolTime);
}

void gpHal_ResetHandler_SetSymbolStartupTime(void)
{
    UInt32 startupSymbolTime;

    // Decrement symbol startup time (will be incremented again when an event too late was detected)
    startupSymbolTime = GP_WB_READ_ES_MAX_WAKEUP_DURATION() - GP_HAL_ES_STARTUP_SYMBOL_TIME_DECREMENT_DEFAULT_US;
    if(startupSymbolTime < GP_HAL_ES_MIN_STARTUP_SYMBOL_TIME_OTHER_US)
    {
        startupSymbolTime = GP_HAL_ES_MIN_STARTUP_SYMBOL_TIME_OTHER_US;
    }

    gpHalES_SetSleepAndWakeupTimes(startupSymbolTime, startupSymbolTime + GP_HAL_ES_BACKUP_DURATION_OTHER_US);
}

//-------------------------------------------------------------------------------------------------------
//  ENABLE / DISABLE CALLBACK FUNCTIONS
//-------------------------------------------------------------------------------------------------------

//ES interrupts
#ifdef GP_COMP_GPHAL_ES_ABS_EVENT
void gpHal_EnableAbsoluteEventCallbackInterrupt(UInt8 eventNbr, Bool enable)
{
    //Enable interrupt mask for the selected absolute interrupt
    DISABLE_GP_GLOBAL_INT();
    if(enable)
    {
        GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPTS(BM(eventNbr) | GP_WB_READ_U32(GP_WB_INT_CTRL_MASK_APPUC_ES_INTERRUPTS_ADDRESS));
    }
    else
    {
        GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPTS(~BM(eventNbr) & GP_WB_READ_U32(GP_WB_INT_CTRL_MASK_APPUC_ES_INTERRUPTS_ADDRESS));
    }
    ENABLE_GP_GLOBAL_INT();
}
#endif

//-------------------------------------------------------------------------------------------------------
//  EVENT SCHEDULER FUNCTIONS
//-------------------------------------------------------------------------------------------------------

void gpHal_GetTime(UInt32* pTime)
{
    GP_ASSERT_SYSTEM(GP_WB_READ_ES_INIT_TIME_REFERENCE_BUSY() == 0);
    DISABLE_GP_GLOBAL_INT();
    *pTime = GP_WB_READ_ES_SYSTEM_TIME_US();
    ENABLE_GP_GLOBAL_INT();
}

//-------------------------------------------------------------------------------------------------------
//  ABSOLUTE EVENT SCHEDULER FUNCTIONS
//-------------------------------------------------------------------------------------------------------

#ifdef GP_COMP_GPHAL_ES_ABS_EVENT
gpHal_AbsoluteEventId_t gpHal_GetAbsoluteEvent(void)
{
    UIntLoop i;

    for(i = 0; i < GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS; i++)
    {
        if(!BIT_TST(gpHal_ES_AbsoluteEventsInUse, i))
        {
            BIT_SET(gpHal_ES_AbsoluteEventsInUse, i);
            return i;
        }
    }

    /* Assert instead of returning */
    GP_ASSERT_DEV_EXT(false);

    return GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID;
}

void gpHal_FreeAbsoluteEvent(gpHal_AbsoluteEventId_t eventId)
{
    GP_ASSERT_DEV_EXT(eventId < GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS);
    GP_ASSERT_DEV_EXT(BIT_TST(gpHal_ES_AbsoluteEventsInUse, eventId));
    BIT_CLR(gpHal_ES_AbsoluteEventsInUse, eventId);
}

void gpHal_ScheduleAbsoluteEvent(gpHal_AbsoluteEventDescriptor_t* pAbsoluteEventDescriptor, gpHal_AbsoluteEventId_t eventNbr)
{
    GP_ASSERT_DEV_EXT(GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > eventNbr);

    DISABLE_GP_GLOBAL_INT();

    UInt32 ev_base;

    ev_base = IDX_TO_BASE(GP_WB_READ_ES_EVENTS_BASE_PTR(), eventNbr);

    GP_WB_WRITE_ABS_EV_EXECUTION_TIME(ev_base, pAbsoluteEventDescriptor->exTime);
    GP_WB_WRITE_ABS_EV_RECURRENCE_PERIOD(ev_base, pAbsoluteEventDescriptor->recPeriod);
    GP_WB_WRITE_ABS_EV_RECURRENCE_AMOUNT(ev_base, pAbsoluteEventDescriptor->recAmount);

    // options
    UInt32 options = 0;
    GP_WB_SET_ABS_EV_EXECUTE_IF_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->executionOptions & GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_FIRST_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_FIRST_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_OTHERS_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_OTHERS_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_LAST_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_LAST_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_FIRST_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_FIRST_TOO_LATE) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_OTHERS_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_OTHERS_TOO_LATE) ? 1 : 0);
    GP_WB_SET_ABS_EV_INTERRUPT_ON_LAST_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->interruptOptions & GP_ES_INTERRUPT_OPTIONS_ON_LAST_TOO_LATE) ? 1 : 0);

    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_FIRST_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_FIRST_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_OTHERS_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_OTHERS_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_LAST_ON_TIME_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_LAST_ON_TIME) ? 1 : 0);
    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_FIRST_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_FIRST_TOO_LATE) ? 1 : 0);
    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_OTHERS_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_OTHERS_TOO_LATE) ? 1 : 0);
    GP_WB_SET_ABS_EV_QUIC_TRIGGER_ON_LAST_TOO_LATE_TO_OPTIONS(options, (pAbsoluteEventDescriptor->quicTriggers & GP_ES_QUIC_TRIGGER_OPTIONS_ON_LAST_TOO_LATE) ? 1 : 0);

    GP_WB_SET_ABS_EV_STATE_TO_OPTIONS(options, pAbsoluteEventDescriptor->control);
    GP_WB_WRITE_ABS_EV_OPTIONS(ev_base, options);

    GP_WB_WRITE_ES_SET_VALID_EVENTS(BM(eventNbr));
    GP_WB_ES_REEVALUATE_EVENTS(); //refresh

    ENABLE_GP_GLOBAL_INT();
}

gpHal_EventState_t gpHal_UnscheduleAbsoluteEvent(gpHal_AbsoluteEventId_t eventNbr)
{
    gpHal_EventState_t state;

    GP_ASSERT_DEV_EXT(GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > eventNbr);

    DISABLE_GP_GLOBAL_INT();
    state = (gpHal_EventState_t)GP_WB_READ_ES_EVENT_HANDLER_STATE();

    if(state != gpHal_EventStateDone)
    {
        GP_WB_WRITE_ES_CLR_VALID_EVENTS(BM(eventNbr));
        GP_WB_ES_REEVALUATE_EVENTS();
    }

    ENABLE_GP_GLOBAL_INT();

    return state;
}

void gpHal_RefreshAbsoluteEvent(gpHal_AbsoluteEventId_t eventNbr, UInt32 absTime, UInt8 control)
{
    GP_ASSERT_DEV_EXT(GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS > eventNbr);

    DISABLE_GP_GLOBAL_INT();
    GP_WB_WRITE_ES_CLR_VALID_EVENTS(BM(eventNbr));

    //be sure the event is not being processed (read) by a event reevaluation triggered by another event
    //reading a single event takes 3.25us (KIWI4), so with a delay of 5us we are safe

    HAL_WAIT_US(5);

    UInt32 ev_base;
    ev_base = IDX_TO_BASE(GP_WB_READ_ES_EVENTS_BASE_PTR(), eventNbr);
    //Update Execution Time
    GP_WB_WRITE_ABS_EV_EXECUTION_TIME(ev_base, absTime);

    //Update Control field
    GP_WB_WRITE_ABS_EV_STATE(ev_base, GP_ES_GET_EVENT_STATE(control));
    GP_WB_WRITE_ABS_EV_RESULT(ev_base, GP_ES_GET_EVENT_RESULT(control));


    GP_WB_WRITE_ES_SET_VALID_EVENTS(BM(eventNbr));
    ENABLE_GP_GLOBAL_INT();

    GP_WB_ES_REEVALUATE_EVENTS(); //refresh
}
#endif //GP_COMP_GPHAL_ES_ABS_EVENT

void gpHal_ResetTime(void)
{
    DISABLE_GP_GLOBAL_INT();

    GP_WB_ES_APPLY_ZERO_CALIBRATION();
    //Wait for application of calibration to finish
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_ES_APPLY_ZERO_CALIBRATION_BUSY(), GP_HAL_DEFAULT_TIMEOUT);
    //Wait for symbol time to be calculated
    GP_DO_WHILE_TIMEOUT_ASSERT(!GP_WB_READ_ES_SYSTEM_TIME_UPDATED_SINCE_LAST_CALIBRATION(), GP_HAL_DEFAULT_TIMEOUT);

    ENABLE_GP_GLOBAL_INT();
}


void gpHal_cbEventTooLate(void)
{
    UInt32 startupSymbolTime;
    gpHal_SleepMode_t sleepMode;

    sleepMode = gpHal_GetSleepMode();

    if(sleepMode != gpHal_SleepMode16MHz)
    {
        startupSymbolTime = GP_WB_READ_ES_MAX_WAKEUP_DURATION();
        startupSymbolTime += GP_HAL_ES_STARTUP_SYMBOL_TIME_INCREMENT_WHEN_TOO_LATE_US;

        if(startupSymbolTime > GP_HAL_ES_MAX_STARTUP_SYMBOL_TIME_OTHER_US)
        {
            startupSymbolTime = GP_HAL_ES_MAX_STARTUP_SYMBOL_TIME_OTHER_US;
        }

        gpHalES_SetSleepAndWakeupTimes(startupSymbolTime, startupSymbolTime + GP_HAL_ES_BACKUP_DURATION_OTHER_US);
    }
}

gpHal_SleepClockMeasurementStatus_t gpHal_GetMeasuredSleepClockFrequency(gpHal_SleepMode_t mode, UInt32* frequencymHz)
{
    gpHal_SleepClockMeasurementStatus_t status = gpHal_SleepClockMeasurementStatusNotStarted;
    UInt32 benchmarkValue;
    UInt16 noOfLpTicks;
    GP_ASSERT_DEV_INT(mode <= gpHal_SleepMode16MHz);

    switch(mode)
    {
        case gpHal_SleepModeRC:
        {
            if(gpHal_SleepModeRC_stable_benchmark == GPHAL_ES_BENCHMARK_COUNTER_INVALID)
            {
                if(gpHalES_SleepModeRC_TsLastBenchmark != 0)
                {
                    // Invalid counter, but TS > 0, measurement started, but not finished
                    status = gpHal_SleepClockMeasurementStatusPending;
                }
            }
            else
            {
                // Valid counter, consider stable
                status = gpHal_SleepClockMeasurementStatusStable;
            }

            benchmarkValue = gpHal_SleepModeRC_stable_benchmark;
            noOfLpTicks = gpHalEs_RCCalibProp.NoOfLPTicks;
            break;
        }
        case gpHal_SleepMode32kHz:
        {
            if(gpHal_32kHzCrystalAvailable())
            {
                status = gpHalEs_Get32kHzBenchmarkStatus();
            }
            benchmarkValue = gpHal_SleepMode32kHz_stable_benchmark;
            noOfLpTicks = (1 << GP_HAL_ES_BENCHMARK_POWER_XTAL);
            break;
        }
        default:
        {
            // Mode 16 MHz or unknown: no measurements performed
            noOfLpTicks = 1;
            benchmarkValue = 0;
            break;
        }
    }

    if(status == gpHal_SleepClockMeasurementStatusStable)
    {
        UInt64 frequencymHz_internal;
        // Calculate frequency
        frequencymHz_internal = ((UInt64)GPHAL_OBM_FREQUENCY_64MHZ * noOfLpTicks) / benchmarkValue;
        frequencymHz_internal *= 1000;
        // Add the milliHz resolution
        frequencymHz_internal +=
            (((UInt64)GPHAL_OBM_FREQUENCY_64MHZ * noOfLpTicks) % benchmarkValue) * 1000 / benchmarkValue;
        *frequencymHz = (UInt32)(frequencymHz_internal & 0xFFFFFFFF);
    }

    return status;
}

gpHal_SleepClockMeasurementStatus_t gpHalEs_Get32kHzBenchmarkStatus(void)
{
    return gpHalEs_32kHz_benchmark_status;
}

gpHal_SleepMode_t gpHalES_GetBackgroundBenchmarkMode(void)
{
    return gpHal_background_benchmark_mode;
}

void gpHalES_SetSleepAndWakeupTimes(UInt32 maxWakeupDuration, UInt32 minSleepDuration)
{
    GP_WB_WRITE_ES_MAX_WAKEUP_DURATION(maxWakeupDuration);
    GP_WB_WRITE_ES_MIN_SLEEP_DURATION(minSleepDuration);

    // Apply and wait for done
    GP_WB_ES_START_RECONVERSION();
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_ES_RECONVERSION_BUSY(), GP_HAL_DEFAULT_TIMEOUT);
}

gpHal_AbsoluteEventId_t gpHalES_StartPeriodicEvent(UInt32 timeStamp, UInt32 interval,
                                                   gpHal_AbsoluteEventCallback_t callback)
{
    gpHal_AbsoluteEventDescriptor_t eventDescriptor;
    gpHal_AbsoluteEventId_t eventId;

    eventId = gpHal_GetAbsoluteEvent();
    GP_ASSERT_DEV_EXT(GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID != eventId);

    MEMSET(&eventDescriptor, 0, sizeof(eventDescriptor));

    eventDescriptor.type = GP_WB_ENUM_EVENT_TYPE_DUMMY;
    eventDescriptor.exTime = timeStamp;
    eventDescriptor.recPeriod = interval;
    eventDescriptor.recAmount = 0xFFFF;
    eventDescriptor.customData = 0x00;
    eventDescriptor.executionOptions = GP_WB_ABS_EV_EXECUTE_IF_TOO_LATE_MASK;
    eventDescriptor.interruptOptions = GP_ES_INTERRUPT_OPTIONS_MASK;
    eventDescriptor.quicTriggers = GP_ES_QUIC_TRIGGER_OPTIONS_MASK;

    eventDescriptor.control = 0x0;

    GP_ES_SET_EVENT_RESULT(eventDescriptor.control, GP_WB_ENUM_EVENT_STATE_INVALID);
    GP_ES_SET_EVENT_STATE(eventDescriptor.control, gpHal_EventStateScheduled);

    gpHal_ScheduleAbsoluteEvent(&eventDescriptor, eventId);
    gpHal_EnableAbsoluteEventCallbackInterrupt(eventId, true);
    gpHal_EnableAbsoluteEventCallbackInterrupt(eventId, true);
    if(callback != NULL)
    {
        gpHal_RegisterAbsoluteEventCallback(callback, eventId);
    }

    return eventId;
}

void gpHalES_StopPeriodicEvent(gpHal_AbsoluteEventId_t eventId)
{
    gpHal_RegisterAbsoluteEventCallback(NULL, eventId);
    gpHal_EnableAbsoluteEventCallbackInterrupt(eventId, false);
    gpHal_UnscheduleAbsoluteEvent(eventId);
}

void gpHal_Set32kHzCrystalAvailable(Bool available)
{
    gpHal_32kHzCrystalAvailableFlag = available;
}

Bool gpHal_32kHzCrystalAvailable(void)
{
    return gpHal_32kHzCrystalAvailableFlag;
}
