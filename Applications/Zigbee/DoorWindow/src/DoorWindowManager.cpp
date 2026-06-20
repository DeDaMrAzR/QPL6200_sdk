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

/** @file "DoorWindowManager.cpp"
 */
#include "DoorWindowManager.h"
#include "gpLog.h"
#include "qDrvGPIO.h"
#include "qPinCfg.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

DoorWindowManager DoorWindowManager::sDoorWindow;

void DoorWindowManager::Init(void)
{
    mOpened = qDrvGPIO_Read(APP_CONTACT_SWITCH);
    mTampered = !qDrvGPIO_Read(APP_TAMPER_BUTTON);
}

void DoorWindowManager::SetCallbacks(DoorWindowCallback_fn aActionInitiated_CB,
                                     DoorWindowCallback_fn aActionCompleted_CB)
{
    mActionInitiated_CB = aActionInitiated_CB;
    mActionCompleted_CB = aActionCompleted_CB;
}

bool DoorWindowManager::InitiateAction(DoorWindowAction_t aAction, int32_t aActor, bool aValue)
{
    bool isStateChanged = false;

    switch(aAction)
    {
        case DoorWindowAction_t::CONTACT_ACTION:
            if(mOpened != aValue)
            {
                GP_LOG_SYSTEM_PRINTF("DoorWindowMgr:CONTACT: %s->%s", 0, mOpened ? "Opened" : "Closed",
                                     aValue ? "Opened" : "Closed");
                mOpened = aValue;
                isStateChanged = true;
            }
            break;

        case DoorWindowAction_t::TAMPER_ACTION:
            if(mTampered != aValue)
            {
                GP_LOG_SYSTEM_PRINTF("DoorWindowMgr:TAMPER: %sTampered->%sTampered", 0, mTampered ? "" : "Not ",
                                     aValue ? "" : "Not ");
                mTampered = aValue;
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

bool DoorWindowManager::IsOpened(void)
{
    return mOpened;
}

bool DoorWindowManager::IsTampered(void)
{
    return mTampered;
}
