/*
 * Copyright (c) 2017-2019, Qorvo Inc
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

/** @file "ResponsiveModePolling.c"
 *
 * This ZigBee helper module can be triggered by the application
 * to temporarily incerease the poll-rate on a sleepy end-device.
 * This effectively causes the sleepy end-device to "Wakeup", ready to receive
 * managment commands, change reporting configuration, change bindings etc
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"
#include "ResponsiveModePolling.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    External data structures
 *****************************************************************************/
extern CZigBeeApplicationSupport aps;


/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static unsigned int m_nCookie;


/*****************************************************************************
 *                    Static functions
 *****************************************************************************/
void ResponsiveModeTimeout(void)
{
    aps.m_nwk.m_nlme.LeaveResponsiveMode(m_nCookie);
    m_nCookie = 0;
}


/*****************************************************************************
 *                    Public functions
 *****************************************************************************/
void StartResponsiveModeWithTimeout(UInt32 timeout_msec)
{
    if(!aps.m_nwk.m_nlme.IsEndDevice())
    {
        return;
    }

    if (!gpSched_UnscheduleEvent(ResponsiveModeTimeout))
    {
        m_nCookie = 0;
        aps.m_nwk.m_nlme.EnterResponsiveMode(m_nCookie);
    }
    gpSched_ScheduleEvent(timeout_msec * 1000, ResponsiveModeTimeout);
}

void StopResponsiveMode(void)
{
    if (gpSched_UnscheduleEvent(ResponsiveModeTimeout))
    {
        ResponsiveModeTimeout();
    }
}
