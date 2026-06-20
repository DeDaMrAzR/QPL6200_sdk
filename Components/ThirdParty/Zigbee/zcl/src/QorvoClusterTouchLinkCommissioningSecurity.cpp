/*
 * Copyright (c) 2018, Qorvo Inc
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
 * $Header: $
 * $Change: $
 * $DateTime: $
 *
 */
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
//#include <stack>
#include <queue>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Synchronization.h"
#include "MemoryDevice.h"
#include "PersistentStorage.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "Compact15dot4.h"
#include "Compact15dot4Ex.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "IEEE802154MACPIB.h"
#include "IEEE802154MAC.h"
#include "IEEE802154MCF.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeCommissioningAttributes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeDeviceProfile.h"
#include "ZigBeeDeviceObject.h"
#include "ZigBeeNetworkCommands.h"
#include "ZigBeeNetwork.h"
#include "ZigBeeStorage.h"
#include "ZigBeeCommissioningAttributesEx.h"

#include "ZigBeeClusterLibrary.h"
#include "ZigBeeClusterTouchlink.h"

#include "QorvoClusterTouchLinkCommissioningSecurity.h"

/**
 * Override function to supply Product key to the Touchlink cluster implementation
*/
bool CZigBeeClusterTouchlink::GetEncryptionKey(uint8_t abKey[16],
      const uint_fast32_t nSessionID, const uint_fast32_t nResponseID,
      const uint_fast8_t nScheme)
{

    // Attempt to use an unsupported scheme? Fail
    if (!(nSupportedSchemes & (1 << nScheme)))
        return false;

    if (nScheme == schemeCertifiedProduct)
    {
        ExpandKey(abKey, nSessionID, nResponseID);
        DeriveTransportKey(abKey, CQorvoClusterTouchLinkCommissioningSecurity::abMasterKey);
        return true;
    }

    // Let the library's default implementation deal with development and
    // pre-certification keys (in a real product these would not be allowed)
    return CZigBeeClusterTouchlink::DefaultGetEncryptionKey(abKey, nSessionID,
        nResponseID, nScheme);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioning


CQorvoClusterTouchLinkCommissioningSecurity::CQorvoClusterTouchLinkCommissioningSecurity()
{
}
