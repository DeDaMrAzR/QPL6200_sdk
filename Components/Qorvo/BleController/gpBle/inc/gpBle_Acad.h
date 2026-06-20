/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpHal_DEFS.h
 *
 *  This file contains internal definitions of the GPHAL.
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
 * $Header$
 * $Change$
 * $DateTime$
 *
 */

#ifndef _GPBLE_ACAD_H
#define _GPBLE_ACAD_H

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GPBLE_ACAD_HEADER_LENGTH                        0x02

// Bluetooth assigned numbers document contains acad data type identifiers
#define GPBLE_ACAD_DATA_TYPE_CHANNEL_MAP_UPDATE_IND     0x28
#define GPBLE_ACAD_DATA_TYPE_BIGINFO                    0x2C

// Core spec supplement v11 | Part A contains ACAD data format and length
// Channel map (5 octets) | Instant (2 octets)
#define GPBLE_ACAD_PAYLOAD_LENGTH_CHANNEL_MAP_UPDATE_IND    0x07
// BigInfo: Core spec | Vol 6, Part B, section 4.4.6.11
#define GPBLE_ACAD_PAYLOAD_LENGTH_BIGINFO_UNENCRYPTED       0x21

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

// Length of the payload + the data type
#define GPBLE_ACAD_GET_DATA_LENGTH(payloadLength)           ((payloadLength) + 1)
// Total length (payload + data type + length byte)
#define GPBLE_ACAD_GET_TOTAL_LENGTH(payloadLength)          ((payloadLength) + 2)

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/


#endif //_GPBLE_ACAD_H
