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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qRegSysRam.h"
#include "qDrvDMA.h"
#include "qDrvSPIS.h"

/*****************************************************************************
 *                    Macro definitions
 *****************************************************************************/

#define Q_DRV_SPIS_TXLATENCY_DEFAULT 4

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief SPIS buffer definition. */
typedef struct {
    UInt8* pData; /**< Data pointer. */
    UInt16 len;   /**< Buffer length. */
} qDrvSPIM_Buffer_t;

/** @brief SPIS instance control block */
typedef struct {
    const qDrvSPIS_Callbacks_t* pCallbacks; /**< Set of user callbacks. */
    void* arg;                              /**< User-defined argument. */
    qDrvSPIM_Buffer_t rx;                   /**< RX buffer. */
    qDrvSPIM_Buffer_t tx;                   /**< TX buffer. */
    qDrvDMA_Channel_t dmaTxChannel;         /**< DMA channel for TX transfers. */
    qDrvDMA_Channel_t dmaRxChannel;         /**< DMA channel for RX transfers. */
    Bool enabled;                           /**< Peripheral enabled. */
} qDrvSPIS_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvSPIS_ControlBlock_t) == sizeof(qDrvSPIS_InternalData_t));

/** @brief SPIS instance constant data block structure */
typedef struct {
    UInt32 dmaTriggerTx;        /**< DMA trigger select - TX FIFO not full. */
    UInt32 dmaTriggerRx;        /**< DMA trigger select - RX FIFO not empty. */
    UInt32 txDataAddress;       /**< TX data register address for DMA usage. */
    UInt32 rxDataAddress;       /**< RX data register address for DMA usage. */
    IRQn_Type irqn;             /**< Peripheral instance interrupt number. */
    qRegSysRam_Retain_t retain; /**< Instance index in SysRam retain register. */
} qDrvSPIS_ConstData_t;

/** @brief Macro for initializing const data block for SPIS instance. */
#define Q_DRV_SPIS_CONST_DATA_INIT(ID)                                                                                 \
    {                                                                                                                  \
        .dmaTriggerTx = Q_CONCAT(qRegDMA_Trigger, SPISL, TxNotFull),      /* */                                        \
            .dmaTriggerRx = Q_CONCAT(qRegDMA_Trigger, SPISL, RxNotEmpty), /* */                                        \
            .txDataAddress = Q_REG_SPIS_TXADDR_GET(ID),                   /* */                                        \
            .rxDataAddress = Q_REG_SPIS_RXADDR_GET(ID),                   /* */                                        \
            .irqn = Q_REG_SPIS_IRQN_GET(ID),                              /* */                                        \
            .retain = Q_REG_SYSRAM_RETAIN_GET(SPI_SL),                    /* */                                        \
    }

/** @brief Constant data for SPIS instances. */
static const qDrvSPIS_ConstData_t spisConstData[REG_SPIS_INSTANCE_COUNT] = {
#if REG_SPIS_INSTANCE_COUNT > 0
    Q_DRV_SPIS_CONST_DATA_INIT(0),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static const qDrvSPIS_t* pInstances[REG_SPIS_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Control block pointer
 */
static inline qDrvSPIS_ControlBlock_t* qDrvSPIS_ControlBlockGet(const qDrvSPIS_t* const pInstance)
{
    return (qDrvSPIS_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance SPIS instance pointer.
 *
 * @retval Const data block pointer
 */
static inline const qDrvSPIS_ConstData_t* qDrvSPIS_ConstDataGet(const qDrvSPIS_t* const pInstance)
{
    return &spisConstData[pInstance->id];
}

/**
 * @brief SPIS DMA RX callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Data chunk pointer.
 * @param[in] chunkSize Chunk size.
 */
static void qDrvSPIS_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    const qDrvSPIS_t* const pInstance = (qDrvSPIS_t* const)arg;
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
            if(pCb->pCallbacks->rx)
            {
                pCb->pCallbacks->rx(pCb->arg, pChunk, chunkSize);
            }
            return;
        case qDrvDMA_EventTypeBufOverflow:
            if(pCb->pCallbacks->error)
            {
                pCb->pCallbacks->error(pCb->arg, qDrvSPIS_ErrorRxOverrun);
            }
            return;
        default:
            // This should never happen.
            Q_ASSERT(false);
            return;
    }
}

/**
 * @brief SPIS DMA TX callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the transmitted data chunk.
 * @param[in] chunkSize Size of the transmitted data chunk.
 */
static void qDrvSPIS_CbDmaTx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    const qDrvSPIS_t* const pInstance = (const qDrvSPIS_t* const)arg;
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    switch(eventType)
    {
        case qDrvDMA_EventTypeBufEmpty:
            (void)qDrvDMA_ChannelDisable(pCb->dmaTxChannel);

            pCb->tx.pData = NULL;
            pCb->tx.len = 0;

            if(pCb->pCallbacks->tx)
            {
                pCb->pCallbacks->tx(pCb->arg, pChunk, chunkSize);
            }
            return;
        default:
            // This should never happen.
            Q_ASSERT(false);
            return;
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvSPIS_TxLatencySet(const qDrvSPIS_t* pInstance, UInt8 txLatency)
{
    Q_ASSERT(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_INVSTATE;
    }

    if(txLatency)
    {
        /* The actual latency is SPI_SL_TX_LATENCY + 1. */
        --txLatency;
    }

    qRegSPIS_TxLatencySet(pInstance->baseAddr, txLatency);

    return Q_OK;
}

qResult_t qDrvSPIS_Init(const qDrvSPIS_t* pInstance, const qDrvSPIS_Config_t* const pConfig,
                        const qDrvSPIS_Callbacks_t* const pCallbacks, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_SPIS_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);
    const qDrvSPIS_ConstData_t* pCData = qDrvSPIS_ConstDataGet(pInstance);

    if(pInstances[pInstance->id])
    {
        return Q_ERR_INVSTATE;
    }

    if(pCallbacks == NULL)
    {
        /* Missing callbacks. */
        return Q_ERR_INVPARAM;
    }

    memset(pCb, 0, sizeof(*pCb));

    pCb->dmaTxChannel = qDrvDMA_Alloc();
    if(pCb->dmaTxChannel == Q_NOCHANNEL)
    {
        return Q_ERR_OUTOFMEMORY;
    }

    pCb->dmaRxChannel = qDrvDMA_Alloc();
    if(pCb->dmaRxChannel == Q_NOCHANNEL)
    {
        qDrvDMA_ChannelFree(pCb->dmaTxChannel);
        pCb->dmaTxChannel = Q_NOCHANNEL;
        return Q_ERR_OUTOFMEMORY;
    }

    qRegSPIS_TxLatencySet(pInstance->baseAddr, Q_DRV_SPIS_TXLATENCY_DEFAULT - 1);
    qRegSPIS_ClrTxOnDeselectSet(pInstance->baseAddr, pConfig->clearTxOnDeselect);
    qRegSPIS_ClrRxOnDeselectSet(pInstance->baseAddr, pConfig->clearRxOnDeselect);
    qRegSPIS_RxFifoClr(pInstance->baseAddr);
    qRegSPIS_TxFifoClr(pInstance->baseAddr);
    qRegSPIS_IRQTxUnderrunClr(pInstance->baseAddr);
    qRegSPIS_IRQRxOverrunClr(pInstance->baseAddr);

    qDrvDMA_ChannelConfig_t rxCfg = {
        .callback = qDrvSPIS_CbDmaRx,
        .periAddress = pCData->rxDataAddress,
        .mode = qDrvDMA_ChannelModePeriToMem,
        .bufferMode = qDrvDMA_BufferModeCircular,
        .wordMode = qRegDMA_WordMode8Bit,
        .dmaTriggerSelect = pCData->dmaTriggerRx,
    };

    qDrvDMA_ChannelConfig_t txCfg = {
        .callback = qDrvSPIS_CbDmaTx,
        .periAddress = pCData->txDataAddress,
        .mode = qDrvDMA_ChannelModeMemToPeri,
        .bufferMode = qDrvDMA_BufferModeSingleShot,
        .wordMode = qRegDMA_WordMode8Bit,
        .dmaTriggerSelect = pCData->dmaTriggerTx,
    };

    qResult_t resultTx = qDrvDMA_ChannelInit(pCb->dmaTxChannel, &txCfg, (void*)pInstance);
    qResult_t resultRx = qDrvDMA_ChannelInit(pCb->dmaRxChannel, &rxCfg, (void*)pInstance);

    if(resultTx != Q_OK || resultRx != Q_OK)
    {
        (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
        (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
        pCb->dmaTxChannel = Q_NOCHANNEL;
        pCb->dmaRxChannel = Q_NOCHANNEL;
        return (resultTx != Q_OK) ? resultTx : resultRx;
    }

    UInt32 eventMask = (1UL << qRegSPIS_IrqTxEmpty) | (1UL << qRegSPIS_IrqTxNotFull) | (1UL << qRegSPIS_IrqTxUnderrun) |
                       (1UL << qRegSPIS_IrqRxNotEmpty) | (1UL << qRegSPIS_IrqRxOverrun);
    qRegIntCtrl_EventIRQDisable(pCData->irqn, eventMask, 0);
    qRegIntCtrl_IRQDisable(pCData->irqn);
    NVIC_SetPriority(pCData->irqn, irqPriority);
    NVIC_ClearPendingIRQ(pCData->irqn);
    NVIC_DisableIRQ(pCData->irqn);
    qSysRam_RetainEnableSet(pCData->retain, true);

    pCb->arg = arg;
    pCb->pCallbacks = pCallbacks;
    pCb->enabled = false;
    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvSPIS_Deinit(const qDrvSPIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);
    const qDrvSPIS_ConstData_t* pCData = qDrvSPIS_ConstDataGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_OK;
    }

    (void)qDrvSPIS_Disable(pInstance);

    if(pCb->dmaTxChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
        pCb->dmaTxChannel = Q_NOCHANNEL;
    }

    if(pCb->dmaRxChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
        pCb->dmaRxChannel = Q_NOCHANNEL;
    }

    qSysRam_RetainEnableSet(pCData->retain, false);
    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

qResult_t qDrvSPIS_RxBufferSet(qDrvSPIS_t* const pInstance, UInt8* pBuf, UInt16 bufLen)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pBuf);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->dmaRxChannel == Q_NOCHANNEL)
    {
        return Q_ERR_INVSTATE;
    }

    if(pCb->enabled)
    {
        return Q_ERR_BUSY;
    }

    if(bufLen <= 1UL)
    {
        /* Single-byte buffer size is not supported. */
        return Q_ERR_INVPARAM;
    }

    qResult_t result = qDrvDMA_BufferSet(pCb->dmaRxChannel, (void*)pBuf, bufLen);

    if(result != Q_OK)
    {
        return result;
    }

    pCb->rx.pData = pBuf;
    pCb->rx.len = bufLen;

    return Q_OK;
}

qResult_t qDrvSPIS_Enable(const qDrvSPIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->dmaRxChannel == Q_NOCHANNEL || !pCb->rx.pData || pCb->enabled)
    {
        return Q_ERR_INVSTATE;
    }

    qRegSPIS_RxFifoClr(pInstance->baseAddr);
    qRegSPIS_TxFifoClr(pInstance->baseAddr);

    qResult_t result = qDrvDMA_ChannelEnable(pCb->dmaRxChannel, 1);
    if(result != Q_OK)
    {
        return result;
    }

    qRegSPIS_EnableSet(pInstance->baseAddr, true);

    pCb->enabled = true;

    return Q_OK;
}

qResult_t qDrvSPIS_Disable(const qDrvSPIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!pCb->enabled)
    {
        return Q_ERR_INVSTATE;
    }

    (void)qDrvSPIS_TxAbort(pInstance);
    qRegSPIS_EnableSet(pInstance->baseAddr, false);
    qDrvDMA_ChannelDisable(pCb->dmaRxChannel);

    pCb->enabled = false;

    return Q_OK;
}

qResult_t qDrvSPIS_PinConfigSet(const qDrvSPIS_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->sck.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->sck.pin, qDrvIOB_PullUp, false);
    }

    if(pPinConfig->mosi.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->mosi.pin, qDrvIOB_PullUp, false);
    }

    if(pPinConfig->miso.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->miso.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    if(pPinConfig->ss.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->ss.pin, qDrvIOB_PullUp, false);
    }

    return Q_OK;
}

qResult_t qDrvSPIS_PinConfigClear(const qDrvSPIS_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvSPIS_Tx(qDrvSPIS_t* const pInstance, UInt8* pBuf, UInt16 bufLen)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pBuf);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(!bufLen)
    {
        return Q_ERR_INVPARAM;
    }

    if(!pCb->enabled)
    {
        return Q_ERR_BUSY;
    }

    if(pCb->tx.pData)
    {
        return Q_ERR_INVSTATE;
    }

    pCb->tx.pData = pBuf;
    pCb->tx.len = bufLen;

    qDrvDMA_BufferSet(pCb->dmaTxChannel, (void*)pCb->tx.pData, pCb->tx.len);
    qDrvDMA_Advance(pCb->dmaTxChannel, pCb->tx.len);
    qResult_t result = qDrvDMA_ChannelEnable(pCb->dmaTxChannel, 0);
    if(result != Q_OK)
    {
        pCb->tx.pData = NULL;
        return result;
    }

    return Q_OK;
}

qResult_t qDrvSPIS_TxAbort(const qDrvSPIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!pCb->enabled)
    {
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelDisable(pCb->dmaTxChannel);
    qRegSPIS_TxFifoClr(pInstance->baseAddr);

    return Q_OK;
}

Bool qDrvSPIS_TxBusyCheck(const qDrvSPIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIS_ControlBlock_t* pCb = qDrvSPIS_ControlBlockGet(pInstance);

    if(pInstances[pInstance->id] == NULL || !pCb->enabled)
    {
        return false;
    }

    return (pCb->tx.pData != NULL);
}
