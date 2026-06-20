/*
 * Copyright (c) 2022, Qorvo Inc
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

#ifndef _GPHAL_KX_MSI_BASIC_H_
#define _GPHAL_KX_MSI_BASIC_H_

#ifndef GP_COMP_CHIPEMU
#include "gp_kx.h" // pull in CMSIS core_cm*.h from HAL_PLATFORM, needed by regmap.h from digital repos
#endif

#include "gpRegMap_mm.h"

#if defined(GP_COMP_CHIPEMU)
#include "gpChipEmu_regaccess.h"
#else

#if defined(__GNUC__)
#define ALWAYS_INLINE static inline __attribute__((always_inline))
#elif defined(__ICCARM__)
#define ALWAYS_INLINE _Pragma("inline=forced")
#else
#define ALWAYS_INLINE static inline
#endif

/* base code for accessing properties */

/* Related to gcc memory barrier */
// https://stackoverflow.com/questions/19965076/gcc-memory-barrier-sync-synchronize-vs-asm-volatile-memory
//
// asm volatile ("" : : : "memory"); actually does nothing at runtime, there's no command performed
// there and the CPU doesn't know about it. it only serves at compile time, to tell the compiler not
// to move loads or stores beyond this point (in any direction) as part of its optimizations. It's called a SW barrier.

/*
 * Defines the maximum variable size, used for alignment purposes
 */
#define GP_WB_MAX_MEMBER_SIZE     8

#ifdef __cplusplus
extern "C" {
#endif

#include "regaccess.h"

//single bit access
#define GP_WB_READ_U1(_address, _bit) ((GP_WB_READ_U8(_address) >> (_bit)) & 0x01)

#define GP_WB_MWRITE_U1(_address, _bit, _val) GP_WB_MWRITE_U8(_address, (1 << _bit), (_val << _bit))

#define GP_WB_READ_SER_U1(address, bit) GP_WB_READ_U1(address, bit)
#define GP_WB_READ_SER_U8(address)      GP_WB_READ_U8(address)
#define GP_WB_READ_SER_U16(address)     GP_WB_READ_U16(address)
#define GP_WB_READ_SER_U32(address)     GP_WB_READ_U32(address)

#ifdef __cplusplus
}
#endif

#endif //defined(GP_COMP_CHIPEMU)

#endif
