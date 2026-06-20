/*
 *   Copyright (c) 2017-2019, Qorvo Inc
 *
 *   Application Ble API
 *   Declarations of the public functions and enumerations of BLE.
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 */

/// @file "ZigBeeCommissioning.h"

#ifndef ZIGBEECOMMISSIONING_H
#define ZIGBEECOMMISSIONING_H
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** @brief Macro defines for Zigbee 3.0's trust center address */
/*          For distributed network - all f's */
#define DISTRIBUTED_TRUST_CENTER_ADDRESS 0xFFFFFFFFFFFFFFFFULL

#define MAX_TIMER_SERVICE_REFRESH_PERIOD (UInt32)100000
// ensure timer refreshment won't block device from going to sleep
#define TIMER_SERVICE_REFRESH_PERIOD                                                                                   \
    (UInt32)(std::min(MAX_TIMER_SERVICE_REFRESH_PERIOD,                                                                \
                      (std::max((UInt32)50000, (UInt32)(HAL_DEFAULT_GOTOSLEEP_THRES + 1000)))))
/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
#define ZIGBEE_DEVTYPE_ROUTER    1
#define ZIGBEE_DEVTYPE_ENDDEVICE 2

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////
// Network capabilities setting API
/** @brief Function to set Network Capabilities NIB attribute.
 *          Details is in ZigBee Specification Table 3-62 Capability Information Bit-Fields
 *   @param devType            ZIGBEE_DEVTYPE_ROUTER or ZIGBEE_DEVTYPE_ENDDEVICE
 *   @param powerSrc           PowerSource in Capabilities,
 *          - True for Main-powered device
 *          - False for other power source
 *   @param rxOnWhenIdle       Enable/Disable Receiver On when Idle in Network Capabilities
*/
void Application_ZigBeeSetCapabilities(UInt8 devType, Bool powerSrc, Bool rxOnWhenIdle);

/** @brief Function to check if device is end device
*/
Bool Application_ZigBeeIsEndDevice(void);

/** @brief Function to check if device is router
*/
Bool Application_ZigBeeIsRouter(void);

/** @brief Function to check if device is Rx On when Idle
*/
Bool Application_ZigBeeIsRxOnWhenIdle(void);

/////////////////////////////////////////////////////////////////////////////
// Zigbee Stack timer service API
void Application_ZigBeeUpdateCurrentTime(void);

/////////////////////////////////////////////////////////////////////////////
// Commissioning/Rejoining parameter API
/** @brief Function to get current stack rejoin mode
*/
unsigned char Application_ZigBeeGetDefaultRejoinMode(void);

/** @brief Function to set current stack rejoin mode
 *          The mode should be startupAllowSecureRejoin and/or startupAllowTrustCenterRejoin.
 *   @param mode             rejoin mode to set.
*/
void Application_ZigBeeSetDefaultRejoinMode(unsigned char mode);

/** @brief Function to get current rejoin fast retry Limit
*/
UInt16 Application_ZigBeeGetRejoinFastRetry(void);

/** @brief Function to get current base rejoin backoff setting
*/
UInt16 Application_ZigBeeGetRejoinBaseBackoff(void);

/** @brief Function to get current join fast retry Limit
*/
UInt16 Application_ZigBeeGetJoinFastRetry(void);

/** @brief Function to get number of commissioning Attempts
*/
UInt16 Application_ZigBeeGetCommissioningAttempt(void);

/** @brief Function to get number of rejoin Attempts
*/
UInt16 Application_ZigBeeGetRejoinAttempt(void);

/////////////////////////////////////////////////////////////////////////////
// Networking status check API
/** @brief Display ZigBee network information
*/
void Application_ZigBeeNetworkInfo(void);

/** @brief Function to check if device is joined to network
*/
Bool Application_ZigBeeIsJoinedNetwork(void);

/** @brief Function to check if device is busy in commissioning/forming network
*/
Bool Application_ZigBeeIsCommissioningInProgress(void);

/** @brief Function to check if the network is distributed network
*/
Bool Application_ZigBeeIsDistributedNetwork(void);

/** @brief Function to check if the device is trust center.
*/
Bool Application_ZigBeeIsTrustCenter(void);

/** @brief Function to check if device is performing factory reset
*/
Bool Application_ZigBeeIsFactoryResetInProgress(void);

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
/** @brief Function to return WWAHu's TouchlinkInterpanEnabled state
*/
Bool Application_ZigBeeIsAllHubsTouchLinkEnabled(void);

#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER
/////////////////////////////////////////////////////////////////////////////
// Networking operation API
/** @brief Starts the Zigbee stack
*/
void Application_ZigBeeStart(void);

/** @brief Function to resume the network only for the factory reset on POR use case.
*/
void Application_ZigBeeStartupResume(void);

/** @brief Start ZB 3.0 Commissioning process
*/
void Application_ZigBeeStartCommissioning(void);

/** @brief Start ZB 3.0 rejoining process
*/
void Application_ZigBeeStartRejoining(void);

/** @brief Start ZB 3.0 Secure rejoin process
*/
void Application_ZigBeeStartSecureRejoin(void);

/** @brief Start ZB 3.0 TC rejoin process
*/
void Application_ZigBeeStartTcRejoin(void);

#ifdef GP_APP_DIVERSITY_DISTRIBUTED_NETWORK
/** @brief Start Distributed network
*/
void Application_ZigBeeStartDistributedNetwork(void);

#endif // GP_APP_DIVERSITY_DISTRIBUTED_NETWORK
/** @brief Leave current ZB 3.0 network Leave
 * !Note that Leave should not be called before Zigbee is started and commissioning is in progress
 * i.e. AppZigBee_IsCommissioningInProgress() = True.
*/
void Application_ZigBeeLeave(void);

/** @brief Function to stop Zigbee 3.0 commissioning
*/
void Application_ZigBeeStopCommissioning(void);

/** @brief get permit joining duration
*/
UInt8 Application_ZigBeeGetPermitJoiningDuration(void);

/** @brief Enable permit joining
*/
Bool Application_ZigBeePermitJoining(UInt8 duration);

/////////////////////////////////////////////////////////////////////////////
// Factory Reset API
/** @brief Function to Reset Zigbee network into factory default
*/
void Application_ZigBeeFactoryReset();

#ifdef GP_APP_DIVERSITY_TOUCHLINK
/////////////////////////////////////////////////////////////////////////////
// TouchLink operation API
/** @brief function to control ZB 3.0 touchlink feature
*   note that touchlink will NOT be enabled if it is prohibited by WWAH cluster.
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeStartTouchLink(Bool enable);

/** @brief function to control ZB 3.0 touchlink stealing feature
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeEnableTouchLinkStealing(Bool enable);

/** @brief function to control ZB 3.0 touchlink reset to factory default feature
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeEnableTouchLinkResetToFactoryNew(Bool enable);

#endif //GP_APP_DIVERSITY_TOUCHLINK
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
/** @brief function to start ZB 3.0 touchlink initiator commissioning
 *
 *   @param resetToFactoryNewRequest      force to send the reset to factory new request to the target
 *   @param forceSecondaryScan            configure to have channel scanning over both primary and secondary channels
 */
void Application_ZigBeeStartTouchLinkCommissioning(bool resetToFactoryNewRequest = false,
                                                   bool forceSecondaryScan = false);

/** @brief Callback indicates rejoining status from TouchLink
 *
 *   @param rejoinInProgress      status of TL rejoining
 */
void Application_cbTLTriggeredRejoin(bool rejoinInProgress);

#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
/** @brief function to force to use touchlink client instance
 */
void Application_ForceZigbeeTouchLinkClientInstance(bool bIsClient);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // ZIGBEECOMMISSIONING_H
