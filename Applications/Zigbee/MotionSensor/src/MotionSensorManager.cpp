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

/** @file "MotionSensorManager.cpp"
 */
#include "MotionSensorManager.h"
#include "ZigBee.h"
#include "OccupancySensorApplication.h"
#include "gpLog.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

// default initialization value for the Illuminance MeasuredValue
constexpr uint16_t kDefaultIlluminanceMeasuredValue = 0x0001;

MotionSensorManager MotionSensorManager::sMotionSensor;

void MotionSensorManager::Init(void)
{
    mOccupied = false;
    mOccupiedToUnoccupiedDelay = COccupancySensorApplication::defaultOccupiedToUnoccupiedDelay;
    mIlluminance = kDefaultIlluminanceMeasuredValue;
}

void MotionSensorManager::SetCallbacks(MotionSensorCallback_fn aActionInitiated_CB,
                                       MotionSensorCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool MotionSensorManager::InitiateAction(MotionSensorAction_t aAction, int32_t aActor)
{
    bool isStateChanged = false;

    switch(aAction)
    {
        case MotionSensorAction_t::OCCUPIED_ACTION:
            GP_LOG_SYSTEM_PRINTF("MotionSensorMgr:OCCUPANCY: %s->Occupied", 0, mOccupied ? "Occupied" : "Unoccupied");
            mOccupied = true;
            isStateChanged = true;
            break;

        case MotionSensorAction_t::UNOCCUPIED_ACTION:
            if(mOccupied)
            {
                GP_LOG_SYSTEM_PRINTF("MotionSensorMgr:OCCUPANCY: Occupied->Unoccupied", 0);
                mOccupied = false;
                isStateChanged = true;
            }
            break;

        default:
            break;
    }

    if(isStateChanged)
    {
        if(mActionInitiated_CB)
        {
            mActionInitiated_CB(aAction, aActor);
        }

        // Do something if required

        if(mActionCompleted_CB)
        {
            mActionCompleted_CB(aAction, aActor);
        }
    }

    return isStateChanged;
}

bool MotionSensorManager::IsOccuipied(void)
{
    return mOccupied;
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

void MotionSensorManager::SetIlluminance(uint16_t value)
{
    mIlluminance = value;
}
