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

#ifndef _QORVO_CLUSTER_POWERCONFIGURATIONMAINS_H_
#define _QORVO_CLUSTER_POWERCONFIGURATIONMAINS_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterPowerConfigurationMains

class CQorvoClusterPowerConfigurationMains : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of Power Configuration cluster
        enum { id = 0x0001 };

        // Cluster revision of this implementation
        enum { revision = 3 };

        // Identifiers of the Power Configuration cluster
        enum {
            attributeMainsVoltage = 0x0000,                    /* Optional */
            attributeMainsFrequency = 0x0001,                  /* Optional */
            attributeMainsAlarmMask = 0x0010,                  /* Optional */
            attributeMainsVoltageMinThreshold = 0x0011,        /* Optional */
            attributeMainsVoltageMaxThreshold = 0x0012,        /* Optional */
            attributeMainsVoltageDwellTripPoint = 0x0013,      /* Optional */
            attributeBatteryVoltage = 0x0020,                  /* Optional */
            attributeBatteryPercentageRemaining = 0x0021,      /* Optional */
            attributeBatteryManufacturer = 0x0030,             /* Optional */
            attributeBatterySize = 0x0031,                     /* Optional */
            attributeBatteryAhrRating = 0x0032,                /* Optional */
            attributeBatteryQuantity = 0x0033,                 /* Optional */
            attributeBatteryRatedVoltage = 0x0034,             /* Optional */
            attributeBatteryAlarmMask = 0x0035,                /* Optional */
            attributeBatteryVoltageMinThreshold = 0x0036,      /* Optional */
            attributeBatteryVoltageThreshold1 = 0x0037,        /* Optional */
            attributeBatteryVoltageThreshold2 = 0x0038,        /* Optional */
            attributeBatteryVoltageThreshold3 = 0x0039,        /* Optional */
            attributeBatteryPercentageMinThreshold = 0x003a,   /* Optional */
            attributeBatteryPercentageThreshold1 = 0x003b,     /* Optional */
            attributeBatteryPercentageThreshold2 = 0x003c,     /* Optional */
            attributeBatteryPercentageThreshold3 = 0x003d,     /* Optional */
            attributeBatteryAlarmState = 0x003e,               /* Optional */
            attributeBattery2Voltage = 0x0040,                 /* Optional */
            attributeBattery2PercentageRemaining = 0x0041,     /* Optional */
            attributeBattery2Manufacturer = 0x0050,            /* Optional */
            attributeBattery2Size = 0x0051,                    /* Optional */
            attributeBattery2AhrRating = 0x0052,               /* Optional */
            attributeBattery2Quantity = 0x0053,                /* Optional */
            attributeBattery2RatedVoltage = 0x0054,            /* Optional */
            attributeBattery2AlarmMask = 0x0055,               /* Optional */
            attributeBattery2VoltageMinThreshold = 0x0056,     /* Optional */
            attributeBattery2VoltageThreshold1 = 0x0057,       /* Optional */
            attributeBattery2VoltageThreshold2 = 0x0058,       /* Optional */
            attributeBattery2VoltageThreshold3 = 0x0059,       /* Optional */
            attributeBattery2PercentageMinThreshold = 0x005a,  /* Optional */
            attributeBattery2PercentageThreshold1 = 0x005b,    /* Optional */
            attributeBattery2PercentageThreshold2 = 0x005c,    /* Optional */
            attributeBattery2PercentageThreshold3 = 0x005d,    /* Optional */
            attributeBattery2AlarmState = 0x005e,              /* Optional */
            attributeBattery3Voltage = 0x0060,                 /* Optional */
            attributeBattery3PercentageRemaining = 0x0061,     /* Optional */
            attributeBattery3Manufacturer = 0x0070,            /* Optional */
            attributeBattery3Size = 0x0071,                    /* Optional */
            attributeBattery3AhrRating = 0x0072,               /* Optional */
            attributeBattery3Quantity = 0x0073,                /* Optional */
            attributeBattery3RatedVoltage = 0x0074,            /* Optional */
            attributeBattery3AlarmMask = 0x0075,               /* Optional */
            attributeBattery3VoltageMinThreshold = 0x0076,     /* Optional */
            attributeBattery3VoltageThreshold1 = 0x0077,       /* Optional */
            attributeBattery3VoltageThreshold2 = 0x0078,       /* Optional */
            attributeBattery3VoltageThreshold3 = 0x0079,       /* Optional */
            attributeBattery3PercentageMinThreshold = 0x007a,  /* Optional */
            attributeBattery3PercentageThreshold1 = 0x007b,    /* Optional */
            attributeBattery3PercentageThreshold2 = 0x007c,    /* Optional */
            attributeBattery3PercentageThreshold3 = 0x007d,    /* Optional */
            attributeBattery3AlarmState = 0x007e,              /* Optional */
        };

    // Construction
    public:
        CQorvoClusterPowerConfigurationMains
            (CZigBeeFoundationApplication &application);

    // Attributes
    public:
        unsigned short m_MainsVoltage;
     /* unsigned char m_MainsFrequency; */
        unsigned char m_MainsAlarmMask;
     /* unsigned short m_MainsVoltageMinThreshold; */
     /* unsigned short m_MainsVoltageMaxThreshold; */
     /* unsigned short m_MainsVoltageDwellTripPoint; */
     /* unsigned char m_BatteryVoltage; */
     /* unsigned char m_BatteryPercentageRemaining; */
     /* char* m_BatteryManufacturer; */
     /* unsigned char m_BatterySize; */
     /* unsigned short m_BatteryAhrRating; */
     /* unsigned char m_BatteryQuantity; */
     /* unsigned char m_BatteryRatedVoltage; */
     /* unsigned char m_BatteryAlarmMask; */
     /* unsigned char m_BatteryVoltageMinThreshold; */
     /* unsigned char m_BatteryVoltageThreshold1; */
     /* unsigned char m_BatteryVoltageThreshold2; */
     /* unsigned char m_BatteryVoltageThreshold3; */
     /* unsigned char m_BatteryPercentageMinThreshold; */
     /* unsigned char m_BatteryPercentageThreshold1; */
     /* unsigned char m_BatteryPercentageThreshold2; */
     /* unsigned char m_BatteryPercentageThreshold3; */
     /* unsigned int  m_BatteryAlarmState; */  // bitmap32
     /* unsigned char m_Battery2Voltage; */
     /* unsigned char m_Battery2PercentageRemaining; */
     /* char* m_Battery2Manufacturer; */
     /* unsigned char m_Battery2Size; */
     /* unsigned short m_Battery2AhrRating; */
     /* unsigned char m_Battery2Quantity; */
     /* unsigned char m_Battery2RatedVoltage; */
     /* unsigned char m_Battery2AlarmMask; */
     /* unsigned char m_Battery2VoltageMinThreshold; */
     /* unsigned char m_Battery2VoltageThreshold1; */
     /* unsigned char m_Battery2VoltageThreshold2; */
     /* unsigned char m_Battery2VoltageThreshold3; */
     /* unsigned char m_Battery2PercentageMinThreshold; */
     /* unsigned char m_Battery2PercentageThreshold1; */
     /* unsigned char m_Battery2PercentageThreshold2; */
     /* unsigned char m_Battery2PercentageThreshold3; */
     /* Bitmap32 m_Battery2AlarmState; */
     /* unsigned char m_Battery3Voltage; */
     /* unsigned char m_Battery3PercentageRemaining; */
     /* char* m_Battery3Manufacturer; */
     /* unsigned char m_Battery3Size; */
     /* unsigned short m_Battery3AhrRating; */
     /* unsigned char m_Battery3Quantity; */
     /* unsigned char m_Battery3RatedVoltage; */
     /* unsigned char m_Battery3AlarmMask; */
     /* unsigned char m_Battery3VoltageMinThreshold; */
     /* unsigned char m_Battery3VoltageThreshold1; */
     /* unsigned char m_Battery3VoltageThreshold2; */
     /* unsigned char m_Battery3VoltageThreshold3; */
     /* unsigned char m_Battery3PercentageMinThreshold; */
     /* unsigned char m_Battery3PercentageThreshold1; */
     /* unsigned char m_Battery3PercentageThreshold2; */
     /* unsigned char m_Battery3PercentageThreshold3; */
     /* Bitmap32 m_Battery3AlarmState; */

     // Implementation
    public:
        // Revision of the Power Configuration cluster
        static const unsigned short nClusterRevision;

};

#define __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsVoltage, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_MainsVoltage)
/*
#define __COMPACT7B_POWERCONFIGURATION_MAINSFREQUENCY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsFrequency, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_MainsFrequency)
*/
#define __COMPACT7B_POWERCONFIGURATION_MAINSALARMMASK() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsAlarmMask, \
        CZigBeeAttribute::typeBitmap8, 0, \
        0x0, \
        m_MainsAlarmMask)
/*
#define __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEMINTHRESHOLD() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsVoltageMinThreshold, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_MainsVoltageMinThreshold)
#define __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEMAXTHRESHOLD() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsVoltageMaxThreshold, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_MainsVoltageMaxThreshold)
#define __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEDWELLTRIPPOINT() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterPowerConfigurationMains, \
        CQorvoClusterPowerConfigurationMains::attributeMainsVoltageDwellTripPoint, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_MainsVoltageDwellTripPoint)
*/

#define __COMPACT7B_POWERCONFIGURATIONMAINS_CLUSTER_ATTRIBUTES() \
    __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGE()  \
 /* __COMPACT7B_POWERCONFIGURATION_MAINSFREQUENCY() */ \
    __COMPACT7B_POWERCONFIGURATION_MAINSALARMMASK()  \
 /* __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEMINTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEMAXTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_MAINSVOLTAGEDWELLTRIPPOINT() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYVOLTAGE()*/ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYPERCENTAGEREMAINING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYMANUFACTURER() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYSIZE() */  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYAHRRATING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYQUANTITY() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYRATEDVOLTAGE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYALARMMASK()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYVOLTAGEMINTHRESHOLD()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYVOLTAGETHRESHOLD1()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYVOLTAGETHRESHOLD2()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYVOLTAGETHRESHOLD3()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYPERCENTAGEMINTHRESHOLD()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYPERCENTAGETHRESHOLD1()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYPERCENTAGETHRESHOLD2()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYPERCENTAGETHRESHOLD3()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERYALARMSTATE()*/  \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2VOLTAGE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2PERCENTAGEREMAINING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2MANUFACTURER() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2SIZE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2AHRRATING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2QUANTITY() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2RATEDVOLTAGE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2ALARMMASK() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2VOLTAGEMINTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2VOLTAGETHRESHOLD1() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2VOLTAGETHRESHOLD2() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2VOLTAGETHRESHOLD3() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2PERCENTAGEMINTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2PERCENTAGETHRESHOLD1() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2PERCENTAGETHRESHOLD2() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2PERCENTAGETHRESHOLD3() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY2ALARMSTATE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3VOLTAGE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3PERCENTAGEREMAINING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3MANUFACTURER() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3SIZE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3AHRRATING() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3QUANTITY() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3RATEDVOLTAGE() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3ALARMMASK() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3VOLTAGEMINTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3VOLTAGETHRESHOLD1() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3VOLTAGETHRESHOLD2() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3VOLTAGETHRESHOLD3() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3PERCENTAGEMINTHRESHOLD() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3PERCENTAGETHRESHOLD1() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3PERCENTAGETHRESHOLD2() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3PERCENTAGETHRESHOLD3() */ \
 /* __COMPACT7B_POWERCONFIGURATION_BATTERY3ALARMSTATE() */ \
    /*End*/

#endif // _QORVO_CLUSTER_POWERCONFIGURATIONMAINS_H_
