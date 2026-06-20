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
 * Component: PersistentStorageBlock
 */

/* set working range of gpNvm_PoolId_t, requires setting number of phy sectors for each pool */
#define GP_NVM_NBR_OF_POOLS                                3

/* number of sectors of pool 1 */
#define GP_NVM_POOL_1_NBR_OF_PHY_SECTORS                   4

/* number of sectors of pool 2 */
#define GP_NVM_POOL_2_NBR_OF_PHY_SECTORS                   8

/* number of sectors of pool 3 */
#define GP_NVM_POOL_3_NBR_OF_PHY_SECTORS                   4


#include "qorvo_internals.h"

#endif //_QORVO_CONFIG_H_
