/*
 * HAL routines
 */

#include "hal.h"
#include <setjmp.h>

#include "hal_defs.h"


/** Below two variables MUST be in retained RAM
 * _jb_sleep[] array must be aligned to 8-byte boundary since
 * save/restore context function are using LDM/STM instructions
 */
#if defined(__GNUC__)
static __attribute__((aligned(8))) UInt32 _jb_sleep[14];
#else
#error "Unknown compiler"
#endif

void hal_init(void)
{
}

extern uint32_t hal_save_context(uint32_t buf[]);
extern uint32_t hal_restore_context(uint32_t buf[], uint32_t jump);

void hal_sleep(void)
{
    // Set Ready-to-go-to-sleep for APPUC SW bit
    GP_WB_WRITE_APPUC_RTGTS_SW(1);

    // Enable deep sleep of the M4
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Store the context
    if (!hal_save_context(_jb_sleep))
    {
        /* If an interupt is triggered the interrupt is immediately cleared in the interrupt handler. If this is
         * done right before going to sleep we won't wake up anymore. Hence, we verify that no interupt has
         * been triggered before going to sleep.
         * If we disable interrupts and an interupt is received afterwards the chip will still wake up from __WFI
         * but the interupt handler will only be executed after re-enabling interrupts.
         * (see http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka16222.html).
         */
        /* don't use the HAL_DISABLE_GLOBAL_INT here because it will disable interrupts at
         * the NVIC level, and therefore it might block interrupts which are needed for waking up the ARM.
         * if you block the interrupts at the top-level, individual interrupts can still wake up the ARM
         * see: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka16222.html
         */
        __disable_irq();
        __ISB(); /* Make sure interrupts are disabled */
        //if (hal_CanGotoSleep() && !hal_HasUnpolledInterruptBeforeSleep())
        if (!hal_HasUnpolledInterruptBeforeSleep())
        {
            __WFI();
        }
    }
    __enable_irq();
    /* We assume all interrupts will have been polled before the next time this function is entered. */
    /* We clear flag after enabling interrupts so that pending interrupts handlers have been done and we
     * do not skip sleep unnecessarily next time we enter this function.
     * We can be interrupted again right before polling starts in which case the interrupt will be handled but
     * we will not go to sleep afterwards. However, one extra iteration is nothing to worry about. */
    hal_ClrUnpolledInterruptBeforeSleep();

    //disable the deep sleep mask here, if we didn't go to sleep and returned from __WFI, deep sleep mask is
    // not cleared from wake reset, and we might go to sleep in the middle of execution
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    // Clear Ready-to-go-to-sleep for APPUC SW bit
    GP_WB_WRITE_APPUC_RTGTS_SW(0);
}

void hal_resume(void)
{
    hal_restore_context(_jb_sleep, 1);
}

void hal_go_to_sleep()
{
    __WFI();
}
