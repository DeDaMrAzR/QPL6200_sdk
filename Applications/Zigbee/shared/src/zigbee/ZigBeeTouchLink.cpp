/*
 * Copyright (c) 2017, Qorvo Inc
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

/** @file "ZigBeeTouchLink.cpp"
 *
 *  source file that provide support to ZigBee touchlink commissioning
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "ZigBeeTouchLink.h"
#include "ZigBeeTouchLink_Callback.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    External data structures
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
#if defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
/*****************************************************************************
 *                    Touchlink configuration overrides
 *****************************************************************************/
const uint8_t CZigBeeClusterTouchlink::nRSSICorrection = 10;
const int8_t CZigBeeClusterTouchlink::nMinimumRSSI = -60;

/**
*   Porting note:
*   If the Touchlink master key is available to you, perform following changes to add:
*    - enable the supportsCertifiedProduct bit to the nSupportedSchemes.
*    - fill in the CQorvoClusterTouchLinkCommissioningSecurity::abMasterKey[] with the correct key.
*/
//Support scheme selection
// Supported keys
const uint16_t CZigBeeClusterTouchlink::nSupportedSchemes =
    // for Development phase
    CZigBeeClusterTouchlink::supportsDevelopment |
    // for a Certification phase
    CZigBeeClusterTouchlink::supportsPrecertification
    // |
    // for a Product
    // CZigBeeClusterTouchlink::supportsCertifiedProduct
    ;

//Touchlink Master key - to be overruled
const unsigned char CQorvoClusterTouchLinkCommissioningSecurity::abMasterKey[16] = {
    0x9f, 0x55, 0x95, 0xf1, 0x02, 0x57, 0xc8, 0xa4, 0x69, 0xcb, 0xf4, 0x2b, 0xc9, 0x3f, 0xee, 0x31};

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
/**   network key override for touchlink commissioning(initiator)
*/
/*const unsigned char CQorvoClusterTouchLinkCommissioningClient::
    abNetworkKey[16] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,0x00};*/

/**   TouchLink scan channel set override for touchlink commissioning(initiator)
*/
/*const unsigned char CQorvoClusterTouchLinkCommissioningClient::
    abScanChannelSet[8] = {11,11,15,15,20,20,25,25}; */

#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#endif //defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)

/*****************************************************************************
 *                    Class implementation
 *****************************************************************************/
#if defined(GP_APP_DIVERSITY_TOUCHLINK)
/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterExTouchlink - overrided class for touchlink(Target)

CExTouchlinkApplication::CExTouchlinkApplication(CZigBeeFoundationApplication& application) : CZigBeeClusterTouchlink(application, enable | enableRecommissioning | enableFactoryReset | enableOnCentralizedNetworks)
{
}

/*****************************************************************************
 *                    Public Function
 *****************************************************************************/

void CExTouchlinkApplication::OnSessionEstablished(void)
{
    //session established, update to the application
    UInt32 sessionID = GetSessionID();
    Application_cbTouchlinkSessionUpdate(statusOnSessionEstablished, (void*)&sessionID);
}

void CExTouchlinkApplication::OnSessionTerminated(const bool bExpired)
{
    //session terminated, update session timedout status
    Application_cbTouchlinkSessionUpdate(statusOnSessionTerminated, (void*)&bExpired);
}

void CExTouchlinkApplication::OnApply(const CZigBeeTouchlinkCommissioningInfo& info)
{
    //Touchlink commissioning action applied, i.e., network form/join
    Application_cbTouchlinkSessionUpdate(statusOnApply, (void*)&info);
}

void CExTouchlinkApplication::AfterCommit()
{
    //Restart zigbee network and seize all application layer timers and other activities
    Application_cbTouchlinkSessionUpdate(statusAfterCommit, NULL);
}

#endif //GP_APP_DIVERSITY_TOUCHLINK
