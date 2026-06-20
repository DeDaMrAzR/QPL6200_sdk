/*
 * Copyright (c) 2024-2025, Qorvo Inc
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

/** @file "App_Common.h"
 */

#ifndef _APP_COMMON_H_
#define _APP_COMMON_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigbeeTask.h"

#ifdef __cplusplus

#include "OccupancySensorApplication.h"
#include "OnOffSensorApplication.h"
#include "LightSensorApplication.h"

#endif //__cplusplus

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
 *                    Extern Data Definitions
 *****************************************************************************/
#ifdef __cplusplus
/** extern current endpoint class */
extern COnOffSensorApplication appsEp_1;
extern COccupancySensorApplication appsEp_2;
extern CLightSensorApplication appsEp_3;
#endif //__cplusplus

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/** @brief Factory Reset the Application for all module
 */
#define Application_FactoryReset() ZigbeeTaskMgr().FactoryReset()

#endif //_APP_COMMON_H_
