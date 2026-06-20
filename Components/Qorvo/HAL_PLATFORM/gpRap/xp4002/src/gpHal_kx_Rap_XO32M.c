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

void rap_xo32m_enable_launch(void)
{
    GP_WB_WRITE_XO32M_BUF_PUP(0);
    GP_WB_WRITE_XO32M_LDO_REFBITS(15);
    GP_WB_WRITE_XO32M_LDO_PUP(1);
    GP_WB_WRITE_XO32M_BIAS_PUP(1);
    GP_WB_WRITE_XO32M_AGC_PUP(1);
}

void rap_xo32m_enable_join(void)
{
    UInt8 ldo_refbits;
    UInt32 cal_nvm_ptr;
    rap_h_wait_us(100);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_XO32M_CLK_XT32M_RDY() != 1), 10000);
    rap_h_wait_us(10);
    GP_WB_WRITE_XO32M_BUF_PUP(1);
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    ldo_refbits = GP_WB_READ_CAL_NVM_XO32M_LDO_REFBITS(cal_nvm_ptr);
    GP_WB_WRITE_XO32M_LDO_REFBITS(ldo_refbits);
}

void rap_xo32m_enable(void)
{
    rap_xo32m_enable_launch();
    rap_xo32m_enable_join();
}

void rap_xo32m_disable(void)
{
    GP_WB_WRITE_XO32M_BUF_PUP(0);
    GP_WB_WRITE_XO32M_AGC_PUP(0);
    GP_WB_WRITE_XO32M_BIAS_PUP(0);
    GP_WB_WRITE_XO32M_LDO_PUP(0);
}



