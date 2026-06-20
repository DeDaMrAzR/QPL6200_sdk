/*
 * Copyright (c) 2022, Qorvo Inc
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
#include "gpHal_kx_NVM.h"
#include "gpHal_reg.h"
#include "q_info_page.h"
#include "gpUtils.h"

#include "qRegInfoPage.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define NON_VIRGIN_MAGIC_WORD (0x4b6f4d61)
/** Magic Word to indicate a SE_FW upgrade is pending */
#define SE_FW_UPGRADE_PENDING_MAGIC_WORD (0x94AA33BA)

/** Magic Word to indicate a APPUC upgrade is pending */
#define APPUC_UPGRADE_PENDING_MAGIC_WORD (0x9F44BFE2)

/* allow fallback mode */
#define BL_CONFIG_MPW_ROMBL_DEVELOPMENT_MASK       0x01
#define BL_CONFIG_MPW_KEEP_NVM_OTP_ACCESSIBLE_MASK 0x02
#define BL_CONFIG_MPW_KEEP_REGMAP_ACCESSIBLE_MASK  0x04

typedef UInt32 crc_t;
#define CRC0_REGION_START_OFFSET offsetof(info_page_t, configuration.rom_calibration_data)
#define CRC0_OFFSET              offsetof(info_page_t, configuration.CRC0)
#define CRC0_REGION_SIZE         (CRC0_OFFSET - CRC0_REGION_START_OFFSET)

#define CRC1_REGION_START_OFFSET offsetof(info_page_t, calibration_data.data)
#define CRC1_OFFSET              offsetof(info_page_t, calibration_data.crc)
#define CRC1_REGION_SIZE         (CRC1_OFFSET - CRC1_REGION_START_OFFSET)

#define CRC2_REGION_START_OFFSET offsetof(info_page_t, reserved.data)
#define CRC2_OFFSET              offsetof(info_page_t, reserved.crc)
#define CRC2_REGION_SIZE         (CRC2_OFFSET - CRC2_REGION_START_OFFSET)

#define CRC3_OFFSET              offsetof(info_page_t, programmer_locks.crc)
#define CRC3_REGION_START_OFFSET (CRC3_OFFSET + sizeof(crc_t))
#define CRC3_REGION_SIZE         (offsetof(info_page_t, alt_addresses) - CRC3_REGION_START_OFFSET)

#define CRC4_OFFSET              offsetof(info_page_t, alt_addresses.crc)
#define CRC4_REGION_START_OFFSET (CRC4_OFFSET + sizeof(crc_t))
#define CRC4_REGION_SIZE         (offsetof(info_page_t, factory_data) - CRC4_REGION_START_OFFSET)

#define CRC5_REGION_START_OFFSET offsetof(info_page_t, configuration.mac_802_15_4)
#define CRC5_OFFSET              offsetof(info_page_t, configuration.crc_address_info)
#define CRC5_REGION_SIZE         (CRC5_OFFSET - CRC5_REGION_START_OFFSET)

#define GPHAL_INFO_PAGE_ARM_START_ADDRESS (GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_INFO_USER_START))

#define MAX_ANTI_TAMPER_READ_OUT_SIZE 4

#define SE_FW_MAX_SIZE_INDICATOR  (32)
#define SE_FW_SIZE_GRANULARITY    (4 * 1024)
#define SE_FW_SIZE_INDICATOR_MASK (~(uint8_t)(SE_FW_MAX_SIZE_INDICATOR - 1))

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static Bool InfoPageIsWordSet(NvmPtr address, const UInt32 expected_word);
static void InfoPageWriteWord(NvmPtr address, UInt32 word);
static void InfoPageWriteUInt8(NvmPtr address, UInt8 word);

/*****************************************************************************
 *                    Static Variable Definitions
 *****************************************************************************/

#ifdef GP_COMP_CHIPEMU
// info_page will point to an address not known at compile-time for ChipEmu builds. Delay assignment to init-function
static info_page_t* info_page = NULL;
#else
// info_page is accessed before the init function, so it needs immediate initialization
static info_page_t* info_page = (info_page_t*)(GPHAL_INFO_PAGE_ARM_START_ADDRESS);
#endif

typedef struct {
    UInt16 section_offset;
    UInt16 section_length;
    UInt16 crc_offset;
} crc_table_t;

static crc_table_t crc_table[] = {
    {CRC5_REGION_START_OFFSET, CRC5_REGION_SIZE, CRC5_OFFSET}, /* CRC5 - CRC Address info */
    {CRC4_REGION_START_OFFSET, CRC4_REGION_SIZE, CRC4_OFFSET}, /* CRC4 - Alternative addresses (SM Provisioning) */
    {CRC3_REGION_START_OFFSET, CRC3_REGION_SIZE, CRC3_OFFSET}, /* CRC3 - Programmer Locks (SM Provisioning) */
    {CRC2_REGION_START_OFFSET, CRC2_REGION_SIZE, CRC2_OFFSET}, /* CRC2 - RFU */
    {CRC1_REGION_START_OFFSET, CRC1_REGION_SIZE, CRC1_OFFSET}, /* CRC1 - Calibration Data (CAL_NVM) */
    {CRC0_REGION_START_OFFSET, CRC0_REGION_SIZE, CRC0_OFFSET}, /* CRC0 - Configuration Data & Calibration Data */
};

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

void gpHal_InfoPageInit(void)
{
#ifdef GP_COMP_CHIPEMU
    info_page = (info_page_t*)GP_HAL_ARM_TO_PHYSICAL(GPHAL_INFO_PAGE_ARM_START_ADDRESS);
#endif // GP_COMP_CHIPEMU
}

UInt32 gpHal_InfoPageGetOffsetOfConfiguration(void)
{
    return (UInt32)&info_page->configuration - (UInt32)info_page;
}

UInt8 gpHal_InfoPagePackageId(void)
{
    return info_page->configuration.bootloader.package_id;
}

Bool gpHal_InfoPageNonVirginMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.non_virgin_magic_word)), NON_VIRGIN_MAGIC_WORD);
}

void gpHal_InfoPageWriteNonVirginMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.non_virgin_magic_word)), NON_VIRGIN_MAGIC_WORD);
}

void gpHal_InfoPageClearNonVirginMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.non_virgin_magic_word)), 0);
}

Bool gpHal_InfoPageRMATestMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.rma_test_magic_word)), RMA_TEST_MAGIC_WORD);
}

void gpHal_InfoPageWriteRMATestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.rma_test_magic_word)), RMA_TEST_MAGIC_WORD);
}

void gpHal_InfoPageClearRMATestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.rma_test_magic_word)), 0);
}

Bool gpHal_InfoPageFullRMATestMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.full_rma_test_magic_word)), FULL_RMA_TEST_MAGIC_WORD);
}

void gpHal_InfoPageWriteFullRMATestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.full_rma_test_magic_word)), FULL_RMA_TEST_MAGIC_WORD);
}

void gpHal_InfoPageClearFullRMATestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.full_rma_test_magic_word)), 0);
}

Bool gpHal_InfoPageRetestMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.retest_magic_word)), RETEST_MAGIC_WORD);
}

void gpHal_InfoPageWriteRetestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.retest_magic_word)), RETEST_MAGIC_WORD);
}

void gpHal_InfoPageClearRetestMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.retest_magic_word)), 0);
}

UInt8 gpHal_InfoPageGetProgPin(void)
{
    UInt8 InfoPageProgPin = info_page->configuration.bootloader.prog_pin;

    /* if bit(7) = 1, use program pin in bit(4:0) */
    if(InfoPageProgPin >> 7)
    {
        return (InfoPageProgPin & 0x1F);
    }

    return INVALID_PROG_PIN;
}

UInt8 gpHal_InfoPageGetProgPinDelay(void)
{
    return (info_page->configuration.bootloader.prog_pin_delay);
}

UInt8 gpHal_InfoPageGetDiagModeTimeout(void)
{
    return (info_page->configuration.bootloader.debug_diag_timeout >> 4);
}

UInt8 gpHal_InfoPageGetDebugModeTimeout(void)
{
    return (info_page->configuration.bootloader.debug_diag_timeout & 0x0F);
}

Bool gpHal_InfoPageUseSchmittTriggersOnSPI(void)
{
    return (info_page->configuration.bootloader.mode_ctrl & INFOPAGE_CONFIG_MODE_CTRL_SCHMITT_ON_SPI_MASK);
}

Bool gpHal_InfoPageUseSchmittTriggersOnUART(void)
{
    return (info_page->configuration.bootloader.mode_ctrl & INFOPAGE_CONFIG_MODE_CTRL_SCHMITT_ON_UART_MASK);
}

Bool gpHal_InfoPageEnableInternalPullOnJtagSwd(void)
{
    return (
        info_page->configuration.bootloader.mode_ctrl & INFOPAGE_CONFIG_MODE_CTRL_JTAG_SWD_INTERNAL_PULL_ENABLE_MASK);
}

UInt8* gpHal_InfoPageGetRRAMReadLockAddress(void)
{
    return (info_page->programmer_locks.rram_read_lock);
}

UInt8* gpHal_InfoPageGetRRAMWriteLockAddress(void)
{
    return (info_page->programmer_locks.rram_write_lock);
}

UInt8* gpHal_InfoPageGetOTPReadLockAddress(void)
{
    return (&info_page->programmer_locks.host_otp_read_lock[0]);
}

UInt8* gpHal_InfoPageGetOTPWriteLockAddress(void)
{
    return (&info_page->programmer_locks.host_otp_write_lock[0]);
}

void gpHal_InfoPageWriteProvisioningResult(UInt8 result, UInt8 last_chunk_processed)
{
    InfoPageWriteUInt8((NvmPtr)(&(info_page->magic_words.provisioning_result.error)), result);
    InfoPageWriteUInt8((NvmPtr)(&(info_page->magic_words.provisioning_result.last_chunk_processed)),
                       last_chunk_processed);
}

Bool gpHal_InfoPagePendingSEFWUpgradeMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.pending_sefw_upgrade_magic_word)),
                             SE_FW_UPGRADE_PENDING_MAGIC_WORD);
}

void gpHal_InfoPageWritePendingSEFWUpgradeMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_sefw_upgrade_magic_word)),
                      SE_FW_UPGRADE_PENDING_MAGIC_WORD);
}

void gpHal_InfoPageClearPendingSEFWUpgradeMagicWord(void)
{
    UInt32 zeroes = 0x00000000;
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_sefw_upgrade_magic_word)), zeroes);
}

UInt32 gpHal_InfoPagePendingSEFWUpgradeOffset(void)
{
    return (info_page->magic_words.pending_sefw_upgrade_offset);
}

void gpHal_InfoPageWritePendingSEFWUpgradeOffset(UInt32 offset)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_sefw_upgrade_offset)), offset);
}

void gpHal_InfoPageClearPendingSEFWUpgradeOffset(void)
{
    UInt32 zeroes = 0x00000000;
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_sefw_upgrade_offset)), zeroes);
}

Bool gpHal_InfoPagePendingAPPUCUpgradeMagicWordIsSet(void)
{
    return InfoPageIsWordSet((NvmPtr)(&(info_page->magic_words.pending_appuc_upgrade_magic_word)),
                             APPUC_UPGRADE_PENDING_MAGIC_WORD);
}

void gpHal_InfoPageWritePendingAPPUCUpgradeMagicWord(void)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_appuc_upgrade_magic_word)),
                      APPUC_UPGRADE_PENDING_MAGIC_WORD);
}

void gpHal_InfoPageClearPendingAPPUCUpgradeMagicWord(void)
{
    UInt32 zeroes = 0x00000000;
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_appuc_upgrade_magic_word)), zeroes);
}

UInt32 gpHal_InfoPagePendingAPPUCUpgradeOffset(void)
{
    return (info_page->magic_words.pending_appuc_upgrade_offset);
}

void gpHal_InfoPageWritePendingAPPUCUpgradeOffset(UInt32 offset)
{
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_appuc_upgrade_offset)), offset);
}

void gpHal_InfoPageClearPendingAPPUCUpgradeOffset(void)
{
    UInt32 zeroes = 0x00000000;
    InfoPageWriteWord((NvmPtr)(&(info_page->magic_words.pending_appuc_upgrade_offset)), zeroes);
}


UInt8 gpHal_InfopageProvisioningResultError(void)
{
    return info_page->magic_words.provisioning_result.error;
}

UInt8 gpHal_InfopageProvisioningResultLastChunkProcessed(void)
{
    return info_page->magic_words.provisioning_result.last_chunk_processed;
}

UInt8 gpHal_InfoPageDebugPins(void)
{
    UInt8 pinout = info_page->configuration.bootloader.jtag_swd_config & INFOPAGE_DEBUG_PINOUT_MASK;

    if((pinout != INFOPAGE_DEBUG_PINOUT_6_9) && (pinout != INFOPAGE_DEBUG_PINOUT_12_15) &&
       (pinout != INFOPAGE_DEBUG_PINOUT_1_4))
    {
        pinout = INFOPAGE_DEBUG_PINOUT_6_9;
    }
    return pinout;
}

Bool gpHal_InfoPageDebugPinsSWDOnly(void)
{
    return info_page->configuration.bootloader.jtag_swd_config & INFOPAGE_DEBUG_PINOUT_SWD_ONLY;
}

UInt32 gpHal_InfoPageNumberOfROMCalibrationEntries(void)
{
    return sizeof(info_page->configuration.rom_calibration_data) /
           sizeof(info_page->configuration.rom_calibration_data[0]);
}

UInt32* gpHal_InfoPageROMCalibrationEntries(void)
{
    return info_page->configuration.rom_calibration_data;
}

Bool gpHal_InfoPageVerifyCRC(UInt8 crc_index)
{
    UInt32 start_address;
    UInt32 crc_address;
    crc_t crc;
    COMPILE_TIME_ASSERT((sizeof(crc_table) / sizeof(crc_table[0])) == INFOPAGE_NOF_CRC);
    start_address = GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_INFO_USER_START) + crc_table[crc_index].section_offset;
    crc_address = GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_INFO_USER_START) + crc_table[crc_index].crc_offset;
    crc = gpUtils_CalculateCrc32((UInt8*)start_address, crc_table[crc_index].section_length);
    return (crc == *(crc_t*)crc_address);
}

static anti_tamper_check_value_t anti_tamper_read_out(const volatile void* addr, const volatile void* control_addr,
                                                      uint8_t* return_value, uint8_t len)
{
    memcpy(return_value, (uint8_t*)addr, len);

    return ANTITAMPER_CHECK_VALUE_OK;
}

anti_tamper_check_value_t gpHal_InfoPageProductLicense(void* value)
{
    return anti_tamper_read_out(&(info_page->configuration.product_license),
                                &(info_page->configuration.product_license_control), value,
                                sizeof(info_page->configuration.product_license));
}
/* Product Info Access functions */
const UInt8* gpHal_InfoPageGetProductId(void)
{
    // The first character of the Product ID will be stored in the lowest address.
    return &(info_page->configuration.product_id[0]);
}

UInt8 gpHal_InfoGetPageProductIdCustomMarker(void)
{
    // Product Custom Marker, used to make distinction between
    // products beyond what is in the Product ID section
    return (info_page->configuration.product_id[10]);
}

UInt8 gpHal_InfoGetPageProductIdUnknownChipVerHandling(void)
{
    // The “UnknownChipVersionHandling” informs older SW about its compatibility
    // of the current chip. Indeed, when the chip version (as read from the silicon)
    // of the current chip is unknown to the SW, the SW will behave as if the current chip had
    return (info_page->configuration.product_id[11]);
}

UInt32 gpHal_InfoPagePageGetProductVersion(void)
{
    // Product Version Number. For Engineering samples, this is x from ESx.
    // For Production samples, this is  x for
    // the ESx version that this production sample is identical to.
    return info_page->configuration.product_version;
}

/* MAC Address Access function */
const UInt8* gpHal_InfoPageGetMacAddress(void)
{
    // 8-byte MAC address assigned by Qorvo
    return &(info_page->configuration.mac_802_15_4[0]);
}

/* BLE Device Address Access function */
const UInt8* gpHal_InfoPageGetBleAddress(void)
{
    // BLE Device Address, stored in little endian format, assigned by Qorvo
    return &(info_page->configuration.ble_address[0]);
}

anti_tamper_check_value_t gpHal_InfoPageGetSeFwSizeIndicator(UInt8* size_indicator)
{
    return anti_tamper_read_out(&(info_page->configuration.bootloader.seuc_rram_size_indicator),
                                &(info_page->configuration.bootloader.seuc_rram_size_indicator_control), size_indicator,
                                sizeof(info_page->configuration.bootloader.seuc_rram_size_indicator));
}

UInt32 gpHal_InfoPageGetSizeSeucFwArea(void)
{
    UInt32 check_result = ANTITAMPER_CHECK_VALUE_NOK;
    /* check that only 32 bits reserved for locking */
    COMPILE_TIME_ASSERT(GP_WB_SECURE_ELEMENT_RESTRICTED_NVM_MAIN_MASK == UINT32_MAX);
    const UInt32 max_se_fw_area = SE_FW_MAX_SIZE_INDICATOR * SE_FW_SIZE_GRANULARITY;
    UInt32 fw_area_size = max_se_fw_area;
    UInt8 fw_size_indicator = 0;
    check_result = gpHal_InfoPageGetSeFwSizeIndicator(&fw_size_indicator);

    /* if one ore more of the hightest 3 bits are set, or the size indicator is maximal, tamper, as this should not be
     * possible */
    if(fw_size_indicator & SE_FW_SIZE_INDICATOR_MASK)
    {
        Q_ASSERT(false);
    }

    if(check_result == ANTITAMPER_CHECK_VALUE_OK)
    {
        fw_area_size = (SE_FW_MAX_SIZE_INDICATOR - fw_size_indicator) * SE_FW_SIZE_GRANULARITY;
    }
    return fw_area_size;
}

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

Bool InfoPageIsWordSet(NvmPtr address, const UInt32 expected_word)
{
    UInt32 word = 0;
    gpHal_NVMError_t read_error;

    read_error = gpHal_NVMRead(GP_HAL_PHYSICAL_TO_ARM(address), sizeof(word), (UInt8*)&word);

    if(gpHal_NVMError_Success != read_error)
    {
        Q_ASSERT(false);
    }

    return expected_word == word;
}

#define InfoPageWrite(address, data)                                                                                   \
    {                                                                                                                  \
        gpHal_NVMError_t write_error;                                                                                  \
                                                                                                                       \
        write_error = gpHal_NVMWrite(GP_HAL_PHYSICAL_TO_ARM(address), sizeof(data), (UInt8*)&data);                    \
                                                                                                                       \
        if(gpHal_NVMError_Success != write_error)                                                                      \
        {                                                                                                              \
            Q_ASSERT(false);                                                                                           \
        }                                                                                                              \
    }

void InfoPageWriteWord(NvmPtr address, UInt32 word)
{
    InfoPageWrite(address, word);
}

void InfoPageWriteUInt8(NvmPtr address, UInt8 word)
{
    InfoPageWrite(address, word);
}

UInt32 gpHal_InfoPageVectorTableOffset(void)
{
    return info_page->magic_words.virgin_mode_appuc_vector_table_offset;
}
