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

#ifndef _Q_REG_TWIS_H_
#define _Q_REG_TWIS_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GP_WB_I2C_SL_0_BASE_ADDRESS          GP_WB_I2C_SL_BASE_ADDRESS
#define I2C_SL_0_IRQn                        I2C_SL_IRQn
#define GP_WB_SYSRAM_RETAIN_I2C_SL_0_ADDRESS GP_WB_SYSRAM_RETAIN_I2C_SL_ADDRESS
#define GP_WB_SYSRAM_RETAIN_I2C_SL_0_LSB     GP_WB_SYSRAM_RETAIN_I2C_SL_LSB

/** @brief TWIS peripheral base address type. */
typedef size_t qRegTWIS_t;

/** @brief Get the I2C_SL peripheral base address by ID. */
#define Q_REG_TWIS_BASEADDR_GET(ID) ((qRegTWIS_t)Q_CONCAT(GP_WB_I2C_SL_, ID, _BASE_ADDRESS))

/** @brief Get the I2C_SL peripheral interrupt number. */
#define Q_REG_TWIS_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(I2C_SL_, ID, _IRQn))

/** @brief Get the I2C_SL peripheral base address by ID. */
#define Q_REG_TWIS_RETAIN_GET(ID) ((qRegTWIS_t)Q_CONCAT(GP_WB_I2C_SL_, ID, _BASE_ADDRESS))

/** @brief Header of the I2C_SL interrupt handler. */
#define Q_REG_TWIS_HANDLER_DEFINE(ID) void i2c_sl_handler_impl(void)

/** @enum qRegTWIS_Irq_t */
//@{
#define qRegTWIS_IrqRxNotEmpty     GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_RX_NOT_EMPTY_INTERRUPT_LSB
#define qRegTWIS_IrqTxNotFull      GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_TX_NOT_FULL_INTERRUPT_LSB
#define qRegTWIS_IrqSlaveAddressed GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_SLAD_INTERRUPT_LSB
#define qRegTWIS_IrqStart          GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_START_INTERRUPT_LSB
#define qRegTWIS_IrqStop           GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_STOP_INTERRUPT_LSB
#define qRegTWIS_IrqTxStretch      GP_WB_INT_CTRL_MASK_APPUC_I2C_SL_TX_STRETCH_INTERRUPT_LSB
/** @typedef qRegTWIS_Irq_t
 *  @brief TWIS Event Interrupt.
 */
typedef UInt8 qRegTWIS_Irq_t;
//@}

/** @brief Write the TX data register. */
static inline void qRegTWIS_TxDataSet(qRegTWIS_t baseAddr, UInt8 txData)
{
    (void)baseAddr;
    GP_WB_WRITE_I2C_SL_TX_DATA(txData);
}

/** @brief Read the RX data register. */
static inline UInt8 qRegTWIS_RxDataGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_I2C_SL_RX_DATA();
}

/** @brief Set the slave address. */
static inline void qRegTWIS_SlaveAddressSet(qRegTWIS_t baseAddr, UInt16 address)
{
    (void)baseAddr;
    GP_WB_WRITE_I2C_SL_SLAVE_ADDRESS(address);
}

/** @brief Read the current slave address. */
static inline UInt16 qRegTWIS_SlaveAddressGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_I2C_SL_SLAVE_ADDRESS();
}

/** @brief Enable/disable the peripheral. */
static inline void qRegTWIS_EnableSet(qRegTWIS_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_I2C_SL_ENABLE(enable);
}

/** @brief Get the peripheral enable state. */
static inline Bool qRegTWIS_EnableGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_ENABLE();
}

/** @brief Enable/disable the slave clock stretching. */
static inline void qRegTWIS_SclStretchSet(qRegTWIS_t baseAddr, Bool stretch)
{
    (void)baseAddr;
    GP_WB_WRITE_I2C_SL_SCL_STRETCH_EN(stretch);
}

/** @brief Get the slave clock stretching enable state. */
static inline Bool qRegTWIS_SclStretchGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_SCL_STRETCH_EN();
}

/** @brief Enable slave selection via the general call address. */
static inline void qRegTWIS_AcceptGeneralCallSet(qRegTWIS_t baseAddr, Bool accept)
{
    (void)baseAddr;
    GP_WB_WRITE_I2C_SL_ACCEPT_GENERAL_CALL(accept);
}

/** @brief Get the general call slave selection enable state.. */
static inline Bool qRegTWIS_AcceptGeneralCallGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_ACCEPT_GENERAL_CALL();
}

/** @brief Get the READ flag. */
static inline Bool qRegTWIS_ReadGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_READ_FLAG();
}

/** @brief Get the GENERAL_CALL flag. */
static inline Bool qRegTWIS_GeneralCallGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_GENERAL_CALL_FLAG();
}

/** @brief Get the BUSY flag. */
static inline Bool qRegTWIS_BusyGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_BUSY();
}

/** @brief Get the BUSY_SLAD flag. */
static inline Bool qRegTWIS_BusySlaveAddressedGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_BUSY_SLAD();
}

/** @brief Get the RX_NOT_EMPTY interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedRxNotEmptyGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_RX_NOT_EMPTY_INTERRUPT();
}

/** @brief Get the SLAD interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedSlaveAddressedGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_SLAD_INTERRUPT();
}

/** @brief Get the START interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedStartGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_START_INTERRUPT();
}

/** @brief Get the STOP interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedStopGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_STOP_INTERRUPT();
}

/** @brief Get the TX_NOT_FULL interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedTxNotFullGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_TX_NOT_FULL_INTERRUPT();
}

/** @brief Get the TX_STRETCH_ interrupt state. */
static inline Bool qRegTWIS_IRQUnmaskedTxStretchGet(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_I2C_SL_UNMASKED_TX_STRETCH_INTERRUPT();
}

/** @brief Clear the SLAD interrupt flag. */
static inline void qRegTWIS_IRQSlaveAddressedClr(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_I2C_SL_CLR_SLAD_INTERRUPT();
}

/** @brief Clear the START interrupt flag. */
static inline void qRegTWIS_IRQStartClr(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_I2C_SL_CLR_START_INTERRUPT();
}

/** @brief Clear the STOP interrupt flag. */
static inline void qRegTWIS_IRQStopClr(qRegTWIS_t baseAddr)
{
    (void)baseAddr;
    GP_WB_I2C_SL_CLR_STOP_INTERRUPT();
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_TWIS_H_ */
