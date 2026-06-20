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

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Macro defines for application name in debug log*/
#define APP_NAME "ZB3.0 DoorWindow"

/** @brief Macro defines for endpoint info in debug log*/
#define EP1 "IAS Device"

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

//Indications
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

#endif //defined(GP_APP_DIVERSITY_ZB)
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_APP_H_
