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

/** @file "ColorLedHandler.h"
 *
 *  Declarations of ColorLedHandler
 */

#ifndef _COLORLEDHANDLER_H_
#define _COLORLEDHANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "global.h"
#include "qPinCfg_Common.h"

void ColorLedHandler_Init(const qPinAltMapper_t* pConfig, uint8_t count);
void ColorLedHandler_SetOnOff(bool onoff);
void ColorLedHandler_SetColorLevel(uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _COLORLEDHANDLER_H_
