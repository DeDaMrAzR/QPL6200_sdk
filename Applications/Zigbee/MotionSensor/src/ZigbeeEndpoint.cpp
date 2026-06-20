/*
 * Copyright (c) 2024-2025, Qorvo Inc
 *
 *
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

/** @file "ZigbeeEndpoint.cpp"
 */

#include "AppTask.h"
#include "AppManager.h"
#include "MotionSensorManager.h"

#include "ZigbeeEndpoint.h"
#include "OnOffSensorApplication.h"
#include "OccupancySensorApplication.h"
#include "LightSensorApplication.h"
#include "App.h"

#include "gpLog.h"
#include "gpSched.h"
#ifdef GP_APP_DIVERSITY_RANDOM_GROUP_ID
#include "gpRandom.h"
#endif

/*****************************************************************************
 *                    Macro definitions
 *****************************************************************************/
// Simple state keeping mechanism broken if endpoints numbered > 8
GP_COMPILE_TIME_VERIFY(ZIGBEE_ENDPOINT_1 < 8);
#ifdef ZIGBEE_ENDPOINT_2
GP_COMPILE_TIME_VERIFY(ZIGBEE_ENDPOINT_2 < 8);
#endif
#ifdef ZIGBEE_ENDPOINT_3
GP_COMPILE_TIME_VERIFY(ZIGBEE_ENDPOINT_3 < 8);
#endif

#define ONE_SECOND (1000000UL)

#define APP_ILLUMINANCE_UPDATE_PERIOD (30 * ONE_SECOND)

/*****************************************************************************
 *                    Application data
 *****************************************************************************/
// Application endpoints
COnOffSensorApplication appsEp_1(aps, ZIGBEE_ENDPOINT_1);
COccupancySensorApplication appsEp_2(aps, ZIGBEE_ENDPOINT_2);
CLightSensorApplication appsEp_3(aps, ZIGBEE_ENDPOINT_3);

ZigbeeEndpoint ZigbeeEndpoint::sZigbeeEndpoint(&appsEp_1);

/*****************************************************************************
 *                    Stack configuration
 *****************************************************************************/
__COMPACT7B_BEGIN_APPLICATION_MAP()
__COMPACT7B_APPLICATION(&zdo)
__COMPACT7B_APPLICATION(&appsEp_1)
__COMPACT7B_APPLICATION(&appsEp_2)
__COMPACT7B_APPLICATION(&appsEp_3)
#ifdef GP_COMP_ZGPSTUB
__COMPACT7B_APPLICATION(&zgp)
#endif // GP_COMP_ZGPSTUB
__COMPACT7B_END_APPLICATION_MAP()

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static void Application_UpdateIlluminanceMeasuredValue(void)
{
    uint16_t measuredValue = MotionSensorMgr().GetIlluminance();
    ZigbeeEndpointMgr().SetIlluminance(measuredValue);

    gpSched_ScheduleEvent(APP_ILLUMINANCE_UPDATE_PERIOD, Application_UpdateIlluminanceMeasuredValue);
}

#ifdef GP_APP_DIVERSITY_RANDOM_GROUP_ID
static unsigned short Application_GetBoundGroupId(void)
{
    unsigned short groupId = 0xFFFF;
    CZigBeeBindingTargets targets;

    aps.m_apsme.LookupBinding(ZIGBEE_ENDPOINT_1, CZigBeeClusterOnOff::id, targets);

    for(std::list<CZigBeeBindingTarget>::const_iterator i = targets.begin(); i != targets.end(); i++)
    {
        if(i->m_bType == CZigBeeAddress::typeGroup)
        {
            groupId = i->m_wGroupAddress;
        }
    }

    if(groupId == 0xFFFF) // Not found
    {
        // Generate a random group Id and add it to bind table
        do
        {
            gpRandom_GetNewSequence(sizeof(unsigned short), reinterpret_cast<uint8_t*>(&groupId));
        } while(groupId < 1 || groupId > 0xFFF7);

        CZigBeeAddress destination = CZigBeeAddress(groupId, CZigBeeAddress::typeGroup);
        aps.m_apsme.Bind(ZIGBEE_ENDPOINT_1, CZigBeeClusterOnOff::id, destination);
    }

    return groupId;
}
#endif // GP_APP_DIVERSITY_RANDOM_GROUP_ID

/*****************************************************************************
 *                    Application functions
 *****************************************************************************/
void ZigbeeEndpoint::Init(void)
{
    // Start periodic illuminace measuring
    gpSched_ScheduleEvent(ONE_SECOND, Application_UpdateIlluminanceMeasuredValue);
}

void ZigbeeEndpoint::SetOccupancy(bool occupied)
{
    bool prevOccupancy = appsEp_2.m_occupancySensing.m_bOccupancy;

    if(occupied && Application_ZigBeeIsJoinedNetwork())
    {
        // Send OnWithTimedOff command
        CZigBeeAddress destAddr = CZigBeeAddress();

#ifndef GP_APP_DIVERSITY_RANDOM_GROUP_ID
        if(ZigbeeTaskMgr().GetControlMode() == ZigbeeTask::ControlMode_Group)
        {
            destAddr.m_wShort = (unsigned short)ZIGBEE_GROUP_ID;
            destAddr.SetType(CZigBeeAddress::typeGroup);
        }
#endif

        appsEp_1.SendOnWithTimedOffCommand(destAddr, MotionSensorMgr().GetOccupiedToUnoccupiedDelay() * 10);

        if(!prevOccupancy)
        {
            // Update illuminance measured value
            uint16_t measuredValue = MotionSensorMgr().GetIlluminance();
            SetIlluminance(measuredValue);

            // Report the attribute
            appsEp_3.ReportIlluminanceMeasuredValue();
        }
    }

    if(prevOccupancy != occupied)
    {
        appsEp_2.SetOccupancyStatus(occupied);
    }
}

void ZigbeeEndpoint::SetIlluminance(uint16_t measuredValue)
{
    appsEp_3.SetIlluminanceMeasuredValue(measuredValue);
}

void ZigbeeEndpoint::SendAddGroup(CZigBeeAddress destination, unsigned short groupId)
{
#if defined(GP_APP_DIVERSITY_RANDOM_GROUP_ID)
    // replace groupId with the one from binding table
    groupId = Application_GetBoundGroupId();
#endif
    appsEp_1.SendGroupAddCommand(destination, groupId);
}

/*****************************************************************************
 *                    ZigBee Application API
 *****************************************************************************/
/** @brief function to send add group to target
 *
 *   @param destination              Class CZigBeeAddress for the destination
 *   @param groupId                  Group ID to be added
 */
void Application_SendAddGroup(CZigBeeAddress destination, unsigned short groupId)
{
    ZigbeeEndpointMgr().SendAddGroup(destination, groupId);
}

/** @brief Get the measured illuminance from the system.
 *
 *   @param pIlluminance             Measured illuminance value
 *   @return success                 True if valid temperature value is returned.
 */
Bool Application_GetIlluminance(UInt16* pIlluminance)
{
    *pIlluminance = MotionSensorMgr().GetIlluminance();

    return true;
}

/** @brief callback for indicating OccupiedToUnOccupiedDelay attribute change.
 *
 *   @param delay                    OccupiedToUnoccupiedDelay value
 */
void Application_cbUpdateOccupiedToUnoccupiedDelay(UInt16 delay)
{
    MotionSensorMgr().SetOccupiedToUnoccupiedDelay(delay);
}
