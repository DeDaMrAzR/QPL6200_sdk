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

/** @file "ZigBee.h"
 *
 * Ubisys stack glue header
*/

#ifndef _ZIGBEE_H_
#define _ZIGBEE_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include <cstring>
#include <cstdlib>
#include <vector>
//#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Synchronization.h"
#include "MemoryDevice.h"
#include "PersistentStorage.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "Compact15dot4.h"
#include "Compact15dot4Ex.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "IEEE802154MACPIB.h"
#include "IEEE802154MAC.h"
#include "IEEE802154MCF.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeCommissioningAttributes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeDeviceProfile.h"
#include "ZigBeeDeviceObject.h"
#include "ZigBeeNetworkCommands.h"
#include "ZigBeeNetwork.h"
#include "ZigBeeStorage.h"
#include "ZigBeeCommissioningAttributesEx.h"
#include "ZigBeeStatistics.h"

#include "ISO13239.h"

#include "ZigBeeClusterLibrary.h"
#include "ZigBeeClusterBasic.h"
#include "ZigBeeClusterBasicEx.h"
#include "ZigBeeClusterIdentify.h"
#include "ZigBeeClusterIdentifyClient.h"
#include "ZigBeeClusterIdentifyEx.h"
#include "ZigBeeClusterManagementIdentify.h"
#include "ZigBeeClusterGroups.h"
#include "ZigBeeClusterScenes.h"
#include "ZigBeeClusterCommissioning.h"
#include "ZigBeeClusterOTAUpgrade.h"
#include "ZigBeeClusterTemperatureMeasurement.h"
#include "ZigBeeClusterDiagnostics.h"
#include "ZigBeeClusterRelativeHumidityMeasurement.h"
#include "ZigBeeClusterElectricalMeasurement.h"
#include "ZigBeeClusterMetering.h"
#include "ZigBeeClusterOnOff.h"
#include "ZigBeeClusterOnOffEx.h"
#include "ZigBeeClusterOnOffClient.h"
#include "ZigBeeClusterLevelControl.h"
#include "ZigBeeClusterLevelControlEx.h"
#include "ZigBeeClusterLevelControlClient.h"
#include "ZigBeeClusterOccupancySensing.h"
#include "ZigBeeClusterIASZone.h"
#include "ZigBeeClusterIASZoneEx.h"
#include "ZigBeeClusterWindowCovering.h"
#include "ZigBeeClusterThermostat.h"

#if defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)
#include "ZigBeeClusterPollControl.h"
#endif

#if defined(UBI_DIVERSITY_ZCL_WWAH_SERVER)
#include "ZigBeeClusterAllHubs.h"
#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER

#if defined(GP_ZGP_IEEE_ENDPOINT)
//GreenPower
#include "ZigBeeClusterGPBase.h"
#include "ZigBeeClusterGPProxy.h"
#include "ZigBeeClusterGPSink.h"
#include "ZigBeeGPStub.h"
#include "ZigBeeGPApplication.h"
#endif

//Qorvo ZCL supplements
#ifdef UBI_DIVERSITY_ZCL_POWER_CONFIGURATION_SERVER
#if defined(GP_APP_DIVERSITY_BATTERY_SUPPORTED)
#include "QorvoClusterPowerConfigurationBattery.h"
#else // using default
#include "QorvoClusterPowerConfiguration.h"
#endif
#endif // UBI_DIVERSITY_ZCL_POWER_CONFIGURATION_SERVER
#include "QorvoClusterGroupsClient.h"
#include "QorvoClusterScenes.h"
#include "QorvoClusterScenesClient.h"
#include "QorvoClusterThermostatUserInterfaceConfiguration.h"
#include "QorvoClusterColorControl.h"
#include "QorvoClusterColorControlClient.h"
#if defined(UBI_DIVERSITY_ZCL_ILLUMINANCE_MEASUREMENT_SERVER)
#include "QorvoClusterIlluminanceMeasurement.h"
#endif // UBI_DIVERSITY_ZCL_ILLUMINANCE_MEASUREMENT_SERVER
#if defined(UBI_DIVERSITY_ZCL_PIR_OCCUPANCY_SENSING)
#include "QorvoClusterPIROccupancySensing.h"
#endif // UBI_DIVERSITY_ZCL_PIR_OCCUPANCY_SENSING
#include "QorvoClusterIASZoneClient.h"

// Qorvo chip specific configurations
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
#include "QorvoClusterOTAUpgradeXp4001.h"
#endif
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
#include "InternalFlashMemoryDeviceXP4001.h"
#endif
#if defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#include "QorvoClusterTouchLinkCommissioningSecurity.h"
#endif //defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#include "ZigBeeClusterTouchlink.h"
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#include "QorvoClusterTouchLinkCommissioningClient.h"
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT

#include "global.h"

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#ifndef ZIGBEE_ENDPOINT_1
#define ZIGBEE_ENDPOINT_1 1
#endif // ZIGBEE_ENDPOINT_1

#define ZIGBEE_FACTORY_RESET_BUSY_TAG_ID    0xFA

#define ZIGBEE_FACTORY_RESET_STATE_BUSY     0x0B
#define ZIGBEE_FACTORY_RESET_STATE_DONE     0xD0

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//Requests
void ubiZPro_Init(void);
void ubiZPro_OnIdle(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_ZIGBEE_H_
