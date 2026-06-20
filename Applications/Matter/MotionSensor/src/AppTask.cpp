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

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#include "ButtonHandler.h"
#include "StatusLed.h"
#include "qPinCfg.h"

#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
#include "combo-handler.h"
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT

#include <app/server/Server.h>

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

    // Init ZCL Data Model and start server
    PlatformMgr().ScheduleWork(AppTask::InitServerWrapper, 0);

    // Setup button handler
    ButtonHandler_Init(buttons, Q_ARRAY_SIZE(buttons), BUTTON_LOW, AppTask::ButtonEventHandlerWrapper);

    // Init motion sensor
    err = MotionSensorMgr().Init();
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "MotionSensorMgr().Init() failed");
        return err;
    }
    MotionSensorMgr().SetCallback(OnOccupancyStateChanged);

    gpSched_ScheduleEvent(1 * ONE_SECOND_US, Application_StartPeriodicBatteryUpdate);

    err = chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds32(LIGHT_SENSOR_MEASURE_INTERVAL_SECONDS),
                                                      UpdateIlluminanceClusterHandler, nullptr);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NotSpecified, "StartTimer failed %s: ", chip::ErrorStr(err));
    }

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

void AppTask::OnOccupancyStateChanged(MotionSensorManager::State aState)
{
    sAppTask.UpdateClusterState();
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
    if (btnIdx != PIR_SENSOR_BUTTON && btnIdx != LIGHT_SENSOR_BUTTON)
    {
        return false;
    }

    ChipLogProgress(NotSpecified, "ButtonEventHandler Btn[%d], %d", btnIdx, btnPressed);

    AppEvent button_event              = {};
    button_event.Type                  = AppEvent::kEventType_Button;
    button_event.ButtonEvent.ButtonIdx = btnIdx;
    button_event.ButtonEvent.Action    = btnPressed;

    if (btnIdx == PIR_SENSOR_BUTTON)
    {
        if (btnPressed == true)
        {
            button_event.OccupancyEvent.Action = static_cast<uint8_t>(MotionSensorManager::Action::kSignalDetected);
        }
        else
        {
            button_event.OccupancyEvent.Action = static_cast<uint8_t>(MotionSensorManager::Action::kSignalLost);
        }

        button_event.Type    = AppEvent::kEventType_Occupancy;
        button_event.Handler = MotionSensorActionEventHandler;
    }
    else if (btnIdx == LIGHT_SENSOR_BUTTON && btnPressed == true)
    {
        button_event.Type                      = AppEvent::kEventType_Illuminance;
        button_event.Handler                   = MotionSensorActionEventHandler;
        button_event.IlluminanceEvent.Measured = MotionSensorMgr().GetIlluminance() < 0xfffe ? 0xfffe : 0x0001;
    }
    else
    {
        return false;
    }

    sAppTask.PostEvent(&button_event);

    return true;
}

void AppTask::UpdateIlluminanceClusterHandler(chip::System::Layer * aLayer, void * aAppState)
{
    uint16_t measuredValue = MotionSensorMgr().GetIlluminance();
    Clusters::IlluminanceMeasurement::Attributes::MeasuredValue::Set(LIGHT_SENSOR_ENDPOINT_ID, measuredValue);

    chip::DeviceLayer::SystemLayer().StartTimer(chip::System::Clock::Seconds32(LIGHT_SENSOR_MEASURE_INTERVAL_SECONDS),
                                                UpdateIlluminanceClusterHandler, nullptr);
}

void AppTask::OccupiedToUnoccupiedDelayExpired(chip::System::Layer * aLayer, void * aAppState)
{
    AppEvent event;
    event.Type                  = AppEvent::kEventType_Occupancy;
    event.OccupancyEvent.Action = static_cast<uint8_t>(MotionSensorManager::Action::kOccupiedToUnoccupied);
    event.Handler               = MotionSensorActionEventHandler;
    sAppTask.PostEvent(&event);
}

void AppTask::MotionSensorActionEventHandler(AppEvent * aEvent)
{
    MotionSensorManager::Action action = MotionSensorManager::Action::kInvalid;

    // TODO: Don't use timer to implement holdtime logic in the application layer
    // This logic is already implemented as part of the Occupancy Sensing Cluster
    // We should get an event that the occupancy state is changed. This should be the trigger for
    // the application to handle the occupancy. We should not maintain the timer ourselves.
    if (aEvent->Type == AppEvent::kEventType_Occupancy)
    {
        action = static_cast<MotionSensorManager::Action>(aEvent->OccupancyEvent.Action);
        if (action == MotionSensorManager::Action::kSignalDetected)
        {
            chip::DeviceLayer::SystemLayer().CancelTimer(OccupiedToUnoccupiedDelayExpired, NULL);

            ChipLogProgress(NotSpecified, "Occupancy signal detected");
        }
        else if (action == MotionSensorManager::Action::kSignalLost)
        {
            chip::DeviceLayer::SystemLayer().CancelTimer(OccupiedToUnoccupiedDelayExpired, NULL);

            CHIP_ERROR err = chip::DeviceLayer::SystemLayer().StartTimer(
                chip::System::Clock::Seconds16(MotionSensorMgr().GetOccupiedToUnoccupiedDelay()), OccupiedToUnoccupiedDelayExpired,
                nullptr);
            if (err != CHIP_NO_ERROR)
            {
                ChipLogError(NotSpecified, "StartTimer failed %s: ", chip::ErrorStr(err));
            }

            ChipLogProgress(NotSpecified, "Occupancy signal lost");
        }

        MotionSensorMgr().InitiateAction(action);
    }
    else if (aEvent->Type == AppEvent::kEventType_Illuminance)
    {
        MotionSensorMgr().SetIlluminance(aEvent->IlluminanceEvent.Measured);

        ChipLogProgress(NotSpecified, "Update Illuminance 0x%04x", aEvent->IlluminanceEvent.Measured);
    }
    else
    {
        ChipLogError(NotSpecified, "A non MotionAction event received %d", aEvent->Type);
    }
}

/**
 * Update cluster status after application level changes
 */
void AppTask::UpdateClusterState(void)
{
    SystemLayer().ScheduleLambda([] {
        bool isOccupied = MotionSensorMgr().IsOccupied();

        ChipLogProgress(NotSpecified, "UpdateClusterState");

        // If the occupancy state was changed, update LED state and cluster state.
        //  - turn on the occupancy LED if occupancy sensor is in occupied state.
        //  - turn off the occupancy LED if occupancy sensor is in unoccupied state.
        Clusters::OccupancySensing::Attributes::Occupancy::Set(OCCUPANCY_SENSOR_ENDPOINT_ID, static_cast<uint8_t>(isOccupied));
        sAppTask.MotionSensorLedSet(isOccupied);

        ChipLogProgress(NotSpecified, "Update Occupancy: %u", isOccupied);
    });
}

void AppTask::OccupancyPostAttributeChangeCallback(chip::AttributeId attributeId, uint8_t type, uint16_t size, uint8_t * value)
{
    if (attributeId == Clusters::OccupancySensing::Attributes::PIROccupiedToUnoccupiedDelay::Id)
    {
        uint16_t OccupiedToUnoccupiedDelay = static_cast<uint16_t>(chip::Encoding::LittleEndian::Get16(value));
        MotionSensorMgr().SetOccupiedToUnoccupiedDelay(OccupiedToUnoccupiedDelay);

        ChipLogProgress(NotSpecified, "OccupancyPostAttributeChangeCallback PIROccupiedToUnoccupiedDelay : 0x%04x",
                        OccupiedToUnoccupiedDelay);
    }
}

/** @brief Set Motion Sensor LED ON or OFF.
 *
 *   @param state     LED on (true) or off (false).
 */
void AppTask::MotionSensorLedSet(bool state)
{
#if CONFIG_ENABLE_MOTION_SENSOR_STATUS_LED
    StatusLed_SetLed(MOTION_SENSOR_LED, state);
#endif
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
