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
 * $Header: $
 * $Change: $
 * $DateTime: $
 * 
 */

#include <climits>
#include <string>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeClusterLibrary.h"
#include "ZigBeeClusterScenes.h"

#include "QorvoClusterScenes.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO
#include "gpLog.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenes

CQorvoClusterScenes::CQorvoClusterScenes
    (CZigBeeFoundationApplication &application) :
CZigBeeClusterScenes(application)
{
}

void CQorvoClusterScenes::OnAddScene(const CZigBeeAddress &destination,
    const CZigBeeAddress &source,
    const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
    const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    ASSERT((header.GetCommand() == commandAddScene) ||
        (header.GetCommand() == commandEnhancedAddScene));

    // Check only if extension field sets exist
    if (asdu.GetSize() > 6)
    {
        unsigned short wClusterID;
        unsigned char nExtLength;
        size_t nOffset = 5; // Scene Name
        nOffset += 1 + *static_cast<const unsigned char *>(asdu.GetDataAt(nOffset));

        while (asdu.GetSize() > nOffset)
        {
            wClusterID = *static_cast<const unsigned short *>(asdu.GetDataAt(nOffset));
            nOffset += 2;
            nExtLength = *static_cast<const unsigned char *>(asdu.GetDataAt(nOffset));
            nOffset += 1 + nExtLength;

            if (asdu.GetSize() < nOffset)
            {
                TransmitDefaultResponse(destination, source, header,
                    statusMalformedCommand);

                return;
            }
            else if (!m_application.FindCluster(wClusterID))
            {
                unsigned short wGroupID = *static_cast<const unsigned short *>(asdu.GetDataAt(0));
                unsigned char nSceneID = *static_cast<const unsigned char *>(asdu.GetDataAt(2));

                _TRACE1(c7b::zcl | 1,"ZCL: scene, Cluster 0x%x does not exist\r\n", wClusterID);

                TransmitResponse(destination, source, header,
                    header.GetCommand() == commandEnhancedAddScene ?
                    commandEnhancedAddSceneResponse : commandAddSceneResponse,
                    wGroupID, nSceneID, statusInvalidField);   

                return;
            }
        }
    }

    CZigBeeClusterScenes::OnAddScene(destination, source, header, 
        asdu, nLinkQuality, nTimestamp);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenes command map

__COMPACT7B_IMPLEMENT_COMMAND_MAP(CQorvoClusterScenes, CZigBeeClusterScenes)

__COMPACT7B_BEGIN_COMMAND_MAP(CQorvoClusterScenes)
	__COMPACT7B_COMMAND_C2S(CZigBeeClusterScenes::commandAddScene,
		&CQorvoClusterScenes::OnAddScene)
	__COMPACT7B_COMMAND_C2S(CZigBeeClusterScenes::commandEnhancedAddScene,
		&CQorvoClusterScenes::OnAddScene)
__COMPACT7B_END_COMMAND_MAP()
