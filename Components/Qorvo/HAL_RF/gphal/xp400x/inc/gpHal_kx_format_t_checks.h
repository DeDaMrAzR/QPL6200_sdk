/*
 * Copyright (c) 2017, Qorvo Inc
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

#ifndef _GPHAL_KX_FORMAT_T_CHECKS_H_
#define _GPHAL_KX_FORMAT_T_CHECKS_H_

/*****************************************************************************
 *                    Include Definitions
 *****************************************************************************/

#include "gpHal.h"
#include "gpHal_DEFS.h"

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

// Make sure that the event information inside the zb_format_t is aligned with the RT event info structure
GP_COMPILE_TIME_VERIFY(GP_WB_RT_EVENT_PRIORITY_ADDRESS                  == GP_WB_ZB_FORMAT_T_PRIORITY_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_RT_EVENT_SUSPEND_EVENT_ADDRESS             == GP_WB_ZB_FORMAT_T_SUSPEND_EVENT_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_RT_EVENT_EXTENDED_PRIO_EN_ADDRESS          == GP_WB_ZB_FORMAT_T_EXTENDED_PRIO_EN_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_RT_EVENT_NR_CONSEC_SKIPPED_EVENTS_ADDRESS  == GP_WB_ZB_FORMAT_T_NR_CONSEC_SKIPPED_EVENTS_ADDRESS);

// Make sure common fields between BLE and ZB format_t are at the same location
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_TIMESTAMP_ADDRESS      == GP_WB_BLE_FORMAT_T_TIMESTAMP_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_FRAME_LEN_ADDRESS      == GP_WB_BLE_FORMAT_T_FRAME_LEN_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_RETURN_CODE_ADDRESS    == GP_WB_BLE_FORMAT_T_RETURN_CODE_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_TX_POWER_ADDRESS       == GP_WB_BLE_FORMAT_T_TX_POWER_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_MODE_CTRL_ADDRESS      == GP_WB_BLE_FORMAT_T_MODE_CTRL_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_EXT_ANTENNA_ADDRESS    == GP_WB_BLE_FORMAT_T_EXT_ANTENNA_ADDRESS);
GP_COMPILE_TIME_VERIFY(GP_WB_ZB_FORMAT_T_ANTENNA_ADDRESS        == GP_WB_BLE_FORMAT_T_ANTENNA_ADDRESS);

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#endif //_GPHAL_KX_FORMAT_T_CHECKS_H_
