/*
 *   Copyright (c) 2015-2016, GreenPeak Technologies
 *   Copyright (c) 2017, Qorvo Inc
 *
 *   Host Controller Interface
 *   Wrapper implementation
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 */


/*****************************************************************************
 *                    Includes Definition
 *****************************************************************************/
#include "global.h"
#include "hal.h"
#include "gpUtils.h"
#include "gpLog.h"
#include "gpAssert.h"
#include "gpSched.h"
#include "gpCom.h"

#include "gpBle.h"
#include "gpModule.h"
#include "gpHci.h"
#include "gpPoolMem.h"

/*****************************************************************************
 *                    Typedef Definition
 *****************************************************************************/

/*****************************************************************************
*                    Static Functions Declaration
*****************************************************************************/

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define GP_MODULE_ID GP_MODULE_ID_HCIAPI
#define GP_COMPONENT_ID GP_COMPONENT_ID_HCI

#ifndef GP_HCI_COMM_ID
#define GP_HCI_COMM_ID GP_COM_DEFAULT_COMMUNICATION_ID
#endif

#if defined(GP_HCI_DIVERSITY_GPCOM_SERVER)
#define DATA_REQUEST(len,buf,commId)   GP_COM_DATA_REQUEST(len,buf,commId)
#endif

/* <CodeGenerator Placeholder> AdditionalMacros */
#define UINT8_SAFE_SUB(a,b) do{     \
    if(b>a)                         \
    {                               \
        GP_LOG_SYSTEM_PRINTF("going negative line %d a %d b %d ",2, __LINE__, a, b); \
        GP_ASSERT_DEV_EXT(0);       \
    }                               \
    else                            \
    {                               \
        a-=b;                       \
    }                               \
}while(0)

// 45 bytes: ||evcode (1)|length (1)|subevcode (1)|num_reports (1)|event_type (1)|address_type (1)|address (6)|data_length (1)|data (31)|rssi (1)||
// Limited to 45 bytes, because we only allow up to 1 full report in an event.
#define GPHCI_ADVERTISEMENT_BUFSIZE_NORMAL (1 + 1 + 1 + 1 + 1 + 1 + 6 + 1 + 31 + 1)
// 45 bytes: ||evcode (1)|length (1)|subevcode (1)|num_reports (1)|event_type (1)|address_type (1)|address (6)|direct_address_type (1)|direct_address (6)|rssi (1)||
// Limited to 45 bytes, because we only allow up to 1 full report in an event.
#define GPHCI_ADVERTISEMENT_BUFSIZE_DIRECT (1 + 1 + 1 + 1 + 1 + 1 + 6 + 1 + 6 + 1)

/* </CodeGenerator Placeholder> AdditionalMacros */
/*****************************************************************************
*                    Static Data
*****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    gpHci_SubeventsLeMeta Indications
 *****************************************************************************/

Bool gpHci_LeConnectionCompleteEvent(gpHci_LeConnectionCompleteEventParams_t* connectionComplete)
{
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (1 + 2*1 + 1 + 1 + 6*1 + 2*1 + 2*1 + 2*1 + 1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define connectionCompletePacket                            (&dataBuf[1 + 1 + 1 + 0])
#define connectionCompletePacket_status                     dataBuf[1 + 1 + 1 + 0]
#define connectionCompletePacket_connectionHandle           dataBuf[1 + 1 + 1 + 0 + 1]
#define connectionCompletePacket_role                       dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1]
#define connectionCompletePacket_peerAddressType            dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1]
#define connectionCompletePacket_peerAddress                dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1 + 1]
#define connectionCompletePacket_connInterval               dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1 + 1 + (6) * 1]
#define connectionCompletePacket_connLatency                dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1 + 1 + (6) * 1 + (2) * 1]
#define connectionCompletePacket_supervisionTo              dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1 + 1 + (6) * 1 + (2) * 1 + (2) * 1]
#define connectionCompletePacket_masterClockAccuracy        dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1 + 1 + (6) * 1 + (2) * 1 + (2) * 1 + (2) * 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&connectionComplete->connectionHandle);
    HOST_TO_LITTLE_UINT16(&connectionComplete->connInterval);
    HOST_TO_LITTLE_UINT16(&connectionComplete->connLatency);
    HOST_TO_LITTLE_UINT16(&connectionComplete->supervisionTo);
    connectionCompletePacket_status = connectionComplete->status;
    MEMCPY(&(connectionCompletePacket_connectionHandle), (UInt8*)&(connectionComplete->connectionHandle), (2) * 1);
    connectionCompletePacket_role = connectionComplete->role;
    connectionCompletePacket_peerAddressType = connectionComplete->peerAddressType;
    MEMCPY(&(connectionCompletePacket_peerAddress), (UInt8*)&(connectionComplete->peerAddress), (6) * 1);
    MEMCPY(&(connectionCompletePacket_connInterval), (UInt8*)&(connectionComplete->connInterval), (2) * 1);
    MEMCPY(&(connectionCompletePacket_connLatency), (UInt8*)&(connectionComplete->connLatency), (2) * 1);
    MEMCPY(&(connectionCompletePacket_supervisionTo), (UInt8*)&(connectionComplete->supervisionTo), (2) * 1);
    connectionCompletePacket_masterClockAccuracy = connectionComplete->masterClockAccuracy;
    LITTLE_TO_HOST_UINT16(&connectionComplete->connectionHandle);
    LITTLE_TO_HOST_UINT16(&connectionComplete->connInterval);
    LITTLE_TO_HOST_UINT16(&connectionComplete->connLatency);
    LITTLE_TO_HOST_UINT16(&connectionComplete->supervisionTo);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeConnectionComplete;
    lengthEvent = 19;

    // Transmit packet
    retVal = DATA_REQUEST(21, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef eventCode
#undef lengthEvent
#undef subEventCode
#undef connectionCompletePacket
#undef connectionCompletePacket_status
#undef connectionCompletePacket_connectionHandle
#undef connectionCompletePacket_role
#undef connectionCompletePacket_peerAddressType
#undef connectionCompletePacket_peerAddress
#undef connectionCompletePacket_connInterval
#undef connectionCompletePacket_connLatency
#undef connectionCompletePacket_supervisionTo
#undef connectionCompletePacket_masterClockAccuracy
}

Bool gpHci_LeAdvertisingReportEvent(gpHci_LeAdvertisingReportEventParams_t* advertisingReport)
{
/* <CodeGenerator Placeholder> gpHci_LeAdvertisingReportEvent_AdditionalManual */
    Bool txSuccess;
    UIntLoop i;
    UInt8 dataBuf[GPHCI_ADVERTISEMENT_BUFSIZE_NORMAL];
    UInt8 *ptr;
    UInt8 *LengthField;
    UInt8 sentinel = GPHCI_ADVERTISEMENT_BUFSIZE_NORMAL;

    // Ref BT4.2 7.7.65.2
    ptr = dataBuf;
    *ptr++ = gpHci_EventCode_LeMeta;
    LengthField = ptr; //remember position of length field
    ptr++;
    *ptr++ = gpHci_LeMetaSubEventCodeAdvertisingReport;
    *ptr++ = advertisingReport->nrOfReports;
    sentinel-=3; // eventCode + subEvent + advertisingReport->nrOfReports
    for(i=0; i<advertisingReport->nrOfReports; i++)
    {
        UINT8_SAFE_SUB(sentinel,sizeof(advertisingReport->reports[i].eventType));
        *ptr++ = advertisingReport->reports[i].eventType;
        UINT8_SAFE_SUB(sentinel,sizeof(advertisingReport->reports[i].addressType));
        *ptr++ = advertisingReport->reports[i].addressType;
        UINT8_SAFE_SUB(sentinel,sizeof(BtDeviceAddress_t));
        MEMCPY(ptr, advertisingReport->reports[i].address.addr, sizeof(BtDeviceAddress_t)); ptr+=sizeof(BtDeviceAddress_t);
        *ptr++ = advertisingReport->reports[i].data.undirected.dataLength;
        UINT8_SAFE_SUB(sentinel,1);
        if (advertisingReport->reports[i].data.undirected.dataLength > 0)
        {
          UINT8_SAFE_SUB(sentinel,advertisingReport->reports[i].data.undirected.dataLength);
          MEMCPY(ptr, advertisingReport->reports[i].data.undirected.data, advertisingReport->reports[i].data.undirected.dataLength);
        }
        ptr += advertisingReport->reports[i].data.undirected.dataLength;
        UINT8_SAFE_SUB(sentinel,sizeof(advertisingReport->reports[i].data.undirected.rssi));
        *ptr++ = advertisingReport->reports[i].data.undirected.rssi;
    }

    *LengthField = ptr - LengthField - 1 /* ptr was also udpated after rssi for last entry  */;
    txSuccess = DATA_REQUEST(2 + (*LengthField), dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);

    return txSuccess;
/* </CodeGenerator Placeholder> gpHci_LeAdvertisingReportEvent_AdditionalManual */
}

Bool gpHci_LeConnectionUpdateCompleteEvent(gpHci_LeConnectionUpdateCompleteEventParams_t* LeConnectionUpdateCompleteEvent)
{
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (1 + 2*1 + 2*1 + 2*1 + 2*1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeConnectionUpdateCompleteEventPacket               (&dataBuf[1 + 1 + 1 + 0])
#define LeConnectionUpdateCompleteEventPacket_status        dataBuf[1 + 1 + 1 + 0]
#define LeConnectionUpdateCompleteEventPacket_connectionHandle dataBuf[1 + 1 + 1 + 0 + 1]
#define LeConnectionUpdateCompleteEventPacket_connInterval  dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1]
#define LeConnectionUpdateCompleteEventPacket_connLatency   dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + (2) * 1]
#define LeConnectionUpdateCompleteEventPacket_supervisionTo dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + (2) * 1 + (2) * 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeConnectionUpdateCompleteEvent->connectionHandle);
    HOST_TO_LITTLE_UINT16(&LeConnectionUpdateCompleteEvent->connInterval);
    HOST_TO_LITTLE_UINT16(&LeConnectionUpdateCompleteEvent->connLatency);
    HOST_TO_LITTLE_UINT16(&LeConnectionUpdateCompleteEvent->supervisionTo);
    LeConnectionUpdateCompleteEventPacket_status = LeConnectionUpdateCompleteEvent->status;
    MEMCPY(&(LeConnectionUpdateCompleteEventPacket_connectionHandle), (UInt8*)&(LeConnectionUpdateCompleteEvent->connectionHandle), (2) * 1);
    MEMCPY(&(LeConnectionUpdateCompleteEventPacket_connInterval), (UInt8*)&(LeConnectionUpdateCompleteEvent->connInterval), (2) * 1);
    MEMCPY(&(LeConnectionUpdateCompleteEventPacket_connLatency), (UInt8*)&(LeConnectionUpdateCompleteEvent->connLatency), (2) * 1);
    MEMCPY(&(LeConnectionUpdateCompleteEventPacket_supervisionTo), (UInt8*)&(LeConnectionUpdateCompleteEvent->supervisionTo), (2) * 1);
    LITTLE_TO_HOST_UINT16(&LeConnectionUpdateCompleteEvent->connectionHandle);
    LITTLE_TO_HOST_UINT16(&LeConnectionUpdateCompleteEvent->connInterval);
    LITTLE_TO_HOST_UINT16(&LeConnectionUpdateCompleteEvent->connLatency);
    LITTLE_TO_HOST_UINT16(&LeConnectionUpdateCompleteEvent->supervisionTo);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeConnectionUpdateComplete;
    lengthEvent = 10;

    // Transmit packet
    retVal = DATA_REQUEST(12, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef eventCode
#undef lengthEvent
#undef subEventCode
#undef LeConnectionUpdateCompleteEventPacket
#undef LeConnectionUpdateCompleteEventPacket_status
#undef LeConnectionUpdateCompleteEventPacket_connectionHandle
#undef LeConnectionUpdateCompleteEventPacket_connInterval
#undef LeConnectionUpdateCompleteEventPacket_connLatency
#undef LeConnectionUpdateCompleteEventPacket_supervisionTo
}

Bool gpHci_LeReadRemoteFeaturesCompleteEvent(gpHci_LeReadRemoteFeaturesCompleteEventParams_t* LeReadRemoteFeaturesCompleteEvent)
{
/* <CodeGenerator Placeholder> gpHci_LeReadRemoteFeaturesCompleteEvent_AdditionalManual */
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (1 + 2*1 + GP_HCI_FEATURE_SET_SIZE)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LEReadRemoteFeaturesCompletePacket              (&dataBuf[1 + 1 + 1 + 0])
#define LEReadRemoteFeaturesCompletePacket_status       dataBuf[1 + 1 + 1 + 0]
#define LEReadRemoteFeaturesCompletePacket_connectionHandle dataBuf[1 + 1 + 1 + 0 + 1]
#define LEReadRemoteFeaturesCompletePacket_features    dataBuf[1 + 1 + 1 + 0 + 1 + 2*1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeReadRemoteFeaturesCompleteEvent->connectionHandle);
    LEReadRemoteFeaturesCompletePacket_status = LeReadRemoteFeaturesCompleteEvent->status;
    MEMCPY(&(LEReadRemoteFeaturesCompletePacket_connectionHandle), (UInt8*)&(LeReadRemoteFeaturesCompleteEvent->connectionHandle), 2*1);
    // Manual
    MEMCPY(&(LEReadRemoteFeaturesCompletePacket_features), (UInt8*)(LeReadRemoteFeaturesCompleteEvent->features), GP_HCI_FEATURE_SET_SIZE);
    LITTLE_TO_HOST_UINT16(&LEReadRemoteFeaturesComplete->connectionHandle);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeReadRemoteFeaturesComplete;
    lengthEvent = 12;

    // Transmit packet
    retVal = DATA_REQUEST(14, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef LEReadRemoteFeaturesCompletePacket
#undef LEReadRemoteFeaturesCompletePacket_status
#undef LEReadRemoteFeaturesCompletePacket_connectionHandle
#undef LEReadRemoteFeaturesCompletePacket_features
/* </CodeGenerator Placeholder> gpHci_LeReadRemoteFeaturesCompleteEvent_AdditionalManual */
}

Bool gpHci_LeLongTermKeyRequestEvent(gpHci_LeLongTermKeyRequestEventParams_t* LeLongTermKeyRequestEvent)
{
/* <CodeGenerator Placeholder> gpHci_LeLongTermKeyRequestEvent_AdditionalManual */
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (2*1 + 8 + 2*1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LELongTermKeyRequestEventPacket                     (&dataBuf[1 + 1 + 1 + 0])
#define LELongTermKeyRequestEventPacket_connectionHandle    dataBuf[1 + 1 + 1 + 0]
#define LELongTermKeyRequestEventPacket_randomNumber        dataBuf[1 + 1 + 1 + 0 + 2*1]
#define LELongTermKeyRequestEventPacket_encryptedDiversifier dataBuf[1 + 1 + 1 + 0 + 2*1 + 8]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeLongTermKeyRequestEvent->connectionHandle);
    HOST_TO_LITTLE_UINT16(&LeLongTermKeyRequestEvent->encryptedDiversifier);
    MEMCPY(&(LELongTermKeyRequestEventPacket_connectionHandle), (UInt8*)&(LeLongTermKeyRequestEvent->connectionHandle), 2*1);
    // Manual
    MEMCPY(&(LELongTermKeyRequestEventPacket_randomNumber), (UInt8*)(LeLongTermKeyRequestEvent->randomNumber), 8);
    MEMCPY(&(LELongTermKeyRequestEventPacket_encryptedDiversifier), (UInt8*)&(LeLongTermKeyRequestEvent->encryptedDiversifier), 2*1);
    LITTLE_TO_HOST_UINT16(&LeLongTermKeyRequestEvent->connectionHandle);
    LITTLE_TO_HOST_UINT16(&LeLongTermKeyRequestEvent->encryptedDiversifier);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeLongTermKeyRequest;
    lengthEvent = 13;

    // Transmit packet
    retVal = DATA_REQUEST(15, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef LELongTermKeyRequestEventPacket
#undef LELongTermKeyRequestEventPacket_connectionHandle
#undef LELongTermKeyRequestEventPacket_randomNumber
#undef LELongTermKeyRequestEventPacket_encryptedDiversifier
/* </CodeGenerator Placeholder> gpHci_LeLongTermKeyRequestEvent_AdditionalManual */
}

Bool gpHci_LeRemoteConnectionParameterRequestEvent(gpHci_LeRemoteConnectionParameterRequestEventParams_t* LeRemoteConnectionParameterRequestEvent)
{
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (2*1 + 2*1 + 2*1 + 2*1 + 2*1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeRemoteConnectionParameterRequestEventPacket       (&dataBuf[1 + 1 + 1 + 0])
#define LeRemoteConnectionParameterRequestEventPacket_connectionHandle dataBuf[1 + 1 + 1 + 0]
#define LeRemoteConnectionParameterRequestEventPacket_connIntervalMin dataBuf[1 + 1 + 1 + 0 + (2) * 1]
#define LeRemoteConnectionParameterRequestEventPacket_connIntervalMax dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1]
#define LeRemoteConnectionParameterRequestEventPacket_connLatency dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1 + (2) * 1]
#define LeRemoteConnectionParameterRequestEventPacket_supervisionTimeout dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1 + (2) * 1 + (2) * 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeRemoteConnectionParameterRequestEvent->connectionHandle);
    HOST_TO_LITTLE_UINT16(&LeRemoteConnectionParameterRequestEvent->connIntervalMin);
    HOST_TO_LITTLE_UINT16(&LeRemoteConnectionParameterRequestEvent->connIntervalMax);
    HOST_TO_LITTLE_UINT16(&LeRemoteConnectionParameterRequestEvent->connLatency);
    HOST_TO_LITTLE_UINT16(&LeRemoteConnectionParameterRequestEvent->supervisionTimeout);
    MEMCPY(&(LeRemoteConnectionParameterRequestEventPacket_connectionHandle), (UInt8*)&(LeRemoteConnectionParameterRequestEvent->connectionHandle), (2) * 1);
    MEMCPY(&(LeRemoteConnectionParameterRequestEventPacket_connIntervalMin), (UInt8*)&(LeRemoteConnectionParameterRequestEvent->connIntervalMin), (2) * 1);
    MEMCPY(&(LeRemoteConnectionParameterRequestEventPacket_connIntervalMax), (UInt8*)&(LeRemoteConnectionParameterRequestEvent->connIntervalMax), (2) * 1);
    MEMCPY(&(LeRemoteConnectionParameterRequestEventPacket_connLatency), (UInt8*)&(LeRemoteConnectionParameterRequestEvent->connLatency), (2) * 1);
    MEMCPY(&(LeRemoteConnectionParameterRequestEventPacket_supervisionTimeout), (UInt8*)&(LeRemoteConnectionParameterRequestEvent->supervisionTimeout), (2) * 1);
    LITTLE_TO_HOST_UINT16(&LeRemoteConnectionParameterRequestEvent->connectionHandle);
    LITTLE_TO_HOST_UINT16(&LeRemoteConnectionParameterRequestEvent->connIntervalMin);
    LITTLE_TO_HOST_UINT16(&LeRemoteConnectionParameterRequestEvent->connIntervalMax);
    LITTLE_TO_HOST_UINT16(&LeRemoteConnectionParameterRequestEvent->connLatency);
    LITTLE_TO_HOST_UINT16(&LeRemoteConnectionParameterRequestEvent->supervisionTimeout);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeRemoteConnectionParameterRequest;
    lengthEvent = 11;

    // Transmit packet
    retVal = DATA_REQUEST(13, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef eventCode
#undef lengthEvent
#undef subEventCode
#undef LeRemoteConnectionParameterRequestEventPacket
#undef LeRemoteConnectionParameterRequestEventPacket_connectionHandle
#undef LeRemoteConnectionParameterRequestEventPacket_connIntervalMin
#undef LeRemoteConnectionParameterRequestEventPacket_connIntervalMax
#undef LeRemoteConnectionParameterRequestEventPacket_connLatency
#undef LeRemoteConnectionParameterRequestEventPacket_supervisionTimeout
}

Bool gpHci_LeDataLengthChangeEvent(gpHci_LeDataLengthChangeEventParams_t* LeMetaDataLengthChangeEvent)
{
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (2*1 + 2*1 + 2*1 + 2*1 + 2*1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeMetaDataLengthChangeEventPacket                   (&dataBuf[1 + 1 + 1 + 0])
#define LeMetaDataLengthChangeEventPacket_connectionHandle  dataBuf[1 + 1 + 1 + 0]
#define LeMetaDataLengthChangeEventPacket_MaxTxOctets       dataBuf[1 + 1 + 1 + 0 + (2) * 1]
#define LeMetaDataLengthChangeEventPacket_MaxTxTime         dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1]
#define LeMetaDataLengthChangeEventPacket_MaxRxOctets       dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1 + (2) * 1]
#define LeMetaDataLengthChangeEventPacket_MaxRxTime         dataBuf[1 + 1 + 1 + 0 + (2) * 1 + (2) * 1 + (2) * 1 + (2) * 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeMetaDataLengthChangeEvent->connectionHandle);
    HOST_TO_LITTLE_UINT16(&LeMetaDataLengthChangeEvent->MaxTxOctets);
    HOST_TO_LITTLE_UINT16(&LeMetaDataLengthChangeEvent->MaxTxTime);
    HOST_TO_LITTLE_UINT16(&LeMetaDataLengthChangeEvent->MaxRxOctets);
    HOST_TO_LITTLE_UINT16(&LeMetaDataLengthChangeEvent->MaxRxTime);
    MEMCPY(&(LeMetaDataLengthChangeEventPacket_connectionHandle), (UInt8*)&(LeMetaDataLengthChangeEvent->connectionHandle), (2) * 1);
    MEMCPY(&(LeMetaDataLengthChangeEventPacket_MaxTxOctets), (UInt8*)&(LeMetaDataLengthChangeEvent->MaxTxOctets), (2) * 1);
    MEMCPY(&(LeMetaDataLengthChangeEventPacket_MaxTxTime), (UInt8*)&(LeMetaDataLengthChangeEvent->MaxTxTime), (2) * 1);
    MEMCPY(&(LeMetaDataLengthChangeEventPacket_MaxRxOctets), (UInt8*)&(LeMetaDataLengthChangeEvent->MaxRxOctets), (2) * 1);
    MEMCPY(&(LeMetaDataLengthChangeEventPacket_MaxRxTime), (UInt8*)&(LeMetaDataLengthChangeEvent->MaxRxTime), (2) * 1);
    LITTLE_TO_HOST_UINT16(&LeMetaDataLengthChangeEvent->connectionHandle);
    LITTLE_TO_HOST_UINT16(&LeMetaDataLengthChangeEvent->MaxTxOctets);
    LITTLE_TO_HOST_UINT16(&LeMetaDataLengthChangeEvent->MaxTxTime);
    LITTLE_TO_HOST_UINT16(&LeMetaDataLengthChangeEvent->MaxRxOctets);
    LITTLE_TO_HOST_UINT16(&LeMetaDataLengthChangeEvent->MaxRxTime);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeDataLengthChange;
    lengthEvent = 11;

    // Transmit packet
    retVal = DATA_REQUEST(13, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef eventCode
#undef lengthEvent
#undef subEventCode
#undef LeMetaDataLengthChangeEventPacket
#undef LeMetaDataLengthChangeEventPacket_connectionHandle
#undef LeMetaDataLengthChangeEventPacket_MaxTxOctets
#undef LeMetaDataLengthChangeEventPacket_MaxTxTime
#undef LeMetaDataLengthChangeEventPacket_MaxRxOctets
#undef LeMetaDataLengthChangeEventPacket_MaxRxTime
}


Bool gpHci_LeDirectAdvertisingReportEvent(gpHci_LeDirectAdvertisingReportEventParams_t* LeDirectAdvertisingReportParams)
{
/* <CodeGenerator Placeholder> gpHci_LeDirectAdvertisingReportEvent_AdditionalManual */
    Bool retVal;
    UIntLoop i;
    UInt8 dataBuf[GPHCI_ADVERTISEMENT_BUFSIZE_DIRECT];
    UInt8 *ptr;
    UInt8 *LengthField;
    UInt8 sentinel = GPHCI_ADVERTISEMENT_BUFSIZE_DIRECT;
    // Ref BT4.2 7.7.65.11
    ptr = dataBuf;
    *ptr++ = gpHci_EventCode_LeMeta;
    LengthField = ptr; //remember position of length field
    ptr++;
    *ptr++ = gpHci_LeMetaSubEventCodeDirectAdvertisingReport;
    *ptr++ = LeDirectAdvertisingReportParams->nrOfReports;
    sentinel-=3; // eventCode + subEvent + LEAdvertisingReportParams->nrOfReports
    for(i=0; i<LeDirectAdvertisingReportParams->nrOfReports; i++)
    {
        UINT8_SAFE_SUB(sentinel,sizeof(LeDirectAdvertisingReportParams->reports[i].eventType));
        *ptr++ = LeDirectAdvertisingReportParams->reports[i].eventType;
        UINT8_SAFE_SUB(sentinel,sizeof(LeDirectAdvertisingReportParams->reports[i].addressType));
        *ptr++ = LeDirectAdvertisingReportParams->reports[i].addressType;
        UINT8_SAFE_SUB(sentinel,sizeof(BtDeviceAddress_t));
        MEMCPY(ptr, LeDirectAdvertisingReportParams->reports[i].address.addr, sizeof(BtDeviceAddress_t)); ptr+=sizeof(BtDeviceAddress_t);
        UINT8_SAFE_SUB(sentinel,sizeof(LeDirectAdvertisingReportParams->reports[i].data.directed.directAddressType));
        *ptr++ = LeDirectAdvertisingReportParams->reports[i].data.directed.directAddressType;
        UINT8_SAFE_SUB(sentinel,sizeof(BtDeviceAddress_t));
        MEMCPY(ptr, LeDirectAdvertisingReportParams->reports[i].data.directed.directAddress.addr, sizeof(BtDeviceAddress_t));
        ptr += sizeof(BtDeviceAddress_t);
        UINT8_SAFE_SUB(sentinel,sizeof(LeDirectAdvertisingReportParams->reports[i].data.directed.rssi));
        *ptr++ = LeDirectAdvertisingReportParams->reports[i].data.directed.rssi;
    }

    *LengthField = ptr - LengthField - 1 /* ptr was also udpated after rssi for last entry  */;
    retVal = DATA_REQUEST(2 + (*LengthField), dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);

    return retVal;
/* </CodeGenerator Placeholder> gpHci_LeDirectAdvertisingReportEvent_AdditionalManual */
}

#if defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)
Bool gpHci_LePhyUpdateCompleteEvent(gpHci_LePhyUpdateCompleteEventParams_t* LePhyUpdateCompleteParams)
{
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (1 + 2*1 + 1 + 1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LePhyUpdateCompleteParamsPacket                     (&dataBuf[1 + 1 + 1 + 0])
#define LePhyUpdateCompleteParamsPacket_status              dataBuf[1 + 1 + 1 + 0]
#define LePhyUpdateCompleteParamsPacket_connectionHandle    dataBuf[1 + 1 + 1 + 0 + 1]
#define LePhyUpdateCompleteParamsPacket_txPhy               dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1]
#define LePhyUpdateCompleteParamsPacket_rxPhy               dataBuf[1 + 1 + 1 + 0 + 1 + (2) * 1 + 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LePhyUpdateCompleteParams->connectionHandle);
    LePhyUpdateCompleteParamsPacket_status = LePhyUpdateCompleteParams->status;
    MEMCPY(&(LePhyUpdateCompleteParamsPacket_connectionHandle), (UInt8*)&(LePhyUpdateCompleteParams->connectionHandle), (2) * 1);
    LePhyUpdateCompleteParamsPacket_txPhy = LePhyUpdateCompleteParams->txPhy;
    LePhyUpdateCompleteParamsPacket_rxPhy = LePhyUpdateCompleteParams->rxPhy;
    LITTLE_TO_HOST_UINT16(&LePhyUpdateCompleteParams->connectionHandle);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodePhyUpdateComplete;
    lengthEvent = 6;

    // Transmit packet
    retVal = DATA_REQUEST(8, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef eventCode
#undef lengthEvent
#undef subEventCode
#undef LePhyUpdateCompleteParamsPacket
#undef LePhyUpdateCompleteParamsPacket_status
#undef LePhyUpdateCompleteParamsPacket_connectionHandle
#undef LePhyUpdateCompleteParamsPacket_txPhy
#undef LePhyUpdateCompleteParamsPacket_rxPhy
}
#endif /* defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED) */









#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeConnectionlessIqReportEvent(gpHci_LeConnectionlessIqReportEventParams_t* LeConnectionlessIqReport)
{
/* <CodeGenerator Placeholder> gpHci_LeConnectionlessIqReportEvent_AdditionalManual */
    Bool retVal;
    UInt8 dataBuf[1 + 1 /* len */+ 1 /* subevtcode */ + 1 /* not needed */ + (2*1 + 1 + 2 + 1 + 1 + 1 + 1 + 2*1 + 1 + 90*(1 + 1))];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeConnectionlessIqReportPacket                      (&dataBuf[1 + 1 + 1 + 0])
#define LeConnectionlessIqReportPacket_syncHandle           dataBuf[1 + 1 + 1 + 0]
#define LeConnectionlessIqReportPacket_channelIndex         dataBuf[1 + 1 + 1 + 0 + 2*1]
#define LeConnectionlessIqReportPacket_rssi                 dataBuf[1 + 1 + 1 + 0 + 2*1 + 1]
#define LeConnectionlessIqReportPacket_rssi_antenna_id      dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2]
#define LeConnectionlessIqReportPacket_cteType              dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1]
#define LeConnectionlessIqReportPacket_slotDurations        dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1 + 1]
#define LeConnectionlessIqReportPacket_packetStatus         dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1 + 1 + 1]
#define LeConnectionlessIqReportPacket_paEventCounter       dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1 + 1 + 1 + 1]
#define LeConnectionlessIqReportPacket_sampleCount          dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1 + 1 + 1 + 1 + 2*1]
#define LeConnectionlessIqReportPacket_iqSamples            dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 2 + 1 + 1 + 1 + 1 + 2*1 + 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeConnectionlessIqReport->syncHandle);
    HOST_TO_LITTLE_UINT16(&LeConnectionlessIqReport->paEventCounter);
    MEMCPY(&(LeConnectionlessIqReportPacket_syncHandle), (UInt8*)&(LeConnectionlessIqReport->syncHandle), 2*1);
    LeConnectionlessIqReportPacket_channelIndex = LeConnectionlessIqReport->channelIndex;
    MEMCPY(&LeConnectionlessIqReportPacket_rssi, (UInt8*)&(LeConnectionlessIqReport->rssi), 2*1);
    LeConnectionlessIqReportPacket_rssi_antenna_id = LeConnectionlessIqReport->rssi_antenna_id;
    LeConnectionlessIqReportPacket_cteType = LeConnectionlessIqReport->cteType;
    LeConnectionlessIqReportPacket_slotDurations = LeConnectionlessIqReport->slotDurations;
    LeConnectionlessIqReportPacket_packetStatus = LeConnectionlessIqReport->packetStatus;
    MEMCPY(&(LeConnectionlessIqReportPacket_paEventCounter), (UInt8*)&(LeConnectionlessIqReport->paEventCounter), 2*1);
    LeConnectionlessIqReportPacket_sampleCount = LeConnectionlessIqReport->sampleCount;
    MEMCPY(&LeConnectionlessIqReportPacket_iqSamples, (UInt8*)&(LeConnectionlessIqReport->iqSamples), 2*LeConnectionlessIqReport->sampleCount);
    LITTLE_TO_HOST_UINT16(&LeConnectionlessIqReport->syncHandle);
    LITTLE_TO_HOST_UINT16(&LeConnectionlessIqReport->paEventCounter);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeConnectionlessIqReport;
    lengthEvent = 13 + 2*LeConnectionlessIqReport->sampleCount;

    // Transmit packet
    retVal = DATA_REQUEST(lengthEvent + 2, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef LeConnectionlessIqReportPacket
#undef LeConnectionlessIqReportPacket_syncHandle
#undef LeConnectionlessIqReportPacket_channelIndex
#undef LeConnectionlessIqReportPacket_rssi
#undef LeConnectionlessIqReportPacket_rssi_antenna_id
#undef LeConnectionlessIqReportPacket_cteType
#undef LeConnectionlessIqReportPacket_slotDurations
#undef LeConnectionlessIqReportPacket_packetStatus
#undef LeConnectionlessIqReportPacket_paEventCounter
#undef LeConnectionlessIqReportPacket_sampleCount
#undef LeConnectionlessIqReportPacket_iqSamples
/* </CodeGenerator Placeholder> gpHci_LeConnectionlessIqReportEvent_AdditionalManual */
}
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */

#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeConnectionIqReportEvent(gpHci_LeConnectionIqReportEventParams_t* LeConnectionIqReport)
{
/* <CodeGenerator Placeholder> gpHci_LeConnectionIqReportEvent_AdditionalManual */
    Bool retVal;
    // we support 2 microsec slots for AoA/AoD: max 90 = 2 * (8+37) I/Q samples
    UInt8 dataBuf[1 + 1 /* len */ + 1 /* subevtcode */ + (2*1 + 1 + 1 + 2 + 1 + 1 + 1 + 1 + 2 + 1 + 90)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeConnectionIqReportPacket                          (&dataBuf[1 + 1 + 1 + 0])
#define LeConnectionIqReportPacket_connectionHandle         dataBuf[1 + 1 + 1 + 0]
#define LeConnectionIqReportPacket_rxPhy                    dataBuf[1 + 1 + 1 + 0 + 2*1]
#define LeConnectionIqReportPacket_dataChannelIndex         dataBuf[1 + 1 + 1 + 0 + 2*1 + 1]
#define LeConnectionIqReportPacket_rssi                     dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1]
#define LeConnectionIqReportPacket_rssi_antenna_id          dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2]
#define LeConnectionIqReportPacket_cteType                  dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1]
#define LeConnectionIqReportPacket_slotDurations            dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1 + 1]
#define LeConnectionIqReportPacket_packetStatus             dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1 + 1 + 1]
#define LeConnectionIqReportPacket_connEventCounter         dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1 + 1 + 1 + 1]
#define LeConnectionIqReportPacket_sampleCount              dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1 + 1 + 1 + 1 + 2]
#define LeConnectionIqReportPacket_iqSamples                dataBuf[1 + 1 + 1 + 0 + 2*1 + 1 + 1 + 2 + 1 + 1 + 1 + 1 + 2 + 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeConnectionIqReport->connectionHandle);
    MEMCPY(&(LeConnectionIqReportPacket_connectionHandle), (UInt8*)&(LeConnectionIqReport->connectionHandle), 2*1);
    LeConnectionIqReportPacket_rxPhy = LeConnectionIqReport->rxPhy;
    LeConnectionIqReportPacket_dataChannelIndex = LeConnectionIqReport->dataChannelIndex;
    MEMCPY(&LeConnectionIqReportPacket_rssi, (UInt8*)&(LeConnectionIqReport->rssi), 2*1);
    LeConnectionIqReportPacket_rssi_antenna_id = LeConnectionIqReport->rssi_antenna_id;
    LeConnectionIqReportPacket_cteType = LeConnectionIqReport->cteType;
    LeConnectionIqReportPacket_slotDurations = LeConnectionIqReport->slotDurations;
    LeConnectionIqReportPacket_packetStatus = LeConnectionIqReport->packetStatus;
    MEMCPY(&LeConnectionIqReportPacket_connEventCounter, (UInt8*)&(LeConnectionIqReport->connEventCounter), 2*1);
    LeConnectionIqReportPacket_sampleCount = LeConnectionIqReport->sampleCount;
    MEMCPY(&LeConnectionIqReportPacket_iqSamples, (UInt8*)&(LeConnectionIqReport->iqSamples), 2*LeConnectionIqReport->sampleCount);
    LITTLE_TO_HOST_UINT16(&LeConnectionIqReport->connectionHandle);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeConnectionIqReport;
    lengthEvent = 2*LeConnectionIqReport->sampleCount + 16 - 2;

    // Transmit packet
    retVal = DATA_REQUEST(lengthEvent + 2, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef LeConnectionIqReportPacket
#undef LeConnectionIqReportPacket_connectionHandle
#undef LeConnectionIqReportPacket_rxPhy
#undef LeConnectionIqReportPacket_dataChannelIndex
#undef LeConnectionIqReportPacket_rssi
#undef LeConnectionIqReportPacket_rssi_antenna_id
#undef LeConnectionIqReportPacket_cteType
#undef LeConnectionIqReportPacket_slotDurations
#undef LeConnectionIqReportPacket_packetStatus
#undef LeConnectionIqReportPacket_sampleCount
#undef LeConnectionIqReportPacket_iqSamples
/* </CodeGenerator Placeholder> gpHci_LeConnectionIqReportEvent_AdditionalManual */
}
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */

#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeCteRequestFailedEvent(gpHci_LeCteRequestFailedEventParams_t* LeCteRequestFailed)
{
/* <CodeGenerator Placeholder> gpHci_LeCteRequestFailedEvent_AdditionalManual */
    Bool retVal;
    UInt8 dataBuf[1 + 1 + 1 + 1 + (1 + 2*1)];

// Defines for offset of members in packet
#define eventCode                  dataBuf[0]
#define lengthEvent                dataBuf[1]
#define subEventCode               dataBuf[2]
#define LeCteRequestFailedPacket                            (&dataBuf[1 + 1 + 1 + 0])
#define LeCteRequestFailedPacket_status                     dataBuf[1 + 1 + 1 + 0]
#define LeCteRequestFailedPacket_connectionHandle           dataBuf[1 + 1 + 1 + 0 + 1]

    // Serialize payload
    HOST_TO_LITTLE_UINT16(&LeCteRequestFailed->connectionHandle);
    LeCteRequestFailedPacket_status = LeCteRequestFailed->status;
    MEMCPY(&(LeCteRequestFailedPacket_connectionHandle), (UInt8*)&(LeCteRequestFailed->connectionHandle), 2*1);
    LITTLE_TO_HOST_UINT16(&LeCteRequestFailed->connectionHandle);

    // Serialize header
    eventCode = gpHci_EventCode_LeMeta;
    subEventCode = gpHci_LeMetaSubEventCodeCteRequestFailed;
    lengthEvent = 4;

    // Transmit packet
    retVal = DATA_REQUEST(6, dataBuf, GP_HCI_COMM_ID | GP_COM_COMM_ID_HW_4);
    return retVal;

#undef LeCteRequestFailedPacket
#undef LeCteRequestFailedPacket_status
#undef LeCteRequestFailedPacket_connectionHandle
/* </CodeGenerator Placeholder> gpHci_LeCteRequestFailedEvent_AdditionalManual */
}
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */








