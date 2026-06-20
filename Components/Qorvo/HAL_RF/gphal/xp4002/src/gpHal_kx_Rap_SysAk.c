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


Bool ak_rap_sys_is_tx20(void)
{
    UInt16 version;
    version = GP_WB_READ_AK_STANDBY_VERSION();
    if ((version & 1) == 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void ak_rap_sys_reset_early(void)
{
    GP_WB_WRITE_AK_SX_TX_OUT_OF_RANGE_HI_THRES(1020);
    GP_WB_WRITE_AK_SX_TX_OUT_OF_RANGE_LO_THRES(3);
    GP_WB_WRITE_AK_RADIOITF_RX0_RXFE_LNA_BLEED_OVERRULE(1);
    GP_WB_WRITE_AK_RADIOITF_RX0_RXFE_LNA_BLEED_OVERRULE_ENA(1);
    GP_WB_WRITE_AK_RADIOITF_RX1_RXFE_LNA_BLEED_OVERRULE(1);
    GP_WB_WRITE_AK_RADIOITF_RX1_RXFE_LNA_BLEED_OVERRULE_ENA(1);
    GP_WB_WRITE_AK_RX_BBU_MAG_TH(323);
    GP_WB_WRITE_AK_RX_RX0_BBGM_CHOP_DIS_MIN_GAIN(1);
    GP_WB_WRITE_AK_RX_RX1_BBGM_CHOP_DIS_MIN_GAIN(1);

    GP_WB_WRITE_AK_RX_RXNB_BBGM_HIGH_CUR_MODE(1);
    GP_WB_WRITE_AK_RX_CIC_D32_ENA(1);
    GP_WB_WRITE_AK_RX_MAX_RSSI_NB_0(1048);
    GP_WB_WRITE_AK_RX_MAX_RSSI_WB_0(1048);
    GP_WB_WRITE_AK_RX_MAX_RSSI_NB_1(1048);
    GP_WB_WRITE_AK_RX_MAX_RSSI_WB_1(1048);

    GP_WB_WRITE_AK_RX_RSSI_ALWAYS_VALID_CFG(0);
    GP_WB_WRITE_AK_RX_RSSI_THRESHOLD_PIP_MS(80);

    GP_WB_WRITE_AK_SX_PLL_SAR_NOT_FLL(0);
    GP_WB_WRITE_AK_SX_LFF_INT_COUPLING_FACTOR(1);

    GP_WB_WRITE_AK_SX_MSK_MI_ADJUST(103);
    GP_WB_WRITE_AK_SX_MSK_DELAY(7);
    GP_WB_WRITE_AK_SX_GFSK_MI_ADJUST(103);
    GP_WB_WRITE_AK_SX_GFSK_DELAY(5);

    GP_WB_WRITE_AK_RADIOITF_SX_SAMPLER_LDO_BLEED_OVERRULE_ENA(1);
    GP_WB_WRITE_AK_RADIOITF_SX_SAMPLER_LDO_BLEED_OVERRULE(1);
    GP_WB_WRITE_AK_RADIOITF_SX_DIV2_LDO_BLEED_OVERRULE_ENA(1);
    GP_WB_WRITE_AK_RADIOITF_SX_DIV2_LDO_BLEED_OVERRULE(1);

    GP_WB_WRITE_AK_SX_DTC_MSK_MI_ADJUST(1);
    GP_WB_WRITE_AK_SX_DTC_GFSK_MI_ADJUST(0);

    GP_WB_WRITE_AK_INT_CTRL_MASK_PHY_SX_CAL_START_INTERRUPT(0);

    GP_WB_WRITE_AK_SX_DCO_VDDRC_BYPASS(1);
    GP_WB_WRITE_AK_TRX_SX_DCO_IBIAS_SEL_TX(7);
    GP_WB_WRITE_AK_TRX_SX_DCO_IBIAS_SEL_RX(7);

    GP_WB_WRITE_AK_RX_IQ_COMP_ENA(1);

    GP_WB_WRITE_AK_RX_BBD_MAG_TH(812);
    GP_WB_WRITE_AK_RX_RFSETTLE_PER(1);
    GP_WB_WRITE_AK_RX_RFDCNT_NORM(24);
    GP_WB_WRITE_AK_RX_BBDCNT_NORM(18);

    GP_WB_WRITE_AK_RX_RXNB_ADC_ERROR_EST_EN(0);

}

void ak_rap_sys_backup_late(void)
{
}

void ak_rap_sys_hal_init(void)
{
}

void ak_rap_sys_temp_change(Int32 temp_deg)
{
    NOT_USED(temp_deg);
    ak_rap_sx_cal_nrt();
}

void ak_rap_sys_cal_nvm_load_defaults(void)
{
}

void ak_rap_sys_cal_nvm_restore(void)
{
    UInt32 cal_nvm_ptr;

    cal_nvm_ptr = ak_rap_h_get_cal_nvm_ptr();
    rap_h_assert_sys(cal_nvm_ptr != 0);

    rap_h_memcpy32(GP_WB_AK_RX_RX0_RXFE_RFDET_GAIN_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_RX0_RXFE_RFDET_GAIN_ADDRESS, 2);
    rap_h_memcpy32(GP_WB_AK_RX_RX0_BBGM_I_DC_TUNE_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE_ADDRESS, 1);

    if (false)
    {
        rap_h_memcpy32(GP_WB_AK_RX_IQCOMP0_REAL0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_IQCOMP0_REAL0_ADDRESS, 10);
        rap_h_memcpy32(GP_WB_AK_RX_BB_GAIN0_0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_BB_GAIN0_0_ADDRESS, 8);
        rap_h_memcpy32(GP_WB_AK_RX_RX_RXFE_LNA_BIAS0_0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0_ADDRESS, 4);
        rap_h_memcpy32(GP_WB_AK_RX_ADC_DC_OFFSET0_REAL0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0_ADDRESS, 10);
        rap_h_memcpy32(GP_WB_AK_RX_BB_GAIN0_COMP_REAL0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0_ADDRESS, 8);
        rap_h_memcpy32(GP_WB_AK_RX_RF_GAIN3_COMP_REAL0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0_ADDRESS, 8);
    }
    else
    {
        rap_h_memcpy32(GP_WB_AK_RX_IQCOMP0_REAL0_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_RX_IQCOMP0_REAL0_ADDRESS, 48);
    }


    rap_h_memcpy32(GP_WB_AK_SX_DTC_RES_CAL_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_SX_DTC_RES_CAL_ADDRESS, 2);

    rap_h_memcpy32(GP_WB_AK_TRX_MACRO_IREF0_REFBITS_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_ADDRESS, 1);

    rap_h_memcpy32(GP_WB_AK_TX_RES_CAL_ADDRESS, cal_nvm_ptr + GP_WB_AK_CAL_NVM_TX_RES_CAL_ADDRESS, 1);

}


