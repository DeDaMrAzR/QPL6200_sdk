/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#if !defined(GP_APP_DIVERSITY_POWERCYCLECOUNTING)
#error This application requires powercycle counting.
#endif

#include "gpSched.h"
#include "powercycle_counting.h"

#include "AppConfig.h"
#include "AppEvent.h"
#include "AppTask.h"
#include "App_Battery.h"
#include "ota.h"

#include "ButtonHandler.h"
#include "StatusLed.h"
#include "qPinCfg.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/DeferredAttributePersistenceProvider.h>
#include <app/clusters/on-off-server/on-off-server.h>
#include <app/server/Server.h>

#ifdef GP_UPGRADE_DIVERSITY_DUAL_BOOT
#include "combo-handler.h"
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;

#include <platform/CHIPDeviceLayer.h>

const uint8_t StatusLedGpios[] = QPINCFG_STATUS_LED;
const ButtonConfig_t buttons[] = QPINCFG_BUTTONS;

AppTask AppTask::sAppTask;

CHIP_ERROR AppTask::Init()
{
    // Initialize common code in base class
    CHIP_ERROR err = BaseAppTask::Init();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "BaseAppTask::Init() failed");
        return err;
    }
    DoorWindowMgr().SetCallback(OnStateChanged);

    // Init ZCL Data Model and start server
    PlatformMgr().ScheduleWork(AppTask::InitServerWrapper, 0);

    // Setup powercycle reset expired handler
    gpAppFramework_SetResetExpiredHandler(AppTask::PowerCycleExpiredHandlerWrapper);

    // Setup button handler
    ButtonHandler_Init(buttons, Q_ARRAY_SIZE(buttons), BUTTON_LOW, AppTask::ButtonEventHandlerWrapper);

    UpdateDeviceLedState(); // update DoorWindowLED
    gpSched_ScheduleEvent(1 * ONE_SECOND_US, Application_StartPeriodicBatteryUpdate);

    return err;
}

void AppTask::InitServer(intptr_t arg)
{
    BaseAppTask::InitServer(arg);

    // Open commissioning after boot if no fabric was available
    if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0)
    {
        ChipLogProgress(NotSpecified, "No fabrics, starting commissioning.");
        AppTask::OpenCommissioning((intptr_t) 0);
    }
}

void AppTask::OnStateChanged(DoorWindowManager::State aState)
{
    // If the door/window state was changed, update LED state and cluster state (only if button was pressed).
    //  - turn on the door/window LED if door/window sensor is in closed state.
    //  - turn off the lock LED if door/window sensor is in opened state.
    if (DoorWindowManager::State::kDoorWindowClosed == aState)
    {
        ChipLogProgress(NotSpecified, "DoorWindow state changed to CLOSED");
        DoorWindowLedSet(true);
    }
    else if (DoorWindowManager::State::kDoorWindowOpened == aState)
    {
        ChipLogProgress(NotSpecified, "DoorWindow state changed to OPEN");
        DoorWindowLedSet(false);
    }

    if (sAppTask.IsSyncClusterToButtonAction())
    {
        sAppTask.UpdateClusterState();
    }
}

bool AppTask::ButtonEventHandler(uint8_t btnIdx, bool btnPressed)
{
    // Call base class ButtonEventHandler
    bool eventHandled = BaseAppTask::ButtonEventHandler(btnIdx, btnPressed);
    if (eventHandled)
    {
        return true;
    }

    // Only go ahead if button index has a supported value
    if (btnIdx != DOORWINDOW_SENSOR_BUTTON)
    {
        return false;
    }

    ChipLogProgress(NotSpecified, "ButtonEventHandler %d, %d", btnIdx, btnPressed);

    AppEvent button_event              = {};
    button_event.Type                  = AppEvent::kEventType_Button;
    button_event.ButtonEvent.ButtonIdx = btnIdx;
    button_event.ButtonEvent.Action    = btnPressed;

    if (btnIdx == DOORWINDOW_SENSOR_BUTTON)
    {
        if (btnPressed)
        {
            button_event.DoorWindowEvent.Action = static_cast<uint8_t>(DoorWindowManager::Action::kSignalDetected);
            ChipLogProgress(NotSpecified, "ButtonEventHandler signal detected");
            button_event.Handler = DoorWindowActionEventHandler;
        }
        else
        {
            button_event.DoorWindowEvent.Action = static_cast<uint8_t>(DoorWindowManager::Action::kSignalLost);
            ChipLogProgress(NotSpecified, "ButtonEventHandler signal lost");
            button_event.Handler = DoorWindowActionEventHandler;
        }
    }
    else
    {
        return false;
    }

    sAppTask.PostEvent(&button_event);

    return true;
}

void AppTask::DoorWindowActionEventHandler(AppEvent * aEvent)
{
    DoorWindowManager::Action action = DoorWindowManager::Action::kInvalid;
    CHIP_ERROR err                   = CHIP_NO_ERROR;

    if (aEvent->Type == AppEvent::kEventType_DoorWindow)
    {
        action = static_cast<DoorWindowManager::Action>(aEvent->DoorWindowEvent.Action);
    }
    else if (aEvent->Type == AppEvent::kEventType_Button)
    {
        action = static_cast<DoorWindowManager::Action>(aEvent->DoorWindowEvent.Action);
        sAppTask.SetSyncClusterToButtonAction(true);
    }
    else
    {
        err    = APP_ERROR_UNHANDLED_EVENT;
        action = DoorWindowManager::Action::kInvalid;
    }

    if (err == CHIP_NO_ERROR)
    {
        DoorWindowMgr().InitiateAction(action);
    }
}

/**
 * Update cluster status after application level changes
 */
void AppTask::UpdateClusterState(void)
{
    SystemLayer().ScheduleLambda([] {
        uint8_t newValue = DoorWindowMgr().IsDoorWindowClosed();
        ChipLogProgress(NotSpecified, "UpdateClusterState StateValue::Set : %d", newValue);

        // write the new boolean state value
        Protocols::InteractionModel::Status status =
            app::Clusters::BooleanState::Attributes::StateValue::Set(DOORWINDOW_ENDPOINT_ID, newValue);
        if (status != Protocols::InteractionModel::Status::Success)
        {
            ChipLogError(NotSpecified, "ERR: updating boolean status value %x", to_underlying(status));
        }
    });
}

void AppTask::UpdateDeviceLedState(void)
{
    SystemLayer().ScheduleLambda([] {
        bool stateValueAttrValue = false;

        /* get boolean state attribute value */
        (void) app::Clusters::BooleanState::Attributes::StateValue::Get(1, &stateValueAttrValue);

        ChipLogProgress(NotSpecified, "StateValue::Get : %d", stateValueAttrValue);
        DoorWindowLedSet(stateValueAttrValue); // stateValueAttrValue: closed-1, open-0
    });
}

/** @brief Set DoorWindow Sensor LED ON or OFF.
 *
 *   @param state     LED on (true) or off (false).
 */
void AppTask::DoorWindowLedSet(bool state)
{
#if CONFIG_ENABLE_DOORWINDOW_SENSOR_STATUS_LED
    StatusLed_SetLed(DOORWINDOW_SENSOR_LED, state);
#endif
}

void AppTask::PostDoorWindowActionRequest(DoorWindowManager::Action aAction)
{
    AppEvent event;
    event.Type                   = AppEvent::kEventType_DoorWindow;
    event.DoorWindowEvent.Action = static_cast<uint8_t>(aAction);
    event.Handler                = DoorWindowActionEventHandler;

    PostEvent(&event);
}

void AppTask::PowerCycleExpiredHandler(uint8_t resetCounts)
{
    if (resetCounts >= NMBR_OF_RESETS_BLE_ADVERTISING)
    {
        // Open commissioning if no fabric was available
        if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0)
        {
            ChipLogProgress(NotSpecified, "No fabrics, starting commissioning.");
            AppTask::OpenCommissioning((intptr_t) 0);
        }
    }
}
