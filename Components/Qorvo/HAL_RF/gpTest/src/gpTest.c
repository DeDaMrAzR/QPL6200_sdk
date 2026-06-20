/*
 * Copyright (c) 2017-2023, Qorvo Inc
 *
 * gpTest.c
 *
 *  The file is contains generic test functions, to be used in the Evaluation application.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpBsp.h"
#include "hal.h"

#include "gpSched.h"
#include "gpHal.h"
#include "gpHal_Statistics.h"
#include "gpReset.h"
#include "gpPd.h"
#include "gpHal_MAC_Ext.h"

#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "esecure.h"
#include "esec_qorvo.h"
#include "qDrvTsens.h"
#endif //defined(GP_DIVERSITY_GPHAL_XP4002)

#include "gpTest.h"
#ifdef GP_COMP_GPHAL_MAC
#include "gpTestMac.h"
#endif
#ifdef GP_COMP_GPTEST_BLE
#include "gpTestBle.h"
#endif

#include "gpLog.h"

#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#include "gpHal_kx_Ipc.h"
#endif



#ifdef GP_COMP_RADIO
#include "gpRadio.h"
#endif

#if defined(GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP)
#include "gpHal_TrimRxMode.h"
#endif // defined(GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP)


#if defined(GP_DIVERSITY_GPHAL_XP4002)
#include "gpAppUcSubsystem_Calibrations.h"
#endif
// #ifdef GP_HAL_DIVERSITY_TEST
// #include "gpHal_Test.h"
// #endif

#if defined(HAL_DIVERSITY_V2)
#include "qRegInfoPage.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_TEST

#if !defined(GP_COMP_GPTEST_BLE) && !defined(GP_COMP_GPHAL_MAC)
#error "define a GPHAL MAC or GP_COMP_GPTEST_BLE"
#endif

#define SINGLE_ED_MEAS                      8*16 /*us*/
#define PDM_CLOCK_DUTY_CYCLE                0.5  // 50%

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

gpTest_PhyMode_t gpTest_PhyMode = gpTest_PhyModeMac;
gpTest_ContinuousWaveMode_t gpTest_ContinuousWaveMode;

#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
UInt16 gpTest_IpcRestart_numberOfRestarts = 0;
UInt16 gpTest_IpcRestart_intervalInUs     = 0;
UInt16 gpTest_IpcRestart_stopDurationUs   = 0;
UInt16 gpTest_IpcRestart_delayUs          = 0;
#define IPC_RESTART_TRIGGER_IMMEDIATE           0x00
#define IPC_RESTART_TRIGGER_TX                  0x01
#define IPC_RESTART_TRIGGER_INVALID             0x02
UInt8  gpTest_IpcRestart_trigger          = 0;
UInt32 gpTest_IpcStopDurationUs = 0 ;
UInt32 gpTest_IpcRestartDurationUs = 0 ;

gpHal_IpcBackupRestoreFlags_t gpTest_IpcBackupRestoreFlags;
#endif



/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
static void gpTest_IpcRestart_doRestart(void);
static void gpTest_IpcRestart_doNextStop(void);
#endif

#if defined(GP_DIVERSITY_GPHAL_XP4002) 
static gpTest_Result_t gpTest_PdmClkOutCfg(UInt8 gpio, Bool state);
#endif


/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

 void gpTest_UnregisterCallbacks(void)
 {
 #ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacUnregisterCallbacks();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_Start(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacStart();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleStart();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }

    // init hardware
    gpTest_SetAntennaDiversity(true);
    gpTest_SetAntenna(gpHal_AntennaSelection_Auto);
}

void gpTest_Stop(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacStop();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleStop();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

/**
 * @brief Getter method for the chip ID.
 * @return The identifier of the silicon.
*/
UInt8 gpTest_GetChipId()
{
    return gpHal_GetChipId();
}

/**
 * @brief Getter method for the chip version.
 * @return The metal fix version of the chip.
*/
UInt8 gpTest_GetChipVersion(void)
{
    return gpHal_GetChipVersion();
}


gpHal_Result_t gpTest_SetChannel(UInt8 channel)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacSetChannel(channel);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleSetChannel(channel);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return gpHal_ResultInvalidRequest;
}

UInt8 gpTest_GetChannel(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetChannel();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetChannel();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return 0xFF;
}

gpHal_Result_t gpTest_SetAntenna(gpHal_AntennaSelection_t antenna)
{
#if defined(GP_COMP_GPHAL_MAC) || defined(GP_COMP_GPTEST_BLE)
    gpTest_SetRxAntenna(antenna);
    gpTest_SetTxAntenna(antenna);
    return gpHal_ResultSuccess;
#else
    GP_ASSERT_DEV_INT(false);
    return gpHal_ResultInvalidRequest;
#endif
}

void gpTest_SetRxAntenna(gpHal_AntennaSelection_t antenna)
{
    /* Set the Rx Antenna for both ZB and BLE */
#ifdef GP_HAL_DIVERSITY_SINGLE_ANTENNA
    antenna = GP_HAL_DIVERSITY_SINGLE_ANTENNA;
#endif
    if(antenna == gpHal_AntennaSelection_Ant0 || antenna == gpHal_AntennaSelection_Ant1)
    {
        gpHal_SetRxAntenna(antenna);
    }
    else if (antenna == gpHal_AntennaSelection_Auto)
    {
        gpHal_SetRxAntenna(gpHal_AntennaSelection_Auto);
    }
    else
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_SetTxAntenna(gpHal_AntennaSelection_t antenna)
{
/* assymetric Set functions :
 * we set the Tx antenna for ZB
 * and not for BLE (for BLE the antenna is the same for RX and TX).
 */
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetTxAntenna(antenna);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return; /* do nothing */
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

gpHal_AntennaSelection_t gpTest_GetTxAntenna(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetTxAntenna();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetTxAntenna();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return 0xFF;
}

gpHal_AntennaSelection_t gpTest_GetRxAntenna(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpHal_GetRxAntenna();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetRxAntenna();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return 0xFF;
}

void gpTest_SetModulation(gpTest_BleTxPhy_t modulation)
{
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetModulation(modulation);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_SetTxPower (Int8 transmitPower)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetTxPower(transmitPower);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetTxPower(transmitPower);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

Int8 gpTest_GetTxPower(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetTxPower();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetTxPower();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return -1;
}

gpTest_Result_t gpTest_SetTxPowerChannel(gpHal_Channel_t channel, Int8 transmitPower)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacSetTxPowerChannel(channel, transmitPower);
#else
    return -1;
#endif
}

gpTest_Result_t gpTest_GetTxPowerChannel(gpHal_Channel_t channel, Int8* pTransmitPower)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetTxPowerChannel(channel, pTransmitPower);
#else
    return -1;
#endif
}

gpHal_TxPower_t gpTest_GetLastUsedTxPower(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetLastUsedTxPower();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

void gpTest_SetAntennaDiversity(Bool OnOff)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetAntennaDiversity(OnOff);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
        if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetAntennaDiversity(OnOff);
    }
    else
#endif
        if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}


Bool gpTest_GetAntennaDiversity(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetAntennaDiversity();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
        if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetAntennaDiversity();
    }
    else
#endif
        if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}


void gpTest_SetContinuousWaveMode(gpTest_ContinuousWaveMode_t newMode)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetContinuousWaveMode(newMode);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetContinuousWaveMode(newMode);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

gpTest_ContinuousWaveMode_t gpTest_GetContinuousWaveMode(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetContinuousWaveMode();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetContinuousWaveMode();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

Bool gpTest_GetContTx(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetContTx();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

void gpTest_SetMaxBE(UInt8 maxBE)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetMaxBE(maxBE);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

UInt8 gpTest_GetMaxBE(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetMaxBE();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

void gpTest_SetMinBE(UInt8 minBE)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetMinBE(minBE);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

UInt8 gpTest_GetMinBE(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetMinBE();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

void gpTest_SetRxState(Bool flag)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetRxState(flag);
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetRxState(flag);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

Bool gpTest_GetRxState(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetRxState();
    }
    else
#endif
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleGetRxState();
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

gpTest_Result_t gpTest_SetNumberOfRetries(UInt8 retries)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacSetNumberOfRetries(retries);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return 0xFF;
}

UInt8 gpTest_GetNumberOfRetries( void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetNumberOfRetries();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return 0xFF;
}

void gpTest_SetMaxCSMABackoffs(UInt8 maxBackoffs)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetMaxCSMABackoffs(maxBackoffs);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

UInt8 gpTest_GetMaxCSMABackoffs(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetMaxCSMABackoffs();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }

    return 0xFF;
}

void gpTest_SetRetransmitOnCcaFail(Bool enable)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetRetransmitOnCcaFail(enable);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}
Bool gpTest_GetRetransmitOnCcaFail(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetRetransmitOnCcaFail();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return false;
}

void gpTest_SetRetransmitRandomBackoff(Bool enable)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetRetransmitRandomBackoff(enable);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}
Bool gpTest_GetRetransmitRandomBackoff(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetRetransmitRandomBackoff();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return false;
}



void gpTest_SetMinBeRetransmit(UInt8 minBERetransmit)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetMinBeRetransmit(minBERetransmit);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}
UInt8 gpTest_GetMinBeRetransmit(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetMinBeRetransmit();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return false;

}
void gpTest_SetMaxBeRetransmit(UInt8 maxBERetransmit)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetMaxBeRetransmit(maxBERetransmit);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}
UInt8 gpTest_GetMaxBeRetransmit(void)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        return gpTest_MacGetMaxBeRetransmit();
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
    return false;

}



void gpTest_SetAsleep(void)
{
      // goto sleep asap
#ifdef GP_SCHED_DIVERSITY_SLEEP
    hal_SleepSetGotoSleepThreshold(1000);
#endif
#ifdef GP_DIVERSITY_LOG
    gpLog_Flush();
#endif
    gpHal_GoToSleepWhenIdle(true);
#ifdef GP_SCHED_DIVERSITY_SLEEP
    hal_SleepSetGotoSleepEnable(true);
#endif
}

Bool gpTest_IsAwake(void)
{
#ifdef GP_DIVERSITY_GPHAL_XP4002
    return true;
#endif
    return gpHal_IsRadioAccessible();
}

void gpTest_SetSleepMode(gpHal_SleepMode_t mode)
{
    gpHal_SetSleepMode(mode);
}

gpHal_SleepMode_t gpTest_GetSleepMode(void)
{
   return gpHal_GetSleepMode();
}

gpHal_SleepClockMeasurementStatus_t gpTest_GetMeasuredSleepClockFrequency(gpHal_SleepMode_t mode, UInt32* frequencymHz)
{
    return gpHal_GetMeasuredSleepClockFrequency(mode, frequencymHz);
}

void gpTest_TxPacket(UInt16 numberOfPackets, UInt16 intervalInMs, UInt8 dataLength, UInt8* pData, UInt8 txOptions)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacTxPacket(numberOfPackets, intervalInMs, dataLength, pData, txOptions);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_MacSetExpectedRx(UInt8 dataLength, UInt8* pData)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTestMac_SetExpectedRx(dataLength, pData);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_TxCorruptedPacket(UInt16 numberOfPackets, UInt16 intervalInMs, UInt8 dataLength, UInt8* pData, UInt8 txOptions)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacTxCorruptedPacket(numberOfPackets, intervalInMs, dataLength, pData, txOptions);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_TxPollPacket(UInt16 numberOfPackets, UInt16 intervalInMs, UInt8 dataLength, UInt8* pData, UInt8 txOptions)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacTxPollPacket(numberOfPackets, intervalInMs, dataLength, pData, txOptions);
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
        if (gpTest_IpcRestart_trigger == IPC_RESTART_TRIGGER_TX)
        {
            gpSched_ScheduleEvent( gpTest_IpcRestart_delayUs, gpTest_IpcRestart_doNextStop);
            gpTest_IpcRestart_trigger = 0;
        }
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_SetRxResponsePacket(UInt32 delayUs, UInt8 dataLength, UInt8* pData, UInt8 txOptions)
{
#ifdef GP_COMP_GPHAL_MAC
    if(gpTest_PhyMode == gpTest_PhyModeMac)
    {
        gpTest_MacSetRxResponsePacket(delayUs, dataLength, pData, txOptions);
    }
    else
#endif
    if(false)
    {
        GP_ASSERT_DEV_INT(false);
    }
}

void gpTest_WakeUpGP(void)
{
    if(!gpTest_IsAwake())
    {
        gpHal_GoToSleepWhenIdle(false);
    }
}

void gpTest_EDScan(UInt16 numberOfScans, UInt16 intervalInMs, UInt16 channelMask)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacEDScan(numberOfScans, intervalInMs, channelMask, SINGLE_ED_MEAS);
#else
    GP_ASSERT_DEV_INT(false);
#endif
}

void gpTest_ExtendedEDScan(UInt16 numberOfScans, UInt16 intervalInMs, UInt16 channelMask, UInt32 duration_us)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacEDScan(numberOfScans, intervalInMs, channelMask, duration_us);
#else
    GP_ASSERT_DEV_INT(false);
#endif
}

void gpTest_ResetRequest(void)
{
    gpReset_ResetSystem();
}


void gpTest_SetCollisionAvoidanceModeToUse(gpHal_CollisionAvoidanceMode_t newMode)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacSetCollisionAvoidanceModeToUse(newMode);
#endif
}


gpHal_CollisionAvoidanceMode_t gpTest_GetCollisionAvoidanceModeInUse(void)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetCollisionAvoidanceModeInUse();
#else
    return 0;
#endif
}

UInt8 gpTest_GetAverageLQI(void)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetAverageLQI();
#else
    return 0;
#endif
}

Int8 gpTest_GetAverageRSSI(void)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetAverageRSSI();
#else
    return 0;
#endif
}

gpTest_Result_t gpTest_GetLQIForChannel(UInt8 channel, UInt8* pLqi)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetLQIForChannel(channel, pLqi);
#else
    return -1;
#endif
}

gpTest_Result_t gpTest_GetRSSIForChannel(UInt8 channel, Int8* pRssi)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetRSSIForChannel(channel, pRssi);
#else
    return -1;
#endif
}

void gpTest_GetVersionInfo(gpTest_VersioningIndex_t versionType, gpVersion_ReleaseInfo_t * pVersion)
{
    switch(versionType)
    {
        case gpTest_VersioningIndexChipId:
        {
            UInt16 HWversion;

            HWversion = gpHal_GetHWVersionId();
            pVersion->major  = (HWversion >> 0)  & (BM(6)-1);
            pVersion->minor = 0;
            pVersion->revision = 0;
            pVersion->patch = 0;
            break;
        }
        default:
        {
            pVersion->major = 0xFF;
            pVersion->minor = 0xFF;
            pVersion->revision = 0xFF;
            pVersion->patch = 0xFF;
            break;
        }
    }
}


UInt8 gpTest_ReadReg(UInt32 address)
{
    return gpHal_ReadReg(address);
}

Bool gpTest_WriteReg(UInt32 address, UInt8 writeByte)
{
    gpHal_WriteReg(address, writeByte);

    return true;
}


void gpTest_SetPacketInPacketMode(Bool newPIP)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetPipMode(newPIP);
#endif
}

Bool gpTest_GetPacketInPacketMode(void)
{
    return gpHal_GetPipMode();
}

void gpTest_GetSettings(gpTest_Settings_t * Settings)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacGetSettings(Settings);
#endif
}

void gpTest_GetStatistics(gpTest_Statistics_t* Statistics)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacGetStatistics(Statistics);
#endif
}

gpTest_Result_t gpTest_GetStatisticsForChannel(gpTest_Statistics_t* pStatistics, UInt8 channel)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacGetStatisticsForChannel(pStatistics, channel);
#else
    return -1;
#endif
}

void gpTest_ResetStatistics(void)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacResetStatistics();
#endif
}

gpTest_Result_t gpTest_ResetStatisticsForChannel(UInt8 channel)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpTest_MacResetStatisticsForChannel(channel);
#else
    return -1;
#endif
}

void gpTest_GetAntennaStatistics(gpTest_AntennaStatistics_t* ant0Statistics, gpTest_AntennaStatistics_t* ant1Statistics)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacGetAntennaStatistics(ant0Statistics, ant1Statistics);
#endif
}

void gpTest_ResetAntennaStatistics(void)
{
#ifdef GP_COMP_GPHAL_MAC
    gpTest_MacResetAntennaStatistics();
#endif
}

void gpTest_PrintfEnable(Bool enable)
{
#ifdef GP_DIVERSITY_LOG
    gpLog_PrintfEnable(enable);
#endif /* GP_DIVERSITY_LOG */
}

gpTest_Result_t gpTest_SetChannelForOtherStacks(UInt8 stack1_channel, UInt8 stack2_channel)
{
    gpTest_Result_t res = gpHal_ResultSuccess;
#ifdef GP_COMP_GPHAL_MAC
    res = gpTest_MacSetChannelForStack(1, stack1_channel);
    if(res == gpHal_ResultSuccess)
    {
        res = gpTest_MacSetChannelForStack(2, stack2_channel);
    }
#endif
    return res;
}

void gpTest_EnableExternalLna(Bool enable)
{
    GP_ASSERT_DEV_INT(false);
    //Need implementation for other chip variants, for older k5,k4 do we want to play with ackTransmitPower for enable/disable Lna ??
}

void gpTest_SetPhyMode(gpTest_PhyMode_t mode)
{
    gpTest_PhyMode = mode;
}

gpTest_PhyMode_t gpTest_GetPhyMode(void)
{
    return gpTest_PhyMode;
}

void gpTest_SetExtendedAddress(MACAddress_t* pExtendedAddress)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetExtendedAddress(pExtendedAddress, gpHal_SourceIdentifier_0);
#else
    GP_ASSERT_DEV_INT(false);
#endif
}

void gpTest_GetExtendedAddress(MACAddress_t* pExtendedAddress)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_GetExtendedAddress(pExtendedAddress, gpHal_SourceIdentifier_0);
#else
    GP_ASSERT_DEV_INT(false);
#endif
}

Bool gpTest_CheckDCDCEnable(void)
{
    return false;
}

void gpTest_SetPanId(UInt16 panID, gpHal_SourceIdentifier_t srcId)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetPanId(panID, srcId);
#else
    NOT_USED(panID);
    NOT_USED(srcId);
#endif

}
void gpTest_SetPromiscuousMode(Bool flag)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetPromiscuousMode(flag);
#else
    NOT_USED(flag);
#endif

}
void gpTest_SetSnifferMode(Bool flag)
{
    NOT_USED(flag);
}
void gpTest_SetShortAddress(UInt16 shortAddress, gpHal_SourceIdentifier_t srcId)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetShortAddress(shortAddress, srcId);
#else
    NOT_USED(shortAddress);
    NOT_USED(srcId);
#endif
}

void gpTest_SetAddressRecognition(Bool enable, Bool panCoordinator)
{
#ifdef GP_COMP_GPHAL_MAC
    gpHal_SetAddressRecognition(enable, panCoordinator);
#else
    NOT_USED(enable);
    NOT_USED(panCoordinator);
#endif
}

UInt16 gpTest_GetPanId(gpHal_SourceIdentifier_t srcId)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpHal_GetPanId(srcId);
#else
    return 0xFFFF;
#endif
}

Bool gpTest_GetPromiscuousMode(void)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpHal_GetPromiscuousMode();
#else
    return false;
#endif
}

UInt16 gpTest_GetShortAddress(gpHal_SourceIdentifier_t srcId)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpHal_GetShortAddress(srcId);
#else
    return 0xFFFF;
#endif
}
Bool gpTest_GetAddressRecognition(void)
{
#ifdef GP_COMP_GPHAL_MAC
    return gpHal_GetAddressRecognition();
#else
    return false;
#endif
}

gpHal_Result_t gpTest_SetMcuClockSpeed(UInt8 clockSpeed)
{
    if(clockSpeed > 0xa)
    {
        return 0xff;
    }
#ifdef GP_DIVERSITY_CORTEXM4
    HAL_SET_MCU_CLOCK_SPEED(clockSpeed);
    return gpHal_ResultSuccess;
#else
    return 0x08;
#endif // GP_DIVERSITY_CORTEXM4
}

#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
ALWAYS_INLINE UInt32 calcTimeDiff(UInt32 t0, UInt32 t1) // should really be implemented in hal
{
    return t0 < t1 ? (t1 - t0) : ((0xFFFFFFFF-t0) + t1);
}

void gpTest_IpcRestart_doRestart(void)
{
    UInt32 t_start;
    UInt32 t_end;

    HAL_TIMER_GET_CURRENT_TIME_1US(t_start);
    gpHal_IpcRestart(&gpTest_IpcBackupRestoreFlags);
    HAL_TIMER_GET_CURRENT_TIME_1US(t_end);
    gpTest_IpcRestartDurationUs = max(gpTest_IpcRestartDurationUs, calcTimeDiff(t_start,t_end));

    gpTest_IpcRestart_numberOfRestarts--;
    gpSched_ScheduleEvent( gpTest_IpcRestart_intervalInUs, gpTest_IpcRestart_doNextStop );
}

void gpTest_IpcRestart_doNextStop(void)
{
    UInt32 t_start;
    UInt32 t_end;

    if (0 == gpTest_IpcRestart_numberOfRestarts)
    {
        gpTest_cbIpcRestartConfirm(0, gpTest_IpcStopDurationUs, gpTest_IpcRestartDurationUs);
        return;
    }

    HAL_TIMER_GET_CURRENT_TIME_1US(t_start);
    gpHal_IpcStop(&gpTest_IpcBackupRestoreFlags);
    HAL_TIMER_GET_CURRENT_TIME_1US(t_end);
    gpTest_IpcStopDurationUs = max(gpTest_IpcStopDurationUs, calcTimeDiff(t_start,t_end));

    gpSched_ScheduleEvent( gpTest_IpcRestart_stopDurationUs, gpTest_IpcRestart_doRestart);
}
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)

gpHal_Result_t gpTest_IpcRestart(UInt16 numberOfRestarts, UInt32 intervalInUs, UInt32 stopDurationUs, UInt32 delayUs, UInt8 trigger)
{
#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
    if (trigger >= IPC_RESTART_TRIGGER_INVALID)
    {
        return gpHal_ResultInvalidParameter;
    }

    gpTest_IpcRestart_numberOfRestarts = numberOfRestarts;
    gpTest_IpcRestart_intervalInUs     = intervalInUs;
    gpTest_IpcRestart_stopDurationUs   = stopDurationUs;
    gpTest_IpcRestart_trigger          = trigger;
    gpTest_IpcRestart_delayUs          = delayUs;
    gpTest_IpcStopDurationUs           = 0;
    gpTest_IpcRestartDurationUs        = 0;

    if (gpTest_IpcRestart_trigger == IPC_RESTART_TRIGGER_IMMEDIATE)
    {
        gpSched_ScheduleEvent( gpTest_IpcRestart_delayUs, gpTest_IpcRestart_doNextStop);
    }
    return gpHal_ResultSuccess;
#else
    NOT_USED(numberOfRestarts);
    NOT_USED(intervalInUs);
    NOT_USED(stopDurationUs);
    NOT_USED(delayUs);
    NOT_USED(trigger);
    return gpHal_ResultInvalidRequest;
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
}


void gpTest_StatisticsCountersGet(gpTest_StatisticsCounter_t* pStatisticsCounters)
{
#if           \
    defined(GP_DIVERSITY_GPHAL_XP4002)
    gpHal_StatisticsMacCounter_t macCnt;
    gpHal_StatisticsCoexCounter_t coexCnt;

    MEMSET(&macCnt,  0x0, sizeof(gpHal_StatisticsMacCounter_t));
    MEMSET(&coexCnt, 0x0, sizeof(gpHal_StatisticsCoexCounter_t));

    gpHal_StatisticsCountersGet(&macCnt, &coexCnt);

    pStatisticsCounters->ccaFails = macCnt.ccaFails;
    pStatisticsCounters->txRetries = macCnt.txRetries;
    pStatisticsCounters->failTxNoAck = macCnt.failTxNoAck;
    pStatisticsCounters->failTxChannelAccess = macCnt.failTxChannelAccess;
    pStatisticsCounters->successTx = macCnt.successTx;
    pStatisticsCounters->totalRx = macCnt.totalRx;
    pStatisticsCounters->pbmOverflow = macCnt.pbmOverflow;

    pStatisticsCounters->coexReq.prio0 = coexCnt.coexReq.prio0;
    pStatisticsCounters->coexReq.prio1 = coexCnt.coexReq.prio1;
    pStatisticsCounters->coexReq.prio2 = coexCnt.coexReq.prio2;
    pStatisticsCounters->coexReq.prio3 = coexCnt.coexReq.prio3;
    pStatisticsCounters->coexGrant.prio0 = coexCnt.coexGrant.prio0;
    pStatisticsCounters->coexGrant.prio1 = coexCnt.coexGrant.prio1;
    pStatisticsCounters->coexGrant.prio2 = coexCnt.coexGrant.prio2;
    pStatisticsCounters->coexGrant.prio3 = coexCnt.coexGrant.prio3;
#else
    NOT_USED(pStatisticsCounters);
#endif
}

void gpTest_StatisticsCountersClear(void)
{
#if           \
    defined(GP_DIVERSITY_GPHAL_XP4002)
    gpHal_StatisticsCountersClear();
#endif
}

void gpTest_ReadProductId(UInt8 *productId)
{
#if   defined(HAL_DIVERSITY_V2)
    const UInt8* temp = gpHal_InfoPageGetProductId();

    GP_ASSERT_SYSTEM(temp);

    MEMCPY(productId, temp, 10);
#else
    UInt64 productId0 = GP_WB_READ_NVR_PRODUCT_ID_0();
    MEMCPY(productId, &productId0, sizeof(UInt64));
    UInt16 productId1 = GP_WB_READ_NVR_PRODUCT_ID_1();
    MEMCPY(productId+sizeof(UInt64), &productId1, sizeof(UInt16));
#endif

}

void gpTest_SetDpiZbBuffering(UInt8 packetsBuffered)
{
    NOT_USED(packetsBuffered);
}

void gpTest_EnableDpiZb(Bool enable)
{
    NOT_USED(enable);
}


void gpTest_SetPwrCtrlInByPassMode(Bool enable)
{
    NOT_USED(enable);
}

Bool gpTest_GetPwrCtrlInByPassMode(void)
{
    return false;
}

void gpTest_SetRxLnaAttDuringTimeoutForRssiBasedAgcMode(Bool enable)
{
    gpHal_SetRxLnaAttDuringTimeoutForRssiBasedAgcMode(enable);
}

void gpTest_EnableDtm(Bool enable)
{
    NOT_USED(enable);
}

gpTest_Result_t gpTest_SetRxModeOptions(Bool enableMultiStandard, Bool enableMultiChannel, Bool enableHighSensitivity)
{
    gpTest_Result_t result = gpHal_ResultSuccess;
#ifdef GP_COMP_RADIO
    if (gpRadio_SetRxMode(enableMultiStandard, enableMultiChannel, enableHighSensitivity) != gpRadio_StatusSuccess)
    {
        result = gpHal_ResultInvalidParameter;
    }
#endif //def GP_COMP_RADIO
    return result;
}

gpTest_Result_t gpTest_GetRxModeOptions(Bool* enableMultiStandard, Bool* enableMultiChannel,
                                        Bool* enableHighSensitivity)
{
    return gpRadio_GetRxMode(enableMultiStandard, enableMultiChannel, enableHighSensitivity);
}


gpTest_Result_t gpTest_GetXo32mLoadValues(UInt32 *valueP, UInt32 *valueN)
{
    gpTest_Result_t result = gpHal_ResultSuccess;
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    static UInt32 valP = 0;
    static UInt32 valN = 0;

    GP_LOG_SYSTEM_PRINTF("gpTest_GetXo32mLoadValues()",0);

    valP = GP_WB_READ_XO32M_CLOADP_SEL();
    valN = GP_WB_READ_XO32M_CLOADN_SEL();
    if ((valueP != NULL) && (valueN != NULL))
    {
        *valueP = valP;
        *valueN = valN;
    }
    else
    {
        result = gpHal_ResultInvalidParameter;
    }
     //GP_LOG_SYSTEM_PRINTF("XO32M_CLOADP=%d XO32M_CLOADN=%d",0,valP, valN);
#else
    result = gpHal_ResultInvalidParameter;
#endif // defined(GP_DIVERSITY_GPHAL_XP4002)

    return result;
}

gpTest_Result_t gpTest_SetPdmClk(gpTest_PDMClkSrc_t src, UInt32 freqHz, UInt8 gpio)
{
#if                                                                             \
     defined(GP_DIVERSITY_GPHAL_XP4002)
    gpTest_Result_t result = gpHal_ResultSuccess;

    static UInt8 lastGpio = 0xFF;

    // Disable the last GPIO mapping:
    if ((lastGpio != 0xFF) && (lastGpio != gpio))
    {
        gpTest_PdmClkOutCfg(lastGpio, false);
    }

    // Set GPIO mapping:
    result = gpTest_PdmClkOutCfg(gpio, true);

    if (result != gpHal_ResultSuccess)
    {
        return result;
    }

    // Store the last GPIO to disable mapping before switching:
    lastGpio = gpio;

    switch(src)
    {
        case gpTest_PDMClkSrc_None:
        {
#if defined(GP_DIVERSITY_GPHAL_XP4002)
            GP_WB_WRITE_ASP_0_CLK_2M_ENABLE(0);
#else
            GP_WB_WRITE_ASP_CLK_2M_ENABLE(0);
#endif //defined(GP_DIVERSITY_GPHAL_XP4002)

#if   defined(GP_DIVERSITY_GPHAL_XP4002)
            GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0(0);
            GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_1(0);
#endif //defined(GP_DIVERSITY_GPHAL_K8C) || defined(GP_DIVERSITY_GPHAL_K8D) || defined(GP_DIVERSITY_GPHAL_K8E)

            // Disable GPIO mapping:
            gpTest_PdmClkOutCfg(gpio, false);
            break;
        }
        case gpTest_PDMClkSrc_2M:
        {
            // For this clock the required frequency should be equal to 2MHz:
            if (freqHz != 2000000)
            {
                result = gpHal_ResultInvalidParameter;
                break;
            }

#if   defined(GP_DIVERSITY_GPHAL_XP4002)
            GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0(0);
            GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_1(0);
            GP_WB_WRITE_ASP_0_SELECT_CLK(GP_WB_ENUM_ASP_CLK_SEL_CLK_2M);
            GP_WB_WRITE_ASP_0_CLK_2M_ENABLE(1);
#endif
// #if defined(GP_DIVERSITY_GPHAL_XP4002)
//             GP_WB_WRITE_ASP_0_SELECT_CLK(GP_WB_ENUM_ASP_CLK_SEL_CLK_2M);
//             GP_WB_WRITE_ASP_0_CLK_2M_ENABLE(1);
// #else
//             GP_WB_WRITE_ASP_CLK_2M_ENABLE(1);
// #endif //defined(GP_DIVERSITY_GPHAL_XP4002)
            break;
        }
#if           \
    defined(GP_DIVERSITY_GPHAL_XP4002)
        case gpTest_PDMClkSrc_PLL:
        {
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
            {
                UInt32 registerValueFreq;
                UInt64 registerValueFreq64 = (UInt64)(192000000 / 2) * ((UInt32)1 << 20);
                registerValueFreq = (UInt32)(registerValueFreq64 / freqHz);

                GP_WB_WRITE_ASP_0_CLK_2M_ENABLE(0);

                GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0(0);
                GP_WB_WRITE_FRACT_CLK_GEN_T_TARGET_0(registerValueFreq);
                GP_WB_WRITE_FRACT_CLK_GEN_ENABLE_FRACT_CLK_0(1);
                GP_WB_WRITE_ASP_0_SELECT_CLK(GP_WB_ENUM_ASP_CLK_SEL_FRACT_CLK_CLKGEN_0);

                switch(gpio)
                {
                    case 12:
                    {
                        GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(0);
                        GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(
                            GP_WB_ENUM_GPIO_12_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0);
                        GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(1);
                        break;
                    }
                    case 21:
                    {
                        GP_WB_WRITE_IOB_GPIO_21_ALTERNATE_ENABLE(0);
                        GP_WB_WRITE_IOB_GPIO_21_ALTERNATE(
                            GP_WB_ENUM_GPIO_21_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0);
                        GP_WB_WRITE_IOB_GPIO_21_ALTERNATE_ENABLE(1);
                        break;
                    }

                    case 22:
                    {
                        GP_WB_WRITE_IOB_GPIO_22_ALTERNATE_ENABLE(0);
                        GP_WB_WRITE_IOB_GPIO_22_ALTERNATE(
                            GP_WB_ENUM_GPIO_22_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0);
                        GP_WB_WRITE_IOB_GPIO_22_ALTERNATE_ENABLE(1);
                        break;
                    }

// #if defined (_GPBSP_DB05_XP4002_QFN40_10DBM_H_)
//                     case 1:
//                     {
//                         GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(0);
//                         GP_WB_WRITE_IOB_GPIO_1_ALTERNATE(
//                             GP_WB_ENUM_GPIO_1_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1);
//                         GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(1);
//                         break;
//                     }
//                     case 3:
//                     {
//                         GP_WB_WRITE_IOB_GPIO_3_ALTERNATE_ENABLE(0);
//                         GP_WB_WRITE_IOB_GPIO_3_ALTERNATE(
//                             GP_WB_ENUM_GPIO_3_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1);
//                         GP_WB_WRITE_IOB_GPIO_3_ALTERNATE_ENABLE(1);
//                         break;
//                     }
//                     case 23:
//                     {
//                         GP_WB_WRITE_IOB_GPIO_23_ALTERNATE_ENABLE(0);
//                         GP_WB_WRITE_IOB_GPIO_23_ALTERNATE(
//                             GP_WB_ENUM_GPIO_23_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1);
//                         GP_WB_WRITE_IOB_GPIO_23_ALTERNATE_ENABLE(1);
//                         break;
//                     }
// #endif //defined (_GPBSP_DB05_XP4002_QFN40_10DBM_H_)

                    default:
                    {
                        result = gpHal_ResultInvalidParameter; // only gpio 12, 21, and 22 available for fract ASP clk
                    }
                }
            }
#endif //defined(GP_DIVERSITY_GPHAL_XP4002)
            break;
        }
#endif
        default:
        {
            result = gpHal_ResultInvalidParameter;
        }
    }
    return result;
#else
    return gpHal_ResultUnsupported;
#endif
}

// K8A only:
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
// On loa there are 2 blocks for ASP clock
// They are routable to other gpio's, so selection of the gpio inherently defines which ASP_x will be used
static gpTest_Result_t gpTest_PdmClkOutCfg(UInt8 gpio, Bool state)
{
    switch(gpio)
    {
#if defined (_GPBSP_DB05_XP4002_QFN40_10DBM_H_)
        case 1:
        {
            GP_WB_WRITE_IOB_GPIO_1_ALTERNATE(GP_WB_ENUM_GPIO_1_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_1_ALTERNATE_ENABLE(state);
            break;
        }
        case 3:
        {
            GP_WB_WRITE_IOB_GPIO_3_ALTERNATE(GP_WB_ENUM_GPIO_3_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_3_ALTERNATE_ENABLE(state);
            break;
        }
        case 23:
        {
            GP_WB_WRITE_IOB_GPIO_23_ALTERNATE(GP_WB_ENUM_GPIO_23_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_23_ALTERNATE_ENABLE(state);
            break;
        }
#endif //defined (_GPBSP_DB05_XP4002_QFN40_10DBM_H_)
        case 12:
        {
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(GP_WB_ENUM_GPIO_12_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(state);
            break;
        }
        case 21:
        {
            GP_WB_WRITE_IOB_GPIO_21_ALTERNATE(GP_WB_ENUM_GPIO_21_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_21_ALTERNATE_ENABLE(state);
            break;
        }
        case 22:
        {
            GP_WB_WRITE_IOB_GPIO_22_ALTERNATE(GP_WB_ENUM_GPIO_22_ALTERNATES_ASP_0_CLK);
            GP_WB_WRITE_IOB_GPIO_22_ALTERNATE_ENABLE(state);
            break;
        }
        default:
        {
            GP_LOG_SYSTEM_PRINTF("GPIO accessed %d ",0,gpio);
            GP_ASSERT_SYSTEM(false);
        }
    }
    return gpHal_ResultSuccess;
}
#endif // !defined(GP_DIVERSITY_QPG7015M_CS3) && (defined(GP_DIVERSITY_GPHAL_K8C) || defined(GP_DIVERSITY_GPHAL_K8D) || defined(GP_DIVERSITY_GPHAL_K8E))


#if defined(GP_DIVERSITY_GPHAL_XP4002)
/**
 *
 *  Retrieve the active state of the alternate configuration for a certain gpio.
 *
 **/
gpTest_Result_t gpTest_get_AlternateEnabled(UInt8 gpio, Bool* alternateEnabled)
{
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        case 10:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_10_ALTERNATE_ENABLE() ? true : false);
            break;

        case 11:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_11_ALTERNATE_ENABLE() ? true : false);
            break;

        case 12:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_12_ALTERNATE_ENABLE() ? true : false);
            break;

        case 13:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_13_ALTERNATE_ENABLE() ? true : false);
            break;

        case 14:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_14_ALTERNATE_ENABLE() ? true : false);
            break;

        case 15:
            *alternateEnabled = (GP_WB_READ_IOB_GPIO_15_ALTERNATE_ENABLE() ? true : false);
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }
    return result;
}

/**
 *
 * Enable/disable the Alternate Function for a certain gpio.
 *
 **/
gpTest_Result_t gpTest_set_AlternateEnabled(UInt8 gpio, Bool alternateEnabled)
{
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        case 10:
            GP_WB_WRITE_IOB_GPIO_10_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        case 11:
            GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        case 12:
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        case 13:
            GP_WB_WRITE_IOB_GPIO_13_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        case 14:
            GP_WB_WRITE_IOB_GPIO_14_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        case 15:
            GP_WB_WRITE_IOB_GPIO_15_ALTERNATE_ENABLE(alternateEnabled ? 1 : 0);
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }

    return result;
}

/**
 *
 *  Get the current alternate selection for a certain gpio.
 *
 *
 **/
gpTest_Result_t gpTest_get_AlternateSelection(UInt8 gpio, UInt8* alternateSelection)
{
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        // we could serve this request from gpio 0 up to 29... for now we provide only 10 -> 15 which are used for the output of the sleep clock
        case 10:
            *alternateSelection = GP_WB_READ_IOB_GPIO_10_ALTERNATE();
            break;

        case 11:
            *alternateSelection = GP_WB_READ_IOB_GPIO_11_ALTERNATE();
            break;

        case 12:
            *alternateSelection = GP_WB_READ_IOB_GPIO_12_ALTERNATE();
            break;

        case 13:
            *alternateSelection = GP_WB_READ_IOB_GPIO_13_ALTERNATE();
            break;

        case 14:
            *alternateSelection = GP_WB_READ_IOB_GPIO_14_ALTERNATE();
            break;

        case 15:
            *alternateSelection = GP_WB_READ_IOB_GPIO_15_ALTERNATE();
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }
    return result;
}

/**
 *
 *
 *  Set the alternate selection for a certain gpio
 *
 *
 **/
gpTest_Result_t gpTest_set_AlternateSelection(UInt8 gpio, UInt8 alternateSelection)
{
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        // we could serve this request from gpio 0 up to 29... for now we provide only 10 -> 15 which are used for the output of the sleep clock
        case 10:
            GP_WB_WRITE_IOB_GPIO_10_ALTERNATE(alternateSelection);
            break;

        case 11:
            GP_WB_WRITE_IOB_GPIO_11_ALTERNATE(alternateSelection);
            break;

        case 12:
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(alternateSelection);
            break;

        case 13:
            GP_WB_WRITE_IOB_GPIO_13_ALTERNATE(alternateSelection);
            break;

        case 14:
            GP_WB_WRITE_IOB_GPIO_14_ALTERNATE(alternateSelection);
            break;

        case 15:
            GP_WB_WRITE_IOB_GPIO_15_ALTERNATE(alternateSelection);
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }
    return result;
}

/**
 *
 *
 *
 *  Get the ID that defines the current debug function for a certain gpio
 *
 *
 *
 **/

gpTest_Result_t gpTest_get_DebugSelection(UInt8 gpio, UInt8* debugSelection)
{
    // - Get the GP_WB_WRITE_IOB_DBGx_SEL of the corresponding GPIO
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        case 10:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_0();
            break;

        case 11:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_1();
            break;

        case 12:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_2();
            break;

        case 13:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_3();
            break;

        case 14:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_4();
            break;

        case 15:
            *debugSelection = GP_WB_READ_DEBUG_DEBUG_SEL_5();
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }
    return result;
}

/**
 *
 *
 * Configure which debug function is selected for a certain gpio.
 *
 *
 **/

gpTest_Result_t gpTest_set_DebugSelection(UInt8 gpio, UInt8 debugSelection)
{
    //- Set the GP_WB_WRITE_IOB_DBGx_SEL of the corresponding GPIO
    gpTest_Result_t result = gpHal_ResultSuccess;

    switch(gpio)
    {
        case 10:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_0(debugSelection);
            break;

        case 11:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_1(debugSelection);
            break;

        case 12:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_2(debugSelection);
            break;

        case 13:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_3(debugSelection);
            break;

        case 14:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_4(debugSelection);
            break;

        case 15:
            GP_WB_WRITE_DEBUG_DEBUG_SEL_5(debugSelection);
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }
    return result;
}

/**
 *    Helper function
 *         clockType must have been checked for valid value prio to this.
 *
 **/
static void gpTest_SetClockTypeViaMessagebox(UInt8 clockType)
{
    power_mode_t pm;

    esec_get_power_mode(&pm);
    pm = Q_POWER_MODE_SET_SLEEP_MODE(pm, clockType);
    esec_set_power_mode(pm);
}

gpTest_Result_t gpTest_MapSleepClock(UInt8 gpio, UInt8 clockType)
{
    // - If clocktype is valid, enable DEBUG function on selected GPIO then enable output
    // - If clocktype is invalid (0xFF), disable DEBUG function then disable output
    gpTest_Result_t result = gpHal_ResultSuccess;
    Bool enableOutput = (clockType != 0xFF);

    /* rtc_timer_src  */
    if((clockType != GP_WB_ENUM_RTC_TIMER_SRC_LFRC) && (clockType != GP_WB_ENUM_RTC_TIMER_SRC_XT32K) && (clockType != GP_WB_ENUM_RTC_TIMER_SRC_XT32M) && (clockType != 0xFF))
    {
        GP_LOG_SYSTEM_PRINTF("Invalid clockType %d", 0, clockType);
        result = gpHal_ResultInvalidParameter;
    }

    switch(gpio)
    {
        case 10:
            GP_WB_WRITE_IOB_GPIO_10_ALTERNATE_ENABLE(0);
            GP_WB_WRITE_DEBUG_DEBUG_SEL_0(GP_WB_ENUM_DEBUG_SEL_ES_RTC_CLK);
            gpTest_SetClockTypeViaMessagebox(clockType);
            GP_WB_WRITE_IOB_GPIO_10_ALTERNATE(GP_WB_ENUM_GPIO_10_ALTERNATES_DEBUG_DEBUG_0);
            GP_WB_WRITE_IOB_GPIO_10_ALTERNATE_ENABLE(enableOutput ? 1 : 0);
            break;

        case 11:
            GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(0);
            GP_WB_WRITE_DEBUG_DEBUG_SEL_1(GP_WB_ENUM_DEBUG_SEL_ES_RTC_CLK);
            gpTest_SetClockTypeViaMessagebox(clockType);
            GP_WB_WRITE_IOB_GPIO_11_ALTERNATE(GP_WB_ENUM_GPIO_11_ALTERNATES_DEBUG_DEBUG_1);
            GP_WB_WRITE_IOB_GPIO_11_ALTERNATE_ENABLE(enableOutput ? 1 : 0);
            break;

        case 12:
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(0);
            GP_WB_WRITE_DEBUG_DEBUG_SEL_2(GP_WB_ENUM_DEBUG_SEL_ES_RTC_CLK);
            gpTest_SetClockTypeViaMessagebox(clockType);
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE(GP_WB_ENUM_GPIO_12_ALTERNATES_DEBUG_DEBUG_2);
            GP_WB_WRITE_IOB_GPIO_12_ALTERNATE_ENABLE(enableOutput ? 1 : 0);
            break;

        default:
            result = gpHal_ResultInvalidParameter;
    }

    return result;
}
#else

/**
 *
 * Enable/disable the Alternate Function for a certain gpio.
 *
 **/
gpTest_Result_t gpTest_set_AlternateEnabled(UInt8 gpio, Bool alternateEnabled)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(alternateEnabled);

#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_set_AlternateEnabled() not implemented.", 0);
#endif
    return result;
}

/**
 *
 *  Get the current alternate selection for a certain gpio.
 *
 *
 **/
gpTest_Result_t gpTest_get_AlternateSelection(UInt8 gpio, UInt8* alternateSelection)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(alternateSelection);
#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_get_AlternateSelection() not implemented.", 0);
#endif
    return result;
}

/**
 *
 *  Set the alternate selection for a certain gpio
 *
 **/
gpTest_Result_t gpTest_set_AlternateSelection(UInt8 gpio, UInt8 alternateSelection)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(alternateSelection);
#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_set_AlternateSelection() not implemented.", 0);
#endif
    return result;
}

/**
 *
 *
 *  Get the ID that defines the current debug function for a certain gpio
 *
 *
 **/

gpTest_Result_t gpTest_get_DebugSelection(UInt8 gpio, UInt8* debugSelection)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(debugSelection);

#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_get_DebugSelection() not implemented.", 0);
#endif
    return result;
}

/**
 *
 *
 * Configure which debug function is selected for a certain gpio.
 *
 *
 **/

gpTest_Result_t gpTest_set_DebugSelection(UInt8 gpio, UInt8 debugSelection)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(debugSelection);

#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_set_DebugSelection() not implemented.", 0);
#endif
    return result;
}

gpTest_Result_t gpTest_MapSleepClock(UInt8 gpio, UInt8 clockType)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(clockType);

#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_MapSleepClock() not implemented.", 0);
#endif
    return result;
}

gpTest_Result_t gpTest_get_AlternateEnabled(UInt8 gpio, Bool* alternateEnabled)
{
    gpTest_Result_t result = gpHal_ResultInvalidParameter;

    NOT_USED(gpio);
    NOT_USED(alternateEnabled);
#ifdef GP_DIVERSITY_LOG
    GP_LOG_SYSTEM_PRINTF("Function gpTest_get_AlternateEnabled not implemented.", 0);
#endif
    return result;
}

#endif // defined(GP_DIVERSITY_GPHAL_XP4002)

void gpTest_EnableAgc(Bool enable)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    gpHal_EnableAgc(enable);
#else
    GP_ASSERT_SYSTEM(false);
    NOT_USED(enable);
#endif
}

Bool gpTest_isAgcEnabled(void)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    return gpHal_isAgcEnabled();
#else
    GP_ASSERT_SYSTEM(false);
    return false;
#endif
}

Bool gpTest_IsLowTemp(void)
{
#ifdef GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP
    return gpHal_IsLowTemp();
#endif // defined "GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP"
    return false;
}

gpTest_Result_t gpTest_Xtal32MHzTrimGetZoneMaxTemp(UInt8 zoneId, Int8* pOutMaxTemp)
{
    if(!pOutMaxTemp)
    {
        return gpHal_ResultInvalidParameter;
    }

    *pOutMaxTemp = 0;
    return gpHal_ResultUnsupported;
}

gpTest_Result_t gpTest_Xtal32MHzTrimGetZoneSetting(UInt8 zoneId, UInt8* pOutSetting)
{
    if(!pOutSetting)
    {
        return gpHal_ResultInvalidParameter;
    }

    *pOutSetting = 0;
    return gpHal_ResultUnsupported;
}

gpTest_Result_t gpTest_Xtal32MHzTrimConfigOverwrite(Int8* pInMaxTempTab, UInt8* pInSettigTab, UInt8 zoneCnt)
{
    return gpHal_ResultUnsupported;
}

Int16 gpTest_GetChipTemperature(void)
{
#if   defined(GP_DIVERSITY_GPHAL_XP4002)
    return (qDrvTsens_ResultGetBlocking());
#else
    return halADC_MeasureTemperature();
#endif // defined(GP_DIVERSITY_GPHAL_K8A) || defined(GP_DIVERSITY_GPHAL_K7C)
}

Int16 gpTest_GetCapacitorLoad(UInt8 tempIndex)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    extern gpAppUcSubsystem_CalRamXo32m_t calRamXo32mData_EVB;
    if(tempIndex < sizeof(calRamXo32mData_EVB))
    {
        return ((Int8*)(&calRamXo32mData_EVB))[tempIndex];
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

UInt8 gpTest_GetSizeOfCapacitorLoad(void)
{
#if defined(GP_DIVERSITY_GPHAL_XP4002)
    extern gpAppUcSubsystem_CalRamXo32m_t calRamXo32mData_EVB;
    return sizeof(calRamXo32mData_EVB);
#else
    return 0;
#endif
}

#if defined(GP_DIVERSITY_GPHAL_XP4002)
gpTest_Result_t gpTest_SetDcdcMode(Bool dcdcOn)
{
    hal_SleepMode_t sleepClock = (hal_SleepMode_t)gpHal_GetSleepMode();
    hal_PowerModeResult_t result;
    if(dcdcOn)
    {
        result = hal_SetPowerMode(hal_StandbyPowerModeELPS, hal_ActivePowerModeEHPS, sleepClock);
    }
    else
    {
        result = hal_SetPowerMode(hal_StandbyPowerModeLPS, hal_ActivePowerModeHPS, sleepClock);
    }

    if(result == hal_PowerModeResultError)
    {
        return gpHal_ResultInvalidRequest;
    }
    if(result == hal_PowerModeResultInvalidParameter)
    {
        return gpHal_ResultInvalidParameter;
    }

    return gpHal_ResultSuccess;
}

gpTest_Result_t gpTest_GetDcdcMode(hal_StandbyPowerMode_t* pStandbyPowerMode, hal_ActivePowerMode_t* pActivePowerMode,
                                   hal_SleepMode_t* pSleepMode)
{
    hal_PowerModeResult_t result;

    result = hal_GetPowerMode(pStandbyPowerMode, pActivePowerMode, pSleepMode);

    if(result == hal_PowerModeResultError)
    {
        return gpHal_ResultInvalidRequest;
    }
    if(result == hal_PowerModeResultInvalidParameter)
    {
        return gpHal_ResultInvalidParameter;
    }

    return gpHal_ResultSuccess;
}
#endif

void gpTest_SetTxModulation(gpTest_BleTxPhy_t modulation)
{
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        gpTest_BleSetTxModulation(modulation);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
}

gpTest_Result_t gpTest_SetRxPhyMask(UInt8 rxPhyMask)
{
#ifdef GP_COMP_GPTEST_BLE
    if(gpTest_PhyMode == gpTest_PhyModeBle)
    {
        return gpTest_BleSetRxPhyMask(rxPhyMask);
    }
    else
#endif
    {
        GP_ASSERT_DEV_INT(false);
    }
    return gpHal_ResultInvalidRequest;
}
