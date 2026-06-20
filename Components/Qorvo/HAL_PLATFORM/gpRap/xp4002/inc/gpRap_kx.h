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

#ifndef _GPRAP_KX_H_
#define _GPRAP_KX_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"

#include "gpHal_kx_Rap_SysCm4.h"
#include "gpHal_kx_Rap_Cp.h"

#define RAP_CP_ID_TESTBUS 0
#define RAP_CP_ID_GP_ADC  1
#define RAP_CP_ID_HR_ADC  2

#define rap_h_wait_us HAL_WAIT_US

#define rap_h_assert_dev(x) GP_ASSERT_DEV_INT(x)
#define rap_h_assert_sys(x) GP_ASSERT_SYSTEM(x)

#define rap_h_log(n, a, ...)
#define rap_h_log_error(a, ...)
#define rap_h_log_warn(a, ...)
#define rap_h_log_info(a, ...)
#define rap_h_log_debug(a, ...)

#define rap_h_atomic_on()  HAL_DISABLE_GLOBAL_INT()
#define rap_h_atomic_off() HAL_ENABLE_GLOBAL_INT()

#endif // _GPRAP_KX_H_
