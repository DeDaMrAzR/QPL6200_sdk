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


#ifndef _GPHAL_KX_RAP_RX_H_
#define _GPHAL_KX_RAP_RX_H_




void ak_rap_rx_adc_set_sample_rate(Bool sample_rate_32M);


void ak_rap_rx_if_set_mode(Bool zif_not_lif);

void ak_rap_rx_zb_set_mode(UInt8 rx_mode);


void ak_rap_rx_ble_set_mode(UInt8 rx_mode);

void ak_rap_rx_antenna_set_mode(UInt8 rx_mode);

void ak_rap_rx_set_mode(UInt8 rx_mode);

UInt8 ak_rap_rx_get_rx_on_when_idle(void);

void ak_rap_rx_set_rx_on_when_idle(UInt8 rx_on_mask);

void ak_rap_rx_set_rx_on_when_idle_chidx(UInt8 chidx, Bool rx_on);

Int8 ak_rap_rx_rssi_to_dbm(UInt8 rssi);

UInt8 ak_rap_rx_dbm_to_rssi(Int8 dbm);


#endif //_GPHAL_KX_RAP_RX_H_
