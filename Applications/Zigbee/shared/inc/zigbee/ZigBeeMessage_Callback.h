/*
 * Copyright (c) 2017, GreenPeak Technologies
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

/** @file "ZigBeeMessage_Callback.h"
 *
 *  Application API
 *
 *  Declarations of the callback functions of ZigBee's Pre/Post APSDE-DATA Message
*/

#ifndef _ZIGBEE_MESSAGE_CALLBACK_H_
#define _ZIGBEE_MESSAGE_CALLBACK_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "global.h"

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

/*****************************************************************************
 *                    Application Callback Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/** @brief callback function when APSDE-DATA.indication received and before process.
 *  Details is in clause 2.2.4.1.3
 */
void Application_cbZigBeePreApsDataIndication(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const unsigned short wProfileID,
    const unsigned short wClusterID, CPacket &asdu,
    const unsigned char bStatus, const bool bSecurityUsed,
    const unsigned char nLinkQuality, const unsigned int nTimestamp);
    
/** @brief callback function when APSDE-DATA.indication received and after processed.
 *  Details is in clause 2.2.4.1.3
 */
void Application_cbZigBeePostApsDataIndication(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const unsigned short wProfileID,
    const unsigned short wClusterID, CPacket &asdu,
    const unsigned char bStatus, const bool bSecurityUsed,
    const unsigned char nLinkQuality, const unsigned int nTimestamp);
    
/** @brief callback function when APSDE-DATA.confirm is triggered.
 *  Details is in clause 2.2.4.1.2
 */
void Application_cbZigBeePostApsDataConfirm(const CZigBeeAddress &destination,
    const unsigned char bSourceEndpoint, const unsigned char bStatus,
    const unsigned int nTimestamp, void *const pCookie);
    
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_ZIGBEE_MESSAGE_CALLBACK_H_
