/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpBaseComps_StackInit.c
 *
 * The file contains the function that initializes all the base components that are used.
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


#define GP_COMPONENT_ID GP_COMPONENT_ID_BASECOMPS

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpBaseComps.h"

#include "hal.h"

#ifdef GP_COMP_SCHED
#include "gpSched.h"
#endif //GP_COMP_SCHED

#ifdef GP_COMP_BATTERYMONITOR
#include "gpBatteryMonitor.h"
#endif //GP_COMP_BATTERYMONITOR

#ifdef GP_COMP_RESET
#include "gpReset.h"
#endif //GP_COMP_RESET

#ifdef GP_COMP_UNIT_TEST
#include "gpUnitTest.h"
#endif //GP_COMP_UNIT_TEST


#ifdef GP_COMP_COM
#include "gpCom.h"
#endif //GP_COMP_COM

#ifdef GP_COMP_LOG
#include "gpLog.h"
#endif //GP_COMP_LOG

#ifdef GP_COMP_TEST
#include "gpTest.h"
#endif //GP_COMP_TEST


#ifdef GP_COMP_GPHAL
#include "gpHal.h"
#endif //GP_COMP_GPHAL

#ifdef GP_COMP_BLE
#include "gpBle.h"
#endif //GP_COMP_BLE

#ifdef GP_COMP_HCI
#include "gpHci.h"
#endif //GP_COMP_HCI

#ifdef GP_COMP_MACCORE
#include "gpMacCore.h"
#endif //GP_COMP_MACCORE
#ifdef GP_COMP_MACDISPATCHER
#include "gpMacDispatcher.h"
#endif // GP_COMP_MACDISPATCHER

#ifdef GP_COMP_RANDOM
#include "gpRandom.h"
#endif //GP_COMP_RANDOM
#ifdef GP_COMP_ENCRYPTION
#include "gpEncryption.h"
#endif //GP_COMP_ENCRYPTION


#ifdef GP_COMP_POOLMEM
#include "gpPoolMem.h"
#endif //GP_COMP_POOLMEM

#ifdef GP_COMP_PD
#include "gpPd.h"
#endif //GP_COMP_PD
#ifdef GP_COMP_PAD
#include "gpPad.h"
#endif //GP_COMP_PAD

#ifdef GP_COMP_NVM
#include "gpNvm.h"
#endif //GP_COMP_NVM

#ifdef GP_COMP_BSPRECFG
#include "gpBspReCfg.h"
#endif //GP_COMP_STAT

#ifdef GP_COMP_KEYSCAN
#include "gpKeyScan.h"
#endif //GP_COMP_KEYSCAN



#ifdef GP_COMP_TXMONITOR
#include "gpTxMonitor.h"
#endif //GP_COMP_TXMONITOR

#ifdef GP_COMP_RANDOM
#include "gpRandom.h"
#endif /* GP_COMP_RANDOM */

#ifdef GP_COMP_VERSION
#include "gpVersion.h"
#endif //GP_COMP_VERSION


#include "gpAssert.h"
#ifdef GP_COMP_RXARBITER
#include "gpRxArbiter.h"
#endif //GP_COMP_RXARBITER

#ifdef GP_COMP_RFCHANNEL
#include "gpRFChannel.h"
#endif //GP_COMP_RFCHANNEL



/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static void (*BaseComps_PreReInitCallback)(void) = NULL;
static void (*BaseComps_PostReInitCallback)(void) = NULL;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpBaseComps_StackDeInit(void)
{

//--------------------------
// the order here is the reverse order of gpBaseComps_StackInit
//--------------------------
#ifdef GP_COMP_BSPRECFG
    gpBspReCfg_DeInit();
#endif //GP_COMP_BSPRECFG

    gpBaseComps_MacDeInit();

#ifdef GP_COMP_PAD
    gpPad_Init(); // clears static state
#endif //GP_COMP_PAD

#ifdef GP_COMP_PD
    gpPd_DeInit(); // clears static state and free pending data
#endif //GP_COMP_PD

#ifdef GP_COMP_RXARBITER
    gpRxArbiter_DeInit();
#endif //GP_COMP_RXARBITER
#ifdef GP_COMP_NVM
    gpNvm_DeInit();
#endif //GP_COMP_NVM


#if defined(GP_COMP_COM)
    gpCom_DeInit();
#endif //GP_COMP_COM

#if defined(GP_COMP_SCHED)
    gpSched_DeInit();
#endif //GP_COMP_SCHED

#if !defined(GP_DIVERSITY_GPHAL_XP4002)
    hal_DeInit();
#endif
}


void gpBaseComps_RegisterPreReInitCallback(void (*callback)(void))
{
    BaseComps_PreReInitCallback = callback;
}

void gpBaseComps_RegisterPostReInitCallback(void (*callback)(void))
{
    BaseComps_PostReInitCallback = callback;
}

void gpBaseComps_StackReInit(void)
{
    if(BaseComps_PreReInitCallback) {
        BaseComps_PreReInitCallback();
    }
    gpBaseComps_StackDeInit();
    hal_Init();
    gpBaseComps_StackInit();
    if(BaseComps_PostReInitCallback) {
        BaseComps_PostReInitCallback();
    }
}

void gpBaseComps_MacDeInit(void)
{
#ifdef GP_COMP_MACDISPATCHER
    gpMacDispatcher_DeInit();
#endif //GP_COMP_MACDISPATCHER

#ifdef GP_COMP_MACCORE
    gpMacCore_DeInit();
#endif //GP_COMP_MACCORE
}

#ifdef GP_DIVERSITY_LINUXKERNEL
void gpBaseComps_StackDeInitKernel(void)
{
#if defined(GP_COMP_SCHED)
    gpSched_ExitKernel();
#endif
#if defined(GP_COMP_COM)
    /* perform gpCom Deinit after gpSched_ExitKernel performed gpBaseComps_StackDeInit() */
    gpCom_DeInitKernel();
#endif

}
#endif // ifdef GP_DIVERSITY_LINUXKERNEL
