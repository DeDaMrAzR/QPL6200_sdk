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
#include "DoorWindowManager.h"
#include "AppBattery.h"

#include "ZigbeeTask.h"
#include "ZigbeeEndpoint.h"
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#include "combo-handler.h"
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#define ZIGBEE_COMMISSION_TIMEOUT 2

#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#define ZIGBEE_TL_COMMISSION_TIMEOUT                                4
#define ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT 6
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT

#define FACTORY_RESET_TIMEOUT 10

#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#define COMBO_STACK_SWITCH_TIMEOUT 12
#endif

AppManager AppManager::sAppMgr;

static const uint8_t StatusLedGpios[] = QPINCFG_STATUS_LED;

void AppManager::Init()
{
    StatusLed_Init(StatusLedGpios, Q_ARRAY_SIZE(StatusLedGpios), true);

#if defined(GP_APP_DIVERSITY_BUTTONHANDLER)
    GetAppButtons().RegisterMultiFunc(APP_MULTI_FUNC_BUTTON);
#endif // GP_APP_DIVERSITY_BUTTONHANDLER

    ZigbeeEndpointMgr().Init();

    DoorWindowMgr().Init();
    DoorWindowMgr().SetCallbacks(reinterpret_cast<DoorWindowManager::DoorWindowCallback_fn>(ActionInitiatedCallback),
                                 reinterpret_cast<DoorWindowManager::DoorWindowCallback_fn>(ActionCompletedCallback));
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
    else if(zigbeeEvent.State == ZigbeeTask::kZigbeeState_AssoPermitJoin)
    {
        StatusLed_BlinkLed(ZIGBEE_STATE_LED, 100, 100);
    }
    else if(zigbeeEvent.State == ZigbeeTask::kZigbeeState_Identifying)
    {
        StatusLed_BlinkLed(ZIGBEE_STATE_LED, 500, 500);
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
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#define ZIGBEE_TL_COMMISSION_COUNT                           6
#define ZIGBEE_TL_COMMISSION_WITH_RESET_TO_FACTORY_NEW_COUNT 7
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#if defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
#define ZIGBEE_TL_TARGET_START_COUNT 9
#endif // GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START
#define FACTORY_RESET_COUNT 10

    if(aEvent->ResetCountEvent.Count == FACTORY_RESET_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Factory Reset Triggered!", 0, aEvent->ResetCountEvent.Count);
        CountdownToFactoryReset();
    }
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
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
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START
    else if(aEvent->ResetCountEvent.Count == ZIGBEE_TL_TARGET_START_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee Touchlink Target Triggered!", 0, aEvent->ResetCountEvent.Count);
        Application_ZigBeeStartTouchLink(true);
    }
#endif // GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START
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
    DoorWindowAction_t action = DoorWindowAction_t::INVALID_ACTION; // init as invalid action
    bool actionValue = false;
    // GP_LOG_SYSTEM_PRINTF("ButtonEventHandler %d, %d, %d", 0, aEvent->ButtonEvent.Index,
    // aEvent->ButtonEvent.State, aEvent->ButtonEvent.HeldSec);

    switch(aEvent->ButtonEvent.Index)
    {
        case APP_MULTI_FUNC_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee commissioning selected!!!", 0);
                GP_LOG_SYSTEM_PRINTF("[BTN] Release within %d second to trigger Zigbee commissioning", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] Hold to select other functions:", 0);
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
                GP_LOG_SYSTEM_PRINTF("[BTN] - Zigbee TouchLink Comissioning start(%d-%ds)", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF(
                    "[BTN] - Zigbee TouchLink Comissioning with reset-to-factory-new request start/stop(%d-%ds)", 0,
                    ZIGBEE_TL_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT);
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT

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
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
                    GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee TouchLink commissioning selected!!!", 0);
#else
                    GP_LOG_SYSTEM_PRINTF("[BTN] Keep holding for %d more seconds to select factory reset", 0,
                                         FACTORY_RESET_TIMEOUT - aEvent->ButtonEvent.HeldSec);
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release to cancel", 0);
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
                }
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
                else if(aEvent->ButtonEvent.HeldSec == ZIGBEE_TL_COMMISSION_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF(
                        "[BTN] Zigbee TouchLink commissioning with reset-to-factory-new request selected!!!", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Keep holding for %d more seconds to select factory reset", 0,
                                         FACTORY_RESET_TIMEOUT - aEvent->ButtonEvent.HeldSec);
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release to cancel", 0);
                }
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
                else if(aEvent->ButtonEvent.HeldSec == FACTORY_RESET_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory reset selected!!!", 0);
#ifndef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                    /* in case of not combo build >>> tigger factory reset */
                    GP_LOG_SYSTEM_PRINTF("[BTN] Triggering Factory reset!!!", 0);
                    GetAppTask().FactoryReset();
#else
                    /* in case of combo build >>> release to tigger factory reset */
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release within %d second to trigger factory reset", 0,
                                         COMBO_STACK_SWITCH_TIMEOUT - FACTORY_RESET_TIMEOUT);
                    GP_LOG_SYSTEM_PRINTF("[BTN] Keep holding to select Combo Stack Switch", 0);
#endif
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
#endif
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
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
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
                    if(!Application_ZigBeeIsCommissioningInProgress())
                    {
                        GP_LOG_SYSTEM_PRINTF(
                            "[BTN] Starting Zigbee TouchLink commission with reset-to-factory-new request", 0);
                        Application_ZigBeeStartTouchLinkCommissioning(true, true);
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("Zigbee commissioning is in progress", 0);
                    }
                }
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
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

        case APP_CONTACT_SWITCH:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = DoorWindowAction_t::CONTACT_ACTION;
                actionValue = false;
            }
            else if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Released)
            {
                action = DoorWindowAction_t::CONTACT_ACTION;
                actionValue = true;
            }
            break;
        case APP_TAMPER_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = DoorWindowAction_t::TAMPER_ACTION;
                actionValue = true;
            }
            else if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Released)
            {
                action = DoorWindowAction_t::TAMPER_ACTION;
                actionValue = false;
            }
            break;
    }

    if(action != DoorWindowAction_t::INVALID_ACTION)
    {
        DoorWindowMgr().InitiateAction(action, kActor_App, actionValue);
    }
    else
    {
        /* Do Nothing */
    }
}

void AppManager::IdentifyEventHandler(AppEvent* aEvent)
{
    // nothing to do
}

void AppManager::ActionInitiatedCallback(uint8_t aAction, int32_t aActor)
{
    // Placeholder for door window sensor ActionInitiatedCallback
}

void AppManager::ActionCompletedCallback(uint8_t aAction, int32_t aActor)
{
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
        case DoorWindowAction_t::CONTACT_ACTION:
        case DoorWindowAction_t::TAMPER_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB update IAS state", 0);
            ZigbeeEndpointMgr().UpdateIasState(DoorWindowMgr().IsOpened(), DoorWindowMgr().IsTampered());
            break;

        default:
            GP_LOG_SYSTEM_PRINTF("ZB handle default", 0);
            break;
    }
}
