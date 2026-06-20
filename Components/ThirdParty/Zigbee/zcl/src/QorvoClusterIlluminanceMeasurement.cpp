/*
 * Copyright (c) 2018, Qorvo Inc
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
 * $Header: $
 * $Change: $
 * $DateTime: $
 *
 */

#include <climits>
#include <string>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeClusterLibrary.h"
#include "QorvoClusterIlluminanceMeasurement.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterIlluminanceMeasurement

const unsigned short CQorvoClusterIlluminanceMeasurement::nClusterRevision =
	CQorvoClusterIlluminanceMeasurement::revision;

CQorvoClusterIlluminanceMeasurement::CQorvoClusterIlluminanceMeasurement
    (CZigBeeFoundationApplication &application,
    const unsigned short nMinLimit,
    const unsigned short nMaxLimit,
    const unsigned short nTolerance,
    const unsigned char nLightSensorType):
CZigBeeCluster(application, id, flagInbound | flagOperational),
m_nMeasuredValue(nMinLimit), m_measuredValueRange(nMinLimit, nMaxLimit),
m_nTolerance(nTolerance), m_nLightSensorType(nLightSensorType)
{
}

void CQorvoClusterIlluminanceMeasurement::AfterPowerup()
{
    // The start-up behavior is configurable: Default is to restore the last setting
    // unless a more specific action has been requested.
    const CZigBeeAttribute *const pMeasuredValue = FindAttribute(attributeMeasuredValue);
    ASSERT(pMeasuredValue);

    // Invoke base class handler first
    CZigBeeCluster::AfterPowerup();

    // Load attribute and report attribute changes to application
    pMeasuredValue->Load(*this);
}

void CQorvoClusterIlluminanceMeasurement::SetMeasuredValue(unsigned short nMeasuredValue)
{
    const CZigBeeAttribute *const pMeasuredValue = FindAttribute(attributeMeasuredValue);
    ASSERT(pMeasuredValue);
    
    // Example case, if nMeasuredValue < minMeasuredValue 
    if (m_measuredValueRange.first != MeasurableValueUndefined) 
    {
        if (nMeasuredValue < m_measuredValueRange.first)
        {
            _TRACE2(c7b::zcl | 1,"Error Illuminance %u < min %u\r\n", nMeasuredValue, m_measuredValueRange.first);
            nMeasuredValue = MeasurableValueUndefined;
        }
    }
    
    // Example case, if nMeasuredValue > maxMeasuredValue 
    if (m_measuredValueRange.second != MeasurableValueUndefined) 
    {
        if (nMeasuredValue > m_measuredValueRange.second)
        {
            _TRACE2(c7b::zcl | 1,"Error Illuminance %u > max %u\r\n", nMeasuredValue, m_measuredValueRange.first);
            nMeasuredValue = MeasurableValueUndefined;
        }
    }
    
    SetAttributeValue(*pMeasuredValue, &nMeasuredValue, sizeof(unsigned short));
    pMeasuredValue->Store(*this);

    _TRACE1(c7b::zcl | 1,"Illuminance measured %u\r\n", nMeasuredValue);
}

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterIlluminanceMeasurement,
	CZigBeeCluster)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterIlluminanceMeasurement)
	__COMPACT7B_CLUSTERREVISION(CQorvoClusterIlluminanceMeasurement)
	__COMPACT7B_ILLUMINANCEMEASUREMENT_MEASUREDVALUE()
	__COMPACT7B_ILLUMINANCEMEASUREMENT_MINMEASURABLEVALUE()
	__COMPACT7B_ILLUMINANCEMEASUREMENT_MAXMEASURABLEVALUE()
	__COMPACT7B_ILLUMINANCEMEASUREMENT_TOLERANCE()
    __COMPACT7B_ILLUMINANCEMEASUREMENT_LIGHTSENSORTYPE()
__COMPACT7B_END_ATTRIBUTE_MAP()
