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

#include "gpHal.h"
#if defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)
#include "hal.h"
#endif //defined(GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM)

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

//wrappers/stubs for RAP code
#define abs(a)              (((a) < 0) ? -(a) : (a))

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*
 * The actual RAP methods.
 */


#define ADC_TIMED_OUT (-404)

void rap_bist_adc_init(void)
{
    rap_h_log_debug("rap_bist_adc_init");
    rap_cp_ctrl_enable(RAP_CP_ID_TESTBUS);
    GP_WB_WRITE_BIST_ADC_LDO_BLEED(1);
    GP_WB_WRITE_BIST_ADC_LDO_PUP(1);
    GP_WB_WRITE_BIST_ADC_REF_PUP(1);
    GP_WB_WRITE_BIST_VCM_BUF_PUP(1);
}

void rap_bist_adc_deinit(void)
{
    rap_h_log_debug("rap_bist_adc_deinit");
    GP_WB_WRITE_BIST_VCM_BUF_PUP(0);
    GP_WB_WRITE_BIST_ADC_REF_PUP(0);
    GP_WB_WRITE_BIST_ADC_LDO_PUP(0);
    GP_WB_WRITE_BIST_ADC_LDO_BLEED(0);
    rap_cp_ctrl_disable(RAP_CP_ID_TESTBUS);
}

void rap_bist_adc_config(UInt8 mode, Bool low, UInt32 t_sample_us, UInt8 n_avg_exp)
{
    UInt32 sample_periods;
    UInt32 accu_count;
    UInt32 dummy;
    rap_h_log_debug("rap_bist_adc_config");
    GP_WB_WRITE_BIST_SCALER_LOW_VOLTAGE_RANGE(low);
    sample_periods = (64 * t_sample_us) - 1;
    rap_h_assert_sys(sample_periods<2048);
    GP_WB_WRITE_BIST_SCALER_WAIT_TIME(sample_periods);
    GP_WB_WRITE_CAL_RAM_BIST_ADC_MODE(mode);
    GP_WB_WRITE_BIST_ADC_SE_TO_DE_MODE(mode & 1);
    accu_count = (1<<(n_avg_exp));
    rap_h_assert_sys(accu_count<=512);
    GP_WB_WRITE_BIST_ADC_COUNT(accu_count);
    GP_WB_WRITE_BIST_ALLOW_ADC_RESULT_OVERWRITE(1);
    GP_WB_WRITE_BIST_SCALER_CK_DIV(0);
    GP_WB_WRITE_BIST_ADC_SKIP_SAMPLES(0);
    NOT_USED(dummy);
    dummy = GP_WB_READ_BIST_ADC_RESULT();
}

void rap_bist_adc_config_mux(UInt8 adc_p, UInt8 adc_n)
{
    rap_h_log_debug("rap_bist_adc_config_mux");
    rap_h_assert_sys(adc_p<8);
    rap_h_assert_sys(adc_n<8);
    GP_WB_WRITE_BIST_ADCP_MUX_PUP(0);
    GP_WB_WRITE_BIST_ADCN_MUX_PUP(0);
    GP_WB_WRITE_BIST_SEL_ADCP_IN(adc_p);
    GP_WB_WRITE_BIST_SEL_ADCN_IN(adc_n);
}

void rap_bist_adc_enable(void)
{
    rap_h_log_debug("rap_bist_adc_enable");
    GP_WB_WRITE_BIST_ADCP_MUX_PUP(1);
    GP_WB_WRITE_BIST_ADCN_MUX_PUP(1);
    GP_WB_WRITE_BIST_ADC_PUP(1);
    GP_WB_WRITE_BIST_SCALER_CONTROL_ENABLE(1);
}

void rap_bist_adc_disable(void)
{
    rap_h_log_debug("rap_bist_adc_disable");
    GP_WB_WRITE_BIST_ADC_PUP(0);
    GP_WB_WRITE_BIST_SCALER_CONTROL_ENABLE(0);
    GP_WB_WRITE_BIST_ADCP_MUX_PUP(0);
    GP_WB_WRITE_BIST_ADCN_MUX_PUP(0);
}

Int32 rap_bist_adc_single_measurement(void)
{
    Int32 ret;
    Bool timedout;
    rap_h_log_debug("rap_bist_adc_single_measurement");

    rap_bist_adc_enable();

    GP_WB_BIST_ADC_START();
    GP_DO_WHILE_TIMEOUT((GP_WB_READ_BIST_ADC_ACCU_RESULT_VALID() != 1), 64000, &timedout);
    if (timedout)
    {
        rap_h_log_debug("ADC_TIMED_OUT");
        ret = ADC_TIMED_OUT;
    }
    else
    {
        ret = GP_WB_READ_BIST_ADC_ACCU_RESULT();
    }

    rap_bist_adc_disable();

    return ret;
}

Int32 rap_bist_adc_measure(void)
{
    UInt8 adc_p;
    UInt8 adc_n;
    Int32 adc_result_1;
    Int32 adc_result_2 = 0;
    Int32 n_samples;
    Int32 adc_result;
    UInt8 mode;
    rap_h_log_debug("rap_bist_adc_measure");
    mode = GP_WB_READ_CAL_RAM_BIST_ADC_MODE();
    n_samples = GP_WB_READ_BIST_ADC_COUNT();
    adc_result_1 = rap_bist_adc_single_measurement();
    rap_h_log_debug("adc_result_1=%ld", adc_result_1);
    if (adc_result_1 == ADC_TIMED_OUT)
    {
        return ADC_TIMED_OUT;
    }
    if (mode == 0)
    {
        adc_p = GP_WB_READ_BIST_SEL_ADCP_IN();
        adc_n = GP_WB_READ_BIST_SEL_ADCN_IN();
        GP_WB_WRITE_BIST_SEL_ADCP_IN(adc_n);
        GP_WB_WRITE_BIST_SEL_ADCN_IN(adc_p);
        adc_result_2 = rap_bist_adc_single_measurement();
        rap_h_log_debug("adc_result_2=%ld", adc_result_2);
        if (adc_result_2 == ADC_TIMED_OUT)
        {
            return ADC_TIMED_OUT;
        }
        GP_WB_WRITE_BIST_SEL_ADCP_IN(adc_p);
        GP_WB_WRITE_BIST_SEL_ADCN_IN(adc_n);
        n_samples = n_samples * 2;
    }
    else if (mode == 1)
    {
        adc_p = GP_WB_READ_BIST_SEL_ADCP_IN();
        adc_n = GP_WB_READ_BIST_SEL_ADCN_IN();
        rap_h_assert_sys(adc_p == adc_n);
    }
    else if (mode == 2)
    {
        adc_result_2 = 1024*n_samples;
    }

    adc_result = 256*(adc_result_1 - adc_result_2) / n_samples;
    rap_h_log_debug("adc_result code = %ld", adc_result/256);
    return adc_result;
}

Int32 rap_bist_adc_to_uv(Int32 adc_result, UInt8 mode, Bool low)
{
    UInt32 bist_adc_lsb_uv = 0;
    Int32 bist_adc_offset_uv = 0;
    UInt32 cal_nvm_ptr;
    Int32 lsb_uv;
    rap_h_log_debug("rap_bist_adc_to_uv");
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    switch (mode)
    {
        case 0:
        {
            if (low)
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_LOW_DIFF_UV(cal_nvm_ptr);
            }
            else
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_HIGH_DIFF_UV(cal_nvm_ptr);
            }
            break;
        }
        case 1:
        {
            if (low)
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_LOW_SE_UV(cal_nvm_ptr);
                bist_adc_offset_uv = GP_WB_READ_CAL_NVM_BIST_ADC_OFFSET_LOW_SE_UV(cal_nvm_ptr);
            }
            else
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_HIGH_SE_UV(cal_nvm_ptr);
                bist_adc_offset_uv = GP_WB_READ_CAL_NVM_BIST_ADC_OFFSET_HIGH_SE_UV(cal_nvm_ptr);
            }
            break;
        }
        case 2:
        {
            if (low)
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_LOW_DIFF_UV(cal_nvm_ptr);
                bist_adc_offset_uv = GP_WB_READ_CAL_NVM_BIST_ADC_OFFSET_LOW_DIFF_UV(cal_nvm_ptr);
            }
            else
            {
                bist_adc_lsb_uv = GP_WB_READ_CAL_NVM_BIST_ADC_LSB_HIGH_DIFF_UV(cal_nvm_ptr);
                bist_adc_offset_uv = GP_WB_READ_CAL_NVM_BIST_ADC_OFFSET_HIGH_DIFF_UV(cal_nvm_ptr);
            }
            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            break;
        }
    }
    rap_h_log_debug("bist_adc_lsb_uv=%lu, bist_adc_offset_uv=%ld", bist_adc_lsb_uv, bist_adc_offset_uv);
    lsb_uv = bist_adc_lsb_uv / 65536;
    return ((adc_result * lsb_uv) / 256) - bist_adc_offset_uv;
}

Int32 rap_bist_adc_measure_uv(void)
{
    Int32 adc_result;
    Int32 adc_result_uv;
    UInt8 mode;
    Bool low;
    rap_h_log_debug("rap_bist_adc_measure_uv");
    low = GP_WB_READ_BIST_SCALER_LOW_VOLTAGE_RANGE();
    mode = GP_WB_READ_CAL_RAM_BIST_ADC_MODE();
    adc_result = rap_bist_adc_measure();
    if (adc_result == ADC_TIMED_OUT)
    {
        return ADC_TIMED_OUT;
    }
    adc_result_uv = rap_bist_adc_to_uv(adc_result, mode, low);
    rap_h_log_debug("adc_result adc_result_uv = %ld", adc_result_uv);
    return adc_result_uv;
}

void rap_bist_adc_batmon_init(void)
{
    GP_WB_WRITE_BIST_MON_LDO_BLEED(1);
    GP_WB_WRITE_BIST_MON_LDO_PUP(1);
    GP_WB_WRITE_BIST_TBUS_ADDR(30);
    GP_WB_WRITE_BIST_TBUS_COAX_EN(1);
    GP_WB_WRITE_BIST_TBUS_PROBE_EN(1);
}

void rap_bist_adc_batmon_deinit(void)
{
    GP_WB_WRITE_BIST_TBUS_PROBE_EN(0);
    GP_WB_WRITE_BIST_TBUS_COAX_EN(0);
    GP_WB_WRITE_BIST_TBUS_ADDR(0);
    GP_WB_WRITE_BIST_MON_LDO_PUP(0);
    GP_WB_WRITE_BIST_MON_LDO_BLEED(0);
}

