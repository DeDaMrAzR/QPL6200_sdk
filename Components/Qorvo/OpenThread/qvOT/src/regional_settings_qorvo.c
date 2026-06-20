/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017-2018, Qorvo Inc
 *
 * regional_settings_qorvo.c
 *
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by
 * Qorvo Inc.
 *
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVOT
// #define GP_LOCAL_LOG

#ifdef GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS

#include "gpLog.h"
#include "regional_settings_qorvo.h"

const Int8 __attribute__((weak)) cPowerSettingTable[][16] = {
    // China, ETSI, ARIB
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    // FCC, Matter on channel 26, 18.5 dBm is allowed, rounding down
    // Note: Zigbee has a higher duty cycle and therefor only 16.5dBm can be emitted on channel 26)
    {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 18}};

const Int8* qvOT_MapCountryToPowerTable(uint16_t country_code)
{
    // By using FCC_COUNTRIES_ALL, we can include all FCC countries at the cost of some additional storage
    uint16_t fcc_countries[] = {FCC_COUNTRIES_MAJOR};

    for(int i = 0; i < sizeof(fcc_countries) / sizeof(fcc_countries[0]); i++)
    {
        if(fcc_countries[i] == country_code)
        {
            GP_LOG_PRINTF("FCC country: %c%c", 0, UINT16_TO_ISO3166(country_code));
            return cPowerSettingTable[1];
        }
    }
    GP_LOG_PRINTF("ETSI country: %c%c", 0, UINT16_TO_ISO3166(country_code));
    return cPowerSettingTable[0];
};

#endif // GP_MACCORE_DIVERSITY_REGIONALDOMAINSETTINGS
