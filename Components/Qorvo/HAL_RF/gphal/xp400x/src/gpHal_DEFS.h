/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_DEFS.h
 *
 *  This file contains internal definitions of the GPHAL.
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

#ifndef _HAL_GP_DEFS_H_
#define _HAL_GP_DEFS_H_


/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gp_global.h"
#include "gpHal.h"
#include "gpHal_arm.h"


#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#include "gpHal_kx_Ipc.h"
#endif //defined(GP_HAL_DIVERSITY_INCLUDE_IPC)

#if defined(GP_COMP_GPHAL_PBM)
#include "gpHal_kx_Pbm.h"
#endif //defined(GP_COMP_GPHAL_PBM)
#ifdef GP_COMP_GPHAL_MAC
#include "gpHal_kx_MAC.h"
#endif //GP_COMP_GPHAL_MAC
#ifdef GP_COMP_GPHAL_BLE
#include "gpHal_kx_BLE.h"
#endif //GP_COMP_GPHAL_BLE

#ifdef GP_COMP_GPHAL_RADIO
#include "gpHal_kx_Fll.h"
#include "gpHal_RtEvents.h"
#endif //GP_COMP_GPHAL_RADIO

#include "gpHal_kx_ES.h"
#include "gpHal_kx_DP.h"
#include "gpHal_kx_Phy.h"

#include "gpLog.h"
#include "gpStat.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


//-------------------------------------------------------------------------------------------------------
//In total six channel indexes are available (0 -> 5). As it is possible to run three IEEE802.15.4 stacks, channel indexes 0, 1 and 2 can be taken
//by these stacks. BLE channel index is reserved on channel index 3. For ED scans, channel index 5 is used.
#define GPHAL_ZB_CHANNEL_IDX (0)
#define GPHAL_ED_SCAN_CHANNEL_IDX (5)

/* gpHalPhy_GetFemTxOptions parameter constants */
#define GPHAL_TX_OPTIONS_FOR_TRANSMIT   false
#define GPHAL_TX_OPTIONS_FOR_ACK        true

#ifndef GP_BSP_RX_ANTSEL_INT_DEFAULT
#if defined(GP_HAL_DIVERSITY_SINGLE_ANTENNA)
#define GP_BSP_RX_ANTSEL_INT_DEFAULT GP_HAL_ANT_TO_ANTSEL_INT(GP_HAL_DIVERSITY_SINGLE_ANTENNA)
#else
#define GP_BSP_RX_ANTSEL_INT_DEFAULT GP_WB_ENUM_ANTSEL_INT_USE_PORT_FROM_DESIGN
#endif
#endif


#ifndef GP_BSP_TX_ANTSEL_INT_DEFAULT
#if defined(GP_HAL_DIVERSITY_SINGLE_ANTENNA)
#define GP_BSP_TX_ANTSEL_INT_DEFAULT GP_HAL_ANT_TO_ANTSEL_INT(GP_HAL_DIVERSITY_SINGLE_ANTENNA)
#else
#define GP_BSP_TX_ANTSEL_INT_DEFAULT GP_WB_ENUM_ANTSEL_INT_USE_PORT_FROM_DESIGN
#endif
#endif

#define GP_HAL_COEX_PIN_CONFIG(REQ_PIN, PRIO0_PIN, PRIO1_PIN, GRANT_PIN)                                               \
    {                                                                                                                  \
        .req = {.pin = REQ_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(REQ_PIN, AKUMA, COEX, SL_REQ)},                   \
        .prio0 = {.pin = PRIO0_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(PRIO0_PIN, AKUMA, COEX, SL_PRIO_0)},          \
        .prio1 = {.pin = PRIO1_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(PRIO1_PIN, AKUMA, COEX, SL_PRIO_1)},          \
        .grant = {.pin = GRANT_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(GRANT_PIN, AKUMA, COEX, SL_GRANT)},           \
    }

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/** PbmAddress is the base address of a pbm entry
 *  this macro checks if startOfData points to a byte in the FRAME area of the PBM entry.
 */
#define GP_HAL_IS_VALID_PBM_FRAME_PTR(PbmAddress, startOfData) GP_HAL_IS_VALID_PBM_FRAME_OFFSET((startOfData)-((PbmAddress)+GPHAL_REGISTER_PBM_FORMAT_T_FRAME_0))

#define GP_HAL_IS_DATA_CONFIRM_CALLBACK_REGISTERED()                            gpHal_DataConfirmCallback

#define GP_HAL_CB_DATA_CONFIRM(status, pdLoh, lastChannelUsed)                  gpHal_DataConfirmCallback(status, pdLoh, lastChannelUsed)

#define GP_HAL_IS_DATA_INDICATION_CALLBACK_REGISTERED()                         gpHal_DataIndicationCallback

#define GP_HAL_CB_DATA_INDICATION(pdLoh, rxInfo)                                gpHal_DataIndicationCallback(pdLoh, rxInfo)

#define GP_HAL_IS_ED_CONFIRM_CALLBACK_REGISTERED()                              gpHal_EDConfirmCallback

#define GP_HAL_CB_ED_CONFIRM(channelMask, protoED)                              gpHal_EDConfirmCallback(channelMask, protoED)

#define GP_HAL_IS_EXTERNAL_EVENT_CALLBACK_REGISTERED()                          gpHal_ExternalEventCallback

#define GP_HAL_CB_EXTERNAL_EVENT()                                              gpHal_ExternalEventCallback()

#define GP_HAL_IS_FRAME_QUEUED_CALLBACK_REGISTERED()                            gpHal_MacFrameQueued

#define GP_HAL_CB_FRAME_QUEUED()                                                gpHal_MacFrameQueued()

#define GP_HAL_IS_FRAME_UNQUEUED_CALLBACK_REGISTERED()                          gpHal_MacFrameUnqueued

#define GP_HAL_CB_FRAME_UNQUEUED()                                              gpHal_MacFrameUnqueued()

#define GP_HAL_ANT_TO_ANTSEL_INT(ant)                                           (1==(ant)?GP_WB_ENUM_ANTSEL_INT_USE_PORT_1:GP_WB_ENUM_ANTSEL_INT_USE_PORT_0)
#define GP_HAL_ANT_TO_ANTSEL_EXT(ant)                                           (1==(ant)?GP_WB_ENUM_ANTSEL_EXT_USE_EXT_PORT_1:GP_WB_ENUM_ANTSEL_EXT_USE_EXT_PORT_0)
//implies USE_PORT_FROM_DESIGN will be threaded as fixed antenna 0
#define GP_HAL_ANTSEL_INT_TO_ANT(antsel)                                        (GP_WB_ENUM_ANTSEL_INT_USE_PORT_1==(antsel)?1:0)
#define GP_HAL_ANTSEL_EXT_TO_ANT(antsel)                                        (GP_WB_ENUM_ANTSEL_EXT_USE_EXT_PORT_1==(antsel)?1:0)


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

#if defined(GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH) && !defined(GP_COMP_CHIPEMU)
extern UInt16 gpHal_RtSystem_FlashData[];
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH



/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_InitSleep(void);

//Component dependent ISR macro's
#ifdef GP_COMP_GPHAL_RADIO
void gpHal_RCIInterrupt(void);
#endif //GP_COMP_GPHAL_RADIO

#if defined(GP_COMP_GPHAL_BLE)
void gpHal_Handle_All_RCIAdvIndIRQ(void);
#endif

#ifdef GP_COMP_GPHAL_RADIO
void gpHal_IPCInterrupt(void);
void gpHal_IpcGpm2XInterrupt(void);
#endif //GP_COMP_GPHAL_RADIO

#if defined(GP_COMP_GPHAL_ES_EXT_EVENT) || defined(GP_COMP_GPHAL_ES_ABS_EVENT) || defined(GP_COMP_GPHAL_ES_REL_EVENT)
void gpHal_ESInterrupt(void);
#endif

typedef struct gpHal_PbmCoexOptions_s {
    UInt8 coex_tx_prio;
    Bool coex_tx_req_en;
    UInt8 coex_ack_rx_prio;
    Bool coex_ack_rx_req_en;
    Bool coex_grant_aware_txpack;
} gpHal_PbmCoexOptions_t;

typedef struct gpHal_PbmTxOptions_s {
    Bool first_boff_is_0;
    Bool csma_ca_enable;
    Bool treat_csma_fail_as_no_ack;
    UInt8 vq_sel;
    Bool skip_cal_tx;
    UInt8 confirm_queue;
    Bool acked_mode;
    UInt8 antenna;
    Bool fcs_insert_dis;
    Bool stop_rx_window_on_full_pbm;
    Bool ed_scan;
    Bool rx_duration_valid;
    Bool start_rx_only_if_pending_bit_set;
    Bool channel_ch0a_en;
    Bool channel_ch0b_en;
    Bool channel_ch0c_en;
    Bool channel_ch1_en;
    UInt8 channel_idx;
    UInt8 tx_power_setting; //pa_ultralow<<7 | pa_low<<6 | tx_power
    UInt8 phy_mode;
    UInt8 antsel_int;
    UInt8 antsel_ext;
    Bool ext_pa_required;
    UInt8 min_be;
    UInt8 max_be;
    UInt8 max_csma_backoffs;
    UInt32 poll_req_max_wait_time;
    UInt8 max_frame_retries;
    UInt8 num_channel_slots;
    UInt8 channel_ch0a;
    UInt8 channel_ch0b;
    UInt8 channel_ch0c;
    UInt8 channel_ch1;
    UInt8 rx_duration;
    UInt8 state;
    UInt8 return_code;
    UInt8 frame_ptr;
    UInt8 frame_len;

    gpHal_PbmCoexOptions_t coex_settings;
    gpHal_TxPower_t txPower;
    Bool retransmit_on_cca_fail;
} gpHal_PbmTxOptions_t;


void gpHalPhy_GetFemTxOptions(gpHal_TxPower_t txPower, gpHal_FllChannel_t fllChannel, gpHal_PbmTxOptions_t* pTxOptions, Bool forAck);

#ifdef GP_COMP_GPHAL_MAC

extern gpHal_MacFrameQueued_t gpHal_MacFrameQueued;
extern gpHal_MacFrameUnqueued_t gpHal_MacFrameUnqueued;

void gpHal_IncrementMacQueueUsage(void);
void gpHal_DecrementMacQueueUsage(void);
#endif //GP_COMP_GPHAL_MAC

extern void gpHal_Scan_EDConfirm(UInt8 channel, UInt8 protoRssi);

void gpHal_InitScan(void);

#ifdef GP_COMP_GPHAL_PBM
void gpHal_PbmInit(void);
#endif //GP_COMP_GPHAL_PBM
#ifdef GP_COMP_GPHAL_RADIO
void gpHal_CalibrateFllChannel(UInt8 srcId, UInt8 fllChannel);
void gpHal_CalibrateFllChannelUnprotected(gpHal_ChannelSlotIdentifier_t slotId, UInt8 fllChannel);
#endif //GP_COMP_GPHAL_RADIO

#ifdef GP_COMP_GPHAL_BLE
void gpHal_RegisterBleCallbacks(gpHal_BleCallbacks_t* pCallbacks);

// callbacks that stay in gpHal layer
void gpHal_cbBleDataIndication(UInt8 pbmEntry);
void gpHal_cbBleDataConfirm(UInt8 pbmEntry);
void gpHal_cbBleAdvertisingIndication(UInt8 pbmEntry);
void gpHal_cbBleConnectionRequestIndication(UInt8 pbmEntry);
void gpHal_cbBleConnectionRequestConfirm(void);
void gpHal_cbBleEventProcessed(UInt8 eventId);
void gpHal_cbBleLastConnEventCountReached(UInt8 eventId);
void gpHal_cbBleScanRequestReceived(void);
void gpHal_cbBleIsoRxIndication(void);

void gpHal_BleCommonInit(void);
void gpHal_BleTestModeInit(void);
void gpHal_BleWlInit(void);
void gpHal_BleValidationInit(void);
void gpHal_BleInitAdvValidationSettings(gpHal_Address_t access_addr_ptr);
void gpHal_BleDirectionFindingInit(void);

void gpHal_BleRpa_Init(void);


Bool gpHal_BleTestModeIsActive(void);
void gpHal_BleSetClockAccuracy(UInt16 caPpm);

UInt32 gpHal_BleGetNextEventTs(gpHal_Address_t eventInfoAddress);
#endif //GP_COMP_GPHAL_BLE


void gpHal_cbEventTooLate(void);

#if defined(GP_COMP_GPHAL_DCDC)
void gpHal_InitDCDC(void);
void gpHalDCDC_BlockDCDC(Bool block);
Bool gpHalDCDC_IsActive(void);
#endif // GP_DIVERSITY_GPHAL_DCDC

UInt8 gpHalPhy_GetTxPowerSetting(gpHal_TxPower_t requested_txPower_dBm, Bool forAck);

void gpHalES_OscillatorBenchmarkDone_Handler(void);

#if defined(GP_DIVERSITY_GPHAL_OSCILLATOR_BENCHMARK)
gpHal_SleepClockMeasurementStatus_t gpHalEs_Get32kHzBenchmarkStatus(void);
#endif

#if defined(GP_COMP_GPHAL_MAC)
void gpHalMac_SetAckTransmitPowers(void);
#endif //defined(GP_COMP_GPHAL_MAC)

#if defined(GP_HAL_DIVERSITY_EXT_MODE_SIGNALS)
UInt8 gpHalPhy_GetTxMode(gpHal_TxPower_t txPower);
#endif



typedef struct {
    gpHal_ContinuousWaveMode_t mode;
    UInt8 channel;
    Int8 txpower;
    gpHal_AntennaSelection_t antenna;
} gpHalPhy_CurrentCWModeSettings_t;

// Fetch the last used active CW settings
gpHalPhy_CurrentCWModeSettings_t *gpHal_GetCurrentContinuousWaveModeSettings(void);

#if defined(GP_DIVERSITY_GPHAL_TRIM_XTAL_32M)
void gpHal_Xtal32MHzTrimAlgorithmInit(void);
#endif

#ifndef GP_DIVERSITY_GPHAL_DISABLE_TRIM_VDD_RAM_TUNE
/* Initialize the VddRamTune tuning algorithm */
void gpHal_VddRamTuneTrimAlgoInit(void);

/* Restore the VddRamtune value last measured by algorithm*/
void gpHal_VddRamTuneTrimAlgoRestoreTuneValue(void);
#endif

#if defined(GP_COMP_GPHAL_MAC)
/** @typedef gpHal_RxModeConfig_t
 *  @brief   The gpHal_RxModeConfig_t typedef defines the structure used to pass Receiver Mode configurations. */
 typedef struct gpHal_RxModeConfig_s {
    gpHal_RxMode_t                  rxMode;
    gpHal_AntennaMode_t             antennaMode;
    gpHal_Channel_t                 channelList[GP_HAL_MAX_NUM_OF_SLOTS];
 } gpHal_RxModeConfig_t;

/** @brief This function sets a new receiver mode configuration.
 *
 *  This function sets the configuration to be applied for receiver (mode, channel(s) used, attenuation) during testing.
 *
 *  @param rxModeConfig             Reference to the storage location of new configuration info
 *
 *  @return void
 */
GP_API void gpHal_SetRxModeConfig(gpHal_RxModeConfig_t* rxModeConfig);

#ifdef GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION
/**
 * @brief Clears the Nonce and Key data for encryption of raw frames.
 *
 * The function will clear the fields for the Nonce, and reset the key configuration for the encryption of raw frames.
 *
 */
void gpHalMac_RawModeInit(void);
#endif //def GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION

void gpHal_InitMAC_RxWindows(void);
void gpHal_HandleTimedRxDoneZb(UInt8 PBMentry);
void gpHal_CalibrateMacChannel(gpHal_ChannelSlotIdentifier_t srcId, UInt8 macChannel);
UInt8 gpHalMac_GetChannel_LowTXPower(UInt8* low_channel);
void gpHalMac_ApplyAckTransmitSettings(gpHal_SourceIdentifier_t srcId, UInt8 channel);

#endif //defined(GP_COMP_GPHAL_MAC)

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Sets the availability of the 32kHz crystal.
 *
 * This function allows the application to set whether the 32kHz crystal is available.
 * The benchmarking algorithm will assess and confirm the availability of the 32kHz crystal.
 *
 * @param[in] available Boolean value indicating the availability of the 32kHz crystal.
 *                      - true: 32kHz crystal is available.
 *                      - false: 32kHz crystal is not available.
 */
void gpHal_Set32kHzCrystalAvailable(Bool available);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_HAL_GP_DEFS_H_
