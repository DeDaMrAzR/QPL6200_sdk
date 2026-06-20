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


void rap_pwr_mps_to_hps(void)
{
    rap_pmu_config_hps();
    rap_xo32m_enable();
    rap_bbpll_resume();
    GP_WB_WRITE_STANDBY_CLK_MAIN_SELECT(GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF192M_CLOCK);
    GP_WB_WRITE_PMUD_RO64M_PUP_IN_ACTIVE(0);
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_HPS);
}

void rap_pwr_hps_to_mps(void)
{
    GP_WB_WRITE_PMUD_RO64M_PUP_IN_ACTIVE(1);
    rap_h_wait_us(10);
    GP_WB_WRITE_STANDBY_CLK_MAIN_SELECT(GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF64M_CLOCK);
    rap_bbpll_stop();
    rap_xo32m_disable();
    rap_pmu_config_mps();
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_MPS);
}

void rap_pwr_hps_to_ehps(void)
{
    rap_dcdc_enable_ehps();
    rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
}

void rap_pwr_ehps_to_hps(void)
{
    rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_HPS);
    rap_dcdc_disable();
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_HPS);
}

void rap_pwr_mps_to_emps(void)
{
    GP_WB_WRITE_DCDC_VFB2_COUNT_MAX(5);
    rap_dcdc_enable_emps();
    rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
}

void rap_pwr_emps_to_mps(void)
{
    rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_MPS);
    rap_dcdc_disable();
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, GP_WB_ENUM_ACTIVE_POWER_MODE_MPS);
}

void rap_pwr_set_active_power_mode(UInt8 active_power_mode)
{
    UInt8 current_active_power_mode;
    current_active_power_mode = GP_WB_READ_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR);

    switch (active_power_mode)
    {
        case GP_WB_ENUM_ACTIVE_POWER_MODE_HPS:
        {
            switch (current_active_power_mode)
            {
                case GP_WB_ENUM_ACTIVE_POWER_MODE_MPS:
                {
                    rap_pwr_mps_to_hps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
                {
                    rap_pwr_emps_to_mps();
                    rap_pwr_mps_to_hps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_HPS:
                {
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
                {
                    rap_pwr_ehps_to_hps();
                    break;
                }
                default:
                {
                    rap_h_assert_sys(0);
                    break;
                }
            }
            break;
        }
        case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
        {
            switch (current_active_power_mode)
            {
                case GP_WB_ENUM_ACTIVE_POWER_MODE_MPS:
                {
                    rap_pwr_mps_to_hps();
                    rap_pwr_hps_to_ehps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
                {
                    rap_pwr_emps_to_mps();
                    rap_pwr_mps_to_hps();
                    rap_pwr_hps_to_ehps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_HPS:
                {
                    rap_pwr_hps_to_ehps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
                {
                    break;
                }
                default:
                {
                    rap_h_assert_sys(0);
                    break;
                }
            }
            break;
        }
        case GP_WB_ENUM_ACTIVE_POWER_MODE_MPS:
        {
            switch (current_active_power_mode)
            {
                case GP_WB_ENUM_ACTIVE_POWER_MODE_MPS:
                {
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
                {
                    rap_pwr_emps_to_mps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_HPS:
                {
                    rap_pwr_hps_to_mps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
                {
                    rap_pwr_ehps_to_hps();
                    rap_pwr_hps_to_mps();
                    break;
                }
                default:
                {
                    rap_h_assert_sys(0);
                    break;
                }
            }
            break;
        }
        case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
        {
            switch (current_active_power_mode)
            {
                case GP_WB_ENUM_ACTIVE_POWER_MODE_MPS:
                {
                    rap_pwr_mps_to_emps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
                {
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_HPS:
                {
                    rap_pwr_hps_to_mps();
                    rap_pwr_mps_to_emps();
                    break;
                }
                case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
                {
                    rap_pwr_ehps_to_hps();
                    rap_pwr_hps_to_mps();
                    rap_pwr_mps_to_emps();
                    break;
                }
                default:
                {
                    rap_h_assert_sys(0);
                    break;
                }
            }
            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            break;
        }
    }
}

void rap_pwr_set_standby_power_mode(UInt8 standby_power_mode)
{
    GP_WB_WRITE_PMU_CTRL_STANDBY_POWER_MODE(RAP_H_PMU_CTRL_PTR, standby_power_mode);
}

void rap_pwr_temp_batt_update(UInt8 active_power_mode)
{
    UInt32 cal_nvm_ptr;
    Int8 pmud_vcore_ldo_refbits_ldo;
    Int8 pmud_global_ldo_refbits_ldo;
    Int8 pmud_vcore_ldo_refbits;
    Int8 pmud_global_ldo_refbits;
    Int8 pmud_vcore_ldo_temp_offset;
    Int8 pmud_global_ldo_temp_offset;
    Int8 pmud_vcore_ldo_dcdc_batt_offset;
    Int8 pmud_global_ldo_dcdc_batt_offset;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();

    pmud_vcore_ldo_refbits_ldo = GP_WB_READ_CAL_NVM_PMUD_VCORE_LDO_REFBITS(cal_nvm_ptr);
    pmud_global_ldo_refbits_ldo = GP_WB_READ_CAL_NVM_PMUD_GLOBAL_LDO_REFBITS(cal_nvm_ptr);

    pmud_vcore_ldo_temp_offset = GP_WB_READ_SE_CAL_RAM_PMUD_VCORE_LDO_TEMP_OFFSET(RAP_H_SE_CAL_RAM_PTR);
    pmud_global_ldo_temp_offset = GP_WB_READ_SE_CAL_RAM_PMUD_GLOBAL_LDO_TEMP_OFFSET(RAP_H_SE_CAL_RAM_PTR);
    pmud_vcore_ldo_refbits_ldo = pmud_vcore_ldo_refbits_ldo + pmud_vcore_ldo_temp_offset;
    pmud_global_ldo_refbits_ldo = pmud_global_ldo_refbits_ldo + pmud_global_ldo_temp_offset;

    pmud_vcore_ldo_refbits = pmud_vcore_ldo_refbits_ldo;
    pmud_global_ldo_refbits = pmud_global_ldo_refbits_ldo;

    switch (active_power_mode)
    {
        case GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS:
        {
            pmud_vcore_ldo_dcdc_batt_offset = GP_WB_READ_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR);
            pmud_global_ldo_dcdc_batt_offset = GP_WB_READ_SE_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR);
            pmud_vcore_ldo_refbits = pmud_vcore_ldo_refbits_ldo + pmud_vcore_ldo_dcdc_batt_offset;
            pmud_global_ldo_refbits = pmud_global_ldo_refbits_ldo + pmud_global_ldo_dcdc_batt_offset;
            break;
        }
        case GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS:
        {
            pmud_vcore_ldo_dcdc_batt_offset = GP_WB_READ_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET(RAP_H_SE_CAL_RAM_PTR);
            pmud_vcore_ldo_refbits = pmud_vcore_ldo_refbits_ldo + pmud_vcore_ldo_dcdc_batt_offset;
            break;
        }
        default:
        {
            break;
        }
    }

    if (pmud_vcore_ldo_refbits_ldo > 15)
    {
        pmud_vcore_ldo_refbits_ldo = 15;
    }
    if (pmud_global_ldo_refbits_ldo > 15)
    {
        pmud_global_ldo_refbits_ldo = 15;
    }
    if (pmud_vcore_ldo_refbits > 15)
    {
        pmud_vcore_ldo_refbits = 15;
    }
    if (pmud_global_ldo_refbits > 15)
    {
        pmud_global_ldo_refbits = 15;
    }

    GP_WB_WRITE_PMUD_VCORE_LDO_REFBITS(pmud_vcore_ldo_refbits);
    GP_WB_WRITE_DCDC_VCORE_LDO_REFBITS(pmud_vcore_ldo_refbits);
    GP_WB_WRITE_DCDC_VCORE_LDO_REFBITS_TX(pmud_vcore_ldo_refbits_ldo);

    GP_WB_WRITE_PMUD_GLOBAL_LDO_REFBITS(pmud_global_ldo_refbits);
    GP_WB_WRITE_DCDC_GLOBAL_LDO_REFBITS(pmud_global_ldo_refbits);
    GP_WB_WRITE_DCDC_GLOBAL_LDO_REFBITS_TX(pmud_global_ldo_refbits_ldo);

}

