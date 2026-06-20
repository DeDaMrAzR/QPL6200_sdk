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

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "MemoryDevice.h"
#include "PersistentStorage.h"
#include "IEEE802154PHY.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "IEEE802154MACPIB.h"
#include "IEEE802154MAC.h"
#include "IEEE802154MCF.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeNetworkCommands.h"
#include "ZigBeeNetwork.h"
#include "ZigBeeDeviceProfile.h"
#include "ZigBeeDeviceObject.h"
#include "ZigBeeClusterLibrary.h"
#include "ZigBeeClusterOTAUpgrade.h"
#include "ZigBeeClusterOTABase.h"
#include "ZigBeeServiceLocator.h"
#include "ZigBeeOTAUpgrade.h"
#include "ZigBeeStorage.h"
#include "CompactAES.h"
#include "CompactMD5.h"

#include "QorvoClusterOTAUpgradeXp4001.h"

#include "gpLog.h"
#ifdef GP_DIVERSITY_BOOTLOADER
#include "gpUpgrade.h"
#include "gpSched.h"
#endif //GP_DIVERSITY_BOOTLOADER

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO

//#define GP_LOCAL_LOG

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

/** @brief max data record request size */
#define MAX_DATA_REQUEST_SIZE                   256

/** @brief hash key size used for AESMMO encryption (hash generation) */
#define HASH_KEY_SIZE                           16

/** @brief value of tag ID for firmware and length of it */
#define TAGID_AND_LENGTH_OF_TAGID               6

/** @brief the mandatory fields in OTA header  */
#define MANDATORY_HEADER_SIZE                   56

/** @brief Security credential version present bit */
#define SECURITY_CREDENTIAL_VERSION_PRESENT     0x01

/** @brief Device specific file bit */
#define DEVICE_SPECIFIC_FILE                    0x02

/** @brief hardware version present bit */
#define HARDWARE_VERSION_PRESENT                0x04

/** @brief OTA header size + hash data */
#define IMAGE_OVERHEAD_SIZE                     88

#if defined(GP_DIVERSITY_KEEP_NRT_IN_FLASH)
// No Jumptables, nrt is in flash
#define IMAGE_OFFSET                            IMAGE_OVERHEAD_SIZE
#elif defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
/* Compression case: 
 *      - jumptables are the first GP_UPGRADE_APP_JUMP_TABLE_SIZE bytes of the OTA zigbee image, 
        - the actual application follows with loaded user license
*/
#define IMAGE_OFFSET                            IMAGE_OVERHEAD_SIZE + GP_UPGRADE_APP_JUMP_TABLE_SIZE
#else
/* Non Compression case: 
 *      - jumptables are the first GP_UPGRADE_APP_JUMP_TABLE_SIZE bytes of the OTA zigbee image, padding zeros added after jumptables
        - the actual application with loaded user license are behind jumptables and padding zeros.
*/
#define IMAGE_OFFSET                            IMAGE_OVERHEAD_SIZE + (gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress()))
#endif //GP_DIVERSITY_KEEP_NRT_IN_FLASH

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

// class for AES Rjindael calculation
CRijndaelAES AES_calc;

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterOTAUpgradeXp4001

// Default number of OTA download page to trigger the save point for OTA resume
__COMPACT_WEAK unsigned char CQorvoClusterOTAUpgradeXp4001::m_nNumberOfPageTriggerSave = 100;

CQorvoClusterOTAUpgradeXp4001::CQorvoClusterOTAUpgradeXp4001
    (CZigBeeFoundationApplication &application) :
CZigBeeClusterOTAUpgrade(application),
dwMStoredFileVersion(0),
dwMStoredFileOffset(0),
dwMStoredCRC(0),
dwMStoredProcessed(0)
{
    m_nOffset = 0;
    m_imageSize = 0;
    memset(dwMStoredHash,0,sizeof(dwMStoredHash));
    memset(dwMExpandedKey,0,sizeof(dwMExpandedKey));
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterOTAUpgradeXp4001 attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterOTAUpgradeXp4001, CZigBeeClusterOTAUpgrade)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterOTAUpgradeXp4001)
    __COMPACT7B_OTA_CLUSTER_M_ATTRIBUTES()
__COMPACT7B_END_ATTRIBUTE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterOTAUpgrade overrides
bool CQorvoClusterOTAUpgradeXp4001::OnImageHeaderAvailable()
{
    UInt8 mandatoryHeaderFields[MANDATORY_HEADER_SIZE] = {0};
    CZigBeeSecurityMMO calcHash;
    unsigned int dwAppImgSize;

    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnImageHeaderAvailable\n",0);

    // Execute the OTA base class
    if(!CZigBeeClusterOTAUpgrade::OnImageHeaderAvailable())
    {
        return false;
    }

    GP_LOG_SYSTEM_PRINTF("  MfgID: 0x%x\n",0, m_pHeader->m_wManufacturerID);
    GP_LOG_SYSTEM_PRINTF("  Image Type: 0x%x\n",0, m_pHeader->m_wImageType);
    GP_LOG_SYSTEM_PRINTF("  File version: 0x%08lx\n",0, m_pHeader->m_dwFileVersion);
    GP_LOG_SYSTEM_PRINTF("  Image Size: %ld\n",0, m_pHeader->m_dwImageSize);
    
#ifdef GP_DIVERSITY_BOOTLOADER
    dwAppImgSize = m_pHeader->m_dwImageSize;
    
    // Calculate the actual OTA application size
    if (dwAppImgSize > IMAGE_OFFSET)
    {
        dwAppImgSize -= IMAGE_OFFSET;
        GP_LOG_SYSTEM_PRINTF("  Required flash size: %d\n",0, dwAppImgSize);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("  Warning: OTA Image total size %lx < IMAGE_OFFSET %x\n",0, m_pHeader->m_dwImageSize, IMAGE_OFFSET);
        dwAppImgSize = 0xFFFFFFFF;
    }

    // Check if there is enough memory in Flash
    if(dwAppImgSize > gpUpgrade_GetOtaAreaSize())
    {
        if (dwAppImgSize == 0xFFFFFFFF)
        {
            GP_LOG_SYSTEM_PRINTF("OTA Xp4001: Error, Invalid required size\n",0);
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("OTA Xp4001: Error, lack of OTA region(flash) %d (OTA max size = %ld)\n",0,
                        dwAppImgSize,
		                gpUpgrade_GetOtaAreaSize());
        }

        // Notify the server that we do not accept this image
        RequestUpgradeEnd(statusInvalidImage);

        // Aborts this download
        Abort();
        return false;
    }

    gpUpgrade_StartWrite();
#endif //GP_DIVERSITY_BOOTLOADER

    {
        //Workaround for warning on copying unaligned structure members
        unsigned int dwFileIdentifier       = m_pHeader->m_dwFileIdentifier;
        unsigned short wHeaderVersion       = m_pHeader->m_wHeaderVersion;
        unsigned short wHeaderSize          = m_pHeader->m_wHeaderSize;
        unsigned short wHeaderFieldControl  = m_pHeader->m_wHeaderFieldControl;
        unsigned short wManufacturerID      = m_pHeader->m_wManufacturerID;
        unsigned short wImageType           = m_pHeader->m_wImageType;
        unsigned int dwFileVersion          = m_pHeader->m_dwFileVersion;
        unsigned short wZigBeeStackVersion  = m_pHeader->m_wZigBeeStackVersion;
        unsigned int dwImageSize            = m_pHeader->m_dwImageSize;

        //Reconstruction of header payload
        memcpy(&mandatoryHeaderFields[0],  &dwFileIdentifier, sizeof(m_pHeader->m_dwFileIdentifier));
        memcpy(&mandatoryHeaderFields[4],  &wHeaderVersion, sizeof(m_pHeader->m_wHeaderVersion));
        memcpy(&mandatoryHeaderFields[6],  &wHeaderSize, sizeof(m_pHeader->m_wHeaderSize));
        memcpy(&mandatoryHeaderFields[8],  &wHeaderFieldControl, sizeof(m_pHeader->m_wHeaderFieldControl));
        memcpy(&mandatoryHeaderFields[10], &wManufacturerID, sizeof(m_pHeader->m_wManufacturerID));
        memcpy(&mandatoryHeaderFields[12], &wImageType, sizeof(m_pHeader->m_wImageType));
        memcpy(&mandatoryHeaderFields[14], &dwFileVersion, sizeof(m_pHeader->m_dwFileVersion));
        memcpy(&mandatoryHeaderFields[18], &wZigBeeStackVersion, sizeof(m_pHeader->m_wZigBeeStackVersion));
        memcpy(&mandatoryHeaderFields[20], &m_pHeader->m_achDescription, sizeof(m_pHeader->m_achDescription));
        memcpy(&mandatoryHeaderFields[52], &dwImageSize, sizeof(m_pHeader->m_dwImageSize));
    }
    
    // restore Qorvo OTA Mfg Specific Attribute
    const CZigBeeAttribute *const pMProcessed = FindAttribute(attributeMStoredProcessed,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    const CZigBeeAttribute *const pMHash = FindAttribute(attributeMStoredHash,
                        CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);

    pMProcessed->Load(*this);
    pMHash->Load(*this);

    // check whether part of image has already been downloaded (& stored)
    if(dwMStoredProcessed == 0)
    {
        // Initialize the Hash and the count of processed data.
        calcHash.InitializeHash(AES_calc,dwMStoredHash, dwMStoredProcessed);

        // Update hash with the OTA Header(mandatory)
        calcHash.UpdateHash(AES_calc, dwMStoredHash, dwMStoredProcessed, mandatoryHeaderFields, sizeof(mandatoryHeaderFields));

        GP_LOG_PRINTF("  Updated Hash(%d) - OTA mandatory header:",0, dwMStoredProcessed);
        for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
        {
            GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
        }
        GP_LOG_PRINTF("\n",0);

        GP_LOG_PRINTF("  Updated ExpandedKey(%d), Key: \n",0, CRijndaelAES::expandedKeySize);
#ifdef GP_LOCAL_LOG
        gpLog_PrintBuffer(CRijndaelAES::expandedKeySize, AES_calc.m_abExpandedKey);
#endif //GP_LOCAL_LOG
        GP_LOG_PRINTF("\n",0);
    }
    return true;
}

void CQorvoClusterOTAUpgradeXp4001::OnBeginElement()
{
    UInt8 optionalHeaderFields[19] = {0};   // this includes element ID and length of it (13 + 6)
    UInt8 headerFieldLength = 0;            // counter for keeping the optional fields count
    CZigBeeSecurityMMO calcHash;
    unsigned short transfer = 0;

    //Image block is available - request data transfer.
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnBeginElement, Element starts at 0x%x, TagId: 0x%04x, element size %d Bytes\n",0,
                        m_nFileOffset,m_wElementTagID,m_dwElementSize);

    //request firmware data if element Tag id is 0(Upgrade Image) and size is not zero
    if( ( m_wElementTagID == 0x0000 ) && (m_dwElementSize) )
    {
        //restore Qorvo OTA Mfg Specific Attribute - previously stored file offset and version if any
        unsigned int requestAddress = 0;
        const CZigBeeAttribute *const pMFileVersion = FindAttribute(attributeMStoredFileVersion,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
        const CZigBeeAttribute *const pMFileOffset = FindAttribute(attributeMStoredFileOffset,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);

        pMFileVersion->Load(*this);
        pMFileOffset->Load(*this);

        //if version attribute doesn't exists or don't match version offered by server
        //or no stored data is available, go to download full image
        if((dwMStoredFileVersion != m_pHeader->m_dwFileVersion ) || (!dwMStoredFileOffset))
        {
            //new image download, erase OTA Area
            GP_LOG_SYSTEM_PRINTF("  New image download, erase OTA flash region\n",0);

#ifdef GP_DIVERSITY_BOOTLOADER
            gpUpgrade_EraseOtaArea();
#endif
            // initialize current request OTA image's address
            requestAddress = m_nFileOffset;
            // initialize downloaded address
            m_nDownloadedImageSize = 0;

            //store firmware version into NVM to resume download after power cycle
            dwMStoredFileVersion = m_pHeader->m_dwFileVersion;
            SetAttributeValue(*pMFileVersion,&dwMStoredFileVersion, sizeof(unsigned int));
            pMFileVersion->Store(*this);
        }
        else
        {
            //stored version matches with current version, resume download from stored offset
            requestAddress = dwMStoredFileOffset;
            m_nDownloadedImageSize = dwMStoredFileOffset - m_nFileOffset;
            
            GP_LOG_SYSTEM_PRINTF("  Resume download, version: 0x%08x, File Offset: 0x%x, Downloaded Image Size %d \n",0
                ,dwMStoredFileVersion
                ,dwMStoredFileOffset
                ,m_nDownloadedImageSize);
            
#ifdef GP_DIVERSITY_BOOTLOADER
            // load stored CRC from attribute
            const CZigBeeAttribute *const pMCRC = FindAttribute(attributeMStoredCRC,
                        CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
            pMCRC->Load(*this);

            GP_LOG_PRINTF("  - Stored CRC 0x%x \n",0,dwMStoredCRC);
            gpUpgrade_SetCrc(dwMStoredCRC);

            //Load stored hash information from attribute
            const CZigBeeAttribute *const pMHash = FindAttribute(attributeMStoredHash,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
            const CZigBeeAttribute *const pMProcessed = FindAttribute(attributeMStoredProcessed,
                                CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
            const CZigBeeAttribute *const pMExpandedKey = FindAttribute(attributeMExpandedKey,
                                CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);

            pMHash->Load(*this);
            pMProcessed->Load(*this);
            pMExpandedKey->Load(*this);

            GP_LOG_PRINTF("  - Loaded Hash(%d):",0, dwMStoredProcessed);
            for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
            {
                GP_LOG_PRINTF("%x ",0 ,dwMStoredHash[hashByte]);
            }
            GP_LOG_PRINTF("\n",0);
#endif //GP_DIVERSITY_BOOTLOADER
            // stored hash key initialization
            //AES_calc.ExpandKey(dwMStoredHash);
            memcpy(AES_calc.m_abExpandedKey, dwMExpandedKey+1, dwMExpandedKey[0]);

            GP_LOG_PRINTF("  - Loaded ExpandKey(%d): \n",0, CRijndaelAES::expandedKeySize);
#ifdef GP_LOCAL_LOG
            gpLog_PrintBuffer(CRijndaelAES::expandedKeySize, AES_calc.m_abExpandedKey);
#endif //GP_LOCAL_LOG
            GP_LOG_PRINTF("\n",0);
        }

        if(m_nDownloadedImageSize==0)
        {
            // Determine the optional OTA header 
            if(m_pHeader->m_wHeaderFieldControl & SECURITY_CREDENTIAL_VERSION_PRESENT)
            {
                // TODO: add security credential version here (used in SE profile)
            }
            if(m_pHeader->m_wHeaderFieldControl & DEVICE_SPECIFIC_FILE)
            {
                // TODO: add device specific file here (IEEE ID)
            }
            if(m_pHeader->m_wHeaderFieldControl & HARDWARE_VERSION_PRESENT)
            {
                memcpy(&optionalHeaderFields[headerFieldLength], &m_pHeader->GetMinimumHardwareVersion(), sizeof(unsigned short));
                headerFieldLength += 2;
                memcpy(&optionalHeaderFields[headerFieldLength], &m_pHeader->GetMaximumHardwareVersion(), sizeof(unsigned short));
                headerFieldLength += 2;
            }

            // Determine the sub-element header - TagID, Length
            memcpy(&optionalHeaderFields[headerFieldLength], &m_wElementTagID, sizeof(m_wElementTagID));
            headerFieldLength += sizeof(m_wElementTagID);
            memcpy(&optionalHeaderFields[headerFieldLength], &m_dwElementSize, sizeof(m_dwElementSize));
            headerFieldLength += sizeof(m_dwElementSize);

            // Update the Hash with the optional OTA header & sub-element header
            calcHash.UpdateHash(AES_calc, dwMStoredHash, dwMStoredProcessed, optionalHeaderFields, headerFieldLength);

            GP_LOG_PRINTF("  Updated Hash(%d) - sub-element header:",0, dwMStoredProcessed);
            for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
            {
                GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
            }
            GP_LOG_PRINTF("\n",0);

            GP_LOG_PRINTF("  Updated ExpandedKey(%d), Key: \n",0, CRijndaelAES::expandedKeySize);
#ifdef GP_LOCAL_LOG
            gpLog_PrintBuffer(CRijndaelAES::expandedKeySize, AES_calc.m_abExpandedKey);
#endif //GP_LOCAL_LOG
            GP_LOG_PRINTF("\n",0);
        }

        // Request record with the max data request size
        transfer = (( m_dwElementSize - m_nDownloadedImageSize ) > MAX_DATA_REQUEST_SIZE ) ? MAX_DATA_REQUEST_SIZE : (m_dwElementSize - m_nDownloadedImageSize);

        RequestDataTransfer(requestAddress,
                            transfer,
                (( m_dwElementSize - m_nDownloadedImageSize) > MAX_DATA_REQUEST_SIZE ) ?
                        static_cast<CZigBeeOTATransfer::CALLBACK>
                        (&CQorvoClusterOTAUpgradeXp4001::OnGetData) :
                        static_cast<CZigBeeOTATransfer::CALLBACK>
                        (&CQorvoClusterOTAUpgradeXp4001::OnDataComplete));

        // update OTA image total size
        m_imageSize = m_dwElementSize;

        // update OTA Header total size( including sub-element header)
        m_nOffset = m_nFileOffset;
    }
    //request hash value if element Tag is 3(Image Integrity Code)
    else if( m_wElementTagID == 0x0003 )
    {
        RequestDataTransfer(m_nFileOffset,m_dwElementSize,
            static_cast<CZigBeeOTATransfer::CALLBACK>
            (&CQorvoClusterOTAUpgradeXp4001::OnGetHashValue));
    }
    else
    {
        //supporting only tag id 0 and 3
        EndElement();
    }
}

void CQorvoClusterOTAUpgradeXp4001::OnGetData(const unsigned char nStatus,
    const unsigned int nAddress, void *const pData,
    const unsigned short cbData)
{
    CZigBeeSecurityMMO calcHash;
    
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnGetData(%d): Downloaded(%d/%ld): %ld Percent\n",0
        ,cbData
        ,m_nDownloadedImageSize
        // OTA Data = Total size - OTA header(including 1st sub-element) size - Last sub-element header size - Last sub-element data size(Hash)
        ,m_pHeader->m_dwImageSize - m_nOffset - HASH_KEY_SIZE - TAGID_AND_LENGTH_OF_TAGID
        ,(m_nDownloadedImageSize *100) / (m_pHeader->m_dwImageSize - m_nOffset));

    //If transfer failed, abort the whole process
    if (nStatus != statusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("  Error: Status: 0x%x, Get data failed, aborting upgrade!\n",0, nStatus);

        //Abort download, revert to idle state and schedule next query
        Abort();

        return;
    }
    
    //update downloaded image size
    m_nDownloadedImageSize = m_nDownloadedImageSize + cbData;
    
    GP_LOG_PRINTF("  Current Hash: ",0);
    for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
    {
        GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
    }
    GP_LOG_PRINTF("\n",0);
    
    // update hash calculation
    calcHash.UpdateHash(AES_calc, dwMStoredHash, dwMStoredProcessed, pData, cbData);

    GP_LOG_PRINTF("  Updated Hash: ",0);
    for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
    {
        GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
    }
    GP_LOG_PRINTF("\n",0);

    //store data - write to flash
#ifdef GP_DIVERSITY_BOOTLOADER
    unsigned short cbWdata = cbData;
    unsigned char* pWdata = (unsigned char*)pData;
#if defined(GP_DIVERSITY_KEEP_NRT_IN_FLASH)
    unsigned int nStartAddress = gpUpgrade_GetOtaAreaStartAddress();
#else
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    unsigned int nStartAddress = gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_SIZE;
#else
    unsigned int nStartAddress = GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress());

    //Write to jump table area
    if ((nAddress - m_nOffset) < GP_UPGRADE_APP_JUMP_TABLE_SIZE)
    {
        if ((nAddress - m_nOffset + cbData) > GP_UPGRADE_APP_JUMP_TABLE_SIZE)
        {
            cbWdata -= (nAddress - m_nOffset + cbData) - GP_UPGRADE_APP_JUMP_TABLE_SIZE;
        }
    }
    //Write to application area
    else if ((nAddress - m_nOffset + cbData) > (gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress())))
    {
        if ((nAddress - m_nOffset) < (gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress())))
        {
            cbWdata = (nAddress - m_nOffset + cbData) - (gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress()));
            nStartAddress += cbData - cbWdata;
            pWdata += cbData - cbWdata;
        }
    }
    //Skip writing of the area between jump table and application
    else
    {
        cbWdata = 0;
    }
#endif
#endif

    if (cbWdata && gpUpgrade_WriteChunk(nStartAddress + nAddress - m_nOffset,
                                        cbWdata,
                                        pWdata) != gpUpgrade_StatusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("  Error: Flash write failed! OTA image address: 0x%x -> DUT Address: 0x%x, length %d",0,
            nAddress - m_nOffset,
            nStartAddress + nAddress - m_nOffset,
            cbWdata);

        //Abort download, revert to idle state and schedule next query
        Abort();
        return;
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("  Flash Write, OTA image address: 0x%x -> DUT Address: 0x%x, length %d\n ",0,
            nAddress - m_nOffset,
            nStartAddress + nAddress - m_nOffset,
            cbWdata);
    }
#endif //GP_DIVERSITY_BOOTLOADER

    //store crc, file offset Hash when received amount of bytes.
    if((m_nDownloadedImageSize - dwMStoredFileOffset) > (MAX_DATA_REQUEST_SIZE * m_nNumberOfPageTriggerSave))
    {
        GP_LOG_SYSTEM_PRINTF("  OTA resume: reached save point(%d x %d pages), save current OTA progress... \n",0
            ,m_nDownloadedImageSize/(MAX_DATA_REQUEST_SIZE * m_nNumberOfPageTriggerSave) 
            ,m_nNumberOfPageTriggerSave);
#ifdef GP_DIVERSITY_BOOTLOADER
        //get CRC and store into attribute
        const CZigBeeAttribute *const pMCRC = FindAttribute(attributeMStoredCRC,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
        dwMStoredCRC = gpUpgrade_GetCrc();
        SetAttributeValue(*pMCRC,&dwMStoredCRC, sizeof(unsigned int));
        pMCRC->Store(*this);
        GP_LOG_PRINTF("  OTA resume: CRC 0x%x \n",0,dwMStoredCRC);
#endif
        //store file offset to resume download after power cycle
        const CZigBeeAttribute *const pMFileOffset = FindAttribute(attributeMStoredFileOffset,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
        dwMStoredFileOffset = nAddress + cbData;
        SetAttributeValue(*pMFileOffset,&dwMStoredFileOffset, sizeof(unsigned int));
        pMFileOffset->Store(*this);
        
        // store hash into NVM
        const CZigBeeAttribute *const pMHash = FindAttribute(attributeMStoredHash,
                    CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
        SetAttributeValue(*pMHash,&dwMStoredHash, sizeof(dwMStoredHash));
        pMHash->StoreEx(*this,&dwMStoredHash,HASH_KEY_SIZE);
        
        // store AES expended key 
        const CZigBeeAttribute *const pMExpandedKey = FindAttribute(attributeMExpandedKey,
                        CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);              
        memcpy(dwMExpandedKey+1, AES_calc.m_abExpandedKey, CRijndaelAES::expandedKeySize);
        // -- set array length for the first octet
        dwMExpandedKey[0] = CRijndaelAES::expandedKeySize;
        SetAttributeValue(*pMExpandedKey,&dwMExpandedKey, CRijndaelAES::expandedKeySize);
        pMExpandedKey->StoreEx(*this,&dwMExpandedKey,CRijndaelAES::expandedKeySize);
        
        // store processed number of data into NVM
        const CZigBeeAttribute *const pMProcessed = FindAttribute(attributeMStoredProcessed,
                        CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
        SetAttributeValue(*pMProcessed,&dwMStoredProcessed, sizeof(dwMStoredProcessed));
        pMProcessed->Store(*this);

        GP_LOG_SYSTEM_PRINTF("  OTA resume: FileOffset: 0x%x(%d Bytes is OTA header)\n",0,dwMStoredFileOffset,m_nOffset);
        GP_LOG_SYSTEM_PRINTF("  OTA resume: Processed: %d\n",0,dwMStoredProcessed);
        GP_LOG_PRINTF("  OTA resume: Hash: ",0);
        for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
        {
            GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
        }
        GP_LOG_PRINTF("\n",0);  
        GP_LOG_PRINTF("  OTA resume: dwMExpandedKey(%d), Key: \n",0, dwMExpandedKey[0]);
#ifdef GP_LOCAL_LOG
        gpLog_PrintBuffer(CRijndaelAES::expandedKeySize, dwMExpandedKey+1);
#endif //GP_LOCAL_LOG
    }

    //request next record
    RequestDataTransfer(nAddress + cbData,
            (( m_dwElementSize - m_nDownloadedImageSize ) > MAX_DATA_REQUEST_SIZE ) ? MAX_DATA_REQUEST_SIZE : (m_dwElementSize - m_nDownloadedImageSize),
            (( m_dwElementSize - m_nDownloadedImageSize) > MAX_DATA_REQUEST_SIZE ) ?
                    static_cast<CZigBeeOTATransfer::CALLBACK>
                    (&CQorvoClusterOTAUpgradeXp4001::OnGetData) :
                    static_cast<CZigBeeOTATransfer::CALLBACK>
                    (&CQorvoClusterOTAUpgradeXp4001::OnDataComplete));
}

void CQorvoClusterOTAUpgradeXp4001::OnDataComplete(const unsigned char nStatus,
    const unsigned int nAddress, void *const pData,
    const unsigned short cbData)
{
    CZigBeeSecurityMMO calcHash;
    
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnDataComplete, status 0x%x\n",0
        ,nStatus);
        
    //If transfer failed, abort the whole process
    if (nStatus != statusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("  Error: Get data failed, aborting upgrade!\n",0);

        //Abort download, revert to idle state and schedule next query
        Abort();

        return;
    }
    // update hash calculation
    calcHash.UpdateHash(AES_calc, dwMStoredHash, dwMStoredProcessed, pData, cbData);

    GP_LOG_PRINTF("  Updated Hash(%d) - OnDataComplete:",0, dwMStoredProcessed);
    for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
    {
        GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
    }
    GP_LOG_PRINTF("\n",0);

#ifdef GP_DIVERSITY_BOOTLOADER
    //store data - write to flash
#if defined(GP_DIVERSITY_KEEP_NRT_IN_FLASH)
    unsigned int nStartAddress = gpUpgrade_GetOtaAreaStartAddress();
#else
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    unsigned int nStartAddress = gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_SIZE;
#else
    unsigned int nStartAddress = GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress());
#endif
#endif
    if (gpUpgrade_WriteChunk(nStartAddress + nAddress - m_nOffset, cbData, (UInt8*)pData) != gpUpgrade_StatusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("  Error: Flash write failed! OTA image address: 0x%x -> DUT Address: 0x%x, length %d",0,
            nAddress - m_nOffset,
            nStartAddress + nAddress - m_nOffset,
            cbData);
        //Abort download, revert to idle state and schedule next query
        Abort();

        return;
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("  Flash Write, OTA image address: 0x%x -> DUT: 0x%x, length %d\n ",0,
            nAddress - m_nOffset,
            nStartAddress + nAddress - m_nOffset,
            cbData);
    }
#endif

    GP_LOG_SYSTEM_PRINTF("  Download Completed, start receiving next OTA sub-element...\n",0);
     
    //We are done with this element. Continue with the next OTA sub-element
    EndElement();
}

void CQorvoClusterOTAUpgradeXp4001::OnGetHashValue(const unsigned char nStatus,
    const unsigned int nAddress, void *const pData,
    const unsigned short cbData)
{
    CZigBeeSecurityMMO calcHash;
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnGetHashValue, status 0x%x, address 0x%x, length %d\n",0
        ,nStatus
        ,nAddress
        ,cbData);

    //If the transfer failed, abort the whole process
    if (nStatus != statusSuccess)
    {
        GP_LOG_SYSTEM_PRINTF("  Error: get hash data failed, aborting upgrade!\n",0);

        //Abort download, revert to idle state and schedule next query
        Abort();

        return;
    }

    calcHash.FinalizeHash(AES_calc,dwMStoredHash,dwMStoredProcessed);

    GP_LOG_PRINTF("  Finalize Hash(%d):",0, dwMStoredProcessed);
    for (UInt8 hashByte = 0; hashByte < sizeof(dwMStoredHash); hashByte++)
    {
        GP_LOG_PRINTF("%02x ",0 ,dwMStoredHash[hashByte]);
    }
    GP_LOG_PRINTF("\n",0);

    // Comparing received and generated hash values
    if(0 != memcmp(pData,dwMStoredHash,sizeof(dwMStoredHash)))
    {
        GP_LOG_SYSTEM_PRINTF("  Error: Hash verification failed!\n",0);
        EraseOTARestorePoints();

        // Notify the server that we do not accept this image
        RequestUpgradeEnd(statusInvalidImage);

        Abort();
        return;
    }

    //We are done with this element. Continue with image complete
    EndElement();
}

bool CQorvoClusterOTAUpgradeXp4001::OnImageComplete()
{
    //Image download complete, verify downloaded image - crc check, hash value check etc.
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnImageComplete\n", 0);

#ifdef GP_DIVERSITY_BOOTLOADER
#if defined(GP_DIVERSITY_KEEP_NRT_IN_FLASH)
    GP_LOG_SYSTEM_PRINTF(" Set pending image, address: 0x%lx, size: %d\n",0,
                         gpUpgrade_GetOtaAreaStartAddress(), m_imageSize);
#else
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
    GP_LOG_SYSTEM_PRINTF("  Set pending image, address: 0x%lx, size: %d\n",0,
                         gpUpgrade_GetOtaAreaStartAddress(), m_imageSize - GP_UPGRADE_APP_JUMP_TABLE_SIZE);
#else
    GP_LOG_SYSTEM_PRINTF("  Set pending image, address: 0x%lx size: %d\n",0,
                         gpUpgrade_GetOtaAreaStartAddress(), m_imageSize-(gpUpgrade_GetOtaAreaStartAddress()-GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress())));
#endif
#endif
    gpLog_Flush();
    gpUpgrade_Status_t status;
    status = gpUpgrade_SetPendingImage(m_pHeader->m_dwFileVersion,
                                       1 /* optional HW version */,
                                       gpUpgrade_GetOtaAreaStartAddress(),
#if defined(GP_DIVERSITY_KEEP_NRT_IN_FLASH)
                                       m_imageSize
#else
#if defined(GP_UPGRADE_DIVERSITY_COMPRESSION)
                                       m_imageSize - GP_UPGRADE_APP_JUMP_TABLE_SIZE
#else
                                       m_imageSize - (gpUpgrade_GetOtaAreaStartAddress() - GP_UPGRADE_APP_JUMP_TABLE_ADDR(gpUpgrade_GetOtaAreaStartAddress()))
#endif
#endif
                                       );
    if(status !=  gpUpgrade_StatusSuccess)
    {
        //Setting pending image in bootloader failed
        GP_LOG_SYSTEM_PRINTF("  Error: Set pending image failed, status:%x\n",0, status);
        // Start from scratch
        EraseOTARestorePoints();
        // Notify the server that we do not accept this image
        RequestUpgradeEnd(statusInvalidImage);

        Abort();
    }
#endif

    return true;
}

void CQorvoClusterOTAUpgradeXp4001::OnApply()
{
    GP_LOG_SYSTEM_PRINTF("OTA Xp4001: OnApply\n",0);
    gpLog_Flush();

    //clear all stored attributes
    EraseOTARestorePoints();

    //Call bootloader to apply new image and reset after download
#ifdef GP_DIVERSITY_BOOTLOADER
    gpSched_ScheduleEvent(5000000, gpUpgrade_Reset);
#endif
}

void CQorvoClusterOTAUpgradeXp4001::OnAbort()
{
    GP_LOG_SYSTEM_PRINTF(__FUNCTION__,0);
    GP_LOG_SYSTEM_PRINTF("\n",0);
}

void CQorvoClusterOTAUpgradeXp4001::EraseOTARestorePoints()
{
    // erase stored CRC
    const CZigBeeAttribute *const pMCRC = FindAttribute(attributeMStoredCRC,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    dwMStoredCRC = 0;
    SetAttributeValue(*pMCRC,&dwMStoredCRC, sizeof(unsigned int));
    pMCRC->Store(*this);
    // erase stored file version
    const CZigBeeAttribute *const pMFileVersion = FindAttribute(attributeMStoredFileVersion,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    dwMStoredFileVersion = 0;
    SetAttributeValue(*pMFileVersion,&dwMStoredFileVersion, sizeof(unsigned int));
    pMFileVersion->Store(*this);
    //erase file offset to resume download after power cycle
    const CZigBeeAttribute *const pMFileOffset = FindAttribute(attributeMStoredFileOffset,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    dwMStoredFileOffset = 0;
    SetAttributeValue(*pMFileOffset,&dwMStoredFileOffset, sizeof(unsigned int));
    pMFileOffset->Store(*this);

    // erase hash into NVM
    const CZigBeeAttribute *const pMHash = FindAttribute(attributeMStoredHash,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    memset(dwMStoredHash,0,sizeof(dwMStoredHash));
    SetAttributeValue(*pMHash,&dwMStoredHash, sizeof(dwMStoredHash));
    pMHash->StoreEx(*this,&dwMStoredHash,HASH_KEY_SIZE);

    // erase processed number of data into NVM
    const CZigBeeAttribute *const pMProcessed = FindAttribute(attributeMStoredProcessed,
                            CZigBeeAttribute::manufacturerSpecific,CZigBeeDeviceObject::wManufacturerID);
    dwMStoredProcessed = 0;
    SetAttributeValue(*pMProcessed,&dwMStoredProcessed, sizeof(dwMStoredProcessed));
    pMProcessed->Store(*this);
}
