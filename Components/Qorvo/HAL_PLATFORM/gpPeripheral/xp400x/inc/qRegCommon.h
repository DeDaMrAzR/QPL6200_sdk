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

#ifndef _Q_REG_COMMON_H_
#define _Q_REG_COMMON_H_

#include "qReg.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @enum qRegCommon_Drive_t */
//@{
#define qRegCommon_DrivePP GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL
#define qRegCommon_DriveOD GP_WB_ENUM_GENERIC_IO_DRIVE_OPEN_DRAIN
/** @typedef qRegCommon_Drive_t
 *  @brief GPIO input drive mode.
 */
typedef UInt8 qRegCommon_Drive_t;
//@}

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_COMMON_H_ */
