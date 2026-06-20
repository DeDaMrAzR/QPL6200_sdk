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


#ifndef _GPHAL_KX_RAP_SENS_H_
#define _GPHAL_KX_RAP_SENS_H_


void rap_sens_gp_adc_init(void);

void rap_sens_gp_adc_deinit(void);

Int32 rap_sens_gp_adc_code_to_uv(UInt16 res_code, Bool mode_8bit, Bool se_not_diff, Bool high_vrange);


void rap_sens_hr_adc_init(void);

void rap_sens_hr_adc_deinit(void);

Int32 rap_sens_hr_adc_code_to_uv(UInt16 res_code, UInt8 gain);


#endif //_GPHAL_KX_RAP_SENS_H_
