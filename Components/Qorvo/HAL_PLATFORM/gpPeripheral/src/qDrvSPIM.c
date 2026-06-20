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
#include "qDrvSPIM.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief SPIM instance control block */
typedef struct {
    qDrvSPIM_Callback_t callback;         /**< Set of user callbacks. */
    void* arg;                            /**< User-defined argument. */
    qDrvSPIM_Transfer_t transfer;         /**< Current transfer info. */
    UInt16 txPos;                         /**< Number of bytes send in current transaction (non-DMA mode). */
    UInt16 rxPos;                         /**< Number of bytes received in current transaction (non-DMA mode). */
    qDrvDMA_Channel_t dmaTxChannel;       /**< DMA channel for TX transfers. */
    qDrvDMA_Channel_t dmaRxChannel;       /**< DMA channel for RX transfers. */
    Bool dmaTxDone;                       /**< DMA TX transaction complete flag. */
    Bool dmaRxDone;                       /**< DMA RX transaction complete flag. */
    Bool active;                          /**< Transmission in progress. */
    qDrvSPIM_TransferMode_t transferMode; /**< Transfer mode. */
} qDrvSPIM_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvSPIM_ControlBlock_t) == sizeof(qDrvSPIM_InternalData_t));

/** @brief SPIM instance constant data block structure */
typedef struct {
    UInt32 dmaTriggerTx;        /**< DMA trigger select - TX FIFO not full. */
    UInt32 dmaTriggerRx;        /**< DMA trigger select - RX FIFO not empty. */
    UInt32 txDataAddress;       /**< TX data register address for DMA usage. */
    UInt32 rxDataAddress;       /**< RX data register address for DMA usage. */
    IRQn_Type irqn;             /**< Peripheral instance interrupt number. */
    qRegSysRam_Retain_t retain; /**< Instance index in SysRam retain register. */
} qDrvSPIM_ConstData_t;

/** @brief Macro for initializing const data block for SPIM instance. */
#define Q_DRV_SPIM_CONST_DATA_INIT(ID)                                                                                 \
    {                                                                                                                  \
        .dmaTriggerTx = Q_CONCAT(qRegDMA_Trigger, SPIM, TxNotFull),      /* */                                         \
            .dmaTriggerRx = Q_CONCAT(qRegDMA_Trigger, SPIM, RxNotEmpty), /* */                                         \
            .txDataAddress = Q_REG_SPIM_TXADDR_GET(ID),                  /* */                                         \
            .rxDataAddress = Q_REG_SPIM_RXADDR_GET(ID),                  /* */                                         \
            .irqn = Q_REG_SPIM_IRQN_GET(ID),                             /* */                                         \
            .retain = Q_REG_SYSRAM_RETAIN_GET(SPI_M),                    /* */                                         \
    }

/** @brief Constant data for SPIM instances. */
static const qDrvSPIM_ConstData_t spimConstData[REG_SPIM_INSTANCE_COUNT] = {
#if REG_SPIM_INSTANCE_COUNT > 0
    Q_DRV_SPIM_CONST_DATA_INIT(0),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static const qDrvSPIM_t* pInstances[REG_SPIM_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance SPIM instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvSPIM_ControlBlock_t* qDrvSPIM_ControlBlockGet(const qDrvSPIM_t* const pInstance)
{
    return (qDrvSPIM_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance SPIM instance pointer.
 *
 * @retval Const data block pointer.
 */
static inline const qDrvSPIM_ConstData_t* qDrvSPIM_ConstDataGet(const qDrvSPIM_t* const pInstance)
{
    return &spimConstData[pInstance->id];
}

/**
 * @brief Flush the SPIM RX buffer.
 *
 * @param[in] pInstance SPIM instance pointer.
 */
static void qDrvSPIM_RxFlush(const qDrvSPIM_t* const pInstance)
{
    while(qRegSPIM_IRQUnmaskedRxNotEmptyGet(pInstance->baseAddr))
    {
        qRegSPIM_RxDataGet(pInstance->baseAddr);
    }
    qRegSPIM_IRQRxOverrunClr(pInstance->baseAddr);
}

static inline Bool callCb(const qDrvSPIM_t* const pInstance)
{
    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);

    if(pCb->dmaRxDone && pCb->dmaTxDone && pCb->active)
    {
        pCb->active = false;
        pCb->dmaRxDone = false;
        pCb->dmaTxDone = false;

        if(pCb->callback)
        {
            pCb->callback(pCb->arg, &pCb->transfer);
        }

        return true;
    }

    return false;
}

/**
 * @brief SPIM DMA TX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the transmitted data chunk.
 * @param[in] chunkSize Size of the transmitted data chunk.
 */
static void qDrvSPIM_CbDmaTx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    (void)pChunk;
    (void)chunkSize;
    const qDrvSPIM_t* const pInstance = (const qDrvSPIM_t* const)arg;
    switch(eventType)
    {
        case qDrvDMA_EventTypeBufEmpty:
        {
            qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
            const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

            (void)qDrvDMA_ChannelDisable(pCb->dmaTxChannel);
            pCb->dmaTxDone = true;

            if(pCb->dmaRxDone)
            {
                /* RX is complete and TX FIFO full. Wait for the FIFO to flush. */
                qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegSPIM_IrqNotBusy), 0);
            }
            break;
        }
        default:
        {
            /* This should never happen. */
            Q_ASSERT(false);
            break;
        }
    }
}

/**
 * @brief SPIM DMA RX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the received data chunk.
 * @param[in] chunkSize Size of the received data chunk.
 */
static void qDrvSPIM_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    (void)pChunk;
    (void)chunkSize;
    const qDrvSPIM_t* const pInstance = (const qDrvSPIM_t* const)arg;
    switch(eventType)
    {
        case qDrvDMA_EventTypeBufFull:
        {
            qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
            const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

            (void)qDrvDMA_ChannelDisable(pCb->dmaRxChannel);
            pCb->dmaRxDone = true;

            if(pCb->dmaTxDone)
            {
                /* TX is complete and RX FIFO empty. */
                qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegSPIM_IrqNotBusy), 0);
            }
            break;
        }
        default:
        {
            /* This should never happen. */
            Q_ASSERT(false);
            break;
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvSPIM_PinConfigSet(const qDrvSPIM_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res;

    res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->sck.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->sck.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    if(pPinConfig->mosi.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->mosi.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    if(pPinConfig->miso.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->miso.pin, qDrvIOB_PullUp, false);
    }

    if(pPinConfig->nss.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->nss.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    return Q_OK;
}

qResult_t qDrvSPIM_PinConfigClear(const qDrvSPIM_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvSPIM_Init(const qDrvSPIM_t* pInstance, const qDrvSPIM_Config_t* const pConfig,
                        const qDrvSPIM_Callback_t callback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_SPIM_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    if(pInstances[pInstance->id])
    {
        return Q_ERR_INVSTATE;
    }

    const qDrvSPIM_TransferConfig_t* transferConfig = &pConfig->transferConfig;

    if(transferConfig->dataBits != qDrvSPIM_DataBits8)
    {
        /* Only 8-bit data width is supported. */
        return Q_ERR_NOTSUPPORTED;
    }

    memset(pCb, 0, sizeof(*pCb));

    if(pConfig->transferMode == qDrvSPIM_TransferModeDMA)
    {
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
    }
    else
    {
        pCb->dmaTxChannel = Q_NOCHANNEL;
        pCb->dmaRxChannel = Q_NOCHANNEL;
    }

    pCb->arg = arg;
    pCb->callback = callback;
    pCb->transferMode = pConfig->transferMode;

    // Clear RX fifo if not empty.
    qDrvSPIM_RxFlush(pInstance);

    if(pConfig->transferMode == qDrvSPIM_TransferModeDMA)
    {
        qDrvDMA_ChannelConfig_t txCfg = {
            .callback = &qDrvSPIM_CbDmaTx,
            .periAddress = pCData->txDataAddress,
            .mode = qDrvDMA_ChannelModeMemToPeri,
            .bufferMode = qDrvDMA_BufferModeSingleShot,
            .wordMode = qRegDMA_WordMode8Bit,
            .dmaTriggerSelect = pCData->dmaTriggerTx,
        };

        qDrvDMA_ChannelConfig_t rxCfg = {
            .callback = qDrvSPIM_CbDmaRx,
            .periAddress = pCData->rxDataAddress,
            .mode = qDrvDMA_ChannelModePeriToMem,
            .bufferMode = qDrvDMA_BufferModeSingleShot,
            .wordMode = qRegDMA_WordMode8Bit,
            .dmaTriggerSelect = pCData->dmaTriggerRx,
        };

        qResult_t resTx = qDrvDMA_ChannelInit(pCb->dmaTxChannel, &txCfg, (void*)pInstance);
        qResult_t resRx = qDrvDMA_ChannelInit(pCb->dmaRxChannel, &rxCfg, (void*)pInstance);

        if(resTx != Q_OK || resRx != Q_OK)
        {
            (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
            (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
            pCb->dmaTxChannel = Q_NOCHANNEL;
            pCb->dmaRxChannel = Q_NOCHANNEL;
            return (resTx != Q_OK) ? resTx : resRx;
        }
        qDrvSPIM_RxFlush(pInstance);
    }

    qRegSPIM_ClkFreqSet(pInstance->baseAddr, transferConfig->clkFreq);
    qRegSPIM_ModeSet(pInstance->baseAddr, transferConfig->mode);
    qRegSPIM_LsbFirstSet(pInstance->baseAddr, transferConfig->bitOrder);
    qRegSPIM_DataBitsSet(pInstance->baseAddr, transferConfig->dataBits);

    NVIC_SetPriority(pCData->irqn, irqPriority);

    NVIC_ClearPendingIRQ(pCData->irqn);
    NVIC_EnableIRQ(pCData->irqn);
    qRegIntCtrl_IRQEnable(pCData->irqn);

    qRegIntCtrl_EventIRQDisable(
        pCData->irqn, (1UL << qRegSPIM_IrqRxNotEmpty) | (1UL << qRegSPIM_IrqTxNotFull) | (1UL << qRegSPIM_IrqNotBusy),
        0);

    qSysRam_RetainEnableSet(pCData->retain, true);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvSPIM_Deinit(const qDrvSPIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_OK;
    }

    (void)qDrvSPIM_Abort(pInstance);

    if(pCb->dmaTxChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
        (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
        pCb->dmaTxChannel = Q_NOCHANNEL;
        pCb->dmaRxChannel = Q_NOCHANNEL;
    }

    qSysRam_RetainEnableSet(pCData->retain, false);
    NVIC_DisableIRQ(pCData->irqn);
    qRegIntCtrl_IRQDisable(pCData->irqn);
    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

qResult_t qDrvSPIM_TransferConfigSet(const qDrvSPIM_t* pInstance, const qDrvSPIM_TransferConfig_t* const pConfig)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);

    if(pCb->active)
    {
        return Q_ERR_BUSY;
    }

    if(pConfig->dataBits != qDrvSPIM_DataBits8)
    {
        /* Only 8-bit data width is supported. */
        return Q_ERR_NOTSUPPORTED;
    }

    qRegSPIM_ClkFreqSet(pInstance->baseAddr, pConfig->clkFreq);
    qRegSPIM_ModeSet(pInstance->baseAddr, pConfig->mode);
    qRegSPIM_LsbFirstSet(pInstance->baseAddr, pConfig->bitOrder);
    qRegSPIM_DataBitsSet(pInstance->baseAddr, pConfig->dataBits);

    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    qRegIntCtrl_EventIRQDisable(
        pCData->irqn, (1UL << qRegSPIM_IrqRxNotEmpty) | (1UL << qRegSPIM_IrqTxNotFull) | (1UL << qRegSPIM_IrqNotBusy),
        0);

    return Q_OK;
}

qResult_t qDrvSPIM_Transfer(const qDrvSPIM_t* pInstance, const qDrvSPIM_Transfer_t* const pTransfer)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pTransfer);

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->active)
    {
        return Q_ERR_BUSY;
    }

    if((pTransfer->txLen == 0) && (pTransfer->txBuf != NULL))
    {
        /* If the TX buffer is not NULL, the TX length must be greater than 0. */
        return Q_ERR_INVPARAM;
    }

    if((pTransfer->rxLen == 0) && (pTransfer->rxBuf != NULL))
    {
        /* If the RX buffer is not NULL, the RX length must be greater than 0. */
        return Q_ERR_INVPARAM;
    }

    if((pTransfer->txLen > 0) && (pTransfer->rxLen > 0) && (pTransfer->txLen < pTransfer->rxLen))
    {
        /* TX buffer must be greater than RX buffer. */
        return Q_ERR_INVPARAM;
    }

    memcpy(&pCb->transfer, pTransfer, sizeof(qDrvSPIM_Transfer_t));

    pCb->active = true;
    pCb->dmaTxDone = false;
    pCb->dmaRxDone = false;
    pCb->txPos = 0;
    pCb->rxPos = 0;

    if(pCb->transferMode == qDrvSPIM_TransferModeDMA)
    {
        qResult_t res;

        /* If TX buffer is not set, then only receive is expected.
         * But peripheral expect to transmit something to receive anything, so transmitting RX buffer is OK. */
        void* txBuffer = pTransfer->txLen ? (void*)pCb->transfer.txBuf : (void*)pCb->transfer.rxBuf;
        UInt16 txBufferLen = pTransfer->txLen ? pTransfer->txLen : pTransfer->rxLen;

        if(pTransfer->txLen == 0)
        {
            memset(pCb->transfer.rxBuf, 0, pTransfer->rxLen);
        }

        if(pCb->transfer.rxLen)
        {
            qRegSPIM_RxDataGet(pInstance->baseAddr);
            qRegSPIM_IRQRxOverrunClr(pInstance->baseAddr);

            res = qDrvDMA_BufferSet(pCb->dmaRxChannel, (void*)pCb->transfer.rxBuf,
                                    pCb->transfer.rxLen + ((pCb->transfer.rxLen == 1UL) ? 1UL : 0UL));
            if(res != Q_OK)
            {
                return res;
            }

            res = qDrvDMA_ChannelEnable(pCb->dmaRxChannel, 0);
            if(res != Q_OK)
            {
                return res;
            }
        }
        else
        {
            /* If RX buffer is not set, then only transmit is expected. */
            pCb->dmaRxDone = true;
        }

        res = qDrvDMA_BufferSet(pCb->dmaTxChannel, txBuffer, txBufferLen + ((txBufferLen == 1UL) ? 1UL : 0UL));
        if(res != Q_OK)
        {
            return res;
        }
        res = qDrvDMA_Advance(pCb->dmaTxChannel, pCb->transfer.txLen);
        if(res != Q_OK)
        {
            return res;
        }
        res = qDrvDMA_ChannelEnable(pCb->dmaTxChannel, 0);
        if(res != Q_OK)
        {
            return res;
        }
    }
    else if(pCb->transferMode == qDrvSPIM_TransferModeInterrupt)
    {
        qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegSPIM_IrqRxNotEmpty), 0);
        qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegSPIM_IrqTxNotFull), 0);
    }

    qRegSPIM_IRQTxEnableSet(pInstance->baseAddr, true);

    return Q_OK;
}

qResult_t qDrvSPIM_Abort(const qDrvSPIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    if(pInstances[pInstance->id] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!pCb->active)
    {
        return Q_ERR_INVSTATE;
    }

    if(pCb->dmaTxChannel != Q_NOCHANNEL)
    {
        qResult_t resTx = qDrvDMA_ChannelDisable(pCb->dmaTxChannel);
        qResult_t resRx = qDrvDMA_ChannelDisable(pCb->dmaRxChannel);

        if(resTx != Q_OK)
        {
            return resTx;
        }

        if(resRx != Q_OK)
        {
            return resRx;
        }
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegSPIM_IrqRxNotEmpty), 0);
    }
    pCb->active = false;

    return Q_OK;
}

Bool qDrvSPIM_BusyCheck(const qDrvSPIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);

    return (pInstances[pInstance->id] && pCb->active);
}

static void qDrvSPIM_IRQHandler(const qDrvSPIM_t* const pInstance)
{
    qDrvSPIM_ControlBlock_t* pCb = qDrvSPIM_ControlBlockGet(pInstance);
    const qDrvSPIM_ConstData_t* pCData = qDrvSPIM_ConstDataGet(pInstance);

    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(pCData->irqn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegSPIM_IrqRxNotEmpty))
    {
        UInt8 rxd = qRegSPIM_RxDataGet(pInstance->baseAddr);
        if(pCb->rxPos < pCb->transfer.rxLen)
        {
            pCb->transfer.rxBuf[pCb->rxPos] = rxd;
        }

        ++pCb->rxPos;

        if(pCb->rxPos >= pCb->transfer.txLen)
        {
            pCb->dmaRxDone = true;
            qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegSPIM_IrqRxNotEmpty), 0);
        }
    }

    if(maskedEvents & (1UL << qRegSPIM_IrqTxNotFull))
    {
        qRegSPIM_TxDataSet(pInstance->baseAddr, pCb->transfer.txBuf[pCb->txPos]);
        ++pCb->txPos;

        if(pCb->txPos >= pCb->transfer.txLen)
        {
            pCb->dmaTxDone = true;
            qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegSPIM_IrqTxNotFull), 0);
        }
    }

    if(pCb->transferMode == qDrvSPIM_TransferModeInterrupt)
    {
        callCb(pInstance);
    }

    if((maskedEvents & (1UL << qRegSPIM_IrqNotBusy)) && (pCb->transferMode == qDrvSPIM_TransferModeDMA))
    {
        /* End of transaction in DMA mode. */
        qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegSPIM_IrqNotBusy), 0);

        callCb(pInstance);
    }
}

#if REG_SPIM_INSTANCE_COUNT > 0
Q_REG_SPIM_HANDLER_DEFINE(0)
{
    qDrvSPIM_IRQHandler(pInstances[0]);
}
#endif /* REG_SPIM_INSTANCE_COUNT > 0 */
