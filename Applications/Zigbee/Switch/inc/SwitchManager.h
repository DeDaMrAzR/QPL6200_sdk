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

/** @file "SwitchManager.h"
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <functional>

#include "AppEvent.h"
#include "AppManager.h"
#include "binding-handler.h"
#include "FreeRTOS.h"
#include "timers.h" // provides FreeRTOS timer support

#define SWITCH_ENDPOINT_ID        1
#define GENERICSWITCH_ENDPOINT_ID 2

typedef enum {
    ColorHueMoveStop = 0x00,
    ColorHueMoveUp = 0x01,
    ColorHueMoveDown = 0x03,
} SwitchColorAction_t;

typedef enum {
    LEVEL_ACTION = 0,
    PRESS_ACTION,
    RELEASE_ACTION,
    COLOR_ACTION,
    TOGGLE_ACTION,
    INVALID_ACTION
} SwitchAction_t;

class SwitchManager {
    public:
    enum Level_t {
        MIN_LEVEL = 0x40,
        MAX_LEVEL = 0xfe,
    };

    enum Color_t {
        COLOR_AMBER = 1,
        COLOR_GREEN,
    };

    void Init(void);

    using SwitchCallback_fn = void (*)(SwitchAction_t aAction, int32_t aActor);

    void SetCallbacks(SwitchCallback_fn aActionInitiated_CB, SwitchCallback_fn aActionCompleted_CB);
    bool InitiateAction(SwitchAction_t aAction, int32_t aActor);

    bool IsSwitchPressed(void);

    // attributes
    uint8_t mLevel;
    uint8_t mColor;
    colorXYData_s mColorXY;

    private:
    friend SwitchManager& SwitchMgr(void);

    void GenericSwitchInitialPress(void);
    void GenericSwitchReleasePress(void);
    void ToggleHandler(void);
    void LevelHandler(void);
    void ColorHandler(void);

    static SwitchManager sSwitch;
    SwitchCallback_fn mActionInitiated_CB;
    SwitchCallback_fn mActionCompleted_CB;
    bool isPressed;
};

inline SwitchManager& SwitchMgr(void)
{
    return SwitchManager::sSwitch;
}
