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

#ifndef _Q_REG_PWMXL_H_
#define _Q_REG_PWMXL_H_

#include "qReg.h"
#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_WB_PWMXLS_0_BASE_ADDRESS GP_WB_PWMXLS_BASE_ADDRESS

/** @brief Pack the single waveform entry parameter value into 16-bit register value. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_PACK(FIELD, VALUE)                                                                  \
    (((VALUE) & ~(UINT32_MAX << Q_CONCAT(GP_WB_PWMXLS, FIELD, NBITS)))                                                 \
     << (Q_CONCAT(GP_WB_PWMXLS, FIELD, LSB) - GP_WB_PWMXLS_VALUE_NBITS))

/** @brief Pack all waveform entry parameters into 16-bit register value. */
#define Q_REG_PWMXL_WAVEFORM_PARAMS_PACK(VALUE_TYPE, IDX, VALIDATE_PHASE, VALIDATE_WIDTH, VALIDATE_DITHER)             \
    (Q_REG_PWMXL_WAVEFORM_PARAM_PACK(_VALUE_TYPE_, VALUE_TYPE) | Q_REG_PWMXL_WAVEFORM_PARAM_PACK(_PWMXL_IDX_, IDX) |   \
     Q_REG_PWMXL_WAVEFORM_PARAM_PACK(_VALIDATE_PHASE_, VALIDATE_PHASE) |                                               \
     Q_REG_PWMXL_WAVEFORM_PARAM_PACK(_VALIDATE_WIDTH_, VALIDATE_WIDTH) |                                               \
     Q_REG_PWMXL_WAVEFORM_PARAM_PACK(_VALIDATE_DITHER_, VALIDATE_DITHER))

/** @brief Extract a single field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_GET(FIELD, VALUE)                                                                   \
    (((VALUE) >> (Q_CONCAT(GP_WB_PWMXLS, FIELD, LSB) - GP_WB_PWMXLS_VALUE_NBITS)) &                                    \
     ~(UINT32_MAX << Q_CONCAT(GP_WB_PWMXLS, FIELD, NBITS)))

/** @brief Extract value type field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_VALUETYPE_GET(VALUE) Q_REG_PWMXL_WAVEFORM_PARAM_GET(_VALUE_TYPE_, VALUE)

/** @brief Extract index field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_IDX_GET(VALUE) Q_REG_PWMXL_WAVEFORM_PARAM_GET(_PWMXL_IDX_, VALUE)

/** @brief Extract validate phase field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEPHASE_GET(VALUE) Q_REG_PWMXL_WAVEFORM_PARAM_GET(_VALIDATE_PHASE_, VALUE)

/** @brief Extract validate width field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEWIDTH_GET(VALUE) Q_REG_PWMXL_WAVEFORM_PARAM_GET(_VALIDATE_WIDTH_, VALUE)

/** @brief Extract validate param field value from 16-bit waveform parameter register. */
#define Q_REG_PWMXL_WAVEFORM_PARAM_VALIDATEDITHER_GET(VALUE) Q_REG_PWMXL_WAVEFORM_PARAM_GET(_VALIDATE_DITHER_, VALUE)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @brief PWMXL peripheral register base address. */
typedef UInt32 qRegPWMXL_t;

/** @brief Get the interrupt number for a given PWMXL instance. */
#define Q_REG_PWMXL_IRQN_GET(ID) (IRQn_Type)(Q_CONCAT(PWMXL_IRQn))

/** @brief Get the PWMXL peripheral base address by instance ID. */
#define Q_REG_PWMXL_BASEADDR_GET(ID) Q_CONCAT(GP_WB_PWMXLS_, ID, _BASE_ADDRESS)

/** @brief Get the PWMXL channel base address. */
#define Q_REG_PWMXL_CHANNEL_BASEADDR_GET(BASEADDR, CHANNEL)                                                            \
    (BASEADDR + (GP_WB_PWMXLS_PWMXL_1_NEXT_PHASE_ADDRESS - GP_WB_PWMXLS_PWMXL_0_NEXT_PHASE_ADDRESS) * (CHANNEL))

/** @brief Get the PWMXL action slot offset. */
#define Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET                                                                            \
    (GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_1_ADDRESS - GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_ADDRESS)

/** @enum qRegPWMXL_ValueType_t */
//@{
#define qRegPWMXL_ValueTypePhase   GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_PHASE
#define qRegPWMXL_ValueTypeWidth   GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_WIDTH
#define qRegPWMXL_ValueTypeDither  GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_DITHER
#define qRegPWMXL_ValueTypeTopWrap GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_TOP_WRAP
/** @typedef qRegPWMXL_ValueType_t
 *  @brief PWMXL waveform generator value type.
 */
typedef UInt8 qRegPWMXL_ValueType_t;
//@}

/** @enum qRegPWMXL_TriggerMode_t */
//@{
#define qRegPWMXL_TriggerModePhaseMatch         GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_PHASE_MATCH
#define qRegPWMXL_TriggerModeExternal           GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_EXTERNAL
#define qRegPWMXL_TriggerModeExternalAfterPhase GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_EXTERNAL_AFTER_PHASE_MATCH
/** @typedef qRegPWMXL_TriggerMode_t
 *  @brief PWMXL trigger mode.
 */
typedef UInt8 qRegPWMXL_TriggerMode_t;
//@}

/** @enum qRegPWMXL_CountMode_t */
//@{
#define qRegPWMXL_CountModePrescaled GP_WB_ENUM_PWMXL_COUNT_MODE_PRESCALED
#define qRegPWMXL_CountModeExternal  GP_WB_ENUM_PWMXL_COUNT_MODE_EXTERNAL
/** @typedef qRegPWMXL_CountMode_t
 *  @brief PWMXL main count mode.
 */
typedef UInt8 qRegPWMXL_CountMode_t;
//@}

/** @enum qRegPWMXL_QuicActionType_t */
//@{
#define qRegPWMXL_QuicActionTypeNone               GP_WB_ENUM_PWMXL_ACTION_NONE
#define qRegPWMXL_QuicActionTypeIncrementMainCount GP_WB_ENUM_PWMXL_ACTION_INCREMENT_MAIN_COUNT
#define qRegPWMXL_QuicActionTypeStartWidthCount    GP_WB_ENUM_PWMXL_ACTION_START_WIDTH_COUNT
#define qRegPWMXL_QuicActionTypeLatchPhase         GP_WB_ENUM_PWMXL_ACTION_LATCH_PHASE
#define qRegPWMXL_QuicActionTypeIncrementWidth     GP_WB_ENUM_PWMXL_ACTION_INCREMENT_WIDTH
#define qRegPWMXL_QuicActionTypeDecrementWidth     GP_WB_ENUM_PWMXL_ACTION_DECREMENT_WIDTH
/** @typedef qRegPWMXL_QuicActionType_t
 *  @brief PWMXL QUIC action type.
 */
typedef UInt8 qRegPWMXL_QuicActionType_t;
//@}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

/** @brief Enable or disable the PWMXL peripheral block.
 *
 *  @param[in] enable           New enable state.
 */
static inline void qRegPWMXL_EnableSet(qRegPWMXL_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_ENABLE(enable);
}

/** @brief Get the peripheral enable state.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current enable state.
 */
static inline Bool qRegPWMXL_EnableGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_PWMXLS_ENABLE();
}

/** @brief Set main counter prescaler divider.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] prescalerDiv     Prescaler divider value.
 */
static inline void qRegPWMXL_PrescalerSet(qRegPWMXL_t baseAddr, UInt8 prescalerDiv)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_PRESCALER_DIV(prescalerDiv);
}

/** @brief Get main counter prescaler divider.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current prescaler divider value.
 */
static inline UInt8 qRegPWMXL_PrescalerGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_PWMXLS_PRESCALER_DIV();
}

/** @brief Set main counter mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] mode             Mode of main counter operation.
 */
static inline void qRegPWMXL_CountModeSet(qRegPWMXL_t baseAddr, qRegPWMXL_CountMode_t mode)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_COUNT_MODE(mode);
}

/** @brief Get main counter mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current main counter mode.
 */
static inline qRegPWMXL_CountMode_t qRegPWMXL_CountModeGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return (qRegPWMXL_CountMode_t)GP_WB_READ_PWMXLS_COUNT_MODE();
}

/** @brief Set the flag to controlling phase matching behavior in EXTERNAL_AFTER_PHASE_MATCH mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] enable           New enable state.
 */
static inline void qRegPWMXL_ClrPhaseMatchOnTopWrapSet(qRegPWMXL_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_CLR_PHASE_ON_ARMING_ON_TOP_WRAP(enable);
}

/** @brief Get the Set flag to controlling phase matching behavior in EXTERNAL_AFTER_PHASE_MATCH mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current flag enable state
 */
static inline Bool qRegPWMXL_ClrPhaseMatchOnTopWrapGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return (Bool)GP_WB_READ_PWMXLS_CLR_PHASE_ON_ARMING_ON_TOP_WRAP();
}

/** @brief Set the next top wrap value.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] topWrap          Next top wrap value.
 */
static inline void qRegPWMXL_NextTopWrapSet(qRegPWMXL_t baseAddr, UInt16 topWrap)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_NEXT_TOP_WRAP(topWrap);
}

/** @brief Set the active top wrap value.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] topWrap          Active top wrap value.
 */
static inline void qRegPWMXL_ActiveTopWrapSet(qRegPWMXL_t baseAddr, UInt16 topWrap)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_ACTIVE_TOP_WRAP(topWrap);
}

/** @brief Get the active top wrap value.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current top wrap value.
 */
static inline UInt16 qRegPWMXL_ActiveTopWrapGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_PWMXLS_ACTIVE_TOP_WRAP();
}

/** @brief Write directly the VALUE field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] value            New value.
 */
static inline void qRegPWMXL_ValueSet(qRegPWMXL_t baseAddr, UInt16 value)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_VALUE(value);
}

/** @brief Get the address pipeline register for DMA transfers.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Register address.
 */
static inline UInt32 qRegPWMXL_ValueAddrGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_PWMXLS_PIPELINED_CONFIG_ADDRESS;
}

/** @brief Write directly the VALUE_TYPE field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] valueType        Value type.
 */
static inline void qRegPWMXL_ValueTypeSet(qRegPWMXL_t baseAddr, qRegPWMXL_ValueType_t valueType)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_VALUE_TYPE(valueType);
}

/** @brief Write directly the IDX field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] idx              Index value.
 */
static inline void qRegPWMXL_IDXSet(qRegPWMXL_t baseAddr, UInt8 idx)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_PWMXL_IDX(idx);
}

/** @brief Write directly the VALIDATE_PHASE field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] enable           Enable phase validation.
 */
static inline void qRegPWMXL_ValidatePhaseSet(qRegPWMXL_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_VALIDATE_PHASE(enable);
}

/** @brief Write directly the VALIDATE_WIDTH field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] enable           Enable width validation.
 */
static inline void qRegPWMXL_ValidateWidthSet(qRegPWMXL_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_VALIDATE_WIDTH(enable);
}

/** @brief Write directly the VALIDATE_DITHER field of pipeline register.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] enable           Enable dither validation.
 */
static inline void qRegPWMXL_ValidateDitherSet(qRegPWMXL_t baseAddr, Bool enable)
{
    (void)baseAddr;
    GP_WB_WRITE_PWMXLS_VALIDATE_DITHER(enable);
}

/** @brief Get unmasked top overflow interrupt state.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current interrupt state.
 */
static inline Bool qRegPWMXL_UnmaskedIRQTopOverflowGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_PWMXLS_UNMASKED_TOP_OVERFLOW_INTERRUPT();
}

/** @brief Get unmasked next buffer not full nterrupt state.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *
 *  @retval Current interrupt state.
 */
static inline Bool qRegPWMXL_UnmaskedIRQBufferNotFullGet(qRegPWMXL_t baseAddr)
{
    (void)baseAddr;
    return GP_WB_READ_PWMXLS_UNMASKED_NEXT_BUFFER_NOT_FULL_INTERRUPT();
}

/** @brief Set the next phase value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            Next value.
 */
static inline void qRegPWMXL_ChannelNextPhaseSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_NEXT_PHASE(channelAddr, value);
}

/** @brief Get the address of next phase field register for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Register address.
 */
static inline UInt32 qRegPWMXL_ChannelNextPhaseAddrGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    /* Field offset is aligned to 32 bits, hence the addition of bit offset divided by 8 bits. */
    UInt32 regAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel) + GP_WB_PWMXL_NEXT_PHASE_ADDRESS +
                     GP_WB_PWMXL_NEXT_PHASE_LSB / 8;

    return regAddr;
}

/** @brief Set the next width value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            Next value.
 */
static inline void qRegPWMXL_ChannelNextWidthSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_NEXT_WIDTH(channelAddr, value);
}

/** @brief Get the address of next width field register for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Register address.
 */
static inline UInt32 qRegPWMXL_ChannelNextWidthAddrGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    /* Field offset is aligned to 32 bits, hence the addition of bit offset divided by 8 bits. */
    UInt32 regAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel) + GP_WB_PWMXL_NEXT_WIDTH_ADDRESS +
                     GP_WB_PWMXL_NEXT_WIDTH_LSB / 8;

    return regAddr;
}

/** @brief Set the next dither value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            Next value.
 */
static inline void qRegPWMXL_ChannelNextDitherSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_NEXT_DITHER(channelAddr, value);
}

/** @brief Get the address of next dither field register for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Register address.
 */
static inline UInt32 qRegPWMXL_ChannelNextDitherAddrGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    /* Field offset is aligned to 32 bits, hence the addition of bit offset divided by 8 bits. */
    UInt32 regAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel) + GP_WB_PWMXL_NEXT_DITHER_ADDRESS +
                     GP_WB_PWMXL_NEXT_DITHER_LSB / 8;

    return regAddr;
}

/** @brief Set the active phase value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            New value.
 */
static inline void qRegPWMXL_ChannelActivePhaseSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_ACTIVE_PHASE(channelAddr, value);
}

/** @brief Get the active phase value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current value.
 */
static inline UInt16 qRegPWMXL_ChannelActivePhaseGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_ACTIVE_PHASE(channelAddr);
}

/** @brief Set the active width value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            New value.
 */
static inline void qRegPWMXL_ChannelActiveWidthSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_ACTIVE_WIDTH(channelAddr, value);
}

/** @brief Get the active width value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current value.
 */
static inline UInt16 qRegPWMXL_ChannelActiveWidthGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_ACTIVE_WIDTH(channelAddr);
}

/** @brief Set the active dither value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] value            New value.
 */
static inline void qRegPWMXL_ChannelActiveDitherSet(qRegPWMXL_t baseAddr, UInt8 channel, UInt16 value)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_ACTIVE_DITHER(channelAddr, value);
}

/** @brief Get the active dither value for a given PWM channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current value.
 */
static inline UInt16 qRegPWMXL_ChannelActiveDitherGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_ACTIVE_DITHER(channelAddr);
}

/** @brief Set the channel trigger mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] mode             Channel trigger mode.
 */
static inline void qRegPWMXL_ChannelTriggerModeSet(qRegPWMXL_t baseAddr, UInt8 channel, qRegPWMXL_TriggerMode_t mode)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_PHASE_TRIGGER_MODE(channelAddr, mode);
}

/** @brief Get the channel trigger mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current trigger mode.
 */
static inline qRegPWMXL_TriggerMode_t qRegPWMXL_ChannelTriggerModeGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return (qRegPWMXL_TriggerMode_t)GP_WB_READ_PWMXL_PHASE_TRIGGER_MODE(channelAddr);
}

/** @brief Enable mapping of channel next phase empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] enable           Enable mapping.
 */
static inline void qRegPWMXL_ChannelMapPhaseEmptySet(qRegPWMXL_t baseAddr, UInt8 channel, Bool enable)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_MAP_PHASE_EMPTY_TO_BUFFER_NOT_FULL(channelAddr, enable);
}

/** @brief Get the state of mapping channel next phase empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current mapping enable state.
 */
static inline Bool qRegPWMXL_ChannelMapPhaseEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return (Bool)GP_WB_READ_PWMXL_MAP_PHASE_EMPTY_TO_BUFFER_NOT_FULL(channelAddr);
}

/** @brief Enable mapping of channel next width empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] enable           Enable mapping.
 */
static inline void qRegPWMXL_ChannelMapWidthEmptySet(qRegPWMXL_t baseAddr, UInt8 channel, Bool enable)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_MAP_WIDTH_EMPTY_TO_BUFFER_NOT_FULL(channelAddr, enable);
}

/** @brief Get the state of mapping channel next width empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current mapping enable state.
 */
static inline Bool qRegPWMXL_ChannelMapWidthEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return (Bool)GP_WB_READ_PWMXL_MAP_WIDTH_EMPTY_TO_BUFFER_NOT_FULL(channelAddr);
}

/** @brief Enable mapping of channel next dither empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] enable           Enable mapping.
 */
static inline void qRegPWMXL_ChannelMapDitherEmptySet(qRegPWMXL_t baseAddr, UInt8 channel, Bool enable)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_MAP_DITHER_EMPTY_TO_BUFFER_NOT_FULL(channelAddr, enable);
}

/** @brief Get the state of mapping channel next dither empty flag
 *         to peripheral buffer not empty event.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current mapping enable state.
 */
static inline Bool qRegPWMXL_ChannelMapDitherEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return (Bool)GP_WB_READ_PWMXL_MAP_DITHER_EMPTY_TO_BUFFER_NOT_FULL(channelAddr);
}

/** @brief Set the channel output drive.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] drive            Output drive mode.
 */
static inline void qRegPWMXL_ChannelOutputDriveSet(qRegPWMXL_t baseAddr, UInt8 channel, qRegCommon_Drive_t drive)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_OUTPUT_DRIVE(channelAddr, drive);
}

/** @brief Get the channel output drive mode.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current output drive mode.
 */
static inline qRegCommon_Drive_t qRegPWMXL_ChannelOutputDriveGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return (qRegCommon_Drive_t)GP_WB_READ_PWMXL_OUTPUT_DRIVE(channelAddr);
}

/** @brief Enable output inversion.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *  @param[in] enable           Enable inversion.
 */
static inline void qRegPWMXL_ChannelInvertSet(qRegPWMXL_t baseAddr, UInt8 channel, Bool enable)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_WRITE_PWMXL_OUTPUT_INVERT(channelAddr, enable);
}

/** @brief Get the channel output inversion enable state.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current enable state.
 */
static inline Bool qRegPWMXL_ChannelInvertGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_OUTPUT_INVERT(channelAddr);
}

/** @brief Get the channel phase empty flag.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current flag state.
 */
static inline Bool qRegPWMXL_ChannelPhaseEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_PHASE_EMPTY(channelAddr);
}

/** @brief Get the channel width empty flag.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current flag state.
 */
static inline Bool qRegPWMXL_ChannelWidthEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_WIDTH_EMPTY(channelAddr);
}

/** @brief Get the channel dither empty flag.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current flag state.
 */
static inline Bool qRegPWMXL_ChannelDitherEmptyGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_DITHER_EMPTY(channelAddr);
}

/** @brief Get the unmasked PHASE_MATCH interrupt for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current interrupt state.
 */
static inline Bool qRegPWMXL_UnmaskedChannelIRQPhaseMatchGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_UNMASKED_PHASE_MATCH_INTERRUPT(channelAddr);
}

/** @brief Get the unmasked PHASE_ON interrupt for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current interrupt state.
 */
static inline Bool qRegPWMXL_UnmaskedChannelIRQPhaseOnGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_UNMASKED_PHASE_ON_INTERRUPT(channelAddr);
}

/** @brief Get the unmasked PHASE_OFF interrupt for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 *
 *  @retval Current interrupt state.
 */
static inline Bool qRegPWMXL_UnmaskedChannelIRQPhaseOffGet(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    return GP_WB_READ_PWMXL_UNMASKED_PHASE_OFF_INTERRUPT(channelAddr);
}

/** @brief Clear the PHASE_MATCH interrupt flag for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 */
static inline void qRegPWMXL_ChannelIRQPhaseMatchClr(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_PWMXL_CLR_PHASE_MATCH_INTERRUPT(channelAddr);
}

/** @brief Clear the PHASE_ON interrupt flag for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 */
static inline void qRegPWMXL_ChannelIRQPhaseOnClr(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_PWMXL_CLR_PHASE_ON_INTERRUPT(channelAddr);
}

/** @brief Clear the PHASE_OFF interrupt flag for a given channel.
 *
 *  @param[in] baseAddr         Peripheral register base address.
 *  @param[in] channel          PWM channel number.
 */
static inline void qRegPWMXL_ChannelIRQPhaseOffClr(qRegPWMXL_t baseAddr, UInt8 channel)
{
    UInt32 channelAddr = Q_REG_PWMXL_CHANNEL_BASEADDR_GET(baseAddr, channel);
    GP_WB_PWMXL_CLR_PHASE_OFF_INTERRUPT(channelAddr);
}

/** @brief Set QUIC channel for specified action index.
 *
 *  @param[in] baseAddr  Peripheral register base address.
 *  @param[in] actionIdx Action index to be used.
 *  @param[in] channel   QUIC channel.
 */
static inline void qRegPWMXL_QUICActionChannelSet(qRegPWMXL_t baseAddr, UInt8 actionIdx, UInt32 channel)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_MASK,
                     (channel << GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_LSB));
}

/** @brief Get QUIC channel from specified action index.
 *
 *  @param[in] baseAddr  Peripheral register base address.
 *  @param[in] actionIdx Action index to be used.
 */
static inline UInt32 qRegPWMXL_QUICActionChannelGet(qRegPWMXL_t baseAddr, UInt8 actionIdx)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    return (GP_WB_READ_U32(address) & GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_MASK) >>
           GP_WB_PWMXLS_QUIC_ACTION_CHANNEL_0_LSB;
}

/** @brief Set QUIC PWMXL channel for specified action index.
 *
 *  @param[in] baseAddr     Peripheral register base address.
 *  @param[in] actionIdx    Action index to be used.
 *  @param[in] pwmxlChannel PWMXL channel.
 */
static inline void qRegPWMXL_QUICActionPwmxlChannelSet(qRegPWMXL_t baseAddr, UInt8 actionIdx, UInt32 pwmxlChannel)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_MASK,
                     (pwmxlChannel << GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_LSB));
}

/** @brief Get QUIC PWMXL channel from specified action index.
 *
 *  @param[in] baseAddr     Peripheral register base address.
 *  @param[in] actionIdx    Action index to be used.
 *  @param[in] pwmxlChannel PWMXL channel.
 */
static inline UInt32 qRegPWMXL_QUICActionPwmxlChannelGet(qRegPWMXL_t baseAddr, UInt8 actionIdx)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    return (GP_WB_READ_U32(address) & GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_MASK) >> GP_WB_PWMXLS_QUIC_ACTION_PWMXL_0_LSB;
}

/** @brief Set QUIC action type for specified action index.
 *
 *  @param[in] baseAddr       Peripheral register base address.
 *  @param[in] actionIdx      Action index to be used.
 *  @param[in] quicActionType PWMXL QUIC action type.
 */
static inline void qRegPWMXL_QUICActionTypeSet(qRegPWMXL_t baseAddr, UInt8 actionIdx,
                                               qRegPWMXL_QuicActionType_t quicActionType)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    GP_WB_MWRITE_U32(address, GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_MASK,
                     (quicActionType << GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_LSB));
}

/** @brief Get QUIC action type from specified action index.
 *
 *  @param[in] baseAddr  Peripheral register base address.
 *  @param[in] actionIdx Action index to be used.
 */
static inline qRegPWMXL_QuicActionType_t qRegPWMXL_QUICActionTypeGet(qRegPWMXL_t baseAddr, UInt8 actionIdx)
{
    (void)baseAddr;
    UInt32 address = GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_ADDRESS + (Q_REG_PWMXL_QUIC_ACTION_SLOT_OFFSET * actionIdx);

    return (qRegPWMXL_QuicActionType_t)((GP_WB_READ_U32(address) & GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_MASK) >>
                                        GP_WB_PWMXLS_QUIC_ACTION_TYPE_0_LSB);
}

#ifdef __cplusplus
}
#endif

#endif /* #define _Q_REG_PWMXL_H_ */
