/*
 * Copyright (c) 2024, Qorvo Inc
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

#ifndef _REG_SYS_RAM_H_
#define _REG_SYS_RAM_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get the retain bit position for a given peripheral.*/
#define Q_REG_SYSRAM_RETAIN_GET(NAME)                                                                                  \
    ((Q_CONCAT(GP_WB_SYSRAM_RETAIN_, NAME, _ADDRESS) - GP_WB_SYSRAM_RETENTION_MAP_0_ADDRESS) * 8uL +                   \
     Q_CONCAT(GP_WB_SYSRAM_RETAIN_, NAME, _LSB))

/** @enum qRegSysRam_Retain_t */
//@{
/* SYSRAM_RETENTION_MAP_0_ADDRESS */
#define qRegSysRam_RetainAppuc       Q_REG_SYSRAM_RETAIN_GET(APPUC)
#define qRegSysRam_RetainIntCtrl     Q_REG_SYSRAM_RETAIN_GET(INT_CTRL_LSB)
#define qRegSysRam_RetainGpio        Q_REG_SYSRAM_RETAIN_GET(GPIO)
#define qRegSysRam_RetainIOB         Q_REG_SYSRAM_RETAIN_GET(IOB)
#define qRegSysRam_RetainUART0       Q_REG_SYSRAM_RETAIN_GET(UART_0)
#define qRegSysRam_RetainUART1       Q_REG_SYSRAM_RETAIN_GET(UART_1)
#define qRegSysRam_RetainUART2       Q_REG_SYSRAM_RETAIN_GET(UART_2)
#define qRegSysRam_RetainRTZ0        Q_REG_SYSRAM_RETAIN_GET(RTZ_0)
#define qRegSysRam_RetainRTZ1        Q_REG_SYSRAM_RETAIN_GET(RTZ_1)
#define qRegSysRam_RetainASP0        Q_REG_SYSRAM_RETAIN_GET(ASP_0)
#define qRegSysRam_RetainASP1        Q_REG_SYSRAM_RETAIN_GET(ASP_1)
#define qRegSysRam_RetainSPISL       Q_REG_SYSRAM_RETAIN_GET(SPI_SL)
#define qRegSysRam_RetainSPIM        Q_REG_SYSRAM_RETAIN_GET(SPI_M)
#define qRegSysRam_RetainQSPI        Q_REG_SYSRAM_RETAIN_GET(QSPI_M)
#define qRegSysRam_RetainIR          Q_REG_SYSRAM_RETAIN_GET(IR)
#define qRegSysRam_RetainI2CM        Q_REG_SYSRAM_RETAIN_GET(I2C_M)
#define qRegSysRam_RetainI2CSL       Q_REG_SYSRAM_RETAIN_GET(I2C_SL)
#define qRegSysRam_RetainI2SM        Q_REG_SYSRAM_RETAIN_GET(I2S_M)
#define qRegSysRam_RetainTimers      Q_REG_SYSRAM_RETAIN_GET(TIMERS)
#define qRegSysRam_RetainDMAs        Q_REG_SYSRAM_RETAIN_GET(DMAS_SCB)
#define qRegSysRam_RetainBist        Q_REG_SYSRAM_RETAIN_GET(BIST)
#define qRegSysRam_RetainI2SVsr      Q_REG_SYSRAM_RETAIN_GET(I2S_VSR)
#define qRegSysRam_RetainKeypad0     Q_REG_SYSRAM_RETAIN_GET(KEYPAD)
#define qRegSysRam_RetainGPADC       Q_REG_SYSRAM_RETAIN_GET(GP_ADC)
#define qRegSysRam_RetainCMA         Q_REG_SYSRAM_RETAIN_GET(CMA)
#define qRegSysRam_RetainOTP         Q_REG_SYSRAM_RETAIN_GET(OTP)
#define qRegSysRam_RetainMSI         Q_REG_SYSRAM_RETAIN_GET(MSI)
#define qRegSysRam_RetainDebug       Q_REG_SYSRAM_RETAIN_GET(DEBUG)
#define qRegSysRam_RetainBBPLL       Q_REG_SYSRAM_RETAIN_GET(BBPLL)
#define qRegSysRam_RetainFractClkGen Q_REG_SYSRAM_RETAIN_GET(FRACT_CLK_GEN)
#define qRegSysRam_RetainDCDC        Q_REG_SYSRAM_RETAIN_GET(DCDC)
#define qRegSysRam_RetainLEDs        Q_REG_SYSRAM_RETAIN_GET(LEDS)
/* SYSRAM_RETENTION_MAP_1_ADDRESS */
#define qRegSysRam_RetainPWMs     Q_REG_SYSRAM_RETAIN_GET(PWMS)
#define qRegSysRam_RetainPWMXLs   Q_REG_SYSRAM_RETAIN_GET(PWMXLS)
#define qRegSysRam_RetainWatchdog Q_REG_SYSRAM_RETAIN_GET(WATCHDOG)
#define qRegSysRam_RetainQUIC     Q_REG_SYSRAM_RETAIN_GET(QUIC)
#define qRegSysRam_RetainEs       Q_REG_SYSRAM_RETAIN_GET(ES)
/** @typedef qRegSysRam_Retain_t
 *  @brief SysRam retain peripheral enumeration.
 */
typedef UInt8 qRegSysRam_Retain_t;
//@}

/** @brief Enable/disable retain for the specified . */

/**
 * @brief Function for enabling or disabling the retain for the specified peripheral.
 *
 * @param[in] retain Retain peripheral to be changed.
 * @param[in] enable True if the retain is to be enabled, false otherwise.
 */
static inline void qSysRam_RetainEnableSet(qRegSysRam_Retain_t retain, Bool enable)
{
    UInt32 retainAddress = GP_WB_SYSRAM_RETAIN_UART_0_ADDRESS + (retain / 32) * sizeof(UInt32);
    UInt32 enableMask = (1UL << ((UInt32)retain % 32));

    GP_WB_MWRITE_U32(retainAddress, enableMask, enable ? enableMask : 0UL);
}

#ifdef __cplusplus
}
#endif

#endif /* _REG_SYS_RAM_H_ */
