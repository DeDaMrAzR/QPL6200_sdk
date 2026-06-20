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

#ifndef _REG_DMA_H_
#define _REG_DMA_H_

#include "hal.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Calculate the address offset between DMA channels. */
#define REG_DMA_CHANNEL_OFFSET (GP_WB_DMAS_SCB_SRC_ADDR_1_ADDRESS - GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS)
/** @brief Calculate the DMA channel base address. */
#define REG_DMA_CHANNEL_BASEADDR(CHANNEL) (GP_WB_DMAS_SCB_BASE_ADDRESS + REG_DMA_CHANNEL_OFFSET * (CHANNEL))

/** @enum qRegDMA_WordMode_t */
//@{
/** @brief 8-bit word. */
#define qRegDMA_WordMode8Bit GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA8BIT
/** @brief 16-bit word. */
#define qRegDMA_WordMode16Bit GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA16BIT
/** @brief 32-bit word. */
#define qRegDMA_WordMode32Bit GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA32BIT
/** @typedef qRegDMA_WordMode_t
 *  @brief DMA word size. 8-, 16- or 32-bit memory accesses.
 */
typedef UInt8 qRegDMA_WordMode_t;
//@}

/** @enum qRegDMA_CompleteMode_t */
//@{
/** @brief Source buffer underrun or destination buffer overflow. */
#define qRegDMA_CompleteModeError GP_WB_ENUM_DMA_SCB_BUFFER_COMPLETE_MODE_ERROR_MODE
/** @brief Source buffer empty or destination buffer full. */
#define qRegDMA_CompleteModeStatus GP_WB_ENUM_DMA_SCB_BUFFER_COMPLETE_MODE_STATUS_MODE
/** @typedef qRegDMA_CompleteMode_t
 *  @brief The type of information that is indicated by channel complete event.
 */
typedef UInt8 qRegDMA_CompleteMode_t;
//@}

/** @enum qRegDMA_QuicAction_t */
//@{
/** @brief No action. */
#define qRegDMA_QuicActionNone GP_WB_ENUM_DMA_SCB_ACTION_NONE
/** @brief Performs a copy, similar to when a trigger happens. */
#define qRegDMA_QuicActionCopy GP_WB_ENUM_DMA_SCB_ACTION_DO_COPY
/** @typedef qRegDMA_QuicAction_t
 *  @brief Action which should happen to the DMA channel when the QUIC trigger arrives.
 */
typedef UInt8 qRegDMA_QuicAction_t;
//@}

/** @enum qRegDMA_CircularBuffer_t */
//@{
/** @brief Copy from source buffer into the destination address. */
#define qRegDMA_CircularBufferSrc GP_WB_ENUM_CIRCULAR_BUFFER_SRC_BUFFER
/** @brief Copy from source address into the destination buffer. */
#define qRegDMA_CircularBufferDest GP_WB_ENUM_CIRCULAR_BUFFER_DEST_BUFFER
/** @typedef qRegDMA_CircularBuffer_t
 *  @brief Circular buffer mode.
 */
typedef UInt8 qRegDMA_CircularBuffer_t;
//@}

/** @enum qRegDMA_Trigger_t */
//@{
#define qRegDMA_TriggerNone                      GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_NO_TRIGGER_SRC
#define qRegDMA_TriggerSPISLTxNotFull            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_SL_TX_NOT_FULL
#define qRegDMA_TriggerSPISLRxNotEmpty           GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_SL_RX_NOT_EMPTY
#define qRegDMA_TriggerSPIMTxNotFull             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_M_TX_NOT_FULL
#define qRegDMA_TriggerSPIMRxNotEmpty            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_M_RX_NOT_EMPTY
#define qRegDMA_TriggerQSPIMTxNotFull            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_QSPI_M_TX_NOT_FULL
#define qRegDMA_TriggerQSPIMRxNotEmpty           GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_QSPI_M_RX_NOT_EMPTY
#define qRegDMA_TriggerI2SMTxNotFull             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_M_TX_NOT_FULL
#define qRegDMA_TriggerI2SMRxNotFull             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_M_RX_NOT_EMPTY
#define qRegDMA_TriggerI2SVSRSampleFifoNotFull   GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_VSR_SAMPLE_FIFO_NOT_FULL
#define qRegDMA_TriggerUART0TxNotFull            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_0_TX_NOT_FULL
#define qRegDMA_TriggerUART0RxNotEmpty           GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_0_RX_NOT_EMPTY
#define qRegDMA_TriggerUART1TxNotFull            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_1_TX_NOT_FULL
#define qRegDMA_TriggerUART1RxNotEmpty           GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_1_RX_NOT_EMPTY
#define qRegDMA_TriggerUART2TxNotFull            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_2_TX_NOT_FULL
#define qRegDMA_TriggerUART2RxNotEmpty           GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_2_RX_NOT_EMPTY
#define qRegDMA_TriggerRTZ0TxNotFull             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_RTZ_0_TX_NOT_FULL
#define qRegDMA_TriggerRTZ1TxNotFull             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_RTZ_1_TX_NOT_FULL
#define qRegDMA_TriggerPWMTxNotFull              GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWM_TX_NOT_FULL
#define qRegDMA_TriggerPWMTimestamp0NotEmpty     GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWM_TIMESTAMP0_NOT_EMPTY
#define qRegDMA_TriggerPWMTimestamp1NotEmpty     GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWM_TIMESTAMP1_NOT_EMPTY
#define qRegDMA_TriggerPWMTimestamp2NotEmpty     GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWM_TIMESTAMP2_NOT_EMPTY
#define qRegDMA_TriggerPWMTimestamp3NotEmpty     GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWM_TIMESTAMP3_NOT_EMPTY
#define qRegDMA_TriggerPWMXLTopOverflow          GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_TOP_OVERFLOW
#define qRegDMA_TriggerPWMXLNextBufferNotFull    GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_NEXT_BUFFER_NOT_FULL
#define qRegDMA_TriggerPWMXL0PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_0_PHASE_ON
#define qRegDMA_TriggerPWMXL0PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_0_PHASE_OFF
#define qRegDMA_TriggerPWMXL1PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_1_PHASE_ON
#define qRegDMA_TriggerPWMXL1PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_1_PHASE_OFF
#define qRegDMA_TriggerPWMXL2PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_2_PHASE_ON
#define qRegDMA_TriggerPWMXL2PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_2_PHASE_OFF
#define qRegDMA_TriggerPWMXL3PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_3_PHASE_ON
#define qRegDMA_TriggerPWMXL3PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_3_PHASE_OFF
#define qRegDMA_TriggerPWMXL4PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_4_PHASE_ON
#define qRegDMA_TriggerPWMXL4PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_4_PHASE_OFF
#define qRegDMA_TriggerPWMXL5PhaseOn             GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_5_PHASE_ON
#define qRegDMA_TriggerPWMXL5PhaseOff            GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_5_PHASE_OFF
#define qRegDMA_TriggerGPADCFifoNotEmpty         GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GP_ADC_FIFO_NOT_EMPTY
#define qRegDMA_TriggerASP0Datastream0Valid      GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_0_DATASTREAM_0_VALID
#define qRegDMA_TriggerASP0Datastream1Valid      GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_0_DATASTREAM_1_VALID
#define qRegDMA_TriggerASP1Datastream0Valid      GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_1_DATASTREAM_0_VALID
#define qRegDMA_TriggerASP1Datastream1Valid      GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_1_DATASTREAM_1_VALID
#define qRegDMA_TriggerBISTADCResultFifoNotEmpty GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_BIST_ADC_RESULT_FIFO_NOT_EMPTY
#define qRegDMA_TriggerBISTTSENSResultFifoNotEmpty                                                                     \
    GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_BIST_TSENS_RESULT_FIFO_NOT_EMPTY
#define qRegDMA_TriggerGPIOTimestampFifo0NotEmpty GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GPIO_TIMESTAMP_FIFO_0_NOT_EMPTY
#define qRegDMA_TriggerGPIOTimestampFifo1NotEmpty GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GPIO_TIMESTAMP_FIFO_1_NOT_EMPTY
/** @typedef qRegDMA_Trigger_t
 *  @brief Selects DMA trigger source. Refer to the datasheel for more information.
 */
typedef UInt8 qRegDMA_Trigger_t;
//@}

/** @enum qRegDMA_Irq_t */
//@{
/** @brief Triggered when a memory access error occurred during a copy action
 *         (could be as a result of wrongly configured addresses).
 */
#define qRegDMA_IrqCpyErr GP_WB_DMAS_SCB_UNMASKED_CPY_ERR_INTERRUPT_LSB
/** @typedef qRegDMA_Irq_t
 *  @brief DMA common interrupt (shared between channels).
 */
typedef UInt8 qRegDMA_Irq_t;
//@}

/** @enum qRegDMA_ChannelIrq_t */
//@{
/** @brief In case of a source buffer, it is set whenever the source buffer filling
 *         level is below the programmed threshold. In case of a destination buffer,
 *         it is set whenever the destination buffer filling level is above theat threshold.
 */
#define qRegDMA_ChannelIrqAlmostComplete 0
/** @brief Status or error interrupt. The meaning and the trigger condition depend
 *         on the circular buffer and buffer complete modes.
 */
#define qRegDMA_ChannelIrqComplete 1
/** @typedef qRegDMA_ChannelIrq_t
 *  @brief DMA channel interrupt.
 */
typedef UInt8 qRegDMA_ChannelIrq_t;
//@}

/** @brief DMA channel interrupt mask. */
typedef UInt32 qRegDMA_IrqMask_t;

static inline void qRegDMA_IRQCpyErrorClear(void)
{
    GP_WB_DMAS_SCB_CLR_CPY_ERR_INTERRUPT();
}

/** @brief Clear the DMA channel interrupt. */
static inline void qRegDMA_ChannelEventIRQClear(UInt8 channel, qRegDMA_ChannelIrq_t irq)
{
    if(irq != qRegDMA_ChannelIrqComplete)
    {
        return;
    }

    size_t ch_baseaddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_DMA_SCB_CLR_BUFFER_COMPLETE_INTERRUPT(ch_baseaddr);
}

/** @brief Configure the amount of idle cycles between accesses to different
 *         DMA channels. It can be used to reduce the DMA memory bandwidth.
 */
static inline void qRegDMA_ChannelSwitchLatencySet(UInt8 cycles)
{
    GP_WB_WRITE_DMAS_SCB_CHANNEL_SWITCH_LATENCY(cycles);
}

/** @brief Read the DMA channel switch latency setting. */
static inline UInt8 qRegDMA_ChannelSwitchLatencyGet(void)
{
    return GP_WB_READ_DMAS_SCB_CHANNEL_SWITCH_LATENCY();
}

/** @brief Set the address of the first entry of the source buffer for a given channel,
 *         which equals the entry address when a single entry source location is used.
 */
static inline void qRegDMA_SrcAddrSet(UInt8 channel, UInt32 addr)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_SRC_ADDR(baseAddr, addr);
}

/** @brief Read the current source address for a given channel. */
static inline UInt32 qRegDMA_SrcAddrGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_SRC_ADDR(baseAddr);
}

/** @brief Set the address of the first entry of the destination buffer for a given channel,
 *         which equals the entry address when a single entry destination location is used.
 */
static inline void qRegDMA_DestAddrSet(UInt8 channel, UInt32 addr)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_DEST_ADDR(baseAddr, addr);
}

/** @brief Read the current destination address for a given channel. */
static inline UInt32 qRegDMA_DestAddrGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_DEST_ADDR(baseAddr);
}

/** @brief Set the size-1 of either the circular source or destination buffer.
 *         The size is specified as a number of words. The word size in configurable. */
static inline void qRegDMA_BufferSizeSet(UInt8 channel, UInt16 size)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_BUFFER_SIZE(baseAddr, size);
}

/** @brief Read the current buffer size register value. */
static inline UInt16 qRegDMA_BufferSizeGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_BUFFER_SIZE(baseAddr);
}

/** @brief Convert real buffer size in bytes to a valid register value. */
static inline UInt16 qRegDMA_BufferSizeToReg(UInt16 realSize, qRegDMA_WordMode_t wordMode)
{
    return (realSize >> wordMode) - 1;
}

/** @brief Convert the value read from buffer size registre to a real size in bytes. */
static inline UInt16 qRegDMA_RegToBufferSize(UInt16 regSize, qRegDMA_WordMode_t wordMode)
{
    return (regSize + 1) << wordMode;
}

/** @brief Set the threshold level for triggering the almost complete Interrupt
 *         It can be used to inform the software that source buffer is running out of data
 *         or the destination buffer is almost full.
 */
static inline void qRegDMA_AlmostCompleteThresholdSet(UInt8 channel, UInt16 threshold)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_BUFFER_ALMOST_COMPLETE_THRESHOLD(baseAddr, threshold);
}

/** @brief Read the current threshold setting for a given channel. */
static inline UInt16 qRegDMA_AlmostCompleteThresholdGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_BUFFER_ALMOST_COMPLETE_THRESHOLD(baseAddr);
}

/** @brief Used to set the pointer value in an atomic way. */
static inline void qRegDMA_BufferPtrValueSet(UInt8 channel, UInt16 value)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_BUFFER_PTR_VALUE(baseAddr, value);
}

/** @brief Read the pointer value set for a given channel. */
static inline UInt16 qRegDMA_BufferPtrValueGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_BUFFER_PTR_VALUE(baseAddr);
}

/** @brief Used to set the pointer wrap value register in an atomic way. */
static inline void qRegDMA_BufferPtrWrapValueSet(UInt8 channel, UInt8 wrap)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_BUFFER_PTR_WRAP_VALUE(baseAddr, wrap);
}

/** @brief Read the pointer wrap value register set for a given channel */
static inline UInt8 qRegDMA_BufferPtrWrapValueGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_BUFFER_PTR_WRAP_VALUE(baseAddr);
}

/** @brief Select between 8-, 16- or 32-bit memory accesses.
 *         Note that the buffer size and level threshold all need to be expressed
 *         as multiples of this memory access size.
 */
static inline void qRegDMA_WordModeSet(UInt8 channel, qRegDMA_WordMode_t mode)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_WORD_MODE(baseAddr, mode);
}

/** @brief Read the channel word mode setting. */
static inline qRegDMA_WordMode_t qRegDMA_WordModeGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (qRegDMA_WordMode_t)GP_WB_READ_DMA_SCB_WORD_MODE(baseAddr);
}

/** @brief Set the channel trigger source. */
static inline void qRegDMA_TriggerSet(UInt8 channel, qRegDMA_Trigger_t trigger)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_CPY_TRIGGER_SRC_SELECT(baseAddr, trigger);
}

/** @brief Read the channel trigger source. */
static inline qRegDMA_Trigger_t qRegDMA_TriggerGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (qRegDMA_WordMode_t)GP_WB_READ_DMA_SCB_CPY_TRIGGER_SRC_SELECT(baseAddr);
}

/** @brief Override the channel trigger. The dma copy will be triggered as long as
 *         this option is set to true. */
static inline void qRegDMA_TriggerByRegmapSet(UInt8 channel, Bool set)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_CPY_TRIGGER_BY_REGMAP(baseAddr, set);
}

/** @brief Read the channel trigger override state. */
static inline Bool qRegDMA_TriggerByRegmapGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_CPY_TRIGGER_BY_REGMAP(baseAddr);
}

/** @brief Set the channel buffer complete mode. If set to error mode,
 *         then the buffer complete interrupt indicates overflow/underrun conditions.
 *         Otherwise (status mode) full/empty conditions are reported.
 */
static inline void qRegDMA_CompleteModeSet(UInt8 channel, qRegDMA_CompleteMode_t mode)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_BUFFER_COMPLETE_INTERRUPT_MODE(baseAddr, mode);
}

/** @brief Read the channel complete mode. */
static inline qRegDMA_CompleteMode_t qRegDMA_RegDMA_CompleteModeGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (qRegDMA_WordMode_t)GP_WB_READ_DMA_SCB_BUFFER_COMPLETE_INTERRUPT_MODE(baseAddr);
}

/** @brief Define the minimum number of idle cycles between accesses
 *         to the same DMA channel. Should be increased for slowly responding copy
 *         triggers. E.g. when a copy trigger takes several clock cycles to be cleared
 *         after a copy, increasing this setting can prevent that an unintended
 *         2nd copy is done.
 */
static inline void qRegDMA_AccessLatencySet(UInt8 channel, UInt8 cycles)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_ACCESS_LATENCY(baseAddr, cycles);
}

/** @brief Read the channel access latency. */
static inline UInt8 qRegDMA_AccessLatencyGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_ACCESS_LATENCY(baseAddr);
}

/** @brief Select which QUIC channel the specific DMA channel
 *         will listen to for a trigger.
 */
static inline void qRegDMA_QuicChannelSet(UInt8 channel, UInt8 quicChannel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_QUIC_ACTION_CHANNEL(baseAddr, quicChannel);
}

/** @brief Read the QUIC channel setting for a given DMA channel. */
static inline UInt8 qRegDMA_QuicChannelGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_QUIC_ACTION_CHANNEL(baseAddr);
}

/** @brief Select which action should happen to the DMA channel
 *         when the QUIC trigger arrives.
 */
static inline void qRegDMA_QuicActionSet(UInt8 channel, qRegDMA_QuicAction_t action)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_QUIC_ACTION_TYPE(baseAddr, action);
}

/** @brief Read the QUIC action setting for a given DMA channel. */
static inline qRegDMA_QuicAction_t qRegDMA_QuicActionGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (qRegDMA_QuicAction_t)GP_WB_READ_DMA_SCB_QUIC_ACTION_TYPE(baseAddr);
}

/** @brief When enabled, the channel buffer completeness is ignored when
 *         triggering a dma copy action.
 */
static inline void qRegDMA_IgnoreCompleteSet(UInt8 channel, Bool set)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_CPY_TRIGGER_IGNORE_BUFFER_COMPLETE(baseAddr, set);
}

/** @brief Read the buffer complete ignore setting for a given channel. */
static inline Bool qRegDMA_IgnoreCompleteGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_CPY_TRIGGER_IGNORE_BUFFER_COMPLETE(baseAddr);
}

/** @brief Select between source and destination as circular buffer location. */
static inline void qRegDMA_BufferSelectSet(UInt8 channel, qRegDMA_CircularBuffer_t mode)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_WRITE_DMA_SCB_CIRCULAR_BUFFER_SELECT(baseAddr, mode);
}

/** @brief Read the channel circular buffer mode. */
static inline qRegDMA_CircularBuffer_t qRegDMA_BufferSelectGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (qRegDMA_CircularBuffer_t)GP_WB_READ_DMA_SCB_CIRCULAR_BUFFER_SELECT(baseAddr);
}

/** @brief Set the internal circular buffer write pointer by previously set buffer pointer
 *         and buffer wrap values. Provides atomic way for updating channel buffer
 *         write pointer.
 */
static inline void qRegDMA_WritePtrSet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_DMA_SCB_SET_WRITE_PTR(baseAddr);
}

/** @brief Set the internal circular buffer read pointer by previously set buffer pointer
 *         and buffer wrap values. Provides atomic way for initializing channel buffer
 *         read pointer.
 */
static inline void qRegDMA_ReadPtrSet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_DMA_SCB_SET_READ_PTR(baseAddr);
}

/** @brief Reset both channel read and write pointers to 0. */
static inline void qRegDMA_PtrReset(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    GP_WB_DMA_SCB_RESET_POINTERS(baseAddr);
}

/** @brief Read the InternalPtrStatus value which contains the InternalPtr and InternalPtrWrap in one shot.
 * This value should be used to calclate the InternalPtr and InternalPtrWrap to avoid race conditions if done in
 * separate read operations. */
static inline UInt32 qRegDMA_InternalPtrStatusGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_INTERNAL_PTR_STATUS(baseAddr);
}

/** @brief Read the actual offset that is being used internally
 *         to access the channel circular buffer (from the InternalPtrStatus). */
static inline UInt16 qRegDMA_InternalPtrFromInternalPtrStatusGet(UInt32 internalPtrStatus)
{
    return GP_WB_GET_DMA_SCB_INTERNAL_PTR_FROM_INTERNAL_PTR_STATUS(internalPtrStatus);
}

/** @brief Read the current internal channel buffer wrap value (from the InternalPtrStatus). */
static inline UInt8 qRegDMA_InternalPtrWrapFromInternalPtrStatusGet(UInt32 internalPtrStatus)
{
    return GP_WB_GET_DMA_SCB_INTERNAL_PTR_WRAP_FROM_INTERNAL_PTR_STATUS(internalPtrStatus);
}

/** @brief Read the actual offset that is being used internally
 *         to access the channel circular buffer. */
static inline UInt16 qRegDMA_InternalPtrGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_INTERNAL_PTR(baseAddr);
}

/** @brief Read the current internal channel buffer wrap value. */
static inline UInt8 qRegDMA_InternalPtrWrapGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return GP_WB_READ_DMA_SCB_INTERNAL_PTR_WRAP(baseAddr);
}

/** @brief Read the status complete value. */
static inline Bool qRegDMA_StatusCompleteGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (Bool)GP_WB_READ_DMA_SCB_UNMASKED_BUFFER_COMPLETE_INTERRUPT(baseAddr);
}

/** @brief Read the status almost complete value. */
static inline Bool qRegDMA_StatusAlmostCompleteGet(UInt8 channel)
{
    size_t baseAddr = REG_DMA_CHANNEL_BASEADDR(channel);
    return (Bool)GP_WB_READ_DMA_SCB_UNMASKED_BUFFER_ALMOST_COMPLETE_INTERRUPT(baseAddr);
}

/** @brief Enable/disable backup of peripheral registers before entering deep sleep mode. */
static inline void qRegDMA_RetainSet(Bool enable)
{
    GP_WB_WRITE_SYSRAM_RETAIN_DMAS_SCB((UInt8)enable);
}

#ifdef __cplusplus
}
#endif

#endif //_REG_UART_H_
