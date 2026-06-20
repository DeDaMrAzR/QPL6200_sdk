/*
 * Copyright (c) 2017-2019, Qorvo Inc
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

/** @file "ZigBeeCommissioning.c"
 *
 * This ZigBee module can be triggered by the application
 * to perform commissioning and related handle. Also including
 * the callback handler of the status return
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"
#include "ZigBeeCommissioning.h"
#include "ZigBeeCommissioning_Callback.h"
#if defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#include "ZigBeeTouchLink_Callback.h"
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)

#include "App.h"
#include "App_Common.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// ZigBee Commissioning attempt for application logic

/** @brief Marco define for Zigbee 3.0 commissioning attempts */
#define APP_JOIN_ATTEMPTS               5

/** @brief Marco define for delay between each Zigbee 3.0 commissioning attempts */
#define APP_JOIN_RETRY_DELAY            MS_TO_US(3000)

/////////////////////////////////////////////////////////////////////////////
// ZigBee Rejoining attempt for application logic

/** @brief Marco define if applciation rejoin algorithm is enabled*/
//#define GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM

/** @brief Marco define for Zigbee 3.0 number of attempts to rejoin existing network */
#define APP_REJOIN_ATTEMPTS 3

/** @brief Marco define for delay between each rejoin attempts */
#define APP_REJOIN_RETRY_DELAY          MS_TO_US(3000)

/** @brief Marco define for base backoff delay between each rejoin cycle */
#define APP_REJOIN_BASE_BACKOFF_DELAY   MS_TO_US(60000)

/** @brief Marco define for Max limit of backoff delay */
#define APP_REJOIN_MAX_BACKOFF_DELAY    30 * MS_TO_US(60000)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct {
    /** @brief Static to keep track of commissioning attempt */
    Int16 joinAttempts;
    /** @brief Static to keep track of rejoining attempt */
    Int16 rejoinAttempts;
    /** @brief Static to keep track of is application triggered rejoining */
    Int16 appTriggeredRejoin;
    /** @brief Static to keep track of is stop Commissssioning enabled */
    bool StopCommissioningEnabled;
} ZigBeeNwkControlVars_t;

/*****************************************************************************
 *                    External data structures
 *****************************************************************************/
extern CZigBeeApplicationSupport aps;
extern CZigBeeFoundationApplication *pApplication;
extern CZigBeeDeviceObject zdo;

/*****************************************************************************
 *                    Stack configuration overrides
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// ZigBee 3.0 joining Algorithm Configuration

/** @brief Maximum number of "join via assocaition" attempts for a join cycle
 *          (excluding the first trigger by application)
 *          To support Stop commissioning, we use application logic instead
 */
const uint16_t CZigBeeApplicationSupportManagement::joinFastRetryLimit = APP_JOIN_ATTEMPTS;

/** @brief Delay between each "join via assocaition" attempts in a join cycle
 *          - 5s(312500 *16 us Ticks), default = 30s
 */
const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::joinFastRetryDelayTicks = APP_JOIN_RETRY_DELAY/16;

/** @brief Base Backoff Delay betwwen each join cycle for each Backoff retry,
 *         the delay will be doubled.(e.g 20s,40s,80s,160s...)
 *         Setting backoff to 0 means disabling backoff retry
 *         default = 0(disable backoff retry)
 */
const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::joinBaseBackoffTicks = 0;

/** @brief Maximum Backoff Delay define, Backoff Delay will not excess this define.
 *          - 15 mins(56250000 *16 us Ticks), default = 8 hours
 */
//const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::joinMaxBackoffTicks = 56250000;

/** @brief Maximum Backoff attempt to reset the Backoff delay to base backoff delay
 *         A value of 0 means do not reset the backoff duration.
 *         Default is 0
 */
//const uint16_t CZigBeeApplicationSupportManagement::joinBackoffLimit=0;

/////////////////////////////////////////////////////////////////////////////
// ZigBee 3.0 Re-joining Algorithm Configuration
//    The rejoining Algorithm is adapting to the WWAH Rejoin Algorithm
//    Details please refer to Enable WWAH Rejoin Algorithm in WWAH Cluster specification

/** @brief Default number of rejoin attempts for a rejoin cycle
 *          (excluding the first trigger by application)
 *          Setting to 0 will disable default rejoining algorithm
 */
#if defined(GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM)
const uint16_t CZigBeeApplicationSupportManagement::rejoinDefaultFastRetryLimit = 0;
#else
const uint16_t CZigBeeApplicationSupportManagement::rejoinDefaultFastRetryLimit = APP_REJOIN_ATTEMPTS;
#endif //GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM

/** @brief Default Delay between each rejoin attempts in a rejoin cycle
 *          default = 30s
 */
const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::rejoinDefaultFastRetryDelayTicks = APP_REJOIN_RETRY_DELAY/16;

/** @brief Default Base Backoff Delay betwwen each rejoin cycle
 *          for each Backoff retry, the delay will be doubled.(e.g 20s,40s,80s,160s...)
 *          Setting backoff to 0 means disabling backoff retry
 *          default = 90s
 */
const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::rejoinDefaultBaseBackoffTicks = APP_REJOIN_BASE_BACKOFF_DELAY/16;

/** @brief Maximum Backoff Delay define, Backoff Delay will not excess this define.
 *          default = 8 hours
 */
const CCompactTimerService::ticks_diff_t CZigBeeApplicationSupportManagement::rejoinDefaultMaxBackoffTicks = APP_REJOIN_MAX_BACKOFF_DELAY/16;

/** @brief Maximum Backoff attempt to reset the Backoff delay to base backoff delay
 *         A value of 0 means do not reset the backoff duration.
 *         Default is 0
 */
const uint16_t CZigBeeApplicationSupportManagement::rejoinDefaultBackoffLimit = 0;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static ZigBeeNwkControlVars_t ZigBeeNwkControl;
static Bool isFactoryResetInProgress = false;
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
static Bool tlTriggeredRejoin = false;
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT

/*****************************************************************************
 *                    private functions
 *****************************************************************************/
#if defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)

bool CZigBeeNetworkScanManager::EvaluateApplicationFilters(const CIEEE802154PANDescriptor& descriptor,
                                                           const CPacket& sdu) const
{
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
    if(!tlTriggeredRejoin)
    {
        return true;
    }
    if(appsEp_1.getTouchLinkTargetInfo().m_nDeviceType == CQorvoClusterTouchLinkCommissioningClient::typeEndDevice)
    {
        return true;
    }
    else
    {
        // If the Target is a Router or Coordinator, the address of the Scan Response should match the address of the
        // beacon
        return (
            descriptor.m_addressCoordinator.m_qwExtended == appsEp_1.getTouchLinkTargetInfo().targetAddress.m_wShort);
    }
#endif
    return true;
}

bool zigBee_IsTouchlinkSessionActive(void)
{
    bool sessionActive = false;

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
    if (!sessionActive)
    {
        sessionActive = appsEp_1.m_touchLinkClient.IsSessionActive();
    }
#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#if defined(GP_APP_DIVERSITY_TOUCHLINK)
    if (!sessionActive)
    {
        sessionActive = appsEp_1.m_touchLink.IsSessionActive();
    }
#endif //GP_APP_DIVERSITY_TOUCHLINK

    return sessionActive;
}

#endif //defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#if defined(GP_APP_DIVERSITY_TOUCHLINK)
void zigBee_EnableTouchLink(Bool enable)
{
    // Locate the TouchLink server cluster on that end-point
    CZigBeeClusterTouchlink *pTouchlink =
        static_cast<CZigBeeClusterTouchlink *>
        (pApplication->FindCluster(CZigBeeClusterTouchlink::id,
        CZigBeeCluster::flagInbound));

    if (pTouchlink)
    {
        // enable receiver if touchLink is enabled.
        if(enable)
        {
            //  enable receiver Rx in phy layer.
            aps.m_nwk.m_mac.m_phy.OnRequestSetTransceiverState(CIEEE802154PHY::transceiverReceiverEnabled);
        }

        // enable TouchLink
        CZigBeeClusterTouchlinkDispatch::EnableTouchlink(aps,enable);
    }
}

#endif // GP_APP_DIVERSITY_TOUCHLINK
void zigBee_executeStopCommissioning(void)
{
    // remove the next commissioning schedular
    aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

    // clear stop commissioning flag
    ZigBeeNwkControl.StopCommissioningEnabled = false;

    //reset all application states
    ZigBeeNwkControl.joinAttempts = 0;
    ZigBeeNwkControl.rejoinAttempts = 0;
    ZigBeeNwkControl.appTriggeredRejoin = false;

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: commissioning stopped.", 0);

#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
    // resume touchlink(target) feature (enable to be stolen from other switch)
    Application_ZigBeeStartTouchLink(true);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
}

/*****************************************************************************
 *                    Public functions
 *****************************************************************************/
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
void Application_ZigBeeSetCapabilities(UInt8 devType, Bool powerSrc, Bool rxOnWhenIdle)
{
    unsigned char newCapabilities = aps.m_nwk.m_nlme.m_nib.m_bCapabilities;

    // In default deivce must be issued a 16-bit network address in association.
    newCapabilities |= CIEEE802154MAC::capabilityAllocateAddress;

    // Clear FFD flag in MAC capability in default
    newCapabilities &= ~CIEEE802154MAC::capabilityFullFunctionDevice;

    // Full function device(FFD) handle
    if(devType == ZIGBEE_DEVTYPE_ROUTER)
    {
#if !defined(GP_MACCORE_DIVERSITY_FFD)
        // Reject device become router if MAC FFD is not supported.
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: FFD is not supported, setting device type to ZED.", 0);
#else
        // Set FFD flag in MAC capability
        newCapabilities |= CIEEE802154MAC::capabilityFullFunctionDevice;
#endif // !GP_MACCORE_DIVERSITY_FFD
    }

    // Rx On when Idle Handle
    if(rxOnWhenIdle)
    {
        newCapabilities |= CIEEE802154MAC::capabilityReceiverOnWhenIdle;
    }
    else
    {
        newCapabilities &= ~CIEEE802154MAC::capabilityReceiverOnWhenIdle;
    }

    // Power Source Handle On when Idle Handle
    if(powerSrc)
    {
        newCapabilities |= CIEEE802154MAC::capabilityMainsPowered;
    }
    else
    {
        newCapabilities &= ~CIEEE802154MAC::capabilityMainsPowered;
    }

    // Update Network Capabilities in NIB attribute if changes detected.
    if(newCapabilities != aps.m_nwk.m_nlme.m_nib.m_bCapabilities)
    {
        aps.m_nwk.m_nlme.SetCapabilities(newCapabilities);

        GP_LOG_PRINTF("NWK: Updated NWK capability: 0x%x", 0, aps.m_nwk.m_nlme.m_nib.m_bCapabilities);
    }
}

/** @brief Function to check if device is end device
*/
Bool Application_ZigBeeIsEndDevice(void)
{
    return aps.m_nwk.m_nlme.IsEndDevice();
}

/** @brief Function to check if device is router
*/
Bool Application_ZigBeeIsRouter(void)
{
    return aps.m_nwk.m_nlme.IsRouter();
}

/** @brief Function to check if device is Rx On when Idle
*/
Bool Application_ZigBeeIsRxOnWhenIdle(void)
{
    return aps.m_nwk.m_nlme.IsReceiverOnWhenIdle();
}

/////////////////////////////////////////////////////////////////////////////
// Zigbee Stack timer service API

void Application_ZigBeeUpdateCurrentTime(void)
{
    theTimerService.UpdateCurrentTime();
}

/////////////////////////////////////////////////////////////////////////////
// Commissioning/Rejoining parameter API
/** @brief Function to get current stack rejoin mode
*/
unsigned char Application_ZigBeeGetDefaultRejoinMode(void)
{
    return aps.m_apsme.m_rejoinStartupMode;
}

/** @brief Function to set current stack rejoin mode
 *          The mode should be startupAllowSecureRejoin and/or startupAllowTrustCenterRejoin.
 *   @param mode             rejoin mode to set.
*/
void Application_ZigBeeSetDefaultRejoinMode(unsigned char mode)
{
    aps.m_apsme.m_rejoinStartupMode = mode;
}

/** @brief Function to get current stack rejoin fast retry Limit
*/
UInt16 Application_ZigBeeGetRejoinFastRetry(void)
{
    return aps.m_apsme.rejoinFastRetryLimit;
}

/** @brief Function to get current stack rejoin base backoff setting
*/
UInt16 Application_ZigBeeGetRejoinBaseBackoff(void)
{
    return aps.m_apsme.rejoinBaseBackoffTicks;
}

/** @brief Function to get current stack join fast retry Limit
*/
UInt16 Application_ZigBeeGetJoinFastRetry(void)
{
    return aps.m_apsme.joinFastRetryLimit;
}

/** @brief Function to get number of commissioning Attempts
*/
UInt16 Application_ZigBeeGetCommissioningAttempt(void)
{
    return ZigBeeNwkControl.joinAttempts;
}

/** @brief Function to get number of rejoin Attempts
*/
UInt16 Application_ZigBeeGetRejoinAttempt(void)
{
    return ZigBeeNwkControl.rejoinAttempts;
}

/////////////////////////////////////////////////////////////////////////////
// Networking status check API
/** @brief Display ZigBee network information
*/
void Application_ZigBeeNetworkInfo(void)
{
#ifdef GP_DIVERSITY_LOG
#if defined(GP_APP_DIVERSITY_ZB)
    if(Application_ZigBeeIsRouter())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device Type - Router", 0);
    }
    else
    {
        if(Application_ZigBeeIsRxOnWhenIdle())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device Type - End Device", 0);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device Type - Sleepy End Device", 0);
        }
    }

    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device is NOT in network",0);
    }
    else
    {
        if(Application_ZigBeeIsDistributedNetwork())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device is in Distributed network",0);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device is in Centralized network",0);
        }
    }

#endif //GP_APP_DIVERSITY_ZB
    GP_LOG_SYSTEM_PRINTF("============================", 0);
#endif // GP_DIVERSITY_LOG
}

/** @brief Function to check if device is joined to network
*/
Bool Application_ZigBeeIsJoinedNetwork(void)
{
    return aps.m_nwk.m_nlme.IsJoined();
}

/** @brief Function to check if device is busy in commissioning/forming network
*/
Bool Application_ZigBeeIsCommissioningInProgress(void)
{
    return aps.m_apsme.IsStartupInProgress();
}

/** @brief Function to check if the network is distributed network
*/
Bool Application_ZigBeeIsDistributedNetwork(void)
{
    // If the network is a distributed security network, trust center address is all F's.
    return aps.m_apsme.IsDistributedSecurityNetwork();
}

/** @brief Function to check if the device is trust center.
*/
Bool Application_ZigBeeIsTrustCenter(void)
{
    return aps.m_apsme.IsTrustCenter();
}

/** @brief Function to check if device is performing factory reset
*/
Bool Application_ZigBeeIsFactoryResetInProgress(void)
{
    return isFactoryResetInProgress;
}

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
/** @brief Function to return WWAHu's TouchlinkInterpanEnabled state
*/
Bool Application_ZigBeeIsAllHubsTouchLinkEnabled(void)
{
    // Locate the WWAHu server cluster on that end-point
    CZigBeeClusterAllHubs *pWWAHu =
        static_cast<CZigBeeClusterAllHubs *>
        (pApplication->FindCluster(CZigBeeClusterAllHubs::id,
        CZigBeeCluster::flagInbound));

    ASSERT(pWWAHu);
    return pWWAHu->bAllowTouchlink;
}
#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER

/////////////////////////////////////////////////////////////////////////////
// Networking operation API
/** @brief Function to starts running the Zigbee stack
*/
void Application_ZigBeeStart(void)
{
    // Return if device is NOT commissioned
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Startup - Not in network", 0);
        return;
    }

    // ZED required to rejoin network according to BDB 7.1 - Initialization procedure
    // TC join in distributed network is prohibited(security vulnerability 19.09 and 19.10)
    if(aps.m_nwk.m_nlme.IsEndDevice())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: (ZED)Rejoining the network...", 0);
        if (Application_ZigBeeIsDistributedNetwork())
        {
            Application_ZigBeeStartSecureRejoin();
        }
        else
        {
            Application_ZigBeeStartRejoining();
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: (ZR)Resuming the network...", 0);

        // allow resume or rejoin network
        // TC join in distributed network is prohibited(security vulnerability 19.09 and 19.10)
        if (Application_ZigBeeIsDistributedNetwork())
        {
            aps.m_apsme.Startup(
                CZigBeeApplicationSupportManagement::startupAllowResume |
                CZigBeeApplicationSupportManagement::startupAllowSecureRejoin);
        }
        else
        {
            aps.m_apsme.Startup(
                CZigBeeApplicationSupportManagement::startupAllowResume |
                CZigBeeApplicationSupportManagement::startupAllowRejoining);
        }
    }
}

/** @brief Function to resume the network only for the factory reset on POR use case.
*/
void Application_ZigBeeStartupResume(void)
{
    // Return if device is NOT commissioned
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_PRINTF("ZIGBEE: Startup - Not in network", 0);
        return;
    }


    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Resuming the network before factory reset...", 0);

    aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowResume);
}

/** @brief Start ZB 3.0 Commissioning process
*/
void Application_ZigBeeStartCommissioning(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        if (Application_ZigBeeIsCommissioningInProgress())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start commissioning failed, commissioning is in progress.", 0);
        }
#if defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        else if(zigBee_IsTouchlinkSessionActive())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start commissioning failed, Touchlink session is active.", 0);
        }
#endif //defined(GP_APP_DIVERSITY_TOUCHLINK) || defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start ZB 3.0 commissioning...", 0);
#ifdef GP_APP_DIVERSITY_TOUCHLINK
            // temporary disable Touchlink(target)
            Application_ZigBeeStartTouchLink(false);

#endif // GP_APP_DIVERSITY_TOUCHLINK
            // Check the Startup Attribute's startup control - Join network
            CZigBeeCommissioningAttributesEx attributes;

            attributes.Load();
            attributes.m_bStartupControl = CZigBeeCommissioningAttributes::startupJoinNetwork;
            attributes.m_qwExtendedPANID = 0;
            attributes.m_qwTrustCenterAddress = 0;
            attributes.Store();
            attributes.Commit(aps);

            // initialize the commissioning attempt count.
            ZigBeeNwkControl.joinAttempts = 0;

            // Stop current running commissioning schedular before start
            aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

            // Start the ZB 3.0 commissioning process
            aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowAssociation);
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start commissioning failed, device is in network", 0);
    }
}

/** @brief Start ZB 3.0 rejoining process
*/
void Application_ZigBeeStartRejoining(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start rejoin failed, device is NOT in network", 0);
        return;
    }

    if (Application_ZigBeeIsCommissioningInProgress())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start rejoin failed, commissioning is in progress.", 0);
        return;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start rejoin existing network...", 0);

    if (Application_ZigBeeGetRejoinFastRetry())
    {
        // enable appTriggeredRejoin flag, rejoin limit will include the application triggered rejoin
        ZigBeeNwkControl.appTriggeredRejoin = true;
    }

    // Stop current running commissioning schedular before start
    aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

    // update stack's rejoin mode
    Application_ZigBeeSetDefaultRejoinMode(CZigBeeApplicationSupportManagement::startupAllowRejoining);

    // launch stack startup with flag - default rejoin
    aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowRejoining);
}

/** @brief Start ZB 3.0 Secure rejoin process
*/
void Application_ZigBeeStartSecureRejoin(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start secure rejoin failed, device is NOT in network", 0);
        return;
    }

    if (Application_ZigBeeIsCommissioningInProgress())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start secure rejoin failed, commissioning is in progress.", 0);
        return;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start secure rejoin...", 0);

    if (Application_ZigBeeGetRejoinFastRetry())
    {
        // enable appTriggeredRejoin flag, rejoin limit will include the application triggered rejoin
        ZigBeeNwkControl.appTriggeredRejoin = true;
    }

    // Stop current running commissioning schedular before start
    aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

    // update stack's rejoin mode
    Application_ZigBeeSetDefaultRejoinMode(CZigBeeApplicationSupportManagement::startupAllowSecureRejoin);

    // launch stack startup with flag - Secure rejoin
    aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowSecureRejoin);
}

/** @brief Start ZB 3.0 TC rejoin process
*/
void Application_ZigBeeStartTcRejoin(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TC rejoin failed, device is NOT in network", 0);
        return;
    }

    if (Application_ZigBeeIsCommissioningInProgress())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TC rejoin failed, commissioning is in progress.", 0);
        return;
    }

    if (Application_ZigBeeIsDistributedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TC rejoin failed, TC rejoin is prohibited in Distributed network", 0);
        return;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TC rejoin...", 0);

    if (Application_ZigBeeGetRejoinFastRetry())
    {
        // enable appTriggeredRejoin flag, rejoin limit will include the application triggered rejoin
        ZigBeeNwkControl.appTriggeredRejoin = true;
    }

    // Stop current running commissioning schedular before start
    aps.m_apsme.ScheduleNextStartupTimer(CZigBeeApplicationSupportManagement::startupAbort);

    // update stack's rejoin mode
    Application_ZigBeeSetDefaultRejoinMode(CZigBeeApplicationSupportManagement::startupAllowTrustCenterRejoin);

    // launch stack startup with flag - TC rejoin
    aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowTrustCenterRejoin);
}

#ifdef GP_APP_DIVERSITY_DISTRIBUTED_NETWORK
/** @brief Start Distributed network
*/
void Application_ZigBeeStartDistributedNetwork(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        if (Application_ZigBeeIsCommissioningInProgress())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Distributed network formation failed, commissioning is in progress.", 0);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start forming Distributed network...", 0);

#ifdef GP_APP_DIVERSITY_TOUCHLINK
            // temporary disable Touchlink(target)
            Application_ZigBeeStartTouchLink(false);

#endif // GP_APP_DIVERSITY_TOUCHLINK
            // Check the Startup Attribute's startup control form network, trust center address to all f's
            CZigBeeCommissioningAttributesEx attributes;
            attributes.Load();

            attributes.m_bStartupControl = CZigBeeCommissioningAttributes::startupFormNetwork;
            attributes.m_qwTrustCenterAddress = DISTRIBUTED_TRUST_CENTER_ADDRESS;

            // store updated attributes
            attributes.Store();
            attributes.Commit(aps);

            // Start forming distributed network
            aps.m_apsme.Startup(CZigBeeApplicationSupportManagement::startupAllowNetworkFormation);
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Distributed network formation failed, device is in network", 0);
    }
}
#endif // GP_APP_DIVERSITY_DISTRIBUTED_NETWORK

/** @brief Leave current ZB 3.0 network Leave
 * !Note that Leave should not be called before Zigbee is started and commissioning is in progress
 * i.e. Application_ZigBeeIsCommissioningInProgress() = True.
*/
void Application_ZigBeeLeave(void)
{
    if(Application_ZigBeeIsJoinedNetwork() && !Application_ZigBeeIsCommissioningInProgress())
    {
        // Issue a network leave
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: send NWK Leave, MAC Address: 0x%llx", 0, aps.m_nwk.m_mac.qwExtendedAddress);
        aps.m_nwk.m_nlme.OnRequestLeave(CZigBeeAddress(0ull,CZigBeeAddress::wOnlyExtendedAddress),false,false);
    }
}

/** @brief Function to stop Zigbee 3.0 commissioning
*/
void Application_ZigBeeStopCommissioning(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Stopping commissioning...", 0);

        // Device is commissioning attempt in progress,
        // queue up the stop commissioning request
        if (Application_ZigBeeIsCommissioningInProgress())
        {
            //set stop commissioning flag
            ZigBeeNwkControl.StopCommissioningEnabled = true;
        }
        // Device is in between commissioning attempts
        // Execute the stop commissioning
        else if (Application_ZigBeeGetCommissioningAttempt())
        {
            zigBee_executeStopCommissioning();
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Stop commissioning failed, device is NOT commissioning in progress", 0);
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("device is in network", 0);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Factory Reset API
/** @brief Function to Reset Zigbee network into factory default
*/
void Application_ZigBeeFactoryReset()
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Starting factory reset...",0);
    {
        //Update factory reset flag in progress to true
        isFactoryResetInProgress = true;

        if(Application_ZigBeeIsJoinedNetwork() && !Application_ZigBeeIsCommissioningInProgress())
        {
            // Issue a network leave, factory reset will be launched at OnZigBeeNetworkLeave()
            Application_ZigBeeLeave();
        }
        // Restore records in all endpoints
        CZigBeeAllFoundationApplications applications(aps);
        while(applications.Advance())
        {
            applications->RestoreDefaults(false);
        }

        //Clear any commissioning info
        aps.m_apsme.FactoryReset(0x0);

        GP_LOG_SYSTEM_PRINTF("ZIGBEE: network information cleared",0);
        gpLog_Flush();
    }
}

/////////////////////////////////////////////////////////////////////////////
// TouchLink operation API
#ifdef GP_APP_DIVERSITY_TOUCHLINK
/** @brief function to control ZB 3.0 touchlink feature
*   note that touchlink will NOT be enabled if it is prohibited by WWAH cluster.
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeStartTouchLink(Bool enable)
{
    if (enable)
    {
#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
        if (!Application_ZigBeeIsAllHubsTouchLinkEnabled())
        {
            GP_LOG_SYSTEM_PRINTF("WARN: Enabling Touchlink is prohibited by WWAHu", 0);
            return;
        }
#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER

        // generate new random PAN ID if current PAN is invalid
        if (aps.m_nwk.m_mac.m_pib.m_wPANID == 0xffff)
        {
            aps.m_nwk.m_mac.m_mlme.SetPANID(rand16());
        }

        // ensure operational channel is set if device is not in network
        if ((!aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel) || (!Application_ZigBeeIsJoinedNetwork()))
        {
            // set channel to 11, Rx channel should be set in ZLL channel(11,15,20,25) in order to receive scan request from TouchLink device
            aps.m_nwk.m_mac.m_phy.SetCurrentChannel(11);
        }

        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Enabling TouchLink(Target), CH: %d", 0, aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Disabling TouchLink(Target)", 0);
    }
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
    Application_ForceZigbeeTouchLinkClientInstance(false);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
    zigBee_EnableTouchLink(enable);
}

/** @brief function to control ZB 3.0 touchlink stealing feature
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeEnableTouchLinkStealing(Bool enable)
{
    GP_LOG_SYSTEM_PRINTF("  %c%c%cabling TouchLink Stealing",0,
            enable ? '|' : 'D',
            enable ? 'E' : 'i',
            enable ? 'n' : 's');

    // Locate the TouchLink server cluster on that end-point
    CZigBeeClusterTouchlink *pTouchlink =
        static_cast<CZigBeeClusterTouchlink *>
        (pApplication->FindCluster(CZigBeeClusterTouchlink::id,
        CZigBeeCluster::flagInbound));

    if (pTouchlink)
    {
        enable ?
            (pTouchlink->m_nEnable |= CZigBeeClusterTouchlink::enableRecommissioning):
            (pTouchlink->m_nEnable &= ~CZigBeeClusterTouchlink::enableRecommissioning);
    }
}

/** @brief function to control ZB 3.0 touchlink reset to factory default feature
*
*   @param enable                Enable/disable the feature
*/
void Application_ZigBeeEnableTouchLinkResetToFactoryNew(Bool enable)
{
    GP_LOG_SYSTEM_PRINTF("  %c%c%cabling TouchLink Reset to Factory New",0,
            enable ? '|' : 'D',
            enable ? 'E' : 'i',
            enable ? 'n' : 's');

    // Locate the TouchLink server cluster on that end-point
    CZigBeeClusterTouchlink *pTouchlink =
        static_cast<CZigBeeClusterTouchlink *>
        (pApplication->FindCluster(CZigBeeClusterTouchlink::id,
        CZigBeeCluster::flagInbound));

    if (pTouchlink)
    {
        enable ?
            (pTouchlink->m_nEnable |= CZigBeeClusterTouchlink::enableFactoryReset):
            (pTouchlink->m_nEnable &= ~CZigBeeClusterTouchlink::enableFactoryReset);
    }
}

#endif // GP_APP_DIVERSITY_TOUCHLINK
#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT

/** @brief function to start ZB 3.0 touchlink initiator commissioning
*/
void Application_ZigBeeStartTouchLinkCommissioning(bool resetToFactoryNewRequest, bool forceSecondaryScan)
{
    if (zigBee_IsTouchlinkSessionActive())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TL scan Error, TouchLink Session is currently active", 0);
        return;
    }
    if(Application_ZigBeeIsCommissioningInProgress() ||
       Application_ZigBeeGetCommissioningAttempt() ||
       Application_ZigBeeGetRejoinAttempt())
    {
        // stop commissioning and rejoining before start touchlink commissioning
        nwk.m_nlme.Reset(true);
        aps.m_apsme.Reset(true);
    }

    // generate new random PAN ID if current PAN is invalid
    if (aps.m_nwk.m_mac.m_pib.m_wPANID == 0xffff)
    {
        aps.m_nwk.m_mac.m_mlme.SetPANID(rand16());
        GP_LOG_PRINTF("  TL: set PanID = 0x%2x", 0, aps.m_nwk.m_mac.m_pib.m_wPANID);
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start TouchLink(Initiator) Commissioning...",0);
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
    Application_ForceZigbeeTouchLinkClientInstance(true);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
    zigBee_EnableTouchLink(true);
    // Start TouchLink(initiator) scanning
    appsEp_1.m_touchLinkClient.StartScanning(resetToFactoryNewRequest, forceSecondaryScan);
}

void Application_cbTLTriggeredRejoin(bool rejoinInProgress)
{
    tlTriggeredRejoin = rejoinInProgress;
}

#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
void Application_ForceZigbeeTouchLinkClientInstance(bool bIsClient)
{
    if(bIsClient)
    {
        CZigBeeClusterTouchlink::pInstance = &appsEp_1.m_touchLinkClient;
    }
    else
    {
        CZigBeeClusterTouchlink::pInstance = &appsEp_1.m_touchLink;
    }
}
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) && defined(GP_APP_DIVERSITY_TOUCHLINK)
/////////////////////////////////////////////////////////////////////////////
// callback Handler
/** @brief Confirm from the Zigbee stack for the Startup process
*
*   @param nStatus               APS status return
*   @param nFlags                Startup flags used
*/
void OnZigBeeStartupComplete(const unsigned char nStatus,
    const unsigned short nFlags)
{
    // Get current commissioning attempt setting
    UInt16 joinFastRetry = Application_ZigBeeGetJoinFastRetry();

    // Get current base rejoin backoff
    UInt16 rejoinBaseBackOff = Application_ZigBeeGetRejoinBaseBackoff();

    // Get current fast rejoin setting
    UInt16 rejoinFastRetry = Application_ZigBeeGetRejoinFastRetry();

#if defined(GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM)
    // Check if stack rejoin algorithm is disabled, use application rejoin instead.
    if(!rejoinFastRetry)
    {
        rejoinFastRetry = APP_REJOIN_ATTEMPTS;
    }
#endif //GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM

    // Include the application triggered rejoin into rejoin limit
    if(ZigBeeNwkControl.appTriggeredRejoin)
    {
        rejoinFastRetry += 1;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Start-up complete (%u, %04hx)", 0, nStatus, nFlags);

    // ZB 3.0 Rejoining handle for network scan cycle completed
    if(nFlags & CZigBeeApplicationSupportManagementBase::startupAllowRejoining)
    {
        if(nStatus == CZigBeeApplicationSupport::statusSuccess)
        {
            GP_LOG_SYSTEM_PRINTF("  Network re-joined - Pan:0x%04x, Address: 0x%04x, Channel: %d", 0, aps.m_nwk.m_mac.m_pib.m_wPANID, aps.m_nwk.m_mac.m_pib.m_wShortAddress, aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel);

            //reset all application states
            ZigBeeNwkControl.joinAttempts = 0;
            ZigBeeNwkControl.rejoinAttempts = 0;
            ZigBeeNwkControl.appTriggeredRejoin = false;

#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
            // Disable TouchLink Stealing and Factory Reset if device is in centralized network for security concern
            if(Application_ZigBeeIsJoinedNetwork() && !Application_ZigBeeIsDistributedNetwork())
            {
                Application_ZigBeeEnableTouchLinkStealing(false);
                Application_ZigBeeEnableTouchLinkResetToFactoryNew(false);
            }
            // resume Touchlink(target) feature (enable to be stolen from other switch)
            Application_ZigBeeStartTouchLink(true);

#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
            if(nFlags & CZigBeeApplicationSupportManagementBase::startupAllowResume)
            {
                // send device anouncement after network resuming succeeded
                zdo.AnnounceDevice();
            }
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
            if(tlTriggeredRejoin)
            {
                appsEp_1.m_touchLinkClient.bindEndpoints();
            }
            appsEp_1.m_touchLinkClient.setTargetStatus(CZigBeeApplicationSupport::statusSuccess);
            goto ZigBeeStartUpComplete;
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        }
        else
        {
            // update rejoin count
            ZigBeeNwkControl.rejoinAttempts++;

            if(ZigBeeNwkControl.rejoinAttempts < rejoinFastRetry)
            {
                GP_LOG_SYSTEM_PRINTF("  Rejoin failed, attempt(%d/%d)",0
                    ,ZigBeeNwkControl.rejoinAttempts, rejoinFastRetry);

#if defined(GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM)
                // Application rejoin logic, used only if stack rejoin algorithm is disabled
                if(!Application_ZigBeeGetRejoinFastRetry())
                {
                    if(ZigBeeNwkControl.rejoinAttempts % 2)
                    {
                        // Schedule for next attempt
                        gpSched_ScheduleEvent(APP_REJOIN_RETRY_DELAY, Application_ZigBeeStartTcRejoin);
                    }
                    else
                    {
                        // Schedule for next attempt
                        gpSched_ScheduleEvent(APP_REJOIN_RETRY_DELAY, Application_ZigBeeStartSecureRejoin);
                    }
                }
#endif //GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM
            }
            else
            {
                if (!rejoinBaseBackOff)
                {
                    GP_LOG_SYSTEM_PRINTF("  Rejoin Attempt limit:(%d/%d) - sleeping", 0, ZigBeeNwkControl.rejoinAttempts, rejoinFastRetry);
                }
                else
                {
                    GP_LOG_SYSTEM_PRINTF("  Rejoin attempt limit:(%d/%d), Schedule for next rejoin cycle... ", 0, ZigBeeNwkControl.rejoinAttempts, rejoinFastRetry);

#if defined(GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM)
                    if (!Application_ZigBeeGetRejoinFastRetry())
                    {
                        // Schedule for next attempt
                        gpSched_ScheduleEvent(APP_REJOIN_BASE_BACKOFF_DELAY, Application_ZigBeeStartSecureRejoin);
                    }
#endif //GP_APP_DIVERSITY_USE_APPLICATION_REJOIN_ALGORITHM
                }

                //reset all application states
                ZigBeeNwkControl.joinAttempts = 0;
                ZigBeeNwkControl.rejoinAttempts = 0;
                ZigBeeNwkControl.appTriggeredRejoin = false;
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
                Application_cbTLTriggeredRejoin(false);
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
            }
        }
    }
    // ZB 3.0 Association handle for network scan cycle completed
    else
    {
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        Application_cbTLTriggeredRejoin(false);
#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
        if(nStatus == CZigBeeApplicationSupport::statusSuccess)
        {
            //reset all application states
            ZigBeeNwkControl.joinAttempts = 0;
            ZigBeeNwkControl.rejoinAttempts = 0;
            ZigBeeNwkControl.appTriggeredRejoin = false;

            if (nFlags & CZigBeeApplicationSupportManagement::startupNetworkFormation)
            {
                GP_LOG_SYSTEM_PRINTF("  Network formed - Pan:0x%04x, Address: 0x%04x, Channel: %d", 0, aps.m_nwk.m_mac.m_pib.m_wPANID, aps.m_nwk.m_mac.m_pib.m_wShortAddress, aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("  Network joined - Pan:0x%04x, Address: 0x%04x, Channel: %d", 0, aps.m_nwk.m_mac.m_pib.m_wPANID, aps.m_nwk.m_mac.m_pib.m_wShortAddress, aps.m_nwk.m_mac.m_phy.m_pib.m_nCurrentChannel);
            }
#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
            // Disable TouchLink Stealing and Factory Reset if device is in centralized network for security concern
            if(Application_ZigBeeIsJoinedNetwork() && !Application_ZigBeeIsDistributedNetwork())
            {
                Application_ZigBeeEnableTouchLinkStealing(false);
                Application_ZigBeeEnableTouchLinkResetToFactoryNew(false);
            }

            // resume Touchlink(target) feature (enable to be stolen from other switch)
            Application_ZigBeeStartTouchLink(true);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
        }
        else
        {
            // Check Stop Commissioning flag
            if(ZigBeeNwkControl.StopCommissioningEnabled)
            {
                zigBee_executeStopCommissioning();
                goto ZigBeeStartUpComplete;
            }

            // increase the number of attempt count
            ZigBeeNwkControl.joinAttempts++;

            // Check case: Commissioning attempt in progress
            if(Application_ZigBeeGetCommissioningAttempt() < joinFastRetry + 1)
            {
                GP_LOG_SYSTEM_PRINTF("  Commissioning failed, attempt(%d/%d) ", 0, ZigBeeNwkControl.joinAttempts, joinFastRetry + 1);
            }
            // Check case: Commissioning attempt limit reach
            else
            {
                GP_LOG_SYSTEM_PRINTF("  Commissioning failed, reach limit(%d/%d)", 0, ZigBeeNwkControl.joinAttempts, joinFastRetry + 1);

#ifdef GP_APP_DIVERSITY_DISTRIBUTED_NETWORK
                //start distributed network
                Application_ZigBeeStartDistributedNetwork();
#else
#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
                // resume Touchlink(target) feature (enable to be stolen from other switch)
                Application_ZigBeeStartTouchLink(true);

#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
#endif // GP_APP_DIVERSITY_DISTRIBUTED_NETWORK

                //reset all application states
                ZigBeeNwkControl.joinAttempts = 0;
                ZigBeeNwkControl.rejoinAttempts = 0;
                ZigBeeNwkControl.appTriggeredRejoin = false;
            }
        }
    }

ZigBeeStartUpComplete:
    // callback to application
    Application_cbZigBeeStartupComplete(nStatus, nFlags);
}

void OnZigBeeStartupProgress(const uint_fast8_t state,
    const uint_fast16_t flags, const uint_fast8_t status)
{
    if (state == CZigBeeApplicationSupportManagementBase::startupInstigated)
    {
        GP_LOG_PRINTF("ZIGBEE: Start-up Progress(%u, %04hx) - Instigated", 0, status, flags);

        if (flags & CZigBeeApplicationSupportManagementBase::startupAllowAssociation)
        {
            // callback to application that ZB 3.0 commissioning is started
            Application_cbZigBeeCommissioningStarted();
        }
    }
    else if (state == CZigBeeApplicationSupportManagementBase::startupComplete)
    {
        GP_LOG_PRINTF("ZIGBEE: Start-up Progress(%u, %04hx) - Complete", 0, status, flags);
    }
    else if (state == CZigBeeApplicationSupportManagementBase::startupAttemptInstigated)
    {
        GP_LOG_PRINTF("ZIGBEE: Start-up Progress(%u, %04hx) - Attempt Instigated", 0, status, flags);
    }
    else if (state == CZigBeeApplicationSupportManagementBase::startupBackoff)
    {
        GP_LOG_PRINTF("ZIGBEE: Start-up Progress(%u, %04hx) - Backoff", 0, status, flags);
    }
    else if (state == CZigBeeApplicationSupportManagementBase::startupAbort)
    {
        GP_LOG_PRINTF("ZIGBEE: Start-up Progress(%u, %04hx) - Abort", 0, status, flags);
    }
}

/** @brief Indication from the Zigbee stack the device was told to leave the network
*/
void OnZigBeeNetworkLeave(const uint8_t nStatus, const bool bRejoin,
    uint8_t &actionFlags)
{
    if(bRejoin == false)
    {
#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
        // resume Touchlink(target) feature (to give device a random new panId and back to ZLL channel)
        Application_ZigBeeStartTouchLink(true);

        // check whether Touchlink(target) session is active(TL stealing)
        if(zigBee_IsTouchlinkSessionActive())
        {
            // Calling EnableReceiver() will change the PanID,
            // here we directly enable receiver Rx in phy layer.
            aps.m_nwk.m_mac.m_phy.OnRequestSetTransceiverState(CIEEE802154PHY::transceiverReceiverEnabled);
        }
        else
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
        {
            if (!Application_ZigBeeIsFactoryResetInProgress())
            {
                // perform full factory reset/reboot after network leave if device is not factory reset in progress
                Application_FactoryReset();
            }
        }
    }
    else
    {
        // enable appTriggeredRejoin flag, rejoin limit will include the application triggered rejoin
        ZigBeeNwkControl.appTriggeredRejoin = true;
    }

    // callback to application
    Application_cbZigBeeLeave(nStatus, bRejoin);
}

/** @brief Indication from the Zigbee stack that the associationPermit flag has changed
*
*   @param bAssociationPermit    current permission state
*/
void OnZigBeeAssociationPermitModified(const bool bAssociationPermit)
{
    GP_LOG_SYSTEM_PRINTF("ZigBee assoc permit %u",0, bAssociationPermit);

    Application_cbAssociationPermitModified(bAssociationPermit);
}

/////////////////////////////////////////////////////////////////////////////
// Permit joining API
/** @brief get permit joining duration
*/
UInt8 Application_ZigBeeGetPermitJoiningDuration(void)
{
    return aps.m_nwk.m_nlme.GetPermitDuration();
}

/** @brief Enable permit joining
*/
Bool Application_ZigBeePermitJoining(UInt8 duration)
{
    Bool success = false;

    if(Application_ZigBeeIsJoinedNetwork())
    {
        // Enable permit join, if the network is distributed trust center.
        if((Application_ZigBeeIsDistributedNetwork() && Application_ZigBeeIsRouter()) ||
           Application_ZigBeeIsTrustCenter())
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: set Permit join for %u s", 0,
                                 duration);

            zdo.RequestPermitJoining(CZigBeeAddress(CZigBeeAddress::wBroadcastRoutersAndCoordinator),
                                     duration, true);

            success = true;
        }
        else
        {
            if(Application_ZigBeeIsDistributedNetwork())
            {
                GP_LOG_SYSTEM_PRINTF("ZIGBEE: set Permit Join - Device is NOT Router", 0);
                success = false;
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("ZIGBEE: set Permit Join - Device is NOT Trust Center", 0);
                success = false;
            }
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: set Permit Join - Device is NOT in network", 0);
    }

    return success;
}
