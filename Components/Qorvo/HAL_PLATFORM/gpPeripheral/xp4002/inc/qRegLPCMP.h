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

#ifndef _Q_REG_LPCMP_H_
#define _Q_REG_LPCMP_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief LPCMP peripheral base address type. */
typedef size_t qRegLPCMP_t;

#define GP_WB_LPCMP_0_BASE_ADDRESS GP_WB_SENS_AO_BASE_ADDRESS
#define PMUD_0_IRQn                PMUD_IRQn
#define Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET                                                                             \
    (GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_2_AND_3_ADDRESS - GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_ADDRESS)
#define Q_REG_LPCMP_SLOT_ADDR_OFFSET                                                                                   \
    (GP_WB_SENS_AO_LPCMP_CONFIG_A_SLOT_1_ADDRESS - GP_WB_SENS_AO_LPCMP_CONFIG_A_SLOT_0_ADDRESS)
#define Q_REG_LPCMP_SLOT_MASK     0x01
#define Q_REG_LPCMP_SLOT_MASK_LEN 1

/** @brief Get the LPCMP peripheral base address by ID. */
#define Q_REG_LPCMP_BASEADDR_GET(ID) ((qRegLPCMP_t)Q_CONCAT(GP_WB_LPCMP_, ID, _BASE_ADDRESS))

/** @brief Get the LPCMP peripheral interrupt number. */
#define Q_REG_LPCMP_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(PMUD, _IRQn))

/** @brief Header of the LPCMP interrupt handler. */
#define Q_REG_LPCMP_HANDLER_DEFINE(ID) void pmud_handler(void)

/** @enum qRegLPCMP_Irq_t */
//@{
#define qRegLPCMP_IrqMatch GP_WB_INT_CTRL_MASK_APPUC_PMUD_LPCMP_MATCH_INTERRUPT_LSB
/** @typedef qRegLPCMP_Irq_t
 *  @brief LPCMP Event Interrupt.
 */
typedef UInt8 qRegLPCMP_Irq_t;
//@}

/** @enum qRegLPCMP_Reference_t */
//@{
#define qRegLPCMP_ReferenceNone     0
#define qRegLPCMP_ReferenceExternal 1
#define qRegLPCMP_ReferenceInternal 2
#define qRegLPCMP_ReferenceVDD      3
/** @typedef qRegLPCMP_Reference_t
 *  @brief LPCMP Reference selector.
 */
typedef UInt8 qRegLPCMP_Reference_t;
//@}

/** @enum qRegLPCMP_Match_t */
//@{
#define qRegLPCMP_MatchBelow 0
#define qRegLPCMP_MatchAbove 1
/** @typedef qRegLPCMP_Match_t
 *  @brief LPCMP compare mode.
 */
typedef UInt8 qRegLPCMP_Match_t;
//@}

/** @enum qRegLPCMP_Channel_t */
//@{
#define qRegLPCMP_ChannelANIO0 GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_0
#define qRegLPCMP_ChannelANIO1 GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_1
#define qRegLPCMP_ChannelANIO2 GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_2
#define qRegLPCMP_ChannelANIO3 GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_3
#define qRegLPCMP_ChannelNone  Q_NOCHANNEL
/** @typedef qRegLPCMP_Channel_t
 *  @brief LPCMP analog input channel.
 */
typedef UInt8 qRegLPCMP_Channel_t;
//@}

/** @brief LPCMP compare slot. */
typedef UInt8 qRegLPCMP_Slot_t;

/** @brief Set the input scaling factor for a given slot. */
static inline void qRegLPCMP_SlotInputScalingSet(qRegLPCMP_Slot_t slot, UInt8 scaling)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_MASK,
                     (UInt32)scaling << GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_LSB);
}

/** @brief Get the input scaling factor for a given slot. */
static inline UInt8 qRegLPCMP_SlotInputScalingGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    return (UInt8)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_MASK) >>
                   GP_WB_SENS_AO_LPCMP_LVL_INP_SLOT_0_LSB);
}

/** @brief Set the reference scaling factor for a given slot. */
static inline void qRegLPCMP_SlotReferenceScalingSet(qRegLPCMP_Slot_t slot, UInt8 scaling)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_MASK,
                     (UInt32)scaling << GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_LSB);
}

/** @brief Get the reference scaling factor for a given slot. */
static inline UInt8 qRegLPCMP_SlotReferenceScalingGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    return (UInt8)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_MASK) >>
                   GP_WB_SENS_AO_LPCMP_LVL_REF_SLOT_0_LSB);
}

/** @brief Set the reference selector for a given slot. */
static inline void qRegLPCMP_SlotReferenceSet(qRegLPCMP_Slot_t slot, qRegLPCMP_Reference_t ref)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_MASK,
                     (UInt32)ref << GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_LSB);
}

/** @brief Get the reference selector for a given slot. */
static inline qRegLPCMP_Reference_t qRegLPCMP_SlotReferenceGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    return (qRegLPCMP_Reference_t)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_MASK) >>
                                   GP_WB_SENS_AO_LPCMP_REF_SEL_SLOT_0_LSB);
}

/** @brief Set the slot compare value. */
static inline void qRegLPCMP_SlotCompareSet(qRegLPCMP_Slot_t slot, qRegLPCMP_Match_t match)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_MASK,
                     (UInt32)match << GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_LSB);
}

/** @brief Get the slot compare value. */
static inline qRegLPCMP_Match_t qRegLPCMP_SlotCompareGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    return (qRegLPCMP_Match_t)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_MASK) >>
                               GP_WB_SENS_AO_LPCMP_COMPARE_VALUE_SLOT_0_LSB);
}

/** @brief Enable/disable the compare event for a given slot. */
static inline void qRegLPCMP_SlotEnableSet(qRegLPCMP_Slot_t slot, Bool enable)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_MASK,
                     (UInt32)enable << GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_LSB);
}

/** @brief Get the compare event enable state for a given slot. */
static inline Bool qRegLPCMP_SlotEnableGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN)) +
                  (Q_REG_LPCMP_SLOT_ADDR_OFFSET * ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK));
    return (Bool)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_MASK) >>
                  GP_WB_SENS_AO_LPCMP_COMPARE_ENABLE_SLOT_0_LSB);
}

/** @brief Set the analog input for a given slot. */
static inline void qRegLPCMP_SlotInputAINSet(qRegLPCMP_Slot_t slot, qRegLPCMP_Channel_t ain)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    UInt8 shift = ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK) *
                  (GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_1_LSB - GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_LSB);
    UInt32 mask = GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_MASK << shift;
    GP_WB_MWRITE_U32(addr, mask, (UInt32)ain << shift);
}

/** @brief Get the analog input for a given slot. */
static inline qRegLPCMP_Channel_t qRegLPCMP_SlotInputAINGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    UInt8 shift = ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK) *
                  (GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_1_LSB - GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_LSB);
    UInt32 mask = GP_WB_SENS_AO_LPCMP_INPUT_SEL_SLOT_0_MASK << shift;
    return (qRegLPCMP_Channel_t)((GP_WB_READ_U32(addr) & mask) >> shift);
}

/** @brief Set the analog reference input for a given slot. */
static inline void qRegLPCMP_SlotReferenceAINSet(qRegLPCMP_Slot_t slot, qRegLPCMP_Channel_t ain)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    UInt8 shift = ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK) *
                  (GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_1_LSB - GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_LSB);
    UInt32 mask = GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_MASK << shift;
    GP_WB_MWRITE_U32(addr, mask, (UInt32)ain << shift);
}

/** @brief Get the analog input for a given slot. */
static inline qRegLPCMP_Channel_t qRegLPCMP_SlotReferenceAINGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    UInt8 shift = ((UInt32)slot & Q_REG_LPCMP_SLOT_MASK) *
                  (GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_1_LSB - GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_LSB);
    UInt32 mask = GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_MASK << shift;
    return (qRegLPCMP_Channel_t)((GP_WB_READ_U32(addr) & mask) >> shift);
}

/** @brief Set the analog reference input for a given slot. */
static inline void qRegLPCMP_SlotWindowEnableSet(qRegLPCMP_Slot_t slot, Bool enable)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_EXT_REF_SEL_SLOT_0_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    GP_WB_MWRITE_U32(addr, GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_MASK,
                     (UInt32)enable << GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_LSB);
}

/** @brief Get the analog input for a given slot. */
static inline Bool qRegLPCMP_SlotWindowEnableGet(qRegLPCMP_Slot_t slot)
{
    size_t addr = GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_ADDRESS +
                  (Q_REG_LPCMP_SLOT_GROUP_ADDR_OFFSET * ((UInt32)slot >> Q_REG_LPCMP_SLOT_MASK_LEN));
    return (Bool)((GP_WB_READ_U32(addr) & GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_MASK) >>
                  GP_WB_SENS_AO_LPCMP_WINDOW_ENABLE_SLOT_0_AND_1_LSB);
}

/** @brief Read the status register containing the compare results for all slots. */
static inline UInt8 qRegLPCMP_StatusGet(void)
{
    return GP_WB_READ_SENS_AO_LPCMP_STATUS();
}

/** @brief Get the compare result for a given slot. */
static inline Bool qRegLPCMP_SlotResultGet(qRegLPCMP_Slot_t slot)
{
    return (Bool)GP_WB_READ_U1(GP_WB_SENS_AO_LPCMP_STATUS_ADDRESS,
                               GP_WB_SENS_AO_LPCMP_COMPARE_RESULT_SLOT_0_LSB + (UInt32)slot);
}

/** @brief Set the LPCMP clock division factor. */
static inline void qRegLPCMP_DivFactorSet(UInt8 divFactor)
{
    GP_WB_WRITE_SENS_AO_LPCMP_CLK_DIV_FACTOR(divFactor);
}

/** @brief Get the LPCMP clock division factor. */
static inline UInt8 qRegLPCMP_DivFactorGet(void)
{
    return (UInt8)GP_WB_READ_SENS_AO_LPCMP_CLK_DIV_FACTOR();
}

/** @brief Set the power-up force flag. */
static inline void qRegLPCMP_PUPForceSet(Bool enable)
{
    GP_WB_WRITE_SENS_AO_LPCMP_PUP_FORCE(enable);
}

/** @brief Get the power-up force flag. */
static inline Bool qRegLPCMP_PUPForceGet(void)
{
    return (Bool)GP_WB_READ_SENS_AO_LPCMP_PUP_FORCE();
}

/** @brief Set the input MUX power-up force flag. */
static inline void qRegLPCMP_InputSelPUPForceSet(Bool enable)
{
    GP_WB_WRITE_SENS_AO_LPCMP_INPUT_SEL_PUP_FORCE(enable);
}

/** @brief Get the input MUX power-up force flag */
static inline Bool qRegLPCMP_InputSelPUPForceGet(void)
{
    return (Bool)GP_WB_READ_SENS_AO_LPCMP_INPUT_SEL_PUP_FORCE();
}

/** @brief Set the reference MUX power-up force flag. */
static inline void qRegLPCMP_ReferenceSelPUPForceSet(Bool enable)
{
    GP_WB_WRITE_SENS_AO_LPCMP_EXT_REF_SEL_PUP_FORCE(enable);
}

/** @brief Get the reference MUX power-up force flag */
static inline Bool qRegLPCMP_ReferenceSelPUPForceGet(void)
{
    return (Bool)GP_WB_READ_SENS_AO_LPCMP_EXT_REF_SEL_PUP_FORCE();
}

/** @brief Enable/disable the LPCMP peripheral. */
static inline void qRegLPCMP_EnableSet(Bool enable)
{
    GP_WB_WRITE_SENS_AO_LPCMP_ENABLE(enable);
}

/** @brief Get the LPCMP peripheral enable state. */
static inline Bool qRegLPCMP_EnableGet(void)
{
    return (Bool)GP_WB_READ_SENS_AO_LPCMP_ENABLE();
}

/** @brief Set the input MUX power-up force flag. */
static inline void qRegLPCMP_LPRefPUPSet(Bool enable)
{
    GP_WB_WRITE_SENS_AO_SENS_LPREF_PUP(enable);
}

/** @brief Get the input MUX power-up force flag */
static inline Bool qRegLPCMP_LPRefPUPGet(void)
{
    return (Bool)GP_WB_READ_SENS_AO_SENS_LPREF_PUP();
}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_LPCMP_ */
