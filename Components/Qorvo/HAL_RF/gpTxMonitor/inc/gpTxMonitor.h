/*
 * Copyright (c) 2013-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
*
*  The file gpTxMonitor.h contains the definitions for monitoring voltages during transmits
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

#ifndef _GP_TX_MONITOR_H_
#define _GP_TX_MONITOR_H_

/*****************************************************************************
*                    Includes Definitions
*****************************************************************************/

#include <global.h>

/**
 * @file gpTxMonitor.h
 *
 * @brief This file contains interface to the gpTxMonitor component
 *
 *  The application requiring the Tx Monitor support only needs to include this header file.
 */

/**
 *
 * This gpTxMonitor component contains routines that monitor voltages during transmissions.
 *
 */

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/**
 *  gpTxMonitor_Init function initializes resources used by the gpTxMonitor component.
 */

//enable/disable gpTxMonitor
void gpTxMonitor_Enable(void);
void gpTxMonitor_Disable(void);

void gpTxMonitor_SetPwrCtrlInByPassMode ( Bool ByPassPwrCtrl );
Bool gpTxMonitor_GetPwrCtrlInByPassMode ( void );
void gpTxMonitor_Init(UInt8 settlingTimeInSymbols);

Bool gpTxMonitor_TriggerUnloadedBatteryVoltage(void);
void gpTxMonitor_ClearUnloadedBatteryVoltage(void);
Bool gpTxMonitor_GetUnloadedBatteryVoltage(UQ2_6* pVoltage);

void gpTxMonitor_ClearLoadedBatteryVoltage(void);
Bool gpTxMonitor_GetLoadedBatteryVoltage(UQ2_6* pVoltage);

void gpTxMonitor_ClearPowerDetectorVoltage(void);
Bool gpTxMonitor_GetPowerDetectorVoltage(UQ2_14* pVoltage);

void gpTxMonitor_Calibrate(void);

UQ2_14 gpTxMonitor_GetPowerDetectorCalVoltage(void);

//internal gphal interfaces for K4
void gpTxMonitor_TxStarted(void);

//internal gphal interfaces for K5 and K7
void gpTxMonitor_AnnounceTxStart(void);
void gpTxMonitor_AnnounceTxFinished(void);


#endif //_GP_TX_MONITOR_H_
