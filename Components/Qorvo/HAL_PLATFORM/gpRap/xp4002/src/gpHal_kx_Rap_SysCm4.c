/*
 * Copyright (c) 2023, Qorvo Inc
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


void rap_sys_cm4_reset_early(void)
{
    GP_WB_WRITE_MSI_MEM_ACCESS_TIMEOUT(0);
    GP_WB_WRITE_BIST_TSENS_DEM_SEL_INPUT(1);
    GP_WB_WRITE_PMUD_CLR_VCORE_LPR_RDY_ON_SOFT_POR(0);
    GP_WB_WRITE_PMUD_GLOBAL_LDO_FASTSTARTUP_EN(1);
    GP_WB_WRITE_DCDC_SW_WIDTH_DURING_CAL(3);
}

void rap_sys_cm4_backup_late(void)
{
}

void rap_sys_cm4_hal_init(void)
{
    GP_WB_WRITE_PMUD_RC64K_USE_VCORE(1);
    GP_WB_WRITE_PMUD_OVD_EN(0);
    GP_WB_WRITE_PMUD_VMT_1K(1);
    GP_WB_WRITE_PMUD_DCDC_RET_REDUCE_CLOCK_DIS(1);
    GP_WB_WRITE_PMUD_VCORE_LPR_LOWBIAS(1);
    GP_WB_WRITE_HR_ADC_OVERVOLTAGE_DET_THRESHOLD(44);
    GP_WB_WRITE_PMUD_DCDC_SW_WIDTH_RET(3);
    GP_WB_WRITE_DCDC_S1_SKIP_TX(1);
    GP_WB_WRITE_DCDC_S2_SKIP_TX(1);
}

Bool rap_sys_cm4_is_tx20(void)
{
    UInt16 ret;
    ret = GP_WB_READ_STANDBY_VERSION();
    return  !(ret & 1);
}


void rap_sys_cm4_cal_nvm_load_defaults(void)
{

    GP_WB_WRITE_CAL_NVM_PMUD_VCORE_LDO_REFBITS(RAP_H_CAL_NVM_RAM_PTR, 10);

}
void rap_sys_cm4_cal_nvm_restore_ao(void)
{
    UInt32 cal_nvm_ptr;

    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();


    rap_h_memcpy32(GP_WB_PMUD_RC64K_TUNE_F_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_PMUD_RC64K_TUNE_F_ADDRESS, 4);


    rap_h_memcpy32(GP_WB_SENS_AO_SENS_TBUS_ADDR_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_SENS_AO_SENS_TBUS_ADDR_ADDRESS, 1);

}

void rap_sys_cm4_cal_nvm_restore(void)
{
    UInt32 cal_nvm_ptr;

    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();


    rap_h_memcpy32(GP_WB_XO32M_CLOADN_SEL_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_XO32M_CLOADN_SEL_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_BBPLL_IREF_REFBITS_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_BBPLL_IREF_REFBITS_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_DCDC_REFBITS1_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_DCDC_REFBITS1_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_STANDBY_BOD_TUNE_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_STANDBY_BOD_TUNE_ADDRESS, 1);


    rap_h_memcpy32(GP_WB_BIST_IREF_REFBITS_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_BIST_IREF_REFBITS_ADDRESS, 2);

    rap_h_memcpy32(GP_WB_GP_ADC_IREF_REFBITS_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_GP_ADC_IREF_REFBITS_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_HR_ADC_LDO_REFBITS_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_HR_ADC_LDO_REFBITS_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_USB_CTRL_DRIVER_FALL_TUNE_ADDRESS, cal_nvm_ptr + GP_WB_CAL_NVM_USB_CTRL_DRIVER_FALL_TUNE_ADDRESS, 1);


}

void rap_sys_cm4_temp_change(Int32 temp_deg)
{
    UInt32 cal_nvm_ptr;
    UInt32 pmud_vddram_lpr_tune_in_active;
    Int32 pmud_vcore_ldo_temp_offset = 0;
    Int32 dcdc_refbits2;
    UInt8 active_power_mode;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    rap_h_log_debug("rap_sys_cm4_temp_change(%ld C)", temp_deg);
    GP_WB_WRITE_CAL_RAM_TEMP_DEG(temp_deg);

    if (temp_deg > -20)
    {
        GP_WB_WRITE_PMUD_PMUA_USE_VCORE(1);
    }
    else
    {
        GP_WB_WRITE_PMUD_PMUA_USE_VCORE(0);
    }

    if (temp_deg > 55)
    {
        pmud_vddram_lpr_tune_in_active = 0;
    }
    else
    {
        pmud_vddram_lpr_tune_in_active = GP_WB_READ_CAL_NVM_PMUD_VDDRAM_LPR_TUNE_IN_ACTIVE(cal_nvm_ptr);
    }
    GP_WB_WRITE_PMUD_VDDRAM_LPR_TUNE_IN_ACTIVE(pmud_vddram_lpr_tune_in_active);

    dcdc_refbits2 = GP_WB_READ_CAL_NVM_DCDC_REFBITS2(cal_nvm_ptr);
    if (temp_deg < 15)
    {
        pmud_vcore_ldo_temp_offset = (25 - temp_deg) / 8;
        dcdc_refbits2 = dcdc_refbits2 + pmud_vcore_ldo_temp_offset;
        if (dcdc_refbits2 > 15)
        {
            dcdc_refbits2 = 15;
        }
    }
    GP_WB_WRITE_SE_CAL_RAM_PMUD_VCORE_LDO_TEMP_OFFSET(RAP_H_SE_CAL_RAM_PTR, pmud_vcore_ldo_temp_offset);

    GP_WB_WRITE_DCDC_REFBITS2(dcdc_refbits2);

    GP_WB_WRITE_SE_CAL_RAM_PMUD_GLOBAL_LDO_TEMP_OFFSET(RAP_H_SE_CAL_RAM_PTR, 0);

    active_power_mode = GP_WB_READ_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR);
    rap_pwr_temp_batt_update(active_power_mode);

}

void rap_sys_cm4_batt_change(UInt32 vbatt_uv)
{
    UInt32 cal_nvm_ptr;
    Int32 dcdc_refbits1;
    UInt32 dcdc_skip;
    UInt8 active_power_mode;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    rap_h_log_debug("rap_sys_cm4_batt_change(%ld uV)", vbatt_uv);
    GP_WB_WRITE_CAL_RAM_VBATT_UV(vbatt_uv);

    rap_dcdc_rm_bits();

    dcdc_refbits1 = GP_WB_READ_CAL_NVM_DCDC_REFBITS1(cal_nvm_ptr);

    if (vbatt_uv < 2200000)
    {
        GP_WB_WRITE_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, 0);
        GP_WB_WRITE_SE_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, 0);
        dcdc_skip = 1;
    }
    else if (vbatt_uv < 2500000)
    {
        GP_WB_WRITE_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, -2);
        GP_WB_WRITE_SE_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, -1);
        dcdc_refbits1 = dcdc_refbits1 + 1;
        dcdc_skip = 0;
    }
    else
    {
        GP_WB_WRITE_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, -2);
        GP_WB_WRITE_SE_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR, -2);
        dcdc_skip = 0;
    }

    if (dcdc_refbits1 > 15)
    {
        dcdc_refbits1 = 15;
    }
    GP_WB_WRITE_DCDC_REFBITS1(dcdc_refbits1);

    if (dcdc_skip == 0)
    {
        GP_WB_WRITE_DCDC_S1_SKIP(0);
        GP_WB_WRITE_DCDC_S2_SKIP(0);
    }

    active_power_mode = GP_WB_READ_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR);
    rap_pwr_temp_batt_update(active_power_mode);

    GP_WB_WRITE_DCDC_S1_SKIP(dcdc_skip);
    GP_WB_WRITE_DCDC_S2_SKIP(dcdc_skip);

}

