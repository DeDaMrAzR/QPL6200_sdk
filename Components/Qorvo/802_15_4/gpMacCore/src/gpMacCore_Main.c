/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
 *
 * gpMacCore_Main.c
 *   This file contains all files of the MAC layer.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

#ifdef GP_DIVERSITY_JUMPTABLES
#define GP_DIVERSITY_ROM_CODE
#endif //def GP_DIVERSITY_JUMPTABLES

#include "gpMacCore.h"
#include "gpMacCore_defs.h"
#include "gpMacCore_defs_Main.h"

#ifndef GP_DIVERSITY_JUMPTABLES
#include "gpMacCore_Attributes.c"

#if defined(GP_MACCORE_DIVERSITY_DIAGCNTRS)
#include "gpMacCore_DiagCntrs.c"
#endif //(GP_MACCORE_DIVERSITY_DIAGCNTRS)


#endif //def GP_DIVERSITY_JUMPTABLES

#ifdef GP_DIVERSITY_ROM_CODE

#include "gpMacCore_RomCode_Helpers.c"
#include "gpMacCore_RomCode_Reset.c"
#include "gpMacCore_RomCode.c"

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR)
#include "gpMacCore_RomCode_Association_Originator.c"
#endif // defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR)

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT)
#include "gpMacCore_RomCode_Association_Recipient.c"
#endif // defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT)

#if defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION)
#include "gpMacCore_RomCode_IndTx.c"
#include "gpMacCore_RomCode_DataPending.c"
#endif // defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION)

#if defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)
#include "gpMacCore_RomCode_Poll_Originator.c"
#endif // defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)

#if    defined(GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_RECIPIENT)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT)
#include "gpMacCore_RomCode_Scan.c"
#endif //

#if defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED)
#include "gpMacCore_RomCode_Security.c"
#endif //(GP_MACCORE_DIVERSITY_SECURITY_ENABLED)

#if defined(GP_MACCORE_DIVERSITY_RAW_FRAMES)
#include "gpMacCore_RomCode_Raw.c"
#endif //GP_MACCORE_DIVERSITY_RAW_FRAMES

#else //GP_DIVERSITY_ROM_CODE

#include "gpMacCore_Helpers.c"
#include "gpMacCore_Reset.c"
#include "gpMacCore.c"

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR)
#include "gpMacCore_Association_Originator.c"
#endif // defined(GP_MACCORE_DIVERSITY_ASSOCIATION_ORIGINATOR)

#if defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT)
#include "gpMacCore_Association_Recipient.c"
#endif // defined(GP_MACCORE_DIVERSITY_ASSOCIATION_RECIPIENT)

#if defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION)
#include "gpMacCore_IndTx.c"
#include "gpMacCore_DataPending.c"
#endif // defined(GP_MACCORE_DIVERSITY_INDIRECT_TRANSMISSION)

#if defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)
#include "gpMacCore_Poll_Originator.c"
#endif // defined(GP_MACCORE_DIVERSITY_POLL_ORIGINATOR)

#if    defined(GP_MACCORE_DIVERSITY_SCAN_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ED_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_ORIGINATOR)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_RECIPIENT)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ACTIVE_RECIPIENT)\
    || defined(GP_MACCORE_DIVERSITY_SCAN_ORPHAN_RECIPIENT)
#include "gpMacCore_Scan.c"
#endif //

#if defined(GP_MACCORE_DIVERSITY_SECURITY_ENABLED)
#include "gpMacCore_Security.c"
#endif //(GP_MACCORE_DIVERSITY_SECURITY_ENABLED)


#if defined(GP_MACCORE_DIVERSITY_RAW_FRAMES)
#include "gpMacCore_Raw.c"
#endif //GP_MACCORE_DIVERSITY_RAW_FRAMES

#endif //def GP_DIVERSITY_ROM_CODE

