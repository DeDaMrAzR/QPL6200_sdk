/*
 *   Copyright (c) 2017-2019, Qorvo Inc
 *
 *   Application Ble API
 *   Declarations of the public functions and enumerations of BLE.
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 */

/// @file "ZigBeeCommissioning_Callback.h"
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
 *                    Application Callback Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/** @brief callback indicated that ZB 3.0 commissioning is started.
 *      Header: ZigBeeCommissioning_Callback.h
*/
void Application_cbZigBeeCommissioningStarted(void);

/** @brief Confirm from the Zigbee stack to user application for the Startup process
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param nStatus               APS status return
 *   @param nFlags                Startup flags used
*/
void Application_cbZigBeeStartupComplete(UInt8 nStatus, UInt16 flags);

/** @brief Callback from Zigbee stack to user application when device leaves the network
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param nStatus               Levae indication status 
 *   @param bRejoin               True when rejoin is enabled. False when disabled.
*/
void Application_cbZigBeeLeave(const uint8_t nStatus, const bool bRejoin);

/** @brief Callback from Zigbee stack to user application when Permit join is modified
 *      Header: ZigBeeCommissioning_Callback.h
 *
 *   @param bAssociationPermit       True when permit join is enabled
*/
void Application_cbAssociationPermitModified(Bool bAssociationPermit);

#ifdef __cplusplus
}
#endif //__cplusplus

