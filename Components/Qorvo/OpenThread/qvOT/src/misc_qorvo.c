/*
 * Copyright (c) 2017, 2020-2021, Qorvo Inc
 *
 * misc_qorvo.c
 *   This file contains the implementation of the qorvo misc api for openthread.
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVOT

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpReset.h"
#include "gpSched.h"
#include "gpLog.h"
#include "gpAssert.h"

#ifdef GP_COMP_GPHAL
#ifdef GP_DIVERSITY_LOADED_USER_LICENSE
#include "hal_user_license.h"
#elif defined(GP_DIVERSITY_APPUC_FW_HEADER)
#include "q_firmware_header.h"
#else
#include "hal_user_license.h"
#endif // GP_DIVERSITY_LOADED_USER_LICENSE
#endif // GP_COMP_GPHAL

#if defined(GP_DIVERSITY_JUMPTABLES)
#include "gpJumpTables_DataTable.h"
#endif // GP_DIVERSITY_JUMPTABLES


#include "misc_qorvo.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
/** Flag to set when reset has fired. Bool will be used externally in the RCP case. */
bool gPlatformPseudoResetWasRequested;

static void delayedReset(void)
{
    gpReset_ResetSystem();
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void qorvoPlatReset(void)
{
    // short delay for the reset to get the ACKs out.
    gpSched_ScheduleEvent(0, delayedReset);
}

void qorvoPlatAssertFail(const char* aFilename, int aLineNumber)
{
    NOT_USED(aFilename);
    NOT_USED(aLineNumber);
    GP_LOG_SYSTEM_PRINTF("ASSERT %s:%d", 0, aFilename, aLineNumber);
    GP_ASSERT_DEV_EXT(false);
}

void qorvoForceLinker(void)
{
    /* prevent the linker from throwing away the user license */
#ifdef GP_COMP_GPHAL
#ifdef GP_DIVERSITY_LOADED_USER_LICENSE
    hal_get_loaded_user_license();
#elif defined(GP_DIVERSITY_APPUC_FW_HEADER)
    hal_get_appuc_header();
#else
    hal_get_user_license();
#endif // GP_DIVERSITY_LOADED_USER_LICENSE
#endif // GP_COMP_GPHAL
#if defined(GP_DIVERSITY_JUMPTABLES)
    const void* forceDataJumpTableInclude = &JumpTables_DataTable;
    NOT_USED(forceDataJumpTableInclude);
#endif // GP_DIVERSITY_JUMPTABLES
}

