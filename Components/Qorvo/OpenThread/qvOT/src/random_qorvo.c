/*
 * Copyright (c) 2017, 2020, Qorvo Inc
 *
 * random_qorvo.c
 *   This file contains the implementation of the qorvo random api for openthread.
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

#include "gpAssert.h"
#include "gpRandom.h"
#include "random_qorvo.h"
#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "esecure.h"
#endif

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void qorvoRandomInit(void)
{
}

void qorvoRandomGet(uint8_t* aOutput, uint8_t aOutputLength)
{
    // Since the higher layers assert when calling this function does not
    // return OT_ERROR_NONE, we can safely assert here as well.
    if(NULL == aOutput || 0 == aOutputLength)
    {
        GP_ASSERT_SYSTEM(false);
    }
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    uint32_t success = esec_get_random_bytes(aOutput, aOutputLength);
    if(success != ESEC_OKAY)
    {
        GP_ASSERT_SYSTEM(false);
    }
#else
    gpRandom_GetFromDRBG((UInt8)aOutputLength, (UInt8*)aOutput);
#endif
}
