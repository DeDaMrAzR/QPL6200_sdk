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

UInt8 ak_rap_rtm_get_trc_warmup_time(UInt8 trc_state)
{
    switch (trc_state)
    {
        case GP_WB_ENUM_AK_TRC_STATE_CAL:
        {
            return 12;
            break;
        }
        case GP_WB_ENUM_AK_TRC_STATE_TX:
        {
            return 22;
            break;
        }
        case GP_WB_ENUM_AK_TRC_STATE_RX_WD:
        case GP_WB_ENUM_AK_TRC_STATE_ED:
        case GP_WB_ENUM_AK_TRC_STATE_CCA:
        {
            return 16;
            break;
        }
        case GP_WB_ENUM_AK_TRC_STATE_OFF:
        {
            return 2;
            break;
        }
        case GP_WB_ENUM_AK_TRC_STATE_IDLE:
        case GP_WB_ENUM_AK_TRC_STATE_RX_ON_IDLE:
        {
            break;
        }
        default:
        {
            break;
        }
    }
    return 0;
}

void ak_rap_rtm_trigger_statereq(UInt8 trc_state)
{
    UInt8 warmup_time;
    warmup_time = ak_rap_rtm_get_trc_warmup_time(trc_state);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_WARMUP_TIME(warmup_time);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_STATE(trc_state);
    GP_WB_AK_RTM_TRC_STATEREQ();
}

