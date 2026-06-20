/*
 * Copyright (c) 2022, Qorvo Inc
 *
 * !!! This is a generated file. !!!
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
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

#include "gpHal.h"
#if defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)
#include "hal.h"
#endif //defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

//wrappers/stubs for RAP code
#define abs(a)              (((a) < 0) ? -(a) : (a))

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*
 * The actual RAP methods.
 */

void ak_rap_rt_ipc_init(void)
{
    GP_WB_WRITE_AK_INT_CTRL_MASK_RTUC_IPCX2RT_INTERRUPT(1);
    GP_WB_WRITE_AK_INT_CTRL_MASK_IPCX2RT_CMD_INTERRUPT(1);
}

void ak_rap_rt_ipc_cmd_launch(UInt8 cmd, UInt32 args)
{
    GP_WB_AK_IPC_CLR_RT2X_CMD_PROCESSED_INTERRUPT();
    GP_WB_WRITE_AK_RT_CMD_COMMAND(cmd);
    GP_WB_WRITE_AK_RT_CMD_ARGS(args);
    GP_WB_WRITE_AK_RT_CMD_RESULT(0);
    GP_WB_AK_IPC_SET_X2RT_CMD_INTERRUPT();
}

UInt16 ak_rap_rt_ipc_cmd_join(void)
{
    UInt16 res;
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_IPC_UNMASKED_RT2X_CMD_PROCESSED_INTERRUPT() != 1), 1000000);
    GP_WB_AK_IPC_CLR_RT2X_CMD_PROCESSED_INTERRUPT();
    res = GP_WB_READ_AK_RT_CMD_RESULT();
    return res;
}

void ak_rap_rt_ipc_cmd(UInt8 cmd, UInt32 args)
{
    UInt16 res;
    ak_rap_rt_ipc_cmd_launch(cmd, args);
    res = ak_rap_rt_ipc_cmd_join();
    rap_h_assert_sys(res == cmd);
}

UInt16 ak_rap_rt_to_ptr(UInt32 ptr)
{
    if (ptr == 0)
    {
        return 0;
    }
    else
    {
        return (ptr-GP_MM_AK_RAM_LINEAR_START);
    }
}

UInt32 ak_rap_rt_from_ptr(UInt16 ptr)
{
    if (ptr == 0)
    {
        return 0;
    }
    else
    {
        return GP_MM_AK_RAM_LINEAR_START+ptr;
    }
}

