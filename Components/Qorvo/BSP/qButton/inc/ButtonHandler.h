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

/** @file "ButtonHandler.h"
 *
 *  Declarations of all GPIOs
 */

#ifndef _BUTTON_HANDLER_H_
#define _BUTTON_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "global.h"

typedef struct {
    uint8_t gpio;
} ButtonConfig_t;

typedef enum { BUTTON_LOW, BUTTON_HIGH } ButtonActiveLevel_t;

/** @brief Callback type for button press callback */
typedef void (*ButtonHandler_Callback_t)(uint8_t btnIdx, bool btnPressed);

void ButtonHandler_Init(const ButtonConfig_t* pButtons, const uint8_t count, ButtonActiveLevel_t level,
                        ButtonHandler_Callback_t pCallback);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _BUTTON_HANDLER_H_
