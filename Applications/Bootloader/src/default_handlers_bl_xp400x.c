/*
 * Copyright (c) 2015-2016, GreenPeak Technologies
 * Copyright (c) 2017-2022, Qorvo Inc
 *
 *   Default handlers for ARM-based devices.
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
 *                    Includes Definitions
 *****************************************************************************/

#include "hal.h"
#include "hal_defs.h"

#include "gpLog.h"
#include "gpHal.h"
#include "gpAssert.h"
#include "gpBsp.h"
#include "gpStat.h"
#include "dev_handlers.h"
#include "gpHal_kx_regprop.h"
#include "hal_user_license.h"

#if defined(GP_DIVERSITY_APPUC_FW_HEADER)
#include "gpUpgrade.h"
#endif
#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
#include "gpUtils.h"
#include "hal_user_license.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

/*****************************************************************************
 *                    External Function Definitions
 *****************************************************************************/
extern int main(void);

#if defined(GP_DIVERSITY_APPUC_FW_HEADER) 
extern const appuc_fw_header_t* userModeBootloader_GetWakeUpBootHeader(Bool nullCheck);
extern void userModeBootloader_JumpToApp(const appuc_fw_header_t* fwHeader);
#else
extern UInt32 userModeBootloader_GetActiveBootAddress(void);
extern void userModeBootloader_JumpToApp(UInt32 startAddress);
#endif // defined(GP_DIVERSITY_APPUC_FW_HEADER) && !defined(GP_DIVERSITY_APP_LICENSE_BASED_BOOT)

extern const UInt32 app_StartAddr_Active;
#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
extern UInt32 upgrade_image_user_license_start__;
#endif // GP_UPGRADE_DIVERSITY_DUAL_BOOT
/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    External Function Declarations
 *****************************************************************************/

/*
 * IVT references.
 */
extern WEAK INTERRUPT_H void reset_handler(void);
extern WEAK INTERRUPT_H void nmi_handler(void);
extern WEAK INTERRUPT_H void hardfault_handler(void);
extern WEAK INTERRUPT_H void memmanage_handler(void);
extern WEAK INTERRUPT_H void busfault_handler(void);
extern WEAK INTERRUPT_H void usagefault_handler(void);
extern WEAK INTERRUPT_H void svcall_handler(void);
extern WEAK INTERRUPT_H void debugmon_handler(void);
extern WEAK INTERRUPT_H void pendsv_handler(void);
extern WEAK INTERRUPT_H void systick_handler(void);
extern WEAK INTERRUPT_H void forced_wakeup_handler(void);
/*****************************************************************************
 *                    Linkerscript Symbols Declarations
 *****************************************************************************/

#include "handler_helpers.c"

#if defined(__GNUC__)
extern const unsigned long _sidata;
extern const unsigned long _sdata;
extern const unsigned long _ldata;
extern const unsigned long _sbss;
extern const unsigned long _lbss;
extern const unsigned long _estack; /* Application stack */
#endif

/* Actual IVT */
__attribute__((section(".isr_vector"))) const intvec_elem __vector_table[] = {
    /* Start of stack (grows downwards, so highest address = address where stack starts) */
    {.__ptr = IVT_GET_STACK_POINTER()},
    /* CM4 core handlers */
    {.__fun = reset_handler},      /* Reset */
    {.__fun = nmi_handler},        /* NMI */
    {.__fun = hardfault_handler},  /* HardFault */
    {.__fun = memmanage_handler},  /* MemManage */
    {.__fun = busfault_handler},   /* BusFault */
    {.__fun = usagefault_handler}, /* UsageFault */
    {.__ptr = NULL},               /* reserved */
    {.__ptr = NULL},               /* reserved */
    {.__ptr = NULL},               /* reserved */
    {.__ptr = NULL},               /* reserved */
    {.__fun = svcall_handler},     /* SVCall */
    {.__fun = debugmon_handler},   /* DebugMon */
    {.__ptr = NULL},               /* reserved */
    {.__fun = pendsv_handler},     /* PendSV */
    {.__fun = systick_handler},    /* SysTick */

                                   /* chip-specific handlers */
    {.__ptr = forced_wakeup_handler}, /* 16 ForcedWakeup */
/* chip-specific handlers */
#include "dev_ivt.h"
};

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void init_data_bss(void);

/*****************************************************************************
 *                    Function Definitions
 *****************************************************************************/
void hal_ResetWatchdog(void)
{
}

UInt16 hal_GetWatchdogTimeRemaining(void)
{
    return 0xFFFF;
}

/** @brief Reset handler is invoked at power-on-reset and when waking up from sleep. */
#ifdef GP_HALCORTEXM4_RESET_HANDLER
void GP_HALCORTEXM4_RESET_HANDLER(void)
#else
void reset_handler(void)
#endif
{
    hal_reset_always();


    if(GP_WB_READ_STANDBY_UNMASKED_PORD_INTERRUPT()) /* from POR */
    {
        // init data
        init_data_bss();

        /* Run bootloader main */
        main();
    }

/* Cover all different cases when we don't use only APPUC FW header. */
#if                         \
    !defined(GP_DIVERSITY_APPUC_FW_HEADER) 
#if defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
    UInt32 bootAddr = 0;

    bootAddr = userModeBootloader_GetActiveBootAddress();
    userModeBootloader_JumpToApp(bootAddr);
#else
    userModeBootloader_JumpToApp(userModeBootloader_GetActiveBootAddress());
#endif // defined(GP_UPGRADE_DIVERSITY_DUAL_BOOT)
#else
    /* For bootloader using APPUC header, this is executed only when CPU wakes up from the deep sleep mode.
     * Be carefull when modifying this code, as it cannot play with any static data (bss, data) since
     * the main application state is retained there.
     */
    userModeBootloader_JumpToApp(userModeBootloader_GetWakeUpBootHeader(true));
#endif // defined(GP_DIVERSITY_APP_LICENSE_BASED_BOOT) || (defined(GP_DIVERSITY_APPUC_FW_HEADER) &&
       // defined(GP_DIVERSITY_APP_LICENSE_BASED_BOOT))

    /* Poor man's halt */
    while(1)
        ;
}

/*
 * Helper functions, will be inlined or can be moved to compiler, environment
 * differentiated files.
 */
#ifdef GP_DIVERSITY_LOG
extern void Log_Unexpected_Fault(UInt32 sp, UInt32 regp);
#endif // GP_DIVERSITY_LOG

static void init_data_bss(void)
{
    extern void __libc_init_array(void);

    // copy data section
    __builtin_memcpy((void*)&_sdata, (void*)&_sidata, (size_t)&_ldata);
    // zero out bss
    __builtin_memset((void*)&_sbss, 0, (size_t)&_lbss);
    // Initialize C++ constructor/destructor code
    __libc_init_array();
}
