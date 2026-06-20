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

/** @file "StatusLed.c"
 *
 * Implementation of Status Led handling
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpSched.h"
#include "qDrvIOB.h"
#include "qDrvGPIO.h"
#include "StatusLed.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define STATUSLED_MAX_COUNT (2)

#define STATUSLED_UNUSED_GPIO (0xFF)
typedef struct {
    uint8_t gpio;
    bool state;
    struct {
        uint16_t onMs;
        uint16_t offMs;
    } blink;
} Led_t;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
static void BlinkTimeout(void* pArg);

/*****************************************************************************
 *                    Variables Definitions
 *****************************************************************************/
static Led_t leds[STATUSLED_MAX_COUNT];
static bool activeLevel;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static Led_t* GetLedByGpio(uint8_t gpio)
{
    for(uint8_t i = 0; i < STATUSLED_MAX_COUNT; i++)
    {
        if(leds[i].gpio == gpio)
        {
            return &leds[i];
        }
    }

    return NULL;
}

static void InitPin(Led_t* led)
{
    qDrvIOB_AlternateSet(led->gpio, qDrvIOB_AlternateNone);

    qDrvGPIO_OutputConfig_t outputConfig = Q_DRV_GPIO_OUTPUT_CONFIG_DEFAULT;

    qDrvGPIO_OutputConfigSet(led->gpio, &outputConfig);
}

static void SetOnOff(Led_t* led, bool state)
{
    if(led == NULL)
    {
        return;
    }

    if(activeLevel)
    {
        qDrvGPIO_Write(led->gpio, (state ? true : false));
    }
    else
    {
        qDrvGPIO_Write(led->gpio, (state ? false : true));
    }

    led->state = state;
}

static void BlinkTimeout(void* pArg)
{
    Led_t* led = (Led_t*)pArg;

    if(led == NULL)
    {
        return;
    }

    if(led->state)
    {
        if(led->blink.offMs > 0)
        {
            SetOnOff(led, false);
            gpSched_ScheduleEventArg(led->blink.offMs * 1000, BlinkTimeout, pArg);
        }
    }
    else
    {
        if(led->blink.onMs > 0)
        {
            SetOnOff(led, true);
            gpSched_ScheduleEventArg(led->blink.onMs * 1000, BlinkTimeout, pArg);
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void StatusLed_Init(const uint8_t* gpio, uint8_t count, bool active)
{
    for(uint8_t i = 0; i < STATUSLED_MAX_COUNT; i++)
    {
        leds[i].gpio = STATUSLED_UNUSED_GPIO;
    }

    if(count > STATUSLED_MAX_COUNT)
    {
        return;
    }

    activeLevel = active;

    for(uint8_t i = 0; i < count; i++)
    {
        Led_t* led = &leds[i];

        led->gpio = gpio[i];

        InitPin(led);

        SetOnOff(led, false);
    }
}

void StatusLed_SetLed(uint8_t gpio, bool state)
{
    Led_t* led = GetLedByGpio(gpio);

    if(led == NULL)
    {
        return;
    }

    gpSched_UnscheduleEventArg(BlinkTimeout, (void*)led);

    SetOnOff(GetLedByGpio(gpio), state);
}

void StatusLed_BlinkLed(uint8_t gpio, uint16_t onMs, uint16_t offMs)
{
    Led_t* led = GetLedByGpio(gpio);

    if(led == NULL)
    {
        return;
    }

    led->blink.onMs = onMs;
    led->blink.offMs = offMs;

    gpSched_UnscheduleEventArg(BlinkTimeout, (void*)led);

    SetOnOff(led, true);
    gpSched_ScheduleEventArg(led->blink.onMs * 1000, BlinkTimeout, (void*)led);
}
