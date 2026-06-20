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
#include "qDrvKeypad.h"

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @enum qDrvKeypad_ButtonAction_t */
//@{
#define qDrvKeypad_ButtonActionNone         0
#define qDrvKeypad_ButtonActionPressed      1
#define qDrvKeypad_ButtonActionPressedEnded 2
#define qDrvKeypad_ButtonActionReleased     3
/** @typedef qDrvKeypad_ButtonAction_t
 *  @brief Button action.
 */
typedef UInt8 qDrvKeypad_ButtonAction_t;
//@}

/** @brief Keypad instance control block. */
typedef struct {
    qDrvKeypad_Cb_t pCallback;              /**< User callbacks. */
    void* arg;                              /**< User-defined argument. */
    UInt8 vector[Q_DRV_KEYPAD_ROWS_COUNT];  /**< Detected keys vector. */
    Bool debounceEnable;                    /**< Debounce enable flag. */
    UInt8 debounceCount;                    /**< Debounce counter. */
    UInt8 buttonRow;                        /**< Detected button row. */
    UInt8 buttonColumn;                     /**< Detected button column. */
    qDrvKeypad_ButtonAction_t buttonAction; /**< Detected button action. */
} qDrvKeypad_ControlBlock_t;

/* Verify that the sizes of the driver internal data and the control block size match. */
Q_STATIC_ASSERT(sizeof(qDrvKeypad_ControlBlock_t) == sizeof(qDrvKeypad_InternalData_t));

/** @brief Keypad instance constant data block structure */
typedef struct {
    IRQn_Type irqn;             /**< Peripheral instance interrupt number. */
    qRegSysRam_Retain_t retain; /**< Instance index in SysRam retain register. */
} qDrvKeypad_ConstData_t;

/** @brief Macro for initializing const data block for Keypad instance. */
#define Q_DRV_KEYPAD_CONST_DATA_INIT(ID)                                                                               \
    {                                                                                                                  \
        .irqn = Q_REG_KEYPAD_IRQN_GET(ID),             /* */                                                           \
            .retain = Q_REG_SYSRAM_RETAIN_GET(KEYPAD), /* */                                                           \
    }

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/** @brief Constant data for Keypad instances. */
static const qDrvKeypad_ConstData_t KeypadConstData[REG_KEYPAD_INSTANCE_COUNT] = {
#if REG_KEYPAD_INSTANCE_COUNT > 0
    Q_DRV_KEYPAD_CONST_DATA_INIT(0),
#endif
};

static const qDrvKeypad_t* pInstances[REG_KEYPAD_INSTANCE_COUNT];

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
static inline qDrvKeypad_ControlBlock_t* qDrvKeypad_ControlBlockGet(qDrvKeypad_t* const pInstance)
{
    return (qDrvKeypad_ControlBlock_t*)(&pInstance->internalData);
}

/**
 * @brief Get the instance control block pointer.
 *
 * @param[in] pInstance Instance pointer.
 *
 * @retval Control block pointer.
 */
static inline const qDrvKeypad_ConstData_t* qDrvKeypad_ConstDataGet(const qDrvKeypad_t* const pInstance)
{
    return &KeypadConstData[pInstance->id];
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

qResult_t qDrvKeypad_PinConfigSet(const qDrvKeypad_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    qResult_t res;

    res = qDrvIOB_AlternateArraySet(pPinConfig->row, pPinConfig->rowCount);
    if(res != Q_OK)
    {
        /* Clear already set pins. */
        (void)qDrvIOB_AlternateArrayClear(pPinConfig->row, pPinConfig->rowCount);
        return res;
    }

    res = qDrvIOB_AlternateArraySet(pPinConfig->column, pPinConfig->columnCount);
    if(res != Q_OK)
    {
        /* Clear already set pins. */
        (void)qDrvIOB_AlternateArrayClear(pPinConfig->row, pPinConfig->rowCount);
        (void)qDrvIOB_AlternateArrayClear(pPinConfig->column, pPinConfig->columnCount);
        return res;
    }

    for(UInt8 n = 0; n < pPinConfig->columnCount; n++)
    {
        UInt8 iobIndex = pPinConfig->column[n].pin;
        qRegIOB_PullModeSet(iobIndex, qRegIOB_PullUp);
        qRegIOB_PullEnableSet(iobIndex, true);
        qRegIOB_InputBufferEnableSet(iobIndex, true);
    }

    for(UInt8 n = 0; n < pPinConfig->rowCount; n++)
    {
        UInt8 iobIndex = pPinConfig->row[n].pin;
        qRegIOB_PullModeSet(iobIndex, qRegIOB_PullUp);
        qRegIOB_PullEnableSet(iobIndex, true);
    }

    return Q_OK;
}

qResult_t qDrvKeypad_PinConfigClear(const qDrvKeypad_PinConfig_t* const pPinConfig)
{
    Q_ASSERT(pPinConfig);

    for(UInt8 n = 0; n < pPinConfig->columnCount; n++)
    {
        UInt8 iobIndex = pPinConfig->column[n].pin;
        qRegIOB_PullEnableSet(iobIndex, false);
        qRegIOB_InputBufferEnableSet(iobIndex, false);
    }

    for(UInt8 n = 0; n < pPinConfig->rowCount; n++)
    {
        UInt8 iobIndex = pPinConfig->row[n].pin;
        qRegIOB_PullEnableSet(iobIndex, false);
    }

    qResult_t res;

    res = qDrvIOB_AlternateArrayClear(pPinConfig->row, pPinConfig->rowCount);
    if(res != Q_OK)
    {
        return res;
    }

    return qDrvIOB_AlternateArrayClear(pPinConfig->column, pPinConfig->columnCount);
}

qResult_t qDrvKeypad_Init(qDrvKeypad_t* const pInstance, const qDrvKeypad_Config_t* const pConfig,
                          qDrvKeypad_Cb_t const pCallback, void* arg, UInt32 irqPriority)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pConfig);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(qDrvKeypad_InitCheck(pInstance))
    {
        /* Instance is already initialized. */
        return Q_ERR_INVSTATE;
    }

    if(pCallback == NULL)
    {
        /* Callback is mandatory. */
        return Q_ERR_INVPARAM;
    }

    if((pConfig->cdp % 250) || (pConfig->rdp % 250))
    {
        /* Charge period must be a multiple of 250 or 0. */
        return Q_ERR_INVPARAM;
    }

    qDrvKeypad_ControlBlock_t* pCb = qDrvKeypad_ControlBlockGet(pInstance);

    memset(pCb, 0, sizeof(*pCb));

    qRegKeypad_t baseAddr = pInstance->baseAddr;

    UInt32 cdp = (pConfig->cdp == 0) ? Q_REG_KEYPAD_CHARGE_PERIOD_DISABLED : ((pConfig->cdp / 250UL) - 1);

    qRegKeypad_PassiveColumnsChargePeriodSet(
        baseAddr, (pConfig->mode == qDrvKeypad_ModePassiveColumn) ? cdp : Q_REG_KEYPAD_CHARGE_PERIOD_DISABLED);

    qRegKeypad_ActiveColumnsChargePeriodSet(
        baseAddr, (pConfig->mode == qDrvKeypad_ModeActiveColumn) ? cdp : Q_REG_KEYPAD_CHARGE_PERIOD_DISABLED);

    UInt32 rdp = (pConfig->rdp >= 250UL) ? ((pConfig->rdp / 250UL) - 1) : 0;
    qRegKeypad_RowDrivePeriodSet(baseAddr, rdp);

    qSysRam_RetainEnableSet(qDrvKeypad_ConstDataGet(pInstance)->retain, true);

    pCb->pCallback = pCallback;
    pCb->arg = arg;

    pCb->debounceEnable = true;
    pCb->debounceCount = 100;

    NVIC_SetPriority(qDrvKeypad_ConstDataGet(pInstance)->irqn, irqPriority);

    pInstances[pInstance->id] = pInstance;

    return Q_OK;
}

qResult_t qDrvKeypad_Deinit(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return Q_OK;
    }

    (void)qDrvKeypad_Disable(pInstance);

    qSysRam_RetainEnableSet(qDrvKeypad_ConstDataGet(pInstance)->retain, false);

    pInstances[pInstance->id] = NULL;

    return Q_OK;
}

Bool qDrvKeypad_InitCheck(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    return pInstances[pInstance->id] != NULL;
}

qResult_t qDrvKeypad_Enable(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegKeypad_EnableGet(pInstance->baseAddr) == true)
    {
        /* Keypad is enabled. */
        return Q_ERR_INVSTATE;
    }

    qDrvKeypad_ControlBlock_t* pCb = qDrvKeypad_ControlBlockGet(pInstance);

    qRegKeypad_EnableSet(pInstance->baseAddr, true);

    qRegKeypad_ScanTrigger(pInstance->baseAddr);

    while(qRegKeypad_IRQUnmaskedScanDoneCheck(pInstance->baseAddr) == false)
    {
        ;
    }

    for(UInt8 i = 0; i < Q_DRV_KEYPAD_ROWS_COUNT; i++)
    {
        pCb->vector[i] = qRegKeypad_KeyVectorGet(pInstance->baseAddr, i);
    }

    qRegKeypad_IRQScanDoneClear(pInstance->baseAddr);

    qRegIntCtrl_EventIRQEnable(qDrvKeypad_ConstDataGet(pInstance)->irqn, (1UL << qRegKeypad_IrqScanDone), 0);

    qRegIntCtrl_IRQEnable(qDrvKeypad_ConstDataGet(pInstance)->irqn);
    NVIC_ClearPendingIRQ(qDrvKeypad_ConstDataGet(pInstance)->irqn);
    NVIC_EnableIRQ(qDrvKeypad_ConstDataGet(pInstance)->irqn);

    return Q_OK;
}

qResult_t qDrvKeypad_Disable(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegKeypad_EnableGet(pInstance->baseAddr) == false)
    {
        /* Keypad is already disabled. */
        return Q_ERR_INVSTATE;
    }

    qRegKeypad_EnableSet(pInstance->baseAddr, false);

    qRegIntCtrl_EventIRQDisable(qDrvKeypad_ConstDataGet(pInstance)->irqn, (1UL << qRegKeypad_IrqScanDone), 0);
    qRegIntCtrl_IRQDisable(qDrvKeypad_ConstDataGet(pInstance)->irqn);
    NVIC_DisableIRQ(qDrvKeypad_ConstDataGet(pInstance)->irqn);
    NVIC_ClearPendingIRQ(qDrvKeypad_ConstDataGet(pInstance)->irqn);

    return Q_OK;
}

Bool qDrvKeypad_EnableCheck(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return false;
    }

    return qRegKeypad_EnableGet(pInstance->baseAddr);
}

qResult_t qDrvKeypad_Trigger(qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegKeypad_EnableGet(pInstance->baseAddr) == false)
    {
        /* Keypad is not enabled. */
        return Q_ERR_INVSTATE;
    }

    qRegKeypad_ScanTrigger(pInstance->baseAddr);

    return Q_OK;
}

qResult_t qDrvKeypad_DebounceSet(qDrvKeypad_t* const pInstance, Bool enable, UInt8 debounceCount)
{
    Q_ASSERT(pInstance);
    Q_ASSERT(pInstance->id < REG_KEYPAD_INSTANCE_COUNT);

    if(!qDrvKeypad_InitCheck(pInstance))
    {
        return Q_ERR_UNINITIALIZED;
    }

    if(qRegKeypad_EnableGet(pInstance->baseAddr) == true)
    {
        /* Keypad is enabled. */
        return Q_ERR_INVSTATE;
    }

    if(enable && debounceCount == 0)
    {
        /* Debounce count must be greater than 0. */
        return Q_ERR_INVPARAM;
    }

    qDrvKeypad_ControlBlock_t* pCb = qDrvKeypad_ControlBlockGet(pInstance);

    pCb->debounceEnable = enable;
    pCb->debounceCount = debounceCount;

    return Q_OK;
}

static inline void qDrvKeypad_IndicateButtonChange(qDrvKeypad_ControlBlock_t* pCb, UInt8 row, UInt8 column,
                                                   Bool pressed)
{
    if(pCb->pCallback)
    {
        pCb->pCallback(pCb->arg, row, column, pressed);
    }
}

static void qDrvKeypad_IRQHandler(const qDrvKeypad_t* const pInstance)
{
    Q_ASSERT(pInstance);

    qDrvKeypad_ControlBlock_t* pCb = qDrvKeypad_ControlBlockGet((qDrvKeypad_t* const)pInstance);
    qRegKeypad_t baseAddr = pInstance->baseAddr;

    /* There is only one interrupt available. */
    qRegKeypad_IRQScanDoneClear(baseAddr);

    if(qRegKeypad_ResultsChangedGet(baseAddr))
    {
        qRegKeypad_ClearResultsChanged(baseAddr);

        UInt8 keyVector;
        UInt8 diff;
        UInt8 column;

        /* Go though all scanned vectors. */
        for(UInt8 row = 0; row < Q_DRV_KEYPAD_ROWS_COUNT; row++)
        {
            keyVector = qRegKeypad_KeyVectorGet(baseAddr, row);

            /* Check if the vector has changed. */
            diff = keyVector ^ pCb->vector[row];

            if(diff)
            {
                /* Update the vector. */
                pCb->vector[row] = keyVector;
            }

            /* Go through all bits in the vector and check whether the button is pressed or released. */
            while(diff)
            {
                column = __builtin_ctz((UInt32)diff);
                BIT_CLR(diff, (UInt32)column);

                Bool pressed = keyVector & (1UL << column);

                if(pCb->debounceEnable == true)
                {
                    if(pCb->buttonAction == qDrvKeypad_ButtonActionNone)
                    {
                        pCb->buttonRow = row;
                        pCb->buttonColumn = column;
                    }

                    if((pCb->buttonRow != row) || (pCb->buttonColumn != column))
                    {
                        continue;
                    }

                    if(pressed)
                    {
                        if(pCb->buttonAction == qDrvKeypad_ButtonActionNone)
                        {
                            pCb->buttonAction = qDrvKeypad_ButtonActionPressed;
                        }
                        else if(pCb->buttonAction == qDrvKeypad_ButtonActionReleased)
                        {
                            pCb->buttonAction = qDrvKeypad_ButtonActionPressedEnded;
                        }
                        else
                        {
                            pCb->buttonAction = qDrvKeypad_ButtonActionNone;
                        }
                    }
                    else
                    {
                        if(pCb->buttonAction == qDrvKeypad_ButtonActionPressedEnded)
                        {
                            pCb->buttonAction = qDrvKeypad_ButtonActionReleased;
                        }
                        else if(pCb->buttonAction != qDrvKeypad_ButtonActionPressedEnded)
                        {
                            pCb->buttonAction = qDrvKeypad_ButtonActionNone;
                        }
                    }
                }
                else
                {
                    qDrvKeypad_IndicateButtonChange(pCb, row, column, pressed);
                }
            }
        }
    }
    else if(qRegKeypad_ResultsValidGet(pInstance->baseAddr) && pCb->debounceEnable == true &&
            pCb->buttonAction != qDrvKeypad_ButtonActionNone)
    {
        UInt8 stable = qRegKeypad_ResultsStableCounterGet(pInstance->baseAddr);
        if(stable >= pCb->debounceCount)
        {
            if(pCb->buttonAction == qDrvKeypad_ButtonActionPressed)
            {
                qDrvKeypad_IndicateButtonChange(pCb, pCb->buttonRow, pCb->buttonColumn, true);
                pCb->buttonAction = qDrvKeypad_ButtonActionPressedEnded;
            }
            else if(pCb->buttonAction == qDrvKeypad_ButtonActionReleased)
            {
                qDrvKeypad_IndicateButtonChange(pCb, pCb->buttonRow, pCb->buttonColumn, false);
                pCb->buttonAction = qDrvKeypad_ButtonActionNone;
            }
        }
    }
}

void keypad_handler_impl(void)
{
    qDrvKeypad_IRQHandler(pInstances[0]);
}
