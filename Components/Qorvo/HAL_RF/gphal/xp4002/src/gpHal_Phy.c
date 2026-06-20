/*
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_Phy.c
 *   This file contains the implementation of the common phy functions
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
#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_Phy.h"
#include "gpSched.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"
#include "gpHal_reg.h"

//Debug
#include "hal.h"
#include "gpBsp.h"
#include "gpAssert.h"
#include "gpHal_kx_Rap.h"
#include "gpHal_kx_Phy.h"

#ifdef GP_COMP_TXMONITOR
#include "gpTxMonitor.h"
#endif //GP_COMP_TXMONITOR

#include "gpHal_RadioMgmt.h"
#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal_kx_BLE.h" /* for the ble<->phy channel conversion macros */

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/* include both power tables */
/* and select the correct one at runtime */
#include "gpHal_kx_Rap_Tx_PaLut10.c"
#include "gpHal_kx_Rap_Tx_PaLut20.c"

static Int8 gpHal_PaLookupOffset;
static const UInt8* PaLookupTable;

// Last used Continuous Wave settings
static gpHalPhy_CurrentCWModeSettings_t HalPhy_CurrentCWModeSettings;

static gpHal_AntennaMode_t              HalPhy_AntennaMode;

// Local states for preparing radio for load
static UInt8 gpHalPhy_PlmeRxModeZbSaved;
static UInt8 gpHalPhy_PlmeDutyCyclingSaved;
static UInt8 gpHalPhy_RxAntIntSaved;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void HalPhy_CalibrateFllChannel_Unprotected(gpHal_ChannelSlotIdentifier_t slotId, UInt8 fllChannel)
{
    HAL_DISABLE_GLOBAL_INT();

    ak_rap_sx_set_channel(slotId, fllChannel);

    HAL_ENABLE_GLOBAL_INT();
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


/* Because gpHal_GetDefaultTransmitPower only supports channels [11,26] */
/* CW ON:
    1. set_channel
    2.force_closed_loop(tx = True)
    3.force_antenna(mode='txt', port=0/1)
    4.force_pa_on()
*/
void gpHal_SetContinuousWaveMode( gpHal_ContinuousWaveMode_t mode, UInt8 channel, Int8 txpower, gpHal_AntennaSelection_t antenna)
{
    if (mode == gpHal_CW_Off)
    {
        HAL_DISABLE_GLOBAL_INT();
        HalPhy_CurrentCWModeSettings.mode = gpHal_CW_Off;
        ak_rap_tx_cw_disable();
        HAL_ENABLE_GLOBAL_INT();

    }
    else
    {   // Code for switching CW On


        if (antenna == gpHal_AntennaSelection_Auto)
        {
            antenna = GP_HAL_TX_ANTENNA;
        }

        gpHal_PbmTxOptions_t gpHalPhy_TxOptions;
        gpHalPhy_TxOptions.antenna = antenna;

        if (mode == gpHal_CW_Modulated || mode == gpHal_CW_Unmodulated)
        {
            /* use fll channel */
            gpHalPhy_GetFemTxOptions(txpower, GP_HAL_CONVERT_MACPHY_TO_FLL_CHANNEL(channel), &gpHalPhy_TxOptions, GPHAL_TX_OPTIONS_FOR_TRANSMIT);
        }
        else
        {
            /* use fll channel */
            gpHalPhy_GetFemTxOptions(txpower, GP_HAL_CONVERT_BLEPHY_TO_FLL_CHANNEL(channel), &gpHalPhy_TxOptions, GPHAL_TX_OPTIONS_FOR_TRANSMIT);
        }


        /*Get all pa settings */

        //Calibrate channel
        if (mode == gpHal_CW_Modulated || mode == gpHal_CW_Unmodulated)
        {
            HalPhy_CalibrateFllChannel_Unprotected(GPHAL_ZB_CHANNEL_IDX, channel-11);
        }
        else
        {
            //use static _Unprotected flavor of the file as the protection is handled in gpHal_BleTestDisable/ReEnebleBleMgr() in gpTest
            HalPhy_CalibrateFllChannel_Unprotected(GP_WB_READ_BLE_MGR_BLE_CHANNEL_IDX(), channel+16);
        }


        {
            /* rap CW code assumes that the channel is set, calibrated and no other activity is ongoing */

            /* translate to variables needed by rap functions */
            UInt8 chidx;

            if (mode == gpHal_CW_Modulated || mode == gpHal_CW_Unmodulated)
            {
                chidx = GPHAL_ZB_CHANNEL_IDX;
            }
            else
            {
                chidx = GP_WB_READ_BLE_MGR_BLE_CHANNEL_IDX();
            }

            UInt8 power = GP_HAL_PHY_TX_POWER(gpHalPhy_TxOptions.tx_power_setting);
            UInt8 pa_mode = 0;
            pa_mode = GP_HAL_PHY_TX_PA_MODE(gpHalPhy_TxOptions.tx_power_setting);

            Bool tx          = true;
            UInt8 modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_UNMODULATED;
            UInt8 fll_channel;
            if (mode == gpHal_CW_Unmodulated || mode == gpHal_CW_Ble_UnModulated)
            {
                modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_UNMODULATED;
            }
            else if  (mode == gpHal_CW_Modulated)
            {
                modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_ZB;
            }
            else if (mode == gpHal_CW_Ble_Modulated)
            {
                modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_BLE;
            }
            else if (mode == gpHal_CW_Ble_HDRModulated)
            {
                modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_BLE_HDR;
            }
            else
            {
                GP_ASSERT_DEV_INT(false);
                /* satisfy the compiler */
                modulation_type = GP_WB_ENUM_AK_TRANSMITTER_MODE_UNMODULATED;
            }

            if(mode == gpHal_CW_Unmodulated || mode == gpHal_CW_Modulated)
            {
                fll_channel = GP_HAL_CONVERT_MACPHY_TO_FLL_CHANNEL(channel);
            }
            else if(
                mode == gpHal_CW_Ble_UnModulated || mode == gpHal_CW_Ble_Modulated || mode == gpHal_CW_Ble_HDRModulated)
            {
                fll_channel = GP_HAL_CONVERT_BLE_TO_FLL_CHANNEL(channel);
            }
            else
            {
                GP_ASSERT_DEV_INT(false);
                /* satisfy the compiler */
                fll_channel = 0;
            }

            // Set the PA first, then enable CW
            ak_rap_tx_config_psf(fll_channel);
            ak_rap_tx_config_pa(pa_mode, power);
            ak_rap_tx_cw_enable(chidx, gpHalPhy_TxOptions.antenna, modulation_type, tx);

            // Save arguments, so that other modules can know the state of the CW
            HalPhy_CurrentCWModeSettings.mode = mode;
            HalPhy_CurrentCWModeSettings.channel = channel;
            HalPhy_CurrentCWModeSettings.txpower = txpower;
            HalPhy_CurrentCWModeSettings.antenna = antenna;

            /* Wait around 100 us before signalling that the FLL should be stable */
            HAL_WAIT_US(100);
            // Clear any interrupt that may have been raised during starting of the CW
            GP_WB_AK_SX_CLR_RX_OUT_OF_RANGE_INTERRUPT();
            GP_WB_AK_SX_CLR_TX_OUT_OF_LOCK_INTERRUPT();
            GP_WB_AK_SX_CLR_TX_OUT_OF_RANGE_INTERRUPT();

        }
    }
}

/* Return the last state of the CW mode */
gpHalPhy_CurrentCWModeSettings_t *gpHal_GetCurrentContinuousWaveModeSettings(void)
{
    return &HalPhy_CurrentCWModeSettings;
}

Int8 gpHal_GetMinTxPower(void)
{
    return gpHal_PaLookupOffset;
}

Int8 gpHal_GetMaxTxPower(void)
{
    if(ak_rap_sys_is_tx20())
    {
        return gpHal_PaLookupOffset + (Int8)sizeof(ak_rap_tx_pa_lut_20dbm) - 1;
    }
    else
    {
        return gpHal_PaLookupOffset + (Int8)sizeof(ak_rap_tx_pa_lut_10dbm) - 1;
    }
}

void gpHalPhy_GetFemTxOptions(gpHal_TxPower_t txPower, gpHal_FllChannel_t fllChannel, gpHal_PbmTxOptions_t* pTxOptions, Bool forAck)
{
    if (gpHal_DefaultTransmitPower == txPower)
    {
        if (fllChannel < GP_HAL_BLE_FLL_START_INDEX)
        {
            /* ZB case */
        #if defined(GP_COMP_GPHAL_MAC)
            txPower = gpHal_GetDefaultTransmitPower(GP_HAL_CONVERT_FLL_TO_MACPHY_CHANNEL(fllChannel));
        #else
            /* you asked for a ZB channel, but did not enable the GP_COMP_GPHAL_MAC */
            GP_ASSERT_DEV_INT(false);
        #endif
        }
        else
        {
            /* BLE case */
            // for now, BLE default transmit power is set to maximum
            txPower = gpHal_GetMaxTxPower();
        }
    }
    else if(gpHal_MinTransmitPower == txPower)
    {
        txPower = gpHal_GetMinTxPower();
    }
    else if(gpHal_MaxTransmitPower == txPower)
    {
        txPower = gpHal_GetMaxTxPower();
    }

    // make sure the requested power is not lower then the lowest value in the PA lookup table
    GP_ASSERT_DEV_EXT(txPower >= gpHal_GetMinTxPower());

    txPower = clamp(txPower, gpHal_GetMinTxPower(), gpHal_GetMaxTxPower());


    //don't overrule pTxOptions->antenna
    pTxOptions->ext_pa_required = false;

#if defined(GP_HAL_DIVERSITY_EXT_MODE_SIGNALS)
    pTxOptions->phy_mode = gpHalPhy_GetTxMode(txPower);
#else
    pTxOptions->phy_mode = 0;
#endif

    pTxOptions->tx_power_setting = gpHalPhy_GetTxPowerSetting(txPower, forAck);
}

UInt8 gpHalPhy_GetTxPowerSetting(gpHal_TxPower_t requested_txPower_dBm, Bool forAck)
{
    UInt8 setting;
    gpHal_TxPower_t    internalPa_requested_txPower_dBm;
    NOT_USED(forAck);
    internalPa_requested_txPower_dBm = requested_txPower_dBm;


    // make sure the requested power is not lower then the lowest value in the PA lookup table
    GP_ASSERT_DEV_EXT(internalPa_requested_txPower_dBm >= gpHal_GetMinTxPower());

    internalPa_requested_txPower_dBm =
        clamp(internalPa_requested_txPower_dBm, gpHal_GetMinTxPower(), gpHal_GetMaxTxPower());

    setting = PaLookupTable[internalPa_requested_txPower_dBm - gpHal_PaLookupOffset];
    return setting;
}

void gpHal_CalibrateFllChannel(gpHal_ChannelSlotIdentifier_t slotId, UInt8 fllChannel)
{
    // claim the radio for management
    gpHalRadioMgmtSynch_claimRadio();

    HalPhy_CalibrateFllChannel_Unprotected(slotId,fllChannel);

    // release the interface
    gpHalRadioMgmtSynch_releaseRadio();
}

void gpHal_CalibrateFllChannelUnprotected(gpHal_ChannelSlotIdentifier_t slotId, UInt8 fllChannel)
{
    HalPhy_CalibrateFllChannel_Unprotected(slotId,fllChannel);
}

void gpHal_PrepareRadioForLoad(void)
{
     GP_ASSERT_SYSTEM(0);
    // TODO_ handle radio grant
    // GP_ASSERT_DEV_EXT(GP_WB_READ_AK_RADIO_ARB_MGMT_GRANTED() == 1);

    //save currently register configuration
    gpHalPhy_PlmeRxModeZbSaved      = GP_WB_READ_AK_PLME_RX_MODE_ZB();
    gpHalPhy_RxAntIntSaved          = GP_WB_READ_AK_RX_RX_ANTSEL_INT();
    gpHalPhy_PlmeDutyCyclingSaved   = GP_WB_READ_AK_PLME_DUTY_CYCLING_START_EN();

    //Set RX as antena diversity, so the current load will always be the same, indepent of the current configuration
    GP_WB_WRITE_AK_PLME_RX_MODE_ZB(GP_WB_ENUM_AK_ZB_RECEIVER_MODE_LPL_AD); //Low power listening + antena diversity
    GP_WB_WRITE_AK_RX_RX_ANTSEL_INT(GP_WB_ENUM_ANTSEL_INT_USE_PORT_FROM_DESIGN); //Rx antenna select internal: 0=ant_sel
    //TODO: not found
    // GP_WB_WRITE_AK_PLME_DUTY_CYCLING_START_EN(GP_WB_ENUM_PHY_STATE_TRANSITION_GENERIC_TRANSITION_1); //generic_transition_1
}

void gpHal_RestoreRadioFromLoad(void)
{
     GP_ASSERT_SYSTEM(0);
    // TODO_ handle radio grant
    // GP_ASSERT_DEV_EXT(GP_WB_READ_AK_RADIO_ARB_MGMT_GRANTED() == 1);
    //restore saved registers
    GP_WB_WRITE_AK_PLME_RX_MODE_ZB(gpHalPhy_PlmeRxModeZbSaved);
    GP_WB_WRITE_AK_RX_RX_ANTSEL_INT(gpHalPhy_RxAntIntSaved);
    // GP_WB_WRITE_AK_PLME_DUTY_CYCLING_START_TRANSITION(gpHalPhy_PlmeDutyCyclingSaved);
}

//-------------------------------------------------------------------------------------------------------
// External PA
//-------------------------------------------------------------------------------------------------------

void gpHal_InitPhy(void)
{
    if(ak_rap_sys_is_tx20())
    {
        gpHal_PaLookupOffset = RAP_TX_PA_LUT_OFFSET_20DBM;
        PaLookupTable = &(ak_rap_tx_pa_lut_20dbm[0]);
    }
    else
    {
        gpHal_PaLookupOffset = RAP_TX_PA_LUT_OFFSET_10DBM;
        PaLookupTable = &(ak_rap_tx_pa_lut_10dbm[0]);
    }
#ifdef GP_HAL_DIVERSITY_EXT_MODE_SIGNALS
    /* Define what to do with mode signals when tx and rx off */
    GP_WB_WRITE_AK_TRX_EXT_MODE_CTRL_FOR_TRX_OFF(GP_BSP_PHY_MODE(GP_BSP_PHY_MODE_TRX_OFF_SIGNALS));
    GP_WB_WRITE_AK_TRX_EXT_MODE_CTRL_FOR_RX_ON_ATT_CTRL_LOW(GP_BSP_PHY_MODE(GP_BSP_PHY_MODE_RX_DEFAULT_SIGNALS));
    GP_WB_WRITE_AK_TRX_EXT_MODE_CTRL_FOR_RX_ON_ATT_CTRL_HIGH(GP_BSP_PHY_MODE(GP_BSP_PHY_MODE_RX_DEFAULT_SIGNALS));

#endif

    GP_WB_WRITE_AK_RX_RX_ANTSEL_INT(GP_BSP_RX_ANTSEL_INT_DEFAULT);
    GP_WB_WRITE_AK_TX_TX_ANTSEL_INT(GP_BSP_TX_ANTSEL_INT_DEFAULT);

    // Set Last used CW Mode settings to defaults
    HalPhy_CurrentCWModeSettings.mode = gpHal_CW_Off;
    HalPhy_CurrentCWModeSettings.channel = 0;
    HalPhy_CurrentCWModeSettings.txpower = gpHal_DefaultTransmitPower;
    HalPhy_CurrentCWModeSettings.antenna = gpHal_AntennaSelection_Unknown;
}

#if defined(GP_HAL_DIVERSITY_EXT_MODE_SIGNALS)
UInt8 gpHalPhy_GetTxMode(gpHal_TxPower_t txPower)
{
    return ( gpHalPhy_IsPaRequired(txPower) ? GP_BSP_PHY_MODE(GP_BSP_PHY_MODE_TX_PA_ENABLED_SIGNALS) : GP_BSP_PHY_MODE(GP_BSP_PHY_MODE_TX_DEFAULT_SIGNALS) );
}
#endif //GP_HAL_DIVERSITY_EXT_MODE_SIGNALS

void gpHal_SetRxLnaAttDuringTimeoutForRssiBasedAgcMode(Bool enable)
{

    // Not found in xp400x register map
    // GP_WB_WRITE_AK_RX_LNA_AGC_ENABLE_ATT_DURING_TIMEOUT(enable);
    GP_ASSERT_SYSTEM(false);
}

UInt8 gpHal_GetPaPowerTableIndex(UInt8 externalOutputPower, UInt8 externalPAGain)
{
    return externalOutputPower - gpHal_PaLookupOffset - externalPAGain;
}

gpHal_AntennaSelection_t gpHal_GetTxAntenna(void)
{
    /* make it clear this is obsolete on the current platform */
    GP_ASSERT_SYSTEM(false);
    return gpHal_AntennaSelection_Auto; /* to satisfy the compiler */
}

gpHal_AntennaSelection_t gpHal_GetRxAntenna(void)
{
    gpHal_AntennaSelection_t antennaSelection=gpHal_AntennaSelection_Unknown;

    if (HalPhy_AntennaMode == gpHal_AntennaModeDiversity)
    {
        antennaSelection = gpHal_AntennaSelection_Auto;
    }
    else if (HalPhy_AntennaMode == gpHal_AntennaModeAntenna0)
    {
        antennaSelection = gpHal_AntennaSelection_Ant0;
    }
    else if (HalPhy_AntennaMode == gpHal_AntennaModeAntenna1)
    {
        antennaSelection = gpHal_AntennaSelection_Ant1;
    }

    return antennaSelection;
}

gpHal_AntennaSelection_t gpHal_GetBleAntenna(void)
{
    /* same antenna as ZB, but can't have antenna diversity */
    return GP_HAL_ANTSEL_INT_TO_ANT(HalPhy_AntennaMode);
}

#ifdef GP_COMP_GPHAL_MAC
extern gpHal_RxModeConfig_t    gpHal_ConfiguredReceiverMode;
#endif //GP_COMP_GPHAL_MAC

void gpHal_SetRxAntenna(gpHal_AntennaSelection_t rxAntenna)
{
#ifdef GP_HAL_DIVERSITY_SINGLE_ANTENNA
    rxAntenna = GP_HAL_DIVERSITY_SINGLE_ANTENNA;
#endif
    if (rxAntenna == gpHal_AntennaSelection_Auto)
    {
        HalPhy_AntennaMode = gpHal_AntennaModeDiversity;
    }
    else if (rxAntenna == gpHal_AntennaSelection_Ant0)
    {
        HalPhy_AntennaMode = gpHal_AntennaModeAntenna0;
    }
    else if (rxAntenna == gpHal_AntennaSelection_Ant1)
    {
        HalPhy_AntennaMode = gpHal_AntennaModeAntenna1;
    }
#ifdef GP_COMP_GPHAL_MAC
    GP_WB_WRITE_AK_RX_ANT_USE_ZB( GP_HAL_ANTSEL_INT_TO_ANT(HalPhy_AntennaMode) );
    if(gpHal_ConfiguredReceiverMode.rxMode != 0)
    {
        /* skip this if the gpHal_Mac is not initialised yet */
        gpHal_ConfiguredReceiverMode.antennaMode = HalPhy_AntennaMode;
        gpHal_SetRxModeConfig(&gpHal_ConfiguredReceiverMode);
    }
#endif //GP_COMP_GPHAL_MAC
#ifdef GP_COMP_GPHAL_BLE
    if(rxAntenna == gpHal_AntennaSelection_Auto)
    {
        gpHal_BleSetAntennaDiversity(true);
        gpHal_BleSetHwAntennaDiversity(true);
    }
    else
    {
        gpHal_BleSetAntennaDiversity(false);
        gpHal_BleSetHwAntennaDiversity(false);
    }
#endif //GP_COMP_GPHAL_BLE
}

void gpHal_SetRadioFirFilter(gpHal_FirFilter_t firFilter)
{
    // Not available
    GP_ASSERT_DEV_INT(false);
}

/*****************************************************************************
 *            Obsolete or for backward compatibility
 *****************************************************************************/

void gpHal_SetAntenna(gpHal_AntennaSelection_t rxAntenna)
{
    /* keep this for backward compatibility */
    gpHal_SetRxAntenna(rxAntenna);
}

Bool gpHal_GetRxAntennaDiversity(void)
{
    /* keep this for backward compatibility */
    return (gpHal_GetRxAntenna() == gpHal_AntennaSelection_Auto);
}
