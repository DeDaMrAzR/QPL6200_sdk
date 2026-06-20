/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

#include "MotionSensorManager.h"
#include "AppTask.h"
#include <lib/support/CHIPMem.h>
#include <lib/support/logging/CHIPLogging.h>

MotionSensorManager MotionSensorManager::sMotionSensor;
using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace chip::DeviceLayer;

CHIP_ERROR MotionSensorManager::Init(void)
{
    chip::BitMask<OccupancySensing::OccupancyBitmap> occupancyBit;
    chip::app::DataModel::Nullable<short unsigned int> attributeMeasuredValue;

    mIlluminance          = 0x0001;
    mCallbackStateChanged = nullptr;

    // Get OccupancyBitmap attribute value
    OccupancySensing::Attributes::Occupancy::Get(OCCUPANCY_SENSOR_ENDPOINT_ID, &occupancyBit);
    mState = (occupancyBit == OccupancySensing::OccupancyBitmap::kOccupied) ? State::kOccupied : State::kUnOccupied;

    // Get MeasuredValue attribute value
    IlluminanceMeasurement::Attributes::MeasuredValue::Get(LIGHT_SENSOR_ENDPOINT_ID, attributeMeasuredValue);
    if (!attributeMeasuredValue.IsNull())
    {
        mIlluminance = attributeMeasuredValue.Value();
    }

    ChipLogProgress(NotSpecified, "MotionSensorManager::Init: isOccupied %d, mIlluminance %d", (mState == State::kOccupied),
                    mIlluminance);
    return CHIP_NO_ERROR;
}

void MotionSensorManager::SetCallback(CallbackStateChanged aCallbackStateChanged)
{
    mCallbackStateChanged = aCallbackStateChanged;
}

bool MotionSensorManager::IsOccupied(void)
{
    return mState == State::kOccupied;
}

void MotionSensorManager::InitiateAction(Action aAction)
{
    if ((mState == State::kUnOccupied) && (aAction == Action::kSignalDetected))
    {
        mState = State::kOccupied;
    }
    else if ((mState == State::kOccupied) && (aAction == Action::kOccupiedToUnoccupied))
    {
        mState = State::kUnOccupied;
    }
    else
    {
        return;
    }

    if (sMotionSensor.mCallbackStateChanged != nullptr)
    {
        sMotionSensor.mCallbackStateChanged(sMotionSensor.mState);
    }
    else
    {
        ChipLogError(NotSpecified, "Callback for state change was not set. Please set an appropriate callback.");
    }
}

uint16_t MotionSensorManager::GetOccupiedToUnoccupiedDelay(void)
{
    return mOccupiedToUnoccupiedDelay;
}

void MotionSensorManager::SetOccupiedToUnoccupiedDelay(uint16_t delay)
{
    mOccupiedToUnoccupiedDelay = delay;
}

uint16_t MotionSensorManager::GetIlluminance(void)
{
    return mIlluminance;
}

void MotionSensorManager::SetIlluminance(uint16_t measured)
{
    mIlluminance = measured;
}
