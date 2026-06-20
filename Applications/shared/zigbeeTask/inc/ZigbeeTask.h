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

/** @file "ZigbeeTask.h"
 *
 * Defines for the Zigbee task.
 */

#ifndef ZIGBEE_TASK_H
#define ZIGBEE_TASK_H

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "ZigBeeInstallcode.h"
#include "ZigBeeCommissioning.h"
#include "ZigBeeCommissioning_Callback.h"
#include "ZigBeeMessage_Callback.h"
#include "ZigBeeFindingAndBinding.h"
#include "gpNvm.h"
#include "gpNvm_NvmProtect.h"

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/
#ifndef ZIGBEE_ENDPOINT_1
#define ZIGBEE_ENDPOINT_1 1
#endif // ZIGBEE_ENDPOINT_1

// Macro defines for ZB group ID to be used in group control mode
#ifndef ZIGBEE_GROUP_ID
#define ZIGBEE_GROUP_ID 0x0001
#endif // ZIGBEE_GROUP_ID

class ZigbeeTask {
    public:
    typedef enum {
        kZigbeeState_NotReady = 0,
        kZigbeeState_Ready,
        kZigbeeState_Identifying,
        kZigbeeState_Commissioning,
        kZigbeeState_JoinedNetwork,
        kZigbeeState_AssoPermitJoin,
        kZigbeeState_DeviceLeft,
        kZigbeeState_Invalid
    } State_t;

    typedef enum {
        ControlMode_Binding = 0,
        ControlMode_Group,
    } FB_ControlMode_t; // finding and binding control mode

    typedef struct {
        State_t State;
        Bool is_BindingGrouping;
        FB_ControlMode_t controlMode;
    } Event_t;

    void Init(void);
    void OpenCommissioning(void);
    void FactoryReset(void);
    void SetState(State_t state);
    Event_t& GetEvent(void);

    Bool isBindingGrouping(void);
    void EnterBindingGroupingMode(void);
    void LeaveBindingGroupingMode(void);
    void ToggleBindingGroupingMode(void);
    static void LeaveBindingGroupingModeWrapper(void* obj);

    FB_ControlMode_t GetControlMode(void);
    void SetControlMode(FB_ControlMode_t mode);
    void ToggleControlMode(void);
    static void OnIdlePreProcessHandles();


#ifdef GP_NVM_DIVERSITY_ELEMENT_IF
    static Bool NvmZigbeeFactoryResetBusy_DefaultInitializer(const ROM void* pTag, UInt8* pBuffer);
#endif // GP_NVM_DIVERSITY_ELEMENT_IF

    private:
    friend ZigbeeTask& ZigbeeTaskMgr(void);

    static ZigbeeTask sZigbeeTask;
    /** @brief Static to indicate device has enabled Binding/Grouping state or not */
    Event_t mEvent;
};

inline ZigbeeTask& ZigbeeTaskMgr(void)
{
    return ZigbeeTask::sZigbeeTask;
}

extern CPhysicalGreenPeak phy;
extern CIEEE802154MAC mac;
extern CZigBeeNetwork nwk;
extern CZigBeeApplicationSupport aps;
extern CZigBeeDeviceObject zdo;

#ifdef GP_COMP_ZGPSTUB
// GreenPower Proxy
extern CZigBeeGPStub stub;
extern CZigBeeGPProxyApplication zgp;
#endif // GP_COMP_ZGPSTUB

#endif // ZIGBEE_TASK_H
