/*
 * Copyright (c) 2024, Qorvo Inc
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

/** @file "DeviceType_LUT.c"
 *
 * Device Type LUT of the device types according to Device Type Library Specification - 19-02016 Revision 8
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "QorvoDeviceType_LUT.h"

devType_t* DeviceType_LUT_getDeviceTypebyId(UInt16 deviceTypeId)
{
    for(uint8_t i=0 ; i<sizeof(deviceTypeLookUpTable)/sizeof(devType_t) ; i++)
    {
        if(deviceTypeLookUpTable[i].devId == deviceTypeId)
        {
            if (deviceTypeLookUpTable[i].mandatoryServerClusterSize == 0)
            {
                return NULL;            
            }
            else
            {
                return (devType_t*)&deviceTypeLookUpTable[i];
            }
            
        }
    }
    return NULL;
}