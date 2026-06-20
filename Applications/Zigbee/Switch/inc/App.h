/*
 * Copyright (c) 2019, Qorvo Inc
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
 */

/** @file "App.h"
 *
 *  Application API
 *
 *  Declarations of the public functions and enumerations of App.
*/

#ifndef _APP_H_
#define _APP_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/** @enum Application_LightDirection_t
*/
#define Application_LightDirectionUp                           0
#define Application_LightDirectionDown                         1
#define Application_LightDirectionStop                         2
/** @typedef Application_LightDirection_t
*/
typedef UInt8                             Application_LightDirection_t;

/** @enum Enumerations for control mode to be used in application
*/
#define App_ControlMode_Binding     0
#define App_ControlMode_Group       1
/** @typedef App_ControlMode_t
*/
typedef UInt8 App_ControlMode_t;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro defines for application name in debug log*/
#ifdef GP_APP_DIVERSITY_ZB
#ifdef GP_APP_DIVERSITY_COLOR_SWITCH
#define APP_NAME "ZB3.0 Color Light Switch"
#else
#define APP_NAME "ZB3.0 Light Switch"
#endif // GP_APP_DIVERSITY_COLOR_SWITCH
#endif // GP_APP_DIVERSITY_ZB

/** @brief Macro defines for endpoint info in debug log*/
#ifdef GP_APP_DIVERSITY_COLOR_SWITCH
#define EP1 "Color Switch"
#else
#define EP1 "Light Switch"
#endif

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
// Indications
#if defined(GP_APP_DIVERSITY_ZB)

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

/** @brief Control on/off and colorLoop of the Color Light
*
*   @param toggle                    Toggle the color light - no effect if colorLoop is True
*   @param colorLoop                 Start/Stop color loop.
*/
void Application_ControlColorLight(Bool toggle, Bool colorLoop);
#endif //defined(GP_APP_DIVERSITY_ZB)
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_APP_H_
