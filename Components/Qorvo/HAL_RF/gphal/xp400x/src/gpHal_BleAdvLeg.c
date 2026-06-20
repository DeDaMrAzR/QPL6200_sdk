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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

//#define GP_LOCAL_LOG

#include "gpHal.h"
#include "gpHal_Ble_DEFS.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#define GP_HAL_BLE_ADV_ABORT_TIMEOUT_US                             1000

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static gpHal_Result_t gpHal_BlePopulateAdvEventInfo(gpHal_AdvEventInfo_t* pInfo);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

gpHal_Result_t gpHal_BlePopulateAdvEventInfo(gpHal_AdvEventInfo_t* pInfo)
{
    UInt8 pbmHandle;
    gpHal_Address_t eventInfoAddress;
    UInt8 len_adv   = 0;
    UInt8 len_scan_rsp = 0;

    pbmHandle = gpHal_BlePdToPbm(pInfo->pdLohAdv, true);

    if(!GP_HAL_CHECK_PBM_VALID(pbmHandle))
    {
        return gpHal_ResultInvalidParameter;
    }

    len_adv = pInfo->pdLohAdv.length;

    eventInfoAddress = (gpHal_Address_t)GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(gpHal_BleServiceEventIdAdvertising);

    GP_WB_WRITE_ADV_EV_INFO_ADV_FT_PTR(eventInfoAddress, gpHal_PbmToFormatPtr(pbmHandle));

    if(gpPd_CheckPdValid(pInfo->pdLohScan.handle) == gpPd_ResultValidHandle)
    {
        // We have a valid scan resp pd
        pbmHandle = gpHal_BlePdToPbm(pInfo->pdLohScan, false); // no Tx if too late for scan rsp

        if(!GP_HAL_CHECK_PBM_VALID(pbmHandle))
        {
            return gpHal_ResultInvalidParameter;
        }

        len_scan_rsp = pInfo->pdLohScan.length;

        GP_WB_WRITE_ADV_EV_INFO_SCAN_RSP_FT_PTR(eventInfoAddress, gpHal_PbmToFormatPtr(pbmHandle));
    }
    else
    {
        // Make sure we always set invalid pbm in case none is specified
        GP_WB_WRITE_ADV_EV_INFO_SCAN_RSP_FT_PTR(eventInfoAddress, GP_HAL_FORMAT_PTR_INVALID);
    }

    GP_WB_WRITE_ADV_EV_INFO_INTRA_EV_TX_SPACING(eventInfoAddress, 0 /* intraEventTxSpacing can be minimized, since we have Tx if too late set for adv PDUs*/);
    GP_WB_WRITE_ADV_EV_INFO_ADV_DELAY_MASK(eventInfoAddress, pInfo->advDelayMax);
    GP_WB_WRITE_ADV_EV_INFO_ADV_CH0(eventInfoAddress, pInfo->channelMap[0]);
    GP_WB_WRITE_ADV_EV_INFO_ADV_CH1(eventInfoAddress, pInfo->channelMap[1]);
    GP_WB_WRITE_ADV_EV_INFO_ADV_CH2(eventInfoAddress, pInfo->channelMap[2]);
    GP_WB_WRITE_ADV_EV_INFO_FRAME_TYPE_ACCEPT_MASK(eventInfoAddress, pInfo->frameTypeAcceptMask);
    GP_WB_WRITE_ADV_EV_INFO_FT_WHITELIST_ENABLE_MASK(eventInfoAddress, pInfo->filterAcceptlistEnableMask);
    GP_ASSERT_DEV_INT(len_adv+3 <= LEN_MAX_ADV_IND);
    GP_ASSERT_DEV_INT(len_scan_rsp+3 <= LEN_MAX_SCAN_RSP);
    GP_WB_WRITE_ADV_EV_INFO_GUARD_TIME(eventInfoAddress, T_ADV_GUARD_TIME(len_adv, len_scan_rsp));
    GP_WB_WRITE_ADV_EV_INFO_CURR_ANTENNA(eventInfoAddress, gpHal_GetBleAntenna());
    GP_WB_WRITE_ADV_EV_INFO_ACCEPT_UNRESOLVED_RPA_SRC(eventInfoAddress, 1);
    GP_WB_WRITE_ADV_EV_INFO_ACCEPT_UNRESOLVED_ID_SRC(eventInfoAddress, 1);

    return gpHal_ResultSuccess;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

gpHal_Result_t gpHal_BleStartAdvertising(UInt32 firstAdvTs, gpHal_AdvEventInfo_t* pInfo)
{
    gpHal_Result_t result;

    // write away the event info structure
    result = gpHal_BlePopulateAdvEventInfo(pInfo);

    if(result != gpHal_ResultSuccess)
    {
        return result;
    }

    pInfo->rtEvent.type = GPHAL_ENUM_EVENT_TYPE_ADVERTISE;
    result = gpHal_BleStartService(&pInfo->rtEvent, firstAdvTs);

    return result;
}

gpHal_Result_t gpHal_BleRestartAdvertising(UInt32 advTs)
{
    return gpHal_BleRestartService(gpHal_BleServiceEventIdAdvertising, advTs, GPHAL_ENUM_EVENT_TYPE_ADVERTISE);
}

gpHal_Result_t gpHal_BleStopAdvertising(gpPd_Loh_t* pdLohAdv, gpPd_Loh_t* pdLohScan)
{
    UInt8 pbmHandle;

    gpHal_Address_t eventInfoAddress = (gpHal_Address_t)GP_HAL_BLE_SERVICE_TO_EVENT_INFO_ADDRESS(gpHal_BleServiceEventIdAdvertising);

    // Read back pbm information
    pbmHandle = GP_HAL_NEW_PTR_TO_PBM(GP_WB_READ_ADV_EV_INFO_ADV_FT_PTR(eventInfoAddress));

    gpHal_BleConfPbmToPd(pbmHandle, pdLohAdv);

    pbmHandle = GP_HAL_NEW_PTR_TO_PBM(GP_WB_READ_ADV_EV_INFO_SCAN_RSP_FT_PTR(eventInfoAddress));

    if(GP_HAL_CHECK_PBM_VALID(pbmHandle))
    {
        gpHal_BleConfPbmToPd(pbmHandle, pdLohScan);
    }

    gpHal_Result_t status = gpHal_BleStopService(gpHal_BleServiceEventIdAdvertising);
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_RT_STATE() == GP_WB_ENUM_RT_STATE_ABORT, GP_HAL_BLE_ADV_ABORT_TIMEOUT_US);

    return status;
}
