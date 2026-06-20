/**
 * @file libs/base/include/debug.h
 * @brief Debug definitions (PRINTFs etc)
 * @copyright Copyright (c) 2016-2018 Silex Insight. All Rights reserved
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdbool.h>
#include "esec_features.h"
#include "esec_map.h"
#include "fatal.h"

#if DEBUG_ENABLED

#ifdef GP_COMP_UNIT_TEST
#include "stdio.h"
#define SX_PRINTF(...) printf(__VA_ARGS__)
#else
#define SX_PRINTF(...) myPrintf(__VA_ARGS__)
#endif

/**
 * Print an array byte per byte
 * @param  addr Start address of the array
 * @param  size Size of the array (in bytes)
 */
#define SX_PRINT_ARRAY(addr, size)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t i = 0;                                                                                                \
        uint8_t* ptr = (uint8_t*)(addr);                                                                               \
        for(i = 0; i < (size); i++)                                                                                    \
            SX_PRINTF("%02x", ptr[i]);                                                                                 \
        SX_PRINTF("\n");                                                                                               \
    } while(0)

/**
 * Print an array byte per byte in reverse order (MSB to LSB)
 * @param  addr Start address of the array
 * @param  size Size of the array (in bytes)
 */
#define SX_PRINT_ARRAY_REVERSE(addr, size)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t i = 0;                                                                                                \
        uint8_t* ptr = (uint8_t*)(addr);                                                                               \
        for(i = 0; i < (size); i++)                                                                                    \
            SX_PRINTF("%02x", ptr[size - i - 1]);                                                                      \
        SX_PRINTF("\n");                                                                                               \
    } while(0)

/**
 * Print an array word by word
 * @param  addr Start address of the array
 * @param  size Size of the array (in bytes)
 */
#define SX_PRINT_ARRAY_32(addr, size)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        uint32_t i = 0;                                                                                                \
        uint32_t* ptr = (uint32_t*)(addr);                                                                             \
        for(i = 0; i < (size) / 4; i++)                                                                                \
            SX_PRINTF("%08x", ptr[i]);                                                                                 \
        SX_PRINTF("\n");                                                                                               \
    } while(0)

/**
 * Print a block_t (array)
 * @param  blk array to print
 */
#define SX_PRINT_BLK(blk)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        const char* s = ((blk.flags & BLOCK_S_CONST_ADDR) ? " continuous" : "incremental");                            \
        SX_PRINTF("%s@(0x%08x | %d | %s) ", #blk, blk.addr, blk.len, s);                                               \
        if(!(blk.flags & BLOCK_S_CONST_ADDR))                                                                          \
            SX_PRINT_ARRAY(blk.addr, blk.len);                                                                         \
        else                                                                                                           \
            SX_PRINTF("\n");                                                                                           \
    } while(0)

#else

#define SX_PRINTF(...)                                                                                                 \
    {                                                                                                                  \
    }
#define SX_PRINT_ARRAY(addr, size)                                                                                     \
    {                                                                                                                  \
    }
#define SX_PRINT_BLK(blk)                                                                                              \
    {                                                                                                                  \
    }
#define SX_PRINT_ARRAY_32(addr, size)                                                                                  \
    {                                                                                                                  \
    }

#endif

/**
 * printf custom implementation (lightweight)
 * @param  format  Describes the format of the output
 * @param  VARARGS arguments to replace in the format string
 * @return         0 if success
 */
void myPrintf(const char* format, ...);

/**
 * Throw an error in console and raise a tamper if condition cond is not met.
 * @param  cond Condition of the assertion (must always be true in theory)
 * @param  msg  Message to display in the error message (a simple string, no formating)
 */
void sx_assert_int(bool cond, char* msg);
#define SX_ASSERT(cond, msg) sx_assert_int(cond, msg)

/**
 * Throw an error in console if condition cond is not met but doesn't requires a message
 * @param  cond Condition of the assertion (must always be true in theory)
 */

void sx_assert_nm_int(bool cond, char* file, int line);
#define SX_ASSERT_NM(cond, file) sx_assert_nm_int(cond, file, __LINE__)

/**
 * Throw an error in console and raise a tamper if condition cond is not met.
 * @param  cond Condition of the assertion (must always be true in theory)
 * @param  variadic parameters
 */
#define SX_ASSERT_VAR(cond, ...)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        if(!(cond))                                                                                                    \
        {                                                                                                              \
            SX_PRINTF("Assert in %s (%d) ", __FUNCTION__, __LINE__);                                                   \
            SX_PRINTF(__VA_ARGS__);                                                                                    \
            SX_PRINTF("\n");                                                                                           \
            fatal_error(TAMPER_SRC_ASSERT);                                                                            \
        }                                                                                                              \
    } while(0)

#if DEBUG_ENABLED
#define SX_DEV_ASSERT(cond, msg)     SX_ASSERT(cond, msg)
#define SX_DEV_ASSERT_NM(cond, file) SX_ASSERT_NM(cond, file)
#else
#define SX_DEV_ASSERT(cond, msg)
#define SX_DEV_ASSERT_NM(cond, file)
#endif

#if defined(SEUC_MPW_FEATURES)
#if defined(SEUC_TARGET_BOOT_FALLBACK) && !defined(SEUC_LOCAL_DEBUG_ENABLE)
#define DBG_PRINTF(...)
#define DBG_PRINT_ARRAY(addr, size)
#else // => not defined(SEUC_TARGET_BOOT_FALLBACK)
#define DBG_PRINTF(...)             SX_PRINTF(__VA_ARGS__)
#define DBG_PRINT_ARRAY(addr, size) SX_PRINT_ARRAY(addr, size)
#endif // defined(SEUC_TARGET_BOOT_FALLBACK)
#else  // => not defined(SEUC_MPW_FEATURES)
#define DBG_PRINTF(...)
#define DBG_PRINT_ARRAY(addr, size)
#endif // defined(SEUC_MPW_FEATURES)

#endif
