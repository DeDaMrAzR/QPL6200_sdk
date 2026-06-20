/*
 * Copyright (c) 2017, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by\
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

#ifndef _GPBLEADVERTISER_H_
#define _GPBLEADVERTISER_H_

#if defined(GP_DIVERSITY_ROM_CODE)
#include "gpBleAdvertiser_RomCode.h"
#else //defined(GP_DIVERSITY_ROM_CODE)


/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "gpHci_Includes.h"
#include "gpBle.h"
#include "gpHal.h"
#include "gpBle_defs.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GPBLEADVERTISER_TIME_OFFSET_INVALID                 0xFFFFFFFF

#ifndef GP_BLE_EA_MAX_ADVDATA_LENGTH
#define GP_BLE_EA_MAX_ADVDATA_LENGTH (1650)
#endif

#ifndef GP_BLE_MAX_NUMBER_PERADV_SETS
#define GP_BLE_MAX_NUMBER_PERADV_SETS   GP_BLE_MAX_NUMBER_ADVERTISING_SETS
#endif //GP_BLE_MAX_NUMBER_PERADV_SETS

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

// Callback prototype for ACAD data population (returns total number of bytes added to PDU)
typedef UInt8 (* gpBlePeriodicAdvertiser_AcadCb_t) (gpHci_AdvertisingHandle_t advertisingHandle, gpPd_Loh_t* pPdLoh, UInt8 nrOfBytesRemaining);

/*****************************************************************************
 *                    Component Function Definitions
 *****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#if defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
#include "gpBleAdvertiser_CodeJumpTableFlash_Defs.h"
#include "gpBleAdvertiser_CodeJumpTableRom_Defs.h"
#endif // defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)

// Advertiser
void gpBleAdvertiser_Init(gpHal_BleCallbacks_t* pCallbacks);
void gpBleAdvertiser_Reset(Bool firstReset);
/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_START */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_START */
Bool gpBleAdvertiser_IsEnabled(void);
Bool gpBleAdvertiser_IsFilterAcceptListUsed(void);
UInt32 gpBleAdvertiser_GetIntervalUs(void);
/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_END */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_END */

/*****************************************************************************
 *                    Service Function Definitions
 *****************************************************************************/

/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_START */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_START */
gpHci_Result_t gpBle_LeSetAdvertisingParameters(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
gpHci_Result_t gpBle_LeSetAdvertisingData(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
gpHci_Result_t gpBle_LeSetScanResponseData(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
gpHci_Result_t gpBle_LeReadAdvertisingChannelTxPower(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
gpHci_Result_t gpBle_LeSetAdvertiseEnable(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_END */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_END */
#ifdef GP_DIVERSITY_DEVELOPMENT
gpHci_Result_t gpBleAdvertiser_SetVsdAdvEventPriority(UInt16 priority);
#endif //  GP_DIVERSITY_DEVELOPMENT




#ifdef __cplusplus
}
#endif

#endif //defined(GP_DIVERSITY_ROM_CODE)

#endif //_GPBLEADVERTISER_H_
