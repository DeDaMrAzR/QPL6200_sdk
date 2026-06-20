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

#ifndef _QORVO_CLUSTER_COLORCONTROL_CLIENT_H_
#define _QORVO_CLUSTER_COLORCONTROL_CLIENT_H_

#include "global.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControlClient

class CQorvoClusterColorControlClient : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of Color Control cluster
        enum { id = 0x0300 };

        // Cluster revision of this implementation
        enum { revision = 3 };
    
        // Cluster command identifiers
        enum { 
            commandMoveToHue = 0x00,                           /* Optional */
            commandMoveHue = 0x01,                             /* Optional */
            commandStepHue = 0x02,                             /* Optional */
            commandMoveToSaturation = 0x03,                    /* Optional */
            commandMoveSaturation = 0x04,                      /* Optional */
            commandStepSaturation = 0x05,                      /* Optional */
            commandMoveToHueAndSaturation = 0x06,              /* Optional */
            commandMoveToColor = 0x07,                         /* Mandatory */
            commandMoveColor = 0x08,                           /* Mandatory */
            commandStepColor = 0x09,                           /* Mandatory */
            commandMoveToColorTemperature = 0x0A,              /* Optional */
            commandEnhancedMoveToHue = 0x40,                   /* Optional */
            commandEnhancedMoveHue = 0x41,                     /* Optional */
            commandEnhancedStepHue = 0x42,                     /* Optional */
            commandEnhancedMoveToHueAndSaturation = 0x43,      /* Optional */
            commandColorLoopSet = 0x44,                        /* Optional */
            commandStopMoveStep = 0x47,                        /* Optional */
            commandMoveColorTemperature = 0x4B,                /* Optional */
            commandStepColorTemperatue = 0x4C,                 /* Optional */
        };

        // Identifiers of the Color Control cluster
        enum { 
            attributeCurrentHue = 0x0000,                      /* Optional */
            attributeCurrentSaturation = 0x0001,               /* Optional */
            attributeRemainingTime = 0x0002,                   /* Optional */
            attributeCurrentX = 0x0003,                        /* Mandatory */
            attributeCurrentY = 0x0004,                        /* Mandatory */
            attributeDriftCompensation = 0x0005,               /* Optional */
            attributeCompensationText = 0x0006,                /* Optional */
            attributeColorTemp = 0x0007,                       /* Optional */
            attributeColorMode = 0x0008,                       /* Optional */
            attributeOptions = 0x000F,                         /* Mandatory */
            attributeNumberOfPrimaries = 0x0010,               /* Optional */
            attributePrimary1X = 0x0011,                       /* Optional */
            attributePrimary1Y = 0x0012,                       /* Optional */
            attributePrimary1Intensity = 0x0013,               /* Optional */
            attributePrimary2X = 0x0015,                       /* Optional */
            attributePrimary2Y = 0x0016,                       /* Optional */
            attributePrimary2Intensity = 0x0017,               /* Optional */
            attributePrimary3X = 0x0019,                       /* Optional */
            attributePrimary3Y = 0x001A,                       /* Optional */
            attributePrimary3Intensity = 0x001B,               /* Optional */
            attributePrimary4X = 0x0020,                       /* Optional */
            attributePrimary4Y = 0x0021,                       /* Optional */
            attributePrimary4Intensity = 0x0022,               /* Optional */
            attributePrimary5X = 0x0024,                       /* Optional */
            attributePrimary5Y = 0x0025,                       /* Optional */
            attributePrimary5Intensity = 0x0026,               /* Optional */
            attributePrimary6X = 0x0028,                       /* Optional */
            attributePrimary6Y = 0x0029,                       /* Optional */
            attributePrimary6Intensity = 0x002A,               /* Optional */
            attributeWhitePointX = 0x0030,                     /* Optional */
            attributeWhitePointY = 0x0031,                     /* Optional */
            attributeColorPointRX = 0x0032,                    /* Optional */
            attributeColorPointRY = 0x0033,                    /* Optional */
            attributeColorPointRIntensity = 0x0034,            /* Optional */
            attributeColorPointGX = 0x0036,                    /* Optional */
            attributeColorPointGY = 0x0037,                    /* Optional */
            attributeColorPointGIntensity = 0x0038,            /* Optional */
            attributeColorPointBX = 0x003A,                    /* Optional */
            attributeColorPointBY = 0x003B,                    /* Optional */
            attributeColorPointBIntensity = 0x003C,            /* Optional */
            attributeEnhancedCurrentHue = 0x4000,              /* Optional */
            attributeEnhancedColorMode = 0x4001,               /* Optional */
            attributeColorLoopActive = 0x4002,                 /* Optional */
            attributeColorLoopDirection = 0x4003,              /* Optional */
            attributeColorLoopTime = 0x4004,                   /* Optional */
            attributeColorLoopStartEnhancedHue = 0x4005,       /* Optional */
            attributeColorLoopStoredEnhancedHue = 0x4006,      /* Optional */
            attributeColorCapabilities = 0x400A,               /* Mandatory */
            attributeColorTempPhysicalMin = 0x400B,            /* Optional */
            attributeColorTempPhysicalMax = 0x400C,            /* Optional */
            attributeColorTempLevelMinMireds = 0x400D,         /* Optional */
            attributeStartUpColorTempMireds = 0x400E,          /* Optional */
        };

        // ColorMode enum
        enum {
            modeHueSaturation = 0x0,
            modeXY = 0x1,
            modeColorTemperatureMireds = 0x2,
            modeEnhancedHueSaturation = 0x3, //EnhancedColorMode only
        };

        // ColorCapabilities bitmask
        enum {
            capabilityHueSaturationSupported = 0x0001,
            capabilityEnhancedHueSaturationSupported = 0x0002,
            capabilityColorLoopSupported = 0x0004,
            capabilityXYSupported = 0x0008,
            capabilityColorTemperatureSupported = 0x0010
        };

    // Construction
    public:
        CQorvoClusterColorControlClient
            (CZigBeeFoundationApplication &application);

    // Client to server command handling
    public:
        void MoveToColor(const CZigBeeAddress &destination,
            unsigned short colorX,
            unsigned short colorY,
            unsigned short transitionTime);
        void MoveColor(const CZigBeeAddress &destination,
            unsigned short colorX,
            unsigned short colorY);
        void StepColor(const CZigBeeAddress &destination,
            signed short stepX,
            signed short stepY);
        void MoveToHueSaturation(const CZigBeeAddress &destination,
            unsigned char hue,
            unsigned char saturation,
            unsigned short transitionTime);
        void MoveHue(const CZigBeeAddress &destination,
            unsigned char mode,
            unsigned char rate);
        void ColorLoopSet(const CZigBeeAddress &destination,
            unsigned char updateFlags,
            const unsigned char action = 0x00,
            const unsigned char direction = 0x00,
            const unsigned short time = 0x0000,
            const unsigned short startHue = 0x0000);
        void MoveToSaturation(const CZigBeeAddress &destination,
            unsigned char saturation,
            unsigned short transitionTime);
        void StepHue(const CZigBeeAddress &destination,
            unsigned char mode,
            unsigned char size,
            unsigned short transitionTime);

    // Implementation
    public:
        // Revision of the Color Control client cluster
        static const unsigned short nClusterRevision;
};

#endif // _ZIGBEE_CLUSTER_COLORCONTROL_CLIENT_H_
