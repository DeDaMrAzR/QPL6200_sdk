#if defined(CHIP_HAVE_CONFIG_H)
#include "CHIPProjectConfig.h"
#endif
#include "global.h"
#include "q_firmware_header.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** Macro helpers for converting version defined as four, comma separated, numbers to uint32_t. */
#define _ARG_1(N, ...) N
#define _ARG_2(N0, N, ...) N
#define _ARG_3(N0, N1, N, ...) N
#define _ARG_4(N0, N1, N2, N, ...) N
#define ARG_1(args) _ARG_1 args
#define ARG_2(args) _ARG_2 args
#define ARG_3(args) _ARG_3 args
#define ARG_4(args) _ARG_4 args

/**
 * @brief Macro for converting version to uint32_t holding major,minor bytes (big endian on flash).
 *
 * @param[in] _VERSION Version defined as four, comma separated, numbers
 *
 * @return Version converted to uint32_t.
 */
#define VERSION_TO_BIGENDIAN_UINT32(_VERSION)                                                                                      \
    (ARG_4((_VERSION)) << 24UL) | (ARG_3((_VERSION)) << 16UL) | (ARG_2((_VERSION)) << 8UL) | ARG_1((_VERSION))

#define UINT32_TO_BIGENDIAN(_VERSION)                                                                                              \
    ((((_VERSION) >> 24UL) & 0xff) << 0 | (((_VERSION) >> 16UL) & 0xff) << 8 | (((_VERSION) >> 8UL) & 0xff) << 16 |                \
     ((_VERSION) &0xff) << 24)

extern uint32_t _fw_header_vpp;

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
            .image_type = 0, /* application */
            .load_completed_magic_word = APPUC_FW_LOAD_COMPLETE_MW,
            .header_version = APPUC_FW_HEADER_VERSION,
            .image_destination_address = (UInt32)&appuc_fw_header_content,
        },
    .appuc_fw_version_info =
        {
#ifdef GP_VERSIONINFO_GLOBAL_VERSION_OVERWRITE
            .app_version = VERSION_TO_BIGENDIAN_UINT32(GP_VERSIONINFO_GLOBAL_VERSION_OVERWRITE),
#elif defined(CHIP_HAVE_CONFIG_H)
            .app_version = UINT32_TO_BIGENDIAN(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION),
#else
            .app_version = VERSION_TO_BIGENDIAN_UINT32(GP_VERSIONINFO_GLOBAL_VERSION),
#endif
            .app_descriptor = 0,
#if defined(GP_COMP_VERSION)
            .app_changelist_specifier = ((0x00FFFFFFUL & GP_CHANGELIST) << 8),
#else
            .app_changelist_specifier = 0,
#endif
            .reserved = 0,
        },
};
