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
#include "hal_kx_ES.h"
#include "gpAssert.h"
#include "gpBsp.h"
#include "dev_handlers.h"
#include "gpHal_Calibration.h"
#include "gpStat.h"
#include "esec_qorvo.h"
#include "hal_SEMailbox.h"
#include "hal_SupplyVoltageMonitor.h"
#include "ram_regmap_inits.h"

#ifdef GP_DIVERSITY_FREERTOS
#include "FreeRTOSConfig.h"
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GP_COMPONENT_ID GP_COMPONENT_ID_HALCORTEXM4

#define HAL_WAIT_TIME_TIME_REF_UPDATE_US  1000

#ifndef GP_DIVERSITY_FREERTOS
#define DEFAULT_IRQ_PRIO 2
#else
#define DEFAULT_IRQ_PRIO configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#endif

// Number of core interrupts.
// Internal IRQs have negative numbers, so first one (Reset_IRQn) with
// the lowest number was used to define the number of internal IRQs.
#define INTERNAL_IRQ_COUNT ABS(Reset_IRQn)

/*****************************************************************************
 *                    External Function Definitions
 *****************************************************************************/

#ifndef GP_DIVERSITY_APPLICATION_MAIN
#define GP_DIVERSITY_APPLICATION_MAIN main
#endif

extern int GP_DIVERSITY_APPLICATION_MAIN(void);

/*****************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// Memory defined as UInt32 for 4 byte alignment
// Memory area for RAM based register map
#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
#define GP_MM_RAM_RAM_REGMAP_SIZE 0x400 // copied from akuma mm.h
static UInt32 hal_RamRegmap[GP_MM_RAM_RAM_REGMAP_SIZE / sizeof(UInt32)] LINKER_SECTION(".rt_fast_ram");
#endif // def GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

static UInt32 hal_iser_backup[2];
static UInt32 hal_intprio_backup[16];
static UInt32 hal_shpr_backup[3];
static UInt32 hal_systick_backup[2];
static UInt16 hal_es_valid_events_backup;
#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
static UInt32 hal_ak_ram_crc_backup;
#endif // def GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

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
extern unsigned long _sdata;
extern const unsigned long _ldata;
extern unsigned long _sbss;
extern unsigned long _akram_start;
extern const unsigned long _akram_length;
extern const unsigned long _lbss;
extern const unsigned long _estack; /* Application stack */
#endif

/* Actual IVT */
#if defined(__GNUC__)
__attribute__((section(".isr_vector")))
#endif
const intvec_elem __vector_table[] = {
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
    {.__ptr = forced_wakeup_handler},/* 16 ForcedWakeup */
/* chip-specific handlers */
#include "dev_ivt.h"
};

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
// Location of the RT system in RRAM
extern UInt16 gpHal_RtSystem_FlashData[];
#endif //GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

static void tosleep_backup(void);
static void wakeup_restore(void);
static void mask_sensitive_registers_from_retention_map(void);

static void init_data_bss(void);

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
static void enable_rtuc(void);
#endif //GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

static void enable_fpu(void);


/*****************************************************************************
 *                    Function Definitions
 *****************************************************************************/

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
void enable_rtuc(void)
{
    UInt32 real_rtuc_code_loc = (UInt32)&gpHal_RtSystem_FlashData[0];

    // Init windows
    GP_WB_WRITE_AK_MM_RAM_REGMAP_BASE_ADDRESS(((UIntPtr)&hal_RamRegmap - GP_MM_AKUMA_RAM_LINEAR_START) / 0x4);

    // enable RTUC clock if we are allowed to enable it
    GP_WB_WRITE_AK_STANDBY_CLK_ENA_RTUC(1);
    // adjust RTUC VTO while it is on reset
    GP_WB_WRITE_AK_RTUC_RESET(1);
    GP_WB_WRITE_AK_RTUC_VECTOR_TABLE_OFFSET((real_rtuc_code_loc)/256);
    GP_WB_WRITE_AK_RTUC_RESET(0);

    while(!GP_WB_READ_ES_SYSTEM_TIME_VALID()); //need to wait system time to be
}

void disable_rtuc(void)
{
    // disable RTUC clock
    GP_WB_WRITE_AK_STANDBY_CLK_ENA_RTUC(0);
    // RTUC in reset
    GP_WB_WRITE_AK_RTUC_RESET(1);
}
#endif //GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

void enable_fpu(void)
{
    SCB->CPACR = ((3 << 10 * 2) | /* set CP10 Full Access */
                  (3 << 11 * 2)); /* set CP11 Full Access */
}

/** @brief Reset handler is invoked at power-on-reset and when waking up from sleep. */
#ifdef GP_HALCORTEXM4_RESET_HANDLER
void GP_HALCORTEXM4_RESET_HANDLER(void)
#else
void reset_handler(void)
#endif
{
    hal_reset_always();

    /*
     * Harmonize VTOR and CORTEXM4_VECTOR_TABLE_OFFSET, prevents
     * ending up in the bootloader upon SYSRESET or LOCKUP
     * Just update if SCB->VTOR is different then zero, it can be zero if we reset from WDT
     */
    if (SCB->VTOR != 0)
    {
        GP_WB_WRITE_APPUC_VECTOR_TABLE_OFFSET(SCB->VTOR >> 8);
    }
    enable_fpu();

    if (!GP_WB_READ_STANDBY_UNMASKED_PORD_INTERRUPT())  /* wake-up from sleep */
    {
        // get the power mode
        power_mode_t power_mode;
        esec_get_power_mode(&power_mode);

        /* Do the real restore work */
        //assuming for now that the ram remap is retained - we may want to move this into the magic word ...
        if ((Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_LPS) ||
            (Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS))
        {
            // disable the cache auto invalidate upon enabling,
            // we don't want to invalidate it when we wakeup from sleep, since it is retained
            GP_WB_WRITE_APPUC_CACHE_DISABLE_INIT_FLUSH(1);

            // Do the actual restore
            wakeup_restore();
        }

        // Command the SEUC to finalize the wakeup
        esec_finalize_wakeup();

        // disable globals interrupts before we start restoring the NVIC.
        // if not, interrupts will already trigger and use the HAL_SP_RESERVED_SIZE
        // and potentially overflow into the next region, affecting the application stack.
        __disable_irq();
        /* Restore ISER, IP, SHP & systick */
        memcpy((void*)NVIC->ISER, hal_iser_backup, sizeof(hal_iser_backup));
        memcpy((void*)NVIC->IP, hal_intprio_backup, sizeof(hal_intprio_backup));
        memcpy((void*)SCB->SHP, hal_shpr_backup, sizeof(hal_shpr_backup));
        SysTick->LOAD = hal_systick_backup[0];
        SysTick->CTRL = hal_systick_backup[1];

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
        if((Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_LPS) ||
           (Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS))
        {
            // check akuma ram after enabling the main clocks
            GP_WB_AK_MM_SM_CRC_START();
            GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_MM_SM_CRC_INIT_BUSY() == 1), 10000);
            UInt32 crc_result = GP_WB_READ_AK_MM_SM_CRC_RESULT();
            if(hal_ak_ram_crc_backup != crc_result)
            {
                UInt8 soft_por_trigger = 0;
                GP_WB_SET_PMUD_SOFT_POR_TO_SOFT_POR_TRIGGERS(soft_por_trigger, 1);
                esec_reset_request(soft_por_trigger);
            }
        }
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

        GP_WB_WRITE_ES_FIRST_CYCLE_TIME_REFERENCE(0);
        GP_WB_WRITE_ES_FIRST_CYCLE_EVENT_HANDLER(0);

        // We need to explicitly wait until the symbol counter is updated before using it.
        GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_ES_INIT_TIME_REFERENCE_BUSY(), HAL_WAIT_TIME_TIME_REF_UPDATE_US);

        if((gpHal_SleepMode_t)Q_POWER_MODE_SLEEP(power_mode) != gpHal_SleepMode16MHz)
        {
            gpHal_ResetHandler_SetSymbolStartupTime();
        }

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
        if((Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_LPS) ||
           (Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS))
        {
            enable_rtuc();
            gpHal_WaitForRtStart();

            // restore RT
            gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_RETAIN_RESTORE, 0, NULL);
            NVIC_ClearPendingIRQ(AKUMA_IRQn); // clear possible akuma interrupt
        }
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

        // Log when the chip wakes up from sleep
        GP_STAT_SAMPLE_TIME();

        // keep stats
        hal_SleepIncrementCount();
        gpHal_CalibrationSetFirstAfterWakeup(true);

        hal_SupplyVoltageMonitorTxCheck();

        // Restore the HAL
        hal_resume();
    }
    else  /*from POR*/
    {
        //init data
        init_data_bss();

        /* Setup sleep-related items */
        hal_init();

        GP_WB_WRITE_ES_FIRST_CYCLE_TIME_REFERENCE(0);
        GP_WB_WRITE_ES_FIRST_CYCLE_EVENT_HANDLER(0);


        // The clearing of the AKRAM is not handled by the RT itself, since that subsystem does
        // not know if it is coming out of reset or out of sleep.
        __builtin_memset((void*)&_akram_start, 0, (size_t)&_akram_length);

        // enable the gpio buffers by default
        GP_WB_WRITE_IOB_GPIO_INPUT_BUFFER_ENABLE(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_MASK);

        // Set default IRQ priorities for external interrupts.
        // NVIC->IP points to the vector of external IRQ priorities only,
        // so the number of internal IRQs must be subtracted.
        memset((void*)NVIC->IP, DEFAULT_IRQ_PRIO << (8 - __NVIC_PRIO_BITS),
               sizeof(__vector_table) / sizeof(__vector_table[1]) - INTERNAL_IRQ_COUNT);

        //initialize events base pointer value
        GP_WB_WRITE_ES_EVENTS_BASE_PTR(SYSRAM_ES_EVENTS_START);
        // We need to explicitly wait until the symbol counter is updated before using it.
        GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_ES_INIT_TIME_REFERENCE_BUSY(), HAL_WAIT_TIME_TIME_REF_UPDATE_US);

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

        hal_SEMailboxInit();
        hal_SEMailbox_SetAkumaClkFreq(GP_WB_ENUM_CLOCK_SPEED_M64);

        enable_rtuc();
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
        gpHal_CalibrationSetFirstAfterWakeup(true);

#ifdef GP_HALCORTEXM4_PRE_MAIN_INIT
        GP_HALCORTEXM4_PRE_MAIN_INIT();
#endif

        /* Move to the application stack pointer */
        __set_MSP((UInt32)(&_estack) - HAL_SP_RESERVED_SIZE);
        GP_DIVERSITY_APPLICATION_MAIN();
    }

    /* Poor man's halt */
    while (1);
}

/** @brief Standby handler is invoked when chip is going to sleep. */
void standby_handler(void)
{
    power_mode_t power_mode;

    // Apply sleep counter calibration factor and realign sleep counter to symbol counter.
    gpHal_RealignTimebase();

    // Log when the chip goes to sleep
    GP_STAT_SAMPLE_TIME();

    // Prepare the chip for sleep and allow the backup to take place in the next step
    // This command also returns the power mode
    esec_prepare_sleep(&power_mode);


    /* Perform the full backup */
    if ((Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_LPS) ||
        (Q_POWER_MODE_STANDBY(power_mode) == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS))
    {
        tosleep_backup();
    }

    /* Backup ISER, IP, SHP & systick */
    memcpy(hal_iser_backup, (void*)NVIC->ISER, sizeof(hal_iser_backup));
    memcpy(hal_intprio_backup, (void*)NVIC->IP,   sizeof(hal_intprio_backup));
    memcpy(hal_shpr_backup, (void*)SCB->SHP, sizeof(hal_shpr_backup));
    hal_systick_backup[0] = SysTick->LOAD;
    hal_systick_backup[1] = SysTick->CTRL;

    // Send the mailbox command to actually transition to sleep
    // Don't wait for the return value, as we want the APPUC to go to sleep afterwards
    // This call will also shut down the APPUC by asserting the RESET line
    esec_go_to_sleep();

    // Actually go to sleep
    hal_go_to_sleep();
}

static void tosleep_backup()
{
    // Store the retained regmaps to sysram
    SYSRAM_SET_MAP0_RETAIN(GP_WB_READ_SYSRAM_RETENTION_MAP_0());
    SYSRAM_SET_MAP1_RETAIN(GP_WB_READ_SYSRAM_RETENTION_MAP_1());

    // Don't backup the sensitive registers, they are done on secuc
    mask_sensitive_registers_from_retention_map();

    /* Do HW SM backup */
    GP_WB_WRITE_SYSRAM_START_PTR(SYSRAM_RETAIN_INIT_START);
    GP_WB_SYSRAM_START_BACKUP();

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
    // backup RT
    gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_RETAIN_BACKUP, 0, NULL);
    // wait rtuc to go to sleep again
    GP_DO_WHILE_TIMEOUT_ASSERT(!GP_WB_READ_AK_RTUC_ASLEEP(), 1000);

    disable_rtuc();

    // get akuma CRC before disabling the main clocks
    GP_WB_AK_MM_SM_CRC_START();
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_AK_MM_SM_CRC_INIT_BUSY() == 1), 10000);
    hal_ak_ram_crc_backup = GP_WB_READ_AK_MM_SM_CRC_RESULT();
#endif // GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH

    hal_Waitus(1);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_SYSRAM_BUSY() == 1), 10000);
    hal_es_valid_events_backup = GP_WB_READ_ES_VALID_EVENTS();
}

static void wakeup_restore()
{
    // Obtain the retained regmaps from sysram
    GP_WB_WRITE_SYSRAM_RETENTION_MAP_0(SYSRAM_GET_MAP0_RETAIN());
    GP_WB_WRITE_SYSRAM_RETENTION_MAP_1(SYSRAM_GET_MAP1_RETAIN());

    // Don't restore the sensitive registers, they are done on secuc
    mask_sensitive_registers_from_retention_map();

    // Order the restore
    GP_WB_WRITE_SYSRAM_START_PTR(SYSRAM_RETAIN_INIT_START);
    GP_WB_SYSRAM_START_RESTORE();
    /* DO NOT MOVE following statement!
     * ... ensures the RESTORE is finished (blocking read).
     */
    hal_Waitus(1);
    GP_DO_WHILE_TIMEOUT_ASSERT((GP_WB_READ_SYSRAM_BUSY() == 1), 10000);
    GP_WB_WRITE_ES_SET_VALID_EVENTS(hal_es_valid_events_backup);
}

static void mask_sensitive_registers_from_retention_map()
{
    // Sensitive registers locked from APPUC are: CMA, BBPLL, XO32M, DCDC, PMUD, STANDBY and SECURE_ELEMENT
    // XO32M, PMUD and STANDBY are always-on, the rest must be excluded from APPUC backup
    GP_WB_WRITE_SYSRAM_RETAIN_CMA(0);
    GP_WB_WRITE_SYSRAM_RETAIN_BBPLL(0);
    GP_WB_WRITE_SYSRAM_RETAIN_DEBUG(0);
    GP_WB_WRITE_SYSRAM_RETAIN_DCDC(0);
    GP_WB_WRITE_SYSRAM_RETAIN_SECURE_ELEMENT(0);
}

/*
 * Helper functions, will be inlined or can be moved to compiler, environment
 * differentiated files.
 */
#ifdef GP_DIVERSITY_LOG
extern void Log_Unexpected_Fault(UInt32 sp, UInt32 regp);
#endif //GP_DIVERSITY_LOG

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
