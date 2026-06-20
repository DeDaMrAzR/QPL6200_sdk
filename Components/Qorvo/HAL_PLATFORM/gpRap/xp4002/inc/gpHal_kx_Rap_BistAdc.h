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


#ifndef _GPHAL_KX_RAP_BISTADC_H_
#define _GPHAL_KX_RAP_BISTADC_H_


#define ADC_TIMED_OUT (-404)

void rap_bist_adc_init(void);

void rap_bist_adc_deinit(void);

void rap_bist_adc_config(UInt8 mode, Bool low, UInt32 t_sample_us, UInt8 n_avg_exp);

void rap_bist_adc_config_mux(UInt8 adc_p, UInt8 adc_n);

void rap_bist_adc_enable(void);

void rap_bist_adc_disable(void);

Int32 rap_bist_adc_single_measurement(void);

Int32 rap_bist_adc_measure(void);

Int32 rap_bist_adc_to_uv(Int32 adc_result, UInt8 mode, Bool low);

Int32 rap_bist_adc_measure_uv(void);

void rap_bist_adc_batmon_init(void);

void rap_bist_adc_batmon_deinit(void);


#endif //_GPHAL_KX_RAP_BISTADC_H_
