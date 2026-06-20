/*
 * Copyright (c) 2024, Qorvo Inc
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

/** @file "LightSensorApplication.cpp"
 *
 * Class definition of Light Sensor endpoint.
 * Cluster collection needed for a Light sensor defined here.
 * Input into the different clusters implemented through class functions.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "ZigBee.h"
#include "ZigBeeMessage_Callback.h"
#include "ZigBeeFindingAndBinding_Callback.h"
#include "LightSensorApplication.h"
#include "ResponsiveModePolling.h"

#include "gpLog.h"
#include "gpSched.h"
#include "App.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID  GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

extern CZigBeeApplicationSupport aps;

/*****************************************************************************
 *                    Class implementation
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CLightSensorApplication cluster map

__COMPACT7B_IMPLEMENT_CLUSTER_MAP(CLightSensorApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_CLUSTER_MAP(CLightSensorApplication)
    __COMPACT7B_CLUSTER(CLightSensorApplication, m_basic)
    __COMPACT7B_CLUSTER(CLightSensorApplication, m_identify)
    __COMPACT7B_CLUSTER(CLightSensorApplication, m_identifyClient)
    __COMPACT7B_CLUSTER(CLightSensorApplication, m_illuminanceMeasurement)
__COMPACT7B_END_CLUSTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CLightSensorApplication input cluster lists

__COMPACT7B_IMPLEMENT_INCLUSTER_LIST(CLightSensorApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CLightSensorApplication output cluster lists

__COMPACT7B_IMPLEMENT_OUTCLUSTER_LIST(CLightSensorApplication,
    CZigBeeFoundationApplication)

/////////////////////////////////////////////////////////////////////////////
// CLightSensorApplication notification map

__COMPACT7B_IMPLEMENT_NOTIFICATION_MAP(CLightSensorApplication,
    CZigBeeFoundationApplication)

__COMPACT7B_BEGIN_NOTIFICATION_MAP(CLightSensorApplication)
    __COMPACT7B_CLUSTER_NOTIFICATION(CZigBeeClusterIdentify::id,
        CZigBeeClusterIdentify::attributeIdentifyTime, 0,
        &CLightSensorApplication::OnUpdateIdentifyTime, 0)
    __COMPACT7B_CLUSTER_NOTIFICATION(CQorvoClusterIlluminanceMeasurement::id,
        CQorvoClusterIlluminanceMeasurement::attributeMeasuredValue, 0
        , &CLightSensorApplication::OnUpdateIlluminanceMeasurement
        , &CLightSensorApplication::OnRefreshIlluminanceMeasurement)
__COMPACT7B_END_NOTIFICATION_MAP


/////////////////////////////////////////////////////////////////////////////
// CLightSensorApplication

CLightSensorApplication::CLightSensorApplication(CZigBeeApplicationSupport& aps, const unsigned char bEndpoint)
    : CZigBeeFoundationApplication(aps, profileID, deviceID, version, bEndpoint), m_basic(*this), m_identify(*this),
      m_identifyClient(*this), m_illuminanceMeasurement(*this, 0x0001, 0xFFFE)
{
}

/*****************************************************************************
 *                    Override functions
 *****************************************************************************/

void CLightSensorApplication::RestoreDefaults(const bool bRemoveAllReferences)
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
}

unsigned char CLightSensorApplication::ProcessBindOrUnbindRequest
    (const CZigBeeBindingSource &source, const CZigBeeBindingTarget &target,
    const bool bBind, const CZigBeeAddress &originator)
{
    // look for in-bound (server) cluster
    CZigBeeCluster* pCluster = FindCluster(source.m_wClusterID, CZigBeeCluster::flagInbound);

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
void CLightSensorApplication::OnIndicateData(const CZigBeeAddress &destination,
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
void CLightSensorApplication::OnConfirmData(const CZigBeeAddress &destination,
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
void CLightSensorApplication::OnUpdateIdentifyTime(const unsigned char nOptions)
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

void CLightSensorApplication::OnUpdateIlluminanceMeasurement(const unsigned char nOptions)
{
    GP_LOG_SYSTEM_PRINTF("ILLUMINANCE: MeasuredValue = 0x%04x", 0, m_illuminanceMeasurement.m_nMeasuredValue);
}

void CLightSensorApplication::OnRefreshIlluminanceMeasurement(const unsigned char nOptions)
{
    UInt16 illuminance;

    if(Application_GetIlluminance(&illuminance))
    {
        m_illuminanceMeasurement.m_nMeasuredValue = illuminance;
        GP_LOG_SYSTEM_PRINTF("ILLUMINANCE: Refresh MeasuredValue = 0x%04x", 0, m_illuminanceMeasurement.m_nMeasuredValue);
    }
}

/*****************************************************************************
 *                    Public functions
 *****************************************************************************/
// Identify Cluster
unsigned short CLightSensorApplication::GetIdentifyMode(void)
{
    return m_identify.m_wIdentifyTime;
}

void CLightSensorApplication::SetIdentifyMode(unsigned short duration)
{
    GP_LOG_PRINTF("Setting identify mode with duration %u",0, duration);
    const CZigBeeAttribute *const pIdentifyTime = m_identify.FindAttribute(CZigBeeClusterIdentify::attributeIdentifyTime);
    ASSERT(pIdentifyTime);

    m_identify.SetAttributeValue(*pIdentifyTime, &duration, sizeof(duration));
}

void CLightSensorApplication::FindingAndBinding(void)
{
    // start finding and binding on target for commissioning time(default = 180s)
    m_identify.StartIdentify(static_cast<uint16_t>(CZigBeeApplicationSupportManagementBase::commissioningTime), true);
}

void CLightSensorApplication::FindingAndBindingInitiator(void)
{
    // start finding and binding on initiator
    m_identifyClient.RunFindingAndBinding(*this, CZigBeeFoundationApplication::commissioningInitiator);

    // while entering/leaving responsive mode polling during Finding and Binding is not implemented
    // all we can do is enter responsive mode polling - for 1 minute
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
    StartResponsiveModeWithTimeout();
#endif // GP_APP_DIVERSITY_BATTERY_SUPPORTED
}

void CLightSensorApplication::SetIlluminanceMeasuredValue(unsigned short nMeasuredValue)
{
    m_illuminanceMeasurement.SetMeasuredValue(nMeasuredValue);
}

void CLightSensorApplication::ReportIlluminanceMeasuredValue(void)
{
    CPacket asdu;
    CZigBeeAddress destination = CZigBeeAddress();
    const CZigBeeAttribute *const pMeasuredValue =
        m_illuminanceMeasurement.FindAttribute(CQorvoClusterIlluminanceMeasurement::attributeMeasuredValue);
    ASSERT(pMeasuredValue);

    m_illuminanceMeasurement.CreateRequest(asdu, static_cast<unsigned char>(CZigBeeCluster::commandReportAttributes),
                                           CZigBeeClusterLibraryHeader::typeProfile |
                                           CZigBeeClusterLibraryHeader::directionServerToClient |
                                           CZigBeeClusterLibraryHeader::disableDefaultResponse);

    asdu.Append(&(pMeasuredValue->m_wAttributeID), sizeof(unsigned short));
    asdu.Append(&(pMeasuredValue->m_nType), sizeof(unsigned char));
    asdu.Append(&m_illuminanceMeasurement.m_nMeasuredValue, sizeof(unsigned short));

    OnRequestData(destination, m_illuminanceMeasurement, asdu, CZigBeeApplicationSupport::transmitAcknowledged, 0);
}

// Cluster Attribute Restorement
void CLightSensorApplication::RestoreBasicAttributeDefaults(void)
{
    GP_LOG_PRINTF("Basic RestoreDefaults", 0);

    // restore all attribute back to default value
    MEMSET(m_basic.achLocationDescription, 0 , 16);
    m_basic.bPhysicalEnvironment = 0x00;

    const CZigBeeAttribute *const pLocationDescription = m_basic.FindAttribute(CZigBeeClusterBasicEx::attributeLocationDescription);
    const CZigBeeAttribute *const pPhysicalEnvironment = m_basic.FindAttribute(CZigBeeClusterBasicEx::attributePhysicalEnvironment);

    // For optional attribute, handle here
    if (pLocationDescription)
    {
        m_basic.SetAttributeValue(*pLocationDescription, &m_basic.achLocationDescription, 16);
    }
    if (pPhysicalEnvironment)
    {
        m_basic.SetAttributeValue(*pPhysicalEnvironment, &m_basic.bPhysicalEnvironment, sizeof(m_basic.bPhysicalEnvironment));
    }
}

void CLightSensorApplication::RestoreIdentifyAttributeDefaults(void)
{
    GP_LOG_PRINTF("Identify RestoreDefaults", 0);
    SetIdentifyMode(0);
}
