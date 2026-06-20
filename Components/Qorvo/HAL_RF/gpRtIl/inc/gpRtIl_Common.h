/*
 * Copyright (c) 2017, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by\
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

#ifndef _GPRTIL_COMMON_H_
#define _GPRTIL_COMMON_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "sw_reg_types.h"
#else
#ifdef GP_COMP_UNIT_TEST
#include "gpRtIl_Layouts.h"
#endif
#endif

#include "qv_rt_nrt_common.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CIGS
#define GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CIGS 2
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CIGS

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CISES
#define GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CISES 5
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CISES

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef enum {
    GPRTIL_RESULT_SUCCESS               = 0x00,
    GPRTIL_RESULT_BUSY                  = 0x01,
    GPRTIL_RESULT_INVALID_PARAMETER     = 0x02,
} gpRtIl_Result_t;

/*****************************************************************************
 *                    Component Function Definitions
 *****************************************************************************/

#endif //_GPRTIL_COMMON_H_
