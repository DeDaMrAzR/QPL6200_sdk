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

#ifndef _Q_DRV_DMA_H_
#define _Q_DRV_DMA_H_

/**
 * @defgroup Q_DRV_DMA DMA driver
 * @{
 *
 * @brief This is a driver for DMA peripheral. Typically it is used inside
 *        other drivers to provide the transfers without CPU interaction.
 *
 * @file qDrvDMA.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegDMA.h"
#include "qDrvCommon.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief DMA channel handle. */
typedef UInt8 qDrvDMA_Channel_t;

/** @enum qDrvDMA_ChannelMode_t */
//@{
/** @brief Peripheral to memory mode. */
#define qDrvDMA_ChannelModePeriToMem 0
/** @brief Memory to peripheral mode. */
#define qDrvDMA_ChannelModeMemToPeri 1
/** @typedef qDrvDMA_ChannelMode_t
 *  @brief Channel mode configuration.
 */
typedef UInt8 qDrvDMA_ChannelMode_t;
//@}

/** @enum qDrvDMA_BufferMode_t */
//@{
/** @brief Buffer mode static. Buffer is used only once after calling @p qDrvDMA_ChannelEnable().
 * Almost complete and complete events are disabled when events occurs and needs to be reenabled with
 * @p qDrvDMA_ChannelEnable() function. */
#define qDrvDMA_BufferModeSingleShot 0
/** @brief Buffer mode dynamic. Buffer is used multiple times after calling @p qDrvDMA_ChannelEnable().
 * Almost complete event is called each time after next @p almostCompleteThreshold words are transmitted.
 * Complete event is called only when buffer error occurs. */
#define qDrvDMA_BufferModeCircular 1
/** @typedef qDrvDMA_BufferMode_t
 *  @brief Buffer mode configuration.
 */
typedef UInt8 qDrvDMA_BufferMode_t;
//@}

/** @enum qDrvDMA_EventType_t */
//@{
/** @brief DMA channel error. Wrong channel configuration. */
#define qDrvDMA_EventTypeChannelError 0
/** @brief Event source buffer empty.
 * Is called if channel mode is @p qDrvDMA_ChannelModeMemToPeri and buffer mode is @p qDrvDMA_BufferModeSingleShot.
 */
#define qDrvDMA_EventTypeBufEmpty 1
/** @brief Event source buffer underrun.
 * Is called if channel mode is @p qDrvDMA_ChannelModeMemToPeri and buffer mode is @p qDrvDMA_BufferModeCircular. */
#define qDrvDMA_EventTypeBufUnderrun 2
/** @brief Event destination buffer full.
 * Is called if channel mode is @p qDrvDMA_ChannelModePeriToMem and buffer mode is @p qDrvDMA_BufferModeSingleShot.
 */
#define qDrvDMA_EventTypeBufFull 3
/** @brief Event destination buffer overflow.
 * Is called if channel mode is @p qDrvDMA_ChannelModePeriToMem and buffer mode is @p qDrvDMA_BufferModeCircular. */
#define qDrvDMA_EventTypeBufOverflow 4
/** @brief Event DMA channel almost-complete. */
#define qDrvDMA_EventTypeAlmostComplete 5
/** @typedef qDrvDMA_EventType_t
 *  @brief Event type.
 */
typedef UInt8 qDrvDMA_EventType_t;
//@}

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvDMA_QuicAction_t */
//@{
/** @brief No action. */
#define qDrvDMA_QuicActionNone qRegDMA_QuicActionNone
/** @brief Performs a copy, similar to when a trigger happens. */
#define qDrvDMA_QuicActionCopy qRegDMA_QuicActionCopy
/** @typedef qDrvDMA_QuicAction_t
 *  @brief Action which should happen to the DMA channel when the QUIC trigger arrives.
 */
typedef qRegDMA_QuicAction_t qDrvDMA_QuicAction_t;
//@}

/** @brief DMA SCB QUIC action configuration structure. */
typedef struct {
    qDrvDMA_Channel_t dmaChannel; /**< QUIC DMA action channel. */
    qDrvDMA_QuicAction_t action;  /**< QUIC action to be applied. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD(); /**< Required action field. */
} qDrvDMA_QuicActionConfig_t;
#endif                            /* REG_QUIC_PRESENT */

/** @brief DMA instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvDMA_t;

/**
 * @brief DMA channel complete event callback type.
 *
 * @param[in] arg       User-defined argument.
 * @param[in] eventType Event type.
 * @param[in] pChunk    Pointer to the data chunk.
 * @param[in] wordCount Number of words in the chunk.
 */
typedef void (*qDrvDMA_CbChannel_t)(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 wordCount);

/** @brief DMA copy error callback type. */
typedef void (*qDrvDMA_CbError_t)(void);

typedef struct {
    qDrvDMA_CbChannel_t callback;    /**< Event callback pointer. */
    UInt32 periAddress;              /**< Address of the peripheral register. */
    qDrvDMA_ChannelMode_t mode;      /**< Data transfer direction. */
    qDrvDMA_BufferMode_t bufferMode; /**< Buffer mode static or dynamic. */
    qRegDMA_WordMode_t wordMode;     /**< DMA word mode. 8, 16 or 32 bits. */
    UInt8 dmaTriggerSelect;          /**< DMA trigger source. */
    Bool noAdvance;                  /**< Do not advance the buffer pointer automatically. */
} qDrvDMA_ChannelConfig_t;

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the DMA instance.
 *
 * @param[in] pInstance DMA instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvDMA_Init(qDrvDMA_t* const pInstance);

/**
 * @brief Allocate the DMA channel.
 *
 * @returns Allocated channel handle or Q_NOCHANNEL when there is no free channels left.
 */
qDrvDMA_Channel_t qDrvDMA_Alloc();

/**
 * @brief Free previously allocated channel.
 *
 * @param[in] channel DMA channel.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 */
qResult_t qDrvDMA_ChannelFree(qDrvDMA_Channel_t channel);

/**
 * @brief Initialize the DMA channel.
 *
 * @param[in] channel        DMA channel.
 * @param[in] pChannelConfig Pointer to the DMA channel configuration structure to be applied.
 * @param[in] arg            User defined callback argument.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 * @retval Q_ERR_INVSTATE Channel is already initialized.
 */
qResult_t qDrvDMA_ChannelInit(qDrvDMA_Channel_t channel, const qDrvDMA_ChannelConfig_t* const pChannelConfig,
                              void* arg);

/**
 * @brief Deinitialize the DMA channel.
 *
 * @param[in] channel DMA channel.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 * @retval Q_ERR_INVSTATE Channel is not initialized.
 */
qResult_t qDrvDMA_ChannelDeinit(qDrvDMA_Channel_t channel);

/**
 * @brief Enable the DMA channel.
 *
 * @param[in] channel                 DMA channel.
 * @param[in] almostCompleteThreshold Word count threshold at which almost complete event occurs.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 * @retval Q_ERR_INVSTATE Channel is not initialized or DMA addresses are not set.
 */
qResult_t qDrvDMA_ChannelEnable(qDrvDMA_Channel_t channel, UInt16 almostCompleteThreshold);

/**
 * @brief Disable the DMA channel.
 *
 * @param[in] channel DMA channel
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 * @retval Q_ERR_INVSTATE Channel is not initialized.
 */
qResult_t qDrvDMA_ChannelDisable(qDrvDMA_Channel_t channel);

/**
 * @brief Check whether the DMA channel is enabled.
 *
 * @param[in] channel DMA channel.
 *
 * @return True if the channel is enabled, false otherwise.
 */
Bool qDrvDMA_ChannelEnabledCheck(qDrvDMA_Channel_t channel);

/**
 * @brief Set the memory buffer to be used by the DMA channel.
 *        This buffer can be used for receiving or transmitting data to the peripheral.
 *
 * @param[in] channel    DMA channel.
 * @param[in] pBuffer    Pointer to the buffer used by the DMA channel.
 * @param[in] bufferSize Buffer size in bytes.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated, wrong buffer size or wrong buffer alignment.
 * @retval Q_ERR_INVSTATE Channel is not initialized or channel is enabled.
 */
qResult_t qDrvDMA_BufferSet(qDrvDMA_Channel_t channel, void* pBuffer, UInt16 bufferSize);

/**
 * @brief Function for getting the pointer to the available chunk data in the buffer.
 *        When the DMA channel mode is @p qDrvDMA_ChannelModePeriToMem,
 *        function returns the pointer to the data, that should be read by the user.
 *        After reading the data, call @p qDrvDMA_Advance() to inform the driver
 *        that the buffer can be overriden by the new data.
 *
 *        When the DMA channel mode is @p qDrvDMA_ChannelModeMemToPeri,
 *        function return the pointer to the data, that can be overriden by the user.
 *        After overriding the data, call @p qDrvDMA_Advance() to inform the driver
 *        that the data can be transmitted to the peripheral.
 *
 * @param[in]  channel    DMA channel.
 * @param[out] pWordCount Pointer to variable where available words count will be stored.
 *
 * @retval NULL If in the @p qDrvDMA_ChannelModePeriToMem mode, there is no new data to be read.
 *              If in the @p qDrvDMA_ChannelModeMemToPeri mode, there is no space for new data to be transmitted.
 * @retval Pointer to the available buffer chunk.
 */
void* qDrvDMA_ContinuousChunkGet(qDrvDMA_Channel_t channel, UInt16* pWordCount);

/**
 * @brief Move read/write pointer and pass the buffer chunk to the DMA driver.
 *        When the DMA channel mode is @p qDrvDMA_ChannelModePeriToMem,
 *        the user should call this function after processing the incoming data, so the DMA can reuse the space.
 *
 *        When the DMA channel mode is @p qDrvDMA_ChannelModeMemToPeri,
 *        this function indicated a chunk ready to be transferred from this buffer and should not be modified
 *        by the user until channel complete/almost complete event.
 *
 * @param[in] channel   DMA channel
 * @param[in] wordCount Number of words to be advanced.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated or the @p wordCount is greater then the buffer size.
 * @retval Q_ERR_INVSTATE Channel is not initialized.
 */
qResult_t qDrvDMA_Advance(qDrvDMA_Channel_t channel, UInt16 wordCount);

/**
 * @brief Enable or disable the channel event interrupt.
 *        Can be used to temporarily silence channel events.
 *        This function must be called after channel is enabled with @p qDrvDMA_ChannelEnable().
 *
 * @param[in] channel DMA channel.
 * @param[in] irq     Channel event to be enabled.
 * @param[in] enable  True if the event is to be enabled, false otherwise.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Channel is not allocated.
 * @retval Q_ERR_INVSTATE Channel is not initialized.
 */
qResult_t qDrvDMA_ChannelIRQEnableSet(qDrvDMA_Channel_t channel, qRegDMA_ChannelIrq_t irq, Bool enable);

/**
 * @brief Check if the specified channel event interrupt is enabled.
 *
 * @param[in] channel DMA channel.
 * @param[in] irq     Channel event to be checked.
 *
 * @returns True if the event is enabled, false otherwise.
 */
Bool qDrvDMA_ChannelIRQEnableCheck(qDrvDMA_Channel_t channel, qRegDMA_ChannelIrq_t irq);

/**
 * @brief Check if the channel event interrupt is pending.
 *
 * @param[in] channel DMA channel.
 *
 * @return True if the event is pending, false otherwise.
 */
Bool qDrvDMA_ChannelCompeteCheck(qDrvDMA_Channel_t channel);

/**
 * @brief Set the DMA copy error callback handler.
 *
 * @param[in] callback Pointer to the callback function.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvDMA_ErrorCallbackSet(qDrvDMA_CbError_t callback);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_DMA_H_
