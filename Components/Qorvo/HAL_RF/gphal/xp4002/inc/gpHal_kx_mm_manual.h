/*
 * Copyright (c) 2014-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
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

/*
* This file should contain all non-hardware dependent defines from the normal mm file.
* Most of the inf page sections that are only used in software should be described here.
*/

#ifdef GP_COMP_CHIPEMU
extern UInt8 ram_emu[];
extern UInt8 sysram_emu[];
extern UInt8 ak_emu[]; //Akuma WISHB + APB + RAM
extern UInt8 sysgpm_emu[];

#define TO_RAM_EMU(a)   (&ram_emu[a - GP_MM_APPUC_RAM_START])
#define FROM_RAM_EMU(a) ((UInt32)(((UInt32)a) + GP_MM_APPUC_RAM_START - ((UInt32)&ram_emu[0])))

#define TO_SYSRAM_EMU(a)   (&sysram_emu[a - GP_MM_SYSRAM_START])
#define FROM_SYSRAM_EMU(a) ((UInt32)(((UInt32)a) + GP_MM_SYSRAM_START - ((UInt32)&sysram_emu[0])))

#define TO_AKRAM_EMU(a)   (&ak_emu[a - GP_MM_AKUMA_START])
#define FROM_AKRAM_EMU(a) ((UInt32)(((UInt32)a) + GP_MM_AKUMA_START - ((UInt32)&ak_emu[0])))
#else
#define TO_RAM_EMU(a)   (a)
#define FROM_RAM_EMU(a) (a)

#define TO_SYSRAM_EMU(a)   (a)
#define FROM_SYSRAM_EMU(a) (a)

#define TO_AKRAM_EMU(a)   (a)
#define FROM_AKRAM_EMU(a) (a)
#endif // GP_COMP_CHIPEMU

#define GP_MM_RAM_LINEAR_START (GP_MM_AKUMA_RAM_LINEAR_START)
