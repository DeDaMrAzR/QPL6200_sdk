/*
 * Copyright (c) 2013-2014, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
 *
 * gpMacCore_DiagRxCntrs.c
 *   This file contains the implementation of diagnostic counters.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

// General includes

#include "gpMacCore.h"
#include "gpPd.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#define MACCORE_DIAG_UNUSED_ENTRY    0xFFFF

typedef struct {
    gpPd_Lqi_t  lqi;
    gpPd_Rssi_t rssi;
    UInt16      shortAddr;
} MacCore_DiagRxCntr_t;

typedef struct {
    UInt32      totalRetryCntr;
    UInt32      txCntr;
    UInt16      shortAddr;
} MacCore_DiagTxCntr_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#define GP_MACCORE_DIAGCNTRS    16
MacCore_DiagRxCntr_t gpMacCore_DiagRxCntr[GP_MACCORE_DIAGCNTRS];
MacCore_DiagTxCntr_t gpMacCore_DiagTxCntr[GP_MACCORE_DIAGCNTRS];

//storing LQI/RSSI for last receieved message (for use in controlled environment only)
MacCore_DiagRxCntr_t gpMacCore_DiagRxLastCntr;

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Internal Function Definitions
 *****************************************************************************/

void MacCore_ResetDiagCntr(void)
{
    UIntLoop i;

    MEMSET(gpMacCore_DiagRxCntr, 0, sizeof(gpMacCore_DiagRxCntr));
    MEMSET(gpMacCore_DiagTxCntr, 0, sizeof(gpMacCore_DiagTxCntr));
    MEMSET(&gpMacCore_DiagRxLastCntr, 0, sizeof(MacCore_DiagRxCntr_t));
    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        gpMacCore_DiagRxCntr[i].shortAddr = MACCORE_DIAG_UNUSED_ENTRY;
        gpMacCore_DiagTxCntr[i].shortAddr = MACCORE_DIAG_UNUSED_ENTRY;
    }
}

/*
    RX
*/
void MacCore_DiagRxCntrUpdateLastMessage(gpPd_Loh_t *pdLoh, UInt16 shortAddr)
{
    UIntLoop i;

    gpMacCore_DiagRxLastCntr.lqi =  gpPd_GetLqi(pdLoh->handle);
    gpMacCore_DiagRxLastCntr.rssi =  gpPd_GetRssi(pdLoh->handle);

    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        if(gpMacCore_DiagRxCntr[i].shortAddr == MACCORE_DIAG_UNUSED_ENTRY)
        {
            gpMacCore_DiagRxCntr[i].shortAddr = shortAddr;
        }
        if(gpMacCore_DiagRxCntr[i].shortAddr == shortAddr)
        {
            // update
            gpMacCore_DiagRxCntr[i].lqi  = gpMacCore_DiagRxLastCntr.lqi;
            gpMacCore_DiagRxCntr[i].rssi = gpMacCore_DiagRxLastCntr.rssi;
            return;
        }
    }
}

/*
 TX
*/
void MacCore_DiagTxCntrUpdateAvgRetryMessageSent(UInt16 retryCntr, UInt16 shortAddr)
{
    UIntLoop i;

    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        if(gpMacCore_DiagTxCntr[i].shortAddr == MACCORE_DIAG_UNUSED_ENTRY)
        {
            gpMacCore_DiagTxCntr[i].shortAddr = shortAddr;
        }
        if(gpMacCore_DiagTxCntr[i].shortAddr == shortAddr)
        {
            // update
            gpMacCore_DiagTxCntr[i].totalRetryCntr += retryCntr;
            gpMacCore_DiagTxCntr[i].txCntr += 1;
            return;
        }
    }
}

/** @brief Fill in Tx counter information about all stored short addresses */
static void MacCore_DiagGetAllTxCntrInfo(gpMacCore_DiagTxCntr_t* pDiagnostics)
{
    UIntLoop i;
    UInt32 txCntr = 0x0;
    UInt32 totalRetryCntr = 0x0;

    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        if(gpMacCore_DiagTxCntr[i].shortAddr != MACCORE_DIAG_UNUSED_ENTRY)
        {
            txCntr         += gpMacCore_DiagTxCntr[i].txCntr;
            totalRetryCntr += gpMacCore_DiagTxCntr[i].totalRetryCntr;
        }
    }

    pDiagnostics->txCntr = txCntr;
    pDiagnostics->avgRetryCntr = (txCntr != 0) ? totalRetryCntr/txCntr : 0;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*
    RX
*/
Bool gpMacCore_DiagRxCntrGetLastMessage(UInt16 shortAddr, gpMacCore_DiagRxCntr_t* pDiagnostics)
{
    UIntLoop i;

    if(NULL == pDiagnostics)
    {
        return false;
    }

    // For 0xFFFF, just get the last stored value, only useful in a controlled environment
    if(GP_MACCORE_STATISTICS_ALL_DEVICES == shortAddr)
    {
        pDiagnostics->lqi = gpMacCore_DiagRxLastCntr.lqi;
        pDiagnostics->rssi = gpMacCore_DiagRxLastCntr.rssi;
        return true;
    }

    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        if(gpMacCore_DiagRxCntr[i].shortAddr == shortAddr)
        {
            pDiagnostics->lqi = gpMacCore_DiagRxCntr[i].lqi;
            pDiagnostics->rssi = gpMacCore_DiagRxCntr[i].rssi;

            // Free up this entry
            gpMacCore_DiagRxCntr[i].shortAddr = MACCORE_DIAG_UNUSED_ENTRY;
            return true;
        }
    }
    return false;
}

/*
 TX
*/
Bool gpMacCore_DiagTxCntrGetAvgRetryMessage(UInt16 shortAddr, gpMacCore_DiagTxCntr_t* pDiagnostics)
{
    UIntLoop i;

    if(NULL == pDiagnostics)
    {
        return false;
    }

    //Init counter
    pDiagnostics->txCntr = 0;
    pDiagnostics->avgRetryCntr = 0x0;

    if(GP_MACCORE_STATISTICS_ALL_DEVICES == shortAddr)
    {
        MacCore_DiagGetAllTxCntrInfo(pDiagnostics);
        return true;
    }

    for(i=0; i < GP_MACCORE_DIAGCNTRS; i++)
    {
        if(gpMacCore_DiagTxCntr[i].shortAddr == shortAddr)
        {
            pDiagnostics->txCntr = gpMacCore_DiagTxCntr[i].txCntr;
            pDiagnostics->avgRetryCntr = (pDiagnostics->txCntr != 0) ? gpMacCore_DiagTxCntr[i].totalRetryCntr/gpMacCore_DiagTxCntr[i].txCntr : 0;
            return true;
        }
    }
    return false;
}

