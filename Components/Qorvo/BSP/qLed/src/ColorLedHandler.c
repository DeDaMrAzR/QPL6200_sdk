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

/** @file "ColorLedHandler.c"
 *
 * Implementation of Color (RGB) Led handling
 */

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpLog.h"
#include "qDrvPWMXL.h"
#include "ColorLedHandler.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define PWM_CHANNEL_GET(x) (ColorLed.channel[x])
#define PWM_CHANNEL_R      PWM_CHANNEL_GET(RGB_CHANNEL_R)
#define PWM_CHANNEL_G      PWM_CHANNEL_GET(RGB_CHANNEL_G)
#define PWM_CHANNEL_B      PWM_CHANNEL_GET(RGB_CHANNEL_B)

#define PWMXL_MAX_DUTY_PERCENT (100UL * 100UL)
#define PWM_DUTY_CYCLE_MULT    (PWMXL_MAX_DUTY_PERCENT / 256)

typedef enum { RGB_CHANNEL_R = 0, RGB_CHANNEL_G, RGB_CHANNEL_B, RGB_CHANNEL_COUNT } ColorChannel_t;

typedef struct {
    bool onoff;
    uint8_t channel[RGB_CHANNEL_COUNT];
    uint8_t level[RGB_CHANNEL_COUNT];
} ColorLed_t;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Variables Definitions
 *****************************************************************************/
// PWMXL instance
static qDrvPWMXL_t pwmxlInst = Q_DRV_PWMXL_INSTANCE_DEFINE(0);
static const qDrvPWMXL_Config_t pwmxlConfig = Q_DRV_PWMXL_CONFIG_DEFAULT();
static ColorLed_t ColorLed;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static void InitPin(ColorChannel_t channel, const qPinAltMapper_t* pConfig)
{
    qDrvPWMXL_PinConfig_t pinConfig;

    pinConfig.pwm.pin = pConfig->pinAlt.pin;
    pinConfig.pwm.alternate = pConfig->pinAlt.alternate;
    ColorLed.channel[channel] = pConfig->channel;
    qDrvPWMXL_PinConfigSet(&pinConfig);

    qDrvPWMXL_ChannelInit(&pwmxlInst, pConfig->channel);
    qDrvPWMXL_DriveSet(&pwmxlInst, pConfig->channel, qRegCommon_DrivePP);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void ColorLedHandler_Init(const qPinAltMapper_t* pConfig, uint8_t count)
{
    if((pConfig == NULL) || (count != RGB_CHANNEL_COUNT))
    {
        return;
    }

    if((pConfig[RGB_CHANNEL_R].channel == pConfig[RGB_CHANNEL_G].channel) ||
       (pConfig[RGB_CHANNEL_G].channel == pConfig[RGB_CHANNEL_B].channel) ||
       (pConfig[RGB_CHANNEL_B].channel == pConfig[RGB_CHANNEL_R].channel))
    {
        // channel overlap
        return;
    }

    // PWMXL
    qDrvPWMXL_Init(&pwmxlInst, &pwmxlConfig, NULL, NULL);

    for(uint8_t i = 0; i < RGB_CHANNEL_COUNT; i++)
    {
        InitPin(i, &pConfig[i]);
    }

    ColorLedHandler_SetOnOff(false);
}

void ColorLedHandler_SetOnOff(bool onoff)
{
    qDrvPWMXL_Enable(&pwmxlInst, onoff);
    ColorLed.onoff = onoff;

    if(onoff)
    {
        GP_LOG_SYSTEM_PRINTF("[ColorLED] Turned ON", 0);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("[ColorLED] Turned OFF", 0);
    }
}

void ColorLedHandler_SetColorLevel(uint8_t r, uint8_t g, uint8_t b)
{
    qDrvPWMXL_ChannelDutySet(&pwmxlInst, PWM_CHANNEL_R, (UInt32)r * PWM_DUTY_CYCLE_MULT);
    qDrvPWMXL_ChannelDutySet(&pwmxlInst, PWM_CHANNEL_G, (UInt32)g * PWM_DUTY_CYCLE_MULT);
    qDrvPWMXL_ChannelDutySet(&pwmxlInst, PWM_CHANNEL_B, (UInt32)b * PWM_DUTY_CYCLE_MULT);

    ColorLed.level[RGB_CHANNEL_R] = r;
    ColorLed.level[RGB_CHANNEL_G] = g;
    ColorLed.level[RGB_CHANNEL_B] = b;

    GP_LOG_SYSTEM_PRINTF("[ColorLED] Update level %d,%d,%d", 0, r, g, b);
}
