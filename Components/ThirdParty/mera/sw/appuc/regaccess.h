#ifndef _REGACCESS_H
#define _REGACCESS_H

#define GP_WB_ADDRESS(addr)         ((uintptr_t) ((UInt32) addr))


/* base code for accessing properties */

/* Related to gcc memory barrier */
// https://stackoverflow.com/questions/19965076/gcc-memory-barrier-sync-synchronize-vs-asm-volatile-memory
//
// asm volatile ("" : : : "memory"); actually does nothing at runtime, there's no command performed
// there and the CPU doesn't know about it. it only serves at compile time, to tell the compiler not
// to move loads or stores beyond this point (in any direction) as part of its optimizations. It's called a SW barrier.

//write
ALWAYS_INLINE void GP_WB_WRITE_U8(uintptr_t address, UInt8 val)
{
    *((volatile UInt8 *) GP_WB_ADDRESS(address)) = val;
}
ALWAYS_INLINE void GP_WB_WRITE_U16(uintptr_t address, UInt16 val)
{
    *((volatile UInt16 *) GP_WB_ADDRESS(address)) = val;
}
ALWAYS_INLINE void GP_WB_WRITE_U24(uintptr_t address, UInt32 val)
{
    *((volatile UInt16 *) GP_WB_ADDRESS(address))   = (UInt16) val;
    *((volatile UInt8 *)  GP_WB_ADDRESS(address+2)) = (UInt8) (val >> 16);
}
ALWAYS_INLINE void GP_WB_WRITE_U32(uintptr_t address, UInt32 val)
{
    *((volatile UInt32 *) GP_WB_ADDRESS(address)) = val;
}
ALWAYS_INLINE void GP_WB_WRITE_U40(uintptr_t address, UInt64 val)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt32 msb;
        } s32;
        UInt64 u64;
    } u;
    u.u64=val;
    *((volatile UInt32 *) GP_WB_ADDRESS(address))  = u.s32.lsb;
    //GP_ASSERT(u.s16.msb < 0x100);
    *((volatile UInt8 *) GP_WB_ADDRESS(address+4)) = (UInt8) u.s32.msb;
}
ALWAYS_INLINE void GP_WB_WRITE_U48(uintptr_t address, UInt64 val)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt32 msb;
        } s32;
        UInt64 u64;
    } u;
    u.u64=val;
    *((volatile UInt32 *) GP_WB_ADDRESS(address))   = u.s32.lsb;
    //GP_ASSERT(u.s16.msb < 0x100);
    *((volatile UInt16 *) GP_WB_ADDRESS(address+4)) = (UInt16) u.s32.msb;
}
ALWAYS_INLINE void GP_WB_WRITE_U56(uintptr_t address, UInt64 val)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt16 msb0;
            UInt16 msb1;
        } s32;
        UInt64 u64;
    } u;
    u.u64=val;
    *((volatile UInt32 *) GP_WB_ADDRESS(address))   = u.s32.lsb;
    *((volatile UInt16 *) GP_WB_ADDRESS(address+4)) = (UInt16) u.s32.msb0;
    *((volatile UInt8 *)  GP_WB_ADDRESS(address+6)) = (UInt8) u.s32.msb1;
}
ALWAYS_INLINE void GP_WB_WRITE_U64(uintptr_t address, UInt64 val)
{
    *((volatile UInt64 *) GP_WB_ADDRESS(address)) = val;
}

//read
ALWAYS_INLINE UInt8 GP_WB_READ_U8(uintptr_t address)
{
    UInt8 ret;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    ret = *((volatile UInt8 *) GP_WB_ADDRESS(address));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return ret;
}
ALWAYS_INLINE UInt16 GP_WB_READ_U16(uintptr_t address)
{
    UInt16 ret;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    ret = *((volatile UInt16 *) GP_WB_ADDRESS(address));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return ret;
}
ALWAYS_INLINE UInt32 GP_WB_READ_U24(uintptr_t address)
{
    UInt32 ret;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    ret = (*((volatile UInt8  *) GP_WB_ADDRESS(address+2)) << 16) | *((volatile UInt16 *) GP_WB_ADDRESS(address));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return ret;
}
ALWAYS_INLINE UInt32 GP_WB_READ_U32(uintptr_t address)
{
    UInt32 ret;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    ret = *((volatile UInt32 *) GP_WB_ADDRESS(address));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return ret;
}

ALWAYS_INLINE UInt64 GP_WB_READ_U40(uintptr_t address)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt32 msb;
        } s32;
        UInt64 u64;
    } u;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    u.s32.lsb =  *((volatile UInt32 *) GP_WB_ADDRESS(address));
    u.s32.msb =  *((volatile UInt8 *)  GP_WB_ADDRESS(address+4));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return u.u64;
}
ALWAYS_INLINE UInt64 GP_WB_READ_U48(uintptr_t address)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt32 msb;
        } s32;
        UInt64 u64;
    } u;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    u.s32.lsb =  *((volatile UInt32 *) GP_WB_ADDRESS(address));
    u.s32.msb =  *((volatile UInt16 *) GP_WB_ADDRESS(address+4));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return u.u64;
}
ALWAYS_INLINE UInt64 GP_WB_READ_U56(uintptr_t address)
{
    union
    {
        struct
        {
            UInt32 lsb;
            UInt16 msb0;
            UInt16 msb1;
        } s32;
        UInt64 u64;
    } u;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    u.s32.lsb =  *((volatile UInt32 *) GP_WB_ADDRESS(address));
    u.s32.msb0 = *((volatile UInt16 *) GP_WB_ADDRESS(address+4));
    u.s32.msb1 = *((volatile UInt8 *)  GP_WB_ADDRESS(address+6));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return u.u64;
}
ALWAYS_INLINE UInt64 GP_WB_READ_U64(uintptr_t address)
{
    UInt64 ret;
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    ret = *((volatile UInt64 *) GP_WB_ADDRESS(address));
    asm volatile ("" : : : "memory"); //memory barrier so gcc don't optimize it away
    return ret;
}


ALWAYS_INLINE void GP_WB_MWRITE_U8(uintptr_t address, UInt8 mask, UInt8 val)
{
    UInt8 org = GP_WB_READ_U8(address);
    UInt8 tmp = (org & ~mask) | (val & mask);

    //just write if different, save some access
    if (org != tmp)
    {
        GP_WB_WRITE_U8(address, tmp);
    }
}

ALWAYS_INLINE void GP_WB_MWRITE_U16(uintptr_t address, UInt16 mask, UInt16 val)
{
    UInt16 org = GP_WB_READ_U16(address);
    UInt16 tmp = (org & ~mask) | (val & mask);

    //just write if different, save some access
    if (org != tmp)
    {
        GP_WB_WRITE_U16(address, tmp);
    }
}

ALWAYS_INLINE void GP_WB_MWRITE_U32(uintptr_t address, UInt32 mask, UInt32 val)
{
    UInt32 org = GP_WB_READ_U32(address);
    UInt32 tmp = (org & ~mask) | (val & mask);

    //just write if different, save some access
    if (org != tmp)
    {
        GP_WB_WRITE_U32(address, tmp);
    }
}

#define GP_WB_READ_SER_U8(address)      GP_WB_READ_U8(address)
#define GP_WB_READ_SER_U16(address)     GP_WB_READ_U16(address)
#define GP_WB_READ_SER_U32(address)     GP_WB_READ_U32(address)

//range checks
#define GP_WB_RANGE_CHECK(val, max) do          \
{                                               \
    COMPILE_TIME_ASSERT((val) <= (max));        \
} while (false)

#define GP_WB_RANGE_CHECK_SIGNED(val, min, max) do  \
    {                                               \
        COMPILE_TIME_ASSERT((val) >= (min));        \
        COMPILE_TIME_ASSERT((val) <= (max));        \
    } while (false)

#define GP_WB_SIZE_CHECK(type, var) do                                      \
{                                                                           \
    type GP_WB_SIZE_CHECK_dummy;                                            \
    Bool GP_WB_SIZE_CHECK_result = (&GP_WB_SIZE_CHECK_dummy == &(var));     \
    NOT_USED(GP_WB_SIZE_CHECK_result);                                      \
} while (false)

#define GP_WB_SIZE_CHECK_U8(var)  GP_WB_SIZE_CHECK(UInt8, var)
#define GP_WB_SIZE_CHECK_U16(var) GP_WB_SIZE_CHECK(UInt16, var)
#define GP_WB_SIZE_CHECK_U24(var) GP_WB_SIZE_CHECK(UInt32, var)
#define GP_WB_SIZE_CHECK_U32(var) GP_WB_SIZE_CHECK(UInt32, var)
#define GP_WB_SIZE_CHECK_U64(var) GP_WB_SIZE_CHECK(UInt64, var)

/* sign extension/casting macros: extends the sign bit for len-bit
   numbers and signed casting */
#define GP_WB_S8(val, len)  (((Int8 )(val<<( 8-len)))>>( 8-len))
#define GP_WB_S16(val, len) (((Int16)(val<<(16-len)))>>(16-len))
#define GP_WB_S32(val, len) (((Int32)(val<<(32-len)))>>(32-len))

/*
 * helper functions for bytestream
 */

ALWAYS_INLINE void VOLATILE_MEMCPY(void * dst, const void * src, UInt16 len)
{
#if defined ( __IAR_SYSTEMS_ICC__ ) /*!< IAR Compiler */
    memcpy(dst, src, len);
#else
    __builtin_memcpy(dst, src, len);
#endif
}

ALWAYS_INLINE void BLOCKREAD(uintptr_t address, void * pBuffer, UInt16 len)
{
    VOLATILE_MEMCPY(pBuffer, (void *) GP_WB_ADDRESS(address), len);
}

ALWAYS_INLINE void BLOCKWRITE(uintptr_t address,
                              const void * pBuffer,
                              UInt16 len)
{
    VOLATILE_MEMCPY((void *) GP_WB_ADDRESS(address), pBuffer, len);
}

//helper macro
#define WRITE_N_BYTES(address, pData, type)                             \
    * ((volatile type *) (address)) = *((type *) (unsigned char *) (pData))


/*
 * if the size is known and "natural", use optimised code,
 * otherwise the blockwrite code.
 *
 * if i put this in an inline function, __builtin_constant_p
 * always return false, so we have to use a macro here.
 * no exact type checking, to avoid warnings for not-taken branches,
 * and because pBuffer migth be a pointer to unsigned char or so.
 * using __builting_choose_expr did not work,
 * since it is for expressions, not statements.
 */
#define GP_WB_WRITE_BYTE_STREAM(_address, pBuffer, len)     \
    do                                                      \
    {                                                       \
        uintptr address = GP_WB_ADDRESS(_address);          \
        if (__builtin_constant_p(len))                      \
        {                                                   \
            if (len == 2)                                   \
            {                                               \
                WRITE_N_BYTES(address, pBuffer, UInt16);    \
            }                                               \
            else if (len == 4)                              \
            {                                               \
                WRITE_N_BYTES(address, pBuffer, UInt32);    \
            }                                               \
            else if (len == 8)                              \
            {                                               \
                WRITE_N_BYTES(address, pBuffer, UInt64);    \
            }                                               \
            else                                            \
            {                                               \
                BLOCKWRITE(address, pBuffer, len);          \
            }                                               \
        }                                                   \
        else                                                \
        {                                                   \
            BLOCKWRITE(address, pBuffer, len);              \
        }                                                   \
    } while (false)

//helper macro
#define READ_N_BYTES(address, pData, type) \
    *((type *) (unsigned char *) (pData)) = * ((volatile type *) (address))


/*
 * if the size is known and "natural", use optimised code,
 * otherwise the blockread code.
 */
#define GP_WB_READ_BYTE_STREAM(_address, pBuffer, len)  \
    do                                                  \
    {                                                   \
        uintptr address = GP_WB_ADDRESS(_address);      \
        if (__builtin_constant_p(len))                  \
        {                                               \
            if (len == 2)                               \
            {                                           \
                READ_N_BYTES(address, pBuffer, UInt16); \
            }                                           \
            else if (len == 4)                          \
            {                                           \
                READ_N_BYTES(address, pBuffer, UInt32); \
            }                                           \
            else if (len == 8)                          \
            {                                           \
                READ_N_BYTES(address, pBuffer, UInt64); \
            }                                           \
            else                                        \
            {                                           \
                BLOCKREAD(address, pBuffer, len);       \
            }                                           \
        }                                               \
        else                                            \
        {                                               \
            BLOCKREAD(address, pBuffer, len);           \
        }                                               \
    } while (false)



#endif
