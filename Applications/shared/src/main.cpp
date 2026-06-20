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

/** @file "main.c"
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

#include "hal.h"
#include "gpHal_DEFS.h"
#include "gpBaseComps.h"
#include "gpSched.h"
#include "gpCom.h"
#include "gpUpgrade.h"

#if defined(GP_APP_DIVERSITY_EARLY_RESTORE_LIGHT)
#include "LightingManager.h"
#endif // GP_APP_DIVERSITY_EARLY_RESTORE_LIGHT

#include "gpLog.h"

#ifdef GP_COMP_NVM
#include "gpNvm.h"
#endif // GP_COMP_NVM

#include "qPinCfg.h"


#include "hal_PowerMode.h"

// Application level logic
#include "AppTask.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Function Definitions
 *****************************************************************************/
#if defined(GP_APP_DIVERSITY_ZIGBEE)
extern "C" int __cfcore_low_level_init(void);
#endif // GP_APP_DIVERSITY_ZIGBEE

/*****************************************************************************
 *                    static Function Definitions
 *****************************************************************************/
#if defined(GP_SCHED_DIVERSITY_SLEEP) || defined(GP_FREERTOS_DIVERSITY_SLEEP)
static void EnableSleep(bool enable)
{
    if(enable)
    {
        // configure sleep mode
        hal_PowerModeResult_t hal_pm_ret_val = hal_PowerModeResultSuccess;

        hal_pm_ret_val = hal_SetPowerMode(hal_StandbyPowerModeELPS, hal_ActivePowerModeEHPS, hal_SleepModeRC);
        GP_ASSERT(GP_DIVERSITY_ASSERT_LEVEL_SYSTEM, hal_PowerModeResultSuccess == hal_pm_ret_val);
    }

    // Enable sleep behavior
    hal_SleepSetGotoSleepEnable(enable);
}
#endif

/*****************************************************************************
 *                    Application Function Definitions
 *****************************************************************************/
extern "C" void PreMain_Init(void)
{
#if defined(GP_APP_DIVERSITY_ZIGBEE)
    __cfcore_low_level_init();
#endif // GP_APP_DIVERSITY_ZIGBEE
}

void Application_Init(void)
{
    AppError error;

    /* Taken from gpSched task - Init has to be completed */

    gpHal_Set32kHzCrystalAvailable(false);

    gpBaseComps_StackInit();

    gpCom_Init();

#ifdef GP_DIVERSITY_LOG
    gpLog_Init();
#endif


#if defined(GP_APP_DIVERSITY_EARLY_RESTORE_LIGHT)
    /* Initialize light manager her to restore LED status ASAP */
    LightingMgr().Init();
#endif // GP_APP_DIVERSITY_EARLY_RESTORE_LIGHT

    gpUpgrade_Init();

#if defined(GP_SCHED_DIVERSITY_SLEEP) || defined(GP_FREERTOS_DIVERSITY_SLEEP)
    EnableSleep(true);
#endif

    /* Initialize IO */
    qResult_t res = qPinCfg_Init(NULL);
    if(res != Q_OK)
    {
        GP_LOG_SYSTEM_PRINTF("qPinCfg_Init failed: %d", 0, res);
        Q_ASSERT(false);
    }


    /* Launch application task */
    GP_LOG_SYSTEM_PRINTF("============================", 0);
    GP_LOG_SYSTEM_PRINTF("Qorvo Application_Init", 0);
    GP_LOG_SYSTEM_PRINTF("============================", 0);

    error = GetAppTask().Init();
    if(error != APP_NO_ERROR)
    {
        GP_LOG_SYSTEM_PRINTF("GetAppTask().Init() failed", 0);
        return;
    }
}

/*****************************************************************************
 * --- Main
 *****************************************************************************/
int main(void)
{
    int result;

    /* Initialize Qorvo stack */
    HAL_INITIALIZE_GLOBAL_INT();

    // Hardware initialization
    HAL_INIT();

    HAL_ENABLE_GLOBAL_INT(); // GP_DIVERSITY_LOG

    gpSched_Init();

    gpSched_ScheduleEvent(0, Application_Init);

    /* Start FreeRTOS */
    vTaskStartScheduler();
    return 0;
}
