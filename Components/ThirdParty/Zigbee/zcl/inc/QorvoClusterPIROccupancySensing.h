/*
 * Copyright (c) 2024, Qorvo Inc
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
 
#ifndef _QORVO_CLUSTER_OCCUPANCY_SENSING_H_
#define _QORVO_CLUSTER_OCCUPANCY_SENSING_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterPIROccupancySensing

class CQorvoClusterPIROccupancySensing : public CZigBeeClusterOccupancySensing
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()

    // Construction
    public:
        CQorvoClusterPIROccupancySensing(CZigBeeFoundationApplication &application,
            const unsigned short nOccupiedToUnoccupiedDelay = 0x0000,
            const unsigned short nUnoccupiedToOccupiedDelay = 0x0000,
            const unsigned char nUnoccupiedToOccupiedThreshold = 0x01);

    // Attributes
    public:
        // Attribute storage for attribute 0x0x0010
        unsigned short m_uOccupiedToUnoccupiedDelay;

        // Attribute storage for attribute 0x0x0011
        unsigned short m_uUnoccupiedToOccupiedDelay;

        // Attribute storage for attribute 0x0x0012
        unsigned char m_nUnoccupiedToOccupiedThreshold;

    // Overrides
    public:
        // Called after power-up. Evaluates the configuration settings
        // to set current attributes and stored attributes appropriately
        virtual void AfterPowerup();
};

#define __COMPACT7B_OCCUPANCYSENSING_PIR_OCCUPIED_TO_UNOCCUPIED_DELAY(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
        CZigBeeClusterOccupancySensing::attributePIROccupiedToUnoccupiedDelay, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::persistent, member)

#define __COMPACT7B_OCCUPANCYSENSING_PIR_UNOCCUPIED_TO_OCCUPIED_DELAY(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
        CZigBeeClusterOccupancySensing::attributePIRUnoccupiedToOccupiedDelay, \
        CZigBeeAttribute::typeUnsigned16, 0, \
        CZigBeeAttribute::persistent, member)

#define __COMPACT7B_OCCUPANCYSENSING_PIR_UNOCCUPIED_TO_OCCUPIED_THRESHOLD(cls, member) \
    __COMPACT7B_ATTRIBUTE_CLUSTER(cls, \
        CZigBeeClusterOccupancySensing::attributePIRUnoccupiedToOccupiedThreshold, \
        CZigBeeAttribute::typeUnsigned8, 0, \
        CZigBeeAttribute::persistent, member)

#endif // _QORVO_CLUSTER_OCCUPANCY_SENSING_H_
