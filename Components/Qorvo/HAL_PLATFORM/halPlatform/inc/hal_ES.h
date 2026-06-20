/*
 * Copyright (c) 2023, Qorvo Inc
 *
 * hal_ES.h
 *
 * This file contains the functions related to the Event Scheduler.
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

#ifndef _HAL_ES_H_
#define _HAL_ES_H_

/**
 * @file hal_ES.h
 * This file contains the functions related to the Event Scheduler.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "hal_kx_ES.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

//Execution options
/** @brief Event exection options mask. */
#define GP_ES_EXECUTION_OPTIONS_MASK 0xF
/** @brief Event exection options : Execute the event even if the trigger time has passed. */
#define GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE 0x4
/** @brief Event exection options : Prohibit standby as long as event is pending */
#define GP_ES_EXECUTION_OPTIONS_PROHIBIT_STANDBY 0x8
/** @brief Event exection options : Execute the event only if the event was triggered on time. */
#define GP_ES_EXECUTION_OPTIONS_NOT_EXECUTE_IF_TOO_LATE 0x0

//Interrupt options
/** @brief Event interrupt options mask (enables all interrupt options).*/
#define GP_ES_INTERRUPT_OPTIONS_MASK 0x3F
/** @brief Event interrupt option: generate interrupt on first event execution that is on time.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_FIRST_ON_TIME 0x01 //First bit
/** @brief Event interrupt option: generate interrupt on event execution other then first or last execution that is on time.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_OTHERS_ON_TIME 0x02 //2nd bit
/** @brief Event interrupt option: generate interrupt on last event execution that is on time.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_LAST_ON_TIME 0x04 //3rd bit
/** @brief Event interrupt option: generate interrupt on first event execution that is too late.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_FIRST_TOO_LATE 0x08 //4th bit
/** @brief Event interrupt option: generate interrupt on execution other then first or last execution that is too late.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_OTHERS_TOO_LATE 0x10 //5th bit
/** @brief Event interrupt option: generate interrupt on last event execution that is too late.*/
#define GP_ES_INTERRUPT_OPTIONS_ON_LAST_TOO_LATE 0x20 //6th bit

/** @brief Event interrupt options mask (enables all interrupt options).*/
#define GP_ES_QUIC_TRIGGER_OPTIONS_MASK               0x3F
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_FIRST_ON_TIME   0x01 // First bit
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_OTHERS_ON_TIME  0x02 // 2nd bit
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_LAST_ON_TIME    0x04 // 3rd bit
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_FIRST_TOO_LATE  0x08 // 4th bit
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_OTHERS_TOO_LATE 0x10 // 5th bit
#define GP_ES_QUIC_TRIGGER_OPTIONS_ON_LAST_TOO_LATE   0x20 // 6th bit

/** @brief Default value for the calibration of the 32kHz crystal */
#define GPHAL_ES_32KHZ_SLEEP_DEFAULT_CALIB 0x3D090000 // 32,768kHz
/** @brief Default value for the calibration of the 16MHz crystal */
#define GPHAL_ES_16MHZ_SLEEP_DEFAULT_CALIB 0x40000000

// Standby/sleep mode
#define GPHAL_ENUM_SLEEP_MODE_RC    GP_WB_ENUM_RTC_TIMER_SRC_LFRC
#define GPHAL_ENUM_SLEEP_MODE_32KHZ GP_WB_ENUM_RTC_TIMER_SRC_XT32K
#define GPHAL_ENUM_SLEEP_MODE_32MHZ GP_WB_ENUM_RTC_TIMER_SRC_XT32M

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

//Result and state fields in the event control field
/** @brief Get the event result (top 4 bits of the control field). */
#define GP_ES_GET_EVENT_RESULT(control) (((control) >> 4) & 0xF)
/** @brief Get the event state (last 4 bits of the control field). */
#define GP_ES_GET_EVENT_STATE(control) ((control)&0xF)
/** @brief Set the event result (top 4 bits of the control field). */
#define GP_ES_SET_EVENT_RESULT(control, result) (control = (control & 0x0F) | (((result) << 4) & 0xF0))
/** @brief Set the event state (last 4 bits of the control field). */
#define GP_ES_SET_EVENT_STATE(control, state) (control = (control & 0xF0) | ((state)&0x0F))

#define GPHAL_ES_ABSOLUTE_EVENT_ID_INVALID 0xFF

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/** @name hal_Result_t*/
//@{
/** @brief The function returned successful. */
#define hal_ResultSuccess             0x0
/** @brief The request was invalid (event not present, ...)*/
#define hal_ResultInvalidRequest      0xc2
/** @brief An invalid parameter was given as a parameter to this function. */
#define hal_ResultInvalidParameter    0xFF
/** @typedef hal_Result_t
 *  @brief The hal_Result_t type defines the result of various HAL functions.
*/
typedef UInt8 hal_Result_t;
//@}

/** @name gpHal_EventType_t */
//@{
/** @typedef gpHal_EventType_t
 *  @brief The gpHal_EventType_t type defines the event type.
*/
typedef UInt8 gpHal_EventType_t;
//@}
//
/** @name gpHal_AbsoluteEventId_t */
//@{
/** @typedef gpHal_AbsoluteEventId_t
 *  @brief The gpHal_AbsoluteEventId_t type holds an absolue event index
*/
typedef UInt8 gpHal_AbsoluteEventId_t;
//@}

//Standby modes
/** @name gpHal_SleepMode_t */
//@{
/** @typedef gpHal_SleepMode_t
 *  @brief The gpHal_SleepMode_t type defines the GP chip sleep mode.
*/
typedef UInt8 gpHal_SleepMode_t;
//@}

/** @brief Event will trigger a dummy event, it only generates an interrupt.*/
#define gpHal_EventTypeDummy        GP_WB_ENUM_EVENT_TYPE_DUMMY

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

// The status of a sleep mode (determined by oscillator benchmark measurements, by comparision against 32 MHz clock)
#define gpHal_SleepClockMeasurementStatusNotStarted 0x00
#define gpHal_SleepClockMeasurementStatusPending    0x01
#define gpHal_SleepClockMeasurementStatusNotStable  0x02
#define gpHal_SleepClockMeasurementStatusStable     0x03

typedef UInt8 gpHal_SleepClockMeasurementStatus_t;

//Event states enumeration
/** @name gpHal_EventState_t */
//@{
/** @brief The event is invalid, will not be executed if execution time is reached.*/
#define gpHal_EventStateInvalid GPHAL_ENUM_EVENT_STATE_INVALID
/** @brief The event is scheduled, it will be executed if execution time is reached.*/
#define gpHal_EventStateScheduled GPHAL_ENUM_EVENT_STATE_SCHEDULED
/** @brief The event is scheduled for immediate, it will be executed as soon as possible.*/
#define gpHal_EventStateScheduledForImmediate GPHAL_ENUM_EVENT_STATE_SCHEDULED_FOR_IMMEDIATE_EXECUTION
/** @brief The event is rescheduled after being triggered before. It will be executed if execution time is reached.*/
#define gpHal_EventStateReScheduled GPHAL_ENUM_EVENT_STATE_RESCHEDULED
/** @brief The event has been executed.*/
#define gpHal_EventStateDone GPHAL_ENUM_EVENT_STATE_DONE
/** @typedef gpHal_EventState_t
 *  @brief The gpHal_EventState_t type defines the Absolute Event state.
*/
typedef UInt8 gpHal_EventState_t;
//@}

/** @name gpHal_EventResult_t */
//@{
/** @brief Event not yet executed. */
#define gpHal_EventResultInvalid GPHAL_ENUM_EVENT_RESULT_UNKNOWN
/** @brief Event was executed on time. */
#define gpHal_EventResultOnTime GPHAL_ENUM_EVENT_RESULT_EXECUTED_ON_TIME
/** @brief Event was executed too late and GP_ES_EXECUTION_OPTIONS_EXECUTE_IF_TOO_LATE was set */
#define gpHal_EventResultTooLate GPHAL_ENUM_EVENT_RESULT_EXECUTED_TOO_LATE
/** @brief Event was executed too late and GP_ES_EXECUTION_OPTIONS_NOT_EXECUTE_IF_TOO_LATE was set */
#define gpHal_EventResultMissed GPHAL_ENUM_EVENT_RESULT_MISSED_TOO_LATE
/** @typedef gpHal_EventResult_t
 *  @brief The gpHal_EventResult_t type defines the event result.
*/
typedef UInt8 gpHal_EventResult_t;
//@}

//Absolute event descriptor
/** @struct gpHal_AbsoluteEventDescriptor
 *  @brief The gpHal_AbsoluteEventDescriptor structure specifying the parameters of an Absolute Event.
 *  @typedef gpHal_AbsoluteEventDescriptor_t
 *  @brief The gpHal_AbsoluteEventDescriptor_t type definition based on the structure gpHal_AbsoluteEventDescriptor.
*/
typedef struct gpHal_AbsoluteEventDescriptor {
    /** This field contains the absolute time (absolute to the symbol counter of the GP chip) at which the event should be executed. */
    UInt32 exTime;
    /** This field contains the period between different periodic executions of this event. This value will only be used by the GP chip when recAmount > 0. */
    UInt32 recPeriod;  //Recurrence Period
                       /** This field contains the amount of recurrences of the event. If set to 0 = 1 execution, set to 1 = 2 executions. Value 0xFFFF indicates an endless recurrent event. */
    UInt16 recAmount;  //Recurrence Amount
                       /** This field contains Custom Data that can be associated with the event. */
    UInt16 customData; //custom uP data
                       /** This field contains the bitfield specifying the execution of the event (see GP_ES_EXECUTION_OPTIONS_MASK).*/
    UInt8 executionOptions;
    /** This field contains the bitfield specifying the interrupts given by the event (see GP_ES_INTERRUPT_OPTIONS_MASK).*/
    UInt8 interruptOptions; //interrupt on_first/others/last_on_time
                            /** This field contains the Event state (see enum gpHal_EventState_t) and the Event result
  * (see enum gpHal_EventResult_t).  The macro GP_ES_SET_EVENT_STATE() (resp. GP_ES_SET_EVENT_RESULT())
  * should be used in order to extract the information and the macro GP_ES_SET_EVENT_STATE() (resp. GP_ES_SET_EVENT_RESULT()) in order to initialize this field.
  *
  * Preferably the result field is written to INVALID at schedule time.  After the event was triggered,
  * this field will return information about the execution status. */
    UInt8 quicTriggers;     // QUIC trggers bitmask
    UInt8 control;          //Control
                            /** This field contains the Event type (see enum gpHal_EventType_t). This specifies what action needs to be performed on execution of the event. */
    UInt8 type;             //Type
} gpHal_AbsoluteEventDescriptor_t;

/** @struct gpHal_ExternalEventDescriptor
 *  @brief The gpHal_ExternalEventDescriptor structure specifying the External Event.
 *  @typedef gpHal_ExternalEventDescriptor_t
 *  @brief The gpHal_ExternalEventDescriptor_t type definition based on the structure gpHal_ExternalEventDescriptor.
*/
typedef struct gpHal_ExternalEventDescriptor {
    /** This field contains the Event type (see enum gpHal_EventType_t). This specifies what action needs to be performed on execution of the event. */
    gpHal_EventType_t type;
} gpHal_ExternalEventDescriptor_t;

/** @typedef gpHal_AbsoluteEventCallback_t
 *  @brief   The gpHal_AbsoluteEventCallback_t callback type definition defines the callback prototype of an Absolute Event interrupt. */
typedef void (*gpHal_AbsoluteEventCallback_t)(void);

/** @name gpHalES_BenchmarkResult_t*/
//@{
/** @brief The function returned successful. */
#define gpHalES_BenchmarkResult_Success 0x0
/** @brief Already running. */
#define gpHalES_BenchmarkResult_Busy 0x1
/** @brief The DCDC is on, it must be disabled first. */
#define gpHalES_BenchmarkResult_ActiveDCDC 0x2
/** @typedef gpHalES_BenchmarkResult_t
 *  @brief The gpHalES_BenchmarkResult_t type defines the result of gpHalES_TriggerOscillatorBenchmark.
 */
typedef UInt8 gpHalES_BenchmarkResult_t;
//@}

/*****************************************************************************
 *                    Public function prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
//ES Enable/Disable callbacks

void gpHal_InitEs(void);

/**
 * @brief This function enables the interrupt line of an Absolute Event interrupt.
 *
 * This function enables the interrupt line of an Absolute Event interrupt by setting the interrupt mask of the Absolute Event with index eventNbr.
 *
 * @param eventNbr The index of the Absolute Event (1..16).
 * @param enable   Enables the interrupt source if true.
*/
GP_API void gpHal_EnableAbsoluteEventCallbackInterrupt(UInt8 eventNbr, Bool enable);

//ES register callbacks
/**
 * @brief Registers the callback for an Absolute Event.
 *
 * This function registers the callback for an Absolute Event. The callback will be executed when the Absolute Event is triggered.
 * The Absolute Event with the correct index needs to be enabled.
 *
 * @param callback The pointer to the callback function.
 * @param eventNbr The index of the Absolute Event (1..16).
*/
GP_API void gpHal_RegisterAbsoluteEventCallback(gpHal_AbsoluteEventCallback_t callback, UInt8 eventNbr);

// ES functions

/** @brief Resets the timebase of the GP chip */
GP_API void gpHal_ResetTime(void);

/**
 * @brief Gets the time of the GP chip.
 *
 * This function returns the current time of the GP chip in us.
 *
 * @param pTime Pointer to the variable where the time will be stored.
*/
GP_API void gpHal_GetTime(UInt32* pTime);

/**
 * @brief Schedules an Absolute Event in the GP chip.
 *
 * This function uploads and activates an event in the GP chip Event Scheduler.  To facilitate a callback on the execution of the event
 * one must register the callback using gpHal_RegisterAbsoluteEventCallback() and enable the interrupt using gpHal_EnableAbsoluteEventCallbackInterrupt().
 *
 * @param pAbsoluteEventDescriptor Pointer to the AbsoluteEventDescriptor_t structure containing the Event options.
 * @param eventNbr                 The index of the Absolute Event (1..16).
*/
GP_API void gpHal_ScheduleAbsoluteEvent(gpHal_AbsoluteEventDescriptor_t* pAbsoluteEventDescriptor, gpHal_AbsoluteEventId_t eventNbr);

/**
 * @brief Allocates an available absolute event id
 *
*/
GP_API gpHal_AbsoluteEventId_t gpHal_GetAbsoluteEvent(void);
/**
 * @brief Frees an allocated absolute event id
 *
*/
GP_API void gpHal_FreeAbsoluteEvent(gpHal_AbsoluteEventId_t EventId);

/**
 * @brief Refreshes an Absolute Event in the GP chip.
 *
 * This function refreshes an already prepared Absolute Event. The event descriptor gpHal_AbsoluteEventDescriptor_t needs to be written as part of the preparation.
 * Writing the gpHal_AbsoluteEventDescriptor_t can be done with GP_ES_WRITE_EVENT_DESCRIPTOR().
 *
 * @param eventNbr                 The index of the Absolute Event (1..16).
 * @param absTime                  The absolute execution time of the event (in us)
 * @param control                  The control field of the event descriptor (see gpHal_AbsoluteEventDescriptor_t).
 *
*/
GP_API void gpHal_RefreshAbsoluteEvent(gpHal_AbsoluteEventId_t eventNbr, UInt32 absTime, UInt8 control);

/**
 * @brief Unschedules an Absolute Event.
 *
 * This function disables the Absolute Event in the GP chip and returns the current EventState.
 * Possible Event States are enumerated in the enumeration gpHal_EventState.
 *
 * @param eventNbr The index of the Absolute Event (1..16).
*/
GP_API gpHal_EventState_t gpHal_UnscheduleAbsoluteEvent(gpHal_AbsoluteEventId_t eventNbr);

// Sleep functions
/**
 * @brief Sets the sleep mode of the GP chip.
 *
 * This function sets the sleep mode of the GP chip. As enumerated under the enumeration gpHal_SleepMode the GP chip can be put into 4 different sleep modes.
 * The desired setting can be made using this function.
 *
 * @param mode The sleepmode enumerated in gpHal_SleepMode.
 * @return gpHal_ResultSuccess only if the operation was successful
*/
GP_API hal_Result_t gpHal_SetSleepMode(gpHal_SleepMode_t mode);

/**
 * @brief Gets the sleep mode of the GP chip.
 *
 * This function returns which sleep mode is currently set. The return mode is returned as enumerated under the enumeration gpHal_SleepMode.
*/
GP_API gpHal_SleepMode_t gpHal_GetSleepMode(void);

/**
 * @brief Gets the actual (measured) deviation of a sleep clock with respect to the 32 MHz clock
 *
 * This function returns whether the measurements for the requested sleep mode have been performed and what the
 * measured frequency is.
*/
GP_API gpHal_SleepClockMeasurementStatus_t gpHal_GetMeasuredSleepClockFrequency(gpHal_SleepMode_t mode, UInt32* frequencymHz);

/**
 * @brief Returns the average sleep clock accuracy of the currently selected sleep clock
 */
GP_API UInt16 gpHal_GetSleepClockAccuracy(void);

/**
 * @brief Get the next average sleep clock accuracy for the given sleep mode.
 * @param nextSleepMode The sleepmode enumerated in gpHal_SleepMode.
 * @return Returns the next average sleep clock accuracy in ppm.
 */
GP_API UInt16 gpHal_GetAverageSleepClockAccuracy(gpHal_SleepMode_t nextSleepMode);

/**
 * @brief Returns the worst-case sleep clock accuracy of the currently selected sleep clock
 */
GP_API UInt16 gpHal_GetWorstSleepClockAccuracy(void);

void gpHal_StartInitialBenchmarkMeasurements(void);
void gpHalES_ApplySimpleCalibration(void);
GP_API gpHalES_BenchmarkResult_t gpHalES_TriggerOscillatorBenchmark(void);
void gpHal_SetDefaultStartupSymbolTimes(gpHal_SleepMode_t mode);
void gpHal_RealignTimebase(void);
void gpHal_cbHalSleepUc(void);
void gpHal_ResetHandler_SetSymbolStartupTime(void);

/** @brief Initializes the parameters for RC sleep clock calibration.
 *         This API can be used to tweak the benchmark time, number of measurements that are
 *         averaged to find a stable benchmark value and also the frequency in which measurements
 *         are done.
 *  @param  nofRCSleepClockTicks     The number of ticks of the RC sleep clock over which the
 *                      benchmark measurement has to be done.
 *  @param  QueueDepth  Set the depth of queue in which the benchmark values are stored.
 *                      A stable benchmark value is calculated by averaging values in queue.
 *  @param  EnableMultipleMeasurments Set this to true to do multiple measurements in a wake cycle.
 *                      If set to zero, once the queue is full, only one measurement is done in
 *                      each wake cycle.
 */
void gphalES_RCSleepClockCalibrationInit(UInt16 nofRCSleepClockTicks, UInt8 QueueDepth, Bool EnableMultipleMeasurments);

/** @brief Helper function to get the parameters set for RC sleep clock calibration
 *
 */
void gphalES_getRCSleepClockCalibrationParameters(UInt16* nofRCSleepClockTicks, UInt8* QueueDepth,
                                                  Bool* EnableMultipleMeasurments);

/**
 * @brief Gets the most recent oscillator benchmark value for given sleep mode
 * @param mode The sleepmode enumerated in gpHal_SleepMode.
 */
UInt32 gpHalES_GetOscillatorBenchmark(gpHal_SleepMode_t sleepMode);

/** @brief Return bitmask of the GPIO pins that triggered the most recent external pin event. */
UInt32 gpHal_GetExternalEventPins(void);

/** @brief Set start-up symbol time and long start-up symbol time */
void gpHalES_SetSleepAndWakeupTimes(UInt32 maxWakeupDuration, UInt32 minSleepDuration);
/** @brief Create calibration tasks to do recalibration of sleep clock */
void gpHalES_setupSleepClockRecalibration(gpHal_SleepMode_t sleepMode);

#if defined(GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB)
/** @brief Always called at the end of the 32kHz calibration.
 *  @param status enumeration that indicates the status of the 32kHz calibration
 *  @param mse The mean square error. Indicates how noisy the 32kHz clock signal is. Smaller is better.
 */
GP_API void gpHal_cb32kHzCalibrationDone(gpHal_SleepClockMeasurementStatus_t status, UInt32 mse);
#endif // GP_DIVERSITY_GPHAL_32KHZ_CALIBRATION_DONE_CB

/*  @brief Reset internal state maintained for a sleep mode */
void gpHalES_resetBenchmarkMeasurement(gpHal_SleepMode_t mode);

/* @brief Enable/Disable oscillator benchmark circuit. keepAwake determines if oscillator
benchmark circuit keeps chip awake to complete a measurement */
void gpHalES_EnableOscillatorBenchmark(Bool enable, Bool keepAwake);
/* @brief Trigger a new oscillator benchmark measurement */
void gpHalEs_StartOscillatorBenchmark(void);
/* @brief Get the sleep clock for which background benchmark measurements are going on*/
gpHal_SleepMode_t gpHalES_GetBackgroundBenchmarkMode(void);
/* @brief Starts ES periodic event */
gpHal_AbsoluteEventId_t gpHalES_StartPeriodicEvent(UInt32 timeStamp, UInt32 interval,
                                                   gpHal_AbsoluteEventCallback_t callback);

/* @brief Stops ES periodic event */
void gpHalES_StopPeriodicEvent(gpHal_AbsoluteEventId_t eventId);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_ES_H_ */
