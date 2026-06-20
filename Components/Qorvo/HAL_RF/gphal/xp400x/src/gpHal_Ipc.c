/*
 * Copyright (c) 2017, Qorvo Inc
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_GPHAL

#include "gpHal.h"
#include "gpHal_DEFS.h"
#include "gpHal_kx_Ipc.h"
#include "gpHal_RadioMgmt.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define GPHAL_IPC_TIMEOUT_US                           10000
#define GPHAL_RT_SYSTEM_HALTED_WAIT_TIME_US            10000
#define GPHAL_IPC_HALT_TIMEOUT_US                      150000 //us

// Maximum timeout before TRC should become idle after disabling rx on when idle flags
#define GPHAL_TRC_IDLE_WAIT_TIME_US                    10000

// Offset from start of RT system where the version bytes are located
// This should only be used for RT from flash or when working with MF >= 8.
#define GPHAL_RT_SYSTEM_VERSION_INFO_START_OFFSET      0x80

// Offset for ROM RT system vector table
#define GPHAL_RT_SYSTEM_ROM_VTOR_START_OFFSET          0x80

#define RT_WINDOW_GRANULARITY                         0x80

#define GPHAL_IPC_RT_STARTUP_TIMEOUT_US                20000

/*****************************************************************************
 *                   Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/******************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/


#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
static UInt8 gpHal_RtSystemVersion_Flash;
static Bool gphalIPC_radioClaimed = false;
#endif

HAL_CRITICAL_SECTION_DEF(Hal_Ipc_Mutex);

/******************************************************************************
 *                    External Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

//! Test if in interrupt mode
Bool inInterrupt(void)
{
#if defined(GP_COMP_CHIPEMU)
    return false;
#else
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
#endif
}

gpHal_Result_t gpHal_IpcTriggerCommand(UInt8 commandId, UInt8 argsLength, UInt8* pArgs)
{
    Bool timeOut = false;
    if(!HAL_VALID_MUTEX(Hal_Ipc_Mutex))
    {
        // not initialized yet
        GP_ASSERT_DEV_INT(false);
    }

    /* RT restore is part of the reset handler, which should not use the mutex, as interrupts are disabled at that */
    /* stage and they should remain disabled after the RT restore. */
    /* RT backup is part of the backup_handler, which is already running in interrupt context and there won't be */
    /* any other code running at that time. */
    /* Note the the bare-metal version of the mutex are also manipulating global interrupts, and will leave */
    /* them enabled after exiting from those functions. */
    Bool useMutex =
        (commandId != GP_WB_ENUM_RT_CMD_TYPE_RETAIN_BACKUP) && (commandId != GP_WB_ENUM_RT_CMD_TYPE_RETAIN_RESTORE);

    if(inInterrupt())
    {
        GP_ASSERT_DEV_INT(!HAL_IS_MUTEX_ACQUIRED(Hal_Ipc_Mutex));
    }

    if(useMutex)
    {
        HAL_ACQUIRE_MUTEX(Hal_Ipc_Mutex);
    }

    if(GP_WB_READ_AK_RTUC_RESET())
    {
        GP_LOG_SYSTEM_PRINTF("rtuc is in reset", 0);
        GP_ASSERT_DEV_INT(false);
    }

    if(GP_WB_READ_AK_INT_CTRL_MASKED_IPCX2RT_CMD_INTERRUPT() ||
       GP_WB_READ_AK_IPC_UNMASKED_RT2X_CMD_PROCESSED_INTERRUPT())
    {
        GP_LOG_SYSTEM_PRINTF("[WARN] RT busy cmd %u (prev: %u) int %d proc %d", 0, commandId, GP_WB_READ_AK_RT_CMD_COMMAND(),
                             GP_WB_READ_AK_INT_CTRL_MASKED_IPCX2RT_CMD_INTERRUPT(),
                             GP_WB_READ_AK_IPC_UNMASKED_RT2X_CMD_PROCESSED_INTERRUPT());
        GP_ASSERT_DEV_INT(false);
    }
    // Program command (+ arguments if any) and set result code to invalid
    GP_WB_WRITE_AK_RT_CMD_COMMAND(commandId);
    GP_WB_WRITE_AK_RT_CMD_RESULT(0);

    if(argsLength > 0)
    {
        GP_ASSERT_DEV_INT(pArgs != NULL);
        GP_HAL_WRITE_BYTE_STREAM(GP_WB_AK_RT_CMD_ARGS_ADDRESS, pArgs, argsLength);
    }

    // Trigger the command to RT and wait for completion
    GP_WB_AK_IPC_SET_X2RT_CMD_INTERRUPT();
    __DSB();
    GP_DO_WHILE_TIMEOUT(!GP_WB_READ_AK_IPC_UNMASKED_RT2X_CMD_PROCESSED_INTERRUPT(), GPHAL_IPC_TIMEOUT_US, &timeOut);

    if(timeOut)
    {
        // Command was not processed by BLE event manager
        GP_LOG_SYSTEM_PRINTF("[WARN] Timeout for gpHal IPC command %u" ,0, commandId);
        if(useMutex)
        {
            HAL_RELEASE_MUTEX(Hal_Ipc_Mutex);
        }
        return gpHal_ResultBusy;
    }
    else
    {
        if(GP_WB_READ_AK_RT_CMD_RESULT() != commandId)
        {
            GP_LOG_SYSTEM_PRINTF("[WARN] Unexp return value for IPC command (%u <> %u)" ,0, GP_WB_READ_AK_RT_CMD_RESULT(), commandId);
            GP_ASSERT_DEV_INT(false);
        }

        GP_WB_AK_IPC_CLR_RT2X_CMD_PROCESSED_INTERRUPT();
        if(useMutex)
        {
            HAL_RELEASE_MUTEX(Hal_Ipc_Mutex);
        }
        return gpHal_ResultSuccess;
    }
}
#if defined(GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH) && defined(GP_HAL_DIVERSITY_INCLUDE_IPC)
#define IPC_ITRFLAG_IPCRT2X_INTERRUPT 0
#define IPC_ITRFLAG_RCI_INTERRUPT      1

void gpHal_IpcStop(gpHal_IpcBackupRestoreFlags_t* pFlags)
{
#ifdef GP_DIVERSITY_GPHAL_XP4002
    gpHal_Result_t result;

    if(GP_WB_READ_AK_RTUC_RESET())
    {
        GP_LOG_PRINTF("rtuc is in reset", 0);
        return;
    }

    GP_ASSERT_DEV_EXT(pFlags != NULL);
    // Backup all needed flags
    pFlags->interruptFlags = 0;
    if (GP_WB_READ_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT())
    {
        BIT_SET(pFlags->interruptFlags,IPC_ITRFLAG_IPCRT2X_INTERRUPT);
        GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT(0);
    }

    result = gpHal_IpcTriggerCommand(GP_WB_ENUM_RT_CMD_TYPE_HALT_RTUC, 0, NULL);
    GP_ASSERT_SYSTEM(gpHal_ResultSuccess == result);

    //Wait until rtuc goes into reset
    GP_DO_WHILE_TIMEOUT_ASSERT(!GP_WB_READ_AK_RTUC_RESET(), GPHAL_IPC_HALT_TIMEOUT_US);
#endif //GP_DIVERSITY_GPHAL_XP4002
}

void gpHal_IpcRestart(gpHal_IpcBackupRestoreFlags_t* pFlags)
{
#ifdef GP_DIVERSITY_GPHAL_XP4002

    if(GP_WB_READ_AK_RTUC_RESET() == 0)
    {
        return;
    }

    UInt8 intmask;
    GP_ASSERT_DEV_EXT(pFlags != NULL);

    // Allow rtuc to run again
    GP_WB_WRITE_AK_RTUC_RESET(0);
    GP_DO_WHILE_TIMEOUT_ASSERT(GP_WB_READ_AK_RTUC_RESET(), GPHAL_IPC_HALT_TIMEOUT_US); //Timeout TBD

    // Restore interrupt and rx on when idle flags
    intmask = (UInt8) (BIT_TST(pFlags->interruptFlags,IPC_ITRFLAG_IPCRT2X_INTERRUPT));
    GP_WB_WRITE_AK_INT_CTRL_MASK_EXT_IPCRT2X_INTERRUPT(intmask);
#endif //GP_DIVERSITY_GPHAL_XP4002
}

#endif //GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH && GP_HAL_DIVERSITY_INCLUDE_IPC

void gpHal_WaitForRtStart(void)
{
#ifndef GP_COMP_CHIPEMU
    GP_DO_WHILE_TIMEOUT_ASSERT(!GP_WB_READ_AK_RTUC_ASLEEP(), GPHAL_IPC_RT_STARTUP_TIMEOUT_US);
#endif // GP_COMP_CHIPEMU
}

void gpHal_IpcInit(void)
{
#ifdef GP_COMP_CHIPEMU
    GP_WB_WRITE_AK_STANDBY_RESET_RTUC(0);
#endif

#ifdef GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH
    gphalIPC_radioClaimed = false;
#endif

    HAL_CREATE_MUTEX(&Hal_Ipc_Mutex);

    gpHal_WaitForRtStart();
}

void gpHal_RtInitVersionInfo(void)
{
#ifdef GP_COMP_CHIPEMU
    gpHal_RtSystemVersion_Flash = GP_DIVERSITY_RT_SYSTEM_IN_FLASH_VERSION;
#elif defined(GP_DIVERSITY_RT_SYSTEM_PARTS_IN_FLASH)
    gpHal_RtSystemVersion_Flash = (gpHal_RtSystem_FlashData[GPHAL_RT_SYSTEM_VERSION_INFO_START_OFFSET/2] & 0xFF);

    // When RT is running from flash, we know the version at compile time.
    // Check at runtime to make sure it still matches
    GP_ASSERT_SYSTEM(gpHal_RtSystemVersion_Flash == GP_DIVERSITY_RT_SYSTEM_IN_FLASH_VERSION);
#endif
}

UInt8 gpHal_GetRtSystemVersion(gpHal_RtSubSystemId_t subsystem_id)
{
    UInt8 version = 0xFF;
    switch (subsystem_id)
    {
        case gpHal_RtSubSystem_BleMgr:
#ifdef GP_DIVERSITY_RT_SYSTEM_BLEMGR_IN_FLASH
            version = gpHal_RtSystemVersion_Flash;
#else
#error "something is wrong with the diversities, need to define GP_DIVERSITY_RT_SYSTEM_BLEMGR_IN_FLASH or GP_DIVERSITY_RT_SYSTEM_IN_ROM"
#endif

            break;
        case gpHal_RtSubSystem_MacFilter:
        case gpHal_RtSubSystem_CalMgr:
#if   defined(GP_DIVERSITY_RT_SYSTEM_IN_FLASH)
            version = gpHal_RtSystemVersion_Flash;
#else
#error "something is wrong with the diversities"
#endif
            break;
        default:
            GP_ASSERT_DEV_EXT(false);
    }
    return version;
}
