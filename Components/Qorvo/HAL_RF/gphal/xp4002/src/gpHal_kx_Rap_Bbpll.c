/*
 * Copyright (c) 2022, Qorvo Inc
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

void rap_bbpll_init(void)
{
    Int32 coarse = 0;
    GP_WB_WRITE_BBPLL_BIAS_FILT_BYPASS_DISABLE(0);
    GP_WB_WRITE_BBPLL_LDO_PUP(1);
    GP_WB_WRITE_BBPLL_BIAS_PUP(1);
    GP_WB_WRITE_BBPLL_DCO_PUP(1);
    GP_WB_WRITE_BBPLL_BIAS_AMP_SPEEDUP(1);
    GP_WB_WRITE_BBPLL_DCRO_RDY_EN(1);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_BBPLL_DCO_RDY() != 1), 100);
    GP_WB_WRITE_BBPLL_BIAS_FILT_BYPASS_DISABLE(1);
    GP_WB_WRITE_BBPLL_DCRO_RDY_EN(0);
    rap_h_wait_us(32);
    coarse = GP_WB_READ_BBPLL_DCO_COARSE();
    GP_WB_WRITE_STANDBY_RESET_STARTUP_CLKGEN_BBPLL(1);
    GP_WB_WRITE_BBPLL_MAIN_ENABLE(1);
    GP_WB_WRITE_STANDBY_RESET_STARTUP_CLKGEN_BBPLL(0);
    GP_WB_WRITE_BBPLL_DCO_COARSE_NEW(coarse);
    GP_WB_WRITE_BBPLL_FE_ENABLE(1);
}

void rap_bbpll_deinit(void)
{
    GP_WB_WRITE_BBPLL_FE_ENABLE(0);
    GP_WB_WRITE_BBPLL_MAIN_ENABLE(0);
    GP_WB_WRITE_BBPLL_DCRO_RDY_EN(0);
    GP_WB_WRITE_BBPLL_BIAS_FILT_BYPASS_DISABLE(0);
    GP_WB_WRITE_BBPLL_LDO_PUP(0);
    GP_WB_WRITE_BBPLL_BIAS_PUP(0);
    GP_WB_WRITE_BBPLL_BIAS_AMP_SPEEDUP(0);
    GP_WB_WRITE_BBPLL_DCO_PUP(0);
}

void rap_bbpll_calibrate(void)
{
    Int32 coarse = 0;
    Int32 fe_err = 0;
    Int32 fe_result;
    Int32 step = 1;
    GP_WB_WRITE_BBPLL_SOFT_POR_BY_BBPLL_FOOR_ENA(0);
    GP_WB_WRITE_BBPLL_LOOP_IS_CLOSED(0);
    GP_WB_WRITE_BBPLL_FREQ_OUT_OF_RANGE_FE_MAX((768+(4*4)));
    GP_WB_WRITE_BBPLL_FREQ_OUT_OF_RANGE_FE_MIN((768-(4*4)));
    GP_WB_WRITE_BBPLL_F_ESTIMATION_DURATION(4);
    coarse = GP_WB_READ_BBPLL_DCO_COARSE();
    coarse = coarse & 31;
    rap_h_wait_us(15);
    GP_WB_BBPLL_START_FE();
    rap_h_wait_us(4);
    fe_result = GP_WB_READ_BBPLL_FE_RESULT();
    fe_err = fe_result - 768;
    if (fe_result >= 768)
    {
        step = -1;
    }
    if (coarse == 0)
    {
        step = +1;
    }
    if (coarse == 31)
    {
        step = -1;
    }
    while (true)
    {
        coarse = coarse + step;
        GP_WB_WRITE_BBPLL_DCO_COARSE_NEW(coarse);
        rap_h_wait_us(2);
        GP_WB_BBPLL_START_FE();
        rap_h_wait_us(4);
        fe_result = GP_WB_READ_BBPLL_FE_RESULT();
        if (step == +1)
        {
            if ((fe_result >= 768) || (coarse == 31))
            {
                break;
            }
        }
        else
        {
            if ((fe_result <= 768) || (coarse == 0))
            {
                break;
            }
        }
        fe_err = fe_result - 768;
    }
    if (abs(fe_result - 768) > abs(fe_err))
    {
        coarse = coarse - step;
        GP_WB_WRITE_BBPLL_DCO_COARSE_NEW(coarse);
        rap_h_wait_us(2);
    }
}

void rap_bbpll_close_loop(void)
{
    rap_h_wait_us(10);
    GP_WB_WRITE_BBPLL_SOFT_POR_BY_BBPLL_FOOR_ENA(0);
    GP_WB_WRITE_BBPLL_LOOP_ALLOW_FOR_FREQ_OUT_OF_RANGE_COARSE_UPDATE(1);
    GP_WB_WRITE_BBPLL_LOOP_ALLOW_FOR_TUNE_OUT_OF_RANGE_COARSE_UPDATE(1);
    GP_WB_WRITE_BBPLL_LOOP_IS_CLOSED(1);
    GP_WB_BBPLL_CLR_IN_LOCK();
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_BBPLL_UNMASKED_IN_LOCK_INTERRUPT() != 1), 10000);
    GP_WB_BBPLL_CLR_IN_LOCK();

    rap_h_wait_us(2);

    GP_WB_BBPLL_CLR_FREQ_OUT_OF_RANGE_LO();
    GP_WB_BBPLL_CLR_FREQ_OUT_OF_RANGE_HI();
    GP_WB_BBPLL_CLR_FREQ_OUT_OF_RANGE_TIMEOUT();
    GP_WB_BBPLL_CLR_OUT_OF_LOCK();

    GP_WB_WRITE_BBPLL_SOFT_POR_BY_BBPLL_FOOR_ENA(1);
    GP_WB_BBPLL_CLR_COARSE_OUT_OF_RANGE_HI();
    GP_WB_BBPLL_CLR_COARSE_OUT_OF_RANGE_LO();
    GP_WB_BBPLL_CLR_TUNE_OUT_OF_RANGE_HI();
    GP_WB_BBPLL_CLR_TUNE_OUT_OF_RANGE_LO();
    GP_WB_BBPLL_CLR_FREQ_OUT_OF_RANGE_CNT();
    GP_WB_BBPLL_CLR_TUNE_OUT_OF_RANGE_CNT();

    GP_WB_WRITE_BBPLL_CP_CLK_EN(1);
}

void rap_bbpll_open_loop(void)
{
    GP_WB_WRITE_BBPLL_CP_CLK_EN(0);
    GP_WB_WRITE_BBPLL_SOFT_POR_BY_BBPLL_FOOR_ENA(0);
    GP_WB_WRITE_BBPLL_LOOP_IS_CLOSED(0);
}

void rap_bbpll_start(void)
{
    rap_bbpll_init();
    rap_bbpll_calibrate();
    rap_bbpll_close_loop();
}


void rap_bbpll_stop(void)
{
    rap_bbpll_open_loop();
    rap_bbpll_deinit();
}

void rap_bbpll_resume(void)
{
    rap_bbpll_init();
    rap_bbpll_close_loop();
}

