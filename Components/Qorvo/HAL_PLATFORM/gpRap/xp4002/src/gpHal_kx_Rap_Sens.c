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


void rap_sens_gp_adc_init(void)
{
    rap_h_log_debug("rap_sens_gp_adc_init");
    rap_cp_ctrl_enable(RAP_CP_ID_GP_ADC);
    GP_WB_WRITE_GP_ADC_LDO_PUP(1);
    GP_WB_WRITE_GP_ADC_LDO_BLEED(1);
    GP_WB_WRITE_GP_ADC_VCMBUF_PUP(1);
    rap_h_wait_us(10);
    GP_WB_WRITE_GP_ADC_VREF_PUP(1);
    rap_h_wait_us(10);
    GP_WB_WRITE_GP_ADC_PUP(1);
}

void rap_sens_gp_adc_deinit(void)
{
    rap_h_log_debug("rap_sens_gp_adc_deinit");
    GP_WB_WRITE_GP_ADC_PUP(0);
    GP_WB_WRITE_GP_ADC_VCMBUF_PUP(0);
    GP_WB_WRITE_GP_ADC_LDO_BLEED(0);
    GP_WB_WRITE_GP_ADC_VREF_PUP(0);
    GP_WB_WRITE_GP_ADC_LDO_PUP(0);
    rap_cp_ctrl_disable(RAP_CP_ID_GP_ADC);
}

Int32 rap_sens_gp_adc_code_to_uv(UInt16 res_code, Bool mode_8bit, Bool se_not_diff, Bool high_vrange)
{
    Int64 uv;
    Int32 uv32;
    UInt32 gain;
    Int32 offset;
    UInt32 cal_nvm_ptr;
    uv = res_code;
    if (mode_8bit)
    {
        uv = uv * 8;
    }
    if ( !(se_not_diff))
    {
        uv = uv - 1024;
    }

    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();

    switch (se_not_diff*2 + high_vrange)
    {

        case 0*2 + 0:
        {
            gain = GP_WB_READ_CAL_NVM_GP_ADC_LSB_LOW_DIFF_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_LOW_DIFF_UV(cal_nvm_ptr);

            break;
        }
        case 0*2 + 1:
        {
            gain = GP_WB_READ_CAL_NVM_GP_ADC_LSB_HIGH_DIFF_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_HIGH_DIFF_UV(cal_nvm_ptr);

            break;
        }
        case 1*2 + 0:
        {
            gain = GP_WB_READ_CAL_NVM_GP_ADC_LSB_LOW_SE_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_LOW_SE_UV(cal_nvm_ptr);

            break;
        }
        case 1*2 + 1:
        {
            gain = GP_WB_READ_CAL_NVM_GP_ADC_LSB_HIGH_SE_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_GP_ADC_OFFSET_HIGH_SE_UV(cal_nvm_ptr);

            break;
        }
        default:
        {
            gain = 0;
            offset = 0;
            break;
        }
    }

    uv = (uv * gain) / (1<<16);
    uv = uv - offset;

    uv32 = uv;
    return uv32;
}


void rap_sens_hr_adc_init(void)
{
    rap_h_log_debug("rap_sens_hr_adc_init");
    rap_cp_ctrl_enable(RAP_CP_ID_HR_ADC);
    GP_WB_WRITE_HR_ADC_BIAS_LDO_PUP(1);
    GP_WB_WRITE_HR_ADC_LDO_PUP(1);
    rap_h_wait_us(2);
    GP_WB_WRITE_HR_ADC_MUX_PUP(1);
    GP_WB_WRITE_HR_ADC_PUP(1);
    GP_WB_WRITE_HR_ADC_REF_BUFFER_PUP(1);
    rap_h_wait_us(2);
}

void rap_sens_hr_adc_deinit(void)
{
    rap_h_log_debug("rap_sens_hr_adc_deinit");
    GP_WB_WRITE_HR_ADC_BIAS_LDO_PUP(0);
    GP_WB_WRITE_HR_ADC_LDO_PUP(0);
    GP_WB_WRITE_HR_ADC_MUX_PUP(0);
    GP_WB_WRITE_HR_ADC_PUP(0);
    GP_WB_WRITE_HR_ADC_REF_BUFFER_PUP(0);
    rap_cp_ctrl_disable(RAP_CP_ID_HR_ADC);
}

Int32 rap_sens_hr_adc_code_to_uv(UInt16 res_code, UInt8 gain)
{
    Int64 uv;
    Int32 uv32;
    UInt32 lsb_gain;
    Int32 offset;
    UInt32 cal_nvm_ptr;
    uv = res_code;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();

    switch (gain)
    {

        case 0:
        {
            lsb_gain = GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN0P25_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN0P25_UV(cal_nvm_ptr);

            break;
        }
        case 1:
        {
            lsb_gain = GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN1_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN1_UV(cal_nvm_ptr);

            break;
        }
        case 2:
        {
            lsb_gain = GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN4_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN4_UV(cal_nvm_ptr);

            break;
        }
        case 3:
        {
            lsb_gain = GP_WB_READ_CAL_NVM_HR_ADC_LSB_GAIN8_UV(cal_nvm_ptr);
            offset = GP_WB_READ_CAL_NVM_HR_ADC_OFFSET_GAIN8_UV(cal_nvm_ptr);

            break;
        }
        default:
        {
            lsb_gain = 0;
            offset = 0;
            break;
        }
    }

    uv = ((uv-(1<<15)) * lsb_gain) >> 16;
    uv = uv - offset;

    uv32 = uv;
    return uv32;
}

