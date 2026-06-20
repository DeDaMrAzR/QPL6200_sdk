/*
 * Copyright (c) 2017, Qorvo Inc
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

/** @file "ZigBeeTouchLink.h"
 *
 * Header that provide support to ZigBee touchlink commissioning
*/

#ifndef _ZIGBEETOUCHLINK_H_
#define _ZIGBEETOUCHLINK_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Class Definitions
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
/** @brief Class CExTouchlinkDeviceId definition - legacy ZLL device ID and version definition*/
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
class CExTouchlinkDeviceId {
    // typedefs & enums
    public:
    /** @brief Enums for legacy ZLL Deivce ID
        */
    enum {
        altDeviceId_OnOffLight = 0x0000,
        altDeviceId_OnOffPlugInUnit = 0x0010,
        altDeviceId_DimmableLight = 0x0100,
        altDeviceId_DimmablePlugInUnit = 0x0110,
        altDeviceId_ColourLight = 0x0200,
        altDeviceId_ExtendedColourLight = 0x0210,
        altDeviceId_ColourTemperatureLight = 0x0220,
        altDeviceId_ColourController = 0x0800,
        altDeviceId_ColourSceneController = 0x0810,
        altDeviceId_NonColourController = 0x0820,
        altDeviceId_NonColourSceneController = 0x0830,
        altDeviceId_ColourBridge = 0x0840,
        altDeviceId_OnOffSensor = 0x0850,
    };

    /** @brief Enums for legacy ZLL Deivce ID version
        */
    enum {
        altDeviceId_Version = 2
    };
};

#endif //(GP_APP_DIVERSITY_TOUCHLINK_CLIENT) || defined(GP_APP_DIVERSITY_TOUCHLINK)
#if defined(GP_APP_DIVERSITY_TOUCHLINK)
/** @brief Class CExTouchlinkApplication definition - override TouchLink Server cluster */
class CExTouchlinkApplication : public CZigBeeClusterTouchlink {
    // Construction
    public:
    CExTouchlinkApplication(CZigBeeFoundationApplication& application);

    // typedefs & enums
    public:
    enum {
        statusOnSessionEstablished = 0x00,
        statusOnSessionTerminated = 0x01,
        statusOnApply = 0x02,
        statusAfterCommit = 0x03,
    };

    // Attributes
    public:
    // Overrides
    public:
    // Invoked when a new session has just started. It is safe call
    // GetSessionID() from within this notification handler
    virtual void OnSessionEstablished();

    // Invoked when an existing session has expired. NOTICE: The session is
    // no longer active; hence, it is unsafe to call GetSessionID() from
    // within this notification handler. The parameter indicates whether
    // the session expired or was terminated, e.g. due to another scan
    // request
    virtual void OnSessionTerminated(const bool bExpired);

    // Called immediately before the new commissioning parameters will
    // take effect, i.e. immediately before forming or joining a network
    // in response to a touchlink commissioning action
    virtual void OnApply(const CZigBeeTouchlinkCommissioningInfo& info);

    // Called when a new set of network parameters has been commissioned
    // via touch-link. The device must now seize all previous activities
    // and start the zigbee networking stack.
    virtual void AfterCommit();
};

#endif //defined(GP_APP_DIVERSITY_TOUCHLINK)
/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#endif //_ZIGBEETOUCHLINK_H_
