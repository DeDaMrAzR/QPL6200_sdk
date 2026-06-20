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
#include "qRegTimerXL16.h"
#include "qRegTimerXL32.h"
#include "qDrvTimerXL.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief Bit mask for a value defining TimerXL block operation mode. */
#define Q_DRV_TIMERXL_BLOCKMODE_MASK 0x3UL

/** @brief Length in bits of the block mode mask. */
#define Q_DRV_TIMERXL_BLOCKMODE_MASK_LENGTH 2UL

/** @brief Get the index value used for addressing blocks in TimerXL register access interface. */
#define Q_DRV_TIMERXL_BLOCKIDX_GET(IDX) ((IDX) >> 1)

/** @brief Get the timer sequence number within its block. */
#define Q_DRV_TIMERXL_SUBIDX_GET(IDX) ((IDX)&1)

/** @brief Get the index value used for 32-bit TimerXL register access interface. */
#define Q_DRV_TIMERXL_REGIDX_32BIT_GET(IDX) ((IDX) >> 1)

/** @brief Get the index value used for 16-bit TimerXL register access interface. */
#define Q_DRV_TIMERXL_REGIDX_16BIT_GET(IDX) (IDX)

/** @brief Number of 16-bit timer instanced per one TimerXL block. */
#define Q_DRV_TIMERXL_BLOCK_SIZE_16BIT 2UL

/** @brief Number of 32-bit timer instanced per one TimerXL block. */
#define Q_DRV_TIMERXL_BLOCK_SIZE_32BIT 1UL

/** @brief Maximum number of timers within one TimerXL block. */
#define Q_DRV_TIMERXL_BLOCK_SIZE_MAX Q_DRV_TIMERXL_BLOCK_SIZE_16BIT

/** @brief Maximum number of timers in use. */
#define Q_DRV_TIMERXL_TIMER_COUNT (REG_TIMERXL_INSTANCE_COUNT * Q_DRV_TIMERXL_BLOCK_SIZE_MAX)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief TimerXL control block structure */
typedef struct {
    qDrvTimerXL_Cb_t callback;
    void* arg;
    qDrvTimerXL_Mode_t mode;
    UInt8 id;
    UInt8 blockId;
} qDrvTimerXL_ControlBlock_t;

/** @brief TimerXL instance constant data block structure */
typedef struct {
    qRegSysRam_Retain_t retain;
    IRQn_Type irqn;
} qDrvTimerXL_ConstData_t;

/** @brief Macro for initializing const data block for TimerXL instance. */
#define Q_DRV_TIMERXL_CONST_DATA_INIT(ID)                                                                              \
    {                                                                                                                  \
        .retain = Q_REG_SYSRAM_RETAIN_GET(Q_CONCAT(TIMERXL_, ID)), /* Bit index in SysRam retain register. */          \
            .irqn = Q_REG_TIMERXL_IRQN_GET(ID),                    /* Interrupt number. */                             \
    }

Q_STATIC_ASSERT(sizeof(qDrvTimerXL_ControlBlock_t) == sizeof(qDrvTimerXL_Internal_t));

/** @brief TimerXL pool data structure */
typedef struct {
    UInt16 pool;
    UInt16 blockModes;
    qDrvTimerXL_t* pInstances[Q_DRV_TIMERXL_TIMER_COUNT];
} drvTimerXL_Pool_t;

Q_STATIC_ASSERT(sizeof(((drvTimerXL_Pool_t*)0)->pool) * 8 >= Q_DRV_TIMERXL_TIMER_COUNT);
Q_STATIC_ASSERT(sizeof(((drvTimerXL_Pool_t*)0)->blockModes) * 8 >=
                Q_DRV_TIMERXL_BLOCKMODE_MASK_LENGTH * REG_TIMERXL_INSTANCE_COUNT);

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Timer pool data. */
static drvTimerXL_Pool_t timerPool;

/** @brief Constant data for TimerXL instances. */
static const qDrvTimerXL_ConstData_t timerConstData[REG_TIMERXL_INSTANCE_COUNT] = {
    Q_DRV_TIMERXL_CONST_DATA_INIT(0),
#if REG_TIMERXL_INSTANCE_COUNT > 1
    Q_DRV_TIMERXL_CONST_DATA_INIT(1),
#endif
#if REG_TIMERXL_INSTANCE_COUNT > 2
    Q_DRV_TIMERXL_CONST_DATA_INIT(2),
#endif
#if REG_TIMERXL_INSTANCE_COUNT > 3
    Q_DRV_TIMERXL_CONST_DATA_INIT(3),
#endif
#if REG_TIMERXL_INSTANCE_COUNT > 4
    Q_DRV_TIMERXL_CONST_DATA_INIT(4),
#endif
#if REG_TIMERXL_INSTANCE_COUNT > 5
    Q_DRV_TIMERXL_CONST_DATA_INIT(5),
#endif
};

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the TimerXL block instance control block pointer.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvTimerXL_ControlBlock_t* qDrvTimerXL_ControlBlockGet(const qDrvTimerXL_t* const pInstance)
{
    return (qDrvTimerXL_ControlBlock_t*)&pInstance->internal;
}

/**
 * @brief Get the TimerXL block const data pointer.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Const data pointer.
 */
static inline const qDrvTimerXL_ConstData_t* qDrvTimerXL_ConstDataGet(const qDrvTimerXL_t* const pInstance)
{
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);

    return &timerConstData[pCb->blockId];
}

/**
 * @brief Get the TimerXL compare match interrupt mask for.
 *
 * @param[in] pInstance TimerXL instance pointer.
 * @param[in] cc        CC channel number.
 *
 * @retval Interrupt mask.
 */
static UInt32 qDrvTimerXL_CCIrqMaskGet(const qDrvTimerXL_t* pInstance, UInt8 cc)
{
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    UInt8 subIdx = Q_DRV_TIMERXL_SUBIDX_GET(pCb->id);
    UInt8 irqIdx = qRegTimerXL_IrqCC0Match0 + (qRegTimerXL_IrqCC1Match0 - qRegTimerXL_IrqCC0Match0) * cc + subIdx;

    return (1 << irqIdx);
}

/**
 * @brief Get the TimerXL Overflow IRQ mask.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Interrupt mask.
 */
static UInt32 qDrvTimerXL_OverflowIrqMaskGet(const qDrvTimerXL_t* pInstance)
{
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    UInt8 subIdx = Q_DRV_TIMERXL_SUBIDX_GET(pCb->id);

    return (1 << (qRegTimerXL_IrqOverflow0 + subIdx));
}

/**
 * @brief Get the TimerXL block instance interrupt number.
 *
 * @param[in] pInstance TimerXL instance pointer.
 *
 * @retval Interrupt number.
 */
static inline IRQn_Type qDrvTimerXL_IRQnGet(const qDrvTimerXL_t* const pInstance)
{
    const qDrvTimerXL_ConstData_t* pCData = qDrvTimerXL_ConstDataGet(pInstance);

    return pCData->irqn;
}

/**
 * @brief Set the TimerXL block mode in the pool variable.
 *
 * @param[in] blockId TimerXL block instance number.
 * @param[in] mode    Timer mode 16-bit or 32-bit.
 */
static void qDrvTimerXL_BlockModeSet(UInt8 blockId, qDrvTimerXL_Mode_t mode)
{
    UInt8 bitPos = blockId * Q_DRV_TIMERXL_BLOCKMODE_MASK_LENGTH;
    timerPool.blockModes = (timerPool.blockModes & ~(Q_DRV_TIMERXL_BLOCKMODE_MASK << bitPos)) |
                           ((mode & Q_DRV_TIMERXL_BLOCKMODE_MASK) << bitPos);
}

/**
 * @brief Read the TimerXL block mode from the pool variable.
 *
 * @param[in] blockId TimerXL block instance number.
 * @param[in] mode    Timer mode 16-bit or 32-bit.
 *
 * @returns Current timer mode.
 */
static qDrvTimerXL_Mode_t qDrvTimerXL_BlockModeGet(UInt8 blockId)
{
    UInt8 bitPos = blockId * Q_DRV_TIMERXL_BLOCKMODE_MASK_LENGTH;
    return (qDrvTimerXL_Mode_t)(timerPool.blockModes >> bitPos) & Q_DRV_TIMERXL_BLOCKMODE_MASK;
}

/**
 * @brief Try to find a free timer instance within the specific
 *        TimerXL block.
 *
 * @param[in] blockId TimerXL block instance number.
 * @param[in] mode    Timer type to find. 16-bit or 32-bit.
 *
 * @returns Sequential number of allocated instance (which might be different than block number)
 *          or @p Q_NOID on failure.
 */
static inline UInt8 qDrvTimerXL_FreeTimerFind(UInt8 blockId, qDrvTimerXL_Mode_t mode)
{
    qDrvTimerXL_Mode_t currMode = qDrvTimerXL_BlockModeGet(blockId);

    if(currMode != mode && currMode != qDrvTimerXL_ModeNone)
    {
        // Block already used with a different timer bit width.
        return Q_NOID;
    }

    UInt8 blockSize = (mode == qDrvTimerXL_Mode16Bit ? Q_DRV_TIMERXL_BLOCK_SIZE_16BIT : Q_DRV_TIMERXL_BLOCK_SIZE_32BIT);

    // Iterate through the pool bitmak.
    const UInt8 startId = blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    const UInt8 endId = (blockId + 1) * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    for(UInt8 timerId = startId; timerId < endId; timerId += Q_DRV_TIMERXL_BLOCK_SIZE_MAX / blockSize)
    {
        if(!BIT_TST(timerPool.pool, timerId))
        {
            return timerId;
        }
    }

    return Q_NOID;
}

static Bool qDrvTimerXL_AllocCheck(const qDrvTimerXL_t* const pInstance)
{
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    if(pCb->mode != qDrvTimerXL_Mode16Bit && pCb->mode != qDrvTimerXL_Mode32Bit)
    {
        return false;
    }

    if(pCb->id >= Q_DRV_TIMERXL_TIMER_COUNT)
    {
        return false;
    }

    return BIT_TST(timerPool.pool, pCb->id);
}

static qRegTimerXL_t qDrvTimerXL_InitializedBaseAddrGet(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);

    if(!qDrvTimerXL_AllocCheck(pInstance))
    {
        return (qRegTimerXL_t)NULL;
    }

    if(!timerPool.pInstances[pCb->id])
    {
        return (qRegTimerXL_t)NULL;
    }

    UInt8 regId;
    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        regId = Q_DRV_TIMERXL_REGIDX_32BIT_GET(pCb->id);
        return qRegTimerXL32_BaseAddrGet(regId);
    }
    else
    {
        regId = Q_DRV_TIMERXL_REGIDX_16BIT_GET(pCb->id);
        return qRegTimerXL16_BaseAddrGet(regId);
    }
}

static qResult_t qDrvTimerXL_TicksCalculate(const qDrvTimerXL_t* const pInstance, UInt64 ticksIn, UInt32* pTicksOut,
                                            UInt8* pPrescaler)
{
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);
    qResult_t result = Q_OK;
    UInt8 prescaler;

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    UInt32 maxTicks = (pCb->mode == qDrvTimerXL_Mode16Bit) ? UINT16_MAX : UINT32_MAX;

    if(pPrescaler)
    {
        // Increase prescaler value until the tick count can fit in 16/32-bit value.
        prescaler = 0;
        while(ticksIn > maxTicks && prescaler < Q_REG_TIMERXL_PRESCALER_MAX)
        {
            ticksIn /= 2;
            ++prescaler;
        }

        *pPrescaler = prescaler;
    }
    else
    {
        // Prescaler is fixed. Read it from the peripheral and do not modify.
        if(pCb->mode == qDrvTimerXL_Mode32Bit)
        {
            prescaler = qRegTimerXL32_PrescalerGet(baseAddr);
        }
        else
        {
            prescaler = qRegTimerXL16_PrescalerGet(baseAddr);
        }

        ticksIn >>= prescaler;
    }

    if(ticksIn > maxTicks)
    {
        result = Q_ERR_OUTOFRANGE;
        ticksIn = maxTicks;
    }

    *pTicksOut = (UInt32)ticksIn;

    return result;
}

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvTimerXL_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qRegQUIC_TriggerSrc_t qDrvTimerXL_QuicTriggerSrcGet(qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig)
{
    qDrvTimerXL_t* const pTimerInstance = (qDrvTimerXL_t* const)pInstance;

    if(!qDrvTimerXL_AllocCheck(pTimerInstance))
    {
        return Q_ERR_INVSTATE;
    }

    qDrvTimerXL_QuicTriggerConfig_t* srcConfig = (qDrvTimerXL_QuicTriggerConfig_t*)pConfig;

    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pTimerInstance);
    UInt8 blockIdx = Q_DRV_TIMERXL_BLOCKIDX_GET(pCb->id);
    UInt8 subIdx = Q_DRV_TIMERXL_SUBIDX_GET(pCb->id);

    qRegQUIC_TriggerSrc_t triggerSrc = qRegQUIC_TriggerSrcTimerXL0CC0Match0;

    /* Could be: qRegQUIC_TriggerSrcTimerXL0CC0Match0, qRegQUIC_TriggerSrcTimerXL1CC0Match0, ... */
    triggerSrc += ((qRegQUIC_TriggerSrcTimerXL1CC0Match0 - qRegQUIC_TriggerSrcTimerXL0CC0Match0) * blockIdx);

    /* Could be: CC0Match0, CC1Match0, CC2Match0, ... */
    triggerSrc +=
        ((qRegQUIC_TriggerSrcTimerXL0CC1Match0 - qRegQUIC_TriggerSrcTimerXL0CC0Match0) * srcConfig->srcTrigger);

    /* Could be: (Match0 or Match1), (Overflow0 or Overflow1), ... */
    triggerSrc += subIdx;

    return triggerSrc;
};

static qResult_t qDrvTimerXL_QuicActionConfigSet(qRegQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pInstance,
                                                 void* pConfig, UInt32* pActionIdx)
{
    qDrvTimerXL_t* const pTimerInstance = (qDrvTimerXL_t* const)pInstance;

    if(!qDrvTimerXL_AllocCheck(pTimerInstance))
    {
        return Q_ERR_INVSTATE;
    }

    qDrvTimerXL_QuicActionConfig_t* const pTimerConfig = (qDrvTimerXL_QuicActionConfig_t* const)pConfig;

    Q_ASSERT(pTimerConfig->actionIdx < REG_TIMERXL_QUIC_ACTION_COUNT);

    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pTimerInstance);
    qRegTimerXL_t baseAddr = qRegTimerXL32_BaseAddrGet(Q_DRV_TIMERXL_REGIDX_32BIT_GET(pCb->id));
    UInt8 subIdx = Q_DRV_TIMERXL_SUBIDX_GET(pCb->id);

    *pActionIdx = pTimerConfig->actionIdx;

    qRegTimerXL_QUICActionTypeSet(baseAddr, pTimerConfig->actionIdx, pTimerConfig->actionType);
    qRegTimerXL_QUICActionChannelSet(baseAddr, pTimerConfig->actionIdx, channel);
    qRegTimerXL_QUICActionTimerSet(baseAddr, pTimerConfig->actionIdx, subIdx);

    return Q_OK;
}

static qResult_t qDrvTimerXL_QuicActionDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                  qDrvQUIC_Internal_Instance_t* pInstance)
{
    qDrvTimerXL_t* const pTimerInstance = (qDrvTimerXL_t* const)pInstance;

    if(!qDrvTimerXL_AllocCheck(pTimerInstance))
    {
        return Q_ERR_INVSTATE;
    }

    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pTimerInstance);
    qRegTimerXL_t baseAddr = qRegTimerXL32_BaseAddrGet(Q_DRV_TIMERXL_REGIDX_32BIT_GET(pCb->id));

    if(qRegTimerXL_QUICActionChannelGet(baseAddr, actionIdx) == quicChannel)
    {
        qRegTimerXL_QUICActionTypeSet(baseAddr, actionIdx, qRegTimerXL_QuicActionTypeNone);
    }

    return Q_OK;
}

static const qDrvQUIC_Internal_Both_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskBoth,
    .apiSpeedGet = qDrvTimerXL_QuicSpeedGet,
    .apiTriggerSrcGet = qDrvTimerXL_QuicTriggerSrcGet,
    .apiTriggerDisconnect = NULL,
    .apiActionConfigSet = qDrvTimerXL_QuicActionConfigSet,
    .apiActionDisconnect = qDrvTimerXL_QuicActionDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvTimerXL_Alloc(qDrvTimerXL_t* const pInstance, qDrvTimerXL_Mode_t mode)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);

    for(UInt8 blockId = 0; blockId < REG_TIMERXL_INSTANCE_COUNT; ++blockId)
    {
        UInt8 timerId = qDrvTimerXL_FreeTimerFind(blockId, mode);
        if(timerId != Q_NOID)
        {
            qDrvTimerXL_BlockModeSet(blockId, mode);
            BIT_SET(timerPool.pool, timerId);
            pCb->id = timerId;
            pCb->blockId = blockId;
            pCb->mode = mode;
            return Q_OK;
        }
    }

    return Q_ERR_OUTOFMEMORY;
}

qResult_t qDrvTimerXL_Init(qDrvTimerXL_t* const pInstance, const qDrvTimerXL_Config_t* const pConfig,
                           const qDrvTimerXL_Cb_t callback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);

    if(!qDrvTimerXL_AllocCheck(pInstance))
    {
        return Q_ERR_INVSTATE;
    }

    UInt8 blockId = Q_DRV_TIMERXL_BLOCKIDX_GET(pCb->id);
    Bool configIrq = true;
    qRegTimerXL_t blockAddr = qRegTimerXL_BlockAddrGet(blockId);
    qRegTimerXL_t baseAddr;
    UInt8 regId;

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        regId = Q_DRV_TIMERXL_REGIDX_32BIT_GET(pCb->id);
        baseAddr = qRegTimerXL32_BaseAddrGet(regId);
        qRegTimerXL_ModeSet(blockAddr, qRegTimerXL_Mode32Bit);
        qRegTimerXL32_CountModeSet(baseAddr, pConfig->countMode);
        qRegTimerXL32_PrescalerSet(baseAddr, pConfig->prescaler);
    }
    else
    {
        regId = Q_DRV_TIMERXL_REGIDX_16BIT_GET(pCb->id);
        baseAddr = qRegTimerXL16_BaseAddrGet(regId);
        qRegTimerXL_ModeSet(blockAddr, qRegTimerXL_Mode16Bit);
        qRegTimerXL16_CountModeSet(baseAddr, pConfig->countMode);
        qRegTimerXL16_PrescalerSet(baseAddr, pConfig->prescaler);
    }

    const UInt8 startId = blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    const UInt8 endId = (blockId + 1) * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    for(UInt8 id = startId; id < endId; ++id)
    {
        if(timerPool.pInstances[id])
        {
            /* Do not re-configure interrupt if it's shared
               with other initialized timer instance.
             */
            configIrq = false;
        }
    }

    if(configIrq)
    {
        qSysRam_RetainEnableSet(timerConstData[blockId].retain, true);
        qRegIntCtrl_IRQEnable(qDrvTimerXL_IRQnGet(pInstance));
        NVIC_ClearPendingIRQ(qDrvTimerXL_IRQnGet(pInstance));
        NVIC_SetPriority(qDrvTimerXL_IRQnGet(pInstance), irqPriority);
        NVIC_EnableIRQ(qDrvTimerXL_IRQnGet(pInstance));
    }

    pCb->arg = arg;
    pCb->callback = callback;
    timerPool.pInstances[pCb->id] = pInstance;

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    return Q_OK;
}

qResult_t qDrvTimerXL_Deinit(qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_OK;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Stop(baseAddr);
    }
    else
    {
        qRegTimerXL16_Stop(baseAddr);
    }

    UInt8 blockId = Q_DRV_TIMERXL_BLOCKIDX_GET(pCb->id);
    Bool blockDeinit = true;

    /* Disable all timer event interrupts. */
    UInt32 irqMask = qDrvTimerXL_OverflowIrqMaskGet(pInstance);
    for(UInt8 cc = 0; cc < REG_TIMERXL_CC_COUNT; ++cc)
    {
        irqMask |= qDrvTimerXL_CCIrqMaskGet(pInstance, cc);
    }

    qRegIntCtrl_EventIRQDisable(qDrvTimerXL_IRQnGet(pInstance), irqMask, 0);

    timerPool.pInstances[pCb->id] = NULL;
    UInt8 startId = blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    UInt8 endId = (blockId + 1) * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    for(UInt8 id = startId; id < endId; ++id)
    {
        if(timerPool.pInstances[id])
        {
            /* Do not deinitialize TimerXL block if it's shared with other initialized timer. */
            blockDeinit = false;
        }
    }

    if(blockDeinit)
    {
        qRegTimerXL_t blockAddr = qRegTimerXL_BlockAddrGet(blockId);

        qSysRam_RetainEnableSet(timerConstData[blockId].retain, false);
        qRegIntCtrl_IRQDisable(qDrvTimerXL_IRQnGet(pInstance));
        NVIC_DisableIRQ(qDrvTimerXL_IRQnGet(pInstance));
        qRegTimerXL_ModeSet(blockAddr, qRegTimerXL_ModeDisabled);
    }

    return Q_OK;
}

Bool qDrvTimerXL_InitCheck(qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return false;
    }

    return true;
}

qResult_t qDrvTimerXL_Free(qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);

    if(!qDrvTimerXL_AllocCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    (void)qDrvTimerXL_Deinit(pInstance);
    BIT_CLR(timerPool.pool, pCb->id);

    /* Check if there are any initialized instances left withing the TimerXL block
       and clear the block mode if there are none. */
    UInt8 blockSize =
        (pCb->mode == qDrvTimerXL_Mode16Bit ? Q_DRV_TIMERXL_BLOCK_SIZE_16BIT : Q_DRV_TIMERXL_BLOCK_SIZE_32BIT);
    UInt8 blockId = Q_DRV_TIMERXL_BLOCKIDX_GET(pCb->id);
    UInt8 startId = blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;
    UInt8 endId = (blockId + 1) * Q_DRV_TIMERXL_BLOCK_SIZE_MAX;

    pCb->id = Q_NOID;
    pCb->mode = qDrvTimerXL_ModeNone;

    for(UInt8 timerId = startId; timerId < endId; timerId += Q_DRV_TIMERXL_BLOCK_SIZE_MAX / blockSize)
    {
        if(BIT_TST(timerPool.pool, timerId))
        {
            return Q_OK;
        }
    }

    qDrvTimerXL_BlockModeSet(blockId, qDrvTimerXL_ModeNone);
    return Q_OK;
}

qResult_t qDrvTimerXL_Start(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Start(baseAddr);
    }
    else
    {
        qRegTimerXL16_Start(baseAddr);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_Stop(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Stop(baseAddr);
    }
    else
    {
        qRegTimerXL16_Stop(baseAddr);
    }

    return Q_OK;
}

Bool qDrvTimerXL_RunningCheck(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return false;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        return qRegTimerXL32_RunningGet(baseAddr);
    }
    else
    {
        return qRegTimerXL32_RunningGet(baseAddr);
    }
}

qResult_t qDrvTimerXL_Reset(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Reset(baseAddr);
    }
    else
    {
        qRegTimerXL16_Reset(baseAddr);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_Preset(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Preset(baseAddr);
    }
    else
    {
        qRegTimerXL16_Preset(baseAddr);
    }

    return Q_OK;
}

UInt32 qDrvTimerXL_CounterGet(const qDrvTimerXL_t* const pInstance)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return 0;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        return qRegTimerXL32_CounterGet(baseAddr);
    }
    else
    {
        return qRegTimerXL16_CounterGet(baseAddr);
    }
}

qResult_t qDrvTimerXL_PrescalerSet(const qDrvTimerXL_t* const pInstance, UInt8 prescaler)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_PrescalerSet(baseAddr, prescaler);
    }
    else
    {
        qRegTimerXL16_PrescalerSet(baseAddr, prescaler);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_Capture(const qDrvTimerXL_t* const pInstance, UInt8 cc)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_Capture(baseAddr, cc);
    }
    else
    {
        qRegTimerXL16_Capture(baseAddr, cc);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_CCSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, UInt32 value)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_CCValueSet(baseAddr, cc, value);
    }
    else
    {
        if(value > UINT16_MAX)
        {
            return Q_ERR_OUTOFRANGE;
        }

        qRegTimerXL16_CCValueSet(baseAddr, cc, value);
    }

    return Q_OK;
}

UInt32 qDrvTimerXL_CCGet(const qDrvTimerXL_t* const pInstance, UInt8 cc)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return 0;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return 0;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        return qRegTimerXL32_CCValueGet(baseAddr, cc);
    }
    else
    {
        return qRegTimerXL16_CCValueGet(baseAddr, cc);
    }
}

qResult_t qDrvTimerXL_CCSingleShotSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_OneshotSet(baseAddr, cc, enable);
    }
    else
    {
        qRegTimerXL16_OneshotSet(baseAddr, cc, enable);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_CCClrOnMatchSet(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable)
{
    Q_ASSERT(pInstance);
    qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(pCb->mode == qDrvTimerXL_Mode32Bit)
    {
        qRegTimerXL32_ClearOnMatchSet(baseAddr, cc, enable);
    }
    else
    {
        qRegTimerXL16_ClearOnMatchSet(baseAddr, cc, enable);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_CCIrqEnable(const qDrvTimerXL_t* const pInstance, UInt8 cc, Bool enable)
{
    Q_ASSERT(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(cc >= REG_TIMERXL_CC_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    UInt32 irqMask = qDrvTimerXL_CCIrqMaskGet(pInstance, cc);
    if(enable)
    {
        qRegIntCtrl_EventIRQEnable(qDrvTimerXL_IRQnGet(pInstance), irqMask, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(qDrvTimerXL_IRQnGet(pInstance), irqMask, 0);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_OverflowIrqEnable(const qDrvTimerXL_t* const pInstance, Bool enable)
{
    Q_ASSERT(pInstance);
    qRegTimerXL_t baseAddr = qDrvTimerXL_InitializedBaseAddrGet(pInstance);

    if(!baseAddr)
    {
        return Q_ERR_UNINITIALIZED;
    }

    UInt32 irqMask = qDrvTimerXL_OverflowIrqMaskGet(pInstance);
    if(enable)
    {
        qRegIntCtrl_EventIRQEnable(qDrvTimerXL_IRQnGet(pInstance), irqMask, 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(qDrvTimerXL_IRQnGet(pInstance), irqMask, 0);
    }

    return Q_OK;
}

qResult_t qDrvTimerXL_UsecToTicksConvert(const qDrvTimerXL_t* const pInstance, UInt32 usec, UInt32* pTicks,
                                         UInt8* pPrescaler)
{
    Q_ASSERT(pTicks);
    Q_ASSERT(pInstance);

    if(!usec)
    {
        usec = 1;
    }

    UInt64 rawTicks = (UInt64)usec * (REG_TIMERXL_CLK_FREQ / 1000000L);

    return qDrvTimerXL_TicksCalculate(pInstance, rawTicks, pTicks, pPrescaler);
}

qResult_t qDrvTimerXL_HzToTicksConvert(const qDrvTimerXL_t* const pInstance, UInt32 frequency, UInt32* pTicks,
                                       UInt8* pPrescaler)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pTicks);

    if(frequency > REG_TIMERXL_CLK_FREQ || !frequency)
    {
        frequency = REG_TIMERXL_CLK_FREQ;
    }

    UInt32 rawTicks = REG_TIMERXL_CLK_FREQ / frequency - 1;

    return qDrvTimerXL_TicksCalculate(pInstance, rawTicks, pTicks, pPrescaler);
}

static void qDrvTimerXL_IrqHandler(UInt8 blockId)
{
    size_t baseAddr;
    UInt32 maskedEvents;
    IRQn_Type irqn = timerConstData[blockId].irqn;
    qDrvTimerXL_Mode_t mode = qDrvTimerXL_BlockModeGet(blockId);
    qDrvTimerXL_t** pInstances = &timerPool.pInstances[blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX];

    Q_ASSERT(mode != qDrvTimerXL_ModeNone);

    qRegIntCtrl_EventsIRQMaskedGet(irqn, &maskedEvents, NULL);

    for(UInt8 subIdx = 0; maskedEvents && subIdx < Q_DRV_TIMERXL_BLOCK_SIZE_MAX; ++subIdx)
    {
        UInt32 checkMask = 1 << (qRegTimerXL_IrqOverflow0 + subIdx);
        if(maskedEvents & checkMask)
        {
            Q_ASSERT(pInstances[subIdx]);
            maskedEvents &= ~checkMask;

            if(mode == qDrvTimerXL_Mode32Bit)
            {
                baseAddr = qRegTimerXL32_BaseAddrGet(blockId);
                qRegTimerXL32_IRQOverflowClear(baseAddr);
            }
            else
            {
                baseAddr = qRegTimerXL16_BaseAddrGet(blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX + subIdx);
                qRegTimerXL16_IRQOverflowClear(baseAddr);
            }

            qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstances[subIdx]);
            if(pCb->callback)
            {
                qDrvTimerXL_Event_t evt = {.type = qDrvTimerXL_EventTypeOverflow_t, .cc = 0};
                pCb->callback(pCb->arg, evt);
            }
            maskedEvents &= ~checkMask;
        }
    }

    for(UInt8 ccSubChannel = 0; maskedEvents && ccSubChannel < (REG_TIMERXL_CC_COUNT * Q_DRV_TIMERXL_BLOCK_SIZE_MAX);
        ++ccSubChannel)
    {
        UInt32 checkMask = (1 << ccSubChannel);
        if(maskedEvents & checkMask)
        {
            UInt8 subIdx = Q_DRV_TIMERXL_SUBIDX_GET(ccSubChannel);
            maskedEvents &= ~checkMask;

            Q_ASSERT(pInstances[subIdx]);

            if(mode == qDrvTimerXL_Mode32Bit)
            {
                baseAddr = qRegTimerXL32_BaseAddrGet(blockId);
                qRegTimerXL32_IRQMatchClear(baseAddr, ccSubChannel / Q_DRV_TIMERXL_BLOCK_SIZE_MAX);
            }
            else
            {
                baseAddr = qRegTimerXL16_BaseAddrGet(blockId * Q_DRV_TIMERXL_BLOCK_SIZE_MAX + subIdx);
                qRegTimerXL16_IRQMatchClear(baseAddr, ccSubChannel / Q_DRV_TIMERXL_BLOCK_SIZE_MAX);
            }

            qDrvTimerXL_ControlBlock_t* pCb = qDrvTimerXL_ControlBlockGet(pInstances[subIdx]);
            if(pCb->callback)
            {
                qDrvTimerXL_Event_t evt = {.type = qDrvTimerXL_EventTypeCompare_t,
                                           .cc = Q_DRV_TIMERXL_BLOCKIDX_GET(ccSubChannel)};
                pCb->callback(pCb->arg, evt);
            }
        }
    }

    return;
}

void timerxl_0_handler(void)
{
    qDrvTimerXL_IrqHandler(0);
}

#if REG_TIMERXL_INSTANCE_COUNT > 1
void timerxl_1_handler(void)
{
    qDrvTimerXL_IrqHandler(1);
}
#endif

#if REG_TIMERXL_INSTANCE_COUNT > 2
void timerxl_2_handler(void)
{
    qDrvTimerXL_IrqHandler(2);
}
#endif

#if REG_TIMERXL_INSTANCE_COUNT > 3
void timerxl_3_handler(void)
{
    qDrvTimerXL_IrqHandler(3);
}
#endif

#if REG_TIMERXL_INSTANCE_COUNT > 4
void timerxl_4_handler(void)
{
    qDrvTimerXL_IrqHandler(4);
}
#endif

#if REG_TIMERXL_INSTANCE_COUNT > 5
void timerxl_5_handler(void)
{
    qDrvTimerXL_IrqHandler(5);
}
#endif
