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

//#define GP_LOCAL_LOG

#ifdef GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_ROM_CODE
#endif //def GP_DIVERSITY_JUMPTABLES

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLELLCPPROCEDURES

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpBle.h"
#include "gpBleConfig.h"
#include "gpBleLlcpProcedures.h"
#include "gpBleLlcpProcedures_defs.h"
#include "gpBle_defs.h"
#include "gpLog.h"
#include "gpSched.h"

#if defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
#include "gpBleLlcpProcedures_CodeJumpTableFlash_Defs_defs.h"
#include "gpBleLlcpProcedures_CodeJumpTableRom_Defs_defs.h"
#endif // defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void Ble_LlcpTerminationGetCtrData(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpBleLlcp_Opcode_t* pOpcode, UInt8* ctrDataLength, UInt8* pCtrData)
{
    GP_ASSERT_DEV_INT(pProcedure->localInit);

    GP_LOG_PRINTF("Termination get ctrData %x",0, *pOpcode);

    switch(*pOpcode)
    {
        case gpBleLlcp_OpcodeTerminateInd:
        {
            UInt8* pReason = (UInt8*)pProcedure->pData;

             MEMCPY(&pCtrData[0], pReason, 1);
            *ctrDataLength = 1;
            break;
        }
        default:
        {
            // Should not happen
            GP_ASSERT_DEV_INT(false);
            break;
        }
    }
}

Ble_LlcpFrameworkAction_t Ble_LlcpTerminationStoreCtrData(Ble_LlcpLinkContext_t* pContext, Ble_LlcpProcedureContext_t* pProcedure, gpPd_Loh_t* pPdLoh, gpBleLlcp_Opcode_t opcode)
{
    GP_ASSERT_DEV_INT(!pProcedure->localInit);

    GP_LOG_PRINTF("Termination store ctrData %x",0, opcode);

    switch(opcode)
    {
        case gpBleLlcp_OpcodeTerminateInd:
        {
            gpBleLlcpProcedureTerminationData_t* pTerm = (gpBleLlcpProcedureTerminationData_t*)pProcedure->pData;
            pTerm->reason = gpPd_ReadByte(pPdLoh->handle, pPdLoh->offset);
            break;
        }
        default:
        {
            // Should not happen
            GP_ASSERT_DEV_INT(false);
            break;
        }
    }

    return Ble_LlcpFrameworkActionContinue;
}


