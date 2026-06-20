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


#ifndef _GPHAL_KX_RAP_DCDC_H_
#define _GPHAL_KX_RAP_DCDC_H_


void rap_dcdc_rm_bits(void);

void rap_dcdc_set_rm(void);

void rap_dcdc_rc_tune_ton_dac_calibration(void);

void rap_dcdc_timerxl_setup(UInt8 index);

void rap_dcdc_y1_y2_calibration(Bool ehps, UInt8 index);

void rap_dcdc_zcd_calibration(void);

void rap_dcdc_init(void);

void rap_dcdc_config_active(Bool ehps);

void rap_dcdc_enable_ehps(void);

void rap_dcdc_enable_emps(void);

void rap_dcdc_enable_ret(void);

void rap_dcdc_disable(void);

void rap_dcdc_validate_config(void);



#endif //_GPHAL_KX_RAP_DCDC_H_
