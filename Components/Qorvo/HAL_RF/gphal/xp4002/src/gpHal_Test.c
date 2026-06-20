/*
 * Copyright (c) 2017-2023, Qorvo Inc
 *
 * gphal_Test.c
 *
 *  The file gpHal_Test.c contains code that should only be activated in Qorvo labs during development cycles.
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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal.h"
#include "gpHal_DEFS.h"

#include "gpHal_HW.h"          //Containing all uC dependent implementations
#include "gpHal_reg.h"

#include "gpLog.h"
#include "gpHal_Test.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
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

void gpHal_EnableAgc(Bool enable)
{
    if (enable)
    {
        GP_WB_WRITE_AK_RX_AGC_GAIN_OVERRULE_ENA(0);
    }
    else
    {
        GP_WB_WRITE_AK_RX_AGC_GAIN_OVERRULE_ENA(1);
    }
    // Registers should still contain these values since reset:
    // GP_WB_WRITE_AK_RX_RF_GAIN_OVERRULE(7);
    // GP_WB_WRITE_AK_RX_BB_GAIN_OVERRULE(4);
}

Bool gpHal_isAgcEnabled(void)
{
    return(GP_WB_READ_AK_RX_AGC_GAIN_OVERRULE_ENA() != 1);
}
