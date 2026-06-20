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

/** @file "ContactSwitchApplication.cpp"
 *
 * Class definition of IAS zone DoorWindow endpoint.
 * Cluster collection needed for an iControl enabled Door/Window sensor defined here.
 * Input into the different clusters implemented through class functions.
 *
 * IAS Zone Enrollment options are: (ZCL specification 8.2.2.2.3)
 * - Trip-to-pair + Auto-Enroll-Response
 * - Auto-Enroll-Request
 *
 * Default the Auto-Enroll-Request is enabled (see IAS Zone cluster initialization).
 * To create a Trip-to-pair only behavior, put bAutoEnrollRequest to false.
 * An IAS Zone state change will trigger an enrollment if it was not enrolled previously.
 *
 * This application will use a combination of both Auto-Enroll-Request and Trip-to-pair for demo purposes.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "ZigBee.h"
#include "ZigBeeCommissioning.h"
#include "ZigBeeMessage_Callback.h"
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#include "ZigBeeTouchLink_Callback.h"
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#include "ContactSwitchApplication.h"
#include "gpLog.h"
#include "gpSched.h"
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
#define GP_COMPONENT_ID                             GP_COMPONENT_ID_APP

// defination for IAS Zone server setting
//   zone type      - please refer to ZCL IAS Zone's Table 8-5. Values of the ZoneType Attribute
//   auto-enroll    - please refer to ZCL IAS Zone's 8.2.2.1.3 Implementation Guidelines and Auto-Enroll-Request behavior
//   request default response   -   default disable
//   notify status  - please refer to ZCL IAS Zone, Table 8-6. Values of the ZoneStatus Attribute
//      default: enabled the restore notification and disable the supervision notification
#define APP_IAS_ZONETYPE                            CZigBeeClusterIASZone::typeContactSwitch
#define APP_IAS_AUTO_ENROLL_REQUEST                 true
#define APP_IAS_REQUEST_DEFAULT_RESPONSE            false
#define APP_IAS_DEFAULT_NOTIFY_STATUS               CZigBeeClusterIASZone::statusRestoreNotifications

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Class implementation
 *****************************************************************************/


/*****************************************************************************
 *                    Override functions
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CContactSwitchApplication cluster map

__COMPACT7B_IMPLEMENT_CLUSTER_MAP(CContactSwitchApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_CLUSTER_MAP(CContactSwitchApplication)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_basic)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_powerConfiguration)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_identify)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_pollControl)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_iasZone)
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_diagnostics)
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_touchLinkClient)
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_touchLink)
#endif // GP_APP_DIVERSITY_TOUCHLINK
#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_wwahu)
#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
#ifdef UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
    __COMPACT7B_CLUSTER(CContactSwitchApplication, m_otaUpgrade)
#endif // UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
__COMPACT7B_END_CLUSTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CLevelControlOutputApplication input cluster lists

__COMPACT7B_IMPLEMENT_INCLUSTER_LIST(CContactSwitchApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CLevelControlOutputApplication output cluster lists

__COMPACT7B_IMPLEMENT_OUTCLUSTER_LIST(CContactSwitchApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CContactSwitchApplication notification map

__COMPACT7B_IMPLEMENT_NOTIFICATION_MAP(CContactSwitchApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_NOTIFICATION_MAP(CContactSwitchApplication)
    __COMPACT7B_CLUSTER_NOTIFICATION(CZigBeeClusterIdentify::id,
        CZigBeeClusterIdentify::attributeIdentifyTime, 0,
        &CContactSwitchApplication::OnUpdateIdentifyTime, 0)
__COMPACT7B_END_NOTIFICATION_MAP

/////////////////////////////////////////////////////////////////////////////
// CContactSwitchApplication

CContactSwitchApplication::CContactSwitchApplication(CZigBeeApplicationSupport& aps, const unsigned char bEndpoint)
    : CZigBeeFoundationApplication(aps, profileID, deviceID, version, bEndpoint
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
                                   ,
                                   CZigBeeFoundationApplication::touchlink |
                                       (alternateVersion << CZigBeeFoundationApplication::alternateDeviceVersionShift) |
                                       CZigBeeFoundationApplication::hasAlternateDeviceID,
                                   alternateDeviceID
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
                                   ),
      m_basic(*this), m_powerConfiguration(*this), m_identify(*this), m_pollControl(*this)
      ,
      m_iasZone(*this, APP_IAS_ZONETYPE, APP_IAS_AUTO_ENROLL_REQUEST, APP_IAS_REQUEST_DEFAULT_RESPONSE,
                APP_IAS_DEFAULT_NOTIFY_STATUS),
      m_diagnostics(*this)
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
      ,
      m_touchLinkClient(*this, CQorvoClusterTouchLinkCommissioningClient::typeEndDevice,
                        0,    // no perfered initiator short address
                        true) // enable device info request
#endif                        // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
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

void CContactSwitchApplication::RestoreDefaults(const bool bRemoveAllReferences)
{
    // Sample implementation - for receiption of the Factory To Reset command of the Basic cluster.
    GP_LOG_SYSTEM_PRINTF("BASIC: Restore defaults triggered, rmAllRef %d", 0, bRemoveAllReferences);

    // From ZigBee Base device Specification - 9.1 Reset via the basic cluster
    // On receipt of the basic cluster, reset to factory defaults command, the target device
    // SHALL reset the attributes of all clusters supported on the target device to their
    // default values. All other values such as network settings, frame counters, groups and
    // bindings SHALL be preserved.

    // Reset Basic
    RestoreBasicAttributeDefaults();

    // Reset Identify
    RestoreIdentifyAttributeDefaults();

    // Reset IAS Zone
    RestoreIasZoneAttributeDefaults();
}

unsigned char CContactSwitchApplication::ProcessBindOrUnbindRequest
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

/** @brief Overrides function for APSDE-DATA.indication
 *  Details is in clause 2.2.4.1.3
 *  Required Header: "ZigBeeMessage_Callback.h"
 */
void CContactSwitchApplication::OnIndicateData(const CZigBeeAddress &destination,
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
void CContactSwitchApplication::OnConfirmData(const CZigBeeAddress &destination,
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
void CContactSwitchApplication::OnUpdateIdentifyTime(const unsigned char nOptions)
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
unsigned char CContactSwitchApplication::GetBatteryVoltage(void)
{
    return m_powerConfiguration.m_BatteryVoltage;
}

void CContactSwitchApplication::SetBatteryVoltage(unsigned char voltage)
{
    const CZigBeeAttribute *const pPowerConfig = m_powerConfiguration.FindAttribute(m_powerConfiguration.attributeBatteryVoltage);
    ASSERT(pPowerConfig);

    m_powerConfiguration.SetAttributeValue(*pPowerConfig, &voltage, sizeof(voltage));
}

unsigned char CContactSwitchApplication::GetBatteryPercentageRemaining(void)
{
    return m_powerConfiguration.m_BatteryPercentageRemaining;
}

void CContactSwitchApplication::SetBatteryPercentageRemaining(unsigned char percentageRemaining)
{
    const CZigBeeAttribute *const pPowerConfig = m_powerConfiguration.FindAttribute(m_powerConfiguration.attributeBatteryPercentageRemaining);
    ASSERT(pPowerConfig);

    m_powerConfiguration.SetAttributeValue(*pPowerConfig, &percentageRemaining, sizeof(percentageRemaining));
}

#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED
// Identify Cluster
unsigned short CContactSwitchApplication::GetIdentifyMode(void)
{
    return m_identify.m_wIdentifyTime;
}

void CContactSwitchApplication::SetIdentifyMode(unsigned short duration)
{
    GP_LOG_PRINTF("Setting identify mode with duration %u",0, duration);
    const CZigBeeAttribute *const pIdentifyTime = m_identify.FindAttribute(CZigBeeClusterIdentify::attributeIdentifyTime);
    ASSERT(pIdentifyTime);

    m_identify.SetAttributeValue(*pIdentifyTime, &duration, sizeof(duration));
}

// Ias Zone Cluster
bool CContactSwitchApplication::isIasEnrolled(void)
{
    return (m_iasZone.m_bEnrolled);
}

bool CContactSwitchApplication::isCIEAddressSet(void)
{
    return (m_iasZone.m_qwCIEAddress != 0xFFFFFFFFFFFFFFFF);
}

unsigned short CContactSwitchApplication::GetIasStatus(void)
{
    return m_iasZone.m_nStatus;
}

bool CContactSwitchApplication::IasStateChange(const unsigned short nAdd,
                                               const unsigned short nRemove)
{
    //Only trigger enrollment/action if CIE address is written - see ZCL spec 8.2.2.2.3
    //No actions towards the CIE can start without this address.
    unsigned char  enrollStatus = false;
    unsigned short addMask = nAdd;
    unsigned short removeMask = nRemove;

#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
    // Low battery check
    if (GetBatteryVoltage() < Application_GetLowBatteryVoltageThreshold())
    {
        GP_LOG_SYSTEM_PRINTF("IAS Zone: Low Battery detected(%u < %u)", 0, GetBatteryVoltage(), Application_GetLowBatteryVoltageThreshold());
        addMask    |=  CZigBeeClusterIASZone::statusBatteryLow;
        removeMask &= ~CZigBeeClusterIASZone::statusBatteryLow;
    }
    else
    {
        addMask    &= ~CZigBeeClusterIASZone::statusBatteryLow;
        removeMask |=  CZigBeeClusterIASZone::statusBatteryLow;
    }

#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED
    // read enroll status
    enrollStatus = isIasEnrolled();

    // network status check
    if (Application_ZigBeeIsJoinedNetwork() && !Application_ZigBeeIsCommissioningInProgress())
    {
        // update the ZoneStatus attribute
        //  - start locating IAS CIE if device is NOT enrolled but CIE address is set.
        // -  trigger Zone status change notification if device is enrolled.
        m_iasZone.OnStatusChange(addMask, removeMask);

        GP_LOG_SYSTEM_PRINTF("IAS Zone: CIE:%c, enrolled:%c, IAS status change = 0x%x", 0,
                             isCIEAddressSet() ? 'Y' : 'N', enrollStatus ? 'Y' : 'N', GetIasStatus());
    }
    else
    {
        // update the ZoneStatus attribute only
        m_iasZone.UpdateStatus(addMask, removeMask);

        GP_LOG_SYSTEM_PRINTF("IAS Zone: NWK NOT ready, IAS status change = 0x%x", 0, GetIasStatus());
    }

    return enrollStatus;
}

void CContactSwitchApplication::IasSendNotification(void)
{
    unsigned char  enrollStatus = false;

    // read enroll status
    enrollStatus = isIasEnrolled();

    // network status and IAS enroll state check
    if (Application_ZigBeeIsJoinedNetwork() && (enrollStatus))
    {
        // Send IAS Zone status change notification directly to binded device
        m_iasZone.TransmitStatusChange(CZigBeeAddress(), GetIasStatus());

        GP_LOG_SYSTEM_PRINTF("IAS Zone: Enrolled, current status = 0x%x", 0, GetIasStatus());
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("IAS Zone: Device is NOT Enrolled or NOT joined", 0);
    }
}

#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
const CQorvoTouchLinkClientTargetInfo CContactSwitchApplication::getTouchLinkScannedTargetInfo(void)
{
    return m_touchLinkClient.getScannedTargetInfo();
}

const CQorvoTouchLinkScanResponseData CContactSwitchApplication::getTouchLinkPotentialTargetInfo(void)
{
    return m_touchLinkClient.getPotentialTargetInfo();
}

const CQorvoTouchLinkClientTargetInfo CContactSwitchApplication::getTouchLinkTargetInfo(void)
{
    return m_touchLinkClient.getTargetInfo();
}
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
// TouchLink Commissioning(Server)
void CContactSwitchApplication::EnableTouchLink(const bool enable)
{
    // Enable answering to Scan Requests
    m_touchLink.Enable(enable);
}

void CContactSwitchApplication::EnableReceiver(void)
{
    // Enable receiver for TL scanning session
    m_touchLink.EnableReceiver();
}

bool CContactSwitchApplication::IsTouchlinkSessionActive(void)
{
    // Return true if session is active
    if(m_touchLink.IsSessionActive())
    {
        return true;
    }
    return false;
}

UInt32 CContactSwitchApplication::GetTouchlinkSessionId(void)
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
void CContactSwitchApplication::RestoreBasicAttributeDefaults(void)
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

void CContactSwitchApplication::RestoreIdentifyAttributeDefaults(void)
{
    GP_LOG_PRINTF("Identify RestoreDefaults",0);
    SetIdentifyMode(0);
}

void CContactSwitchApplication::RestoreIasZoneAttributeDefaults(void){
    GP_LOG_PRINTF("IAS Zone RestoreDefaults",0);

    // restore all attribute back to default value
    m_iasZone.m_bEnrolled = 0x0;
    m_iasZone.m_qwCIEAddress = 0xffffffffffffffff;

    const CZigBeeAttribute *const pEnrolled = m_iasZone.FindAttribute(CZigBeeClusterIASZone::attributeStatus);
    const CZigBeeAttribute *const pCIEAddress = m_iasZone.FindAttribute(CZigBeeClusterIASZone::attributeCIEAddress);

    // For mandatory attribute, handle here
    ASSERT(pEnrolled);
    m_iasZone.SetAttributeValue(*pEnrolled, &m_iasZone.m_bEnrolled, sizeof(m_iasZone.m_bEnrolled));
    ASSERT(pCIEAddress);
    m_iasZone.SetAttributeValue(*pCIEAddress, &m_iasZone.m_qwCIEAddress, sizeof(m_iasZone.m_qwCIEAddress));
}

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
void CContactSwitchApplication::SendWwahuPoweringNotification(bool nOnOff,uint_fast8_t nReason)
{
    GP_LOG_SYSTEM_PRINTF("WWAHu: Power Notification %c%c%c, reason 0x%x ", 0, 'O', nOnOff ? 'n' : 'f', nOnOff ? ' ' : 'f', nReason);
    m_wwahu.SendPoweringNotification(nOnOff, nReason);
}
#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
