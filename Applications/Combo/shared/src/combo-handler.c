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

/** @file "combo-handler.c"
 *
 * Implementation of Combo handler for stack switching
 */

#include "gpHal.h"
#include "gpLog.h"
#include "gpReset.h"
#include "gpUpgrade.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#if defined(GP_DIVERSITY_DUALBOOT_JUMP_ADDRESS)
const uint32_t bootAddress = ((uint32_t)GP_DIVERSITY_DUALBOOT_JUMP_ADDRESS);
#else
#error "Address for second application is not defined."
#endif

gpUpgrade_Status_t ComboHandler_SwitchStack(void)
{
    gpUpgrade_Status_t status;

    status = gpUpgrade_SetBootAddress(bootAddress);

    if(status == gpUpgrade_StatusSuccess)
    {
        // Disable sleep to avoid waking up from other app
        hal_SleepSetGotoSleepEnable(false);
    }

    return status;
}
