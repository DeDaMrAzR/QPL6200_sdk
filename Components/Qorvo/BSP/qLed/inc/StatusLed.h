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

/** @file "StatusLed.h"
 *
 *  Declarations of StatusLed
 */

#ifndef _STATUSLED_H_
#define _STATUSLED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "global.h"
#include <stdbool.h>

void StatusLed_Init(const uint8_t* gpio, uint8_t count, bool active);
void StatusLed_SetLed(uint8_t gpio, bool state);
void StatusLed_BlinkLed(uint8_t gpio, uint16_t onMs, uint16_t offMs);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _STATUSLED_H_
