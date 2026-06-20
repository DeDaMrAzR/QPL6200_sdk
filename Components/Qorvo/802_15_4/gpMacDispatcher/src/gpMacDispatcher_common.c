/*
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacDispatcher_common.c
 *   Parts of MAC Dispatcher that are common between multi-stack and single-stack implementations.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACDISPATCHER
//#define GP_LOCAL_LOG

#include "gpMacCore.h"
#include "gpMacDispatcher.h"
#include "gpMacDispatcher_def.h"
#include "gpLog.h"
#include "gpAssert.h"

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Internal Non-Static Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_TX_ANTENNA_DIVERSITY
// Called when initializing or resetting a networking stack.
void MacDispatcher_InitAutoTxAntennaToggling(gpMacCore_StackId_t stackId)
{
    GP_ASSERT_DEV_INT(stackId < GP_DIVERSITY_NR_OF_STACKS);
    if (stackId < GP_DIVERSITY_NR_OF_STACKS)
    {
        gpMacDispatcher_StackMapping[stackId].autoTxAntennaTogglingEnabled = false;
    }
}

// Called from gpMacCore_cbDataConfirm to switch TX antenna after TX failure.
void MacDispatcher_HandleAutoTxAntennaToggling(gpMacDispatcher_Result_t status, gpMacCore_StackId_t stackId)
{
    GP_ASSERT_DEV_INT(stackId < GP_DIVERSITY_NR_OF_STACKS);
    if ((status == gpMacDispatcher_ResultNoAck) || (status == gpMacDispatcher_ResultChannelAccessFailure))
    {
        if ((stackId < GP_DIVERSITY_NR_OF_STACKS) &&
            gpMacDispatcher_StackMapping[stackId].autoTxAntennaTogglingEnabled)
        {
            UInt8 txAntenna = gpMacCore_GetTxAntenna(stackId) == 0 ? 1 : 0; //toggle tx antenna
            GP_LOG_PRINTF("MacDisp: Switching to TX antenna %u", 0, txAntenna);
            gpMacCore_SetTxAntenna(txAntenna, stackId);
        }
    }
}
#endif //GP_DIVERSITY_TX_ANTENNA_DIVERSITY

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

#ifdef GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
void gpMacDispatcher_SetMinInterferenceLevels(Int8* pInterferenceLevels)
{
    gpMacCore_SetMinInterferenceLevels(pInterferenceLevels) ;
}
#endif //GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR

#ifdef GP_DIVERSITY_TX_ANTENNA_DIVERSITY
void gpMacDispatcher_SetAutoTxAntennaToggling(Bool enable, gpMacDispatcher_StackId_t stackId)
{
    GP_ASSERT_DEV_INT(stackId < GP_DIVERSITY_NR_OF_STACKS);
    if (stackId < GP_DIVERSITY_NR_OF_STACKS)
    {
        gpMacDispatcher_StackMapping[stackId].autoTxAntennaTogglingEnabled = enable;
    }
}

Bool gpMacDispatcher_GetAutoTxAntennaToggling(gpMacDispatcher_StackId_t stackId)
{
    GP_ASSERT_DEV_INT(stackId < GP_DIVERSITY_NR_OF_STACKS);
    if (stackId < GP_DIVERSITY_NR_OF_STACKS)
    {
        return gpMacDispatcher_StackMapping[stackId].autoTxAntennaTogglingEnabled;
    }
    else
    {
        return false;
    }
}
#endif //GP_DIVERSITY_TX_ANTENNA_DIVERSITY
