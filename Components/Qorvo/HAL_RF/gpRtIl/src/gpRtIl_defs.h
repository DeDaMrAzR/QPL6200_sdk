/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
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

// #define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_RTIL

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpRtIl.h"
#include "gpHal.h"
#include "gpHal_kx_Rt.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

// How many descriptors we can queue simultaneously on a CIS
#define RTIL_CIS_TX_DESCRIPTORS_LIST_SIZE     12
#define RTIL_CIS_RX_DESCRIPTORS_LIST_SIZE     8

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

// Use this macro to read data from an RT (16 bit address) and return it as a 32 bit address
#define RTIL_READ_INDIRECT_RT_TABLE_ADDRESS(rt_base_address)    (FROM_RT_ADDR(GP_WB_READ_U16(FROM_RT_ADDR(rt_base_address))))

// To keep fileconsistency happy :(
#define GP_WB_SPECIAL_READ_U32(address)         GP_WB_READ_U32(address)

#define RTIL_QUEUE_CIRC_INC(idx, wrap)  (((idx) == (wrap) - 1) ? 0 : ((idx) + 1))

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
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

void gpRtIl_BleInit(gpHal_BleCallbacks_t* pCallbacks);
void RtIl_BleChannelMapInit(void);
void gpRtIl_BleIsoDataInit(void);
void gpRtIl_BleIsoDataReset(void);
gpHal_Result_t RtIl_BleEventStart(UInt8 eventNr, UInt32 timeStamp);
gpHal_Result_t RtIl_BleEventStop(UInt8 eventId);

void RtIl_BleBigInit(void);
void RtIl_BleCigInit(gpHal_BleCallbacks_t* pCallbacks);




gpRtIl_Result_t gpRtIl_PreparePdLoh(gpPd_Loh_t* pPdLoh);
