/*
 * Copyright (c) 2022, Qorvo Inc
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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

#include "hal.h"
#include "hal_interrupts.h"

#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
void gpHal_ESInterrupt(void);
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data
 *****************************************************************************/
#if !defined(GP_DIVERSITY_FREERTOS) 
volatile Bool hal_SysTickInterruptPending = false;
#endif // GP_DIVERSITY_FREERTOS || GP_FREERTOS_DIVERSITY_USE_EXT_TIMER

/*****************************************************************************
 *                    Interrupt handler
 *****************************************************************************/

/*****************************************************************************
 *                    Private Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

Bool hal_PlatformInterruptIsPending(void)
{
    Bool pending = false;

#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
    pending |= GP_WB_READ_INT_CTRL_UNMASKED_APPUC_ES_INTERRUPT();
#endif //defined(GP_COMP_GPHAL_ES_ABS_EVENT)

#if !defined(GP_DIVERSITY_FREERTOS) 
    pending |= hal_HasSysTickInterruptPending();
#endif // GP_DIVERSITY_FREERTOS || GP_FREERTOS_DIVERSITY_USE_EXT_TIMER

    return pending;
}

void hal_HandlePolledPlatformInterrupt(void)
{
#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
        while (GP_WB_READ_INT_CTRL_UNMASKED_APPUC_ES_INTERRUPT())
        {
            gpHal_ESInterrupt();
        }
#endif //defined(GP_COMP_GPHAL_ES_ABS_EVENT)
}

void hal_PlatformInterruptsRestoreMasksAfterHandling(void)
{
#if defined(GP_COMP_GPHAL_ES_ABS_EVENT)
        GP_WB_WRITE_INT_CTRL_MASK_APPUC_ES_INTERRUPT(1);
#endif
}

#if !defined(GP_DIVERSITY_FREERTOS) 
void systick_handler_impl(void)
{
    hal_SetSysTickInterruptPending();
}
#endif // GP_DIVERSITY_FREERTOS || GP_FREERTOS_DIVERSITY_USE_EXT_TIMER
