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
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeClusterLibrary.h"

#include "QorvoClusterPowerConfiguration.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterPowerConfiguration
const unsigned short CQorvoClusterPowerConfiguration::nClusterRevision =
	CQorvoClusterPowerConfiguration::revision;

CQorvoClusterPowerConfiguration::CQorvoClusterPowerConfiguration
	(CZigBeeFoundationApplication &application) :
CZigBeeCluster(application, id)
/*m_MainsVoltage(0x0),
m_MainsFrequency(0x0),*/
,m_MainsAlarmMask(0x00)
/*m_MainsVoltageMinThreshold(0x0000),
m_MainsVoltageMaxThreshold(0xFFFF),
m_MainsVoltageDwellTripPoint(0x0000),*/
,m_BatteryVoltage(0x0)
,m_BatteryPercentageRemaining(0x00)
/*m_BatteryManufacturer(""),*/
,m_BatterySize(0xFF)
/*m_BatteryAhrRating(0x0),*/
,m_BatteryQuantity(0x0)
/*m_BatteryRatedVoltage(0x0),*/
,m_BatteryAlarmMask(0x00)
,m_BatteryVoltageMinThreshold(0x00)
,m_BatteryVoltageThreshold1(0x00)
,m_BatteryVoltageThreshold2(0x00)
,m_BatteryVoltageThreshold3(0x00)
,m_BatteryPercentageMinThreshold(0x00)
,m_BatteryPercentageThreshold1(0x00)
,m_BatteryPercentageThreshold2(0x00)
,m_BatteryPercentageThreshold3(0x00)
,m_BatteryAlarmState(0x00000000)
/*m_Battery2Voltage(0x0),
m_Battery2PercentageRemaining(0x00),
m_Battery2Manufacturer(""),
m_Battery2Size(0xFF),
m_Battery2AhrRating(0x0),
m_Battery2Quantity(0x0),
m_Battery2RatedVoltage(0x0),
m_Battery2AlarmMask(0x00),
m_Battery2VoltageMinThreshold(0x00),
m_Battery2VoltageThreshold1(0x00),
m_Battery2VoltageThreshold2(0x00),
m_Battery2VoltageThreshold3(0x00),
m_Battery2PercentageMinThreshold(0x00),
m_Battery2PercentageThreshold1(0x00),
m_Battery2PercentageThreshold2(0x00),
m_Battery2PercentageThreshold3(0x00),
m_Battery2AlarmState(0x00000000),
m_Battery3Voltage(0x0),
m_Battery3PercentageRemaining(0x00),
m_Battery3Manufacturer(""),
m_Battery3Size(0xFF),
m_Battery3AhrRating(0x0),
m_Battery3Quantity(0x0),
m_Battery3RatedVoltage(0x0),
m_Battery3AlarmMask(0x00),
m_Battery3VoltageMinThreshold(0x00),
m_Battery3VoltageThreshold1(0x00),
m_Battery3VoltageThreshold2(0x00),
m_Battery3VoltageThreshold3(0x00),
m_Battery3PercentageMinThreshold(0x00),
m_Battery3PercentageThreshold1(0x00),
m_Battery3PercentageThreshold2(0x00),
m_Battery3PercentageThreshold3(0x00),
m_Battery3AlarmState(0x00000000)*/
{
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterPowerConfiguration attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterPowerConfiguration, CZigBeeCluster)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterPowerConfiguration)
    __COMPACT7B_CLUSTERREVISION(CQorvoClusterPowerConfiguration)
    __COMPACT7B_POWERCONFIGURATION_CLUSTER_ATTRIBUTES()
__COMPACT7B_END_ATTRIBUTE_MAP()

