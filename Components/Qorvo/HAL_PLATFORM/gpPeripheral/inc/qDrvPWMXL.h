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

#ifndef _Q_DRV_PWMXL_H_
#define _Q_DRV_PWMXL_H_

/**
 * @defgroup Q_DRV_PWMXL_H Pulse modulation width XL driver
 * @{
 *
 * @brief This is a driver for PWMXL.
 *
 * @file qDrvPWMXL.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegPWMXL.h"
#include "qDrvCommon.h"
#include "qDrvIOB.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/** @enum qDrvPWMXL_CountMode_t */
//@{
/** @brief Internal clock with prescaler */
#define qDrvPWMXL_CountModePrescaled qRegPWMXL_CountModePrescaled
/** @brief External trigger */
#define qDrvPWMXL_CountModeExternal qRegPWMXL_CountModeExternal
/** @typedef qDrvPWMXL_CountMode_t
 *  @brief PWMXL count mode chooses between prescaled mode or count mode
 *         for the main counter clock. Prescaled mode uses the output
 *         of the prescaler, count mode uses an external trigger.
 */
typedef UInt8 qDrvPWMXL_CountMode_t;
//@}

/** @enum qDrvPWMXL_TriggerMode_t */
//@{
/** @brief Output is set after phase match. */
#define qDrvPWMXL_TriggerModePhaseMatch qRegPWMXL_TriggerModePhaseMatch
/** @brief Channel output (for a time defined by duty cycle)
 *         is triggered after external QUIC event.
 */
#define qDrvPWMXL_TriggerModeExternal qRegPWMXL_TriggerModeExternal
/** @brief Same as @p qDrvPWMXL_TriggerModePhaseMatch, but the output
 *         is set only in a window after phase match (main counter >= channel phase value).
 */
#define qDrvPWMXL_TriggerModeExternalAfterPhase qRegPWMXL_TriggerModeExternalAfterPhase
/** @typedef qDrvPWMXL_EntryValueType_t
 *  @brief PWMXL channel trigger mode.
 */
typedef UInt8 qDrvPWMXL_TriggerMode_t;
//@}

/** @enum qDrvPWMXL_EntryValueType_t */
//@{
/** @brief Set channel phase value. */
#define qDrvPWMXL_EntryValueTypePhase qRegPWMXL_ValueTypePhase
/** @brief Set channel width value. */
#define qDrvPWMXL_EntryValueTypeWidth qRegPWMXL_ValueTypeWidth
/** @brief Set channel dither value. */
#define qDrvPWMXL_EntryValueTypeDither qRegPWMXL_ValueTypeDither
/** @brief Set main counter period. */
#define qDrvPWMXL_EntryValueTypeTopWrap qRegPWMXL_ValueTypeTopWrap
/** @typedef qDrvPWMXL_EntryValueType_t
 *  @brief Value type for 32-bit PWMXL DMA entry.
 */
typedef UInt8 qDrvPWMXL_EntryValueType_t;
//@}

/** @enum qDrvPWMXL_DmaMode_t */
//@{
/** @brief Buffer contains 16-bit values. */
#define qDrvPWMXL_DmaMode16bit 0
/** @brief Buffer contains 32-bit entries ( @p qDrvPWMXL_WaveformEntry_t ). */
#define qDrvPWMXL_DmaMode32bitWaveform 1
/** @typedef qDrvPWMXL_DmaMode_t
 *  @brief Waveform mode. 16-bit values or 32-bit entries with descriptor.
 */
typedef UInt8 qDrvPWMXL_DmaMode_t;
//@}

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvPWMXL_QuicSrcTrigger_t */
//@{
#define qDrvPWMXL_QuicSrcTriggerPhaseMatch  0
#define qDrvPWMXL_QuicSrcTriggerPhaseOff    1
#define qDrvPWMXL_QuicSrcTriggerPhaseOn     2
#define qDrvPWMXL_QuicSrcTriggerTopOverflow 3
/** @typedef qDrvPWMXL_QuicSrcTrigger_t
 *  @brief PWMXL Quic source trigger.
 */
typedef UInt8 qDrvPWMXL_QuicSrcTrigger_t;
//@}

/** @enum qDrvPWMXL_QuicActionType_t */
//@{
#define qDrvPWMXL_QuicActionTypeNone               qRegPWMXL_QuicActionTypeNone
#define qDrvPWMXL_QuicActionTypeIncrementMainCount qRegPWMXL_QuicActionTypeIncrementMainCount
#define qDrvPWMXL_QuicActionTypeStartWidthCount    qRegPWMXL_QuicActionTypeStartWidthCount
#define qDrvPWMXL_QuicActionTypeLatchPhase         qRegPWMXL_QuicActionTypeLatchPhase
#define qDrvPWMXL_QuicActionTypeIncrementWidth     qRegPWMXL_QuicActionTypeIncrementWidth
#define qDrvPWMXL_QuicActionTypeDecrementWidth     qRegPWMXL_QuicActionTypeDecrementWidth
/** @typedef qDrvPWMXL_QuicActionType_t
 *  @brief PWMXL QUIC action type.
 */
typedef UInt8 qDrvPWMXL_QuicActionType_t;
//@}

/** @brief PWMXL QUIC trigger configuration structure. */
typedef struct {
    qDrvPWMXL_QuicSrcTrigger_t srcTrigger; /**< Required source trigger. */
    UInt8 pwmChannel; /**< QUIC PWMXL channel. Skipped if source trigger is set to the top overflow. */
    Q_DRV_QUIC_INTERNAL_TRIGGER_FIELD(); /**< Required trigger field. */
} qDrvPWMXL_QuicTriggerConfig_t;

/** @brief PWMXL QUIC action configuration structure. */
typedef struct {
    UInt8 actionIdx;                       /**< QUIC action index. */
    UInt8 pwmChannel;                      /**< QUIC PWMXL channel. */
    qDrvPWMXL_QuicActionType_t actionType; /**< QUIC action type. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();    /**< Required action field. */
} qDrvPWMXL_QuicActionConfig_t;
#endif                                     /* REG_QUIC_PRESENT */

/** @brief Structure containing one entry used to generate advanced arbitrary waveform.
 *         For more information how to use it, please refer to the chip databook,
 *         chapter 13.8.2 (PWMXL, Advanced features).
 */
typedef struct __attribute__((packed, aligned(4))) {
    UInt16 value;  /**< 16-bit value which will be copied into target defined by value type field. */
    UInt16 params; /**< 16-bit fields with entry parameters.
                        Can be filled with @p Q_DRV_PWMXL_WAVEFORM_PARAMS_PACK macro
                        or @p qDrvPWMXL_WaveformParamsPack function. */
} qDrvPWMXL_WaveformEntry_t;

typedef struct {
    qDrvPWMXL_EntryValueType_t valueType; /**< Value type. */
    UInt8 idx;                            /**< Defines what PWM channel the value is for.
                                               Only applicable to the phase / width or dither values */
    Bool validatePhase;                   /**< When set, the NEXT_PHASE fifo will be validated.
                                               This means that it will be marked as filled.
                                               When not validated, the FIFO can already hold
                                               the new value set by PWMXLS_VALUE, but the status
                                               of the FIFO will still be seen as empty. */
    Bool validateWidth;                   /**< When set, the NEXT_WIDTH fifo will be validated.
                                               This means that it will be marked as filled.
                                               When not validated, the FIFO can already hold
                                               the new value set by PWMXLS_VALUE, but the status
                                               of the FIFO will still be seen as empty. */
    Bool validateDither;                  /**< When set, the NEXT_DITHER fifo will be validated.
                                               This means that it will be marked as filled.
                                               When not validated, the FIFO can already hold
                                               the new value set by PWMXLS_VALUE, but the status
                                               of the FIFO will still be seen as empty. */
} qDrvPWMXL_WaveformEntryParams_t;

/** @brief Structure used to map PWMXL_MAP_x_EMPTY event (where x is PHASE, WIDTH or DITHER)
 *         of a specific channel to NEXT_BUFFER_NOT_FULL interrupt.
 *
 *         This is needed for DMA triggering.
 */
typedef struct {
    UInt8 next_phase : 1;  /**< Channel NOT_FULL event trigger when
                                the next phase value FIFO is not full. */
    UInt8 next_width : 1;  /**< Channel NOT_FULL event trigger when
                                the next width value FIFO is not full. */
    UInt8 next_dither : 1; /**< Channel NOT_FULL event trigger when
                                the next dither value FIFO is not full. */
    UInt8 channel : 5;     /**< Channel number. */
} qDrvPWMXL_BufferMap_t;

/** @brief PWMXL instance config structure. */
typedef struct {
    qDrvPWMXL_CountMode_t countMode; /**< Clock input selection. */
    UInt8 prescaler;                 /**< Main timer clock prescaler_divider. Clock frequency is equal
                                          to SYS_CLK / (2 ^ prescaler), where 0 <= prescaler <= 7. */
    UInt16 periodTicks;              /**< Main counter overflow period, equal to (period+1) cycles. */
} qDrvPWMXL_Config_t;

/** @brief PWMXL internal control block placeholder. */
typedef struct {
    UInt8 data[24 + REG_PWMXL_CHANNEL_COUNT * 4];
} __attribute__((aligned(4))) qDrvPWMXL_Internal_t;

/** @brief PWMXL instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi;  /**< Quic API pointer implementation. */
#endif                             /* REG_QUIC_PRESENT */
    qDrvPWMXL_Internal_t internal; /**< Internal data buffer. Should not be modified by the user. */
    const qRegPWMXL_t baseAddr;    /**< Peripheral instance base address. */
    const UInt8 id;                /**< Peripheral instance ID. */
} qDrvPWMXL_t;

/** @brief PWMXL pin definition structure. */
typedef struct {
    qDrvIOB_PinAlt_t pwm; /**< PWM output pin/alternate pair. */
} qDrvPWMXL_PinConfig_t;

/** @brief Callback invoked to fetch more samples during waveform playback (DMA transfer).
 *
 *  This function is required if the application use waveform generation feature.
 *
 *  @param[in] pInstance  PWMXL instance pointer.
 *  @param[in] bufferDone Pointer to a buffer chunk that has been processed.
 *                        This memory area can be re-filled with new samples.
 *  @param[in] length     Number of bytes processed.
 *  @param[in] mode       Current operation mode - 16-bit samples or 32-bit extended entries.
 *                        Can be used to convert buffer length to number of samples.
 *  @param[in] arg        User context variable.
 */
typedef void (*qDrvPWMXL_Cb_t)(const qDrvPWMXL_t* pInstance, void* bufferDone, UInt16 length, qDrvPWMXL_DmaMode_t mode,
                               void* arg);

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/* @brief Macro for packing waveform entry parameters into 16-bit register value. */
#define Q_DRV_PWMXL_WAVEFORM_PARAMS_PACK REG_PWMXL_WAVEFORM_PARAMS_PACK

#define Q_DRV_PWMXL_DEFAULT_DRIVE     qRegCommon_DrivePP
#define Q_DRV_PWMXL_DEFAULT_FREQUENCY 2000UL
#define Q_DRV_PWMXL_CLOCK_FREQUENCY   REG_CHIP_CLK_FREQ

/**
 * @brief Macro for initializing the PWMXL driver instance.
 *
 * @param[in] ID Peripheral instance number.
 */
#define Q_DRV_PWMXL_INSTANCE_DEFINE(ID)                                                                                \
    {                                                                                                                  \
        .internal = {},                               /* Initialize internal data to zeros. */                         \
            .baseAddr = Q_REG_PWMXL_BASEADDR_GET(ID), /* Initialize the base address of the peripheral instance. */    \
            .id = ID,                                 /* Initialize the ID of the peripheral instance. */              \
    }

/** @brief Default configuration macro for PWMXL instance. */
#define Q_DRV_PWMXL_CONFIG_DEFAULT()                                                                                   \
    {                                                                                                                  \
        .periodTicks = (Q_DRV_PWMXL_CLOCK_FREQUENCY / Q_DRV_PWMXL_DEFAULT_FREQUENCY), /* */                            \
            .prescaler = 0,                                                           /* */                            \
            .countMode = qDrvPWMXL_CountModePrescaled                                 /* */                            \
    }

/**
 * @brief Macro for defining instance pin configuration.
 *        The selected pin must have an alternative function available,
 *        witch corresponds to the given PWM channel.
 *
 * @param[in] PWMXL_CHANNEL PWM channel number.
 * @param[in] PIN           Output pin number.
 */
#define Q_DRV_PWMXL_PIN_CONFIG(PWMXL_CHANNEL, PIN)                                                                     \
    {                                                                                                                  \
        .pwm = {.pin = PIN, .alternate = Q_DRV_IOB_ALT_GET(PIN, PWMXL_PWMXL, PWMXL_CHANNEL)}, /* */                    \
    }

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Configure the PWM output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer, which can be filled
 *                       with @p Q_DRV_PWMXL_PIN_CONFIG macro.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin number or pin in use.
 */
qResult_t qDrvPWMXL_PinConfigSet(const qDrvPWMXL_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the PWM output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin number or pin not mapped.
 */
qResult_t qDrvPWMXL_PinConfigClear(const qDrvPWMXL_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize PWMXL instance.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 * @param[in] callback  Event callback function.
 * @param[in] arg       User context variable.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Peripheral is already initialized.
 * @retval Q_ERR_INVPARAM Incorrect configuration.
 */
qResult_t qDrvPWMXL_Init(qDrvPWMXL_t* pInstance, const qDrvPWMXL_Config_t* const pConfig, const qDrvPWMXL_Cb_t callback,
                         void* arg);

/**
 * @brief Deinitialize the PWMXL instance.
 *
 * @param[in] pInstance PWMXL instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvPWMXL_Deinit(const qDrvPWMXL_t* pInstance);

/**
 * @brief Check if the PWMXL instance is initialized.
 *
 * @param[in] pInstance PWMXL instance pointer.
 *
 * @return True if the instance is initialized, false otherwise.
 */
Bool qDrvPWMXL_InitCheck(const qDrvPWMXL_t* pInstance);

/**
 * @brief Set the main counter period.
 *        If the PWM is currently running, the new period value will be applied
 *        in the next cycle. Note that prescaler value is not double-buffered.
 *        If the new prescaler value is different, current cycle length will be altered.
 *
 *
 * @param[in] pInstance   PWMXL instance pointer.
 * @param[in] periodTicks Main clock period equal to (period + 1) input clock cycles.
 * @param[in] prescaler   Main clock prescaler value.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_PeriodSet(const qDrvPWMXL_t* pInstance, UInt16 periodTicks, UInt8 prescaler);

/**
 * @brief Calculate main counter tick count and prescaler value
 *        for a desired PWM frequency.
 *
 * @param[in]  frequency Desired PWM frequency.
 * @param[out] ticks     Calculated tick count.
 * @param[out] prescaler Calculated prescaler divider value.
 *
 * @return True if the frequency can be achieved, false otherwise.
 */
Bool qDrvPWMXL_FrequencyCalculate(UInt32 frequency, UInt16* ticks, UInt8* prescaler);

/**
 * @brief Convert number of main counter ticks to microseconds including current prescaler value.
 *        The peripheral instance must be initialized.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] ticks     Number of ticks to convert.
 */
UInt32 qDrvPWMXL_TicksToUsConvert(const qDrvPWMXL_t* pInstance, UInt16 ticks);

/**
 * @brief Pack waveform parameters into 16-bit parameter register value required
 *        for extended waveform generation mode.
 *
 * @param[in] params Entry parameter structure pointer.
 *
 * @retval 16-bit parameter field value.
 */
UInt16 qDrvPWMXL_WaveformParamsPack(const qDrvPWMXL_WaveformEntryParams_t* params);

/**
 * @brief Extract 16-bit waveform parameter field into a structure.
 *
 * @param[in]  value  Register value.
 * @param[out] params Output parameter structure pointer.
 */
void qDrvPWMXL_WaveformParamsUnpack(UInt16 value, qDrvPWMXL_WaveformEntryParams_t* params);

/**
 * @brief Initialize single PWM channel.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWM channel number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVSTATE      Channel is already initialized.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Incorrect channel number.
 */
qResult_t qDrvPWMXL_ChannelInit(const qDrvPWMXL_t* pInstance, UInt8 channel);

/**
 * @brief Deinitialize single PWM channel.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWM channel number.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Incorrect channel number.
 */
qResult_t qDrvPWMXL_ChannelDeinit(const qDrvPWMXL_t* pInstance, UInt8 channel);

/**
 * @brief Enable or disable main counter and PWM output on all configured channels.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] enable    Enable state.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Peripheral already in requested state.
 */
qResult_t qDrvPWMXL_Enable(const qDrvPWMXL_t* pInstance, Bool enable);

/**
 * @brief Set duty cycle in percent for a specific channel.
 *        For more advanced control, like phase shift, use @p qDrvPWMXL_ChannelDutyExtSet function.
 *
 * @param[in] pInstance   PWMXL instance pointer.
 * @param[in] channel     PWN channel.
 * @param[in] dutyPercent Duty cycle in percents in units of 0,01% (0 - 10000).
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_ChannelDutySet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 dutyPercent);

/**
 * @brief Set duty cycle in main clock ticks for specific channel.
 *        For more advanced control, like phase shift, use @p qDrvPWMXL_ChannelDutyExtSet function.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWN channel.
 * @param[in] ticks     Duty cycle in main clock ticks.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_ChannelDutyRawSet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 ticks);

/**
 * @brief Set duty cycle in main clock ticks with optional phase and dither values.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWN channel.
 * @param[in] ticks     Duty cycle in main clock ticks.
 * @param[in] phase     Phase shift in main clock cycles.
 * @param[in] dither    Dither value. For more information,
 *                      please refer to the chip databook.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_ChannelDutyExtSet(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16 ticks, UInt16 phase,
                                      UInt8 dither);

/**
 * @brief Set output inversion for specific channel.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWN channel.
 * @param[in] invert    Output inversion.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_InvertSet(const qDrvPWMXL_t* pInstance, UInt8 channel, Bool invert);

/**
 * @brief Set output inversion for specific channel.
 *
 * @param[in] pInstance PWMXL instance pointer.
 * @param[in] channel   PWN channel.
 * @param[in] drive     Output drive. Push-pull or open drain.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_DriveSet(const qDrvPWMXL_t* pInstance, UInt8 channel, qRegCommon_Drive_t drive);

/**
 * @brief Set output trigger mode for specific channel.
 *
 * @param[in] pInstance   PWMXL instance pointer.
 * @param[in] channel     PWN channel.
 * @param[in] triggerMode Output trigger mode.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_INVPARAM      Invalid channel number.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvPWMXL_TriggerModeSet(const qDrvPWMXL_t* pInstance, UInt8 channel, qDrvPWMXL_TriggerMode_t triggerMode);

/** @brief Start the 16-bit waveform generation using DMA.
 *  @note  Event callback must be set in order to use this feature. It will be triggered
 *         on underflow and every time after processing 1/2 of the buffer notifying user
 *         that it's ready to be filled with new samples. The waveform generation is stopped
 *         by calling @p qDrvPWMXL_WaveformStop function.
 *
 * @param[in] pInstance   PWMXL instance pointer.
 * @param[in] channel     PWN channel.
 * @param[in] buffer      Sample buffer containing 16-bit values.
 * @param[in] sampleCount Length of the buffer in samples. Must be a multiple of 2.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_BUSY              Waveform mode is already active.
 * @retval Q_ERR_OUTOFMEMORY   DMA channel allocation failed.
 * @retval Q_ERR_INVPARAM      Invalid channel number or buffer parameters.
 */
qResult_t qDrvPWMXL_WaveformStart(const qDrvPWMXL_t* pInstance, UInt8 channel, UInt16* buffer, UInt16 sampleCount);

/** @brief Generate the extended arbitrary waveform using DMA.
 *         Event callback must be set in order to use this feature. It will be triggered
 *         on underflow and every time after processing 1/2 of the buffer notifying user
 *         that it's ready to be filled with new samples. The waveform generation is stopped
 *         by calling @p qDrvPWMXL_WaveformStop function.
 *
 * @param[in] pInstance   PWMXL instance pointer.
 * @param[in] mapping     Maps a specific channel's buffer empty event to
 *                        NEXT_BUFFER_NOT_FULL interrupt in order to trigger/pause DMA action.
 *                        For more information how to use it, please refer to the chip
 *                        databook: Peripherals -> PWMXL -> Advanced Features.
 * @param[in] buffer      Buffer containing DMA 32-bit entries containing descriptor and value.
 * @param[in] sampleCount Length of the buffer in samples. Must be a multiple of 2.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_BUSY              Waveform mode is already active.
 * @retval Q_ERR_OUTOFMEMORY   DMA channel allocation failed.
 * @retval Q_ERR_INVPARAM      Invalid channel number or buffer parameters.
 */
qResult_t qDrvPWMXL_ExtWaveformStart(const qDrvPWMXL_t* pInstance, qDrvPWMXL_BufferMap_t mapping,
                                     qDrvPWMXL_WaveformEntry_t* buffer, UInt16 sampleCount);

/** @brief Stop the waveform generation.
 *
 * @param[in] pInstance PWMXL instance pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVSTATE      Waveform mode is not active.
 */
qResult_t qDrvPWMXL_WaveformStop(const qDrvPWMXL_t* pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_PWMXL_H_
