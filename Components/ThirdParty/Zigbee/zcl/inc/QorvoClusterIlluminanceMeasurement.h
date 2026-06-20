/*
 * Copyright (c) 2017, Qorvo Inc
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

#ifndef _QORVO_CLUSTER_ILLUMINANCEMEASUREMENT_H_
#define _QORVO_CLUSTER_ILLUMINANCEMEASUREMENT_H_
/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterIlluminanceMeasurement

class CQorvoClusterIlluminanceMeasurement : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()

    // typedefs & enums
    public:
    // Cluster identifier of temperature measurement cluster
        enum { id = 0x0400 };

        // Cluster revision of this implementation
        enum { revision = 3 };

        // Identifiers of the illuminance measurement cluster
        enum { attributeMeasuredValue, attributeMinMeasurableValue,
            attributeMaxMeasurableValue, attributeTolerance, attributeLightSensorType };

        // Min/Max Measurable Value
        enum {
            MeasurableValueUndefined = 0xFFFF
        };

        // Light Sensor Type
        enum {
            LightSensorTypePhotodiode, LightSensorTypeCMOS, LightSensorTypeUnknown = 0xFF
        };
    // Construction
    public:
        CQorvoClusterIlluminanceMeasurement(CZigBeeFoundationApplication &application,
            const unsigned short nMinMeasurableValue = MeasurableValueUndefined,
            const unsigned short nMaxMeasurableValue = MeasurableValueUndefined,
            const unsigned short nTolerance = 0,
            const unsigned char nLightSensrType = LightSensorTypeUnknown
            );           

    // Attributes
    public:
        // Represents the Illuminance in Lux(lx). Mandatory. The Illuminance should be within 
        // the MinMeasurableValue and MaxMeasurableValue
        unsigned short m_nMeasuredValue;

        // Range of Illuminance measurements supported. Notice
        // this should not change. Rather it is the full range the device is
        // capable of measuring
        std::pair<unsigned short, unsigned short> m_measuredValueRange;

        
        // The Tolerance attribute SHALL indicate the magnitude of the possible error that is 
        // associated with MeasuredValue, using the same units and resolution
        unsigned short m_nTolerance;
        
        // The LightSensorType attribute specifies the electronic type of the light sensor
        unsigned char m_nLightSensorType;

    // Overrides
    public:
        // Called after power-up. Evaluates the Fan configuration settings
        // to set current attributes and stored attributes appropriately
        virtual void AfterPowerup();
        
        // Sets the value of the Illuminance into measuredValue attribute and notifies
        // the application
        void SetMeasuredValue(const unsigned short nMeasuredValue);
        

    // Implementation
    public:
        // Revision of the IlluminanceMeasurement cluster
        static const unsigned short nClusterRevision;
};

#define __COMPACT7B_ILLUMINANCEMEASUREMENT_MEASUREDVALUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterIlluminanceMeasurement, \
        CQorvoClusterIlluminanceMeasurement::attributeMeasuredValue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable | CZigBeeAttribute::persistent, \
        m_nMeasuredValue)

#define __COMPACT7B_ILLUMINANCEMEASUREMENT_MINMEASURABLEVALUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterIlluminanceMeasurement, \
        CQorvoClusterIlluminanceMeasurement::attributeMinMeasurableValue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_measuredValueRange.first)

#define __COMPACT7B_ILLUMINANCEMEASUREMENT_MAXMEASURABLEVALUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterIlluminanceMeasurement, \
        CQorvoClusterIlluminanceMeasurement::attributeMaxMeasurableValue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_measuredValueRange.second)

#define __COMPACT7B_ILLUMINANCEMEASUREMENT_TOLERANCE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterIlluminanceMeasurement, \
        CQorvoClusterIlluminanceMeasurement::attributeTolerance, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_nTolerance)
        
#define __COMPACT7B_ILLUMINANCEMEASUREMENT_LIGHTSENSORTYPE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterIlluminanceMeasurement, \
        CQorvoClusterIlluminanceMeasurement::attributeLightSensorType, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_nLightSensorType)

#endif  // _QORVO_CLUSTER_ILLUMINANCEMEASUREMENT_H_
