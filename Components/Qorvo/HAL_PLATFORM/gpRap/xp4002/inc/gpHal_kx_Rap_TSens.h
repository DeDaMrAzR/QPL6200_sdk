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


#ifndef _GPHAL_KX_RAP_TSENS_H_
#define _GPHAL_KX_RAP_TSENS_H_

void rap_tsens_init(void);

void rap_tsens_deinit(void);

void rap_tsens_measure_launch(UInt8 cycles);

UInt16 rap_tsens_measure_join(void);

UInt16 rap_tsens_measure(UInt8 cycles);

Int32 rap_tsens_measure_deg(UInt8 cycles);

Int32 rap_tsens_to_deg(UInt16 res);

UInt16 rap_tsens_from_deg(Int32 res_deg);


#endif //_GPHAL_KX_RAP_TSENS_H_
