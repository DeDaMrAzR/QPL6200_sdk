/*
 *   Copyright (c) 2023, Qorvo Inc
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
#include "gpHci.h"
#include "gpHci_SubeventsLeMeta.h"
#include "gpLog.h"

/*****************************************************************************
 *                    Typedef Definition
 *****************************************************************************/

/*****************************************************************************
*                    Static Functions Declaration
*****************************************************************************/

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_HCI

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

Bool gpHci_LeMetaEvent(gpHci_EventCode_t eventCode, gpHci_LeMetaEventParams_t* pBuf)
{
    Bool txSuccess = false;

    switch(pBuf->subEventCode)
    {
        case gpHci_LeMetaSubEventCodeConnectionComplete:
        {
            txSuccess = gpHci_LeConnectionCompleteEvent((gpHci_LeConnectionCompleteEventParams_t*)&pBuf->params.connectionCompleteEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeAdvertisingReport:
        {
            txSuccess = gpHci_LeAdvertisingReportEvent((gpHci_LeAdvertisingReportEventParams_t*)&pBuf->params.advertisingReportEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeConnectionUpdateComplete:
        {
            txSuccess = gpHci_LeConnectionUpdateCompleteEvent((gpHci_LeConnectionUpdateCompleteEventParams_t*)&pBuf->params.connectionUpdateCompleteEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeReadRemoteFeaturesComplete:
        {
            txSuccess = gpHci_LeReadRemoteFeaturesCompleteEvent((gpHci_LeReadRemoteFeaturesCompleteEventParams_t*)&pBuf->params.readRemoteFeaturesCompleteEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeLongTermKeyRequest:
        {
            txSuccess = gpHci_LeLongTermKeyRequestEvent((gpHci_LeLongTermKeyRequestEventParams_t*)&pBuf->params.longTermKeyRequestEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeRemoteConnectionParameterRequest:
        {
            txSuccess = gpHci_LeRemoteConnectionParameterRequestEvent((gpHci_LeRemoteConnectionParameterRequestEventParams_t*)&pBuf->params.remoteConnectionParameterRequestEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeDataLengthChange:
        {
            txSuccess = gpHci_LeDataLengthChangeEvent((gpHci_LeDataLengthChangeEventParams_t*)&pBuf->params.dataLengthChangeEvent);
            break;
        }
        case gpHci_LeMetaSubEventCodeDirectAdvertisingReport:
        {
            txSuccess = gpHci_LeDirectAdvertisingReportEvent((gpHci_LeDirectAdvertisingReportEventParams_t*)&pBuf->params.directAdvertisingReportEvent);
            break;
        }
#if defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)
        case gpHci_LeMetaSubEventCodePhyUpdateComplete:
        {
            txSuccess = gpHci_LePhyUpdateCompleteEvent((gpHci_LePhyUpdateCompleteEventParams_t*)&pBuf->params.phyUpdateCompleteEvent);
            break;
        }
#endif /* defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED) */
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
        case gpHci_LeMetaSubEventCodeConnectionlessIqReport:
        {
            txSuccess = gpHci_LeConnectionlessIqReportEvent((gpHci_LeConnectionlessIqReportEventParams_t*)&pBuf->params.connectionlessIqReportEvent);
            break;
        }
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
        case gpHci_LeMetaSubEventCodeConnectionIqReport:
        {
            txSuccess = gpHci_LeConnectionIqReportEvent((gpHci_LeConnectionIqReportEventParams_t*)&pBuf->params.connectionIqReportEvent);
            break;
        }
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
        case gpHci_LeMetaSubEventCodeCteRequestFailed:
        {
            txSuccess = gpHci_LeCteRequestFailedEvent((gpHci_LeCteRequestFailedEventParams_t*)&pBuf->params.cteRequestFailedEvent);
            break;
        }
#endif /* defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED) */
        default:
        {
            GP_LOG_SYSTEM_PRINTF("LEMetaHandler: unknown LEMeta subcode 0x%lx", 2, (long unsigned int)pBuf->subEventCode);
            break;
        }
    }

    return txSuccess;
}
