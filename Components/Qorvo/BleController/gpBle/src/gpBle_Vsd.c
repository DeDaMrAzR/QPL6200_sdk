/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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
 * $Header$
 * $Change$
 * $DateTime$
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

//#define GP_LOCAL_LOG

#include "gpBle.h"
#include "gpBle_defs.h"
#include "gpSched.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLE

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
static UInt32 Ble_RfCalibrationIntervalMs;
#endif //GP_DIVERSITY_DEVELOPMENT

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
static void Ble_TriggerPeriodicCalibrations(void);
#endif //GP_DIVERSITY_DEVELOPMENT

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
void Ble_TriggerPeriodicCalibrations(void)
{
#ifdef GP_DIVERSITY_GPHAL_XP4002
    void gpAppUcSubsystem_TriggerCalibrations(void);
    gpAppUcSubsystem_TriggerCalibrations();
#endif
    gpSched_ScheduleEvent(MS_TO_US(Ble_RfCalibrationIntervalMs), Ble_TriggerPeriodicCalibrations);
}
#endif //GP_DIVERSITY_DEVELOPMENT

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpBle_VsdReset(void)
{
#ifdef GP_DIVERSITY_DEVELOPMENT
    gpSched_UnscheduleEvent(Ble_TriggerPeriodicCalibrations);
#endif //GP_DIVERSITY_DEVELOPMENT
}

/*****************************************************************************
 *                    Service Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_DEVELOPMENT
gpHci_Result_t gpBle_VsdEnablePeriodicRfCalibration(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    Ble_RfCalibrationIntervalMs = pParams->VsdEnablePeriodicRfCalibration.intervalMs;

    gpSched_UnscheduleEvent(Ble_TriggerPeriodicCalibrations);

    if(Ble_RfCalibrationIntervalMs != 0)
    {
        gpSched_ScheduleEvent(MS_TO_US(Ble_RfCalibrationIntervalMs), Ble_TriggerPeriodicCalibrations);
    }

    return gpHci_ResultSuccess;
}
#endif //GP_DIVERSITY_DEVELOPMENT
