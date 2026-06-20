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

#ifndef _Q_REG_SPIM_H_
#define _Q_REG_SPIM_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SPIM peripheral base address type. */
typedef size_t qRegSPIM_t;

#define GP_WB_SPI_M_0_BASE_ADDRESS    GP_WB_SPI_M_BASE_ADDRESS
#define SPI_M_0_IRQn                  SPI_M_IRQn
#define GP_WB_SPI_M_0_RX_DATA_ADDRESS GP_WB_SPI_M_RX_DATA_ADDRESS
#define GP_WB_SPI_M_0_TX_DATA_ADDRESS GP_WB_SPI_M_TX_DATA_ADDRESS

/** @brief Get the SPIM peripheral base address by ID. */
#define Q_REG_SPIM_BASEADDR_GET(ID) ((qRegSPIM_t)Q_CONCAT(GP_WB_SPI_M_, ID, _BASE_ADDRESS))

/** @brief Get the SPI_M peripheral interrupt number. */
#define Q_REG_SPIM_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(SPI_M, _IRQn))

/** @brief Get the SPI_M TX data register address. */
#define Q_REG_SPIM_TXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_SPI_M_, ID, _TX_DATA_ADDRESS))

/** @brief Get the SPI_M RX data register address. */
#define Q_REG_SPIM_RXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_SPI_M_, ID, _RX_DATA_ADDRESS))

/** @brief Header of the SPIM interrupt handler. */
#define Q_REG_SPIM_HANDLER_DEFINE(ID) void spi_m_handler(void)

/** @enum qRegSPIM_Mode_t */
//@{
#define qRegSPIM_Mode0 0 /* CPOL = 0, CPHA = 0 */
#define qRegSPIM_Mode1 1 /* CPOL = 0, CPHA = 1 */
#define qRegSPIM_Mode2 2 /* CPOL = 1, CPHA = 0 */
#define qRegSPIM_Mode3 3 /* CPOL = 1, CPHA = 1 */
/** @typedef qRegSPIM_Mode_t
 *  @brief SPI mode - clock polarity and
 */
typedef UInt8 qRegSPIM_Mode_t;
//@}

/** @enum qRegSPIM_ClkFreq_t */
//@{
#define qRegSPIM_ClkFreq32M  0
#define qRegSPIM_ClkFreq16M  1
#define qRegSPIM_ClkFreq8M   2
#define qRegSPIM_ClkFreq4M   3
#define qRegSPIM_ClkFreq2M   4
#define qRegSPIM_ClkFreq1M   5
#define qRegSPIM_ClkFreq500k 6
#define qRegSPIM_ClkFreq250k 7
/** @typedef qRegSPIM_ClkFreq_t
 *  @brief SPI master clock frequency.
 */
typedef UInt8 qRegSPIM_ClkFreq_t;
//@}

/** @enum qRegSPIM_DataBits_t */
//@{
#define qRegSPIM_DataBits8  7
#define qRegSPIM_DataBits16 15
#define qRegSPIM_DataBits32 31
/** @typedef qRegSPIM_DataBits_t
 *  @brief SPI frame size.
 */
typedef UInt8 qRegSPIM_DataBits_t;
//@}

/** @enum qRegSPIM_Irq_t */
//@{
#define qRegSPIM_IrqNotBusy    GP_WB_INT_CTRL_MASK_APPUC_SPI_M_NOT_BUSY_INTERRUPT_LSB
#define qRegSPIM_IrqRxNotEmpty GP_WB_INT_CTRL_MASK_APPUC_SPI_M_RX_NOT_EMPTY_INTERRUPT_LSB
#define qRegSPIM_IrqRxOverrun  GP_WB_INT_CTRL_MASK_APPUC_SPI_M_RX_OVERRUN_INTERRUPT_LSB
#define qRegSPIM_IrqTxNotFull  GP_WB_INT_CTRL_MASK_APPUC_SPI_M_TX_NOT_FULL_INTERRUPT_LSB
/** @typedef qRegSPIM_Irq_t
 *  @brief SPIM Event Interrupt.
 */
typedef UInt8 qRegSPIM_Irq_t;
//@}

/** @brief Write the TX data register. */
static inline void qRegSPIM_TxDataSet(qRegSPIM_t baseAddr, UInt32 txData)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_TX_DATA(txData);
}

/** @brief Read the RX data register. */
static inline UInt32 qRegSPIM_RxDataGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_SPI_M_RX_DATA();
}

/** @brief Set the SPI frame size. */
static inline void qRegSPIM_DataBitsSet(qRegSPIM_t baseAddr, qRegSPIM_DataBits_t dataBits)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_DATA_BITS(dataBits);
}

/** @brief Get the SPI frame size. */
static inline qRegSPIM_DataBits_t qRegSPIM_DataBitsGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (qRegSPIM_DataBits_t)GP_WB_READ_SPI_M_DATA_BITS();
}

/** @brief Set the SPI mode of operation. */
static inline void qRegSPIM_ModeSet(qRegSPIM_t baseAddr, qRegSPIM_Mode_t mode)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_MODE(mode);
}

/** @brief Get the SPI mode of operation. */
static inline qRegSPIM_Mode_t qRegSPIM_ModeGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (qRegSPIM_Mode_t)GP_WB_READ_SPI_M_MODE();
}

/** @brief Set the SPI data bit order. */
static inline void qRegSPIM_LsbFirstSet(qRegSPIM_t baseAddr, Bool lsbFirst)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_LSB_FIRST(lsbFirst);
}

/** @brief Get the SPI data bit order. */
static inline Bool qRegSPIM_LsbFirstGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_LSB_FIRST();
}

/** @brief Set the SPI master clock frequency. */
static inline void qRegSPIM_ClkFreqSet(qRegSPIM_t baseAddr, qRegSPIM_ClkFreq_t freq)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_SCLK_FREQ(freq);
}

/** @brief Get the SPI master clock frequency. */
static inline qRegSPIM_ClkFreq_t qRegSPIM_ClkFreqGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (qRegSPIM_ClkFreq_t)GP_WB_READ_SPI_M_SCLK_FREQ();
}

/** @brief Enable/disable the free-running mode. */
static inline void qRegSPIM_FreeRunningSet(qRegSPIM_t baseAddr, Bool freeRunning)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_FREE_RUNNING(freeRunning);
}

/** @brief Get the free-running mode state. */
static inline Bool qRegSPIM_FreeRunningGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_FREE_RUNNING();
}

/** @brief Enable/disable transfer stalling when RX FIFO is full. */
static inline void qRegSPIM_StallOnRxFullSet(qRegSPIM_t baseAddr, Bool stall)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_STALL_ON_RX_FULL(stall);
}

/** @brief Get the state of transfer stalling feature. */
static inline Bool qRegSPIM_StallOnRxFullGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_STALL_ON_RX_FULL();
}

/** @brief Enable/disable TX FIFO interrupt generation. */
static inline void qRegSPIM_IRQTxEnableSet(qRegSPIM_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_M_ENABLE_TX_INTERRUPTS(enable);
}

/** @brief Get the enable state of TX FIFO interrupt generation. */
static inline Bool qRegSPIM_IRQTxEnableGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_ENABLE_TX_INTERRUPTS();
}

/** @brief Get the NOT_BUSY interrupt state. */
static inline Bool qRegSPIM_IRQUnmaskedNotBusyGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_UNMASKED_NOT_BUSY_INTERRUPT();
}

/** @brief Get the RX_NOT_EMPTY interrupt state. */
static inline Bool qRegSPIM_IRQUnmaskedRxNotEmptyGet(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_UNMASKED_RX_NOT_EMPTY_INTERRUPT();
}

/** @brief Get the RX_OVERRUN interrupt state. */
static inline Bool qRegSPIM_IRQUnmaskedRxOverrun(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_UNMASKED_RX_OVERRUN_INTERRUPT();
}

/** @brief Get the TX_NOT_FULL interrupt state. */
static inline Bool qRegSPIM_IRQUnmaskedTxNotFull(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_M_UNMASKED_TX_NOT_FULL_INTERRUPT();
}

/** @brief Clear the RX_OVERRUN interrupt flag. */
static inline void qRegSPIM_IRQRxOverrunClr(qRegSPIM_t baseAddr)
{
    (void)baseAddr;
    GP_WB_SPI_M_CLR_RX_OVERRUN_INTERRUPT();
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_SPIM_H_ */
