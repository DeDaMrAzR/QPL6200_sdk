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
 
#ifndef _QORVO_CLUSTER_SCENES_H_
#define _QORVO_CLUSTER_SCENES_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenes

class CQorvoClusterScenes : public CZigBeeClusterScenes
{
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // Construction
    public:
        CQorvoClusterScenes
            (CZigBeeFoundationApplication &application);

    // Implementation
    public:
        // Overridden command handler for "add scene" and "enhanced add scene"
        void OnAddScene(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

};

#endif // _QORVO_CLUSTER_SCENES_H_
