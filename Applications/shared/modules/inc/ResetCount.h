/*
 * Copyright (c) 2023, Qorvo Inc
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

/** @file "App_ResetCount.h"
 *
 *  Application API
 *
 *  Declarations of the public functions and enumerations of Reset Counting
 */

#ifndef _APP_RESETCOUNT_H_
#define _APP_RESETCOUNT_H_

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
typedef struct {
    uint8_t Count;
} ResetCountEvent_t;

/*****************************************************************************
 *                    Callback Function Prototypes
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/
void ResetCount_Init(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_APP_RESETCOUNT_H_
