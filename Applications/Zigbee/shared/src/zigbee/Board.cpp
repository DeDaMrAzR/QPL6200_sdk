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

/** @file "Board.cpp"
 *
 * Board-specific settings for Application
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO

#include "ZigBee.h"

#include "gpReset.h"
#ifdef _DEBUG
#include "gpCom.h"
#endif //_DEBUG
#include "gpHal.h"
#include "gpLog.h"
#include "gpNvm.h"
#include "gpNvm_NvmProtect.h"
#include "gpSched.h"

/////////////////////////////////////////////////////////////////////////////
// Master clock frequency

const unsigned int CBoard::nMasterClockFrequency = 32000000;

/*****************************************************************************
 *                    Factory block settings
 *****************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
#define FACTORYBLOCK_ELEMENT(offset) __C7B_FACTORYBLOCK_ELEMENT(offset)
#else
#define FACTORYBLOCK_ELEMENT(offset) __attribute__((section(".c7bfb_offset_" #offset))) const
#endif

/////////////////////////////////////////////////////////////////////////////
// Factory block, ZCL basic cluster stock data (part 1)

FACTORYBLOCK_ELEMENT(0x0)
char CZigBeeClusterBasic::achManufacturerName[32] = {0};

FACTORYBLOCK_ELEMENT(0x20)
char CZigBeeClusterBasic::achModelIdentifier[32] = {0};

FACTORYBLOCK_ELEMENT(0x40)
char CZigBeeClusterBasic::achDateCode[16] = {0};

FACTORYBLOCK_ELEMENT(0x78)
unsigned short CZigBeeDeviceObject::wManufacturerID = 0;

FACTORYBLOCK_ELEMENT(0x7A)
unsigned short CZigBeeClusterOTAUpgrade::wImageTypeID = 0;

FACTORYBLOCK_ELEMENT(0xC0)
unsigned char CZigBeeClusterBasicEx::abProductCode[16] = {0};

FACTORYBLOCK_ELEMENT(0xD0)
char CZigBeeClusterBasicEx::achProductURL[48] = {0};

/////////////////////////////////////////////////////////////////////////////
// CIEEE802154MAC 64-bit IEEE extended address. This is a special version
// with optional support for built-in addresses in Qorvo transceivers

#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended
__no_init unsigned long long CIEEE802154MAC::qwExtendedAddress;
#pragma language=default
#else
/* temporary for ARMGCCEMB build */
__attribute__((section(".noinit"))) unsigned long long CIEEE802154MAC::qwExtendedAddress;
#endif

FACTORYBLOCK_ELEMENT(0x50)
volatile unsigned long long qwExtendedAddress = 0;

/////////////////////////////////////////////////////////////////////////////
// Factory block, ZCL basic cluster stock data (part 2)

FACTORYBLOCK_ELEMENT(0x58)
unsigned char CZigBeeClusterBasic::bHardwareVersion = 0;

FACTORYBLOCK_ELEMENT(0x59)
unsigned char CZigBeeClusterBasic::bPowerSource = 0;

/////////////////////////////////////////////////////////////////////////////
// Factory block, ZigBee security level (either 0 or 5)

FACTORYBLOCK_ELEMENT(0x74)
uint8_t CZigBeeNetworkInformationBase::nSecurityLevel = 5;

/////////////////////////////////////////////////////////////////////////////
// CZigBeePersistentStorage (location and size from factory block)

FACTORYBLOCK_ELEMENT(0x68)
uint32_t CZigBeeStorage::dwWorkingAreaAddress = 1;

FACTORYBLOCK_ELEMENT(0x6C)
uint16_t CZigBeeStorage::wWorkingAreaSize = 0;

#if defined(GP_DIVERSITY_GPHAL_XP4002)
// Configuration for XP4001 and XP4002 builds
// assume (FLASH_PAGE_SIZE == 512) && (FLASH_SECTOR_SIZE == 1024)
CInternalFlashMemoryDeviceXP4001 theInternalFlash(9, 10, GP_MM_CMA_NVM_LINEAR_START + 0x20000);
#elif(FLASH_PAGE_SIZE == 256) && (FLASH_SECTOR_SIZE == 512)
// Configuration for K8A builds
CInternalFlashMemoryDeviceK8x theInternalFlash(8, 9, GP_MM_FLASH_LINEAR_START);
#elif(FLASH_PAGE_SIZE == 512) && (FLASH_SECTOR_SIZE == 1024)
// Configuration for K8C builds
CInternalFlashMemoryDeviceK8x theInternalFlash(9, 10, GP_MM_FLASH_LINEAR_START);
#else
#error "Unsupported flash organization"
#endif
CZigBeeStorage theStorage(theInternalFlash,
    CZigBeeStorage::dwWorkingAreaAddress, CZigBeeStorage::wWorkingAreaSize);


/////////////////////////////////////////////////////////////////////////////
// CZigBeePersistentStorage (factory-fresh settings)

FACTORYBLOCK_ELEMENT(0x6e)
uint16_t CZigBeeStorage::wFactoryAreaSize = 0;

FACTORYBLOCK_ELEMENT(0x70)
uint32_t CZigBeeStorage::dwFactoryAreaAddress = 2;

const uint32_t CZigBeeStorage::backupAreaAddress = 0xffffffff;
const uint16_t CZigBeeStorage::backupAreaSize = 0;


// CPhysicalGreenPeak override
#if defined(GP_DIVERSITY_GPHAL_XP4002) 
unsigned char CPhysicalGreenPeak::CalculateLinkQuality(unsigned char nCorrelationValue, signed char nRSSI)
{
#ifdef _DEBUG
    _TRACE2(c15dot4phy | 3, "corr = %u, RSSI = %d\r\n", nCorrelationValue, nRSSI);
#endif

    // For the Qorvo platform, use the cross-correlation value which is
    // actually the LQI value, for LQA calculation
    return nCorrelationValue;
}
#endif // defined(GP_DIVERSITY_GPHAL_XP4001) || defined(GP_DIVERSITY_GPHAL_XP4002) || defined(GP_DIVERSITY_GPHAL_K8E) ||
       // defined(GP_DIVERSITY_GPHAL_K8A)

static gpNvm_Result_t gpNvm_CopyPool(gpNvm_PoolId_t srcPool, gpNvm_PoolId_t dstPool)
{
    gpNvm_LookupTable_Handle_t handle;
    gpNvm_Result_t result;
    UInt8 nrOfMatches;
    UIntLoop i;

    //Copy content from 1 pool to another
    result = gpNvm_BuildLookupProtected(&handle, srcPool, gpNvm_UpdateFrequencyIgnore, 0, NULL,
                                        GP_NVM_NBR_OF_UNIQUE_TAGS, &nrOfMatches);
    if (result == gpNvm_Result_NoDataAvailable)
    {
        GP_LOG_PRINTF("No data to copy %u",0,result);
        return result;
    }
    else if(result!= gpNvm_Result_DataAvailable)
    {
        GP_LOG_PRINTF("ERROR: BuildLookup failed %u",0,result);
        return result;
    }

    //Erase destination pool
    result = gpNvm_ErasePool(dstPool);
    if (result != gpNvm_Result_DataAvailable)
    {
        GP_LOG_PRINTF("Destination pool not found %u",0,result);
        return result;
    }

    gpNvm_ResetIterator(handle);
    // List NVM items.
    for (i = 0; i < nrOfMatches; i++)
    {
        gpNvm_UpdateFrequency_t updFreq;
        UInt8 tokenLen, dataLen;
        UInt8 tokenBuf[255];
        UInt8 dataBuf[255];

        result = gpNvm_ReadNextProtected(handle, srcPool, &updFreq, sizeof(tokenBuf), &tokenLen, tokenBuf,
                                         sizeof(dataBuf), &dataLen, dataBuf);
        if (result != gpNvm_Result_DataAvailable)
        {
            GP_LOG_PRINTF("ERROR: gpNvm_ReadNextProtected failed %u", 0, result);
            return result;
        }

        result = gpNvm_WriteProtected(dstPool, updFreq, tokenLen, tokenBuf, dataLen, dataBuf);
        if (result != gpNvm_Result_DataAvailable)
        {
            GP_LOG_PRINTF("ERROR: gpNvm_WriteProtected failed %u", 0, result);
            return result;
        }
    }

    // Release lookup table.
    gpNvm_FreeLookupProtected(handle);

    return gpNvm_Result_DataAvailable;
}


void CZigBeeStorage::RevertToFactoryFresh()
{
#if defined(ZIGBEE_FACTORY_RESET_BUSY_TAG_ID) && defined(GP_APP_DIVERSITY_ZB) 
    UInt8 resetState;

    //Set reset state to busy
    resetState = ZIGBEE_FACTORY_RESET_STATE_BUSY;
    gpNvm_BackupProtected(GP_COMPONENT_ID_APP, ZIGBEE_FACTORY_RESET_BUSY_TAG_ID, &resetState);
#endif

    gpNvm_CopyPool(CZigBeeStorage::dwFactoryAreaAddress,
        CZigBeeStorage::dwWorkingAreaAddress);

#if defined(ZIGBEE_FACTORY_RESET_BUSY_TAG_ID) && defined(GP_APP_DIVERSITY_ZB) 
    //Set reset state to done
    resetState = ZIGBEE_FACTORY_RESET_STATE_DONE;
    gpNvm_BackupProtected(GP_COMPONENT_ID_APP, ZIGBEE_FACTORY_RESET_BUSY_TAG_ID, &resetState);
#endif

    gpLog_Flush();
}

/*****************************************************************************
 *                    ZigBee Stack board initialization
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// One and only instance of CCompactTimerService class

CCompactTimerService theTimerService;

CCompactTimerService::ticks_t
CCompactTimerService::CalculateTicks(const uint_fast64_t microseconds)
{
    // Notice: This is an efficient overload for a timer service with 16us
    // resolution

    return microseconds >> 4;
}

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////
// Debugging bridge

static void DebugBridgeHandler(uint16_t cbPayload, uint8_t *pbPayload,
    gpCom_CommunicationId_t communicationId)
{
    // Let the debug unit process the received frame
    for (uint_fast16_t i = 0; i < cbPayload; i++)
        CDebugUnit::OnReceive(*pbPayload++);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoard

void SetupMainClock()
{
    // Currently not used on this platform
}


void CBoard::Preinitialize()
{
    // Assign extended address from factory block to RAM copy (which can be
    // used to store the built-in IEEE address as well). Make sure that
    // CIEEE802154MAC::qwExtendedAddress is declared as __no_init, otherwise
    // it will be initialized to null after Preinitialize() returns
    CIEEE802154MAC::qwExtendedAddress = qwExtendedAddress;
}

#ifdef GP_COMP_ZGPSTUB
static void Initialize_ZigBeeGPStub(void)
{
    extern CZigBeeGPStub stub;
    stub.Initialize();
}
#endif

bool CBoard::Initialize()
{
#ifdef _DEBUG
    // Register a handler that receives frames over Qorvo's COM module
    gpCom_RegisterModule(GP_COMPONENT_ID_UBIZPRO, &DebugBridgeHandler);
#endif //_DEBUG

    // Low-level stack initialization. Let PHY configure micro-controller
    // peripherals (SPI, IRQ) after board initialization. Also assigns a true
    // random seed, if supported by the hardware
    extern CPhysicalGreenPeak phy;

    phy.Initialize();
#ifdef GP_COMP_ZGPSTUB
    gpSched_ScheduleEvent(0, Initialize_ZigBeeGPStub);
#endif //GP_COMP_ZGPSTUB

    // return true since there is no return statement avialable from phy.Initialize();
    return true;
}


void CBoard::Reboot()
{
    // Reset the system
    gpReset_ResetBySwPor();
}

bool CBoard::WasResetCausedBySoftware()
{
    return (gpReset_ResetReason_SW_Por == gpReset_GetResetReason());
}
