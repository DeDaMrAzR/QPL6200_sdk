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

#ifndef _Q_REG_H_
#define _Q_REG_H_

/**
 * @defgroup Q_REG_H Common definitions
 * @{
 *
 * @brief Common definitions shared across drivers.
 *
 * @file qReg.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qRegCommonInternal.h"
#include "qRegChip.h"

#include "hal.h"

#include <inttypes.h>

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/**
 * @brief Macro for getting the number of elements in the array.
 *
 * @param[in] array Name of the array.
 *
 * @return Array element count.
 */
#define Q_ARRAY_SIZE(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

/**
 * @brief Macro for concatenating multiple arguments. Support up to 8 arguments.
 *
 * @param[in] ... Arguments to concatenate.
 */
#define Q_CONCAT(...) Q_CONCAT_2(_Q_CONCAT_, Q_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define Q_GET_0_ARG(arg0, ...)                                           arg0
#define Q_GET_1_ARG(arg0, arg1, ...)                                     arg1
#define Q_GET_2_ARG(arg0, arg1, arg2, ...)                               arg2
#define Q_GET_3_ARG(arg0, arg1, arg2, arg3, ...)                         arg3
#define Q_GET_4_ARG(arg0, arg1, arg2, arg3, arg4, ...)                   arg4
#define Q_GET_5_ARG(arg0, arg1, arg2, arg3, arg4, arg5, ...)             arg5
#define Q_GET_6_ARG(arg0, arg1, arg2, arg3, arg4, arg5, arg6, ...)       arg6
#define Q_GET_7_ARG(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, ...) arg7

/**
 * @brief Macro for getting the N-th argument.
 *
 * @param[in] N   Argument index.
 * @param[in] ... Arguments to get from.
 */
#define Q_ARG(N, ...) Q_CONCAT(Q_GET_, N, _ARG)(__VA_ARGS__)

/**
 * @brief Concatenate two arguments.
 *
 * @deprecated
 * @see Q_CONCAT
 */
#define CONCAT_2(A, B) Q_CONCAT(A, B)

/**
 * @brief Concatenate three arguments.
 *
 * @deprecated
 * @see Q_CONCAT
 */
#define CONCAT_3(A, B, C) Q_CONCAT(A, B, C)

/**
 * @brief Remove brackets from argument.
 *
 * @param[in] ... Arguments to remove brackets from.
 */
#define Q_DEBRACKET(...) __VA_ARGS__

/**
 * @brief Macro for calling fn for each argument passed to a macro. Supports up to 32 arguments.
 *
 * @param[in] fn Macro to be called for each argument.
 * @param[in] ... List of arguments (up to 32).
 */
#define Q_FOREACH(fn, ...) _Q_FOREACH(fn, Q_COUNT_ARGS(__VA_ARGS__), __VA_ARGS__)

/** @brief Undefined pin. */
#define Q_NOPIN 0xFF

/** @brief Undefined ID. */
#define Q_NOID 0xFF

/** @brief Undefined index. */
#define Q_NOINDEX 0xFF

/** @brief Undefined channel. */
#define Q_NOCHANNEL 0xFF

/** @brief Undefined flag. */
#define Q_NOFLAG 0xFF

/** @brief Calculate the offset in bytes (not words!) of the specific bit position
 *         for a given word size.
 */
#define Q_BYTEOFFSET(BITPOS, WORDSIZE) (((BITPOS) / ((WORDSIZE)*8)) * (WORDSIZE))

/** @brief System assert macro for given condition. */
#define Q_ASSERT(x)        GP_ASSERT_SYSTEM(x)
#define Q_STATIC_ASSERT(x) _Static_assert(x)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/** @enum qResult_t */
//@{
#define Q_OK                0 /**< @brief Operation successful. */
#define Q_ERR_GENERAL       1 /**< @brief General error. */
#define Q_ERR_BUSY          2 /**< @brief Resource busy. */
#define Q_ERR_TIMEOUT       3 /**< @brief Timeout reached. */
#define Q_ERR_INVPARAM      4 /**< @brief Invalid parameters. */
#define Q_ERR_INVSTATE      5 /**< @brief Invalid state. */
#define Q_ERR_OUTOFRANGE    6 /**< @brief Parameter value out of range. */
#define Q_ERR_OUTOFMEMORY   7 /**< @brief Out of memory. */
#define Q_ERR_NOTSUPPORTED  8 /**< @brief Configuration not supported. */
#define Q_ERR_UNINITIALIZED 9 /**< @brief Driver not initialized. */
/** @typedef qResult_t
 *  @brief Common result type.
 */
typedef UInt16 qResult_t;
//@}

/** @} */

#endif // _Q_REG_H_
