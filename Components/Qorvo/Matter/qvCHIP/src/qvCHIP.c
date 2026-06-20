/*
 * Copyright (c) 2020-2021, Qorvo Inc
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
 */

/** @file "qvCHIP.c"
 *
 *  CHIP wrapper API
 *
 *  Implementation of qvCHIP
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVCHIP

/* <CodeGenerator Placeholder> General */
/* </CodeGenerator Placeholder> General */

#include "qvCHIP.h"
#include "hal.h"
#include "hal_PowerMode.h"

#include "gpHal.h"
#include "gpHal_DEFS.h"

#include "gpBaseComps.h"
#include "gpSched.h"
#include "gpVersion.h"
#include "gpLog.h"
#include "gpCom.h"
#include "gpReset.h"
#include "gpRandom.h"
#include "gpUpgrade.h"

#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "gpNvm.h"
#endif

#if !defined(GP_BASECOMPS_DIVERSITY_NO_GPCOM_INIT) || !defined(GP_BASECOMPS_DIVERSITY_NO_GPLOG_INIT)
#error "gpCom and gpLog components get initialized during qvCHIP_init! Make sure diversities are set so these \
        components does not get initialized in gpBaseComps_StackInit."
#endif //! defined(GP_BASECOMPS_DIVERSITY_NO_GPCOM_INIT) || !defined(GP_BASECOMPS_DIVERSITY_NO_GPLOG_INIT)
#if !defined(GP_BASECOMPS_DIVERSITY_NO_GPSCHED_INIT)
#error "gpSched is initialized during qvCHIP_init! It should only be done once, a missing diversity needs to be set"
#endif

/* </CodeGenerator Placeholder> Includes */

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> Macro */
/** @brief Threshold to check of time of inactivity before going to sleep. (in us)*/
#ifndef QVCHIP_APP_GOTOSLEEP_THRESHOLD
#define QVCHIP_APP_GOTOSLEEP_THRESHOLD 1000
#endif
/* </CodeGenerator Placeholder> Macro */

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> FunctionalMacro */
/* </CodeGenerator Placeholder> FunctionalMacro */

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> TypeDefinitions */
/* </CodeGenerator Placeholder> TypeDefinitions */

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticData */
static size_t internalWatermarkOffset = 0;
static bool sleepEnabled = false;
static bool deviceSleepDisabledAlways = false;
/* </CodeGenerator Placeholder> StaticData */

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticFunctionPrototypes */
/* </CodeGenerator Placeholder> StaticFunctionPrototypes */

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/* <CodeGenerator Placeholder> StaticFunctionDefinitions */

/** @brief Print wrapper information
 */
void CHIP_Info(void)
{
    gpVersion_SoftwareInfo_t appInfo;
    UInt8 matterRomVersion = 0;
    UInt8 minMatterRomVersion = 0;
#ifdef GP_DIVERSITY_LOG
    UInt8 nrtRomVersion = 0;
    nrtRomVersion = gpVersion_GetNrtRomVersion();
#endif // GP_DIVERSITY_LOG

    // Print version info
    gpVersion_GetSoftwareInfo(&appInfo);

    matterRomVersion = gpVersion_GetMatterRomVersion();
    minMatterRomVersion = gpVersion_GetMinimalMatterRomVersion();

    if((matterRomVersion == 0xFF) || ((matterRomVersion != 0) && (matterRomVersion < minMatterRomVersion)))
    {
        GP_LOG_SYSTEM_PRINTF("Wrong ROM version detected, ROMv%d or higher expected (%d)", 0, minMatterRomVersion,
                             matterRomVersion);
        GP_ASSERT_SYSTEM(false);
    }

    GP_LOG_SYSTEM_PRINTF("qvCHIP v%u.%u.%u.%u ROMv%u/%u (CL:%lu) r:%x", 0, appInfo.version.major, appInfo.version.minor,
                         appInfo.version.revision, appInfo.version.patch, nrtRomVersion, matterRomVersion,
                         (appInfo.changeList >> 8), // LSB reserved
                         gpReset_GetResetReason());

    gpLog_Flush();
}

static void qvCHIP_deferred_initialisation(void* pArg)
{
    // Initialize gpCom/gpLog before gpBaseComps_StackInit() get logging enabled.
    // This allows to indicate ROM version compatibility immediately to the user.
    gpCom_Init();
#ifdef GP_DIVERSITY_LOG
    gpLog_Init();
#endif                               // GP_DIVERSITY_LOG
    qvCHIP_NvmSetVariableSettings(); // needs to be called before gpNvm_Init

    gpHal_Set32kHzCrystalAvailable(false);

    gpBaseComps_StackInit();

#if defined(GP_DIVERSITY_GPHAL_XP4002)
    gpNvm_Init();
#endif // QPG6200

    qvCHIP_KvsInit();

    gpUpgrade_Init();

    CHIP_Info();

    if(pArg)
    {
        gpSched_ScheduleEvent(0, (application_init_callback_t)pArg);
    }
}
/* </CodeGenerator Placeholder> StaticFunctionDefinitions */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

int qvCHIP_init(application_init_callback_t application_init_callback)
{
    /* <CodeGenerator Placeholder> Implementation_qvCHIP_init */

    /* Taken from gpSched task - Init has to be completed */
    HAL_INITIALIZE_GLOBAL_INT();

    // Hardware initialization
    HAL_INIT();

    HAL_ENABLE_GLOBAL_INT();

    /* Initialize gpSched already so we can schedule callbacks already */
    gpSched_Init();
    /* Make sure to run the stack-intensive initialisation code from the scheduler task with larger stack */
    gpSched_ScheduleEventArg(0, qvCHIP_deferred_initialisation, application_init_callback);

    return 0;
    /* </CodeGenerator Placeholder> Implementation_qvCHIP_init */
}
void qvCHIP_Printf(uint8_t module, const char* formattedMsg)
{
    /* <CodeGenerator Placeholder> Implementation_qvCHIP_Printf */
    const char* newLine = "\r\n";
    NOT_USED(module);

    gpCom_Flush();
    gpCom_DataRequest(GP_COMPONENT_ID, strlen(formattedMsg), (UInt8*)formattedMsg, GP_COM_DEFAULT_COMMUNICATION_ID);
    gpCom_DataRequest(GP_COMPONENT_ID, strlen(newLine), (UInt8*)newLine, GP_COM_DEFAULT_COMMUNICATION_ID);
    /* </CodeGenerator Placeholder> Implementation_qvCHIP_Printf */
}

qvStatus_t qvCHIP_RandomGet(uint8_t outputLength, uint8_t* pOutput)
{
    /* <CodeGenerator Placeholder> Implementation_qvCHIP_RandomGet */
    if(NULL == pOutput || 0 == outputLength)
    {
        GP_ASSERT_SYSTEM(false);
        return QV_STATUS_INVALID_ARGUMENT;
    }

    gpRandom_GetNewSequence(outputLength, pOutput);

    return QV_STATUS_NO_ERROR;
    /* </CodeGenerator Placeholder> Implementation_qvCHIP_RandomGet */
}

qvStatus_t qvCHIP_RandomGetDRBG(uint8_t outputLength, uint8_t* pOutput)
{
    /* <CodeGenerator Placeholder> ImplementationqvCHIP_RandomGetDRBG */
    if(NULL == pOutput || 0 == outputLength)
    {
        GP_ASSERT_SYSTEM(false);
        return QV_STATUS_INVALID_ARGUMENT;
    }

    gpRandom_GetFromDRBG(outputLength, pOutput);

    return QV_STATUS_NO_ERROR;
    /* </CodeGenerator Placeholder> ImplementationqvCHIP_RandomGetDRBG */
}

void qvCHIP_ResetSystem(void)
{
    /* <CodeGenerator Placeholder> Implementation_qvCHIP_ResetSystem */
    gpReset_ResetBySwPor();
    /* </CodeGenerator Placeholder> Implementation_qvCHIP_ResetSystem */
}

bool qvCHIP_GetHeapStats(size_t* pHeapFree, size_t* pHeapUsed, size_t* pHighWatermark)
{
    /* <CodeGenerator Placeholder> Implementation_qvCHIP_GetHeapStats */
    size_t maxHeapAvailable;
    if((pHeapFree == NULL) || (pHeapUsed == NULL) || (pHighWatermark == NULL))
    {
        GP_ASSERT_SYSTEM(false);
        return false;
    }

    hal_GetHeapInUse((uint32_t*)pHeapUsed, (uint32_t*)pHighWatermark, (uint32_t*)&maxHeapAvailable);
    *pHeapFree = maxHeapAvailable - *pHeapUsed;

    // Workaround to allow resetting high watermark by using an internal offset between the actual value
    // and the new reset value (which will take the value of the currently used heap)
    if(internalWatermarkOffset > 0)
    {
        if((*pHighWatermark - internalWatermarkOffset) < *pHeapUsed)
        {
            internalWatermarkOffset = *pHighWatermark - *pHeapUsed;
        }
        *pHighWatermark -= internalWatermarkOffset;
    }
    return true;
    /* </CodeGenerator Placeholder> Implementation_qvCHIP_GetHeapStats */
}

void qvCHIP_ResetHeapStats(void)
{
    size_t pHeapUsed;
    size_t pHighWatermark;
    size_t maxHeapAvailable;
    hal_GetHeapInUse((uint32_t*)&pHeapUsed, (uint32_t*)&pHighWatermark, (uint32_t*)&maxHeapAvailable);
    internalWatermarkOffset = pHighWatermark - pHeapUsed;
}

qvResetReason_t qvCHIP_GetResetReason(void)
{
    return (qvResetReason_t)gpReset_GetResetReason();
}

void qvCHIP_DisableSleepAlwaysForDevice(void)
{
    deviceSleepDisabledAlways = true;
    qvCHIP_EnableSleep(false);
}

void qvCHIP_EnableSleep(bool enable)
{
    // Calling the API below will enable the ARM processor to enter a low-power mode.
    // IC peripherals, such as but not limited to the receiver, GPIO, LED block, PWM, etc.
    // will remain awake if the application requires their functionality even with this API
    // called and the ARM in sleep mode.

    if(enable)
    {
        if(deviceSleepDisabledAlways == true)
        {
            // Never Enter Sleep Mode for FTD device
            return;
        }
#if defined(GP_DIVERSITY_QPG6200_QFN32_DK)
        hal_PowerModeResult_t result = hal_PowerModeResultSuccess;
        result = hal_SetPowerMode(hal_StandbyPowerModeELPS, hal_ActivePowerModeEHPS, hal_SleepModeRC);
        GP_ASSERT_SYSTEM(hal_PowerModeResultSuccess == result);
#else  // defined(GP_DIVERSITY_QPG6200_QFN32_DK)
        if(!gpHal_32kHzCrystalAvailable())
        {
            /* Select internal 32kHz RC oscillator */
            gpHal_SetSleepMode(gpHal_SleepModeRC);
        }
#endif // defined(GP_DIVERSITY_QPG6200_QFN32_DK)

        hal_SleepSetGotoSleepThreshold(QVCHIP_APP_GOTOSLEEP_THRESHOLD);

        if(!sleepEnabled)
        {
            sleepEnabled = true;
            hal_SleepSetGotoSleepEnable(true);
        }
    }
    else
    {
        if(sleepEnabled)
        {
            sleepEnabled = false;
            hal_SleepSetGotoSleepEnable(false);
        }
    }
}
