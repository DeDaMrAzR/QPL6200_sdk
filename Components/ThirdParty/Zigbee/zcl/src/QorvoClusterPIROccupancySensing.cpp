/*
 * Copyright (c) 2020, Qorvo Inc
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
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeClusterLibrary.h"
#include "ZigBeeClusterOccupancySensing.h"

#include "QorvoClusterPIROccupancySensing.h"

/////////////////////////////////////////////////////////////////////////////
// QorvoClusterPIROccupancySensing

CQorvoClusterPIROccupancySensing::CQorvoClusterPIROccupancySensing
    (CZigBeeFoundationApplication &application,
    const unsigned short nOccupiedToUnoccupiedDelay,
    const unsigned short nUnoccupiedToOccupiedDelay,
    const unsigned char nUnoccupiedToOccupiedThreshold):
CZigBeeClusterOccupancySensing(application),
m_uOccupiedToUnoccupiedDelay(nOccupiedToUnoccupiedDelay),
m_uUnoccupiedToOccupiedDelay(nUnoccupiedToOccupiedDelay),
m_nUnoccupiedToOccupiedThreshold(nUnoccupiedToOccupiedThreshold)
{
}


/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterPIROccupancySensing attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterPIROccupancySensing, CZigBeeClusterOccupancySensing)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterPIROccupancySensing)
    __COMPACT7B_OCCUPANCYSENSING_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY(CQorvoClusterPIROccupancySensing,
        m_uOccupiedToUnoccupiedDelay)
    __COMPACT7B_OCCUPANCYSENSING_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY(CQorvoClusterPIROccupancySensing,
        m_uUnoccupiedToOccupiedDelay)
    __COMPACT7B_OCCUPANCYSENSING_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD(CQorvoClusterPIROccupancySensing,
        m_nUnoccupiedToOccupiedThreshold)
__COMPACT7B_END_ATTRIBUTE_MAP()


void CQorvoClusterPIROccupancySensing::AfterPowerup(void)
{
    const CZigBeeAttribute *const pOccupiedToUnoccupiedDelay = FindAttribute(attributePIROccupiedToUnoccupiedDelay);
    const CZigBeeAttribute *const pUnoccupiedToOccupiedDelay = FindAttribute(attributePIRUnoccupiedToOccupiedDelay);
    const CZigBeeAttribute *const pUnoccupiedToOccupiedThreshold = FindAttribute(attributePIRUnoccupiedToOccupiedThreshold);
    ASSERT(pOccupiedToUnoccupiedDelay);
    ASSERT(pUnoccupiedToOccupiedDelay);
    ASSERT(pUnoccupiedToOccupiedThreshold);

    // Invoke base class handler first
    CZigBeeCluster::AfterPowerup();

    if(pOccupiedToUnoccupiedDelay->Load(*this))
    {
        OnAttributeChanged(attributePIROccupiedToUnoccupiedDelay);
    }
    if(pUnoccupiedToOccupiedDelay->Load(*this))
    {
        OnAttributeChanged(attributePIRUnoccupiedToOccupiedDelay);
    }
    if(pUnoccupiedToOccupiedThreshold->Load(*this))
    {
        OnAttributeChanged(attributePIRUnoccupiedToOccupiedThreshold);
    }
}
