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

/** @file "AppManager.cpp"
 */

#include "gpLog.h"
#include "gpSched.h"
#include "qPinCfg.h"
#include "AppButtons.h"
#include "AppTask.h"
#include "AppManager.h"
#include "StatusLed.h"
#include "MotionSensorManager.h"
#include "AppBattery.h"

#include "ZigbeeTask.h"
#include "ZigbeeEndpoint.h"
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#include "combo-handler.h"
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

AppManager AppManager::sAppMgr;

static const uint8_t StatusLedGpios[] = QPINCFG_STATUS_LED;

void AppManager::Init()
{
    StatusLed_Init(StatusLedGpios, Q_ARRAY_SIZE(StatusLedGpios), true);

#if defined(GP_APP_DIVERSITY_BUTTONHANDLER)
    GetAppButtons().RegisterMultiFunc(APP_MULTI_FUNC_BUTTON);
#endif // GP_APP_DIVERSITY_BUTTONHANDLER

    ZigbeeEndpointMgr().Init();

    MotionSensorMgr().Init();
    MotionSensorMgr().SetCallbacks(
        reinterpret_cast<MotionSensorManager::MotionSensorCallback_fn>(ActionInitiatedCallback),
        reinterpret_cast<MotionSensorManager::MotionSensorCallback_fn>(ActionCompletedCallback));
}

void AppManager::UpdateLEDs(void)
{
    ZigbeeTask::Event_t zigbeeEvent = ZigbeeTaskMgr().GetEvent();

    // zigbee event led status
    if(zigbeeEvent.State == ZigbeeTask::kZigbeeState_JoinedNetwork)
    {
        StatusLed_SetLed(ZIGBEE_STATE_LED, false);
    }
    else if(zigbeeEvent.State == ZigbeeTask::kZigbeeState_Commissioning)
    {
        StatusLed_BlinkLed(ZIGBEE_STATE_LED, 50, 50);
    }
    else
    {
        StatusLed_SetLed(ZIGBEE_STATE_LED, false);
    }
}

void AppManager::EventHandler(AppEvent* aEvent)
{
    switch(aEvent->Type)
    {
        case AppEvent::kEventType_ResetCount:
            ResetCountEventHandler(aEvent);
            break;

        case AppEvent::kEventType_Buttons:
            ButtonEventHandler(aEvent);
            break;

        case AppEvent::kEventType_Identify:
            IdentifyEventHandler(aEvent);
            break;

        case AppEvent::kEventType_Invalid:
        default:
            GP_LOG_SYSTEM_PRINTF("Event Type %d discarded", 0, aEvent->Type);
            break;
    }
}

static void CountdownToFactoryReset(void)
{
    static uint8_t countdown = 5;

    if(countdown > 0)
    {
        StatusLed_SetLed(ZIGBEE_STATE_LED, (countdown % 2) ? true : false);
        countdown--;
        gpSched_ScheduleEvent(1000000UL, CountdownToFactoryReset);
    }
    else
    {
        GetAppTask().FactoryReset();
    }
}

void AppManager::ResetCountEventHandler(AppEvent* aEvent)
{
#define ZIGBEE_TL_COMMISSION_COUNT                           6
#define ZIGBEE_TL_COMMISSION_WITH_RESET_TO_FACTORY_NEW_COUNT 7
#define ZB_FINDING_AND_BINDING_COUNT 8
#define ZIGBEE_TL_TARGET_START_COUNT 9
#define FACTORY_RESET_COUNT 10

    if(aEvent->ResetCountEvent.Count == FACTORY_RESET_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Factory Reset Triggered!", 0, aEvent->ResetCountEvent.Count);
        CountdownToFactoryReset();
    }
    else if(aEvent->ResetCountEvent.Count == ZIGBEE_TL_COMMISSION_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee Touchlink commissioning Triggered!", 0, aEvent->ResetCountEvent.Count);
        Application_ZigBeeStartTouchLinkCommissioning();
    }
    else if(aEvent->ResetCountEvent.Count == ZIGBEE_TL_COMMISSION_WITH_RESET_TO_FACTORY_NEW_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee Touchlink commissioning with reset-to-factory-new request Triggered!",
                             0, aEvent->ResetCountEvent.Count);
        Application_ZigBeeStartTouchLinkCommissioning(true, true);
    }
    else if(aEvent->ResetCountEvent.Count == ZB_FINDING_AND_BINDING_COUNT)
    {
        if(Application_ZigBeeIsJoinedNetwork())
        {
            GP_LOG_SYSTEM_PRINTF("[RST] Starting Zigbee Finding and Bindling initiator", 0);
            // start Finding and Binding(initiator)
            Application_StartFindingAndBinding_Initiator(ZIGBEE_ENDPOINT_1);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("[RST] Can't start Zigbee F&B - not in a network", 0);
        }
    }
    else if(aEvent->ResetCountEvent.Count == ZIGBEE_TL_TARGET_START_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee Touchlink Target Triggered!", 0, aEvent->ResetCountEvent.Count);
        Application_ZigBeeStartTouchLink(true);
    }
    else
    {
        if(!Application_ZigBeeIsJoinedNetwork())
        {
            GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee commissioning Triggered!", 0, aEvent->ResetCountEvent.Count);
            ZigbeeTaskMgr().OpenCommissioning();
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("[RST]: %d", 0, aEvent->ResetCountEvent.Count);
        }
    }
}

void AppManager::ButtonEventHandler(AppEvent* aEvent)
{
    // GP_LOG_SYSTEM_PRINTF("ButtonEventHandler %d, %d, %d", 0, aEvent->ButtonEvent.Index,
    // aEvent->ButtonEvent.State, aEvent->ButtonEvent.HeldSec);

    switch(aEvent->ButtonEvent.Index)
    {
        case APP_MULTI_FUNC_BUTTON:
#define ZIGBEE_COMMISSION_TIMEOUT                                   2
#define ZIGBEE_TL_COMMISSION_TIMEOUT                                4
#define ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT 6
#define ZB_FINDING_AND_BINDING_TIMEOUT 8
#define FACTORY_RESET_TIMEOUT 10
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#define COMBO_STACK_SWITCH_TIMEOUT 12
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee commissioning selected!!!", 0);
                GP_LOG_SYSTEM_PRINTF("[BTN] Release within %d seconds to trigger Zigbee commissioning", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] Hold to select other functions:", 0);
                GP_LOG_SYSTEM_PRINTF("[BTN] - Zigbee TouchLink Comissioning start(%d-%ds)", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF(
                    "[BTN] - Zigbee TouchLink Comissioning with reset-to-factory-new request start/stop(%d-%ds)", 0,
                    ZIGBEE_TL_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] - Zigbee Finding and Binding (%d-%ds)", 0,
                                     ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT,
                                     ZB_FINDING_AND_BINDING_TIMEOUT);
#ifndef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                GP_LOG_SYSTEM_PRINTF("[BTN] - Factory Reset (>%ds)", 0, FACTORY_RESET_TIMEOUT);
#else
                GP_LOG_SYSTEM_PRINTF("[BTN] - Factory Reset (%d-%ds)", 0, FACTORY_RESET_TIMEOUT,
                                     COMBO_STACK_SWITCH_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] - Combo Stack Switch (>%ds)", 0, COMBO_STACK_SWITCH_TIMEOUT);
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
            }
            else if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Held)
            {
                if(aEvent->ButtonEvent.HeldSec == ZIGBEE_COMMISSION_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee TouchLink commissioning selected!!!", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == ZIGBEE_TL_COMMISSION_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF(
                        "[BTN] Zigbee TouchLink commissioning with reset-to-factory-new request selected!!!", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee Finding and binding selected!!!", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == ZB_FINDING_AND_BINDING_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory reset selected!!! Release in 2 seconds to cancel", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == FACTORY_RESET_TIMEOUT)
                {
#ifndef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                    /* in case of not combo build >>> tigger factory reset */
                    GP_LOG_SYSTEM_PRINTF("[BTN] Triggering Factory reset!!!", 0);
                    GetAppTask().FactoryReset();
#else
                    /* in case of combo build >>> release to tigger factory reset */
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release within %d second to trigger factory reset", 0,
                                         COMBO_STACK_SWITCH_TIMEOUT - FACTORY_RESET_TIMEOUT);
                    GP_LOG_SYSTEM_PRINTF("[BTN] Keep holding to select Combo Stack Switch", 0);
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
                }
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                else if(aEvent->ButtonEvent.HeldSec == COMBO_STACK_SWITCH_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Combo Stack Switch selected!!!", 0);
                    GP_LOG_SYSTEM_PRINTF("[BTN] Triggering Combo Stack Switch!!!", 0);

                    if(ComboHandler_SwitchStack() == gpUpgrade_StatusSuccess)
                    {
                        /* Trigger Factory reset only in case of stack switching succeeds */
                        GetAppTask().FactoryReset();
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("!!! Combo Stack Switch Failed", 0);
                    }
                }
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
            }
            else if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Released)
            {
                if(aEvent->ButtonEvent.HeldSec < ZIGBEE_COMMISSION_TIMEOUT)
                {
                    if(!Application_ZigBeeIsJoinedNetwork())
                    {
                        GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee commission", 0);
                        ZigbeeTaskMgr().OpenCommissioning();
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("Zigbee commission has been established", 0);
                    }
                }
                else if(aEvent->ButtonEvent.HeldSec < ZIGBEE_TL_COMMISSION_TIMEOUT)
                {
                    if(!Application_ZigBeeIsCommissioningInProgress())
                    {
                        GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee TouchLink commission", 0);
                        Application_ZigBeeStartTouchLinkCommissioning();
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("Zigbee commissioning is in progress", 0);
                    }
                }
                else if(aEvent->ButtonEvent.HeldSec < ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee TouchLink commission with reset-to-factory-new request",
                                         0);
                    Application_ZigBeeStartTouchLinkCommissioning(true, true);
                }
                else if(aEvent->ButtonEvent.HeldSec < ZB_FINDING_AND_BINDING_TIMEOUT)
                {
                    if(Application_ZigBeeIsJoinedNetwork())
                    {
                        GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee Finding and Bindling initiator", 0);
                        // start Finding and Binding(initiator)
                        Application_StartFindingAndBinding_Initiator(ZIGBEE_ENDPOINT_1);
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("[BTN] Can't start Zigbee F&B - not in a network", 0);
                    }
                }
                else if(aEvent->ButtonEvent.HeldSec < FACTORY_RESET_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory Reset has been canceled", 0);
                }
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                /* Only in case of combo build */
                else if(aEvent->ButtonEvent.HeldSec < COMBO_STACK_SWITCH_TIMEOUT)
                {
                    GetAppTask().FactoryReset();
                }
#endif
            }
            break;

        case APP_CONTROL_MODE_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                if(Application_ZigBeeIsJoinedNetwork())
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Control mode button pressed", 0);
                    ZigbeeTaskMgr().ToggleControlMode();
                }
            }
            break;

        case APP_OCCUPANCY_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                MotionSensorMgr().InitiateAction(MotionSensorAction_t::OCCUPIED_ACTION, kActor_App);
            }
            break;

        case APP_ILLUMINANCE_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                // Emulating illuminace changes, toggle min value(0x0001) and max value(0xfffe)
                uint16_t value = MotionSensorMgr().GetIlluminance() < 0xfffe ? 0xfffe : 0x0001;
                GP_LOG_SYSTEM_PRINTF("[BTN] Illuminance button pressed - illuminance value 0x%04x", 0, value);
                MotionSensorMgr().SetIlluminance(value);
            }
            break;
    }
}

void AppManager::IdentifyEventHandler(AppEvent* aEvent)
{
    static bool identifying = false;

    if(aEvent->IdentifyEvent.isIdentifying)
    {
        if(!identifying)
        {
            StatusLed_BlinkLed(ZIGBEE_STATE_LED, 500, 500);
            identifying = true;
        }
    }
    else
    {
        UpdateLEDs();
        identifying = false;
    }
}

static void UnoccupiedDelayExpired(void)
{
    MotionSensorMgr().InitiateAction(MotionSensorAction_t::UNOCCUPIED_ACTION, kActor_App);
}

void AppManager::ActionInitiatedCallback(uint8_t aAction, int32_t aActor)
{
    // Placeholder for MotionSensor sensor ActionInitiatedCallback
}

void AppManager::ActionCompletedCallback(uint8_t aAction, int32_t aActor)
{
    if(aAction == MotionSensorAction_t::OCCUPIED_ACTION)
    {
        // Updates the delay for changing to unoccupied state
        gpSched_UnscheduleEvent(UnoccupiedDelayExpired);
        gpSched_ScheduleEvent(MotionSensorMgr().GetOccupiedToUnoccupiedDelay() * 1000000, UnoccupiedDelayExpired);
    }

    if(aActor == kActor_App)
    {
        GetAppMgr().UpdateZigbeeClusterState(aAction);
    }
    else
    {
        /* Do Nothing */
    }
}

void AppManager::UpdateZigbeeClusterState(uint8_t aAction)
{
    GP_LOG_SYSTEM_PRINTF("UpdateZigbeeClusterState - action %d", 0, aAction);

    switch(aAction)
    {
        case MotionSensorAction_t::OCCUPIED_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB update Occupancy state - Occupied", 0);
            ZigbeeEndpointMgr().SetOccupancy(true);
            break;

        case MotionSensorAction_t::UNOCCUPIED_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB update Occupancy state - Unoccupied", 0);
            ZigbeeEndpointMgr().SetOccupancy(false);
            break;

        default:
            GP_LOG_SYSTEM_PRINTF("ZB handle default", 0);
            break;
    }
}
