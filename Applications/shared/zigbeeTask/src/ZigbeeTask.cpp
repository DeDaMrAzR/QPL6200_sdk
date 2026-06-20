/*
 * Copyright (c) 2024-2025, Qorvo Inc
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

/** @file "ZigbeeTask.cpp"
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "gpHal.h"
#include "gpReset.h"
#include "gpSched.h"
#include "gpLog.h"
#include "gpVersion.h"
#ifdef GP_DIVERSITY_BOOTLOADER
#include "gpUpgrade.h"
#endif // GP_DIVERSITY_BOOTLOADER

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#include "gpRandom.h"
#endif

#include "AppTask.h"
#include "AppManager.h"
#include "ZigbeeTask.h"
#include "ZigbeeEndpoint.h"
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
#include "AppBattery.h"
#endif // GP_APP_DIVERSITY_BATTERY_SUPPORTED

// P334 headers
#include "App.h"

/*****************************************************************************
 *                    Macro definitions
 *****************************************************************************/
/** @brief Macro defines for Basic's FirmwareBuildID */
#define _ZIGBEE_VERSION(_major, _minor, _revision, _patch)                                                             \
    {                                                                                                                  \
        0x7, _major + '0', '.', _minor + '0', '.', _revision + '0', '.', _patch + '0'                                  \
    }
#define ZIGBEE_VERSION(_args) _ZIGBEE_VERSION(_args)

/** @brief Macro Define for 1 second */
#define ONE_SECOND 1000000 // 1 s

/** @brief Macro defines for Binding/Grouping mode timeout */
#define APP_BINDING_GROUPING_MODE_TIMEOUT_US 5 * ONE_SECOND

/*****************************************************************************
 *                    NVM data definitions
 *****************************************************************************/
#ifdef GP_NVM_DIVERSITY_ELEMENT_IF
#define GP_APP_NVM_BASE_TAG_ID (UInt16)(GP_COMPONENT_ID << 8)

#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
#define NETWORKKEY_TAG_ID 0 // Used for Random Network Key
#endif

const gpNvm_IdentifiableTag_t ROM gpApplication_NvmElements[] FLASH_PROGMEM = {
    {GP_APP_NVM_BASE_TAG_ID + ZIGBEE_FACTORY_RESET_BUSY_TAG_ID, NULL, sizeof(UInt8), gpNvm_UpdateFrequencyLow,
     reinterpret_cast<gpNvm_cbDefaultValueInitializer_t>(&ZigbeeTaskMgr().NvmZigbeeFactoryResetBusy_DefaultInitializer),
     NULL},
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
    {GP_APP_NVM_BASE_TAG_ID + NETWORKKEY_TAG_ID, NULL, sizeof(UInt8) * 16, gpNvm_UpdateFrequencyLow, NULL, NULL},
#endif
};
#else
const gpNvm_Tag_t ROM gpApplication_NvmSection[] FLASH_PROGMEM = {
    {NULL, sizeof(UInt8), gpNvm_UpdateFrequencyLow, NULL}, // Factory Reset Zigbee Busy element
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
    {NULL, sizeof(UInt8) * 16, gpNvm_UpdateFrequencyLow, NULL},
#endif
};
#endif // GP_NVM_DIVERSITY_ELEMENT_IF

/*****************************************************************************
 *                    Default class overrides
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterBasic constant attribute data (device scope)

const unsigned char CZigBeeClusterBasic::bApplicationVersion = 0x01;
const unsigned char CZigBeeClusterBasic::bStackVersion = 0x01;

const unsigned char CZigBeeClusterBasicEx::bGenericDeviceClass = -1;
const unsigned char CZigBeeClusterBasicEx::bGenericDeviceType = -1;

extern const gpVersion_SoftwareInfo_t gpVersionSw;
const char CZigBeeClusterBasicEx::achFirmwareBuildID[] = ZIGBEE_VERSION(GP_VERSIONINFO_GLOBAL_VERSION);

bool bRemainAwake = false;

/*****************************************************************************
 *                    Application data
 *****************************************************************************/
ZigbeeTask ZigbeeTask::sZigbeeTask;

CPhysicalGreenPeak phy;
CIEEE802154MAC mac(phy);
CZigBeeNetwork nwk(mac);
CZigBeeApplicationSupport aps(nwk);
CZigBeeDeviceObject zdo(aps);

extern CZigBeeFoundationApplication* pApplication;

#ifdef GP_COMP_ZGPSTUB
// GreenPower Proxy
CZigBeeGPStub stub;
CZigBeeGPProxyApplication zgp(aps);
#endif // GP_COMP_ZGPSTUB

static ZigbeeTask::State_t lastState = ZigbeeTask::kZigbeeState_Invalid;

/*****************************************************************************
 *                    Application functions
 *****************************************************************************/
/** @brief Entry point of application.
 *   All Init before the scheduler continues needs to be handled here.
 *   Typically follow-up actions are scheduled using gpSched or triggers are initialized.
 */
void ZigbeeTask::Init(void)
{
    UInt8 resetStateZigbee;

    // -- Register Application NVM sections
#ifdef GP_NVM_DIVERSITY_ELEMENT_IF
    gpNvm_RegisterElementsProtected(gpApplication_NvmElements, number_of_elements(gpApplication_NvmElements));
#else
    gpNvm_RegisterSection(GP_COMPONENT_ID, gpApplication_NvmSection,
                          sizeof(gpApplication_NvmSection) / sizeof(gpNvm_Tag_t), NULL);
#endif // GP_NVM_DIVERSITY_ELEMENT_IF

    // Factory reset complete check
    // Check reset state first, if busy, resume factory reset
    gpNvm_RestoreProtected(GP_COMPONENT_ID, ZIGBEE_FACTORY_RESET_BUSY_TAG_ID, &resetStateZigbee);
    GP_LOG_PRINTF("ZIGBEE: reset state: 0x%x", 0, resetStateZigbee);
    if(resetStateZigbee != ZIGBEE_FACTORY_RESET_STATE_DONE)
    {
        GP_LOG_PRINTF("SYSTEM: pervious factory reset imcomplete, restart factory reset.", 0);
        FactoryReset();
        GetAppTask().ResetSystem();
    }

    mEvent.State = kZigbeeState_NotReady;

    // Initialize ZigBee stack
    ubiZPro_Init();

    // Get the endpoint application
    pApplication = static_cast<CZigBeeFoundationApplication*>(aps.GetApplication(ZIGBEE_ENDPOINT_1));

    // Update NWK capabilities for ZigBee
    // - Sleepy End Device, Battery Powered.
    Application_ZigBeeSetCapabilities(ZIGBEE_DEVTYPE_ENDDEVICE, false, false);

    // Printlog for ZigBee network status
    Application_ZigBeeNetworkInfo();

    // Start running ZigBee Stack
    Application_ZigBeeStart();

    // Post Startup Event - commissioned case handle
    if(Application_ZigBeeIsJoinedNetwork())
    {
        //Restore Network Key From NVM
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        gpNvm_RestoreProtected(GP_COMPONENT_ID, NETWORKKEY_TAG_ID,
                               CQorvoClusterTouchLinkCommissioningClient::abNetworkKey);
#endif
        // -- Dump install code from factory block
        Application_ZigBeeDumpInstallCode();

#if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
        // -- Set application control mode according to current network type
        if(Application_ZigBeeIsDistributedNetwork())
        {
            // Set to Group control mode
            SetControlMode(ControlMode_Group);
        }
        else
        {
            // Set to Binding control mode
            SetControlMode(ControlMode_Binding);
        }
#endif // #if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
    }
    else
    // Post Startup Event - NOT commissioned case handle
    {
        // Random and back up Network Key if device doesn't joined
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
        gpRandom_GetNewSequence(sizeof(CQorvoClusterTouchLinkCommissioningClient::abNetworkKey),
            CQorvoClusterTouchLinkCommissioningClient::abNetworkKey);
        gpNvm_Backup(GP_COMPONENT_ID, NETWORKKEY_TAG_ID, CQorvoClusterTouchLinkCommissioningClient::abNetworkKey);
#endif
        // -- Load install code from factory block
        Application_ZigBeeApplyInstallCode();
    }

#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
    // -- Start periodic check for Battery
    gpSched_ScheduleEvent(1 * ONE_SECOND, Application_StartPeriodicBatteryUpdate);

#endif // GP_APP_DIVERSITY_BATTERY_SUPPORTED

    gpSched_RegisterPreProcessOnIdleCallback(OnIdlePreProcessHandles);
    // Post Startup Event - General/Customer specific
#if defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
    // -- Enable TouchLink listening
    Application_ZigBeeStartTouchLink(true);
#endif // defined(GP_APP_DIVERSITY_TOUCHLINK) && !defined(GP_APP_DIVERSITY_TOUCHLINK_MANUAL_START)
}

void ZigbeeTask::OpenCommissioning(void)
{
    // commissioned case:
    if(Application_ZigBeeIsJoinedNetwork())
    {
        // enable Permit join if device is in Distributed network.
        // otherwise shows remaining permit join time
        UInt8 duration = Application_ZigBeeGetPermitJoiningDuration();

        if(Application_ZigBeeIsDistributedNetwork())
        {
            duration = duration ? 0 : CZigBeeApplicationSupportManagement::commissioningTime;
            Application_ZigBeePermitJoining(duration);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE: Permit join remaining duration: %u", 0, duration);
        }
    }
    // NOT commissioned case:
    else
    {
        // Abort commissioning if Device is starting commissioning
        if(Application_ZigBeeIsCommissioningInProgress() || Application_ZigBeeGetCommissioningAttempt())
        {
            // Abort commissioning
            Application_ZigBeeStopCommissioning();
        }
        // Start ZB 3.0 commissioning if Device is not in network.
        else
        {
            // start ZB 3.0 commissioning
            // Application_cbZigBeeCommissioningStarted() will be launched afterward
            Application_ZigBeeStartCommissioning();
        }
    }
}

void ZigbeeTask::FactoryReset(void)
{
    Application_ZigBeeFactoryReset();


}

void ZigbeeTask::SetState(ZigbeeTask::State_t state)
{
    mEvent.State = state;
}

ZigbeeTask::Event_t& ZigbeeTask::GetEvent(void)
{
    if(Application_ZigBeeIsJoinedNetwork())
    {
        mEvent.State = kZigbeeState_JoinedNetwork;
    }

    return mEvent;
}

/**
 *   Porting note:
 *   This is a sample code to define device is going to binding/grouping mode by checking sZigbeeTask.is_BindingGrouping
 */
/** @brief Example application API to trigger starting Binding/Grouping mode
 */
Bool ZigbeeTask::isBindingGrouping(void)
{
    return mEvent.is_BindingGrouping;
}

void ZigbeeTask::EnterBindingGroupingMode(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Binding/Grouping - Device is not in network", 0);
        return;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Binding/Grouping - Trigger button within %u sec", 0,
                         APP_BINDING_GROUPING_MODE_TIMEOUT_US / 1000000);

    mEvent.is_BindingGrouping = true;

    // Schedule timeout to leave binding mode
    gpSched_UnscheduleEventArg(ZigbeeTask::LeaveBindingGroupingModeWrapper, this);
    gpSched_ScheduleEventArg(APP_BINDING_GROUPING_MODE_TIMEOUT_US, ZigbeeTask::LeaveBindingGroupingModeWrapper, this);
}

void ZigbeeTask::LeaveBindingGroupingModeWrapper(void* obj)
{
    ((ZigbeeTask*)obj)->LeaveBindingGroupingMode();
}

/** @brief Example application API to trigger Stopping Binding/Grouping mode
 */
void ZigbeeTask::LeaveBindingGroupingMode(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Binding/Grouping - Device is not in network", 0);
        return;
    }

    mEvent.is_BindingGrouping = false;

    // Remove timeout handling
    gpSched_UnscheduleEventArg(ZigbeeTask::LeaveBindingGroupingModeWrapper, this);
}

/** @brief Example application API to toggle Binding/Grouping mode
 */
void ZigbeeTask::ToggleBindingGroupingMode(void)
{
    if(!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Binding/Grouping - Device is not in network", 0);
        return;
    }

    if(!mEvent.is_BindingGrouping)
    {
        EnterBindingGroupingMode();
    }
    else
    {
        LeaveBindingGroupingMode();
    }
}

/** @brief function get current control mode
 */
ZigbeeTask::FB_ControlMode_t ZigbeeTask::GetControlMode(void)
{
    return mEvent.controlMode;
}

/** @brief function set current control mode in application
 *
 *   @param mode              Control mode to set
 */
void ZigbeeTask::SetControlMode(FB_ControlMode_t mode)
{
    if(mode == ControlMode_Binding)
    {
        mEvent.controlMode = mode;
        GP_LOG_SYSTEM_PRINTF("APPS: set control mode - Binding", 0);
    }
    else if(mode == ControlMode_Group)
    {
        mEvent.controlMode = mode;
        GP_LOG_SYSTEM_PRINTF("APPS: set control mode - Group", 0);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("APPS, Error: unknown control mode", 0);
    }
}

/** @brief function set toggle control mode in application
 */
void ZigbeeTask::ToggleControlMode(void)
{
    FB_ControlMode_t mode = GetControlMode();

    if(mode == ControlMode_Binding)
    {
        SetControlMode(ControlMode_Group);
    }
    else
    {
        SetControlMode(ControlMode_Binding);
    }
}

void ZigbeeTask::OnIdlePreProcessHandles()
{
    Application_ZigBeeUpdateCurrentTime();
}

/*****************************************************************************
 *  Factory reset NVM corruption protection
 *****************************************************************************/
#ifdef GP_NVM_DIVERSITY_ELEMENT_IF
/** @brief Initialize nvm factory reset busy value to default
 */
Bool ZigbeeTask::NvmZigbeeFactoryResetBusy_DefaultInitializer(const ROM void* pTag, UInt8* pBuffer)
{
    gpNvm_IdentifiableTag_t tag;
    UInt8 value = ZIGBEE_FACTORY_RESET_STATE_DONE;

    MEMCPY_P((UInt8*)&tag, pTag, sizeof(gpNvm_IdentifiableTag_t));
    if(NULL == pBuffer)
    {
        pBuffer = tag.pRamLocation;
        if(NULL == pBuffer)
        {
            return false;
        }
    }

    MEMCPY_P(pBuffer, (UInt8*)&value, sizeof(UInt8));
    return true;
}
#endif // GP_NVM_DIVERSITY_ELEMENT_IF

/*****************************************************************************
 *  ZigBee commissioning callback
 *****************************************************************************/
/** @brief callback indicated that ZB 3.0 commissioning is started.
 *      Header: ZigBeeCommissioning_Callback.h
 */
void Application_cbZigBeeCommissioningStarted(void)
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Commissioning started", 0);
    ZigbeeTaskMgr().SetState(ZigbeeTask::kZigbeeState_Commissioning);
    GetAppMgr().UpdateLEDs();
}

/** @brief Confirm from the Zigbee stack to user application for the Startup process
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param nStatus               APS status return
 *   @param nFlags                Startup flags used
 */
void Application_cbZigBeeStartupComplete(UInt8 nStatus, UInt16 flags)
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: startup complete, status - %d", 0, nStatus);
    ZigbeeTaskMgr().SetState(ZigbeeTask::kZigbeeState_Ready);
    GetAppMgr().UpdateLEDs();

#if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH) || defined(GP_APPLICATION_MOTIONSENSOR)
    // Add parent into group for group control if Device is in distributed network
    if(nStatus == CZigBeeApplicationSupport::statusSuccess)
    {
        if(Application_ZigBeeIsDistributedNetwork())
        {
            // Set to Group control mode
            ZigbeeTaskMgr().SetControlMode((ZigbeeTask::FB_ControlMode_t)App_ControlMode_Group);

            // Add parent into group for group control
            CZigBeeAddress targetAddress = aps.m_nwk.m_nlme.GetParentAddressEx();
            targetAddress.m_wPANID = aps.m_nwk.m_mac.m_pib.m_wPANID;
            targetAddress.SetEndpoint(1);

            ZigbeeEndpointMgr().SendAddGroup(targetAddress, ZIGBEE_GROUP_ID);
        }
        else
        {
            if(flags & CZigBeeApplicationSupportManagementBase::startupAllowAssociation)
            {
                // Set to Binding control mode
                ZigbeeTaskMgr().SetControlMode((ZigbeeTask::FB_ControlMode_t)App_ControlMode_Binding);
            }
        }
    }
#endif // defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH) ||
       // defined(GP_APPLICATION_MOTIONSENSOR)
}

/** @brief Callback from Zigbee stack to user application when device leaves the network
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param rejoin                    True when rejoin is enabled. False when disabled.
 */
void Application_cbZigBeeLeave(const uint8_t nStatus, const bool bRejoin)
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: On NWK Leave indication. Status: 0x%x, Rejoin:%u", 0, nStatus, bRejoin);
    ZigbeeTaskMgr().SetState(ZigbeeTask::kZigbeeState_DeviceLeft);
    GetAppMgr().UpdateLEDs();
}

/** @brief Callback from Zigbee stack to user application when Permit join is modified
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param bAssociationPermit       True when permit join is enabled
 */
void Application_cbAssociationPermitModified(Bool bAssociationPermit)
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Association permit modified", 0);
    ZigbeeTaskMgr().SetState(ZigbeeTask::kZigbeeState_AssoPermitJoin);
    GetAppMgr().UpdateLEDs();
}

/** @brief callback when device starts identifying
 *
 *   @param endpoint                  The endpoint is identify
 */
void Application_cbIdentifyStart(UInt8 endpoint)
{
    lastState = ZigbeeTaskMgr().GetEvent().State;
    ZigbeeTaskMgr().SetState(ZigbeeTask::kZigbeeState_Identifying);
    Application_cbLedIndication_Identify(true, endpoint);
}

/** @brief callback when device stops identifying
 *   @param endpoint       The endpoint to identify
 */
void Application_cbIdentifyStop(UInt8 endpoint)
{
    ZigbeeTaskMgr().SetState(lastState);
    Application_cbLedIndication_Identify(false, endpoint);
}

/** @brief callback for LED indication for identifying.
 *
 *   @param isIdentifying        whether device is identifying
 *   @param endpoint             identifying endpoint
 */
void Application_cbLedIndication_Identify(Bool isIdentifying, UInt8 endpoint)
{
    AppEvent appIdentifyEvent = {};

    appIdentifyEvent.Type = AppEvent::kEventType_Identify;
    appIdentifyEvent.IdentifyEvent.isIdentifying = isIdentifying;

    GetAppTask().PostEvent(&appIdentifyEvent);
}

/*****************************************************************************
 *                    Override functions
 *****************************************************************************/

/** @brief function override when device performs a factory reset
 */
void CZigBeeApplicationSupportManagement::FactoryReset(const unsigned int nOptions)
{
    ASSERT(!(nOptions & factoryResetStage2));
    unsigned int option = nOptions;

    // avoid rebooting if device is not in network
    if(!aps.m_nwk.m_nlme.IsJoined())
    {
        option = (nOptions & (~factoryReboot));
    }

    _TRACE1(c7b::apsme | 1, "APSME: option (%u)\r\n", option);

    CZigBeeNetworkSecurityMaterial material(CZigBeeNetworkSecurityMaterial::typeNetworkKey);

    CZigBeeApplicationSupport& mAps = static_cast<CZigBeeApplicationSupport&>(m_aps);

    // Notify the application objects and clusters (stage 1)
    mAps.OnFactoryReset(option);

    // Persist the frame counter accross factory resets?
    if((option & factoryResetSecurityFrameCounters) || !mAps.m_nwk.m_nlme.LoadFrameCounter(material))
        material.m_nOutgoingFrameCounter = UINT32_MAX;

    // Full factory reset, i.e. revert to factory-programmed persistent values
    theStorage.RevertToFactoryFresh();
    // Store the frame counter. No need to store ahead, just keep the existing
    // value
    if(material.m_nOutgoingFrameCounter != UINT32_MAX)
    {
        mAps.m_nwk.m_nlme.StoreFrameCounter(material, 0);
        _TRACE1(c7b::apsme | 1, "APSME: outgoing frame counter (%u) preserved\r\n", material.m_nOutgoingFrameCounter);
    }

    // Notify the application objects and clusters (stage 2)
    mAps.OnFactoryReset(option | factoryResetStage2);

    // Reboot the device?
    if(option & factoryReboot)
    {
        CBoard::Reboot();
    }
}

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
/** @brief Application specific implementation of the power-on notification reason
 */
uint_fast8_t CZigBeeClusterAllHubs::GetPowerOnReason(CPacket& reason)
{
    UInt8 resetReason = gpReset_GetResetReason();
    UInt8 wwahResetReason = CZigBeeClusterAllHubs::reasonUnknown;

    GP_LOG_SYSTEM_PRINTF("ZIGBEE WWAH: get Power On resetReason = 0x%x", 0, resetReason);
    switch(resetReason)
    {
        case gpReset_ResetReason_HW_BrownOutDetected:
            wwahResetReason = CZigBeeClusterAllHubs::reasonBrownout;
            break;
        case gpReset_ResetReason_HW_Watchdog:
            wwahResetReason = CZigBeeClusterAllHubs::reasonWatchdog;
            break;
        case gpReset_ResetReason_HW_Por:
            wwahResetReason = CZigBeeClusterAllHubs::reasonHardwareReset;
            break;
        case gpReset_ResetReason_SW_Por:
            wwahResetReason = CZigBeeClusterAllHubs::reasonSoftwareReset;
            break;
    }

    return wwahResetReason;
}
#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
