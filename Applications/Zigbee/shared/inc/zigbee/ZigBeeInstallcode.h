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

/** @file "ZigBeeInstallcode.h"
 *
 * Source code that provide support to install code in Zigbee stack
*/

#ifndef _ZIGBEEINSTALLCODE_H_
#define _ZIGBEEINSTALLCODE_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define FACTORYBLOCK_INSTALLCODE_OFFSET 0x100

#define INSTALL_CODE_SIZE 16 + 2 // 2 byte CRC
#define LINK_KEY_SIZE     16

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

/** @brief Apply the Install code from persistent memory
*   After passing the CRC check, the perconfigured link key will be generated from install code
*   and applied into system
*/
void Application_ZigBeeApplyInstallCode(void);

/** @brief Dump the Install code from persistent memory
*/
void Application_ZigBeeDumpInstallCode(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_ZIGBEEINSTALLCODE_H_
