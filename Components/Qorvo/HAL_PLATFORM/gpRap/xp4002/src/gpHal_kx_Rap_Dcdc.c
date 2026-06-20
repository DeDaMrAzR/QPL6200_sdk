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


void rap_dcdc_rm_bits(void)
{
    UInt32 vbatt_uv;
    vbatt_uv = GP_WB_READ_CAL_RAM_VBATT_UV();
    rap_h_assert_sys(vbatt_uv > 0);

    if (vbatt_uv < 2500000)
    {
        GP_WB_WRITE_DCDC_RM1(0);
        GP_WB_WRITE_DCDC_RM2(1);
    }
    else if (vbatt_uv < 2700000)
    {
        GP_WB_WRITE_DCDC_RM1(1);
        GP_WB_WRITE_DCDC_RM2(2);
    }
    else if (vbatt_uv < 3000000)
    {
        GP_WB_WRITE_DCDC_RM1(2);
        GP_WB_WRITE_DCDC_RM2(3);
    }
    else if (vbatt_uv < 3300000)
    {
        GP_WB_WRITE_DCDC_RM1(3);
        GP_WB_WRITE_DCDC_RM2(4);
    }
    else if (vbatt_uv < 3600000)
    {
        GP_WB_WRITE_DCDC_RM1(4);
        GP_WB_WRITE_DCDC_RM2(4);
    }
    else
    {
        GP_WB_WRITE_DCDC_RM1(5);
        GP_WB_WRITE_DCDC_RM2(5);
    }
}

void rap_dcdc_set_rm(void)
{
    UInt8 int_value;
    UInt8 read_value;

    GP_WB_DCDC_CLR_ZC_CORR1_TOO_HI();
    GP_WB_DCDC_CLR_ZC_CORR1_TOO_LO();
    GP_WB_DCDC_CLR_ZC_CORR2_TOO_HI();
    GP_WB_DCDC_CLR_ZC_CORR2_TOO_LO();

    GP_WB_WRITE_DCDC_ZC_CORR1_HI_THRESHOLD(8);
    GP_WB_WRITE_DCDC_ZC_CORR1_LO_THRESHOLD(-8);
    GP_WB_WRITE_DCDC_ZC_CORR2_HI_THRESHOLD(8);
    GP_WB_WRITE_DCDC_ZC_CORR2_LO_THRESHOLD(-8);

    while (true)
    {
        int_value = GP_WB_READ_DCDC_UNMASKED_ZC_CORR1_TOO_HI();
        if (int_value == 1)
        {
            read_value = GP_WB_READ_DCDC_RM1();
            GP_WB_WRITE_DCDC_RM1(read_value+1);
            rap_h_wait_us(64);
            GP_WB_DCDC_CLR_ZC_CORR1_TOO_HI();
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        int_value = GP_WB_READ_DCDC_UNMASKED_ZC_CORR1_TOO_LO();
        if (int_value == 1)
        {
            read_value = GP_WB_READ_DCDC_RM1();
            GP_WB_WRITE_DCDC_RM1(read_value-1);
            rap_h_wait_us(64);
            GP_WB_DCDC_CLR_ZC_CORR1_TOO_LO();
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        int_value = GP_WB_READ_DCDC_UNMASKED_ZC_CORR2_TOO_HI();
        if (int_value == 1)
        {
            read_value = GP_WB_READ_DCDC_RM2();
            GP_WB_WRITE_DCDC_RM2(read_value+1);
            rap_h_wait_us(64);
            GP_WB_DCDC_CLR_ZC_CORR2_TOO_HI();
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        int_value = GP_WB_READ_DCDC_UNMASKED_ZC_CORR2_TOO_LO();
        if (int_value == 1)
        {
            read_value = GP_WB_READ_DCDC_RM2();
            GP_WB_WRITE_DCDC_RM2(read_value-1);
            rap_h_wait_us(64);
            GP_WB_DCDC_CLR_ZC_CORR2_TOO_LO();
        }
        else
        {
            break;
        }
    }

}

void rap_dcdc_rc_tune_ton_dac_calibration(void)
{
    UInt8 i;
    UInt8 j;
    UInt8 y2_skip_value;
    UInt8 y2_cal_value;
    UInt8 clkp_value;
    GP_WB_WRITE_PMUD_DCDC_PUP(1);

    i = 3;
    j = 0;
    y2_skip_value = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EHPS(RAP_H_SE_CAL_RAM_PTR);
    y2_cal_value = y2_skip_value + 2;

    GP_WB_WRITE_PMUD_DCDC_S2_OVERRULE(0);
    GP_WB_WRITE_PMUD_DCDC_S2_OVERRULE_ENA(1);
    GP_WB_WRITE_PMUD_ENABLE_VCORE_LPR_ON_DCDC_OVERLOAD(0);

    GP_WB_WRITE_DCDC_BIST_EN(1);
    GP_WB_WRITE_PMUD_DCDC_RET_EN(1);
    rap_h_wait_us(32);
    GP_WB_WRITE_DCDC_TON_DAC_OVERRULE(1);
    GP_WB_WRITE_PMUD_DCDC_TON_DAC_OVERRULE_ENA(1);
    GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE(0);
    GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE_ENA(1);
    GP_WB_WRITE_PMUD_DCDC_RC_TUNE(0);


    while (i<8)
    {
        GP_WB_WRITE_PMUD_DCDC_TON_DAC_OVERRULE(i);
        while (j<16)
        {
            GP_WB_WRITE_PMUD_DCDC_RC_TUNE(j);
            rap_h_wait_us(30);
            GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE(1);
            GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE(0);
            rap_h_wait_us(10);
            clkp_value = GP_WB_READ_DCDC_CLKP_LENGTH();
            if (clkp_value >= y2_cal_value)
            {
                break;
            }
            j = j+1;
        }
        if (clkp_value >= y2_cal_value)
        {
            GP_WB_WRITE_PMUD_DCDC_RC_TUNE(j);
            GP_WB_WRITE_PMUD_DCDC_TON_DAC_DEFAULT(i);
            break;
        }
        else
        {
            i = i+1;
        }
    }


    GP_WB_WRITE_PMUD_DCDC_TON_DAC_OVERRULE_ENA(0);
    GP_WB_WRITE_DCDC_TON_DAC_OVERRULE(0);

    GP_WB_WRITE_PMUD_DCDC_RET_EN(0);
    GP_WB_WRITE_PMUD_ENABLE_VCORE_LPR_ON_DCDC_OVERLOAD(1);
    GP_WB_WRITE_PMUD_DCDC_S2_OVERRULE_ENA(0);
    rap_h_wait_us(32);
    GP_WB_WRITE_DCDC_BIST_EN(0);
    GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE_ENA(0);
    GP_WB_WRITE_PMUD_DCDC_CLKP_OVERRULE(0);
    GP_WB_WRITE_PMUD_DCDC_PUP(0);
}

void rap_dcdc_timerxl_setup(UInt8 index)
{
    UInt32 timerxl_offset;
    UInt32 timer16_offset;
    UInt32 qaddr;
    UInt8 idx;
    UInt8 qchannel;

    idx = index;
    timer16_offset = RAP_TIMERXL_TIMER16_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2) + RAP_TIMERXL_TIMER16_INTERVAL*(idx%2);
    timerxl_offset = RAP_TIMERXL_TIMERXL_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2);
    GP_WB_WRITE_TIMERXL_MODE(timerxl_offset, GP_WB_ENUM_TIMERXL_OPERATING_MODE_DUAL_16BITS);
    GP_WB_WRITE_TIMERXL16_COUNT_MODE(timer16_offset, GP_WB_ENUM_TIMERXL_COUNT_MODE_EXTERNAL);

    idx = index+1;
    timer16_offset = RAP_TIMERXL_TIMER16_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2) + RAP_TIMERXL_TIMER16_INTERVAL*(idx%2);
    timerxl_offset = RAP_TIMERXL_TIMERXL_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2);
    GP_WB_WRITE_TIMERXL16_PRESCALER_DIV(timer16_offset, 6);
    GP_WB_WRITE_TIMERXL16_CC_0_COMPARECAPTURE(timer16_offset, 1);
    GP_WB_WRITE_TIMERXL16_CC_1_COMPARECAPTURE(timer16_offset, 499);
    GP_WB_WRITE_TIMERXL16_CC_1_CLR_ON_MATCH(timer16_offset, 1);

    qchannel = index;
    qaddr = RAP_TIMERXL_QUIC_BASE + (qchannel * RAP_TIMERXL_QUIC_INTERVAL);
    timerxl_offset = RAP_TIMERXL_TIMERXL_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2);
    if (index == 0)
    {
        GP_WB_WRITE_QUIC_CHANNEL_TRIGGER_SRC(qaddr, GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP1);
    }
    else
    {
        GP_WB_WRITE_QUIC_CHANNEL_TRIGGER_SRC(qaddr, GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP2);
    }
    GP_WB_WRITE_QUIC_CHANNEL_EDGE_SEL(qaddr, GP_WB_ENUM_CHANNEL_EDGE_SEL_RISING);

    qchannel = index+1;
    qaddr = RAP_TIMERXL_QUIC_BASE + (qchannel * RAP_TIMERXL_QUIC_INTERVAL);
    if (index == 0)
    {
        GP_WB_WRITE_QUIC_CHANNEL_TRIGGER_SRC(qaddr, GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_0_MATCH_1);
    }
    else
    {
        GP_WB_WRITE_QUIC_CHANNEL_TRIGGER_SRC(qaddr, GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_0_MATCH_1);
    }
    GP_WB_WRITE_QUIC_CHANNEL_EDGE_SEL(qaddr, GP_WB_ENUM_CHANNEL_EDGE_SEL_LEVEL);

    idx = index;
    timerxl_offset = RAP_TIMERXL_TIMERXL_BASE + RAP_TIMERXL_TIMERXL_INTERVAL*(idx/2);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_CHANNEL_0(timerxl_offset, idx);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_TIMERXL_0(timerxl_offset, idx%2);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_TYPE_0(timerxl_offset, GP_WB_ENUM_TIMERXL_ACTION_INCREMENT_TIMER);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_CHANNEL_1(timerxl_offset, idx+1);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_TIMERXL_1(timerxl_offset, idx%2);
    GP_WB_WRITE_TIMERXL_QUIC_ACTION_TYPE_1(timerxl_offset, GP_WB_ENUM_TIMERXL_ACTION_TOGGLE_TIMER);

}

void rap_dcdc_y1_y2_calibration(Bool ehps, UInt8 index)
{
    UInt8 y_skip_cal;
    UInt32 q_min;
    UInt32 q;
    UInt8 clkp_length;
    UInt8 clkp_length_nom;
    UInt8 clkp_length_min_x;
    UInt32 nr_clkp_pulses;
    GP_WB_WRITE_DCDC_OMEGA1(3);
    GP_WB_WRITE_DCDC_OMEGA2(3);
    GP_WB_WRITE_DCDC_SW_WIDTH_THRESHOLD3(0);

    q_min = 10e6;
    y_skip_cal = 7;

    if (index == 0)
    {
        GP_WB_WRITE_DCDC_CHARGE_COUNT1_ENA(1);
        for (clkp_length=7; clkp_length<16; clkp_length++)
        {
            clkp_length_nom = clkp_length;
            clkp_length_min_x = clkp_length-2;
            GP_WB_WRITE_DCDC_Y1_SKIP(clkp_length_min_x);
            GP_WB_WRITE_DCDC_Y1_MIN(clkp_length_min_x);
            GP_WB_WRITE_DCDC_Y1_MAX(clkp_length_nom);
            GP_WB_TIMERXL_0_TIMERXL16_0_RESET();
            GP_WB_TIMERXL_0_TIMERXL16_1_RESET();
            GP_WB_TIMERXL_0_TIMERXL16_1_START();
            GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_TIMERXL_0_UNMASKED_TIMERXL16_1_OVERFLOW_INTERRUPT() != 1), 10000);
            rap_h_wait_us(10);
            GP_WB_TIMERXL_0_TIMERXL16_1_STOP();
            nr_clkp_pulses = GP_WB_READ_TIMERXL_0_TIMERXL16_0_COUNTER();
            q = clkp_length_nom * clkp_length_nom * nr_clkp_pulses;
            if (q < q_min)
            {
                q_min = q;
                y_skip_cal = clkp_length_min_x;
            }
            GP_WB_TIMERXL_0_TIMERXL16_1_CLR_OVERFLOW_INTERRUPT();
            rap_h_wait_us(10);
        }
        y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_SKIP(RAP_H_SE_CAL_RAM_PTR);
        y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_MIN(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y1_SKIP(y_skip_cal);
        GP_WB_WRITE_DCDC_Y1_MIN(y_skip_cal);
        GP_WB_WRITE_DCDC_Y1_MAX(100);
        GP_WB_WRITE_DCDC_CHARGE_COUNT1_ENA(0);
    }

    if (index == 2)
    {
        GP_WB_WRITE_DCDC_CHARGE_COUNT2_ENA(1);
        for (clkp_length=7; clkp_length<16; clkp_length++)
        {
            if (ehps)
            {
                clkp_length_nom = clkp_length;
                clkp_length_min_x = clkp_length-2;
            }
            else
            {
                clkp_length_nom = clkp_length-5;
                clkp_length_min_x = clkp_length-7;
            }
            GP_WB_WRITE_DCDC_Y2_SKIP(clkp_length_min_x);
            GP_WB_WRITE_DCDC_Y2_MIN(clkp_length_min_x);
            GP_WB_WRITE_DCDC_Y2_MAX(clkp_length_nom);
            GP_WB_TIMERXL_1_TIMERXL16_0_RESET();
            GP_WB_TIMERXL_1_TIMERXL16_1_RESET();
            GP_WB_TIMERXL_1_TIMERXL16_1_START();
            GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_TIMERXL_1_UNMASKED_TIMERXL16_1_OVERFLOW_INTERRUPT() != 1), 10000);
            rap_h_wait_us(10);
            GP_WB_TIMERXL_1_TIMERXL16_1_STOP();
            nr_clkp_pulses = GP_WB_READ_TIMERXL_1_TIMERXL16_0_COUNTER();
            q = clkp_length_nom * clkp_length_nom * nr_clkp_pulses;
            if (q < q_min)
            {
                q_min = q;
                y_skip_cal = clkp_length_min_x;
            }
            GP_WB_TIMERXL_1_TIMERXL16_1_CLR_OVERFLOW_INTERRUPT();
            rap_h_wait_us(10);
        }
        if (ehps)
        {
            y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EHPS(RAP_H_SE_CAL_RAM_PTR);
            y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EHPS(RAP_H_SE_CAL_RAM_PTR);
            GP_WB_WRITE_DCDC_Y2_SKIP(y_skip_cal);
            GP_WB_WRITE_DCDC_Y2_MIN(y_skip_cal);
        }
        else
        {
            y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EMPS(RAP_H_SE_CAL_RAM_PTR);
            y_skip_cal = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EMPS(RAP_H_SE_CAL_RAM_PTR);
            GP_WB_WRITE_DCDC_Y2_SKIP(y_skip_cal);
            GP_WB_WRITE_DCDC_Y2_MIN(y_skip_cal);
        }
        GP_WB_WRITE_DCDC_Y2_MAX(100);
        GP_WB_WRITE_DCDC_CHARGE_COUNT2_ENA(0);
    }

    GP_WB_WRITE_DCDC_SW_WIDTH_THRESHOLD3(30);

}

void rap_dcdc_zcd_calibration(void)
{
    Bool act_en;
    Bool dcdc_s2_skip;
    UInt8 dcdc_zcd_offset_value;
    Bool timedout;
    act_en = GP_WB_READ_DCDC_ACT_EN();
    rap_h_assert_sys(act_en);

    GP_WB_WRITE_DCDC_ZCD_DIS(0);
    GP_WB_WRITE_DCDC_ZC_CORR_ENA(0);

    dcdc_s2_skip = GP_WB_READ_DCDC_S2_SKIP();
    if (dcdc_s2_skip == 0)
    {
        GP_WB_WRITE_DCDC_S1_SKIP(1);

        GP_WB_WRITE_DCDC_CAL_EN(1);

        dcdc_zcd_offset_value = GP_WB_READ_PMUD_DCDC_ZCD_OFFSET_RET();
        GP_WB_WRITE_DCDC_ZCD_RESET_OFFSET_VALUE(dcdc_zcd_offset_value);
        GP_WB_WRITE_DCDC_ZCD_RESET_OFFSET(1);

        GP_WB_DCDC_START_ZCD_CAL();
        GP_DO_WHILE_TIMEOUT((GP_WB_READ_DCDC_ZCD_CAL_READY() != 1), 1000, &timedout);
        GP_WB_WRITE_DCDC_CAL_EN(0);

        GP_WB_WRITE_DCDC_ZCD_RESET_OFFSET(0);
        GP_WB_WRITE_DCDC_ZCD_RESET_OFFSET_VALUE(16);

        if ( !(timedout))
        {
            dcdc_zcd_offset_value = GP_WB_READ_DCDC_DCDC_ZCD_OFFSET();
            GP_WB_WRITE_PMUD_DCDC_ZCD_OFFSET_RET(dcdc_zcd_offset_value);
        }

        GP_WB_WRITE_DCDC_S1_SKIP(0);
    }

}

void rap_dcdc_init(void)
{
    UInt8 u8;
    u8 = 0;
    GP_WB_SET_IOB_GPIO_19_ALTERNATE_TO_GPIO_19_ALTERNATE_CFG(u8, GP_WB_ENUM_GPIO_19_ALTERNATES_ANALOG_LXH);
    GP_WB_SET_IOB_GPIO_19_ALTERNATE_ENABLE_TO_GPIO_19_ALTERNATE_CFG(u8, 1);
    GP_WB_WRITE_IOB_GPIO_19_ALTERNATE_CFG(u8);
    u8 = 0;
    GP_WB_SET_IOB_GPIO_20_ALTERNATE_TO_GPIO_20_ALTERNATE_CFG(u8, GP_WB_ENUM_GPIO_20_ALTERNATES_ANALOG_LXL);
    GP_WB_SET_IOB_GPIO_20_ALTERNATE_ENABLE_TO_GPIO_20_ALTERNATE_CFG(u8, 1);
    GP_WB_WRITE_IOB_GPIO_20_ALTERNATE_CFG(u8);
}

void rap_dcdc_config_active(Bool ehps)
{
    UInt8 tmp = 3;
    UInt16 standby_version = 0;
    UInt32 sw_width = 3;
    UInt32 vbatt_uv;
    vbatt_uv = GP_WB_READ_CAL_RAM_VBATT_UV();

    GP_WB_WRITE_DCDC_SW_WIDTH_THRESHOLD1(30);
    GP_WB_WRITE_DCDC_SW_WIDTH_THRESHOLD2(30);
    GP_WB_WRITE_DCDC_SW_WIDTH_THRESHOLD3(30);
    GP_WB_WRITE_PMUD_DCDC_SW_WIDTH_OVERRULE_ENA(1);

    if (ehps)
    {
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_SKIP(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y1_SKIP(tmp);
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EHPS(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y2_SKIP(tmp);
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_MIN(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y1_MIN(tmp);
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EHPS(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y2_MIN(tmp);
        GP_WB_WRITE_DCDC_S1_SKIP(0);
        GP_WB_WRITE_DCDC_S2_SKIP(0);
        GP_WB_WRITE_DCDC_N1_SKIP(8);
        GP_WB_WRITE_DCDC_N2_SKIP(8);
        GP_WB_WRITE_DCDC_N_COUNT(4);

        standby_version = GP_WB_READ_STANDBY_VERSION();
        if ( !(standby_version & 1))
        {
            GP_WB_WRITE_PMUD_USE_DCDC_LDO_REFBITS(1);
            GP_WB_WRITE_DCDC_TX_MODE(GP_WB_ENUM_DCDC_TX_MODE_AUTO_PHY_TX);
        }
        else
        {
            GP_WB_WRITE_DCDC_Y1_PRELOAD_ENA(2);
            GP_WB_WRITE_DCDC_Y1_PA_RAMPDN_PRELOAD(0);
            GP_WB_WRITE_PMUD_USE_DCDC_LDO_REFBITS(0);
            GP_WB_WRITE_DCDC_TX_MODE(GP_WB_ENUM_DCDC_TX_MODE_DISABLED);
        }
    }
    else
    {
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EMPS(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y2_SKIP(tmp);
        tmp = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EMPS(RAP_H_SE_CAL_RAM_PTR);
        GP_WB_WRITE_DCDC_Y2_MIN(tmp);
        GP_WB_WRITE_DCDC_S1_SKIP(1);
        GP_WB_WRITE_DCDC_S2_SKIP(0);
        GP_WB_WRITE_DCDC_N1_SKIP(6);
        GP_WB_WRITE_DCDC_N2_SKIP(6);
        GP_WB_WRITE_DCDC_N_COUNT(1);
    }

    if (vbatt_uv < 2200000)
    {
        GP_WB_WRITE_DCDC_S1_SKIP(1);
        GP_WB_WRITE_DCDC_S2_SKIP(1);
    }

    GP_WB_WRITE_PMUD_DCDC_SW_WIDTH_OVERRULE(sw_width);

    GP_WB_WRITE_DCDC_ZC_CORR_ENA(1);
    GP_WB_WRITE_DCDC_ZCD_DIS(1);
}

void rap_dcdc_enable_ehps(void)
{
    rap_dcdc_init();
    rap_dcdc_config_active(1);
    rap_dcdc_rm_bits();
    GP_WB_WRITE_PMUD_DCDC_PUP(1);
    GP_WB_WRITE_DCDC_ACT_EN(1);
    GP_WB_WRITE_PMUD_DCDC_RET_EN(0);
    rap_h_wait_us(32);
}

void rap_dcdc_enable_emps(void)
{
    rap_dcdc_init();
    rap_dcdc_config_active(0);
    rap_dcdc_rm_bits();
    GP_WB_WRITE_PMUD_DCDC_PUP(1);
    GP_WB_WRITE_DCDC_ACT_EN(1);
    GP_WB_WRITE_PMUD_DCDC_RET_EN(0);
    rap_h_wait_us(32);
}

void rap_dcdc_enable_ret(void)
{
    GP_WB_WRITE_PMUD_USE_DCDC_LDO_REFBITS(0);
    GP_WB_WRITE_DCDC_ACT_EN(0);
    GP_WB_WRITE_PMUD_DCDC_RET_EN(1);
    rap_h_wait_us(32);
}

void rap_dcdc_disable(void)
{
    GP_WB_WRITE_PMUD_USE_DCDC_LDO_REFBITS(0);
    GP_WB_WRITE_DCDC_ACT_EN(0);
    GP_WB_WRITE_PMUD_DCDC_RET_EN(0);
    rap_h_wait_us(32);
    GP_WB_WRITE_DCDC_ZC_CORR_ENA(0);
    GP_WB_WRITE_PMUD_WAKEUP_ON_DCDC_OVERLOAD(0);
    GP_WB_WRITE_PMUD_DCDC_PUP(0);
}

void rap_dcdc_validate_config(void)
{
    UInt8 y1_skip;
    UInt8 y1_min;
    UInt8 y2_skip_ehps;
    UInt8 y2_min_ehps;
    UInt8 y2_skip_emps;
    UInt8 y2_min_emps;
    UInt8 y1_max;
    UInt8 y2_max;
    y1_skip = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_SKIP(RAP_H_SE_CAL_RAM_PTR);
    y1_min = GP_WB_READ_SE_CAL_RAM_DCDC_Y1_MIN(RAP_H_SE_CAL_RAM_PTR);
    y2_skip_ehps = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EHPS(RAP_H_SE_CAL_RAM_PTR);
    y2_min_ehps = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EHPS(RAP_H_SE_CAL_RAM_PTR);
    y2_skip_emps = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_SKIP_EMPS(RAP_H_SE_CAL_RAM_PTR);
    y2_min_emps = GP_WB_READ_SE_CAL_RAM_DCDC_Y2_MIN_EMPS(RAP_H_SE_CAL_RAM_PTR);
    y1_max = GP_WB_READ_DCDC_Y1_MAX();
    y2_max = GP_WB_READ_DCDC_Y2_MAX();

    rap_h_assert_sys(y1_min <= y1_skip);
    rap_h_assert_sys(y1_skip <= y1_max);

    rap_h_assert_sys(y2_min_ehps <= y2_skip_ehps);
    rap_h_assert_sys(y2_skip_ehps <= y2_max);

    rap_h_assert_sys(y2_min_emps <=  y2_skip_emps);
    rap_h_assert_sys(y2_skip_emps <= y2_max);
}


