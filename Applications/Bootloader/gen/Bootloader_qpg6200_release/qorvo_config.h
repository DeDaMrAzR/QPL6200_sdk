/*
 * Copyright (c) 2020, Qorvo Inc
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

/** @file "qorvo_config.h"
 *
 */

#ifndef _QORVO_CONFIG_H_
#define _QORVO_CONFIG_H_

/*
 * Version info
 */

#define GP_CHANGELIST                                      0
#define GP_VERSIONINFO_APP                                 Bootloader_qpg6200_release
#define GP_VERSIONINFO_BASE_COMPS                          0,0,0,0
#define GP_VERSIONINFO_BLE_COMPS                           0,0,0,0
#define GP_VERSIONINFO_DATE                                2026-03-11
#define GP_VERSIONINFO_GLOBAL_VERSION                      3,2,0,2
#define GP_VERSIONINFO_GLOBAL_VERSION_OVERWRITE            3,2,0,2
#define GP_VERSIONINFO_HOST                                UNKNOWN
#define GP_VERSIONINFO_PROJECT                             P346_IoT_SDK
#define GP_VERSIONINFO_USER                                UNKNOWN@UNKNOWN


/*
 * Component: gpAssert
 */

/* Choose exit as default assert action */
#define GP_DIVERSITY_ASSERT_ACTION_EXIT

/* Choose to do nothing as default assert reporting */
#define GP_DIVERSITY_ASSERT_REPORTING_NOTHING


/*
 * Component: gpBsp
 */

/* Contains filename of BSP header file to include */
#define GP_BSP_FILENAME                                    "gpBsp_QPG6200_QFN32_DK_LED_PB_10dBm.h"


/*
 * Component: gpSched
 */

/* Callback after every main loop iteration. */
#define GP_SCHED_NR_OF_IDLE_CALLBACKS                      0


/*
 * Component: halCortexM4
 */

/* Use application header */
#define GP_DIVERSITY_APPUC_FW_HEADER

/* Use simplified bootloader IVT implementation */
#define GP_HALCORTEXM4_DIVERSITY_BOOTLOADER_IVT


#include "qorvo_internals.h"

#endif //_QORVO_CONFIG_H_
