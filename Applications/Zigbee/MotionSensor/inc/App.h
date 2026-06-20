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

#ifndef _APP_H_
#define _APP_H_

/// @file "App.h"
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/** @enum Enumerations for control mode to be used in application
 */
#define App_ControlMode_Binding 0
#define App_ControlMode_Group   1
/** @typedef App_ControlMode_t
 */
typedef UInt8 App_ControlMode_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro defines for application name in debug log*/
#define APP_NAME "ZB3.0 Motion Sensor"

/** @brief Macro defines for endpoint info in debug log*/
#define EP1 "OnOff Sensor"
#define EP2 "Occupancy Sensor"
#define EP3 "Light Sensor"

/** @brief Macro defines for group ID to be used in group control mode */
#if !defined(GP_APPLICATION_GROUP_ID)
#define GP_APPLICATION_GROUP_ID 0x0001
#endif // !defined(GP_APPLICATION_GROUP_ID)

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
// Requests
/** @brief Get the measured illuminance from the system.
 *   @param pIlluminance              Measured illuminance value
 *   @return success                  True if valid temperature value is returned.
 */
Bool Application_GetIlluminance(UInt16* pIlluminance);

// Indications
/** @brief The identification process has started (e.g. LED indicator)
 *
 *   @param endpoint                  The endpoint triggering the identify
 */
void Application_cbIdentifyStart(UInt8 endpoint);

/** @brief The identification process has stopped (e.g. LED indicator)
 *
 *   @param endpoint                  The endpoint triggering the identify
 */
void Application_cbIdentifyStop(UInt8 endpoint);

/** @brief callback for LED indication for identifying.
 *
 *   @param isIdentifying        whether device is identifying
 *   @param endpoint             identifying endpoint
 */
void Application_cbLedIndication_Identify(Bool isIdentifying, UInt8 endpoint);

/** @brief callback for indicating OccupiedToUnOccupiedDelay attribute change.
 *
 *   @param delay                OccupiedToUnoccupiedDelay value
 */
void Application_cbUpdateOccupiedToUnoccupiedDelay(UInt16 delay);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_APP_H_
