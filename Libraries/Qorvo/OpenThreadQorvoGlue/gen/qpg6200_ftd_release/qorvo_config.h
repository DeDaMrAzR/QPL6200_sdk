/*
 * Copyright (c) 2020, Qorvo Inc
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
 */

/** @file "qorvo_config.h"
 *
 */

#ifndef _QORVO_CONFIG_H_
#define _QORVO_CONFIG_H_

/*
 * Version info
 */

#define GP_CHANGELIST                                      0


/*
 * Component: gpAssert
 */

/* Choose reset as default assert action */
#define GP_DIVERSITY_ASSERT_ACTION_RESET

/* Choose to do nothing as default assert reporting */
#define GP_DIVERSITY_ASSERT_REPORTING_NOTHING


/*
 * Component: gpBsp
 */

/* Contains filename of BSP header file to include */
#define GP_BSP_FILENAME                                    "gpBsp_QPG6200_QFN32_DK_LED_PB_10dBm.h"

/* UART baudrate */
#define GP_BSP_UART_COM_BAUDRATE                           115200


/*
 * Component: gpCom
 */

/* Enable SYN datastream encapsulation */
#define GP_COM_DIVERSITY_SERIAL

/* Use UART for COM - defined as default in code */
#define GP_DIVERSITY_COM_UART


/*
 * Component: gphal
 */

/* Minimum amount of 16-bit short address entries for a single stack to keep data pending for */
#define GPHAL_DP_SHORT_LIST_MIN_STACK                      64


/*
 * Component: gpNvm
 */

/* Maximum number of unique tags in each pool. Used for memory allocation at Tag level API */
#define GP_NVM_NBR_OF_UNIQUE_TAGS                          86

#define GP_NVM_TYPE                                        6


/*
 * Component: gpPeripheral
 */

/* Enable gpPeripheral module. */
#define HAL_DIVERSITY_V2

/* High interrupt priority level value. */
#define Q_DRV_IRQ_PRIO_HIGH                                configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY


/*
 * Component: gpSched
 */

/* Callback after every main loop iteration. */
#define GP_SCHED_NR_OF_IDLE_CALLBACKS                      0


/*
 * Component: halCortexM4
 */

/* Use application header */
#define GP_DIVERSITY_APPUC_FW_HEADER

/* Set if hal has real mutex capability. Used to skip even disabling/enabling global interrupts. */
#define HAL_MUTEX_SUPPORTED


#include "qorvo_internals.h"

#endif //_QORVO_CONFIG_H_
