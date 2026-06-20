/*
 * Copyright (c) 2025, Qorvo Inc
 *
 *   Functions to do range checking of addresses
 *
 *   This software is owned by Qorvo Inc
 *   and protected under applicable copyright laws.
 *   It is delivered under the terms of the license
 *   and is intended and supplied for use solely and
 *   exclusively with products manufactured by
 *   Qorvo Inc.
 *
 *
 *   THIS SOFTWARE IS PROVIDED IN AN "AS IS"
 *   CONDITION. NO WARRANTIES, WHETHER EXPRESS,
 *   IMPLIED OR STATUTORY, INCLUDING, BUT NOT
 *   LIMITED TO, IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *   QORVO INC. SHALL NOT, IN ANY
 *   CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 *   INCIDENTAL OR CONSEQUENTIAL DAMAGES,
 *   FOR ANY REASON WHATSOEVER.
 *
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_UTILS

#include "gpUtils.h"

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*!
 *  \brief   function to check whether region A is completely inside region B
 *
 *  \param   region_a_addr  Start address of region A
 *  \param   region_a_length Length of region A
 *  \param   region_b_addr  Start address of region B
 *  \param   region_b_length Length of region B
 *
 *  \return  false if overflow happened during the check or region A is not completely inside region B
 *           true if region A is completely inside region B
 *
 *  note: this function relies on unsigned arithemetic overflows. Make sure you're only passing unsigned values
 *
 */
Bool gpUtils_is_region_a_within_region_b(UInt32 region_a_addr, UInt32 region_a_length, UInt32 region_b_addr,
                                          UInt32 region_b_length)
{
    // Check for overflow in range A
    if((region_a_addr + region_a_length) < region_a_addr)
    {
        return false;
    }

    // Check for overflow in range B
    if((region_b_addr + region_b_length) < region_b_addr)
    {
        return false;
    }

    // Check if the check range is within the allowed range
    return (region_a_addr >= region_b_addr) && ((region_a_addr + region_a_length) <= (region_b_addr + region_b_length));
}
