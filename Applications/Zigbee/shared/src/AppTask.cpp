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

/** @file "AppTask.xpp"
 */

#include "hal.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpReset.h"
#include "gpSched.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "queue.h"

#include "qPinCfg.h"
#include "AppButtons.h"
#include "AppManager.h"
#include "AppTask.h"

#if defined(GP_APP_DIVERSITY_RESETCOUNTING)
#include "ResetCount.h"
#endif

#include "ZigbeeTask.h"
#include "hal_PowerMode.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/** @brief Macro Define for 1 second */
#define ONE_SECOND 1000000 // 1 s

#define APP_EVENT_QUEUE_SIZE 20
#define APP_TASK_STACK_SIZE  (4 * 1024)
#define APP_TASK_PRIORITY    2

/** @brief Threshold to check of time of inactivity before going to sleep. (in us)*/
#ifndef APP_GOTOSLEEP_THRESHOLD
#define APP_GOTOSLEEP_THRESHOLD 1000
#endif

#define MAX_GPIO_CALLBACKS 3

#define _PRINT_APP_VERSION(_major, _minor, _revision, _patch)                                                          \
    {                                                                                                                  \
        GP_LOG_SYSTEM_PRINTF("Current Software Version: %d.%d.%d.%d", 0, _major, _minor, _revision, _patch);           \
    }
#define PRINT_APP_VERSION(_args) _PRINT_APP_VERSION(_args)

namespace {
uint8_t sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];
StaticQueue_t sAppEventQueueStruct;
QueueHandle_t sAppEventQueue;

StackType_t appStack[APP_TASK_STACK_SIZE / sizeof(StackType_t)];
StaticTask_t appTaskStruct;
TaskHandle_t sAppTaskHandle;
} // namespace

AppTask AppTask::sAppTask;

AppError AppTask::Init()
{
#if defined(GP_APP_DIVERSITY_RESETCOUNTING)
    ResetCount_Init();
#endif

    PRINT_APP_VERSION(GP_VERSIONINFO_GLOBAL_VERSION);

    sAppEventQueue =
        xQueueCreateStatic(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent), sAppEventQueueBuffer, &sAppEventQueueStruct);
    if(sAppEventQueue == nullptr)
    {
        GP_LOG_SYSTEM_PRINTF("Failed to allocate app event queue", 0);
        return APP_NO_MEMORY;
    }

    // Start App task.
    sAppTaskHandle = xTaskCreateStatic(Main, APP_TASK_NAME, (sizeof(appStack) / sizeof((appStack)[0])), nullptr, 1,
                                       appStack, &appTaskStruct);
    if(sAppTaskHandle == nullptr)
    {
        return APP_NO_MEMORY;
    }

#if defined(GP_APP_DIVERSITY_BUTTONHANDLER)
    // Setup button handler
    GetAppButtons().Init();
#endif

    // Init Application
    GetAppMgr().Init();

    // Init Zigbee Task
    ZigbeeTaskMgr().Init();

    GetAppMgr().UpdateLEDs();


    return APP_NO_ERROR;
}

void AppTask::EnableSleep(bool enable)
{
    if(enable)
    {
#if defined(GP_DIVERSITY_GPHAL_XP4002)
        hal_PowerModeResult_t hal_pm_ret_val = hal_PowerModeResultSuccess;

        hal_pm_ret_val = hal_SetPowerMode(hal_StandbyPowerModeELPS, hal_ActivePowerModeEHPS, hal_SleepModeRC);
        GP_ASSERT(GP_DIVERSITY_ASSERT_LEVEL_SYSTEM, hal_PowerModeResultSuccess == hal_pm_ret_val);
#else
        if(!gpHal_32kHzCrystalAvailable())
        {
            /* Select internal 32kHz RC oscillator */
            gpHal_SetSleepMode(gpHal_SleepModeRC);
        }
#endif
        hal_SleepSetGotoSleepThreshold(APP_GOTOSLEEP_THRESHOLD);
    }
    hal_SleepSetGotoSleepEnable(enable);
}

void AppTask::Main(void* pvParameter)
{
    AppEvent event;

    while(true)
    {
        if(xQueueReceive(sAppEventQueue, &event, portMAX_DELAY) == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
        }
    }
}

void AppTask::PostEvent(AppEvent* aEvent)
{
    if((aEvent == NULL) || (aEvent->Type == AppEvent::kEventType_Invalid))
    {
        return;
    }

    if(sAppEventQueue != nullptr)
    {
        xPSR_Type psr;
        psr.w = __get_xPSR();

        if(psr.b.ISR != 0)
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            if(!xQueueSendFromISR(sAppEventQueue, aEvent, &xHigherPriorityTaskWoken))
            {
                GP_LOG_SYSTEM_PRINTF("IRQ Failed to post event", 0);
            }
            else
            {
                if(xHigherPriorityTaskWoken)
                {
                    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                }
            }
        }
        else
        {
            if(!xQueueSend(sAppEventQueue, aEvent, 1))
            {
                GP_LOG_SYSTEM_PRINTF("Failed to post event to app task event queue", 0);
            }
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Event Queue is nullptr should never happen", 0);
    }
}

void AppTask::DispatchEvent(AppEvent* aEvent)
{
    GetAppMgr().EventHandler(aEvent);
}

void AppTask::FactoryReset(void)
{
    // Init Zigbee Task
    ZigbeeTaskMgr().FactoryReset();


    ResetSystem();
}

void AppTask::ResetSystem(void)
{
    gpSched_ScheduleEvent(0, gpReset_ResetBySwPor);
}

static qDrvGPIO_CbInput_t callbackGPIO[MAX_GPIO_CALLBACKS];

static void AppTask_DeferredGPIOCallback(void* arg, uint32_t ulParameter2)
{
    uint8_t* gpio = (uint8_t*)arg;

    for(uint8_t i = 0; i < MAX_GPIO_CALLBACKS; i++)
    {
        if(callbackGPIO[i])
        {
            callbackGPIO[i](NULL, *gpio);
        }
    }
}

void AppTask_GPIOCallback(void* arg, uint8_t gpio)
{
    static uint8_t gpioinfo;

    gpioinfo = gpio;

    // The actual processing is to be deferred to a task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerPendFunctionCallFromISR(AppTask_DeferredGPIOCallback, &gpioinfo, 0, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

AppError AppTask_RegisterGPIOCallback(qDrvGPIO_CbInput_t pCallbacks)
{
    for(uint8_t i = 0; i < MAX_GPIO_CALLBACKS; i++)
    {
        if(callbackGPIO[i] == NULL)
        {
            callbackGPIO[i] = pCallbacks;
            return APP_NO_ERROR;
        }
    }

    return APP_NO_MEMORY;
}
