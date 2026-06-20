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
 */

/** @file "gpHci_SubeventsLeMeta.h"
 *
 *  Host Controller Interface
 *
 *  Declarations of the public functions and enumerations of gpHci_SubeventsLeMeta.
*/

#ifndef _GPHCI_SUBEVENTSLEMETA_H_
#define _GPHCI_SUBEVENTSLEMETA_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpHci_types.h"

/* <CodeGenerator Placeholder> AdditionalIncludes */
#include "gpHci_Includes.h"
/* </CodeGenerator Placeholder> AdditionalIncludes */

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

// List of all subeventcodes
#define gpHci_LeMetaSubEventCodeConnectionComplete                       0x01
#define gpHci_LeMetaSubEventCodeAdvertisingReport                        0x02
#define gpHci_LeMetaSubEventCodeConnectionUpdateComplete                 0x03
#define gpHci_LeMetaSubEventCodeReadRemoteFeaturesComplete               0x04
#define gpHci_LeMetaSubEventCodeLongTermKeyRequest                       0x05
#define gpHci_LeMetaSubEventCodeRemoteConnectionParameterRequest         0x06
#define gpHci_LeMetaSubEventCodeDataLengthChange                         0x07
#define gpHci_LeMetaSubEventCodeEnhancedConnectionComplete               0x0A
#define gpHci_LeMetaSubEventCodeDirectAdvertisingReport                  0x0B
#define gpHci_LeMetaSubEventCodePhyUpdateComplete                        0x0C
#define gpHci_LeMetaSubEventCodeExtendedAdvertisingReport                0x0D
#define gpHci_LeMetaSubEventCodePeriodicAdvertisingSyncEstablished       0x0E
#define gpHci_LeMetaSubEventCodePeriodicAdvertisingReport                0x0F
#define gpHci_LeMetaSubEventCodePeriodicAdvertisingSyncLost              0x10
#define gpHci_LeMetaSubEventCodeScanTimeout                              0x11
#define gpHci_LeMetaSubEventCodeAdvertisingSetTerminated                 0x12
#define gpHci_LeMetaSubEventCodeScanRequestReceived                      0x13
#define gpHci_LeMetaSubEventCodeChannelSelectionAlgorithm                0x14
#define gpHci_LeMetaSubEventCodeConnectionlessIqReport                   0x15
#define gpHci_LeMetaSubEventCodeConnectionIqReport                       0x16
#define gpHci_LeMetaSubEventCodeCteRequestFailed                         0x17
#define gpHci_LeMetaSubEventCodePeriodicAdvertisingSyncTransferReceived  0x18
#define gpHci_LeMetaSubEventCodeCisEstablished                           0x19
#define gpHci_LeMetaSubEventCodeCisRequest                               0x1A
#define gpHci_LeMetaSubEventCodeCreateBigComplete                        0x1B
#define gpHci_LeMetaSubEventCodeTerminateBigComplete                     0x1C
#define gpHci_LeMetaSubEventCodeRequestPeerScaComplete                   0x1F
#define gpHci_LeMetaSubEventCodeBigInfoAdvertisingReport                 0x22

/** @union gpHci_LeMetaInfo_t */
typedef union {
    gpHci_LeConnectionCompleteEventParams_t connectionCompleteEvent;
    gpHci_LeAdvertisingReportEventParams_t advertisingReportEvent;
    gpHci_LeConnectionUpdateCompleteEventParams_t connectionUpdateCompleteEvent;
    gpHci_LeReadRemoteFeaturesCompleteEventParams_t readRemoteFeaturesCompleteEvent;
    gpHci_LeLongTermKeyRequestEventParams_t longTermKeyRequestEvent;
    gpHci_LeRemoteConnectionParameterRequestEventParams_t remoteConnectionParameterRequestEvent;
    gpHci_LeDataLengthChangeEventParams_t dataLengthChangeEvent;
    gpHci_LeDirectAdvertisingReportEventParams_t directAdvertisingReportEvent;
#if defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)
    gpHci_LePhyUpdateCompleteEventParams_t phyUpdateCompleteEvent;
#endif //defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
    gpHci_LeConnectionlessIqReportEventParams_t connectionlessIqReportEvent;
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
    gpHci_LeConnectionIqReportEventParams_t connectionIqReportEvent;
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
    gpHci_LeCteRequestFailedEventParams_t cteRequestFailedEvent;
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
} gpHci_LeMetaInfo_t;

/* <CodeGenerator Placeholder> AdditionalTypeDefinitions */
typedef struct {
    gpHci_LeMetaSubEventCode_t     subEventCode;
    gpHci_LeMetaInfo_t             params;
} gpHci_LeMetaEventParams_t;
/* </CodeGenerator Placeholder> AdditionalTypeDefinitions */
/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//Indications
Bool gpHci_LeConnectionCompleteEvent(gpHci_LeConnectionCompleteEventParams_t* connectionComplete);

Bool gpHci_LeAdvertisingReportEvent(gpHci_LeAdvertisingReportEventParams_t* advertisingReport);

Bool gpHci_LeConnectionUpdateCompleteEvent(gpHci_LeConnectionUpdateCompleteEventParams_t* LeConnectionUpdateCompleteEvent);

Bool gpHci_LeReadRemoteFeaturesCompleteEvent(gpHci_LeReadRemoteFeaturesCompleteEventParams_t* LeReadRemoteFeaturesCompleteEvent);

Bool gpHci_LeLongTermKeyRequestEvent(gpHci_LeLongTermKeyRequestEventParams_t* LeLongTermKeyRequestEvent);

Bool gpHci_LeRemoteConnectionParameterRequestEvent(gpHci_LeRemoteConnectionParameterRequestEventParams_t* LeRemoteConnectionParameterRequestEvent);

Bool gpHci_LeDataLengthChangeEvent(gpHci_LeDataLengthChangeEventParams_t* LeMetaDataLengthChangeEvent);


Bool gpHci_LeDirectAdvertisingReportEvent(gpHci_LeDirectAdvertisingReportEventParams_t* LeDirectAdvertisingReportParams);

#if defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)
Bool gpHci_LePhyUpdateCompleteEvent(gpHci_LePhyUpdateCompleteEventParams_t* LePhyUpdateCompleteParams);
#endif //defined(GP_DIVERSITY_BLE_PHY_UPDATE_SUPPORTED)









#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeConnectionlessIqReportEvent(gpHci_LeConnectionlessIqReportEventParams_t* LeConnectionlessIqReport);
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)

#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeConnectionIqReportEvent(gpHci_LeConnectionIqReportEventParams_t* LeConnectionIqReport);
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)

#if defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)
Bool gpHci_LeCteRequestFailedEvent(gpHci_LeCteRequestFailedEventParams_t* LeCteRequestFailed);
#endif //defined(GP_DIVERSITY_DIRECTIONFINDING_SUPPORTED)








#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_GPHCI_SUBEVENTSLEMETA_H_

