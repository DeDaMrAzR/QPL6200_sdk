/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2021, Qorvo Inc
 *
 *   Hardware Abstraction Layer for ARM-based devices.
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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "hal.h"
#include "hal_defs.h"
#include "gpAssert.h"
#include "hal_SEMailbox.h"

/*****************************************************************************
 *                    CLK frequency
 *****************************************************************************/

void hal_SetMcuClockSpeed(UInt8 clockSpeed)
{
    if(clockSpeed > GP_WB_ENUM_CLOCK_SPEED_M192)
    {
        // Invalid clock speed
        GP_ASSERT_SYSTEM(false);
    }


    (void) hal_SEMailbox_SetAppucClkFreq(clockSpeed);
}

UInt8 hal_GetMcuClockSpeed(void)
{
    return GP_WB_READ_STANDBY_PRESCALE_UCCORE();
}
