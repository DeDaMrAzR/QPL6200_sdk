/*
 * Copyright (c) 2024-2025, Qorvo Inc
 *
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
 *
 */

/** @file "ZigbeeEndpoint_BatteryMonitor.h"
 */

#ifndef _ZIGBEE_ENDPOINT_BATTERYMONITOR_H_
#define _ZIGBEE_ENDPOINT_BATTERYMONITOR_H_
#include <stdbool.h>
#include <stdint.h>

template <class T> class ZigbeeEndpoint_BatteryMonitor {
    private:
    T* p_appsEp_1;

    public:
    ZigbeeEndpoint_BatteryMonitor(T* p_Obj)
    {
        p_appsEp_1 = p_Obj;
    };
    ~ZigbeeEndpoint_BatteryMonitor(void){};
    unsigned char GetBatteryVoltage(void)
    {
        return p_appsEp_1->GetBatteryVoltage();
    };
    unsigned char GetBatteryPercentageRemaining(void)
    {
        return p_appsEp_1->GetBatteryPercentageRemaining();
    };
    void SetBatteryPercentageRemaining(unsigned char percentageRemaining)
    {
        p_appsEp_1->SetBatteryPercentageRemaining(percentageRemaining);
    };
    void SetBatteryVoltage(unsigned char voltage)
    {
        p_appsEp_1->SetBatteryVoltage(voltage);
    };
};

#endif //_ZIGBEE_ENDPOINT_BATTERYMONITOR_H_
