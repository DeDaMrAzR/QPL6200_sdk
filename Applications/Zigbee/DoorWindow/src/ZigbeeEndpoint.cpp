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
#include "DoorWindowManager.h"

#include "ZigbeeEndpoint.h"
#include "ContactSwitchApplication.h"

#include "gpLog.h"

/*****************************************************************************
 *                    Macro definitions
 *****************************************************************************/
// Simple state keeping mechanism broken if endpoints numbered > 8
GP_COMPILE_TIME_VERIFY(ZIGBEE_ENDPOINT_1 < 8);
#ifdef ZIGBEE_ENDPOINT_2
GP_COMPILE_TIME_VERIFY(ZIGBEE_ENDPOINT_2 < 8);
#endif // APPLICATION_ENDPOINT_2

#define ONE_SECOND (1000000UL)

/*****************************************************************************
 *                    Application data
 *****************************************************************************/
// Application endpoints
CContactSwitchApplication appsEp_1(aps, ZIGBEE_ENDPOINT_1);

ZigbeeEndpoint ZigbeeEndpoint::sZigbeeEndpoint(&appsEp_1);

/*****************************************************************************
 *                    Stack configuration
 *****************************************************************************/
__COMPACT7B_BEGIN_APPLICATION_MAP()
__COMPACT7B_APPLICATION(&zdo)
__COMPACT7B_APPLICATION(&appsEp_1)
#ifdef GP_COMP_ZGPSTUB
__COMPACT7B_APPLICATION(&zgp)
#endif // GP_COMP_ZGPSTUB
__COMPACT7B_END_APPLICATION_MAP()

/*****************************************************************************
 *                    Application functions
 *****************************************************************************/
void ZigbeeEndpoint::Init(void)
{
    /* Nothing to do, return */
    return;
}

void ZigbeeEndpoint::UpdateIasState(bool alarmed, bool tampered)
{
    uint16_t prevIasState = appsEp_1.GetIasStatus();
    uint16_t iasState = prevIasState;

    if(alarmed)
    {
        iasState |= CZigBeeClusterIASZone::statusAlarm1;
    }
    else
    {
        iasState &= ~CZigBeeClusterIASZone::statusAlarm1;
    }

    if(tampered)
    {
        iasState |= CZigBeeClusterIASZone::statusTampered;
    }
    else
    {
        iasState &= ~CZigBeeClusterIASZone::statusTampered;
    }

    // Door/Window alarm/Tamper alarm state change detected.
    if(prevIasState != iasState)
    {
        // Handle IAS Zone status
        appsEp_1.IasStateChange(iasState, ~iasState);
    }
}
