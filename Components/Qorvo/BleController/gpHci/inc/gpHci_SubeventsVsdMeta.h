/*
 * Copyright (c) 2023, Qorvo Inc
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

/** @file "gpHci_SubeventsVsdMeta.h"
 *
 *  Host Controller Interface
 *
 *  Declarations of the public functions and enumerations of gpHci_SubeventsVsdMeta.
*/

#ifndef _GPHCI_SUBEVENTSVSDMETA_H_
#define _GPHCI_SUBEVENTSVSDMETA_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpHci_types.h"

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
#define gpHci_VsdMetaSubEventCodeLogging                                  0x00
#define gpHci_VsdMetaSubEventCodeForwardProcessedMessages                 0x01
#define gpHci_VsdMetaSubEventCodeFilterAcceptListModified                 0x02
#define gpHci_VsdMetaSubEventCodeAngleUpdateInd                           0x03

/** @union gpHci_LeMetaInfo_t */
typedef union {
    gpHci_VsdForwardEventProcessedMessagesParams_t forwardProcessedMessagesEvent;
    gpHci_VsdFilterAcceptListModifiedEventParams_t filterAcceptListModifiedEvent;
} gpHci_VsdMetaInfo_t;

/* <CodeGenerator Placeholder> AdditionalTypeDefinitions */
typedef struct {
    gpHci_VsdMetaSubEventCode_t    subEventCode;
    gpHci_VsdMetaInfo_t            params;
} gpHci_VsdMetaEventParams_t;
/* </CodeGenerator Placeholder> AdditionalTypeDefinitions */
/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//Indications
Bool gpHci_VsdLogging(void);

Bool gpHci_VsdForwardEventProcessedMessages(gpHci_VsdForwardEventProcessedMessagesParams_t* eventProcessedParams);

Bool gpHci_VsdFilterAcceptListModifiedEvent(gpHci_VsdFilterAcceptListModifiedEventParams_t* filterAcceptListModifiedEvent);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_GPHCI_SUBEVENTSVSDMETA_H_

