/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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

#ifndef _GPHAL_KX_RT_H_
#define _GPHAL_KX_RT_H_

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#include "global.h"
#include "gpHal_kx_defs.h"
#include "gpHal_kx_gpm.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_HAL_RT_PROCESSOR_ALIGNMENT_BYTE_SIZE     4

#define GP_HAL_GET_RT_PROCESSING_DELAY()    GP_WB_READ_RT_EV_PROCESSING_DELAY()
#define GP_HAL_GET_RT_CLEANUP_TIME()        GP_WB_READ_RT_EV_CLEANUP_TIME()

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

// Address conversion macro's
#define TO_RT_ADDR(address)             gpHal_RtAddressToOffset(address)
#define FROM_RT_ADDR(address)           ak_rap_rt_from_ptr(address)

#ifndef GP_COMP_CHIPEMU
#define FROM_RT_ADDR_SYSRAM(address) (GP_MM_SYSRAM_START + address)
#else
#define FROM_RT_ADDR_SYSRAM(address)    (GP_MM_AKUMA_RAM_LINEAR_START + address)
#endif // GP_COMP_CHIPEMU

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef PACKED_PRE struct  {
    UInt16   pCis;      //!< Pointer to the CIS
    UInt16   pCig;      //!< Pointer to the CIG
} PACKED_POST ble_mgr_start_cis_t;

typedef PACKED_PRE struct  {
    UInt32   exec_time;  //!< transmission time of the PBM
    UInt16   optsOffset; //!< PBM options offset
} PACKED_POST zb_mgr_zb_timed_tx_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Function Prototypes
 *****************************************************************************/

UIntPtr gpHal_GetCurrentRxDataPtr(void);
UInt16 gpHal_RtAddressToOffset(UInt32 address);
UInt32 gpHal_RtRciPop(UInt32 queue_ptr);
Bool gpHal_RtAddFrPtr(UInt32 fr_ptr);

#endif //_GPHAL_KX_RT_H_
