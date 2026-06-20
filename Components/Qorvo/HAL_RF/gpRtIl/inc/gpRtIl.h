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

#ifndef _GPRTIL_H_
#define _GPRTIL_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpHal_Ble.h"

#include "gpRtIl_Common.h"





/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_ISO_TX_DESCRIPTORS
#define GP_DIVERSITY_BLE_MAX_NR_OF_ISO_TX_DESCRIPTORS 30
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_ISO_TX_DESCRIPTORS

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_ISO_RX_DESCRIPTORS
#define GP_DIVERSITY_BLE_MAX_NR_OF_ISO_RX_DESCRIPTORS 30
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_ISO_RX_DESCRIPTORS

#define GP_RTIL_CIS_EC_INVALID  0xFFFFFFFF

#define GP_RTIL_BLE_CHANNEL_MAP_HANDLE_INVALID          0xFF

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Component Function Definitions
 *****************************************************************************/

void gpRtIl_Init(gpHal_BleCallbacks_t* pCallbacks);
void gpRtIl_Reset(void);

// BLE part
void gpRtIl_BleWriteChannelMap(gpHal_BleChannelMapHandle_t handle, gpHal_ChannelMap_t* pChanMap);
void gpRtIl_BleReadChannelMap(gpHal_BleChannelMapHandle_t handle, UInt8* pChanMap);
UInt16 gpRtIl_BleChannelMapHandleToOffset(gpHal_BleChannelMapHandle_t handle);
void gpRtIl_BleSwitchChannelMap(gpHal_BleChannelMapHandle_t* pHandleCurrent, gpHal_BleChannelMapHandle_t* pHandleNew, UInt8* pCompressedPtr);

void gpRtIl_GetPdFromCompressedRxFormatPtr(UInt16 formatPtr, gpPd_Loh_t* pPdLoh);

#endif //_GPRTIL_H_
