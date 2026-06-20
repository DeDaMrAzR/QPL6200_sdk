/*
 * Copyright (c) 2024-2025, Qorvo Inc
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
 */

/** @file "AppBattery.cpp"
 *
 * Battery functionality for application with battery as source.
 * The Reading will be set in the power configuration cluster
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"
#include "hal.h"
#include "gpHal.h"

#include "App_Common.h"
#include "AppBattery.h"
#include "ZigbeeEndpoint.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Extern Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Application function prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Default class overrides
 *****************************************************************************/

/*****************************************************************************
 *                    Stack configuration
 *****************************************************************************/

/*****************************************************************************
 *                    Application function definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional callbacks to application
 *****************************************************************************/

/*****************************************************************************
 *                    ZigBee Application API
 *****************************************************************************/

/** @brief Application specific implementation of the PowerDescriptor
          Below implementation is an reference example for battery powered devices
 *        Battery reading will be updated once received ZDO PowerDescriptor Requst.
*/
void CZigBeeDeviceObject::QueryPowerDescriptor(CZigBeeNodePowerDescriptor& descriptor)
{
    // implementation for ZDO node Power Descriptor
    // Details please refer to ZigBee Spec 2.3.2.4 Node Power Descriptor

    // Set PowerDescriptor power mode = (0x02)Rx On when triggered
    descriptor.m_nCurrentPowerMode = CZigBeeNodePowerDescriptor::modeEventTriggeredWakeUp;
    // Set PowerDescriptor Available power source = (0x02)Disposable battery
    descriptor.m_nAvailablePowerSources = CZigBeeNodePowerDescriptor::sourceDisposable;
    // Set PowerDescriptor Current power source = (0x02)Disposable battery
    descriptor.m_nCurrentPowerSource = CZigBeeNodePowerDescriptor::sourceDisposable;

    // Update power source level(Battery)
    Application_UpdateBattery();

    // Assign proper values to the power level dependant on battery'
    if(ZigbeeEndpointMgr().GetBatteryVoltage() <= (APP_BATTERY_CRITICAL_THRESHOLD / 10))
    {
        // battery voltage < critical battery level
        descriptor.m_nCurrentPowerLevel = CZigBeeNodePowerDescriptor::levelCritical;
    }
    else if(ZigbeeEndpointMgr().GetBatteryPercentageRemaining() > 132)
    {
        // battery remaining > 66% (2*66)
        descriptor.m_nCurrentPowerLevel = CZigBeeNodePowerDescriptor::level100Percent;
    }
    else if(ZigbeeEndpointMgr().GetBatteryPercentageRemaining() > 66)
    {
        // battery remaining > 33% (2*33)
        descriptor.m_nCurrentPowerLevel = CZigBeeNodePowerDescriptor::level66Percent;
    }
    else
    {
        // battery remaining < 33%
        descriptor.m_nCurrentPowerLevel = CZigBeeNodePowerDescriptor::level33Percent;
    }
}

/*****************************************************************************
 * ---  Battery monitoring
 *****************************************************************************/

/** @brief Function to retrieve Battery voltage value from system
 *
 *   @return volage                   low voltage threshold in 100mV/unit.
 */
UInt8 Application_GetLowBatteryVoltageThreshold(void)
{
    return (APP_BATTERY_LOW_THRESHOLD / 10);
}

/** @brief Function to retrieve Battery voltage value from system
 *
 *   @paramp pBatteryVoltage           measured battery voltage in 2.6 format
 *                                     - 2 bits V, 6 bits 1/64 volt (in 0.01 units).
 *   @return success                   True if valid temperature value is returned.
 */
Bool Application_GetBatteryVoltage(UQ2_6* pBatteryVoltage)
{
    Bool success;

    // Measure Battery Voltage
    success = gpBatteryMonitor_Measure(gpBatteryMonitor_MeasurementTypeUnloadedWithRecovery, pBatteryVoltage);

    return success;
}

/** @brief example function to caluclate and update the battery remaining percentage
 *
 *   @param   batteryVoltage             Current battery voltage in 2.6 format
 *   @return  remainingPercentage        The battery remaining percentage in a 0.5% unit
 */
UInt8 Application_UpdateBatteryRemaining(UQ2_6 voltage)
{
    /**
     *   Porting note:
     *   Add/Modify the the battery remaining percentage measurement
     *   according to your specific HW/Battery here.
     *   Currently provided is an example by converting battery voltage into remaining percentage.
     */
    UInt8 remainingPercentage;
    UInt16 batteryVoltage; // in 10mV per unit

    batteryVoltage = BATTERY_MONITOR_GET_INTEGER_PART(voltage) * 100 + BATTERY_MONITOR_GET_FLOATING_PART(voltage);

    if(batteryVoltage >= APP_BATTERY_FULL_THRESHOLD)
    {
        remainingPercentage = 100;
        GP_LOG_SYSTEM_PRINTF("  Battery: FULL", 0);
    }
    // Battery Section 1: Full ~ Medium
    else if(batteryVoltage >= APP_BATTERY_MEDIUM_THRESHOLD)
    {
        remainingPercentage = (100 - ((APP_BATTERY_FULL_THRESHOLD - batteryVoltage) * (100 - APP_BATTERY_MEDIUM)) /
                                         (APP_BATTERY_FULL_THRESHOLD - APP_BATTERY_MEDIUM_THRESHOLD));

        GP_LOG_SYSTEM_PRINTF("  Battery: above MEDIUM", 0);
    }
    // Battery Section 2: Medium ~ Low
    else if(batteryVoltage >= APP_BATTERY_LOW_THRESHOLD)
    {
        remainingPercentage = (APP_BATTERY_MEDIUM - ((APP_BATTERY_MEDIUM_THRESHOLD - batteryVoltage) *
                                                     (APP_BATTERY_MEDIUM - APP_BATTERY_LOW)) /
                                                        (APP_BATTERY_MEDIUM_THRESHOLD - APP_BATTERY_LOW_THRESHOLD));
        GP_LOG_SYSTEM_PRINTF("  Battery: above LOW", 0);
    }
    // Battery Section 3: Low ~ Critical
    else if(batteryVoltage >= APP_BATTERY_CRITICAL_THRESHOLD)
    {
        remainingPercentage = (APP_BATTERY_LOW - (APP_BATTERY_LOW_THRESHOLD - batteryVoltage) * APP_BATTERY_LOW /
                                                     (APP_BATTERY_LOW_THRESHOLD - APP_BATTERY_CRITICAL_THRESHOLD));
        GP_LOG_SYSTEM_PRINTF("  Battery: below LOW,", 0);
    }
    else
    {
        remainingPercentage = 0;
        GP_LOG_SYSTEM_PRINTF("  Battery: CRITICAL", 0);
    }

    GP_LOG_SYSTEM_PRINTF("  Battery: %u.%02uV, %d(%)", 0, batteryVoltage / 100, batteryVoltage % 100,
                         remainingPercentage);

    // Update Power configuration's BatteryPercentageRemaining attribute
    ZigbeeEndpointMgr().SetBatteryPercentageRemaining(remainingPercentage * 2); // requires 0.5% per unit

    return remainingPercentage * 2;
}

/** @brief Function to update Power Confiugration cluster following attributes
 *       0x0020 BatteryVoltage attribute
 *       0x0021 BatteryPercentageRemainin attribute
 */
void Application_UpdateBattery(void)
{
    UQ2_6 batteryVoltage;
    UInt8 voltage;
    Bool status = false;

    // get current battery voltage reading
    status = Application_GetBatteryVoltage(&batteryVoltage);

    if(status)
    {
        // Update Power configuration's BatteryPercentageRemaining attribute
        Application_UpdateBatteryRemaining(batteryVoltage);

        // convert the reading from 2.6 format to 100mV unit
        voltage = BATTERY_MONITOR_GET_INTEGER_PART(batteryVoltage) * 10 +
                  BATTERY_MONITOR_GET_FLOATING_PART(batteryVoltage) / 10;

        // round up the reading
        if(BATTERY_MONITOR_GET_FLOATING_PART(batteryVoltage) >= 5)
        {
            voltage++;
        }

        // Update Power configuration's BatteryVoltage attribute
        ZigbeeEndpointMgr().SetBatteryVoltage(voltage);
    }

    // indicate application that the Battery Voltage update is completed/failed.
    Application_cbBatteryVoltageUpdated(status);
}

/** @brief Function to start/restart periodic update battery voltage reading
 */
void Application_StartPeriodicBatteryUpdate(void)
{
    // Update Battery reading
    Application_UpdateBattery();

    // remove pervious schedular first
    if(gpSched_ExistsEvent(Application_StartPeriodicBatteryUpdate))
    {
        gpSched_UnscheduleEvent(Application_StartPeriodicBatteryUpdate);
    }

    // schedule the periodic battery voltage reading update
    gpSched_ScheduleEvent(APP_BATTERY_UPDATE_PERIOD, Application_StartPeriodicBatteryUpdate);
}

/** @brief Function to stop periodic update battery voltage reading
 */
void Application_StopPeriodicBatteryUpdate(void)
{
    // Update Battery reading
    Application_UpdateBattery();

    // remove periodic battery voltage reading update schedular
    if(gpSched_ExistsEvent(Application_StartPeriodicBatteryUpdate))
    {
        gpSched_UnscheduleEvent(Application_StartPeriodicBatteryUpdate);
    }
}

/*****************************************************************************
 *  Power Configuration
 *****************************************************************************/
/** @brief callback when device has updated its battery voltage and remaining percentage
 */
void Application_cbBatteryVoltageUpdated(UInt8 status)
{
#ifdef GP_DIVERSITY_LOG
    if(status)
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE PowerCfg: Battery %u.%u(V), %u.%u(%)", 0,
                             ZigbeeEndpointMgr().GetBatteryVoltage() / 10, ZigbeeEndpointMgr().GetBatteryVoltage() % 10,
                             ZigbeeEndpointMgr().GetBatteryPercentageRemaining() / 2,
                             (ZigbeeEndpointMgr().GetBatteryPercentageRemaining() % 2) * 5);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE PowerCfg: Error in reading battery voltage", 0);
    }
#endif // GP_DIVERSITY_LOG
}
