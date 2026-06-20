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

/** @file "qPinCfg.c"
 *
 * Implementation of generic pin configurations
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include <stdbool.h>
#include "gpLog.h"
#include "qPinCfg.h"
#include "FreeRTOSConfig.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Variables Definitions
 *****************************************************************************/
static qDrvGPIO_t gpio;
static const qDrvGPIO_Config_t cfg = Q_DRV_GPIO_CONFIG_DEFAULT;
static qDrvGPIO_Callbacks_t gpioCallback;

#if defined(QPINCFG_UNUSED)
static const uint8_t unusedPins[] = QPINCFG_UNUSED;
#endif // QPINCFG_UNUSED

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
qResult_t qPinCfg_Init(qDrvGPIO_CbInput_t pCallback)
{
    qResult_t res;

    // Initialize the IOB module to configure the unused pins.
    (void)qDrvIOB_Init();

    gpioCallback.gpio = pCallback;

#if !defined(QPINCFG_EXTERNAL_INIT)
    res = qDrvGPIO_Init(&gpio, &cfg, &gpioCallback, NULL, Q_DRV_IRQ_PRIO_DEFAULT);
#else
    res = Q_OK;
#endif // !QPINCFG_EXTERNAL_INIT

#if defined(QPINCFG_UNUSED)
    for(uint8_t i = 0; i < Q_ARRAY_SIZE(unusedPins); i++)
    {
        if(qDrvIOB_SupportedPinCheck(unusedPins[i]))
        {
            GP_LOG_PRINTF("Unused pin: %d", 0, unusedPins[i]);
            res = qDrvIOB_ConfigUnusedSet(unusedPins[i]);
            if(res != Q_OK)
            {
                return res;
            }
        }
        else
        {
            GP_LOG_PRINTF("Unavailable pin: %d ...skipped", 0, unusedPins[i]);
        }
    }
#endif // QPINCFG_UNUSED

    return res;
}
