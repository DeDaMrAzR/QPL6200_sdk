/*
 * Copyright (c) 2021, Qorvo Inc
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

#ifndef _QORVO_CLUSTER_SCENE_CLIENT_H_
#define _QORVO_CLUSTER_SCENE_CLIENT_H_

/////////////////////////////////////////////////////////////////////////////
// CQorvoSceneExtensionFieldSet

class CQorvoSceneExtensionFieldSet
{
    // typedefs & enums
    public:
    
    // define of the max size for Scene extension field Set
    enum {
        // Max size of one extension field supported(adjustable)
        maxSizeOfExtensionFieldSet = 24
    };

    public:
        unsigned short wClusterId;
        unsigned char nLength;
        unsigned char data[maxSizeOfExtensionFieldSet];

    // Construction
    public:
        CQorvoSceneExtensionFieldSet(unsigned short clusterId = 0);

    // Operations
    public:
        bool AppendField(unsigned char length, const void *const pData);

        bool AppendField(unsigned char data);

        bool AppendField(unsigned short data);
};

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterScenesClient

class CQorvoClusterScenesClient : public CZigBeeCluster
{
    __COMPACT7B_DECLARE_ATTRIBUTE_MAP()
    __COMPACT7B_DECLARE_COMMAND_MAP()

    // typedefs & enums
	public:
		// Cluster identifier of the Scenes client cluster
		enum { id = 0x0005 };

        // Cluster revision of this implementation
		enum { revision = 3 };
        
        // Cluster command identifiers (client to server)
        enum {
            commandAddScene = 0x00,                  			/* Mandatory */
            commandViewScene = 0x01,                            /* Mandatory */
            commandRemoveScene = 0x02,                          /* Mandatory */
			commandRemoveAllScenes = 0x03,                      /* Mandatory */
            commandStoreScene = 0x04,                           /* Mandatory */
            commandRecallScene = 0x05,                          /* Mandatory */
			commandGetSceneMembership = 0x06,                   /* Mandatory */
            commandEnhancedAddScene = 0x40,                     /* Optional */
			commandEnhancedViewScene = 0x41,                    /* Optional */
            commandCopyScene =0x42,                             /* Optional */
        };

		// Cluster command identifiers (server to client)
		enum {
            commandAddSceneResponse = 0x00,                     /* Mandatory */
            commandViewSceneResponse = 0x01,                    /* Mandatory */
            commandRemoveSceneResponse = 0x02,                  /* Mandatory */
			commandRemoveAllScenesResponse = 0x03,              /* Mandatory */
            commandStoreSceneResponse = 0x04,                   /* Mandatory */
			commandGetSceneMembershipResponse = 0x06,           /* Mandatory */
            commandEnhancedAddSceneResponse = 0x40,             /* Optional */
			commandEnhancedViewSceneResponse = 0x41,            /* Optional */
            commandCopySceneResponse =0x42,                     /* Optional */
        };

        enum {
            // Max size of Scene Name supported(ZCL 3.7.2.3.1)
            maxSizeOfSceneName = 16
        };

	// Construction
	public:
		CQorvoClusterScenesClient
			(CZigBeeFoundationApplication &application);

    // Receipt of server to client(S2C) command handling
    protected:
        // Handler for Add Scene Response command
        void OnAddSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
        
        // Handler for View Scene Response command    
        void OnViewSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Remove Scene Response command
        void OnRemoveSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
         
        // Handler for Remove All Scenes Response command   
        void OnRemoveAllScenesResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
    
        // Handler for Store Scene Response command      
        void OnStoreSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Get Scene Membership Response command            
        void OnGetSceneMembershipResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
           
        // Handler for Enhanced Add Scene Response command   
        void OnEnhancedAddSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Enhanced View Scene Response command   
        void OnEnhancedViewSceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);
            
        // Handler for Copy Scene Response command   
        void OnCopySceneResponse(const CZigBeeAddress &destination,
            const CZigBeeAddress &source,
            const CZigBeeClusterLibraryHeader &header, CPacket &asdu,
            const unsigned char nLinkQuality, const unsigned int nTimestamp);

	// Operations
	public:
        // Assembles and transmits a Add Scene command (client to server)
        void AddScene(const CZigBeeAddress &destination,
            const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned short transitionTime,
            char* sceneName,
            std::list<CQorvoSceneExtensionFieldSet>& extFieldList,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);
        
		// Assembles and transmits a View Scene command (client to server)
		void ViewScene(const CZigBeeAddress &destination,
            const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

		// Assembles and transmits a Remove Scene command (client to server)
		void RemoveScene(const CZigBeeAddress &destination,
            const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

		// Assembles and transmits a Remove All Scenes command (client to server)
		void RemoveAllScenes(const CZigBeeAddress &destination,
			const unsigned short groupId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

		// Assembles and transmits a Store Scene command (client to server)
		void StoreScene(const CZigBeeAddress &destination,
			const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

        // Assembles and transmits a Recall Scene command (client to server)
		void RecallScene(const CZigBeeAddress &destination,
			const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned short transitionTime,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

		// Assembles and transmits a Get Scene Membership command (client to server)
		void GetSceneMembership(const CZigBeeAddress &destination,
			const unsigned short groupId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

		// Assembles and transmits a Enhanced Add Scene command (client to server)
		void EnhancedAddScene(const CZigBeeAddress &destination,
            const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned short transitionTime_In100ms,
            char* sceneName,
            std::list<CQorvoSceneExtensionFieldSet>& extFieldList,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);
        
        // Assembles and transmits a Enhanced View Scene command (client to server)
		void EnhancedViewScene(const CZigBeeAddress &destination,
            const unsigned short groupId,
            const unsigned char sceneId,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);
        
        // Assembles and transmits a Copy Scene command (client to server)
		void CopyScene(const CZigBeeAddress &destination,
			const unsigned char mode,
            const unsigned short groupIdFrom,
            const unsigned char sceneIdFrom,
            const unsigned short groupIdTo,
            const unsigned char sceneIdTo,
            const unsigned char nTransmitOptions =
				CZigBeeApplicationSupportBase::transmitAcknowledged);

	// Implementation
	public:
		// Revision of the Scenes client cluster
		static const unsigned short nClusterRevision;
};

#define __COMPACT7B_SCENECLIENT_CLUSTER_COMMANDS() \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandAddSceneResponse, &CQorvoClusterScenesClient::OnAddSceneResponse)\
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandViewSceneResponse, &CQorvoClusterScenesClient::OnViewSceneResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandRemoveSceneResponse, &CQorvoClusterScenesClient::OnRemoveSceneResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandRemoveAllScenesResponse, &CQorvoClusterScenesClient::OnRemoveAllScenesResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandStoreSceneResponse, &CQorvoClusterScenesClient::OnStoreSceneResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandGetSceneMembershipResponse, &CQorvoClusterScenesClient::OnGetSceneMembershipResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandEnhancedAddSceneResponse, &CQorvoClusterScenesClient::OnEnhancedAddSceneResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandEnhancedViewSceneResponse, &CQorvoClusterScenesClient::OnEnhancedViewSceneResponse) \
    __COMPACT7B_COMMAND_S2C(CQorvoClusterScenesClient::commandCopySceneResponse, &CQorvoClusterScenesClient::OnCopySceneResponse) \
    /*End*/

#endif // _QORVO_CLUSTER_SCENE_CLIENT_H_ 
