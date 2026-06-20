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

/** @file "qorvo_internals.h"
 *
 */

#ifndef _QORVO_INTERNALS_H_
#define _QORVO_INTERNALS_H_

/*
 * Enabled components
 */

#define GP_COMP_GPHAL_MAC
#define GP_COMP_GPHAL_PBM
#define GP_COMP_GPHAL_RADIO
#define GP_COMP_GPHAL_SEC
#define GP_COMP_QVOT

/*
 * Components numeric ids
 */

#define GP_COMPONENT_ID_APP                                1
#define GP_COMPONENT_ID_ASSERT                             29
#define GP_COMPONENT_ID_BASECOMPS                          35
#define GP_COMPONENT_ID_BSP                                8
#define GP_COMPONENT_ID_COM                                10
#define GP_COMPONENT_ID_ENCRYPTION                         124
#define GP_COMPONENT_ID_FREERTOS                           24
#define GP_COMPONENT_ID_GPHAL                              7
#define GP_COMPONENT_ID_HALAPPUCSUBSYSTEM                  6
#define GP_COMPONENT_ID_HALCORTEXM4                        6
#define GP_COMPONENT_ID_HALPLATFORM                        6
#define GP_COMPONENT_ID_LOG                                11
#define GP_COMPONENT_ID_MACCORE                            109
#define GP_COMPONENT_ID_MACDISPATCHER                      114
#define GP_COMPONENT_ID_NVM                                32
#define GP_COMPONENT_ID_PAD                                126
#define GP_COMPONENT_ID_PD                                 104
#define GP_COMPONENT_ID_PERIPHERAL                         83
#define GP_COMPONENT_ID_POOLMEM                            106
#define GP_COMPONENT_ID_QVOT                               157
#define GP_COMPONENT_ID_RADIO                              204
#define GP_COMPONENT_ID_RANDOM                             108
#define GP_COMPONENT_ID_RAP                                80
#define GP_COMPONENT_ID_REGMAP                             82
#define GP_COMPONENT_ID_RESET                              33
#define GP_COMPONENT_ID_RT_NRT_COMMON                      -1
#define GP_COMPONENT_ID_RXARBITER                          2
#define GP_COMPONENT_ID_SCHED                              9
#define GP_COMPONENT_ID_STAT                               22
#define GP_COMPONENT_ID_UTILS                              4
#define GP_COMPONENT_ID_WMRK                               51

/*
 * Component: gpBsp
 */

#define GP_DIVERSITY_QPG6200_QFN32_DK

/*
 * Component: gpCom
 */

#define GP_COM_DIVERSITY_SERIAL_NO_SYN_NO_CRC
#define GP_COM_DIVERSITY_SERIAL_NO_SYN_SENDTO_ID           157
#define GP_COM_DIVERSITY_SERIAL_SYN_DISABLED
#define GP_COM_ZERO_COPY_BLOCK_TRANSFERS

/*
 * Component: gpFreeRTOS
 */

#define GP_DIVERSITY_FREERTOS
#define GP_FREERTOS_DIVERSITY_STATIC_ALLOC

/*
 * Component: gphal
 */

#define GP_COMP_GPHAL_ES
#define GP_COMP_GPHAL_ES_ABS_EVENT
#define GP_COMP_GPHAL_ES_EXT_EVENT
#define GP_COMP_GPHAL_ES_REL_EVENT
#define GP_DIGITAL_COEX
#define GP_DIVERSITY_GPHAL_INTERN
#define GP_DIVERSITY_GPHAL_OSCILLATOR_BENCHMARK
#define GP_DIVERSITY_RT_SYSTEM_ADDR_FROM_LINKERSCRIPT
#define GP_DIVERSITY_RT_SYSTEM_BLEMGR_IN_FLASH
#define GP_DIVERSITY_RT_SYSTEM_GENERATE
#define GP_DIVERSITY_RT_SYSTEM_IN_FLASH
#define GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
#define GP_HAL_DIVERSITY_DUTY_CYCLE
#define GP_HAL_DIVERSITY_INCLUDE_IPC
#define GP_HAL_DIVERSITY_RAW_ENHANCED_ACK_RX
#define GP_HAL_DIVERSITY_RAW_FRAME_ENCRYPTION
#define GP_HAL_INCLUDE_INFO_PAGE_HELPERS

/*
 * Component: gpLog
 */

#define GP_LOG_DIVERSITY_NO_TIME_NO_COMPONENT_ID
#define GP_LOG_DIVERSITY_VSNPRINTF

/*
 * Component: gpMacCore
 */

#define GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR
#define GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT
#define GP_MACCORE_DIVERSITY_FFD
#define GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION
#define GP_MACCORE_DIVERSITY_POLL_ORIGINATOR
#define GP_MACCORE_DIVERSITY_POLL_RECIPIENT
#define GP_MACCORE_DIVERSITY_RAW_FRAMES
#define GP_MACCORE_DIVERSITY_RX_WINDOWS
#define GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR
#define GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT
#define GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR
#define GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR
#define GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR
#define GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT
#define GP_MACCORE_DIVERSITY_SCAN_RECIPIENT
#define GP_MACCORE_DIVERSITY_SECURITY_ENABLED
#define GP_MACCORE_DIVERSITY_THREAD_1_2
#define GP_MACCORE_DIVERSITY_TIMEDTX

/*
 * Component: gpNvm
 */

#define GP_DIVERSITY_NVM
#define GP_NVM_DIVERSITY_ELEMENT_IF
#define GP_NVM_DIVERSITY_ELEMIF_KEYMAP
#define GP_NVM_DIVERSITY_SUBPAGEDFLASH_V2
#define GP_NVM_DIVERSITY_TAG_IF
#define GP_NVM_DIVERSITY_USE_POOLMEM
#define GP_NVM_USE_ASSERT_SAFETY_NET

/*
 * Component: gpPd
 */

#define GP_DIVERSITY_PD_USE_PBM_VARIANT

/*
 * Component: gpSched
 */

#define GP_SCHED_DIVERSITY_SCHEDULE_INSECONDSAPI
#define GP_SCHED_DIVERSITY_USE_ARGS

/*
 * Component: gpUtils
 */

#define GP_UTILS_DIVERSITY_LINKED_LIST

/*
 * Component: halCortexM4
 */

#define GP_BSP_CONTROL_WDT_TIMER
#define GP_HAL_NVM_APPUC_OFFSET                            77824
#define GP_KX_FLASH_SIZE                                   2048
#define GP_KX_SYSRAM_SIZE                                  32
#define GP_KX_UCRAM_SIZE                                   192
#define HAL_DIVERSITY_SEMAILBOX
#define HAL_DIVERSITY_UART
#define QPG6200

/*
 * Component: qvOT
 */

#define QVOT_DIVERSITY_GLUE_ONLY_BUILD

/*
 * Other flags
 */

#define GP_BLE_NR_OF_CONNECTION_COMPLETE_EVENT_BUFFERS     0
#define GP_BLE_NR_OF_SUPPORTED_PROCEDURES                  0
#define GP_BLE_NR_OF_SUPPORTED_PROCEDURE_CALLBACKS         0
#define GP_COM_DIVERSITY_NO_RX
#define GP_DIVERSITY_CORTEXM4
#define GP_DIVERSITY_GPHAL_INDIRECT_TRANSMISSION
#define GP_DIVERSITY_GPHAL_XP4002
#define GP_DIVERSITY_NR_OF_STACKS                          2
#define GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM
#define GP_GIT_DESCRIBE                                    "6c19d689c50d"
#define GP_GIT_SHA                                         6c19d689c50d8d62daf368e4ab3ab01ee7571dc8
#define GP_GIT_SHA_SHORT                                   6c19d68
#define GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS                 0
#define GP_RX_ARBITER_DUTY_CYCLE
#define QVOT_MAX_CHILDREN                                  64
#define QVOT_THREAD_1_2

#endif //_QORVO_INTERNALS_H_
