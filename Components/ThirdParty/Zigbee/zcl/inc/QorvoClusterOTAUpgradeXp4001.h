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
 * $Header: $
 * $Change: $
 * $DateTime: $
 * 
 */


#ifndef _QORVO_CLUSTER_OTAUPGRADE_XP4001_H_
#define _QORVO_CLUSTER_OTAUPGRADE_XP4001_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterOTAUpgradeXp4001

class CQorvoClusterOTAUpgradeXp4001 : public CZigBeeClusterOTAUpgrade
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    
    // typedefs & enums
    public:
    
    // Identifiers of the attribute set
    enum {
        attributeMStoredFileVersion = 0x5000,       /* Manufacture specific custom attribute */
        attributeMStoredFileOffset  = 0x5001,       /* Manufacture specific custom attribute */
        attributeMStoredCRC         = 0x5002,       /* Manufacture specific custom attribute */
        attributeMStoredHash        = 0x5003,       /* Manufacture specific custom attribute */
        attributeMStoredProcessed   = 0x5004,       /* Manufacture specific custom attribute */
        attributeMExpandedKey       = 0x5005,       /* Manufacture specific custom attribute */
    };
        
    // Construction
    public:
        CQorvoClusterOTAUpgradeXp4001(CZigBeeFoundationApplication &application);

    // Manufacture specific attributes
	public:
        // The stored file version of previous download
        unsigned int dwMStoredFileVersion;
    
        // The stored file offset of previous download
        unsigned int dwMStoredFileOffset;
    
        // The stored CRC value previous download
        unsigned int dwMStoredCRC;

        // The stored key for AES-MMO encryption
        unsigned char  dwMStoredHash[16];

        // The stored Expanded key for AES-MMO encryption
        unsigned char  dwMExpandedKey[256];

        // The stored processed data bytes
        size_t  dwMStoredProcessed;

    // Overrides CZigBeeClusterOTAUpgrade
    protected:
        // Called when the complete OTA header, including optional fields, is
        // available for processing. The image is available in m_pHeader
        virtual bool OnImageHeaderAvailable();

        // Called to process an element contained in the OTA image. The element
        // data starts at m_nFileOffset, type and length are available in the
        // m_wElementTagID and m_dwElementSize data members. In overrides of
        // this method, make sure that after all required data blocks have been
        // requested, EndElement() is called to continue with the next element
        // or end the upgrade process properly
        virtual void OnBeginElement();

        // Called when the last element has been processed. The method must
        // make a state transition to either "download complete" by returning
        // true or "wait for more image" by returning false. Typically, this
        // comprises checking hash values etc.
        virtual bool OnImageComplete();

        // Called when the download or upgrade has been aborted by the server
        // or client. Can be used in derived classes to perform clean-up
        virtual void OnAbort();

        // Called after the download has completed successfully and the server
        // instructed to apply the image now
        virtual void OnApply();

    // Implementation
    protected:
        // Size of downloaded image data
        unsigned int m_nDownloadedImageSize;

        // OTA Header Size(including sub-element header)
        unsigned int m_nOffset;

        // Image Size of the OTA image file
        unsigned int m_imageSize;

    public:
        // number of OTA download page to trigger the save point for OTA resume */
        // i.e. Device will resume OTA download at the save point if process is being interrupted(e.g. POR)
        static unsigned char m_nNumberOfPageTriggerSave;
        
        // Called when a data record is complete. Called multiple times: once
        // for every data record in the stream
        void OnGetData(const unsigned char nStatus,
             const unsigned int nAddress, void *const pData,
             const unsigned short cbData);
                 
        // Called when the final "complete" record is complete
        void OnDataComplete(const unsigned char nStatus,
                const unsigned int nAddress, void *const pData,
                const unsigned short cbData);
        
        // Called when a hash value record is complete. 
        void OnGetHashValue(const unsigned char nStatus,
               const unsigned int nAddress, void *const pData,
               const unsigned short cbData);

        // Function used for clearing the OTA restore points stored in NVM
        void EraseOTARestorePoints(void);
};

#define __COMPACT7B_OTA_M_FILEVERSION() \
	__COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
        CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMStoredFileVersion, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		dwMStoredFileVersion)
        
#define __COMPACT7B_OTA_M_FILEOFFSET() \
	__COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
		CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMStoredFileOffset, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		dwMStoredFileOffset)
        
#define __COMPACT7B_OTA_M_CRC() \
	__COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
		CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMStoredCRC, \
		CZigBeeAttribute::typeUnsigned32, 0, \
		CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
		dwMStoredCRC)

#define __COMPACT7B_OTA_M_HASH() \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
        CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMStoredHash, \
        CZigBeeAttribute::typeSecurityKey, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent /*| CZigBeeAttribute::dynamic*/, \
        dwMStoredHash)

#define __COMPACT7B_OTA_M_PDATA() \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
        CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMStoredProcessed, \
        CZigBeeAttribute::typeUnsigned32, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
        dwMStoredProcessed)

#define __COMPACT7B_OTA_M_EXPANDEDKEY() \
    __COMPACT7B_ATTRIBUTE_CLUSTER_M(CQorvoClusterOTAUpgradeXp4001, \
        CZigBeeDeviceObject::wManufacturerID, CQorvoClusterOTAUpgradeXp4001::attributeMExpandedKey, \
        CZigBeeAttribute::typeString, 0, \
        CZigBeeAttribute::accessReadOnly | CZigBeeAttribute::persistent, \
        dwMExpandedKey)

#define __COMPACT7B_OTA_CLUSTER_M_ATTRIBUTES() \
    __COMPACT7B_OTA_M_FILEVERSION() \
    __COMPACT7B_OTA_M_FILEOFFSET() \
    __COMPACT7B_OTA_M_CRC() \
    __COMPACT7B_OTA_M_HASH()  \
    __COMPACT7B_OTA_M_PDATA() \
    __COMPACT7B_OTA_M_EXPANDEDKEY() \

#endif // _QORVO_CLUSTER_OTAUPGRADE_XP4001_H_
