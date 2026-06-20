/*
* Copyright (c) 2015-2016, GreenPeak Technologies
* Copyright (c) 2017, Qorvo Inc
*
* gpHal_MAC.c
*   This file contains the implementation of the MAC functions
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

/*****************************************************************************
*                    Includes Definitions
*****************************************************************************/
//#define GP_LOCAL_LOG

#include "gpPd.h"

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Ble.h"
#include "gpHal_Ble_DEFS.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

#include "hal_SupplyVoltageMonitor.h"

#include "gpHal_RadioMgmt.h"

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

// Test Event Info memory
#ifdef GP_COMP_CHIPEMU
#define GP_HAL_BLE_DIRECTION_FINDING_SAMPLES_START      gpChipEmu_GetGpMicroStructCteSamplesStart(GP_MM_RAM_LINEAR_START)
#else
#define GP_HAL_BLE_DIRECTION_FINDING_SAMPLES_START           ((UIntPtr)&gpHal_BleTestModeSampleBuffer[0])
#endif // GP_COMP_CHIPEMU

#define GPHAL_BLE_DIRECT_TEST_MODE_CRC          0x555555

#define GPHAL_BLE_TIME_SLOT_DURATION_US                 625

#define GPHAL_BLE_TESTMODE_VALIDATION_THRESH_NDR                    GPHAL_BLE_VALIDATION_THRESHOLD_DEFAULT
#define GPHAL_BLE_TESTMODE_VALIDATION_START_IDX_NDR                 7
#define GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_PRESENT_NDR     0
#define GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_START_IDX_NDR   0

// Currently HDR settings are same as NDR, but this can always change
#define GPHAL_BLE_TESTMODE_VALIDATION_THRESH_HDR                    GPHAL_BLE_TESTMODE_VALIDATION_THRESH_NDR
#define GPHAL_BLE_TESTMODE_VALIDATION_START_IDX_HDR                 GPHAL_BLE_TESTMODE_VALIDATION_START_IDX_NDR
#define GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_PRESENT_HDR     GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_PRESENT_NDR
#define GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_START_IDX_HDR   GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_START_IDX_NDR

/*****************************************************************************
*                   Functional Macro Definitions
*****************************************************************************/

/* compile time verification of info structures */
GP_COMPILE_TIME_VERIFY(GPHAL_BLE_TEST_INFO_SIZE >= GP_WB_BLE_TEST_INFO_SIZE);

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

// Info structure memory (shared with RT)
COMPILER_ALIGNED(GP_WB_MAX_MEMBER_SIZE) static UInt8 gpHal_BleTestInfo[GPHAL_BLE_TEST_INFO_SIZE] LINKER_SECTION(".rt_fast_ram");


// Indicates RT is running in (direct) test mode
static Bool gpHal_BleTestModeActive;

/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/

static gpHal_Address_t gpHal_BleTestModeGetContextStartAddress(void);
static UInt16 gpHalBle_GetPacketInterval(UInt8 pbmHandle, UInt16 pbmLength, gpHal_BleTxPhy_t phy);
static void gpHal_BlePopulateValidationSettings(gpHal_phyMask_t phyMask);
static gpHal_Result_t gpHal_BlePopulateTestInfo(gpHal_TestInfo_t* pInfo);
static gpHal_BleRxPhy_t rxPhyMaskToRxPhy(gpHal_phyMask_t mask);

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

gpHal_Address_t gpHal_BleTestModeGetContextStartAddress(void)
{
    return GP_HAL_PHYSICAL_TO_ARM(gpHal_BleTestInfo);
}


UInt16 gpHalBle_GetPacketInterval(UInt8 pbmHandle, UInt16 pbmLength, gpHal_BleTxPhy_t phy)
{
    UInt16 totalDurationUs;
    UInt16 packetIntervalUnit;
    UInt16 extraOverheadUs;
    UInt8 byteDurationUs;


    switch(phy)
    {
        case gpHal_BleTxPhy1Mb:
        {
            byteDurationUs = 8;
            // preamble + crc
            extraOverheadUs = 8 + 24;
            break;
        }
        case gpHal_BleTxPhy2Mb:
        {
            byteDurationUs = 4;
            // preamble + crc
            extraOverheadUs = 8 + 12;
            break;
        }
#ifdef GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
        case gpHal_BleTxPhyCoded125kb:
        {
            byteDurationUs = 64;
            // preamble + CI + TERM1 + CRC + TERM2
            extraOverheadUs = 80 + 16 + 24 + 192 + 24;
            break;
        }
        case gpHal_BleTxPhyCoded500kb:
        {
            byteDurationUs = 16;
            // preamble + remaining from sync word + CI + TERM1 + CRC + TERM2
            // the sync word remainder comes from the fact that the sync word is present in the pbm (and incorporated by using pdLoh.length).
            // For all other phys, the sync word has the same PHY as the rest of the packet, but in this case, the sync word uses S=8 coding.
            // This means that we have 64 us (4bytes sync word * 16us per byte) added to the calculation, but the syncword duration is 256.
            // Therefor, we need to add an extra 192 us (256-64) to the overhead calculation
            extraOverheadUs = 80 + 192 + 16 + 24 + 48 + 6;
            break;
        }
#endif // GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
        default:
        {
            // Should not happen
            extraOverheadUs = 0;
            byteDurationUs = 0;
            GP_ASSERT_DEV_INT(false);
            break;
        }
    }

    totalDurationUs = (pbmLength * byteDurationUs) + extraOverheadUs;


    packetIntervalUnit = (totalDurationUs + 249) / GPHAL_BLE_TIME_SLOT_DURATION_US;

    if((totalDurationUs + 249) % GPHAL_BLE_TIME_SLOT_DURATION_US != 0)
    {
        packetIntervalUnit++;
    }

    return (packetIntervalUnit * GPHAL_BLE_TIME_SLOT_DURATION_US);
}

void gpHal_BlePopulateValidationSettings(gpHal_phyMask_t phyMask)
{
    gpHal_Address_t infoAddress = gpHal_BleTestModeGetContextStartAddress();

    if(phyMask.mask & GPHAL_BLE_PHY_MASK_1MB)
    {
        GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_THRESH(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_THRESH_NDR);
        GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_START_IDX(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_START_IDX_NDR);
        GP_WB_WRITE_BLE_TEST_INFO_FAKE_PREAMBLE_PRESENT(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_PRESENT_NDR);
        GP_WB_WRITE_BLE_TEST_INFO_FAKE_PREAMBLE_START_IDX(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_START_IDX_NDR);
    }
    else if (phyMask.mask & GPHAL_BLE_PHY_MASK_2MB)
    {
        // Use optimized settings for high data rate
        GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_THRESH(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_THRESH_HDR);
        GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_START_IDX(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_START_IDX_HDR);
        GP_WB_WRITE_BLE_TEST_INFO_FAKE_PREAMBLE_PRESENT(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_PRESENT_HDR);
        GP_WB_WRITE_BLE_TEST_INFO_FAKE_PREAMBLE_START_IDX(infoAddress, GPHAL_BLE_TESTMODE_VALIDATION_FAKE_PREAMBLE_START_IDX_HDR);
    }
    else
    {
        // Currently no special settings needed for coded PHY
    }

    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_0(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_0);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_0(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_0);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_1(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_1);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_1(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_1);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_2(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_2);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_2(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_2);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_3(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_3);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_3(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_3);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_4(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_4);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_4(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_4);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_5(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_5);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_5(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_5);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_ADDR_6(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_ADDR_6);
    GP_WB_WRITE_BLE_TEST_INFO_VALIDATION_SIGN_6(infoAddress, GPHAL_BLE_DTM_ACCESS_CODE_VAL_SIGN_6);
}

gpHal_BleRxPhy_t rxPhyMaskToRxPhy(gpHal_phyMask_t mask)
{
#ifdef GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
    if (GPHAL_BLE_PHY_MASK_CODED == mask.mask)
    {
        return gpHal_BleRxPhyCoded;
    }
    else
#endif // GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
    if (GPHAL_BLE_PHY_MASK_2MB == mask.mask)
    {
        return gpHal_BleRxPhy2Mb;
    }
    else if ( GPHAL_BLE_PHY_MASK_1MB == mask.mask
#ifdef GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
             || (GPHAL_BLE_PHY_MASK_1MB|GPHAL_BLE_PHY_MASK_CODED) == mask.mask
#endif // GP_HAL_DIVERSITY_BLE_LONG_RANGE_SUPPORTED
             )
    {
        return gpHal_BleRxPhy1Mb;
    }
    else if (0 == mask.mask)
    {
        return gpHal_BleRxPhy1Mb;
    }
    else
    {
        GP_LOG_PRINTF("Can't convert rxPhyMask %u to phy", 0, mask.mask);
        GP_ASSERT_DEV_INT(false);
        return gpHal_BleRxPhy1Mb;
    }
}

gpHal_Result_t gpHal_BlePopulateTestInfo(gpHal_TestInfo_t* pInfo)
{
    gpHal_Address_t infoAddress = gpHal_BleTestModeGetContextStartAddress();

    GP_WB_WRITE_BLE_TEST_INFO_RX_NOT_TX(infoAddress, (pInfo->tx ? false: true));
    if (pInfo->tx)
    {
        GP_WB_WRITE_BLE_TEST_INFO_TX_PHY_MODE(infoAddress, pInfo->phy.txPhy);
    }
    else
    {
        // If supported by the RT then BLE_TEST_INFO_RX_ALLPHY_MASK will overrule this
        gpHal_BleRxPhy_t rxPhy = rxPhyMaskToRxPhy(pInfo->phy.rxPhyMask);
        GP_WB_WRITE_BLE_TEST_INFO_RX_PHY_MODE(infoAddress, rxPhy);
    }
    GP_WB_WRITE_BLE_TEST_INFO_CHANNEL(infoAddress, GP_HAL_CONVERT_PHY_TO_BLE_CHANNEL(pInfo->channel));
    GP_WB_WRITE_BLE_TEST_INFO_ANTENNA(infoAddress, pInfo->antenna);
    GP_WB_WRITE_BLE_TEST_INFO_WHITENING_ENABLE(infoAddress, 0);
    GP_WB_WRITE_BLE_TEST_INFO_ACCESS_ADDRESS(infoAddress, pInfo->accesscode);
    GP_WB_WRITE_BLE_TEST_INFO_CRC_INIT(infoAddress, GPHAL_BLE_DIRECT_TEST_MODE_CRC);
    GP_WB_WRITE_BLE_TEST_INFO_WHITENING_INIT_REV(infoAddress, 0);

    if(pInfo->tx)
    {
        UInt8 pbmHandle;

        pbmHandle = gpHal_BlePdToPbm(pInfo->pdLoh, false);

        if(!GP_HAL_CHECK_PBM_VALID(pbmHandle))
        {
            return gpHal_ResultInvalidParameter;
        }

        GP_WB_WRITE_BLE_TEST_INFO_TX_PACKET_INTERVAL(infoAddress, gpHalBle_GetPacketInterval(pbmHandle, pInfo->pdLoh.length, pInfo->phy.txPhy));
        GP_WB_WRITE_BLE_TEST_INFO_TX_PACKET_COUNT(infoAddress, pInfo->txPacketCount);
        GP_WB_WRITE_BLE_TEST_INFO_FT_PTR(infoAddress, gpHal_PbmToFormatPtr(pbmHandle));
    }
    else
    {
        GP_WB_WRITE_BLE_TEST_INFO_FT_PTR(infoAddress, GP_HAL_FORMAT_PTR_INVALID);
        GP_WB_WRITE_BLE_TEST_INFO_RX_PACKET_COUNT_ANT0(infoAddress, 0);
        GP_WB_WRITE_BLE_TEST_INFO_RX_PACKET_COUNT_ANT1(infoAddress, 0);
        GP_WB_WRITE_BLE_TEST_INFO_FORWARD_RX_PDUS(infoAddress, pInfo->forwardRxPdus);
        GP_WB_WRITE_BLE_TEST_INFO_RX_ALLPHY_MASK(infoAddress, pInfo->phy.rxPhyMask.mask);

        // An RX test also requires that validation settings are applied
        if (GPHAL_BLE_ADV_ACCESS_ADDRESS == pInfo->accesscode)
        {
            GP_LOG_PRINTF("Applying ADV acc addr validation parameters",0);
            // Init the test info structure with adv access code validation settings
            gpHal_BleInitAdvValidationSettings(infoAddress + GP_WB_BLE_TEST_INFO_ACCESS_ADDRESS_ADDRESS);
        }
        else
        {
            GP_ASSERT_DEV_INT(GPHAL_BLE_DIRECT_TEST_MODE_SYNCWORD == pInfo->accesscode);
            {
                GP_LOG_PRINTF("Applying dtm addr validation parameters",0);
                gpHal_BlePopulateValidationSettings(pInfo->phy.rxPhyMask);
            }
        }
    }


    return gpHal_ResultSuccess;
}

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/

void gpHal_BleTestModeInit(void)
{
    gpHal_BleTestModeActive = false;

    GP_HAL_REGISTER_AKRAM_REGION(gpHal_BleTestInfo);


    // RT can forward testmode PDUs to NRT and uses the scan_req_rx interrupt for this.
    GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_SCAN_REQ_RX_INTERRUPT(1);
}

gpHal_Result_t gpHal_BleStartTestMode(gpHal_TestInfo_t* pInfo)
{
    gpHal_Result_t result;
    UInt16 contextOffset;

    if(gpHal_BleTestModeActive)
    {
        // Should not happen (blocked in higher layer)
        GP_ASSERT_DEV_INT(false);
        return gpHal_ResultBusy;
    }

    if(pInfo == NULL)
    {
        return gpHal_ResultInvalidParameter;
    }

    UInt8 rxPhyMask = pInfo->phy.rxPhyMask.mask;
    if (!pInfo->tx && rxPhyMask != GPHAL_BLE_PHY_MASK_1MB
                   && rxPhyMask != GPHAL_BLE_PHY_MASK_2MB
                   && rxPhyMask != GPHAL_BLE_PHY_MASK_CODED
                   && rxPhyMask != (GPHAL_BLE_PHY_MASK_1MB | GPHAL_BLE_PHY_MASK_CODED))
    {
        // 1M and Coded are available concurrently. 2M only singularly.
        return gpHal_ResultInvalidParameter;
    }

    result = gpHal_BlePopulateTestInfo(pInfo);

    if(result != gpHal_ResultSuccess)
    {
        return result;
    }

    contextOffset = TO_RT_ADDR(gpHal_BleTestModeGetContextStartAddress()) & 0xFFFF;

    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_BLE_DIRECT_TEST_MODE_START, sizeof(contextOffset), (UInt8*)&contextOffset);

    if(result == gpHal_ResultSuccess)
    {
        gpHal_BleTestModeActive = true;
    }

    return result;
}

gpHal_Result_t gpHal_BleTestMode_EnableSampleCollection(void)
{
    gpHal_Address_t infoAddress;

    infoAddress = gpHal_BleTestModeGetContextStartAddress();

    if(GP_WB_READ_BLE_TEST_INFO_RAW_PHASE_MEASUREMENT_ARMED(infoAddress))
    {
        // Already armed, nothing to do
        return gpHal_ResultBusy;
    }

    // Arm again, allow new measurements to be taken
    GP_WB_WRITE_BLE_TEST_INFO_RAW_PHASE_MEASUREMENT_ARMED(infoAddress, 1);

    return gpHal_ResultSuccess;
}

gpHal_Result_t gpHal_BleEndTestMode(gpHal_BleTestModeEnd_t* pInfo)
{
    gpHal_Address_t address;
    gpHal_Result_t result;
    UInt8 pbmEntry = GP_PBM_INVALID_HANDLE;

    address = gpHal_BleTestModeGetContextStartAddress();

    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_BLE_DIRECT_TEST_MODE_STOP, 0, NULL);

    if(result == gpHal_ResultSuccess)
    {
        gpHal_BleTestModeActive = false;

        if(!GP_WB_READ_BLE_TEST_INFO_RX_NOT_TX(address))
        {
        }
    }

    if(GP_WB_READ_BLE_TEST_INFO_RX_NOT_TX(address))
    {
        pInfo->nrOfPackets = GP_WB_READ_BLE_TEST_INFO_RX_PACKET_COUNT_ANT0(address) + GP_WB_READ_BLE_TEST_INFO_RX_PACKET_COUNT_ANT1(address);
    }
    else
    {
        pInfo->nrOfPackets = GP_WB_READ_BLE_TEST_INFO_TX_PACKET_COUNT(address);
        pbmEntry = GP_HAL_NEW_PTR_TO_PBM(GP_WB_READ_BLE_TEST_INFO_FT_PTR(address));
    }

    if(GP_HAL_CHECK_PBM_VALID(pbmEntry) && GP_HAL_IS_PBM_ALLOCATED(pbmEntry))
    {
        gpPd_Loh_t pdLoh;

        gpHal_BleConfPbmToPd(pbmEntry, &pdLoh);
        pInfo->pdHandle = pdLoh.handle;
    }
    else
    {
        pInfo->pdHandle = GP_PD_INVALID_HANDLE;
    }

    return result;
}

Bool gpHal_BleTestModeIsActive(void)
{
    return gpHal_BleTestModeActive;
}

gpHal_Result_t gpHal_BleTestModeGetPhaseSamplesBuffer(UInt32* pSamples)
{
    gpHal_Address_t infoAddress;
    if(!gpHal_BleTestModeIsActive())
    {
        return gpHal_ResultInvalidRequest;
    }

    infoAddress = gpHal_BleTestModeGetContextStartAddress();

    if(GP_WB_READ_BLE_TEST_INFO_RAW_PHASE_MEASUREMENT_ARMED(infoAddress))
    {
        // Armed, buffer can still be used by HW
        return gpHal_ResultBusy;
    }


    GP_ASSERT_SYSTEM(false);
    return gpHal_ResultSuccess;
}

gpHal_Result_t gpHal_BleTestModeGetRxPackets(gpHal_BleTestModeRxPackets_t *pPackets)
{
    gpHal_Address_t address;
    address = gpHal_BleTestModeGetContextStartAddress();
    pPackets->ant0 = GP_WB_READ_BLE_TEST_INFO_RX_PACKET_COUNT_ANT0(address);
    pPackets->ant1 = GP_WB_READ_BLE_TEST_INFO_RX_PACKET_COUNT_ANT1(address);
    return gpHal_ResultSuccess;
}
