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

#include "qRegHwInfo.h"
#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qRegSysRam.h"
#include "qDrvLPCMP.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/** @brief LPCMP instance control block. */
typedef struct {
    qDrvLPCMP_Callback_t callback; /**< Set of user callbacks. */
    void* arg;                     /**< User-defined argument. */
    UInt8 slotConfigMask;          /**< Configured slot bitmask. */
    Bool enabled;                  /**< Driver enable state. */
} qDrvLPCMP_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvLPCMP_ControlBlock_t) == sizeof(qDrvLPCMP_InternalData_t));

/* The driver supports only a single LPCMP instance. */
#if defined(REG_LPCMP_INSTANCE_COUNT)
Q_STATIC_ASSERT(REG_LPCMP_INSTANCE_COUNT <= 1);
#endif

/** @brief LPCMP instance constant data block structure. */
typedef struct {
    IRQn_Type irqn; /**< Peripheral instance interrupt number. */
} qDrvLPCMP_ConstData_t;

/** @brief Macro for initializing const data block for LPCMP instance. */
#define Q_DRV_LPCMP_CONST_DATA_INIT(ID)                                                                                \
    {                                                                                                                  \
        .irqn = Q_REG_LPCMP_IRQN_GET(ID), /* */                                                                        \
    }

/** @brief Constant data for LPCMP instances. */
static const qDrvLPCMP_ConstData_t lpcmpConstData[REG_LPCMP_INSTANCE_COUNT] = {
#if REG_LPCMP_INSTANCE_COUNT > 0
    Q_DRV_LPCMP_CONST_DATA_INIT(0),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static const qDrvLPCMP_t* pInstances[REG_LPCMP_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance LPCMP instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvLPCMP_ControlBlock_t* qDrvLPCMP_ControlBlockGet(const qDrvLPCMP_t* const pInstance)
{
    return (qDrvLPCMP_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance LPCMP instance pointer.
 *
 * @retval Const data block pointer.
 */
static inline const qDrvLPCMP_ConstData_t* qDrvLPCMP_ConstDataGet(const qDrvLPCMP_t* const pInstance)
{
    return lpcmpConstData;
}

/**
 * @brief LPCMP interrupt handler.
 *
 * @param[in] pInstance LPCMP instance pointer.
 */
static void qDrvLPCMP_IRQHandler(const qDrvLPCMP_t* const pInstance)
{
    if(!pInstance)
    {
        /* Spurious interrupt. */
        return;
    }

    UInt32 maskedEvents;
    UInt8 slotMask;
    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);
    const qDrvLPCMP_ConstData_t* pCData = qDrvLPCMP_ConstDataGet(pInstance);

    if(!pCb->callback)
    {
        /* Chip wakeup only mode. IRQ not routed to the core. */

        const qDrvLPCMP_ConstData_t* pCData = qDrvLPCMP_ConstDataGet(pInstance);
        NVIC_DisableIRQ(pCData->irqn);

        return;
    }

    qRegIntCtrl_EventsIRQMaskedGet(pCData->irqn, &maskedEvents, NULL);
    slotMask = qRegLPCMP_StatusGet();

    if(maskedEvents & (1UL << qRegLPCMP_IrqMatch))
    {
        qRegLPCMP_EnableSet(false);
        while(slotMask)
        {
            qRegLPCMP_Slot_t slot = (qRegLPCMP_Slot_t)__builtin_ctz((UInt32)slotMask);
            BIT_CLR(slotMask, (UInt32)slot);

            qRegLPCMP_SlotEnableSet((qRegLPCMP_Slot_t)slot, false);
            pCb->callback(pCb->arg, (qRegLPCMP_Slot_t)slot);
        }

        HAL_WAIT_US(10);
        qRegLPCMP_EnableSet(pCb->enabled);
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvLPCMP_PinConfigSet(const qDrvLPCMP_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    if(pPinConfig->anio.pin == Q_NOPIN)
    {
        return Q_ERR_INVPARAM;
    }

    (void)qDrvIOB_Init();

    qResult_t res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    qDrvIOB_ConfigInputSet(pPinConfig->anio.pin, qDrvIOB_PullNone, false);

    return Q_OK;
}

qResult_t qDrvLPCMP_PinConfigClear(const qDrvLPCMP_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvLPCMP_Init(const qDrvLPCMP_t* pInstance, const qDrvLPCMP_Config_t* const pConfig,
                         const qDrvLPCMP_Callback_t callback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);
    const qDrvLPCMP_ConstData_t* pCData = qDrvLPCMP_ConstDataGet(pInstance);

    if(pInstances[0])
    {
        return Q_ERR_INVSTATE;
    }

    memset(pCb, 0, sizeof(*pCb));

    pCb->arg = arg;
    pCb->callback = callback;

    qRegLPCMP_DivFactorSet(pConfig->clkDiv);
    qRegLPCMP_EnableSet(false);
    qRegLPCMP_LPRefPUPSet(true);
    qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegLPCMP_IrqMatch), 0);
    NVIC_SetPriority(pCData->irqn, irqPriority);
    NVIC_ClearPendingIRQ(pCData->irqn);
    qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegLPCMP_IrqMatch), 0);

    for(UInt8 slot = 0; slot < REG_LPCMP_CHANNEL_COUNT; ++slot)
    {
        qRegLPCMP_SlotEnableSet((qRegLPCMP_Slot_t)slot, false);
    }

    if(pCb->callback)
    {
        qRegIntCtrl_IRQEnable(pCData->irqn);
        NVIC_EnableIRQ(pCData->irqn);
    }
    else
    {
        qRegIntCtrl_IRQDisable(pCData->irqn);
        NVIC_DisableIRQ(pCData->irqn);
    }

    pInstances[0] = pInstance;

    return Q_OK;
}

qResult_t qDrvLPCMP_Deinit(const qDrvLPCMP_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance))
    {
        return Q_OK;
    }

    const qDrvLPCMP_ConstData_t* pCData = qDrvLPCMP_ConstDataGet(pInstance);

    qRegLPCMP_EnableSet(false);
    qRegLPCMP_LPRefPUPSet(false);
    NVIC_DisableIRQ(pCData->irqn);
    qRegIntCtrl_IRQDisable(pCData->irqn);

    pInstances[0] = NULL;

    return Q_OK;
}

Bool qDrvLPCMP_InitCheck(const qDrvLPCMP_t* pInstance)
{
    Q_ASSERT(pInstance);

    return pInstances[0] != NULL;
}

qResult_t qDrvLPCMP_EnableSet(const qDrvLPCMP_t* pInstance, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);

    pCb->enabled = enable;
    qRegLPCMP_EnableSet(enable);

    return Q_OK;
}

qResult_t qDrvLPCMP_SlotEnableSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(slot >= REG_LPCMP_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);

    if(!(pCb->slotConfigMask & (1U << slot)))
    {
        return Q_ERR_INVSTATE;
    }

    Bool cmpEnabled = qRegLPCMP_EnableGet();

    qRegLPCMP_EnableSet(false);
    qRegLPCMP_SlotEnableSet(slot, enable);

    if(cmpEnabled && enable)
    {
        HAL_WAIT_US(10);
    }

    qRegLPCMP_EnableSet(cmpEnabled);

    return Q_OK;
}

qResult_t qDrvLPCMP_SlotConfigSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot,
                                  const qDrvLPCMP_SlotConfig_t* pConfig)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(slot >= REG_LPCMP_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    if(pConfig->refSelect == qDrvLPCMP_ReferenceNone)
    {
        /* Reference is not selected. */
        return Q_ERR_INVPARAM;
    }

    if((pConfig->refSelect == qDrvLPCMP_ReferenceExternal) && (pConfig->refANIO == qRegLPCMP_ChannelNone))
    {
        /* External reference selected but not provided. */
        return Q_ERR_INVPARAM;
    }

    UInt8 anioMask = qRegHwInfo_AnioMaskGet(qRegHwInfo_PackageGet());

    if(!(anioMask & (1UL << pConfig->inputANIO)) ||
       ((pConfig->refSelect == qDrvLPCMP_ReferenceExternal) && !(anioMask & (1UL << pConfig->refANIO))))
    {
        /* Anio channels are not available in the package. */
        return Q_ERR_NOTSUPPORTED;
    }

    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);
    Bool cmpEnabled = qRegLPCMP_EnableGet();
    Bool slotEnabled = qRegLPCMP_SlotEnableGet(slot);

    qRegLPCMP_EnableSet(false);
    qRegLPCMP_SlotEnableSet(slot, false);

    qRegLPCMP_SlotInputAINSet(slot, pConfig->inputANIO);
    qRegLPCMP_SlotInputScalingSet(slot, pConfig->inputScale);
    qRegLPCMP_SlotReferenceSet(slot, pConfig->refSelect);
    qRegLPCMP_SlotReferenceAINSet(slot, (pConfig->refSelect == qDrvLPCMP_ReferenceExternal) ? pConfig->refANIO : 0);
    qRegLPCMP_SlotReferenceScalingSet(slot, pConfig->refScale);
    qRegLPCMP_SlotCompareSet(slot, pConfig->match);

    pCb->slotConfigMask |= 1U << slot;

    qRegLPCMP_SlotEnableSet(slot, slotEnabled);
    qRegLPCMP_EnableSet(cmpEnabled);

    return Q_OK;
}

qResult_t qDrvLPCMP_SlotWindowSet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot, Bool enable)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(slot >= REG_LPCMP_CHANNEL_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    qDrvLPCMP_ControlBlock_t* pCb = qDrvLPCMP_ControlBlockGet(pInstance);
    UInt8 slotLow = slot & (~Q_REG_LPCMP_SLOT_MASK_LEN);

    for(UInt8 slotCheck = slotLow; slotCheck < slotLow + (1 << Q_REG_LPCMP_SLOT_MASK_LEN); ++slotCheck)
    {
        if(!(pCb->slotConfigMask & (1U << slotCheck)))
        {
            return Q_ERR_INVSTATE;
        }
    }

    Bool cmpEnabled = qRegLPCMP_EnableGet();
    Bool slotEnabled = qRegLPCMP_SlotEnableGet(slot);

    qRegLPCMP_EnableSet(false);
    qRegLPCMP_SlotEnableSet(slot, false);
    qRegLPCMP_SlotWindowEnableSet(slotLow, enable);
    qRegLPCMP_SlotEnableSet(slot, slotEnabled);
    qRegLPCMP_EnableSet(cmpEnabled);

    return Q_OK;
}

Bool qDrvLPCMP_SlotResultGet(const qDrvLPCMP_t* pInstance, qRegLPCMP_Slot_t slot)
{
    Q_ASSERT(pInstance);

    if(!qDrvLPCMP_InitCheck(pInstance) || slot >= REG_LPCMP_CHANNEL_COUNT)
    {
        return false;
    }

    return qRegLPCMP_SlotResultGet(slot);
}

#if REG_LPCMP_INSTANCE_COUNT > 0
Q_REG_LPCMP_HANDLER_DEFINE(0)
{
    qDrvLPCMP_IRQHandler(pInstances[0]);
}
#endif /* REG_LPCMP_INSTANCE_COUNT > 0 */
