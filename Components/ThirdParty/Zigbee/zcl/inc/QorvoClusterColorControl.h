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

#ifndef _QORVO_CLUSTER_COLORCONTROL_H_
#define _QORVO_CLUSTER_COLORCONTROL_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControl

class CQorvoClusterColorControl : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // typedefs & enums
    public:
        // Cluster identifier of Color Control cluster
        enum { id = 0x0300 };

        // Cluster command identifiers
        enum { 
            commandMoveToHue = 0x00,                           /* Mandatory */
            commandMoveHue = 0x01,                             /* Mandatory */
            commandStepHue = 0x02,                             /* Mandatory */
            commandMoveToSaturation = 0x03,                    /* Mandatory */
            commandMoveSaturation = 0x04,                      /* Mandatory */
            commandStepSaturation = 0x05,                      /* Mandatory */
            commandMoveToHueAndSaturation = 0x06,              /* Mandatory */
            commandMoveToColor = 0x07,                         /* Mandatory */
            commandMoveColor = 0x08,                           /* Mandatory */
            commandStepColor = 0x09,                           /* Mandatory */
            commandMoveToColorTemperature = 0x0A,              /* Optional */
            commandEnhancedMoveToHue = 0x40,                   /* Mandatory */
            commandEnhancedMoveHue = 0x41,                     /* Mandatory */
            commandEnhancedStepHue = 0x42,                     /* Mandatory */
            commandEnhancedMoveToHueAndSaturation = 0x43,      /* Mandatory */
            commandColorLoopSet = 0x44,                        /* Optional */
            commandStopMoveStep = 0x47,                        /* Mandatory */
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
            attributeEnhancedCurrentHue = 0x4000,              /* Mandatory */
            attributeEnhancedColorMode = 0x4001,               /* Mandatory */
            attributeColorLoopActive = 0x4002,                 /* Optional */
            attributeColorLoopDirection = 0x4003,              /* Optional */
            attributeColorLoopTime = 0x4004,                   /* Optional */
            attributeColorLoopStartEnhancedHue = 0x4005,       /* Optional */
            attributeColorLoopStoredEnhancedHue = 0x4006,      /* Optional */
            attributeColorCapabilities = 0x400A,               /* Mandatory */
            attributeColorTempPhysicalMin = 0x400B,            /* Optional */
            attributeColorTempPhysicalMax = 0x400C,            /* Optional */
            attributeColorTempLevelMinMireds = 0x400D,         /* Optional */
            attributeStartUpColorTempMireds = 0x4010,          /* Optional */
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
            capabilityEnhancedHueSupported = 0x0002,
            capabilityColorLoopSupported = 0x0004,
            capabilityXYSupported = 0x0008,
            capabilityColorTemperatureSupported = 0x0010
        };
        
        // move to hue direction field
        enum {
            hueDirectionShortestDistance = 0x00,
            hueDirectionLongestDistance = 0x01,
            hueDirectionUp = 0x02,
            hueDirectionDown = 0x03,
        };
        
        // move mode
        enum {
            moveStop = 0x00,
            moveUp = 0x01,
            moveDown = 0x03,
        };
        
        // step mode
        enum {
            stepUp = 0x01,
            stepDown = 0x03
        };
        
        // color loop update flags bit number
        enum {
            colorLoopUpdateAction = 0x00,
            colorLoopUpdateDirection = 0x01,
            colorLoopUpdateTime = 0x02,
            colorLoopUpdateStartHue = 0x03
        };
        
        //color loop action field
        enum {
            deActivateColorLoop = 0x00,
            activateColorLoopStartEnHue = 0x01,
            activateColorLoopEnCurrentHue = 0x02
        };

        // color loop direction field
        enum {
            colorLoopDirectionDecrement = 0x00,
            colorLoopDirectionIncrement = 0x01,
        };

        // startup colorTemperatureMireds
        enum {
            colorTempMiredsReserved = 0xffff
        };

        // define the default number of update to store the attribute into nvm when moving attribute
        enum { defaultNoOfUpdateToNvm = 5};

        // define the default update period in ms when moving attribute
        enum { defaultUpdatePeriod = 100};

        // Generic color Control cluster options
        enum { optionExecuteIfOff = 0x01};

    // Construction
    public:
        CQorvoClusterColorControl
            (CZigBeeFoundationApplication &application,
                const unsigned char nCurrentHue = 0,
                const unsigned char nCurrentSaturation = 0xfe);

    // Attributes
    public:
        unsigned char m_CurrentHue;
        unsigned char m_CurrentSaturation;
        unsigned short m_RemainingTime;
        unsigned short m_CurrentX;
        unsigned short m_CurrentY;
     /* unsigned char m_DriftCompensation; */
     /* CharString m_CompensationText; */
        unsigned short m_ColorTemp;
        unsigned char m_ColorMode;
        unsigned char m_Options;
        unsigned char m_NumberOfPrimaries;
        unsigned short m_Primary1X;
        unsigned short m_Primary1Y;
        unsigned char m_Primary1Intensity;
        unsigned short m_Primary2X;
        unsigned short m_Primary2Y;
        unsigned char m_Primary2Intensity;
        unsigned short m_Primary3X;
        unsigned short m_Primary3Y;
        unsigned char m_Primary3Intensity;
     /* unsigned short m_Primary4X; */
     /* unsigned short m_Primary4Y; */
     /* unsigned char m_Primary4Intensity; */
     /* unsigned short m_Primary5X; */
     /* unsigned short m_Primary5Y; */
     /* unsigned char m_Primary5Intensity; */
     /* unsigned short m_Primary6X; */
     /* unsigned short m_Primary6Y; */
     /* unsigned char m_Primary6Intensity; */
     /* unsigned short m_WhitePointX; */
     /* unsigned short m_WhitePointY; */
     /* unsigned short m_ColorPointRX; */
     /* unsigned short m_ColorPointRY; */
     /* unsigned char m_ColorPointRIntensity; */
     /* unsigned short m_ColorPointGX; */
     /* unsigned short m_ColorPointGY; */
     /* unsigned char m_ColorPointGIntensity; */
     /* unsigned short m_ColorPointBX; */
     /* unsigned short m_ColorPointBY; */
     /* unsigned char m_ColorPointBIntensity; */
        unsigned short m_EnhancedCurrentHue;
        unsigned char m_EnhancedColorMode;
        unsigned char m_ColorLoopActive;
        unsigned char m_ColorLoopDirection;
        unsigned short m_ColorLoopTime;
        unsigned short m_ColorLoopStartEnhancedHue;
        unsigned short m_ColorLoopStoredEnhancedHue;
        unsigned short m_ColorCapabilities;
        unsigned short m_ColorTempPhysicalMin;
        unsigned short m_ColorTempPhysicalMax;
        unsigned short m_ColorTempLevelMinMireds; 
        unsigned short m_StartUpColorTempMireds; 

    // Overrides
    public:
        // Stores relevant attributes as a scene extension field set in the
        // supplied packet object
        virtual bool StoreSceneAttributes(CPacket &scene) const;

        // Loads an extension field set from the supplied packet object and
        // applies the values to the appropriate attributes. Transition time is
        // specified in tenths of a second
        virtual bool LoadSceneAttributes(const CPacket &scene,
            const unsigned int nTransitionTime);
            
        // Called after power-up. Evaluates the color configuration settings
        // to set current attributes and stored attributes appropriately
        virtual void AfterPowerup();
        
    // Operations
    public:

        // Enable the color temperature be set by the higher level
        void SetColorMinMaxTemp(const unsigned short minColorTemp, const unsigned short maxColorTemp);

        // Set color mode and sync to Enhanced Color Mode
        void SetColorMode(unsigned char colorMode);
    
        // Implements the Stop Move Step command
        unsigned char StopMoveStep(unsigned char nOptionMask = 0, unsigned char nOptionOverride = 0);

        // Implements the color loop command
        unsigned char ColorLoopSet(const unsigned char nUpdateFlags, const unsigned char nAction,
            const unsigned char nDirection = colorLoopDirectionIncrement,
            const unsigned short nTransitionTime = 1,
            const unsigned short nStartHue = 0x0000,
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);
            
        // Implements the MoveHue command
        unsigned char MoveHue(const unsigned char nMode, const unsigned char nRate,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);
        
        // Implements the MoveToHue command
        unsigned char MoveToHue(const unsigned char nHue, const unsigned char nDirection,
            unsigned short nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the StepHue command
        unsigned char StepHue(const unsigned char nStepMode, const unsigned char nStepSize,
            unsigned char nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the MoveSaturation command
        unsigned char MoveSaturation(const unsigned char nMode, const unsigned char nRate,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);
        
        // Implements the MoveToSaturation command
        unsigned char MoveToSaturation(const unsigned char nSaturation, unsigned short nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the StepSaturation command
        unsigned char StepSaturation(const unsigned char nStepMode, const unsigned char nStepSize,
            unsigned char nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the MoveToHueAndSaturation command
        unsigned char MoveToHueAndSaturation(const unsigned char nHue, const unsigned char nSaturation,
            unsigned short nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the EnhancedMoveHue command
        unsigned char EnhancedMoveHue(const unsigned char nMode, const unsigned short nRate,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the EnhancedMoveToHue command
        unsigned char EnhancedMoveToHue(const unsigned short nEnhancedHue, const unsigned char nDirection,
            unsigned short nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the EnhancedStepHue command
        unsigned char EnhancedStepHue(const unsigned char nStepMode, const unsigned short nStepSize,
            unsigned short nTransitionTime,
            unsigned char nOptionMask = 0,
            unsigned char nOptionOverride = 0);

        // Implements the EnhancedMoveToHueAndSaturation command
        unsigned char EnhancedMoveToHueAndSaturation(const unsigned short nEnhancedHue, const unsigned char nSaturation,
           unsigned short nTransitionTime, 
           unsigned char nOptionMask = 0, 
           unsigned char nOptionOverride = 0);

        // Implements the MoveColor command
        unsigned char MoveColor(const short nRateX, const short nRateY, 
           unsigned char nOptionMask = 0, 
           unsigned char nOptionOverride = 0);

        // Implements the MoveToColor command
        unsigned char MoveToColor(const unsigned short nColorX, const unsigned short nColorY,
            unsigned short nTransitionTime, 
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);

        // Implements the StepColor command
        unsigned char StepColor(const short nStepX, const short nStepY,
            unsigned short nTransitionTime, 
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);
        
        // Implements the MoveColorTemp command
        unsigned char MoveColorTemperature(const unsigned char nMode, const unsigned short nRate, 
            unsigned short nMinLimit = 0, unsigned short nMaxLimit = 0, 
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);

        // Implements the MoveToColorTemp command
        unsigned char MoveToColorTemperature(const unsigned short nColortemp, const unsigned short nTransitionTime, 
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);

         // Implements the StepColorTemp command
        unsigned char StepColorTemperature(const unsigned char nStepMode, const unsigned short nStepSize,
            unsigned short nTransitionTime, unsigned short nMinLimit = 0, unsigned short nMaxLimit = 0, 
            unsigned char nOptionMask = 0, 
            unsigned char nOptionOverride = 0);      
            
    // Client to server command handling
    protected:
        void OnMoveToHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnMoveHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnStepHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnMoveToSaturation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnMoveSaturation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnStepSaturation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnMoveToHueAndSaturation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        void OnMoveToColor(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        void OnMoveColor(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        void OnStepColor(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        void OnMoveToColorTemperature(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        
        void OnEnhancedMoveToHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnEnhancedMoveHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnEnhancedStepHue(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnEnhancedMoveToHueAndSaturation(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnColorLoopSet(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnStopMoveStep(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnMoveColorTemperature(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

        void OnStepColorTemperature(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
    
    // Server to client command handling
    public:
        
    //Implementation
    
    protected:
        // application specific, number of update to store the attribute into nvm when moving attribute
        unsigned char m_NoOfUpdateToNvm;

        // application specific, update period when moving attribute
        unsigned char m_UpdatePeriod;

        //Periodic update trigger - XY color attribute
        static void UpdateColorXY(void* pArg);
        
        //Periodic update trigger - Hue and Saturation color attribute
        static void UpdateColorHS(void* pArg);

        //Periodic update trigger - Enhanced Hue and Saturation color attribute
        static void UpdateColorEnHS(void* pArg);
                
        //periodic update trigger - color temperature attribute
        static void UpdateColorTemperature(void* pArg);
        
        //periodic update trigger - current Saturatione attribute
        static void UpdateColorMoveSaturation(void* pArg);
        
        //Periodic update trigger - current Hue attribute
        static void UpdateColorMoveHue(void* pArg);
        
        //Periodic update trigger - enhanced current Hue attribute
        static void UpdateColorEnhancedMoveHue(void* pArg);
        
        //periodic update for color Loop set
        static void UpdateColorLoop(void* pArg);
        
        //Set enhanced color mode attribute upon reception of any commands
        void SetEnhancedColorMode(unsigned char enhancedColorMode);
        
        //store hue attribute value
        void ColorControlSetHue(unsigned char hue);
        
        //store enhanced hue attribute value
        void ColorControlSetEnhancedHue(unsigned short enhancedHue);
        
        //store saturation attribute value
        void ColorControlSetSaturation(unsigned char saturation);
        
        //store XY attribute value
        void ColorControlSetXY(unsigned short colorX, unsigned short colorY);
        
        //store Color Temperature attribute value
        void ColorControlSetColorTemperature(unsigned short colorTemperature);

        //store remaining time attribute    
        void ColorControlSetRemainingTime(unsigned short remainingTime);
        
        //clear remaining time attribute
        void ColorControlClearRemainingTime(void);
        
        //store color loop active attribute
        void ColorControlSetColorLoopActive(unsigned char active);

        //check the executeIfOff bit in option and OnOff status, return true if allow to execute the command
        bool ColorControlCheckExecuteIfOff(unsigned char nOptionMask = 0, unsigned char nOption = 0);
};

#define __COMPACT7B_COLORCONTROL_CURRENTHUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeCurrentHue, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
        m_CurrentHue)
#define __COMPACT7B_COLORCONTROL_CURRENTSATURATION() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeCurrentSaturation, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
        m_CurrentSaturation)
#define __COMPACT7B_COLORCONTROL_REMAININGTIME() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeRemainingTime, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_RemainingTime)
#define __COMPACT7B_COLORCONTROL_CURRENTX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeCurrentX, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
        m_CurrentX)
#define __COMPACT7B_COLORCONTROL_CURRENTY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeCurrentY, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
        m_CurrentY)
#if 0
#define __COMPACT7B_COLORCONTROL_DRIFTCOMPENSATION() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeDriftCompensation, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_DriftCompensation)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COMPENSATIONTEXT() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeCompensationText, \
        CZigBeeAttribute::typeCharString, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_CompensationText)
#endif
#define __COMPACT7B_COLORCONTROL_COLORTEMP() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorTemp, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::reportable, \
        m_ColorTemp)
#define __COMPACT7B_COLORCONTROL_COLORMODE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorMode, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorMode)
#define __COMPACT7B_COLORCONTROL_OPTIONS() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeOptions, \
        CZigBeeAttribute::typeBitmap8, 0, \
        0x0, \
        m_Options)
#define __COMPACT7B_COLORCONTROL_NUMBEROFPRIMARIES() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeNumberOfPrimaries, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_NumberOfPrimaries)
#define __COMPACT7B_COLORCONTROL_PRIMARY1X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary1X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary1X)
#define __COMPACT7B_COLORCONTROL_PRIMARY1Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary1Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary1Y)
#define __COMPACT7B_COLORCONTROL_PRIMARY1INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary1Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary1Intensity)
#define __COMPACT7B_COLORCONTROL_PRIMARY2X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary2X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary2X)
#define __COMPACT7B_COLORCONTROL_PRIMARY2Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary2Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary2Y)
#define __COMPACT7B_COLORCONTROL_PRIMARY2INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary2Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary2Intensity)
#define __COMPACT7B_COLORCONTROL_PRIMARY3X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary3X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary3X)
#define __COMPACT7B_COLORCONTROL_PRIMARY3Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary3Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary3Y)
#define __COMPACT7B_COLORCONTROL_PRIMARY3INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary3Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary3Intensity)
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY4X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary4X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary4X)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY4Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary4Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary4Y)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY4INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary4Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary4Intensity)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY5X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary5X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary5X)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY5Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary5Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary5Y)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY5INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary5Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary5Intensity)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY6X() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary6X, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary6X)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY6Y() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary6Y, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary6Y)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_PRIMARY6INTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributePrimary6Intensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_Primary6Intensity)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_WHITEPOINTX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeWhitePointX, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_WhitePointX)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_WHITEPOINTY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeWhitePointY, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_WhitePointY)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTRX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointRX, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointRX)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTRY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointRY, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointRY)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTRINTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointRIntensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        0x0, \
        m_ColorPointRIntensity)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTGX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointGX, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointGX)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTGY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointGY, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointGY)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTGINTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointGIntensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        0x0, \
        m_ColorPointGIntensity)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTBX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointBX, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointBX)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTBY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointBY, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        0x0, \
        m_ColorPointBY)
#endif
#if 0
#define __COMPACT7B_COLORCONTROL_COLORPOINTBINTENSITY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorPointBIntensity, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        0x0, \
        m_ColorPointBIntensity)
#endif

#define __COMPACT7B_COLORCONTROL_ENHANCEDCURRENTHUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeEnhancedCurrentHue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_EnhancedCurrentHue)

#define __COMPACT7B_COLORCONTROL_ENHANCEDCOLORMODE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeEnhancedColorMode, \
        CZigBeeAttribute::typeEnumeration8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_EnhancedColorMode)

#define __COMPACT7B_COLORCONTROL_COLORLOOPACTIVE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorLoopActive, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorLoopActive)

#define __COMPACT7B_COLORCONTROL_COLORLOOPDIRECTION() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorLoopDirection, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorLoopDirection)

#define __COMPACT7B_COLORCONTROL_COLORLOOPTIME() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorLoopTime, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorLoopTime)

#define __COMPACT7B_COLORCONTROL_COLORLOOPSTARTENHANCEDHUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorLoopStartEnhancedHue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorLoopStartEnhancedHue)
        
#define __COMPACT7B_COLORCONTROL_COLORLOOPSTOREDENHANCEDHUE() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorLoopStoredEnhancedHue, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorLoopStoredEnhancedHue)

#define __COMPACT7B_COLORCONTROL_COLORCAPABILITIES() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorCapabilities, \
        CZigBeeAttribute::typeBitmap16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorCapabilities)
#define __COMPACT7B_COLORCONTROL_COLORTEMPPHYSICALMIN() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorTempPhysicalMin, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorTempPhysicalMin)
#define __COMPACT7B_COLORCONTROL_COLORTEMPPHYSICALMAX() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorTempPhysicalMax, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorTempPhysicalMax)
#define __COMPACT7B_COLORCONTROL_COLORTEMPLEVELMINMIREDS() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeColorTempLevelMinMireds, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::accessReadOnly, \
        m_ColorTempLevelMinMireds)
#define __COMPACT7B_COLORCONTROL_STARTUPCOLORTEMPMIREDS() \
    __COMPACT7B_ATTRIBUTE_CLUSTER(CQorvoClusterColorControl, \
        CQorvoClusterColorControl::attributeStartUpColorTempMireds, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::persistent, \
        m_StartUpColorTempMireds)

#define __COMPACT7B_COLORCONTROL_CLUSTER_ATTRIBUTES() \
    __COMPACT7B_COLORCONTROL_CURRENTHUE() \
    __COMPACT7B_COLORCONTROL_CURRENTSATURATION() \
    __COMPACT7B_COLORCONTROL_REMAININGTIME() \
    __COMPACT7B_COLORCONTROL_CURRENTX() \
    __COMPACT7B_COLORCONTROL_CURRENTY() \
 /* __COMPACT7B_COLORCONTROL_DRIFTCOMPENSATION() */ \
 /* __COMPACT7B_COLORCONTROL_COMPENSATIONTEXT() */ \
    __COMPACT7B_COLORCONTROL_COLORTEMP() \
    __COMPACT7B_COLORCONTROL_COLORMODE() \
    __COMPACT7B_COLORCONTROL_OPTIONS() \
    __COMPACT7B_COLORCONTROL_NUMBEROFPRIMARIES() \
    __COMPACT7B_COLORCONTROL_PRIMARY1X()  \
    __COMPACT7B_COLORCONTROL_PRIMARY1Y()  \
    __COMPACT7B_COLORCONTROL_PRIMARY1INTENSITY() \
    __COMPACT7B_COLORCONTROL_PRIMARY2X()  \
    __COMPACT7B_COLORCONTROL_PRIMARY2Y()  \
    __COMPACT7B_COLORCONTROL_PRIMARY2INTENSITY() \
    __COMPACT7B_COLORCONTROL_PRIMARY3X()  \
    __COMPACT7B_COLORCONTROL_PRIMARY3Y() \
    __COMPACT7B_COLORCONTROL_PRIMARY3INTENSITY() \
 /* __COMPACT7B_COLORCONTROL_PRIMARY4X() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY4Y() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY4INTENSITY() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY5X() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY5Y() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY5INTENSITY() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY6X() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY6Y() */ \
 /* __COMPACT7B_COLORCONTROL_PRIMARY6INTENSITY() */ \
 /* __COMPACT7B_COLORCONTROL_WHITEPOINTX() */ \
 /* __COMPACT7B_COLORCONTROL_WHITEPOINTY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTRX() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTRY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTRINTENSITY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTGX() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTGY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTGINTENSITY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTBX() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTBY() */ \
 /* __COMPACT7B_COLORCONTROL_COLORPOINTBINTENSITY() */ \
    __COMPACT7B_COLORCONTROL_ENHANCEDCURRENTHUE() \
    __COMPACT7B_COLORCONTROL_ENHANCEDCOLORMODE() \
    __COMPACT7B_COLORCONTROL_COLORLOOPACTIVE() \
    __COMPACT7B_COLORCONTROL_COLORLOOPDIRECTION() \
    __COMPACT7B_COLORCONTROL_COLORLOOPTIME() \
    __COMPACT7B_COLORCONTROL_COLORLOOPSTARTENHANCEDHUE() \
    __COMPACT7B_COLORCONTROL_COLORLOOPSTOREDENHANCEDHUE() \
    __COMPACT7B_COLORCONTROL_COLORCAPABILITIES() \
    __COMPACT7B_COLORCONTROL_COLORTEMPPHYSICALMIN() \
    __COMPACT7B_COLORCONTROL_COLORTEMPPHYSICALMAX() \
    __COMPACT7B_COLORCONTROL_COLORTEMPLEVELMINMIREDS()  \
    __COMPACT7B_COLORCONTROL_STARTUPCOLORTEMPMIREDS()  \
    /*End*/

#define __COMPACT7B_COLORCONTROL_CLUSTER_COMMANDS() \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveToHue, &CQorvoClusterColorControl::OnMoveToHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveHue, &CQorvoClusterColorControl::OnMoveHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandStepHue, &CQorvoClusterColorControl::OnStepHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveToSaturation, &CQorvoClusterColorControl::OnMoveToSaturation) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveSaturation, &CQorvoClusterColorControl::OnMoveSaturation) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandStepSaturation, &CQorvoClusterColorControl::OnStepSaturation) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveToHueAndSaturation, &CQorvoClusterColorControl::OnMoveToHueAndSaturation) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveToColor, &CQorvoClusterColorControl::OnMoveToColor) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveColor, &CQorvoClusterColorControl::OnMoveColor) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandStepColor, &CQorvoClusterColorControl::OnStepColor) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveToColorTemperature, &CQorvoClusterColorControl::OnMoveToColorTemperature) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandEnhancedMoveToHue, &CQorvoClusterColorControl::OnEnhancedMoveToHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandEnhancedMoveHue, &CQorvoClusterColorControl::OnEnhancedMoveHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandEnhancedStepHue, &CQorvoClusterColorControl::OnEnhancedStepHue) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandEnhancedMoveToHueAndSaturation, &CQorvoClusterColorControl::OnEnhancedMoveToHueAndSaturation) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandColorLoopSet, &CQorvoClusterColorControl::OnColorLoopSet) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandStopMoveStep, &CQorvoClusterColorControl::OnStopMoveStep) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandMoveColorTemperature, &CQorvoClusterColorControl::OnMoveColorTemperature) \
    __COMPACT7B_COMMAND_C2S(CQorvoClusterColorControl::commandStepColorTemperatue, &CQorvoClusterColorControl::OnStepColorTemperature) \
    /*End*/

#endif // _ZIGBEE_CLUSTER_COLORCONTROL_H_
