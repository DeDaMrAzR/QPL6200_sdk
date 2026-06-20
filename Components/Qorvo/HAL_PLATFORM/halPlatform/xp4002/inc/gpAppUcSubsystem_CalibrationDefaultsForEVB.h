/*
 * Copyright (c) 2024, Qorvo Inc
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
 */

/** @file "gpAppUcSubsystem.h"
 *
 *
 * @defgroup INIT INIT primitives
 * This module groups the primitives for initialisation.
 *
 * @defgroup SET SET primitives
 * This module groups the primitives for setting the different calibration configs.
 *
 */

#ifndef _GPAPPUCSUBSYSTEMDEFAULTSFOREVB_H_
#define _GPAPPUCSUBSYSTEMDEFAULTSFOREVB_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpAppUcSubsystem_Calibrations.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

gpAppUcSubsystem_CalRamDcdc_t calRamDcdcData_EVB = {
    .y2_skip_ehps = 17,
    .y2_skip_emps = 5,
    .y2_min_ehps = 17,
    .y2_min_emps = 5,
    .y1_skip = 17,
    .y1_min = 17,
};

gpAppUcSubsystem_CalRamXo32m_t calRamXo32mData_EVB = {
    .cload_comp_0 = 15,
    .cload_comp_1 = 17,
    .cload_comp_2 = 19,
    .cload_comp_3 = 17,
    .cload_comp_4 = 14,
    .cload_comp_5 = 8,
    .cload_comp_6 = 2,
    .cload_comp_7 = -4,
    .cload_comp_8 = -9,
    .cload_comp_9 = -10,
    .cload_comp_10 = -10,
    .cload_comp_11 = -10,
    .cload_comp_12 = -10,
    .cload_comp_13 = -10,
    .cload_comp_14 = -9,
    .cload_comp_15 = -3,
    .cload_comp_16 = 12,
};

#endif //_GPAPPUCSUBSYSTEMDEFAULTSFOREVB_H_
