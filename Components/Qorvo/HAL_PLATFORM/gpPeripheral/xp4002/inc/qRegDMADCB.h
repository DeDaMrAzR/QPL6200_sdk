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

#ifndef _REG_DMADCB_H_
#define _REG_DMADCB_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Calculate the address offset between DMA DCB channels. */
#define Q_REG_DMADCB_CHANNEL_OFFSET 0
/** @brief Calculate the DMA DCB channel base address. */
#define Q_REG_DMADCB_CHANNEL_BASEADDR(CHANNEL) (GP_WB_DMAS_DCB_BASE_ADDRESS + Q_REG_DMADCB_CHANNEL_OFFSET * (CHANNEL))

/** @brief Calculate the DMA DCB QUIC action slot offset. */
#define Q_REG_DMADCB_QUIC_ACTION_OFFSET                                                                                \
    (GP_WB_DMAS_DCB_QUIC_ACTION_1_CHANNEL_0_ADDRESS - GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_ADDRESS)

/** @brief DMA channel interrupt mask. */
#define Q_REG_DMADCB_CHANNEL_IRQ_MASK (~(0xFFFFFFFFuL << (GP_WB_INT_CTRL_MASK_APPUC_DMA_DCB_CPY_ERR_INTERRUPT_LSB)))

/** @brief Header of the DMA_DCB interrupt handler. */
#define Q_REG_DMADCB_HANDLER_DEFINE() void dma_dcb_handler_impl(void)

/** @enum qRegDMADCB_WordMode_t */
//@{
/** @brief 8-bit word. */
#define qRegDMADCB_WordMode8Bit GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA8BIT
/** @brief 16-bit word. */
#define qRegDMADCB_WordMode16Bit GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA16BIT
/** @brief 32-bit word. */
#define qRegDMADCB_WordMode32Bit GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA32BIT
/** @typedef qRegDMADCB_WordMode_t
 *  @brief DMA word size. 8-, 16- or 32-bit memory accesses.
 */
typedef UInt8 qRegDMADCB_WordMode_t;
//@}

/** @enum qRegDMADCB_QuicAction_t */
//@{
/** @brief No action. */
#define qRegDMADCB_QuicActionNone GP_WB_ENUM_DMA_DCB_ACTION_NONE
/** @brief Reset source pointer. */
#define qRegDMADCB_QuicActionResetSrc GP_WB_ENUM_DMA_DCB_ACTION_RESET_SRC_PTR
/** @brief Reset destination pointer. */
#define qRegDMADCB_QuicActionResetDest GP_WB_ENUM_DMA_DCB_ACTION_RESET_DEST_PTR
/** @brief Reset source and destination pointer. */
#define qRegDMADCB_QuicActionResetBoth GP_WB_ENUM_DMA_DCB_ACTION_RESET_BOTH_PTR
/** @typedef qRegDMADCB_QuicAction_t
 *  @brief Action which should happen to the DMA DCB channel when the QUIC trigger arrives.
 */
typedef UInt8 qRegDMADCB_QuicAction_t;
//@}

/** @enum qRegDMADCB_Irq_t */
//@{
/** @brief Triggered when a memory access error occurred during a copy action
 *         (could be as a result of wrongly configured addresses).
 */
#define qRegDMADCB_IrqCpyErr GP_WB_INT_CTRL_MASK_APPUC_DMA_DCB_CPY_ERR_INTERRUPT_LSB
/** @typedef qRegDMADCB_Irq_t
 *  @brief DMA DCB common interrupt (shared between channels).
 */
typedef UInt8 qRegDMADCB_Irq_t;
//@}

/** @brief Set the source buffer address for a given channel. */
static inline void qRegDMADCB_SrcAddrSet(UInt8 channel, UInt32 addr)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_ADDR(baseAddr, addr);
}

/** @brief Read the current source address for a given channel. */
static inline UInt32 qRegDMADCB_SrcAddrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_ADDR(baseAddr);
}

/** @brief Set the destination buffer address for a given channel. */
static inline void qRegDMADCB_DestAddrSet(UInt8 channel, UInt32 addr)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_DEST_ADDR(baseAddr, addr);
}

/** @brief Read the current destination address for a given channel. */
static inline UInt32 qRegDMADCB_DestAddrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_DEST_ADDR(baseAddr);
}

/** @brief Set the source buffer size. */
static inline void qRegDMADCB_SrcBufferSizeSet(UInt8 channel, UInt16 size)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_BUFFER_SIZE(baseAddr, size);
}

/** @brief Read the current source buffer size. */
static inline UInt16 qRegDMADCB_SrcBufferSizeGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_BUFFER_SIZE(baseAddr);
}

/** @brief Set the destination buffer size. */
static inline void qRegDMADCB_DestBufferSizeSet(UInt8 channel, UInt16 size)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_DEST_BUFFER_SIZE(baseAddr, size);
}

/** @brief Read the current destination buffer size. */
static inline UInt16 qRegDMADCB_DestBufferSizeGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_DEST_BUFFER_SIZE(baseAddr);
}

/** @brief Set the source buffer write pointer. */
static inline void qRegDMADCB_SrcWritePtrSet(UInt8 channel, UInt16 offset)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_WRITE_PTR_VALUE(baseAddr, offset);
}

/** @brief Read the current source buffer write pointer. */
static inline UInt16 qRegDMADCB_SrcWritePtrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_WRITE_PTR_VALUE(baseAddr);
}

/** @brief Set the source buffer write pointer wrap value. */
static inline void qRegDMADCB_SrcWritePtrWrapSet(UInt8 channel, UInt8 wrap)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_WRITE_PTR_WRAP(baseAddr, wrap);
}

/** @brief Read the current source buffer write pointer wrap value. */
static inline UInt8 qRegDMADCB_SrcWritePtrWrapGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_WRITE_PTR_WRAP(baseAddr);
}

/** @brief Set the source buffer read pointer. */
static inline void qRegDMADCB_SrcReadPtrSet(UInt8 channel, UInt16 offset)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_READ_PTR_VALUE(baseAddr, offset);
}

/** @brief Read the current source buffer read pointer. */
static inline UInt16 qRegDMADCB_SrcReadPtrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_READ_PTR_VALUE(baseAddr);
}

/** @brief Set the source buffer read pointer wrap value. */
static inline void qRegDMADCB_SrcReadPtrWrapSet(UInt8 channel, UInt8 wrap)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_SRC_READ_PTR_WRAP(baseAddr, wrap);
}

/** @brief Read the current source buffer read pointer wrap value. */
static inline UInt8 qRegDMADCB_SrcReadPtrWrapGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_SRC_READ_PTR_WRAP(baseAddr);
}

/** @brief Set the destination buffer write pointer. */
static inline void qRegDMADCB_DestWritePtrSet(UInt8 channel, UInt16 offset)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_DEST_WRITE_PTR_VALUE(baseAddr, offset);
}

/** @brief Read the current destination buffer write pointer. */
static inline UInt16 qRegDMADCB_DestWritePtrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_DEST_WRITE_PTR_VALUE(baseAddr);
}

/** @brief Set the destination buffer write pointer wrap value. */
static inline void qRegDMADCB_DestWritePtrWrapSet(UInt8 channel, UInt8 wrap)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_DEST_WRITE_PTR_WRAP(baseAddr, wrap);
}

/** @brief Read the current destination buffer write pointer wrap value. */
static inline UInt8 qRegDMADCB_DestWritePtrWrapGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_DEST_WRITE_PTR_WRAP(baseAddr);
}

/** @brief Set the destination buffer read pointer. */
static inline void qRegDMADCB_DestReadPtrSet(UInt8 channel, UInt16 offset)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_DEST_READ_PTR_VALUE(baseAddr, offset);
}

/** @brief Read the current destination buffer read pointer. */
static inline UInt16 qRegDMADCB_DestReadPtrGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_DEST_READ_PTR_VALUE(baseAddr);
}

/** @brief Set the destination buffer read pointer wrap value. */
static inline void qRegDMADCB_DestReadPtrWrapSet(UInt8 channel, UInt8 wrap)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_DEST_READ_PTR_WRAP(baseAddr, wrap);
}

/** @brief Read the current destination buffer read pointer wrap value. */
static inline UInt8 qRegDMADCB_DestReadPtrWrapGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_DEST_READ_PTR_WRAP(baseAddr);
}

/** @brief Define the minimum number of idle cycles between accesses to the same DMA channel. */
static inline void qRegDMADCB_AccessLatencySet(UInt8 channel, UInt8 cycles)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_ACCESS_LATENCY(baseAddr, cycles);
}

/** @brief Read the channel access latency. */
static inline UInt8 qRegDMADCB_AccessLatencyGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_DCB_ACCESS_LATENCY(baseAddr);
}

/** @brief Select between 8-, 16- or 32-bit memory accesses. */
static inline void qRegDMADCB_WordModeSet(UInt8 channel, qRegDMADCB_WordMode_t mode)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_WORD_MODE(baseAddr, mode);
}

/** @brief Read the channel word mode setting. */
static inline qRegDMADCB_WordMode_t qRegDMADCB_WordModeGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return (qRegDMADCB_WordMode_t)GP_WB_READ_DMA_DCB_WORD_MODE(baseAddr);
}

/** @brief Set the channel trigger. */
static inline void qRegDMADCB_TriggerSet(UInt8 channel, Bool trigger)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_DCB_CPY_TRIGGER(baseAddr, trigger);
}

/** @brief Read the channel trigger state. */
static inline Bool qRegDMADCB_TriggerGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return (Bool)GP_WB_READ_DMA_DCB_CPY_TRIGGER(baseAddr);
}

/** @brief Reset all DMA pointers. */
static inline void qRegDMADCB_PtrReset(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    GP_WB_DMA_DCB_RESET_POINTERS(baseAddr);
}

/** @brief Read the buffer complete interrupt flag. */
static inline Bool qRegDMADCB_IRQBufferCompleteGet(UInt8 channel)
{
    size_t baseAddr = Q_REG_DMADCB_CHANNEL_BASEADDR(channel);
    return (Bool)GP_WB_READ_DMA_DCB_UNMASKED_BUFFER_COMPLETE_INTERRUPT(baseAddr);
}

/** @brief Set the DMA channel switch latenct. */
static inline void qRegDMADCB_SwitchLatencySet(UInt8 cycles)
{
    GP_WB_WRITE_DMAS_DCB_CHANNEL_SWITCH_LATENCY(cycles);
}

/** @brief Read the channel switch latency. */
static inline UInt8 qRegDMADCB_SwitchLatencyGet()
{
    return GP_WB_READ_DMAS_DCB_CHANNEL_SWITCH_LATENCY();
}

/** @brief Clear the copy error interrupt flag. */
static inline void qRegDMADCB_IRQCpyErrorClear(void)
{
    GP_WB_DMAS_DCB_CLR_CPY_ERR_INTERRUPT();
}

/** @brief Read the copy error interrupt flag. */
static inline Bool qRegDMADCB_IRQCpyErrorGet()
{
    return (Bool)GP_WB_READ_DMAS_DCB_UNMASKED_CPY_ERR_INTERRUPT();
}

/** @brief Select the QUIC channel for the specific DMA channel/action. */
static inline void qRegDMADCB_QUICActionChannelSet(UInt8 channel, UInt8 actionIdx, UInt8 quicChannel)
{
    (void)channel;

    UInt32 addr = GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_ADDRESS + (Q_REG_DMADCB_QUIC_ACTION_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(addr, GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_MASK,
                     ((UInt32)quicChannel << GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_LSB));
}

/** @brief Read the QUIC channel/action setting for a given DMA channel. */
static inline UInt8 qRegDMADCB_QUICActionChannelGet(UInt8 channel, UInt8 actionIdx)
{
    (void)channel;

    UInt32 addr = GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_ADDRESS + (Q_REG_DMADCB_QUIC_ACTION_OFFSET * actionIdx);

    return (UInt8)((GP_WB_READ_U32(addr) & GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_MASK) >>
                   GP_WB_DMAS_DCB_QUIC_ACTION_0_CHANNEL_0_LSB);
}

/** @brief Set the QUIC action type for a given DMA channel/action. */
static inline void qRegDMADCB_QUICActionTypeSet(UInt8 channel, UInt8 actionIdx, qRegDMADCB_QuicAction_t action)
{
    (void)channel;

    UInt32 addr = GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_ADDRESS + (Q_REG_DMADCB_QUIC_ACTION_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(addr, GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_MASK,
                     ((UInt32)action << GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_LSB));
}

/** @brief Read the QUIC action type for a given DMA channel/action. */
static inline qRegDMADCB_QuicAction_t qRegDMADCB_QUICActionTypeGet(UInt8 channel, UInt8 actionIdx)
{
    UInt32 addr = GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_ADDRESS + (Q_REG_DMADCB_QUIC_ACTION_OFFSET * actionIdx);

    return (qRegDMADCB_QuicAction_t)((GP_WB_READ_U32(addr) & GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_MASK) >>
                                     GP_WB_DMAS_DCB_QUIC_ACTION_0_TYPE_0_LSB);
}

/** @brief Enable/disable backup of peripheral registers before entering deep sleep mode. */
static inline void qRegDMADCB_RetainSet(Bool enable)
{
    GP_WB_WRITE_SYSRAM_RETAIN_DMAS_DCB((UInt8)enable);
}

#ifdef __cplusplus
}
#endif

#endif //_REG_UART_H_
