/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpHal.h"
#include "gpHal_MAC.h"
#include "gpHal_DEFS.h"
#include "gpHal_RadioInterrupts_DEFS.h"

#include "hal_SupplyVoltageMonitor.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

//RF  callbacks
static gpHal_DataConfirmCallback_t          gpHal_DataConfirmCallback       = NULL;
static gpHal_DataIndicationCallback_t       gpHal_DataIndicationCallback    = NULL;
gpHal_EDConfirmCallback_t                   gpHal_EDConfirmCallback         = NULL;

//MAC callbacks
gpHal_MacFrameQueued_t                      gpHal_MacFrameQueued = NULL;
gpHal_MacFrameUnqueued_t                    gpHal_MacFrameUnqueued = NULL;


/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void gpHal_SwitchChannel(gpHal_Address_t pbmOptAddress,UInt8 retries);
static void gpHal_DataConfirmInterrupt(UInt8 PBMentry);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void gpHal_SwitchChannel(gpHal_Address_t pbmOptAddress,UInt8 retries)
{
    //Using A/B/C PBM settings to keep track of SW multi-channel behavior
    //A and B to be looped minimal
    UInt8 channelB = GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0B(pbmOptAddress);

    //Only shift through enabled channels - fi just doing SW MAC retries - only channel A enabled
    if(GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0B_EN(pbmOptAddress))
    {
        //Move to channel B -> A
        GP_WB_WRITE_ZB_FORMAT_T_CHANNEL_CH0A(pbmOptAddress,channelB);
        if((retries > 1)
                && GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0C_EN(pbmOptAddress)) //A -> B -> C still to be looped, move C -> B
        {
            UInt8 channelC = GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0C(pbmOptAddress);
            //Move to channel C -> B
            GP_WB_WRITE_ZB_FORMAT_T_CHANNEL_CH0B(pbmOptAddress,channelC);
        }
    }
}

void gpHal_DataConfirmInterrupt(UInt8 PBMentry)
{
    UInt8 result;
    UInt8 ackRequest;
    gpPd_Loh_t pdLoh;
    UInt8 retries;
    Bool rf4ce_mode;

    gpHal_DecrementMacQueueUsage();

    gpHal_Address_t pbmOptAddress = GP_HAL_PBM_ENTRY2ADDR_OPT_BASE(PBMentry);
    // Fetch status
    result = GP_WB_READ_ZB_FORMAT_T_RETURN_CODE(pbmOptAddress);
    //Fetch Ack from options
    ackRequest = GP_WB_READ_ZB_FORMAT_T_ACKED_MODE(pbmOptAddress);

    //SW or HW multichannel ? - SW only channel A enabled
    retries = GP_WB_READ_ZB_FORMAT_T_MAX_FRAME_RETRIES(pbmOptAddress);
    rf4ce_mode = (GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0B_EN(pbmOptAddress) ||
                  GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0C_EN(pbmOptAddress));
    if((!ackRequest && retries) && rf4ce_mode) // rf4ce and channels remaining
    {
        // This block of code is specifically for RF4CE, it will forward the multicast frame
        // (non-acked mode) on all the RF4CE Channels, always. This behaviour is unwanted
        // during other 802.15.4 operation, because it wil cause a broadcast frame to be
        // retried many times on a single channel.
        gpHal_SwitchChannel(pbmOptAddress,retries);
        retries--;
        GP_WB_WRITE_ZB_FORMAT_T_MAX_FRAME_RETRIES(pbmOptAddress, retries);
        gpHal_DataRequest_base(PBMentry);
    }
    else
    {
        if(GP_HAL_IS_DATA_CONFIRM_CALLBACK_REGISTERED())
        {
            UInt8 lastChannelUsed;
            UInt8 offset, length;
            gpHal_Result_t halResult;
            gpHal_SourceIdentifier_t srcId;

            //Get result of TX
            length = GP_WB_READ_ZB_FORMAT_T_FRAME_LEN(pbmOptAddress);
            offset = GP_WB_READ_ZB_FORMAT_T_FRAME_OFFSET(pbmOptAddress);

            /*
             * For compatilibity with k7b and prior.
             */
            halResult = gpHal_ResultSuccess;
            switch(result)
            {
                case GP_WB_ENUM_PBM_RETURN_CODE_TX_SUCCESS:
                    gpHal_StatisticsMacCounters.successTx++;
                    halResult = gpHal_ResultSuccess;    // k7b:GP_WB_ENUM_PBM_RESULT_SUCCESS
                    break;
                case GP_WB_ENUM_PBM_RETURN_CODE_CCA_FAILURE:
                    gpHal_StatisticsMacCounters.failTxChannelAccess++;
                    halResult = gpHal_ResultCCAFailure; // k7b:GP_WB_ENUM_PBM_RESULT_CHANNELACCESSFAILURE
                    break;
                case GP_WB_ENUM_PBM_RETURN_CODE_NO_ACK:
                    gpHal_StatisticsMacCounters.failTxNoAck++;
                    halResult = gpHal_ResultNoAck;      // k7b:GP_WB_ENUM_PBM_RESULT_NOACK
                    break;
                case GP_WB_ENUM_PBM_RETURN_CODE_TX_ABORTED:
                    gpHal_StatisticsMacCounters.failTxChannelAccess++;
                    halResult = gpHal_ResultGrantTimeout;      // k7b:GP_WB_ENUM_PBM_RESULT_NOACK
                    break;
                case GP_WB_ENUM_PBM_RETURN_CODE_NO_GRANT:
                    gpHal_StatisticsMacCounters.failTxChannelAccess++;
                    halResult = gpHal_ResultGrantTimeout;
                    break;
                default:
                    GP_ASSERT_SYSTEM(false);
            }

            //Fetch the channel the packet was sent on
            srcId = GP_WB_READ_ZB_FORMAT_T_CHANNEL_IDX(pbmOptAddress);
            lastChannelUsed = gpHal_GetRxChannel(srcId);


            gpHal_StatisticsMacCounters.txRetries+= GP_WB_READ_ZB_FORMAT_T_RETRY_CNT(pbmOptAddress) ;
            gpPd_cbDataConfirm(PBMentry, offset, length, &pdLoh);
            if (gpHal_ResultSuccess == halResult)
            {
                hal_SupplyVoltageMonitorAnnounceTxFinished();
            }
            if (GP_HAL_IS_FRAME_UNQUEUED_CALLBACK_REGISTERED())
            {
                GP_HAL_CB_FRAME_UNQUEUED();
            }

            GP_HAL_CB_DATA_CONFIRM(halResult, pdLoh, lastChannelUsed);
        }
        else
        {
            GP_ASSERT_SYSTEM(false);
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_RadioInterruptsHandleDataConf0Zb(void)
{
    UInt8 PBMentry = gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_DATA_CNF_0_HEAD_PTR_ADDRESS);

    GP_LOG_PRINTF("c0", 0);
    GP_ASSERT_SYSTEM(GP_HAL_CHECK_PBM_VALID(PBMentry));
    gpHal_DataConfirmInterrupt(PBMentry);
}

void gpHal_RadioInterruptsHandleDataConf1Zb(void)
{
    GP_LOG_PRINTF("c1", 0);

    gpHal_DecrementMacQueueUsage();

    //Get result
    if(GP_HAL_IS_ED_CONFIRM_CALLBACK_REGISTERED())
    {
        UInt8 ed;
        UInt8 scanChannel;
        gpHal_Address_t pbmOptAddress = gpHal_RtRciPop(GP_WB_RT_RCI_DATA_CNF_1_HEAD_PTR_ADDRESS);
        UInt8 PBMentry = gpHal_findPbmHandle(pbmOptAddress);

        GP_ASSERT_SYSTEM(GP_HAL_CHECK_PBM_VALID(PBMentry));
        scanChannel = GP_WB_READ_ZB_FORMAT_T_CHANNEL_CH0A(pbmOptAddress);
        ed = GP_WB_READ_ZB_FORMAT_T_ED_RESULT(pbmOptAddress);
        gpHal_Scan_EDConfirm(scanChannel, ed);
    }
    else
    {
        GP_ASSERT_SYSTEM(false);
    }
}

void gpHal_RadioInterruptsHandleDataIndZb(void)
{
    GP_LOG_PRINTF("i", 0);
    if(GP_HAL_IS_DATA_INDICATION_CALLBACK_REGISTERED())
    {
        gpPd_Loh_t pdLoh;
        UInt8 length, offset  = 0;
        gpHal_RxInfo_t rxInfo;
        gpHal_Address_t pbmOptAddress;
        UInt8 PBMentry;

        //Packet has been received, HW has received and notified SW with interrupt
        gpHal_StatisticsMacCounters.totalRx++;

        MEMSET(&rxInfo, 0, sizeof(rxInfo));


        pbmOptAddress = gpHal_RtRciPop(GP_WB_RT_RCI_DATA_IND_HEAD_PTR_ADDRESS);
        PBMentry = gpHal_findPbmHandle(pbmOptAddress);
        GP_ASSERT_SYSTEM(GP_HAL_CHECK_PBM_VALID(PBMentry));
        offset = GP_WB_READ_FORMAT_R_FRAME_PTR(pbmOptAddress) - GP_HAL_PBM_ENTRY2ADDR_DATA_BASE(PBMentry);
        length = GP_WB_READ_FORMAT_R_FRAME_LEN(pbmOptAddress)  - 2; // 2 is length of FCS (2 octets)
        GP_LOG_PRINTF("fr:%lx pbm:%d o:%d l:%d" , 0, (UInt32)pbmOptAddress, PBMentry, offset, length);
        GP_LOG_PRINTF("i: ch %d lqi %d", 0, gpHal_GetRxedChannel(PBMentry), GP_WB_READ_FORMAT_R_LQI(pbmOptAddress));

        if (length > GPHAL_MAX_15_4_PACKET_LENGTH)
        {
            length = GPHAL_MAX_15_4_PACKET_LENGTH;
        }

#ifdef GP_HAL_DIVERSITY_READ_RECEIVING_ANTENNA
        rxInfo.rxReceivingAntenna = GP_WB_READ_FORMAT_R_ANTENNA(pbmOptAddress);
#endif
        gpPd_DataIndication(PBMentry, offset, length, &pdLoh, gpPd_BufferTypeZigBee);
        if (GP_PD_INVALID_HANDLE != pdLoh.handle)
        {
            //Fill in already fetched info by gpPd for compatibility
            rxInfo.rxChannel = gpPd_GetRxChannel(pdLoh.handle);
            GP_HAL_CB_DATA_INDICATION(pdLoh, &rxInfo);
        }
    }
    else
    {
        GP_ASSERT_SYSTEM(false);
    }
}

void gpHal_RadioInterruptsHandleTimedRxDoneZb(void)
{
    UInt8 PBMentry = gpHal_RadioInterruptsGetPbmFromQueueHead(GP_WB_RT_RCI_DATA_CNF_2_HEAD_PTR_ADDRESS);

    GP_LOG_PRINTF("c2", 0);
    GP_ASSERT_SYSTEM(GP_HAL_CHECK_PBM_VALID(PBMentry));
    gpHal_HandleTimedRxDoneZb(PBMentry);
}

//RF register callbacks
void gpHal_RegisterDataConfirmCallback(gpHal_DataConfirmCallback_t callback)
{
    gpHal_DataConfirmCallback=callback;
}

void gpHal_RegisterDataIndicationCallback(gpHal_DataIndicationCallback_t callback)
{
    gpHal_DataIndicationCallback=callback;
}

void gpHal_RegisterEDConfirmCallback(gpHal_EDConfirmCallback_t callback)
{
    gpHal_EDConfirmCallback=callback;
}

//MAC callbacks
void gpHal_RegisterMacFrameQueuedCallback(gpHal_MacFrameQueued_t callback)
{
    gpHal_MacFrameQueued = callback;
}

void gpHal_RegisterMacFrameUnqueuedCallback(gpHal_MacFrameUnqueued_t callback)
{
    gpHal_MacFrameUnqueued = callback;
}

