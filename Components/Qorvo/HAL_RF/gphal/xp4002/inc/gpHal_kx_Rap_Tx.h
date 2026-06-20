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


#ifndef _GPHAL_KX_RAP_TX_H_
#define _GPHAL_KX_RAP_TX_H_


void ak_rap_tx_config_antenna(Bool se);

void ak_rap_tx_cw_enable(UInt8 chidx, Bool antenna, UInt8 modulation_type, Bool tx);

void ak_rap_tx_cw_disable(void);

void ak_rap_tx_set_psf_coeffs(UInt8 filter, const UInt16* coeffs);

void ak_rap_tx_config_psf(UInt8 sx_channel);

UInt32 ak_rap_tx_power2pbo(UInt8 mode, UInt8 power);

void ak_rap_tx_config_pa(UInt8 mode, UInt8 power);


#endif //_GPHAL_KX_RAP_TX_H_
