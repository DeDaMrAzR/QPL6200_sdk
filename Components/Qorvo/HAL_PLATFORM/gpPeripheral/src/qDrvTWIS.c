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
#include "qDrvTWIS.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvTWIS_State_t */
//@{
/** @brief No active transfer. */
#define qDrvTWIS_StateIdle 0
/** @brief Master write, slave read. */
#define qDrvTWIS_StateRX 1
/** @brief Master read, slave write. */
#define qDrvTWIS_StateTX 2
/** @typedef qDrvTWIS_State_t
 *  @brief TWI Slave transfer state.
 */
typedef UInt8 qDrvTWIS_State_t;
//@}

/** @brief TWIS instance control block */
typedef struct {
    qDrvTWIS_Callback_t callback; /**< Set of user callbacks. */
    void* arg;                    /**< User-defined argument. */
    UInt8* txBuf;                 /**< TX buffer pointer. */
    UInt8* rxBuf;                 /**< RX buffer pointer. */
    UInt16 txLen;                 /**< TX buffer length. */
    UInt16 rxLen;                 /**< RX buffer length. */
    UInt16 txPos;                 /**< Number of bytes send in current transaction. */
    UInt16 rxPos;                 /**< Number of bytes received in current transaction. */
    qDrvTWIS_State_t trState;     /**< TWI transfer state. */
} qDrvTWIS_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvTWIS_ControlBlock_t) == sizeof(qDrvTWIS_InternalData_t));

/** @brief TWIS instance constant data block structure */
typedef struct {
    IRQn_Type irqn;             /**< Peripheral isntance interrupt number. */
    qRegSysRam_Retain_t retain; /**< Instance index in SysRam retain register. */
} qDrvTWIS_ConstData_t;

/** @brief Macro for initializing const data block for TWIS instance. */
#define Q_DRV_TWIS_CONST_DATA_INIT(ID)                                                                                 \
    {                                                                                                                  \
        .irqn = Q_REG_TWIS_IRQN_GET(ID),                              /* */                                            \
            .retain = Q_REG_SYSRAM_RETAIN_GET(Q_CONCAT(I2C_SL_, ID)), /* */                                            \
    }

/** @brief Constant data for TWIS instances. */
static const qDrvTWIS_ConstData_t twisConstData[REG_TWIS_INSTANCE_COUNT] = {
#if REG_TWIS_INSTANCE_COUNT > 0
    Q_DRV_TWIS_CONST_DATA_INIT(0),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
static const qDrvTWIS_t* pInstances[REG_TWIS_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance TWIS instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvTWIS_ControlBlock_t* qDrvTWIS_ControlBlockGet(const qDrvTWIS_t* const pInstance)
{
    return (qDrvTWIS_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance TWIS instance pointer.
 *
 * @retval Const data block pointer.
 */
static inline const qDrvTWIS_ConstData_t* qDrvTWIS_ConstDataGet(const qDrvTWIS_t* const pInstance)
{
    return &twisConstData[pInstance->id];
}

/**
 * @brief TWIS instance transfer complete handler.
 *
 * @param[in] pInstance TWIS instance pointer.
 */
static void qDrvTWIS_TransferComplete(const qDrvTWIS_t* const pInstance)
{
    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);
    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegTWIS_IrqRxNotEmpty), 0);

    UInt16 bytesSent = pCb->txPos;

    if(pCb->trState == qDrvTWIS_StateRX)
    {
        pCb->trState = qDrvTWIS_StateIdle;
        if(pCb->callback && pCb->rxPos)
        {
            pCb->callback(pCb->arg, qDrvTWIS_EventRx, pCb->rxBuf, pCb->rxPos);
        }
    }
    else if(pCb->trState == qDrvTWIS_StateTX)
    {
        UInt8* buf = pCb->txBuf;
        pCb->trState = qDrvTWIS_StateIdle;
        pCb->txBuf = NULL;
        pCb->txPos = 0;

        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvTWIS_EventTx, buf, bytesSent);
        }
    }
}

/**
 * @brief TWIS instance interrupt handler.
 *
 * @param[in] pInstance TWIS instance pointer.
 */
static void qDrvTWIS_IRQHandler(const qDrvTWIS_t* const pInstance)
{
    if(!pInstance)
    {
        /* Spurious interrupt. */
        return;
    }

    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);
    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(pCData->irqn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegTWIS_IrqRxNotEmpty))
    {
        UInt8 rxd = qRegTWIS_RxDataGet(pInstance->baseAddr);
        if(pCb->rxBuf && pCb->rxLen > pCb->rxPos)
        {
            pCb->rxBuf[pCb->rxPos] = rxd;
            ++pCb->rxPos;
        }
    }

    if(maskedEvents & (1UL << qRegTWIS_IrqSlaveAddressed))
    {
        qRegTWIS_IRQSlaveAddressedClr(pInstance->baseAddr);

        if(qRegTWIS_ReadGet(pInstance->baseAddr))
        {
            /* Master read operation. */
            pCb->trState = qDrvTWIS_StateTX;
        }
        else
        {
            /* Master write operation. */
            pCb->trState = qDrvTWIS_StateRX;
            pCb->rxPos = 0;
            pCb->txPos = 0;
            qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegTWIS_IrqRxNotEmpty), 0);
        }
    }

    if(maskedEvents & (1UL << qRegTWIS_IrqStart))
    {
        qRegTWIS_IRQStartClr(pInstance->baseAddr);
        qDrvTWIS_TransferComplete(pInstance);
        pCb->txPos = 0;
    }

    if(maskedEvents & (1UL << qRegTWIS_IrqStop))
    {
        qRegTWIS_IRQStopClr(pInstance->baseAddr);

        qDrvTWIS_TransferComplete(pInstance);
        pCb->txPos = 0;
    }

    if(maskedEvents & ((1UL << qRegTWIS_IrqTxNotFull)))
    {
        if(pCb->txBuf && pCb->txLen && pCb->txPos < pCb->txLen)
        {
            qRegTWIS_TxDataSet(pInstance->baseAddr, pCb->txBuf[pCb->txPos]);
            ++pCb->txPos;
        }
        else
        {
            qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegTWIS_IrqTxNotFull), 0);
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvTWIS_PinConfigSet(const qDrvTWIS_PinConfig_t* const pPinConfig)
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

qResult_t qDrvTWIS_PinConfigClear(const qDrvTWIS_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvTWIS_Init(const qDrvTWIS_t* pInstance, const qDrvTWIS_Config_t* const pConfig,
                        const qDrvTWIS_Callback_t callback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_TWIS_INSTANCE_COUNT);
    Q_ASSERT(pConfig);

    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);
    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    if(qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    memset(pCb, 0, sizeof(*pCb));

    pCb->arg = arg;
    pCb->callback = callback;

    qRegIntCtrl_IRQDisable(pCData->irqn);
    qRegTWIS_EnableSet(pInstance->baseAddr, false);
    qRegTWIS_AcceptGeneralCallSet(pInstance->baseAddr, pConfig->acceptGeneralCall);
    qRegTWIS_SlaveAddressSet(pInstance->baseAddr, pConfig->address);
    (void)qRegTWIS_RxDataGet(pInstance->baseAddr);

    qRegIntCtrl_EventIRQDisable(pCData->irqn,
                                (1UL << qRegTWIS_IrqRxNotEmpty) | (1UL << qRegTWIS_IrqTxNotFull) |
                                    (1UL << qRegTWIS_IrqSlaveAddressed) | (1UL << qRegTWIS_IrqStart) |
                                    (1UL << qRegTWIS_IrqStop) | (1UL << qRegTWIS_IrqTxStretch),
                                0);
    qRegTWIS_IRQSlaveAddressedClr(pInstance->baseAddr);
    qRegTWIS_IRQStartClr(pInstance->baseAddr);
    qRegTWIS_IRQStopClr(pInstance->baseAddr);
    NVIC_SetPriority(pCData->irqn, irqPriority);
    NVIC_ClearPendingIRQ(pCData->irqn);
    NVIC_EnableIRQ(pCData->irqn);
    qRegIntCtrl_EventIRQEnable(
        pCData->irqn, (1UL << qRegTWIS_IrqStart) | (1UL << qRegTWIS_IrqStop) | (1UL << qRegTWIS_IrqSlaveAddressed), 0);
    qSysRam_RetainEnableSet(pCData->retain, true);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvTWIS_Deinit(const qDrvTWIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_OK;
    }

    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    (void)qDrvTWIS_RxBufferSet(pInstance, NULL, 0);
    qRegTWIS_EnableSet(pInstance->baseAddr, false);
    qSysRam_RetainEnableSet(pCData->retain, false);
    NVIC_DisableIRQ(pCData->irqn);
    qRegIntCtrl_IRQDisable(pCData->irqn);
    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvTWIS_InitCheck(const qDrvTWIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvTWIS_RxBufferSet(const qDrvTWIS_t* pInstance, UInt8* pBuffer, UInt16 bufferLen)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);
    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    if(pBuffer && bufferLen)
    {
        NVIC_DisableIRQ(pCData->irqn);

        pCb->rxBuf = pBuffer;
        pCb->rxLen = bufferLen;
        pCb->rxPos = 0;

        NVIC_EnableIRQ(pCData->irqn);
    }
    else
    {
        pCb->rxBuf = NULL;
    }

    return Q_OK;
}

qResult_t qDrvTWIS_Enable(const qDrvTWIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    qRegTWIS_EnableSet(pInstance->baseAddr, true);
    qRegIntCtrl_IRQEnable(pCData->irqn);

    return Q_OK;
}

qResult_t qDrvTWIS_Disable(const qDrvTWIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    qRegTWIS_EnableSet(pInstance->baseAddr, false);
    qRegIntCtrl_IRQDisable(pCData->irqn);

    return Q_OK;
}

qResult_t qDrvTWIS_Tx(const qDrvTWIS_t* pInstance, UInt8* pBuffer, UInt16 bufferLen)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!pBuffer || !bufferLen)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);
    const qDrvTWIS_ConstData_t* pCData = qDrvTWIS_ConstDataGet(pInstance);

    if(pCb->trState == qDrvTWIS_StateTX || (pCb->txBuf && qRegTWIS_BusyGet(pInstance->baseAddr)))
    {
        /* Previous data has already been put into the hardware buffer. */
        return Q_ERR_BUSY;
    }

    pCb->txBuf = pBuffer;
    pCb->txLen = bufferLen;

    qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegTWIS_IrqTxNotFull), 0);

    return Q_OK;
}

Bool qDrvTWIS_BusyCheck(const qDrvTWIS_t* pInstance)
{
    Q_ASSERT(pInstance);

    qDrvTWIS_ControlBlock_t* pCb = qDrvTWIS_ControlBlockGet(pInstance);

    if(qRegTWIS_BusySlaveAddressedGet(pInstance->baseAddr) || (pCb->txBuf && qRegTWIS_BusyGet(pInstance->baseAddr)))
    {
        return true;
    }

    return false;
}

qResult_t qDrvTWIS_ClockStretchingSet(const qDrvTWIS_t* pInstance, Bool clkStretching)
{
    Q_ASSERT(pInstance);

    if(!qDrvTWIS_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegTWIS_SclStretchSet(pInstance->baseAddr, clkStretching);

    return Q_OK;
}

#if REG_TWIS_INSTANCE_COUNT > 0
Q_REG_TWIS_HANDLER_DEFINE(0)
{
    qDrvTWIS_IRQHandler(pInstances[0]);
}
#endif /* REG_TWIS_INSTANCE_COUNT > 0 */
