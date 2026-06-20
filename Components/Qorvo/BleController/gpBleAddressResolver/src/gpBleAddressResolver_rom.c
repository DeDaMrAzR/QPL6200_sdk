/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
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

#ifdef GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_ROM_CODE
#endif //def GP_DIVERSITY_JUMPTABLES

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

//#define GP_LOCAL_LOG

#define GP_COMPONENT_ID GP_COMPONENT_ID_BLEADDRESSRESOLVER

#include "gpBle.h"
#include "gpBleComps.h"
#include "gpBle_defs.h"
#include "gpAssert.h"
#include "gpLog.h"
#include "gpHal.h"
#include "gpBleAddressResolver.h"
#include "gpBleAddressResolver_defs.h"

#ifdef GP_COMP_BLEADVERTISER
#include "gpBleAdvertiser.h"
#endif //GP_COMP_BLEADVERTISER

#if defined(GP_DIVERSITY_BLE_OBSERVER) 
#include "gpBleScanner.h"
#endif //GP_DIVERSITY_BLE_OBSERVER || GP_DIVERSITY_BLE_CENTRAL || GP_DIVERSITY_BLE_LEGACY_SCANNING

#if defined(GP_DIVERSITY_BLE_PERIPHERAL)
#include "gpBleInitiator.h"
#include "gpBleLlcp.h"
#endif //GP_DIVERSITY_BLE_CENTRAL || GP_DIVERSITY_BLE_PERIPHERAL

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define BLE_HOST_NR_OF_FILTER_ACCEPT_LIST_ENTRIES    (BLE_MAX_NR_OF_FILTER_ACCEPT_LIST_ENTRIES - NUMBER_OF_SPECIAL_ENTRIES_SUBTRACTED_BY_ROM)

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

#if defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
#include "gpBleAddressResolver_CodeJumpTableFlash_Defs_rom_c.h"
#include "gpBleAddressResolver_CodeJumpTableRom_Defs_rom_c.h"
#endif // defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)

/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_START */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_START */

//Checker/action
STATIC_FUNC gpHci_Result_t Ble_ManipulateFilterAcceptListChecker(gpHci_FilterAcceptListAddressType_t addressType);
STATIC_FUNC gpHci_Result_t Ble_ManipulateFilterAcceptListAllowedChecker(void);

/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_END */
/* JUMPTABLE_ROM_FUNCTION_DEFINITIONS_END */

/*****************************************************************************
 *                    Tmp extern Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void Ble_ClearFilterAcceptList(Bool clearAll)
{
    if(clearAll)
    {
        gpHal_BleUpdateFilterAcceptListEntryStateBulk(BLE_ADDRESSRESOLVER_VALID_STATE_MASK_ALL,
                                               BLE_ADDRESSRESOLVER_VALID_STATE_MASK_ALL,
                                               BLE_ADDRESSRESOLVER_VALID_STATE_MASK_NONE);
    }
    else
    {
        gpBle_ClearWL(gpBle_WlEntryRegular);
    }
}

gpHci_Result_t Ble_AddFilterAcceptListEntry(gpHci_FilterAcceptListAddressType_t addressType, BtDeviceAddress_t* pAddress, UInt8 stateMask, UInt8 rangeStart, UInt8 rangeStop)
{
    // deprecated! function definition must stay in place for backward compability reasons
    return gpHci_ResultCommandDisallowed;
}

gpHci_Result_t Ble_RemoveFilterAcceptListEntry(gpHci_FilterAcceptListAddressType_t addressType, BtDeviceAddress_t* pAddress, UInt8 rangeStart, UInt8 rangeStop)
{
    // deprecated! function definition must stay in place for backward compability reasons
    return gpHci_ResultCommandDisallowed;
}

gpHci_Result_t Ble_ManipulateFilterAcceptListChecker(gpHci_FilterAcceptListAddressType_t addressType)
{
    gpHci_Result_t result;

    result = Ble_ManipulateFilterAcceptListAllowedChecker();

    if(result == gpHci_ResultSuccess)
    {
        if(!BLE_FILTER_ACCEPT_LIST_ADDRESS_TYPE_VALID(addressType))
        {
            result = gpHci_ResultInvalidHCICommandParameters;
        }
    }
    return result;
}

gpHci_Result_t Ble_ManipulateFilterAcceptListAllowedChecker(void)
{
#ifdef GP_COMP_BLEADVERTISER
    if(gpBleAdvertiser_IsEnabled() && gpBleAdvertiser_IsFilterAcceptListUsed())
    {
        GP_LOG_PRINTF("WL manip not allowed during adv",0);
        // the advertising filter policy uses the white list and advertising is enabled
        return gpHci_ResultCommandDisallowed;
    }
#endif // GP_COMP_BLEADVERTISER
    return gpHci_ResultSuccess;
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

gpHci_Result_t gpBleAddressResolver_AddSpecialFilterAcceptListAddress(gpHci_FilterAcceptListAddressType_t addressType, BtDeviceAddress_t* pAddress, UInt8 stateMask)
{
    return gpHal_BleUpdateFilterAcceptListEntryState(addressType, pAddress, stateMask, stateMask, true);
}

gpHci_Result_t gpBleAddressResolver_RemoveSpecialFilterAcceptListAddress(gpHci_FilterAcceptListAddressType_t addressType, BtDeviceAddress_t* pAddress)
{
    gpHci_Result_t ret = gpHci_ResultMemoryCapacityExceeded;

    UInt8 wlId = gpHal_BleFindFilterAcceptListEntry(addressType, pAddress, 0x00, GP_HAL_BLE_MAX_NR_OF_FILTER_ACCEPT_LIST_ENTRIES-1);
    if(wlId != GP_HAL_BLE_WL_ID_INVALID)
    {
        UInt8 entryState = gpHal_BleGetFilterAcceptListEntryState(wlId);
        if(!(entryState & GPHAL_ENUM_FILTER_ACCEPT_LIST_ENTRY_TYPE_MASK))
        {
            ret = gpHal_BleUpdateFilterAcceptListEntryState(addressType, pAddress, BLE_ADDRESSRESOLVER_VALID_STATE_MASK_ALL, BLE_ADDRESSRESOLVER_VALID_STATE_MASK_NONE, false);
        }
    }

    return ret;
}

void gpBleAddressResolver_UpdateFilterAcceptListEntryState(gpHci_FilterAcceptListAddressType_t addressType, BtDeviceAddress_t* pAddress, UInt8 state, Bool set)
{
    gpHal_BleUpdateFilterAcceptListEntryState(addressType, pAddress, state, set?state:0x00, false);
}

void gpBleAddressResolver_UpdateFilterAcceptListEntryStateBulk(UInt8 state, Bool set)
{
    gpHal_BleUpdateFilterAcceptListEntryStateBulk(BLE_ADDRESSRESOLVER_VALID_STATE_MASK_ALL, state, set?state:0x00);
}

#if defined(GP_DIVERSITY_BLE_PERIPHERAL)
// Loop over all connected devices and take appropriate action for them in the filter accept list
void gpBleAddressResolver_EnableConnectedDevicesInFilterAcceptList(UInt8 state, Bool set)
{
    UIntLoop i;

    for(i = 0; i < BLE_LLCP_MAX_NR_OF_CONNECTIONS; i++)
    {
        if(Ble_LlcpIsConnectionCreated(i))
        {
            Bool peerAddressType;
            BtDeviceAddress_t peerAddress;

            Ble_LlcpGetPeerAddressInfo(i, &peerAddressType, &peerAddress);
            gpHal_BleUpdateFilterAcceptListEntryState(peerAddressType, &peerAddress, state, set?state:0x00, false);
        }
    }
}
#endif // GP_DIVERSITY_BLE_CENTRAL || GP_DIVERSITY_BLE_PERIPHERAL

/*****************************************************************************
 *                    Public Service Function Definitions
 *****************************************************************************/

gpHci_Result_t gpBle_LeReadFilterAcceptListSize(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    GP_LOG_PRINTF("BLE Read WL size",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    pEventBuf->payload.commandCompleteParams.returnParams.filterAcceptListSize = BLE_HOST_NR_OF_FILTER_ACCEPT_LIST_ENTRIES;

    return gpHci_ResultSuccess;
}

gpHci_Result_t gpBle_LeClearFilterAcceptList(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;

    GP_LOG_PRINTF("Clear WL",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_ManipulateFilterAcceptListAllowedChecker();

    if(result == gpHci_ResultSuccess)
    {
        Ble_ClearFilterAcceptList(false);
    }

    return result;
}

gpHci_Result_t gpBle_LeAddDeviceToFilterAcceptList(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;

    gpHci_LeAddDeviceToFilterAcceptListCommand_t* pAddDeviceToWlParams = &pParams->LeAddDeviceToFilterAcceptList;

    GP_LOG_PRINTF("Add device to WL",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_ManipulateFilterAcceptListChecker(pAddDeviceToWlParams->addressType);

    if(result == gpHci_ResultSuccess)
    {
        result = gpBle_AddDeviceToWL(gpBle_WlEntryRegular, pAddDeviceToWlParams->addressType, &pAddDeviceToWlParams->address);
    }

    return result;
}

gpHci_Result_t gpBle_LeRemoveDeviceFromFilterAcceptList(gpHci_CommandParameters_t* pParams, gpBle_EventBuffer_t* pEventBuf)
{
    gpHci_Result_t result;

    gpHci_LeRemoveDeviceFromFilterAcceptListCommand_t* pRemoveDeviceFromWlParams = &pParams->LeRemoveDeviceFromFilterAcceptList;

    GP_LOG_PRINTF("Remove device from WL",0);

    BLE_SET_RESPONSE_EVENT_COMMAND_COMPLETE(pEventBuf->eventCode);

    result = Ble_ManipulateFilterAcceptListChecker(pRemoveDeviceFromWlParams->addressType);

    if(result == gpHci_ResultSuccess)
    {
        result = gpBle_RemoveDeviceFromWL(gpBle_WlEntryRegular, pRemoveDeviceFromWlParams->addressType, &pRemoveDeviceFromWlParams->address);
    }

    return result;
}

