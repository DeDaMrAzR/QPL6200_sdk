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

#ifndef _REG_UART_H_
#define _REG_UART_H_

#include "hal.h"
#include "qRegCommon.h"
#include "hal_SEMailbox.h" // TO BE REMOVED

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of UART peripheral. */
typedef uint32_t qRegUART_t;

/**
 * @brief Macro for getting the UART peripheral base address by ID.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_UART_BASE_ADDR_GET(ID) (qRegUART_t)(Q_CONCAT(GP_WB_UART_, ID, _BASE_ADDRESS))

/**
 * @brief Macro for getting device specific interrupt number.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_UART_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(UART_, ID, _IRQn))

/**
 * @brief Macro for getting the TX register data address.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_UART_TX_DATA_ADDRESS_GET(ID) (UInt32)(Q_CONCAT(GP_WB_UART_, ID, _TX_DATA_ADDRESS))

/**
 * @brief Macro for getting the RX register data address.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_UART_RX_DATA_ADDRESS_GET(ID) (UInt32)(Q_CONCAT(GP_WB_UART_, ID, _RX_DATA_ADDRESS))

/** @enum qRegUART_Parity_t */
//@{
#define qRegUART_ParityNone GP_WB_ENUM_PARITY_OFF
#define qRegUART_ParityOdd  GP_WB_ENUM_PARITY_ODD
#define qRegUART_ParityEven GP_WB_ENUM_PARITY_EVEN
/** @typedef qRegUART_Parity_t
 *  @brief Parity bit mode.
 */
typedef UInt8 qRegUART_Parity_t;
//@}

/** @enum qRegUART_Stopbits_t */
//@{
#define qRegUART_StopbitsOne 0
#define qRegUART_StopbitsTwo 1
/** @typedef qRegUART_Stopbits_t
 *  @brief Number of stop bits.
 */
typedef UInt8 qRegUART_Stopbits_t;
//@}

/** @enum qRegUART_Bitlength_t */
//@{
#define qRegUART_Bitlength5 4
#define qRegUART_Bitlength6 5
#define qRegUART_Bitlength7 6
#define qRegUART_Bitlength8 7
#define qRegUART_Bitlength9 8
/** @typedef qRegUART_Bitlength_t
 *  @brief Number of data bits in UART frame.
 */
typedef UInt8 qRegUART_Bitlength_t;
//@}

/** @enum qRegUART_Irq_t */
//@{
#define qRegUART_IrqTxNotBusy      GP_WB_INT_CTRL_MASK_APPUC_UART_0_TX_NOT_BUSY_INTERRUPT_LSB
#define qRegUART_IrqTxNotFull      GP_WB_INT_CTRL_MASK_APPUC_UART_0_TX_NOT_FULL_INTERRUPT_LSB
#define qRegUART_IrqRxNotEmpty     GP_WB_INT_CTRL_MASK_APPUC_UART_0_RX_NOT_EMPTY_INTERRUPT_LSB
#define qRegUART_IrqRxOverrun      GP_WB_INT_CTRL_MASK_APPUC_UART_0_RX_OVERRUN_INTERRUPT_LSB
#define qRegUART_IrqRxParityError  GP_WB_INT_CTRL_MASK_APPUC_UART_0_RX_PARITY_ERROR_INTERRUPT_LSB
#define qRegUART_IrqRxFramingError GP_WB_INT_CTRL_MASK_APPUC_UART_0_RX_FRAMING_ERROR_INTERRUPT_LSB
/** @typedef qRegUART_Irq_t
 *  @brief UART Interrupt.
 */
typedef UInt8 qRegUART_Irq_t;
//@}

/** @brief TX data FIFO. Transmission in triggered by writing this. */
static inline void qRegUART_TxDataSet(qRegUART_t baseAddr, UInt8 data)
{
    GP_WB_WRITE_UART_TX_DATA((size_t)baseAddr, data);
}

/** @brief RX data FIFO. Reading this will clear the data from the RX data FIFO. */
static inline UInt8 qRegUART_RxDataGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_RX_DATA((size_t)baseAddr);
}

/** @brief Set the UART baudrate. */
static inline void qRegUART_BaudrateSet(qRegUART_t baseAddr, UInt32 baudrate)
{
    UInt16 period = (((REG_UART_BAUDRATE_GENERATOR_CLOCK + (8 * baudrate / 2)) / (8 * baudrate)) - 1);
    GP_WB_WRITE_UART_BAUD_RATE((size_t)baseAddr, period);
}

/** @brief Read the UART baudrate. */
static inline UInt32 qRegUART_BaudrateGet(qRegUART_t baseAddr)
{
    UInt32 baudrate = GP_WB_READ_UART_BAUD_RATE((size_t)baseAddr);
    baudrate = REG_UART_BAUDRATE_GENERATOR_CLOCK / (8 * (baudrate + 1));
    return baudrate;
}

/** @brief Set the number of data bits in an UART frame. */
static inline void qRegUART_BitlengthSet(qRegUART_t baseAddr, qRegUART_Bitlength_t bitlength)
{
    GP_WB_WRITE_UART_DATA_BITS((size_t)baseAddr, bitlength);
}

/** @brief Read the currently set number of data bits in an UART frame. */
static inline qRegUART_Bitlength_t qRegUART_BitlengthGet(qRegUART_t baseAddr)
{
    return (qRegUART_Bitlength_t)GP_WB_READ_UART_DATA_BITS((size_t)baseAddr);
}

/** @brief Set the parity bit mode. */
static inline void qRegUART_ParitySet(qRegUART_t baseAddr, qRegUART_Parity_t parity)
{
    GP_WB_WRITE_UART_PARITY((size_t)baseAddr, parity);
}

/** @brief Read the parity bit mode. */
static inline qRegUART_Parity_t qRegUART_ParityGet(qRegUART_t baseAddr)
{
    return (qRegUART_Parity_t)GP_WB_READ_UART_PARITY((size_t)baseAddr);
}

/** @brief Set the number of stop bits. */
static inline void qRegUART_StopbitsSet(qRegUART_t baseAddr, qRegUART_Stopbits_t stopbits)
{
    GP_WB_WRITE_UART_STOP_BITS((size_t)baseAddr, stopbits);
}

/** @brief Read the number of stop bits. */
static inline qRegUART_Stopbits_t qRegUART_StopbitsGet(qRegUART_t baseAddr)
{
    return (qRegUART_Stopbits_t)GP_WB_READ_UART_STOP_BITS((size_t)baseAddr);
}

/** @brief In case of the HW MSI interface (stream_to_regmap = 0),
 *         interpret bit 8 as sync bit to reset internal logic.
 */
static inline void qRegUART_UseSyncBitSet(qRegUART_t baseAddr, Bool syncBit)
{
    GP_WB_WRITE_UART_USE_SYNC_BIT((size_t)baseAddr, syncBit);
}

/** @brief Read the state of MSI sync bit setting. */
static inline Bool qRegUART_UseSyncBitGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_USE_SYNC_BIT((size_t)baseAddr);
}

/** @brief Enable the UART receiver. */
static inline void qRegUART_RxEnableSet(qRegUART_t baseAddr, Bool enable)
{
    GP_WB_WRITE_UART_RX_ENABLE((size_t)baseAddr, enable);
}

/** @brief Read the enable state of UART receiver. */
static inline Bool qRegUART_RxEnableGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_RX_ENABLE((size_t)baseAddr);
}

/** @brief Enable the TX FIFO interrupts. Allows SW to stop interrupt generation
 *         from the SPI master when the UART is not needed.
 */
static inline Bool qRegUART_TxIntEnableGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_ENABLE_TX_INTERRUPTS((size_t)baseAddr);
}

/** @brief Read the enable state of uart TX interrupts. */
static inline void qRegUART_TxIntEnableSet(qRegUART_t baseAddr, Bool enable)
{
    GP_WB_WRITE_UART_ENABLE_TX_INTERRUPTS((size_t)baseAddr, enable);
}

/** @brief In case of the HW MSI interface (stream_to_regmap = 0),
 *         bit reverse the data part of the reply.
 */
static inline void qRegUART_BitReverseReplySet(qRegUART_t baseAddr, Bool enable)
{
    GP_WB_WRITE_UART_BIT_REVERSE_REPLY((size_t)baseAddr, enable);
}

/** @brief Read the state of bit reverse reply setting. */
static inline Bool qRegUART_BitReverseReplyGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_BIT_REVERSE_REPLY((size_t)baseAddr);
}

/** @brief Enable the flow control (CTS/RTS handling). */
static inline void qRegUART_HWFlowControlEnableSet(qRegUART_t baseAddr, Bool enable)
{
    GP_WB_WRITE_UART_ENABLE_HW_FLOW_CONTROL((size_t)baseAddr, enable);
}

/** @brief Read the flow control enable state. */
static inline Bool qRegUART_HWFlowControlEnableGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_ENABLE_HW_FLOW_CONTROL((size_t)baseAddr);
}

/** @brief Read the status flag for UART RX not busy.
 *         Active when no RX transfer is ongoing.
 */
static inline Bool qRegUART_RxNotBusyGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_RX_NOT_BUSY((size_t)baseAddr);
}

/** @brief Read the TX not full flag. */
static inline Bool qRegUART_IRQUnmaskedTxNotFullGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_UNMASKED_TX_NOT_FULL_INTERRUPT(baseAddr);
}

/** @brief Read the TX not busy flag. */
static inline Bool qRegUART_IRQUnmaskedTxNotBusyGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_UNMASKED_TX_NOT_BUSY_INTERRUPT(baseAddr);
}

static inline Bool qRegUART_IRQUnmaskedRxNotEmptyGet(qRegUART_t baseAddr)
{
    return GP_WB_READ_UART_UNMASKED_RX_NOT_EMPTY_INTERRUPT(baseAddr);
}

/**
 * @brief Function for clearing the RX overrun interrupt.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegUART_IRQRxOverrunErrorClear(qRegUART_t baseAddr)
{
    GP_WB_UART_CLR_RX_OVERRUN_INTERRUPT(baseAddr);
}

/**
 * @brief Function for clearing the RX parity error interrupt.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegUART_IRQRxParityErrorClear(qRegUART_t baseAddr)
{
    GP_WB_UART_CLR_RX_PARITY_ERROR_INTERRUPT(baseAddr);
}

/**
 * @brief Function for clearing the RX framing error interrupt.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegUART_IRQRxFramingErrorClear(qRegUART_t baseAddr)
{
    GP_WB_UART_CLR_RX_FRAMING_ERROR_INTERRUPT(baseAddr);
}

#ifdef __cplusplus
}
#endif

#endif //_REG_UART_H_
