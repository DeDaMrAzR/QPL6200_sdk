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

/** @file "ZigbeeEndpoint.h"
 */

#ifndef _ZIGBEE_ENDPOINT_H_
#define _ZIGBEE_ENDPOINT_H_

#include <stdbool.h>
#include <stdint.h>

#include "ZigbeeTask.h"
#include "ZigbeeEndpoint_BatteryMonitor.h"
#if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
#include "ColorSwitchApplication.h"
#endif // defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
#if defined(GP_APPLICATION_DOORWINDOW)
#include "ContactSwitchApplication.h"
#endif // GP_APPLICATION_DOORWINDOW
#if defined(GP_APPLICATION_MOTIONSENSOR)
#include "OnOffSensorApplication.h"
#endif // GP_APPLICATION_MOTIONSENSOR
class ZigbeeEndpoint
#if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
    : public ZigbeeEndpoint_BatteryMonitor<CColorSwitchApplication>
#elif defined(GP_APPLICATION_DOORWINDOW)
    : public ZigbeeEndpoint_BatteryMonitor<CContactSwitchApplication>
#elif defined(GP_APPLICATION_MOTIONSENSOR)
    : public ZigbeeEndpoint_BatteryMonitor<COnOffSensorApplication>
#endif
{
    public:
    void Init(void);
#if defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
    ZigbeeEndpoint(CColorSwitchApplication* pappEp) : ZigbeeEndpoint_BatteryMonitor(pappEp){};
    ~ZigbeeEndpoint(){};
    void SetLevel(uint8_t level);
    void OnOffToggle(void);
    void StartLeveling(unsigned char mode, unsigned char duration = 5);
    void StopLeveling(void);
    static void StopLevelingWrapper(void* obj);
    void StartColorControlMoveHue(unsigned char mode, unsigned char duration = 5);
    static void StopColorControlMoveHueWrapper(void* obj);
    void StopColorControlMoveHue(void);
    void SendAddGroup(CZigBeeAddress destination, unsigned short groupId);
    void StartTouchLinkClientScanning(void);
    void StopTouchLinkClientScanning(void);
    bool IsTouchLinkClientScanning(void);
#endif // defined(GP_APP_DIVERSITY_COLOR_SWITCH) || defined(GP_APPLICATION_SWITCH)
#if defined(GP_APPLICATION_DOORWINDOW)
    ZigbeeEndpoint(CContactSwitchApplication* pappEp) : ZigbeeEndpoint_BatteryMonitor(pappEp){};
    ~ZigbeeEndpoint(){};
    void UpdateIasState(bool alarmed, bool tampered);
#endif
#if defined(GP_APPLICATION_MOTIONSENSOR)
    ZigbeeEndpoint(COnOffSensorApplication* pappEp) : ZigbeeEndpoint_BatteryMonitor(pappEp){};
    ~ZigbeeEndpoint(){};
    void SetOccupancy(bool occupied);
    void SetIlluminance(uint16_t measuredValue);
    void SendAddGroup(CZigBeeAddress destination, unsigned short groupId);
#endif

    private:
    friend ZigbeeEndpoint& ZigbeeEndpointMgr(void);

    static ZigbeeEndpoint sZigbeeEndpoint;
};

inline ZigbeeEndpoint& ZigbeeEndpointMgr(void)
{
    return ZigbeeEndpoint::sZigbeeEndpoint;
}

#endif // _ZIGBEE_ENDPOINT_H_
