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

/** @file "OnOffSensorApplication.h"
 *
 * Class declaration of On/Off sensor endpoint.
 * All the members of the class are declared here
 * Provides a simple on/off control to operate with lights.
*/

#ifndef _ONOFFSENSORAPPLICATION_H_
#define _ONOFFSENSORAPPLICATION_H_

/////////////////////////////////////////////////////////////////////////////
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#include "ZigBeeTouchLink.h"
#endif //(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)

/** @brief Class COnOffSensorApplication definition */
class COnOffSensorApplication : public CZigBeeFoundationApplication
{
    __COMPACT7B_DECLARE_NOTIFICATION_MAP()
    __COMPACT7B_DECLARE_CLUSTER_MAP()
    __COMPACT7B_DECLARE_INCLUSTER_LIST()
    __COMPACT7B_DECLARE_OUTCLUSTER_LIST()

    // typedefs & enums
    public:
        /** @brief Enums for Home Automation profile */
        enum { profileID = 0x0104, version = 1};

        /** @brief Enums for Deivce ID - On/Off Sensor */
        enum { deviceID = 0x0850};

#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
        /** @brief Enums for legacy ZLL Deivce ID */
        enum {alternateDeviceID = CExTouchlinkDeviceId::altDeviceId_OnOffSensor};

        /** @brief Enums for legacy ZLL Deivce ID version */
        enum {alternateVersion = CExTouchlinkDeviceId::altDeviceId_Version};

#endif //#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT

    // Construction
    public:
        /** @brief COnOffSensorApplication constructor method */
        COnOffSensorApplication(CZigBeeApplicationSupport &aps,
            const unsigned char bEndpoint);

    // Attributes
    public:
        /** @brief Instance of Basic cluster */
        CZigBeeClusterBasicEx m_basic;

#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
        /** @brief Instance of Power Configuration cluster - Battery */
        CQorvoClusterPowerConfigurationBattery m_powerConfiguration;
#else
        /** @brief Instance of Power Configuration cluster */
        CQorvoClusterPowerConfiguration m_powerConfiguration;
#endif

        /** @brief Instance of Identify cluster */
        CZigBeeClusterIdentify m_identify;

        /** @brief Instance of Identify cluster client with management*/
        CZigBeeClusterManagementIdentify m_identifyClient;

        /** @brief Instance of Groups cluster client*/
        CQorvoClusterGroupsClient m_groupClient;

        /** @brief Instance of On/off cluster client */
        CZigBeeClusterOnOffClient m_onOffClient;

        /** @brief Instance of Poll Control cluster */
        CZigBeeClusterPollControl m_pollControl;

        /** @brief Instance of Diagnostics cluster */
        CZigBeeClusterDiagnostics m_diagnostics;

#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
        /** @brief Instance of Touch Link cluster */
        CQorvoClusterTouchLinkCommissioningClient m_touchLinkClient;

#endif //GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
        /** @brief Instance of touchlink cluster */
        CExTouchlinkApplication m_touchLink;

#endif // GP_APP_DIVERSITY_TOUCHLINK
#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
        /** @brief Instance of All Hubs cluster */
        CZigBeeClusterAllHubs m_wwahu;

#endif //UBI_DIVERSITY_ZCL_WWAH_SERVER
#ifdef UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT
        /** @brief Instance of OTA cluster */
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
        CQorvoClusterOTAUpgradeXp4001 m_otaUpgrade;
#endif
#endif // UBI_DIVERSITY_ZCL_OTA_UPGRADE_CLIENT

    // Operations
    public:
#ifdef GP_APP_DIVERSITY_BATTERY_SUPPORTED
        /** @brief Function to get battery voltage */
        unsigned char GetBatteryVoltage(void);

        /** @brief Function to set battery voltage locally */
        void SetBatteryVoltage(unsigned char voltage);

        /** @brief Function to get battery percentage remaining locally */
        unsigned char GetBatteryPercentageRemaining(void);

        /** @brief Function to set battery percentage remaining locally */
        void SetBatteryPercentageRemaining(unsigned char percentageRemaining);

#endif //GP_APP_DIVERSITY_BATTERY_SUPPORTED
        /** @brief Function to get identify mode locally */
        unsigned short GetIdentifyMode(void);

        /** @brief Function to set identify mode locally */
        void SetIdentifyMode(unsigned short duration);

        /** @brief Function to start finding and binding - start identify on target */
        void FindingAndBinding(void);

        /** @brief Function to start finding and binding - start identify on initiator */
        void FindingAndBindingInitiator(void);

        /** @brief Function to send add group command*/
        void SendGroupAddCommand(const CZigBeeAddress& destination, unsigned short groupId);

        /** @brief Function to send On command */
        void SendOnCommand(const CZigBeeAddress &destination,
            const unsigned char transmitOptions = CZigBeeApplicationSupportBase::transmitAcknowledged);

        /** @brief Function to send Off command */
        void SendOffCommand(const CZigBeeAddress &destination,
            const unsigned char transmitOptions = CZigBeeApplicationSupportBase::transmitAcknowledged);

        /** @brief Function to send OnWithTimedOff command */
        void SendOnWithTimedOffCommand(
            const CZigBeeAddress& destination, unsigned short onTime,
            const unsigned char transmitOptions = CZigBeeApplicationSupportBase::transmitAcknowledged);

#ifdef GP_APP_DIVERSITY_TOUCHLINK_CLIENT
        /** @brief Function to read touchlink Scanned Target information*/
        const CQorvoTouchLinkClientTargetInfo getTouchLinkScannedTargetInfo(void);
        /** @brief Function to read touchlink potential Target information*/
        const CQorvoTouchLinkScanResponseData getTouchLinkPotentialTargetInfo(void);
        /** @brief Function to read touchlink Target information*/
        const CQorvoTouchLinkClientTargetInfo getTouchLinkTargetInfo(void);

#endif // GP_APP_DIVERSITY_TOUCHLINK_CLIENT
#ifdef GP_APP_DIVERSITY_TOUCHLINK
        /** @brief Function to start touchlink - allow scans on target */
        void EnableTouchLink(const bool enable);

        /** @brief Function to check if touchlink session is active */
        bool IsTouchlinkSessionActive(void);

        /** @brief Function get touchlink session id */
        UInt32 GetTouchlinkSessionId(void);

        /** @brief Enable receiver for TL sessions */
        void EnableReceiver(void);

#endif // GP_APP_DIVERSITY_TOUCHLINK
        /** @brief Function to restore basic cluster's attribute to default */
        void RestoreBasicAttributeDefaults(void);

        /** @brief Function to restore identify cluster's attribute to default */
        void RestoreIdentifyAttributeDefaults(void);

#ifdef UBI_DIVERSITY_ZCL_WWAH_SERVER
        /** @brief Function to send WWAHu cluster's Power On/Off notification via bindings */
        void SendWwahuPoweringNotification(bool nOnOff, uint_fast8_t nReason);

#endif // UBI_DIVERSITY_ZCL_WWAH_SERVER
    // Overrides
    public:
        /** @brief Function to restore to default settings.
         *  Triggered by Basic Reset To Factory Defaults command
         *  @param bRemoveAllReferences Remove persistent references
         */
        virtual void RestoreDefaults(const bool bRemoveAllReferences = true);

        /** @brief Function to handle a ZDO bind or unbind request for the application */
        virtual unsigned char ProcessBindOrUnbindRequest
            (const CZigBeeBindingSource &source,
            const CZigBeeBindingTarget &target, const bool bBind,
            const CZigBeeAddress &originator);

        /** @brief Overrides function for APSDE-DATA.indication
         *  Details is in clause 2.2.4.1.3
         */
        void OnIndicateData(const CZigBeeAddress &destination,
            const CZigBeeAddress &source, const unsigned short wProfileID,
            const unsigned short wClusterID, CPacket &asdu,
            const unsigned char bStatus, const bool bSecurityUsed,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        /** @brief Overrides function for APSDE-DATA.confirm
         *  Details is in clause 2.2.4.1.2
         */
        void OnConfirmData(const CZigBeeAddress &destination,
            const unsigned char bSourceEndpoint, const unsigned char bStatus,
            const unsigned int nTimestamp, void *const pCookie);

    // Implementation
    public:
        /** @brief This function is called internally when the identify time has changed */
        void OnUpdateIdentifyTime(const unsigned char nOptions);
};

#endif // _ONOFFSENSORAPPLICATION_H_
