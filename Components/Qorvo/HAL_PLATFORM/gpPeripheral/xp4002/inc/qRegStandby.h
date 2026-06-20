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

#ifndef _Q_REG_STANDBY_H_
#define _Q_REG_STANDBY_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @enum qRegStandby_ResetFunc_t */
//@{
#define qRegStandby_ResetFuncAkuma            GP_WB_STANDBY_RESET_FUNC_AKUMA_LSB
#define qRegStandby_ResetFuncClkgenFracClkGen GP_WB_STANDBY_RESET_FUNC_CLKGEN_FRACT_CLK_GEN_LSB
#define qRegStandby_ResetFuncCssys            GP_WB_STANDBY_RESET_FUNC_CSSYS_LSB
#define qRegStandby_ResetFuncEs               GP_WB_STANDBY_RESET_FUNC_ES_LSB
#define qRegStandby_ResetFuncPeripAsp0        GP_WB_STANDBY_RESET_FUNC_PERIP_ASP_0_LSB
#define qRegStandby_ResetFuncPeripAsp1        GP_WB_STANDBY_RESET_FUNC_PERIP_ASP_1_LSB
#define qRegStandby_ResetFuncPeripGpAdc       GP_WB_STANDBY_RESET_FUNC_PERIP_GP_ADC_LSB
#define qRegStandby_ResetFuncPeripHrAdc       GP_WB_STANDBY_RESET_FUNC_PERIP_HR_ADC_LSB
#define qRegStandby_ResetFuncPeripI2cM0       GP_WB_STANDBY_RESET_FUNC_PERIP_I2C_M_0_LSB
#define qRegStandby_ResetFuncPeripI2cM1       GP_WB_STANDBY_RESET_FUNC_PERIP_I2C_M_1_LSB
#define qRegStandby_ResetFuncPeripI2cSl       GP_WB_STANDBY_RESET_FUNC_PERIP_I2C_SL_LSB
#define qRegStandby_ResetFuncPeripI2s0        GP_WB_STANDBY_RESET_FUNC_PERIP_I2S_0_LSB
#define qRegStandby_ResetFuncPeripI2s1        GP_WB_STANDBY_RESET_FUNC_PERIP_I2S_1_LSB
#define qRegStandby_ResetFuncPeripIpc         GP_WB_STANDBY_RESET_FUNC_PERIP_IPC_LSB
#define qRegStandby_ResetFuncPeripIr          GP_WB_STANDBY_RESET_FUNC_PERIP_IR_LSB
#define qRegStandby_ResetFuncPeripKeypad      GP_WB_STANDBY_RESET_FUNC_PERIP_KEYPAD_LSB
#define qRegStandby_ResetFuncPeripLed         GP_WB_STANDBY_RESET_FUNC_PERIP_LED_LSB
#define qRegStandby_ResetFuncPeripPwmxl       GP_WB_STANDBY_RESET_FUNC_PERIP_PWMXL_LSB
#define qRegStandby_ResetFuncPeripQspiM       GP_WB_STANDBY_RESET_FUNC_PERIP_QSPI_M_LSB
#define qRegStandby_ResetFuncPeripRtz0        GP_WB_STANDBY_RESET_FUNC_PERIP_RTZ_0_LSB
#define qRegStandby_ResetFuncPeripRtz1        GP_WB_STANDBY_RESET_FUNC_PERIP_RTZ_1_LSB
#define qRegStandby_ResetFuncPeripSpiM        GP_WB_STANDBY_RESET_FUNC_PERIP_SPI_M_LSB
#define qRegStandby_ResetFuncPeripSsp         GP_WB_STANDBY_RESET_FUNC_PERIP_SSP_LSB
#define qRegStandby_ResetFuncPeripTimerxl0    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_0_LSB
#define qRegStandby_ResetFuncPeripTimerxl1    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_1_LSB
#define qRegStandby_ResetFuncPeripTimerxl2    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_2_LSB
#define qRegStandby_ResetFuncPeripTimerxl3    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_3_LSB
#define qRegStandby_ResetFuncPeripTimerxl4    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_4_LSB
#define qRegStandby_ResetFuncPeripTimerxl5    GP_WB_STANDBY_RESET_FUNC_PERIP_TIMERXL_5_LSB
#define qRegStandby_ResetFuncPeripUart0       GP_WB_STANDBY_RESET_FUNC_PERIP_UART_0_LSB

#define qRegStandby_ResetFuncPeripUart1 (GP_WB_STANDBY_RESET_FUNC_PERIP_UART_1_LSB + 32UL)
#define qRegStandby_ResetFuncPeripUart2 (GP_WB_STANDBY_RESET_FUNC_PERIP_UART_2_LSB + 32UL)
#define qRegStandby_ResetFuncPeripUsb   (GP_WB_STANDBY_RESET_FUNC_PERIP_USB_LSB + 32UL)
#define qRegStandby_ResetFuncPmudResync (GP_WB_STANDBY_RESET_FUNC_PMUD_RESYNC_LSB + 32UL)
#define qRegStandby_ResetFuncQuic       (GP_WB_STANDBY_RESET_FUNC_QUIC_LSB + 32UL)

/** @typedef qRegStandby_ResetFunc_t
 *  @brief Standby reset func.
 */
typedef UInt8 qRegStandby_ResetFunc_t;
//@}

/**
 * @brief Enable or disable the reset func for the peripheral.
 *
 * @param[in] resetFunc Standby reset func.
 * @param[in] enable    True if the reset is to be enabled, false otherwise.
 *
 * @retval Q_OK Success.
 */
qResult_t qRegStandby_ResetFuncEnableSet(qRegStandby_ResetFunc_t resetFunc, Bool enable);

/**
 * @brief Configure wakeup pin modes.
 *
 * @param[in] wakeupPinModes0 Wakeup pin modes for the first 32 pins.
 * @param[in] wakeupPinModes1 Wakeup pin modes for the next 32 pins.
 *
 * @return Q_OK Success.
 */
qResult_t qRegStandby_WakeupPinsConfigure(UInt32 wakeupPinModes0, UInt32 wakeupPinModes1);

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_STANDBY_H_ */
