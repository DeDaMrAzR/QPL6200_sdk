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

/** @file "ButtonHandler.c"
 *
 * Implementation of Button handling
 */

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "FreeRTOS.h"
#include "timers.h"
#include "gpLog.h"
#include "gpSched.h"
#include "qDrvIOB.h"
#include "qDrvGPIO.h"
#include "ButtonHandler.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define BUTTON_MAX_CALLBACKS      3
#define BUTTON_DEBOUNCE_PERIOD_MS (20)

#define BUTTON_HANDLER_MAX_PIN_SUPPORTED (40)

typedef uint32_t ButtonState_t;
#define BUTTON_STATE_BITSIZE (32)

#define DIV_ROUND_UP(n, d)         ((n + d - 1) / d)
#define BUTTON_HANDLER_STATE_COUNT DIV_ROUND_UP(BUTTON_HANDLER_MAX_PIN_SUPPORTED, BUTTON_STATE_BITSIZE)

#define BUTTON_GET_BIT(a, x) BIT_TST(a[button->gpio / BUTTON_STATE_BITSIZE], (button->gpio % BUTTON_STATE_BITSIZE))
#define BUTTON_SET_BIT(a, x) BIT_SET(a[button->gpio / BUTTON_STATE_BITSIZE], (button->gpio % BUTTON_STATE_BITSIZE))

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
static void Button_GPIOCallback(void* arg, uint8_t gpio);
/*****************************************************************************
 *                    Variables Definitions
 *****************************************************************************/
static ButtonConfig_t* Buttons;
static ButtonHandler_Callback_t ButtonCallback;
static uint8_t ButtonCount;
static ButtonActiveLevel_t activeLevel;
static ButtonState_t ButtonState[BUTTON_HANDLER_STATE_COUNT];
static qDrvGPIO_CbInput_t buttonCallback[BUTTON_MAX_CALLBACKS];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static void InitPin(ButtonConfig_t* button)
{
    qDrvGPIO_InputConfig_t inputConfig = {
        .pull = qDrvIOB_PullUp,
        .schmittTrigger = false,
        .irqType = qDrvGPIO_IrqTypeNone,
        .highPriority = false,
        .wakeup = qDrvGPIO_WakeupNone,
        .callback = Button_GPIOCallback,
    };

    qDrvGPIO_InputConfigSet(button->gpio, &inputConfig);

    GP_LOG_PRINTF("[BTN] Added GPIO %d", 0, button->gpio);
}

static void SetEnableInterrupt(bool enable)
{
    for(uint8_t i = 0; i < ButtonCount; i++)
    {
        ButtonConfig_t* button = &Buttons[i];
        qDrvGPIO_IrqType_t irqType =
            (BUTTON_GET_BIT(ButtonState, button->gpio) ? qDrvGPIO_IrqTypeRisingEdge : qDrvGPIO_IrqTypeFallingEdge);

        qDrvGPIO_WakeupSet(button->gpio, (enable ? hal_WakeUpModeBoth : hal_WakeUpModeNone));

        if(enable)
        {
            qDrvGPIO_IrqConfigSet(button->gpio, irqType, false);
            qDrvGPIO_IrqEnable(button->gpio);
        }
        else
        {
            qDrvGPIO_IrqDisable(button->gpio);
        }
    }
}

static void PollGPIOs(void)
{
    ButtonState_t state[BUTTON_HANDLER_STATE_COUNT];

    GP_LOG_PRINTF("[BTN] Polling GPIO...", 0);

    for(uint8_t i = 0; i < BUTTON_HANDLER_STATE_COUNT; i++)
    {
        state[i] = 0;
    }

    for(uint8_t i = 0; i < ButtonCount; i++)
    {
        ButtonConfig_t* button = &Buttons[i];
        bool isPressed = false;

        if(qDrvGPIO_Read(button->gpio) == activeLevel)
        {
            BUTTON_SET_BIT(state, button->gpio);
            isPressed = true;
        }

        if(ButtonCallback != NULL)
        {
            bool wasPressed = BUTTON_GET_BIT(ButtonState, button->gpio);
            if(wasPressed != isPressed)
            {
                ButtonCallback(button->gpio, isPressed);
                GP_LOG_PRINTF("[BTN] GPIO %d changed: %d->%d", 0, button->gpio, wasPressed, isPressed);
            }
        }
    }

    for(uint8_t i = 0; i < BUTTON_HANDLER_STATE_COUNT; i++)
    {
        ButtonState[i] = state[i];
    }

    SetEnableInterrupt(true);
}

static void InterruptRoutine(void* arg, uint8_t gpio)
{
    for(uint8_t i = 0; i < ButtonCount; i++)
    {
        ButtonConfig_t* button = &Buttons[i];

        if(gpio == button->gpio)
        {
            SetEnableInterrupt(false);

            // Delay check for debouncing of button/signal
            if(!gpSched_ExistsEvent(PollGPIOs))
            {
                gpSched_ScheduleEvent(BUTTON_DEBOUNCE_PERIOD_MS * 1000, PollGPIOs);
            }

            return;
        }
    }
}

static bool Button_RegisterGPIOCallback(qDrvGPIO_CbInput_t pCallbacks)
{
    for(uint8_t i = 0; i < BUTTON_MAX_CALLBACKS; i++)
    {
        if(buttonCallback[i] == NULL)
        {
            buttonCallback[i] = pCallbacks;
            return true;
        }
    }

    return false;
}

static void Button_DeferredGPIOCallback(void* arg, uint32_t ulParameter2)
{
    uint8_t* gpio = (uint8_t*)arg;

    for(uint8_t i = 0; i < BUTTON_MAX_CALLBACKS; i++)
    {
        if(buttonCallback[i])
        {
            buttonCallback[i](NULL, *gpio);
        }
    }
}

static void Button_GPIOCallback(void* arg, uint8_t gpio)
{
    static uint8_t gpioinfo;

    gpioinfo = gpio;

    // The actual processing is to be deferred to a task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerPendFunctionCallFromISR(Button_DeferredGPIOCallback, &gpioinfo, 0, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void ButtonHandler_Init(const ButtonConfig_t* pButtons, const uint8_t count, ButtonActiveLevel_t level,
                        ButtonHandler_Callback_t pCallback)
{
    Buttons = (ButtonConfig_t*)pButtons;
    ButtonCount = count;
    activeLevel = level;
    ButtonCallback = NULL; // set to NULL deliberately to avoid PollGPIOs() calling to this callback

    for(uint8_t i = 0; i < ButtonCount; i++)
    {
        InitPin(&Buttons[i]);
    }

    PollGPIOs(); // update ButtonState to align with hardware status, enable interrupt afterwards

    Button_RegisterGPIOCallback(InterruptRoutine);

    ButtonCallback = pCallback;
}
