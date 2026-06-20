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

/** @file "AppButtons.h"
 */

#ifndef _APPBUTTONS_H_
#define _APPBUTTONS_H_

#include <stdbool.h>
#include <stdint.h>

#define APP_BUTTON_NO_MULTIFUNC 0xFF

typedef struct {
    uint8_t Index;
    enum {
        kButtonState_Released = 0,
        kButtonState_Pressed = 1,
        kButtonState_Held = 2,
        kButtonState_Invalid,
    } State;
    uint8_t HeldSec;
} ButtonEvent_t;

class AppButtons {
    public:
    void Init();
    void RegisterMultiFunc(uint8_t gpio);

    private:
    friend AppButtons& GetAppButtons(void);
    static void EventHandler(uint8_t btnIdx, bool btnPressed);
    static void FunctionKeyTimeout(void);

    static AppButtons sAppButtons;
    uint8_t multifunc_button;
    uint8_t functionKeyHeldSec;
};

inline AppButtons& GetAppButtons(void)
{
    return AppButtons::sAppButtons;
}

#endif // _APPBUTTONS_H_
