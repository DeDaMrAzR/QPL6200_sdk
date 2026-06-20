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

void rap_tsens_init(void)
{
    UInt8 bist_iref_pup;
    bist_iref_pup = GP_WB_READ_BIST_IREF_PUP();
    rap_h_assert_sys(bist_iref_pup == 1);

    GP_WB_WRITE_BIST_TSENS_BIAS_DIST_PUP(1);
    GP_WB_WRITE_BIST_TSENS_LDO_PUP(1);
    GP_WB_WRITE_BIST_TSENS_PUP(1);
    GP_WB_WRITE_BIST_TSENS_BIAS_VBE_PUP(1);
    GP_WB_WRITE_BIST_TSENS_BIAS_DVBE_PUP(1);
    rap_h_wait_us(10);
}

void rap_tsens_deinit(void)
{
    GP_WB_WRITE_BIST_TSENS_BIAS_DVBE_PUP(0);
    GP_WB_WRITE_BIST_TSENS_BIAS_VBE_PUP(0);
    GP_WB_WRITE_BIST_TSENS_PUP(0);
    GP_WB_WRITE_BIST_TSENS_LDO_PUP(0);
    GP_WB_WRITE_BIST_TSENS_BIAS_DIST_PUP(0);
}

void rap_tsens_measure_launch(UInt8 cycles)
{
    GP_WB_WRITE_BIST_TSENS_HANNING_CYCLES_N(cycles);
    GP_WB_BIST_TSENS_DO_SINGLE_MEASUREMENT();
}

UInt16 rap_tsens_measure_join(void)
{
    UInt16 res;
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_BIST_UNMASKED_TSENS_RESULT_FIFO_NOT_EMPTY_INTERRUPT() != 1), 150);
    res = GP_WB_READ_BIST_TSENS_RESULT();
    return res;
}

UInt16 rap_tsens_measure(UInt8 cycles)
{
    UInt16 res;
    rap_tsens_measure_launch(cycles);
    res = rap_tsens_measure_join();
    return res;
}

Int32 rap_tsens_measure_deg(UInt8 cycles)
{
    UInt16 res;
    Int32 res_deg;
    res = rap_tsens_measure(cycles);
    res_deg = rap_tsens_to_deg(res);
    return res_deg;
}

Int32 rap_tsens_to_deg(UInt16 res)
{
    Int32 tsens_offset;
    Int32 res_deg;
    Int32 res_int;
    UInt32 cal_nvm_ptr;
    res_int = res;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    tsens_offset = GP_WB_READ_CAL_NVM_BIST_TSENS_OFFSET(cal_nvm_ptr);
    res_deg = (((res_int - tsens_offset - 3712) << 16) / 3458);
    return res_deg;
}

UInt16 rap_tsens_from_deg(Int32 res_deg)
{
    Int32 tsens_offset;
    Int32 res;
    UInt32 cal_nvm_ptr;
    cal_nvm_ptr = rap_h_get_cal_nvm_ptr();
    tsens_offset = GP_WB_READ_CAL_NVM_BIST_TSENS_OFFSET(cal_nvm_ptr);
    res = (((res_deg * 3458) >> 8) + 3712 + tsens_offset);
    return res;
}

