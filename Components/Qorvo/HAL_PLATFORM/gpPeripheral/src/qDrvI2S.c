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
#include "qRegStandby.h"
#include "qRegSysRam.h"
#include "qDrvI2S.h"
#include "qDrvIOB.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief I2S instance control block. */
typedef struct {
    qDrvI2S_Callbacks_t* pCallbacks;     /**< Set of user callbacks. */
    void* arg;                           /**< User-defined argument. */
    qDrvDMA_Channel_t dmaTxChannel;      /**< DMA channel for TX transfers. */
    qDrvDMA_Channel_t dmaRxChannel;      /**< DMA channel for RX transfers. */
    qDrvI2S_Mode_t mode;                 /**< I2S mode. */
    qDrvI2S_TransferMode_t transferMode; /**< I2S transmitter and receiver mode configuration. */
    UInt8* txBuffer;                     /**< TX buffer pointer. */
    UInt8* rxBuffer;                     /**< RX buffer pointer. */
    UInt16 txBufferSize;                 /**< TX buffer size. */
    UInt16 txSampleCount;                /**< TX sample count. */
    UInt16 rxBufferSize;                 /**< RX buffer size. */
    UInt16 rxSampleCount;                /**< RX sample count. */
} qDrvI2S_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvI2S_ControlBlock_t) == sizeof(qDrvI2S_InternalData_t));

/** @brief I2S instance constant data block structure */
typedef struct {
    UInt32 dmaTriggerTx;               /**< DMA trigger select - TX FIFO not full. */
    UInt32 dmaTriggerRx;               /**< DMA trigger select - RX FIFO not empty. */
    UInt32 txDataAddress;              /**< TX data register address for DMA usage. */
    UInt32 rxDataAddress;              /**< RX data register address for DMA usage. */
    IRQn_Type irqn;                    /**< Peripheral instance interrupt number. */
    qRegSysRam_Retain_t retain;        /**< Instance index in SysRam retain register. */
    qRegStandby_ResetFunc_t resetFunc; /**< Reset function for the peripheral. */
} qDrvI2S_ConstData_t;

/** @brief Macro for initializing const data block for I2S instance. */
#define Q_DRV_I2S_CONST_DATA_INIT(ID)                                                                                  \
    {                                                                                                                  \
        .dmaTriggerTx = Q_REG_I2S_TRIGGER_TX_NOT_FULL_GET(ID),        /* */                                            \
            .dmaTriggerRx = Q_REG_I2S_TRIGGER_RX_NOT_EMPTY_GET(ID),   /* */                                            \
            .txDataAddress = Q_REG_I2S_TXADDR_GET(ID),                /* */                                            \
            .rxDataAddress = Q_REG_I2S_RXADDR_GET(ID),                /* */                                            \
            .irqn = Q_REG_I2S_IRQN_GET(ID),                           /* */                                            \
            .retain = Q_REG_SYSRAM_RETAIN_GET(Q_CONCAT(I2S_, ID)),    /* */                                            \
            .resetFunc = Q_CONCAT(qRegStandby_ResetFuncPeripI2s, ID), /* */                                            \
    }

/** @brief Constant data for I2S instances. */
static const qDrvI2S_ConstData_t I2SConstData[REG_I2S_INSTANCE_COUNT] = {
#if REG_I2S_INSTANCE_COUNT > 0
    Q_DRV_I2S_CONST_DATA_INIT(0),
#endif
#if REG_I2S_INSTANCE_COUNT > 1
    Q_DRV_I2S_CONST_DATA_INIT(1),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static const qDrvI2S_t* pInstances[REG_I2S_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance Instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvI2S_ControlBlock_t* qDrvI2S_ControlBlockGet(qDrvI2S_t* const pInstance)
{
    return (qDrvI2S_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance Instance pointer.
 *
 * @retval Control block pointer.
 */
static inline const qDrvI2S_ConstData_t* qDrvI2S_ConstDataGet(const qDrvI2S_t* const pInstance)
{
    return &I2SConstData[pInstance->id];
}

/**
 * @brief I2S DMA TX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the already transmitted chunk of data.
 * @param[in] chunkSize Size of the transmitted chunk.
 */
static void qDrvI2S_CbDmaTx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
        {
            qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet((qDrvI2S_t* const)arg);
            /* Inform the user that already transmitted data can be overridden for the next transmission. */
            pCb->pCallbacks->tx(pCb->arg, (UInt8*)pChunk, chunkSize);
            break;
        }
        case qDrvDMA_EventTypeBufUnderrun:
        {
            /* This should never happen. */
            Q_ASSERT(false);
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
 * @brief I2S DMA TX transmission callback handler.
 *
 * @param[in] arg       Used-supplied argument.
 * @param[in] eventType DMA event.
 * @param[in] pChunk    Pointer to the already received chunk of data.
 * @param[in] chunkSize Size of the received chunk.
 */
static void qDrvI2S_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
        {
            qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet((qDrvI2S_t* const)arg);
            /* Inform the user that new data are available in the buffer. */
            pCb->pCallbacks->rx(pCb->arg, pChunk, chunkSize);
            break;
        }
        case qDrvDMA_EventTypeBufOverflow:
        {
            /* This should never happen. */
            Q_ASSERT(false);
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

qResult_t qDrvI2S_PinConfigSet(const qDrvI2S_PinConfig_t* const pPinConfig, qDrvI2S_Mode_t mode)
{
    Q_ASSERT(pPinConfig);

    if((pPinConfig->sck.pin == Q_NOPIN) || (pPinConfig->ws.pin == Q_NOPIN))
    {
        /* SCK and WS pins must me configured. */
        return Q_ERR_INVPARAM;
    }

    if((mode & Q_DRV_I2S_MODE_TX_MASK) && (pPinConfig->sdo.pin == Q_NOPIN))
    {
        /* SDO pin must be configured for transmitter mode. */
        return Q_ERR_INVPARAM;
    }

    if((mode & Q_DRV_I2S_MODE_RX_MASK) && (pPinConfig->sdi.pin == Q_NOPIN))
    {
        /* SDI pin must be configured for receiver mode. */
        return Q_ERR_INVPARAM;
    }

    if(!!(mode & Q_DRV_I2S_MODE_MASTER_MASK) == !!(mode & Q_DRV_I2S_MODE_SLAVE_MASK))
    {
        /* Master or slave mode must be set. */
        return Q_ERR_INVPARAM;
    }

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    if(mode & Q_DRV_I2S_MODE_MASTER_MASK)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->sck.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
        qDrvIOB_ConfigOutputSet(pPinConfig->ws.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    if(mode & Q_DRV_I2S_MODE_SLAVE_MASK)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->sck.pin, qDrvIOB_PullNone, false);
        qDrvIOB_ConfigInputSet(pPinConfig->ws.pin, qDrvIOB_PullNone, false);
    }

    if(mode & Q_DRV_I2S_MODE_TX_MASK)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->sdo.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    if(mode & Q_DRV_I2S_MODE_RX_MASK)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->sdi.pin, qDrvIOB_PullNone, false);
    }

    if(pPinConfig->refIn.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->refIn.pin, qDrvIOB_PullNone, false);
    }

    return Q_OK;
}

qResult_t qDrvI2S_PinConfigClear(const qDrvI2S_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvI2S_Init(qDrvI2S_t* const pInstance, const qDrvI2S_Config_t* const pConfig,
                       qDrvI2S_Callbacks_t* const pCallbacks, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(qDrvI2S_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if((pConfig->prescaler != Q_DRV_I2S_PRESCALER_NONE) && (pConfig->prescaler > Q_DRV_I2S_PRESCALER_MAX))
    {
        /* Invalid prescaler value. */
        return Q_ERR_INVPARAM;
    }

    if(!(pConfig->mode & Q_DRV_I2S_MODE_TX_MASK) && !(pConfig->mode & Q_DRV_I2S_MODE_RX_MASK))
    {
        /* At least one mode must be enabled. */
        return Q_ERR_INVPARAM;
    }

    if(pConfig->transferMode == qDrvI2S_TransferModePolling &&
       ((pConfig->mode & Q_DRV_I2S_MODE_TX_MASK) && (pConfig->mode & Q_DRV_I2S_MODE_RX_MASK)))
    {
        /* Polling mode is not supported for full duplex operation. */
        return Q_ERR_INVPARAM;
    }

    if(!!(pConfig->mode & Q_DRV_I2S_MODE_MASTER_MASK) == !!(pConfig->mode & Q_DRV_I2S_MODE_SLAVE_MASK))
    {
        /* Master or slave mode must be set. */
        return Q_ERR_INVPARAM;
    }

    if((pConfig->mode & Q_DRV_I2S_MODE_TX_MASK) && !(pConfig->txBytes.left || pConfig->txBytes.right))
    {
        /* Transmit mode set without bytes. */
        return Q_ERR_INVPARAM;
    }

    if((pConfig->mode & Q_DRV_I2S_MODE_RX_MASK) && !(pConfig->rxBytes.left || pConfig->rxBytes.right))
    {
        /* Receive mode set without bytes. */
        return Q_ERR_INVPARAM;
    }

    if(pConfig->transferMode != qDrvI2S_TransferModePolling)
    {
        if(pCallbacks == NULL)
        {
            /* Missing callbacks for asynchronous modes. */
            return Q_ERR_INVPARAM;
        }

        if((pCallbacks->rx == NULL) && (pConfig->mode & Q_DRV_I2S_MODE_RX_MASK))
        {
            /* Missing RX callback. */
            return Q_ERR_INVPARAM;
        }

        if((pCallbacks->tx == NULL) && (pConfig->mode & Q_DRV_I2S_MODE_TX_MASK))
        {
            /* Missing TX callback. */
            return Q_ERR_INVPARAM;
        }
    }

    /* Disable the peripheral. */
    qRegI2S_EnableSet(pInstance->baseAddr, false);

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    memset(pCb, 0, sizeof(*pCb));

    if((pConfig->transferMode != qDrvI2S_TransferModeDMA) || !(pConfig->mode & Q_DRV_I2S_MODE_TX_MASK))
    {
        /* Disable transmit DMA channel. */
        pCb->dmaTxChannel = Q_NOCHANNEL;
    }

    if((pConfig->transferMode != qDrvI2S_TransferModeDMA) || !(pConfig->mode & Q_DRV_I2S_MODE_RX_MASK))
    {
        /* Disable receive DMA channel. */
        pCb->dmaRxChannel = Q_NOCHANNEL;
    }

    if(pConfig->transferMode == qDrvI2S_TransferModeDMA)
    {
        /* Allocate TX DMA channel. */
        if(pConfig->mode & Q_DRV_I2S_MODE_TX_MASK)
        {
            pCb->dmaTxChannel = qDrvDMA_Alloc();
            if(pCb->dmaTxChannel == Q_NOCHANNEL)
            {
                return Q_ERR_OUTOFMEMORY;
            }

            qDrvDMA_ChannelConfig_t txCfg = {
                .callback = &qDrvI2S_CbDmaTx,
                .periAddress = qDrvI2S_ConstDataGet(pInstance)->txDataAddress,
                .mode = qDrvDMA_ChannelModeMemToPeri,
                .bufferMode = qDrvDMA_BufferModeCircular,
                .wordMode = qRegDMA_WordMode8Bit,
                .dmaTriggerSelect = qDrvI2S_ConstDataGet(pInstance)->dmaTriggerTx,
            };

            if(qDrvDMA_ChannelInit(pCb->dmaTxChannel, &txCfg, (void*)pInstance) != Q_OK)
            {
                (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
                pCb->dmaTxChannel = Q_NOCHANNEL;
                return Q_ERR_OUTOFMEMORY;
            }
        }

        /* Allocate RX DMA channel. */
        if(pConfig->mode & Q_DRV_I2S_MODE_RX_MASK)
        {
            pCb->dmaRxChannel = qDrvDMA_Alloc();
            if(pCb->dmaRxChannel == Q_NOCHANNEL)
            {
                (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
                pCb->dmaTxChannel = Q_NOCHANNEL;
                return Q_ERR_OUTOFMEMORY;
            }

            qDrvDMA_ChannelConfig_t rxCfg = {
                .callback = qDrvI2S_CbDmaRx,
                .periAddress = qDrvI2S_ConstDataGet(pInstance)->rxDataAddress,
                .mode = qDrvDMA_ChannelModePeriToMem,
                .bufferMode = qDrvDMA_BufferModeCircular,
                .wordMode = qRegDMA_WordMode8Bit,
                .dmaTriggerSelect = qDrvI2S_ConstDataGet(pInstance)->dmaTriggerRx,
            };

            if(qDrvDMA_ChannelInit(pCb->dmaRxChannel, &rxCfg, (void*)pInstance) != Q_OK)
            {
                (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
                (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
                pCb->dmaTxChannel = Q_NOCHANNEL;
                pCb->dmaRxChannel = Q_NOCHANNEL;
                return Q_ERR_OUTOFMEMORY;
            }
        }
    }

    qSysRam_RetainEnableSet(qDrvI2S_ConstDataGet(pInstance)->retain, true);

    /* Keep in reset if not started. */
    qRegStandby_ResetFuncEnableSet(qDrvI2S_ConstDataGet(pInstance)->resetFunc, true);

    qRegI2S_t baseAddr = pInstance->baseAddr;

    /* Check if I2S mode is slave. */
    qRegI2S_SlaveModeEnableSet(baseAddr, (pConfig->mode & Q_DRV_I2S_MODE_SLAVE_MASK) ? true : false);

    /* Set I2S clock source. */
    qRegI2S_ClkSelSet(baseAddr, (qRegI2S_ClkSel_t)pConfig->clkSrc);

    /* Set I2S word length. */
    qRegI2S_WordLenSet(baseAddr, pConfig->wordLen);

    /* Set I2S WS bits offset. */
    qRegI2S_WsOffsetSet(baseAddr, pConfig->wsOffset);

    /* Set I2S clock frequency. */
    if(pConfig->prescaler == Q_DRV_I2S_PRESCALER_NONE)
    {
        qRegI2S_PrescalerEnableSet(baseAddr, false);
    }
    else
    {
        qRegI2S_PrescalerSet(baseAddr, (UInt8)pConfig->prescaler);
        qRegI2S_PrescalerEnableSet(baseAddr, true);
    }

    if(pConfig->mode & Q_DRV_I2S_MODE_TX_MASK)
    {
        qRegI2S_TxBytesLeftSet(baseAddr, pConfig->txBytes.left);
        qRegI2S_TxBytesRightSet(baseAddr, pConfig->txBytes.right);
    }
    else
    {
        qRegI2S_TxBytesLeftSet(baseAddr, 0);
        qRegI2S_TxBytesRightSet(baseAddr, 0);
    }

    if(pConfig->mode & Q_DRV_I2S_MODE_RX_MASK)
    {
        qRegI2S_RxBytesLeftSet(baseAddr, pConfig->rxBytes.left);
        qRegI2S_RxBytesRightSet(baseAddr, pConfig->rxBytes.right);
    }
    else
    {
        qRegI2S_RxBytesLeftSet(baseAddr, 0);
        qRegI2S_RxBytesRightSet(baseAddr, 0);
    }

    pCb->pCallbacks = pCallbacks;
    pCb->arg = arg;
    pCb->mode = pConfig->mode;
    pCb->transferMode = pConfig->transferMode;

    NVIC_SetPriority(qDrvI2S_ConstDataGet(pInstance)->irqn, irqPriority);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvI2S_Deinit(qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        return Q_OK;
    }

    /* Disable the instance. */
    (void)qDrvI2S_Stop(pInstance);

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    if(pCb->transferMode == qDrvI2S_TransferModeDMA)
    {
        if(pCb->dmaTxChannel != Q_NOCHANNEL)
        {
            (void)qDrvDMA_ChannelFree(pCb->dmaTxChannel);
        }

        if(pCb->dmaRxChannel != Q_NOCHANNEL)
        {
            (void)qDrvDMA_ChannelFree(pCb->dmaRxChannel);
        }
    }
    else if(pCb->transferMode == qDrvI2S_TransferModeInterrupt)
    {
        qRegIntCtrl_IRQDisable(qDrvI2S_ConstDataGet(pInstance)->irqn);
        NVIC_DisableIRQ(qDrvI2S_ConstDataGet(pInstance)->irqn);
        NVIC_ClearPendingIRQ(qDrvI2S_ConstDataGet(pInstance)->irqn);
    }

    qSysRam_RetainEnableSet(qDrvI2S_ConstDataGet(pInstance)->retain, false);

    pCb->rxBuffer = NULL;
    pCb->rxBufferSize = 0;
    pCb->txBuffer = NULL;
    pCb->txBufferSize = 0;

    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvI2S_InitCheck(qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvI2S_TxBufferSet(qDrvI2S_t* const pInstance, UInt8* pBuffer, UInt16 bufferSize)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pBuffer);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegI2S_EnableGet(pInstance->baseAddr))
    {
        /* Instance is already transmitting data. */
        return Q_ERR_BUSY;
    }

    if(bufferSize < 2UL)
    {
        /* Wrong buffer size. */
        return Q_ERR_INVPARAM;
    }

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    if(!(pCb->mode & Q_DRV_I2S_MODE_TX_MASK))
    {
        /* No TX mode configured. */
        return Q_ERR_INVSTATE;
    }

    pCb->txBuffer = pBuffer;
    pCb->txBufferSize = bufferSize;

    if(pCb->transferMode == qDrvI2S_TransferModeDMA)
    {
        return qDrvDMA_BufferSet(pCb->dmaTxChannel, pBuffer, bufferSize);
    }

    return Q_OK;
}

qResult_t qDrvI2S_RxBufferSet(qDrvI2S_t* const pInstance, UInt8* pBuffer, UInt16 bufferSize)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pBuffer);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegI2S_EnableGet(pInstance->baseAddr))
    {
        /* Instance is already transmitting data. */
        return Q_ERR_BUSY;
    }

    if(bufferSize < 2UL)
    {
        /* Wrong buffer size. */
        return Q_ERR_INVPARAM;
    }

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    if(!(pCb->mode & Q_DRV_I2S_MODE_RX_MASK))
    {
        /* No RX mode configured. */
        return Q_ERR_INVSTATE;
    }

    pCb->rxBuffer = pBuffer;
    pCb->rxBufferSize = bufferSize;

    if(pCb->transferMode == qDrvI2S_TransferModeDMA)
    {
        return qDrvDMA_BufferSet(pCb->dmaRxChannel, pBuffer, bufferSize);
    }

    return Q_OK;
}

qResult_t qDrvI2S_Start(qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegI2S_t baseAddr = pInstance->baseAddr;

    if(qRegI2S_EnableGet(baseAddr))
    {
        /* Instance is already transmitting or receiving data. */
        return Q_ERR_BUSY;
    }

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    if((pCb->txBuffer == NULL) && (pCb->mode & Q_DRV_I2S_MODE_TX_MASK))
    {
        /* Tx buffer is required to be set in transmit mode. */
        return Q_ERR_GENERAL;
    }

    if((pCb->rxBuffer == NULL) && (pCb->mode & Q_DRV_I2S_MODE_RX_MASK))
    {
        /* Rx buffer is required to be set in receive mode. */
        return Q_ERR_GENERAL;
    }

    /* Release the reset. */
    qRegStandby_ResetFuncEnableSet(qDrvI2S_ConstDataGet(pInstance)->resetFunc, false);

    qResult_t res = Q_OK;

    if(pCb->transferMode == qDrvI2S_TransferModeDMA)
    {
        if(pCb->dmaTxChannel != Q_NOCHANNEL)
        {
            res = qDrvDMA_Advance(pCb->dmaTxChannel, pCb->txBufferSize);
            if(res != Q_OK)
            {
                return res;
            }

            res = qDrvDMA_ChannelEnable(pCb->dmaTxChannel, pCb->txBufferSize / 2);
            if(res != Q_OK)
            {
                return res;
            }
        }

        if(pCb->dmaRxChannel != Q_NOCHANNEL)
        {
            res = qDrvDMA_ChannelEnable(pCb->dmaRxChannel, pCb->rxBufferSize / 2);
            if(res != Q_OK)
            {
                if(pCb->dmaTxChannel != Q_NOCHANNEL)
                {
                    (void)qDrvDMA_ChannelDisable(pCb->dmaTxChannel);
                }
                return res;
            }
        }
    }

    if(pCb->transferMode == qDrvI2S_TransferModeInterrupt)
    {
        if(pCb->mode & Q_DRV_I2S_MODE_TX_MASK)
        {
            pCb->txSampleCount = 0;
            qRegIntCtrl_EventIRQEnable(qDrvI2S_ConstDataGet(pInstance)->irqn, (1UL << qRegI2S_IrqTxNotFull), 0);
        }

        if(pCb->mode & Q_DRV_I2S_MODE_RX_MASK)
        {
            pCb->rxSampleCount = 0;
            qRegIntCtrl_EventIRQEnable(qDrvI2S_ConstDataGet(pInstance)->irqn, (1UL << qRegI2S_IrqRxNotEmpty), 0);
        }

        qRegIntCtrl_IRQEnable(qDrvI2S_ConstDataGet(pInstance)->irqn);
        NVIC_ClearPendingIRQ(qDrvI2S_ConstDataGet(pInstance)->irqn);
        NVIC_EnableIRQ(qDrvI2S_ConstDataGet(pInstance)->irqn);
    }

    UInt8* buffer = pCb->txBuffer;

    if(pCb->mode & Q_DRV_I2S_MODE_TX_MASK)
    {
        /* Enable TX interrupts. */
        qRegI2S_TxIrqEnableSet(baseAddr, (pCb->mode & Q_DRV_I2S_MODE_TX_MASK) ? true : false);

        if(pCb->transferMode == qDrvI2S_TransferModePolling || pCb->transferMode == qDrvI2S_TransferModeInterrupt)
        {
            /* Make sure the first byte of the first left sample is in the tx fifo before enabling the I2S master. */
            qRegI2S_TxDataSet(baseAddr, buffer[0]);
            buffer++;
        }
    }

    qRegI2S_EnableSet(baseAddr, true);

    if((pCb->transferMode == qDrvI2S_TransferModePolling) && (pCb->mode & Q_DRV_I2S_MODE_TX_MASK))
    {
        for(UInt16 i = 1; i < pCb->txBufferSize; i++)
        {
            while(!qRegI2S_IRQUnmaskedTxNotFullGet(baseAddr))
            {
                ;
            }
            qRegI2S_TxDataSet(baseAddr, *buffer);
            buffer++;
        }

        qRegI2S_EnableSet(baseAddr, false);
    }

    return Q_OK;
}

qResult_t qDrvI2S_Stop(qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        /* Instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(!qRegI2S_EnableGet(pInstance->baseAddr))
    {
        /* Instance already stopped. */
        return Q_ERR_INVSTATE;
    }

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet(pInstance);

    if(pCb->transferMode == qDrvI2S_TransferModePolling)
    {
        /* Can not stop polling mode. */
        return Q_ERR_NOTSUPPORTED;
    }

    qRegStandby_ResetFuncEnableSet(qDrvI2S_ConstDataGet(pInstance)->resetFunc, true);
    qRegI2S_EnableSet(pInstance->baseAddr, false);
    qRegI2S_TxIrqEnableSet(pInstance->baseAddr, false);

    if(pCb->transferMode == qDrvI2S_TransferModeDMA)
    {
        if(pCb->dmaTxChannel != Q_NOCHANNEL)
        {
            (void)qDrvDMA_ChannelDisable(pCb->dmaTxChannel);
        }

        if(pCb->dmaRxChannel != Q_NOCHANNEL)
        {
            (void)qDrvDMA_ChannelDisable(pCb->dmaRxChannel);
        }
    }
    else if(pCb->transferMode == qDrvI2S_TransferModeInterrupt)
    {
        qRegIntCtrl_EventIRQDisable(qDrvI2S_ConstDataGet(pInstance)->irqn, (1UL << qRegI2S_IrqTxNotFull), 0);
        qRegIntCtrl_EventIRQDisable(qDrvI2S_ConstDataGet(pInstance)->irqn, (1UL << qRegI2S_IrqRxNotEmpty), 0);
    }

    return Q_OK;
}

Bool qDrvI2S_BusyCheck(qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_I2S_INSTANCE_COUNT);

    if(!qDrvI2S_InitCheck(pInstance))
    {
        return false;
    }

    /* Busy means that the instance is transmitting the data. */
    return qRegI2S_EnableGet(pInstance->baseAddr);
}

static void qDrvI2S_IRQHandler(const qDrvI2S_t* const pInstance)
{
    Q_ASSERT(pInstance);

    qDrvI2S_ControlBlock_t* pCb = qDrvI2S_ControlBlockGet((qDrvI2S_t* const)pInstance);
    qRegI2S_t baseAddr = pInstance->baseAddr;
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(qDrvI2S_ConstDataGet(pInstance)->irqn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegI2S_IrqTxNotFull))
    {
        UInt8 data = pCb->txBuffer[pCb->txSampleCount++];
        qRegI2S_TxDataSet(baseAddr, data);
        UInt16 halfBufferSize = pCb->txBufferSize / 2UL;

        if(pCb->txSampleCount == halfBufferSize)
        {
            pCb->pCallbacks->tx(pCb->arg, pCb->txBuffer, halfBufferSize);
        }

        if(pCb->txSampleCount == pCb->txBufferSize)
        {
            pCb->txSampleCount = 0;
            pCb->pCallbacks->tx(pCb->arg, pCb->txBuffer + halfBufferSize, halfBufferSize);
        }
    }

    if(maskedEvents & (1UL << qRegI2S_IrqRxNotEmpty))
    {
        UInt8 data = qRegI2S_RxDataGet(baseAddr);
        pCb->rxBuffer[pCb->rxSampleCount++] = data;
        UInt16 halfBufferSize = pCb->rxBufferSize / 2UL;

        if(pCb->rxSampleCount == halfBufferSize)
        {
            pCb->pCallbacks->rx(pCb->arg, pCb->rxBuffer, halfBufferSize);
        }

        if(pCb->rxSampleCount == pCb->rxBufferSize)
        {
            pCb->rxSampleCount = 0;
            pCb->pCallbacks->rx(pCb->arg, pCb->rxBuffer + halfBufferSize, halfBufferSize);
        }
    }
}

void i2s_0_handler_impl(void)
{
    qDrvI2S_IRQHandler(pInstances[0]);
}

void i2s_1_handler_impl(void)
{
    qDrvI2S_IRQHandler(pInstances[1]);
}
