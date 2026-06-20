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

#ifndef _Q_REG_SPIS_H_
#define _Q_REG_SPIS_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief SPIS peripheral base address type. */
typedef size_t qRegSPIS_t;

#define GP_WB_SPI_SL_0_BASE_ADDRESS    GP_WB_SPI_SL_BASE_ADDRESS
#define SPI_SL_0_IRQn                  SPI_SL_IRQn
#define GP_WB_SPI_SL_0_RX_DATA_ADDRESS GP_WB_SPI_SL_RX_DATA_ADDRESS
#define GP_WB_SPI_SL_0_TX_DATA_ADDRESS GP_WB_SPI_SL_TX_DATA_ADDRESS

/** @brief Get the SPIS peripheral base address by ID. */
#define Q_REG_SPIS_BASEADDR_GET(ID) ((qRegSPIS_t)Q_CONCAT(GP_WB_SPI_SL_, ID, _BASE_ADDRESS))

/** @brief Get the SPIS peripheral interrupt number. */
#define Q_REG_SPIS_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(SPI_SL, _IRQn))

/** @brief Get the SPIS TX data register address. */
#define Q_REG_SPIS_TXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_SPI_SL_, ID, _TX_DATA_ADDRESS))

/** @brief Get the SPIS RX data register address. */
#define Q_REG_SPIS_RXADDR_GET(ID) (UInt32)(Q_CONCAT(GP_WB_SPI_SL_, ID, _RX_DATA_ADDRESS))

/** @brief Header of the SPIS interrupt handler. */
#define Q_REG_SPIS_HANDLER_DEFINE(ID) void spi_sl_handler(void)

/** @enum qRegSPIS_Irq_t */
//@{
#define qRegSPIS_IrqRxNotEmpty GP_WB_INT_CTRL_MASK_APPUC_SPI_SL_RX_NOT_EMPTY_INTERRUPT_LSB
#define qRegSPIS_IrqRxOverrun  GP_WB_INT_CTRL_MASK_APPUC_SPI_SL_RX_OVERRUN_INTERRUPT_LSB
#define qRegSPIS_IrqTxEmpty    GP_WB_INT_CTRL_MASK_APPUC_SPI_SL_TX_EMPTY_INTERRUPT_LSB
#define qRegSPIS_IrqTxNotFull  GP_WB_INT_CTRL_MASK_APPUC_SPI_SL_TX_NOT_FULL_INTERRUPT_LSB
#define qRegSPIS_IrqTxUnderrun GP_WB_INT_CTRL_MASK_APPUC_SPI_SL_TX_UNDERRUN_INTERRUPT_LSB
/** @typedef qRegSPIS_Irq_t
 *  @brief SPIS Event Interrupt.
 */
typedef UInt8 qRegSPIS_Irq_t;
//@}

/** @brief Write the TX data register. */
static inline void qRegSPIS_TxDataSet(qRegSPIS_t baseAddr, UInt8 txData)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_TX_DATA(txData);
}

/** @brief Read the RX data register. */
static inline UInt8 qRegSPIS_RxDataGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_SPI_SL_RX_DATA();
}

/** @brief Set the SPI enable flag. */
static inline void qRegSPIS_EnableSet(qRegSPIS_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_ENABLE(enable);
}

/** @brief Get the SPI enable flag state. */
static inline Bool qRegSPIS_EnableGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_ENABLE();
}

/** @brief Enable/disable clearing of TX FIFO on deselect. */
static inline void qRegSPIS_ClrTxOnDeselectSet(qRegSPIS_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_CLR_TX_FIFO_ON_DESELECT(enable);
}

/** @brief Get the state of clearing of TX FIFO on deselect. */
static inline Bool qRegSPIS_ClrTxOnDeselectGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_CLR_TX_FIFO_ON_DESELECT();
}

/** @brief Enable/disable clearing of RX FIFO on deselect. */
static inline void qRegSPIS_ClrRxOnDeselectSet(qRegSPIS_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_CLR_RX_FIFO_ON_DESELECT(enable);
}

/** @brief Get the state of clearing of RX FIFO on deselect. */
static inline Bool qRegSPIS_ClrRxOnDeselectGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_CLR_RX_FIFO_ON_DESELECT();
}

/** @brief Enable/disable half-duplex mode. */
static inline void qRegSPIS_HalfDuplexSet(qRegSPIS_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_HALF_DUPLEX(enable);
}

/** @brief Get the half-duplex mode state. */
static inline Bool qRegSPIS_HalfDuplexGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_HALF_DUPLEX();
}

/** @brief Set SPIS TX latency. */
static inline void qRegSPIS_TxLatencySet(qRegSPIS_t baseAddr, UInt8 latency)
{
    (void)baseAddr;
    GP_WB_WRITE_SPI_SL_TX_LATENCY(latency);
}

/** @brief Read the current SPI TX latency setting. */
static inline UInt8 qRegSPIS_TxLatencyGet(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (UInt8)GP_WB_READ_SPI_SL_TX_LATENCY();
}

/** @brief Clear SPIS TX FIFO. */
static inline void qRegSPIS_TxFifoClr(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_SPI_SL_CLR_TX_FIFO();
}

/** @brief Clear SPIS RX FIFO. */
static inline void qRegSPIS_RxFifoClr(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_SPI_SL_CLR_RX_FIFO();
}

/** @brief Get the RX_NOT_EMPTY interrupt state. */
static inline Bool qRegSPIS_IRQUnmaskedRxNotEmpty(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_UNMASKED_RX_NOT_EMPTY_INTERRUPT();
}

/** @brief Get the RX_OVERRUN interrupt state. */
static inline Bool qRegSPIS_IRQUnmaskedRxOverrun(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_UNMASKED_RX_OVERRUN_INTERRUPT();
}

/** @brief Get the TX_EMPTY interrupt state. */
static inline Bool qRegSPIS_IRQUnmaskedTxEmpty(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_UNMASKED_TX_EMPTY_INTERRUPT();
}
/** @brief Get the TX_NOT_FULL interrupt state. */
static inline Bool qRegSPIS_IRQUnmaskedTxNotFull(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_UNMASKED_TX_NOT_FULL_INTERRUPT();
}

/** @brief Get the TX_UNDERRUN interrupt state. */
static inline Bool qRegSPIS_IRQUnmaskedTxUnderrun(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_SPI_SL_UNMASKED_TX_UNDERRUN_INTERRUPT();
}

/** @brief Clear the RX_OVERRUN interrupt flag. */
static inline void qRegSPIS_IRQRxOverrunClr(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_SPI_SL_CLR_RX_OVERRUN_INTERRUPT();
}

/** @brief Clear the TX_UNDERRUN interrupt flag. */
static inline void qRegSPIS_IRQTxUnderrunClr(qRegSPIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_SPI_SL_CLR_TX_UNDERRUN_INTERRUPT();
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_SPIS_ */
