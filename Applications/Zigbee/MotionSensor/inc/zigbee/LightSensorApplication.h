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

/** @file "LightSensorApplication.h"
 *
 * Class declaration of Light Sensor endpoint.
 * All the members of the class are declared here
 * Provides a Light sensor application using the Illuminance Measurement cluster.
 */

#ifndef _LIGHTSENSORAPPLICATION_H_
#define _LIGHTSENSORAPPLICATION_H_

/** @brief Class CLightSensorApplication definition */
class CLightSensorApplication : public CZigBeeFoundationApplication
{
    __COMPACT7B_DECLARE_NOTIFICATION_MAP()
    __COMPACT7B_DECLARE_CLUSTER_MAP()
    __COMPACT7B_DECLARE_INCLUSTER_LIST()
    __COMPACT7B_DECLARE_OUTCLUSTER_LIST()

    // typedefs & enums
    public:
        /** @brief Enums for Home Automation profile */
        enum { profileID = 0x0104, version = 1};

        /** @brief Enums for Deivce ID - Light Sensor */
        enum { deviceID = 0x0106};

    // Construction
    public:
        /** @brief CLightSensorApplication constructor method */
        CLightSensorApplication(CZigBeeApplicationSupport &aps,
            const unsigned char bEndpoint);

    // Attributes
    public:
        /** @brief Instance of Basic cluster */
        CZigBeeClusterBasicEx m_basic;

        /** @brief Instance of Identify cluster */
        CZigBeeClusterIdentify m_identify;

        /** @brief Instance of Identify cluster client with management*/
        CZigBeeClusterManagementIdentify m_identifyClient;

        /** @brief Instance of Illuminance Measurement cluster */
        CQorvoClusterIlluminanceMeasurement m_illuminanceMeasurement;

    // Operations
    public:
        /** @brief Function to get identify mode locally */
        unsigned short GetIdentifyMode(void);

        /** @brief Function to set identify mode locally */
        void SetIdentifyMode(unsigned short duration);

        /** @brief Function to start finding and binding - start identify on target */
        void FindingAndBinding(void);

        /** @brief Function to start finding and binding - start identify on initiator */
        void FindingAndBindingInitiator(void);

        /** @brief Function to set illuminance measured value attribute */
        void SetIlluminanceMeasuredValue(unsigned short nMeasuredValue);

        /** @brief Function to set illuminance measured value attribute */
        void ReportIlluminanceMeasuredValue(void);

        /** @brief Function to restore basic cluster's attribute to default */
        void RestoreBasicAttributeDefaults(void);

        /** @brief Function to restore identify cluster's attribute to default */
        void RestoreIdentifyAttributeDefaults(void);

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

        /** @brief This function is called when the measured illuminance value has been updated. */
        void OnUpdateIlluminanceMeasurement(const unsigned char nOptions);

        /** @brief This function is called when the measured illuminance value is requested. */
        void OnRefreshIlluminanceMeasurement(const unsigned char nOptions);
};

#endif // _LIGHTSENSORAPPLICATION_H_
