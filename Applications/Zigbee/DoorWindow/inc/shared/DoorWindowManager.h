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

/** @file "DoorWindowManager.h"
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <functional>

#include "AppEvent.h"
#include "AppManager.h"
#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support

#define DOORWINDOW_ENDPOINT_ID  1

typedef enum { CONTACT_ACTION = 0, TAMPER_ACTION, INVALID_ACTION } DoorWindowAction_t;

class DoorWindowManager {
    public:
    void Init(void);

    using DoorWindowCallback_fn = void (*)(DoorWindowAction_t aAction, int32_t aActor);

    void SetCallbacks(DoorWindowCallback_fn aActionInitiated_CB, DoorWindowCallback_fn aActionCompleted_CB);
    bool InitiateAction(DoorWindowAction_t aAction, int32_t aActor, bool aValue);
    bool IsOpened(void);
    bool IsTampered(void);

    private:
    friend DoorWindowManager& DoorWindowMgr(void);
    bool mOpened;
    bool mTampered;

    static DoorWindowManager sDoorWindow;
    DoorWindowCallback_fn mActionInitiated_CB;
    DoorWindowCallback_fn mActionCompleted_CB;
};

inline DoorWindowManager& DoorWindowMgr(void)
{
    return DoorWindowManager::sDoorWindow;
}
