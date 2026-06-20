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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qRegStandby.h"
#include "qRegSysRam.h"
#include "qDrvDMA.h"
#include "qDrvUART.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @enum qDrvUART_AsyncMode_t */
//@{
/** @brief Unused. */
#define qDrvUART_AsyncModeNone 0
/** @brief Interrupt configuration. */
#define qDrvUART_AsyncModeInterrupt 1
/** @brief DMA configuration. */
#define qDrvUART_AsyncModeDMA 2
/** @typedef qDrvUART_AsyncMode_t
 *  @brief UART transmitter and receiver mode configuration.
 */
typedef UInt8 qDrvUART_AsyncMode_t;
//@}

/** @brief UART TX transfer state structure. */
typedef struct {
    UInt8* buffer; /**< Buffer which was sent. */
    UInt16 length; /**< Number of bytes transferred. */
    UInt16 pos;    /**< First data index in TX buffer. */
} qDrvUART_Tx_t;

/** @brief UART RX transfer state structure. */
typedef struct {
    UInt8* buffer;   /**< Incoming data buffer. */
    UInt16 length;   /**< Number of bytes received. */
    UInt16 writePos; /**< First data in RX buffer. */
    UInt16 readPos;  /**< First data in RX buffer. */
    UInt16 threshold;
} qDrvUART_Rx_t;

/** @brief UART instance control block */
typedef struct {
    const qDrvUART_Callbacks_t* callbacks; /**< Set of user callbacks. */
    void* arg;
    qDrvUART_Tx_t tx;               /**< TX transfer state. */
    qDrvUART_Rx_t rx;               /**< RX transfer state. */
    qDrvDMA_Channel_t dmaRxChannel; /**< DMA channel for RX transfers. */
    qDrvDMA_Channel_t dmaTxChannel; /**< DMA channel for TX transfers. */
    Bool txTransmitting;            /**< TX active. */
    Bool rxReceiving;               /**< RX active. */
    qDrvUART_AsyncMode_t txMode;    /**< TX mode. */
    qDrvUART_AsyncMode_t rxMode;    /**< RX mode. */
    Bool noRxAdvance;               /**< RX buffer should not be advanced automatically. */
#if REG_UART_HAS_HWFC
    Bool hwfc;                      /**< Hardware flow control enabled. */
#endif

} qDrvUART_ControlBlock_t;

/* Control block data are stored in the internal data memory region of the instance.   */
Q_STATIC_ASSERT(sizeof(qDrvUART_ControlBlock_t) == sizeof(qDrvUART_InternalData_t));

/** @brief Constant values - UART driver constant instance data that are unable to be calculated at runtime. */
typedef struct {
    IRQn_Type irqn;             /**< Interrupt number used by the peripheral. */
    UInt32 dmaTriggerSelectTx;  /**< DMA source select tx not full of the peripheral. */
    UInt32 txDataAddress;       /**< Transmitter register address of the peripheral. */
    UInt32 dmaTriggerSelectRx;  /**< DMA source select rx not empty of the peripheral. */
    UInt32 rxDataAddress;       /**< Receiver register address of the peripheral. */
    qRegSysRam_Retain_t retain; /**< Reset function for the peripheral. */
} qDrvUART_ConstValues_t;

/**
 * @brief Macro for initializing const values required by a UART driver.
 *
 * @param[in] ID Id number of the instance.
 */
#define Q_DRV_UART_CONST_VALUES_INIT(ID)                                                                               \
    {                                                                                                                  \
        .irqn = Q_REG_UART_IRQN_GET(ID),                                           /* Init IRQn. */                    \
            .dmaTriggerSelectTx = Q_CONCAT(qRegDMA_Trigger, UART, ID, TxNotFull),  /* Init DMA trigger select. */      \
            .txDataAddress = Q_REG_UART_TX_DATA_ADDRESS_GET(ID),                   /* DMA TX register. */              \
            .dmaTriggerSelectRx = Q_CONCAT(qRegDMA_Trigger, UART, ID, RxNotEmpty), /* Init DMA trigger select. */      \
            .rxDataAddress = Q_REG_UART_RX_DATA_ADDRESS_GET(ID),                   /* DMA RX register.*/               \
            .retain = Q_REG_SYSRAM_RETAIN_GET(Q_CONCAT(UART_, ID)),                /* Reset function. */               \
    }

/** @brief Constant values of all UART instances. */
static const qDrvUART_ConstValues_t uartConstValues[REG_UART_INSTANCE_COUNT] = {
#if REG_UART_INSTANCE_COUNT > 0
    Q_DRV_UART_CONST_VALUES_INIT(0),
#endif
#if REG_UART_INSTANCE_COUNT > 1
    Q_DRV_UART_CONST_VALUES_INIT(1),
#endif
#if REG_UART_INSTANCE_COUNT > 2
    Q_DRV_UART_CONST_VALUES_INIT(2),
#endif
#if REG_UART_INSTANCE_COUNT > 3
    Q_DRV_UART_CONST_VALUES_INIT(3),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static qDrvUART_t* pInstances[REG_UART_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the interrupt number of the peripheral.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Interrupt number.
 */
static inline IRQn_Type qDrvUART_IRQnGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].irqn;
}

/**
 * @brief Get the DMA TX trigger select of the peripheral.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Interrupt number.
 */
static inline UInt32 qDrvUART_DmaTriggerSelectTxGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].dmaTriggerSelectTx;
}

/**
 * @brief Get the TX data address register of the peripheral. Can be used as DMA destination address.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Address register.
 */
static inline UInt32 qDrvUART_TxDataAddressGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].txDataAddress;
}

/**
 * @brief Get the DMA RX trigger select of the peripheral.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Interrupt number.
 */
static inline UInt32 qDrvUART_DmaTriggerSelectRxGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].dmaTriggerSelectRx;
}

/**
 * @brief Get the RX data address register of the peripheral. Can be used as DMA destination address.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Address register.
 */
static inline UInt32 qDrvUART_RxDataAddressGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].rxDataAddress;
}

/**
 * @brief Get the retain for the specified peripheral instance.
 *
 * @param [in] pInstance Pointer to the driver instance structure.
 *
 * @retval Retain value.
 */
static inline qRegSysRam_Retain_t qDrvUART_RetainGet(qDrvUART_t* const pInstance)
{
    return uartConstValues[pInstance->id].retain;
}

/**
 * @brief Function for getting the pointer to the control block.
 *
 * @param[in] pInstance Pointer to the driver instance structure.
 *
 * @retval Pointer to the control block
 */
static inline qDrvUART_ControlBlock_t* qDrvUART_ControlBlockGet(qDrvUART_t* const pInstance)
{
    return (qDrvUART_ControlBlock_t*)(&pInstance->internalData);
}

static void qDrvUART_CbDmaTx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    switch(eventType)
    {
        case qDrvDMA_EventTypeBufEmpty:
        {
            qDrvUART_t* const pInstance = (qDrvUART_t* const)arg;
            qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

            (void)qDrvDMA_ChannelDisable(pControlBlock->dmaTxChannel);

            if(qRegUART_IRQUnmaskedTxNotBusyGet(pInstance->baseAddr))
            {
                /* It the DMA event is handled after the transmission is complete, call the callback. */
                pControlBlock->txTransmitting = false;
                qRegUART_TxIntEnableSet(pInstance->baseAddr, false);

                if(pControlBlock->callbacks && pControlBlock->callbacks->tx)
                {
                    pControlBlock->callbacks->tx(pControlBlock->arg, pChunk, chunkSize);
                }
            }
            else
            {
                /* If UART is still transmitting data, wait for the transmission complete event. */
                qRegIntCtrl_EventIRQEnable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotBusy), 0);
            }

            break;
        }
        default:
        {
            /* This should never happen with a valid DMA configuration. */
            Q_ASSERT(false);
            break;
        }
    }
}

#ifndef REG_UART_NO_RX
static void qDrvUART_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 chunkSize)
{
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet((qDrvUART_t* const)arg);
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
        {
            pControlBlock->callbacks->rx(pControlBlock->arg, pChunk, chunkSize);
            break;
        }
        case qDrvDMA_EventTypeBufOverflow:
        {
            /* RX overflow error. */

            if(pControlBlock->callbacks && pControlBlock->callbacks->error)
            {
                pControlBlock->callbacks->error(pControlBlock->arg, qDrvUART_ErrMaskRxOverrun);
            }
            break;
        }
        default:
        {
            /* This should never happen with a valid DMA configuration. */
            Q_ASSERT(false);
            break;
        }
    }
}
#endif /* REG_UART_NO_RX */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvUART_PinConfigSet(qDrvUart_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    if(pPinConfig->tx.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->tx.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

#ifndef REG_UART_NO_RX
    if(pPinConfig->rx.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->rx.pin, qDrvIOB_PullUp, false);
    }
#endif

#if REG_UART_HAS_HWFC
    if(pPinConfig->cts.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigOutputSet(pPinConfig->cts.pin, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }
#endif

    return Q_OK;
}

qResult_t qDrvUART_PinConfigClear(qDrvUart_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvUART_Init(qDrvUART_t* const pInstance, const qDrvUART_Config_t* const pConfig,
                        const qDrvUART_Callbacks_t* const pCallbacks, void* arg, UInt32 interruptPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pInstance->id < REG_UART_INSTANCE_COUNT);

    qResult_t result;

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);
    qRegUART_t baseAddr = pInstance->baseAddr;

    if(qDrvUART_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_INVSTATE;
    }

#if REG_UART_HAS_HWFC
    pControlBlock->hwfc = pConfig->hwfc;
#endif

    if(pConfig->bitlength > qRegUART_Bitlength8)
    {
        return Q_ERR_NOTSUPPORTED;
    }

    /* Clear the control block data. */
    memset((void*)pControlBlock, 0, sizeof(qDrvUART_ControlBlock_t));

    if(pConfig->txDma)
    {
        /* Init DMA for transmitter. */
        pControlBlock->dmaTxChannel = qDrvDMA_Alloc();
        if(pControlBlock->dmaTxChannel == Q_NOCHANNEL)
        {
            return Q_ERR_OUTOFMEMORY;
        }

        qDrvDMA_ChannelConfig_t txChannelConfig = {
            .callback = &qDrvUART_CbDmaTx,
            .periAddress = qDrvUART_TxDataAddressGet(pInstance),
            .mode = qDrvDMA_ChannelModeMemToPeri,
            .bufferMode = qDrvDMA_BufferModeSingleShot,
            .wordMode = qRegDMA_WordMode8Bit,
            .dmaTriggerSelect = qDrvUART_DmaTriggerSelectTxGet(pInstance),
        };

        result = qDrvDMA_ChannelInit(pControlBlock->dmaTxChannel, &txChannelConfig, (void*)pInstance);
        if(result != Q_OK)
        {
            qDrvDMA_ChannelFree(pControlBlock->dmaTxChannel);
            pControlBlock->dmaTxChannel = Q_NOCHANNEL;
            return result;
        }
    }
    else
    {
        /* No DMA channel. */
        pControlBlock->dmaTxChannel = Q_NOCHANNEL;
    }

#ifndef REG_UART_NO_RX
    if(pConfig->rxDma)
    {
        /* Init DMA for receiver. */
        pControlBlock->dmaRxChannel = qDrvDMA_Alloc();
        if(pControlBlock->dmaRxChannel == Q_NOCHANNEL)
        {
            if(pControlBlock->dmaTxChannel != Q_NOCHANNEL)
            {
                qDrvDMA_ChannelFree(pControlBlock->dmaTxChannel);
                pControlBlock->dmaTxChannel = Q_NOCHANNEL;
            }
            return Q_ERR_OUTOFMEMORY;
        }

        qDrvDMA_ChannelConfig_t rxChannelConfig = {
            .callback = &qDrvUART_CbDmaRx,
            .periAddress = qDrvUART_RxDataAddressGet(pInstance),
            .mode = qDrvDMA_ChannelModePeriToMem,
            .bufferMode = qDrvDMA_BufferModeCircular,
            .wordMode = qRegDMA_WordMode8Bit,
            .dmaTriggerSelect = qDrvUART_DmaTriggerSelectRxGet(pInstance),
            .noAdvance = pConfig->noRxAdvance,
        };

        result = qDrvDMA_ChannelInit(pControlBlock->dmaRxChannel, &rxChannelConfig, (void*)pInstance);
        if(result != Q_OK)
        {
            qDrvDMA_ChannelFree(pControlBlock->dmaRxChannel);
            pControlBlock->dmaRxChannel = Q_NOCHANNEL;

            if(pControlBlock->dmaTxChannel != Q_NOCHANNEL)
            {
                qDrvDMA_ChannelFree(pControlBlock->dmaTxChannel);
                pControlBlock->dmaTxChannel = Q_NOCHANNEL;
            }

            return result;
        }
    }
    else
    {
        /* No DMA channel. */
        pControlBlock->dmaRxChannel = Q_NOCHANNEL;
    }
#endif

    pControlBlock->noRxAdvance = pConfig->noRxAdvance;

#ifdef REG_UART_HAS_HWFC
    /* Enable HWFC. */
    qRegUART_HWFlowControlEnableSet(baseAddr, pControlBlock->hwfc);
#endif /* REG_UART_HAS_HWFC */

    /* Disable receiver. */
    qRegUART_RxEnableSet(baseAddr, false);

    /* Disable transmitter interrupts. */
    qRegUART_TxIntEnableSet(baseAddr, false);

    /* Configure UART. */
    qRegUART_BaudrateSet(baseAddr, pConfig->baudrate);
    qRegUART_BitlengthSet(baseAddr, pConfig->bitlength);
    qRegUART_ParitySet(baseAddr, pConfig->parity);
    qRegUART_StopbitsSet(baseAddr, pConfig->stopbits);
    qSysRam_RetainEnableSet(qDrvUART_RetainGet(pInstance), true);

    pControlBlock->callbacks = pCallbacks;
    pControlBlock->arg = arg;

    NVIC_SetPriority(qDrvUART_IRQnGet(pInstance), interruptPriority);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvUART_Deinit(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_UART_INSTANCE_COUNT);

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        return Q_OK;
    }

    (void)qDrvUART_RxAbort(pInstance);
    (void)qDrvUART_TxIrqAbort(pInstance, NULL);
    (void)qDrvUART_TxDMAAbort(pInstance, NULL);
    (void)qDrvUART_IrqDisable(pInstance);

    if(pControlBlock->dmaTxChannel != Q_NOCHANNEL)
    {
        qDrvDMA_ChannelFree(pControlBlock->dmaTxChannel);
        pControlBlock->dmaTxChannel = Q_NOCHANNEL;
    }

    if(pControlBlock->dmaRxChannel != Q_NOCHANNEL)
    {
        qDrvDMA_ChannelFree(pControlBlock->dmaRxChannel);
        pControlBlock->dmaRxChannel = Q_NOCHANNEL;
    }

    /* Disable event interrupts. */
    qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance),
                                (1UL << qRegUART_IrqRxOverrun) | (1UL << qRegUART_IrqRxParityError) |
                                    (1UL << qRegUART_IrqRxFramingError) | (1UL << qRegUART_IrqTxNotFull) |
                                    (1UL << qRegUART_IrqTxNotBusy) | (1UL << qRegUART_IrqRxNotEmpty),
                                0);

    qSysRam_RetainEnableSet(qDrvUART_RetainGet(pInstance), false);
    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvUART_InitCheck(qDrvUART_t* const pInstance)
{
    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvUART_CallbackSet(qDrvUART_t* const pInstance, const qDrvUART_Callbacks_t* const pCallbacks, void* arg)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting || pControlBlock->rxReceiving)
    {
        /* Uart is already transmitting or receiving. */
        return Q_ERR_BUSY;
    }

    pControlBlock->callbacks = pCallbacks;
    pControlBlock->arg = arg;

    return Q_OK;
}

qResult_t qDrvUART_IrqEnable(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    /* Enable global interrupts from the peripheral. */
    qRegIntCtrl_IRQEnable(qDrvUART_IRQnGet(pInstance));

    /* Enable NVIC interrupts. */
    NVIC_ClearPendingIRQ(qDrvUART_IRQnGet(pInstance));

    /* Enable NVIC interrupts. */
    NVIC_EnableIRQ(qDrvUART_IRQnGet(pInstance));

    return Q_OK;
}

qResult_t qDrvUART_IrqDisable(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    /* Disable NVIC interrupts. */
    NVIC_DisableIRQ(qDrvUART_IRQnGet(pInstance));

    /* Disable global interrupts from the peripheral. */
    qRegIntCtrl_IRQDisable(qDrvUART_IRQnGet(pInstance));

    /* Clear pending interrupt. */
    NVIC_ClearPendingIRQ(qDrvUART_IRQnGet(pInstance));

    return Q_OK;
}

Bool qDrvUART_IrqEnabledCheck(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return false;
    }

    return NVIC_GetEnableIRQ(qDrvUART_IRQnGet(pInstance));
}

qResult_t qDrvUART_Tx(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pData);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting == true)
    {
        /* Previous transmission is already in progress. */
        return Q_ERR_BUSY;
    }

    if(length == 0)
    {
        /* Wrong buffer size. */
        return Q_ERR_INVPARAM;
    }

    /* Mark the transmission as active. */
    pControlBlock->txTransmitting = true;

    /* Clear interrupt transmission data. */
    pControlBlock->tx.buffer = NULL;
    pControlBlock->tx.length = 0;
    pControlBlock->tx.pos = 0;

    qRegUART_t baseAddr = pInstance->baseAddr;

    qRegUART_TxIntEnableSet(pInstance->baseAddr, true);

    for(UInt16 i = 0; i < length; i++)
    {
        /* Wait for the TX FIFO to be not full. */
        while(qRegUART_IRQUnmaskedTxNotFullGet(baseAddr) == false)
        {
        }

        /* Write the data to the TX FIFO. */
        qRegUART_TxDataSet(baseAddr, pData[i]);
    }

    /* Wait for the TX FIFO to be empty. */
    while(qRegUART_IRQUnmaskedTxNotBusyGet(baseAddr) == false)
    {
    }

    qRegUART_TxIntEnableSet(pInstance->baseAddr, false);
    pControlBlock->txTransmitting = false;

    return Q_OK;
}

qResult_t qDrvUART_TxIrq(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pData);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(length == 0)
    {
        /* Wrong buffer size. */
        return Q_ERR_INVPARAM;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting)
    {
        /* Previous transmission is already in progress. */
        return Q_ERR_BUSY;
    }

    /* Mark the transmission as active. */
    pControlBlock->txTransmitting = true;

    /* Set the TX mode. */
    pControlBlock->txMode = qDrvUART_AsyncModeInterrupt;

    /* Set the TX buffer parameters. */
    pControlBlock->tx.buffer = (UInt8*)pData;
    pControlBlock->tx.length = length;
    pControlBlock->tx.pos = 0;

    qRegUART_TxIntEnableSet(pInstance->baseAddr, true);

    /* Apply the first byte to the TX FIFO. */
    qRegUART_TxDataSet(pInstance->baseAddr, pControlBlock->tx.buffer[pControlBlock->tx.pos++]);
    if(length == 1)
    {
        qRegIntCtrl_EventIRQEnable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotBusy), 0);
    }
    else
    {
        qRegIntCtrl_EventIRQEnable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotFull), 0);
    }

    return Q_OK;
}

qResult_t qDrvUART_TxDMA(qDrvUART_t* const pInstance, const UInt8* const pData, UInt16 length)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pData);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(length == 0)
    {
        /* Wrong buffer size. */
        return Q_ERR_INVPARAM;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->dmaTxChannel == Q_NOCHANNEL)
    {
        /* DMA is not initialized. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->txTransmitting)
    {
        /* Previous transmission is already in progress. */
        return Q_ERR_BUSY;
    }

    /* Mark the transmission as active. */
    pControlBlock->txTransmitting = true;

    /* Set the TX mode. */
    pControlBlock->txMode = qDrvUART_AsyncModeDMA;

    /* Set the TX buffer parameters. */
    pControlBlock->tx.buffer = (UInt8*)pData;
    pControlBlock->tx.length = length;
    pControlBlock->tx.pos = 0;

    /* In this case the buffer is reset on every TX call and transferred only once
       (non-circular), so the driver will wait for complete event.
       DMA does not support 1-byte buffer size, so in such case length is set to 2
       and pointer is advanced by one byte only.
     */
    qResult_t result;

    result = qDrvDMA_BufferSet(pControlBlock->dmaTxChannel, (void*)pData, length + ((length == 1UL) ? 1UL : 0UL));
    if(result != Q_OK)
    {
        return result;
    }

    result = qDrvDMA_Advance(pControlBlock->dmaTxChannel, length);
    if(result != Q_OK)
    {
        return result;
    }

    qRegUART_TxIntEnableSet(pInstance->baseAddr, true);

    return qDrvDMA_ChannelEnable(pControlBlock->dmaTxChannel, 0);
}

Bool qDrvUART_TxBusyCheck(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return false;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    return pControlBlock->txTransmitting;
}

Bool qDrvUART_TxFifoEmptyCheck(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return false;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txMode == qDrvUART_AsyncModeNone)
    {
        return qRegUART_IRQUnmaskedTxNotFullGet(pInstance->baseAddr);
    }
    else if(pControlBlock->txMode == qDrvUART_AsyncModeDMA)
    {
        return qDrvDMA_ChannelCompeteCheck(pControlBlock->dmaTxChannel);
    }
    else if(pControlBlock->txMode == qDrvUART_AsyncModeInterrupt)
    {
        return pControlBlock->tx.pos >= pControlBlock->tx.length;
    }

    return false;
}

qResult_t qDrvUART_TxIrqAbort(qDrvUART_t* const pInstance, UInt16* pAlreadySend)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting == false)
    {
        /* There is no transmission to be aborted. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->txMode != qDrvUART_AsyncModeInterrupt)
    {
        /* Wrong TX async mode. */
        return Q_ERR_INVSTATE;
    }

    /* Abort the transmission. */
    qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance),
                                (1UL << qRegUART_IrqTxNotFull) | (1UL << qRegUART_IrqTxNotBusy), 0);

    /* Wait for the TX FIFO to be empty. */
    while(qRegUART_IRQUnmaskedTxNotBusyGet(pInstance->baseAddr) == false)
    {
    }

    if(pAlreadySend)
    {
        *pAlreadySend = pControlBlock->tx.pos;
    }

    pControlBlock->txTransmitting = false;

    return Q_OK;
}

qResult_t qDrvUART_TxDMAAbort(qDrvUART_t* const pInstance, UInt16* pAlreadySend)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting == false)
    {
        /* There is no transmission to be aborted. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->txMode != qDrvUART_AsyncModeDMA)
    {
        /* Wrong TX async mode. */
        return Q_ERR_INVSTATE;
    }

    qDrvDMA_ChannelDisable(pControlBlock->dmaTxChannel);

    /* Wait for the TX FIFO to be empty. */
    while(qRegUART_IRQUnmaskedTxNotBusyGet(pInstance->baseAddr) == false)
    {
    }

    if(pAlreadySend)
    {
        *pAlreadySend = qRegDMA_InternalPtrGet(pControlBlock->dmaTxChannel);
    }

    pControlBlock->txTransmitting = false;

    return Q_OK;
}

UInt8* qDrvUART_TxRemainingChunkGet(qDrvUART_t* const pInstance, UInt16* pByteCount)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        *pByteCount = 0;
        return NULL;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->tx.length == 0 || pControlBlock->tx.buffer == NULL)
    {
        *pByteCount = 0;
        return NULL;
    }

    UInt16 alreadySend = 0;

    if(pControlBlock->txMode == qDrvUART_AsyncModeDMA)
    {
        alreadySend = qRegDMA_InternalPtrGet(pControlBlock->dmaTxChannel);
    }
    else if(pControlBlock->txMode == qDrvUART_AsyncModeInterrupt)
    {
        alreadySend = pControlBlock->tx.pos;
    }

    *pByteCount = pControlBlock->tx.length - alreadySend;
    return pControlBlock->tx.buffer + alreadySend;
}

#ifndef REG_UART_NO_RX

qResult_t qDrvUART_RxEnable(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    /* Enable receiver. */
    qRegUART_RxEnableSet(pInstance->baseAddr, true);

    return Q_OK;
}

qResult_t qDrvUART_RxDisable(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    /* Disable receiver. */
    qRegUART_RxEnableSet(pInstance->baseAddr, false);

    return Q_OK;
}

Bool qDrvUART_RxEnableCheck(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return false;
    }

    return qRegUART_RxEnableGet(pInstance->baseAddr);
}

qResult_t qDrvUART_Rx(qDrvUART_t* const pInstance, UInt8* const pData)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pData);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qRegUART_t baseAddr = pInstance->baseAddr;

    if(!qRegUART_IRQUnmaskedRxNotEmptyGet(baseAddr))
    {
        /* No data in the RX FIFO. */
        return Q_ERR_INVSTATE;
    }

    *pData = qRegUART_RxDataGet(pInstance->baseAddr);

    return Q_OK;
}

qResult_t qDrvUART_RxIrq(qDrvUART_t* const pInstance, UInt8* const pData, UInt16 length, UInt16 threshold)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(pData && (length < 2UL))
    {
        /* Buffer size is too short. */
        return Q_ERR_INVPARAM;
    }

    if((threshold > 0) && (threshold >= length))
    {
        /* Wrong threshold value. */
        return Q_ERR_INVPARAM;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->rxReceiving == true)
    {
        /* Receiver is already enabled. */
        return Q_ERR_BUSY;
    }

    /* Mark the receiver as busy. */
    pControlBlock->rxReceiving = true;

    /* Set the RX mode. */
    pControlBlock->rxMode = qDrvUART_AsyncModeInterrupt;

    /* Set the RX buffer parameters. */
    pControlBlock->rx.buffer = pData;
    pControlBlock->rx.length = length;
    pControlBlock->rx.writePos = 0;
    pControlBlock->rx.readPos = 0;
    pControlBlock->rx.threshold = threshold;

    /* Read the last received data from the RX FIFO. */
    (void)qRegUART_RxDataGet(pInstance->baseAddr);

    /* Enable receiver event interrupts. */
    qRegIntCtrl_EventIRQEnable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqRxNotEmpty), 0);

    return Q_OK;
}

qResult_t qDrvUART_RxDMA(qDrvUART_t* const pInstance, UInt8* pData, UInt16 length, UInt16 threshold)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pData);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(length < 2UL)
    {
        /* Buffer size is too short. */
        return Q_ERR_INVPARAM;
    }

    if((threshold > 0) && (threshold >= length))
    {
        /* Wrong threshold value. */
        return Q_ERR_INVPARAM;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->rxReceiving == true)
    {
        /* Receiver is already enabled. */
        return Q_ERR_BUSY;
    }

    if(pControlBlock->dmaRxChannel == Q_NOCHANNEL)
    {
        /* DMA is not initialized. */
        return Q_ERR_INVSTATE;
    }

    /* Mark the receiver as busy. */
    pControlBlock->rxReceiving = true;

    /* Set the RX mode. */
    pControlBlock->rxMode = qDrvUART_AsyncModeDMA;

    qResult_t result = qDrvDMA_BufferSet(pControlBlock->dmaRxChannel, (void*)pData, length);
    if(result != Q_OK)
    {
        return result;
    }

    /* Read the last received data from the RX FIFO. */
    (void)qRegUART_RxDataGet(pInstance->baseAddr);

    /* Enable receiver with the DMA. */
    return qDrvDMA_ChannelEnable(pControlBlock->dmaRxChannel, threshold);
}

qResult_t qDrvUART_RxAbort(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(pControlBlock->rxReceiving == false)
    {
        /* There is no ongoing reception. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->rxMode == qDrvUART_AsyncModeDMA)
    {
        (void)qDrvDMA_ChannelDisable(pControlBlock->dmaRxChannel);
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeInterrupt)
    {
        qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqRxNotEmpty), 0);
    }

    // pControlBlock->rxMode = qDrvUART_AsyncModeNone;
    pControlBlock->rxReceiving = false;

    return Q_OK;
}

qResult_t qDrvUART_RxFifoFlush(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if((pControlBlock->callbacks == NULL) || (pControlBlock->callbacks->rx == NULL))
    {
        /* RX callback is not set. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->rxReceiving == false)
    {
        /* There is no ongoing reception. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->rxMode == qDrvUART_AsyncModeDMA)
    {
        HAL_DISABLE_GLOBAL_INT();
        for(int i = 0; i < 2; i++)
        {
            UInt16 wordCount;
            UInt8* pChunk = (UInt8*)qDrvDMA_ContinuousChunkGet(pControlBlock->dmaRxChannel, &wordCount);

            if((pChunk == NULL) || (wordCount == 0UL))
            {
                /* Already precessed all received chunks. */
                break;
            }

            pControlBlock->callbacks->rx(pControlBlock->arg, pChunk, wordCount);

            if(pControlBlock->noRxAdvance == false)
            {
                (void)qDrvDMA_Advance(pControlBlock->dmaRxChannel, wordCount);
            }
        }
        HAL_ENABLE_GLOBAL_INT();
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeInterrupt)
    {
        HAL_DISABLE_GLOBAL_INT();
        qDrvUART_Rx_t* rx = &pControlBlock->rx;
        UInt16 writePos = rx->writePos;
        UInt16 readPos = rx->readPos;

        if(writePos > readPos)
        {
            pControlBlock->callbacks->rx(pControlBlock->arg, &rx->buffer[readPos], writePos - readPos);
        }

        if(writePos < readPos)
        {
            pControlBlock->callbacks->rx(pControlBlock->arg, &rx->buffer[readPos], rx->length - readPos);
            if(writePos)
            {
                pControlBlock->callbacks->rx(pControlBlock->arg, rx->buffer, writePos);
            }
        }

        if(pControlBlock->noRxAdvance == false)
        {
            rx->readPos = writePos;
        }

        HAL_ENABLE_GLOBAL_INT();
    }

    return Q_OK;
}

Bool qDrvUART_RxNewDataCheck(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return false;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->rxMode == qDrvUART_AsyncModeNone)
    {
        return qRegUART_RxNotBusyGet(pInstance->baseAddr) == false;
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeDMA)
    {
        UInt16 wordCount;

        HAL_DISABLE_GLOBAL_INT();
        (void)qDrvDMA_ContinuousChunkGet(pControlBlock->dmaRxChannel, &wordCount);
        HAL_ENABLE_GLOBAL_INT();

        /* If the words count is not zero,
           it means that some new data are in the DMA buffer,
           but the threshold was not triggered yet. */
        return wordCount != 0UL;
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeInterrupt)
    {
        qDrvUART_Rx_t* rx = &pControlBlock->rx;

        HAL_DISABLE_GLOBAL_INT();
        Bool positionEqual = rx->writePos == rx->readPos;
        HAL_ENABLE_GLOBAL_INT();

        /* If the read and the write positions are not the same,
           it means that some new data are in the RX buffer,
           but the threshold was not triggered yet. */
        return positionEqual == false;
    }
    return false;
}

UInt8* qDrvUART_RxContinuousChunkGet(qDrvUART_t* const pInstance, UInt16* pByteCount)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pByteCount);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return NULL;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if((pControlBlock->noRxAdvance == false) || (pControlBlock->rxMode == qDrvUART_AsyncModeNone))
    {
        *pByteCount = 0;
        return NULL;
    }

    if(pControlBlock->rxMode == qDrvUART_AsyncModeDMA)
    {
        return qDrvDMA_ContinuousChunkGet(pControlBlock->dmaRxChannel, pByteCount);
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeInterrupt)
    {
        qDrvUART_Rx_t* rx = &pControlBlock->rx;
        if(rx->writePos > rx->readPos)
        {
            *pByteCount = rx->writePos - rx->readPos;
        }
        else
        {
            *pByteCount = rx->length - rx->readPos;
        }

        return &rx->buffer[rx->readPos];
    }

    *pByteCount = 0;
    return NULL;
}

qResult_t qDrvUart_RxAdvance(qDrvUART_t* const pInstance, UInt16 chunkSize)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->noRxAdvance == false)
    {
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->rxMode == qDrvUART_AsyncModeNone)
    {
        /* Async mode is not set. */
        return Q_ERR_INVSTATE;
    }

    if(pControlBlock->rxMode == qDrvUART_AsyncModeDMA)
    {
        return qDrvDMA_Advance(pControlBlock->dmaRxChannel, chunkSize);
    }
    else if(pControlBlock->rxMode == qDrvUART_AsyncModeInterrupt)
    {
        qDrvUART_Rx_t* rx = &pControlBlock->rx;
        UInt16 availableBytes = 0;

        if(rx->writePos > rx->readPos)
        {
            availableBytes = rx->writePos - rx->readPos;
        }
        else
        {
            availableBytes = (rx->length - rx->readPos) + rx->writePos;
        }

        if(chunkSize > availableBytes)
        {
            return Q_ERR_INVPARAM;
        }

        rx->readPos += chunkSize;
        rx->readPos %= rx->length;

        return Q_OK;
    }

    return Q_ERR_INVSTATE;
}

qResult_t qDrvUART_RxErrorIrqEnableSet(qDrvUART_t* const pInstance, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(enable)
    {
        qRegUART_IRQRxOverrunErrorClear(pInstance->baseAddr);
        qRegUART_IRQRxParityErrorClear(pInstance->baseAddr);
        qRegUART_IRQRxFramingErrorClear(pInstance->baseAddr);
        /* Enable receiver error event interrupts. */
        qRegIntCtrl_EventIRQEnable(
            qDrvUART_IRQnGet(pInstance),
            (1UL << qRegUART_IrqRxOverrun) | (1UL << qRegUART_IrqRxParityError) | (1UL << qRegUART_IrqRxFramingError),
            0);
    }
    else
    {
        /* Disable receiver error event interrupts. */
        qRegIntCtrl_EventIRQDisable(
            qDrvUART_IRQnGet(pInstance),
            (1UL << qRegUART_IrqRxOverrun) | (1UL << qRegUART_IrqRxParityError) | (1UL << qRegUART_IrqRxFramingError),
            0);
    }

    return Q_OK;
}

static void qDrvUART_RxHandler(qDrvUART_t* const pInstance)
{
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);
    qRegUART_t baseAddr = pInstance->baseAddr;

    UInt8 data = qRegUART_RxDataGet(baseAddr);

    qDrvUART_Rx_t* rx = &pControlBlock->rx;

    /* If the receiver works in the buffered mode. */
    if((rx->buffer != NULL) && (rx->threshold > 1))
    {
        rx->buffer[rx->writePos] = data;
        rx->writePos++;
        rx->writePos %= rx->length;

        UInt16 availableBytes = 0;
        if(rx->writePos > rx->readPos)
        {
            availableBytes = rx->writePos - rx->readPos;
        }
        else
        {
            availableBytes = (rx->length - rx->readPos) + rx->writePos;
        }

        if(availableBytes == rx->threshold)
        {
            if(rx->writePos > rx->readPos)
            {
                pControlBlock->callbacks->rx(pControlBlock->arg, &rx->buffer[rx->readPos], rx->threshold);
            }

            if(rx->writePos < rx->readPos)
            {
                pControlBlock->callbacks->rx(pControlBlock->arg, &rx->buffer[rx->readPos], rx->length - rx->readPos);

                if(rx->writePos)
                {
                    pControlBlock->callbacks->rx(pControlBlock->arg, rx->buffer, rx->writePos);
                }
            }

            if(pControlBlock->noRxAdvance == false)
            {
                rx->readPos = rx->writePos;
            }
        }
    }
    else
    {
        pControlBlock->callbacks->rx(pControlBlock->arg, &data, 1);
    }
}
#endif /* REG_UART_NO_RX */

static void qDrvUART_IRQHandler(qDrvUART_t* const pInstance)
{
    qRegUART_t baseAddr = pInstance->baseAddr;
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(qDrvUART_IRQnGet(pInstance), &maskedEvents, NULL);

#ifndef REG_UART_NO_RX
    qDrvUART_ErrMask_t errMask = 0;
    if(maskedEvents & (1UL << qRegUART_IrqRxOverrun))
    {
        qRegUART_IRQRxOverrunErrorClear(baseAddr);
        errMask |= qDrvUART_ErrMaskRxOverrun;
    }

    if(maskedEvents & (1UL << qRegUART_IrqRxParityError))
    {
        qRegUART_IRQRxParityErrorClear(baseAddr);
        errMask |= qDrvUART_ErrMaskRxParity;
    }

    if(maskedEvents & (1UL << qRegUART_IrqRxFramingError))
    {
        qRegUART_IRQRxFramingErrorClear(baseAddr);
        errMask |= qDrvUART_ErrMaskRxFraming;
    }

    if(errMask && pControlBlock->callbacks && pControlBlock->callbacks->error)
    {
        pControlBlock->callbacks->error(pControlBlock->arg, errMask);
    }

    if(maskedEvents & (1UL << qRegUART_IrqRxNotEmpty))
    {
        qDrvUART_RxHandler(pInstance);
    }
#endif

    if(maskedEvents & (1UL << qRegUART_IrqTxNotFull))
    {
        qRegUART_TxDataSet(baseAddr, pControlBlock->tx.buffer[pControlBlock->tx.pos++]);

        if(pControlBlock->tx.pos >= pControlBlock->tx.length)
        {
            qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotFull), 0);

            /* Wait for the transmission complete event. */
            qRegIntCtrl_EventIRQEnable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotBusy), 0);
        }
    }

    if(maskedEvents & (1UL << qRegUART_IrqTxNotBusy))
    {
        qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotBusy), 0);
        if(pControlBlock->txTransmitting)
        {
            pControlBlock->txTransmitting = false;
            qRegUART_TxIntEnableSet(pInstance->baseAddr, false);

            UInt8* pData = (UInt8*)pControlBlock->tx.buffer;
            UInt16 len = pControlBlock->tx.length;
            pControlBlock->tx.buffer = NULL;

            if(pControlBlock->callbacks && pControlBlock->callbacks->tx)
            {
                pControlBlock->callbacks->tx(pControlBlock->arg, pData, len);
            }
        }
    }
}

#if REG_UART_INSTANCE_COUNT > 0
void uart_0_handler_impl(void)
{
    qDrvUART_IRQHandler(pInstances[0]);
}
#endif /* REG_UART_INSTANCE_COUNT > 0 */

#if REG_UART_INSTANCE_COUNT > 1
void uart_1_handler_impl(void)
{
    qDrvUART_IRQHandler(pInstances[1]);
}
#endif /* REG_UART_INSTANCE_COUNT > 1 */

#if REG_UART_INSTANCE_COUNT > 2
void uart_2_handler_impl(void)
{
    qDrvUART_IRQHandler(pInstances[2]);
}
#endif /* REG_UART_INSTANCE_COUNT > 2 */

#if REG_UART_INSTANCE_COUNT > 3
void uart_3_handler_impl(void)
{
    qDrvUART_IRQHandler(pInstances[3]);
}
#endif /* REG_UART_INSTANCE_COUNT > 3 */

qResult_t qDrvUART_TxFlush(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->txTransmitting == true)
    {
        /* Ongoing transmission is in progress. Please call abort before. */
        return Q_ERR_BUSY;
    }

    if((pControlBlock->tx.buffer == NULL) || (pControlBlock->tx.length == 0) ||
       (pControlBlock->tx.pos >= pControlBlock->tx.length))

    {
        /* There is no data to be transmitted. */
        return Q_ERR_INVSTATE;
    }

    pControlBlock->txTransmitting = true;

    qRegUART_t baseAddr = pInstance->baseAddr;

    while(qRegUART_IRQUnmaskedTxNotBusyGet(baseAddr) == false)
    {
        HAL_WDT_RESET();
        ; // Wait for the transmitter to be not busy.
    }

    for(; pControlBlock->tx.pos < pControlBlock->tx.length; pControlBlock->tx.pos++)
    {
        qRegUART_TxDataSet(baseAddr, pControlBlock->tx.buffer[pControlBlock->tx.pos]);

        while(qRegUART_IRQUnmaskedTxNotFullGet(baseAddr) == false)
        {
            HAL_WDT_RESET();
            ; // Wait for the TX FIFO to be empty.
        }
    }

    while(qRegUART_IRQUnmaskedTxNotBusyGet(baseAddr) == false)
    {
        HAL_WDT_RESET();
        // Wait for the transmitter to be not busy.
    }

    UInt8* pData = (UInt8*)pControlBlock->tx.buffer;
    UInt16 len = pControlBlock->tx.length;
    pControlBlock->tx.buffer = NULL;
    pControlBlock->tx.length = 0;

    pControlBlock->txTransmitting = false;

    if(pControlBlock->callbacks && pControlBlock->callbacks->tx)
    {
        pControlBlock->callbacks->tx(pControlBlock->arg, pData, len);
    }

    return Q_OK;
}

static inline void qDrvUART_TxEndTransmission(qDrvUART_t* const pInstance)
{
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    // Transfer complete. Clear busy flag before calling user callback,
    // so the next transfer could be triggered from there.
    qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotFull), 0);
    pControlBlock->txTransmitting = false;

    UInt8* pData = (UInt8*)pControlBlock->tx.buffer;
    pControlBlock->tx.buffer = NULL;

    if(pControlBlock->callbacks && pControlBlock->callbacks->tx)
    {
        UInt16 len = pControlBlock->tx.length;
        pControlBlock->tx.length = 0;
        pControlBlock->callbacks->tx(pControlBlock->arg, pData, len);
    }
}

static inline void qDrvUART_TxHandler(qDrvUART_t* const pInstance)
{
    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);
    qRegUART_t baseAddr = pInstance->baseAddr;

    if(!pControlBlock->tx.buffer)
    {
        /* Abort the transmission. */
        qRegIntCtrl_EventIRQDisable(qDrvUART_IRQnGet(pInstance), (1UL << qRegUART_IrqTxNotFull), 0);
        pControlBlock->txTransmitting = false;
        return;
    }

    if(pControlBlock->tx.pos >= pControlBlock->tx.length)
    {
        qDrvUART_TxEndTransmission(pInstance);
        return;
    }

    UInt8 dataToTx = pControlBlock->tx.buffer[pControlBlock->tx.pos];
    pControlBlock->tx.pos++;

    qRegUART_TxDataSet(baseAddr, dataToTx);
}

UInt16 qDrvUART_TxRemainingBytesGet(qDrvUART_t* const pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvUART_InitCheck(pInstance))
    {
        /* Peripheral instance is not initialized. */
        return 0;
    }

    qDrvUART_ControlBlock_t* pControlBlock = qDrvUART_ControlBlockGet(pInstance);

    if(pControlBlock->tx.length != 0 && pControlBlock->tx.buffer)
    {
        return pControlBlock->tx.length - pControlBlock->tx.pos;
    }

    return 0;
}
