/*
 * Copyright (c) 2012-2014, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacCore_Reset.c
 *   This file contains the implementation of the non beaconed MAC protocol.
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

// General includes
#include "gpPd.h"
#include "gpHal.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpLog.h"
#include "gpSched.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


gpMacCore_Result_t gpMacCore_Reset(Bool setDefaultPib, gpMacCore_StackId_t stackId)
{
    MacCore_StopRunningRequests(stackId);
    MacCore_SetDefaultStackValues(setDefaultPib, stackId);
    gpRxArbiter_ResetStack(stackId);
#ifdef GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
    if (DIVERSITY_INDIRECT_TRANSMISSION())
    {
        MacCore_IndTxBufferCleanUp(stackId);
    }
#endif //GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION


#ifdef GP_MACCORE_DIVERSITY_DIAGCNTRS
    if (DIVERSITY_DIAGCNTRS())
    {
        MacCore_ResetDiagCntr();
    }
#endif // GP_MACCORE_DIVERSITY_DIAGCNTRS

    //Enable PIO interrupt
    gpHal_EnablePrimitiveCallbackInterrupt(true);

#ifdef GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
    if (DIVERSITY_SCAN_ED_ORIGINATOR())
    {
        MacCore_ResetMinInterferenceLevels();
    }
#endif

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
#endif //GP_MACCORE_DIVERSITY_RAW_FRAMES

    return gpMacCore_ResultSuccess;
}

