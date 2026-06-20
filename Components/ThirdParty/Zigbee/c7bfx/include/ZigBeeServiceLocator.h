// ZigBeeServiceLocator.h : header file
//
// Copyright(C) 2012-2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// This module offers a reusable service discovery manager, which identifies
// a suitable peer via descriptor matching and returns the short and extended
// addresses of that peer

/////////////////////////////////////////////////////////////////////////////
// CZigBeeServiceLocator

class CZigBeeServiceLocator : public CCompactTimerClient
{
	// typedefs & enums
	public:
		// Called when service-provider has been located. Should return true
		// to delete the service locator object, false otherwise
		typedef bool (CZigBeeCluster::*CALLBACK)
			(CZigBeeServiceLocator &locator, const unsigned char nStatus);

	// Construction
	protected:
		CZigBeeServiceLocator(CZigBeeCluster &cluster,
			const CZigBeeAddress &address,
			const std::set<unsigned short> *const pInput = 0,
			const std::set<unsigned short> *const pOutput = 0,
			const CALLBACK pfnComplete = 0, void *const pArgument = 0,
			const uint8_t enhancedServiceDiscoveryOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			CZigBeeServiceLocator **const reference = 0);

		~CZigBeeServiceLocator();

	// Attributes
	public:
		// Address of the mating service including short and extended address,
		// as well as application endpoint
		CZigBeeAddress m_address;

		// The related cluster instance
		CZigBeeCluster &m_cluster;

	// Operations
	public:
		// Create a locator instance on the heap
		static void Create(CZigBeeCluster &cluster,
			const CZigBeeAddress &address,
			const std::set<unsigned short> *const pInput = 0,
			const std::set<unsigned short> *const pOutput = 0,
			const CALLBACK pfnComplete = 0, void *const pArgument = 0,
			const uint8_t enhancedServiceDiscoveryOptions =
				CZigBeeApplicationSupportBase::priorityMedium,
			CZigBeeServiceLocator **const reference = 0);

		// Destroy and free a locator instance from the heap
		static bool Delete(CZigBeeServiceLocator *&instance);

		// Can be called as soon as network startup has completed successfully
		// and either a service needs to be discovered via match descriptor
		// or an address needs to be resolved. Calls the completion handler
		// and deletes the service locator object afterwards
		void Resolve();

	// Implementation
	public:
		// Time between match descriptor attempts
		enum { attemptTicks = 60 * 62500u, attemptJitterTicks = 62500u };

		// Set of input and output clusters
		std::set<unsigned short> m_in, m_out;

		// Called when service discovery is complete
		CALLBACK m_pfnComplete;

		// Supplied to the callback when service discovery is complete
		void *m_pArgument;

		// If the locating process can be prematurely ended, this refers to the
		// instance pointer that references this object, or null otherwise
		CZigBeeServiceLocator **const m_reference;

		// Current attempt count. Operation fails if attempt limit is reached
		unsigned char m_nAttempt;

		// Number of match descriptor attempts before operation fails. If equal
		// to 0xff there is no limit on match descriptor attempts
		unsigned char m_nMatchDescriptorAttemptLimit;

		// Number of network address or IEEE address lookup attempts before
		// operation fails or service discovery is attempted
		unsigned char m_nAddressLookupAttemptLimit;

		// Timer that is used to start service localization with jitter delay
		CCompactTimer *m_pTimer;

		// Enhanced transmission options for performing service discovery.
		// By default, reflection is disabled so that the local entity will
		// be excluded during an ongoing service discovery.
		uint8_t m_enhancedServiceDiscoveryOptions;

		// ZDO request transmission that is pending responses
		CZigBeeDeviceObjectRequest *m_request;

		// To prevent concurrent network address lookups for the same EUI-64
		// address, APS offers a network address lookup service that initiates
		// a ZDO lookup only upon receiving an initial query request and then
		// captures the subsequent requests' completion callback handlers if
		// querying the same EUI-64 address. Once the ZDO lookup is finished,
		// captured completion handlers are called for all relevant queries.
		// If a ZDO lookup already exists for a locally initiated network
		// address query, we need to record this scenario for enabling proper
		// handling if the local query needs to be aborted later before its
		// completion.
		bool m_externalNetworkAddressLookup;

		// Invoked when the ZDO match descriptor request has completed
		static void OnMatchDescriptorsComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Invoked when the network address request has completed
		static void OnNetworkAddressLookupComplete(const CZigBeeAddress &address,
			void *pArgument, const unsigned char nStatus);

		// Invoked when the ZDO IEEE address request has completed
		static void OnExtendedAddressLookupComplete(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Starts a new service discovery attempt
		void StartServiceDiscovery(void *pArgument = 0);

		// Starts a new network address lookup attempt
		void LookupNetworkAddress(void *pArgument = 0);

		// Starts a new extended address lookup attempt
		void LookupExtendedAddress(void *pArgument = 0);

		// Completes the request with the specified status
		void Complete(const unsigned char nStatus);

		// Terminate the locating process if necessary
		bool Terminate(const uint_fast8_t status);
};
