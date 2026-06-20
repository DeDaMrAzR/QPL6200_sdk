/*
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * gpMacCore.c
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
//#define GP_LOCAL_LOG
// General includes
#include "hal.h"
#include "gpHal.h"
#include "gpPd.h"
#include "gpLog.h"
#include "gpAssert.h"
#include "gpPoolMem.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpMacCore_enumcheck.h"

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT) || defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR) || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)
#include "gpSched.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE

#define MAC_VS_IE_ID     0x00
#define THREAD_OUI_BYTE0 0x9B
#define THREAD_OUI_BYTE1 0xB8
#define THREAD_OUI_BYTE2 0xEA
#define THREAD_VS_IE_ID_LINKMETRICS  0x00

#define THREAD_LINK_METRIC_PDUCOUNT   0x01
#define THREAD_LINK_METRIC_LQI        0x02
#define THREAD_LINK_METRIC_LINKMARGIN 0x04
#define THREAD_LINK_METRIC_RSSI       0x08

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct MacCore_RxPduMetrics_s {
    UInt32 linkMetricConfiguration;
    Bool  dataCleared;
    UInt8 pduCount;
    UInt8 lqiAvg;
    UInt8 rssiMarginAvg;
    UInt8 rssiAvg;
} MacCore_RxPduMetrics_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

MacCore_RxPduMetrics_t MacCore_RxPduMetrics;

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

UInt8 MacCoreGetRawStack(void)
{
    UInt8 i;

    for(i=0; i<DIVERSITY_NR_OF_STACKS; i++)
    {
        if(gpMacCore_GetBeaconPayloadLength(i) == 0xFF)
        {
            return i;
        }
    }
    return 0xFF;
}

#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
STATIC_FUNC void MacCore_UpdateEnhAckVsIe(void)
{
        UInt8 vsIe[8];
        UInt8 index = 1; /* skip the length field now and complete it at the end */
        vsIe[index++] = MAC_VS_IE_ID;
        vsIe[index++] = THREAD_OUI_BYTE0;
        vsIe[index++] = THREAD_OUI_BYTE1;
        vsIe[index++] = THREAD_OUI_BYTE2;
        vsIe[index++] = THREAD_VS_IE_ID_LINKMETRICS;


        if(MacCore_RxPduMetrics.linkMetricConfiguration & THREAD_LINK_METRIC_PDUCOUNT)
        {
            vsIe[index] = MacCore_RxPduMetrics.pduCount;
            index++;
        }
        if(MacCore_RxPduMetrics.linkMetricConfiguration & THREAD_LINK_METRIC_LQI)
        {
            vsIe[index] = MacCore_RxPduMetrics.lqiAvg;
            index++;
        }
        if(MacCore_RxPduMetrics.linkMetricConfiguration & THREAD_LINK_METRIC_RSSI)
        {
            vsIe[index] = MacCore_RxPduMetrics.rssiAvg;
            index++;
        }
        if(MacCore_RxPduMetrics.linkMetricConfiguration & THREAD_LINK_METRIC_LINKMARGIN)
        {
            vsIe[index] = MacCore_RxPduMetrics.rssiMarginAvg;
            index++;
        }

#ifdef GP_LOCAL_LOG
        GP_LOG_SYSTEM_PRINTF("setting vsie len %d config=0x%x", 0, index, (UInt16)(MacCore_RxPduMetrics.linkMetricConfiguration&0xFFFF));
        gpLog_PrintBuffer(index, vsIe);
#endif // GP_LOCAL_LOG

        GP_ASSERT_DEV_INT(index < 9);
        vsIe[0] = index - 2; // subtract the length of the IE header */

        gpHal_SetEnhAckVSIE(index, &vsIe[0]);
}

STATIC_FUNC void MacCore_DisableEnhAckVsIe(void)
{
        gpHal_SetEnhAckVSIE(0, 0);
}

STATIC_FUNC void MacCore_UpdateMetricWeight8(UInt8 *pMetric, UInt8 newSample, Bool dataCleared)
{
    if(dataCleared)
    {
        *pMetric = newSample;
    }
    else
    {
        *pMetric = (UInt8)((((UInt16)(*pMetric)) * 7 + (UInt16)newSample + 4 /* rounding */)/8);
    }
}

STATIC_FUNC UInt8 MacCore_ScaleToUint8(Int16 value, Int16 expectedMinValue, Int16 expectedMaxValue)
{
    UInt8 ret = 0;

    if(value <= expectedMinValue)
    {
        ret = 0;
    }
    else if(value >= expectedMaxValue)
    {
        ret = UINT8_MAX;
    }
    else
    {
        /* Scale the value to use the complete range of a UInt8, for values between the epxected min and max.
         * This formula only works for the case where a small range is expanded to a larger range.
         * This is valid for the relevant use-cases: 0..130 and -130..0 => expand to 0..255.
         * If ever this is not valid anymore, rounding may need to be added to ensure equal usage off all values
         * in the final range.
         */
        ret = (UInt8) ((value - expectedMinValue) * UINT8_MAX / (expectedMaxValue - expectedMinValue));
    }
    GP_LOG_PRINTF("val: %d ret: %d emin: %d emax: %d", 0, value, ret, expectedMinValue, expectedMaxValue);
    return ret;
}

void MacCore_StoreLinkMetrics(MacCore_HeaderDescriptor_t *pmdi, gpPd_Loh_t pdLoh)
{
    if(MacCore_RxPduMetrics.linkMetricConfiguration > 0)
    {
        Int8 signed_rssi, signed_sens_level;
        UInt8 rssi, lqi, rssiMargin;

        MacCore_RxPduMetrics.pduCount++;

        lqi = gpPd_GetLqi(pdLoh.handle);
        MacCore_UpdateMetricWeight8(&(MacCore_RxPduMetrics.lqiAvg),  lqi, MacCore_RxPduMetrics.dataCleared);

        signed_rssi = gpPd_GetRssi(pdLoh.handle);
        /* Thread spec v1.2.1RC2 paragraph 4.11.3.4.3 Link Metrics Data and Sub-TLV Formats */
        rssi = MacCore_ScaleToUint8(signed_rssi, -130, 0);
        MacCore_UpdateMetricWeight8(&(MacCore_RxPduMetrics.rssiAvg),  rssi, MacCore_RxPduMetrics.dataCleared);

        signed_sens_level = gpHal_GetSensitivityLevel();
        if(signed_rssi > signed_sens_level)
        {
            GP_LOG_PRINTF("rssi %d sens %d", 0, signed_rssi, signed_sens_level);
            /* Thread spec v1.2.1RC2 paragraph 4.11.3.4.3 Link Metrics Data and Sub-TLV Formats */
            rssiMargin = MacCore_ScaleToUint8(signed_rssi - signed_sens_level, 0, 130);
        }
        else
        {
            rssiMargin = 0;
        }
        MacCore_UpdateMetricWeight8(&(MacCore_RxPduMetrics.rssiMarginAvg), rssiMargin, MacCore_RxPduMetrics.dataCleared);

        MacCore_UpdateEnhAckVsIe();

        /* now we don't have an empty measurement set anymore */
        MacCore_RxPduMetrics.dataCleared = false;
    }
}
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void MacCore_GetRawModeEncryptionOffsets(gpPd_Loh_t currentPdLoh, UInt16* pAuxOffset, UInt16* pDataOffset, UInt16* pCslIeOffset)
{
    UInt16 frameControl;
    UInt8 currentOffset = 0;

    gpPd_ReadByteStream(currentPdLoh.handle, currentPdLoh.offset, GP_MACCORE_FC_SIZE, (UInt8*) &(frameControl));
    currentOffset += GP_MACCORE_FC_SIZE;
    currentOffset += GP_MACCORE_SN_SIZE;

    if(MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) == gpMacCore_AddressModeShortAddress)
    {
        currentOffset += GP_MACCORE_SHORT_ADDR_SIZE + GP_MACCORE_PANID_SIZE;
    }
    if(MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) == gpMacCore_AddressModeShortAddress)
    {
        currentOffset += GP_MACCORE_SHORT_ADDR_SIZE + GP_MACCORE_PANID_SIZE;
    }
    if(MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) == gpMacCore_AddressModeExtendedAddress)
    {
        currentOffset += GP_MACCORE_EXT_ADDR_SIZE + GP_MACCORE_PANID_SIZE;
    }
    if(MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) == gpMacCore_AddressModeExtendedAddress)
    {
        currentOffset += GP_MACCORE_EXT_ADDR_SIZE + GP_MACCORE_PANID_SIZE;
    }

    if( (MACCORE_FRAMECONTROL_FRAMEVERSION_GET(frameControl) == gpMacCore_MacVersion2003) ||
        (MACCORE_FRAMECONTROL_FRAMEVERSION_GET(frameControl) == gpMacCore_MacVersion2006) )
    {
        if(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(frameControl) == 0x01)
        {
            currentOffset -= GP_MACCORE_PANID_SIZE; /* subtract the size of one of the two panids again */
        }
    }
    else if (MACCORE_FRAMECONTROL_FRAMEVERSION_GET(frameControl) == gpMacCore_MacVersion2015)
    {
        // if no addressing information is present, the pan_id_compression field indicates wheter there is a
        // dst pan_id included
        if((MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) == gpMacCore_AddressModeNoAddress) &&
           (MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) == gpMacCore_AddressModeNoAddress) )
        {
            if(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(frameControl) == 0x01)
            {
                currentOffset += GP_MACCORE_PANID_SIZE; /* add a panid without any other address info */
            }
        }
        // If only either the dst or the src addressing is present, the pan_id_compression field indicates
        // wheter the pan_id of the respective address is NOT present
        else if( ( (MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) != gpMacCore_AddressModeNoAddress) &&
                   (MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) == gpMacCore_AddressModeNoAddress) ) ||
                 ( (MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) == gpMacCore_AddressModeNoAddress) &&
                   (MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) != gpMacCore_AddressModeNoAddress) ) )
        {
            if(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(frameControl) == 0x01)
            {
                currentOffset -= GP_MACCORE_PANID_SIZE; /* subtract the single panid size again */
            }
        }
        // if both addresses are long addresses, src pan_id is never sent. And the dst pan_id is present only
        // when the pan_id_compression is 0
        else if((MACCORE_FRAMECONTROL_DSTADDRMODE_GET(frameControl) == gpMacCore_AddressModeExtendedAddress) &&
                (MACCORE_FRAMECONTROL_SRCADDRMODE_GET(frameControl) == gpMacCore_AddressModeExtendedAddress) )
        {
            currentOffset -= GP_MACCORE_PANID_SIZE;     /* only 1 panid is added, so subtract the previous addition*/
            if(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(frameControl) == 0x01)
            {
                currentOffset -= GP_MACCORE_PANID_SIZE; /* in this case also remove the 2nd panid size */
            }
        }
        // in all other cases, the pan_id_compression field indicates wheter the src pan_id is present or not
        else
        {
            if(MACCORE_FRAMECONTROL_PANCOMPRESSION_GET(frameControl) == 0x01)
            {
                currentOffset -= GP_MACCORE_PANID_SIZE; /* subtract the size of one of the two panids again */
            }
        }
    }
    else
    {
        GP_ASSERT_DEV_EXT(false); /* unknown frame version */
    }

    // store offset to the Aux header in the frame.
    *pAuxOffset = currentPdLoh.offset + currentOffset;

    if(MACCORE_FRAMECONTROL_SECURITY_GET(frameControl))
    {
        UInt8 securityControl;
        UInt8 keyIdMode;
        securityControl = gpPd_ReadByte(currentPdLoh.handle, currentPdLoh.offset + currentOffset);
        currentOffset += sizeof(UInt8);  // security control
        currentOffset += sizeof(UInt32); // framecounter
        keyIdMode = MACCORE_SECCONTROL_KEYIDMODE_GET(securityControl);

        switch(keyIdMode)
        {
            case gpMacCore_KeyIdModeImplicit:       {currentOffset += 0; break;}
            case gpMacCore_KeyIdModeExplicit1Octet: {currentOffset += 1; break;}
            case gpMacCore_KeyIdModeExplicit4Octet: {currentOffset += 5; break;}
            case gpMacCore_KeyIdModeExplicit8Octet: {currentOffset += 9; break;}
            default:                                {GP_ASSERT_DEV_INT(0); break;}
        }
    }

    /* skip past the ie header fields to get to the to-be-encrypted data. */
    if(MACCORE_FRAMECONTROL_IE_PRESENT_GET(frameControl))
    {
        UInt16 ie_header;
        UInt8 ie_length, ie_type, ie_id;

        while(currentOffset + sizeof(UInt16) < currentPdLoh.length)
        {
            gpPd_ReadByteStream(currentPdLoh.handle, currentPdLoh.offset + currentOffset, sizeof(UInt16), (UInt8*)&ie_header);
            ie_length = MACCORE_IEHEADER_LENGTH_GET(ie_header);
            ie_id     = MACCORE_IEHEADER_ELEMENTID_GET(ie_header);
            ie_type   = MACCORE_IEHEADER_TYPE_GET(ie_header);

            if((ie_type == MACCORE_IE_TYPE_HEADER) && (ie_id == MACCORE_IE_ID_CSL))
            {
                /* remember the CLS IE offset */
                *pCslIeOffset = currentPdLoh.offset + currentOffset;
            }

            currentOffset += sizeof(UInt16);
            currentOffset += ie_length;

            if((ie_type == MACCORE_IE_TYPE_HEADER) && ((ie_id == MACCORE_IE_ID_HT1) || (ie_id == MACCORE_IE_ID_HT2)))
            {
                /* we're done with the header IEs. */
                /* exit the loop */
                break;
            }
        }

    }

    /* check if it's a command */
    if(MACCORE_FRAMECONTROL_FRAMETYPE_GET(frameControl) == 0x3)
    {
        // store offset to the to-be-encrypted data in the frame.
        *pDataOffset = currentPdLoh.offset + currentOffset + 1;
    }
    else
    {
        // store offset to the to-be-encrypted data in the frame.
        *pDataOffset = currentPdLoh.offset + currentOffset;
    }
}

gpMacCore_Result_t gpMacCore_ConfigureEnhAckProbing_STACKID(UInt8 linkMetrics, MACAddress_t* pExtendedAddress, UInt16 shortAddress MACCORE_STACKID_ARG_2)
{
#if defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
    /* perform some checks on the validity of the linkMetrics configuration */
    UInt8 supportedMetrics = THREAD_LINK_METRIC_PDUCOUNT | THREAD_LINK_METRIC_LQI | THREAD_LINK_METRIC_RSSI | THREAD_LINK_METRIC_LINKMARGIN;
    UInt8 index, metricCount;

    if(linkMetrics & (~supportedMetrics))
    {
        GP_LOG_PRINTF("unsupported metrics: 0x%lx", 0, linkMetrics);
        return gpMacCore_ResultInvalidParameter;
    }

    /* count the requested metrics */
    metricCount = 0;
    for(index = 0; index < 8*sizeof(UInt8); index++)
    {
        if((linkMetrics >> index) & 0x01)
        {
            metricCount++;
        }
    }

    if(metricCount > 2)
    {
        GP_LOG_PRINTF("too many metrics: 0x%lx", 0, linkMetrics);
        return gpMacCore_ResultInvalidParameter;
    }

    MacCore_RxPduMetrics.linkMetricConfiguration = linkMetrics;
    MacCore_RxPduMetrics.pduCount = 0;
    MacCore_RxPduMetrics.lqiAvg = 0;
    MacCore_RxPduMetrics.rssiMarginAvg = 0;
    MacCore_RxPduMetrics.rssiAvg = 0;

    /* do not perform the averaging when the configuration has changed and the data set is cleared */
    MacCore_RxPduMetrics.dataCleared = true;

    if(linkMetrics>0)
    {
        MacCore_UpdateEnhAckVsIe();
    }
    else
    {
        MacCore_DisableEnhAckVsIe();
    }

    return gpMacCore_ResultSuccess;
#else
    GP_LOG_PRINTF("Link Metrics not enabled", 0);
    return gpMacCore_ResultInvalidParameter;
#endif // defined(GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION)
}


