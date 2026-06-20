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


#define RAP_TX_PA_LUT_OFFSET_10DBM (-13)

static const UInt8 ak_rap_tx_pa_lut_10dbm[] = {
     ((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 9 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 10 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 11 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 13 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 14 )
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 16)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 18)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 20)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 23)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 26)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 29)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 33)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 37)
    ,((AK_RAP_TX_PA_MODE_THERMO_PLUS<<6) | 42)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 23)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 26)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 29)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 33)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 37)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 42)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 48)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 54)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 61)
    ,((AK_RAP_TX_PA_MODE_THERMO     <<6) | 63)
};

