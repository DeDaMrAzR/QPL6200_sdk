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


#ifndef _GPHAL_KX_RAP_SX_H_
#define _GPHAL_KX_RAP_SX_H_


void ak_rap_sx_set_channel_launch(UInt8 idx, UInt8 sx_channel);

void ak_rap_sx_set_channel_join(void);

void ak_rap_sx_set_channel(UInt8 idx, UInt8 sx_channel);

void ak_rap_sx_cal_nrt(void);

UInt32 ak_rap_sx_measure_freq_khz(UInt8 duration);


#endif //_GPHAL_KX_RAP_SX_H_
