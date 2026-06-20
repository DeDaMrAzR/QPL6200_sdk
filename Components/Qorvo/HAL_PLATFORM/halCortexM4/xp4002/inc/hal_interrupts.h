/*
 * Copyright (c) 2017, Qorvo Inc
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


#ifndef _HAL_PLATFORMINTERRUPTS_H_
#define _HAL_PLATFORMINTERRUPTS_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#if !defined(GP_DIVERSITY_FREERTOS) 
extern volatile Bool hal_SysTickInterruptPending;
#endif // GP_DIVERSITY_FREERTOS || GP_FREERTOS_DIVERSITY_USE_EXT_TIMER

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Check if an interrupts is pending which needs to be handled in the main loop (i.e. a polled interrupt)
 */
Bool hal_PlatformInterruptIsPending(void);

/**
 * @brief calls the relevant functions to handled "polled" platform interrupts
 */
void hal_HandlePolledPlatformInterrupt(void);

/**
 * @brief Restores the mask, such that polled interrupts can be received again after they are handled
 */
void hal_PlatformInterruptsRestoreMasksAfterHandling(void);

#if !defined(GP_DIVERSITY_FREERTOS) 
/** @brief Sets the systick interrupt pending flag */
static INLINE void hal_SetSysTickInterruptPending(void)
{
    hal_SysTickInterruptPending = true;
}

/** @brief Clears the systick interrupt pending flag */
static INLINE void hal_ClrSysTickInterruptPending(void)
{
    hal_SysTickInterruptPending = false;
}

/** @brief Gets the systick interrupt pending flag */
static INLINE Bool hal_HasSysTickInterruptPending(void)
{
    return hal_SysTickInterruptPending;
}
#endif // GP_DIVERSITY_FREERTOS || GP_FREERTOS_DIVERSITY_USE_EXT_TIMER

#ifdef __cplusplus
}
#endif

#endif // _HAL_PLATFORMINTERRUPTS_H_
