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

/** @file "DimmableLedHandler.h"
 *
 *  Declarations of DimmableLedHandler
 */

#ifndef _DIMMABLELEDHANDLER_H_
#define _DIMMABLELEDHANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "global.h"
#include "qPinCfg_Common.h"

void DimmableLedHandler_Init(qPinAltMapper_t* pConfig, uint8_t count);
void DimmableLedHandler_SetOnOff(uint8_t channel, bool onOff);
void DimmableLedHandler_SetBrightness(uint8_t channel, uint8_t pwm, bool fade);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _DIMMABLELEDHANDLER_H_
