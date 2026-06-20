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
#include "SwitchManager.h"
#include "AppBattery.h"

#include "ZigbeeTask.h"
#include "ZigbeeEndpoint.h"
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#include "combo-handler.h"
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#define APP_MULTI_FUNC_BUTTON PB5_BUTTON_GPIO_PIN
#define APP_LEVEL_BUTTON      PB1_BUTTON_GPIO_PIN
#define APP_COLOR_BUTTON      PB3_BUTTON_GPIO_PIN
#define APP_TOGGLE_BUTTON     PB4_BUTTON_GPIO_PIN
#define APP_FUNCTION2_SWITCH  PB2_BUTTON_GPIO_PIN // Switch press or release

#define ZIGBEE_STATE_LED GREEN_LED_GPIO_PIN       // white warm
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#define COMBO_MATTER_SELECT_COUNT 4
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
#define BUTTON_HOLD_TRIGGER (500000)
#define BUTTON_HOLD_TIME    (200000)

AppManager AppManager::sAppMgr;
static uint8_t countdown = 0;

static const uint8_t StatusLedGpios[] = QPINCFG_STATUS_LED;

void AppManager::Init()
{
    StatusLed_Init(StatusLedGpios, Q_ARRAY_SIZE(StatusLedGpios), true);

#if defined(GP_APP_DIVERSITY_BUTTONHANDLER)
    GetAppButtons().RegisterMultiFunc(APP_MULTI_FUNC_BUTTON);
#endif // GP_APP_DIVERSITY_BUTTONHANDLER

    ZigbeeEndpointMgr().Init();

    // Matter Switch
    SwitchMgr().Init();
    SwitchMgr().SetCallbacks(reinterpret_cast<SwitchManager::SwitchCallback_fn>(ActionInitiatedCallback),
                             reinterpret_cast<SwitchManager::SwitchCallback_fn>(ActionCompletedCallback));
}

void AppManager::UpdateLEDs(void)
{
    // If system has "full connectivity", keep the LED On constantly.
    //
    // If thread and service provisioned, but not attached to the thread network
    // yet OR no connectivity to the service OR subscriptions are not fully
    // established THEN blink the LED Off for a short period of time.
    //
    // If the system has ble connection(s) uptill the stage above, THEN blink
    // the LEDs at an even rate of 100ms.
    //
    // // Otherwise, blink the LED ON for a very short time.

    ZigbeeTask::Event_t zigbeeEvent = ZigbeeTaskMgr().GetEvent();

    GP_LOG_SYSTEM_PRINTF("Update LED => %d", 0, zigbeeEvent.State);
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

    if(countdown-- > 0)
    {
        SwitchMgr().InitiateAction(SwitchAction_t::TOGGLE_ACTION, kActor_App);
        gpSched_ScheduleEvent(1000000UL, CountdownToFactoryReset);
    }
    else
    {
        GetAppTask().FactoryReset();
    }
}

void AppManager::ResetCountEventHandler(AppEvent* aEvent)
{
#define ZIGBEE_COMMISSION_COUNT                              5
#define ZIGBEE_TL_COMMISSION_COUNT                           6
#define ZIGBEE_TL_COMMISSION_WITH_RESET_TO_FACTORY_NEW_COUNT 7
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
#define ZB_FINDING_AND_BINDING_COUNT 8
#endif // #if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) ||
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
#if defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
#define ZIGBEE_TL_TARGET_START_COUNT 9
#endif // GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START
#define FACTORY_RESET_COUNT 10

    if(aEvent->ResetCountEvent.Count == ZIGBEE_COMMISSION_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee commissioning Triggered!", 0, aEvent->ResetCountEvent.Count);
        ZigbeeTaskMgr().OpenCommissioning();
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
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
    else if(aEvent->ResetCountEvent.Count == ZB_FINDING_AND_BINDING_COUNT)
    {
        // Clear Binding/Grouping mode
        ZigbeeTaskMgr().LeaveBindingGroupingMode();
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR)
        GP_LOG_SYSTEM_PRINTF("[RST] Starting Zigbee Finding and Bindling initiator", 0);
        // start Finding and Binding(initiator)
        Application_StartFindingAndBinding_Initiator(ZIGBEE_ENDPOINT_1);
#elif defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
        GP_LOG_SYSTEM_PRINTF("[RST] Starting Zigbee Finding and Bindling target", 0);
        // Toggle the Finding and Binding(target)
        Application_ToggleFindingAndBinding_Target(ZIGBEE_ENDPOINT_1);
#endif // GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
    }
#endif // #if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) ||
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
#if defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
    else if(aEvent->ResetCountEvent.Count == ZIGBEE_TL_TARGET_START_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Zigbee Touchlink Target Triggered!", 0, aEvent->ResetCountEvent.Count);
        Application_ZigBeeStartTouchLink(true);
    }
#endif // GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START
    else if(aEvent->ResetCountEvent.Count == FACTORY_RESET_COUNT)
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d, Factory Reset Triggered!", 0, aEvent->ResetCountEvent.Count);
        CountdownToFactoryReset();
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("[RST]: %d", 0, aEvent->ResetCountEvent.Count);
    }
}

void AppManager::ButtonEventHandler(AppEvent* aEvent)
{
    SwitchAction_t action = SwitchAction_t::INVALID_ACTION; // init as invalid action

    switch(aEvent->ButtonEvent.Index)
    {
        case APP_MULTI_FUNC_BUTTON:
#define ZIGBEE_COMMISSION_TIMEOUT                                   2
#define ZIGBEE_TL_COMMISSION_TIMEOUT                                4
#define ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT 6
#define ZB_FINDING_AND_BINDING_TIMEOUT 8
#define FACTORY_RESET_TIMEOUT     10
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#define COMBO_ZIGBEE_STACK_TIMEOUT 12
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee commissioning or binding selected!!! (<%ds)", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] Hold to select other functions:", 0);
                GP_LOG_SYSTEM_PRINTF("[BTN] - Zigbee TouchLink Comissioning start/stop(%d-%ds)", 0,
                                     ZIGBEE_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF(
                    "[BTN] - Zigbee TouchLink Comissioning with reset-to-factory-new request start/stop(%d-%ds)", 0,
                    ZIGBEE_TL_COMMISSION_TIMEOUT, ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT);
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                GP_LOG_SYSTEM_PRINTF("[BTN] - Zigbee Finding and binding (%d-%ds)", 0,
                                     ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT,
                                     ZB_FINDING_AND_BINDING_TIMEOUT);
#endif // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) ||
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                GP_LOG_SYSTEM_PRINTF("[BTN] - Factory Reset (%d-%ds)", 0, FACTORY_RESET_TIMEOUT,
                                     COMBO_ZIGBEE_STACK_TIMEOUT);
                GP_LOG_SYSTEM_PRINTF("[BTN] - Stack switching & factory reset (>%ds)", 0, COMBO_ZIGBEE_STACK_TIMEOUT);
#else
                GP_LOG_SYSTEM_PRINTF("[BTN] - Factory Reset (>%ds)", 0, FACTORY_RESET_TIMEOUT);
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
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                else if(aEvent->ButtonEvent.HeldSec == ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Zigbee Finding and binding selected!!!", 0);
                }
#endif // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) ||
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                else if(aEvent->ButtonEvent.HeldSec == ZB_FINDING_AND_BINDING_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory reset selected!!! Release in 2 seconds to cancel", 0);
                }
                else if(aEvent->ButtonEvent.HeldSec == FACTORY_RESET_TIMEOUT)
                {
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release to start factory reset or Continue holding to switch stack", 0);
#else
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release to start factory reset", 0);
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
                }
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                else if(aEvent->ButtonEvent.HeldSec == COMBO_ZIGBEE_STACK_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Release to start stack switching...", 0);
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
                    GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee TouchLink commission", 0);
                    Application_ZigBeeStartTouchLinkCommissioning();
                }
                else if(aEvent->ButtonEvent.HeldSec < ZIGBEE_TL_COMMISSION_WITH_WITH_RESET_TO_FACTORY_NEW_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee TouchLink commission with reset-to-factory-new request",
                                         0);
                    Application_ZigBeeStartTouchLinkCommissioning(true, true);
                }
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                else if(aEvent->ButtonEvent.HeldSec < ZB_FINDING_AND_BINDING_TIMEOUT)
                {
                    // Clear Binding/Grouping mode
                    ZigbeeTaskMgr().LeaveBindingGroupingMode();
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR)
                    GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee Finding and Bindling initiator", 0);
                    // start Finding and Binding(initiator)
                    Application_StartFindingAndBinding_Initiator(ZIGBEE_ENDPOINT_1);
#elif defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                    GP_LOG_SYSTEM_PRINTF("[BTN] Starting Zigbee Finding and Bindling target", 0);
                    // Toggle the Finding and Binding(target)
                    Application_ToggleFindingAndBinding_Target(ZIGBEE_ENDPOINT_1);
#endif // GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
                }
#endif // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR) &&
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET)
                else if(aEvent->ButtonEvent.HeldSec < FACTORY_RESET_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory Reset has been canceled", 0);
                }
#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
                else if(aEvent->ButtonEvent.HeldSec >= COMBO_ZIGBEE_STACK_TIMEOUT)
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Switching stack...", 0);
                    gpUpgrade_Status_t status = ComboHandler_SwitchStack();
                    if(status == gpUpgrade_StatusSuccess)
                    {
                        GetAppTask().FactoryReset();
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("[BTN] failed with status: %d", 0, status);
                    }
                }
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
                else
                {
                    GP_LOG_SYSTEM_PRINTF("[BTN] Factory Resetting", 0);
                    GetAppTask().FactoryReset();
                }
            }
            break;

        case APP_LEVEL_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = SwitchAction_t::LEVEL_ACTION;
            }
            break;
        case APP_COLOR_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = SwitchAction_t::COLOR_ACTION;
            }
            break;
        case APP_TOGGLE_BUTTON:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = SwitchAction_t::TOGGLE_ACTION;
            }
            break;
        case APP_FUNCTION2_SWITCH:
            if(aEvent->ButtonEvent.State == ButtonEvent_t::kButtonState_Pressed)
            {
                action = SwitchMgr().IsSwitchPressed() ? SwitchAction_t::RELEASE_ACTION : SwitchAction_t::PRESS_ACTION;
            }
            break;
    }

    if(action != SwitchAction_t::INVALID_ACTION)
    {
        SwitchMgr().InitiateAction(action, kActor_App);
    }
    else
    {
        /* Do Nothing */
    }
}

void AppManager::IdentifyEventHandler(AppEvent* aEvent) {}

void AppManager::ActionInitiatedCallback(uint8_t aAction, int32_t aActor)
{
    // Placeholder for switch action
    switch(aAction)
    {
        case SwitchAction_t::LEVEL_ACTION:
            GP_LOG_SYSTEM_PRINTF("Level Button pressed", 0);
            break;

        case SwitchAction_t::PRESS_ACTION:
            GP_LOG_SYSTEM_PRINTF("Switch inital press", 0);
            break;

        case SwitchAction_t::RELEASE_ACTION:
            GP_LOG_SYSTEM_PRINTF("Switch release press", 0);
            break;

        case SwitchAction_t::COLOR_ACTION:
            GP_LOG_SYSTEM_PRINTF("Color button pressed", 0);
            break;

        case SwitchAction_t::TOGGLE_ACTION:
            GP_LOG_SYSTEM_PRINTF("Toggle Button pressed", 0);
            break;

        default:
            break;
    }
}

void AppManager::ActionCompletedCallback(uint8_t aAction, int32_t aActor)
{
    switch(aAction)
    {
        case SwitchAction_t::PRESS_ACTION:
            GP_LOG_SYSTEM_PRINTF("Switch is Pressed", 0);
            break;

        case SwitchAction_t::RELEASE_ACTION:
            GP_LOG_SYSTEM_PRINTF("Switch is Released", 0);
            break;

        case SwitchAction_t::LEVEL_ACTION:
        case SwitchAction_t::COLOR_ACTION:
        case SwitchAction_t::TOGGLE_ACTION:
            GP_LOG_SYSTEM_PRINTF("Binding command sent", 0);
            break;

        default:
            break;
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

    static SwitchColorAction_t mode;

    switch(aAction)
    {
        case SwitchAction_t::LEVEL_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB update Level", 0);
            ZigbeeEndpointMgr().SetLevel(SwitchMgr().mLevel);
            break;

        case SwitchAction_t::COLOR_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB update Color - direction %d", 0, mode);

            mode = (mode == ColorHueMoveDown) ? ColorHueMoveUp : ColorHueMoveDown; // toggle the
                                                                                   // mode
            ZigbeeEndpointMgr().StartColorControlMoveHue((unsigned char)mode);

            break;

        case SwitchAction_t::TOGGLE_ACTION:
            GP_LOG_SYSTEM_PRINTF("ZB handle Toggle", 0);
            ZigbeeEndpointMgr().OnOffToggle();
            break;

        // switching the binding mode
        case SwitchAction_t::PRESS_ACTION:
        case SwitchAction_t::RELEASE_ACTION:
            GP_LOG_SYSTEM_PRINTF("toggle binding Grouping mode", 0);
            if(ZigbeeTaskMgr().isBindingGrouping())
            {
                ZigbeeTaskMgr().ToggleControlMode();
            }
#if defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET) || defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR)
            ZigbeeTaskMgr().ToggleBindingGroupingMode();
#endif // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET) ||
       // defined(GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR)
            break;

        default:
            GP_LOG_SYSTEM_PRINTF("ZB handle default", 0);
            break;
    }
}
