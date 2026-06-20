/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_DEFS.h
 *
 *  This file contains internal definitions of the GPHAL.
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

#ifndef _HAL_GP_BLE_DEFS_H_
#define _HAL_GP_BLE_DEFS_H_


/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#ifdef GP_COMP_GPHAL_BLE
#include "gpHal_kx_BLE.h"
#endif //GP_COMP_GPHAL_BLE

#include "gpHal_Ble.h"
#include "gpHal_DEFS.h"

#ifdef GP_COMP_CHIPEMU
#include "gpChipEmu.h"
#endif //GP_COMP_CHIPEMU

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

// Guard time related timings
#define T_SAFETY_MARGIN         (100 /* unit us */)
#define T_PROCESSING            (GP_WB_READ_RT_EV_PROCESSING_DELAY() + T_SAFETY_MARGIN)

#define T_CAL_DELAY              (GP_WB_READ_RT_T_CAL_DURATION())

#define LEN_MAX_ADV_IND                 (1 /*preamble*/ + 4 /*addr*/ + 2 /*header*/ +  37 /* max payload */      + 3 /*CRC*/)
#define LEN_SCAN_REQ                    (1 /*preamble*/ + 4 /*addr*/ + 2 /*header*/ +  12 /* fixed payload */    + 3 /*CRC*/)
#define LEN_MAX_SCAN_RSP                (1 /*preamble*/ + 4 /*addr*/ + 2 /*header*/ +  37 /* max payload */      + 3 /*CRC*/)
#define LEN_CON_REQ                     (1 /*preamble*/ + 4 /*addr*/ + 2 /*header*/ +  34 /* fixed payload */    + 3 /*CRC*/)
#define LEN_MIN_DATA                    (1 /*preamble*/ + 4 /*addr*/ + 2 /*header*/ +   0 /* data */             + 3 /*CRC*/)

/* max 1568us with 1120 for rx/tx */
#define T_ADV_GUARD_TIME(len_adv, len_scan_rsp) ((T_CAL_DELAY) + (T_OFF2TX_DELAY) + (T_FROM_LEN(len_adv)) + (T_IFS) + (T_FROM_LEN(LEN_CON_REQ)) + (T_IFS) + (T_FROM_LEN(len_scan_rsp)) + T_PROCESSING)


#define GPHAL_BLE_MAX_NR_OF_PHYS 1

#define GPHAL_BLE_CONN_ID_INVALID     0xFF

// Event Info memory
extern UInt8 gpHal_Ble_EventInfoMemory[];

#define GPHAL_BLE_TEST_INFO_SIZE                0x36

#define GPHAL_BLE_NR_OF_SERVICE_EVENTS    5


// Size of different info structures
#define GPHAL_BLE_SERVICE_EVENT_INFO_SIZE       0x38
#define GPHAL_BLE_CONN_EVENT_INFO_SIZE          0xA0
#define GPHAL_BLE_CHAN_MAP_INFO_SIZE            0x30



#define GP_HAL_BLE_EXT_INIT_VIRT_INFO_SIZE                   (0x06)
#define GP_HAL_BLE_EXT_INIT_INFO_SIZE                        (0x0E)


#define GP_HAL_BLE_EXT_SCAN_INFO_SIZE                        (0x02)




#ifdef GP_BSP_PHY_MAX_NR_OF_ANTENNAS
// keep the dflags checker happy - this board property is used in Components/Qorvo/BleController
#endif

// Tmp: replace with better construction once we have an idea what it should look like
#define GP_HAL_NEW_PTR_TO_PBM(ptr)          gpHal_findPbmHandle(FROM_RT_ADDR(ptr))

#define GP_HAL_INFO_PTR_INVALID             0xFFFF

/*****************************************************************************
*                   Functional Macro Definitions
*****************************************************************************/

/* Convert length to tx/rx speed */
#define T_FROM_LEN_AND_BW(len_B, bw_Mbs)    ( ((UInt16)len_B * 8) / (bw_Mbs==gpHal_BleTxPhy2Mb? 2 : 1) )
#define T_FROM_LEN(len_B)                   T_FROM_LEN_AND_BW(len_B,gpHal_BleTxPhy1Mb)

ALWAYS_INLINE UInt16 gpHal_Ble_Ram_Address_To_Start_Offset(UInt32 addr)
{
    UInt32 x = addr - GP_MM_AKUMA_RAM_LINEAR_START;
    return (UInt16)x;
}

#define GPHAL_BLE_RAM_ADDRESS_TO_START_OFFSET(addr)         gpHal_Ble_Ram_Address_To_Start_Offset(addr)

#define GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(service)   (GP_HAL_PHYSICAL_TO_ARM(gpHal_Ble_EventInfoMemory) + service*GPHAL_BLE_SERVICE_EVENT_INFO_SIZE)
#define GP_HAL_BLE_CONNECTION_TO_EVENT_INFO_ADDRESS(connId) (GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(GPHAL_BLE_NR_OF_SERVICE_EVENTS) + connId * GPHAL_BLE_CONN_EVENT_INFO_SIZE)

#define GP_HAL_BLE_SERVICE_TO_OFFSET_FROM_START(service)    /*TO_GPM_ADDR(GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(service))*/ GPHAL_BLE_RAM_ADDRESS_TO_START_OFFSET(GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(service))
#define GP_HAL_BLE_CONNECTION_TO_OFFSET_FROM_START(connId)  GPHAL_BLE_RAM_ADDRESS_TO_START_OFFSET(GP_HAL_BLE_CONNECTION_TO_EVENT_INFO_ADDRESS(connId))

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

typedef struct {
    UInt16 infoPtr;
    UInt8 connId;
    gpHal_AbsoluteEventId_t eventNr;
    Bool running;
    Bool virtual;
} gpHal_BleConnectionContext_t;

typedef struct {
    gpHal_AbsoluteEventId_t eventNr;
} gpHal_BleServiceContext_t;

#define gpHal_BleServiceEventIdAdvertising   0x00
#define gpHal_BleServiceEventIdScanning      0x01
#define gpHal_BleServiceEventIdInitiating    0x02
#define gpHal_BleServiceEventIdSubEv         0x03
#define gpHal_BleServiceEventIdBgsc          0x04
typedef UInt8 gpHal_BleServiceEventId_t;

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

gpHal_Result_t gpHal_BleStartService(gpHal_RtEvent_t* pRtEvent, UInt32 timeStamp);
gpHal_Result_t gpHal_BleStopService(gpHal_BleServiceEventId_t serviceEvent);
gpHal_Result_t gpHal_BleRestartService(gpHal_BleServiceEventId_t serviceEvent, UInt32 timeStamp, UInt8 type);
gpHal_Result_t gpHal_BleStartEvent(rt_event_handle_t eventHandle, UInt32 timeStamp, gpHal_RtEvent_t* pRtEvent);
gpHal_Result_t gpHal_BleRestartEvent(rt_event_handle_t eventHandle, UInt32 timeStamp);
gpHal_Result_t gpHal_BleStopEvent(rt_event_handle_t eventHandle);

// PD - PBM conversion
void gpHal_BleConfPbmToPd(UInt8 pbmHandle, gpPd_Loh_t* pPdLoh);
void gpHal_BleIndPbmToPd(UInt8 pbmEntry, gpPd_Loh_t* pPdLoh);

void gpHal_BlePopulateDefaultPbmOptions(UInt8 pbmHandle, Bool TxIfTooLate);
void gpHal_BleSetPbmTxPower(UInt8 pbmHandle, Int8 txPower);
UInt8 gpHal_BlePdToPbm(gpPd_Loh_t pdLoh, Bool TxIfTooLate);

// Connection context mapping
gpHal_BleConnectionContext_t* gpHal_BleAddConnIdMapping(UInt8 connId);
gpHal_BleConnectionContext_t* gpHal_BleGetConnMappingFromId(UInt8 connId);
gpHal_BleConnectionContext_t* gpHal_BleGetConnMappingFromEvent(UInt8 eventNr);
void gpHal_BleRemoveConnIdMapping(gpHal_BleConnectionContext_t* pMapping);


#ifdef GP_DIVERSITY_GPHAL_WHITELIST_UPDATE_CALLBACK
void gpHal_BleWl_cbUpdateIndex(UInt8 addressType, const BtDeviceAddress_t* pAddress, UInt8 idx);
UInt8 gpHal_BleWl_FindEntry(UInt8 addressType, BtDeviceAddress_t* pAddress);
#endif //GP_DIVERSITY_GPHAL_WHITELIST_UPDATE_CALLBACK

UInt8 gpHal_Ble_GetAdvAntenna(void);


void gpHal_BleUpdateCleanupTime(Bool multipleEventsAllocated);

#ifdef __cplusplus
}
#endif
#endif //_HAL_GP_BLE_DEFS_H_
