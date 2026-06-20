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

/** @file "ResetCount.cpp"
 *
 * Implementation of Reset Counting
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID_APPFRAMEWORK 56
#define GP_COMPONENT_ID              GP_COMPONENT_ID_APPFRAMEWORK

#include "gpAssert.h"
#include "gpLog.h"
#include "gpReset.h"
#include "gpNvm.h"
#include "gpNvm_NvmProtect.h"
#include "gpSched.h"

#include "AppTask.h"
#include "ResetCount.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define RESET_COUNT_TIMEOUT_US 2000000 // 2s

#define GP_APP_NVM_BASE_TAG_ID (UInt16)(GP_COMPONENT_ID << 8)

#define RESET_COUNTS_TAG_ID 0

static Bool DefaultInitializer(const ROM void* pTag, UInt8* pBuffer);

const gpNvm_IdentifiableTag_t ROM gpApplication_NvmElements[] FLASH_PROGMEM = {
    {GP_APP_NVM_BASE_TAG_ID + RESET_COUNTS_TAG_ID, NULL, sizeof(UInt8), gpNvm_UpdateFrequencyLow, DefaultInitializer,
     NULL},
};

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static Bool DefaultInitializer(const ROM void* pTag, UInt8* pBuffer)
{
    gpNvm_IdentifiableTag_t tag;
    UInt8 value = 0;

    MEMCPY_P((UInt8*)&tag, pTag, sizeof(gpNvm_IdentifiableTag_t));
    if(NULL == pBuffer)
    {
        pBuffer = tag.pRamLocation;
        if(NULL == pBuffer)
        {
            return false;
        }
    }

    MEMCPY_P(pBuffer, (UInt8*)&value, sizeof(UInt8));
    return true;
}

static void HardwareResetTriggered(void)
{
    UInt8 resetCounts;

    gpNvm_RestoreProtected(GP_COMPONENT_ID, RESET_COUNTS_TAG_ID, &resetCounts);

    resetCounts++;

    gpNvm_BackupProtected(GP_COMPONENT_ID, RESET_COUNTS_TAG_ID, &resetCounts);
    GP_LOG_SYSTEM_PRINTF("[RST] Stored reset counts: %u", 0, resetCounts);
}

static UInt8 GetResetCount(void)
{
    uint8_t resetCounts;
    const uint8_t resetCountsCleared = 0;

    gpNvm_RestoreProtected(GP_COMPONENT_ID, RESET_COUNTS_TAG_ID, &resetCounts);

    GP_LOG_PRINTF("Processing reset counts: %u", 0, resetCounts);

    gpNvm_BackupProtected(GP_COMPONENT_ID, RESET_COUNTS_TAG_ID, (uint8_t*)&resetCountsCleared);

    return resetCounts;
}

static void TimeoutTriggered(void)
{
    AppEvent appResetCountEvent = {};

    appResetCountEvent.Type = AppEvent::kEventType_ResetCount;
    appResetCountEvent.ResetCountEvent.Count = GetResetCount();

    GetAppTask().PostEvent(&appResetCountEvent);
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void ResetCount_Init(void)
{
    gpReset_ResetReason_t resetReason;

    gpNvm_RegisterElementsProtected(gpApplication_NvmElements, number_of_elements(gpApplication_NvmElements));

    resetReason = gpReset_GetResetReason();
    if((resetReason == gpReset_ResetReason_HW_Por) || (resetReason == gpReset_ResetReason_UnSpecified))
    {
        HardwareResetTriggered();
    }

    gpSched_ScheduleEvent(RESET_COUNT_TIMEOUT_US, TimeoutTriggered);
}
