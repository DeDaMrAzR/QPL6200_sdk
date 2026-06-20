/*
 * Copyright (c) 2009-2010, 2012-2013, 2015, GreenPeak Technologies
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Check enumrations compatibility
 *****************************************************************************/
#include "gpMacCore.h"

#define CHECK_ENUM_EQUAL(a,b) GP_COMPILE_TIME_VERIFY(((int) (a)) == ((int) (b)))
//MAC results
CHECK_ENUM_EQUAL(gpHal_ResultSuccess              , gpMacCore_ResultSuccess);
CHECK_ENUM_EQUAL(gpHal_ResultInvalidHandle        , gpMacCore_ResultInvalidHandle);
CHECK_ENUM_EQUAL(gpHal_ResultCCAFailure           , gpMacCore_ResultChannelAccessFailure);
CHECK_ENUM_EQUAL(gpHal_ResultNoAck                , gpMacCore_ResultNoAck);
