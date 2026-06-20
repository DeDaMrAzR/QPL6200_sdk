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


#ifndef _GPHAL_KX_RAP_H_
#define _GPHAL_KX_RAP_H_

// If the build is not targetting the SEUC, put in SYSRAM and use values from rap.h
// This is for non SEUC builds or minimal SEUC builds where the APPUC is in control
#define RAP_H_PMU_CTRL_PTR      GP_MM_ADDR_FROM_SYS(0x400004F0)
#define RAP_H_SE_CAL_RAM_PTR    GP_MM_ADDR_FROM_SYS(0x40000050)

// copied from rap.h:
#define RAP_H_CAL_NVM_RAM_PTR   GP_MM_ADDR_FROM_SYS(0x40000500)
#define RAP_H_CAL_NVM_RRAM_PTR  GP_MM_ADDR_FROM_SYS(0x10200200)

// copied and adapted from rap.h:
ALWAYS_INLINE UInt32 rap_h_get_cal_nvm_ptr(void)
{
    return RAP_H_CAL_NVM_RRAM_PTR; // use a compile time constant pointer
}

// copied and adapted from rap.h:
ALWAYS_INLINE UInt32 ak_rap_h_get_cal_nvm_ptr(void)
{
    return RAP_H_CAL_NVM_RRAM_PTR + 0x100; // use a compile time constant pointer
}

// generated rap files:
#include "gpHal_kx_Rap_Bbpll.h"
#include "gpHal_kx_Rap_Dcdc.h"
#include "gpHal_kx_Rap_nvm_def.h"
#include "gpHal_kx_Rap_nvm.h"
#include "gpHal_kx_Rap_Pmu.h"
#include "gpHal_kx_Rap_Pwr_gen.h"
#include "gpHal_kx_Rap_Rt.h"
#include "gpHal_kx_Rap_Rx.h"
#include "gpHal_kx_Rap_Rx_Constants.h"
#include "gpHal_kx_Rap_Tx.h"
#include "gpHal_kx_Rap_Tx_Constants.h"
#include "gpHal_kx_Rap_Sx.h"
#include "gpHal_kx_Rap_SysCm4.h"
#include "gpHal_kx_Rap_SysSeuc.h"
#include "gpHal_kx_Rap_SysAk.h"
#include "gpHal_kx_Rap_XO32M.h"
#include "gpHal_kx_Rap_TSens.h"
#include "gpHal_kx_TimerXL_def.h"

// non-generated rap files:
#include "rap/rap_pwr.h"
#include "rap/rap_appuc.h"

#define rap_h_wait_us HAL_WAIT_US

ALWAYS_INLINE void rap_h_memcpy32(UInt32 dst, UInt32 src, UInt32 n_words)
{
    UInt32* dst_p = (UInt32*)(dst);
    UInt32* src_p = (UInt32*)(src);
    for(UInt32 i = 0; i < n_words; ++i)
        dst_p[i] = src_p[i];
}

#define rap_h_assert_dev(x)  GP_ASSERT_DEV_INT(x)
#define rap_h_assert_sys(x)  GP_ASSERT_SYSTEM(x)

#define rap_h_log(n, a, ...)
#define rap_h_log_error(a, ...)
#define rap_h_log_warn(a, ...)
#define rap_h_log_info(a, ...)
#define rap_h_log_debug(a, ...)

#endif //_GPHAL_KX_RAP_H_
