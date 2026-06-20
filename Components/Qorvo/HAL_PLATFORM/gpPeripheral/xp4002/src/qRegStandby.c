/*
 * Copyright (c) 2024, Qorvo Inc
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

#include "qRegStandby.h"
#include "esec_qorvo.h"

/* Static check for masks that are the last of RESET0 and the first od RESET1 registers. */
Q_STATIC_ASSERT(qRegStandby_ResetFuncPeripUart0 == 31);
Q_STATIC_ASSERT(qRegStandby_ResetFuncPeripUart1 == 32);

qResult_t qRegStandby_ResetFuncEnableSet(qRegStandby_ResetFunc_t resetFunc, Bool enable)
{
    UInt32 resetIndex = (UInt32)resetFunc / 32UL;
    UInt32 mask = 1UL << ((UInt32)resetFunc % 32UL);

    UInt32 resetValue = resetIndex ? GP_WB_READ_STANDBY_RESET_1() : GP_WB_READ_STANDBY_RESET_0();

    resetValue &= ~(mask);
    if(enable)
    {
        resetValue |= mask;
    }

    (void)esec_standby_reset_write(resetIndex, resetValue);

    return Q_OK;
}

qResult_t qRegStandby_WakeupPinsConfigure(UInt32 wakeupPinModes0, UInt32 wakeupPinModes1)
{
    (void)esec_config_gpio_wakeup(wakeupPinModes0, wakeupPinModes1);

    return Q_OK;
}
