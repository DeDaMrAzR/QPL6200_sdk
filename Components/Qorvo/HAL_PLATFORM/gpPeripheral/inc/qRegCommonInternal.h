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

#ifndef _REG_COMMON_INTERNAL_H_
#define _REG_COMMON_INTERNAL_H_

/**
 * @defgroup DRV_COMMON Common internal definitions
 * @{
 *
 * @brief Common definitions shared across gpPeripheral module.
 *
 * @file regCommonInternal.h
 *
 */

/*****************************************************************************
 *                   Macro Definitions
 *****************************************************************************/

#define _Q_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22,  \
                 _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42,   \
                 _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,   \
                 _63, N, ...)                                                                                          \
    N

#define _Q_RSEQ_N()                                                                                                    \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36,    \
        35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,  \
        7, 6, 5, 4, 3, 2, 1, 0

#define _Q_NARG_(...)     _Q_ARG_N(__VA_ARGS__)
#define Q_COUNT_ARGS(...) _Q_NARG_(__VA_ARGS__, _Q_RSEQ_N())

#define Q_CONCAT_2_(p1, p2) p1##p2

#define Q_CONCAT_2(p1, p2) Q_CONCAT_2_(p1, p2)

#define _Q_CONCAT_0(arg, ...) arg
#define _Q_CONCAT_1(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_0(__VA_ARGS__))
#define _Q_CONCAT_2(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_1(__VA_ARGS__))
#define _Q_CONCAT_3(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_2(__VA_ARGS__))
#define _Q_CONCAT_4(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_3(__VA_ARGS__))
#define _Q_CONCAT_5(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_4(__VA_ARGS__))
#define _Q_CONCAT_6(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_5(__VA_ARGS__))
#define _Q_CONCAT_7(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_6(__VA_ARGS__))
#define _Q_CONCAT_8(arg, ...) Q_CONCAT_2(arg, _Q_CONCAT_7(__VA_ARGS__))

#define _Q_FOREACH_0(fn, ...)
#define _Q_FOREACH_1(fn, arg, ...)  fn(arg)
#define _Q_FOREACH_2(fn, arg, ...)  fn(arg) _Q_FOREACH_1(fn, __VA_ARGS__)
#define _Q_FOREACH_3(fn, arg, ...)  fn(arg) _Q_FOREACH_2(fn, __VA_ARGS__)
#define _Q_FOREACH_4(fn, arg, ...)  fn(arg) _Q_FOREACH_3(fn, __VA_ARGS__)
#define _Q_FOREACH_5(fn, arg, ...)  fn(arg) _Q_FOREACH_4(fn, __VA_ARGS__)
#define _Q_FOREACH_6(fn, arg, ...)  fn(arg) _Q_FOREACH_5(fn, __VA_ARGS__)
#define _Q_FOREACH_7(fn, arg, ...)  fn(arg) _Q_FOREACH_6(fn, __VA_ARGS__)
#define _Q_FOREACH_8(fn, arg, ...)  fn(arg) _Q_FOREACH_7(fn, __VA_ARGS__)
#define _Q_FOREACH_9(fn, arg, ...)  fn(arg) _Q_FOREACH_8(fn, __VA_ARGS__)
#define _Q_FOREACH_10(fn, arg, ...) fn(arg) _Q_FOREACH_9(fn, __VA_ARGS__)
#define _Q_FOREACH_11(fn, arg, ...) fn(arg) _Q_FOREACH_10(fn, __VA_ARGS__)
#define _Q_FOREACH_12(fn, arg, ...) fn(arg) _Q_FOREACH_11(fn, __VA_ARGS__)
#define _Q_FOREACH_13(fn, arg, ...) fn(arg) _Q_FOREACH_12(fn, __VA_ARGS__)
#define _Q_FOREACH_14(fn, arg, ...) fn(arg) _Q_FOREACH_13(fn, __VA_ARGS__)
#define _Q_FOREACH_15(fn, arg, ...) fn(arg) _Q_FOREACH_14(fn, __VA_ARGS__)
#define _Q_FOREACH_16(fn, arg, ...) fn(arg) _Q_FOREACH_15(fn, __VA_ARGS__)
#define _Q_FOREACH_17(fn, arg, ...) fn(arg) _Q_FOREACH_16(fn, __VA_ARGS__)
#define _Q_FOREACH_18(fn, arg, ...) fn(arg) _Q_FOREACH_17(fn, __VA_ARGS__)
#define _Q_FOREACH_19(fn, arg, ...) fn(arg) _Q_FOREACH_18(fn, __VA_ARGS__)
#define _Q_FOREACH_20(fn, arg, ...) fn(arg) _Q_FOREACH_19(fn, __VA_ARGS__)
#define _Q_FOREACH_21(fn, arg, ...) fn(arg) _Q_FOREACH_20(fn, __VA_ARGS__)
#define _Q_FOREACH_22(fn, arg, ...) fn(arg) _Q_FOREACH_21(fn, __VA_ARGS__)
#define _Q_FOREACH_23(fn, arg, ...) fn(arg) _Q_FOREACH_22(fn, __VA_ARGS__)
#define _Q_FOREACH_24(fn, arg, ...) fn(arg) _Q_FOREACH_23(fn, __VA_ARGS__)
#define _Q_FOREACH_25(fn, arg, ...) fn(arg) _Q_FOREACH_24(fn, __VA_ARGS__)
#define _Q_FOREACH_26(fn, arg, ...) fn(arg) _Q_FOREACH_25(fn, __VA_ARGS__)
#define _Q_FOREACH_27(fn, arg, ...) fn(arg) _Q_FOREACH_26(fn, __VA_ARGS__)
#define _Q_FOREACH_28(fn, arg, ...) fn(arg) _Q_FOREACH_27(fn, __VA_ARGS__)
#define _Q_FOREACH_29(fn, arg, ...) fn(arg) _Q_FOREACH_28(fn, __VA_ARGS__)
#define _Q_FOREACH_30(fn, arg, ...) fn(arg) _Q_FOREACH_29(fn, __VA_ARGS__)
#define _Q_FOREACH_31(fn, arg, ...) fn(arg) _Q_FOREACH_30(fn, __VA_ARGS__)
#define _Q_FOREACH_32(fn, arg, ...) fn(arg) _Q_FOREACH_31(fn, __VA_ARGS__)

#define __Q_FOREACH(fn, nargs, ...) _Q_FOREACH_##nargs(fn, __VA_ARGS__)
#define _Q_FOREACH(fn, nargs, ...)  __Q_FOREACH(fn, nargs, __VA_ARGS__)

/** @} */

#endif // _REG_COMMON_INTERNAL_H_
