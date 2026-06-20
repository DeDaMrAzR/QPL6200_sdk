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


void ak_rap_sx_set_channel_launch(UInt8 idx, UInt8 sx_channel)
{
    UInt32 args;
    args = (sx_channel<<8) + idx;
    ak_rap_rt_ipc_cmd_launch(GP_WB_ENUM_AK_RT_CMD_TYPE_SX_CAL_RT, args);
}

void ak_rap_sx_set_channel_join(void)
{
    UInt16 result;
    result = ak_rap_rt_ipc_cmd_join();
    rap_h_assert_sys(result == GP_WB_ENUM_AK_RT_CMD_TYPE_SX_CAL_RT);
}

void ak_rap_sx_set_channel(UInt8 idx, UInt8 sx_channel)
{
    ak_rap_sx_set_channel_launch(idx, sx_channel);
    ak_rap_sx_set_channel_join();
}

void ak_rap_sx_cal_nrt(void)
{
    ak_rap_rt_ipc_cmd(GP_WB_ENUM_AK_RT_CMD_TYPE_SX_CAL_NRT, 0);
}

UInt32 ak_rap_sx_measure_freq_khz(UInt8 duration)
{
    UInt32 freq;
    UInt32 rf_counter;
    rap_h_assert_sys(duration>=2);
    rap_h_assert_sys(duration<=12);
    GP_WB_WRITE_AK_SX_RFCOUNTER_PUP(1);
    GP_WB_WRITE_AK_SX_RFCOUNTER_CLR(1);
    GP_WB_WRITE_AK_SX_RFCOUNTER_CLR(0);
    GP_WB_WRITE_AK_SX_RFCOUNTER_TIME(duration);
    GP_WB_AK_SX_RFCOUNTER_START();
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_SX_RFCOUNTER_DONE() != 1), 1024);
    rf_counter = GP_WB_READ_AK_SX_RFCOUNTER();
    freq = (rf_counter * 8000) >> (duration-2);
    return freq;
}

