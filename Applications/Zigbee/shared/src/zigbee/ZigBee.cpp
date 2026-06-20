/*
 * Copyright (c) 2017, Qorvo Inc
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

/** @file "ZigBee.cpp"
 *
 * Wrapper file for initialization of the ZigBee stack and its debug functionality
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#ifdef _DEBUG
#include "hal.h"
#endif //_DEBUG
#include "gpSched.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External data structures
 *****************************************************************************/
#ifdef _DEBUG
extern CPhysicalGreenPeak phy;
extern CIEEE802154MAC mac;
extern CZigBeeNetwork nwk;
static CDebugUnit dbgu;
#endif //_DEBUG
extern CZigBeeApplicationSupport aps;
extern bool bRemainAwake;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
CZigBeeFoundationApplication* pApplication =
    static_cast<CZigBeeFoundationApplication*>(aps.GetApplication(ZIGBEE_ENDPOINT_1));

#ifdef _DEBUG
/*****************************************************************************
 *                    Debug Function Definitions
 *****************************************************************************/


void DumpTime()
{
    TRACE1("TS: %" PRIu64 " symbols\r\n", theTimerService.GetCurrentTime());
}

void CDebugUnit::DumpApplication()
{
    DumpTime();
    theTimerService.Dump();

    phy.Dump();
    mac.Dump();
    nwk.Dump();
    aps.Dump();
}

void CDebugUnit::ValidateApplication()
{
    /*
    // Make sure the heap is still in a good shape
    ASSERT_HEAPVALID();

    // Make sure the MAC layer is sane and "on track"
    mac.AssertValid();

    CCriticalSectionLock csl;
    ASSERT(nwk.IsRoutingTableValid());
    ASSERT(nwk.IsRouteDiscoveryTableValid());
    */
}

__DECLARE_DEBUG_COMMAND_EX(__cmdHandleDumpHeapInfo, "dbgu_heap",
     &CDebugUnit::HandleDumpHeapInfo, false)

void CDebugUnit::HandleDumpHeapInfo(const unsigned int nArguments,
     const char *const apszArguments[])
{
     DumpHeapInfo(nArguments ? !strcmp(apszArguments[0], "1") : false,
          (nArguments > 1) ? !strcmp(apszArguments[1], "1") : false);
}

__DECLARE_DEBUG_COMMAND_EX(__cmdHandleHeapValidation, "dbgu_heap_validation",
     &CDebugUnit::HandleHeapValidation, false)

void CDebugUnit::HandleHeapValidation(const unsigned int nArguments,
     const char *const apszArguments[])
{
     bValidateHeap = strcmp(apszArguments[0], "0");

     _TRACE1(facilityCore, "heap validation %s\r\n", bValidateHeap ?
          "enabled" : "disabled");
}

__DECLARE_DEBUG_COMMAND_EX(__cmdHandleDumpStackInfo, "dbgu_stack",
     &CDebugUnit::HandleDumpStackInfo, false)

void CDebugUnit::HandleDumpStackInfo(const unsigned int nArguments,
     const char *const apszArguments[])
{
#ifdef __IAR_SYSTEMS_ICC__
     CDebugUnit::DumpStackInfo();
#endif
}

/////////////////////////////////////////////////////////////////////////////
// predefined prompt items

static void PromptDumpHeap(const bool bPriority)
{
    CDebugUnit::DumpHeapInfo(true, true);
}

__DECLARE_DEBUG_PROMPTITEM('h', '\x08', "(h)eap", PromptDumpHeap)

static void PromptDumpStack(const bool bPriority)
{
#ifdef __IAR_SYSTEMS_ICC__
    CDebugUnit::DumpStackInfo();
#endif
}

__DECLARE_DEBUG_PROMPTITEM('s', '\x13', "(s)tack", PromptDumpStack)

/////////////////////////////////////////////////////////////////////////////
// Assertion overloads

void CDebugUnit::OnOutOfMemory()
{
    CCriticalSectionLock csl;

    OutputStringEx(facilityAssertion, "\r\nFailed to allocate memory\r\n");

    //Added Qorvo behavior
    HAL_LED_SET(GRN);
    HAL_LED_SET(RED);

    SystemHalt();

    HAL_LED_CLR(GRN);
    HAL_LED_CLR(RED);
}
/*Development assertions*/
static unsigned int ubiZPro_IgnoreList[] = {
    898, //UBI-280
    2204, //UBI-305
    944, //UBI-299
    411, //
};

void CDebugUnit::OnAssertionFailed(const char *const pszFile,
    const unsigned int nLine)
{
    UInt8 i;
    Bool skip;

    CCriticalSectionLock csl;

    const unsigned int nOutput = nOutputMask;
    nOutputMask |= facilityAssertion;

    OutputStringEx(facilityAssertion, "\r\nASSERTION FAILED: %s, %u.\r\n",
        pszFile, nLine);

    //Added Qorvo behavior
    HAL_LED_SET(GRN);
    HAL_LED_SET(RED);

    skip = false;
    for(i = 0; i < number_of_elements(ubiZPro_IgnoreList); i++)
    {
        if(nLine == ubiZPro_IgnoreList[i])
        {
            skip = true;
            break;
        }
    }
    if(!skip)
    {
        SystemHalt();
    }

    nOutputMask = nOutput;

    HAL_LED_CLR(GRN);
    HAL_LED_CLR(RED);
}

#endif //_DEBUG

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void ubiZPro_OnIdle(void)
{
#ifdef _DEBUG
#endif //_DEBUG

    bRemainAwake = aps.OnIdle();

#ifdef _DEBUG
    CDebugUnit::ExecutePendingCommand();
#endif //_DEBUG
}

void ubiZPro_Init(void)
{
    gpSched_RegisterOnIdleCallback(ubiZPro_OnIdle);

    CBoard::Initialize();

    // Low-level stack initialization
    aps.AfterPowerup();
}
