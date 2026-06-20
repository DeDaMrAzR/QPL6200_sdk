/*
 * Copyright (c) 2011-2016, GreenPeak Technologies
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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
// #define GP_LOCAL_LOG

#include "hal.h"
#include "gpHal.h"
#include "gpHal_Phy.h"
#include "gpBatteryMonitor.h"

#ifdef GP_COMP_TXMONITOR
#include "gpTxMonitor.h"
#endif

#include "gpLog.h"
#include "gpSched.h"

#include "qDrvBistAdc.h"
#if defined(REG_QUIC_PRESENT)
#include "qDrvQUIC.h"
#include "qDrvAkumaTriggers.h"
#endif /* defined(REG_QUIC_PRESENT) */

#include "hal_SupplyVoltageMonitor.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#ifdef GP_COMP_GPHAL_MAC
#define BATTERYMONITOR_CHECK_LOAD() gpHal_GetRxOnWhenIdle()
#else
#define BATTERYMONITOR_CHECK_LOAD() (false)
#endif

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#define BATTERYMONITOR_LOAD_DURATION                                                                                   \
    (2000) // 2000us (length of applying rf-rx load before start of loaded adc measurement)
#define BATTERYMONITOR_RECOVER_DURATION                                                                                \
    (2000) // 2000us (length of idling              before start of unloaded adc measurement)
#define BATTERYMONITOR_DUMMY_VALUE (0xC0) // 3V

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    qDrvBistAdc_t bistAdc;
} gpBatteryMonitor_State_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static gpBatteryMonitor_State_t batMonState;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpBatteryMonitor_Init(void)
{
}

void gpBatteryMonitor_enableVoltageMeasurement()
{
    qDrvBistAdc_Enable(&batMonState.bistAdc);
}
void gpBatteryMonitor_triggerVoltageMeasurement()
{
    qDrvBistAdc_Trigger(&batMonState.bistAdc);
}
Bool gpBatteryMonitor_voltageMeasurementReady()
{
    return qDrvBistAdc_ReadyCheck(&batMonState.bistAdc);
}
UQ2_6 gpBatteryMonitor_getVoltageMeasurement()
{
    // convert from uV to UQ2_6
    return (UQ2_6)(qDrvBistAdc_ResultGet(&batMonState.bistAdc) / 15625);
}
void gpBatteryMonitor_disableVoltageMeasurement()
{
    qDrvBistAdc_Disable(&batMonState.bistAdc);
}


Bool gpBatteryMonitor_Measure(gpBatteryMonitor_MeasurementType_t type, UQ2_6* pVoltage)
{
    Int32 voltageMin;

    NOT_USED(type);

    hal_SupplyVoltageMonitorResultGetBlocking();
    voltageMin = hal_SupplyVoltageMonitorResultGetMin();
    GP_LOG_PRINTF("[BATMON]: measure %ld ", 0, voltageMin);
    if(voltageMin == HAL_BATTERY_VOLTAGE_INVALID)
    {
        return false;
    }

    *pVoltage = (UQ2_6)(voltageMin / 15625);

    GP_LOG_PRINTF("bat: 0x%02x", 0, *pVoltage);
    return true;
}

// register callback for measurement with non-blocking radio claim
void gpBatteryMonitor_registerCbNonBlockingMeasurement(gpBatteryMonitor_cb cb)
{
    // not supported
    GP_ASSERT_DEV_INT(false);
    return;
}

// nonblocking radio claim API
Bool gpBatteryMonitor_MeasureWithNonBlockingClaim(gpBatteryMonitor_MeasurementType_t type)
{
    // not supported
    GP_ASSERT_DEV_INT(false);
    return false;
}
