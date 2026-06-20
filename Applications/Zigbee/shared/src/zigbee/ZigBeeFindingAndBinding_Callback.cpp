/*
 * Copyright (c) 2018, Qorvo Inc
 *
 *
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

/** @file "ZigBeeFindingAndBinding.cpp"
 *
 * ZigBee finding and Binding functionality for application.
*/

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"
#include "hal.h"
#include "gpHal.h"

#include "App.h"
#include "App_Common.h"
#include "ZigBeeFindingAndBinding_Callback.h"

/*****************************************************************************
 *                    External data
 *****************************************************************************/

extern CZigBeeApplicationSupport aps;

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID         GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Extern Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Application function prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Application function definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Finding and Binding Callback
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
/*****************************************************************************
 *  --- ZigBee Binding callbacks
 *****************************************************************************/
/** @brief callback when a binding is set/removed as a result of finding and binding
 *  Required Header: "ZigBeeFindingAndBinding_Callback.h"
*/
void Application_cbZigBeePostManageBinding(CZigBeeDeviceObjectBinding &binding, const Bool bBind)
{
#ifdef GP_DIVERSITY_LOG
    if (binding.m_bDestinationAddressingMode == CZigBeeAddress::typeGroup)
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: EP%d: %c%cBind Cluster 0x%04x to group 0x%x",0
            ,binding.m_bSourceEndpoint
            ,bBind ? ' ' : 'U'
            ,bBind ? ' ' : 'n'
            ,binding.m_wClusterID
            ,binding.m_wDestinationAddress);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE EP%d: %c%cBind Cluster 0x%04x to Device:%016llx EP:%d",0
            ,binding.m_bSourceEndpoint
            ,bBind ? ' ' : 'U'
            ,bBind ? ' ' : 'n'
            ,binding.m_wClusterID
            ,binding.m_extended.m_qwDestinationAddress
            ,binding.m_extended.m_bDestinationEndpoint);
    }
#endif //GP_DIVERSITY_LOG
#ifdef GP_APPLICATION_SWITCH

    // set to control via binding after bind is set
    Application_SetControlMode(App_ControlMode_Binding);
#endif //GP_APPLICATION_SWITCH
}

#endif //GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
