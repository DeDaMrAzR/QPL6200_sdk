/*
 * Copyright (c) 2024, Qorvo Inc
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

#ifndef _Q_REG_INFO_PAGE_H_
#define _Q_REG_INFO_PAGE_H_

#include "qRegCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_PROG_PIN       (0xFF)
#define INFO_PAGE_CM_ONLY_SIZE (0x700)

#define INFOPAGE_CONFIG_MODE_CTRL_SCHMITT_ON_SPI_MASK                1
#define INFOPAGE_CONFIG_MODE_CTRL_SCHMITT_ON_UART_MASK               2
#define INFOPAGE_CONFIG_MODE_CTRL_JTAG_SWD_INTERNAL_PULL_ENABLE_MASK 4

#define INFOPAGE_DEBUG_PINOUT_MASK  (0x7f)
#define INFOPAGE_DEBUG_PINOUT_6_9   (0x00)
#define INFOPAGE_DEBUG_PINOUT_12_15 (0x01)
#define INFOPAGE_DEBUG_PINOUT_1_4   (0x02)

#define INFOPAGE_DEBUG_PINOUT_SWD_ONLY (0x80)

#define INFOPAGE_CM_APPLY_ON_POR_ONLY_MASK  (1 << 23)
#define INFOPAGE_CM_APPLY_ON_POR_ONLY_SHIFT (23)
#define INFOPAGE_CM_ADDRESS_MASK            (0xfffff)
#define INFOPAGE_CM_ADDRESS_SHIFT           (0)
#define INFOPAGE_CM_VALUE_MASK              (0xff000000)
#define INFOPAGE_CM_VALUE_SHIFT             (24)

#define INFOPAGE_RRAM_READ_LOCK_SIZE_BYTES  (8)
#define INFOPAGE_RRAM_WRITE_LOCK_SIZE_BYTES (8)
#define INFOPAGE_OTP_READ_LOCK_SIZE_BYTES   (1)
#define INFOPAGE_OTP_WRITE_LOCK_SIZE_BYTES  (1)

#define RMA_TEST_MAGIC_WORD      (0x4576526f)
#define FULL_RMA_TEST_MAGIC_WORD (0x416d5266)
#define RETEST_MAGIC_WORD        (0x516F7276)

#define INFOPAGE_NOF_CRC (6)

void gpHal_InfoPageInit(void);
UInt8 gpHal_InfoPagePackageId(void);
Bool gpHal_InfoPageNonVirginMagicWordIsSet(void);
void gpHal_InfoPageWriteNonVirginMagicWord(void);
void gpHal_InfoPageClearNonVirginMagicWord(void);
Bool gpHal_InfoPageRMATestMagicWordIsSet(void);
void gpHal_InfoPageWriteRMATestMagicWord(void);
void gpHal_InfoPageClearRMATestMagicWord(void);
Bool gpHal_InfoPageFullRMATestMagicWordIsSet(void);
void gpHal_InfoPageClearFullRMATestMagicWord(void);
void gpHal_InfoPageWriteFullRMATestMagicWord(void);
Bool gpHal_InfoPageRetestMagicWordIsSet(void);
void gpHal_InfoPageWriteRetestMagicWord(void);
void gpHal_InfoPageClearRetestMagicWord(void);
UInt8 gpHal_InfoPageGetProgPin(void);
UInt8 gpHal_InfoPageGetProgPinDelay(void);
UInt8 gpHal_InfoPageGetDiagModeTimeout(void);
UInt8 gpHal_InfoPageGetDebugModeTimeout(void);
Bool gpHal_InfoPageUseSchmittTriggersOnSPI(void);
Bool gpHal_InfoPageUseSchmittTriggersOnUART(void);
Bool gpHal_InfoPageEnableInternalPullOnJtagSwd(void);
UInt8* gpHal_InfoPageGetRRAMReadLockAddress(void);
UInt8* gpHal_InfoPageGetRRAMWriteLockAddress(void);
void gpHal_InfoPageWriteProvisioningResult(UInt8 result, UInt8 last_chunk_processed);
UInt32 gpHal_InfoPageGetOffsetOfConfiguration(void);
Bool gpHal_InfoPagePendingSEFWUpgradeMagicWordIsSet(void);
void gpHal_InfoPageWritePendingSEFWUpgradeMagicWord(void);
void gpHal_InfoPageClearPendingSEFWUpgradeMagicWord(void);
UInt32 gpHal_InfoPagePendingSEFWUpgradeOffset(void);
void gpHal_InfoPageWritePendingSEFWUpgradeOffset(UInt32 offset);
void gpHal_InfoPageClearPendingSEFWUpgradeOffset(void);
UInt8* gpHal_InfoPageGetOTPReadLockAddress(void);
UInt8* gpHal_InfoPageGetOTPWriteLockAddress(void);
void gpHal_InfoPageWriteFullRMATestMagicWord(void);
Bool gpHal_InfoPageFullRMATestMagicWordIsSet(void);
void gpHal_InfoPageClearFullRMATestMagicWord(void);

Bool gpHal_InfoPagePendingAPPUCUpgradeMagicWordIsSet(void);
void gpHal_InfoPageWritePendingAPPUCUpgradeMagicWord(void);
void gpHal_InfoPageClearPendingAPPUCUpgradeMagicWord(void);
UInt32 gpHal_InfoPagePendingAPPUCUpgradeOffset(void);
void gpHal_InfoPageWritePendingAPPUCUpgradeOffset(UInt32 offset);
void gpHal_InfoPageClearPendingAPPUCUpgradeOffset(void);

Bool gpHal_InfoPageUseRomBootloaderDevelopmentMode(void);
Bool gpHal_InfoPageKeepNVMandOTPAccessible(void);
Bool gpHal_InfoPageKeepRegMapAccessible(void);

#define ANTITAMPER_CHECK_VALUE_OK  0xA5FF3C00
#define ANTITAMPER_CHECK_VALUE_NOK 0x5A00C3FF
typedef UInt32 anti_tamper_check_value_t; // anti_tamper_check_value_t

/*!
 *  \brief      Get provisioning procedure result.
 *  \return     Provisioning result (see q_provisioning.h for details).
 */
UInt8 gpHal_InfopageProvisioningResultError(void);

/*!
 *  \brief      Get last processed chunk indication.
 *  \return     The number of the last chunk processed by the provisioning process (0 if not applicable).
 */
UInt8 gpHal_InfopageProvisioningResultLastChunkProcessed(void);

/*!
 *  \brief      Get debug pinout configuration.
 *  \return     INFOPAGE_DEBUG_PINOUT_6_9, INFOPAGE_DEBUG_PINOUT_12_15 or INFOPAGE_DEBUG_PINOUT_1_4.
 */
UInt8 gpHal_InfoPageDebugPins(void);

/*!
 *  \brief      Get debug pinout configuration.
 *  \return     True if only the 2 SWD pins should be mapped, false otherwise.
 */
Bool gpHal_InfoPageDebugPinsSWDOnly(void);

/*!
 *  \brief      Get the number of ROM calibration entries
 *  \return     Number of calibration entries
 */
UInt32 gpHal_InfoPageNumberOfROMCalibrationEntries(void);

/*!
 *  \brief      Get the ROM calibration entries
 *  \return     Pointer to first entry
 */
UInt32* gpHal_InfoPageROMCalibrationEntries(void);

/* Product Info Access functions */
/*!
 *  \brief      Get the Product ID
 *
 * The first character of the Product ID will be stored in the lowest address.
 *
 *  \return     Pointer to the product ID (Read Only data)
 */
const UInt8* gpHal_InfoPageGetProductId(void);

/*!
 *  \brief      Get the Product ID Custom Marker
 *
 * Product Custom Marker, used to make distinction between
 * products beyond what is in the Product ID section
 *
 *  \return     Copy of the data
 */
UInt8 gpHal_InfoGetPageProductIdCustomMarker(void);

/*!
 *  \brief      Get the Product ID Unknown Chip Version Handling
 *
 * The “UnknownChipVersionHandling” informs older SW about its compatibility
 * of the current chip. Indeed, when the chip version (as read from the silicon)
 * of the current chip is unknown to the SW, the SW will behave as if the current chip had
 * “UnknownChipVersionHanlding” as chip version
 *
 *  \return     Copy of the data
 */
UInt8 gpHal_InfoGetPageProductIdUnknownChipVerHandling(void);

/*!
 *  \brief      Get the Product Version
 *
 * Product Version Number. For Engineering samples, this is x from ESx.
 * For Production samples, this is  x for
 * the ESx version that this production sample is identical to.
 *
 *  \return     Copy of the data
 */
UInt32 gpHal_InfoPagePageGetProductVersion(void);

/* MAC Address Access function */
/*!
 *  \brief      Get the MAC Address
 *
 * 8-byte MAC address assigned by Qorvo
 *
 *  \return     Pointer to the MAC Address (Read Only data)
 */
const UInt8* gpHal_InfoPageGetMacAddress(void);

/* BLE Device Address Access function */
/*!
 *  \brief      Get the BLE Device Address
 *
 * BLE Device Address, stored in little endian format, assigned by Qorvo
 *
 *  \return     Pointer to the BLE Device Address (Read Only data)
 */
const UInt8* gpHal_InfoPageGetBleAddress(void);

/*!
 *  \brief      Verify CRCx, x is crc_index
 *  \return     True: CRC correct, False: CRC incorrect
 */
Bool gpHal_InfoPageVerifyCRC(UInt8 crc_index);

/*!
 *  \brief      Read the product license
 *  \return     Product license
 */
anti_tamper_check_value_t gpHal_InfoPageProductLicense(void* value);

/*!
 *  \brief      Get size of SEUC firmware area (including firmware header)
 *  \return     SE firmare area size
 */
UInt32 gpHal_InfoPageGetSizeSeucFwArea(void);

/*!
 *  \brief      Get SE FW size indicator
 *  @param      se_fw_size_indicator    Pointer where se_fw_size_indicator will be updated
 *  \return     SE_FW_SIZE_INDICATOR_CHECK_NOK - if tamper detected. SE_FW_SIZE_INDICATOR_CHECK_OK - if no tamper
 * detected.
 */
anti_tamper_check_value_t gpHal_InfoPageGetSeFwSizeIndicator(UInt8* se_fw_size_indicator);

/*!
 *  \brief      Read the appuc vector table offset
 *  \return     Vector table offset
 */
UInt32 gpHal_InfoPageVectorTableOffset(void);

#ifdef __cplusplus
}
#endif

#endif /* _Q_REG_INFO_PAGE_H_ */
