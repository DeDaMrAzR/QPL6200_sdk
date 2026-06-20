/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
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

#ifndef _GPHAL_KX_PUBLIC_H_
#define _GPHAL_KX_PUBLIC_H_

#if defined(GP_DIVERSITY_ROM_CODE)
#include "gpHal_RomCode_kx_public.h"
#else //defined(GP_DIVERSITY_ROM_CODE)

#include "gpBsp.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_HAL_ENABLE_INTERRUPTS(enable)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
    } while(false)

#define GP_HAL_ENABLE_PIO_INT(enable) /*PIO = RCI */                                        \
    do {                                                                                    \
          GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_IND_INTERRUPT(enable);                 \
          GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_0_INTERRUPT(enable); /*Data Cfm*/  \
          GP_WB_WRITE_AK_INT_CTRL_MASK_IPCRT2X_RCI_DATA_CNF_1_INTERRUPT(enable); /*ED scan*/   \
    } while (false)

#define GP_HAL_ENABLE_EMPTY_QUEUE_CALLBACK_INTERRUPT(enable) GP_WB_WRITE_AK_INT_CTRL_MASK_QTA_SCH0_EMPTY_INTERRUPT(enable)
#define GP_HAL_ENABLE_BUSY_TX_CALLBACK_INTERRUPT(enable)     /*No longer available ?*/

//gpHal_MAC.h specific macro's

#define GP_HAL_GET_ADDRESS_RECOGNITION()          (GP_WB_READ_MACFILT_DST_PAN_ID_CHECK_ON() || GP_WB_READ_MACFILT_DST_ADDR_CHECK_ON())
//Note - bitmap given as 'filter when set' - actual register 'accepts when set' - inverted here
#define GP_HAL_SET_FRAME_TYPE_FILTER_MASK(bitmap) GP_HAL_WRITE_REG(GP_WB_MACFILT_ACCEPT_FT_BCN_ADDRESS, (UInt8)(~(bitmap)))
#define GP_HAL_GET_FRAME_TYPE_FILTER_MASK()       ((UInt8)(~(GP_HAL_READ_REG(GP_WB_MACFILT_ACCEPT_FT_BCN_ADDRESS))))
#define GP_HAL_GET_RX_ON_WHEN_IDLE()              (!!(GP_WB_READ_RT_RX_ON_WHEN_IDLE_CH()))

#define GP_HAL_SET_PIP_MODE(pipmode)              GP_WB_WRITE_AK_RX_EN_PIP(pipmode)
#define GP_HAL_GET_PIP_MODE()                     GP_WB_READ_AK_RX_EN_PIP()

//Mac state containing timed, calibration and receiver status
#define GP_HAL_MAC_STATE_INIT()                 { gpHal_MacState = 0; }
#define GP_HAL_MAC_STATE_SET_TIMED()            BIT_SET(gpHal_MacState, 0)
#define GP_HAL_MAC_STATE_CLR_TIMED()            BIT_CLR(gpHal_MacState, 0)
#define GP_HAL_MAC_STATE_GET_TIMED()            BIT_TST(gpHal_MacState, 0)

#define GP_HAL_MAC_STATE_SET_CALIBRATED()       BIT_SET(gpHal_MacState, 1)
#define GP_HAL_MAC_STATE_GET_CALIBRATED()       BIT_TST(gpHal_MacState, 1)

#define GP_HAL_MAC_STATE_SET_RXON()             BIT_SET(gpHal_MacState, 2)
#define GP_HAL_MAC_STATE_CLR_RXON()             BIT_CLR(gpHal_MacState, 2)
#define GP_HAL_MAC_STATE_GET_RXON()             BIT_TST(gpHal_MacState, 2)

#define GP_HAL_MAC_STATE_SET_PROMISCUOUS_MODE() BIT_SET(gpHal_MacState, 3)
#define GP_HAL_MAC_STATE_CLR_PROMISCUOUS_MODE() BIT_CLR(gpHal_MacState, 3)
#define GP_HAL_MAC_STATE_GET_PROMISCUOUS_MODE() BIT_TST(gpHal_MacState, 3)

#define GP_HAL_MAC_STATE_SET_ACKPBM_INITIALIZED() BIT_SET(gpHal_MacState, 7)
#define GP_HAL_MAC_STATE_GET_ACKPBM_INITIALIZED() BIT_TST(gpHal_MacState, 7)

// Number of SRCIDS supported by this hal
#define GP_HAL_MAC_NUMBER_OF_RX_SRCIDS       3

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Bool gpHal_wb_memcmp(const UInt8 *ptr0, UIntPtr addr, UInt8 length);

/** @brief Disables the HW RX MAC.
 *
 *  This functions enables the re-use of PBMs for other purposes.
 *  The use case for this is IR TX.
*/
GP_API UInt16* gpHal_ClaimPbmMemory(void);
/** @brief Enables the HW RX MAC.
*/
GP_API Bool gpHal_ReleasePbmMemory(void);

// Default, minimum and maximum Tx powers for this module
#define GPHAL_MIN_TRANSMIT_POWER        (-24)


Int8 gpHal_GetMinTxPower(void);
Int8 gpHal_GetMaxTxPower(void);

#ifndef GP_COMP_CHIPEMU
#define GPHAL_MAX_TRANSMIT_POWER (gpHal_GetMaxTxPower())
#else
#define GPHAL_MAX_TRANSMIT_POWER (7)
#endif


#define GPHAL_DEFAULT_TRANSMIT_POWER    GPHAL_MAX_TRANSMIT_POWER

/* Code for sleep handling */
void gpHalDCDC_AfterWakeup(void);
void gpHalDCDC_BeforeSleep(void);

#if defined(GP_COMP_GPHAL_DCDC)
#define GPHAL_DCDC_AFTER_WAKEUP()         gpHalDCDC_AfterWakeup()
#define GPHAL_DCDC_BEFORE_SLEEP()         gpHalDCDC_BeforeSleep()
#else
#define GPHAL_DCDC_AFTER_WAKEUP()
#define GPHAL_DCDC_BEFORE_SLEEP()
#endif // GP_COMP_GPHAL_DCDC

#define HAL_SLEEP_CB_BEFORESLEEP()     do {\
    GPHAL_DCDC_BEFORE_SLEEP();\
} while(0)
#define HAL_SLEEP_CB_AFTERWAKEUP()     do {\
    GPHAL_DCDC_AFTER_WAKEUP();\
} while(0)

/** @brief Indicates if chip is emulated
*/
#define gpHal_IsEmulated()     false

#ifdef __cplusplus
}
#endif

#endif //defined(GP_DIVERSITY_ROM_CODE)

#endif  /* _GPHAL_KX_PUBLIC_H_ */
