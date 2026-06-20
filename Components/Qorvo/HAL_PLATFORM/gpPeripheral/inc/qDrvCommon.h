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

#ifndef _Q_DRV_COMMON_H_
#define _Q_DRV_COMMON_H_

/**
 * @defgroup Q_DRV_COMMON Common Code
 * @{
 *
 * @brief This is a common code for all other drivers.
 *
 * @file qDrvCommon.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qReg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#ifndef Q_DRV_IRQ_PRIO_HIGH
/** @brief High interrupt priority. */
#define Q_DRV_IRQ_PRIO_HIGH 0
#endif

#ifndef Q_DRV_IRQ_PRIO_MID
/** @brief Medium interrupt priority. */
#define Q_DRV_IRQ_PRIO_MID (Q_DRV_IRQ_PRIO_HIGH + 1)
#endif

#ifndef Q_DRV_IRQ_PRIO_LOW
/** @brief Low interrupt priority. */
#define Q_DRV_IRQ_PRIO_LOW (Q_DRV_IRQ_PRIO_MID + 1)
#endif

#ifndef Q_DRV_IRQ_PRIO_DEFAULT
/** @brief Default interrupt priority. */
#define Q_DRV_IRQ_PRIO_DEFAULT Q_DRV_IRQ_PRIO_MID
#endif

/*****************************************************************************
 *                    Public Function Declarations
 *****************************************************************************/

/**
 * @brief Function for integer dividing of a number.
 *
 * @param[in] number  Number to be divided and rounded.
 * @param[in] divisor Divisor of the number.
 *
 * @return Divided and rounded integer.
 */
static inline Int32 qDrvCommon_DivideRoundClosest(Int32 number, Int32 divisor)
{
    return ((number < 0) == (divisor < 0)) ? ((number + divisor / 2) / divisor) : ((number - divisor / 2) / divisor);
}

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_COMMON_H_
