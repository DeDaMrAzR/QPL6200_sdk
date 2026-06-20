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

#ifndef _Q_DRV_ES_H_
#define _Q_DRV_ES_H_

/**
 * @defgroup Event scheduler driver
 * @{
 *
 * @brief This is a driver for event scheduler (ES).
 *
 * @file qDrvES.h
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "qDrvCommon.h"

#if defined(REG_QUIC_PRESENT)
#include "qDrvQUICInternal.h"
#endif /* defined(REG_QUIC_PRESENT) */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
#if defined(REG_QUIC_PRESENT)
/** @brief ES event trigger configuration structure. */
typedef struct {
    UInt8 esEventId;                     /**< ES event identification.*/
    Q_DRV_QUIC_INTERNAL_TRIGGER_FIELD(); /**< Required trigger field. */
} qDrvES_QuicTriggerConfig_t;
#endif                                   /* REG_QUIC_PRESENT */

/** @brief ES event triggers instance structure. */
typedef struct {
#if defined(REG_QUIC_PRESENT)
    qDrvQUIC_Internal_t* quicApi; /**< QUIC API pointer implementation. */
#endif                            /* REG_QUIC_PRESENT */
} qDrvES_t;

/**
 * @brief Initializes the state variables for the ES event triggers driver.
 *
 * @param[in] pInstance   Pointer to the ES instance.
 *
 * @retval Q_OK           Success.
 * @retval Q_ERR_INVSTATE Instance is already initialized.
 */
qResult_t qDrvES_Init(qDrvES_t* const pInstance);

/**
 * @brief Deinitialize the ES instance.
 *
 * @param[in] pInstance Pointer to the ES instance.
 *
 * @retval Q_OK Success.
 */
qResult_t qDrvES_Deinit(qDrvES_t* const pInstance);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // _Q_DRV_ES_H_
