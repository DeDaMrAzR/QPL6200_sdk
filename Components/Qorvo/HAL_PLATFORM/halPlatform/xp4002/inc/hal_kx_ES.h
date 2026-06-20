/*
 * Copyright (c) 2023, Qorvo Inc
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

#ifndef _HAL_KX_ES_H_
#define _HAL_KX_ES_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpHal_OscillatorBenchmark.h"

#ifdef __cplusplus
extern "C" {
#endif


//Backwards compatibility
#define GPHAL_ENUM_EVENT_STATE_INVALID                           GP_WB_ENUM_EVENT_STATE_INVALID
#define GPHAL_ENUM_EVENT_STATE_SCHEDULED                         GP_WB_ENUM_EVENT_STATE_SCHEDULED
#define GPHAL_ENUM_EVENT_STATE_SCHEDULED_FOR_IMMEDIATE_EXECUTION GP_WB_ENUM_EVENT_STATE_SCHEDULED_FOR_IMMEDIATE_EXECUTION
#define GPHAL_ENUM_EVENT_STATE_RESCHEDULED                       GP_WB_ENUM_EVENT_STATE_RESCHEDULED
#define GPHAL_ENUM_EVENT_STATE_DONE                              GP_WB_ENUM_EVENT_STATE_DONE

#define GPHAL_ENUM_EVENT_RESULT_UNKNOWN           GP_WB_ENUM_EVENT_RESULT_UNKNOWN
#define GPHAL_ENUM_EVENT_RESULT_EXECUTED_ON_TIME  GP_WB_ENUM_EVENT_RESULT_EXECUTED_ON_TIME
#define GPHAL_ENUM_EVENT_RESULT_EXECUTED_TOO_LATE GP_WB_ENUM_EVENT_RESULT_EXECUTED_TOO_LATE
#define GPHAL_ENUM_EVENT_RESULT_MISSED_TOO_LATE   GP_WB_ENUM_EVENT_RESULT_MISSED_TOO_LATE
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
#ifndef GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS
#error GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS not defined
#endif
#endif // defined(GP_COMP_GPHAL_ES_ABS_EVENT)

/** @brief Default value for the calibration of the RC oscilator */
#define GPHAL_ES_RC_SLEEP_DEFAULT_CALIB (0x3E800000)

/** @brief Event will trigger RX on.*/
#define gpHal_EventTypeRXOn GP_WB_ENUM_EVENT_TYPE_MAC_RX_ON0
/** @brief Event will trigger RX off.*/
#define gpHal_EventTypeRXOff GP_WB_ENUM_EVENT_TYPE_MAC_RX_OFF0
/** @brief Event will trigger a dummy event, it only generates an interrupt.*/
#define gpHal_EventTypeDummy GP_WB_ENUM_EVENT_TYPE_DUMMY

/** @brief The GP chip will be in event mode during standby and can only be woken up by an external event.
    The event timer is stopped and can only be restarted by a full reset of the GP chip.*/
#define gpHal_SleepModeEvent GPHAL_ENUM_SLEEP_MODE_RC

/** @brief The GP chip will run a RC timer during standby.*/
#define gpHal_SleepModeRC GPHAL_ENUM_SLEEP_MODE_RC

/** @brief The GP chip will run a 32kHz clock during standby.*/
#define gpHal_SleepMode32kHz GPHAL_ENUM_SLEEP_MODE_32KHZ

/** @brief The GP chip will run a 16MHz clock during standby.*/
#define gpHal_SleepMode16MHz GPHAL_ENUM_SLEEP_MODE_32MHZ

/* typedef UInt8 gpHal_SleepMode_t; */

#define GPHAL_ES_CALIBRATION_FACTOR_USE_PREVIOUS 0xFFFFFFFF

#define GPHAL_ES_BENCHMARK_COUNTER_INVALID 0xFFFFFFFF

#define GP_HAL_ES_BACKUP_DURATION_OTHER_US (UInt16)500

#define GP_HAL_ES_RC64K_TC_CORRECTION_MIN     (-4)
#define GP_HAL_ES_RC64K_TC_CORRECTION_MAX     (3)
#define GP_HAL_ES_RC64K_TC_CORRECTION_INVALID (0x7F)

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*pin and edge are provided by BSP
 */
#define GPHAL_ES_ENABLE_EXT_WAKEUP()  GP_BSP_KX_WRITE_WAKEUP_PIN_MODE(GP_BSP_KX_WAKEUP_EDGE)
#define GPHAL_ES_DISABLE_EXT_WAKEUP() GP_BSP_KX_WRITE_WAKEUP_PIN_MODE(GP_WB_ENUM_WAKEUP_PIN_MODE_NO_EDGE)

#define GPHAL_ES_EVENT_SIZE SYSRAM_ES_EVENT_OFFSET
#define GPHAL_ES_EVENT_NR_TO_START_OFFSET(eventNr)                                                                     \
    ((gpHal_Address_t)(SYSRAM_ES_EVENTS_START + (eventNr)*GPHAL_ES_EVENT_SIZE))

#define GP_ES_IS_OSCILLATOR_BENCHMARK_RUNNING() GP_WB_READ_INT_CTRL_MASK_ES_OSCILLATOR_BENCHMARK_DONE_INTERRUPT()
/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // _HAL_KX_ES_H_
