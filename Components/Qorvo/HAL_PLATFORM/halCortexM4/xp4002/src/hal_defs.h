/*
 * Copyright (c) 2022, Qorvo Inc
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

#ifndef _HAL_DEFS_H_
#define _HAL_DEFS_H_

#include "global.h"
#include "gpRap_kx.h"
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define HAL_SP_STACK_END_ADDRESS   IVT_GET_STACK_POINTER()
#define HAL_SP_RESERVED_SIZE 320 // reserved for the reset_handler stack
#define HAL_SP_BACKUP_SIZE   192  // bottom of the application stack; to be backed-up on sleep
#define HAL_SP_BACKUP_MARGIN 0x20 // margin

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

extern volatile Bool hal_PolledInterruptPending;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static INLINE void hal_SetUnpolledInterruptBeforeSleep(void)
{
    hal_PolledInterruptPending = true;
}

static INLINE void hal_ClrUnpolledInterruptBeforeSleep(void)
{
    hal_PolledInterruptPending = false;
}

static INLINE Bool hal_HasUnpolledInterruptBeforeSleep(void)
{
    return hal_PolledInterruptPending;
}

ALWAYS_INLINE void hal_reset_always(void)
{
    // Always clear ready to go to sleep
    GP_WB_WRITE_APPUC_RTGTS_SW(0);

    // Needed to communicate with secuc MB
    GP_WB_WRITE_ESEC_MB_CONFIG_RXINTEN(1);
    GP_WB_WRITE_ESEC_MB_CONFIG_TXINTEN(1);
    GP_WB_WRITE_INT_CTRL_MASK_APPUC_SECURE_ELEMENT_MAILBOX_RX_INTERRUPT(1);
}

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

// hal_UART.c
void hal_UartBeforeSleep(void);
void hal_UartAfterSleep(void);

// hal_wait_xxx.S
void hal_wait_loop(UInt32 loops);
void hal_init(void);
void hal_InitSleep(void);
void hal_sleep(void);
void hal_go_to_sleep(void);
void hal_resume(void);
void hal_SleepIncrementCount(void);
#endif /*__HAL_DEFS_H__*/
