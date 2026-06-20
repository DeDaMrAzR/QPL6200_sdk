/*
 * Copyright (c) 2016, GreenPeak Technologies
 * Copyright (c) 2017-2021, Qorvo Inc
 *
 *   gpPTC
 *   Implementation of gpPTC
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
#define GP_COMPONENT_ID GP_COMPONENT_ID_PTC
// #define GP_PTC_COMM_ID GP_COM_COMM_ID_RF
//  #define GP_LOCAL_LOG

#include "gpTest.h"
#ifdef GP_PTC_ENABLE_BLE
#include "gpTestBle.h"
#endif
#include "gpLog.h"
#include "gpCom.h"
#include "gpSched.h"
#include "gpVersion.h"
#include "gpPoolMem.h"
#include "gpBaseComps.h"
#include "gpPTC_server.h"
#include "gpPTC.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define PTC_VERSION     1
#define UINT8_MAXVALUE  255
#define UINT16_MAXVALUE 65535

#define PTC_PAN_ID                  (UInt16)0xCAFE
#define PTC_RFCOMMUNICATION_CHANNEL 17
#define PTC_SHORT_ADDRESS           (UInt16)0xFC7C

#define PTC_TRIMCAP_ZONE_COUNT 7 // From GP_HAL_TRIMCAP_ZONE_COUNT - may be not defined, but PTC API supports 7 zones

// TODO: replace this implementation when offcial version is ready - APPSCS-1911
#if defined(GP_DIVERSITY_GPHAL_XP4002)
#define PTC_GET_APP_CHANGELIST __CONCAT(0x, GP_GIT_SHA_SHORT)
#else
#define PTC_GET_APP_CHANGELIST 0
#endif

#define PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX ((UInt8)3)

#define PTC_CHECK_802_15_4_CHANNEL_RANGE(channel) (((channel) >= 11) && ((channel) <= 26))

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef struct _ModeParams {
    UInt8 clientid;
    UInt32 exectime;
    UInt8 modeid;
    gpPTC_ModeExecution_t onoff;
    UInt8 numberofextraargs;
    gpPTC_Parameter_t* parameters;

} gpPTC_ModeParams_t;

typedef struct _TxmcParams {
    UInt16 packetCount;
    UInt16 packetInterval;
    UInt16 packetTransmitted;
    UInt8 channel;
    UInt8 dataLength;
    UInt8* pData;
} gpPTC_TxmcParams_t;

//@}
/** @name gpPTC_BLE_DataRate_t
//@{
 */
/** @brief 1 MB/s (default) */
#define GPPTC_BLE_DATARATE_1MBS 1
/** @brief 2 MB/s */
#define GPPTC_BLE_DATARATE_2MBS 2
#ifdef GP_DIVERSITY_BLE_CODED_PHY_SUPPORTED
/** @brief 125 kB/s (if LR is enabled) */
#define GPPTC_BLE_DATARATE_125KBS 3
/** @brief 500 kB/s (if LR is enabled) */
#define GPPTC_BLE_DATARATE_500KBS 4
#endif
/*
 *  This typedef defines the possible BLE Data Rates.
 *  @typedef gpPTC_BLE_DataRate_t
 */
typedef UInt8 gpPTC_BLE_DataRate_t;

//@}
/** @name gpPTC_BLE_RxDataRate_t
//@{
 */
/** @brief 1 MB/s (default) */
#define GPPTC_BLE_RXDATARATE_1MBS 0
/** @brief 2 MB/s */
#define GPPTC_BLE_RXDATARATE_2MBS 1
#ifdef GP_DIVERSITY_BLE_CODED_PHY_SUPPORTED
/** @brief Long range listening */
#define GPPTC_BLE_RXDATARATE_LR 2
/** @brief 1MB/s + LR */
#define GPPTC_BLE_RXDATARATE_CONCURRENT 3
#endif

/*
 *  This typedef defines the possible BLE Data Rates for RX.
 *  @typedef gpPTC_BLE_DataRate_t
 */
typedef UInt8 gpPTC_BLE_RxDataRate_t;

/*
 *  This typedef defines the possible BLE Data Rates for TX - the same as general data rates.
 *  @typedef gpPTC_BLE_TxDataRate_t
 */
typedef gpPTC_BLE_DataRate_t gpPTC_BLE_TxDataRate_t;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static gpTest_ContinuousWaveMode_t gpPTC_CWMode = 0;
static UInt8 gpPTC_MapClockIOPin_Enabled = 0;
static Bool gpPTC_IsBusy = false;
static UInt32 gpPTC_RRAddress = 0;
static UInt32 gpPTC_WRAddress = 0;
static UInt32 gpPTC_WsetChannel = 0;
static UInt32 gpPTC_WgetChannel = 0;
static UInt32 gpPTC_WsetPower = 0;
static UInt8 gpPTC_WRValue = 0;
static UInt8 gpPTC_PCHStatChannel = 0;
static UInt32 gpPTC_WRBitMask = 0;
static Bool gpPTC_PrintRXPackets = false;
static UInt16 gpPTC_PacketCount = 10;
static UInt16 gpPTC_ScanCount = 0;
static UInt16 gpPTC_PacketInterValInMS = 5;
static UInt16 gpPTC_ScanInterValInMS = 3;
static UInt16 gpPTC_PacketLength = 5;

static UInt8* gpPTC_pTxData = NULL;
static UInt8 gpPTC_pTxDataLength = 0;

static Bool gpPTC_OldRxState;
static Int32 gpPTC_AllRSSI = 0;
static UInt32 gpPTC_AllLQI = 0;
static UInt16 gpPTC_RXPacketCount = 0;
// static UInt16 gpPTC_RXRetryCount=0;
static UInt16 gpPTC_TXPacketCount = 0;
static UInt8 gpPTC_DirectTestMode_PacketType = 0;
static gpPTC_BLE_DataRate_t gpPTC_BLE_DataRateValue = GPPTC_BLE_DATARATE_1MBS;
static gpPTC_BLE_RxDataRate_t gpPTC_BLE_RxDataRateValue = GPPTC_BLE_RXDATARATE_1MBS;
static gpPTC_BLE_TxDataRate_t gpPTC_BLE_TxDataRateValue = GPPTC_BLE_DATARATE_1MBS;

static UInt8 gpPTC_MapIOClockPin = 0;
static gpHal_SleepMode_t gpPTC_MapIOClockType = 0;
#if defined(GP_DIVERSITY_GPHAL_XP4002)
static UInt8 gpPTC_DBG_Sel_OrigValue = 0;
static Bool gpPTC_ALT_En_OrigValue = 0;
static UInt8 gpPTC_ALT_Sel_OrigValue = 0;
static gpHal_SleepMode_t gpPTC_SleepMode_OrigValue = 0;
#else
static UInt8 gpPTC_DBG_Sel_OrigValue = 0;
static UInt8 gpPTC_DBG_En_OrigValue = 0;
static gpHal_SleepMode_t gpPTC_SleepMode_OrigValue = 0;
#endif
static UInt8 gpPTC_EDScanBusy = false;
static UInt8 gpPTC_RXMultiStandard = gpPTC_Disabled;
static UInt8 gpPTC_RXHighSensitivity = gpPTC_Disabled;
static UInt8 gpPTC_RXMultichannel = gpPTC_Disabled;

static UInt8 gpPTC_SrcId1 = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
static UInt8 gpPTC_SrcId2 = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;

static gpPTC_TxmcParams_t gpPTC_TxmcParams[PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX];
static UInt8 gpPTC_Txmc_CurrentChSeqIdx = 0;
static UInt8 gpPTC_Txmc_ChSeqIdxToTransmit = 0xFF;
static Bool gpPTC_Txmc_On = false;

static gpPTC_StatsDump_t gpPTC_DumpRxp = gpPTC_StatsDump_Off;
static gpPTC_StatsDump_t gpPTC_DumpP = gpPTC_StatsDump_Off;

static Bool gpPTC_BleTxOn = false;

static struct {
    Bool state;
    gpPTC_PDMClkSrc_t src;
    UInt32 freqHz;
    UInt8 gpio;
} gpPTC_PDMClkCfg = {false, gpPTC_PDMClkSrc_2M, 2000, 3};

static struct {
    Int8 zoneMaxTemps[PTC_TRIMCAP_ZONE_COUNT - 1];
    UInt8 zoneSettings[PTC_TRIMCAP_ZONE_COUNT];
} gpPTC_XtalTrimCapCfg;

static gpPTC_Attribute_t gpPTC_attributeBuffer[PTC_ATTRIBUTES_MAX_NUMBER];

static Bool UseOTAProtocol = true;

GP_COMPILE_TIME_VERIFY((gpHal_CollisionAvoidanceModeNoCCA == gpPTC_CSMAMode_NoCCA) &&
                       (gpHal_CollisionAvoidanceModeCCA == gpPTC_CSMAMode_CCA) &&
                       (gpHal_CollisionAvoidanceModeCSMA == gpPTC_CSMAMode_CSMA));

/*GP_COMPILE_TIME_VERIFY((gpHal_SleepModeEvent == gpPTC_SleepModeEvent) &&
                       (gpHal_SleepModeRC   == gpPTC_SleepModeRC) &&
                       (gpHal_SleepMode16MHz  == gpPTC_SleepMode16MHz));
                       //(gpHal_SleepModeReset  == gpPTC_SleepModeReset)); */

GP_COMPILE_TIME_VERIFY((gpPTC_RF4CE == gpTest_PhyModeMac) && (gpPTC_BLE == gpTest_PhyModeBle));

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void gpPTC_mainReEnableCw(gpTest_ContinuousWaveMode_t CWState);
static void gpPTC_RestoreAttributes(Bool skipPhyMode);
static Bool gpPTC_CheckCompatibleHWVersion(void);
static gpPTC_Result_t gpPTC_ExecAttribute(gpPTC_Attribute_t attribute);
static void gpPTC_SetModeRequestFromStruct(void* pArg);
static void gpPTC_SetModeRequestToStruct(UInt8 clientID, UInt8 modeID, UInt32 exectime, gpPTC_ModeExecution_t OnOff,
                                         UInt8 numberOfExtraParameters, gpPTC_Parameter_t* parameters,
                                         gpPTC_ModeParams_t* pParam);
static void gpPTC_enableCWMode_M(void);
static void gpPTC_enableCWMode_U(void);

#if defined(GP_DIVERSITY_GPHAL_XP4002)
// move this to seperate header file and make implementation for chip variants ?
// or move this to gpTest ?
static gpPTC_Result_t restoreClkToGPIORegisters(void);
static gpPTC_Result_t mapClkToGPIORegisters(void);
#endif
static void Txmc_TransmitNextChannel(void);
/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

Bool gpPTC_CheckCompatibleHWVersion(void)
{
#ifdef GPHAL_CHIP_ID_K8A
    // if (gpHal_GetChipVersion() < 4)
    // {
    //   GP_LOG_PRINTF("Id %i V %i",0,GPHAL_CHIP_ID_K8A,gpHal_ChipVersion);
    //   return false;
    // }
#endif
    // for now : allow all other variants ?
    return true;
}

void gpPTC_enableCWMode_M(void)
{
    if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
    {
        if(gpPTC_BLE_DataRateValue == GPPTC_BLE_DATARATE_2MBS || gpPTC_BLE_TxDataRateValue == GPPTC_BLE_DATARATE_2MBS)
        {
            gpTest_SetContinuousWaveMode(CW_BLE_HDRMODULATED);
        }
        else
        {
            gpTest_SetContinuousWaveMode(CW_BLE_MODULATED);
        }
    }
    else
    {
        gpTest_SetContinuousWaveMode(CW_MODULATED);
    }
    GP_LOG_PRINTF("CW M ON", 0);
}

void gpPTC_enableCWMode_U(void)
{
    gpTest_SetContinuousWaveMode(CW_UNMODULATED);
    GP_LOG_PRINTF("CW U ON", 0);
}

void gpPTC_mainReEnableCw(gpTest_ContinuousWaveMode_t CWState)
{
    void_func contWaveFunc = gpPTC_enableCWMode_U;
    switch(CWState)
    {
        case CW_MODULATED:
        {
            contWaveFunc = gpPTC_enableCWMode_M;
            break;
        }
        case CW_UNMODULATED:
        {
            contWaveFunc = gpPTC_enableCWMode_U;
            break;
        }
        case CW_OFF:
        {
            // if it was already off, do nothing
            return;
        }
        default:
        {
            GP_ASSERT_DEV_INT(false);
            return;
        }
    }

    gpSched_UnscheduleEvent(contWaveFunc);
    gpSched_ScheduleEvent(0, contWaveFunc);
}

gpPTC_Result_t gpPTC_ExecAttribute(gpPTC_Attribute_t attribute)
{
    if(!gpPTC_CheckCompatibleHWVersion())
    {
        return gpPTC_ResultInvalidChip;
    }
    gpPTC_Result_t result = gpPTC_ResultSuccess;
    GP_LOG_PRINTF("SetAttr 0x%x to 0x%lx", 0, attribute.id, attribute.value);
    switch(attribute.id)
    {
        case gpPTC_AttributeChannel:
        case gpPTC_AttributeChannelSrcId0:
        {
            gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
            if(CWState != CW_OFF)
            {
                gpTest_SetContinuousWaveMode(CW_OFF);
            }
            result = gpTest_SetChannel((UInt8)attribute.value);

            if(result != gpPTC_ResultSuccess)
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                gpPTC_mainReEnableCw(CWState);
            }
            break;
        }
        case gpPTC_AttributeChannelSrcId1:
        case gpPTC_AttributeChannelSrcId2:
        {
            gpTest_ContinuousWaveMode_t CWState;

            if(gpTest_GetPhyMode() != gpTest_PhyModeMac)
            {
                result = gpPTC_ResultUnsupported;
                break;
            }

            CWState = gpTest_GetContinuousWaveMode();

            // Need to disable CW when changing channels
            // CW is not re-enabled as this attribute indicates RX
            if(gpTest_GetContinuousWaveMode() != CW_OFF)
            {
                gpTest_SetContinuousWaveMode(CW_OFF);
            }

            if(attribute.id == gpPTC_AttributeChannelSrcId1)
            {
                gpPTC_SrcId1 = (UInt8)attribute.value;
            }
            else
            {
                gpPTC_SrcId2 = (UInt8)attribute.value;
            }

            // Perform a live update of the channel (if RX enabled):
            if(gpTest_GetRxState() && (gpPTC_RXMultichannel == gpPTC_Enabled))
            {
                gpTest_SetChannelForOtherStacks(gpPTC_SrcId1, gpPTC_SrcId2);
            }

            gpPTC_mainReEnableCw(CWState);

            break;
        }

        case gpPTC_AttributeTXPower:
        {
            gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
            if(CWState != CW_OFF)
            {
                gpTest_SetContinuousWaveMode(CW_OFF);
            }
            gpTest_SetTxPower(attribute.value);
            result = gpPTC_ResultSuccess;
            Int8 powerset = gpTest_GetTxPower();

            if(attribute.value != powerset)
            {
                GP_LOG_PRINTF("Set %i, expected : %i", 0, powerset, (int)attribute.value);
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                gpPTC_mainReEnableCw(CWState);
            }
            break;
        }
        case gpPTC_AttributeAntenna:
        {
#if defined(GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA)
#if GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA()
            result = gpPTC_ResultUnsupported;
            break;
#endif
#endif
            gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
            if(CWState != CW_OFF)
            {
                gpTest_SetContinuousWaveMode(CW_OFF);
            }
            gpTest_SetAntennaDiversity(false);
            result = gpTest_SetAntenna((UInt8)attribute.value);
            if(result != gpPTC_ResultSuccess)
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                gpPTC_mainReEnableCw(CWState);
            }
            break;
        }
        case gpPTC_AttributeSleepMode:
        {
            UInt32 frequencymHz;
            gpTest_SleepClockMeasurementStatus_t status =
                gpTest_GetMeasuredSleepClockFrequency(attribute.value, &frequencymHz);
            if((status != gpHal_SleepClockMeasurementStatusStable) ||
               (status == gpHal_SleepClockMeasurementStatusNotStarted))
                result = gpPTC_ResultSetAttributeFailed;
            else
                gpTest_SetSleepMode(attribute.value);
            break;
        }
        case gpPTC_AttributeAntennaDiversity:
        {
#if defined(GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA)
#if GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA()
            result = gpPTC_ResultUnsupported;
            break;
#endif
#endif
            gpTest_SetAntennaDiversity(attribute.value == 1 ? true : false);
            if(attribute.value != gpTest_GetAntennaDiversity())
            {
                GP_LOG_PRINTF("Set %i, expected : %i", 0, gpTest_GetAntennaDiversity(), (int)attribute.value);
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributeCWMode:
        {
            if(gpTest_GetContinuousWaveMode() != CW_OFF)
                gpTest_SetContinuousWaveMode(CW_OFF);
            gpPTC_CWMode = ((UInt8)attribute.value == 1) ? CW_UNMODULATED : CW_MODULATED;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeCSMAMode:
        {
            gpTest_SetCollisionAvoidanceModeToUse(attribute.value);
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeMAXBE:
        {
            gpTest_SetMaxBE((UInt8)attribute.value);
            if(gpTest_GetMaxBE() == (UInt8)attribute.value)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            break;
        }
        case gpPTC_AttributeMINBE:
        {
            gpTest_SetMinBE((UInt8)attribute.value);
            if(gpTest_GetMinBE() == (UInt8)attribute.value)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            break;
        }
        case gpPTC_AttributeMaxCSMABackoff:
        {
            gpTest_SetMaxCSMABackoffs((UInt8)attribute.value);
            if(gpTest_GetMaxCSMABackoffs() == (UInt8)attribute.value)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            break;
        }
        case gpPTC_AttributeMaxMACRetries:
        {
            if(gpTest_SetNumberOfRetries((UInt8)attribute.value) != gpHal_ResultSuccess)
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                if(gpTest_GetNumberOfRetries() == (UInt8)attribute.value)
                {
                    result = gpPTC_ResultSuccess;
                }
                else
                {
                    result = gpPTC_ResultSetAttributeFailed;
                }
            }
            break;
        }
        case gpPTC_AttributePanID:
        {
            gpTest_SetPanId((UInt16)attribute.value, gpHal_SourceIdentifier_0);
            gpTest_SetPanId((UInt16)attribute.value, gpHal_SourceIdentifier_1);
            gpTest_SetPanId((UInt16)attribute.value, gpHal_SourceIdentifier_2);

            if(gpTest_GetPanId(gpHal_SourceIdentifier_0) == (UInt16)attribute.value &&
               gpTest_GetPanId(gpHal_SourceIdentifier_1) == (UInt16)attribute.value &&
               gpTest_GetPanId(gpHal_SourceIdentifier_2) == (UInt16)attribute.value)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            break;
        }
        case gpPTC_AttributeShortAddress:
        {
            gpTest_SetShortAddress((UInt16)attribute.value, gpHal_SourceIdentifier_0);
            gpTest_SetShortAddress((UInt16)attribute.value, gpHal_SourceIdentifier_1);
            gpTest_SetShortAddress((UInt16)attribute.value, gpHal_SourceIdentifier_2);

            if(gpTest_GetShortAddress(gpHal_SourceIdentifier_0) == (UInt16)attribute.value &&
               gpTest_GetShortAddress(gpHal_SourceIdentifier_1) == (UInt16)attribute.value &&
               gpTest_GetShortAddress(gpHal_SourceIdentifier_2) == (UInt16)attribute.value)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetAttributeFailed;
            }
            break;
        }

        case gpPTC_AttributePacketInPacket:
        {
            gpTest_SetPacketInPacketMode((UInt8)attribute.value);
            if(attribute.value != gpTest_GetPacketInPacketMode())
            {
                GP_LOG_PRINTF("Set %i, expected : %i", 0, gpTest_GetPacketInPacketMode(), (int)attribute.value);
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributePromiscuousMode:
        {
            gpTest_SetPromiscuousMode((Bool)attribute.value);
            if(attribute.value != gpTest_GetPromiscuousMode())
            {
                GP_LOG_PRINTF("Set %i, expected : %i", 0, gpTest_GetPromiscuousMode(), (int)attribute.value);
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributeReadRegisterAddress:
        {
            gpPTC_RRAddress = attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWriteRegisterAddress:
        {
            gpPTC_WRAddress = attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWsetChannel:
        {
            gpPTC_WsetChannel = (UInt8)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWgetChannel:
        {
            gpPTC_WgetChannel = (UInt8)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWsetPower:
        {
            gpPTC_WsetPower = (Int8)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributePCHStatChannel:
        {
            gpPTC_PCHStatChannel = (Int8)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWriteRegisterValue:
        {
            // in case of only a number of bits that should be set : first read out the value, then change it
            gpPTC_WRValue = (UInt8)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeWriteRegisterBitsMask:
        {
            gpPTC_WRBitMask = (UInt32)attribute.value;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_AttributeScanCount:
        {
            if(attribute.value > UINT16_MAXVALUE)
            {
                result = gpPTC_ResultInvalidArgument;
            }
            else
            {
                gpPTC_ScanCount = (UInt16)attribute.value;
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributePacketCount:
        {
            // prevent truncation !!
            if(attribute.value > UINT16_MAXVALUE)
            {
                result = gpPTC_ResultInvalidArgument;
            }
            else if(attribute.value == 0)
            {
                return gpPTC_ResultInvalidArgument;
            }
            else
            {
                gpPTC_PacketCount = (UInt16)attribute.value;
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributePacketIntervalInMS:
        {
            if(attribute.value > UINT16_MAXVALUE)
            {
                result = gpPTC_ResultInvalidArgument;
            }
            else
            {
                gpPTC_PacketInterValInMS = (UInt16)attribute.value;
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributeScanIntervalInMS:
        {
            if(attribute.value > UINT16_MAXVALUE)
            {
                result = gpPTC_ResultInvalidArgument;
            }
            else
            {
                gpPTC_ScanInterValInMS = (UInt16)attribute.value;
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributePacketLength:
        {
            if(attribute.value > UINT16_MAXVALUE)
            {
                result = gpPTC_ResultInvalidArgument;
            }
            else
            {
                /* add some packet size limitation check here */
                if(gpTest_GetPhyMode() == gpTest_PhyModeMac && attribute.value > 125)
                {
                    return gpPTC_ResultInvalidArgument;
                }
                else if(gpTest_GetPhyMode() == gpTest_PhyModeBle && attribute.value > 241)
                {
                    return gpPTC_ResultInvalidArgument;
                }
                gpPTC_PacketLength = (UInt16)attribute.value;
                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_AttributePhyMode:
        {
            if(gpTest_GetRxState() == 1)
            {
                // change of this attribute is only valid if RX mode is off
                result = gpPTC_ResultSetAttributeFailed;
            }
            else
            {
                // turn off running modes...
                gpTest_SetContinuousWaveMode(CW_OFF);
                gpSched_UnscheduleEvent(gpPTC_enableCWMode_M);
                gpSched_UnscheduleEvent(gpPTC_enableCWMode_U);
                gpTest_Stop();
                gpTest_SetPhyMode((UInt8)attribute.value);
                gpTest_Start();
                gpPTC_RestoreAttributes(true);

                result = gpPTC_ResultSuccess;
            }
            break;
        }
        case gpPTC_BLE_DTM_PacketType:
        {
            gpPTC_DirectTestMode_PacketType = (UInt8)attribute.value;
            break;
        }
        case gpPTC_BLE_DataRate:
        {
#if defined(GP_PTC_ENABLE_BLE)
            if(gpTest_GetPhyMode() != gpTest_PhyModeBle)
            {
                result = gpPTC_ResultUnsupported;
            }
            else
            {
                gpPTC_BLE_DataRateValue = (UInt8)attribute.value;
                /* TODO : remove constant values and replace with chip dependent ones */
                /* temp workaround for use with rpi (which has no chip dependency) is to use constant values */
                gpTest_BleTxPhy_t phy = gpHal_BleTxPhyInvalid;
                switch(gpPTC_BLE_DataRateValue)
                {
                    case GPPTC_BLE_DATARATE_1MBS:
                        phy = gpHal_BleTxPhy1Mb;
                        break;
                    case GPPTC_BLE_DATARATE_2MBS:
                        phy = gpHal_BleTxPhy2Mb;
                        break;
#ifdef GP_DIVERSITY_BLE_CODED_PHY_SUPPORTED
                    case GPPTC_BLE_DATARATE_125KBS:
                        phy = gpHal_BleTxPhyCoded125kb;
                        break;
                    case GPPTC_BLE_DATARATE_500KBS:
                        phy = gpHal_BleTxPhyCoded500kb;
                        break;
#endif
                    default:
                        // No support for coded phy yet
                        GP_ASSERT_DEV_INT(false);
                        break;
                }

                gpTest_SetModulation(phy);

                // Check if CW mode is on:
                gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
                if(CWState != CW_OFF)
                {
                    gpTest_SetContinuousWaveMode(CW_OFF);
                }

                // Restore CW mode:
                gpPTC_mainReEnableCw(CWState);
                break;
            }
#else
            result = gpPTC_ResultUnsupported;
#endif /* defined(GP_PTC_ENABLE_BLE) */
            break;
        }
        case gpPTC_BLE_RxDataRate:
        {
#if defined(GP_PTC_ENABLE_BLE) && defined(GP_DIVERSITY_GPHAL_XP4002)
            if(gpTest_GetPhyMode() != gpTest_PhyModeBle)
            {
                result = gpPTC_ResultUnsupported;
            }
            else
            {
                gpPTC_BLE_RxDataRateValue = (UInt8)attribute.value;

                gpTest_BleRxPhy_t phyMask = BM(gpHal_BleRxPhyInvalid);
                switch(gpPTC_BLE_RxDataRateValue)
                {
                    case GPPTC_BLE_RXDATARATE_1MBS:
                        phyMask = BM(gpHal_BleRxPhy1Mb);
                        break;
                    case GPPTC_BLE_RXDATARATE_2MBS:
                        phyMask = BM(gpHal_BleRxPhy2Mb);
                        break;
                    case GPPTC_BLE_RXDATARATE_LR:
                        phyMask = BM(gpHal_BleRxPhyCoded);
                        break;
                    case GPPTC_BLE_RXDATARATE_CONCURRENT:
                        phyMask = BM(gpHal_BleRxPhy1Mb) | BM(gpHal_BleRxPhyCoded);
                        break;
                    default:
                        return gpPTC_ResultInvalidArgument;
                }
                if(gpHal_ResultInvalidParameter == gpTest_SetRxPhyMask(phyMask))
                {
                    return gpPTC_ResultInvalidArgument;
                }

                // Check if CW mode is on:
                gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
                if(CWState != CW_OFF)
                {
                    gpTest_SetContinuousWaveMode(CW_OFF);
                }

                // Restore CW mode:
                gpPTC_mainReEnableCw(CWState);
                break;
            }
#else
            result = gpPTC_ResultUnsupported;
#endif /* defined(GP_PTC_ENABLE_BLE) */
            break;
        }
        case gpPTC_BLE_TxDataRate:
        {
#if defined(GP_PTC_ENABLE_BLE) && defined(GP_DIVERSITY_GPHAL_XP4002)
            if(gpTest_GetPhyMode() != gpTest_PhyModeBle)
            {
                result = gpPTC_ResultUnsupported;
            }
            else
            {
                gpPTC_BLE_TxDataRateValue = (UInt8)attribute.value;

                gpTest_BleTxPhy_t phy = gpHal_BleTxPhyInvalid;
                switch(gpPTC_BLE_TxDataRateValue)
                {
                    case GPPTC_BLE_DATARATE_1MBS:
                        phy = gpHal_BleTxPhy1Mb;
                        break;
                    case GPPTC_BLE_DATARATE_2MBS:
                        phy = gpHal_BleTxPhy2Mb;
                        break;
                    case GPPTC_BLE_DATARATE_125KBS:
                        phy = gpHal_BleTxPhyCoded125kb;
                        break;
                    case GPPTC_BLE_DATARATE_500KBS:
                        phy = gpHal_BleTxPhyCoded500kb;
                        break;
                    default:
                        return gpPTC_ResultInvalidArgument;
                }

                gpTest_SetTxModulation(phy);

                // Check if CW mode is on:
                gpTest_ContinuousWaveMode_t CWState = gpTest_GetContinuousWaveMode();
                if(CWState != CW_OFF)
                {
                    gpTest_SetContinuousWaveMode(CW_OFF);
                }

                // Restore CW mode:
                gpPTC_mainReEnableCw(CWState);
                break;
            }
#else
            result = gpPTC_ResultUnsupported;
#endif /* defined(GP_PTC_ENABLE_BLE) */
            break;
        }
        case gpPTC_AttributeMapClockIOPin:
        {
            gpPTC_MapIOClockPin = (UInt8)attribute.value;
            break;
        }
        case gpPTC_AttributeMapClockType:
        {
            gpPTC_MapIOClockType = (gpHal_SleepMode_t)attribute.value;
            break;
        }
        case gpPTC_AttributeRxMultiStandard:
        {
            // Change of this attribute is only valid if RX mode is off:
            if(!gpTest_GetRxState())
            {
                // Try to set new value immediately:
                if(gpTest_SetRxModeOptions(attribute.value == gpPTC_Enabled, gpPTC_RXMultichannel == gpPTC_Enabled,
                                           gpPTC_RXHighSensitivity == gpPTC_Enabled) == gpHal_ResultSuccess)
                {
                    gpPTC_RXMultiStandard = (UInt8)attribute.value;
                    break;
                }
            }

            result = gpPTC_ResultSetAttributeFailed;
            break;
        }
        case gpPTC_AttributeRxHighSensitivity:
        {
            // Change of this attribute is only valid if RX mode is off,
            // cannot disable HS mode when already enabled by the Low Temperature mode:
            if(!gpTest_GetRxState() && !(gpTest_IsLowTemp() && (attribute.value == gpPTC_Disabled)))
            {
                // Try to set new value immediately:
                if(gpTest_SetRxModeOptions(gpPTC_RXMultiStandard == gpPTC_Enabled,
                                           gpPTC_RXMultichannel == gpPTC_Enabled,
                                           attribute.value == gpPTC_Enabled) == gpHal_ResultSuccess)
                {
                    gpPTC_RXHighSensitivity = (UInt8)attribute.value;
                    break;
                }
            }

            result = gpPTC_ResultSetAttributeFailed;
            break;
        }
        case gpPTC_AttributeRxMultiChannel:
        {
            // Change of this attribute is only valid if RX mode is off:
            if(!gpTest_GetRxState())
            {
                // Try to set new value immediately:
                if(gpTest_SetRxModeOptions(gpPTC_RXMultiStandard == gpPTC_Enabled, attribute.value == gpPTC_Enabled,
                                           gpPTC_RXHighSensitivity == gpPTC_Enabled) == gpHal_ResultSuccess)
                {
                    gpPTC_RXMultichannel = (UInt8)attribute.value;
                    break;
                }
            }

            result = gpPTC_ResultSetAttributeFailed;
            break;
        }
        // APPHW-3412: Only needed for products that expect WiFi interference - Is blocked by dll if not exposable
        case gpPTC_AttributeRxLnaAttDuringTimeout:
        {
#if !defined(GP_DIVERSITY_GPHAL_XP4002)
            // gpTest api is available for all chips, but will be stubbed if no implementation is available
            gpTest_SetRxLnaAttDuringTimeoutForRssiBasedAgcMode((Bool)attribute.value);

#else  // GP_DIVERSITY_GPHAL_XP4002
       // MIDLEVELAGCATT is no longer supported by XP4001.
            result = gpPTC_ResultInvalidChip;
#endif // GP_DIVERSITY_GPHAL_XP4002
            break;
        }
        case gpPTC_AttributePDMClkSrc:
        {
            gpPTC_PDMClkCfg.src = (gpPTC_PDMClkSrc_t)attribute.value;
            break;
        }
        case gpPTC_AttributePDMClkFreq:
        {
            if(!GP_TEST_PDM_CLK_FREQ_VALIDATE((UInt32)attribute.value))
            {
                result = gpPTC_ResultInvalidParameter;
                break;
            }

            gpPTC_PDMClkCfg.freqHz = (UInt32)attribute.value;
            break;
        }
        case gpPTC_AttributePDMClkOutPin:
        {
            gpPTC_PDMClkCfg.gpio = (UInt8)attribute.value;
            break;
        }
        case gpPTC_AttributeXtalTrimCapZone1MaxTemp:
        case gpPTC_AttributeXtalTrimCapZone2MaxTemp:
        case gpPTC_AttributeXtalTrimCapZone3MaxTemp:
        case gpPTC_AttributeXtalTrimCapZone4MaxTemp:
        case gpPTC_AttributeXtalTrimCapZone5MaxTemp:
        case gpPTC_AttributeXtalTrimCapZone6MaxTemp:
        {
            UInt8 idx = attribute.id - gpPTC_AttributeXtalTrimCapZone1MaxTemp;

            GP_ASSERT_DEV_INT(idx < (PTC_TRIMCAP_ZONE_COUNT - 1));
            gpPTC_XtalTrimCapCfg.zoneMaxTemps[idx] = (Int8)attribute.value;
            break;
        }
        case gpPTC_AttributeXtalTrimCapZone1Setting:
        case gpPTC_AttributeXtalTrimCapZone2Setting:
        case gpPTC_AttributeXtalTrimCapZone3Setting:
        case gpPTC_AttributeXtalTrimCapZone4Setting:
        case gpPTC_AttributeXtalTrimCapZone5Setting:
        case gpPTC_AttributeXtalTrimCapZone6Setting:
        case gpPTC_AttributeXtalTrimCapZone7Setting:
        {
            UInt8 idx = attribute.id - gpPTC_AttributeXtalTrimCapZone1Setting;

            GP_ASSERT_DEV_INT(idx < PTC_TRIMCAP_ZONE_COUNT);
            gpPTC_XtalTrimCapCfg.zoneSettings[idx] = (UInt8)attribute.value;
            break;
        }

        case gpPTC_AttributeTxmcCurrentChSeq:
        {
            if(attribute.value >= PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX)
            {
                return gpPTC_ResultInvalidArgument;
            }
            gpPTC_Txmc_CurrentChSeqIdx = (UInt8)attribute.value;
            break;
        }

        case gpPTC_AttributeTxmcChannel:
        {
            if((attribute.value < 0) || (attribute.value > UINT8_MAXVALUE))
            {
                return gpPTC_ResultInvalidArgument;
            }
            gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].channel = (UInt8)attribute.value;
            break;
        }
        case gpPTC_AttributeTxmcPacketCount:
        {
            if((attribute.value <= 0) || (attribute.value > UINT16_MAXVALUE))
            {
                return gpPTC_ResultInvalidArgument;
            }
            gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetCount = (UInt16)attribute.value;
            break;
        }

        case gpPTC_AttributeTxmcPacketInterval:
        {
            if((attribute.value < 0) || (attribute.value > UINT16_MAXVALUE))
            {
                return gpPTC_ResultInvalidArgument;
            }
            gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetInterval = (UInt16)attribute.value;
            break;
        }

        case gpPTC_AttributeDumpRxp:
        {
            gpPTC_DumpRxp = (gpPTC_StatsDump_t)attribute.value;
            break;
        }

        case gpPTC_AttributeDumpP:
        {
            gpPTC_DumpP = (gpPTC_StatsDump_t)attribute.value;
            break;
        }
        case gpPTC_AttributeDCDC:
        {
#if defined(GP_DIVERSITY_GPHAL_XP4002)
            result = gpTest_SetDcdcMode((Bool)attribute.value);
#else
            // Setting DCDC in runtime available only for xp400x
            result = gpPTC_ResultInvalidChip;
#endif
            break;
        }
        default:
        {
            GP_LOG_PRINTF("Unknown attribute %x", 0, attribute.id);
            attribute.value = 0xFFFF;
            result = gpPTC_ResultInvalidArgument;
            break;
        }
    }
    return result;
}

/* exectime is in ms */
void gpPTC_SetModeRequestToStruct(UInt8 clientID, UInt8 modeID, UInt32 exectime, gpPTC_ModeExecution_t OnOff,
                                  UInt8 numberOfExtraParameters, gpPTC_Parameter_t* parameters,
                                  gpPTC_ModeParams_t* pParam)
{
    pParam->clientid = clientID;
    pParam->modeid = modeID;
    pParam->exectime = exectime;
    pParam->onoff = OnOff;
    pParam->numberofextraargs = numberOfExtraParameters;
    MEMCPY(&pParam->parameters, parameters, sizeof(gpPTC_Parameter_t));
}

static void Txmc_TransmitNextChannel(void)
{
    for(UIntLoop i = 0; i < PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX; i++)
    {
        gpPTC_Txmc_ChSeqIdxToTransmit++;
        gpPTC_Txmc_ChSeqIdxToTransmit %= PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX;

        if((gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].packetTransmitted <
            gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].packetCount) &&
           (PTC_CHECK_802_15_4_CHANNEL_RANGE(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].channel)))
        {
            GP_ASSERT_DEV_INT(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].pData != NULL);
            gpTest_SetChannel(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].channel);
            gpTest_TxPacket(1, 0, gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].dataLength,
                            gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].pData, 0);
            break;
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpPTC_Init(void)
{
#if defined(GP_PTC_DIVERSITY_GPCOM_SERVER)
    gpPTC_InitServer();
#endif
    gpPTC_IsBusy = false;
    for(UIntLoop i = 0; i < PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX; i++)
    {
        gpPTC_TxmcParams[i].packetCount = 0;
        gpPTC_TxmcParams[i].packetInterval = 0;
        gpPTC_TxmcParams[i].packetTransmitted = 0;
        gpPTC_TxmcParams[i].channel = GP_HAL_MULTICHANNEL_INVALID_CHANNEL;
        gpPTC_TxmcParams[i].dataLength = 0;
        gpPTC_TxmcParams[i].pData = 0;
    }
    GP_LOG_PRINTF("App Going to test mode", 0);
    // gpTest_Start contains registration of DI, DC callbacks...
    gpTest_Start();
    gpTest_WakeUpGP();

    // gpHal_RegisterEDConfirmCallback(gpTest_cbEDConfirm);
}

void gpPTC_Discover(UInt8 clientID, gpPTC_MACAddress_t senderMacAddress)
{
    (void)clientID;
    (void)senderMacAddress;

    GP_LOG_SYSTEM_PRINTF("NO SUPPORT FOR MULTIPLE CONNECTIONS", 0);
}

gpPTC_ModeExecution_t gpPTC_GetModeRequest(UInt8 clientID, UInt8 modeID)
{
    gpPTC_ModeExecution_t result = gpPTC_ModeExecution_Single;
    switch(modeID)
    {
        case gpPTC_ModeCarrierWave:
            if(gpTest_GetContinuousWaveMode() == CW_OFF)
            {
                result = gpPTC_ModeExecution_Off;
            }
            else
            {
                result = gpPTC_ModeExecution_On;
            }
            break;
        case gpPTC_ModeRx:

            if(gpTest_GetPhyMode() == gpTest_PhyModeMac)
            {
                if(gpTest_GetRxState() == gpPTC_ModeExecution_On)
                {
                    result = gpPTC_ModeExecution_On;
                }
                else
                {
                    result = gpPTC_ModeExecution_Off;
                }
            }
            else if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
            {
                // special case for BLE because we are using Directtestmode rx...
                // should be handled in gpTest...

                if(gpPTC_OldRxState)
                {
                    result = gpPTC_ModeExecution_On;
                }
                else
                {
                    result = gpPTC_ModeExecution_Off;
                }
            }
            break;
        default:
            GP_LOG_PRINTF("Not implemented getmode %i", 0, modeID);
            result = gpPTC_ModeExecution_Single;
            break;
    }
    return result;
}

/********************************************************************
 * Set the cached values of the attributes                           *
 * skipPhyMode : restoration can be called when switching phy mode,  *
 *         so when this is set, do not restore the phy mode attribute*
 *********************************************************************/
static void gpPTC_RestoreAttributes(Bool skipPhyMode)
{
    UIntLoop i;
    for(i = 1; i < PTC_ATTRIBUTES_MAX_NUMBER; i++)
    {
        if(gpPTC_attributeBuffer[i].id != 0)
        {
            // skip restoring the phy mode, as the restore has been triggered by the setting of the phy mode to a new
            // value
            if(skipPhyMode && gpPTC_attributeBuffer[i].id == gpPTC_AttributePhyMode)
            {
                continue;
            }
            GP_LOG_PRINTF("r id : 0x%x val : %i ", 0, gpPTC_attributeBuffer[i].id, (int)gpPTC_attributeBuffer[i].value);
            gpPTC_ExecAttribute(gpPTC_attributeBuffer[i]);
        }
    }
}

void gpPTC_SetClientIDRequest(UInt8 clientID)
{
}

void gpPTC_ResetDUT(void)
{
#ifdef GP_COMP_GPHAL
    gpSched_ScheduleEvent(1000, hal_Reset);
#endif
}

UInt32 gpPTC_GetDUTApiVersion(void)
{
    return GP_PTC_API_VERSION;
}

gpPTC_Result_t gpPTC_GetDUTInfoRequest(UInt8 clientID, UInt8* version, gpPTC_MACAddress_t* macAddr,
                                       gpPTC_DeviceAddress_t* bleAddr, gpPTC_swVersionNumber_t* appVersion,
                                       gpPTC_partNumber_t* partNumber, gpPTC_ProductName_t* productName,
                                       gpPTC_swVersionNumber_t* ptcVersion, gpPTC_ProductID_t* productID)
{
    GP_LOG_PRINTF("Dut Info", 0);

    gpVersion_ReleaseInfo_t releaseInfo;
    MACAddress_t mac;
    gpTest_GetExtendedAddress(&mac);
    MEMCPY(macAddr, &mac, sizeof(MACAddress_t));

#ifdef GP_PTC_ENABLE_BLE
    BtDeviceAddress_t ble;
    // this call will read from the info/user page, this should be the one to use
    // assuming that the device address should already be written
    // for testing purposes, this will read from what is in the register
    // this is written by the setDeviceAddress call in main.c
    gpTest_BleGetDeviceAddress(&ble);
    MEMCPY(bleAddr, &ble.addr, sizeof(BtDeviceAddress_t));
#endif

    gpVersion_GetSoftwareVersion(&releaseInfo);
    appVersion->major = releaseInfo.major;
    appVersion->minor = releaseInfo.minor;
    appVersion->revision = releaseInfo.revision;
    appVersion->patch = releaseInfo.patch;
    appVersion->changelist = PTC_GET_APP_CHANGELIST; // TODO: replace this implementation when offcial way is ready -
                                                     // APPSCS-1911

#ifdef GP_PTC_VERSION
    const ROM gpVersion_ReleaseInfo_t FLASH_PROGMEM gpVersionPTC = {GP_PTC_VERSION};
    MEMCPY_P(ptcVersion, &gpVersionPTC, sizeof(gpVersion_ReleaseInfo_t));
    ptcVersion->changelist = 0;
#else
    ptcVersion->major = releaseInfo.major;
    ptcVersion->minor = releaseInfo.minor;
    ptcVersion->revision = releaseInfo.revision;
    ptcVersion->patch = releaseInfo.patch;
    // changelist information is not known for the ptc component
    ptcVersion->changelist = 0;
#endif

    partNumber->header0 = gpTest_GetChipId();
    partNumber->header1 = gpTest_GetChipVersion();

    *version = PTC_VERSION;
    MEMSET(productName->name, '\0', sizeof(productName->name));

    gpTest_ReadProductId((UInt8*)productID);

#ifdef GP_PTC_PRODUCTNAME
    char product[] = {XSTRINGIFY(GP_PTC_PRODUCTNAME)};
    /* check to make sure productname is smaller than 40 chars */
    COMPILE_TIME_ASSERT(sizeof(product) < 40);
    MEMCPY(productName->name, &product, sizeof(product));
#else
#error "No PTC Productname defined !!"
#endif
    return gpPTC_ResultSuccess;
}

/* Use this call to set some payload data that can be used by a command */
gpPTC_Result_t gpPTC_SetByteDataForAttributeRequest(UInt8 clientID, gpPTC_AttributeId_t attributeID, UInt8 dataLen,
                                                    UInt8* pData)
{
    if(!gpPTC_CheckCompatibleHWVersion())
    {
        return gpPTC_ResultInvalidChip;
    }
    if(dataLen <= 3 || dataLen > 125)
    {
        return gpPTC_ResultInvalidParameter;
    }
    if(attributeID == gpPTC_AttributeSetTxData)
    {
        if(gpPTC_pTxData != NULL)
        {
            gpPoolMem_Free(gpPTC_pTxData);
        }
        gpPTC_pTxData = (UInt8*)GP_POOLMEM_MALLOC(dataLen);
        MEMCPY(gpPTC_pTxData, pData, dataLen);

        gpPTC_pTxDataLength = dataLen;
    }
    else if(attributeID == gpPTC_AttributeTxmcData)
    {
        if(gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].pData != NULL)
        {
            gpPoolMem_Free(gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].pData);
        }
        gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].pData = (UInt8*)GP_POOLMEM_MALLOC(dataLen);
        MEMCPY(gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].pData, pData, dataLen);

        gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].dataLength = (UInt8)dataLen;
    }
    return gpPTC_ResultSuccess;
}

gpPTC_Result_t gpPTC_ExecuteCustomCommand(UInt8 clientID, UInt8 dataLenIn, UInt8* pDataIn, UInt8* dataLenOut,
                                          UInt8* pDataOut)
{
    if(!gpPTC_CheckCompatibleHWVersion())
    {
        return gpPTC_ResultInvalidChip;
    }
    MEMSET(pDataOut, 0, sizeof(*dataLenOut));
// first byte of the data is the command id
#define commandId (pDataIn[0])
#define command   (pDataIn + 1)
    if(!gpPTC_RunCustomCommand(commandId, dataLenIn, pDataIn, dataLenOut, pDataOut))
    {
        GP_LOG_PRINTF("Unsupported custom command 0x%x", 0, commandId);
        return gpPTC_ResultInvalidParameter;
    }
#undef commandId
#undef command

    return gpPTC_ResultSuccess;
}

/*
 * Implementation of the 32kHz crystal test
 * The returned data is the result of this test
 * @param lenIn Length of the input byte array
 * @param pDataIn pointer to the input byte array
 * @param pLenOut pointer to the output byte array (defined by the functionality)
 * @param pDataOut pointer to the output byte array
 * byte 0   : 00 - status : not started = 0; pending = 1; not stable = 2; stable = 3
 * byte 1-4 : frequency in mHz
 */
Bool gpPTC_Execute32KhzTest(UInt8 dataLenIn, UInt8* pDataIn, UInt8* dataLenOut, UInt8* pDataOut)
{
    UInt32 frequencymHz = 0;

    gpHal_SleepClockMeasurementStatus_t status =
        gpTest_GetMeasuredSleepClockFrequency(gpHal_SleepMode32kHz, &frequencymHz);
    *dataLenOut = 5;
    pDataOut[0] = status;
    // reverse the order
    UInt32 newValue = (frequencymHz & 0x000000FFU) << 24 | (frequencymHz & 0x0000FF00U) << 8 |
                      (frequencymHz & 0x00FF0000U) >> 8 | (frequencymHz & 0xFF000000U) >> 24;
    MEMCPY(&pDataOut[1], &newValue, sizeof(newValue));
    GP_LOG_PRINTF("status %i pDataOut 0x%i", 0, status, pDataOut[1]);
    return true;
}

/* Setting of attributes : assuming checking of possible values has already been done at client side
 RF Settings are only being applied when actually send a tx packet.
*/
gpPTC_Result_t gpPTC_SetAttributeRequest(UInt8 clientID, UInt8 numberOfAttr, gpPTC_Attribute_t* attributes)
{
    gpPTC_Result_t result = gpPTC_ResultSuccess;
    UIntLoop i;

    for(i = 0; i < numberOfAttr; i++)
    {
        gpPTC_Attribute_t attribute = attributes[i];
        result = gpPTC_ExecAttribute(attribute);
        if(result != gpPTC_ResultSuccess)
        {
            break;
        }
        else
        {
            // Store all attributes so they can be restored later
            gpPTC_attributeBuffer[attribute.id] = attribute;
        }
    }

    return result;
}

gpPTC_Result_t gpPTC_GetAttributeRequest(UInt8 clientID, UInt8 numberOfAttr, gpPTC_Attribute_t* attributes)
{
    if(!gpPTC_CheckCompatibleHWVersion())
    {
        return gpPTC_ResultInvalidChip;
    }
    gpPTC_Result_t result = gpPTC_ResultSuccess;
    UIntLoop i = 0;

    // look for the corresponding attribute based on id
    for(i = 0; i < numberOfAttr; i++)
    {
        switch(attributes[i].id)
        {
            case gpPTC_AttributeChannel:
            case gpPTC_AttributeChannelSrcId0:
            {
                attributes[i].value = gpTest_GetChannel();
                break;
            }
            case gpPTC_AttributeChannelSrcId1:
            {
                attributes[i].value = gpPTC_SrcId1;
                break;
            }
            case gpPTC_AttributeChannelSrcId2:
            {
                attributes[i].value = gpPTC_SrcId2;
                break;
            }
            case gpPTC_AttributeTXPower:
            {
                attributes[i].value = gpTest_GetTxPower();
                break;
            }
            case gpPTC_AttributeAntenna:
            {
#if defined(GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA)
#if GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA()
                attributes[i].value = 0xFF;
                break;
#endif
#endif
                attributes[i].value = gpTest_GetRxAntenna();
                break;
            }
            case gpPTC_AttributeAntennaDiversity:
            {
#if defined(GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA)
#if GP_BSP_HAS_DIFFERENTIAL_SINGLE_ANTENNA()
                attributes[i].value = 0xFF;
                break;
#endif
#endif
                attributes[i].value = gpTest_GetAntennaDiversity();
                break;
            }
            case gpPTC_AttributeSleepMode:
            {
                attributes[i].value = gpTest_GetSleepMode();
                break;
            }
            case gpPTC_AttributeCWMode:
            {
                attributes[i].value = (gpPTC_CWMode == CW_UNMODULATED ? 1 : 0);
                break;
            }
            case gpPTC_AttributeCSMAMode:
            {
                attributes[i].value = gpTest_GetCollisionAvoidanceModeInUse();
                break;
            }
            case gpPTC_AttributeMAXBE:
            {
                attributes[i].value = gpTest_GetMaxBE();
                break;
            }
            case gpPTC_AttributeMINBE:
            {
                attributes[i].value = gpTest_GetMinBE();
                break;
            }
            case gpPTC_AttributeMaxCSMABackoff:
            {
                attributes[i].value = gpTest_GetMaxCSMABackoffs();
                break;
            }
            case gpPTC_AttributeMaxMACRetries:
            {
                attributes[i].value = gpTest_GetNumberOfRetries();
                break;
            }
            case gpPTC_AttributePanID:
            {
                attributes[i].value = gpTest_GetPanId(gpHal_SourceIdentifier_0);
                break;
            }
            case gpPTC_AttributeShortAddress:
            {
                attributes[i].value = gpTest_GetShortAddress(gpHal_SourceIdentifier_0);
                break;
            }
            case gpPTC_AttributePowerControlLoop:
            {
                attributes[i].value = false;
                break;
            }
            case gpPTC_AttributePacketInPacket:
            {
                attributes[i].value = gpTest_GetPacketInPacketMode();
                break;
            }
            case gpPTC_AttributePromiscuousMode:
            {
                attributes[i].value = gpTest_GetPromiscuousMode();
                break;
            }
            case gpPTC_AttributePacketCount:
            {
                attributes[i].value = gpPTC_PacketCount;
                break;
            }
            case gpPTC_AttributeScanCount:
            {
                attributes[i].value = gpPTC_ScanCount;
                break;
            }
            case gpPTC_AttributePacketIntervalInMS:
            {
                attributes[i].value = gpPTC_PacketInterValInMS;
                break;
            }
            case gpPTC_AttributeScanIntervalInMS:
            {
                attributes[i].value = gpPTC_ScanInterValInMS;
                break;
            }
            case gpPTC_AttributePacketLength:
            {
                attributes[i].value = gpPTC_PacketLength;
                break;
            }
                //   other attributes are not relevant ?
            case gpPTC_AttributeReadRegisterAddress:
            {
                attributes[i].value = gpPTC_RRAddress;
                break;
            }
            case gpPTC_AttributeWriteRegisterAddress:
            {
                attributes[i].value = gpPTC_WRAddress;
                break;
            }
            case gpPTC_AttributeWriteRegisterValue:
            {
                attributes[i].value = gpPTC_WRValue;
                break;
            }
            case gpPTC_AttributeWsetChannel:
            {
                attributes[i].value = gpPTC_WsetChannel;
                break;
            }
            case gpPTC_AttributeWgetChannel:
            {
                attributes[i].value = gpPTC_WgetChannel;
                break;
            }
            case gpPTC_AttributeWsetPower:
            {
                attributes[i].value = gpPTC_WsetPower;
                break;
            }
            case gpPTC_AttributePCHStatChannel:
            {
                attributes[i].value = gpPTC_PCHStatChannel;
                break;
            }
            case gpPTC_AttributeWriteRegisterBitsMask:
            {
                attributes[i].value = gpPTC_WRBitMask;
                break;
            }
            case gpPTC_AttributePhyMode:
            {
                attributes[i].value = gpTest_GetPhyMode();
                break;
            }
            case gpPTC_BLE_DTM_PacketType:
            {
                attributes[i].value = gpPTC_DirectTestMode_PacketType;
                break;
            }
            case gpPTC_BLE_DataRate:
            {
                attributes[i].value = gpPTC_BLE_DataRateValue;
                break;
            }
            case gpPTC_BLE_RxDataRate:
            {
                attributes[i].value = gpPTC_BLE_RxDataRateValue;
                break;
            }
            case gpPTC_BLE_TxDataRate:
            {
                attributes[i].value = gpPTC_BLE_TxDataRateValue;
                break;
            }
            case gpPTC_AttributeMapClockType:
            {
                attributes[i].value = (UInt8)gpPTC_MapIOClockType;
                break;
            }
            case gpPTC_AttributeMapClockIOPin:
            {
                attributes[i].value = gpPTC_MapIOClockPin;
                break;
            }
            case gpPTC_AttributeRxMultiStandard:
            {
                gpTest_GetRxModeOptions((Bool*)&gpPTC_RXMultiStandard, (Bool*)&gpPTC_RXMultichannel,
                                        (Bool*)&gpPTC_RXHighSensitivity);

                attributes[i].value = gpPTC_RXMultiStandard;
                break;
            }
            case gpPTC_AttributeRxHighSensitivity:
            {
                gpTest_GetRxModeOptions((Bool*)&gpPTC_RXMultiStandard, (Bool*)&gpPTC_RXMultichannel,
                                        (Bool*)&gpPTC_RXHighSensitivity);

                attributes[i].value = gpPTC_RXHighSensitivity;

                break;
            }
            case gpPTC_AttributeRxMultiChannel:
            {
                gpTest_GetRxModeOptions((Bool*)&gpPTC_RXMultiStandard, (Bool*)&gpPTC_RXMultichannel,
                                        (Bool*)&gpPTC_RXHighSensitivity);

                attributes[i].value = gpPTC_RXMultichannel;
                break;
            }
            case gpPTC_AttributeRxLnaAttDuringTimeout:
            {

#if defined(GP_DIVERSITY_GPHAL_XP4002)
                // MIDLEVELAGCATT is no longer supported by XP4001.
                // Result disabled as a workaround for failing command that prints value of every attribute.
                // Failure is still raised when calling ExecAttribute() on this attribute
                // result = gpPTC_ResultInvalidChip;
#endif
                break;
            }
            case gpPTC_AttributePDMClkSrc:
            {
                attributes[i].value = gpPTC_PDMClkCfg.src;
                break;
            }
            case gpPTC_AttributePDMClkFreq:
            {
                attributes[i].value = gpPTC_PDMClkCfg.freqHz;
                break;
            }
            case gpPTC_AttributePDMClkOutPin:
            {
                attributes[i].value = gpPTC_PDMClkCfg.gpio;
                break;
            }
            case gpPTC_AttributeXtalTrimCapZone1MaxTemp:
            case gpPTC_AttributeXtalTrimCapZone2MaxTemp:
            case gpPTC_AttributeXtalTrimCapZone3MaxTemp:
            case gpPTC_AttributeXtalTrimCapZone4MaxTemp:
            case gpPTC_AttributeXtalTrimCapZone5MaxTemp:
            case gpPTC_AttributeXtalTrimCapZone6MaxTemp:
            {
                UInt8 idx = attributes[i].id - gpPTC_AttributeXtalTrimCapZone1MaxTemp;
                GP_ASSERT_DEV_INT(idx < (PTC_TRIMCAP_ZONE_COUNT - 1));
                gpTest_Xtal32MHzTrimGetZoneMaxTemp(idx + 1, &gpPTC_XtalTrimCapCfg.zoneMaxTemps[idx]);
                attributes[i].value = gpPTC_XtalTrimCapCfg.zoneMaxTemps[idx];
                break;
            }
            case gpPTC_AttributeXtalTrimCapZone1Setting:
            case gpPTC_AttributeXtalTrimCapZone2Setting:
            case gpPTC_AttributeXtalTrimCapZone3Setting:
            case gpPTC_AttributeXtalTrimCapZone4Setting:
            case gpPTC_AttributeXtalTrimCapZone5Setting:
            case gpPTC_AttributeXtalTrimCapZone6Setting:
            case gpPTC_AttributeXtalTrimCapZone7Setting:
            {
                UInt8 idx = attributes[i].id - gpPTC_AttributeXtalTrimCapZone1Setting;
                GP_ASSERT_DEV_INT(idx < PTC_TRIMCAP_ZONE_COUNT);
                gpTest_Xtal32MHzTrimGetZoneSetting(idx + 1, &gpPTC_XtalTrimCapCfg.zoneSettings[idx]);
                attributes[i].value = gpPTC_XtalTrimCapCfg.zoneSettings[idx];
                break;
            }

            case gpPTC_AttributeTxmcCurrentChSeq:
            {
                attributes[i].value = gpPTC_Txmc_CurrentChSeqIdx;
                break;
            }

            case gpPTC_AttributeTxmcChannel:
            {
                attributes[i].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].channel;
                break;
            }

            case gpPTC_AttributeTxmcPacketCount:
            {
                attributes[i].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetCount;
                break;
            }

            case gpPTC_AttributeTxmcPacketInterval:
            {
                attributes[i].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetInterval;
                break;
            }

            case gpPTC_AttributeDumpRxp:
            {
                attributes[i].value = gpPTC_DumpRxp;
                break;
            }

            case gpPTC_AttributeDumpP:
            {
                attributes[i].value = gpPTC_DumpP;
                break;
            }

            case gpPTC_AttributeDCDC:
            {
#if defined(GP_DIVERSITY_GPHAL_XP4002)

                hal_StandbyPowerMode_t standbyPowerMode;
                hal_ActivePowerMode_t activePowerMode;
                hal_SleepMode_t sleepMode;

                result = gpTest_GetDcdcMode(&standbyPowerMode, &activePowerMode, &sleepMode);

                attributes[i].value =
                    ((standbyPowerMode == hal_StandbyPowerModeELPS) && (activePowerMode == hal_ActivePowerModeEHPS))
                        ? (UInt8) true
                        : (UInt8) false;
#else
                result = gpPTC_ResultInvalidChip;
#endif
                break;
            }

            default:
            {
                break;
            }
        }
        GP_LOG_PRINTF("GetAttr  %x %lx", 0, attributes[i].id, (UInt32)attributes[i].value);
        if(result != gpPTC_ResultSuccess)
            break;
    }
    return result;
}

void gpPTC_SetModeRequestFromStruct(void* pArg)
{
    gpPTC_ModeParams_t* pParam = (gpPTC_ModeParams_t*)pArg;
    gpPTC_SetModeRequest(pParam->clientid, pParam->modeid, pParam->exectime, pParam->onoff, pParam->numberofextraargs,
                         pParam->parameters);
    gpPoolMem_Free(pParam);
}

void gpTest_cbDataConfirm(UInt8 status, UInt16 packetsSentOK, UInt16 packetsSentError)
{
    GP_LOG_PRINTF("gpTest_cbDataConfirm %x", 0, status);
    // this should not be shown in case of OTA communication... but in this case callbacks should be assigned to other
    if(!UseOTAProtocol)
    {
        //    gpTest_cbDataConfirm(status,packetsSentOK,packetsSentError);
    }

    if(gpPTC_Txmc_On)
    {
        gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].packetTransmitted++;
        gpSched_ScheduleEvent(MS_TO_US(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].packetInterval),
                              Txmc_TransmitNextChannel);
    }
}

gpPTC_Result_t gpPTC_SetModeRequest(UInt8 clientID, UInt8 modeID, UInt32 exectime, gpPTC_ModeExecution_t OnOff,
                                    UInt8 numberOfExtraParameters, gpPTC_Parameter_t* parameters)
{
    if(!gpPTC_CheckCompatibleHWVersion())
    {
        return gpPTC_ResultInvalidChip;
    }
    gpPTC_Result_t result = gpPTC_ResultSuccess;
    GP_LOG_PRINTF("setmode %x %x exectime=%lu", 0, (UInt16)modeID, (UInt8)OnOff, (UInt32)exectime);

    if(exectime != 0 && OnOff == gpPTC_ModeExecution_On)
    {
        // what happens with the re-init in case of
        // schedule to off command for the command
        gpPTC_ModeParams_t* params = (gpPTC_ModeParams_t*)GP_POOLMEM_MALLOC(sizeof(gpPTC_ModeParams_t));
        gpPTC_SetModeRequestToStruct(clientID, modeID, 0, gpPTC_ModeExecution_Off, numberOfExtraParameters, parameters,
                                     params);
        // schedule the set mode off function
        gpSched_ScheduleEventArg(exectime * 1000, gpPTC_SetModeRequestFromStruct, params);
    }

    switch(modeID)
    {
        case gpPTC_ModeInfo:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            /* this is a placeholder since this should be handled in a seperate request because of the different return
             * value structure */
            /* keeping this here for OTA information ge*/
            break;
        }
        case gpPTC_ModeCarrierWave:
        {
            if(OnOff == gpPTC_ModeExecution_Off && gpTest_GetContinuousWaveMode() != CW_OFF)
            {
                gpTest_SetContinuousWaveMode(CW_OFF);

                gpSched_UnscheduleEvent(gpPTC_enableCWMode_M);
                gpSched_UnscheduleEvent(gpPTC_enableCWMode_U);

                if(gpPTC_OldRxState != gpTest_GetRxState())
                    gpTest_SetRxState(gpPTC_OldRxState);
            }
            else if(OnOff == gpPTC_ModeExecution_On)
            {
                gpPTC_OldRxState = gpTest_GetRxState();
                if(gpPTC_OldRxState != 0)
                    gpTest_SetRxState(0);

                gpPTC_mainReEnableCw(gpPTC_CWMode);
            }
            break;
        }
        case gpPTC_ModeEDScan:
        {
            UInt16 channelMask = 0;
            if(gpTest_GetPhyMode() != gpTest_PhyModeMac)
            {
                result = gpPTC_ResultUnsupported;
            }
            else
            {
                if(gpTest_GetContinuousWaveMode() != CW_OFF)
                {
                    result = gpPTC_ResultBusy;
                    break;
                }
                else
                {
                    if(OnOff == gpPTC_ModeExecution_On)
                    {
                        /* Don't expect user to input channelMask, get current channel and bit shift this into the
                         * channelmask */
                        BIT_SET(channelMask, (gpTest_GetChannel() - 11));
                        gpTest_EDScan(gpPTC_ScanCount, gpPTC_PacketInterValInMS, channelMask);
                        gpPTC_EDScanBusy = true;
                    }
                    else
                    {
                        /* no way of interrupting the ed scan once started, not foreseen in gphal, so try to do
                         * something meaningfull */
                        if(gpPTC_EDScanBusy)
                            result = gpPTC_ResultBusy;
                        else
                            result = gpPTC_ResultUnsupported;
                    }
                }
            }
            break;
        }
        case gpPTC_ModeRx:
        {
            if(OnOff == gpPTC_ModeExecution_On)
            {
                if(gpTest_SetRxModeOptions(gpPTC_RXMultiStandard == gpPTC_Enabled,
                                           gpPTC_RXMultichannel == gpPTC_Enabled,
                                           gpPTC_RXHighSensitivity == gpPTC_Enabled) != gpHal_ResultSuccess)
                {
                    return gpPTC_ResultInvalidRXMode;
                }

                gpTest_SetRxState(1);
                gpPTC_OldRxState = gpTest_GetRxState();
            }
            else
            {
                gpTest_SetRxState(0);

                if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                {
#ifdef GP_PTC_ENABLE_BLE
                    gpPTC_RXPacketCount += gpTest_BleGetNumberOfRxPackets();
#endif
                }
                gpPTC_OldRxState = gpTest_GetRxState();
            }

            if((gpTest_GetPhyMode() == gpTest_PhyModeMac) && (gpPTC_RXMultichannel == gpPTC_Enabled))
            {
                if(gpPTC_OldRxState)
                {
                    gpTest_SetChannelForOtherStacks(gpPTC_SrcId1, gpPTC_SrcId2);
                }
                else
                {
                    gpTest_SetChannelForOtherStacks(GP_HAL_MULTICHANNEL_INVALID_CHANNEL,
                                                    GP_HAL_MULTICHANNEL_INVALID_CHANNEL);
                }
            }

            break;
        }
        case gpPTC_ModeTransmitPacket:
        {
            switch(OnOff)
            {
                case gpPTC_ModeExecution_On:
                {
                    if(gpPTC_PacketCount == 0)
                    {
                        return gpPTC_ResultInvalidArgument;
                    }
                    if(gpTest_GetPhyMode() == gpTest_PhyModeMac)
                    {
                        if((gpPTC_pTxData == NULL) || (gpPTC_pTxDataLength == 0))
                        {
                            return gpPTC_ResultTXDataNotSet;
                        }
                        /* This is already checked in gpPTC_SetByteDataForAttributeRequest */
                        else if((gpPTC_pTxDataLength < 3) || (gpPTC_pTxDataLength > 125))
                        {
                            return gpPTC_ResultInvalidTXDataLength;
                        }
                        else
                        {
                            gpTest_TxPacket(gpPTC_PacketCount, gpPTC_PacketInterValInMS, gpPTC_pTxDataLength,
                                            gpPTC_pTxData, 0);
                        }
                        gpPTC_IsBusy = true;
                    }
                    else if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
#ifdef GP_PTC_ENABLE_BLE
                        if(gpPTC_PacketLength < 3 || (gpPTC_PacketLength > 241))
                        {
                            return gpPTC_ResultInvalidArgument;
                        }
                        gpPTC_BleTxOn = true;
                        gpTest_BleTestEnd();
                        /// TODO : cleanup
                        gpTest_BleSetNumberTxPacketsInTestMode(gpPTC_PacketCount);
                        gpTest_BleTransmitterTest(gpPTC_PacketLength, gpPTC_DirectTestMode_PacketType);
#endif
                    }

                    break;
                }
                case gpPTC_ModeExecution_Off:
                {
                    if(gpTest_GetPhyMode() == gpTest_PhyModeMac)
                    {
                        gpTest_TxPacket(0, 0, 0, NULL, false);
                        gpPTC_IsBusy = false;
                    }
                    else if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
#ifdef GP_PTC_ENABLE_BLE
                        // normally this returns the number of sent packets, but this is now always 0xFFFF
                        gpPTC_TXPacketCount += gpTest_BleTestEnd();
                        gpPTC_BleTxOn = false;
#endif
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case gpPTC_ModeTransmitRandomPackets:
        {
            switch(OnOff)
            {
                case gpPTC_ModeExecution_On:
                {
                    if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
                        return gpPTC_ResultUnsupported;
                    }
                    else
                    {
                        if(gpPTC_PacketInterValInMS < 0)
                        {
                            return gpPTC_ResultInvalidArgument;
                        }
                        if(gpPTC_PacketCount <= 0)
                        {
                            return gpPTC_ResultInvalidArgument;
                        }
                        if((gpPTC_PacketLength <= 3) || (gpPTC_PacketLength > 125))
                        {
                            return gpPTC_ResultInvalidArgument;
                        }
                        gpTest_TxPacket(gpPTC_PacketCount, gpPTC_PacketInterValInMS, gpPTC_PacketLength, NULL, true);
                        gpPTC_IsBusy = true;
                    }
                    break;
                }
                case gpPTC_ModeExecution_Off:
                {
                    if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
                        return gpPTC_ResultUnsupported;
                    }
                    else
                    {
                        gpTest_TxPacket(0, 0, 0, NULL, false);
                        gpPTC_IsBusy = false;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }

            break;
        }
        case gpPTC_ModePrintPacketCount:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            /* send back the counters as extraparameters */
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 6);
            gpTest_Statistics_t stats;
            gpTest_GetStatistics(&stats);

            if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
            {
                if((1 == gpTest_GetRxState()) || (true == gpPTC_BleTxOn))
                {
                    // Read BLE statistics is only valid if RX and TX mode is off
                    return gpPTC_ResultSetModeFailed;
                }
                parameters[2].value = gpPTC_RXPacketCount;
                parameters[3].value = gpPTC_TXPacketCount;
                parameters[4].value = 0;
            }
            else
            {
                parameters[2].value = stats.ReceivedPacketCounterC;
                parameters[3].value = stats.PacketsSentOK;
                parameters[4].value = stats.PacketsSentError;
            }
            if(gpPTC_RXPacketCount)
            {
                parameters[0].value = gpPTC_AllRSSI / gpPTC_RXPacketCount;
                parameters[1].value = gpPTC_AllLQI / gpPTC_RXPacketCount;
            }
            else
            {
                parameters[0].value = 0;
                parameters[1].value = 0;
            }
            parameters[5].value = gpPTC_DumpP;
            break;
        }
        case gpPTC_ModePrintPacketCountForChannel:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            /* send back the counters as extraparameters */
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 5);
            gpTest_Statistics_t stats;
            if(gpTest_GetStatisticsForChannel(&stats, gpPTC_PCHStatChannel) == gpHal_ResultInvalidParameter)
            {
                return gpPTC_ResultInvalidArgument;
            }

            if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
            {
                // not supported yet
                return gpPTC_ResultUnsupported;
            }
            else
            {
                if(gpTest_GetRSSIForChannel(gpPTC_PCHStatChannel, (Int8*)&parameters[0].value) ==
                   gpHal_ResultInvalidParameter)
                {
                    return gpPTC_ResultInvalidArgument;
                }
                if(gpTest_GetLQIForChannel(gpPTC_PCHStatChannel, (UInt8*)&parameters[1].value) ==
                   gpHal_ResultInvalidParameter)
                {
                    return gpPTC_ResultInvalidArgument;
                }
                parameters[2].value = stats.ReceivedPacketCounterC;
                parameters[3].value = stats.PacketsSentOK;
                parameters[4].value = stats.PacketsSentError;
            }
            break;
        }
        case gpPTC_ModeResetCounters:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            gpTest_ResetStatistics();
            gpPTC_RXPacketCount = 0;
            gpPTC_TXPacketCount = 0;
            gpPTC_AllRSSI = 0;
            gpPTC_AllLQI = 0;
            break;
        }
        case gpPTC_ModeResetCountersForChannel:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            if(gpTest_ResetStatisticsForChannel(gpPTC_PCHStatChannel) == gpHal_ResultInvalidParameter)
            {
                return gpPTC_ResultInvalidArgument;
            }
            gpPTC_RXPacketCount = 0;
            gpPTC_TXPacketCount = 0;
            break;
        }
        case gpPTC_ModePrintReceivedPackets:
        {
            // BLE mode is way faster than UART
            if(gpTest_GetPhyMode() != gpTest_PhyModeMac)
            {
                gpPTC_PrintRXPackets = false;
                result = gpPTC_ResultUnsupported;
            }
            else
            {
                if(OnOff == gpPTC_ModeExecution_On)
                {
                    gpPTC_PrintRXPackets = true;
                }
                else
                {
                    gpPTC_PrintRXPackets = false;
                }
            }
            break;
        }
        case gpPTC_ModeSleep:
        {
            if(OnOff == gpPTC_ModeExecution_On)
            {
                GP_LOG_PRINTF("Configuring for sleep... ", 0);
#if defined(GP_DIVERSITY_GPHAL_XP4002)
                if(gpPTC_MapClockIOPin_Enabled != 0)
                {
                    restoreClkToGPIORegisters();
                }
#endif
                gpTest_SetAsleep();
            }
            else
            {
                hal_SleepSetGotoSleepEnable(false);
                if(!gpTest_IsAwake())
                {
                    gpTest_WakeUpGP();
                }
            }
            break;
        }
        case gpPTC_ModeWakeUp:
        {
            // can be removed....
            if(!gpTest_IsAwake())
            {
                gpTest_WakeUpGP();
            }
            break;
        }
        case gpPTC_ModeReadRegister:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 1);

            UInt8 regVal = gpTest_ReadReg(gpPTC_RRAddress);
            parameters[0].value = regVal;
            result = gpPTC_ResultSuccess;
            break;
        }
        case gpPTC_ModeGetTxPowerChannel:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 1);

            Int8 transmitPower;
            result = gpTest_GetTxPowerChannel((UInt8)gpPTC_WgetChannel, &transmitPower);
            parameters[0].value = transmitPower;
            break;
        }
        case gpPTC_ModeWriteRegister:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            Bool retVal = gpTest_WriteReg(gpPTC_WRAddress, gpPTC_WRValue);
            if(retVal)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetModeFailed;
            }
            break;
        }
        case gpPTC_ModeSetTxPowerChannel:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            result = gpTest_SetTxPowerChannel((UInt8)gpPTC_WsetChannel, (Int8)gpPTC_WsetPower);
            break;
        }
        case gpPTC_ModeWriteRegisterBits:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            //   GP_LOG_PRINTF("gpPTC_WRAddress %x",0,(UInt32)gpPTC_WRAddress);
            //  GP_LOG_PRINTF("gpPTC_WRValue %x",0,(UInt32)gpPTC_WRValue);
            // first readout the register
            UInt8 readVal = gpTest_ReadReg(gpPTC_WRAddress);
            // GP_LOG_PRINTF("readreg %x",0,readVal);
            // GP_LOG_PRINTF("gpPTC_WRBitMask %i",0,(readVal & ~gpPTC_WRBitMask));
            UInt8 bitmask = (UInt8)gpPTC_WRBitMask;
            int i;
            for(i = 0; i < 8; i++)
            {
                int res = bitmask & 1;
                if(res == 1)
                    break;
                bitmask >>= 1;
            }
            // apply inverse bit mask with the value to set
            // value needs to be shifted to  the correct position
            UInt8 newValue = (readVal & ~gpPTC_WRBitMask) | (gpPTC_WRValue << i);
            // get the first set bit from the bitmask to get the starting position

            // GP_LOG_PRINTF("newValue %x",0,newValue);
            Bool retVal = gpTest_WriteReg(gpPTC_WRAddress, newValue);

            if(retVal)
            {
                result = gpPTC_ResultSuccess;
            }
            else
            {
                result = gpPTC_ResultSetModeFailed;
            }
            break;
        }
#if defined(GP_DIVERSITY_GPHAL_XP4002)
        case gpPTC_ModeSetClockToGPIO:
        {
            if(gpPTC_MapIOClockType != gpHal_SleepMode32kHz)
            {
                return gpPTC_ResultUnsupported;
            }

            if(OnOff == gpPTC_ModeExecution_On)
            {
                // make sure no other mode is running !
                if(gpPTC_IsBusy)
                {
                    // what to do ?
                    return gpPTC_ResultBusy;
                }

                gpPTC_IsBusy = true;
                gpPTC_MapClockIOPin_Enabled = gpPTC_MapIOClockPin;

                result = mapClkToGPIORegisters();
            }
            else if(OnOff == gpPTC_ModeExecution_Off)
            {
                if(gpPTC_MapClockIOPin_Enabled == 0)
                {
                    return gpPTC_ResultSetModeFailed;
                }

                restoreClkToGPIORegisters();
            }
            break;
        }
#endif // GP_DIVERSITY_GPHAL_K8A || defined(GP_DIVERSITY_GPHAL_K8C) || defined(GP_DIVERSITY_GPHAL_K8D) ||
       // defined(GP_DIVERSITY_GPHAL_K8E)
       // || defined(GP_DIVERSITY_GPHAL_XP4002)
        case gpPTC_ModeSetPdmClock:
        {
            gpTest_PDMClkSrc_t src = gpPTC_PDMClkSrc_None;

            gpPTC_PDMClkCfg.state = (OnOff == gpPTC_ModeExecution_On);

            if(!gpPTC_PDMClkCfg.state)
            {
                src = gpPTC_PDMClkSrc_None;
            }
            else if(gpPTC_PDMClkCfg.src == gpPTC_PDMClkSrc_2M)
            {
                src = gpTest_PDMClkSrc_2M;
            }
            else if(gpPTC_PDMClkCfg.src == gpPTC_PDMClkSrc_PLL)
            {
                src = gpTest_PDMClkSrc_PLL;
            }

            result = gpTest_SetPdmClk(src, gpPTC_PDMClkCfg.freqHz, gpPTC_PDMClkCfg.gpio);
            break;
        }

        case gpPTC_ModeSetXtalTrimCap:
        {
            result = gpTest_Xtal32MHzTrimConfigOverwrite(gpPTC_XtalTrimCapCfg.zoneMaxTemps,
                                                         gpPTC_XtalTrimCapCfg.zoneSettings, PTC_TRIMCAP_ZONE_COUNT);
            break;
        }
        case gpPTC_ModeTransmitMultipleChannels:
        {
            switch(OnOff)
            {
                case gpPTC_ModeExecution_On:
                {
                    Bool dataFound = false;
                    if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
                        return gpPTC_ResultUnsupported;
                    }
                    for(UIntLoop i = 0; i < PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX; i++)
                    {
                        gpPTC_TxmcParams[i].packetTransmitted = 0;
                        if(!dataFound && (gpPTC_TxmcParams[i].pData != NULL) && (gpPTC_TxmcParams[i].dataLength > 0))
                        {
                            dataFound = true;
                        }
                    }
                    if(!dataFound)
                    {
                        return gpPTC_ResultTXDataNotSet;
                    }
                    for(UIntLoop i = 0; i < PTC_NB_OF_TXMC_CHANNEL_SEQUENCE_IDX; i++)
                    {
                        if(PTC_CHECK_802_15_4_CHANNEL_RANGE(gpPTC_TxmcParams[i].channel))
                        {
                            gpPTC_Txmc_ChSeqIdxToTransmit = i;
                            gpPTC_Txmc_On = true;

                            GP_ASSERT_DEV_INT(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].pData != NULL);
                            gpTest_SetChannel(gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].channel);
                            gpTest_TxPacket(1, 0, gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].dataLength,
                                            gpPTC_TxmcParams[gpPTC_Txmc_ChSeqIdxToTransmit].pData, 0);
                            gpPTC_IsBusy = true;
                            break;
                        }
                    }
                    break;
                }
                case gpPTC_ModeExecution_Off:
                {
                    if(gpTest_GetPhyMode() == gpTest_PhyModeBle)
                    {
                        return gpPTC_ResultUnsupported;
                    }
                    gpPTC_Txmc_On = false;
                    gpTest_TxPacket(0, 0, 0, NULL, false);
                    gpPTC_IsBusy = false;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case gpPTC_ModePrintTxmcConfiguraiton:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 4);

            parameters[0].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].channel;
            parameters[1].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetCount;
            parameters[2].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].dataLength;
            parameters[3].value = gpPTC_TxmcParams[gpPTC_Txmc_CurrentChSeqIdx].packetInterval;
            break;
        }
        case gpPTC_ModePrintAntennaStatistics:
        {
            // Prevent On or Off setting for these modes
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            if(gpTest_PhyModeBle == gpTest_GetPhyMode())
            {
                return gpPTC_ResultUnsupported;
            }
            /* send back the counters as extraparameters */
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 4);
            gpTest_AntennaStatistics_t antStats[GP_TEST_NUMBER_OF_ANTENNAS];
            gpTest_GetAntennaStatistics(&antStats[0], &antStats[1]);
            parameters[0].value = antStats[0].ReceivedPacketCounter;
            if(antStats[0].ReceivedPacketCounter)
            {
                parameters[1].value = antStats[0].CumulativeRssi / antStats[0].ReceivedPacketCounter;
            }
            else
            {
                parameters[1].value = 0;
            }
            parameters[2].value = antStats[1].ReceivedPacketCounter;
            if(antStats[1].ReceivedPacketCounter)
            {
                parameters[3].value = antStats[1].CumulativeRssi / antStats[1].ReceivedPacketCounter;
            }
            else
            {
                parameters[3].value = 0;
            }
            break;
        }
        case gpPTC_ModeResetAntennaStatistics:
        {
            // Prevent On or Off setting for this mode
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            gpTest_ResetAntennaStatistics();
            break;
        }
        case gpPTC_ModeGetChipTemp:
        {
            // Prevent On or Off setting for this mode
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            GP_ASSERT_DEV_INT(numberOfExtraParameters == 1);
            parameters[0].value = gpTest_GetChipTemperature();
            break;
        }
        case gpPTC_ModeGetCapacitorLoad:
        {
#if defined(GP_DIVERSITY_GPHAL_XP4002)
            // Prevent On or Off setting for this mode
            if(OnOff != gpPTC_ModeExecution_Single)
            {
                return gpPTC_ResultSetModeFailed;
            }
            GP_ASSERT_DEV_INT(numberOfExtraParameters == gpTest_GetSizeOfCapacitorLoad());
            for(UInt8 idx = 0; idx < gpTest_GetSizeOfCapacitorLoad(); idx++)
            {
                parameters[idx].value = gpTest_GetCapacitorLoad(idx);
            }
#else
            // Setting Capacitor Load in runtime available only for xp4002
            result = gpPTC_ResultInvalidChip;
#endif
            break;
        }

        default:
        {
            result = gpPTC_ResultUnsupported;
            break;
        }
    } // switch
    return result;
}

Int16 gpPTC_GetInternalTemp(UInt8 clientID)
{
    return gpTest_GetChipTemperature();
}

Int16 gpPTC_GetCapacitorLoad(UInt8 tempIndex)
{
    return gpTest_GetCapacitorLoad(tempIndex);
}

gpPTC_Result_t mapClkToGPIORegisters(void)
{
    gpPTC_Result_t result = gpPTC_ResultSuccess;

#if defined(GP_DIVERSITY_GPHAL_XP4002)
    // Set the standby mode by writing the register
    // Using the gpTest_SetSleepMode function will not work, because this function
    // will only set a variable that will be used when actually going to sleep
    gpPTC_SleepMode_OrigValue = gpHal_GetSleepMode();
    gpHal_SetSleepMode(gpPTC_MapIOClockType);

    gpTest_get_DebugSelection(gpPTC_MapIOClockPin, &gpPTC_DBG_Sel_OrigValue);
    gpTest_get_AlternateEnabled(gpPTC_MapIOClockPin, &gpPTC_ALT_En_OrigValue);
    gpTest_get_AlternateSelection(gpPTC_MapIOClockPin, &gpPTC_ALT_Sel_OrigValue);

    if(gpPTC_MapIOClockType == gpHal_SleepMode32kHz)
    {
        gpTest_MapSleepClock(gpPTC_MapIOClockPin, GP_WB_ENUM_RTC_TIMER_SRC_XT32M);
    }
#endif
    return result;
}

gpPTC_Result_t restoreClkToGPIORegisters(void)
{
    gpPTC_Result_t result = gpPTC_ResultSuccess;

#if defined(GP_DIVERSITY_GPHAL_XP4002)
    if(gpHal_SetSleepMode(gpPTC_SleepMode_OrigValue) != gpHal_ResultSuccess)
    {
        result = gpPTC_ResultBusy;
    }

    gpTest_set_DebugSelection(gpPTC_MapClockIOPin_Enabled, gpPTC_DBG_Sel_OrigValue);
    gpTest_MapSleepClock(gpPTC_MapClockIOPin_Enabled, 0xFF);
    gpTest_set_AlternateSelection(gpPTC_MapClockIOPin_Enabled, gpPTC_ALT_Sel_OrigValue);
    gpTest_set_AlternateEnabled(gpPTC_MapClockIOPin_Enabled, gpPTC_ALT_En_OrigValue);

    if(result == gpPTC_ResultSuccess)
    {
        gpPTC_IsBusy = false;
        gpPTC_MapClockIOPin_Enabled = 0;
    }
#endif

    return result;
}

void gpTest_cbDataIndication(UInt8 length, gpPd_Offset_t dataOffset, gpPd_Handle_t handle)
{
    UInt8 psdu[GP_TEST_MAX_LENGTH_PACKET];
    UInt8 antenna;
    Int8 rssi;
    UInt8 lqi;
    UInt8 channel;
    gpPTC_StatsDump_t statsDump;

    static UInt16 prevRxAntenna0 = 0;
    length = ((length <= GP_TEST_MAX_LENGTH_PACKET) ? length : GP_TEST_MAX_LENGTH_PACKET); // Clip length

    gpPTC_RXPacketCount++;
    rssi = (Int8)gpPd_GetRssi(handle);
    gpPTC_AllRSSI += rssi;

    lqi = (UInt8)gpPd_GetLqi(handle);
    gpPTC_AllLQI += lqi;

    channel = (UInt8)gpPd_GetRxChannel(handle);

    gpTest_Statistics_t stats;
    gpTest_GetStatistics(&stats);
    antenna = (stats.RxAntenna0 != prevRxAntenna0 && stats.RxAntenna0 != 0) ? 0 : 1;
    prevRxAntenna0 = stats.RxAntenna0;

    statsDump = gpPTC_DumpRxp;

    if(gpPTC_PrintRXPackets)
    {
        gpPd_ReadByteStream(handle, dataOffset, length, psdu);
        gpPTC_RXPacketIndication(lqi, antenna, rssi, channel, length, psdu, statsDump);
    }
    gpPd_FreePd(handle);
}

void gpTest_cbEDConfirm(UInt8 result, UInt16 channelMask, UInt8* pData, Bool Finished)
{
    UInt16 channelIndex = (gpTest_GetChannel() - 11);
    if(Finished)
    {
        gpPTC_EDScanBusy = false;
    }
    gpPTC_EDConfirm(result, Finished, pData[channelIndex]);
}
