/*
 * Copyright (c) 2024-2025, Qorvo Inc
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
 */

/** @file "AppButtons.c"
 *
 * Implementation of Button handling
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID_APPFRAMEWORK 56
#define GP_COMPONENT_ID              GP_COMPONENT_ID_APPFRAMEWORK

#include "global.h"
#include "gpAssert.h"
#include "gpLog.h"
#include "gpSched.h"

#include "qPinCfg.h"
#include "AppTask.h"
#include "AppButtons.h"
#include "ButtonHandler.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define FUNCTION_KEY_STEP_TIMEOUT 1000000

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
AppButtons AppButtons::sAppButtons;

static const ButtonConfig_t buttons[] = QPINCFG_BUTTONS;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
void AppButtons::Init(void)
{
    // Setup button handler
    ButtonHandler_Init(buttons, Q_ARRAY_SIZE(buttons), BUTTON_LOW, EventHandler);

    sAppButtons.functionKeyHeldSec = 0;
    sAppButtons.multifunc_button = 0xFF;
}

void AppButtons::RegisterMultiFunc(uint8_t gpio)
{
    sAppButtons.multifunc_button = gpio;

    GP_LOG_SYSTEM_PRINTF("GPIO %d is now time sensitive.", 0, gpio);
}

void AppButtons::EventHandler(uint8_t btnIdx, bool btnPressed)
{
    AppEvent appButtonEvent = {};

    appButtonEvent.Type = AppEvent::kEventType_Buttons;
    appButtonEvent.ButtonEvent.Index = btnIdx;
    appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Invalid;

    if(btnIdx == sAppButtons.multifunc_button)
    {
        if(btnPressed)
        {
            appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Pressed;
            appButtonEvent.ButtonEvent.HeldSec = 0;
            gpSched_ScheduleEvent(FUNCTION_KEY_STEP_TIMEOUT, FunctionKeyTimeout);
        }
        else
        {
            if(gpSched_ExistsEvent(FunctionKeyTimeout))
            {
                appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Released;
                appButtonEvent.ButtonEvent.HeldSec = sAppButtons.functionKeyHeldSec;
                sAppButtons.functionKeyHeldSec = 0;

                gpSched_UnscheduleEvent(FunctionKeyTimeout);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("Ignore Button release", 0);
            }
        }
    }
    else
    {
        if(btnPressed)
        {
            appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Pressed;
            appButtonEvent.ButtonEvent.HeldSec = 0;
        }
        else
        {
            appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Released;
            appButtonEvent.ButtonEvent.HeldSec = 0;
        }
    }

    if(appButtonEvent.ButtonEvent.State != ButtonEvent_t::kButtonState_Invalid)
    {
        GetAppTask().PostEvent(&appButtonEvent);
    }
}

void AppButtons::FunctionKeyTimeout(void)
{
    sAppButtons.functionKeyHeldSec++;

    AppEvent appButtonEvent = {};

    appButtonEvent.Type = AppEvent::kEventType_Buttons;
    appButtonEvent.ButtonEvent.Index = sAppButtons.multifunc_button;
    appButtonEvent.ButtonEvent.State = ButtonEvent_t::kButtonState_Held;
    appButtonEvent.ButtonEvent.HeldSec = sAppButtons.functionKeyHeldSec;

    GetAppTask().PostEvent(&appButtonEvent);

    gpSched_ScheduleEvent(FUNCTION_KEY_STEP_TIMEOUT, FunctionKeyTimeout);
}
