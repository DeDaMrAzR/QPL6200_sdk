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

/** @file "ZigBeeTouchLink_Callback.cpp"
 *
 * source code for the callback in ZigBee touchlink commissioning
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_ZB
#include "ZigBee.h"
#endif //GP_APP_DIVERSITY_ZB

#include "gpLog.h"
#include "gpSched.h"

#include "App.h"
#include "App_Common.h"
#include "ZigBeeTouchLink_Callback.h"
#include "ZigBeeCommissioning.h"

/*****************************************************************************
 *                    External data
 *****************************************************************************/

extern CZigBeeApplicationSupport aps;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

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
 *                    static functions
 *****************************************************************************/
static void setRxOnWhenIdle(bool isOn)
{
    unsigned char nState = isOn ? CIEEE802154PHY::transceiverReceiverEnabled : CIEEE802154PHY::transceiverOff;
    // config receiver Rx in phy layer.
    aps.m_nwk.m_mac.m_phy.OnRequestSetTransceiverState((const unsigned char)nState);
    GP_LOG_PRINTF("TouchLink: setRxOnWhenIdle with rx status - %x", 0, nState);

    // stop commissioning and rejoining before stopping touchlink commissioning
    nwk.m_nlme.Reset(true);
    aps.m_apsme.Reset(true);
}
/*****************************************************************************
 *                    Override functions
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
void CQorvoClusterTouchLinkCommissioningClient::OnUpdateCommissioningInfo(const unsigned char nStatus,
                                                                          const CZigBeeAddress& destination)
{
    // reserved
}

void CQorvoClusterTouchLinkCommissioningClient::OnTouchLinkCommissioningStatus(const unsigned char nState)
{
    // Launch callback in Application for Applciation Specific handle
    Application_cbTouchlinkClientCommissioningProgress(nState);
}
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT

/*****************************************************************************
 *                    TouchLink commissioning status Callback
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_TOUCHLINK
/**@brief Updates touchlink session status */
void Application_cbTouchlinkSessionUpdate(uint8_t status, void* pArg)
{
    switch(status)
    {
        case CExTouchlinkApplication::statusOnSessionEstablished:
        {
            GP_LOG_SYSTEM_PRINTF("TouchLink: Scan Request received, sessionID: 0x%08lx", 0, (*(uint32_t*)pArg));
            break;
        }
        case CExTouchlinkApplication::statusOnSessionTerminated:
        {
            Bool bexpired = (*(Bool*)pArg);
            if(bexpired)
            {
                GP_LOG_SYSTEM_PRINTF("TouchLink: session has expired", 0);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("TouchLink: session timed out", 0);
            }
            break;
        }
        case CExTouchlinkApplication::statusOnApply:
        {
            CZigBeeTouchlinkCommissioningInfo info = (*(CZigBeeTouchlinkCommissioningInfo*)pArg);
            GP_LOG_SYSTEM_PRINTF("TouchLink: Applying new network parameters...", 0);
            GP_LOG_SYSTEM_PRINTF("  Channel: %d", 0, info.m_nChannel);
            GP_LOG_SYSTEM_PRINTF("  Network PanID: 0x%04x", 0, info.m_wPANID);
            GP_LOG_SYSTEM_PRINTF("  Device Address: 0x%04x", 0, info.m_wNetworkAddress);
            break;
        }
        case CExTouchlinkApplication::statusAfterCommit:
        {
            GP_LOG_SYSTEM_PRINTF("TouchLink: network parameters applied, restart the stack", 0);

            // Post touchlink commit event
            // -- Stop identify mode
            appsEp_1.SetIdentifyMode(0);

            // -- Reset current zigbee stack
            aps.m_nwk.m_nlme.Reset(true);
            aps.m_apsme.Reset(true);

            // -- Re-initialize stack to start the commited network.
            aps.m_apsme.Startup();

            break;
        }
        default:
            break;
    }
}

#endif //GP_APP_DIVERSITY_TOUCHLINK
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
/** @brief Callback for Touchlink Client commissioning process
*
*   @param status                    Touchlink Client commissioning progress status
*/
void Application_cbTouchlinkClientCommissioningProgress(uint8_t status)
{
    switch(status)
    {
        case CQorvoClusterTouchLinkCommissioningClient::statusStopped:
        {
            // Printout log
            GP_LOG_SYSTEM_PRINTF("TouchLink: TouchLink commissioning Stopped", 0);
            //  disable receiver Rx in phy layer.
            setRxOnWhenIdle(false);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusScanRequest:
        {
            // Printout session ID
            GP_LOG_SYSTEM_PRINTF("TouchLink Client: Scan Request Start, sessionID: 0x%08x", 0, appsEp_1.m_touchLinkClient.m_nTransactionId);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusScanResponseReceived:
        {
            // Printout received response details
            GP_LOG_SYSTEM_PRINTF("TouchLink Client: Scan Response received", 0);
            GP_LOG_SYSTEM_PRINTF("  Target Device Type: %d", 0, appsEp_1.getTouchLinkScannedTargetInfo().m_nDeviceType);
            GP_LOG_SYSTEM_PRINTF("  Target KeyType: %d", 0, appsEp_1.getTouchLinkScannedTargetInfo().wKeyBitMask);
            GP_LOG_SYSTEM_PRINTF("  Target Channel: %d", 0, appsEp_1.getTouchLinkScannedTargetInfo().m_nChannel);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusDetermindedPotentialTarget:
        {
            // Printout received response details
            GP_LOG_SYSTEM_PRINTF("TouchLink Client: Potential Target Determinded", 0);
            GP_LOG_SYSTEM_PRINTF("  Potential Target Address(Ext):", 0);
            // avoid take the address of an rvalue, which is not allowed in C++
            const CQorvoTouchLinkScanResponseData& potentialTargetInfo = appsEp_1.getTouchLinkPotentialTargetInfo();
            gpLog_PrintBuffer(sizeof(appsEp_1.getTouchLinkPotentialTargetInfo().m_qwExtended),
                              (UInt8*)&potentialTargetInfo.m_qwExtended);
            GP_LOG_SYSTEM_PRINTF("  Target KeyType: %d", 0, appsEp_1.getTouchLinkPotentialTargetInfo().keyBitMask);
            GP_LOG_SYSTEM_PRINTF("  Target Channel: %d", 0, appsEp_1.getTouchLinkPotentialTargetInfo().m_scanChannel);
            GP_LOG_SYSTEM_PRINTF("  Target Device Type: %d", 0,
                                 appsEp_1.getTouchLinkPotentialTargetInfo().scanRsp.m_nZigBeeInformation & 0b11);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusTargetStartNetwork:
        {
            // Printout network start request details.
            GP_LOG_SYSTEM_PRINTF("TouchLink Client: Start Target network", 0);
            GP_LOG_SYSTEM_PRINTF("  KeyType: %d", 0, appsEp_1.getTouchLinkTargetInfo().wKeyBitMask);
            GP_LOG_SYSTEM_PRINTF("  Target Address(Ext):", 0);
            const CQorvoTouchLinkClientTargetInfo& targetInfo1 = appsEp_1.getTouchLinkTargetInfo();
            gpLog_PrintBuffer(sizeof(appsEp_1.getTouchLinkTargetInfo().targetAddress.m_qwExtended),
                              (uint8_t*)&targetInfo1.targetAddress.m_qwExtended);
            GP_LOG_SYSTEM_PRINTF("  Set Channel: %d", 0, appsEp_1.getTouchLinkTargetInfo().m_nChannel);
            GP_LOG_SYSTEM_PRINTF("  Set Target PanId: 0x%04x", 0, appsEp_1.getTouchLinkTargetInfo().targetAddress.m_wPANID);
            GP_LOG_SYSTEM_PRINTF("  Set Target Address: 0x%04x", 0, appsEp_1.getTouchLinkTargetInfo().targetAddress.m_wShort);
            GP_LOG_SYSTEM_PRINTF("  Set Initiator Address: 0x%04x", 0,
                                 appsEp_1.getTouchLinkTargetInfo().m_nInitiatorNetworkAddress);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusTargetStartJoining:
        {
            // Printout network join request details.
            GP_LOG_SYSTEM_PRINTF("TouchLink Client: Start Target joining", 0);
            GP_LOG_SYSTEM_PRINTF("  Target Address(Ext):", 0);
            const CQorvoTouchLinkClientTargetInfo& targetInfo2 = appsEp_1.getTouchLinkTargetInfo();
            gpLog_PrintBuffer(sizeof(appsEp_1.getTouchLinkTargetInfo().targetAddress.m_qwExtended),
                              (uint8_t*)&targetInfo2.targetAddress.m_qwExtended);
            GP_LOG_SYSTEM_PRINTF("  Set Target Address: 0x%04x", 0, appsEp_1.getTouchLinkTargetInfo().targetAddress.m_wShort);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusTargetNetworkStarted:
        {
            // Printout received network start response details.
            if(appsEp_1.getTouchLinkTargetInfo().targetStatus == CZigBeeApplicationSupport::statusSuccess)
            {
                GP_LOG_SYSTEM_PRINTF("TouchLink Client: Target network start Success", 0);
                GP_LOG_SYSTEM_PRINTF("  Ext.PanID: 0x%08lx%08lx", 0, appsEp_1.getTouchLinkTargetInfo().qwExtendedPanId.MSB, appsEp_1.getTouchLinkTargetInfo().qwExtendedPanId.LSB);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("TouchLink: Target network start Failed", 0);
            }
            //  disable receiver Rx in phy layer.
            setRxOnWhenIdle(false);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusInitiatorRejoining:
        {
            GP_LOG_SYSTEM_PRINTF("TouchLink: Initiator is rejoining", 0);
            Application_cbTLTriggeredRejoin(true);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusTargetNetworkJoined:
        {
            if(appsEp_1.getTouchLinkTargetInfo().targetStatus == CZigBeeApplicationSupport::statusSuccess)
            {
                // Printout received network join response details.
                GP_LOG_SYSTEM_PRINTF("TouchLink Client: Target join Success", 0);
                GP_LOG_SYSTEM_PRINTF("  network Ext.PanID: 0x%08lx%08lx", 0, appsEp_1.getTouchLinkTargetInfo().qwExtendedPanId.MSB, appsEp_1.getTouchLinkTargetInfo().qwExtendedPanId.LSB);
                GP_LOG_SYSTEM_PRINTF("  Number of subdevices: %d", 0,
                                     appsEp_1.getTouchLinkTargetInfo().devInfoRsp.m_nCount);
                GP_LOG_SYSTEM_PRINTF("  Start index: %d", 0,
                                     appsEp_1.getTouchLinkTargetInfo().devInfoRsp.m_nStartIndex);
                GP_LOG_SYSTEM_PRINTF("  Device Information Count: %d", 0,
                                     appsEp_1.getTouchLinkTargetInfo().devInfoRsp.m_nRecords);
                GP_LOG_SYSTEM_PRINTF("  Device Information:", 0);
                for(uint8_t i = 0; i < appsEp_1.getTouchLinkTargetInfo().devInfoRsp.m_nRecords; i++)
                {
                    const CZigBeeTouchlinkDeviceInformation& devInfoRec =
                        appsEp_1.getTouchLinkTargetInfo().devInfoRec[i];
                    GP_LOG_SYSTEM_PRINTF("    Target Address:", 0);
                    gpLog_PrintBuffer(sizeof(appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_qwAddress),
                                      (uint8_t*)&devInfoRec.m_qwAddress);
                    GP_LOG_SYSTEM_PRINTF("    Device Information Record %d", 0, i);
                    GP_LOG_SYSTEM_PRINTF("      Endpoint 0x%04x", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_info.m_nEndpoint);
                    GP_LOG_SYSTEM_PRINTF("      Profile ID 0x%04x", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_info.m_wProfileID);
                    GP_LOG_SYSTEM_PRINTF("      Device ID 0x%04x", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_info.m_wDeviceID);
                    GP_LOG_SYSTEM_PRINTF("      version %d", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_info.m_nVersion);
                    GP_LOG_SYSTEM_PRINTF("      Group Count %d", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_info.m_nGroupCount);
                    GP_LOG_SYSTEM_PRINTF("    Sort order: %d", 0,
                                         appsEp_1.getTouchLinkTargetInfo().devInfoRec[i].m_nSortOrder);
                }
#ifdef GP_APPLICATION_GROUP_ID
                GP_LOG_SYSTEM_PRINTF("TouchLink Client: Post commissioning Event, add target into group %d", 0,
                                     GP_APPLICATION_GROUP_ID);

                /*  Porting note:
                 *   Below is an example of application specific handle - add target into to group with ID =
                 * GP_APPLICATION_GROUP_ID */
                // -- Set destination to the target's address(short)
                CZigBeeAddress destAddress(appsEp_1.getTouchLinkTargetInfo().targetAddress.m_wShort);

                // -- Set destination's address type and endpoint
                destAddress.SetType(CIEEE802154Address::typeShort);
                destAddress.SetEndpoint(1);

                // -- Send add group to the new joined target
                Application_SendAddGroup(destAddress, GP_APPLICATION_GROUP_ID);
#endif // GP_APPLICATION_GROUP_ID
                appsEp_1.m_touchLinkClient.bindEndpoints();
            }
            else
            {
                // Printout received network join response with status failed.
                GP_LOG_SYSTEM_PRINTF("TouchLink Client: Target join Failed", 0);
            }
            //  disable receiver Rx in phy layer.
            setRxOnWhenIdle(false);
            break;
        }
        case CQorvoClusterTouchLinkCommissioningClient::statusApplyNetwork:
        {
            // Printout log
            GP_LOG_SYSTEM_PRINTF("TouchLink: Applying new network parameter...", 0);
        }
        default:
            break;
    }
}
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
