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
#include "ZigBeeCommissioning.h"
#include "gpLog.h"
#include "gpSched.h"
#include "hal.h"
#include "gpHal.h"

#include "App_Common.h"
#include "ZigBeeFindingAndBinding.h"

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
 *                    Finding and Binding
 *****************************************************************************/
#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET
/** @brief Start/Stop Finding and Binding process(Target) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint     - Target endpoint for the operation
*   @param enable       - enable/disable finding and binding
*/
void Application_SetFindingAndBinding_Target(UInt8 endpoint, Bool enable)
{
    if (!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Device is not in network",0);
        return;
    }

    GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: %c%c%cabling EP:%u Finding & Binding(Target)...",0,
        enable ? '|' : 'D',
        enable ? 'E' : 'i',
        enable ? 'n' : 's',
        endpoint);

    switch(endpoint)
    {
        case ZIGBEE_ENDPOINT_1:
        {
            enable ? appsEp_1.FindingAndBinding() : appsEp_1.SetIdentifyMode(0);
            break;
        }
#ifdef ZIGBEE_ENDPOINT_2
        case ZIGBEE_ENDPOINT_2:
        {
            enable ? appsEp_2.FindingAndBinding() : appsEp_2.SetIdentifyMode(0);
            break;
        }
#endif // ZIGBEE_ENDPOINT_2
        default:
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Error EP:%u does NOT exist",0,endpoint);
        }
    }
}

/** @brief Toggle Finding and Binding process(Target) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint   - Target endpoint for the operation
*/
void Application_ToggleFindingAndBinding_Target(UInt8 endpoint)
{
    bool isEndpointIdentifying;

    if (!Application_ZigBeeIsJoinedNetwork())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Device is not in network",0);
        return;
    }

    switch(endpoint)
    {
        case ZIGBEE_ENDPOINT_1:
        {
            isEndpointIdentifying = appsEp_1.GetIdentifyMode()? true : false;

            Application_SetFindingAndBinding_Target(ZIGBEE_ENDPOINT_1, !isEndpointIdentifying);
            break;
        }
#ifdef ZIGBEE_ENDPOINT_2
        case ZIGBEE_ENDPOINT_2:
        {
            isEndpointIdentifying = appsEp_2.GetIdentifyMode()? true : false;

            Application_SetFindingAndBinding_Target(ZIGBEE_ENDPOINT_2, !isEndpointIdentifying);
            break;
        }
#endif // ZIGBEE_ENDPOINT_2
        default:
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Error, EP:%u does NOT exist",0,endpoint);
        }
    }
}

#endif //GP_APP_DIVERSITY_FINDING_AND_BINDING_TARGET
#ifdef GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
/** @brief Start Finding and Binding process(Initiator) on an endpoint
*    note: Please extend if required to support more endpoints for your application
*
*   @param endpoint   - initiator endpoint for the operation
*/
void Application_StartFindingAndBinding_Initiator(UInt8 endpoint)
{
    GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Start Finding & Binding(Initiator) on EP:%u...",0,endpoint);

    switch(endpoint)
    {
        case ZIGBEE_ENDPOINT_1:
        {
            appsEp_1.FindingAndBindingInitiator();
            break;
        }
#ifdef ZIGBEE_ENDPOINT_2
        case ZIGBEE_ENDPOINT_2:
        {
            appsEp_2.FindingAndBindingInitiator();
            break;
        }
#endif // ZIGBEE_ENDPOINT_2
        default:
        {
            GP_LOG_SYSTEM_PRINTF("ZIGBEE F&B: Error, EP:%u does NOT exist",0,endpoint);
        }
    }
}
#endif //GP_APP_DIVERSITY_FINDING_AND_BINDING_INITIATOR
