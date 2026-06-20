/*
 * Copyright (c) 2017, GreenPeak Technologies
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
 */

/** @file "ZigBeeFindingAndBinding.h"
 *
 *  Application API
 *
 *  Declarations of the public functions and enumerations of ZigBee's finding and binding
*/

#ifndef _ZIGBEE_FINDINGANDBINDING_CALLBACK_H_
#define _ZIGBEE_FINDINGANDBINDING_CALLBACK_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "global.h"
#include "ZigBee.h"
/** @enum Enumerations for control mode to be used in application
 */
#define App_ControlMode_Binding 0
#define App_ControlMode_Group   1
/** @typedef App_ControlMode_t
 */
typedef UInt8 App_ControlMode_t;

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
 *                    Application Callback Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/** @brief callback when a binding is set/removed as a result of finding and binding
*/
void Application_cbZigBeePostManageBinding(CZigBeeDeviceObjectBinding &binding, const Bool bBind);

#if defined(GP_APP_DIVERSITY_ZB)
/** @brief function get current control mode in application
 */
App_ControlMode_t Application_GetControlMode(void);

/** @brief function set current control mode in application
 *
 *   @param mode              Control mode to set
 */
void Application_SetControlMode(App_ControlMode_t mode);

/** @brief function set toggle control mode in application
 */
void Application_ToggleControlMode(void);

#endif // GP_APP_DIVERSITY_ZB
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_ZIGBEE_FINDINGANDBINDING_CALLBACK_H_
