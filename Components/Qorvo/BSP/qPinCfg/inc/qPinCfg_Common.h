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

/** @file "qPinCfg_Common.h"
 *
 *  Declarations of qPinCfg common features
 */

#ifndef _QPINCFG_COMMON_H__
#define _QPINCFG_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "qDrvIOB.h"
#include "qDrvGPIO.h"

/*****************************************************************************
 *                    Board File Override
 *****************************************************************************/
#define _QPINCFG_BOARD QPINCFG_BOARD_FILENAME

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define QPINCFG_GPIO_LIST(...)                                                                                         \
    {                                                                                                                  \
        __VA_ARGS__                                                                                                    \
    }

#define QPINCFG_ALT_LIST(...)                                                                                          \
    {                                                                                                                  \
        __VA_ARGS__                                                                                                    \
    }

#define GPIO_AS_ALT(gpio, channel, alternate)                                                                          \
    {                                                                                                                  \
        {gpio, Q_DRV_IOB_ALT_GET(gpio, alternate, channel)}, channel                                                   \
    }

#define _LED_CHANNEL_GET(gpio) Q_CONCAT(_LED_CHANNEL_ON_GPIO_, gpio)
#define GPIO_AS_DLED(gpio)     GPIO_AS_ALT(gpio, _LED_CHANNEL_GET(gpio), LED_LED)

#define _PWMXL_CHANNEL_GET(gpio) Q_CONCAT(_PWMXL_CHANNEL_ON_GPIO_, gpio)
#define GPIO_AS_PWMXL(gpio)      GPIO_AS_ALT(gpio, _PWMXL_CHANNEL_GET(gpio), PWMXL_PWMXL)

typedef struct {
    qDrvIOB_PinAlt_t pinAlt;
    uint8_t channel;
} qPinAltMapper_t;

qResult_t qPinCfg_Init(qDrvGPIO_CbInput_t pCallback);

/*****************************************************************************
 *                    Header inclusion
 *****************************************************************************/
#include _QPINCFG_BOARD
#include "qPinAltMapper.h"

#ifdef __cplusplus
}
#endif

#endif // _QPINCFG_COMMON_H__
