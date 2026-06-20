/*
 * Copyright (c) 2022, Qorvo Inc
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

#ifndef _GPHAL_KX_DEFS_H_
#define _GPHAL_KX_DEFS_H_

#include "gpHal_kx_enum_manual.h"
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GPHAL_CHIP_ID_XP4002           18
#ifndef GP_HAL_EXPECTED_CHIP_ID
#define GP_HAL_EXPECTED_CHIP_ID     GPHAL_CHIP_ID_XP4002
#endif

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS
#define GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS 16
#endif

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_SLAVE_CONNECTIONS
#define GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_SLAVE_CONNECTIONS 1
#endif

#if GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS > 16
#error We only support up to 16 connections
#endif

#if GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_SLAVE_CONNECTIONS > GP_DIVERSITY_BLE_MAX_NR_OF_SUPPORTED_CONNECTIONS
#error Number of slave connections cannot be bigger than the total number of connections
#endif

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_FILTER_ACCEPT_LIST_ENTRIES
#define GP_DIVERSITY_BLE_MAX_NR_OF_FILTER_ACCEPT_LIST_ENTRIES    33
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_FILTER_ACCEPT_LIST_ENTRIES

#ifndef GP_DIVERSITY_BLE_MAX_NR_OF_RESOLVINGLIST_ENTRIES
#define GP_DIVERSITY_BLE_MAX_NR_OF_RESOLVINGLIST_ENTRIES    4
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_RESOLVINGLIST_ENTRIES

#if GP_DIVERSITY_BLE_MAX_NR_OF_RESOLVINGLIST_ENTRIES > 4
#error We only support up to 4 resolving list entries (see SDB004-315).
#endif //GP_DIVERSITY_BLE_MAX_NR_OF_RESOLVINGLIST_ENTRIES

#include "hal.h"
#include "sw_reg_halprop_basic.h"
#include "sw_reg_halprop2.h"
#include "gpHal_PortingFixes.h" /* this needs to be after the halprop includes, because it redefines some constant */

#include "gpHal_kx_enum.h"
#include "sw_reg_enum.h"

#include "gpHal_kx_Pbm.h"
#include "gpHal_kx_Rap.h"
#include "gpHal_kx_public.h"
#include "sys_ahb_bus_ids.h"

#include "gpHal_kx_hw_constants.h"
#include "gpHal_kx_mm_manual.h"

#include "gpHal_OffsetChecks.h"
#include "gpHal_kx_Rt.h"

#endif //_GPHAL_KX_DEFS_H_
