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

#ifndef _REG_RTZ_H_
#define _REG_RTZ_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Definition of RTZ peripheral. */
typedef uint32_t qRegRTZ_t;

/**
 * @brief Macro for getting the RTZ peripheral base address by ID.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_RTZ_BASE_ADDR_GET(ID) (qRegRTZ_t)(Q_CONCAT(GP_WB_RTZ_, ID, _BASE_ADDRESS))

/**
 * @brief Macro for getting device specific interrupt number.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_RTZ_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(RTZ_, ID, _IRQn))

/**
 * @brief Macro for getting the TX register data address.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_REG_RTZ_TX_DATA_ADDRESS_GET(ID) (UInt32)(Q_CONCAT(GP_WB_RTZ_, ID, _TX_DATA_ADDRESS))

/** @enum qRegRTZ_WordSize_t */
//@{
/** @brief 8 bit mode. */
#define qRegRTZ_WordSize8Bit GP_WB_ENUM_RTZ_BUFFER_MODE_DATA8BIT
/** @brief 16 bit mode. */
#define qRegRTZ_WordSize16Bit GP_WB_ENUM_RTZ_BUFFER_MODE_DATA16BIT
/** @brief 32 bit mode. */
#define qRegRTZ_WordSize32Bit GP_WB_ENUM_RTZ_BUFFER_MODE_DATA32BIT
/** @typedef qRegRTZ_WordSize_t
 *  @brief RTZ buffer mode.
 */
typedef UInt8 qRegRTZ_WordSize_t;
//@}

/** @enum qRegRTZ_QuicActionType_t */
//@{
/** @brief Action none. */
#define qRegRTZ_QuicActionTypeNone GP_WB_ENUM_RTZ_ACTION_NONE
/** @brief Action trigger sequence. */
#define qRegRTZ_QuicActionTypeTriggerSequence GP_WB_ENUM_RTZ_ACTION_TRIGGER_SEQUENCE
/** @typedef qRegRTZ_QuicActionType_t
 *  @brief RTZ action.
 */
typedef UInt8 qRegRTZ_QuicActionType_t;
//@}

/** @enum qRegRTZ_Event_t */
//@{
/** @brief Buffer underrun error. */
#define qRegRTZ_EventUnderrunError GP_WB_INT_CTRL_MASK_APPUC_RTZ_0_BUFFER_UNDERRUN_INTERRUPT_LSB
/** @brief Sequence transmission complete. */
#define qRegRTZ_EventComplete GP_WB_INT_CTRL_MASK_APPUC_RTZ_0_SEQUENCE_DONE_INTERRUPT_LSB
/** @brief TX buffer not full. */
#define qRegRTZ_EventTxNotFull GP_WB_INT_CTRL_MASK_APPUC_RTZ_0_TX_NOT_FULL_INTERRUPT_LSB
/** @typedef qRegRTZ_event_t
 *  @brief RTZ event.
 */
typedef UInt8 qRegRTZ_Event_t;
//@}

/**
 * @brief Function to setting TX data FIFO argument.
 *
 * Internal buffer data always is overwritten, when tx_data is written, even if the buffer was not empty.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] txData   Tx data to be written to the FIFO.
 */
static inline void qRegRTZ_TxDataSet(qRegRTZ_t baseAddr, UInt32 txData)
{
    GP_WB_WRITE_RTZ_TX_DATA(baseAddr, txData);
}

/**
 * @brief Function for setting the initial internal random seed used for jitter generation.
 *
 * @param[in] baseAddr   Register base address of the peripheral.
 * @param[in] randomSeed Random seed initial value to be set.
 */
static inline void qRegRTZ_InitRandomSeedSet(qRegRTZ_t baseAddr, UInt32 randomSeed)
{
    GP_WB_WRITE_RTZ_INIT_RANDOM_SEED(baseAddr, randomSeed);
}

/**
 * @brief Function for setting the buffer mode.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] wordSize Buffer word size to be set.
 */
static inline void qRegRTZ_WordSizeSet(qRegRTZ_t baseAddr, qRegRTZ_WordSize_t wordSize)
{
    GP_WB_WRITE_RTZ_BUFFER_WORD_MODE(baseAddr, (UInt32)wordSize);
}

/**
 * @brief Function for getting the buffer mode.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Buffer mode.
 */
static inline qRegRTZ_WordSize_t qRegRTZ_BufferWordModeGet(qRegRTZ_t baseAddr)
{
    return (qRegRTZ_WordSize_t)GP_WB_READ_RTZ_BUFFER_WORD_MODE(baseAddr);
}

/**
 * @brief Function for setting the pixel word alignment.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if new pixel is started, a new tx data word is used (so pixels word aligned).
 *                     False if the next pixel data continues on the next data bit present in the buffer.
 */
static inline void qRegRTZ_AlignPixelOnNextWordEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_ALIGN_PIXEL_ON_NEXT_WORD(baseAddr, enable);
}

/**
 * @brief Function for getting the status of the the pixel word alignment.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if new pixel is started, a new tx data word is used (so pixels word aligned).
 * @retval False if the next pixel data continues on the next data bit present in the buffer.
 */
static inline Bool qRegRTZ_AlignPixelOnNextWordEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_ALIGN_PIXEL_ON_NEXT_WORD(baseAddr);
}

/**
 * @brief Function for enabling or disabling a stop symbol after the trailing data sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the stop symbol is to be enabled, false otherwise.
 */
static inline void qRegRTZ_StopSymbolAfterTrailingDataEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_ENABLE_STOP_SYMBOL_AFTER_TRAILING_DATA(baseAddr, enable);
}

/**
 * @brief Function for getting the status of the stop symbol after the trailing data sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the stop symbol is enabled, false otherwise.
 */
static inline Bool qRegRTZ_StopSymbolAfterTrailingDataEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_ENABLE_STOP_SYMBOL_AFTER_TRAILING_DATA(baseAddr);
}

/**
 * @brief Function for enabling zero output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the inversion is to be enabled, false otherwise.
 */
static inline void qRegRTZ_ZeroOutputInvertEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_ZERO_OUTPUT_INVERT(baseAddr, enable);
}

/**
 * @brief Function for getting the status of zero output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the inversion is enabled, false otherwise.
 */
static inline Bool qRegRTZ_ZeroOutputInvertEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_ZERO_OUTPUT_INVERT(baseAddr);
}

/**
 * @brief Function for enabling one output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the inversion is to be enabled, false otherwise.
 */
static inline void qRegRTZ_OneOutputInvertEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_ONE_OUTPUT_INVERT(baseAddr, enable);
}

/**
 * @brief Function for getting the status of one output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the inversion is enabled, false otherwise.
 */
static inline Bool qRegRTZ_OneOutputInvertEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_ONE_OUTPUT_INVERT(baseAddr);
}

/**
 * @brief Function for enabling stop output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the inversion is to be enabled, false otherwise.
 */
static inline void qRegRTZ_StopOutputInvertEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_STOP_OUTPUT_INVERT(baseAddr, enable);
}

/**
 * @brief Function for getting the status of stop output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the inversion is enabled, false otherwise.
 */
static inline Bool qRegRTZ_StopOutputInvertEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_STOP_OUTPUT_INVERT(baseAddr);
}

/**
 * @brief Function for enabling reset output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the inversion is to be enabled, false otherwise.
 */
static inline void qRegRTZ_ResetOutputInvertEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_RESET_OUTPUT_INVERT(baseAddr, enable);
}

/**
 * @brief Function for getting the status of reset output inversion.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the inversion is enabled, false otherwise.
 */
static inline Bool qRegRTZ_ResetOutputInvertEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_RESET_OUTPUT_INVERT(baseAddr);
}

/**
 * @brief Function for enabling physical output inversion.
 *
 * This is an overall inversion, so it applies to all symbols/states.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the inversion is to be enabled, false otherwise.
 */
static inline void qRegRTZ_PhysicalOutputInvertEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_PHYSICAL_OUTPUT_INVERT(baseAddr, enable);
}

/**
 * @brief Function for getting the status of physical output inversion.
 *
 * This is an overall inversion, so it applies to all symbols/states.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the inversion is enabled, false otherwise.
 */
static inline Bool qRegRTZ_PhysicalOutputInvertEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_PHYSICAL_OUTPUT_INVERT(baseAddr);
}

/**
 * @brief Function for enabling LSB first mode within each word.
 *
 * It is up to the user to align the data to the LSB or MSB of the word.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True for LSB mode, false for MSB mode.
 */
static inline void qRegRTZ_LsbFirstEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_LSB_FIRST(baseAddr, enable);
}

/**
 * @brief Function for getting the status of LSB first mode.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True for LSB mode, false for MSB mode.
 */
static inline Bool qRegRTZ_LsbFirstEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_LSB_FIRST(baseAddr);
}

/**
 * @brief Function for enabling the trailing data after all pixels.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if trailing data are to be enabled, false otherwise.
 */
static inline void qRegRTZ_TrailingDataEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_ENABLE_TRAILING_DATA(baseAddr, enable);
}

/**
 * @brief Function for getting the status of trailing data.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if trailing data are enabled, false otherwise.
 */
static inline Bool qRegRTZ_TrailingDataEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_ENABLE_TRAILING_DATA(baseAddr);
}

/**
 * @brief Function for setting the high time for the ZERO symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    High time value.
 */
static inline void qRegRTZ_HighTimeZeroSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TZERO_HIGH(baseAddr, value);
}

/**
 * @brief Function for getting the high time for the ZERO symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval High time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_HighTimeZeroGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TZERO_HIGH(baseAddr);
}

/**
 * @brief Function for setting the low time for the ZERO symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    Low time value.
 */
static inline void qRegRTZ_LowTimeZeroSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TZERO_LOW(baseAddr, value);
}

/**
 * @brief Function for getting the low time for the ZERO symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Low time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_LowTimeZeroGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TZERO_LOW(baseAddr);
}

/**
 * @brief Function for setting the high time for the ONE symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    High time value.
 */
static inline void qRegRTZ_HighTimeOneSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TONE_HIGH(baseAddr, value);
}

/**
 * @brief Function for getting the high time for the ONE symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval High time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_HighTimeOneGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TONE_HIGH(baseAddr);
}

/**
 * @brief Function for setting the low time for the ONE symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    Low time value.
 */
static inline void qRegRTZ_LowTimeOneSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TONE_LOW(baseAddr, value);
}

/**
 * @brief Function for getting the low time for the ONE symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Low time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_LowTimeOneGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TONE_LOW(baseAddr);
}

/**
 * @brief Function for setting the high time for the STOP symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    High time value.
 */
static inline void qRegRTZ_HighTimeStopSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TSTOP_HIGH(baseAddr, value);
}

/**
 * @brief Function for getting the high time for the STOP symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval High time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_HighTimeStopGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TSTOP_HIGH(baseAddr);
}

/**
 * @brief Function for setting the low time for the STOP symbol.
 *
 * Actual duration is 31.25 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    Low time value.
 */
static inline void qRegRTZ_LowTimeStopSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TSTOP_LOW(baseAddr, value);
}

/**
 * @brief Function for getting the low time for the STOP symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Low time value. Actual duration is 31.25 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_LowTimeStopGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TSTOP_LOW(baseAddr);
}

/**
 * @brief Function for setting the high time for the RESET symbol.
 *
 * Actual duration is 500 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    High time value.
 */
static inline void qRegRTZ_HighTimeResetSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TRESET_HIGH(baseAddr, value);
}

/**
 * @brief Function for getting the high time for the RESET symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval High time value. Actual duration is 500 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_HighTimeResetGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TRESET_HIGH(baseAddr);
}

/**
 * @brief Function for setting the low time for the RESET symbol.
 *
 * Actual duration is 500 ns * (value + 1).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    Low time value.
 */
static inline void qRegRTZ_LowTimeResetSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_TRESET_LOW(baseAddr, value);
}

/**
 * @brief Function for getting the low time for the RESET symbol.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Low time value. Actual duration is 500 ns * (value + 1).
 */
static inline UInt32 qRegRTZ_LowTimeResetGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_TRESET_LOW(baseAddr);
}

/**
 * @brief Function for setting the jitter magnitude bits
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] value    Jitter magnitude bits value.
 */
static inline void qRegRTZ_JitterMagnitudeBitsSet(qRegRTZ_t baseAddr, UInt32 value)
{
    GP_WB_WRITE_RTZ_JITTER_MAGNITUDE_BITS(baseAddr, value);
}

/**
 * @brief Function for getting the jitter magnitude bits
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Jitter magnitude bits
 */
static inline UInt32 qRegRTZ_JitterMagnitudeBitsGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_JITTER_MAGNITUDE_BITS(baseAddr);
}

/**
 * @brief Function for enabling the jitter on T0L and T1L duration.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the jitter is to be enabled, false otherwise.
 */
static inline void qRegRTZ_TLowJitterEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_TLOW_JITTER_ENABLE(baseAddr, enable);
}

/**
 * @brief Function for getting the jitter on T0L and T1L duration.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the jitter is enabled, false otherwise.
 */
static inline Bool qRegRTZ_TLowJitterEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_TLOW_JITTER_ENABLE(baseAddr);
}

/**
 * @brief Function for enabling the jitter on T0H and T1H duration.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the jitter is to be enabled, false otherwise.
 */
static inline void qRegRTZ_THighJitterEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_THIGH_JITTER_ENABLE(baseAddr, enable);
}

/**
 * @brief Function for getting the jitter on T0H and T1H duration.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the jitter is enabled, false otherwise.
 */
static inline Bool qRegRTZ_THighJitterEnableCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_THIGH_JITTER_ENABLE(baseAddr);
}

/**
 * @brief Function for setting the number of data bits per pixel.
 *
 * Actual number of bits = dataBits + 1.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] dataBits Data bits value to be set.
 */
static inline void qRegRTZ_DataBitsSet(qRegRTZ_t baseAddr, UInt32 dataBits)
{
    GP_WB_WRITE_RTZ_DATA_BITS(baseAddr, dataBits);
}

/**
 * @brief Function for getting the number of data bits per pixel.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Data bits value. Actual number of bits = dataBits + 1.
 */
static inline UInt32 qRegRTZ_DataBitsGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_DATA_BITS(baseAddr);
}

/**
 * @brief Function for setting the number of pixels in the sequence.
 *
 * Actual number of pixels = pixelsCount + 1.
 *
 * @param[in] baseAddr    Register base address of the peripheral.
 * @param[in] pixelsCount Pixels count value to be set.
 */
static inline void qRegRTZ_PixelsCountSet(qRegRTZ_t baseAddr, UInt32 pixelsCount)
{
    GP_WB_WRITE_RTZ_PIXELS(baseAddr, pixelsCount);
}

/**
 * @brief Function for getting the number of pixels in the sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Pixels count value. Actual number of pixels = pixelsCount + 1.
 */
static inline Bool qRegRTZ_PixelsCountGet(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_PIXELS(baseAddr);
}

/**
 * @brief Function for setting the number of data bits that are sent
 * out, after the regular pixel sequence.
 *
 * Actual nr of trailing bits = trailingDataBits + 1.
 *
 * @param[in] baseAddr         Register base address of the peripheral.
 * @param[in] trailingDataBits Number of trailing data bits.
 */
static inline void qRegRTZ_TrailingDataBitsSet(qRegRTZ_t baseAddr, UInt32 trailingDataBits)
{
    GP_WB_WRITE_RTZ_TRAILING_DATA_BITS(baseAddr, trailingDataBits);
}

/**
 * @brief Function for getting the number of data bits that are sent
 * out, after the regular pixel sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval Number of trailing data bits. Actual nr of trailing bits = trailingDataBits + 1.
 */
static inline Bool qRegRTZ_TrailingDataBitsGet(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_TRAILING_DATA_BITS(baseAddr);
}

/**
 * @brief Function for getting the status of unmasked interrupt flag for TX buffer not full.
 * The interrupt is cleared implicitly when writing new TX data fills the buffer.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the unmasked interrupt occurred, false otherwise.
 */
static inline Bool qRegRTZ_IRQUnmaskedTxNotFullCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_UNMASKED_TX_NOT_FULL_INTERRUPT(baseAddr);
}

/**
 * @brief Function for getting the status of unmasked interrupt flag indicating a full sequence was completed.
 * Needs to be actively cleared.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the unmasked interrupt occurred, false otherwise.
 */
static inline Bool qRegRTZ_IRQUnmaskedSequenceDoneCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_UNMASKED_SEQUENCE_DONE_INTERRUPT(baseAddr);
}

/**
 * @brief Function for getting the status of unmasked interrupt flag indicating a full sequence was completed.
 * Needs to be actively cleared.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the unmasked interrupt occurred, false otherwise.
 */
static inline Bool qRegRTZ_IRQUnmaskedBufferUnderrunCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_UNMASKED_BUFFER_UNDERRUN_INTERRUPT(baseAddr);
}

/**
 * @brief Function for getting the status whether the send sequence is ongoing.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the send sequence is ongoing, false otherwise.
 */
static inline Bool qRegRTZ_SequenceBusyCheck(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_SEQUENCE_BUSY(baseAddr);
}

/**
 * @brief Function for setting the status of repeating the sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] enable   True if the sequence is to be repeated, false otherwise.
 */
static inline void qRegRTZ_RepeatSequenceEnableSet(qRegRTZ_t baseAddr, Bool enable)
{
    GP_WB_WRITE_RTZ_REPEAT_SEQUENCE(baseAddr, enable);
}

/**
 * @brief Function for getting the status of repeating the sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval True if the sequence is repeated, false otherwise.
 */
static inline Bool qRegRTZ_RepeatSequenceEnableGet(qRegRTZ_t baseAddr)
{
    return (Bool)GP_WB_READ_RTZ_REPEAT_SEQUENCE(baseAddr);
}

/**
 * @brief Function for triggering the start of the sequence.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegRTZ_SequenceTrigger(qRegRTZ_t baseAddr)
{
    GP_WB_RTZ_TRIGGER_SEQUENCE(baseAddr);
}

/**
 * @brief Function for clearing the sequence done interrupt.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegRTZ_IRQSequenceDoneClear(qRegRTZ_t baseAddr)
{
    GP_WB_RTZ_CLR_SEQUENCE_DONE_INTERRUPT(baseAddr);
}

/**
 * @brief Function for clearing the underrun interrupt (bad usage condition).
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegRTZ_IRQBufferUnderrunClear(qRegRTZ_t baseAddr)
{
    GP_WB_RTZ_CLR_BUFFER_UNDERRUN_INTERRUPT(baseAddr);
}

/**
 * @brief Function for clearing the tx buffer.
 * For correct operation, the RTZ sequence should not be busy when clearing the buffer.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 */
static inline void qRegRTZ_TxDataBufferClear(qRegRTZ_t baseAddr)
{
    GP_WB_RTZ_CLR_TX_DATA_BUFFER(baseAddr);
}

/**
 * @brief Function for setting the QUIC action channel.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 * @param[in] channel  Channel to be set to.
 */
static inline void qRegRTZ_QuicActionChannelSet(qRegRTZ_t baseAddr, UInt32 channel)
{
    GP_WB_WRITE_RTZ_QUIC_ACTION_CHANNEL(baseAddr, channel);
}

/**
 * @brief Function for getting the QUIC action channel.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval QUIC action channel.
 */
static inline UInt32 qRegRTZ_QuicActionChannelGet(qRegRTZ_t baseAddr)
{
    return (UInt32)GP_WB_READ_RTZ_QUIC_ACTION_CHANNEL(baseAddr);
}

/**
 * @brief Function for setting the type of the QUIC action.
 *
 * @param[in] baseAddr   Register base address of the peripheral.
 * @param[in] quicAction QUIC action to be set.
 */
static inline void qRegRTZ_QuicActionTypeSet(qRegRTZ_t baseAddr, qRegRTZ_QuicActionType_t quicActionType)
{
    GP_WB_WRITE_RTZ_QUIC_ACTION_TYPE(baseAddr, (UInt32)quicActionType);
}

/**
 * @brief Function for getting the type of the QUIC action.
 *
 * @param[in] baseAddr Register base address of the peripheral.
 *
 * @retval QUIC action type.
 */
static inline qRegRTZ_QuicActionType_t qRegRTZ_QuicActionTypeGet(qRegRTZ_t baseAddr)
{
    return (qRegRTZ_QuicActionType_t)GP_WB_READ_RTZ_QUIC_ACTION_TYPE(baseAddr);
}

#ifdef __cplusplus
}
#endif

#endif /* _REG_RTZ_H_ */
