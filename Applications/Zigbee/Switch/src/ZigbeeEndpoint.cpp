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
#include "SwitchManager.h"

#include "ZigbeeEndpoint.h"
#include "ColorSwitchApplication.h"
#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
#include "ZigBeeFindingAndBinding_Callback.h"
#endif // GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR

#include "ZigBeeTouchLink_Callback.h"

#include "gpLog.h"
#include "gpSched.h"

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
CColorSwitchApplication appsEp_1(aps, ZIGBEE_ENDPOINT_1);

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
 *                    static functions
 *****************************************************************************/
static void CheckControlMode(CZigBeeAddress* pDestAddr)
{
    if(ZigbeeTaskMgr().GetControlMode() == ZigbeeTask::ControlMode_Group)
    {
        pDestAddr->m_wShort = (unsigned short)ZIGBEE_GROUP_ID;
        pDestAddr->SetType(CZigBeeAddress::typeGroup);
    }
    GP_LOG_PRINTF("CZigBeeAddress destAddr - 0x%lx", 0, *pDestAddr);
}

static void CheckParentLink()
{
    if(aps.m_apsme.IsStartupRequired() && aps.m_apsme.HasStartedOnce())
    {
        GP_LOG_PRINTF("Parent-link detection has failed, triggering rejoin", 0);
        // reset the backoff period
        aps.m_apsme.m_startupBackoffTicks = 0;
        // reset stack's rejoin mode
        aps.m_apsme.Startup(aps.m_apsme.m_rejoinStartupMode);
    }
}

/*****************************************************************************
 *                    Application functions
 *****************************************************************************/
void ZigbeeEndpoint::Init(void)
{
    /* Nothing to do, return */
    return;
}

void ZigbeeEndpoint::SetLevel(uint8_t level)
{
    CZigBeeAddress destAddr = CZigBeeAddress();
    uint8_t sLevel;

    GP_LOG_PRINTF("ZigbeeEndpoint::SetLevel - %d", 0, level);
    sLevel = (level == SwitchManager::MIN_LEVEL) ? SwitchManager::MAX_LEVEL : SwitchManager::MIN_LEVEL;
    CheckControlMode(&destAddr);

    CheckParentLink();

    appsEp_1.SendLevelingMoveToLevelCommand(destAddr, level);
}

void ZigbeeEndpoint::OnOffToggle(void)
{
    CZigBeeAddress destAddr = CZigBeeAddress();

    GP_LOG_PRINTF("ZigbeeEndpoint::OnOffToggle", 0);

    CheckControlMode(&destAddr);

    CheckParentLink();

    appsEp_1.SendOnOffToggleCommand(destAddr);
}

void ZigbeeEndpoint::StartLeveling(unsigned char mode, unsigned char duration)
{
    CZigBeeAddress destAddr = CZigBeeAddress();

    GP_LOG_PRINTF("ZigbeeEndpoint::StartLeveling with mode - %d for %ds", 0, mode, duration);

    CheckControlMode(&destAddr);

    CheckParentLink();

    // Send leveling control stop command
    appsEp_1.SendLevelingMoveCommand(destAddr, mode);

    if(duration > 0)
    {
        gpSched_ScheduleEventArg(duration * ONE_SECOND, ZigbeeEndpoint::StopLevelingWrapper, this);
    }
}

void ZigbeeEndpoint::StopLevelingWrapper(void* obj)
{
    ((ZigbeeEndpoint*)obj)->StopLeveling();
}

void ZigbeeEndpoint::StopLeveling(void)
{
    CZigBeeAddress destAddr = CZigBeeAddress();

    GP_LOG_PRINTF("ZigbeeEndpoint::StopLeveling", 0);

    CheckControlMode(&destAddr);

    CheckParentLink();

    // Send leveling control stop command
    appsEp_1.SendLevelingStopCommand(destAddr);
}

void ZigbeeEndpoint::StartColorControlMoveHue(unsigned char mode, unsigned char duration)
{
    CZigBeeAddress destAddr = CZigBeeAddress();

    GP_LOG_PRINTF("ZigbeeEndpoint::StartColorControlMoveHue with mode - %d for %ds", 0, mode, duration);

    CheckControlMode(&destAddr);

    CheckParentLink();

    // Send leveling control stop command
    appsEp_1.SendColorControlMoveHueCommand(destAddr, mode);

    if(duration > 0)
    {
        /* Don't schedule the event if already scheduled to avoid overflowing the gpsched queue */
        if(!gpSched_ExistsEvent(reinterpret_cast<void_func>(ZigbeeEndpoint::StopColorControlMoveHueWrapper)))
        {
            gpSched_ScheduleEventArg(duration * ONE_SECOND, ZigbeeEndpoint::StopColorControlMoveHueWrapper, this);
        }
    }
}

void ZigbeeEndpoint::StopColorControlMoveHueWrapper(void* obj)
{
    ((ZigbeeEndpoint*)obj)->StopColorControlMoveHue();
}

void ZigbeeEndpoint::StopColorControlMoveHue(void)
{
    CZigBeeAddress destAddr = CZigBeeAddress();

    GP_LOG_PRINTF("ZigbeeEndpoint::StopColorControlMoveHue", 0);

    CheckControlMode(&destAddr);

    CheckParentLink();

    // Send leveling control stop command
    appsEp_1.SendColorControlMoveHueCommand(destAddr, CQorvoClusterColorControl::moveStop);
}

void ZigbeeEndpoint::SendAddGroup(CZigBeeAddress destination, unsigned short groupId)
{
    appsEp_1.SendGroupAddCommand(destination, groupId);
}

void ZigbeeEndpoint::StartTouchLinkClientScanning(void)
{
    appsEp_1.m_touchLinkClient.StartScanning();
}

void ZigbeeEndpoint::StopTouchLinkClientScanning(void)
{
    appsEp_1.m_touchLinkClient.StopScanning();
}

bool ZigbeeEndpoint::IsTouchLinkClientScanning(void)
{
    return appsEp_1.m_touchLinkClient.IsSessionActive();
}

/*****************************************************************************
 *  ZigBee Application API
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

/** @brief function get current control mode in application
 */
App_ControlMode_t Application_GetControlMode(void)
{
    return ZigbeeTaskMgr().GetControlMode();
}

/** @brief function set current control mode in application
 *
 *   @param mode              Control mode to set
 */
void Application_SetControlMode(App_ControlMode_t mode)
{
    ZigbeeTaskMgr().SetControlMode((ZigbeeTask::FB_ControlMode_t)mode);
}

/** @brief function set toggle control mode in application
 */
void Application_ToggleControlMode(void)
{
    ZigbeeTaskMgr().ToggleControlMode();
}
