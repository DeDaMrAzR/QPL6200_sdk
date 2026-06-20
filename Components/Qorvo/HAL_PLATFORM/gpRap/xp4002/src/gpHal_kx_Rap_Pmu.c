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

void rap_pmu_config_standby(UInt8 standby_power_mode)
{
    UInt16 standby_power_settings = 0;
    GP_WB_SET_PMUD_RRAM_POC_IO_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
    switch (standby_power_mode)
    {
        case GP_WB_ENUM_STANDBY_POWER_MODE_LPS:
        {
            GP_WB_SET_PMUD_SWITCH_OFF_MAIN_DOMAIN_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            break;
        }
        case GP_WB_ENUM_STANDBY_POWER_MODE_ELPS:
        {
            GP_WB_SET_PMUD_SWITCH_OFF_MAIN_DOMAIN_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LPR_DISABLE_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            break;
        }
        case GP_WB_ENUM_STANDBY_POWER_MODE_LPS_RET:
        {
            GP_WB_SET_PMUD_CUTOFF_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_IBIASGEN_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LDO_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDDRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_NVM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_RRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LPR_DISABLE_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            break;
        }
        case GP_WB_ENUM_STANDBY_POWER_MODE_ELPS_RET:
        {
            GP_WB_SET_PMUD_CUTOFF_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_IBIASGEN_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LDO_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 0);
            GP_WB_SET_PMUD_SW_VDDRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_NVM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_RRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LPR_DISABLE_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            break;
        }
        case GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET:
        {
            GP_WB_SET_PMUD_BGAP_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_CUTOFF_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_IBIASGEN_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LDO_PUP_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_USE_BGREF_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VREF_LOCAL_FB_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDDRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_NVM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_SW_VDD_RRAM_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_ISO_ANAN_IN_STANDBY_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            GP_WB_SET_PMUD_VCORE_LPR_DISABLE_TO_STANDBY_POWER_MODE_SETTINGS(standby_power_settings, 1);
            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            break;
        }
    }
    GP_WB_WRITE_PMUD_STANDBY_POWER_MODE_SETTINGS(standby_power_settings);
}

void rap_pmu_config_hps(void)
{
    UInt16 active_power_settings = 0;
    active_power_settings = GP_WB_READ_PMUD_ACTIVE_POWER_MODE_SETTINGS();
    GP_WB_SET_PMUD_BGAP_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_CUTOFF_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_IBIASGEN_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_VCORE_LDO_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_SW_VDDRAM_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_RO64M_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);

    GP_WB_WRITE_STANDBY_ISO_ANAN(1);

    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(1);
    GP_WB_WRITE_STANDBY_VCORE_RDY_PUP(1);
    GP_WB_WRITE_PMUD_ACTIVE_POWER_MODE_SETTINGS(active_power_settings);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_STANDBY_VCORE_RDY() != 1), 100);
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(0);
    GP_WB_WRITE_PMUD_VCORE_LPR_DISABLE(1);

    GP_WB_WRITE_PMUD_GLOBAL_LDO_PUP(1);
    GP_WB_WRITE_PMUD_GLOBAL_LDO_STARTUP_PUP(1);
    rap_h_wait_us(1);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_PMUD_GLOBAL_LDO_RDY() != 1), 10000);
    GP_WB_WRITE_PMUD_GLOBAL_LDO_STARTUP_PUP(0);

    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_PMUD_BGAP_RDY() != 1), 10000);
    GP_WB_WRITE_PMUD_DISABLE_VDDB_CUTOFF(1);
    GP_WB_WRITE_PMUD_USE_BGREF_IN_ACTIVE(1);
    GP_WB_WRITE_PMUD_VREF_LOCAL_FB_IN_ACTIVE(1);
    rap_h_wait_us(2);
    GP_WB_WRITE_PMUD_DISABLE_VDDB_CUTOFF(0);

    GP_WB_WRITE_GP_ADC_MACRO_LDO_PUP (1);
    GP_WB_WRITE_GP_ADC_IREF_PUP (1);
    GP_WB_WRITE_BBPLL_IREF_PUP(1);
    GP_WB_WRITE_BBPLL_MACRO_LDO_PUP(1);
    GP_WB_WRITE_BIST_IREF_PUP(1);
    GP_WB_WRITE_BIST_MACRO_LDO_PUP(1);
}

void rap_pmu_config_mps(void)
{
    UInt16 active_power_settings = 0;
    GP_WB_SET_PMUD_CUTOFF_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_IBIASGEN_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_VCORE_LDO_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_SW_VDDRAM_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_RO64M_PUP_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);
    GP_WB_SET_PMUD_SW_VDD_NVM_IN_ACTIVE_TO_ACTIVE_POWER_MODE_SETTINGS(active_power_settings, 1);

    GP_WB_WRITE_GP_ADC_IREF_PUP (0);
    GP_WB_WRITE_GP_ADC_MACRO_LDO_PUP (0);
    GP_WB_WRITE_BBPLL_IREF_PUP(0);
    GP_WB_WRITE_BBPLL_MACRO_LDO_PUP(0);
    GP_WB_WRITE_BIST_IREF_PUP(0);
    GP_WB_WRITE_BIST_MACRO_LDO_PUP(0);
    GP_WB_WRITE_PMUD_GLOBAL_LDO_PUP(0);

    GP_WB_WRITE_STANDBY_ISO_ANAN(1);

    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(1);
    GP_WB_WRITE_STANDBY_VCORE_RDY_PUP(1);
    GP_WB_WRITE_PMUD_DISABLE_VDDB_CUTOFF(1);
    GP_WB_WRITE_PMUD_ACTIVE_POWER_MODE_SETTINGS(active_power_settings);
    rap_h_wait_us(2);
    GP_WB_WRITE_PMUD_DISABLE_VDDB_CUTOFF(0);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_STANDBY_VCORE_RDY() != 1), 100);
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(0);
    GP_WB_WRITE_PMUD_VCORE_LPR_DISABLE(1);
}

void rap_pmu_vcore_ldo_enable(void)
{
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(1);
    GP_WB_WRITE_STANDBY_VCORE_RDY_PUP(1);
    GP_WB_WRITE_PMUD_VCORE_LDO_PUP_IN_ACTIVE(1);
    rap_h_wait_us(2);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_STANDBY_VCORE_RDY() != 1), 100);
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(0);
}

void rap_pmu_vcore_ldo_disable(void)
{
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(1);
    GP_WB_WRITE_PMUD_VCORE_LDO_PUP_IN_ACTIVE(0);
    GP_WB_WRITE_STANDBY_VCORE_RDY_PUP(0);
    rap_h_wait_us(2);
    GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(0);
}

