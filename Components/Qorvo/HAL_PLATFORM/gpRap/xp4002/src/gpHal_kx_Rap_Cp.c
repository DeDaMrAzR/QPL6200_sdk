/*
 * Copyright (c) 2024, Qorvo Inc
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


void rap_cp_ctrl_enable(UInt32 id)
{
    UInt32 cp_users;
    rap_h_log_debug("rap_cp_ctrl_enable id = %ld", id);
    rap_h_assert_sys(id < 3);
    rap_h_atomic_on();
    GP_WB_WRITE_BIST_CMP_PUP(1);
    rap_h_wait_us(10);
    GP_WB_WRITE_BIST_CP_CTRL_ENABLE_HW_CTRL(1);
    cp_users = GP_WB_READ_CAL_RAM_CP_USERS();
    cp_users = cp_users | (1 << id);
    GP_WB_WRITE_CAL_RAM_CP_USERS(cp_users);
    rap_h_atomic_off();
}

void rap_cp_ctrl_disable(UInt32 id)
{
    UInt32 cp_users;
    rap_h_log_debug("rap_cp_ctrl_disable id = %ld", id);
    rap_h_assert_sys(id < 3);
    rap_h_atomic_on();
    cp_users = GP_WB_READ_CAL_RAM_CP_USERS();
    cp_users = cp_users & (~(1 << id));
    GP_WB_WRITE_CAL_RAM_CP_USERS(cp_users);
    if (cp_users == 0)
    {
        GP_WB_WRITE_BIST_CP_CTRL_ENABLE_HW_CTRL(0);
        GP_WB_WRITE_BIST_CMP_PUP(0);
        GP_WB_WRITE_BIST_CP_LDO_BLEED(0);
    }
    rap_h_atomic_off();
}

