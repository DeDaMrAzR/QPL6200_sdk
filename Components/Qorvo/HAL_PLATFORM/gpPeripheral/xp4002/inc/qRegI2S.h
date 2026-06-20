/*
 * Copyright (c) 2023, Qorvo Inc
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

#ifndef _Q_REG_I2S_H_
#define _Q_REG_I2S_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief I2S peripheral base address type. */
typedef I2S_0_REGMAP_T* qRegI2S_t;

/** @brief Get the I2S peripheral base address by ID. */
#define Q_REG_I2S_BASEADDR_GET(ID) ((qRegI2S_t)Q_CONCAT(GP_WB_I2S_, ID, _BASE_ADDRESS))

/** @brief Get the I2S peripheral interrupt number. */
#define Q_REG_I2S_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(I2S_, ID, _IRQn))

/** @brief Get the I2S TX data register address. */
#define Q_REG_I2S_TXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_I2S_, ID, _TX_DATA_ADDRESS))

/** @brief Get the I2S RX data register address. */
#define Q_REG_I2S_RXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_I2S_, ID, _RX_DATA_ADDRESS))

#define Q_REG_I2S_TRIGGER_TX_NOT_FULL_GET(ID)                                                                          \
    (UInt32)(Q_CONCAT(GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_, ID, _TX_NOT_FULL))

#define Q_REG_I2S_TRIGGER_RX_NOT_EMPTY_GET(ID)                                                                         \
    (UInt32)(Q_CONCAT(GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_, ID, _RX_NOT_EMPTY))

/** @enum qRegI2S_ClkSel_t */
//@{
#define qRegI2S_ClkSelMain   GP_WB_ENUM_I2S_CLK_SEL_MAIN_CLK
#define qRegI2S_ClkSelFract0 GP_WB_ENUM_I2S_CLK_SEL_FRACT_CLK_CLKGEN_0
#define qRegI2S_ClkSelFract1 GP_WB_ENUM_I2S_CLK_SEL_FRACT_CLK_CLKGEN_1
#define qRegI2S_ClkSelIob    GP_WB_ENUM_I2S_CLK_SEL_IOB_CLK
/** @typedef qRegI2S_ClkSel_t
 *  @brief I2S clock frequency.
 */
typedef UInt8 qRegI2S_ClkSel_t;
//@}

/** @enum qRegI2S_Irq_t */
//@{
#define qRegI2S_IrqRxNotEmpty GP_WB_INT_CTRL_MASK_APPUC_I2S_0_RX_NOT_EMPTY_INTERRUPT_LSB
#define qRegI2S_IrqTxNotFull  GP_WB_INT_CTRL_MASK_APPUC_I2S_0_TX_NOT_FULL_INTERRUPT_LSB
/** @typedef qRegI2S_Irq_t
 *  @brief I2S Interrupt.
 */
typedef UInt8 qRegI2S_Irq_t;
//@}

/** @brief Write the TX data register. */
static inline void qRegI2S_TxDataSet(qRegI2S_t baseAddr, UInt8 txData)
{
    baseAddr->tx_data.reg8[0] = txData;
}

/** @brief Read the RX data register. */
static inline UInt8 qRegI2S_RxDataGet(qRegI2S_t baseAddr)
{
    return baseAddr->rx_data.reg8[0];
}

/** @brief Set the I2S clock selection. */
static inline void qRegI2S_ClkSelSet(qRegI2S_t baseAddr, qRegI2S_ClkSel_t clkSel)
{
    baseAddr->select_clk.reg8[0] = clkSel;
}

/** @brief Get the I2S clock selection. */
static inline qRegI2S_ClkSel_t qRegI2S_ClkSelGet(qRegI2S_t baseAddr)
{
    return (qRegI2S_ClkSel_t)baseAddr->select_clk.reg8[0];
}

/** @brief Set the I2S enable mode. */
static inline void qRegI2S_EnableSet(qRegI2S_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->config_0.reg32[0] |= GP_WB_I2S_0_ENABLE_MASK;
    }
    else
    {
        baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_ENABLE_MASK;
    }
}

/** @brief Get the I2S enable mode. */
static inline Bool qRegI2S_EnableGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_ENABLE_MASK) ? true : false;
}

/** @brief Enable or disable TX interrupts. */
static inline void qRegI2S_TxIrqEnableSet(qRegI2S_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->config_0.reg32[0] |= GP_WB_I2S_1_ENABLE_TX_INTERRUPTS_MASK;
    }
    else
    {
        baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_1_ENABLE_TX_INTERRUPTS_MASK;
    }
}

/** @brief Get the TX interrupt enable status. */
static inline Bool qRegI2S_TxIrqEnableGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_1_ENABLE_TX_INTERRUPTS_MASK) ? true : false;
}

/** @brief Enable or disable slave mode. */
static inline void qRegI2S_SlaveModeEnableSet(qRegI2S_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->config_0.reg32[0] |= GP_WB_I2S_1_SLAVE_MODE_MASK;
    }
    else
    {
        baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_1_SLAVE_MODE_MASK;
    }
}

/** @brief Get the slave mode enable status. */
static inline Bool qRegI2S_SlaveModeEnableGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_1_SLAVE_MODE_MASK) ? true : false;
}

/** @brief Set offset in SCK cycles between the WS toggle and the first data bit. For real I2S mode, this should be 1.
 */
static inline void qRegI2S_WsOffsetSet(qRegI2S_t baseAddr, UInt8 wsOffset)
{
    baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_WS_OFFSET_MASK;
    baseAddr->config_0.reg32[0] |= (wsOffset << GP_WB_I2S_0_WS_OFFSET_LSB) & GP_WB_I2S_0_WS_OFFSET_MASK;
}

/** @brief Get the WS offset. */
static inline UInt8 qRegI2S_WsOffsetGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_WS_OFFSET_MASK) >> GP_WB_I2S_0_WS_OFFSET_LSB;
}

/** @brief Set the SCK bit clock frequency prescaler: processing_clk_freq / 2*(sck_freq + 1). */
static inline void qRegI2S_PrescalerSet(qRegI2S_t baseAddr, UInt8 prescaler)
{
    baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_SCK_FREQ_MASK;
    baseAddr->config_0.reg32[0] |= (prescaler << GP_WB_I2S_0_SCK_FREQ_LSB) & GP_WB_I2S_0_SCK_FREQ_MASK;
}

/** @brief Get the SCK bit clock frequency prescaler. */
static inline UInt8 qRegI2S_PrescalerGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_SCK_FREQ_MASK) >> GP_WB_I2S_0_SCK_FREQ_LSB;
}

/** @brief Set the word length. */
static inline void qRegI2S_WordLenSet(qRegI2S_t baseAddr, UInt8 wordLen)
{
    baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_WORD_LEN_MASK;
    baseAddr->config_0.reg32[0] |= (wordLen << GP_WB_I2S_0_WORD_LEN_LSB) & GP_WB_I2S_0_WORD_LEN_MASK;
}

/** @brief Get the word length. */
static inline UInt8 qRegI2S_WordLenGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_WORD_LEN_MASK) >> GP_WB_I2S_0_WORD_LEN_LSB;
}

/** @brief Set number of data bytes to be transmitted on SDO for the left channel (WS=0). */
static inline void qRegI2S_TxBytesLeftSet(qRegI2S_t baseAddr, UInt8 txBytesLeft)
{
    baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_TX_BYTES_LEFT_MASK;
    baseAddr->config_0.reg32[0] |= (txBytesLeft << GP_WB_I2S_0_TX_BYTES_LEFT_LSB) & GP_WB_I2S_0_TX_BYTES_LEFT_MASK;
}

/** @brief Get the number of data bytes to be transmitted on SDO for the left channel (WS=0). */
static inline UInt8 qRegI2S_TxBytesLeftGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_TX_BYTES_LEFT_MASK) >> GP_WB_I2S_0_TX_BYTES_LEFT_LSB;
}

/** @brief Set number of data bytes to be transmitted on SDO for the right channel (WS=1). */
static inline void qRegI2S_TxBytesRightSet(qRegI2S_t baseAddr, UInt8 txBytesRight)
{
    baseAddr->config_0.reg32[0] &= ~GP_WB_I2S_0_TX_BYTES_RIGHT_MASK;
    baseAddr->config_0.reg32[0] |= (txBytesRight << GP_WB_I2S_0_TX_BYTES_RIGHT_LSB) & GP_WB_I2S_0_TX_BYTES_RIGHT_MASK;
}

/** @brief Get the number of data bytes to be transmitted on SDO for the right channel (WS=1). */
static inline UInt8 qRegI2S_TxBytesRightGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_TX_BYTES_RIGHT_MASK) >> GP_WB_I2S_0_TX_BYTES_RIGHT_LSB;
}

/** @brief Set number of data bytes to be received on SDI for the left channel (WS=0). */
static inline void qRegI2S_RxBytesLeftSet(qRegI2S_t baseAddr, UInt8 rxBytesLeft)
{
    baseAddr->config_1.reg32[0] &= ~GP_WB_I2S_0_RX_BYTES_LEFT_MASK;
    baseAddr->config_1.reg32[0] |= (rxBytesLeft << GP_WB_I2S_0_RX_BYTES_LEFT_LSB) & GP_WB_I2S_0_RX_BYTES_LEFT_MASK;
}

/** @brief Get the number of data bytes to be received on SDI for the left channel (WS=0). */
static inline UInt8 qRegI2S_RxBytesLeftGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_0.reg32[0] & GP_WB_I2S_0_RX_BYTES_LEFT_MASK) >> GP_WB_I2S_0_RX_BYTES_LEFT_LSB;
}

/** @brief Set number of data bytes to be received on SDI for the right channel (WS=1). */
static inline void qRegI2S_RxBytesRightSet(qRegI2S_t baseAddr, UInt8 rxBytesRight)
{
    baseAddr->config_1.reg32[0] &= ~GP_WB_I2S_0_RX_BYTES_RIGHT_MASK;
    baseAddr->config_1.reg32[0] |= (rxBytesRight << GP_WB_I2S_0_RX_BYTES_RIGHT_LSB) & GP_WB_I2S_0_RX_BYTES_RIGHT_MASK;
}

/** @brief Get the number of data bytes to be received on SDI for the right channel (WS=1). */
static inline UInt8 qRegI2S_RxBytesRightGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_1.reg32[0] & GP_WB_I2S_0_RX_BYTES_RIGHT_MASK) >> GP_WB_I2S_0_RX_BYTES_RIGHT_LSB;
}

/** @brief Enable or disable prescaler. */
static inline void qRegI2S_PrescalerEnableSet(qRegI2S_t baseAddr, Bool enable)
{
    if(enable)
    {
        baseAddr->config_1.reg32[0] |= GP_WB_I2S_0_ENABLE_PRESCALER_MASK;
    }
    else
    {
        baseAddr->config_1.reg32[0] &= ~GP_WB_I2S_0_ENABLE_PRESCALER_MASK;
    }
}

/** @brief Get the prescaler enable status. */
static inline Bool qRegI2S_PrescalerEnableGet(qRegI2S_t baseAddr)
{
    return (baseAddr->config_1.reg32[0] & GP_WB_I2S_0_ENABLE_PRESCALER_MASK) ? true : false;
}

/** @brief Get RX not empty interrupt status. */
static inline Bool qRegI2S_IRQUnmaskedRxNotEmptyGet(qRegI2S_t baseAddr)
{
    return baseAddr->unmasked_interrupts.reg32[0] & GP_WB_I2S_0_UNMASKED_RX_NOT_EMPTY_INTERRUPT_MASK ? true : false;
}

/** @brief Get TX not full interrupt status. */
static inline Bool qRegI2S_IRQUnmaskedTxNotFullGet(qRegI2S_t baseAddr)
{
    return baseAddr->unmasked_interrupts.reg32[0] & GP_WB_I2S_0_UNMASKED_TX_NOT_FULL_INTERRUPT_MASK ? true : false;
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_I2S_H_ */
