/*
 * Copyright (c) 2023, Qorvo Inc
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


#define RAP_TX_PA_LUT_OFFSET_20DBM (-10)

static const UInt8 ak_rap_tx_pa_lut_20dbm[] = {
     ((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 4 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 4 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 5)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 6)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 6)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 7)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 8)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 9)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 10)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 11 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 12 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 14 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 16 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 18)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 20)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 22)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 25)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 28)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 31)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 35)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 20)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 22)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 25)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 28)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 32)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 35)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 40)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 45)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 51)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 57)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 63)
};

