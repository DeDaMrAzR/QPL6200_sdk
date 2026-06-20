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

#ifndef _Q_DRV_DMADCB_H_
#define _Q_DRV_DMADCB_H_

/**
 * @defgroup Q_DRV_DMADCB_H DMA dual circular buffer driver
 * @{
 *
 * @brief This is a driver for DMA dual circular buffer.
 *
 * @file qDrvDMADCB.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegDMADCB.h"
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
typedef UInt8 qDrvDMADCB_Channel_t;

/** @enum qDrvDMADCB_WordMode_t */
//@{
/** @brief 8-bit word. */
#define qDrvDMADCB_WordMode8Bit qRegDMADCB_WordMode8Bit
/** @brief 16-bit word. */
#define qDrvDMADCB_WordMode16Bit qRegDMADCB_WordMode16Bit
/** @brief 32-bit word. */
#define qDrvDMADCB_WordMode32Bit qRegDMADCB_WordMode32Bit
/** @typedef qDrvDMADCB_WordMode_t
 *  @brief DMA DCB word size. 8-, 16- or 32-bit memory accesses.
 */
typedef qRegDMADCB_WordMode_t qDrvDMADCB_WordMode_t;
//@}

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvDMADCB_QuicAction_t */
//@{
/** @brief No action. */
#define qDrvDMADCB_QuicActionNone qRegDMADCB_QuicActionNone
/** @brief Reset source pointer. */
#define qDrvDMADCB_QuicActionResetSrc qRegDMADCB_QuicActionResetSrc
/** @brief Reset destination pointer. */
#define qDrvDMADCB_QuicActionResetDest qRegDMADCB_QuicActionResetDest
/** @brief Reset source and destination pointer. */
#define qDrvDMADCB_QuicActionResetBoth qRegDMADCB_QuicActionResetBoth
/** @typedef qDrvDMADCB_QuicAction_t
 *  @brief Action triggered by QUIC signal.
 */
typedef qRegDMADCB_QuicAction_t qDrvDMADCB_QuicAction_t;
//@}

/** @brief DMA DCB QUIC action configuration structure. */
typedef struct {
    qDrvDMADCB_Channel_t dmaChannel;    /**< DMA channel. */
    UInt8 actionIdx;                    /**< QUIC DMA action channel. */
    qDrvDMADCB_QuicAction_t action;     /**< QUIC action to be applied. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD(); /**< Required action field. */
} qDrvDMADCB_QuicActionConfig_t;
#endif                                  /* REG_QUIC_PRESENT */

/** @brief DMA DCB instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvDMADCB_t;

/**
 * @brief DMA DCB channel complete event callback type.
 *
 * @param[in] arg     User defined callback argument.
 * @param[in] channel DMA DCB channel.
 */
typedef void (*qDrvDMADCB_CbComplete_t)(void* arg, qDrvDMADCB_Channel_t channel);

/** @brief DMA DCB copy error callback type. */
typedef void (*qDrvDMADCB_CbError_t)(void);

/** @brief DMA DCB channel configuration structure. */
typedef struct {
    qDrvDMADCB_CbComplete_t callback; /**< Event callback pointer. */
    qDrvDMADCB_WordMode_t wordMode;   /**< DMA word mode. 8, 16 or 32 bits. */
} qDrvDMADCB_ChannelConfig_t;

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Initialize the DMA DCB instance.
 *
 * @param[in] pInstance DMA DCB instance pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance already initialized.
 */
qResult_t qDrvDMADCB_Init(qDrvDMADCB_t* const pInstance);

/**
 * @brief Deinitialize the DMA DCB instance.
 *
 * @param[in] pInstance DMA DCB instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvDMADCB_Deinit(qDrvDMADCB_t* const pInstance);

/**
 * @brief Check whether the DMA DCB instance is initialized.
 *
 * @param[in] pInstance DMA DCB instance pointer.
 *
 * @return True if the instance is initialized, false otherwise.
 */
Bool qDrvDMADCB_InitCheck(qDrvDMADCB_t* const pInstance);

/**
 * @brief Allocate the DMA DCB channel.
 *
 * @returns Allocated channel handle or Q_NOCHANNEL when there is no free channels left
 *          or the instance is not initialized.
 */
qDrvDMADCB_Channel_t qDrvDMADCB_ChannelAlloc(void);

/**
 * @brief Free the previously allocated DMA DCB channel.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ChannelFree(qDrvDMADCB_Channel_t channel);

/**
 * @brief Initialize the DMA DCB channel.
 *
 * @param[in] channel        DMA DCB channel.
 * @param[in] pChannelConfig Pointer to the DMA DCB channel configuration structure.
 * @param[in] arg            User defined callback argument.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated or already initialized.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ChannelInit(qDrvDMADCB_Channel_t channel, const qDrvDMADCB_ChannelConfig_t* const pChannelConfig,
                                 void* arg);

/**
 * @brief Deinitialize the DMA DCB channel.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated or initialized.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ChannelDeinit(qDrvDMADCB_Channel_t channel);

/**
 * @brief Check whether the DMA DCB channel is initialized.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @return True if the channel is initialized, false otherwise.
 */
Bool qDrvDMADCB_ChannelInitCheck(qDrvDMADCB_Channel_t channel);

/**
 * @brief Enable the DMA DCB channel.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated or initialized or buffers are not set.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ChannelEnable(qDrvDMADCB_Channel_t channel);

/**
 * @brief Disable the DMA DCB channel.
 *
 * @param[in] channel DMA DCB channel
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated / not initialized.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ChannelDisable(qDrvDMADCB_Channel_t channel);

/**
 * @brief Check whether the DMA DCB channel is enabled.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @return True if the channel is enabled, false otherwise.
 */
Bool qDrvDMADCB_ChannelEnabledCheck(qDrvDMADCB_Channel_t channel);

/**
 * @brief Set the source and destination buffers.
 *
 * @param[in] channel     DMA DCB channel.
 * @param[in] pSrcBuf     Source buffer pointer.
 * @param[in] srcBufSize  Source buffer length in bytes.
 * @param[in] pDestBuf    Destination buffer pointer.
 * @param[in] destBufSize Destination buffer length in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated / not initialized or invalid buffer parameters.
 * @retval Q_ERR_BUSY          The copy operation is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_BufferSet(qDrvDMADCB_Channel_t channel, void* pSrcBuf, UInt16 srcBufSize, void* pDestBuf,
                               UInt16 destBufSize);

/**
 * @brief Advance the source buffer write pointer to indicate the new data
 *        available for DMA to copy.
 *
 * @param[in] channel   DMA DCB channel.
 * @param[in] wordCount Number of words to be advanced.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated / not initialized.
 * @retval Q_ERR_OUTOFMEMORY   @p wordCount is greater then the available buffer size.
 * @retval Q_ERR_BUSY          The copy operation is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_SrcAdvance(qDrvDMADCB_Channel_t channel, UInt16 wordCount);

/**
 * @brief Advance the destination buffer read pointer to indicate the incoming data
 *        has been processed and is available for DMA to write.
 *
 * @param[in] channel   DMA DCB channel.
 * @param[in] wordCount Number of words to be advanced.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Channel is not allocated / not initialized.
 * @retval Q_ERR_OUTFOMEMORY   @p wordCount is greater then the available data length.
 * @retval Q_ERR_BUSY          The copy operation is already in progress.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_DestAdvance(qDrvDMADCB_Channel_t channel, UInt16 wordCount);

/**
 * @brief Check the channel transfer state.
 *
 * @param[in] channel DMA DCB channel.
 *
 * @return True if there is a copy operation in progress, false otherwise.
 */
Bool qDrvDMADCB_ChannelBusyCheck(qDrvDMADCB_Channel_t channel);

/**
 * @brief Set the DMA DCB copy error callback handler.
 *
 * @param[in] callback Pointer to the callback function. NULL value removes the callback if set.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvDMADCB_ErrorCallbackSet(qDrvDMADCB_CbError_t callback);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_DMADCB_H_
