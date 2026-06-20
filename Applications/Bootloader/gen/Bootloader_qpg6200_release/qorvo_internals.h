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

#define GP_COMP_ASSERT
#define GP_COMP_LOG
#define GP_COMP_LZMA
#define GP_COMP_UPGRADE
#define GP_COMP_UTILS

/*
 * Components numeric ids
 */

#define GP_COMPONENT_ID_APP                                1
#define GP_COMPONENT_ID_ASSERT                             29
#define GP_COMPONENT_ID_BSP                                8
#define GP_COMPONENT_ID_GPHAL                              7
#define GP_COMPONENT_ID_HALAPPUCSUBSYSTEM                  6
#define GP_COMPONENT_ID_HALCORTEXM4                        6
#define GP_COMPONENT_ID_HALPLATFORM                        6
#define GP_COMPONENT_ID_LOG                                11
#define GP_COMPONENT_ID_LZMA                               105
#define GP_COMPONENT_ID_PAD                                126
#define GP_COMPONENT_ID_PD                                 104
#define GP_COMPONENT_ID_RANDOM                             108
#define GP_COMPONENT_ID_RAP                                80
#define GP_COMPONENT_ID_REGMAP                             82
#define GP_COMPONENT_ID_RESET                              33
#define GP_COMPONENT_ID_RT_NRT_COMMON                      -1
#define GP_COMPONENT_ID_SCHED                              9
#define GP_COMPONENT_ID_STAT                               22
#define GP_COMPONENT_ID_UPGRADE                            115
#define GP_COMPONENT_ID_UTILS                              4
#define GP_COMPONENT_ID_VERSION                            129

/*
 * Component: gpBsp
 */

#define GP_DIVERSITY_QPG6200_QFN32_DK

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
#define GP_HAL_INCLUDE_INFO_PAGE_HELPERS

/*
 * Component: gpUpgrade
 */

#define GP_DIVERSITY_FLASH_APP_START_OFFSET                0xC400
#define GP_DIVERSITY_UPGRADE_IMAGE_USER_LICENSE_OFFSET     0xC000
#define GP_UPGRADE_DIVERSITY_COMPRESSION
#define GP_UPGRADE_DIVERSITY_DUAL_BOOT

/*
 * Component: gpUtils
 */

#define GP_UTILS_DIVERSITY_LINKED_LIST

/*
 * Component: halCortexM4
 */

#define GP_BSP_CONTROL_WDT_TIMER
#define GP_DIVERSITY_FLASH_BL_SIZE                         0x8000
#define GP_HAL_NVM_APPUC_OFFSET                            77824
#define GP_KX_FLASH_SIZE                                   2048
#define GP_KX_SYSRAM_SIZE                                  32
#define GP_KX_UCRAM_SIZE                                   192
#define HAL_DIVERSITY_SEMAILBOX
#define QPG6200

/*
 * Other flags
 */

#define GP_BLE_NR_OF_CONNECTION_COMPLETE_EVENT_BUFFERS     0
#define GP_BLE_NR_OF_SUPPORTED_PROCEDURES                  0
#define GP_BLE_NR_OF_SUPPORTED_PROCEDURE_CALLBACKS         0
#define GP_DIVERSITY_APP_WITH_SECURE_BOOT_HEADER
#define GP_DIVERSITY_BOOTLOADER_BUILD
#define GP_DIVERSITY_CORTEXM4
#define GP_DIVERSITY_GPHAL_XP4002
#define GP_DIVERSITY_KEEP_NRT_IN_FLASH
#define GP_DIVERSITY_REGMAPS_IN_HAL_PLATFORM
#define GP_GIT_DESCRIBE                                    "6c19d689c50d"
#define GP_GIT_SHA                                         6c19d689c50d8d62daf368e4ab3ab01ee7571dc8
#define GP_GIT_SHA_SHORT                                   6c19d68
#define GP_HAL_ES_ABS_EVENT_NMBR_OF_EVENTS                 0

#endif //_QORVO_INTERNALS_H_
