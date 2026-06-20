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

/** @file "MotionSensorManager.h"
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <functional>

#include "AppEvent.h"
#include "AppManager.h"
#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support

typedef enum {
    OCCUPIED_ACTION = 0,
    UNOCCUPIED_ACTION,
    INVALID_ACTION
} MotionSensorAction_t;

class MotionSensorManager {
    public:
    void Init(void);

    using MotionSensorCallback_fn = void (*)(MotionSensorAction_t aAction, int32_t aActor);

    void SetCallbacks(MotionSensorCallback_fn aActionInitiated_CB, MotionSensorCallback_fn aActionCompleted_CB);
    bool InitiateAction(MotionSensorAction_t aAction, int32_t aActor);
    bool IsOccuipied(void);
    uint16_t GetOccupiedToUnoccupiedDelay(void);
    void SetOccupiedToUnoccupiedDelay(uint16_t delay);
    uint16_t GetIlluminance(void);
    void SetIlluminance(uint16_t value);

    private:
    friend MotionSensorManager& MotionSensorMgr(void);
    bool mOccupied;
    uint16_t mOccupiedToUnoccupiedDelay;
    uint16_t mIlluminance;

    static MotionSensorManager sMotionSensor;
    MotionSensorCallback_fn mActionInitiated_CB;
    MotionSensorCallback_fn mActionCompleted_CB;
};

inline MotionSensorManager& MotionSensorMgr(void)
{
    return MotionSensorManager::sMotionSensor;
}
