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

/// @file "ResponsiveModePolling.h"
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define RESPONSIVEMODETIMEOUT   (60000 /* 60 seconds */)
#define INDIRECTTRANSMITTIMEOUT ( 7600 /* 7.6 seconds*/)

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

//Requests
/** @brief Start responsive mode polling and start a timeout
           Stop responsive mode polling upon timeout
*/
void StartResponsiveModeWithTimeout(UInt32 timeout_msec = RESPONSIVEMODETIMEOUT);

/** @brief Stop responsive mode polling and abort the timeout
*/
void StopResponsiveMode(void);

#ifdef __cplusplus
}
#endif //__cplusplus

