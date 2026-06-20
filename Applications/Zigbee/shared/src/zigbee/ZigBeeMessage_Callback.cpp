/*
 * Copyright (c) 2018, Qorvo Inc
 *
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
 *
 */

/** @file "ZigBeeMessage_Callback.cpp"
 *
 * Callback functions of ZigBee's Pre/Post APSDE-DATA Message.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"
#include "hal.h"
#include "gpHal.h"

#include "App.h"
#include "App_Common.h"
#include "ZigBeeMessage_Callback.h"

//#define GP_LOCAL_LOG

/*****************************************************************************
 *                    External data
 *****************************************************************************/

extern CZigBeeApplicationSupport aps;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID         GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Extern Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Application function prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Application function definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    ZigBee Message callback
 *****************************************************************************/
/** @brief callback function when APSDE-DATA.indication received and before process.
 *  Details is in clause 2.2.4.1.3
 */
void Application_cbZigBeePreApsDataIndication(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const unsigned short wProfileID,
    const unsigned short wClusterID, CPacket &asdu,
    const unsigned char bStatus, const bool bSecurityUsed,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    GP_LOG_PRINTF("ZIGBEE APSDE: Pre Indication, src EP:%d, dest EP:%d, Cluster:0x%04x, LQI:%d", 0, source.m_nEndpoint, destination.m_nEndpoint, wClusterID, nLinkQuality);
}

/** @brief callback function when APSDE-DATA.indication received and after processed.
 *  Details is in clause 2.2.4.1.3
 */
void Application_cbZigBeePostApsDataIndication(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const unsigned short wProfileID,
    const unsigned short wClusterID, CPacket &asdu,
    const unsigned char bStatus, const bool bSecurityUsed,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    GP_LOG_PRINTF("ZIGBEE APSDE: Post Indication, src EP:%d, dest EP:%d, Cluster:0x%04x, LQI:%d", 0, source.m_nEndpoint, destination.m_nEndpoint, wClusterID, nLinkQuality);
}

/** @brief callback function when APSDE-DATA.confirm is triggered.
 *  Details is in clause 2.2.4.1.2
 */
void Application_cbZigBeePostApsDataConfirm(const CZigBeeAddress &destination,
    const unsigned char bSourceEndpoint, const unsigned char bStatus,
    const unsigned int nTimestamp, void *const pCookie)
{
    GP_LOG_PRINTF("ZIGBEE APSDE: Post Confirm, dest EP:%d, Status: 0x%x",0,destination.m_nEndpoint ,bStatus);
}
