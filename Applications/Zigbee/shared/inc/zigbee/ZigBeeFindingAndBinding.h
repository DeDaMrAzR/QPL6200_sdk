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

#ifndef _ZIGBEE_FINDINGANDBINDING_H_
#define _ZIGBEE_FINDINGANDBINDING_H_

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

/** @brief Start/Stop Finding and Binding process(Target) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint     - Target endpoint for the operation
*   @param enable       - enable/disable finding and binding
*/
void Application_SetFindingAndBinding_Target(UInt8 endpoint, Bool enable);

/** @brief Toggle Finding and Binding process(Target) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint   - Target endpoint for the operation
*/
void Application_ToggleFindingAndBinding_Target(UInt8 endpoint);

/** @brief Start Finding and Binding process(Initiator) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint   - initiator endpoint for the operation
*/
void Application_StartFindingAndBinding_Initiator(UInt8 endpoint);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_ZIGBEE_FINDINGANDBINDING_H_
