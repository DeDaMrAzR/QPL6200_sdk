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

#ifndef _QORVO_CLUSTER_THERMOSTATUSERINTERFACECONFIGURATION_H_
#define _QORVO_CLUSTER_THERMOSTATUSERINTERFACECONFIGURATION_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterThermostatUserInterfaceConfiguration
class CQorvoClusterThermostatUserInterfaceConfiguration : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of Thermostat User Interface Configuration cluster
        enum { id = 0x0204 };

        // Identifiers of the Thermostat User Interface Configuration cluster
        enum { 
            attributeTemperaturedisplaymode = 0x0000,          /* Mandatory */
            attributeKeypadlockout = 0x0001,                   /* Mandatory */
            attributeScheduleprogrammingvisibility = 0x0002,   /* Optional */
        };

    // Construction
    public:
        CQorvoClusterThermostatUserInterfaceConfiguration
            (CZigBeeFoundationApplication &application);

    // Attributes
    public:
        unsigned char m_Temperaturedisplaymode;
        unsigned char m_Keypadlockout;
     /* unsigned char m_Scheduleprogrammingvisibility; */
};

#define __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_TEMPERATUREDISPLAYMODE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterThermostatUserInterfaceConfiguration, \
        CQorvoClusterThermostatUserInterfaceConfiguration::attributeTemperaturedisplaymode, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        0x0, \
        m_Temperaturedisplaymode)
#define __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_KEYPADLOCKOUT() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterThermostatUserInterfaceConfiguration, \
        CQorvoClusterThermostatUserInterfaceConfiguration::attributeKeypadlockout, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        0x0, \
        m_Keypadlockout)
#if 0
#define __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_SCHEDULEPROGRAMMINGVISIBILITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterThermostatUserInterfaceConfiguration, \
        CQorvoClusterThermostatUserInterfaceConfiguration::attributeScheduleprogrammingvisibility, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        0x0, \
        m_Scheduleprogrammingvisibility)
#endif

#define __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_CLUSTER_ATTRIBUTES() \
    __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_TEMPERATUREDISPLAYMODE() \
    __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_KEYPADLOCKOUT() \
 /* __COMPACT7B_THERMOSTATUSERINTERFACECONFIGURATION_SCHEDULEPROGRAMMINGVISIBILITY() */ \
    /*End*/

#endif // _ZIGBEE_CLUSTER_THERMOSTATUSERINTERFACECONFIGURATION_H_
