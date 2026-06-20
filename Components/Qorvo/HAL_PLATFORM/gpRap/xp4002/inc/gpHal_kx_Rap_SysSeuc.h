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


#ifndef _GPHAL_KX_RAP_SYSSEUC_H_
#define _GPHAL_KX_RAP_SYSSEUC_H_


void rap_sys_seuc_reset_early(void);

void rap_sys_seuc_backup_late(void);

void rap_sys_seuc_hal_init(void);

Bool rap_sys_seuc_is_tx20(void);


void rap_sys_seuc_cal_nvm_load_defaults(void);
void rap_sys_seuc_cal_nvm_restore_ao(void);

void rap_sys_seuc_cal_nvm_restore(void);

void rap_sys_seuc_temp_change(Int32 temp_deg);

void rap_sys_seuc_batt_change(UInt32 vbatt_uv);


#endif //_GPHAL_KX_RAP_SYSSEUC_H_
