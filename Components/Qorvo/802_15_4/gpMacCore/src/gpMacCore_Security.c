// enable security.c
/*
 * Copyright (c) 2012-2016, GreenPeak Technologies
 * Copyright (c) 2017, Qorvo Inc
 *
 * gpMacCore_Security.c
 *   This file contains the implementation of the security functions for non beaconed MAC protocol.
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
 * Alternatively, this software may be distributed under the terms of the
 * modified BSD License or the 3-clause BSD License as published by the Free
 * Software Foundation @ https://directory.fsf.org/wiki/License:BSD-3-Clause
 *
 *
 */

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

// General includes
#include "gpPd.h"
#include "gpHal.h"
#include "gpMacCore.h"
#include "gpMacCore_defs.h"


/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define GP_COMPONENT_ID GP_COMPONENT_ID_MACCORE


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

#if GP_MACCORE_NUMBER_OF_KEY_DEVICE_DESCRIPTORS>16
#error if more keydevicedescriptors, the decryption can not store the correct keydevicedescriptor
#endif //GP_MACCORE_NUMBER_OF_KEY_DEVICE_DESCRIPTORS


#if GP_MACCORE_NUMBER_OF_KEY_DESCRIPTOR_ENTRIES>16
#error if more keydescriptors, the decryption can not store the correct keydescriptor
#endif //GP_MACCORE_NUMBER_OF_KEY_DESCRIPTOR_ENTRIES

#if GP_MACCORE_NUMBER_OF_DEVICE_DESCRIPTORS>255
#error if more devicedescriptors, the decryption can not store the correct devicedescriptor
#endif //GP_MACCORE_NUMBER_OF_DEVICE_DESCRIPTORS


/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External Data Definition
 *****************************************************************************/

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static void MacCore_ConstructNonce(MACAddress_t* srcAddr, UInt32 frameCounter, gpEncryption_SecLevel_t securityLevel, UInt8* pNonce);
static Bool MacCore_SecurityLevelSubfieldCompair( gpEncryption_SecLevel_t sec1 , gpEncryption_SecLevel_t sec2);
static Bool MacCore_OutgoingFrameKeyRetrievalProcedure(const gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacCore_Security_t* pSecOptions, gpPd_Loh_t* p_PdLoh, UInt8* pKey , gpMacCore_StackId_t stackId);
static gpMacCore_KeyTablesEntries_t MacCore_KeyDescriptorLookup( gpMacCore_KeyIdLookupDescriptor_t *pKeyIdLookupDescriptor , gpMacCore_KeyDescriptor_t *pKeyDescriptor );
static Bool MacCore_DeviceDescriptorLookup( gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_DeviceLookupDescriptor_t *pDeviceLookupDescriptor );
static MacCore_SecurityLevelPassed_t MacCore_IncomingSecurityLevelCheckingProcedure(gpEncryption_SecLevel_t securityLevel , gpMacCore_FrameType_t frameType , gpMacCore_Command_t commandFrameIdentifier );
static Bool MacCore_IncomingKeyUsagePolicyCheckingProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_FrameType_t frametype , gpMacCore_Command_t commandFrameIdentifier );
static Bool MacCore_IncomingFrameSecurityRetrievalProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_KeyDeviceDescriptor_t *pKeyDeviceDescriptor , MacCore_HeaderDescriptor_t *pDataIndicationMacValues);
static gpMacCore_KeyDeviceListEntries_t MacCore_BlacklistCheckingProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_DeviceLookupDescriptor_t *pDeviceLookupDescriptor , gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_KeyDeviceDescriptor_t *pKeyDeviceDescriptor );
static void MacCore_WriteAuxSecHeaderInPd(UInt32 frameCounter, gpMacCore_Security_t* pSecOptions, gpPd_Loh_t* p_PdLoh);
static void MacCore_EncryptPacket(MACAddress_t* pExtendedAddress, UInt32 frameCounter, gpEncryption_SecLevel_t securityLevel, UInt8* pKey, UInt8 headerLength, UInt8 dataLength, gpPd_Loh_t* p_PdLoh, gpMacCore_StackId_t stackId);

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/

void MacCore_WriteAuxSecHeaderInPd(UInt32 frameCounter, gpMacCore_Security_t* pSecOptions, gpPd_Loh_t* p_PdLoh)
{
    UInt8 securityControl=0;

    if(gpMacCore_KeyIdModeImplicit != pSecOptions->keyIdMode)
    {
        gpPd_PrependWithUpdate(p_PdLoh, 1, &pSecOptions->keyIndex);
    }
    switch (pSecOptions->keyIdMode)
    {
        case gpMacCore_KeyIdModeImplicit:
        {
            break;
        }
        case gpMacCore_KeyIdModeExplicit1Octet:
        {
            break;
        }
        case gpMacCore_KeyIdModeExplicit4Octet:
        {
            MacCore_PrependStreamAndUpdatePd((UInt8*)&pSecOptions->pKeySource.pKeySource4, 4, p_PdLoh);
            break;
        }
        case gpMacCore_KeyIdModeExplicit8Octet:
        {
            MacCore_PrependStreamAndUpdatePd((UInt8*)&pSecOptions->pKeySource.pKeySource8, 8, p_PdLoh);
            break;
        }
        default:
        {
            //not possible, covered in MacCore_PrepareOutgoingSecuredPacket
            break;
        }
    }

    //Add FrameCounter
    MacCore_PrependStreamAndUpdatePd((UInt8*)&frameCounter, 4, p_PdLoh);

    //Add Security control Field: SecurityLevel || KeyIdMode
    MACCORE_SECCONTROL_SECLEVEL_SET(securityControl, pSecOptions->securityLevel);
    MACCORE_SECCONTROL_KEYIDMODE_SET(securityControl, pSecOptions->keyIdMode);
    gpPd_PrependWithUpdate(p_PdLoh, 1, &securityControl);
}

//create nonce: big endian srcAddress || big endian framecounter || security level
void MacCore_ConstructNonce(MACAddress_t* srcAddr, UInt32 frameCounter, gpEncryption_SecLevel_t securityLevel, UInt8* pNonce)
{
    MEMCPY(pNonce, srcAddr ,GP_MACCORE_NONCE_LENGTHOF_ADDRESS);
    HOST_TO_BIG_UINT64(pNonce);
    HOST_TO_BIG_UINT32(&frameCounter);
    MEMCPY(&pNonce[GP_MACCORE_NONCE_STARTOF_FRAME_COUNTER],&frameCounter,GP_MACCORE_NONCE_LENGTHOF_FRAME_COUNTER);
    pNonce[GP_MACCORE_NONCE_STARTOF_SEC_LEVEL] = securityLevel;
}

// In section 7.6.2.2.1 Security Level subfield: sec1 >= sec2 <=> b2 is bigger or equal and the integer value of b1b0 is bigger or equal
Bool MacCore_SecurityLevelSubfieldCompair( gpEncryption_SecLevel_t sec1 , gpEncryption_SecLevel_t sec2)
{
    // asserts should not occur, as security level is properly trimmed before
    GP_ASSERT_DEV_INT(sec1 <= gpEncryption_SecLevelENC_MIC128);
    GP_ASSERT_DEV_INT(sec2 <= gpEncryption_SecLevelENC_MIC128);

    return (((sec1 & 0x04) >= (sec2 & 0x04)) & ((sec1 & 0x03) >= (sec2 & 0x03)));
}

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

/*
 * This function more or less follows section 7.5.8.2.1 (outgoing frame security procedure) from the MAC-2006 spec.
 * However, this functions contains extra functionality at some places (and the steps from the spec might be reordered for clarity)
 * Also, this function will never be entered when the specified security level is zero
*/
gpMacCore_Result_t MacCore_PrepareOutgoingSecuredPacket(gpMacCore_AddressInfo_t* pSrcAddrInfo, gpMacCore_AddressInfo_t* pDstAddrInfo, gpPd_Loh_t *p_PdLoh, UInt8 txOptions, gpMacCore_Security_t *pSecOptions , gpMacCore_FrameType_t frameType, gpMacCore_StackId_t stackId )
{
    UInt8 key[16];
    // information about length of data and header in pd
    UInt8 headerLength = 0;
    UInt8 dataLength = 0;
    UInt32 frameCounter = gpMacCore_GetFrameCounter(stackId);

    if(pSecOptions->securityLevel > gpEncryption_SecLevelENC_MIC128 || pSecOptions->keyIdMode > gpMacCore_KeyIdModeExplicit8Octet)
    {
        return gpMacCore_ResultInvalidParameter;
    }
    // in section 7.5.8.2.1 Outgoing frame security procedure: c) macSecurityEnabled == FALSE and the security level != 0 ==> UNSUPPORTED_SECURITY.
    if(!gpMacCore_GetSecurityEnabled(stackId))
    {
        return gpMacCore_ResultUnsupportedSecurity;
    }

    // in section 7.5.8.2.1 Outgoing frame security procedure f)  If the frame counter == 0xffffffff, return with a status of COUNTER_ERROR.
    if(frameCounter == GP_MACCORE_FRAME_COUNTER_MAXIMUM)
    {
        return gpMacCore_ResultCounterError;
    }

    // in section 7.5.8.2.1 Outgoing frame security procedure:  g) call the outgoing frame key retrieval procedure as described in 7.5.8.2.2
    if(!MacCore_OutgoingFrameKeyRetrievalProcedure(pDstAddrInfo,pSecOptions, p_PdLoh, key,stackId))
    {
        return gpMacCore_ResultUnavailableKey;
    }
    headerLength = p_PdLoh->length ;//save length without header

    // in section 7.5.8.2.1 Outgoing frame security procedure: h) insert aux sec header into the frame
    MacCore_WriteAuxSecHeaderInPd(frameCounter, pSecOptions, p_PdLoh);

    // Write MAC header to the PD
    MacCore_WriteMacHeaderInPd(frameType, pSrcAddrInfo, pDstAddrInfo, txOptions, pSecOptions->securityLevel, p_PdLoh,stackId);
    if( p_PdLoh->length  > GP_MACCORE_MAX_PHY_PACKET_SIZE_NO_FCS )
    {
        return gpMacCore_ResultFrameTooLong;
    }
    // calculate the length of header and data (needed to distinguish between data for authentication and encryption)
    headerLength = p_PdLoh->length - headerLength;
    dataLength = p_PdLoh->length - headerLength;

    /* in case of a command frame, do not encrypt the command byte, but start on then byte after then command byte */
    if(frameType == gpMacCore_FrameTypeCommand)
    {
        headerLength += 1;
        dataLength -= 1;
    }

    // in section 7.5.8.2.1 Outgoing frame security procedure: i) do the encryption
    MacCore_EncryptPacket(&(pDstAddrInfo->address.Extended), frameCounter, pSecOptions->securityLevel, (UInt8*)&key, headerLength, dataLength, p_PdLoh, stackId);

    // in section 7.5.8.2.1 Outgoing frame security procedure j) increment framecounter.
    MacCore_SetFrameCounter(frameCounter + 1);

    // in section 7.5.8.2.1 Outgoing frame security procedure: k) return success (secured frame in pd).
    return gpMacCore_ResultSuccess;
}

void MacCore_EncryptPacket(MACAddress_t* pExtendedAddress, UInt32 frameCounter, gpEncryption_SecLevel_t securityLevel, UInt8* pKey, UInt8 headerLength, UInt8 dataLength, gpPd_Loh_t* p_PdLoh, gpMacCore_StackId_t stackId)
{
    gpEncryption_CCMOptions_t CCMOptions;
    UInt8 nonce[13];
    MACAddress_t address;
    gpHal_GetExtendedAddress(&address, stackId);

    MEMSET(&CCMOptions, 0, sizeof(gpEncryption_CCMOptions_t));
    // nonce construction
    MacCore_ConstructNonce(&address, frameCounter, securityLevel, nonce);

    //Set handle, nonce ,key and miclength
    CCMOptions.pdHandle = p_PdLoh->handle;
    CCMOptions.pNonce = nonce;
    CCMOptions.pKey = pKey;
    CCMOptions.micLength = GP_ENCRYPTION_SECLEVEL2MICLENGTH(securityLevel);

    /**
     * For encryption, 4 values need to be set (depending on the descurity level used. These are:
     *
     * auxOffset:  indicates the start of the authentication data (a data) in the pd. This is the first byte of the header in this case.
     * dataOffset: indicates the start of the data that needs to be encrypted (m data). In our case, this is the first byte of the MAC payload.
     * auxLength:  indicates the number of bytes that will be used to calculate the authentication data.
     * secLength:  indicates the number of bytes of the data that will be encrypted.
     *
     * Remark: when we do not need encryption (so m data is 0), we do need to specify secDataOffset to point AFTER the payload.
     *         Otherwise, the data will be (partly) overwritten with the MIC.
     */
    if(securityLevel == gpEncryption_SecLevelNothing)
    {
        // Should never occur as we check on valid security level before
    }
    else if(securityLevel<= gpEncryption_SecLevelMIC128)
    {
        // only authentication is used. a data is complete packet (MHR || AuxSecHader || Non-PayloadField || Unsecured PayloadField )and m data is nothing
        CCMOptions.dataOffset = p_PdLoh->offset + p_PdLoh->length;
        CCMOptions.dataLength = 0;
        CCMOptions.auxOffset = p_PdLoh->offset;
        CCMOptions.auxLength = p_PdLoh->length;
    }
    else if(securityLevel == gpEncryption_SecLevelENC)
    {
        // only encryption is used. a data is 0 and m data are the unsecured payload fiels (complete payload in case of a data command)
        CCMOptions.dataOffset = p_PdLoh->offset + headerLength;
        CCMOptions.dataLength = dataLength;
        CCMOptions.auxOffset = p_PdLoh->offset;
        CCMOptions.auxLength = 0;
    }
    else if(securityLevel <= gpEncryption_SecLevelENC_MIC128)
    {
        // use security and authentication. a data is MHR || AuxSecHeader || Non-PayloadField and m data is PayloadField
        CCMOptions.dataOffset = p_PdLoh->offset + headerLength;
        CCMOptions.dataLength = dataLength;
        CCMOptions.auxOffset = p_PdLoh->offset;
        CCMOptions.auxLength = headerLength;
    }
    else
    {
        //not possible, covered in MacCore_PrepareOutgoingSecuredPacket
    }

    // request encryption
    gpEncryption_CCMEncrypt(&CCMOptions);
    p_PdLoh->length += CCMOptions.micLength;
}

// in section 7.5.8.2.2 Outgoing frame key retrieval procedure
Bool MacCore_OutgoingFrameKeyRetrievalProcedure(const gpMacCore_AddressInfo_t* pDstAddrInfo, gpMacCore_Security_t* pSecOptions, gpPd_Loh_t* p_PdLoh, UInt8* pKey,gpMacCore_StackId_t stackId)
{
    /* in section 7.5.8.2.2 Outgoing frame key retrieval procedure
        The outgoing frame key retrieval procedure involves the following steps as applicable:
    */
    gpMacCore_KeyIdLookupDescriptor_t keyIdLookupDescriptor;
    gpMacCore_KeyDescriptor_t keyDescriptor;

    switch(pSecOptions->keyIdMode)
    {
        // in section 7.5.8.2.2 Outgoing frame key retrieval procedure a) KeyIdMode == 0x00 (implicit key identification)
        case gpMacCore_KeyIdModeImplicit:
        {
            switch (pDstAddrInfo->addressMode)
            {
                case gpMacCore_AddressModeNoAddress:
                {
                    UInt16 panCoordShortAddr = gpMacCore_GetPanCoordShortAddress();
                    if( panCoordShortAddr <= 0xFFFD )
                    {
                        // lookup data = |source pan (2)| + |Pan coord short address (2)| + |0x00 (1)|
                        UInt16 panId = gpMacCore_GetPanId(stackId);
                        MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &panId , 2);
                        MEMCPY( &keyIdLookupDescriptor.lookupData[2] , &panCoordShortAddr , 2);
                        keyIdLookupDescriptor.lookupData[4] = 0 ;
                        keyIdLookupDescriptor.lookupDataSize = 0;
                    }
                    else
                    {
                        if( panCoordShortAddr == 0xFFFE )
                        {
                            // lookup data = |Pan coord ext address (8)| + |0x00 (1)|
                            gpMacCore_Address_t panCoordAddress;
                            gpMacCore_GetPanCoordExtendedAddress( &panCoordAddress.Extended);
                            MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &panCoordAddress.Extended , 8);
                            keyIdLookupDescriptor.lookupData[8] = 0 ;
                            keyIdLookupDescriptor.lookupDataSize = 1;
                        }
                        else
                        {
                            GP_ASSERT_DEV_EXT(false);   // something is wrong in the security initialisation
                        }
                    }
                    break;
                }
                case gpMacCore_AddressModeShortAddress:
                {
                    // lookup data = |dest pan Id (2)| + |dest addr (2)| + |0x00 (1)|
                    MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &pDstAddrInfo->panId, 2);
                    MEMCPY( &keyIdLookupDescriptor.lookupData[2] , &pDstAddrInfo->address.Short, 2);
                    keyIdLookupDescriptor.lookupData[4] = 0 ;
                    keyIdLookupDescriptor.lookupDataSize = 0;
                    break;
                }
                case gpMacCore_AddressModeExtendedAddress:
                {
                    // lookup data = |ext dest addr (8)| + |0x00 (1)|
                    MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &pDstAddrInfo->address.Extended, 8);
                    keyIdLookupDescriptor.lookupData[8] = 0 ;
                    keyIdLookupDescriptor.lookupDataSize = 1;
                    break;

                }
                default:
                {
                    // should never be triggered as we check on valid destination address mode before this point
                    break;
                }
            }
            break;
        }
        // in section 7.5.8.2.2 Outgoing frame key retrieval procedure:  b) KeyIdMode != to 0x00 (explicit key identification)
        case gpMacCore_KeyIdModeExplicit1Octet:
        {
            // lookup data = |Default key source (8)| + |keyIndex (1)|
            gpMacCore_GetDefaultKeySource( keyIdLookupDescriptor.lookupData);
            keyIdLookupDescriptor.lookupData[8] = pSecOptions->keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 1;
            break;
        }
        case gpMacCore_KeyIdModeExplicit4Octet:
        {
            // lookup data = |Key source param (4)| + |keyIndex (1)|
            MEMCPY(keyIdLookupDescriptor.lookupData, pSecOptions->pKeySource.pKeySource4, 4);
            keyIdLookupDescriptor.lookupData[4] = pSecOptions->keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 0;
            break;
        }
        case gpMacCore_KeyIdModeExplicit8Octet:
        {
            // lookup data = |Key source param (8)| + |keyIndex (1)|
            MEMCPY(keyIdLookupDescriptor.lookupData, pSecOptions->pKeySource.pKeySource8, 8);
            keyIdLookupDescriptor.lookupData[8] = pSecOptions->keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 1;
            break;
        }
        default:
        {
            // should never occur as we check on valid keyIdMode before
            break;
        }
    }
    // in section 7.5.8.2.2 Outgoing frame key retrieval procedure: c) try to get key descriptor.
    if (MacCore_KeyDescriptorLookup( &keyIdLookupDescriptor , &keyDescriptor ) != GP_MACCORE_INVALID_KEY_TABLE_ENTRY )
    {
        // in section 7.5.8.2.2 Outgoing frame key retrieval procedure: d) + e) set key and return success
        MEMCPY(pKey,keyDescriptor.key,16);
        return true;

    }
    return false;
}

/*
 * This function more or less follows section 7.5.8.2.3 (incoming frame security procedure) from the MAC-2006 spec.
 * However, this functions contains extra functionality at some places (and the steps from the spec might be reordered for clarity)
*/
gpMacCore_Result_t MacCore_IncomingFrameSecurityProcedure(gpPd_Loh_t *p_PdLoh, MacCore_HeaderDescriptor_t* dataIndicationMacValues, UInt8 macHeaderLength)
{
    gpMacCore_Result_t result = gpMacCore_ResultSuccess;
    UInt16                          startOfData = p_PdLoh->offset;
    gpEncryption_CCMOptions_t              CCMOptions;
    MacCore_SecurityLevelPassed_t   securityLevelPassed;
    gpMacCore_KeyDescriptor_t       keyDescriptor;
    gpMacCore_DeviceDescriptor_t    deviceDescriptor;
    gpMacCore_KeyDeviceDescriptor_t keyDeviceDescriptor;
    UInt8 nonce[13];
    UInt8 firstPayloadByte;
    UInt8 secLevel;
    UInt8 startData;

    CCMOptions.pNonce = nonce;
    CCMOptions.pdHandle= p_PdLoh->handle;
    startData = p_PdLoh->offset - macHeaderLength;
    firstPayloadByte = gpPd_ReadByte(p_PdLoh->handle, p_PdLoh->offset);

    // In section 7.5.8.2.3 Incoming frame security procedure a) If the Security Enabled subfield == 0 ==> security level = nothing
    if(!MACCORE_FRAMECONTROL_SECURITY_GET(dataIndicationMacValues->frameControl))
    {
        secLevel = gpEncryption_SecLevelNothing;
    }
    else
    {
        if(!MACCORE_FRAMECONTROL_FRAMEVERSION_GET(dataIndicationMacValues->frameControl))
        {
            // In section 7.5.8.2.3 Incoming frame security procedure b) security + old frame version ==> legacy
            return gpMacCore_ResultUnsupportedLegacy;
        }
        secLevel = dataIndicationMacValues->secOptions.securityLevel;
        // In section 7.5.8.2.3 Incoming frame security procedure c) security level 0 indicated in header ==> unsupported
        if(dataIndicationMacValues->secOptions.securityLevel == gpEncryption_SecLevelNothing)
        {
            return gpMacCore_ResultUnsupportedSecurity;
        }
    }

    // In section 7.5.8.2.3 Incoming frame security procedure d) return success or unsupported security, depending on the MAC's security setting
    if( secLevel == gpEncryption_SecLevelNothing )
    {
        return gpMacCore_ResultSuccess;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure e) check if minimal security level is present.
    securityLevelPassed = MacCore_IncomingSecurityLevelCheckingProcedure( secLevel , dataIndicationMacValues->frameType , (gpMacCore_Command_t)firstPayloadByte);
    if( securityLevelPassed == MacCore_SecurityLevelPassedFalse )
    {
        return gpMacCore_ResultImproperSecurityLevel;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure f) if the security level is set to zero, return succes
    // Fix, we added the check on securitylevelPassed, otherwise section i) can not be valid. Needs to be confirmed
    if( secLevel == gpEncryption_SecLevelNothing &&  securityLevelPassed == MacCore_SecurityLevelPassedTrue )
    {
        return gpMacCore_ResultSuccess;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure g) obtain key and device info to construct the key. return unavailable key if not found
    if(!MacCore_IncomingFrameSecurityRetrievalProcedure( &keyDescriptor , &deviceDescriptor , &keyDeviceDescriptor , dataIndicationMacValues))
    {
        return gpMacCore_ResultUnavailableKey;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure h) check key usage policy
    if(!MacCore_IncomingKeyUsagePolicyCheckingProcedure( &keyDescriptor , dataIndicationMacValues->frameType , (gpMacCore_Command_t)firstPayloadByte))
    {
        return gpMacCore_ResultImproperKeyType;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure i) check if proper security level
    if(!deviceDescriptor.exempt && securityLevelPassed == MacCore_SecurityLevelPassedConditionallyPassed )
    {
        return gpMacCore_ResultImproperSecurityLevel;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure j) + k) check if frame counter is valid
    if( dataIndicationMacValues->frameCounter == GP_MACCORE_FRAME_COUNTER_MAXIMUM || dataIndicationMacValues->frameCounter < deviceDescriptor.frameCounter )
    {
        return gpMacCore_ResultCounterError;
    }

    // In section 7.5.8.2.3 Incoming frame security procedure l) prepare for CCM* decryption

    // construct the nonce: |Ext src address (8)|Frame counter (4)|Security level (1)|
    MacCore_ConstructNonce(&deviceDescriptor.extAddress, dataIndicationMacValues->frameCounter, secLevel, CCMOptions.pNonce);


    CCMOptions.micLength = GP_ENCRYPTION_SECLEVEL2MICLENGTH(secLevel);

    if(secLevel >= gpEncryption_SecLevelMIC32 && secLevel <= gpEncryption_SecLevelMIC128)
    {
        CCMOptions.dataLength = 0 ;
        CCMOptions.auxLength = macHeaderLength;
    }
    else if(secLevel >= gpEncryption_SecLevelENC && secLevel <= gpEncryption_SecLevelENC_MIC128)
    {
        CCMOptions.dataLength = p_PdLoh->length - CCMOptions.micLength;
        CCMOptions.auxLength = macHeaderLength;
    }

    CCMOptions.pKey = keyDescriptor.key ;
    CCMOptions.dataOffset = startOfData;
    CCMOptions.auxOffset = startData;

    if(dataIndicationMacValues->frameType == gpMacCore_FrameTypeCommand)
    {
        /* for command frames the encrypted payload starts after the command byte */
        CCMOptions.auxLength  += 1;
        CCMOptions.dataOffset += 1;
        CCMOptions.dataLength -= 1;
    }
    // decrypt the packet
    // fixme: can result be ommited?
    result = gpEncryption_CCMDecrypt(&CCMOptions);
    p_PdLoh->length -= CCMOptions.micLength;

    // fixme: keep in mind
    /* in section 7.5.6.2 Reception and rejection
    If the status from the incoming frame security procedure is not SUCCESS, the MLME shall issue the
    corresponding confirm or MLME-COMM-STATUS.indication primitive with the status parameter set to the
    status from the incoming frame security procedure, indicating the error, and with the security-related
    parameters set to the corresponding parameters returned by the unsecuring process.
    */
    return result;
}


/* in section 7.5.8.2.4 Incoming frame security material retrieval procedure
 * The input to this procedure is the frame to be unsecured. The outputs from this procedure are a passed or
 * failed status and, if passed, a KeyDescriptor, a DeviceDescriptor, and a KeyDeviceDescriptor.
 * The incoming frame security material retrieval procedure involves the following steps as applicable:
*/
Bool MacCore_IncomingFrameSecurityRetrievalProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_KeyDeviceDescriptor_t *pKeyDeviceDescriptor , MacCore_HeaderDescriptor_t *pDataIndicationMacValues)
{
    gpMacCore_KeyIdLookupDescriptor_t  keyIdLookupDescriptor;
    gpMacCore_DeviceLookupDescriptor_t deviceLookupDescriptor;
    gpMacCore_KeyDeviceListEntries_t   keyDeviceDescriptorIndex;
    gpMacCore_KeyTablesEntries_t       keyDescriptorIndex;

    MEMSET(&deviceLookupDescriptor, 0, sizeof(deviceLookupDescriptor));
    switch(pDataIndicationMacValues->secOptions.keyIdMode)
    {
        case gpMacCore_KeyIdModeImplicit:
        {
            switch( pDataIndicationMacValues->srcAddrInfo.addressMode)
            {
                case gpMacCore_AddressModeNoAddress:
                {
                    UInt16 PanCoordShortAddress = gpMacCore_GetPanCoordShortAddress();
                    if( PanCoordShortAddress <= 0xFFFD )
                    {
                        // lookup data = |dst pan (2)| + |Pan coord short address (2)| + |0x00 (1)|
                        MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &pDataIndicationMacValues->dstAddrInfo.panId , 2);
                        MEMCPY( &keyIdLookupDescriptor.lookupData[2] , &PanCoordShortAddress , 2);
                        keyIdLookupDescriptor.lookupData[4] = 0;
                        keyIdLookupDescriptor.lookupDataSize = 0;
                    }
                    else
                    {
                        // lookup data = |pan coord ext addr (8)| + |0x00 (1)|
                        MACAddress_t PANCoordExtendedAddress;
                        gpMacCore_GetPanCoordExtendedAddress( &PANCoordExtendedAddress);
                        MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &PANCoordExtendedAddress, 8);
                        keyIdLookupDescriptor.lookupData[8] = 0;
                        keyIdLookupDescriptor.lookupDataSize = 1;
                    }
                    break;
                }
                case gpMacCore_AddressModeShortAddress:
                {
                    // fixme: verify with spec that behavior is correct
                    // lookup data = |src pan Id (2)| + |src addr (2)| + |0x00 (1)|
                    MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &pDataIndicationMacValues->srcAddrInfo.panId, 2);
                    MEMCPY( &keyIdLookupDescriptor.lookupData[2] , &pDataIndicationMacValues->srcAddrInfo.address.Short , 2);
                    keyIdLookupDescriptor.lookupData[4] = 0;
                    keyIdLookupDescriptor.lookupDataSize = 0;
                    break;
                }
                case gpMacCore_AddressModeExtendedAddress:
                {
                    // lookup data = |ext src addr (8)| + |0x00 (1)|
                    MEMCPY( &keyIdLookupDescriptor.lookupData[0] , &pDataIndicationMacValues->srcAddrInfo.address.Extended , 8);
                    keyIdLookupDescriptor.lookupData[8] = 0;
                    keyIdLookupDescriptor.lookupDataSize = 1;
                    break;
                }
                default:
                {
                    // should not occur, as the addressMode is only 2 bit.
                    break;
                }
            }
            break;
        }
        case gpMacCore_KeyIdModeExplicit1Octet:
        {
            // lookup data = |Default key source (8)| + |keyIndex (1)|
            gpMacCore_GetDefaultKeySource( keyIdLookupDescriptor.lookupData);
            keyIdLookupDescriptor.lookupData[8] = pDataIndicationMacValues->secOptions.keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 1;
            break;
        }
        case gpMacCore_KeyIdModeExplicit4Octet:
        {
            // lookup data = |Key source param (4)| + |keyIndex (1)|
            MEMCPY( keyIdLookupDescriptor.lookupData , pDataIndicationMacValues->secOptions.pKeySource.pKeySource4 , 4 );
            keyIdLookupDescriptor.lookupData[4] = pDataIndicationMacValues->secOptions.keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 0;
            break;
        }
        case gpMacCore_KeyIdModeExplicit8Octet:
        {
            // lookup data = |Key source param (8)| + |keyIndex (1)|
            MEMCPY( keyIdLookupDescriptor.lookupData , pDataIndicationMacValues->secOptions.pKeySource.pKeySource8 , 8 );
            keyIdLookupDescriptor.lookupData[8] = pDataIndicationMacValues->secOptions.keyIndex;
            keyIdLookupDescriptor.lookupDataSize = 1;
            break;
        }
        default:
        {
            // should not occur, as the keyIdMode is only 2 bit.
            break;
        }
    }

    // in section 7.5.8.2.4 Incoming frame security material retrieval procedure c)  try to get the key descriptor
    keyDescriptorIndex = MacCore_KeyDescriptorLookup( &keyIdLookupDescriptor , pKeyDescriptor);
    if( keyDescriptorIndex == GP_MACCORE_INVALID_KEY_TABLE_ENTRY )
    {
        return false;
    }

    switch( pDataIndicationMacValues->srcAddrInfo.addressMode)
    {
        case gpMacCore_AddressModeNoAddress:
        {
            UInt16 panCoordShortAddress = gpMacCore_GetPanCoordShortAddress();
            if( panCoordShortAddress <= 0xFFFD )
            {
                deviceLookupDescriptor.DeviceLookup.shortAndPanId.panId = pDataIndicationMacValues->dstAddrInfo.panId;
                deviceLookupDescriptor.DeviceLookup.shortAndPanId.shortAddress = panCoordShortAddress;
                deviceLookupDescriptor.lookupSize = 4;
            }
            else
            {
                gpMacCore_GetPanCoordExtendedAddress( &deviceLookupDescriptor.DeviceLookup.extendedAddress);
                deviceLookupDescriptor.lookupSize = 8;
            }
            break;
        }
        case gpMacCore_AddressModeShortAddress:
        {
            // when intrapan srcPanId==dstPanId in the pDataIndicationMacValues structure
            deviceLookupDescriptor.DeviceLookup.shortAndPanId.panId = pDataIndicationMacValues->srcAddrInfo.panId;
            deviceLookupDescriptor.DeviceLookup.shortAndPanId.shortAddress = pDataIndicationMacValues->srcAddrInfo.address.Short;
            deviceLookupDescriptor.lookupSize = 4;
            break;
        }
        case gpMacCore_AddressModeExtendedAddress:
        {
            MEMCPY(&deviceLookupDescriptor.DeviceLookup.extendedAddress, &pDataIndicationMacValues->srcAddrInfo.address.Extended , 8 );
            deviceLookupDescriptor.lookupSize = 8;
            break;
        }
        default:
        {
            // should not occur, as the addressMode is only 2 bit.
            break;
        }
    }

    // in section 7.5.8.2.4 Incoming frame security material retrieval procedure e) + f) blacklist checking + return
    keyDeviceDescriptorIndex = MacCore_BlacklistCheckingProcedure( pKeyDescriptor , &deviceLookupDescriptor , pDeviceDescriptor , pKeyDeviceDescriptor );
    return keyDeviceDescriptorIndex != GP_MACCORE_INVALID_KEY_DEVICE_LIST_ENTRY;
}

/**
 * Do not delete commented logging in this function. Very useful for debugging
*/

// pKeyDescriptor == stored, pKeyIdLookupDescriptor == constructed
gpMacCore_KeyTablesEntries_t MacCore_KeyDescriptorLookup( gpMacCore_KeyIdLookupDescriptor_t *pKeyIdLookupDescriptor , gpMacCore_KeyDescriptor_t *pKeyDescriptor )
{
    gpMacCore_KeyTablesEntries_t keyTableEntry;
    gpMacCore_KeyIdLookupListEntries_t keyIdLookupEntry;
    for( keyTableEntry = 0 ; keyTableEntry < gpMacCore_GetKeyTableEntries() ; keyTableEntry++)
    {
        // get the stored key descriptor entry
        gpMacCore_GetKeyDescriptor( pKeyDescriptor, keyTableEntry);
        //GP_LOG_SYSTEM_PRINTF("entries in desc tabel: %x",0,pKeyDescriptor->keyIdLookupListEntries);

        for( keyIdLookupEntry = 0 ; keyIdLookupEntry < pKeyDescriptor->keyIdLookupListEntries ; keyIdLookupEntry++)
        {
            // in section 7.5.8.2.5 KeyDescriptor lookup procedure a) check on lookup size and lookup descriptor
            //GP_LOG_SYSTEM_PRINTF("LUP desc: ",0);
            //gpLog_PrintBuffer(sizeof(gpMacCore_KeyIdLookupDescriptor_t), pKeyIdLookupDescriptor);
            //GP_LOG_SYSTEM_PRINTF("stored",0);
            //gpLog_PrintBuffer(sizeof(gpMacCore_KeyIdLookupDescriptor_t), pKeyDescriptor->keyIdLookupDescriptors);
            if(pKeyDescriptor->keyIdLookupDescriptors[keyIdLookupEntry].lookupDataSize == pKeyIdLookupDescriptor->lookupDataSize)
            {
                UInt8 compareLength = 0;
                if(pKeyIdLookupDescriptor->lookupDataSize == 0)
                {
                    compareLength = 5;
                }
                else if(pKeyIdLookupDescriptor->lookupDataSize == 1)
                {
                    compareLength = 9;
                }
                else
                {
                    // fixme: how do we handle this?
                    GP_ASSERT_DEV_EXT(false);   // lookupDataSize is not allowed to be > 1
                }
                //GP_LOG_SYSTEM_PRINTF("data: given en stored:",0);
                //gpLog_PrintBuffer(compareLength, pKeyIdLookupDescriptor->lookupData);
                //gpLog_PrintBuffer(compareLength, pKeyDescriptor->keyIdLookupDescriptors[keyIdLookupEntry].lookupData);
                if( MEMCMP(pKeyDescriptor->keyIdLookupDescriptors[keyIdLookupEntry].lookupData , pKeyIdLookupDescriptor->lookupData , compareLength ) == 0 )
                {
                    // we found the correct one.
                    //GP_LOG_SYSTEM_PRINTF("we found a correct entry",0);
                    return keyTableEntry;
                }
            }
        }
    }
    return GP_MACCORE_INVALID_KEY_TABLE_ENTRY;
}

// in section 7.5.8.2.6 Blacklist checking procedure
gpMacCore_KeyDeviceListEntries_t MacCore_BlacklistCheckingProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_DeviceLookupDescriptor_t *pDeviceLookupDescriptor , gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_KeyDeviceDescriptor_t *pKeyDeviceDescriptor )
{
    gpMacCore_KeyDeviceListEntries_t entry;
    for( entry=0 ; entry < pKeyDescriptor->keyDeviceListEntries ; entry++)
    {
        // unique device
        if( pKeyDescriptor->keyDeviceDescriptorList[entry].uniqueDevice == true )
        {
            // not blacklisted
            if( pKeyDescriptor->keyDeviceDescriptorList[entry].blackListed == false )
            {
                *pDeviceDescriptor = pKeyDescriptor->keyDeviceDescriptorList[entry].deviceDescriptor;
                *pKeyDeviceDescriptor = pKeyDescriptor->keyDeviceDescriptorList[entry];
                return entry;
            }
            else
            {
                return GP_MACCORE_INVALID_KEY_DEVICE_LIST_ENTRY ;
            }
        }
        else
        {
            // in section 7.5.8.2.6 Blacklist checking procedure: a3) device descriptor lookup procedure
            if( MacCore_DeviceDescriptorLookup( &pKeyDescriptor->keyDeviceDescriptorList[entry].deviceDescriptor , pDeviceLookupDescriptor /* constructed from packet */ ))
            {
                *pDeviceDescriptor = pKeyDescriptor->keyDeviceDescriptorList[entry].deviceDescriptor;
                *pKeyDeviceDescriptor = pKeyDescriptor->keyDeviceDescriptorList[entry];
                if( pKeyDescriptor->keyDeviceDescriptorList[entry].blackListed == false )
                {
                    return entry;
                }
                else
                {
                    return GP_MACCORE_INVALID_KEY_DEVICE_LIST_ENTRY ;
                }
            }
        }
    }

    // in section 7.5.8.2.6 Blacklist checking procedure: b) return failure
    return GP_MACCORE_INVALID_KEY_DEVICE_LIST_ENTRY ;
}

// in section 7.5.8.2.7 DeviceDescriptor lookup procedure
Bool MacCore_DeviceDescriptorLookup( gpMacCore_DeviceDescriptor_t *pDeviceDescriptor , gpMacCore_DeviceLookupDescriptor_t *pDeviceLookupDescriptor )
{
    Bool result = false;
    if( pDeviceLookupDescriptor->lookupSize == 4 )
    {
        if (pDeviceDescriptor->panId == pDeviceLookupDescriptor->DeviceLookup.shortAndPanId.panId &&
            pDeviceDescriptor->shortAddr == pDeviceLookupDescriptor->DeviceLookup.shortAndPanId.shortAddress)
        {
            result = true;
        }
    }
    else if( pDeviceLookupDescriptor->lookupSize == 8 )
    {
        if( pDeviceDescriptor->extAddress.LSB == pDeviceLookupDescriptor->DeviceLookup.extendedAddress.LSB &&
            pDeviceDescriptor->extAddress.MSB == pDeviceLookupDescriptor->DeviceLookup.extendedAddress.MSB )
        {
            result = true;
        }
    }
    return result;
}

// in section 7.5.8.2.8 Incoming security level checking procedure
MacCore_SecurityLevelPassed_t MacCore_IncomingSecurityLevelCheckingProcedure(gpEncryption_SecLevel_t securityLevel , gpMacCore_FrameType_t frameType , gpMacCore_Command_t commandFrameIdentifier )
{
    gpMacCore_SecurityLevelTableEntries_t entry;

    for( entry=0; entry < gpMacCore_GetSecurityLevelTableEntries(); entry++)
    {
        gpMacCore_SecurityLevelDescriptor_t securityLevelDescriptor;

        MEMSET(&securityLevelDescriptor, 0, sizeof(securityLevelDescriptor));
        gpMacCore_GetSecurityLevelDescriptor (&securityLevelDescriptor , entry);
        if( frameType != gpMacCore_FrameTypeCommand )
        {
            // in section 7.5.8.2.8 Incoming security level checking procedure: a1) compare incoming security level with minimum
            if( securityLevelDescriptor.frameType == frameType )
            {
                if (MacCore_SecurityLevelSubfieldCompair( securityLevel , securityLevelDescriptor.securityMinimum))
                {
                    return MacCore_SecurityLevelPassedTrue;
                }
                else
                {
                    if( securityLevelDescriptor.deviceOverrideSecurityMinimum == true && securityLevel == gpEncryption_SecLevelNothing )
                    {
                        return MacCore_SecurityLevelPassedConditionallyPassed;
                    }
                    else
                    {
                        return MacCore_SecurityLevelPassedFalse;
                    }
                }
            }
        }
        else
        {
            // in section 7.5.8.2.8 Incoming security level checking procedure: a2) compare incoming security level with minimum
            if( securityLevelDescriptor.frameType == frameType && securityLevelDescriptor.commandFrameIdentifier == commandFrameIdentifier)
            {
                if (MacCore_SecurityLevelSubfieldCompair( securityLevel , securityLevelDescriptor.securityMinimum ))
                {
                    return MacCore_SecurityLevelPassedTrue;
                }
                else
                {
                    if( securityLevelDescriptor.deviceOverrideSecurityMinimum == true && securityLevel == gpEncryption_SecLevelNothing )
                    {
                        return MacCore_SecurityLevelPassedConditionallyPassed;
                    }
                    else
                    {
                        return MacCore_SecurityLevelPassedFalse;
                    }
                }
            }
        }
    }
    // in section 7.5.8.2.8 Incoming security level checking procedure b) return passed.
    return MacCore_SecurityLevelPassedTrue;
}

/*7.5.8.2.9 Incoming key usage policy checking procedure
The inputs to this procedure are the KeyDescriptor, the frame type, and the command frame identifier. The
output from this procedure is a passed or failed status.
*/
Bool MacCore_IncomingKeyUsagePolicyCheckingProcedure( gpMacCore_KeyDescriptor_t *pKeyDescriptor , gpMacCore_FrameType_t frametype , gpMacCore_Command_t commandFrameIdentifier )
{
    gpMacCore_KeyUsageListEntries_t entry;
    for( entry=0 ; entry < pKeyDescriptor->keyUsageListEntries; entry++ )
    {
        if( pKeyDescriptor->keyUsageDescriptorList[entry].frameType != gpMacCore_FrameTypeCommand )
        {
            if( pKeyDescriptor->keyUsageDescriptorList[entry].frameType == frametype )
            {
                return true;
            }
        }
        else
        {
            if( pKeyDescriptor->keyUsageDescriptorList[entry].frameType == frametype && pKeyDescriptor->keyUsageDescriptorList[entry].commandFrameIdentifier == commandFrameIdentifier )
            {
                return true;
            }
        }
    }
    return false;
}

