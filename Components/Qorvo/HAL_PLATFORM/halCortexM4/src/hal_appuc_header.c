/*
 * Copyright (c) 2016, GreenPeak Technologies
 * Copyright (c) 2024, Qorvo Inc
 *
 *   Default logging handlers for ARM-based devices.
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
#include "q_firmware_header.h"
#include <string.h>

#if !defined(GP_DIVERSITY_GPHAL_XP4002)
#error APPUC header definitions can only be used for xp4002 devices
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#if defined(GP_COMP_VERSION) || defined(GP_DIVERSITY_BOOTLOADER_BUILD)
/** Macro helpers for converting version defined as four, comma separated, numbers to uint32_t. */
#define _ARG_1(N, ...)             N
#define _ARG_2(N0, N, ...)         N
#define _ARG_3(N0, N1, N, ...)     N
#define _ARG_4(N0, N1, N2, N, ...) N
#define ARG_1(args)                _ARG_1 args
#define ARG_2(args)                _ARG_2 args
#define ARG_3(args)                _ARG_3 args
#define ARG_4(args)                _ARG_4 args

/**
 * @brief Macro for converting version to uint32_t.
 *
 * @param[in] _VERSION Version defined as four, comma separated, numbers
 *
 * @return Version converted to uint32_t.
 */
#define VERSION_TO_BIGENDIAN_UINT32(_VERSION)                                                                          \
    (ARG_4((_VERSION)) << 24UL) | (ARG_3((_VERSION)) << 16UL) | (ARG_2((_VERSION)) << 8UL) | ARG_1((_VERSION))
#endif

/*****************************************************************************
 *                    Default fw header
 *****************************************************************************/

extern uint32_t _fw_header_vpp;
/* default minimal definition of loaded license */
extern const int _sappuc_fw_header;

#if defined(GP_DIVERSITY_CUSTOM_APPUC_FW_HEADER)
extern const appuc_fw_header_t appuc_fw_header_content __attribute__((section(".appuc_fw_header")));
#else
/* Firmware header definition, linked into section fw_header, at start of application */
const appuc_fw_header_t appuc_fw_header_content __attribute__((section(".appuc_fw_header"))) = {
    .certificate =
        {
            .content =
                {
                    .magic_number = APPUC_FW_HEADER_NON_SECURE_BOOT_MW,
                },
        },
    .auth =
        {
            .vpp = (UInt32)&_fw_header_vpp,
            .image_type = APPUC_FW_FIRST_STAGE_BL,
            .load_completed_magic_word = APPUC_FW_LOAD_COMPLETE_MW,
            .header_version = APPUC_FW_HEADER_VERSION,
            .image_destination_address = (UInt32)&appuc_fw_header_content,
        },
#if defined(GP_COMP_VERSION) || defined(GP_DIVERSITY_BOOTLOADER_BUILD)
    .appuc_fw_version_info =
        {
            .app_version = VERSION_TO_BIGENDIAN_UINT32(GP_VERSIONINFO_GLOBAL_VERSION),
            .app_descriptor = 0,
            .app_changelist_specifier = ((0x00FFFFFFUL & GP_CHANGELIST) << 8),
            .reserved = 0,
        },
#endif
};
#endif
/*****************************************************************************
 *                    Functions
 *****************************************************************************/
const appuc_fw_header_t* hal_get_appuc_header(void)
{
    return (appuc_fw_header_t*)&_sappuc_fw_header;
}

#if defined(GP_COMP_VERSION) || defined(GP_DIVERSITY_BOOTLOADER_BUILD)
const gpVersion_SoftwareInfo_t* get_sw_info(void)
{
    /* We need to create gpVersion_SoftwareInfo_t as it's not saved 1:1 in flash. */
    static gpVersion_SoftwareInfo_t info = {.ctrl = 0, .reserved = 0, .number = 0};

    memcpy(&info.version, &appuc_fw_header_content.appuc_fw_version_info.app_version, sizeof(gpVersion_ReleaseInfo_t));
    memcpy(&info.changeList, &appuc_fw_header_content.appuc_fw_version_info.app_changelist_specifier, sizeof(uint32_t));

    return &info;
}
#endif
