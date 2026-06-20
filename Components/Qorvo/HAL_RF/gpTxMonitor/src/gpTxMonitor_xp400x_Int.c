/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 *   This file contains the implementation for monitoring voltages during transmits
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_TXMONITOR

// #define GP_LOCAL_LOG

// local defines for ADC_CONFIGURATION
#define CLEARED_BATTERY_VOLTAGE        0x0000
#define CLEARED_POWER_DETECTOR_VOLTAGE 0x0000

#include "gpLog.h"
#include "gpTxMonitor.h"
#include "gpHal.h"
#include "gpBatteryMonitor.h"

#include "qDrvBistAdc.h"

#if !defined(GP_DIVERSITY_GPHAL_XP4002)
#Error Only xp4002 supported !
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct gpTxMonitor_VoltageDescriptor_s {
    Bool valid;
    UQ2_6 voltage;
} gpTxMonitor_VoltageDescriptor_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static gpTxMonitor_VoltageDescriptor_t gpTxMonitor_LoadedBatteryVoltage;
static Bool gpTxMonitor_MeasurementBusy;
static Bool gpTxMonitor_Enabled;

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void gpTxMonitor_Enable(void)
{
    gpTxMonitor_Enabled = true;
}
void gpTxMonitor_Disable(void)
{
    gpTxMonitor_Enabled = false;
}

void gpTxMonitor_Init(UInt8 settlingTimeInSymbols)
{
    NOT_USED(settlingTimeInSymbols);
    gpTxMonitor_MeasurementBusy = false;
    gpTxMonitor_Enabled = true;

    gpTxMonitor_ClearLoadedBatteryVoltage();
}

void gpTxMonitor_AnnounceTxStart(void)
{
    if(gpTxMonitor_MeasurementBusy == true || !gpTxMonitor_Enabled)
    {
        return;
    }

    /* use QUIC to trigger bist adc measurement upon Tx */
    gpBatteryMonitor_enableVoltageMeasurement();
    gpTxMonitor_MeasurementBusy = true;
}

void gpTxMonitor_AnnounceTxFinished(void)
{
    // if gpTxMonitor was disabled, it didn't start the continuous measurement
    if(!gpTxMonitor_MeasurementBusy)
    {
        return;
    }

    Bool timeout;
    GP_DO_WHILE_TIMEOUT((gpBatteryMonitor_voltageMeasurementReady() != 1), 64000, &timeout);
    gpTxMonitor_LoadedBatteryVoltage.valid = !timeout;
    if(gpTxMonitor_LoadedBatteryVoltage.valid)
    {
        gpTxMonitor_LoadedBatteryVoltage.voltage = gpBatteryMonitor_getVoltageMeasurement(); // 2 bit = V , 6 bit = 1/64
                                                                                             // V, since voltage range
                                                                                             // is 2V and VBATT is
                                                                                             // VDDD/2
    }
    else
    {
        gpTxMonitor_LoadedBatteryVoltage.voltage = 0;
    }
    gpBatteryMonitor_disableVoltageMeasurement();

    gpTxMonitor_MeasurementBusy = false;
}

Bool gpTxMonitor_TriggerUnloadedBatteryVoltage(void)
{
    // not supported, use gpBatteryMonitor_Measure
    GP_ASSERT_DEV_INT(false);
    return false;
}

void gpTxMonitor_ClearUnloadedBatteryVoltage(void)
{
    // not supported
    GP_ASSERT_DEV_INT(false);
    return;
}

Bool gpTxMonitor_GetUnloadedBatteryVoltage(UQ2_6* pVoltage)
{
    // not supported
    GP_ASSERT_DEV_INT(false);
    return false;
}

void gpTxMonitor_ClearLoadedBatteryVoltage(void)
{
    gpTxMonitor_LoadedBatteryVoltage.valid = false;
    gpTxMonitor_LoadedBatteryVoltage.voltage = CLEARED_BATTERY_VOLTAGE;
}

Bool gpTxMonitor_GetLoadedBatteryVoltage(UQ2_6* pVoltage)
{
    *pVoltage = gpTxMonitor_LoadedBatteryVoltage.voltage;
    return gpTxMonitor_LoadedBatteryVoltage.valid;
}
