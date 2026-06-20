/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
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

#ifdef GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_ROM_CODE
#endif //def GP_DIVERSITY_JUMPTABLES
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLEADVERTISER

#include "global.h"
#include "gpBle.h"
#include "gpBleComps.h"
#include "gpBleAddressResolver.h"
#include "gpBle_defs.h"
#include "gpBleActivityManager.h"
#include "gpAssert.h"
#include "gpPd.h"
#include "gpSched.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpHal_Ble_Manual.h"
#include "gpBleConfig.h"

#if defined(GP_DIVERSITY_BLE_PERIPHERAL)
#include "gpBleLlcp.h"
#endif //GP_DIVERSITY_BLE_CENTRAL || GP_DIVERSITY_BLE_PERIPHERAL

#include "gpBleAdvertiser.h"

#include "gpBleAdvertiser_defs.h"

/*****************************************************************************
 *                    Tmp extern Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#if defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
#include "gpBleAdvertiser_CodeJumpTableFlash_Defs_rom_c.h"
#include "gpBleAdvertiser_CodeJumpTableRom_Defs_rom_c.h"
#endif // defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)

/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_START */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_START */
STATIC_FUNC gpHci_Result_t Ble_SetAdvertiseAction(gpHci_LeSetAdvertiseEnableCommand_t* params, gpBle_EventBuffer_t* pEventBuf);
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_END */
/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_END */

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

gpHci_Result_t Ble_SetAdvertisingOrScanDataChecker(UInt8 length, UInt8 maxLength)
{
    if(length > maxLength)
    {
        GP_LOG_PRINTF("Invalid length: %x > %x",0, length, maxLength);
        return gpHci_ResultInvalidHCICommandParameters;
    }

    return gpHci_ResultSuccess;
}

gpHci_Result_t Ble_SetAdvertisingOrScanDataAction(UInt8 length, UInt8* pData, Bool advData)
{
    gpHci_Result_t result = gpHci_ResultSuccess;
    UInt8* pDestData;
    UInt8* pDestLength;

    if(advData)
    {
        pDestData   = GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advData;
        pDestLength = &GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advDataLength;
    }
    else
    {
        pDestData = GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->scanRespData;
        pDestLength = &GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->scanRespDataLength;
    }

    // Update the correct length field
    *pDestLength = length;

    // Update the data
    MEMCPY(pDestData, pData, length);

    if(!GP_HCI_IS_DIRECTED_ADV_TYPE(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advParams.advertisingType) && GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advertisingEnabled)
    {
        // The approach of stopping/restarting advertising works, but is not optimal.
        // Cleaner solution would be to introduce a pause and resume (where it is not needed to do a full stop and restart)
        Ble_SetAdvertiseDisable(NULL, false, false);
        result = Ble_SetAdvertiseEnable();
    }

    return result;
}

gpHci_Result_t Ble_SetAdvertiseChecker(UInt8 advertisingEnable)
{
    if(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advertisingEnabled && advertisingEnable)
    {
        GP_LOG_PRINTF("Advertising already enabled ",0);
        return gpHci_ResultSuccess; // SDP004-2343
    }

    return gpHci_ResultSuccess;
}

gpHci_Result_t Ble_SetAdvertiseAction(gpHci_LeSetAdvertiseEnableCommand_t* params, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;

    GP_LOG_PRINTF("BLE Set advertising enable: %x",0, params->enable);

    if(params->enable)
    {
        result = Ble_SetAdvertiseEnable();
    }
    else
    {
        if(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advertisingEnabled)
        {
            // We need longer processing for a disable (while advertising is still enabled), delay the command complete
            BLE_SET_RESPONSE_EVENT_NOTHING(pEventBuf->eventCode);
        }

        result = Ble_SetAdvertiseDisable(NULL, true, false);
    }

    return result;
}

void Ble_HighDutyCycleDirectedAdvTimeout(void)
{
    Ble_SetAdvertiseDisable(NULL, false, true);
}

UInt8 Ble_GetFilterAcceptListMaskFromAdvParams(gpHci_AdvFilterPolicy_t filterPolicy)
{
    UInt8 mask = 0;

    if(GP_HCI_IS_DIRECTED_ADV_TYPE(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advParams.advertisingType))
    {
        // Directed advertising, filter policy can be ignored
        mask = GPHAL_ENUM_ADV_EV_INFO_FT_CONNECT_REQ_WL_EN_MASK;
    }
    else
    {
        if(filterPolicy & Ble_AdvFilterPolicyMask_ScanReq)
        {
            mask |= GPHAL_ENUM_ADV_EV_INFO_FT_SCAN_REQ_WL_EN_MASK;
        }

        if((filterPolicy & Ble_AdvFilterPolicyMask_ConnReq))
        {
            mask |= GPHAL_ENUM_ADV_EV_INFO_FT_CONNECT_REQ_WL_EN_MASK;
        }
    }

    return mask;
}

void Ble_AdvChannelMapToChannels(UInt8 channelMapping, UInt8* pChannels, UInt8 numberOfChannels)
{
    UIntLoop i;
    UInt8 channelIndex = 0;

    GP_ASSERT_DEV_INT(pChannels != NULL);

    MEMSET(pChannels, BLE_ADV_CHANNEL_INVALID, numberOfChannels);

    for(i = 0; i < numberOfChannels; i++)
    {
        if((channelMapping & BM(i)) != 0)
        {
            GP_LOG_PRINTF("Setting channel %d ",2, BLE_ADV_CHANNEL_37 + i);
            pChannels[channelIndex] = BLE_ADV_CHANNEL_37 + i;
            channelIndex++;
        }
    }
}

/*
 * Mapping not easy to calculate, use a translation table.
 * See LL spec section 2.3
*/
Ble_AdvertisingPduType_t Ble_ConvertAdvTypeToPduType(gpHci_AdvertisingType_t pduType)
{
    /* Mapping from pduType to advertising report event type */
    Ble_AdvertisingPduType_t mapping[BLE_NR_OF_VALID_ADV_PDU_TYPES] =
    {
        Ble_AdvertisingPduType_ConnectableUndirected,      /* gpHci_AdvertisingType_ConnectableUndirected */
        Ble_AdvertisingPduType_ConnectableDirected,        /* gpHci_AdvertisingType_ConnectableLowDutyCycleDirected */
        Ble_AdvertisingPduType_ScannableUndirected,        /* gpHci_AdvertisingType_ScannableUndirected */
        Ble_AdvertisingPduType_NonConnectableUndirected,   /* gpHci_AdvertisingType_NonConnectableUndirected */
        Ble_AdvertisingPduType_ConnectableDirected         /* gpHci_AdvertisingType_ConnectableHighDutyCycleDirected */
    };

    if(!BLE_ADVERTISING_TYPE_VALID(pduType))
    {
        return Ble_AdvertisingPduType_Invalid;
    }

    return mapping[pduType];
}

void Ble_FreePdsIfValid(gpPd_Loh_t* pPdLohAdv, gpPd_Loh_t* pPdLohScan)
{
    Ble_RMFreeResource(0, pPdLohAdv->handle);
    if(gpPd_CheckPdValid(pPdLohScan->handle) == gpPd_ResultValidHandle)
    {
        Ble_RMFreeResource(0,pPdLohScan->handle);
    }
}

/*****************************************************************************
 *                    Component Function Definitions
 *****************************************************************************/

Bool gpBleAdvertiser_IsEnabled(void)
{
    return GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advertisingEnabled;
}

Bool gpBleAdvertiser_IsFilterAcceptListUsed(void)
{
    return BLE_ADV_FILTER_ACCEPT_LIST_USED(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advParams.filterPolicy);
}

UInt32 gpBleAdvertiser_GetIntervalUs(void)
{
    if(gpBleAdvertiser_IsEnabled())
    {
        return BLE_TIME_UNIT_625_TO_US(GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advTimeParams.interval);
    }
    else
    {
        return BLE_TIME_UNIT_625_TO_US(BLE_ADVERTISING_INTERVAL_DEFAULT_INVALID);
    }
}

/*****************************************************************************
 *                    Service Function Definitions
 *****************************************************************************/

gpHci_Result_t gpBle_LeSetAdvertisingParameters(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;
    gpHci_LeSetAdvertisingParametersCommand_t* advParams = &pParams->LeSetAdvertisingParameters;

    GP_LOG_PRINTF("BLE Set Adv Prmtrs %u/%u %u/%u/%u %x %x",0, advParams->advertisingIntervalMin,
                                                               advParams->advertisingIntervalMax,
                                                               advParams->advertisingType,
                                                               advParams->ownAddressType,
                                                               advParams->peerAddressType,
                                                               advParams->channelMap,
                                                               advParams->filterPolicy);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_SetAdvertisingParametersChecker(advParams);

    if(result == gpHci_ResultSuccess)
    {
        MEMCPY(&GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advParams, advParams, sizeof(gpHci_LeSetAdvertisingParametersCommand_t));
        GP_LOG_PRINTF("Advertising parameters stored",0);
    }

    return result;
}

gpHci_Result_t gpBle_LeSetAdvertisingData(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;
    gpHci_LeSetAdvertisingDataCommand_t* pAdvDataParams = &pParams->LeSetAdvertisingData;

    GP_LOG_PRINTF("BLE Set advertising data",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_SetAdvertisingOrScanDataChecker(pAdvDataParams->length, GP_HCI_ADVERTISING_DATA_PAYLOAD_SIZE_MAX);

    if(result == gpHci_ResultSuccess)
    {
        result = Ble_SetAdvertisingOrScanDataAction(pAdvDataParams->length, pAdvDataParams->data, true);
        GP_LOG_PRINTF("Advertising data stored",0);
    }

    return result;
}

gpHci_Result_t gpBle_LeSetScanResponseData(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;
    gpHci_LeSetScanResponseDataCommand_t* pScanRespDataParams = &pParams->LeSetScanResponseData;

    GP_LOG_PRINTF("BLE Set scan resp data",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_SetAdvertisingOrScanDataChecker(pParams->LeSetScanResponseData.length, GP_HCI_SCAN_RESPONSE_DATA_PAYLOAD_SIZE_MAX);

    if(result == gpHci_ResultSuccess)
    {
        result = Ble_SetAdvertisingOrScanDataAction(pScanRespDataParams->length, pScanRespDataParams->data, false);
        GP_LOG_PRINTF("Scan resp data stored",0);
    }

    return result;
}

gpHci_Result_t gpBle_LeReadAdvertisingChannelTxPower(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    GP_LOG_PRINTF("BLE Read adv channel Tx power",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    pEventBuf->payload.commandCompleteParams.returnParams.advChannelTxPower = GP_BLEADVERTISER_GET_GLOBALS()->Ble_AdvertisingAttributes_ptr->advChannelTxPower;

    return gpHci_ResultSuccess;
}

gpHci_Result_t gpBle_LeSetAdvertiseEnable(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;
    gpHci_LeSetAdvertiseEnableCommand_t* pAdvEnableParams = &pParams->LeSetAdvertiseEnable;

    GP_LOG_PRINTF("BLE Set adv enable",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result =  Ble_SetAdvertiseChecker(pAdvEnableParams->enable);

    if(result == gpHci_ResultSuccess)
    {
        result = Ble_SetAdvertiseAction(pAdvEnableParams, pEventBuf);
    }

    return result;
}
