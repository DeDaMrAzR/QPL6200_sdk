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

/** @file "SwitchManager.cpp"
 */

#include "SwitchManager.h"
#include "gpLog.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

SwitchManager SwitchManager::sSwitch;

void SwitchManager::Init(void)
{
    // init - TODO
}

void SwitchManager::SetCallbacks(SwitchCallback_fn aActionInitiated_CB, SwitchCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool SwitchManager::InitiateAction(SwitchAction_t aAction, int32_t aActor)
{
    bool isClusterChange = false;

    if(mActionInitiated_CB)
    {
        mActionInitiated_CB(aAction, aActor);
    }

    switch(aAction)
    {
        case SwitchAction_t::LEVEL_ACTION:
            LevelHandler();
            isClusterChange = true;
            break;

        case SwitchAction_t::PRESS_ACTION:
            GenericSwitchInitialPress();
            isClusterChange = true;
            break;

        case SwitchAction_t::RELEASE_ACTION:
            GenericSwitchReleasePress();
            isClusterChange = true;
            break;

        case SwitchAction_t::COLOR_ACTION:
            ColorHandler();
            isClusterChange = true;
            break;

        case SwitchAction_t::TOGGLE_ACTION:
            ToggleHandler();
            isClusterChange = true;
            break;

        default:
            break;
    }

    if(mActionCompleted_CB)
    {
        mActionCompleted_CB(aAction, aActor);
    }

    return isClusterChange;
}

bool SwitchManager::IsSwitchPressed()
{
    return isPressed;
}

void SwitchManager::ToggleHandler()
{
    // place holder
}

void SwitchManager::LevelHandler()
{
    // toggle the level for now
    mLevel = (mLevel == MIN_LEVEL) ? MAX_LEVEL : MIN_LEVEL;
}

void SwitchManager::ColorHandler()
{
    if(mColor == COLOR_AMBER)
    {
        mColor = COLOR_GREEN;
        mColorXY.x = 36044;
        mColorXY.y = 29490;
        GP_LOG_SYSTEM_PRINTF("Color - AMBER", 0);
    }
    else
    {
        mColor = COLOR_AMBER;
        mColorXY.x = 7536;
        mColorXY.y = 54131;
        GP_LOG_SYSTEM_PRINTF("Color - GREEN", 0);
    }
}

void SwitchManager::GenericSwitchInitialPress(void)
{
    // Press moves Position from 0 (idle) to 1 (press)
    isPressed = true;
}

void SwitchManager::GenericSwitchReleasePress(void)
{
    // Release moves Position from 1 (press) to 0
    isPressed = false;
}
