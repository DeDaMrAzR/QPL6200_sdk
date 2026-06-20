/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
 *
 * gphal_Reset.c
 *
 *  The file gpHal_reset.c contains startup code of the gphal (init, reset).
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal.h"
#ifdef GP_COMP_GPHAL_RADIO
#include "gpHal_DEFS.h"
#endif //GP_COMP_GPHAL_RADIO


#include "gpHal_DEFS.h"
#include "gpHal_Statistics.h"

//GP hardware dependent register definitions
#include "gpHal_HW.h"          //Containing all uC dependent implementations
#include "gpHal_reg.h"
#include "gpHal_Calibration.h"
#include "gpAssert.h"

#include "gpLog.h"
#include "gpReset.h"

#include "hal_kx_Flash.h"

#if defined(GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP)
#include "gpHal_TrimRxMode.h"
#endif // GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP


#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpAppUcSubsystem_Calibrations.h"
#ifndef GP_COMP_CHIPEMU
#include "hal_SEMailbox.h"
#endif //GP_COMP_CHIPEMU


#include "qDrvIOB.h"
#include "qRegInfoPage.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GPHAL_TEST_ADDRESS         GP_WB_AK_RX_BT_OBSERVE_CO_0_ADDRESS
#define GPHAL_TEST_ADDRESS_VALUE   0x9876

#define XP32KP_PIN 25 // GPIO_25
#define XP32KN_PIN 26 // GPIO_26
/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

// pan coordinator regmap field has different encoding: (1 = no pan coordinator, 2 = pan coordinator)
#define GP_HAL_PAN_COORDINATOR_TO_ADDRESSMAP_FIELD(panCoordinator)  (panCoordinator + 1)

#ifndef GP_HAL_BSP_INIT
//No generated GPIO initialization available - for external usage
#define GP_HAL_BSP_INIT()
#endif //GP_HAL_BSP_INIT

#if defined(GP_HAL_DIVERSITY_EXT_MODE_SIGNALS)
#if defined(GP_BSP_RF_DEBUG_TX_SUPPORTED) || defined(GP_BSP_RF_DEBUG_RX_SUPPORTED)
#error Cannot combine RF debug signals with external model signals
#endif //defined(GP_BSP_RF_DEBUG_TX_SUPPORTED) || defined(GP_BSP_RF_DEBUG_RX_SUPPORTED)
#endif //GP_HAL_DIVERSITY_EXT_MODE_SIGNALS

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void gpHal_InitRadioSettings(void);

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_Init(Bool timedMAC)
{
    NOT_USED(timedMAC);

    gpHal_InfoPageInit();

    if(gpHal_32kHzCrystalAvailable())
    {
        // Enable to work with 32kHz Xtal sleep mode0
        qRegIOB_AlternateEnableSet(XP32KP_PIN, false);
        qRegIOB_AlternateSet(XP32KP_PIN, GP_WB_ENUM_GPIO_25_ALTERNATES_ANALOG_XP32KP);
        qRegIOB_AlternateEnableSet(XP32KP_PIN, true);

        qRegIOB_AlternateEnableSet(XP32KN_PIN, false);
        qRegIOB_AlternateSet(XP32KN_PIN, GP_WB_ENUM_GPIO_26_ALTERNATES_ANALOG_XP32KN);
        qRegIOB_AlternateEnableSet(XP32KN_PIN, true);

#ifndef GP_COMP_CHIPEMU
        hal_SEMailbox_XT32kPowerUp(true);
#endif // GP_COMP_CHIPEMU
    }

#if defined(GP_HAL_DIVERSITY_INCLUDE_IPC) 
    gpHal_IpcInit();
#endif // defined(GP_HAL_DIVERSITY_INCLUDE_IPC) && !defined(GP_DIVERSITY_NO_RT)


#ifdef GP_COMP_GPHAL_RADIO
    gpHal_RadioInterruptsInit();
#endif //GP_COMP_GPHAL_RADIO

    gpHal_InitCalibration();
#if defined(GP_COMP_CHIPEMU)
    // chipemu do not use hal init
    gpHal_InitEs();
#endif // GP_COMP_CHIPEMU

    gpHal_FlashInit();

    /* calls ak_rap_sys_hal_init */
    gpHal_InitRadioSettings();


#if defined(GP_COMP_GPHAL_PBM) 
    /* needs to be after the FpgaInit */
    gpHal_PbmInit();
#endif // defined(GP_COMP_GPHAL_PBM) && !defined(GP_DIVERSITY_NO_RT)

    gpAppUcSubsystem_CalibrationInit();

#ifdef GP_COMP_GPHAL_RADIO
    gpHal_RtEventsInit();
    gpHal_InitPhy();
#endif //GP_COMP_GPHAL_RADIO

#ifdef GP_COMP_GPHAL_BLE
    gpHal_InitBle();
#endif //GP_COMP_GPHAL_BLE

#ifdef GP_COMP_GPHAL_MAC
    gpHal_InitScan();
    gpHal_InitMAC();
#endif //GP_COMP_GPHAL_MAC

#ifdef GP_DIVERSITY_GPHAL_INDIRECT_TRANSMISSION
    gpHal_InitDataPending();
#endif //GP_DIVERSITY_GPHAL_INDIRECT_TRANSMISSION


    gpHal_InitSleep();


#ifdef GP_DIVERSITY_DEVELOPMENT
#ifdef GP_BSP_RF_DEBUG_RX_SUPPORTED
    GP_WB_WRITE_AK_TRX_EXT_MODE_CTRL_FOR_RX_ON(BM(GP_BSP_RF_DEBUG_RX_MODE_CTRL_BIT));
    GP_BSP_RF_DEBUG_RX_INIT();
#endif //GP_BSP_RF_DEBUG_RX_SUPPORTED
#ifdef GP_BSP_RF_DEBUG_TX_SUPPORTED
    GP_BSP_RF_DEBUG_TX_INIT();
#endif //GP_BSP_RF_DEBUG_TX_SUPPORTED
#endif //GP_DIVERSITY_DEVELOPMENT

#ifndef GP_COMP_CHIPEMU
    hal_SEMailboxInit();
#endif

    gpHal_StatisticsCountersClear();

#if defined(GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP)
    gpHal_RxModeSwitchAlgoInit();
#endif // defined(GP_HAL_DIVERSITY_SWITCH_TO_HS_AT_LOW_TEMP)
}

Bool gpHal_DidGPReset(void)
{
    if(GP_WB_READ_STANDBY_UNMASKED_PORD_INTERRUPT())
    {
        GP_WB_STANDBY_CLR_PORD_INTERRUPT();
        return true;
    }
    return false;
}

Bool gpHal_CheckMsi(void)
{
    return (GPHAL_TEST_ADDRESS_VALUE == GP_WB_READ_U16(GPHAL_TEST_ADDRESS));
}

void gpHal_AdvancedInit(void)
{
#if(defined(GP_COMP_GPHAL_ES) && (defined(GP_SCHED_DIVERSITY_SLEEP) || defined(GP_FREERTOS_DIVERSITY_SLEEP)))
    // Start taking benchmark measurements for calibrating the sleep clock
    gpHal_StartInitialBenchmarkMeasurements();
#endif //ifdef GP_COMP_GPHAL_ES
#ifdef GP_DIVERSITY_GPHAL_DCDC_ENABLED
    gpHal_DCDCEnable();
#endif

}

void gpHal_InitRadioSettings(void)
{
    ak_rap_sys_reset_early();
    ak_rap_sys_hal_init();
}
