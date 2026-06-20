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
#include "qDrvIR.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define Q_DRV_IR_MINIMUM_UNIT_PS 31250

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief IR generator instance control block. */
typedef struct {
    qDrvIR_Callback_t callback; /**< Set of user callbacks. */
    void* arg;                  /**< User-defined argument. */
    qDrvIR_Mode_t mode;         /**< Current mode of operation. */
} qDrvIR_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvIR_ControlBlock_t) == sizeof(qDrvIR_InternalData_t));

/* The driver supports only a single IR generator instance. */
#if defined(REG_IR_INSTANCE_COUNT)
Q_STATIC_ASSERT(REG_IR_INSTANCE_COUNT <= 1);
#endif

/** @brief IR generator instance constant data block structure. */
typedef struct {
    IRQn_Type irqn; /**< Peripheral instance interrupt number. */
} qDrvIR_ConstData_t;

/** @brief Macro for initializing const data block for IR generator instance. */
#define Q_DRV_IR_CONST_DATA_INIT(ID)                                                                                   \
    {                                                                                                                  \
        .irqn = Q_REG_IR_IRQN_GET(ID), /* */                                                                           \
    }

/** @brief Constant data for IR generator instances. */
static const qDrvIR_ConstData_t irConstData[REG_IR_INSTANCE_COUNT] = {
#if REG_IR_INSTANCE_COUNT > 0
    Q_DRV_IR_CONST_DATA_INIT(0),
#endif
};

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

static const qDrvIR_t* pInstances[REG_IR_INSTANCE_COUNT];

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/**
 * @brief Calculate the RAM offset value from buffer pointer.
 *
 * @param[in] pInstance Sample buffer pointer.
 *
 * @retval RAM offset for IR sequencer or UINT16_MAX on invalid pointer.
 */
static inline UInt16 qDrvIR_RAMOffsetCalculate(void* pBuf)
{
    if((size_t)pBuf < REG_RAM_START_ADDRESS)
    {
        return UINT16_MAX;
    }
    return (UInt16)(((size_t)pBuf - REG_RAM_START_ADDRESS) / sizeof(UInt16));
}

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @retval Control block pointer.
 */
static inline qDrvIR_ControlBlock_t* qDrvIR_ControlBlockGet(const qDrvIR_t* const pInstance)
{
    return (qDrvIR_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance const data pointer.
 *
 * @param[in] pInstance IR generator instance pointer.
 *
 * @retval Const data block pointer.
 */
static inline const qDrvIR_ConstData_t* qDrvIR_ConstDataGet(const qDrvIR_t* const pInstance)
{
    return irConstData;
}

/**
 * @brief IR generator interrupt handler.
 *
 * @param[in] pInstance IR generator instance pointer.
 */
static void qDrvIR_IRQHandler(const qDrvIR_t* const pInstance)
{
    if(!pInstance)
    {
        /* Spurious interrupt. */
        return;
    }

    UInt32 maskedEvents;
    qDrvIR_ControlBlock_t* pCb = qDrvIR_ControlBlockGet(pInstance);
    const qDrvIR_ConstData_t* pCData = qDrvIR_ConstDataGet(pInstance);

    qRegIntCtrl_EventsIRQMaskedGet(pCData->irqn, &maskedEvents, NULL);

    if(maskedEvents & (1UL << qRegIR_IrqSequenceStart))
    {
        qRegIR_ClrSeqStartIrq();
        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvIR_EventStart);
        }
    }

    if(maskedEvents & (1UL << qRegIR_IrqIndexMatch))
    {
        qRegIR_ClrIndexMatchIrq();
        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvIR_EventMatch);
        }
    }

    if(maskedEvents & (1UL << qRegIR_IrqSequenceRepeat))
    {
        qRegIR_ClrSeqRepeatIrq();
        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvIR_EventRepeat);
        }
    }

    if(maskedEvents & (1UL << qRegIR_IrqSequenceDone))
    {
        qRegIR_ClrSeqDoneIrq();
        if(pCb->callback)
        {
            pCb->callback(pCb->arg, qDrvIR_EventDone);
        }
    }
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvIR_PinConfigSet(const qDrvIR_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    qResult_t res;

    if(pPinConfig->output.pin == Q_NOPIN)
    {
        return Q_ERR_INVPARAM;
    }

    res = qDrvIOB_AlternateArraySet(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
    if(res != Q_OK)
    {
        return res;
    }

    qDrvIOB_ConfigOutputSet(pPinConfig->output.pin, qDrvIOB_Drive12mA, qDrvIOB_SlewRateSlow);

    if(pPinConfig->input.pin != Q_NOPIN)
    {
        qDrvIOB_ConfigInputSet(pPinConfig->input.pin, qDrvIOB_PullNone, false);
    }

    return Q_OK;
}

qResult_t qDrvIR_PinConfigClear(const qDrvIR_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    return qDrvIOB_AlternateArrayClear(pPinConfig->pinArray, Q_ARRAY_SIZE(pPinConfig->pinArray));
}

qResult_t qDrvIR_Init(const qDrvIR_t* pInstance, const qDrvIR_Config_t* const pConfig, const qDrvIR_Callback_t callback,
                      void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);

    qDrvIR_ControlBlock_t* pCb = qDrvIR_ControlBlockGet(pInstance);
    const qDrvIR_ConstData_t* pCData = qDrvIR_ConstDataGet(pInstance);

    if(pInstances[0])
    {
        return Q_ERR_INVSTATE;
    }

    switch(pConfig->outputDrive)
    {
        case qDrvIR_DriveHighSink:
            if(pConfig->outputInvert)
            {
                return Q_ERR_INVPARAM;
            }

            qRegIOB_IRLedPowerSet(true);
            /* Fall through. */

        case qDrvIR_DriveOpenDrain:
            qRegIR_OutputDriveSet(qRegCommon_DriveOD);
            break;

        default:
            qRegIR_OutputDriveSet(qRegCommon_DrivePP);
            break;
    }

    memset(pCb, 0, sizeof(*pCb));

    pCb->arg = arg;
    pCb->callback = callback;

    qRegIR_OutputInvertSet(pConfig->outputInvert);
    qRegIR_InputInvertSet(pConfig->inputInvert);
    qRegIR_RepeatSet(false);
    qRegIR_LatchOnStartSet(true);
    qRegIR_LatchRamSeqOnRepeatSet(true);
    qRegIR_LatchRamSeqOnStartSet(true);
    qRegIR_AlignOnStartSet(true);

    qRegIntCtrl_EventIRQDisable(pCData->irqn,
                                (1UL << qRegIR_IrqSequenceStart) | (1UL << qRegIR_IrqIndexMatch) |
                                    (1UL << qRegIR_IrqSequenceRepeat) | (1UL << qRegIR_IrqSequenceDone),
                                0);
    NVIC_SetPriority(pCData->irqn, irqPriority);
    NVIC_ClearPendingIRQ(pCData->irqn);

    if(pCb->callback)
    {
        qRegIntCtrl_IRQEnable(pCData->irqn);
        NVIC_EnableIRQ(pCData->irqn);
        qRegIR_ClrSeqDoneIrq();

        uint32_t irqMask =
            (1UL << qRegIR_IrqSequenceDone) | (1UL << qRegIR_IrqSequenceStart) | (1UL << qRegIR_IrqSequenceRepeat);
        qRegIntCtrl_EventIRQEnable(pCData->irqn, irqMask, 0);
    }
    else
    {
        qRegIntCtrl_IRQDisable(pCData->irqn);
        NVIC_DisableIRQ(pCData->irqn);
    }

    pInstances[0] = pInstance;

    return Q_OK;
}

qResult_t qDrvIR_Deinit(const qDrvIR_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_OK;
    }

    const qDrvIR_ConstData_t* pCData = qDrvIR_ConstDataGet(pInstance);

    qRegIR_ModulationModeSet(qRegIR_ModeOff);
    qRegIOB_IRLedPowerSet(false);
    NVIC_DisableIRQ(pCData->irqn);
    qRegIntCtrl_IRQDisable(pCData->irqn);

    pInstances[0] = NULL;

    return Q_OK;
}

qResult_t qDrvIR_Abort(const qDrvIR_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegIR_ModulationModeSet(qRegIR_ModeOff);
    qRegIR_RepeatSet(false);

    return Q_OK;
}

qResult_t qDrvIR_Stop(const qDrvIR_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    qRegIR_RepeatSet(false);

    return Q_OK;
}

Bool qDrvIR_BusyCheck(const qDrvIR_t* pInstance)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return false;
    }

    return qRegIR_BusyGet();
}

void qDrvIR_CarrierParamCalculate(const qDrvIR_t* pInstance, UInt32 periodNs, UInt8 dutyCycle,
                                  UInt8* pPrescalerDiv, UInt8* pWrap, UInt8* pThreshold)
{
    Q_ASSERT(pPrescalerDiv);
    Q_ASSERT(pWrap);
    Q_ASSERT(pThreshold);

    UInt8 divider = 0;
    UInt32 period = ((UInt64)periodNs * 1000ULL) / Q_DRV_IR_MINIMUM_UNIT_PS;

    while (period >= 256)
    {
        period = period >> 1;
        divider += 1;
    }

    *pPrescalerDiv = divider;

    if (period !=0)
    {
        if (dutyCycle > 100)
        {
            dutyCycle = 100;
        }

        *pWrap = (UInt8)period - 1;
        period = (UInt8)(udiv_ceil(period * (uint32_t)dutyCycle, 100UL));

        if(period)
        {
            period -= 1;
        }

        *pThreshold = period;
    }
    else
    {
        *pWrap = 0;
        *pThreshold = 0;
    }
}

qResult_t qDrvIR_CarrierPrescalerSet(const qDrvIR_t* pInstance, UInt8 prescalerDiv)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(prescalerDiv > Q_REG_IR_PRESCALER_MAX)
    {
        return Q_ERR_INVPARAM;
    }

    if (qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    qRegIR_PrescalerSet(prescalerDiv);
    return Q_OK;
}

qResult_t qDrvIR_CarrierTimingSet(const qDrvIR_t* pInstance, UInt8 wrap, UInt8 threshold)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if (qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    qRegIR_ThresholdSet(threshold);
    qRegIR_WrapSet(wrap);
    return Q_OK;
}

qResult_t qDrvIR_AltCarrierTimingSet(const qDrvIR_t* pInstance, UInt8 wrap, UInt8 threshold)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if (qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    qRegIR_AltThresholdSet(threshold);
    qRegIR_AltWrapSet(wrap);
    return Q_OK;
}

qResult_t qDrvIR_AltCarrierSet(const qDrvIR_t* pInstance, Bool alt)
{
    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    if(alt)
    {
        qRegIR_AltCarrierSet();
    }
    else
    {
        qRegIR_AltCarrierUnset();
    }

    return Q_OK;
}

qResult_t qDrvIR_IndexMatchSet(const qDrvIR_t* pInstance, UInt8 index, Bool altToggle, Bool irqEnable)
{
    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    const qDrvIR_ConstData_t* pCData = qDrvIR_ConstDataGet(pInstance);

    qRegIR_SeqMatchIndexSet(index);
    qRegIR_ToggleAltCarrierSet(altToggle);

    if(index & irqEnable)
    {
        qRegIntCtrl_EventIRQEnable(pCData->irqn, (1UL << qRegIR_IrqIndexMatch), 0);
    }
    else
    {
        qRegIntCtrl_EventIRQDisable(pCData->irqn, (1UL << qRegIR_IrqIndexMatch), 0);
    }

    return Q_OK;
}

qResult_t qDrvIR_PatternStart(const qDrvIR_t* pInstance, UInt16* pPattern, UInt8 length, Bool repeat)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_INVSTATE;
    }

    UInt16 patternOffset = qDrvIR_RAMOffsetCalculate(pPattern);

    if(patternOffset == UINT16_MAX || !length)
    {
        return Q_ERR_INVPARAM;
    }

    if(qRegIR_BusyGet())
    {
        if(qRegIR_ModulationModeGet() != qRegIR_ModePattern)
        {
            return Q_ERR_BUSY;
        }

        qRegIR_RepeatSet(false);
    }

    qRegIR_ModulationModeSet(qRegIR_ModePattern);
    qRegIR_SeqStartPtrSet(patternOffset);

    /* Actual number of words = length + 1. */
    qRegIR_SeqLengthSet(length - 1);

    qRegIR_RepeatSet(repeat);

    if(!qRegIR_BusyGet())
    {
        qRegIR_SeqStart();
    }

    return Q_OK;
}

qResult_t qDrvIR_PatternTimeUnitSet(const qDrvIR_t* pInstance, UInt8 tu)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(!tu)
    {
        return Q_ERR_INVPARAM;
    }

    if(qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    qRegIR_CarrierTUSet(tu - 1);

    return Q_OK;
}

qResult_t qDrvIR_TimeUnitSet(const qDrvIR_t* pInstance, qDrvIR_TimeUnit_t tu)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(tu > qDrvIR_TimeUnit4us)
    {
        return Q_ERR_INVPARAM;
    }

    if (qRegIR_BusyGet())
    {
        return Q_ERR_BUSY;
    }

    qRegIR_TimerTUSet(tu);

    return Q_OK;
}

qResult_t qDrvIR_TimePatternStart(const qDrvIR_t* pInstance, UInt16* pPattern, UInt8 length, Bool repeat)
{
    Q_ASSERT(pInstance);

    if(pInstances[0] == NULL)
    {
        return Q_ERR_UNINITIALIZED;
    }

    UInt16 patternOffset = qDrvIR_RAMOffsetCalculate(pPattern);

    if(patternOffset == UINT16_MAX || !length || length & 1)
    {
        return Q_ERR_INVPARAM;
    }

    if (qRegIR_BusyGet())
    {
        if (qRegIR_ModulationModeGet() != qRegIR_ModeTime)
        {
            return Q_ERR_BUSY;
        }

        qRegIR_RepeatSet(false);
    }

    qRegIR_ModulationModeSet(qRegIR_ModeTime);
    qRegIR_SeqStartPtrSet(patternOffset);

    /* Actual number of words = length + 1. */
    qRegIR_SeqLengthSet(length - 1);

    qRegIR_RepeatSet(repeat);

    if (!qRegIR_BusyGet())
    {
        qRegIR_SeqStart();
    }

    return Q_OK;
}

#if REG_IR_INSTANCE_COUNT > 0
Q_REG_IR_HANDLER_DEFINE(0)
{
    qDrvIR_IRQHandler(pInstances[0]);
}
#endif /* REG_IR_INSTANCE_COUNT > 0 */
