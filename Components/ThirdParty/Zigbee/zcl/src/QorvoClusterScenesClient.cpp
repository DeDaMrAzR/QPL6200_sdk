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
 * $Header: $
 * $Change: $
 * $DateTime: $
 * 
 */

#include <climits>
#include <string>
#include <list>
#include <set>
#include <map>
#include "CompactFramework.h"
#include "CompactTimer.h"
#include "CompactTimerService.h"
#include "Compact15dot4.h"
#include "Packet.h"
#include "IEEE802154CommonTypes.h"
#include "IEEE802154Security.h"
#include "ZigBeeBase.h"
#include "ZigBeeCommonTypes.h"
#include "ZigBeeSecurity.h"
#include "ZigBeeApplication.h"
#include "ZigBeeApplicationSecurity.h"
#include "ZigBeeApplicationSupportBase.h"
#include "ZigBeeApplicationSupport.h"
#include "ZigBeeClusterLibrary.h"

#include "QorvoClusterScenesClient.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO
#include "gpLog.h"

#define GP_LOCAL_LOG

/////////////////////////////////////////////////////////////////////////////
// CQorvoSceneExtensionFieldSet

CQorvoSceneExtensionFieldSet::CQorvoSceneExtensionFieldSet(unsigned short clusterId)
{
    wClusterId = clusterId;
    nLength = 0;

    MEMSET(data,0, sizeof(data));
}

bool CQorvoSceneExtensionFieldSet::AppendField(unsigned char length, const void *const pData)
{
    if (nLength + length > maxSizeOfExtensionFieldSet)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error! AppendField excess limit, field Length: %d -> %d, limit: %d\n",0,
            nLength,
            nLength + length,
            maxSizeOfExtensionFieldSet);
        
        return false;
    }

    MEMCPY(data + nLength, pData, length);

    nLength += length;
    
    return true;
}

bool CQorvoSceneExtensionFieldSet::AppendField(unsigned char data)
{
    AppendField(sizeof(unsigned char), &data);
    
    return true;
}

bool CQorvoSceneExtensionFieldSet::AppendField(unsigned short data)
{
    AppendField(sizeof(unsigned short), &data);
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenesClient

const unsigned short CQorvoClusterScenesClient::nClusterRevision =
    CQorvoClusterScenesClient::revision;

CQorvoClusterScenesClient::CQorvoClusterScenesClient
    (CZigBeeFoundationApplication &application) :
CZigBeeCluster(application, id, flagOutbound)
{
}

/*************************************
Response command handling
*************************************/
void CQorvoClusterScenesClient::OnAddSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    unsigned char sceneId;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Add Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Add Scene Response, length = %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x\n", 0
        ,status
        ,groupId
        ,sceneId);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnViewSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    unsigned char sceneId;
    unsigned short transitionTime;
    unsigned char sceneNameLen;
    char sceneName[maxSizeOfSceneName];
    CQorvoSceneExtensionFieldSet extFieldSet;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed View Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
   
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);

    if (status == statusSuccess)
    {
        // Check payload length to determine Transition Time, scene namem extension field set is included or not
        if (asdu.GetSize() >= 7) 
        {
            // -- Retrieve transition time and Scane Name Length
            UInt16_buf2api(&transitionTime, pData, 1, &index);
            UInt8_buf2api(&sceneNameLen, pData, 1, &index);

            // -- Retrieve Scane Name
            if (asdu.GetSize() > (7 + sceneNameLen))
            {
                if (sceneNameLen)
                {
                    char_buf2api(&sceneName[0], pData, sceneNameLen, &index);
                }
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed View Scene Response(Scene Name/Ext field), length = %d\n", 0, asdu.GetSize());
                TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
                return;
            }
      
            GP_LOG_SYSTEM_PRINTF("Scene Client: View Scene Response, length: %d\n", 0, asdu.GetSize());
            GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x, TransitionTime(s): %d\n", 0
                ,status
                ,groupId
                ,sceneId
                ,transitionTime);
            
            if (sceneNameLen)
            {
                GP_LOG_SYSTEM_PRINTF("  SceneName(%d): ",0, sceneNameLen);
                for(int x = 0 ; x < sceneNameLen ; x++)
                {
                    GP_LOG_SYSTEM_PRINTF("%c",0 ,sceneName[x]);
                }

                GP_LOG_SYSTEM_PRINTF("\n",0);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("  No SceneName \n",0);
            }
            
            GP_LOG_SYSTEM_PRINTF("  Extension field Set:\n", 0);

            // -- Retrieve Extension field set            
            while (index < asdu.GetSize())
            {
                // -- Each extension field set must include cluster Id(2 bytes) and length(1 byte) of the field.
                if (index +3 >= asdu.GetSize())
                {
                    GP_LOG_SYSTEM_PRINTF("  Malformed in extension field\n",0);
                    GP_LOG_PRINTF("    index: %d, remaining payload: %d\n",0 
                        ,index
                        ,asdu.GetSize() - index);
                    break;                
                }
                else
                {
                    // -- Retrieve cluster Id and field length of the extension field set.
                    UInt16_buf2api(&extFieldSet.wClusterId, pData, 1, &index);
                    UInt8_buf2api(&extFieldSet.nLength, pData, 1, &index);
               
                    // -- Retrieve details of the extension field set
                    if ((extFieldSet.nLength) && (asdu.GetSize() >= ( index + extFieldSet.nLength)))
                    {
                        UInt8_buf2api(&extFieldSet.data[0], pData, extFieldSet.nLength, &index);
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("  Malformed in extension field\n",0);
                        GP_LOG_PRINTF("    index: %d, payload length: %d, ext. length: %d\n",0 
                            ,index
                            ,asdu.GetSize()
                            ,extFieldSet.nLength);
                        break;                
                    }

                    // printout the details of the extension field sets
                    GP_LOG_SYSTEM_PRINTF("  - cluster: 0x%04x, length: %d payload: 0x", 0
                        ,extFieldSet.wClusterId    
                        ,extFieldSet.nLength);

                    for(int y = 0 ; y < extFieldSet.nLength ; y++)
                    {
                        GP_LOG_SYSTEM_PRINTF("%02x",0 , extFieldSet.data[y]);
                    }

                    GP_LOG_SYSTEM_PRINTF("\n",0);
                }
            }
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed View Scene Response, length = %d\n", 0, asdu.GetSize());
            TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
            return;
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: View Scene Response, payload length: %d\n", 0, asdu.GetSize());
        GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId 0x:%x, SceneId: 0x%x\n", 0
            ,status
            ,groupId
            ,sceneId);
    }
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnRemoveSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    unsigned char sceneId;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Remove Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Remove Scene Response, length: %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x\n", 0
        ,status
        ,groupId
        ,sceneId);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnRemoveAllScenesResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 3)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Remove All Scenes Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Remove All Scenes Response, length: %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x\n", 0
        ,status
        ,groupId);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnStoreSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status = statusSuccess;
    unsigned short groupId;
    unsigned char sceneId;

    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Store Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();
    
    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Store Scene Response, length: %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x\n", 0
        ,status
        ,groupId
        ,sceneId);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnGetSceneMembershipResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned char capacity;
    unsigned short groupId;
    unsigned char sceneCount;
    unsigned char sceneListId;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Get Scene Membership Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt8_buf2api(&capacity, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);

    if (status == statusSuccess)
    {
        // Handle for Scene Count and Scene List
        if (asdu.GetSize() > 4)
        {
            // -- Retrieve Scene Count
            UInt8_buf2api(&sceneCount, pData, 1, &index);
        
            // -- Retrieve Scene ID List according to number of scene count.
            if (asdu.GetSize() >= (index + sceneCount))
            {
                GP_LOG_SYSTEM_PRINTF("Scene Client: Get Scene Membership Response, length: %d\n", 0, asdu.GetSize());
                GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, Capacity: %d, GroupId: 0x%x, Scene Count: %d\n", 0
                    ,status
                    ,capacity
                    ,groupId
                    ,sceneCount);

                if (sceneCount)
                {   
                    GP_LOG_SYSTEM_PRINTF("  SceneId: ", 0);
                    for (int i = 0 ; i < sceneCount ; i++)
                    {
                        UInt8_buf2api(&sceneListId, pData, 1, &index);
                        GP_LOG_SYSTEM_PRINTF("0x%02x, ", 0, sceneListId);
                    }
            
                    GP_LOG_SYSTEM_PRINTF("\n",0);
                }
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Get Scene Membership Response(SceneList), length = %d\n", 0, asdu.GetSize());
                TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
                return;
            }

        }
        else
            {
                GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Get Scene Membership Response, length = %d\n", 0, asdu.GetSize());
                TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
                return;
            }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Get Scene Membership Response, length: %d\n", 0, asdu.GetSize());
                GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, Capacity: %d, GroupId: 0x%x\n", 0
                    ,status
                    ,capacity
                    ,groupId);
    }
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnEnhancedAddSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    unsigned char sceneId;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Enhanced Add Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Enhanced Add Scene Response, length: %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x\n", 0
        ,status
        ,groupId
        ,sceneId);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnEnhancedViewSceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupId;
    unsigned char sceneId;
    unsigned short transitionTime;
    unsigned char sceneNameLen;
    char sceneName[maxSizeOfSceneName];
    CQorvoSceneExtensionFieldSet extFieldSet;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Enhanced View Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupId, pData, 1, &index);
    UInt8_buf2api(&sceneId, pData, 1, &index);
    
    if (status == statusSuccess)
    {
        // Check payload length to determine Transition Time, scene namem extension field set is included or not
        if (asdu.GetSize() >= 7) 
        {
            // -- Retrieve transition time and Scane Name Length
            UInt16_buf2api(&transitionTime, pData, 1, &index);
            UInt8_buf2api(&sceneNameLen, pData, 1, &index);

            // -- Retrieve Scane Name
            if (asdu.GetSize() > (7 + sceneNameLen))
            {
                if (sceneNameLen)
                {
                    char_buf2api(&sceneName[0], pData, sceneNameLen, &index);
                }
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Enhanced View Scene Response(Scene Name/Ext field), length = %d\n", 0, asdu.GetSize());
                TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
                return;
            }
      
            GP_LOG_SYSTEM_PRINTF("Scene Client: Enhanced View Scene Response, length: %d\n", 0, asdu.GetSize());
            GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x  TransitionTime(1/10 s): %d\n", 0
                ,status
                ,groupId
                ,sceneId
                ,transitionTime);
            
            if (sceneNameLen)
            {
                GP_LOG_SYSTEM_PRINTF("  SceneName(%d): ",0, sceneNameLen);
                for(int x = 0 ; x < sceneNameLen ; x++)
                {
                    GP_LOG_SYSTEM_PRINTF("%c",0 ,sceneName[x]);
                }

                GP_LOG_SYSTEM_PRINTF("\n",0);
            }
            else
            {
                GP_LOG_SYSTEM_PRINTF("  No SceneName \n",0);
            }
           
           GP_LOG_SYSTEM_PRINTF("  Extension field Set:\n", 0);

            // -- Retrieve Extension field set            
            while (index < asdu.GetSize())
            {
                // -- Each extension field set must include cluster Id(2 bytes) and length(1 byte) of the field.
                if (index +3 >= asdu.GetSize())
                {
                    GP_LOG_SYSTEM_PRINTF("  Malformed in extension field\n",0);
                    GP_LOG_PRINTF("    index: %d, remaining payload: %d\n",0 
                        ,index
                        ,asdu.GetSize() - index);
                    break;                
                }
                else
                {
                    // -- Retrieve cluster Id and field length of the extension field set.
                    UInt16_buf2api(&extFieldSet.wClusterId, pData, 1, &index);
                    UInt8_buf2api(&extFieldSet.nLength, pData, 1, &index);
               
                    // -- Retrieve details of the extension field set
                    if ((extFieldSet.nLength) && (asdu.GetSize() >= ( index + extFieldSet.nLength)))
                    {
                        UInt8_buf2api(&extFieldSet.data[0], pData, extFieldSet.nLength, &index);
                    }
                    else
                    {
                        GP_LOG_SYSTEM_PRINTF("  Malformed in extension field\n",0);
                        GP_LOG_PRINTF("    index: %d, payload length: %d, ext. length: %d\n",0 
                            ,index
                            ,asdu.GetSize()
                            ,extFieldSet.nLength);
                        break;                
                    }

                    // printout the details of the extension field sets
                    GP_LOG_SYSTEM_PRINTF("  - cluster: 0x%04x, length: %d payload: 0x", 0
                        ,extFieldSet.wClusterId    
                        ,extFieldSet.nLength);

                    for(int y = 0 ; y < extFieldSet.nLength ; y++)
                    {
                        GP_LOG_SYSTEM_PRINTF("%02x",0 ,extFieldSet.data[y]);
                    }

                    GP_LOG_SYSTEM_PRINTF("\n",0);
                }
            }
        }
        else
        {
            GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Enhanced View Scene Response, length = %d\n", 0, asdu.GetSize());
            TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
            return;
        }
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Enhanced View Scene Response, length: %d\n", 0, asdu.GetSize());
        GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, GroupId: 0x%x, SceneId: 0x%x\n", 0
            ,status
            ,groupId
            ,sceneId);
    }
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

void CQorvoClusterScenesClient::OnCopySceneResponse(const CZigBeeAddress &destination,
    const CZigBeeAddress &source, const CZigBeeClusterLibraryHeader &header,
    CPacket &asdu, const unsigned char nLinkQuality, const unsigned int nTimestamp)
{
    unsigned char status;
    unsigned short groupIdFrom;
    unsigned char sceneIdFrom;
    
    // return malformed if the payload length is not valid
    if (asdu.GetSize() < 4)
    {
        GP_LOG_SYSTEM_PRINTF("Scene Client: Error, malformed Copy Scene Response, length = %d\n", 0, asdu.GetSize());
        TransmitDefaultResponse(destination, source, header, statusMalformedCommand);
        return;
    }
    
    //Get Data from payload 
    UInt16 index = 0;
    Int8* pData;    

    // Retrieve the payload
    pData = (Int8*)asdu.GetData();

    // Retrieve the mandatory parameter from payload
    UInt8_buf2api(&status, pData, 1, &index);
    UInt16_buf2api(&groupIdFrom, pData, 1, &index);
    UInt8_buf2api(&sceneIdFrom, pData, 1, &index);

    GP_LOG_SYSTEM_PRINTF("Scene Client: Copy Scene Response, length: %d\n", 0, asdu.GetSize());
    GP_LOG_SYSTEM_PRINTF("  Status: 0x%x, groupId From: 0x%x, sceneId From: 0x%x\n", 0
        ,status
        ,groupIdFrom
        ,sceneIdFrom);
    
    // Transmit a default response frame with status
    TransmitDefaultResponse(destination, source, header, statusSuccess);
}

/*************************************
Server to client command handling
*************************************/
void CQorvoClusterScenesClient::AddScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned short transitionTime,
    char* sceneName,
    std::list<CQorvoSceneExtensionFieldSet>& extFieldList,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;
    UInt8 sceneNameLength = strlen(sceneName);
    UInt8 frameSize = 9 + sceneNameLength;
  
    for (std::list<CQorvoSceneExtensionFieldSet>::const_iterator i = extFieldList.begin(); i != extFieldList.end(); i++)
    {   
        frameSize += 3;                 // In extension field set, cluster Id (2), Length (1)
        frameSize += i->nLength;    
    }

    // Create ZCL command frame
    CreateRequest(asdu,
        static_cast<unsigned char>(CQorvoClusterScenesClient::commandAddScene),
        CZigBeeClusterLibraryHeader::typeCluster,
        frameSize);

    // Append the payload
    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));
    asdu.Append(&transitionTime, sizeof(unsigned short));
    asdu.Append(&sceneNameLength, sizeof(UInt8));
    asdu.Append(&sceneName[0], sceneNameLength);

    for (std::list<CQorvoSceneExtensionFieldSet>::const_iterator i = extFieldList.begin(); i != extFieldList.end(); i++)
    {
        asdu.Append(&i->wClusterId, sizeof(unsigned short));
        asdu.Append(&i->nLength, sizeof(unsigned char));
        asdu.Append(&i->data[0], i->nLength);
    }

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::ViewScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandViewScene);

    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::RemoveScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandRemoveScene);

    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::RemoveAllScenes(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandRemoveAllScenes);

    asdu.Append(&groupId, sizeof(unsigned short));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::StoreScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandStoreScene);

    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::RecallScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned short transitionTime,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandRecallScene);

    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));
    asdu.Append(&transitionTime, sizeof(unsigned short));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::GetSceneMembership(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandGetSceneMembership);

    asdu.Append(&groupId, sizeof(unsigned short));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::EnhancedAddScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned short transitionTime_In100ms,
    char* sceneName,
    std::list<CQorvoSceneExtensionFieldSet>& extFieldList,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;
    UInt8 sceneNameLength = strlen(sceneName);
    UInt8 frameSize = 9 + sceneNameLength;
  
    for (std::list<CQorvoSceneExtensionFieldSet>::const_iterator i = extFieldList.begin(); i != extFieldList.end(); i++)
    {   
        frameSize += 3;                 // In extension field set, cluster Id (2), Length (1)
        frameSize += i->nLength;    
    }

    // Create ZCL command frame
    CreateRequest(asdu,
        static_cast<unsigned char>(CQorvoClusterScenesClient::commandEnhancedAddScene),
        CZigBeeClusterLibraryHeader::typeCluster,
        frameSize);

    // Append the payload
    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));
    asdu.Append(&transitionTime_In100ms, sizeof(unsigned short));
    asdu.Append(&sceneNameLength, sizeof(UInt8));
    asdu.Append(&sceneName[0], sceneNameLength);

    for (std::list<CQorvoSceneExtensionFieldSet>::const_iterator i = extFieldList.begin(); i != extFieldList.end(); i++)
    {
        asdu.Append(&i->wClusterId, sizeof(unsigned short));
        asdu.Append(&i->nLength, sizeof(unsigned char));
        asdu.Append(&i->data[0], i->nLength);
    }

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::EnhancedViewScene(const CZigBeeAddress &destination,
    const unsigned short groupId,
    const unsigned char sceneId,
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandEnhancedViewScene);

    asdu.Append(&groupId, sizeof(unsigned short));
    asdu.Append(&sceneId, sizeof(unsigned char));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

void CQorvoClusterScenesClient::CopyScene(const CZigBeeAddress &destination,
    const unsigned char mode,
    const unsigned short groupIdFrom,
    const unsigned char sceneIdFrom,
    const unsigned short groupIdTo,
    const unsigned char sceneIdTo, 
    const unsigned char nTransmitOptions)
{
    CPacket asdu;

    // Create ZCL command frame, including payload
    CreateRequest(asdu, CQorvoClusterScenesClient::commandCopyScene);

    asdu.Append(&mode, sizeof(unsigned char));
    asdu.Append(&groupIdFrom, sizeof(unsigned short));
    asdu.Append(&sceneIdFrom, sizeof(unsigned char));
    asdu.Append(&groupIdTo, sizeof(unsigned short));
    asdu.Append(&sceneIdTo, sizeof(unsigned char));

    // Transmit ZCL frame via ZCL
    OnRequestData(destination, asdu, nTransmitOptions);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenes command map

__COMPACT7B_IMPLEMENT_COMMAND_MAP(CQorvoClusterScenesClient,
    CZigBeeCluster)

__COMPACT7B_BEGIN_COMMAND_MAP(CQorvoClusterScenesClient)
    __COMPACT7B_SCENECLIENT_CLUSTER_COMMANDS()
__COMPACT7B_END_COMMAND_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenesClient attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterScenesClient,
	CZigBeeCluster)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterScenesClient)
	__COMPACT7B_CLUSTERREVISION(CQorvoClusterScenesClient)
__COMPACT7B_END_ATTRIBUTE_MAP()
