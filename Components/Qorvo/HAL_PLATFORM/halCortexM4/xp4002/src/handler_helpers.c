/*
 * Copyright (c) 2022, Qorvo Inc
 *
 *   Handler helpers
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

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

// IVT entries can be stack pointer or function pointer ==> extra mapping needed
typedef void (*intfunc)(void);
#if defined(__GNUC__)
typedef union { intfunc __fun; const void * __ptr; } intvec_elem;
#endif
#if defined(__IAR_SYSTEMS_ICC__)
typedef union { intfunc __fun; void * __ptr; } intvec_elem;
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#if defined(__GNUC__)
#define IVT_GET_STACK_POINTER() (&_estack)
#endif
#if defined(__IAR_SYSTEMS_ICC__)
#define IVT_GET_STACK_POINTER() (__sfe("CSTACK"))
#endif

#ifdef GP_HALCORTEXM4_PRE_MAIN_INIT
extern void GP_HALCORTEXM4_PRE_MAIN_INIT(void);
#endif // GP_HALCORTEXM4_PRE_MAIN_INIT

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
#if defined(GP_KX_HEAP_SIZE) && defined(__GNUC__)
// Heap start and end locations in RAM provided by linker
extern const unsigned long _sheap;
extern const unsigned long _eheap;
#endif //__GNUC__
/*****************************************************************************
 *                    Function Definitions
 *****************************************************************************/




#if defined(__GNUC__)

void _exit(int rc)
{
    for(;;)
    {
    }
}

void abort(void)
{
    _exit(1);
}

char* _sbrk(void* reentr, size_t incr)
{
#if defined(GP_KX_HEAP_SIZE) && defined(__GNUC__)
    static char* heapEnd = 0;
    char* prevHeapEnd;

    if(heapEnd == 0)
    {
        heapEnd = (char*)&_sheap;
    }
    prevHeapEnd = heapEnd;
    if(incr > (UIntPtr)&_eheap - (UIntPtr)heapEnd)
    {
        //Heap depleted
        return ((void*)-1); // error - no more memory
    }

    heapEnd += incr;
    return prevHeapEnd;
#else
    _exit(1);
    return NULL;
#endif
}

// Stub functions for C++ related lib _init/_fini functions
void __attribute__((weak)) _init(void)
{

}
void __attribute__((weak)) _fini(void)
{

}

void __attribute__((weak)) __cxa_pure_virtual(void)
{
  while (1);
}

// Stub functions when using 'nostdlib' */
void __attribute__((weak)) _write(void)
{
  while (1);
}

void __attribute__((weak)) _close(void)
{
  while (1);
}

void __attribute__((weak)) _fstat(void)
{
  while (1);
}

void __attribute__((weak)) _isatty(void)
{
  while (1);
}

void __attribute__((weak)) _lseek(void)
{
  while (1);
}

void __attribute__((weak)) _read(void)
{
  while (1);
}
#endif //__GNUC__


/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
