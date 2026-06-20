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
 *
 */

#ifndef _QV_RT_NRT_PLATFORM_H_
#define _QV_RT_NRT_PLATFORM_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "sw_reg_halprop_basic.h"
#include "sw_reg_halprop2.h"
#include "sw_reg_types.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define RT_EVENT_HANDLE_TO_OFFSET(event_handle) (FROM_RT_ADDR(GP_WB_READ_RT_EVENT_BASE_PTR()) + GP_WB_RT_EVENT_SIZE * (event_handle))
#define RT_ATTRIBUTE_SECTION_RAM_FUNC __attribute__((section(".ram_func")))

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

// We are reusing flag_d interrupt as iso_rx interrupt, but we cannot change the HW-generated regmap names
#define GP_WB_AK_IPC_SET_RT2X_ISO_RX_INTERRUPT() GP_WB_AK_IPC_SET_RT2X_FLAG_D_INTERRUPT()

// Some other overrules for macro's we cannot change in source
#define GP_WB_READ_AK_RT_ZB_RX_TIMESTAMP_OFFSET()           GP_WB_READ_RT_ZB_RX_TIMESTAMP_OFFSET()
#define GP_WB_READ_AK_RT_BLE_RX_TIMESTAMP_OFFSET()          GP_WB_READ_RT_BLE_RX_TIMESTAMP_OFFSET()
#define GP_WB_READ_AK_RT_BLE_HDR_RX_TIMESTAMP_OFFSET()      GP_WB_READ_RT_BLE_HDR_RX_TIMESTAMP_OFFSET()
#define GP_WB_READ_AK_RT_BLE_LR125_RX_TIMESTAMP_OFFSET()    GP_WB_READ_RT_BLE_LR125_RX_TIMESTAMP_OFFSET()
#define GP_WB_READ_AK_RT_BLE_LR500_RX_TIMESTAMP_OFFSET()    GP_WB_READ_RT_BLE_LR500_RX_TIMESTAMP_OFFSET()

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

#endif //_QV_RT_NRT_PLATFORM_H_
