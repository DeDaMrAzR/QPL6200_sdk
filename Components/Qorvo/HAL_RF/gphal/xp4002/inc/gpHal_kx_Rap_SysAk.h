/*
 * Copyright (c) 2024, Qorvo Inc
 *
 * !!! This is a generated file. !!!
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


#ifndef _GPHAL_KX_RAP_SYSAK_H_
#define _GPHAL_KX_RAP_SYSAK_H_


Bool ak_rap_sys_is_tx20(void);

void ak_rap_sys_reset_early(void);

void ak_rap_sys_backup_late(void);

void ak_rap_sys_hal_init(void);

void ak_rap_sys_temp_change(Int32 temp_deg);

void ak_rap_sys_cal_nvm_load_defaults(void);

void ak_rap_sys_cal_nvm_restore(void);



#endif //_GPHAL_KX_RAP_SYSAK_H_
