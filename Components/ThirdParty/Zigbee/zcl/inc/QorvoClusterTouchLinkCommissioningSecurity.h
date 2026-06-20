/*
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
 * $Header: $
 * $Change: $
 * $DateTime: $
 * 
 */

#ifndef _QORVO_CLUSTER_TOUCHLINKCOMMISSIONING_SEC_H_
#define _QORVO_CLUSTER_TOUCHLINKCOMMISSIONING_SEC_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterTouchLinkCommissioningSecurity

class CQorvoClusterTouchLinkCommissioningSecurity
{
    // Construction
	public:
		CQorvoClusterTouchLinkCommissioningSecurity();
        
    public:
        // Touchlink Master key
        static const unsigned char abMasterKey[16];
        //MTK
        // Touchlink PreCertification key
        static const unsigned char abPreCertKey[16];
        //MTK
        // Touchlink Development key
        static const unsigned char abDevKey[16];
        
};
#endif // _QORVO_CLUSTER_TOUCHLINKCOMMISSIONING_SEC_H_

