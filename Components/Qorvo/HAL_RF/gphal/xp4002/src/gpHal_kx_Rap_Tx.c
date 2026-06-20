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


void ak_rap_tx_config_antenna(Bool se)
{
    GP_WB_WRITE_AK_TRX_TX_A0_RFIO_RF0_SHORT(0);
    GP_WB_WRITE_AK_TRX_TX_A0_RFIO_RF1_SHORT(se);
    GP_WB_WRITE_AK_TRX_TX_A1_RFIO_RF0_SHORT(se);
    GP_WB_WRITE_AK_TRX_TX_A1_RFIO_RF1_SHORT(0);
}

void ak_rap_tx_cw_enable(UInt8 chidx, Bool antenna, UInt8 modulation_type, Bool tx)
{
    GP_WB_WRITE_AK_PAT_FRAME_LEN(1);
    GP_WB_WRITE_AK_PAT_ANTENNA(antenna);
    GP_WB_WRITE_AK_PAT_TEST_MODE(1);

    GP_WB_WRITE_AK_PAT_FEC_ENCODER_ENABLE(0);
    GP_WB_WRITE_AK_PAT_INTERLEAVER_ENABLE(0);
    GP_WB_WRITE_AK_PAT_WHITENING_PN9_LATE_ENABLE(0);
    GP_WB_WRITE_AK_PAT_PHY_MODE(modulation_type);

    GP_WB_WRITE_AK_PLME_SX_TX_NOT_RX_OVERRULE(tx);
    GP_WB_WRITE_AK_PLME_SX_TX_NOT_RX_OVERRULE_ENA(1);

    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_STATE(GP_WB_ENUM_AK_TRC_STATE_TX);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_CHANNEL_IDX(chidx);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_TIMESTAMP_VALID(0);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_WARMUP_TIME(22);
    GP_WB_AK_RTM_TRC_STATEREQ();

    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_RTM_TRC_STATE() != GP_WB_ENUM_AK_TRC_STATE_TX), 100);
}

void ak_rap_tx_cw_disable(void)
{
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_STATE(GP_WB_ENUM_AK_TRC_STATE_IDLE);
    GP_WB_WRITE_AK_RTM_TRC_STATEREQ_TIMESTAMP_VALID(0);
    GP_WB_AK_RTM_TRC_STATEREQ();

    GP_WB_WRITE_AK_PAT_TEST_MODE(0);

    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_RTM_TRC_STATE() != GP_WB_ENUM_AK_TRC_STATE_IDLE), 100);

    GP_WB_WRITE_AK_PLME_SX_TX_NOT_RX_OVERRULE_ENA(0);
}

void ak_rap_tx_set_psf_coeffs(UInt8 filter, const UInt16* coeffs)
{
    UInt16 tmp;
    if (filter == AK_RAP_TX_PSF_FILTER_GFSK)
    {
        tmp = coeffs[0];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF0(tmp);
        tmp = coeffs[1];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF1(tmp);
        tmp = coeffs[2];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF2(tmp);
        tmp = coeffs[3];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF3(tmp);
        tmp = coeffs[4];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF4(tmp);
        tmp = coeffs[5];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF5(tmp);
        tmp = coeffs[6];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF6(tmp);
        tmp = coeffs[7];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF7(tmp);
        tmp = coeffs[8];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF8(tmp);
        tmp = coeffs[9];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF9(tmp);
        tmp = coeffs[10];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF10(tmp);
        tmp = coeffs[11];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF11(tmp);
        tmp = coeffs[12];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF12(tmp);
        tmp = coeffs[13];
        GP_WB_WRITE_AK_SX_GFSK_FILTER_COEFF13(tmp);
    }
    else
    {
        tmp = coeffs[0];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF0(tmp);
        tmp = coeffs[1];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF1(tmp);
        tmp = coeffs[2];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF2(tmp);
        tmp = coeffs[3];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF3(tmp);
        tmp = coeffs[4];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF4(tmp);
        tmp = coeffs[5];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF5(tmp);
        tmp = coeffs[6];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF6(tmp);
        tmp = coeffs[7];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF7(tmp);
        tmp = coeffs[8];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF8(tmp);
        tmp = coeffs[9];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF9(tmp);
        tmp = coeffs[10];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF10(tmp);
        tmp = coeffs[11];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF11(tmp);
        tmp = coeffs[12];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF12(tmp);
        tmp = coeffs[13];
        GP_WB_WRITE_AK_SX_MSK_FILTER_COEFF13(tmp);
    }
}

void ak_rap_tx_config_psf(UInt8 sx_channel)
{
    Bool tx20;
    tx20 = ak_rap_sys_is_tx20();
    if (tx20)
    {
        if (sx_channel<1)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS25");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS25);
        }
        else if (sx_channel<14)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS18");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS18);
        }
        else if (sx_channel<15)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS38ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS38ISI);
        }
        else if (sx_channel<16)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS42ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS42ISI);
        }
        else if (sx_channel<55)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_GFSK to AK_RAP_TX_PSF_COEFFS_GAUSS38ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_GFSK, AK_RAP_TX_PSF_COEFFS_GAUSS38ISI);
        }
        else
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_GFSK to AK_RAP_TX_PSF_COEFFS_GAUSS42ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_GFSK, AK_RAP_TX_PSF_COEFFS_GAUSS42ISI);
        }
    }
    else
    {
        if (sx_channel<14)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_DIRAC");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_DIRAC);
        }
        else if (sx_channel<15)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS25");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS25);
        }
        else if (sx_channel<16)
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_MSK to AK_RAP_TX_PSF_COEFFS_GAUSS42ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_MSK, AK_RAP_TX_PSF_COEFFS_GAUSS42ISI);
        }
        else
        {
            rap_h_log_debug("config AK_RAP_TX_PSF_FILTER_GFSK to AK_RAP_TX_PSF_COEFFS_GAUSS38ISI");
            ak_rap_tx_set_psf_coeffs(AK_RAP_TX_PSF_FILTER_GFSK, AK_RAP_TX_PSF_COEFFS_GAUSS38ISI);
        }
    }
}

UInt32 ak_rap_tx_power2pbo(UInt8 mode, UInt8 power)
{
    UInt32 pbo_code;
    switch (mode)
    {
        case AK_RAP_TX_PA_MODE_THERMO:
        case AK_RAP_TX_PA_MODE_THERMO_PLUS:
        case AK_RAP_TX_PA_MODE_THERMO_MINUS:
        case AK_RAP_TX_PA_MODE_OFF:
        {
            pbo_code = (power & 3) + (((1<<(power / 4))-1) * 4);

            break;
        }
        case AK_RAP_TX_PA_MODE_1HOT:
        case AK_RAP_TX_PA_MODE_1HOT_PLUS:
        case AK_RAP_TX_PA_MODE_1HOT_MINUS:
        {
            pbo_code = (power & 3)  +  ((1<<(power / 4)) / 2) * 4;

            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            pbo_code = 0;
            break;
        }
    }
    return pbo_code;
}

void ak_rap_tx_config_pa(UInt8 mode, UInt8 power)
{
    UInt32 pbo_code;

    switch (mode)
    {
        case AK_RAP_TX_PA_MODE_THERMO:
        case AK_RAP_TX_PA_MODE_THERMO_PLUS:
        case AK_RAP_TX_PA_MODE_THERMO_MINUS:
        case AK_RAP_TX_PA_MODE_OFF:
        {
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_MINUS_OVERRULE_ENA(0);
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_PLUS_OVERRULE_ENA(0);
            GP_WB_WRITE_AK_PAT_TX_POWER_P(power);
            GP_WB_WRITE_AK_PAT_TX_POWER_N(power);
            break;
        }
        case AK_RAP_TX_PA_MODE_1HOT:
        case AK_RAP_TX_PA_MODE_1HOT_PLUS:
        case AK_RAP_TX_PA_MODE_1HOT_MINUS:
        {
            pbo_code = ak_rap_tx_power2pbo(mode, power);
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_MINUS_OVERRULE(pbo_code);
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_PLUS_OVERRULE(pbo_code);
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_MINUS_OVERRULE_ENA(1);
            GP_WB_WRITE_AK_RADIOITF_TX_PBO_PLUS_OVERRULE_ENA(1);
            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            break;
        }
    }

    switch (mode)
    {
        case AK_RAP_TX_PA_MODE_THERMO:
        case AK_RAP_TX_PA_MODE_1HOT:
        {
            GP_WB_WRITE_AK_PAT_TX_POWER_P_EN(1);
            GP_WB_WRITE_AK_PAT_TX_POWER_N_EN(1);
            break;
        }
        case AK_RAP_TX_PA_MODE_THERMO_PLUS:
        case AK_RAP_TX_PA_MODE_1HOT_PLUS:
        {
            GP_WB_WRITE_AK_PAT_TX_POWER_P_EN(1);
            GP_WB_WRITE_AK_PAT_TX_POWER_N_EN(0);
            break;
        }
        case AK_RAP_TX_PA_MODE_THERMO_MINUS:
        case AK_RAP_TX_PA_MODE_1HOT_MINUS:
        {
            GP_WB_WRITE_AK_PAT_TX_POWER_P_EN(0);
            GP_WB_WRITE_AK_PAT_TX_POWER_N_EN(1);
            break;
        }
        case AK_RAP_TX_PA_MODE_OFF:
        {
            GP_WB_WRITE_AK_PAT_TX_POWER_P_EN(0);
            GP_WB_WRITE_AK_PAT_TX_POWER_N_EN(0);
            break;
        }
        default:
        {
            rap_h_assert_sys(0);
            break;
        }
    }
}

