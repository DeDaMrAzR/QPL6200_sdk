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

#ifndef _Q_REG_TWIM_H_
#define _Q_REG_TWIM_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief TWIM peripheral base address type. */
typedef size_t qRegTWIM_t;

/** @brief Get the I2C_M peripheral base address by ID. */
#define Q_REG_TWIM_BASEADDR_GET(ID) ((qRegTWIM_t)Q_CONCAT(GP_WB_I2C_M_, ID, _BASE_ADDRESS))

/** @brief Get the I2C_M peripheral interrupt number. */
#define Q_REG_TWIM_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(I2C_M_, ID, _IRQn))

/** @brief Header of the I2C_M interrupt handler. */
#define Q_REG_TWIM_HANDLER_DEFINE(ID) void Q_CONCAT(i2c_m_, ID, _handler_impl)(void)

/** @enum qRegTWIM_Irq_t */
//@{
#define qRegTWIM_IrqArbLost GP_WB_INT_CTRL_MASK_APPUC_I2C_M_0_ARB_LOST_INTERRUPT_LSB
#define qRegTWIM_IrqClkSync GP_WB_INT_CTRL_MASK_APPUC_I2C_M_0_CLK_SYNC_INTERRUPT_LSB
#define qRegTWIM_IrqDone    GP_WB_INT_CTRL_MASK_APPUC_I2C_M_0_DONE_INTERRUPT_LSB
/** @typedef qRegTWIM_Irq_t
 *  @brief TWIM Event Interrupt.
 */
typedef UInt8 qRegTWIM_Irq_t;
//@}

/** @enum qRegTWIM_Control_t */
//@{
#define qRegTWIM_ControlWrite GP_WB_I2C_M_WRITE_LSB
#define qRegTWIM_ControlRead  GP_WB_I2C_M_READ_LSB
#define qRegTWIM_ControlStop  GP_WB_I2C_M_STOP_LSB
#define qRegTWIM_ControlStart GP_WB_I2C_M_START_LSB
/** @typedef qRegTWIM_Control_t
 *  @brief TWIM control flag.
 */
typedef UInt8 qRegTWIM_Control_t;
//@}

/** @brief Write the prescaler register. */
static inline void qRegTWIM_PrescalerSet(qRegTWIM_t baseAddr, UInt16 prescaler)
{
    GP_WB_WRITE_I2C_M_PRESCALER(baseAddr, prescaler);
}

/** @brief Read the prescaler register. */
static inline UInt16 qRegTWIM_PrescalerGet(qRegTWIM_t baseAddr)
{
    return GP_WB_READ_I2C_M_PRESCALER(baseAddr);
}

/** @brief Enable/disable the peripheral. */
static inline void qRegTWIM_EnableSet(qRegTWIM_t baseAddr, Bool enable)
{
    GP_WB_WRITE_I2C_M_ENABLE(baseAddr, enable);
}

/** @brief Get the peripheral enable state. */
static inline Bool qRegTWIM_EnableGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_ENABLE(baseAddr);
}

/** @brief Enable/disable the ACK flag transmission. */
static inline void qRegTWIM_AckSet(qRegTWIM_t baseAddr, Bool ack)
{
    GP_WB_WRITE_I2C_M_ACK(baseAddr, ack);
}

/** @brief Get the ACK transmission enable state. */
static inline Bool qRegTWIM_AckGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_ACK(baseAddr);
}

/** @brief Disable the clock synchronization. */
static inline void qRegTWIM_ClkSyncDisableSet(qRegTWIM_t baseAddr, Bool syncDisable)
{
    GP_WB_WRITE_I2C_M_CLK_SYNC_DISABLE(baseAddr, syncDisable);
}

/** @brief Get the clock synchronization disable flag state. */
static inline Bool qRegTWIM_ClkSyncDisableGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_CLK_SYNC_DISABLE(baseAddr);
}

/** @brief Write the TX data register. */
static inline void qRegTWIM_TxDataSet(qRegTWIM_t baseAddr, UInt8 txData)
{
    GP_WB_WRITE_I2C_M_TX_DATA(baseAddr, txData);
}

/** @brief Read the TX data register. */
static inline UInt8 qRegTWIM_TxDataGet(qRegTWIM_t baseAddr)
{
    return GP_WB_READ_I2C_M_TX_DATA(baseAddr);
}

/** @brief Read the RX data register. */
static inline UInt8 qRegTWIM_RxDataGet(qRegTWIM_t baseAddr)
{
    return GP_WB_READ_I2C_M_RX_DATA(baseAddr);
}

/** @brief Write the control register. */
static inline void qRegTWIM_ControlSet(qRegTWIM_t baseAddr, UInt8 controlFlags)
{
    GP_WB_WRITE_I2C_M_CONTROL(baseAddr, controlFlags);
}

/** @brief Set the WRITE control flag. */
static inline void qRegTWIM_WriteSet(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_WRITE(baseAddr);
}

/** @brief Set the READ control flag. */
static inline void qRegTWIM_ReadSet(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_READ(baseAddr);
}

/** @brief Set the STOP control flag. */
static inline void qRegTWIM_StopSet(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_STOP(baseAddr);
}

/** @brief Set the START control flag. */
static inline void qRegTWIM_StartSet(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_START(baseAddr);
}

/** @brief Get the read/write busy control flag. */
static inline Bool qRegTWIM_RWBusyGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_READ_WRITE_BUSY(baseAddr);
}

/** @brief Get the transfer busy control flag. */
static inline Bool qRegTWIM_TransferBusyGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_TRANSFER_BUSY(baseAddr);
}

/** @brief Get the RX ACK control flag. */
static inline Bool qRegTWIM_RXAckGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_RX_ACK(baseAddr);
}

/** @brief Get the clock synchronization control flag. */
static inline Bool qRegTWIM_ClkSyncGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_CLK_SYNC(baseAddr);
}

/** @brief Clear the ARB_LOST interrupt flag. */
static inline void qRegTWIM_IRQArbLostClr(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_CLR_ARB_LOST_INTERRUPT(baseAddr);
}

/** @brief Clear the CLK_SYNC interrupt flag. */
static inline void qRegTWIM_IRQClkSyncClr(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_CLR_CLK_SYNC_INTERRUPT(baseAddr);
}

/** @brief Clear the DONE interrupt flag. */
static inline void qRegTWIM_IRQDoneClr(qRegTWIM_t baseAddr)
{
    GP_WB_I2C_M_CLR_DONE_INTERRUPT(baseAddr);
}

/** @brief Get the ARB_LOST interrupt state. */
static inline Bool qRegTWIM_IRQUnmaskedArbLostGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_UNMASKED_ARB_LOST_INTERRUPT(baseAddr);
}

/** @brief Get the CLK_SYNC interrupt state. */
static inline Bool qRegTWIM_IRQUnmaskedClkSyncGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_UNMASKED_CLK_SYNC_INTERRUPT(baseAddr);
}

/** @brief Get the DONE interrupt state. */
static inline Bool qRegTWIM_IRQUnmaskedDoneGet(qRegTWIM_t baseAddr)
{
    return (Bool)GP_WB_READ_I2C_M_UNMASKED_DONE_INTERRUPT(baseAddr);
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_TWIM_H_ */
