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

/** @file "ZigBeeStackConfiguration.cpp"
 *
 *  source file includes configuration to the ZigBee stack and clusters
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** @brief Marco define for Zigbee 3.0 commissioning network discovery cycle */
#define APP_COMMISSIONING_SCAN_CYCLE 1

/*****************************************************************************
 *                    External data structures
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Core Stack configuration overrides
 *****************************************************************************/

// Define of the number of scan cycle in ZB 3.0 commissioning network discovery process
const unsigned char CZigBeeApplicationSupportManagement::nNetworkDiscoveryCycles = APP_COMMISSIONING_SCAN_CYCLE;

// Always form a network even if all channels do not pass the threshold criteria
const bool CZigBeeApplicationInformationBase::bForceFormation = true;

/*****************************************************************************
 *                    Commissioning configuration overrides
 *****************************************************************************/
/* Define of primary channel set used in a scan cycle during network discovery process
 *  - According to ZigBee Base Device Dehaviour 5.2.3,
 *    channel 11,15,20,25 will be used to support TouchLink commissioning
 */
//const unsigned int CZigBeeApplicationInformationBase::dwPrimaryChannelSet = (BM(11)|BM(15)|BM(20)|BM(25));

/* Define MAC's active scan duration during commissioning
 *  - default is 3
 */
// const unsigned char CZigBeeApplicationSupportManagement::nScanDuration = 3;

/*****************************************************************************
 *                    TCLK exchange configuration overrides
 *****************************************************************************/
// Always enable device to request a new trust center link key after joining
const bool CZigBeeApplicationInformationBase::bRequestTrustCenterLinkKey = true;

// General delay period in ticks to start a TCLK exchange procedure including querying stack core revision.
//const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::nTrustCenterKeyExchangeStartDelay = 0;

// Maximum of jitter delay period in ticks, which is meant to be added to the general start delay period of a TCLK exchange procedure.
//const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::nTrustCenterKeyExchangeStartJitter = 0;

// Maximum time duration in ticks for a node waiting for a response when sending an APS RequestKey or VerifyKey to the trust center while refreshing TC link key.
//const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::nTrustCenterKeyExchangeTimeout = 312500;

/*****************************************************************************
 *                    Stack Table configuration overrides
 *****************************************************************************/
//const unsigned int CZigBeeNetworkInformationBase::nNeighborTableLimit = 78;
const unsigned int CZigBeeNetworkInformationBase::nBroadcastTableLimit = 9;       //Default 32 - minimal 9
const unsigned int CZigBeeNetworkInformationBase::nRouteDiscoveryTableLimit = 10; //Default 32
//const unsigned int CZigBeeNetworkInformationBase::nRoutingTableLimit = 96;
//const unsigned int CZigBeeNetworkInformationBase::nSourceRouteLimit = 128;
//const unsigned int CZigBeeNetworkInformationBase::nAddressMapLimit = 96;


/*****************************************************************************
 *                    OTA upgrade server timing overrides
 *****************************************************************************/
// Set OTA current version
const unsigned int CZigBeeClusterOTAUpgrade::dwCurrentFileVersion = GP_VERSIONINFO_LONG;

// Set OTA query startup delay (in 16us ticks) - 1 min ( 60000000 / 16 ), default = 15s
const unsigned int CZigBeeClusterOTAUpgrade::nStartupDelayTicks = 3750000;

// Set OTA regular period (in 16us ticks) - 8h (8 * 3600 000000 / 16), default = 8h
//const unsigned int CZigBeeClusterOTAUpgrade::nRegularQueryTicks = 1800000000;

// Set OTA retry period (in 16us ticks) - 1h (3600 000000 / 16), default = 1m
const unsigned int CZigBeeClusterOTAUpgrade::nRetryQueryTicks = 225000000;

/*****************************************************************************
 *                    Distributed security link key overrides
 *****************************************************************************/
/**
*   Porting note:
*   Some systems will require the official ZB3.0 security key to be used
*   Override the default pre-certification key here if you have the key.
*/
//const unsigned char CZigBeeApplicationSecurityMaterial::
//    abDistributedSecurityLinkKey[16] = { 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5,
//    0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf };

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
/*****************************************************************************
 *                    WWAHu server overrides
 *****************************************************************************/
// set wwahu's default MACPollFailureWaitTime to 3
uint8_t CZigBeeClusterAllHubs::nDataPollRetryInterval = 3;

#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER
