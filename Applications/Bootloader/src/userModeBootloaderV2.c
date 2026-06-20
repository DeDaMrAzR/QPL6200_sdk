/*
 * Copyright (c) 2017-2023, Qorvo Inc
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
 */
/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

#include "global.h"
#include "hal.h"

#include "gpUpgrade.h"
#include "gpUpgrade_defs.h"

#include "gpBaseComps.h"
#include "gpLog.h"

#include "hal_SEMailbox.h"

#include "q_firmware_header.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define VTOR_OFFSET_MASK        0xFFFFFFF8UL
#define VTOR_FW_HEADER_MULTIPLE 8U

/* CP10 Access Bits */
#define CPACR_CP10_Pos         20U
#define CPACR_CP10_FULL_ACCESS (3UL << CPACR_CP10_Pos)

/* CP11 Access Bits */
#define CPACR_CP11_Pos         22U
#define CPACR_CP11_FULL_ACCESS (3UL << CPACR_CP11_Pos)

/*****************************************************************************
 *                   Public functions definitions
 *****************************************************************************/

const appuc_fw_header_t* userModeBootloader_GetWakeUpBootHeader(Bool nullCheck);

/*****************************************************************************
 *                   Static functions definitions
 *****************************************************************************/
NORETURN static void userModeBootloader_ModifySpStartApp(UInt32 stack_pointer, UInt32 app_reset_handler);
static void userModeBootloader_Panic(void);

/*****************************************************************************
 *                   Const
 *****************************************************************************/
/* Primary application slot address. */
extern UInt32 __app_Start__;
static const appuc_fw_header_t* const primaryAppHeader = (appuc_fw_header_t*)&__app_Start__; /* Primary app APPUC header
                                                                                              */

/* Bootloader mailbox address. The pointer must be stored in NVM memory. It cannot be stored in bss or data section
 * because it is used to restore application after exiting sleep mode. The bss and data section are overwritten by the
 * main application. */
extern UInt32 upgrade_image_user_license_start__;
static const gpUpgrade_Metadata_t* const bootMetadata =
    (gpUpgrade_Metadata_t*)&upgrade_image_user_license_start__; /* Bootloader metadata address from linker */

/*****************************************************************************
 *                   Static functions
 *****************************************************************************/
#if defined(GP_DIVERSITY_DEVELOPMENT)
static void userModeBootloader_LedBlink(UInt8 count)
{
    while(count--)
    {
        HAL_LED_SET(RED);
        HAL_LED_SET(GRN);
        HAL_WAIT_MS(10);
        HAL_LED_CLR(RED);
        HAL_LED_CLR(GRN);
        HAL_WAIT_MS(30);
        HAL_LED_CLR(RED);
    }
    HAL_WAIT_MS(30);
}
#endif // defined(GP_DIVERSITY_DEVELOPMENT)

/* Bootloader panic handling, spin lock */
NORETURN static void userModeBootloader_Panic(void)
{
    GP_LOG_SYSTEM_PRINTF("Booting aborted, due to unrecoverable error. Restarting system... ", 0);

#if defined(GP_DIVERSITY_DEVELOPMENT)
    UInt8 cnt = 0;

    while(cnt++ < 10)
    {
        HAL_LED_SET(RED);
        HAL_WAIT_MS(1000);
        HAL_LED_CLR(RED);
        HAL_WAIT_MS(1000);
    }
#endif // defined(GP_DIVERSITY_DEVELOPMENT)

    gpLog_Flush();
    hal_SEMailbox_ResetRequest(GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP);

    while(1)
    {
    }
}

static void userModeBootloader_NvicInterruptsDisable(void)
{
    for(size_t i = 0; i < number_of_elements(NVIC->ICER); i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
    }

    for(size_t i = 0; i < number_of_elements(NVIC->ICPR); i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }
}

static void userModeBootloader_FallbackToPrimary(const appuc_fw_header_t** header)
{
    if(primaryAppHeader->certificate.content.magic_number != GP_UPGRADE_CERTIFICATE_MAGIC_NUMBER)
    {
        GP_LOG_SYSTEM_PRINTF("Cannot find primary application", 0);
        userModeBootloader_Panic();
    }

    GP_LOG_SYSTEM_PRINTF("Fallback to primary application", 0);

    *header = primaryAppHeader;
}

/* Set stack pointer and jump to the active application reset handler, this call does not return */
NORETURN static void userModeBootloader_ModifySpStartApp(UInt32 stack_pointer, UInt32 app_reset_handler)
{
    /* Get the application reset handler address from ISR vector
     * table and load stack pointer initial value into Main stack
     * pointer register. We want to be sure that we set MSP instead of
     * PSP, so we are using the MSP set function directly.
     */
    __set_MSP(stack_pointer);
    ((void (*)(void))app_reset_handler)();

    userModeBootloader_Panic();
}

static void userModeBootloader_OtaExecute(void)
{
    GP_LOG_SYSTEM_PRINTF("Image pending", 0);
    /* New image pending to be installed */
    gpUpgrade_Status_t upgStatus = gpUpgrade_InstallPendingImage();
    /* Upgrade status flow */
    if(upgStatus == gpUpgrade_StatusSuccess)
    {
    GP_LOG_SYSTEM_PRINTF("Install pending image success", 0);
    }
    /* In this case part of the application code might have been overwritten by the OTA content during decompression.
     * This is a critical error and the system must be restarted.
     */
    else if(upgStatus == gpUpgrade_StatusLoadImageFailed)
    {
        GP_LOG_SYSTEM_PRINTF("Install pending image fail during decompression", 0);
        userModeBootloader_Panic();
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Install pending image fail, err: %u. Ignore pending image", 0, upgStatus);
    }
}

bool userModeBootloader_IsApplicationPresent(const appuc_fw_header_t* fwHeader)
{
    UInt32 magicNumber = fwHeader->certificate.content.magic_number;

    return (magicNumber == GP_UPGRADE_CERTIFICATE_MAGIC_NUMBER);
 
}

bool userModeBootloader_IsPrimaryApplication(const appuc_fw_header_t* fwHeader)
{
    return (fwHeader == primaryAppHeader);
}

static void userModeBootloader_EnableFpu(void)
{
    SCB->CPACR |= (CPACR_CP10_FULL_ACCESS | CPACR_CP11_FULL_ACCESS); /* Set CP10 and CP11 Full Access */
}

static void userModeBootloader_DisableFpu(void)
{
    SCB->CPACR &= ~(CPACR_CP10_FULL_ACCESS | CPACR_CP11_FULL_ACCESS); /* Unset CP10 and CP11 Full Access */
}

static void UserModeBootloader_PrepareWakeUp(const appuc_fw_header_t* fwHeader)
{
    gpUpgrade_Status_t status;

    if(fwHeader != userModeBootloader_GetWakeUpBootHeader(false))
    {
        status = gpUpgrade_StoreWakeUpAddress(fwHeader);
        if(status != gpUpgrade_StatusSuccess)
        {
            GP_LOG_SYSTEM_PRINTF("Falied to store booting destination for exiting sleep mode, err: %u", 0, status);
            userModeBootloader_Panic();
        }

        GP_LOG_SYSTEM_PRINTF("Address for wake up from sleep mode is set to: %p", 0, fwHeader);
    }
}

/*****************************************************************************
 *                   Global functions
 *****************************************************************************/

/* Relocate to the active user application vector table. It must use only stack or variables stored in
 * the NVM memory.
 */
void userModeBootloader_JumpToApp(const appuc_fw_header_t* fwHeader)
{
    if(!fwHeader)
    {
        userModeBootloader_Panic();
    }

    // Set app isr vector. Vector table address is stored as a multiplication of 256-bit in a linker script.
    const UInt32 vectorTableAddress = fwHeader->auth.vpp << VTOR_FW_HEADER_MULTIPLE;

    if((vectorTableAddress < (uintptr_t)primaryAppHeader) || (vectorTableAddress > VTOR_OFFSET_MASK))
    {
        userModeBootloader_Panic();
    }

#if defined(GP_DIVERSITY_DEVELOPMENT)
    userModeBootloader_LedBlink(6);
#endif

    __disable_irq();    /* Disable interrupts. */
    __set_BASEPRI(0);   /* Turn of priority-based masking. */
    __set_CONTROL(0);   /* Set CONTROL to its reset value 0. */
    __set_PRIMASK(0);   /* Set PRIMASK to its reset value 0. */
    __set_FAULTMASK(0); /* Set FAULTMASK to its reset value 0. */

    userModeBootloader_NvicInterruptsDisable();

    SCB->VTOR = vectorTableAddress & VTOR_OFFSET_MASK;

    /* Get SP and the main application reset handler address from Vector table. */
    UInt32 stack_pointer = *((UInt32*)vectorTableAddress);
    UInt32 app_reset_handler = *((UInt32*)(vectorTableAddress + sizeof(UInt32)));

    /* VTOR is known now */
    userModeBootloader_ModifySpStartApp(stack_pointer, app_reset_handler);
    /* Does not return */
}

const appuc_fw_header_t* userModeBootloader_GetWakeUpBootHeader(Bool nullCheck)
{
    const appuc_fw_header_t* wakeUpHeader;

    /* The metadata pointer must be loaded from the NVM memory. */
    wakeUpHeader = (const appuc_fw_header_t*)bootMetadata->wakeUpAddress;

    if(!wakeUpHeader && nullCheck)
    {
        userModeBootloader_Panic();
    }

    return wakeUpHeader;
}

/*****************************************************************************
 *                   main
 *****************************************************************************/

int main(void)
{
    gpUpgrade_Status_t status;

    /* Bootloader is built with hard float abi. For example some utilities require FPU being enable. */
    userModeBootloader_EnableFpu();

#if defined(GP_DIVERSITY_DEVELOPMENT)
    HAL_LED_INIT_LEDS();
#endif // defined(GP_DIVERSITY_DEVELOPMENT)

    /* Upgrade flow only on POR */
    UInt8 resetReason = GP_WB_READ_PMUD_POR_REASON();
#if defined(GP_DIVERSITY_LOG)
        gpBaseComps_StackInit();
#endif // defined(GP_DIVERSITY_LOG)

    GP_LOG_SYSTEM_PRINTF("Bootloader start...", 0);
    hal_SEMailboxInit();

    status = gpUpgrade_Init(bootMetadata);
    if(status != gpUpgrade_StatusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("gpUpgrade_Init failed, err: %u", 0, status);
        userModeBootloader_Panic();
    }

    if(gpUpgrade_IsSecureBootEnabled())
    {
        GP_LOG_SYSTEM_PRINTF("Secure boot Enabled", 0);
    }

    /* PORD reason that trigger a software upgrade */
    if(resetReason == GP_WB_ENUM_POR_REASON_HW_POR ||              /* Chip reset. */
       resetReason == GP_WB_ENUM_POR_REASON_SOFT_POR_BOOTLOADER || /* Reset via ROM bootloader. */
       resetReason == GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP ||
       resetReason == GP_WB_ENUM_POR_REASON_SOFT_POR_BY_WATCHDOG /* Reset via WDT timeout. */)

    {
        if(gpUpgrade_IsImagePending())
        {
            userModeBootloader_OtaExecute();
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("OTA image not pending", 0);
        }
    }

    const appuc_fw_header_t* fwHeader = gpUpgrade_GetActiveBootHeader();

    if(!userModeBootloader_IsApplicationPresent(fwHeader))
    {
        GP_LOG_SYSTEM_PRINTF("Application has not been found at %p address", 0, (void*)fwHeader);

        if (userModeBootloader_IsPrimaryApplication(fwHeader))
        {
            GP_LOG_SYSTEM_PRINTF("Cannot find primary application", 0);
            userModeBootloader_Panic();
        }

        GP_LOG_SYSTEM_PRINTF("Fallback to the primary application at %p address", 0, (void*)primaryAppHeader);
        
        fwHeader = primaryAppHeader;

        if (!userModeBootloader_IsApplicationPresent(fwHeader))
        {
            GP_LOG_SYSTEM_PRINTF("Cannot find primary application", 0);
            userModeBootloader_Panic();
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Application has been found at %p address", 0, (void*)fwHeader);
    }

    Bool fallback;
    Bool isValid = false;

    do
    {
        /* In this place, only try fallback for application other than the primary one. For example when signature or
         * certificate checks fail. */
        fallback = !userModeBootloader_IsPrimaryApplication(fwHeader);
        isValid = gpUpgrade_ApplicationValidate(fwHeader);

        if(!isValid)
        {
            /* Application validation failed, try fallback to primary. */
            if(fallback)
            {
                userModeBootloader_FallbackToPrimary(&fwHeader);
                continue;
            }

            userModeBootloader_Panic();
        }

    } while(fallback && !isValid);

    /* When system is in a sleep mode, we need to store the wake-up address in the persistent memory, to boot valid
     * application since we do not execute the main function and bss and data section cannot be used to not corrupt the
     * main application data. To avoid many write cycle address is stored only when it is different from the current
     * one. The current address in mailbox can be corrupted and boot address can be different from the one stored in the
     * mailbox. That is why, it is stored additionally.
     */
    UserModeBootloader_PrepareWakeUp(fwHeader);

    GP_LOG_SYSTEM_PRINTF("Application validate correctly", 0);
    GP_LOG_SYSTEM_PRINTF("Booting application at %p address", 0, (void*)fwHeader);

    GP_LOG_SYSTEM_PRINTF("========================", 0);

#if defined(GP_DIVERSITY_LOG)
    gpLog_Flush();
    gpBaseComps_StackDeInit();
#endif

    userModeBootloader_DisableFpu();
    userModeBootloader_JumpToApp(fwHeader);

    while(1)
    {
        /* Should never reach this point */
    }
}
