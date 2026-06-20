/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_MISC.c
 *
 * This file contains miscellaneous functions
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

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_MISC.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

#ifndef GP_COMP_CHIPEMU
#include "esecure.h"
#endif // ndef GP_COMP_CHIPEMU

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  RANDOM GENERATION FUNCTIONS
//-------------------------------------------------------------------------------------------------------

// Recommendation to only use this function for seeding an RBG (such as a DRBG or LCG)
// In case you just need a random number (e.g. for a random delay), use gpRandom_GetNewSequence() or gpRandom_GenerateLargeRandom()
UInt8 gpHal_GetRandomSeed(void)
{
    UInt8 value = 0;
#ifndef GP_COMP_CHIPEMU
    UInt32 status;

    status = esec_get_random_bytes((uint8_t*)&value, sizeof(value));
    GP_ASSERT_DEV_INT(status == ESEC_OKAY);
#endif
    return value;
}
