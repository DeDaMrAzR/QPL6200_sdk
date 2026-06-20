/*
 *
 *    Copyright (c) 2024 Project CHIP Authors
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

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <functional>

#include "AppConfig.h"
#include "AppEvent.h"

#include <lib/core/CHIPError.h>

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/cluster-objects.h>
#include <app/data-model/DecodableList.h>
#include <app/util/im-client-callbacks.h>

#define OCCUPANCY_SENSOR_ENDPOINT_ID 1
#define LIGHT_SENSOR_ENDPOINT_ID 2

class MotionSensorManager
{
public:
    enum class Action : uint8_t
    {
        kSignalDetected = 0,
        kSignalLost,
        kOccupiedToUnoccupied,
        kInvalid
    };

    enum class State : uint8_t
    {
        kOccupied = 0,
        kUnOccupied,
        kInvalid
    };

    CHIP_ERROR Init(void);
    void InitiateAction(Action aAction);

    typedef void (*CallbackStateChanged)(State aState);
    void SetCallback(CallbackStateChanged aCallbackStateChanged);
    bool IsOccupied(void);
    uint16_t GetOccupiedToUnoccupiedDelay(void);
    void SetOccupiedToUnoccupiedDelay(uint16_t delay);
    uint16_t GetIlluminance(void);
    void SetIlluminance(uint16_t measured);
    void SendOnOffToBingingTarget(bool bOn);

private:
    friend MotionSensorManager & MotionSensorMgr(void);
    State mState;
    uint16_t mOccupiedToUnoccupiedDelay;
    uint16_t mIlluminance;
    CallbackStateChanged mCallbackStateChanged;
    static MotionSensorManager sMotionSensor;
};

inline MotionSensorManager & MotionSensorMgr(void)
{
    return MotionSensorManager::sMotionSensor;
}
