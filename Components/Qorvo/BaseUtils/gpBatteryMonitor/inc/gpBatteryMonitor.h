/*
 * Copyright (c) 2011-2012, 2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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


#ifndef _GP_BATTERY_MONITOR_H_
#define _GP_BATTERY_MONITOR_H_

#include "global.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#define gpBatteryMonitor_MeasurementTypeUnloadedWithoutRecovery   0
#define gpBatteryMonitor_MeasurementTypeUnloadedWithRecovery      1
#define gpBatteryMonitor_MeasurementTypeLoaded                    2
typedef UInt8 gpBatteryMonitor_MeasurementType_t;

typedef struct
{
    /* status = false if measurement failed either due to failed ADC measurement or failed loading of radio */
    /* status = true if measurement is successful */
    Bool status;
    /* Measured voltage in 2.6 format, valid only if status == true */
    UQ2_6 voltage;
} gpBatteryMonitor_MeasurementResult;

/* Callback type to be registered for battery measurements which does non-blocking radio claim */
typedef void (*gpBatteryMonitor_cb)(void* measurementResult);

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

#define BATTERY_MONITOR_GET_INTEGER_PART(measVoltage)   ((measVoltage & 0xC0) >> 6)
#define BATTERY_MONITOR_GET_FLOATING_PART(measVoltage) (((measVoltage & 0x3F)*100) >> 6)

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

GP_API void gpBatteryMonitor_Init(void);

/** @brief Measure the supply voltage under different conditions.
* Measurement results are synchronous. This API does blocking wait for radio grant while doing loaded measurements.
*   @param type               Type of measurement to perform
*   @param pVoltage           Pointer in which measured voltage is returned.
                              In 2.6 format - 2 bits V, 6 bits 1/64 volt (in 0.01 units).
    @return                   False if measurement failed (busy or failed ADC measurement). True if success.
*/
GP_API Bool gpBatteryMonitor_Measure(gpBatteryMonitor_MeasurementType_t type, UQ2_6* pVoltage);

#ifndef GP_COMP_TXMONITOR
/** @brief API to register callback that will be called once asynchronous measurement using
gpBatteryMonitor_MeasureWithNonBlockingClaim() is finished */
void gpBatteryMonitor_registerCbNonBlockingMeasurement(gpBatteryMonitor_cb cb);

/** @brief Measure the supply voltage under different conditions.
* Measurement results are asynchronous. Measurements will be passed on to the application via callback registered
* using gpBatteryMonitor_registerCbNonBlockingMeasurement().
* This avoids blocking wait for radio grant (as done by gpBatteryMonitor_Measure()) while doing a loaded measurement.
* For data-streaming BLE-connections with the maximum connection interval, worst case time radio can be claimed by BLE
* is 4s, so a callback could be delayed up to that time.
*   @param type               Type of measurement to perform
    @return                   False if measurement failed (busy). True if success.
*/
Bool gpBatteryMonitor_MeasureWithNonBlockingClaim(gpBatteryMonitor_MeasurementType_t type);
#endif

void gpBatteryMonitor_enableVoltageMeasurement();
void gpBatteryMonitor_triggerVoltageMeasurement();
Bool gpBatteryMonitor_voltageMeasurementReady();
UQ2_6 gpBatteryMonitor_getVoltageMeasurement();
void gpBatteryMonitor_disableVoltageMeasurement();

#ifdef __cplusplus
}
#endif


#endif //_GP_BATTERY_MONITOR_H_
