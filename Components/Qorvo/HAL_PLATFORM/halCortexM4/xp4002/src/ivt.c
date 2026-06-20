/*
 * File: ivt_basic.c
 *       Initial boot routines: reset and backup handlers.
 * Copyright (C) 2018 Qorvo International.
 *
 * OPEN ITEMS:
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "hal.h"
#include "gpAssert.h"
#include "global.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void unexpected_int(void);

// This should be done after declaration of the aliased functions
#include "dev_alias.h"

#if defined(GP_HALCORTEXM4_DIVERSITY_BOOTLOADER_IVT)
void hal_IntHandlerPrologue(void) {}

void hal_IntHandlerEpilogue(void) {}
#endif

/*****************************************************************************
 *                    External Function Prototypes
 *****************************************************************************/

// Declaration of ISR handlers
#include "dev_handlers.h"

/*****************************************************************************
 *                    Interrupt Vector Table
 *****************************************************************************/
/*
 * main
 */
extern int main(void);

/*
 * Compiler and/or environment dependent functions.
 */

/*
 * IVT references.
 */

#define HAL_DEFINE_INTERRUPT_WRAPPER(rawfunc, implfunc)             \
INTERRUPT_H void rawfunc (void)                                     \
{                                                                   \
    hal_IntHandlerPrologue();                                       \
    implfunc ();                                                    \
    hal_IntHandlerEpilogue();                                       \
}

#define HAL_DEFINE_INTERRUPT_WRAPPER_NOPENDING(rawfunc, implfunc)   \
INTERRUPT_H void rawfunc (void)                                     \
{                                                                   \
    implfunc ();                                                    \
}

extern INTERRUPT_H void pendsv_handler_impl(void);
extern INTERRUPT_H void systick_handler_impl(void);
extern INTERRUPT_H void timer_handler_impl(void);
extern INTERRUPT_H void akuma_handler_impl(void);
extern INTERRUPT_H void i2c_m_0_handler_impl(void);
extern INTERRUPT_H void i2c_m_1_handler_impl(void);
extern INTERRUPT_H void i2c_sl_handler_impl(void);
extern INTERRUPT_H void dma_dcb_handler_impl(void);
extern INTERRUPT_H void dma_scb_handler_impl(void);
extern INTERRUPT_H void i2s_0_handler_impl(void);
extern INTERRUPT_H void i2s_1_handler_impl(void);
extern INTERRUPT_H void es_handler_impl(void);
extern INTERRUPT_H void ipc_handler_impl(void);
extern INTERRUPT_H void ir_handler_impl(void);
extern INTERRUPT_H void keypad_handler_impl(void);
extern INTERRUPT_H void uart_0_handler_impl(void);
extern INTERRUPT_H void uart_1_handler_impl(void);
extern INTERRUPT_H void uart_2_handler_impl(void);
extern INTERRUPT_H void gpio_0_handler_impl(void);
extern INTERRUPT_H void gpio_1_handler_impl(void);
extern INTERRUPT_H void watchdog_handler_impl(void);

#if !defined(GP_DIVERSITY_FREERTOS)
HAL_DEFINE_INTERRUPT_WRAPPER(pendsv_handler, pendsv_handler_impl)
#endif

#if defined(GP_DIVERSITY_FREERTOS) 
HAL_DEFINE_INTERRUPT_WRAPPER_NOPENDING(systick_handler, systick_handler_impl)
#else
HAL_DEFINE_INTERRUPT_WRAPPER(systick_handler,   systick_handler_impl)
#endif

HAL_DEFINE_INTERRUPT_WRAPPER(timer_handler,     timer_handler_impl)

HAL_DEFINE_INTERRUPT_WRAPPER(akuma_handler,       akuma_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(i2c_m_0_handler,   i2c_m_0_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(i2c_m_1_handler,   i2c_m_1_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(i2c_sl_handler,     i2c_sl_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(dma_dcb_handler,   dma_dcb_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(dma_scb_handler,   dma_scb_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(i2s_0_handler,       i2s_0_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(i2s_1_handler,       i2s_1_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(es_handler,             es_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(ipc_handler,           ipc_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(ir_handler,             ir_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(keypad_handler,     keypad_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(uart_0_handler,     uart_0_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(uart_1_handler,     uart_1_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(uart_2_handler,     uart_2_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(gpio_0_handler,     gpio_0_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(gpio_1_handler,     gpio_1_handler_impl)
HAL_DEFINE_INTERRUPT_WRAPPER(watchdog_handler, watchdog_handler_impl)

void reset_handler(void);
extern INTERRUPT_H void nmi_handler(void);
extern INTERRUPT_H void hardfault_handler(void);
extern INTERRUPT_H void memmanage_handler(void);
extern INTERRUPT_H void busfault_handler(void);
extern INTERRUPT_H void usagefault_handler(void);
extern INTERRUPT_H void svcall_handler(void);
extern INTERRUPT_H void debugmon_handler(void);
extern INTERRUPT_H void pendsv_handler(void);
extern INTERRUPT_H void systick_handler(void);
extern INTERRUPT_H void forced_wakeup_handler(void);
#include "dev_handlers.h"

extern unsigned long _estack;  /* Stack for first boot */
#define IVT_GET_STACK_POINTER()     (&_estack)

// IVT entries can be stack pointer or function pointer ==> extra mapping needed
typedef void( *intfunc )( void );
typedef union { intfunc __fun; void * __ptr; } intvec_elem;

__attribute__((section(".isr_vector"))) __attribute__((used))
const intvec_elem __ivt[] = {
    /* Start of stack (grows downwards, so highest address = address where stack starts) */
    { .__ptr = IVT_GET_STACK_POINTER() },
    /* CM4 core handlers */
    { .__ptr = (void*) reset_handler},      /* 1 Reset */
    { .__ptr = (void*) nmi_handler},        /* 2 NMI */
    { .__ptr = (void*) hardfault_handler},  /* 3 HardFault */
    { .__ptr = (void*) memmanage_handler},  /* 4 MemManage */
    { .__ptr = (void*) busfault_handler},   /* 5 BusFault */
    { .__ptr = (void*) usagefault_handler}, /* 6 UsageFault */
    { .__ptr = NULL },                      /* 7 reserved */
    { .__ptr = NULL },                      /* 8 reserved */
    { .__ptr = NULL },                      /* 9 reserved */
    { .__ptr = NULL },                      /* 10 reserved */
    { .__ptr = (void*)svcall_handler},      /* 11 SVCall */
    { .__ptr = (void*)debugmon_handler},    /* 12 DebugMon */
    { .__ptr = NULL },                      /* 13 reserved */
    { .__ptr = (void*)pendsv_handler},      /* 14 PendSV */
    { .__ptr = (void*)systick_handler},     /* 15 SysTick */

    /* chip-specific handlers */
    { .__ptr = (void*)forced_wakeup_handler},         /* 16 ForcedWakeup */
    #include "dev_ivt.h"
};

/*
 * Helper functions, will be inlined or can be moved to compiler, environment
 * differentiated files.
 */


extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _ldata;
extern unsigned long _sbss;
extern unsigned long _lbss;

/*****************************************************************************
 *                    Additional sections
 *****************************************************************************/

/*
 * Default alias functions for weak and undefined isr functions.
 */
#ifdef GP_DIVERSITY_LOG
extern void Log_Unexpected_Interrupt(void);
extern void Log_Unexpected_Fault(UInt32 sp, UInt32 regp);
#endif //GP_DIVERSITY_LOG

void unexpected_fault_validsp(UInt32 sp, UInt32 regp)
{
#ifdef GP_DIVERSITY_LOG
    Log_Unexpected_Fault(sp, regp);
#endif
    /* never ignore - perhaps move to explicit SW reset? */
    GP_ASSERT_SYSTEM(false);
}

void unexpected_int(void)
{
#ifdef GP_DIVERSITY_LOG
    Log_Unexpected_Interrupt();
#endif
    /* ignore */
    GP_ASSERT_DEV_INT(false);
}
