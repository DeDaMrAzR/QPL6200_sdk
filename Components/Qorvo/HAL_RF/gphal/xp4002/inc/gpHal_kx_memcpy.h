/*
 * Copyright (c) 2018-2022, Qorvo Inc
 *
 * memcpy_* copied from gpHub/loa/pre_silicon_sw_dev/ip/akuma/sw/cm0p/cm0p_hal.h,
 * to be overridden by gpChipEmu, which needs to know
 * the distinction between in-memory pointers and target
 * address space pointers.
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

#ifndef _GPHAL_KX_MEMCPY_H_
#define _GPHAL_KX_MEMCPY_H_

#ifdef GP_COMP_CHIPEMU
#include "gpChipEmu_hal_memcpy.h"
#else

#include "global.h"

ALWAYS_INLINE void hal_memcpy(UInt8* dst, const UInt8* src, UInt32 size, UInt32 align)
{
    NOT_USED(align);
    memcpy(dst, src, size);
}

ALWAYS_INLINE void hal_memcpy_ptr2wb(UIntPtr dst, const UInt8* src, UInt32 size, UInt32 align)
{
    hal_memcpy((UInt8*)dst, src, size, align);
}

ALWAYS_INLINE void hal_memcpy_wb2ptr(UInt8 *dst, UIntPtr src, UInt32 size, UInt32 align)
{
    hal_memcpy(dst, (UInt8*)src, size, align);
}

ALWAYS_INLINE void hal_memcpy_wb2wb(UIntPtr dst, UIntPtr src, UInt32 size, UInt32 align)
{
    hal_memcpy((UInt8*)dst, (UInt8*)src, size, align);
}

ALWAYS_INLINE int hal_memcmp(UInt8* s1, const UInt8* s2, UInt32 size, UInt32 align)
{
    NOT_USED(align);
    return memcmp(s1, s2, size);
}

ALWAYS_INLINE int hal_memcmp_ptr2wb(UIntPtr s1, const UInt8* s2, UInt32 size, UInt32 align)
{
    return hal_memcmp((UInt8*)s1, s2, size, align);
}

ALWAYS_INLINE int hal_memcmp_wb2ptr(UInt8* s1, const UIntPtr s2, UInt32 size, UInt32 align)
{
    return hal_memcmp(s1, (UInt8*)s2, size, align);
}

ALWAYS_INLINE int hal_memcmp_wb2wb(UIntPtr s1, const UIntPtr s2, UInt32 size, UInt32 align)
{
    return hal_memcmp((UInt8*)s1, (UInt8*)s2, size, align);
}

#endif //GP_COMP_CHIPEMU

#endif //_GPHAL_KX_MEMCPY_H_
