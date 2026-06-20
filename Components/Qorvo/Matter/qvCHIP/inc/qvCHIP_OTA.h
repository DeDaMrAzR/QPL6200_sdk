/*
 * Copyright (c) 2020, Qorvo Inc
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

/** @file "qvCHIP_OTA.h"
 *
 *  CHIP wrapper OTA Upgrade API
 *
 *  Declarations of the OTA specific public functions and enumerations of qvCHIP.
 */

#ifndef _QVCHIP_OTA_H_
#define _QVCHIP_OTA_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define CONTENT_HASH_SIZE (32)

#define QVCHIP_OTA_MAX_IMAGES (4)

/*****************************************************************************
 *                    Enum Definitions
 *****************************************************************************/

/** @enum qvCHIP_OtaImageType_t */
//@{
#define qvCHIP_OtaImageTypeInvalid              0x00
#define qvCHIP_OtaImageTypePrimaryApplication   0xF0
#define qvCHIP_OtaImageTypeSecureElement        0xF1
#define qvCHIP_OtaImageTypeBootloader           0xF2
#define qvCHIP_OtaImageTypeSecondaryApplication 0xF3
/** @typedef qvCHIP_OtaImageType_t
    @brief Type of an image contained in OTA package

    @li @c qvCHIP_OtaImageTypePrimaryApplication        Primary application image type (e.g. Matter).
    @li @c qvCHIP_OtaImageTypeSecureElement             Secure Element image type.
    @li @c qvCHIP_OtaImageTypeBootloader                Bootloader image type.
    @li @c qvCHIP_OtaImageTypeSecondaryApplication      Secondary application image type (e.g. Zigbee).
*/
typedef uint32_t qvCHIP_OtaImageType_t;
//@}

/** @enum qvCHIP_OtaImageDownloadState_t */
//@{
#define qvCHIP_OtaDownloadNotStarted 0x00
#define qvCHIP_OtaDownloadIgnored    0x01
#define qvCHIP_OtaDownloadInProgress 0x02
#define qvCHIP_OtaPendingApply       0x03
#define qvCHIP_OtaDownloadCompleted  0x04
/** @typedef qvCHIP_OtaImageDownloadState_t
    @brief State of OTA download for a binary image

    @li @c qvCHIP_OtaDownloadNotStarted       Download not started yet.
    @li @c qvCHIP_OtaDownloadIgnored          Download for this image will be skipped.
    @li @c qvCHIP_OtaDownloadInProgress       Download is in progress.
    @li @c qvCHIP_OtaPendingApply             Download is completed and we rebooted to flash the new image.
    @li @c qvCHIP_OtaDownloadCompleted        Download was completed.
*/
typedef uint8_t qvCHIP_OtaImageDownloadState_t;
//@}

/** @enum qvCHIP_OtaStatus_t */
//@{
#define qvCHIP_OtaStatusSuccess               0x00
#define qvCHIP_OtaStatusFailedChecksumError   0x01
#define qvCHIP_OtaStatusFailedVerify          0x02
#define qvCHIP_OtaStatusFailedVersionError    0x03
#define qvCHIP_OtaStatusFailedProgramError    0x04
#define qvCHIP_OtaStatusFailedRescueInstalled 0x05
#define qvCHIP_OtaStatusWriteError            0x06
#define qvCHIP_OtaStatusInvalidAddress        0x07
#define qvCHIP_OtaStatusOutOfRange            0x08
#define qvCHIP_OtaStatusStoreImageFailed      0x09
#define qvCHIP_OtaStatusLoadImageFailed       0x0A
#define qvCHIP_OtaStatusParseFailed           0x0B
#define qvCHIP_OtaStatusInvalidImage          0x0C
#define qvCHIP_OtaStatusPreCheckFailed        0x0D
#define qvCHIP_OtaStatusInvalidParam          0x0E
/** @typedef qvCHIP_OtaStatus_t
    @brief General return status for all functions of this API

    @li @c qvCHIP_OtaStatusSuccess               The requested operation was completed successfully.
    @li @c qvCHIP_OtaStatusFailedChecksumError   The requested operation failed due to checksum error.
    @li @c qvCHIP_OtaStatusFailedVerify          The image verification failed.
    @li @c qvCHIP_OtaStatusFailedVersionError    Version is not newer then currently running.
    @li @c qvCHIP_OtaStatusFailedProgramError    The requested operation failed due to program error.
    @li @c qvCHIP_OtaStatusFailedRescueInstalled Failed rescue image installment.
    @li @c qvCHIP_OtaStatusWriteError            Failed to write to ota area.
    @li @c qvCHIP_OtaStatusInvalidAddress        Unexpected Address.
    @li @c qvCHIP_OtaStatusOutOfRange            Trying to access an address that is outside the range.
    @li @c qvCHIP_OtaStatusStoreImageFailed      Failure in storing the image.
    @li @c qvCHIP_OtaStatusLoadImageFailed       Failure in loading the image.
    @li @c qvCHIP_OtaStatusParseFailed           Failure in parsing the image.
    @li @c qvCHIP_OtaStatusInvalidImage          Invalid image.
    @li @c qvCHIP_OtaStatusPreCheckFailed        Upgrade image pre-wipe check failed.
    @li @c qvCHIP_OtaStatusInvalidParam          Pointer passed as an argument is NULL.
*/
typedef uint8_t qvCHIP_OtaStatus_t;
//@}

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Functional Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*! Attribute structure */
typedef struct {
    uint16_t vendorId;             /*! VendorId info from image header */
    uint16_t productId;            /*! ProductId info from image header */
    uint32_t softwareVersion;      /*! Software version of the binary */
    uint32_t minApplicableVersion; /*! Minimum running software version to be compatible with the OTA image */
    uint32_t maxApplicableVersion; /*! Maximum running software version to be compatible with the OTA image */
} qvCHIP_Ota_ImageHeader_t;

/*! OTA sub-image structure definition for the multi-image OTA download */
typedef struct __attribute__((packed)) {
    qvCHIP_OtaImageType_t imageIdentifier;  /*! Image type identifier */
    uint32_t imageSize;                     /*! Size in bytes of the content starting at data offset */
    uint32_t imageVersion;                  /*! Image version */
    uint32_t dataOffset;                    /*! Offset of the image data in the package structure */
    uint8_t contentHash[CONTENT_HASH_SIZE]; /*! Hash over the image content to check integrity */
} qvCHIP_Ota_SubImageHeader_t;

/*! Overall structure for OTA download to store in NVM */
typedef struct {
    // Fixed side - this data will not change
    uint32_t blockSize;              // Store OTA block size in NVM structure
    uint64_t lastUsedProviderNodeID; // Store information about last used provider for next query
    uint16_t lastUsedProviderEndpoint;
    uint8_t lastUsedProviderFabricIndex;
    qvCHIP_Ota_SubImageHeader_t imageHeaders[QVCHIP_OTA_MAX_IMAGES];
    // Dynamic side - this data will change as OTA upgrade progresses
    uint8_t currentImageIndex; // Index of the currently processing image
    qvCHIP_OtaImageDownloadState_t imageState[QVCHIP_OTA_MAX_IMAGES];
} qvCHIP_OtaProgressStorage_t;

/** @pointer to function qvCHIP_OtaImageValidationCback_t
 *  @brief Application API: Pointer to callback for header validation.
 */
typedef bool (*qvCHIP_OtaHeaderValidationCback_t)(qvCHIP_Ota_ImageHeader_t imageHeader);

/** @pointer to function qvCHIP_OtaUpgradeHandledCback_t
 *  @brief Application API: Pointer to callback for upgrade complete.
 */
typedef void (*qvCHIP_OtaUpgradeHandledCback_t)(bool upgradeHandled, qvCHIP_OtaStatus_t upgradeStatus);

/** @pointer to function qvCHIP_OtaEraseCompleteCback_t
 *  @brief Application API: Pointer to callback for erase complete.
 */
typedef void (*qvCHIP_OtaEraseCompleteCback_t)(void);

/*****************************************************************************
 *                    Public Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Flash API
 *****************************************************************************/

/** @brief Application API: call validation callback from application, if defined, or return False.
 */
bool qvCHIP_OtaValidateImage(qvCHIP_Ota_ImageHeader_t imageHeader);

/** @brief Application API: erase the OTA area (blocking).
 */
void qvCHIP_OtaEraseArea(void);

/** @brief Application API: start non-blocking erase of the OTA area.
 *
 *   @param cb                        Pointer to erase complete callback
 */
void qvCHIP_OtaStartEraseArea(qvCHIP_OtaEraseCompleteCback_t cb);

/** @brief Application API: get total size of the OTA upgrade area.
 *
 *   @return size Number of bytes in the OTA upgrade area.
 */
uint32_t qvCHIP_OtaGetAreaSize(void);

/** @brief Application API: get total size of the bootloader OTA upgrade area.
 *
 *   @return size Number of bytes in the bootloader OTA upgrade area.
 */
uint32_t qvCHIP_OtaGetAreaSizeForBootloader(void);

/** @brief Application API: check there is enough remaining size of the OTA upgrade area.
 *
 *   @param imageSize             the size of the image to be written.
 *   @return true if there is enough space, false otherwise.
 */
bool qvCHIP_OtaCheckRemainingAreaSize(uint32_t imageSize);

/** @brief Application API: initialize CRC so a writing operation can start.
 */
void qvCHIP_OtaStartWrite(void);

qvCHIP_OtaStatus_t qvCHIP_OtaGetHash(uint8_t* calculated_hash, uint32_t length);
/** @brief Application API: write chunk of data to external or internal storage device and update CRC.
 *
 *   @param offset                    offset of the data chunk.
 *   @param length                    Number of bytes to write.
 *   @param dataChunk                 Pointer to the data to write.
 *   @return result
 */
qvCHIP_OtaStatus_t qvCHIP_OtaWriteChunk(uint32_t offset, uint16_t length, uint8_t* dataChunk);

/** @brief Application API: read chunk of data from external or internal storage device.
 *
 *   @param offset                    Offset of the data to read.
 *   @param length                    Number of bytes to read.
 *   @param dataChunk                 Pointer to the buffer to store read data.
 *   @return result
 */
qvCHIP_OtaStatus_t qvCHIP_OtaReadChunk(uint32_t offset, uint16_t length, uint8_t* dataChunk);

/** @brief Application API: get current value of CRC.
 *
 *   @return crcValue
 */
uint32_t qvCHIP_OtaGetCrc(void);

/** @brief Application API: set the value of CRC.
 *
 *   @param crcValue
 */
void qvCHIP_OtaSetCrc(uint32_t crcValue);

/** @brief Application API: set the new pending image information for the User Mode Bootloader.
 *
 *   This writes the flag that there is a image available in flash. The User Mode Bootloader
 *   inspects this information to determine whether to carry out an update, and if so where to find the update image.
 *
 *   @return status                   qvCHIP_OtaStatusSuccess if the pending image set is successful,
 *                                    qvCHIP_OtaStatusStoreImageFailed if the pending image set failed
 */
qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingImage(void);

/** @brief Application API: set the new pending image information for the User Mode Bootloader upgrade
 *
 *   This writes the flag that there is a image available in flash. The SE will
 *   inspect this information to determine whether to carry out an update, and if so where to find the update image.
 *
 *   @return status                   qvCHIP_OtaStatusSuccess if the pending image set is successful,
 *                                    qvCHIP_OtaStatusStoreImageFailed if the pending image set failed
 */
qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingBootloader();

/** @brief Application API: set the new pending image information for the Secure Element upgrade
 *
 *   This writes the flag that there is a image available in flash. The SE will
 *   inspect this information to determine whether to carry out an update, and if so where to find the update image.
 *
 *   @return status                   qvCHIP_OtaStatusSuccess if the pending image set is successful,
 *                                    qvCHIP_OtaStatusStoreImageFailed if the pending image set failed
 */
qvCHIP_OtaStatus_t qvCHIP_OtaSetPendingSecureElement(void);
/** @brief Application API: this function stores an application callback to be
 *  invoked when the initialisation is done if an upgrade is handled.
 *
 *   The callback informs the application of the result of the update.
 *
 *   @return status                   qvCHIP_OtaStatusSuccess if the pending image set is successful,
 *                                    qvCHIP_OtaStatusStoreImageFailed if the pending image set failed
 */
void qvCHIP_OtaSetUpgradeHandledCb(qvCHIP_OtaUpgradeHandledCback_t upgradeHandledCb);

/** @brief Application API: this function stores an application callback to be
 *   invoked when an image download starts to validate the image header according to
 *   application specific criteria.
 *
 *   The callback does the validation and returns True or False, depending on if the image
 *   is accepted or not.
 *
 *   @param headerValidationCb     Parameter containing the pointer to the header validation callback
 */
void qvCHIP_OtaSetHeaderValidationCb(qvCHIP_OtaHeaderValidationCback_t headerValidationCb);

/** @brief Application API: trigger a hardware reset that will jump to the bootloader (to execute update).
 */
void qvCHIP_OtaReset(void);

/** @brief Application API: verifies image content with a hash. There are no parameters, as
 *                          qvCHIP_Ota layer already has all the information about current image.
 *
 *  @return true on success
 */
bool qvCHIP_OtaVerifyContentHash();

/** @brief Application API: validates the header signature contained in the header and stored
 *                          information from the header into OTA progress structures.
 *
 *  @param header                        pointer to header data to be validated and stored
 *  @param data_size                     size in bytes of the data pointed by header pointer
 *  @param header_size                   size in bytes of the header consumed
 *  @param public_key                    pointer to data containing public key
 *  @param public_key_size               size in bytes of the data pointed by public key pointer
 *
 *  @return true on success
 */
qvCHIP_OtaStatus_t qvCHIP_OtaValidateandStoreHeader(const uint8_t* header, uint16_t data_size, uint16_t* header_size,
                                                    const uint8_t* public_key, size_t public_key_size);

/** @brief Application API: returns a boolean result indicating if an OTA download is currently
 *                          in progress, based on the values in the progress info structure.
 *
 *  @return true if a download is in progress, false otherwise
 */
bool qvCHIP_OtaImageDownloadInProgress(void);

/** @brief Application API: changes the image state for the image at current index.
 *
 *  @param newState                     can take values from qvCHIP_OtaImageDownloadState_t enum
 */
void qvCHIP_OtaSetImageState(qvCHIP_OtaImageDownloadState_t newState);

/** @brief Application API: store OTA block size in progress info structure.
 *
 *  @param blockSize                     block size in bytes for OTA transfer
 */
void qvCHIP_OtaSetBlockSize(uint32_t blockSize);

/** @brief Application API: returns the OTA block size.
 *
 *  @return uint32_t                     block size in bytes for OTA transfer
 */
uint32_t qvCHIP_OtaGetBlockSize(void);

/** @brief Application API: stores into the OTA progress structure information about last used provider.
 *
 *  @param nodeId                        Pointer to memory area for Matter Node ID of last used provider
 *  @param endpoint                      Pointer to memory area for Matter endpoint of last used provider
 *  @param fabricIndex                   Pointer to memory area for Matter fabric index of last used provider
 */
void qvCHIP_OtaSetLastProvider(uint64_t nodeId, uint16_t endpoint, uint8_t fabricIndex);

/** @brief Application API: retrieves from progress structure information about last used provider.
 *
 *  @param nodeId                        Pointer to memory area for Matter Node ID of last used provider
 *  @param endpoint                      Pointer to memory area for Matter endpoint of last used provider
 *  @param fabricIndex                   Pointer to memory area for Matter fabric index of last used provider
 *
 *  @return true on success
 */
bool qvCHIP_OtaGetLastProvider(uint64_t* nodeId, uint16_t* endpoint, uint8_t* fabricIndex);

/** @brief Application API: returns the index of the current image and loads into the parameters
 *                          more data about the current image in progress.
 *
 *  @param imageHeader                   pointer to load a structure containing header information
 *                                        about the current image
 *  @param imageState                    pointer to load information about the current state of the
 *                                        currently downloading image
 *
 *  @return uint8_t                      index of the next image in the progress structure array
 */
uint8_t qvCHIP_OtaGetCurrentImage(qvCHIP_Ota_SubImageHeader_t* imageHeader, qvCHIP_OtaImageDownloadState_t* imageState);

/** @brief Application API: progress current image index in the OTA progress info structure.
 *
 *  @return uint8_t                      index of the next image in the progress structure array
 */
uint8_t qvCHIP_OtaGoNextImage(void);

/** @brief Application API: clear the progress info structure.
 */
void qvCHIP_OtaResetProgressInfo(void);

#if defined(QPG6200) // Use GN define
/** @brief Application API: get the version of a bootloader image
 * @param version output written
 * @param app_changelist_specifier output written
 * @param version bootloader image to read, for active bootloader use NULL
 *
 * @return true on success
 */
bool qvCHIP_GetBootloaderVersion(uint32_t* version, uint32_t* app_changelist_specifier, const void* bootloader_header);

/** @brief compare bootloader versions
 * @param version_a, app_changelist_specifier_a first comparison input
 * @param version_b, app_changelist_specifier_b second comparison input
 *
 * @return integer  a > b -> >=1
 *                  a == b -> 0
 *                  b > a -> <=-1
 */
int qvCHIP_CompareBootloaderVersion(const uint32_t version_a, const uint32_t app_changelist_specifier_a,
                                    const uint32_t version_b, const uint32_t app_changelist_specifier_b);

/** @brief Application API: get the version of a secure element image.
 * @param version output written
 *
 * @return true on success
 */

bool qvCHIP_GetSecureElementVersion(uint32_t* version);

/** @brief Application API: get the version of the running application from header info in the binary.
 *  @param app_type      identifies primary or secondary application
 *  @param version_word  memory area to get the version information as 4 bytes
 *
 *  @return true on success
 */
bool qvCHIP_GetApplicationVersionFromHeader(qvCHIP_OtaImageType_t app_type, uint32_t* version_word);
#endif
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_QVCHIP_OTA_H_
