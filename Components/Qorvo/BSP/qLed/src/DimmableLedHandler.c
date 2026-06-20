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

/** @file "WhiteLedHandler.c"
 *
 * Implementation of Warm and Cool White LEDs handling
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "DimmableLedHandler.h"
#include "gpLog.h"
#include "qDrvLED.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define DIMMABLE_LED_MAX_NUM 4

typedef struct {
    bool onOff;
    bool slope;
    bool fade;
    uint32_t freq;
    uint8_t pwm;
    qDrvLED_ChannelConfig_t channelConfig;
    qPinAltMapper_t* pPinConfig;
} DimLed_t;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/
static DimLed_t* GetDimLed(uint8_t channel, bool bAllow_GetNew_DimLed);

/*****************************************************************************
 *                    Variables Definitions
 *****************************************************************************/
static qDrvLED_t ledInst;
static qDrvLED_Config_t ledConfig = Q_DRV_LED_CONFIG_DEFAULT;
static DimLed_t DimLed[DIMMABLE_LED_MAX_NUM];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static DimLed_t* GetDimLed(uint8_t channel, bool bAllow_GetNew_DimLed)
{
    UInt8 i;

    for(i = 0; i < DIMMABLE_LED_MAX_NUM; i++)
    {
        if(DimLed[i].pPinConfig != NULL)
        {
            if(DimLed[i].pPinConfig->channel == channel)
            {
                return &DimLed[i];
            }
        }
        else if(bAllow_GetNew_DimLed)
        {
            return &DimLed[i];
        }
    }

    return NULL;
}

static void InitPin(qPinAltMapper_t* pConfig)
{
    qDrvLED_PinConfig_t ledPinConfig;
    DimLed_t* pDimLed = GetDimLed(pConfig->channel, true);

    if(pDimLed == NULL)
    {
        GP_LOG_SYSTEM_PRINTF("Dimmable LED initiation failure", 0);
        return;
    }

    // init DimLed
    pDimLed->pPinConfig = pConfig;
    pDimLed->channelConfig.drive = GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL;
    pDimLed->channelConfig.invert = false;
    pDimLed->channelConfig.allowSleep = true;
    pDimLed->freq = ledConfig.frequency;
    pDimLed->slope = false;
    pDimLed->onOff = false;

    // init ledPinConfig
    ledPinConfig.led.pin = pDimLed->pPinConfig->pinAlt.pin;
    ledPinConfig.led.alternate = pDimLed->pPinConfig->pinAlt.alternate;

    qDrvLED_PinConfigSet(&ledPinConfig);
    qDrvLED_Configure(&ledInst, pDimLed->pPinConfig->channel, &(pDimLed->channelConfig));
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void DimmableLedHandler_Init(qPinAltMapper_t* pConfig, uint8_t count)
{
    memset(DimLed, 0x0, sizeof(DimLed_t) * DIMMABLE_LED_MAX_NUM);

    qDrvLED_Init(&ledInst, &ledConfig);

    for(uint8_t i = 0; i < count; i++)
    {
        InitPin(&pConfig[i]);
    }
}

void DimmableLedHandler_SetOnOff(uint8_t channel, bool onOff)
{
    DimLed_t* pDimLed = GetDimLed(channel, false);

    qDrvLED_Set(&ledInst, pDimLed->pPinConfig->channel, onOff);
    pDimLed->onOff = onOff;
}

void DimmableLedHandler_SetBrightness(uint8_t channel, uint8_t pwm, bool fade)
{
    DimLed_t* pDimLed = GetDimLed(channel, false);

    qDrvLED_SetBrightness(&ledInst, pDimLed->pPinConfig->channel, pwm, fade);
    pDimLed->pwm = pwm;
    pDimLed->fade = fade;
}
