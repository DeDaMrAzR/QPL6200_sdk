/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacCore_Scan.c
 *   This file contains the implementation of the non beaconed MAC protocol.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

// General includes
#include "gpPd.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpSched.h"
#include "gpPoolMem.h"
/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

gpMacCore_Result_t MacCore_SendCommandBeaconRequest(UInt8 channel)
{
    gpHal_Result_t result;
    gpPd_Loh_t pdLoh;
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    gpHal_DataReqOptions_t  dataReqOptions = { 0 };
    UInt8 channels[3] = {channel, GP_HAL_MULTICHANNEL_INVALID_CHANNEL, GP_HAL_MULTICHANNEL_INVALID_CHANNEL};
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    // initialize pd
    if(!MacCore_AllocatePdLoh(&pdLoh))
    {
        return gpMacCore_ResultTransactionOverflow;
    }

    dataReqOptions.macScenario = gpHal_MacDefault;
    dataReqOptions.srcId = maccore_globals->gpMacCore_pScanState->stackId;
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    dataReqOptions.rawEncryptionEnable = false;
    dataReqOptions.rawKeepFrameCounter = false;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

    // set transmit attributes for this stack
    gpPad_SetTxChannels(MacCore_GetPad(maccore_globals->gpMacCore_pScanState->stackId), channels);

    srcAddrInfo.addressMode = gpMacCore_AddressModeNoAddress;
    srcAddrInfo.panId = gpMacCore_GetPanId(maccore_globals->gpMacCore_pScanState->stackId);

    dstAddrInfo.addressMode = gpMacCore_AddressModeShortAddress;
    dstAddrInfo.address.Short = GP_MACCORE_SHORT_ADDR_BROADCAST;
    dstAddrInfo.panId = GP_MACCORE_PANID_BROADCAST;

    MacCore_WriteMacHeaderInPd(gpMacCore_FrameTypeCommand, &srcAddrInfo, &dstAddrInfo,0,0,&pdLoh,maccore_globals->gpMacCore_pScanState->stackId);

    gpPd_WriteByte(pdLoh.handle, GP_MACCORE_MAX_MAC_HEADER_SIZE, gpMacCore_CommandBeaconRequest);
    pdLoh.length++;

    // send beacon request
    result = MacCore_TxDataRequest(&dataReqOptions, pdLoh, maccore_globals->gpMacCore_pScanState->stackId);
    if (result != gpHal_ResultSuccess)
    {
        gpPd_FreePd(pdLoh.handle);
    }
    return result;
}

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
gpMacCore_Result_t MacCore_SendCommandOrphanNotification(UInt8 channel)
{
    gpHal_Result_t result;
    gpPd_Loh_t pdLoh;
    gpMacCore_AddressInfo_t srcAddrInfo;
    gpMacCore_AddressInfo_t dstAddrInfo;
    gpHal_DataReqOptions_t  dataReqOptions = { 0 };
    UInt8 channels[3] = {channel, GP_HAL_MULTICHANNEL_INVALID_CHANNEL, GP_HAL_MULTICHANNEL_INVALID_CHANNEL};
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    // initialize pd
    if(!MacCore_AllocatePdLoh(&pdLoh))
    {
        return gpMacCore_ResultTransactionOverflow;
    }
    dataReqOptions.macScenario = gpHal_MacDefault;
    dataReqOptions.srcId = maccore_globals->gpMacCore_pScanState->stackId;
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    dataReqOptions.rawEncryptionEnable = false;
    dataReqOptions.rawKeepFrameCounter = false;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

    // set transmit attributes for this stack
    gpPad_SetTxChannels(MacCore_GetPad(maccore_globals->gpMacCore_pScanState->stackId), channels);

    srcAddrInfo.addressMode = gpMacCore_AddressModeExtendedAddress;
    gpMacCore_GetExtendedAddress(&srcAddrInfo.address.Extended, maccore_globals->gpMacCore_pScanState->stackId);
    srcAddrInfo.panId = GP_MACCORE_PANID_BROADCAST;

    dstAddrInfo.addressMode = gpMacCore_AddressModeShortAddress;
    dstAddrInfo.address.Short = GP_MACCORE_SHORT_ADDR_BROADCAST;
    dstAddrInfo.panId = GP_MACCORE_PANID_BROADCAST;

    MacCore_WriteMacHeaderInPd(gpMacCore_FrameTypeCommand, &srcAddrInfo, &dstAddrInfo, 0, 0, &pdLoh,maccore_globals->gpMacCore_pScanState->stackId);

    gpPd_WriteByte(pdLoh.handle, GP_MACCORE_MAX_MAC_HEADER_SIZE, gpMacCore_CommandOrphanNotification);
    pdLoh.length++;

    // send orphan notification
    result = MacCore_TxDataRequest(&dataReqOptions, pdLoh, maccore_globals->gpMacCore_pScanState->stackId);
    if (result != gpHal_ResultSuccess)
    {
        gpPd_FreePd(pdLoh.handle);
    }
    return result;
}
#endif //GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR

STATIC_FUNC UInt8 MacCore_GetNextScanChannel(void)
{
    // search channel
    UInt8 channel = 0;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    UInt16 searchChannel = (UInt16)maccore_globals->gpMacCore_pScanState->scanChannels;

    while((searchChannel & 1) == 0)
    {
        channel++;
        searchChannel = searchChannel>>1;
    }
    searchChannel = searchChannel>>1;
    maccore_globals->gpMacCore_pScanState->scanChannels = searchChannel<<(channel+1);
    channel += 11;
    // set the channel to scan
    return channel;
}

void MacCore_PrepareScanVars(UInt8 frameTypeFilterMask)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    maccore_globals->gpMacCore_pScanState->un_t.origFilterMask = gpHal_GetFrameTypeFilterMask();
    gpHal_SetFrameTypeFilterMask(frameTypeFilterMask);
    maccore_globals->gpMacCore_pScanState->unscannedChannels = 0;
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
    gpRxArbiter_SetStackRxOn(true, maccore_globals->gpMacCore_pScanState->stackId);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
}

#if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
void MacCore_DoEDScan(void)
{
    gpHal_Result_t result;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    //Scan full mask in one request
    result = gpHal_EDRequest(maccore_globals->gpMacCore_pScanState->scanDurationUc, maccore_globals->gpMacCore_pScanState->requestChannels);
    if(result != gpHal_ResultSuccess)
    {
        //Put results on stack and free malloc-ed memory
        UInt32 unscannedChannels = (UInt32)(((UInt32)maccore_globals->gpMacCore_pScanState->requestChannels)<<11);
        UInt8* pEnergyDetectList = maccore_globals->gpMacCore_pScanState->pEnergyDetectList;

        gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel , maccore_globals->gpMacCore_pScanState->stackId);

        gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
        maccore_globals->gpMacCore_pScanState=NULL;

        gpMacCore_cbScanConfirm(gpMacCore_ResultTransactionOverflow, gpMacCore_ScanTypeED, unscannedChannels, 0, pEnergyDetectList);
    }
}

void MacCore_DoIteratedScan(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    // Scan each channel multiple times and get average result.
    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    maccore_globals->gpMacCore_pScanState->scannedChannelCounter = 0;
    maccore_globals->gpMacCore_pScanState->cummulatedRssi = 0;
    maccore_globals->gpMacCore_pScanState->scanDurationRemaining = maccore_globals->gpMacCore_pScanState->scanDurationUc;

    // Start first scan.
    MacCore_DoScanIteration();
}

STATIC_FUNC void MacCore_DoScanIteration(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }

    if (!maccore_globals->gpMacCore_pScanState->scanChannels ||
        maccore_globals->gpMacCore_pScanState->scannedChannelCounter >= maccore_globals->gpMacCore_pScanState->resultListSizeMax)
    {
        //Put results on stack and free malloc-ed memory
        gpMacCore_Result_t   status   = maccore_globals->gpMacCore_pScanState->unscannedChannels == 0 ? gpMacCore_ResultSuccess : gpMacCore_ResultLimitReached;
        gpMacCore_ScanType_t scanType = maccore_globals->gpMacCore_pScanState->currentScanType;
        UInt32 unscannedChannels      = (UInt32)(((UInt32)maccore_globals->gpMacCore_pScanState->unscannedChannels)<<11);
        UInt8 resultListSize          = maccore_globals->gpMacCore_pScanState->scannedChannelCounter;
        UInt8* pEnergyDetectList = maccore_globals->gpMacCore_pScanState->pEnergyDetectList;

        // done scanning all channels
        gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel , maccore_globals->gpMacCore_pScanState->stackId);

        gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
        maccore_globals->gpMacCore_pScanState = NULL;

        gpMacCore_cbScanConfirm(status, scanType, unscannedChannels, resultListSize, pEnergyDetectList);
    }
    else
    {
        UInt8 channel;
        UInt32 scanTime;

        // Select channel to scan.
        for (channel = 0; channel < 16; ++channel)
        {
            if (BIT_TST(maccore_globals->gpMacCore_pScanState->scanChannels, channel))
            {
                break;
            }
        }
        GP_ASSERT_DEV_INT(channel != 16);

        //Bypass if selected channel is blocked by RDS
#ifdef GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
        if (DIVERSITY_REGIONALDOMAINSETTINGS())
        {
            if ((maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAI) || (maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAIMasked))
            {
                if (MacCore_IsRdsChannelBlocked(channel+11,maccore_globals->gpMacCore_pScanState->stackId))
                {
                    maccore_globals->gpMacCore_pScanState->pEnergyDetectList[maccore_globals->gpMacCore_pScanState->scannedChannelCounter] = 255;
                    //Update unscanned channels
                    BIT_CLR(maccore_globals->gpMacCore_pScanState->unscannedChannels, channel);

                    MacCore_DoScanPrepareNextIteration(channel);

                    gpSched_ScheduleEvent(0, MacCore_DoScanIteration);

                    return;
                }
            }
        }
#endif //GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS

        // Calculate time to scan.
        if (maccore_globals->gpMacCore_pScanState->scanDurationRemaining >= GP_MACCORE_WIFI_BEACON_SCAN_DURATION)
        {
            scanTime = GP_MACCORE_WIFI_BEACON_SCAN_DURATION;
        }
        else
        {
            scanTime = maccore_globals->gpMacCore_pScanState->scanDurationRemaining;
        }
        maccore_globals->gpMacCore_pScanState->scanDurationRemaining -= scanTime;


        {
            gpHal_Result_t result = gpHal_EDRequest(scanTime, BM(channel));

            if(result != gpHal_ResultSuccess)
            {
                //Put results on stack and free malloc-ed memory
                gpMacCore_ScanType_t scanType = maccore_globals->gpMacCore_pScanState->currentScanType;
                UInt32 unscannedChannels      = (UInt32)(((UInt32)maccore_globals->gpMacCore_pScanState->unscannedChannels)<<11);
                UInt8* pEnergyDetectList = maccore_globals->gpMacCore_pScanState->pEnergyDetectList;

                gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel , maccore_globals->gpMacCore_pScanState->stackId);

                gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
                maccore_globals->gpMacCore_pScanState = NULL;

                // Flag error
                gpMacCore_cbScanConfirm(gpMacCore_ResultTransactionOverflow, scanType, unscannedChannels, 0, pEnergyDetectList);
            }
        }
    }
}

/** @brief Continue to the next channel to do an iterated scan on */
STATIC_FUNC void MacCore_DoScanPrepareNextIteration(UInt8 channel)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    //Remove scannned channel
    BIT_CLR(maccore_globals->gpMacCore_pScanState->scanChannels, channel);

    maccore_globals->gpMacCore_pScanState->scannedChannelCounter++;
    //Reset vars for next scan
    maccore_globals->gpMacCore_pScanState->scanDurationRemaining = maccore_globals->gpMacCore_pScanState->scanDurationUc;
    maccore_globals->gpMacCore_pScanState->cummulatedRssi = 0;
}
#endif // #if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)

#if defined (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
void MacCore_DoActiveScan(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    if (maccore_globals->gpMacCore_pScanState->scanChannels)
    {
        gpMacCore_Result_t result;
        UInt8 channel;
        channel = MacCore_GetNextScanChannel();

        // First schedule timeout
        gpSched_ScheduleEvent(maccore_globals->gpMacCore_pScanState->scanDurationUc + GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US , MacCore_DoActiveScan);
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
        gpSched_ScheduleEvent(maccore_globals->gpMacCore_pScanState->scanDurationUc , MacCore_ScanRxOffWindow);
        gpRxArbiter_SetStackRxOn(true, maccore_globals->gpMacCore_pScanState->stackId);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
        gpRxArbiter_SetStackChannel(channel, maccore_globals->gpMacCore_pScanState->stackId);
        result = MacCore_SendCommandBeaconRequest(channel);

        if(result != gpMacCore_ResultSuccess)
        {
            maccore_globals->gpMacCore_pScanState->unscannedChannels |= BM(channel - 11);
        }
    }
    else
    {
        //Put results on stack and free malloc-ed memory
        gpMacCore_Result_t status;
        UInt32 unscannedChannels = (UInt32)(((UInt32)maccore_globals->gpMacCore_pScanState->unscannedChannels)<<11);

        // status gpMacCore_ResultSuccess indicates all channels have been scanned successfully. Beacon information is passed to higher layer
        // Testspec (TP/154/MAC/SCANNING-02): we need to return result no_beacon if no beacon received.
        status = (maccore_globals->gpMacCore_pScanState->resultListSize > 0) ? gpMacCore_ResultSuccess : gpMacCore_ResultNoBeacon;

        gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel , maccore_globals->gpMacCore_pScanState->stackId);
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
        gpRxArbiter_SetStackRxOn(false, maccore_globals->gpMacCore_pScanState->stackId);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
        gpHal_SetFrameTypeFilterMask(maccore_globals->gpMacCore_pScanState->un_t.origFilterMask);
        gpHal_SetBeaconSrcPanChecking(true);

        gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
        maccore_globals->gpMacCore_pScanState=NULL;

        gpMacCore_cbScanConfirm(status, gpMacCore_ScanTypeActive, unscannedChannels, 0, NULL);
    }
}
#endif // #if defined (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)

#if defined (GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
void MacCore_DoOrphanScan(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
    if (DIVERSITY_SCAN_ORPHAN_ORIGINATOR())
    {

        if(maccore_globals->gpMacCore_pScanState->scanChannels)
        {
            gpMacCore_Result_t result;
            UInt8 channel;
            channel = MacCore_GetNextScanChannel();
            // First schedule timeout
            gpSched_ScheduleEvent(maccore_globals->gpMacCore_pScanState->scanDurationUc + GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US , MacCore_DoOrphanScan);
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
            gpSched_ScheduleEvent(maccore_globals->gpMacCore_pScanState->scanDurationUc , MacCore_ScanRxOffWindow);
            gpRxArbiter_SetStackRxOn(true, maccore_globals->gpMacCore_pScanState->stackId);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
            gpRxArbiter_SetStackChannel(channel, maccore_globals->gpMacCore_pScanState->stackId);
            result = MacCore_SendCommandOrphanNotification(channel);

            if(result != gpMacCore_ResultSuccess)
            {
                maccore_globals->gpMacCore_pScanState->unscannedChannels |=  BM(channel - 11);
            }
        }
        else
        {
            MacCore_HandleOrphanScanEnd(gpMacCore_ResultNoBeacon);
        }
    }
#else //GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
    gpMacCore_cbScanConfirm(gpMacCore_ResultInvalidParameter, gpMacCore_ScanTypeOrphan, 1, 0, NULL);
#endif //GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
}
#endif // (GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)

#ifdef GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
void MacCore_HandleOrphanScanEnd(gpMacCore_Result_t result)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    UInt32 unscannedChannels;

#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
    gpRxArbiter_SetStackRxOn(false, maccore_globals->gpMacCore_pScanState->stackId);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)
    if(result == gpMacCore_ResultNoBeacon)
    {
        gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel, maccore_globals->gpMacCore_pScanState->stackId);
    }
    gpHal_SetFrameTypeFilterMask(maccore_globals->gpMacCore_pScanState->un_t.origFilterMask);
    maccore_globals->gpMacCore_pScanState->unscannedChannels |= maccore_globals->gpMacCore_pScanState->scanChannels; //Add remaining channels to unscanned channels
    unscannedChannels = (UInt32)(((UInt32)maccore_globals->gpMacCore_pScanState->unscannedChannels)<<11);

    gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
    maccore_globals->gpMacCore_pScanState=NULL;

    gpMacCore_cbScanConfirm(result, gpMacCore_ScanTypeOrphan, unscannedChannels, 0, NULL);
}
#endif //GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR

#if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
STATIC_FUNC void MacCore_EDScan_Confirm(UInt16 channelMask, UInt8 *proto_energy_level)
{
    UInt8 channel;
    UInt8 amountScanned;
    UInt8 amountReturned;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    amountScanned = 0;
    amountReturned = 0;

    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }
    //Scan be interrupted - partial mask could be received
    GP_ASSERT_DEV_INT(channelMask & maccore_globals->gpMacCore_pScanState->requestChannels);

    for (channel=0; channel<16; ++channel)
    {
        if (!BIT_TST(channelMask, channel))
        {
            continue;
        }

        //Only store results that fit in the list
        if(amountScanned <= maccore_globals->gpMacCore_pScanState->resultListSizeMax)
        {
            UInt8 measuredEnergy;

            //Update energy values to requested type
            measuredEnergy = gpHal_CalculateED(proto_energy_level[channel]);

            maccore_globals->gpMacCore_pScanState->pEnergyDetectList[amountReturned] = measuredEnergy;
            ++amountReturned;
        }

        ++amountScanned;
    }

    // done scanning all channels
    gpRxArbiter_SetStackChannel(maccore_globals->gpMacCore_pScanState->origPhyChannel , maccore_globals->gpMacCore_pScanState->stackId);

    {
    //Put results on stack and free malloc-ed memory
    gpMacCore_Result_t   status   = (amountScanned > maccore_globals->gpMacCore_pScanState->resultListSizeMax) ? gpMacCore_ResultLimitReached : gpMacCore_ResultSuccess;
    gpMacCore_ScanType_t scanType = maccore_globals->gpMacCore_pScanState->currentScanType;
    UInt8* pEnergyDetectList = maccore_globals->gpMacCore_pScanState->pEnergyDetectList;
    UInt32 unscannedChannels = (UInt32)((UInt32)(maccore_globals->gpMacCore_pScanState->requestChannels & ~channelMask) << 11);

    gpPoolMem_Free(maccore_globals->gpMacCore_pScanState);
    maccore_globals->gpMacCore_pScanState=NULL;

    gpMacCore_cbScanConfirm(status, scanType, unscannedChannels, amountReturned, pEnergyDetectList);
    }
}

STATIC_FUNC void MacCore_IteratedScan_Confirm(UInt16 channelMask, UInt8 *proto_energy_level)
{
    UInt8 channel;
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_ASSERT_DEV_INT(false);
        return;
    }
    GP_ASSERT_DEV_INT(maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeRSSI ||
                      maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeRSSIMasked ||
                      maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAI ||
                      maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAIMasked);

    // Find current channel.
    for (channel = 0; channel < 16; ++channel)
    {
        if (BIT_TST(channelMask, channel))
        {
            break;
        }
    }

    if(channel == 16)
    {
        GP_ASSERT_DEV_EXT(false);
        return;
    }
    GP_ASSERT_SYSTEM(BIT_TST(maccore_globals->gpMacCore_pScanState->unscannedChannels, channel));

    // Add to running sum of measured RSSI.
    maccore_globals->gpMacCore_pScanState->cummulatedRssi += gpHal_CalculateRSSI(gpHal_ConvertProtoEDToProtoRSSI(proto_energy_level[channel]));

    if (maccore_globals->gpMacCore_pScanState->scanDurationRemaining == 0)
    {
        Int8 measuredRssi;

        // Completed last scan for this channel.
        GP_ASSERT_DEV_INT(maccore_globals->gpMacCore_pScanState->scannedChannelCounter < maccore_globals->gpMacCore_pScanState->resultListSizeMax);

        //Clear scanned channel from unscanned channels
        BIT_CLR(maccore_globals->gpMacCore_pScanState->unscannedChannels, channel);

        // Calc avg value
        if (maccore_globals->gpMacCore_pScanState->scanDurationUc < GP_MACCORE_WIFI_BEACON_SCAN_DURATION)
        {
            measuredRssi = maccore_globals->gpMacCore_pScanState->cummulatedRssi;
        }
        else
        {
            measuredRssi = div_round(maccore_globals->gpMacCore_pScanState->cummulatedRssi, (UInt8)udiv_ceil(maccore_globals->gpMacCore_pScanState->scanDurationUc, GP_MACCORE_WIFI_BEACON_SCAN_DURATION));
        }

        // Clip to minimum interference levels
        if ((maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeRSSIMasked) || (maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAIMasked))
        {
            measuredRssi = max(measuredRssi, GP_MACCORE_GET_GLOBALS_CONST()->MacCore_MinInterferenceLevels[channel]);
        }

        // Convert to AI if applicable
        if ((maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAI) || (maccore_globals->gpMacCore_pScanState->currentScanType == gpMacCore_ScanTypeAIMasked))
        {
            UInt8 measuredAi;
            measuredAi = MacCore_ConvertRssiToAbsoluteInterference(measuredRssi);
            maccore_globals->gpMacCore_pScanState->pEnergyDetectList[maccore_globals->gpMacCore_pScanState->scannedChannelCounter] = measuredAi;
        }
        else
        {
            maccore_globals->gpMacCore_pScanState->pEnergyDetectList[maccore_globals->gpMacCore_pScanState->scannedChannelCounter] = measuredRssi;
        }

        MacCore_DoScanPrepareNextIteration(channel);
    }

    // Initiate next scan.
    // Go through scheduler to avoid calling gpHal_EDRequest from gpHal_cbEDConfirm context.
    gpSched_ScheduleEvent(0, MacCore_DoScanIteration);
}

STATIC_FUNC UInt8 MacCore_ConvertRssiToAbsoluteInterference(Int8 valueRssi)
{
    Int16 result = 0;
    if (valueRssi > -45)
    {
        result = 254;
    }
    else if (valueRssi > -95)
    {
        result = (24130 + valueRssi * 254) / 50;
    }
    return (UInt8)result;
}

#endif // #if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)

void gpHal_cbEDConfirm(UInt16 channelMask, UInt8 *proto_energy_level)
{
#if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
    if (DIVERSITY_SCAN_ED_ORIGINATOR())
    {
        gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
        if(NULL == maccore_globals->gpMacCore_pScanState)
        {
            return;
        }

        switch (maccore_globals->gpMacCore_pScanState->currentScanType)
        {
            case gpMacCore_ScanTypeRSSI:
            case gpMacCore_ScanTypeRSSIMasked:
            case gpMacCore_ScanTypeAI:
            case gpMacCore_ScanTypeAIMasked:
                MacCore_IteratedScan_Confirm(channelMask, proto_energy_level);
                break;

            default:
                MacCore_EDScan_Confirm(channelMask, proto_energy_level);
                break;
        }
    }
#endif // #if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
}

#if(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
void MacCore_ScanRxOffWindow(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    gpRxArbiter_SetStackRxOn(false, maccore_globals->gpMacCore_pScanState->stackId);
}
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)

void MacCore_StopScan(void)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();
    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_LOG_SYSTEM_PRINTF("No scan to stop", 0);
        return;
    }

    switch(maccore_globals->gpMacCore_pScanState->currentScanType)
    {
#if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
        case gpMacCore_ScanTypeED:
        {
            if (DIVERSITY_SCAN_ED_ORIGINATOR())
            {
                //Trigger stop of further scans
                gpHal_EDRequest(0, 0x0);

                //Wait for ED confirm - handles scanned/unscanned
            }
            break;
        }
        case gpMacCore_ScanTypeRSSI:
        case gpMacCore_ScanTypeRSSIMasked:
        case gpMacCore_ScanTypeAI:
        case gpMacCore_ScanTypeAIMasked:
        {
            if (DIVERSITY_SCAN_ED_ORIGINATOR())
            {
                //Clear out working set to stop scan
                maccore_globals->gpMacCore_pScanState->scanChannels = 0x0;

                //Trigger stop of further scans
                gpHal_EDRequest(0, 0x0);

                //Wait for ED confirm - handles scanned/unscanned
            }
            break;
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
#if defined (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
        case gpMacCore_ScanTypeActive:
        {
            if (DIVERSITY_SCAN_ACTIVE_ORIGINATOR())
            {
                //Stop all scheduled actions
                gpSched_UnscheduleEvent(MacCore_DoActiveScan);
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
                gpSched_UnscheduleEvent(MacCore_ScanRxOffWindow);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)

                //Mark remaining channels as unscanned
                maccore_globals->gpMacCore_pScanState->unscannedChannels = maccore_globals->gpMacCore_pScanState->requestChannels & \
                                                                           maccore_globals->gpMacCore_pScanState->scanChannels;
                //Clear out working set to stop scan
                maccore_globals->gpMacCore_pScanState->scanChannels = 0x0;

                //Trigger cleanup of scan
                MacCore_DoActiveScan();
            }
            break;
        }
#endif // (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
#if defined (GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
        case gpMacCore_ScanTypeOrphan:
        {
            if (DIVERSITY_SCAN_ORPHAN_ORIGINATOR())
            {
                //Stop all scheduled actions
                gpSched_UnscheduleEvent(MacCore_DoOrphanScan);
#if (GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US != 0)
                gpSched_UnscheduleEvent(MacCore_ScanRxOffWindow);
#endif //(GP_MACCORE_SCAN_RXOFFWINDOW_TIME_US == 0)

                //Mark remaining channels as unscanned
                maccore_globals->gpMacCore_pScanState->unscannedChannels = maccore_globals->gpMacCore_pScanState->requestChannels & \
                                                                           maccore_globals->gpMacCore_pScanState->scanChannels;
                //Clear out working set to stop scan
                maccore_globals->gpMacCore_pScanState->scanChannels = 0x0;

                //Trigger cleanup of scan
                MacCore_DoOrphanScan();
            }
            break;
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
        default:
        {
            break;
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void gpMacCore_ScanRequest(gpMacCore_ScanType_t scanType, UInt32 scanChannels, UInt8 scanDuration, UInt8 resultListSize, UInt8* pEdScanResultList, UInt8 stackId)
{
    gpMacCore_GlobalVars_t* maccore_globals = GP_MACCORE_GET_GLOBALS();

    if(scanType == gpMacCore_ScanTypeStop)
    {
        MacCore_StopScan();
        return;
    }

    // param and state checks
    if(GP_MACCORE_CHECK_IF_SCANTYPE_INVALID(scanType))
    {
        GP_LOG_SYSTEM_PRINTF("Scantype: %x invalid",0,scanType);
        gpMacCore_cbScanConfirm(gpMacCore_ResultInvalidParameter, scanType, scanChannels, 0, pEdScanResultList);
        return;
    }

    if(GP_MACCORE_CHECK_IF_SCANDURATION_INVALID(scanDuration))
    {
        GP_LOG_SYSTEM_PRINTF("Scanduration: %x invalid",0,scanDuration);
        gpMacCore_cbScanConfirm(gpMacCore_ResultInvalidParameter, scanType, scanChannels, 0, pEdScanResultList);
        return;
    }

    if(maccore_globals->gpMacCore_pScanState != NULL)
    {
        GP_LOG_PRINTF("Transaction overflow for scan request", 0);
        gpMacCore_cbScanConfirm(gpMacCore_ResultScanInProgress, scanType, scanChannels, 0, pEdScanResultList);
        return;
    }

    maccore_globals->gpMacCore_pScanState = GP_POOLMEM_MALLOC(sizeof(gpMacCore_ScanState_t));
    if(maccore_globals->gpMacCore_pScanState == NULL)
    {
        GP_ASSERT_SYSTEM(false);
        return;
    }

    maccore_globals->gpMacCore_pScanState->stackId = stackId;
    maccore_globals->gpMacCore_pScanState->origPhyChannel     = gpRxArbiter_GetStackChannel(stackId);
    //Working set of channels
    maccore_globals->gpMacCore_pScanState->scanChannels       = ((((UInt16)scanChannels)>>11)+((((UInt16)(scanChannels>>16)))<<5));
    //Initial request
    maccore_globals->gpMacCore_pScanState->requestChannels    = maccore_globals->gpMacCore_pScanState->scanChannels;
    //Unscanned tracker to return in Confirm
    maccore_globals->gpMacCore_pScanState->unscannedChannels  = maccore_globals->gpMacCore_pScanState->requestChannels;
    maccore_globals->gpMacCore_pScanState->scanDurationUc     = (scanDuration==0xff)?(0):(((UInt32)GP_MACCORE_BASE_SUPERFRAME_DURATION *((UInt32)(((UInt16)1)<<scanDuration)+1))*GP_MACCORE_SYMBOL_DURATION);
    maccore_globals->gpMacCore_pScanState->resultListSizeMax  = resultListSize;
    maccore_globals->gpMacCore_pScanState->currentScanType    = scanType;

    switch(scanType)
    {
#if defined (GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)
        case gpMacCore_ScanTypeED:
        {
            if (DIVERSITY_SCAN_ED_ORIGINATOR())
            {
                maccore_globals->gpMacCore_pScanState->pEnergyDetectList = pEdScanResultList;
                MacCore_DoEDScan();
                return;
            }
            break;
        }
        case gpMacCore_ScanTypeRSSI:
        case gpMacCore_ScanTypeRSSIMasked:
        case gpMacCore_ScanTypeAI:
        case gpMacCore_ScanTypeAIMasked:
        {
            if (DIVERSITY_SCAN_ED_ORIGINATOR())
            {
                maccore_globals->gpMacCore_pScanState->pEnergyDetectList = pEdScanResultList;
                MacCore_DoIteratedScan();
                return;
            }
            break;
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
#if defined (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
        case gpMacCore_ScanTypeActive:
        {
            if (DIVERSITY_SCAN_ACTIVE_ORIGINATOR())
            {
                /*
                 * This implementation differs from the spec. According to (a hack in) the spec, we should set the Pan id to 0xFFFF before starting an active scan
                 * (and because of this setting, all incoming beacons are accepted, because there is no Pan id checking when our pan Id is 0xFFFF).
                 * However, what we do in the code is just disabling source pan checking for incoming beacons, during the period of an active scan.
                 * This is a cleaner approach than what is described in the spec, but the final result is the same. (see also SW-1899).
                */
                maccore_globals->gpMacCore_pScanState->resultListSize = 0;
                MacCore_PrepareScanVars(GP_MACCORE_FRAME_TYPE_FILTER_MASK_ONLY_BCN);
                gpHal_SetBeaconSrcPanChecking(false);
                MacCore_DoActiveScan();
                return;
            }
            break;
        }
#endif // (GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)
#if defined (GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
        case gpMacCore_ScanTypeOrphan:
        {
            if (DIVERSITY_SCAN_ORPHAN_ORIGINATOR())
            {
                MacCore_PrepareScanVars(GP_MACCORE_FRAME_TYPE_FILTER_MASK_ONLY_CMD);
                MacCore_DoOrphanScan();
                return;
            }
            break;
        }
#endif // GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
        default:
        {
            break;
        }
    }

    GP_LOG_SYSTEM_PRINTF("Scantype: %x not supported", 0, scanType);
    gpMacCore_cbScanConfirm(gpMacCore_ResultInvalidParameter, scanType, scanChannels, 0, pEdScanResultList);
}

#ifdef GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
gpMacCore_Result_t gpMacCore_SetMinInterferenceLevels(Int8* pInterferenceLevels)
{
    gpMacCore_Result_t result = gpMacCore_ResultInvalidParameter;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    if (pInterferenceLevels)
    {
        MEMCPY(maccore_constglobals->MacCore_MinInterferenceLevels, pInterferenceLevels, GP_MACCORE_NUMBER_OF_CHANNELS * sizeof(Int8));
        result = gpMacCore_ResultSuccess;
    }

    return result;
}

void MacCore_ResetMinInterferenceLevels(void)
{
    UIntLoop i;
    gpMacCore_ConstGlobalVars_t* maccore_constglobals = GP_MACCORE_GET_GLOBALS_CONST();

    for (i = 0; i < GP_MACCORE_NUMBER_OF_CHANNELS; i++)
    {
        maccore_constglobals->MacCore_MinInterferenceLevels[i] = -127;
    }
}
#endif // #ifdef GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR

