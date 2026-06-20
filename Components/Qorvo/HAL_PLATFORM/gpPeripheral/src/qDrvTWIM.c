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
#include "qDrvTWIM.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvTWIM_TransferState_t */
//@{
#define qDrvTWIM_TransferStateIdle  0
#define qDrvTWIM_TransferStateTx    1
#define qDrvTWIM_TransferStateRx    2
#define qDrvTWIM_TransferStateAbort 3
/** @typedef qDrvTWIM_TransferState_t
 *  @brief TWI Master transfer state.
 */
typedef UInt8 qDrvTWIM_TransferState_t;
//@}

/** @brief TWIM instance control block */
typedef struct {
    qDrvTWIM_Callback_t callback;     /**< Set of user callbacks. */
    void* arg;                        /**< User-defined argument. */
    qDrvTWIM_Transfer_t transfer;     /**< Current transfer info. */
    UInt16 txPos;                     /**< Number of bytes send in current transaction. */
    UInt16 rxPos;                     /**< Number of bytes received in current transaction. */
    UInt8 retries;                    /**< Number of retries. */
    qDrvTWIM_TransferState_t trState; /**< Transfer state. */
    Bool noAck;                       /**< Slave ACK state. */
    UInt8 busClear;                   /**< I2C bus clear in progress. */
} qDrvTWIM_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvTWIM_ControlBlock_t) == sizeof(qDrvTWIM_InternalData_t));

/** @brief TWIM instance constant data block structure. */
typedef struct {
    IRQn_Type irqn;             /**< Peripheral instance interrupt number. */
    qRegSysRam_Retain_t retain; /**< Instance index in SysRam retain register. */
} qDrvTWIM_ConstData_t;

/** @brief Macro for initializing const data block for TWIM instance. */
#define Q_DRV_TWIM_CONST_DATA_INIT(ID)                                                                                 \
    {                                                                                                                  \
        .irqn = Q_REG_TWIM_IRQN_GET(ID),                             /* */                                             \
            .retain = Q_REG_SYSRAM_RETAIN_GET(Q_CONCAT(I2C_M_, ID)), /* */                                             \
    }

/** @brief Constant data for TWIM instances. */
static const qDrvTWIM_ConstData_t twimConstData[REG_TWIM_INSTANCE_COUNT] = {
#if REG_TWIM_INSTANCE_COUNT > 0
    Q_DRV_TWIM_CONST_DATA_INIT(0),
#endif
#if REG_TWIM_INSTANCE_COUNT > 1
    Q_DRV_TWIM_CONST_DATA_INIT(1),
#endif
};

#define Q_DRV_TWIM_ADDR_WRITE(addr) ((addr) << 1)
#define Q_DRV_TWIM_ADDR_READ(addr)  (((addr) << 1) | 0x01)

#define Q_DRV_TWIM_RX_RETRY_COUNT            10
#define Q_DRV_TWIM_BUSCLEAR_REPETITION_COUNT 3

#define Q_DRV_TWIM_STOP  true
#define Q_DRV_TWIM_START true
#define Q_DRV_TWIM_ACK   true

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static const qDrvTWIM_t* pInstances[REG_TWIM_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance TWIM instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvTWIM_ControlBlock_t* qDrvTWIM_ControlBlockGet(const qDrvTWIM_t* const pInstance)
{
    return (qDrvTWIM_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance TWIM instance pointer.
 *
 * @retval Const data block pointer.
 */
static inline const qDrvTWIM_ConstData_t* qDrvTWIM_ConstDataGet(const qDrvTWIM_t* const pInstance)
{
    return &twimConstData[pInstance->id];
}

/**
 * @brief Get the transfer active flag.
 *
 * @param[in] pInstance TWIM instance pointer.
 *
 * @retval Active state.
 */
static inline Bool qDrvTWIM_ActiveCheck(const qDrvTWIM_t* const pInstance)
{
    return qRegTWIM_EnableGet(pInstance->baseAddr);
}

/**
 * @brief Send a single byte.
 *
 * @param[in] pInstance TWIM instance pointer.
 * @param[in] start     Generate start condition.
 * @param[in] data      Byte to send.
 */
static void qDrvTWIM_Tx(const qDrvTWIM_t* const pInstance, Bool start, UInt8 data)
{
    UInt8 flags = (1U << qRegTWIM_ControlWrite) | (start ? (1U << qRegTWIM_ControlStart) : 0);

    qRegTWIM_TxDataSet(pInstance->baseAddr, data);
    qRegTWIM_ControlSet(pInstance->baseAddr, flags);
}

/**
 * @brief Trigger a single byte reception.
 *
 * @param[in] pInstance TWIM instance pointer.
 * @param[in] ack       Send ack.
 * @param[in] stop      Generate stop condition.
 */
static inline void qDrvTWIM_Rx(const qDrvTWIM_t* const pInstance, Bool ack, Bool stop)
{
    UInt8 flags = (1U << qRegTWIM_ControlRead) | (stop ? (1U << qRegTWIM_ControlStop) : 0);

    qRegTWIM_AckSet(pInstance->baseAddr, !ack);
    qRegTWIM_ControlSet(pInstance->baseAddr, flags);
}

/**
 * @brief Process next byte of the current transfer.
 *
 * @param[in] pInstance TWIM instance pointer.
 */
static inline void qDrvTWIM_TxDoneProcess(const qDrvTWIM_t* const pInstance)
{
    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);

    if(!qDrvTWIM_ActiveCheck(pInstance))
    {
        return;
    }

    if(pCb->trState == qDrvTWIM_TransferStateIdle)
    {
        if(pCb->busClear)
        {
            --pCb->busClear;
            if(!pCb->busClear)
            {
                qRegTWIM_EnableSet(pInstance->baseAddr, false);

                if(pCb->callback)
                {
                    pCb->callback(pCb->arg, qDrvTWIM_EventBusClearDone, NULL);
                }

                return;
            }

            qDrvTWIM_Rx(pInstance, !Q_DRV_TWIM_ACK, Q_DRV_TWIM_STOP);
            return;
        }

        // End of transfer.
        qRegTWIM_EnableSet(pInstance->baseAddr, false);

        if(pCb->callback)
        {
            qDrvTWIM_Event_t evt = pCb->noAck ? qDrvTWIM_EventNoACK : qDrvTWIM_EventDone;
            pCb->callback(pCb->arg, evt, &pCb->transfer);
        }
        return;
    }

    if(pCb->trState == qDrvTWIM_TransferStateTx)
    {
        Bool nack = qRegTWIM_RXAckGet(pInstance->baseAddr);

        if(nack)
        {
            if(pCb->retries)
            {
                --pCb->retries;
                qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_WRITE(pCb->transfer.address));
            }
            else
            {
                pCb->noAck = true;
                pCb->trState = qDrvTWIM_TransferStateIdle;
                qRegTWIM_StopSet(pInstance->baseAddr);
            }

            return;
        }

        if(pCb->txPos >= pCb->transfer.txLen)
        {
            // Last byte transferred.
            if(pCb->transfer.rxLen)
            {
                // Trigger repeated start and read operation.
                pCb->retries = Q_DRV_TWIM_RX_RETRY_COUNT;
                pCb->trState = qDrvTWIM_TransferStateRx;
                qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_READ(pCb->transfer.address));
            }
            else
            {
                // Nothing to read. Generate stop condition.
                pCb->trState = qDrvTWIM_TransferStateIdle;
                qRegTWIM_StopSet(pInstance->baseAddr);
            }
        }
        else
        {
            qDrvTWIM_Tx(pInstance, !Q_DRV_TWIM_START, pCb->transfer.tx[pCb->txPos]);
            ++pCb->txPos;
        }
    }
    else if(pCb->trState == qDrvTWIM_TransferStateRx)
    {
        if(pCb->rxPos)
        {
            pCb->transfer.rx[pCb->rxPos - 1] = qRegTWIM_RxDataGet(pInstance->baseAddr);
        }
        else
        {
            Bool nack = qRegTWIM_RXAckGet(pInstance->baseAddr);
            if(nack)
            {
                if(pCb->retries)
                {
                    --pCb->retries;
                    qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_READ(pCb->transfer.address));
                }
                else
                {
                    pCb->noAck = true;
                    pCb->trState = qDrvTWIM_TransferStateIdle;

                    qRegTWIM_StopSet(pInstance->baseAddr);
                }

                return;
            }
        }

        if(pCb->rxPos >= pCb->transfer.rxLen)
        {
            pCb->trState = qDrvTWIM_TransferStateIdle;
            qRegTWIM_StopSet(pInstance->baseAddr);
        }
        else if(pCb->rxPos >= pCb->transfer.rxLen - 1)
        {
            qDrvTWIM_Rx(pInstance, !Q_DRV_TWIM_ACK, !Q_DRV_TWIM_STOP);
        }
        else
        {
            qDrvTWIM_Rx(pInstance, Q_DRV_TWIM_ACK, !Q_DRV_TWIM_STOP);
        }

        ++pCb->rxPos;
    }
    else
    {
        // Abort.
        qRegTWIM_EnableSet(pInstance->baseAddr, false);
    }

    return;
}

/**
 * @brief TWIM instance interrupt handler.
 *
 * @param[in] pInstance TWIM instance pointer.
 */
static void qDrvTWIM_IRQHandler(const qDrvTWIM_t* const pInstance)
{
    if(!pInstance)
    {
        /* Spurious interrupt. */
        return;
    }

    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);
    const qDrvTWIM_ConstData_t* pCData = qDrvTWIM_ConstDataGet(pInstance);

    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(pCData->irqn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegTWIM_IrqArbLost))
    {
        qRegTWIM_IRQArbLostClr(pInstance->baseAddr);

        if(pCb->retries)
        {
            if(pCb->trState == qDrvTWIM_TransferStateTx)
            {
                qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_WRITE(pCb->transfer.address));
            }
            else if(pCb->trState == qDrvTWIM_TransferStateRx)
            {
                qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_READ(pCb->transfer.address));
            }
            else
            {
                pCb->retries = 0;
                qRegTWIM_EnableSet(pInstance->baseAddr, false);
            }

            return;
        }

        qRegTWIM_EnableSet(pInstance->baseAddr, false);

        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvTWIM_EventArbitrationLost, &pCb->transfer);
        }
    }

    if(maskedEvents & (1UL << qRegTWIM_IrqDone))
    {
        qRegTWIM_IRQDoneClr(pInstance->baseAddr);
        qDrvTWIM_TxDoneProcess(pInstance);
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvTWIM_PinConfigSet(const qDrvTWIM_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    if(pPinConfig->sda.pin == Q_NOPIN || pPinConfig->scl.pin == Q_NOPIN)
    {
        return Q_ERR_INVPARAM;
    }

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    qDrvIOB_ConfigInputSet(pPinConfig->sda.pin, qDrvIOB_PullNone, false);
    qDrvIOB_ConfigInputSet(pPinConfig->scl.pin, qDrvIOB_PullNone, false);

    return Q_OK;
}

qResult_t qDrvTWIM_PinConfigClear(const qDrvTWIM_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvTWIM_Init(const qDrvTWIM_t* pInstance, const qDrvTWIM_Config_t* const pConfig,
                        const qDrvTWIM_Callback_t callback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_TWIM_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);
    const qDrvTWIM_ConstData_t* pCData = qDrvTWIM_ConstDataGet(pInstance);

    if(qDrvTWIM_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    memset(pCb, 0, sizeof(*pCb));

    pCb->arg = arg;
    pCb->callback = callback;

    qRegTWIM_EnableSet(pInstance->baseAddr, false);
    qRegTWIM_ClkSyncDisableSet(pInstance->baseAddr, !pConfig->clockStretching);
    qRegTWIM_PrescalerSet(pInstance->baseAddr, pConfig->freq);
    (void)qRegTWIM_RxDataGet(pInstance->baseAddr);

    qRegIntCtrl_EventIRQDisable(
        pCData->irqn, (1UL << qRegTWIM_IrqArbLost) | (1UL << qRegTWIM_IrqClkSync) | (1UL << qRegTWIM_IrqDone), 0);
    qRegTWIM_IRQArbLostClr(pInstance->baseAddr);
    qRegTWIM_IRQClkSyncClr(pInstance->baseAddr);
    qRegTWIM_IRQDoneClr(pInstance->baseAddr);
    NVIC_SetPriority(pCData->irqn, irqPriority);
    NVIC_ClearPendingIRQ(pCData->irqn);
    NVIC_EnableIRQ(pCData->irqn);
    qRegIntCtrl_IRQEnable(pCData->irqn);
    qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegTWIM_IrqDone) | (1UL << qRegTWIM_IrqArbLost), 0);
    qSysRam_RetainEnableSet(pCData->retain, true);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvTWIM_Deinit(const qDrvTWIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIM_InitCheck(pInstance))
    {
        return Q_OK;
    }

    const qDrvTWIM_ConstData_t* pCData = qDrvTWIM_ConstDataGet(pInstance);

    (void)qDrvTWIM_Abort(pInstance);
    qRegTWIM_EnableSet(pInstance->baseAddr, false);
    qSysRam_RetainEnableSet(pCData->retain, false);
    NVIC_DisableIRQ(pCData->irqn);
    qRegIntCtrl_IRQDisable(pCData->irqn);
    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvTWIM_InitCheck(const qDrvTWIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvTWIM_Transfer(const qDrvTWIM_t* pInstance, const qDrvTWIM_Transfer_t* const pTransfer)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pTransfer);

    if(!qDrvTWIM_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!pTransfer->txLen && !pTransfer->rxLen)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);

    if(qDrvTWIM_ActiveCheck(pInstance))
    {
        return Q_ERR_BUSY;
    }

    memcpy(&pCb->transfer, pTransfer, sizeof(*pTransfer));

    if(!pCb->transfer.tx)
    {
        pCb->transfer.txLen = 0;
    }

    if(!pCb->transfer.rx)
    {
        pCb->transfer.rxLen = 0;
    }

    qRegTWIM_EnableSet(pInstance->baseAddr, true);
    qRegTWIM_AckSet(pInstance->baseAddr, true);

    pCb->noAck = false;
    pCb->txPos = 0;
    pCb->rxPos = 0;
    pCb->retries = Q_DRV_TWIM_RX_RETRY_COUNT;

    if(pCb->transfer.txLen)
    {
        pCb->trState = qDrvTWIM_TransferStateTx;
        qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_WRITE(pTransfer->address));
    }
    else
    {
        pCb->trState = qDrvTWIM_TransferStateRx;
        qDrvTWIM_Tx(pInstance, Q_DRV_TWIM_START, Q_DRV_TWIM_ADDR_READ(pTransfer->address));
    }

    return Q_OK;
}

qResult_t qDrvTWIM_BusClear(const qDrvTWIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIM_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);

    if(qDrvTWIM_ActiveCheck(pInstance))
    {
        return Q_ERR_BUSY;
    }

    qRegTWIM_EnableSet(pInstance->baseAddr, true);
    qRegTWIM_AckSet(pInstance->baseAddr, false);

    pCb->noAck = false;
    pCb->busClear = Q_DRV_TWIM_BUSCLEAR_REPETITION_COUNT;
    pCb->trState = qDrvTWIM_TransferStateIdle;

    qDrvTWIM_Rx(pInstance, !Q_DRV_TWIM_ACK, Q_DRV_TWIM_STOP);

    return Q_OK;
}

qResult_t qDrvTWIM_Abort(const qDrvTWIM_t* pInstance)
{
    if(!qDrvTWIM_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvTWIM_ControlBlock_t* pCb = qDrvTWIM_ControlBlockGet(pInstance);

    if(!qDrvTWIM_ActiveCheck(pInstance))
    {
        return Q_OK;
    }

    pCb->trState = qDrvTWIM_TransferStateAbort;
    qRegTWIM_StopSet(pInstance->baseAddr);

    return Q_OK;
}

Bool qDrvTWIM_BusyCheck(const qDrvTWIM_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIM_InitCheck(pInstance))
    {
        return false;
    }

    return qDrvTWIM_ActiveCheck(pInstance);
}

#if REG_TWIM_INSTANCE_COUNT > 0
Q_REG_TWIM_HANDLER_DEFINE(0)
{
    qDrvTWIM_IRQHandler(pInstances[0]);
}
#endif /* REG_TWIM_INSTANCE_COUNT > 0 */

#if REG_TWIM_INSTANCE_COUNT > 1
Q_REG_TWIM_HANDLER_DEFINE(1)
{
    qDrvTWIM_IRQHandler(pInstances[1]);
}
#endif /* REG_TWIM_INSTANCE_COUNT > 1 */
