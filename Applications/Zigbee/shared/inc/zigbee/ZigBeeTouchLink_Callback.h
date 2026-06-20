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

/** @file "ZigBeeTouchLink_Callback.h"
 *
 * Header for the callback in ZigBee touchlink commissioning
*/

#ifndef _ZIGBEETOUCHLINK_CALLBACK_H_
#define _ZIGBEETOUCHLINK_CALLBACK_H_

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
 *                    Public Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Callbakc Function Prototypes
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_TOUCHLINK
/** @brief Updates touchlink session status
*
*   @param status                    APS status return
*   @param pArg                      pointer to touchlink status argument
*/
void Application_cbTouchlinkSessionUpdate(uint8_t status, void* pArg);

#endif //GP_APP_DIVERSITY_TOUCHLINK
#if defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)
/** @brief Callback for Touchlink Client commissioning process
*
*   @param status                    Touchlink Client commissioning progress status
*/
void Application_cbTouchlinkClientCommissioningProgress(uint8_t status);

#endif //defined(GP_APP_DIVERSITY_TOUCHLINK_CLIENT)

#if defined(GP_APP_DIVERSITY_ZB)
/** @brief function to send add group to target
 *
 *   @param destination              Class CZigBeeAddress for the destination
 *   @param groupId                  Group ID to be added
 */
void Application_SendAddGroup(CZigBeeAddress destination, unsigned short groupId);
#endif // GP_APP_DIVERSITY_ZB

#endif //_ZIGBEETOUCHLINK_H_
