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

#include "qRegHwInfo.h"
#include "qRegIntCtrl.h"
#include "qRegInternal.h"
#include "qRegSysRam.h"
#include "qDrvDMA.h"
#include "qDrvGPIO.h"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#if REG_GPIO_HAS_TIMESTAMP
typedef struct {
    UInt32* buffer;               /**< Timestamp data buffer. */
    UInt16 bufferLength;          /**< Timestamp buffer length. */
    UInt16 bufferIndex;           /**< Timestamp buffer index. */
    qDrvDMA_Channel_t dmaChannel; /**< Timestamp DMA channel. */
    Bool enabled;                 /**< Timestamp is enabled. */
    Bool mode;                    /**< Timestamp mode. */
} qDrvGPIO_TimestampControlBlock_t;
#endif

/** @brief GPIO callback data */
typedef struct {
    qDrvGPIO_CbInput_t callback; /**< GPIO callback. */
    void* arg;                   /**< User argument. */
} qDrvGPIO_CallbackData_t;

/** @brief GPIO instance control block */
typedef struct {
    const qDrvGPIO_Callbacks_t* callbacks; /**< Set of user callbacks. */
    void* arg;                             /**< User argument. */
#if REG_GPIO_HAS_TIMESTAMP
    qDrvGPIO_TimestampControlBlock_t timestampControlBlock[REG_GPIO_TIMESTAMP_COUNT]; /**< Timestamp control block. */
#endif
    Bool initialized;            /**< Peripheral has been initialized. */
    UInt32 lowPriorityMask;      /**< Mask of GPIO pins with low priority IRQs. */
    UInt32 highPriorityMask;     /**< Mask of GPIO pins with high priority IRQs. */
    UInt32 configuredInputMask;  /**< Mask of GPIO pins configured as inputs. */
    UInt32 configuredOutputMask; /**< Mask of GPIO pins configured as outputs. */
    qDrvGPIO_CallbackData_t gpioCallbacks[REG_GPIO_PIN_COUNT]; /**< Set of callbacks specified by pin. */
    UInt32 inputMask;                                          /**< Mask of GPIO pins available as inputs. */
    UInt32 outputMask;                                         /**< Mask of GPIO pins available as outputs. */
} qDrvGPIO_ControlBlock_t;

#if REG_GPIO_HAS_TIMESTAMP
/** @brief Constant values - GPIO timestamp driver constant data that are unable to be calculated at runtime. */
typedef struct {
    qRegDMA_Trigger_t trigger; /**< DMA source select of the timestamp. */
    UInt32 address;            /**< Timestamp data address register. */
    UInt8 index;               /**< Timestamp index. */
} qDrvGPIO_TimestampConstValues_t;
#endif                         /* REG_GPIO_HAS_TIMESTAMP */

/**
 * @brief Macro for initializing const values required by a GPIO timestamp driver.
 *
 *@param[in] ID Timestamp id.
 */
#define Q_DRV_GPIO_TIMESTAMP_CONST_VALUES_INIT(ID)                                                                     \
    {                                                                                                                  \
        .trigger = Q_CONCAT(qRegDMA_TriggerGPIOTimestampFifo, ID, NotEmpty),                                           \
        .address = Q_CONCAT(GP_WB_GPIO_TIMESTAMP_, ID, _ADDRESS), .index = ID,                                         \
    }

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#if REG_GPIO_HAS_TIMESTAMP
static const qDrvGPIO_TimestampConstValues_t timestampConstValues[REG_GPIO_TIMESTAMP_COUNT] = {
    Q_DRV_GPIO_TIMESTAMP_CONST_VALUES_INIT(0),
    Q_DRV_GPIO_TIMESTAMP_CONST_VALUES_INIT(1),
};
#endif /* REG_GPIO_HAS_TIMESTAMP */

static qDrvGPIO_ControlBlock_t controlBlock;

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static inline Bool qDrvGPIO_InitCheck(void)
{
    return controlBlock.initialized;
}

static qResult_t qDrvGPIO_PinAvailableCheck(UInt8 gpio)
{
    if(gpio >= REG_GPIO_PIN_COUNT)
    {
        return Q_ERR_INVPARAM;
    }

    UInt8 alternate;
    qResult_t res = qDrvIOB_AlternateGet(gpio, &alternate);

    if(res == Q_ERR_INVPARAM)
    {
        /* GPIO pin is not available on this package. */
        return Q_ERR_NOTSUPPORTED;
    }

    if(res != Q_OK)
    {
        /* Pass through all other error codes. */
        return res;
    }

    if(alternate != qDrvIOB_AlternateNone)
    {
        /* GPIO pin is already used by another peripheral. */
        return Q_ERR_BUSY;
    }

    return Q_OK;
}

#if REG_GPIO_HAS_TIMESTAMP
static void qDrvGPIO_CbDmaRx(void* arg, qDrvDMA_EventType_t eventType, void* pChunk, UInt16 wordCount)
{
    switch(eventType)
    {
        case qDrvDMA_EventTypeAlmostComplete:
        {
            UInt8* index = (UInt8*)arg;
            controlBlock.callbacks->timestamp(controlBlock.arg, *index, (UInt32*)pChunk, wordCount);
            break;
        }
        case qDrvDMA_EventTypeBufOverflow:
        {
            /* Not supported yet. */
            Q_ASSERT(false);
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
#endif

#if defined(REG_QUIC_PRESENT)
static qRegQUIC_Speed_t qDrvGPIO_QuicSpeedGet(qDrvQUIC_Internal_Instance_t* pInstance)
{
    (void)pInstance;
    return qRegQUIC_Speed32M;
}

static qRegQUIC_TriggerSrc_t qDrvGPIO_QuicTriggerSrcGet(qDrvQUIC_Internal_Instance_t* pInstance, void* pConfig)
{
    (void)pInstance;

    if(!qDrvGPIO_InitCheck())
    {
        return qRegQUIC_TriggerSrcNoTrigger;
    }

    qDrvGPIO_QuicTriggerConfig_t* triggerConfig = (qDrvGPIO_QuicTriggerConfig_t*)pConfig;

    Q_ASSERT(triggerConfig->triggerIdx < REG_GPIO_QUIC_ACTION_COUNT);

    qResult_t result = qDrvGPIO_PinAvailableCheck(triggerConfig->pinIdx);
    if(result != Q_OK)
    {
        return qRegQUIC_TriggerSrcNoTrigger;
    }

    qRegQUIC_TriggerSrc_t triggerSrc = qRegQUIC_TriggerSrcGpio0 + (triggerConfig->triggerIdx);

    (void)qDrvIOB_Init();

    // TODO: check if this is correct
    if(triggerConfig->mask & qDrvGPIO_QuicConfigMaskDirection)
    {
        qRegGPIO_DirectionSet(triggerConfig->pinIdx, qRegGPIO_DirectionInput);
    }

    if(triggerConfig->mask & qDrvGPIO_QuicConfigMaskAlternate)
    {
        qDrvIOB_AlternateSet(triggerConfig->pinIdx, qDrvIOB_AlternateNone);
    }

    if(triggerConfig->mask & qDrvGPIO_QuicConfigMaskDefault)
    {
        qDrvIOB_ConfigInputSet(triggerConfig->pinIdx, qDrvIOB_PullNone, false);
    }

    qRegGPIO_QuicTriggerGPIOSet(triggerConfig->triggerIdx, triggerConfig->pinIdx);

    BIT_SET(controlBlock.configuredInputMask, triggerConfig->pinIdx);
    BIT_CLR(controlBlock.configuredOutputMask, triggerConfig->pinIdx);

    return triggerSrc;
}

static qResult_t qDrvGPIO_QuicActionConfigSet(qRegQUIC_Channel_t channel, qDrvQUIC_Internal_Instance_t* pInstance,
                                              void* pConfig, UInt32* pActionIdx)
{
    (void)pInstance;

    if(!qDrvGPIO_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qDrvGPIO_QuicActionConfig_t* const pActionConfig = (qDrvGPIO_QuicActionConfig_t*)pConfig;

    Q_ASSERT(pActionConfig->actionIdx < REG_GPIO_QUIC_ACTION_COUNT);

    qResult_t result = qDrvGPIO_PinAvailableCheck(pActionConfig->pinIdx);
    if(result != Q_OK)
    {
        return result;
    }

    if(pActionConfig->mask & qDrvGPIO_QuicConfigMaskDirection)
    {
        qRegGPIO_DirectionSet(pActionConfig->pinIdx, qRegGPIO_DirectionOutput);
    }

    if(pActionConfig->mask & qDrvGPIO_QuicConfigMaskAlternate)
    {
        qDrvIOB_AlternateSet(pActionConfig->pinIdx, qDrvIOB_AlternateNone);
    }

    if(pActionConfig->mask & qDrvGPIO_QuicConfigMaskDefault)
    {
        qDrvIOB_ConfigOutputSet(pActionConfig->pinIdx, qDrvIOB_Drive2mA, qDrvIOB_SlewRateSlow);
    }

    *pActionIdx = pActionConfig->actionIdx;

    qRegGPIO_QuicActionChannelSet(pActionConfig->actionIdx, channel);
    qRegGPIO_QuicActionGPIOSet(pActionConfig->actionIdx, pActionConfig->pinIdx);
    qRegGPIO_QuicActionTypeSet(pActionConfig->actionIdx, pActionConfig->actionType);

    BIT_CLR(controlBlock.configuredInputMask, pActionConfig->pinIdx);
    BIT_SET(controlBlock.configuredOutputMask, pActionConfig->pinIdx);

    return Q_OK;
}

static qResult_t qDrvGPIO_QuicTriggerDisconnect(qRegQUIC_Channel_t quicChannel, UInt32 actionIdx,
                                                qDrvQUIC_Internal_Instance_t* pInstance)
{
    if(qRegGPIO_QuicActionChannelGet(actionIdx) == quicChannel)
    {
        qRegGPIO_QuicActionTypeSet(actionIdx, qRegGPIO_QuicActionTypeNone);
    }

    return Q_OK;
}

static qDrvQUIC_Internal_Both_t quicApi = {
    .apiSupportedMask = qDrvQUIC_Internal_SupportedApiMaskBoth,
    .apiSpeedGet = qDrvGPIO_QuicSpeedGet,
    .apiTriggerSrcGet = qDrvGPIO_QuicTriggerSrcGet,
    .apiTriggerDisconnect = NULL,
    .apiActionConfigSet = qDrvGPIO_QuicActionConfigSet,
    .apiActionDisconnect = qDrvGPIO_QuicTriggerDisconnect,
};
#endif /* defined(REG_QUIC_PRESENT) */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvGPIO_Init(qDrvGPIO_t* const pInstance, const qDrvGPIO_Config_t* pConfig,
                        const qDrvGPIO_Callbacks_t* const pCallbacks, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    if(qDrvGPIO_InitCheck())
    {
        return Q_ERR_INVSTATE;
    }

    memset(&controlBlock, 0, sizeof(controlBlock));

    qRegHwInfo_Package_t package = qRegHwInfo_PackageGet();

    controlBlock.inputMask = qRegHwInfo_InputMaskGet(package);
    controlBlock.outputMask = qRegHwInfo_OutputMaskGet(package);

    qRegIntCtrl_EventIRQDisable(GPIO_0_IRQn, controlBlock.inputMask, 0);
    qRegIntCtrl_EventIRQDisable(GPIO_1_IRQn, controlBlock.inputMask, 0);

    qRegGPIO_MainClkSet(pConfig->sampleClock);
    qRegGPIO_MainClkEnableSet(true);
    qRegGPIO_IRQClkEnableSet(true);
    qSysRam_RetainEnableSet(qRegSysRam_RetainGpio, true);

    NVIC_SetPriority(GPIO_0_IRQn, irqPriority);
    NVIC_SetPriority(GPIO_1_IRQn, irqPriority);

    controlBlock.callbacks = pCallbacks;
    controlBlock.arg = arg;

#if REG_GPIO_HAS_TIMESTAMP
    controlBlock.timestampControlBlock[0].dmaChannel = Q_NOCHANNEL;
    controlBlock.timestampControlBlock[1].dmaChannel = Q_NOCHANNEL;
#endif /* REG_GPIO_HAS_TIMESTAMP */

#if defined(REG_QUIC_PRESENT)
    pInstance->quicApi = (qDrvQUIC_Internal_t*)&quicApi;
#endif /* defined(REG_QUIC_PRESENT) */

    controlBlock.initialized = true;

    return Q_OK;
}

qResult_t qDrvGPIO_Deinit(void)
{
    if(!qDrvGPIO_InitCheck())
    {
        return Q_OK;
    }

    qRegIntCtrl_EventIRQDisable(GPIO_0_IRQn, controlBlock.inputMask, 0);
    qRegIntCtrl_EventIRQDisable(GPIO_1_IRQn, controlBlock.inputMask, 0);

    while(controlBlock.configuredInputMask)
    {
        UInt8 gpio = (UInt8)__builtin_ctz(controlBlock.configuredInputMask);
        BIT_CLR(controlBlock.configuredInputMask, (UInt32)gpio);

        qDrvIOB_ConfigUnusedSet(gpio);
    }

    UInt32 mask = controlBlock.configuredOutputMask;

    while(controlBlock.configuredOutputMask)
    {
        UInt8 gpio = (UInt8)__builtin_ctz(controlBlock.configuredOutputMask);
        BIT_CLR(controlBlock.configuredOutputMask, (UInt32)gpio);

        qRegGPIO_DirectionSet(gpio, qRegGPIO_DirectionInput);

        qDrvIOB_ConfigUnusedSet(gpio);
    }

    // TODO: check if this is correct
    qRegGPIO_OutputMaskSet((~qRegGPIO_OutputMaskGet()) & mask);

    for(UInt8 gpio = 0; gpio < REG_GPIO_PIN_COUNT; ++gpio)
    {
        qRegGPIO_WakeupSet(gpio, qRegGPIO_WakeupNone);
    }

    NVIC_DisableIRQ(GPIO_0_IRQn);
    NVIC_DisableIRQ(GPIO_1_IRQn);

    qRegIntCtrl_IRQDisable(GPIO_0_IRQn);
    qRegIntCtrl_IRQDisable(GPIO_1_IRQn);

    qSysRam_RetainEnableSet(qRegSysRam_RetainGpio, false);
    qRegGPIO_MainClkEnableSet(false);
    qRegGPIO_IRQClkEnableSet(false);

    controlBlock.callbacks = NULL;

#if REG_GPIO_HAS_TIMESTAMP
    (void)qDrvGPIO_TimestampDisable(0);
    (void)qDrvGPIO_TimestampDisable(1);

    if(controlBlock.timestampControlBlock[0].dmaChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(controlBlock.timestampControlBlock[0].dmaChannel);
    }

    if(controlBlock.timestampControlBlock[1].dmaChannel != Q_NOCHANNEL)
    {
        (void)qDrvDMA_ChannelFree(controlBlock.timestampControlBlock[1].dmaChannel);
    }
    memset(controlBlock.timestampControlBlock, 0, sizeof(controlBlock.timestampControlBlock));
#endif /* REG_GPIO_HAS_TIMESTAMP */

    controlBlock.initialized = false;

    return Q_OK;
}

qResult_t qDrvGPIO_InputConfigSet(UInt8 gpio, qDrvGPIO_InputConfig_t* pConfig)
{
    Q_ASSERT(pConfig);

    if(!qDrvGPIO_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qResult_t res = qDrvGPIO_PinAvailableCheck(gpio);
    if(res != Q_OK)
    {
        return res;
    }

    if(!BIT_TST(controlBlock.inputMask, gpio))
    {
        /* GPIO pin can not be an input. */
        return Q_ERR_INVPARAM;
    }

    BIT_CLR(controlBlock.configuredInputMask, gpio);
    BIT_CLR(controlBlock.configuredOutputMask, gpio);

    res = qDrvIOB_ConfigInputSet(gpio, pConfig->pull, pConfig->schmittTrigger);
    if(res != Q_OK)
    {
        return res;
    }

    qRegGPIO_DirectionSet(gpio, qRegGPIO_DirectionInput);
    controlBlock.gpioCallbacks[gpio].callback = pConfig->callback;
    controlBlock.gpioCallbacks[gpio].arg = pConfig->arg;

    BIT_SET(controlBlock.configuredInputMask, gpio);

    res = qDrvGPIO_IrqConfigSet(gpio, pConfig->irqType, pConfig->highPriority);
    if(res != Q_OK)
    {
        return res;
    }

    res = qDrvGPIO_WakeupSet(gpio, pConfig->wakeup);
    if(res != Q_OK)
    {
        return res;
    }

    return Q_OK;
}

qResult_t qDrvGPIO_OutputConfigSet(UInt8 gpio, qDrvGPIO_OutputConfig_t* pConfig)
{
    Q_ASSERT(pConfig);

    if(!qDrvGPIO_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    qResult_t res = qDrvGPIO_PinAvailableCheck(gpio);
    if(res != Q_OK)
    {
        return res;
    }

    if(!BIT_TST(controlBlock.outputMask, gpio))
    {
        /* GPIO pin can not be an output. */
        return Q_ERR_INVPARAM;
    }

    BIT_CLR(controlBlock.configuredInputMask, gpio);
    BIT_CLR(controlBlock.configuredOutputMask, gpio);

    res = qDrvIOB_ConfigOutputSet(gpio, pConfig->drive, pConfig->slewRate);
    if(res != Q_OK)
    {
        return res;
    }

    qRegGPIO_DirectionSet(gpio, qRegGPIO_DirectionOutput);

    BIT_SET(controlBlock.configuredOutputMask, gpio);

    return Q_OK;
}

qResult_t qDrvGPIO_IrqConfigSet(UInt8 gpio, qDrvGPIO_IrqType_t irqType, Bool highPriority)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(gpio >= REG_GPIO_PIN_COUNT)
    {
        /* Invalid GPIO pin number. */
        return Q_ERR_INVPARAM;
    }

    UInt32 gpioMask = 1UL << (UInt32)gpio;

    if((controlBlock.configuredInputMask & gpioMask) == 0)
    {
        /* GPIO pin is not configured as input. */
        return Q_ERR_INVSTATE;
    }

    UInt32 mask0;
    UInt32 mask1;
    qRegIntCtrl_EventIRQEnableGet(GPIO_0_IRQn, &mask0, 0);
    qRegIntCtrl_EventIRQEnableGet(GPIO_1_IRQn, &mask1, 0);

    if((mask0 | mask1) & gpioMask)
    {
        /* GPIO interrupt is already enabled. */
        return Q_ERR_BUSY;
    }

    BIT_CLR(controlBlock.lowPriorityMask, gpio);
    BIT_CLR(controlBlock.highPriorityMask, gpio);

    if(irqType != qDrvGPIO_IrqTypeNone)
    {
        if(highPriority)
        {
            BIT_SET(controlBlock.highPriorityMask, gpio);
        }
        else
        {
            BIT_SET(controlBlock.lowPriorityMask, gpio);
        }
    }

    switch(irqType)
    {
        case qDrvGPIO_IrqTypeHighLevel:
            qRegGPIO_EventIRQTypeSet(gpio, qRegGPIO_IrqTypeLevel);
            qRegGPIO_EventIRQLevelSet(gpio, qRegGPIO_IrqLevelHighOrFalling);
            break;

        case qDrvGPIO_IrqTypeLowLevel:
            qRegGPIO_EventIRQTypeSet(gpio, qRegGPIO_IrqTypeLevel);
            qRegGPIO_EventIRQLevelSet(gpio, qRegGPIO_IrqLevelLowOrRising);
            break;

        case qDrvGPIO_IrqTypeRisingEdge:
            qRegGPIO_EventIRQTypeSet(gpio, qRegGPIO_IrqTypeEdge);
            qRegGPIO_EventIRQLevelSet(gpio, qRegGPIO_IrqLevelLowOrRising);
            break;

        case qDrvGPIO_IrqTypeFallingEdge:
            qRegGPIO_EventIRQTypeSet(gpio, qRegGPIO_IrqTypeEdge);
            qRegGPIO_EventIRQLevelSet(gpio, qRegGPIO_IrqLevelHighOrFalling);
            break;

        default:
            break;
    }

    return Q_OK;
}

qResult_t qDrvGPIO_IrqEnable(UInt8 gpio)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(gpio >= REG_GPIO_PIN_COUNT)
    {
        /* Invalid GPIO pin number. */
        return Q_ERR_INVPARAM;
    }

    if((controlBlock.callbacks == NULL || controlBlock.callbacks->gpio == NULL) &&
       controlBlock.gpioCallbacks[gpio].callback == NULL)
    {
        /* Callbacks are not set. */
        return Q_ERR_INVSTATE;
    }

    IRQn_Type irqnGpio;
    UInt32 gpioMask = 1UL << (UInt32)gpio;

    if(controlBlock.highPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_0_IRQn;
    }
    else if(controlBlock.lowPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_1_IRQn;
    }
    else
    {
        /* GPIO pin is not configured for interrupt. */
        return Q_ERR_INVSTATE;
    }

    UInt32 mask;
    qRegIntCtrl_EventIRQEnableGet(irqnGpio, &mask, NULL);

    if(mask & gpioMask)
    {
        /* GPIO interrupt is already enabled. */
        return Q_ERR_BUSY;
    }

    if(mask == 0)
    {
        /* Enable the interrupts. */
        qRegIntCtrl_IRQEnable(irqnGpio);
        NVIC_EnableIRQ(irqnGpio);
    }

    qRegGPIO_EventIRQClear(gpio);
    qRegIntCtrl_EventIRQEnable(irqnGpio, gpioMask, 0);

    return Q_OK;
}

qResult_t qDrvGPIO_IrqDisable(UInt8 gpio)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(gpio >= REG_GPIO_PIN_COUNT)
    {
        /* Invalid GPIO pin number. */
        return Q_ERR_INVPARAM;
    }

    IRQn_Type irqnGpio;
    UInt32 gpioMask = 1UL << (UInt32)gpio;

    if(controlBlock.highPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_0_IRQn;
    }
    else if(controlBlock.lowPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_1_IRQn;
    }
    else
    {
        /* GPIO pin is not configured for interrupt. */
        return Q_ERR_INVSTATE;
    }

    UInt32 mask;
    qRegIntCtrl_EventIRQEnableGet(irqnGpio, &mask, NULL);
    if((mask & gpioMask) == 0)
    {
        /* GPIO interrupt is already disabled. */
        return Q_ERR_BUSY;
    }

    qRegIntCtrl_EventIRQDisable(irqnGpio, gpioMask, 0);

    qRegIntCtrl_EventIRQEnableGet(irqnGpio, &mask, NULL);
    if(mask == 0)
    {
        /* Disable the interrupts if there is no GPIO pins configured. */
        NVIC_DisableIRQ(irqnGpio);
        qRegIntCtrl_IRQDisable(irqnGpio);
    }

    return Q_OK;
}

Bool qDrvGPIO_IrqEnabledCheck(UInt8 gpio)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return false;
    }

    if(gpio >= REG_GPIO_PIN_COUNT)
    {
        /* Invalid GPIO pin number. */
        return false;
    }

    IRQn_Type irqnGpio;
    UInt32 gpioMask = 1UL << (UInt32)gpio;

    if(controlBlock.highPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_0_IRQn;
    }
    else if(controlBlock.lowPriorityMask & gpioMask)
    {
        irqnGpio = GPIO_1_IRQn;
    }
    else
    {
        /* GPIO pin is not configured for interrupt. */
        return false;
    }

    UInt32 mask;
    qRegIntCtrl_EventIRQEnableGet(irqnGpio, &mask, NULL);

    return (mask & gpioMask) > 0;
}

qResult_t qDrvGPIO_WakeupSet(UInt8 gpio, qDrvGPIO_Wakeup_t wakeup)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(gpio >= REG_GPIO_PIN_COUNT || !BIT_TST(Q_REG_GPIO_WAKEUP_MASK, gpio))
    {
        /* Invalid GPIO pin number for wakeup mode. */
        return Q_ERR_INVPARAM;
    }

    UInt32 gpioMask = 1UL << (UInt32)gpio;
    if((wakeup != qDrvGPIO_WakeupNone) && ((controlBlock.configuredInputMask & gpioMask) == 0))
    {
        /* GPIO pin is not configured as input. */
        return Q_ERR_INVSTATE;
    }

    qRegGPIO_WakeupSet(gpio, wakeup);

    return Q_OK;
}

Bool qDrvGPIO_Read(UInt8 gpio)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return false;
    }

    if((controlBlock.configuredInputMask & (1UL << gpio)) == 0)
    {
        /* GPIO pin is not configured as input. */
        return false;
    }

    return qRegGPIO_InputGet(gpio);
}

UInt32 qDrvGPIO_MaskRead(void)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return 0;
    }

    return qRegGPIO_InputMaskGet() & controlBlock.configuredInputMask;
}

qResult_t qDrvGPIO_Write(UInt8 gpio, Bool value)
{
    if(!qDrvGPIO_InitCheck())
    {
        return Q_ERR_UNINITIALIZED;
    }

    if((controlBlock.configuredOutputMask & (1UL << gpio)) == 0)
    {
        /* GPIO pin is not configured as output. */
        return Q_ERR_INVPARAM;
    }

    qRegGPIO_OutputSet(gpio, value);

    return Q_OK;
}

qResult_t qDrvGPIO_MaskWrite(UInt32 mask)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if((mask & controlBlock.configuredOutputMask) != mask)
    {
        /* Some GPIO pins are not configured as outputs. */
        return Q_ERR_INVPARAM;
    }

    qRegGPIO_OutputMaskSet(mask);

    return Q_OK;
}

Bool qDrvGPIO_OutputGet(UInt8 gpio)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return false;
    }

    if((controlBlock.configuredOutputMask & (1UL << gpio)) == 0)
    {
        /* GPIO pin is not configured as output. */
        return false;
    }

    return qRegGPIO_OutputGet(gpio);
}

UInt32 qDrvGPIO_MaskOutputGet(void)
{
    if(!qDrvGPIO_InitCheck())
    {
        return 0;
    }

    return qRegGPIO_OutputMaskGet() & controlBlock.configuredOutputMask;
}

#if REG_GPIO_HAS_TIMESTAMP
qResult_t qDrvGPIO_TimestampConfigSet(UInt8 tsNum, UInt8 gpio, qDrvGPIO_TimestampMode_t mode)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(gpio >= REG_GPIO_PIN_COUNT || tsNum >= REG_GPIO_TIMESTAMP_COUNT)
    {
        /* Invalid GPIO pin or timestamp index. */
        return Q_ERR_INVPARAM;
    }

    if(qRegGPIO_EventIRQTypeGet(gpio) != qRegGPIO_IrqTypeEdge)
    {
        /* Timestamp can be used only with edge interrupt. */
        return Q_ERR_INVSTATE;
    }

    qDrvGPIO_TimestampControlBlock_t* pTcB = &controlBlock.timestampControlBlock[tsNum];

    if(pTcB->enabled == true)
    {
        /* Timestamp is enabled. */
        return Q_ERR_BUSY;
    }

    switch(tsNum)
    {
        case 0:
            qRegGPIO_Timestamp0SrcSet(gpio);
            break;
        case 1:
            qRegGPIO_Timestamp1SrcSet(gpio);
            break;
        default:
            break;
    }

    if(mode == qDrvGPIO_TimestampModeNone)
    {
        if(pTcB->dmaChannel != Q_NOCHANNEL)
        {
            (void)qDrvDMA_ChannelFree(pTcB->dmaChannel);
            pTcB->dmaChannel = Q_NOCHANNEL;
        }

        pTcB->buffer = NULL;
        pTcB->bufferLength = 0;
        pTcB->bufferIndex = 0;

        return Q_OK;
    }

    if(mode == qDrvGPIO_TimestampModeInterrupt)
    {
        if(controlBlock.callbacks == NULL || controlBlock.callbacks->timestamp == NULL)
        {
            /* Callbacks are not set. */
            return Q_ERR_INVSTATE;
        }
    }

    if(mode == qDrvGPIO_TimestampModeDma)
    {
        if(pTcB->dmaChannel != Q_NOCHANNEL)
        {
            /* DMA channel is already allocated. */
            return Q_ERR_INVSTATE;
        }

        pTcB->dmaChannel = qDrvDMA_Alloc();

        if(pTcB->dmaChannel == Q_NOCHANNEL)
        {
            /* No free DMA channel. */
            return Q_ERR_OUTOFMEMORY;
        }
    }

    controlBlock.timestampControlBlock[tsNum].mode = mode;

    return Q_OK;
}

qResult_t qDrvGPIO_TimestampBufferSet(UInt8 tsNum, UInt32* buffer, UInt16 samplesCount)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(tsNum >= REG_GPIO_TIMESTAMP_COUNT)
    {
        /* Wrong timestamp index. */
        return Q_ERR_INVPARAM;
    }

    /* Non-buffered mode is acceptable. */
    if((buffer != NULL) && (samplesCount < 2))
    {
        /* Buffer size is too small. */
        return Q_ERR_INVPARAM;
    }

    qDrvGPIO_TimestampControlBlock_t* pTcB = &controlBlock.timestampControlBlock[tsNum];

    if(pTcB->enabled == true)
    {
        /* Timestamp is enabled. */
        return Q_ERR_BUSY;
    }

    if(pTcB->mode == qDrvGPIO_TimestampModeNone)
    {
        /* Timestamp is not configured. */
        return Q_ERR_INVSTATE;
    }

    if(pTcB->mode == qDrvGPIO_TimestampModeInterrupt)
    {
        pTcB->buffer = buffer;
        pTcB->bufferIndex = 0;
    }

    if(pTcB->mode == qDrvGPIO_TimestampModeDma)
    {
        if(pTcB->dmaChannel == Q_NOCHANNEL)
        {
            /* DMA channel is not allocated. */
            return Q_ERR_OUTOFMEMORY;
        }

        qDrvGPIO_TimestampConstValues_t const* tConstValues = &timestampConstValues[tsNum];

        qDrvDMA_ChannelConfig_t rxChannelConfig = {
            .callback = &qDrvGPIO_CbDmaRx,
            .periAddress = tConstValues->address,
            .mode = qDrvDMA_ChannelModePeriToMem,
            .bufferMode = qDrvDMA_BufferModeCircular,
            .wordMode = qRegDMA_WordMode32Bit,
            .dmaTriggerSelect = tConstValues->trigger,
        };

        qResult_t result = qDrvDMA_ChannelInit(pTcB->dmaChannel, &rxChannelConfig, (void*)(&(tConstValues->index)));
        if(result != Q_OK)
        {
            qDrvDMA_ChannelFree(pTcB->dmaChannel);

            pTcB->dmaChannel = Q_NOCHANNEL;
            return result;
        }

        result = qDrvDMA_BufferSet(pTcB->dmaChannel, (void*)buffer, samplesCount * sizeof(UInt32));
        if(result != Q_OK)
        {
            return result;
        }
    }

    pTcB->bufferLength = samplesCount;

    return Q_OK;
}

qResult_t qDrvGPIO_TimestampEnable(UInt8 tsNum)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(tsNum >= REG_GPIO_TIMESTAMP_COUNT)
    {
        /* Wrong timestamp index. */
        return Q_ERR_INVPARAM;
    }

    qDrvGPIO_TimestampControlBlock_t* pTcB = &controlBlock.timestampControlBlock[tsNum];

    if(pTcB->mode == qDrvGPIO_TimestampModeNone)
    {
        /* Timestamp is not configured. */
        return Q_ERR_INVSTATE;
    }

    if(pTcB->enabled == true)
    {
        /* Timestamp is already enabled. */
        return Q_ERR_BUSY;
    }

    /* Clear the timestamp FIFO. */
    switch(tsNum)
    {
        case 0:
            (void)qRegGPIO_Timestamp0Get();
            break;
        case 1:
            (void)qRegGPIO_Timestamp1Get();
            break;
        default:
            break;
    }

    pTcB->enabled = true;

    if(pTcB->mode == qDrvGPIO_TimestampModeDma)
    {
        if(pTcB->dmaChannel == Q_NOCHANNEL)
        {
            /* DMA channel is not allocated. */
            return Q_ERR_INVSTATE;
        }

        return qDrvDMA_ChannelEnable(pTcB->dmaChannel, pTcB->bufferLength / 2);
    }
    return Q_OK;
}

qResult_t qDrvGPIO_TimestampDisable(UInt8 tsNum)
{
    if(!qDrvGPIO_InitCheck())
    {
        /* GPIO driver is not initialized. */
        return Q_ERR_UNINITIALIZED;
    }

    if(tsNum >= REG_GPIO_TIMESTAMP_COUNT)
    {
        /* Wrong timestamp index. */
        return Q_ERR_INVPARAM;
    }

    qDrvGPIO_TimestampControlBlock_t* pTcB = &controlBlock.timestampControlBlock[tsNum];

    if(pTcB->enabled == false)
    {
        /* Timestamp is already disabled. */
        return Q_ERR_BUSY;
    }

    if(pTcB->mode == qDrvGPIO_TimestampModeDma)
    {
        if(pTcB->dmaChannel == Q_NOCHANNEL)
        {
            /* DMA channel is not allocated. */
            return Q_ERR_INVSTATE;
        }

        qResult_t result = qDrvDMA_ChannelDisable(pTcB->dmaChannel);

        if(result != Q_OK)
        {
            return result;
        }
    }

    pTcB->enabled = false;

    return Q_OK;
}

Bool qDrvGPIO_TimestampEnabledCheck(UInt8 tsNum)
{
    if(!qDrvGPIO_InitCheck())
    {
        return false;
    }

    if(tsNum >= REG_GPIO_TIMESTAMP_COUNT)
    {
        return false;
    }

    return controlBlock.timestampControlBlock[tsNum].enabled;
}

static void qDrvGPIO_TimestampHandle(UInt8 tsNum, UInt32 data)
{
    qDrvGPIO_TimestampControlBlock_t* pTcB = &controlBlock.timestampControlBlock[tsNum];

    if(pTcB->enabled == false)
    {
        /* Skip when timestamp is not enabled. */
        return;
    }

    if(pTcB->buffer == NULL)
    {
        /* Non buffered mode. */
        controlBlock.callbacks->timestamp(controlBlock.arg, tsNum, &data, 1);
    }
    else
    {
        /* Buffered mode. */
        pTcB->buffer[pTcB->bufferIndex++] = data;

        UInt16 halfBuffer = pTcB->bufferLength / 2;

        if(pTcB->bufferIndex == halfBuffer)
        {
            controlBlock.callbacks->timestamp(controlBlock.arg, tsNum, pTcB->buffer, halfBuffer);
        }

        if(pTcB->bufferIndex == pTcB->bufferLength)
        {
            controlBlock.callbacks->timestamp(controlBlock.arg, tsNum, pTcB->buffer + halfBuffer, halfBuffer);
            pTcB->bufferIndex = 0;
        }
    }
}

#endif /* REG_GPIO_HAS_TIMESTAMP */

static void qDrvGPIO_IRQHandler(IRQn_Type irqnGpio)
{
    UInt32 maskedEvents;
    qRegIntCtrl_EventsIRQMaskedGet(irqnGpio, &maskedEvents, NULL);

    while(maskedEvents != 0)
    {
        UInt8 gpio = (UInt8)__builtin_ctz(maskedEvents);
        BIT_CLR(maskedEvents, (UInt32)gpio);
        qRegGPIO_EventIRQClear(gpio);

        if(controlBlock.gpioCallbacks[gpio].callback)
        {
            /* Run gpio-specified callback if set. */
            controlBlock.gpioCallbacks[gpio].callback(controlBlock.gpioCallbacks[gpio].arg, gpio);
        }
        else if(controlBlock.callbacks && controlBlock.callbacks->gpio)
        {
            /* Run default global callback if set. */
            controlBlock.callbacks->gpio(controlBlock.arg, gpio);
        }
    }

#if REG_GPIO_HAS_TIMESTAMP
    if(qRegGPIO_TimestampFifo0NotEmpty() &&
       (controlBlock.timestampControlBlock[0].mode == qDrvGPIO_TimestampModeInterrupt))
    {
        qDrvGPIO_TimestampHandle(0, qRegGPIO_Timestamp0Get());
    }

    if(qRegGPIO_TimestampFifo1NotEmpty() &&
       (controlBlock.timestampControlBlock[1].mode == qDrvGPIO_TimestampModeInterrupt))
    {
        qDrvGPIO_TimestampHandle(1, qRegGPIO_Timestamp1Get());
    }
#endif /* REG_GPIO_HAS_TIMESTAMP */
}

void gpio_0_handler_impl(void)
{
    qDrvGPIO_IRQHandler(GPIO_0_IRQn);
}

void gpio_1_handler_impl(void)
{
    qDrvGPIO_IRQHandler(GPIO_1_IRQn);
}
