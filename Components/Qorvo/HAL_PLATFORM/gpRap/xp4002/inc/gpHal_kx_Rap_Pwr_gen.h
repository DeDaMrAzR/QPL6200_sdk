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


#ifndef _GPHAL_KX_RAP_PWR_GEN_H_
#define _GPHAL_KX_RAP_PWR_GEN_H_


void rap_pwr_mps_to_hps(void);

void rap_pwr_hps_to_mps(void);

void rap_pwr_hps_to_ehps(void);

void rap_pwr_ehps_to_hps(void);

void rap_pwr_mps_to_emps(void);

void rap_pwr_emps_to_mps(void);

void rap_pwr_set_active_power_mode(UInt8 active_power_mode);

void rap_pwr_set_standby_power_mode(UInt8 standby_power_mode);

void rap_pwr_temp_batt_update(UInt8 active_power_mode);


#endif //_GPHAL_KX_RAP_PWR_GEN_H_
