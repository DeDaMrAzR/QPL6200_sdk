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

#ifndef _Q_DRV_RTZ_H_
#define _Q_DRV_RTZ_H_

/**
 * @defgroup Q_DRV_RTZ_H RTZ driver
 * @{
 *
 * @brief This is a driver for RTZ peripheral.
 *
 * @file qDrvRTZ.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegRTZ.h"
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

/** @enum qDrvRTZ_BitOrder_t */
//@{
/** @brief LSB first. */
#define qDrvRTZ_BitOrderLSBFirst 1
/** @brief MSB first. */
#define qDrvRTZ_BitOrderMSBFirst 0
/** @typedef qDrvRTZ_BitOrder_t
 *  @brief Data bit order within single buffer word.
 *         Setting bit order will affect memory layout depending on chosen word size and alignment configuration.
 */
typedef UInt8 qDrvRTZ_BitOrder_t;
//@}

/** @enum qDrvRTZ_Event_t */
//@{
/** @brief Buffer underrun error. */
#define qDrvRTZ_EventUnderrunError qRegRTZ_EventUnderrunError
/** @brief Sequence transmission complete. */
#define qDrvRTZ_EventComplete qRegRTZ_EventComplete
/** @brief TX buffer not full. */
#define qDrvRTZ_EventTxNotFull qRegRTZ_EventTxNotFull
/** @typedef qDrvRTZ_event_t
 *  @brief RTZ event.
 */
typedef UInt8 qDrvRTZ_Event_t;
//@}

/** @enum qDrvRTZ_TransferMode_t */
//@{
/** @brief Pooling configuration. */
#define qDrvRTZ_TransferModePolling 0
/** @brief Interrupt configuration. */
#define qDrvRTZ_TransferModeInterrupt 1
/** @brief DMA configuration. */
#define qDrvRTZ_TransferModeDMA 2
/** @typedef qDrvRTZ_TransferMode_t
 *  @brief RTZ transmitter mode configuration.
 */
typedef UInt8 qDrvRTZ_TransferMode_t;
//@}

#if defined(REG_QUIC_PRESENT)
/** @enum qDrvRTZ_QuicActionType_t */
//@{
/** @brief Action none. */
#define qDrvRTZ_QuicActionTypeNone qRegRTZ_QuicActionTypeNone
/** @brief Action trigger sequence. */
#define qDrvRTZ_QuicActionTypeTriggerSequence qRegRTZ_QuicActionTypeTriggerSequence
/** @typedef qDrvRTZ_QuicActionType_t
 *  @brief RTZ action.
 */
typedef UInt8 qDrvRTZ_QuicActionType_t;
//@}
#endif /* defined(REG_QUIC_PRESENT) */

/** @brief RTZ symbol timing configuration. */
typedef struct {
    Int32 t0H;     /**< Time in nanoseconds of the high state for a ZERO symbol. */
    Int32 t0L;     /**< Time in nanoseconds of the low state for a ZERO symbol. */
    Int32 t1H;     /**< Time in nanoseconds of the high state for a ONE symbol. */
    Int32 t1L;     /**< Time in nanoseconds of the low state for a ONE symbol. */
    Int32 tStopH;  /**< Time in nanoseconds of the high state for a STOP symbol. */
    Int32 tStopL;  /**< Time in nanoseconds of the low state for a STOP symbol. */
    Int32 tResetH; /**< Time in nanoseconds of the high state for a RESET symbol. */
    Int32 tResetL; /**< Time in nanoseconds of the low state for a RESET symbol. */
} qDrvRTZ_TimingsConfig_t;

/** @brief RTZ timings configuration unused value. */
#define Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED (-1)

/** @brief Default config timings for WS2811 controller. */
#define DRV_RTZ_TIMINGS_CONFIG_DEFAULT_WS2811()                                                                        \
    {                                                                                                                  \
        .t0H = 500,                                     /* 0.5 us */                                                   \
            .t0L = 2000,                                /* 2.0 us */                                                   \
            .t1H = 1200,                                /* 1.2 us */                                                   \
            .t1L = 1300,                                /* 1.3 us */                                                   \
            .tStopH = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED,  /* Not used. */                                                \
            .tStopL = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED,  /* Not used. */                                                \
            .tResetH = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED, /* Not used. */                                                \
            .tResetL = 55000,                           /* Above 50 us. */                                             \
    }

/** @brief Default config timings for WS2812B controller. */
#define DRV_RTZ_TIMINGS_CONFIG_DEFAULT_WS2812B()                                                                       \
    {                                                                                                                  \
        .t0H = 400,                                     /* 0.4 us */                                                   \
            .t0L = 850,                                 /* 0.85 us */                                                  \
            .t1H = 800,                                 /* 0.8 us */                                                   \
            .t1L = 450,                                 /* 0.45 us */                                                  \
            .tStopH = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED,  /* Not used. */                                                \
            .tStopL = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED,  /* Not used. */                                                \
            .tResetH = Q_DRV_RTZ_TIMINGS_CONFIG_UNUSED, /* Not used. */                                                \
            .tResetL = 55000,                           /* Above 50 us. */                                             \
    }

/** @brief RTZ data frame configuration. */
typedef struct {
    UInt32 pixelsCount;          /**< Number of pixel. */
    UInt8 dataBits;              /**< Number of data bits per pixel. */
    UInt8 trailingDataBits;      /**< Number of trailing data bits. */
    qDrvRTZ_BitOrder_t bitOrder; /**< Bit order within one buffer word. */
    qRegRTZ_WordSize_t wordSize; /**< Buffer word size. */
    Bool wordAlign; /**< Align of the single pixel data to the buffer word. If number of data bits is not equal to the
                       buffer word size, the application must account for extra padding bits. */
} qDrvRTZ_FrameConfig_t;

/** @brief RTZ output inversion configuration. */
typedef struct {
    Bool zero;     /**< Invert ZERO symbol. */
    Bool one;      /**< Invert ONE symbol. */
    Bool stop;     /**< Invert STOP symbol. */
    Bool reset;    /**< Invert RESET symbol. */
    Bool physical; /**< Polarity of the output will be inverted. This is an overall inversion, so applies to all
                      symbols/states. */
} qDrvRTZ_InvertConfig_t;

/** @brief Default disabled invert config for all symbols. */
#define DRV_RTZ_INVERT_CONFIG_DEFAULT_FALSE()                                                                          \
    {                                                                                                                  \
        .zero = false,         /* Symbol ZERO not inverted. */                                                         \
            .one = false,      /* Symbol ONE not inverted. */                                                          \
            .stop = false,     /* Symbol STOP not inverted. */                                                         \
            .reset = false,    /* RESETnot inverted. */                                                                \
            .physical = false, /* Polarity of the output is not inverted. */                                           \
    }

/** @brief RTZ jitter configuration. */
typedef struct {
    UInt8 magnitudeBits; /**< Defines max jitter amplitude calculated as 2**(magnitudeBits+1)-1. */
    Bool tLow;           /**< True to enable the jitter on T0L and T1L duration, false otherwise. */
    Bool tHigh;          /**< True to enable the jitter on T0H and T1H duration, false otherwise. */
} qDrvRTZ_JitterConfig_t;

/** @brief Default disabled jitter config. */
#define DRV_RTZ_JITTER_CONFIG_DEFAULT_FALSE()                                                                          \
    {                                                                                                                  \
        .magnitudeBits = 0, /* Disable jitter bits. */                                                                 \
            .tLow = false,  /* Disable the jitter on T0L and T1L duration. */                                          \
            .tHigh = false, /* Disable the jitter on T0H and T1H duration. */                                          \
    }

/** @brief RTZ instance config structure. */
typedef struct {
    qDrvRTZ_TimingsConfig_t timings;     /**< Symbol timings. */
    qDrvRTZ_FrameConfig_t frame;         /**< Frame size and layout configuration. */
    qDrvRTZ_InvertConfig_t invert;       /**< Output inversion configuration. */
    qDrvRTZ_JitterConfig_t jitter;       /**< Jitter configuration. */
    qDrvRTZ_TransferMode_t transferMode; /**< Transfer mode. */
} qDrvRTZ_Config_t;

#if defined(REG_QUIC_PRESENT)
/** @brief RTZ QUIC action configuration structure. */
typedef struct {
    qDrvRTZ_QuicActionType_t actionType; /**< QUIC action to be applied. */
    Q_DRV_QUIC_INTERNAL_ACTION_FIELD();  /**< Required action field. */
} qDrvRTZ_QuicActionConfig_t;
#endif                                   /* REG_QUIC_PRESENT */

/** @brief RTZ internal internal control block placeholder. */
typedef struct {
    UInt32 buffer[5];
} qDrvRTZ_Internal_t;

/** @brief RTZ instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< Quic API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
    qDrvRTZ_Internal_t internal;  /**< Internal data buffer. Should not be modified by the user. */
    const qRegRTZ_t baseAddr;     /**< Peripheral instance base address. */
    const UInt8 id;               /**< Peripheral instance ID. */
} qDrvRTZ_t;

/** @brief RTZ pin configuration structure. */
typedef struct {
    qDrvIOB_PinAlt_t tx; /**< RTZ output pin/alternate pair. */
} qDrvRTZ_PinConfig_t;

/**
 * @brief RTZ status callback. Called on sequence completion or an error.
 *
 * @param[in] arg   User supplied argument.
 * @param[in] event Event type
 */
typedef void (*qDrvRTZ_Cb_t)(void* arg, qDrvRTZ_Event_t event);

/**
 * @brief Macro for creating a RTZ driver instance.
 *
 * @param[in] ID Id number of the peripheral.
 */
#define Q_DRV_RTZ_INSTANCE_DEFINE(ID)                                                                                  \
    {                                                                                                                  \
        .internal = {},                              /* Initialize internal data to zeros. */                          \
            .baseAddr = Q_REG_RTZ_BASE_ADDR_GET(ID), /* Initialize the base address of the peripheral instance. */     \
            .id = ID,                                /* Initialize the ID of the peripheral instance. */               \
    }

/**
 * @brief Macro for defining instance pin configuration.
 *
 * @param[in] INSTANCE_ID RTZ instance ID.
 * @param[in] TX_PIN      RTZ TX pin.
 */
#define Q_DRV_RTZ_PIN_CONFIG(INSTANCE_ID, TX_PIN)                                                                      \
    {                                                                                                                  \
        .tx = {.pin = TX_PIN, .alternate = Q_DRV_IOB_ALT_FUNCT_GET(TX_PIN, RTZ, INSTANCE_ID, TX)}, /* */               \
    }

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

/**
 * @brief Configure the RTZ output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer, can be filled with @p Q_DRV_PWMXL_PIN_CONFIG macro.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin number or pin in use.
 */
qResult_t qDrvRTZ_PinConfigSet(const qDrvRTZ_PinConfig_t* const pPinConfig);

/**
 * @brief Deinitialize the RTZ output pin.
 *
 * @param[in] pPinConfig Pin configuration structure pointer.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVPARAM Invalid pin number or pin not mapped.
 */
qResult_t qDrvRTZ_PinConfigClear(const qDrvRTZ_PinConfig_t* const pPinConfig);

/**
 * @brief Initialize RTZ instance.
 *
 * @param[in] pInstance   RTZ instance pointer.
 * @param[in] pConfig     Configuration structure pointer.
 * @param[in] pCallback   Callback function pointer.
 * @param[in] arg         User supplied argument for the callback.
 * @param[in] irqPriority ARM interrupt priority.
 *
 * @retval Q_OK              Success.
 * @retval Q_ERR_INVSTATE    Instance is already initialized.
 * @retval Q_ERR_INVPARAM    Provided configuration is not correct.
 * @retval Q_ERR_OUTOFMEMORY DMA channel allocation failed.
 */
qResult_t qDrvRTZ_Init(qDrvRTZ_t* const pInstance, const qDrvRTZ_Config_t* const pConfig, const qDrvRTZ_Cb_t pCallback,
                       void* arg, UInt32 irqPriority);

/**
 * @brief Reconfigure RTZ instance.
 *
 * @note After reconfiguration, function @p qDrvRTZ_BufferSet() needs to be called.
 *
 * @param[in] pInstance RTZ instance pointer.
 * @param[in] pConfig   Configuration structure pointer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 */
qResult_t qDrvRTZ_Reconfigure(qDrvRTZ_t* const pInstance, const qDrvRTZ_Config_t* const pConfig);

/**
 * @brief Deinitialize the RTZ instance.
 *
 * @param[in] pInstance RTZ instance pointer.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvRTZ_Deinit(qDrvRTZ_t* const pInstance);

/**
 * @brief Check whether the instance is initialised.
 *
 * @param[in] pInstance Pointer to the driver instance structure.
 *
 * @retval True if the instance is initialised, false otherwise.
 */
Bool qDrvRTZ_InitCheck(qDrvRTZ_t* const pInstance);

/**
 * @brief Set the memory buffer where the driver driver stores the pixels data.
 *        Memory buffer should be aligned to the buffer mode.
 *
 * @param[in] pInstance  RTZ instance pointer.
 * @param[in] pBuffer    Buffer pointer where the driver stores the pixels data.
 * @param[in] bufferSize Buffer size in bytes.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Wrong buffer size or buffer address is not aligned to buffer mode.
 */
qResult_t qDrvRTZ_BufferSet(qDrvRTZ_t* const pInstance, void* pBuffer, UInt16 bufferSize);

/**
 * @brief Set pixels data in the memory buffer.
 *
 * @note For updating the pixels call the @p qDrvRTZ_Update() function.
 *
 * @param[in] pInstance       RTZ instance pointer.
 * @param[in] pPixelBuffer    Pointer to the pixels buffer data. Buffer should be aligned to buffer mode.
 * @param[in] startPixelIndex Index of the first pixel which is to be changed.
 * @param[in] pixelCount      Number if pixels stored in @p pPixelBuffer.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_INVPARAM      Wrong pixel start index or pixels count.
 * @retval Q_ERR_GENERAL       Memory buffer is not set.
 */
qResult_t qDrvRTZ_PixelSet(qDrvRTZ_t* const pInstance, const void* const pPixelBuffer, UInt16 startPixelIndex,
                           UInt16 pixelCount);

/**
 * @brief Update pixels connected to the peripheral.
 *
 * @param[in] pInstance RTZ instance pointer.
 * @param[in] trigger   True if the sequence is to be updated immediately, false if trigger will be called with QUIC
 * action.
 *
 * @retval Q_OK                Success.
 * @retval Q_ERR_UNINITIALIZED Instance is not initialized.
 * @retval Q_ERR_GENERAL       Memory buffer is not set.
 */
qResult_t qDrvRTZ_Update(qDrvRTZ_t* const pInstance, Bool trigger);

/**
 * @brief Check if the peripheral is transmitting the sequence.
 *
 * @param[in] instance RTZ instance pointer.
 *
 * @returns True if the peripheral is transmitting the sequence, false otherwise.
 */
Bool qDrvRTZ_BusyCheck(qDrvRTZ_t* const pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_RTZ_H_
