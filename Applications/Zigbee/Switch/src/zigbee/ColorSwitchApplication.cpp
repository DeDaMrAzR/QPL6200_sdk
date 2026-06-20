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

/** @file "ColorSwitchApplication.cpp"
 *
 * Class definition of Color switch endpoint.
 * Cluster collection needed for color switch application defined here
 * Input into the different clusters implemented through class functions.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "ZigBeeCommissioning.h"
#include "ZigBeeMessage_Callback.h"
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#include "ZigBeeTouchLink_Callback.h"
#endif //(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
#include "ZigBeeFindingAndBinding_Callback.h"
#endif //GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
#include "ColorSwitchApplication.h"
#include "gpLog.h"
#include "gpSched.h"
#include "ResponsiveModePolling.h"
#include "App.h"
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
#if defined(GP_APP_DIVERSITY_BATTERYMODULE_OVERRIDE)
#include "AppBattery.h"
#else
#include "App_Battery.h"
#endif // GP_APP_DIVERSITY_BATTERYMODULE_OVERRIDE
#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID  GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

extern CZigBeeApplicationSupport aps;

/*****************************************************************************
 *                    Override functions
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CColorSwitchApplication cluster map

__COMPACT7B_IMPLEMENT_CLUSTER_MAP(CColorSwitchApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_CLUSTER_MAP(CColorSwitchApplication)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_basic)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_powerConfiguration)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_identify)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_identifyClient)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_groupClient)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_sceneClient)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_onOffClient)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_levelClient)
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_pollControl)
#ifdef GP_APP_DIVERSITY_COLOR_SWITCH
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_colorClient)
#endif // GP_APP_DIVERSITY_COLOR_SWITCH
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_touchLinkClient)
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_touchLink)
#endif // GP_APP_DIVERSITY_TOUCHLINK
#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_wwahu)
#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
#ifdef UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
    __COMPACT7B_CLUSTER(CColorSwitchApplication, m_otaUpgrade)
#endif // UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
__COMPACT7B_END_CLUSTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CLevelControlOutputApplication input cluster lists

__COMPACT7B_IMPLEMENT_INCLUSTER_LIST(CColorSwitchApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CLevelControlOutputApplication output cluster lists

__COMPACT7B_IMPLEMENT_OUTCLUSTER_LIST(CColorSwitchApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CColorSwitchApplication notification map

__COMPACT7B_IMPLEMENT_NOTIFICATION_MAP(CColorSwitchApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_NOTIFICATION_MAP(CColorSwitchApplication)
    __COMPACT7B_CLUSTER_NOTIFICATION(CZigBeeClusterIdentify::id,
        CZigBeeClusterIdentify::attributeIdentifyTime, 0,
        &CColorSwitchApplication::OnUpdateIdentifyTime, 0)
__COMPACT7B_END_NOTIFICATION_MAP

/////////////////////////////////////////////////////////////////////////////
// CColorSwitchApplication

CColorSwitchApplication::CColorSwitchApplication(CZigBeeApplicationSupport& aps, const unsigned char bEndpoint)
    : CZigBeeFoundationApplication(aps, profileID, deviceID, version, bEndpoint,
                                   CZigBeeFoundationApplication::commissioningInitiator
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
                                       | CZigBeeFoundationApplication::touchlink |
                                       (alternateVersion << CZigBeeFoundationApplication::alternateDeviceVersionShift) |
                                       CZigBeeFoundationApplication::hasAlternateDeviceID,
                                   alternateDeviceID
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT || GP_APP_DIVERSITY_TOUCHLINK
                                   ),
      m_basic(*this), m_powerConfiguration(*this), m_identify(*this), m_identifyClient(*this), m_groupClient(*this),
      m_sceneClient(*this), m_onOffClient(*this), m_levelClient(*this), m_pollControl(*this)
#ifdef GP_APP_DIVERSITY_COLOR_SWITCH
      ,
      m_colorClient(*this)
#endif // GP_APP_DIVERSITY_COLOR_SWITCH
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
      ,
      m_touchLinkClient(*this, CQorvoClusterTouchLinkCommissioningClient::typeEndDevice,
                        0,    // no perfered initiator short address
                        true) // enable device info request
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
      ,
      m_touchLink(*this)
#endif // GP_APP_DIVERSITY_TOUCHLINK
#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
      ,
      m_wwahu(*this)
#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER
#ifdef UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
      ,
      m_otaUpgrade(*this)
#endif // UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
{
}

/*****************************************************************************
 *                    Override functions
 *****************************************************************************/
//Activate Basic Cluster ResetToFactoryDefaults command
__COMPACT7B_BEGIN_COMMAND_MAP(CZigBeeClusterBasic)
      __COMPACT7B_COMMAND_C2S(CZigBeeClusterBasic::commandResetToFactoryDefaults,
            &CZigBeeClusterBasic::OnFactoryDefaults)
__COMPACT7B_END_COMMAND_MAP()

void CColorSwitchApplication::RestoreDefaults(const bool bRemoveAllReferences)
{
    // Sample implementation - for receiption of the Factory To Reset command of the Basic cluster.
    GP_LOG_SYSTEM_PRINTF("Basic: Restore defaults triggered, rmAllRef %d", 0, bRemoveAllReferences);

    // From ZigBee Base device Specification - 9.1 Reset via the basic cluster
    // On receipt of the basic cluster, reset to factory defaults command, the target device
    // SHALL reset the attributes of all clusters supported on the target device to their
    // default values. All other values such as network settings, frame counters, groups and
    // bindings SHALL be preserved.

    // Reset Basic
    RestoreBasicAttributeDefaults();

    // Reset Identify
    RestoreIdentifyAttributeDefaults();
}

unsigned char CColorSwitchApplication::ProcessBindOrUnbindRequest
    (const CZigBeeBindingSource &source, const CZigBeeBindingTarget &target,
    const bool bBind, const CZigBeeAddress &originator)
{
    // look for in-bound (server) cluster
    CZigBeeCluster *pCluster = FindCluster(source.m_wClusterID, CZigBeeCluster::flagInbound);

    // No in-bound (server) cluster? Then look for out-bound (client) cluster
    if (!pCluster)
    {
        pCluster = FindCluster(source.m_wClusterID, CZigBeeCluster::flagOutbound);
    }

    // reject if cluster is not exist for bind request case,
    // Pass on for unbind request case
    return pCluster ?
        pCluster->ProcessBindOrUnbindRequest(source, target, bBind, originator) :
        (bBind ? CZigBeeDeviceObject::statusNotSupported : CZigBeeDeviceObject::statusNoEntry);
}

#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
/** @brief Funcation to be called when a binding is set/removed
 *  Required Header: "ZigBeeFindingAndBinding_Callback.h"
*/
void CZigBeeClusterManagementIdentify::AfterManageBinding(CZigBeeDeviceObjectBinding &binding, const bool bBind)
{
    Application_cbZigBeePostManageBinding(binding,bBind);
}

#endif //GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
/** @brief Overrides function for APSDE-DATA.indication
 *  Details is in clause 2.2.4.1.3
 *  Required Header: "ZigBeeMessage_Callback.h"
 */
void CColorSwitchApplication::OnIndicateData(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const unsigned short wProfileID,
    const unsigned short wClusterID, CPacket &asdu,
    const unsigned char bStatus, const bool bSecurityUsed,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    // launch application callback - before ZCL command received.
    Application_cbZigBeePreApsDataIndication(destination, source, wProfileID, wClusterID, asdu, bStatus, bSecurityUsed, nLinkQuality, nTimestamp);

    // Explicitly call the base class - CZigBeeFoundationApplication::OnIndicateData()
    CZigBeeFoundationApplication::OnIndicateData(destination, source, wProfileID, wClusterID, asdu, bStatus, bSecurityUsed, nLinkQuality, nTimestamp);

    // launch application callback - after ZCL command received.
    Application_cbZigBeePostApsDataIndication(destination, source, wProfileID, wClusterID, asdu, bStatus, bSecurityUsed, nLinkQuality, nTimestamp);
}

/** @brief Overrides function for APSDE-DATA.confirm
 *  Details is in clause 2.2.4.1.2
 *  Required Header: "ZigBeeMessage_Callback.h"
 */
void CColorSwitchApplication::OnConfirmData(const CZigBeeAddress &destination,
    const unsigned char bSourceEndpoint, const unsigned char bStatus,
    const unsigned int nTimestamp, void *const pCookie)
{
    // Explicitly call the base class - CZigBeeFoundationApplication::OnConfirmData()
    CZigBeeFoundationApplication::OnConfirmData(destination, bSourceEndpoint, bStatus, nTimestamp, pCookie);

    // launch application callback - after ZCL command sent.
    Application_cbZigBeePostApsDataConfirm(destination, bSourceEndpoint, bStatus, nTimestamp, pCookie);
}

/*****************************************************************************
 *                   Attribute callback functions
 *****************************************************************************/
void CColorSwitchApplication::OnUpdateIdentifyTime(const unsigned char nOptions)
{
    GP_LOG_SYSTEM_PRINTF("IDENT: identify-time = %u", 0,
                         m_identify.m_wIdentifyTime);

    //start identify indication on receiving first callback
    if(m_identify.m_wIdentifyTime)
    {
        //start identify on current endpoint
        Application_cbIdentifyStart(m_bEndpoint);
    }

    //stop identify if time is zero
    if(!m_identify.m_wIdentifyTime)
    {
        Application_cbIdentifyStop(m_bEndpoint);
    }
}

/*****************************************************************************
 *                    Public functions
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
// Power configuration Cluster
unsigned char CColorSwitchApplication::GetBatteryVoltage(void)
{
    return m_powerConfiguration.m_BatteryVoltage;
}

void CColorSwitchApplication::SetBatteryVoltage(unsigned char voltage)
{
    const CZigBeeAttribute *const pPowerConfig = m_powerConfiguration.FindAttribute(m_powerConfiguration.attributeBatteryVoltage);
    ASSERT(pPowerConfig);

    m_powerConfiguration.SetAttributeValue(*pPowerConfig, &voltage, sizeof(voltage));
}

unsigned char CColorSwitchApplication::GetBatteryPercentageRemaining(void)
{
    return m_powerConfiguration.m_BatteryPercentageRemaining;
}

void CColorSwitchApplication::SetBatteryPercentageRemaining(unsigned char percentageRemaining)
{
    const CZigBeeAttribute *const pPowerConfig = m_powerConfiguration.FindAttribute(m_powerConfiguration.attributeBatteryPercentageRemaining);
    ASSERT(pPowerConfig);

    m_powerConfiguration.SetAttributeValue(*pPowerConfig, &percentageRemaining, sizeof(percentageRemaining));
}

#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED
// Identify Cluster
unsigned short CColorSwitchApplication::GetIdentifyMode(void)
{
    return m_identify.m_wIdentifyTime;
}

void CColorSwitchApplication::SetIdentifyMode(unsigned short duration)
{
    GP_LOG_PRINTF("Setting identify mode with duration %u",0, duration);
    const CZigBeeAttribute *const pIdentifyTime = m_identify.FindAttribute(CZigBeeClusterIdentify::attributeIdentifyTime);
    ASSERT(pIdentifyTime);

    m_identify.SetAttributeValue(*pIdentifyTime, &duration, sizeof(duration));
}

void CColorSwitchApplication::FindingAndBinding(void)
{
    // start finding and binding on target for commissioning time(default = 180s)
    m_identify.StartIdentify(static_cast<uint16_t>(CZigBeeApplicationSupportManagementBase::commissioningTime), true);
}

void CColorSwitchApplication::FindingAndBindingInitiator(void)
{
    // start finding and binding on initiator
    m_identifyClient.RunFindingAndBinding(*this, CZigBeeFoundationApplication::commissioningInitiator);

    // while entering/leaving responsive mode polling during Finding and Binding is not implemented
    // all we can do is enter responsive mode polling - for 1 minute
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
    StartResponsiveModeWithTimeout();
#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED
}

// group Cluster Client
void CColorSwitchApplication::SendGroupAddCommand(const CZigBeeAddress& destination,
                                                  unsigned short groupId)
{
    char groupName[1];

    groupName[0] = 0x0;

    // launch add group API in group cluster client
    m_groupClient.AddGroup(destination,groupId,groupName);
}

// OnOff Cluster Client
void CColorSwitchApplication::SendOnOffToggleCommand(const CZigBeeAddress &destination,
    const unsigned char transmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    m_onOffClient.CreateCommand(asdu, CZigBeeClusterOnOff::commandToggle);

    // Transmit ZCL frame via ZCL
    m_onOffClient.OnRequestData(destination, asdu, transmitOptions);

    // poll long enough to retrieve the ZCL Default  Response
    //StartResponsiveModeWithTimeout(INDIRECTTRANSMITTIMEOUT);
}

// Leveling cluster client
void CColorSwitchApplication::SendLevelingMoveToLevelCommand(const CZigBeeAddress& destination,
                                                             const unsigned char level, const unsigned short transitionTime,
                                                             const bool onOff, const unsigned char transmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    m_levelClient.CreateMoveToLevelCommand(asdu, level, transitionTime, onOff);

    // Transmit ZCL frame via ZCL
    m_levelClient.OnRequestData(destination, asdu, transmitOptions);

    // poll long enough to retrieve the ZCL Default  Response
    //StartResponsiveModeWithTimeout(INDIRECTTRANSMITTIMEOUT);
}

void CColorSwitchApplication::SendLevelingMoveCommand(const CZigBeeAddress& destination,
                                                      const unsigned char mode, const unsigned char rate,
                                                      const bool onOff, const unsigned char transmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    m_levelClient.CreateMoveCommand(asdu, mode, rate, onOff);

    // Transmit ZCL frame via ZCL
    m_levelClient.OnRequestData(destination, asdu, transmitOptions);

    // poll long enough to retrieve the ZCL Default  Response
    //StartResponsiveModeWithTimeout(INDIRECTTRANSMITTIMEOUT);
}

void CColorSwitchApplication::SendLevelingStopCommand(const CZigBeeAddress& destination,
                                                      const unsigned char transmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    m_levelClient.CreateStopCommand(asdu, true);

    // Transmit ZCL frame via ZCL
    m_levelClient.OnRequestData(destination, asdu, transmitOptions);

    // poll long enough to retrieve the ZCL Default Response
    //StartResponsiveModeWithTimeout(INDIRECTTRANSMITTIMEOUT);
}

#ifdef GP_APP_DIVERSITY_COLOR_SWITCH
// Color Control Cluster Client
void CColorSwitchApplication::SendColorControlMoveToHueAndSaturationCommand(const CZigBeeAddress& destination,
                                                                            unsigned char hue, unsigned char saturation, const unsigned short transitionTime)
{
    m_colorClient.MoveToHueSaturation(destination, hue, saturation, transitionTime);
}

void CColorSwitchApplication::SendColorControlMoveHueCommand(const CZigBeeAddress& destination,
                                                             unsigned char mode, unsigned char rate)
{
    m_colorClient.MoveHue(destination, mode, rate);
}

#endif //GP_APP_DIVERSITY_COLOR_SWITCH
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
const CQorvoTouchLinkClientTargetInfo CColorSwitchApplication::getTouchLinkScannedTargetInfo(void)
{
    return m_touchLinkClient.getScannedTargetInfo();
}

const CQorvoTouchLinkScanResponseData CColorSwitchApplication::getTouchLinkPotentialTargetInfo(void)
{
    return m_touchLinkClient.getPotentialTargetInfo();
}

const CQorvoTouchLinkClientTargetInfo CColorSwitchApplication::getTouchLinkTargetInfo(void)
{
    return m_touchLinkClient.getTargetInfo();
}
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
// TouchLink Commissioning(Server)
void CColorSwitchApplication::EnableTouchLink(const bool enable)
{
    // Enable answering to Scan Requests
    m_touchLink.Enable(enable);
}

void CColorSwitchApplication::EnableReceiver(void)
{
    // Enable receiver for TL scanning session
    m_touchLink.EnableReceiver();
}

bool CColorSwitchApplication::IsTouchlinkSessionActive(void)
{
    // Return true if session is active
    if(m_touchLink.IsSessionActive())
    {
        return true;
    }
    return false;
}

UInt32 CColorSwitchApplication::GetTouchlinkSessionId(void)
{
    UInt32 sessionId = 0;
    // return session id if the session is active
    if(m_touchLink.IsSessionActive())
    {
        sessionId = m_touchLink.GetSessionID();
    }
    return sessionId;
}
#endif // GP_APP_DIVERSITY_TOUCHLINK

// Cluster Attribute Restorement
void CColorSwitchApplication::RestoreBasicAttributeDefaults(void)
{
    GP_LOG_PRINTF("Basic RestoreDefaults",0);

    // restore all attribute back to default value
    MEMSET(m_basic.achLocationDescription, 0 , 16);
    m_basic.bPhysicalEnvironment = 0x00;

    const CZigBeeAttribute *const pLocationDescription = m_basic.FindAttribute(CZigBeeClusterBasicEx::attributeLocationDescription);
    const CZigBeeAttribute *const pPhysicalEnvironment = m_basic.FindAttribute(CZigBeeClusterBasicEx::attributePhysicalEnvironment);

    // For optional attribute, handle here
    if (pLocationDescription)
    {
        m_basic.SetAttributeValue(*pLocationDescription, &m_basic.achLocationDescription, 16 );
    }
    if (pPhysicalEnvironment)
    {
        m_basic.SetAttributeValue(*pPhysicalEnvironment, &m_basic.bPhysicalEnvironment, sizeof(m_basic.bPhysicalEnvironment));
    }
}

void CColorSwitchApplication::RestoreIdentifyAttributeDefaults(void)
{
    GP_LOG_PRINTF("Identify RestoreDefaults",0);
    SetIdentifyMode(0);
}

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
void CColorSwitchApplication::SendWwahuPoweringNotification(bool nOnOff,uint_fast8_t nReason)
{
    GP_LOG_SYSTEM_PRINTF("WWAHu: Power Notification %c%c%c, reason 0x%x ", 0, 'O', nOnOff ? 'n' : 'f', nOnOff ? ' ' : 'f', nReason);
    m_wwahu.SendPoweringNotification(nOnOff, nReason);
}
#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
