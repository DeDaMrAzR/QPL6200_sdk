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

#ifndef _Q_REG_INT_CTRL_H_
#define _Q_REG_INT_CTRL_H_

#include "qRegCommon.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate interrupt index based on IRQn_Type.
 *        The DMA_SCB_IRQn is the only one with two interrupt registers.
 *        So every interrupt register with index greater then DMA_SCB_IRQn needs to be increased by 1.
 *        In this case, decreasing by 1 is skipped.
 * */
#define Q_REG_INT_CTRL_INTERRUPT_INDEX_GET(IRQN) (((IRQN) <= DMA_SCB_IRQn) ? ((IRQN)-1) : (IRQN))

/**
 * @brief Address of the first unmasked interrupts register in the Interrupt Tree Register map.
 *        Should be used to check whether any interrupt from the peripheral occurred.
 */
#define Q_REG_INT_CTRL_UNMASKED_INTERRUPTS_START_ADDRESS GP_WB_INT_CTRL_UNMASKED_APPUC_INTERRUPTS_0_ADDRESS

/**
 * @brief Address of the first masked interrupt register in the Interrupt Tree Register map.
 *        Should be used to check whether any interrupt from the peripheral occurred.
 */
#define Q_REG_INT_CTRL_MASKED_INTERRUPT_START_ADDRESS GP_WB_INT_CTRL_MASKED_APPUC_INTERRUPTS_0_ADDRESS

/**
 * @brief Address of the first masked interrupts register in the Interrupt Tree Register map.
 *        Each next word represents a register with masked events from the specified peripheral.
 *        Should be used to determine which events occurred.
 */
#define Q_REG_INT_CTRL_MASKED_EVENTS_INTERRUPTS_START_ADDRESS GP_WB_INT_CTRL_MASKED_APPUC_AKUMA_MAIN_INTERRUPT_ADDRESS

/**
 * @brief Address of the first mask interrupt register in the Interrupt Tree Register map.
 *        Each next word represents a register with peripheral events that can be enabled.
 */
#define Q_REG_INT_CTRL_MASK_EVENTS_INTERRUPTS_START_ADDRESS GP_WB_INT_CTRL_MASK_APPUC_AKUMA_MAIN_INTERRUPT_ADDRESS

/**
 * @brief Enable the interrupt for the specified peripheral.
 *
 * @param[in] irqn   Interrupt number used by the peripheral.
 */
static inline void qRegIntCtrl_IRQEnable(IRQn_Type irqn)
{
    /* AKUMA_IRQn is the first soc interrupt. */
    UInt32 interruptIndex = (UInt32)(irqn - AKUMA_IRQn);
    UInt32 mask = (1UL << (interruptIndex % 32UL));
    UInt32 interruptsAddress = ((interruptIndex / 32UL) ? GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_1_ADDRESS
                                                        : GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_0_ADDRESS);

    GP_WB_MWRITE_U32(interruptsAddress, mask, mask);
}

/**
 * @brief Disable the interrupt for the specified peripheral.
 *
 * @param[in] irqn   Interrupt number used by the peripheral.
 */
static inline void qRegIntCtrl_IRQDisable(IRQn_Type irqn)
{
    /* AKUMA_IRQn is the first soc interrupt. */
    UInt32 interruptIndex = (UInt32)(irqn - AKUMA_IRQn);
    UInt32 mask = (1UL << (interruptIndex % 32UL));
    UInt32 interruptsAddress = ((interruptIndex / 32UL) ? GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_1_ADDRESS
                                                        : GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_0_ADDRESS);

    GP_WB_MWRITE_U32(interruptsAddress, mask, 0);
}

/**
 * @brief Check if the interrupt is enabled.
 *
 * @param[in] irqn Interrupt number used by the peripheral.
 *
 * @retval True if the interrupt is enabled, false otherwise.
 */
static inline Bool qRegIntCtrl_IRQEnableCheck(IRQn_Type irqn)
{
    /* AKUMA_IRQn is the first soc interrupt. */
    UInt32 interruptIndex = (UInt32)(irqn - AKUMA_IRQn);
    UInt32 mask = (1UL << (interruptIndex % 32UL));
    UInt32 interruptsAddress = ((interruptIndex / 32UL) ? GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_1_ADDRESS
                                                        : GP_WB_INT_CTRL_MASK_APPUC_INTERRUPTS_0_ADDRESS);

    return (GP_WB_READ_U32(interruptsAddress) & mask) == mask;
}

/**
 * @brief Get the register with masked events from the specified peripheral.
 *
 * @param[in] irqn  Interrupt number used by the peripheral.
 * @param[in] mask0 Pointer to the variable where the first word of the mask will be stored.
 * @param[in] mask1 Pointer to the variable where the second word of the mask will be stored. Used only for
 * DMA_SCB_IRQn.
 */
static inline void qRegIntCtrl_EventsIRQMaskedGet(IRQn_Type irqn, UInt32* mask0, UInt32* mask1)
{
    UInt32 addressOffset = ((UInt32)Q_REG_INT_CTRL_INTERRUPT_INDEX_GET(irqn)) * sizeof(UInt32);
    UInt32 eventsInterruptsMaskAddress = Q_REG_INT_CTRL_MASKED_EVENTS_INTERRUPTS_START_ADDRESS + addressOffset;

    if(mask0 != NULL)
    {
        *mask0 = GP_WB_READ_U32(eventsInterruptsMaskAddress);
    }

    /* Only DMA_SCB_IRQn interrupt has two mask registers. */
    if((mask1 != NULL) && (irqn == DMA_SCB_IRQn))
    {
        *mask1 = GP_WB_READ_U32(eventsInterruptsMaskAddress + sizeof(UInt32));
    }
}

/**
 * @brief Enable events from the specified peripheral.
 *
 * @param[in] irqn  Interrupt number used by the peripheral.
 * @param[in] mask0 First mask of events to be enabled.
 * @param[in] mask1 Second mask of events to be enabled. Used only for DMA_SCB_IRQn.
 */
static inline void qRegIntCtrl_EventIRQEnable(IRQn_Type irqn, UInt32 mask0, UInt32 mask1)
{
    UInt32 addressOffset = ((UInt32)Q_REG_INT_CTRL_INTERRUPT_INDEX_GET(irqn)) * sizeof(UInt32);
    UInt32 eventsInterruptAddress = Q_REG_INT_CTRL_MASK_EVENTS_INTERRUPTS_START_ADDRESS + addressOffset;

    if(mask0)
    {
        GP_WB_MWRITE_U32(eventsInterruptAddress, mask0, mask0);
    }

    if(mask1 && (irqn == DMA_SCB_IRQn))
    {
        GP_WB_MWRITE_U32(eventsInterruptAddress + sizeof(UInt32), mask1, mask1);
    }
}

/**
 * @brief Disable events from the specified peripheral.
 *
 * @param[in] irqn  Interrupt number used by the peripheral.
 * @param[in] mask0 First mask of events to be enabled.
 * @param[in] mask1 Second mask of events to be enabled. Used only for DMA_SCB_IRQn.
 */
static inline void qRegIntCtrl_EventIRQDisable(IRQn_Type irqn, UInt32 mask0, UInt32 mask1)
{
    UInt32 addressOffset = ((UInt32)Q_REG_INT_CTRL_INTERRUPT_INDEX_GET(irqn)) * sizeof(UInt32);
    UInt32 eventsInterruptAddress = Q_REG_INT_CTRL_MASK_EVENTS_INTERRUPTS_START_ADDRESS + addressOffset;

    if(mask0)
    {
        GP_WB_MWRITE_U32(eventsInterruptAddress, mask0, 0UL);
    }

    if(mask1 && (irqn == DMA_SCB_IRQn))
    {
        GP_WB_MWRITE_U32(eventsInterruptAddress + sizeof(UInt32), mask1, 0UL);
    }
}

/**
 * @brief Get the register with enabled events from the specified peripheral.
 *
 * @param[in] irqn  Interrupt number used by the peripheral.
 * @param[in] mask0 Pointer to the variable where the first word of the mask will be stored.
 * @param[in] mask1 Pointer to the variable where the second word of the mask will be stored. Used only for
 * DMA_SCB_IRQn.
 */
static inline void qRegIntCtrl_EventIRQEnableGet(IRQn_Type irqn, UInt32* mask0, UInt32* mask1)
{
    UInt32 addressOffset = ((UInt32)Q_REG_INT_CTRL_INTERRUPT_INDEX_GET(irqn)) * sizeof(UInt32);
    UInt32 eventsInterruptAddress = Q_REG_INT_CTRL_MASK_EVENTS_INTERRUPTS_START_ADDRESS + addressOffset;

    if(mask0 != NULL)
    {
        *mask0 = GP_WB_READ_U32(eventsInterruptAddress);
    }

    if((mask1 != NULL) && (irqn == DMA_SCB_IRQn))
    {
        *mask1 = GP_WB_READ_U32(eventsInterruptAddress + sizeof(UInt32));
    }
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_INT_CTRL_H_ */
