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


#ifndef _GPHAL_KX_RAP_NVM_H_
#define _GPHAL_KX_RAP_NVM_H_


void rap_nvm_enable_mri(void);

void rap_nvm_disable_mri(void);

void rap_nvm_por(UInt16 por_duration_us);

void rap_nvm_recall(void);

void rap_nvm_write_config(UInt32 xaddr);

void rap_nvm_read_config(UInt32 xaddr);

void rap_nvm_enable_testmode(void);

void rap_nvm_disable_testmode(void);

void rap_nvm_wait_for_idle(void);

void rap_nvm_write(UInt32 addr, Bool set_addr, Bool auto_increment_enable, Bool write_enable);


void rap_nvm_write_eotp(UInt32 addr);

void rap_nvm_destroy_eotp(UInt32 addr);

void rap_nvm_read(UInt32 addr, Bool set_addr, Bool auto_increment_enable);

Bool rap_nvm_check_tmc_register_access(UInt32 addr, Bool testmode_enabled);

void rap_nvm_init(void);


#endif //_GPHAL_KX_RAP_NVM_H_
