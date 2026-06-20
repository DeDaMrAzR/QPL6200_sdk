/*
 * Copyright (c) 2017, Qorvo Inc
 *
 * This software is owned by Qorvo Inc
 * and protected under applicable copyright laws.
 * It is delivered under the terms of the license
 * and is intended and supplied for use solely and
 * exclusively with products manufactured by\
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

#ifndef _GPBLECHANNELMAPMANAGER_H_
#define _GPBLECHANNELMAPMANAGER_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpHci_Includes.h"
#include "gpBle.h"
#include "gpBle_defs.h"

#ifdef GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED
#include "gpBleLlcp.h"
#endif // GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

// Callback definition for services that want to get notified whenever a new global channel map is known in the controller
typedef void (* gpBleChannelMapManager_NewChannelMapCb_t) (gpBleChannelMapHandle_t newChannelMapHandle, gpHci_ChannelMap_t* pChannelMap);

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpBleChannelMapManager_Init(void);
void gpBleChannelMapManager_Reset(Bool firstReset);

// Channel map context management functions
gpBleChannelMapHandle_t gpBleChannelMapManager_AllocateHandle(gpHci_ChannelMap_t* pChannelMap);
Bool gpBleChannelMapManager_IsHandleInUse(gpBleChannelMapHandle_t handle);
Bool gpBleChannelMapManager_AddUser(gpBleChannelMapHandle_t handle);
Bool gpBleChannelMapManager_RemoveUser(gpBleChannelMapHandle_t handle);
Bool gpBleChannelMapManager_FreeHandle(gpBleChannelMapHandle_t handle);
Bool gpBleChannelMapManager_PopulateMap(gpBleChannelMapHandle_t handle, gpHci_ChannelMap_t* pChannelMap);
void gpBleChannelMapManager_GetChannelMap(gpBleChannelMapHandle_t handle, UInt8* pChannels);
void gpBleChannelMapManager_SwitchChannelMap(gpBleChannelMapHandle_t* pHandleCurrent, gpBleChannelMapHandle_t* pHandleNew, UInt8* pCompressedPtr);


/*****************************************************************************
 *                    Service Function Definitions
 *****************************************************************************/

gpHci_Result_t gpBle_LeSetHostChannelClassification(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);

#ifdef GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED
gpHci_Result_t gpBle_LeReadChannelMap(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf);
#endif //GP_DIVERSITY_BLE_ACL_CONNECTIONS_SUPPORTED

#endif //_GPBLECHANNELMAPMANAGER_H_
