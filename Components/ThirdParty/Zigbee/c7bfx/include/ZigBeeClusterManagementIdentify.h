// ZigBeeClusterManagementIdentify.h : header file
//
// Copyright(C) 2014 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library
//
// Provides a client-side implementation of the identify cluster, suitable
// for running EZ-mode "finding & binding" on a management endpoint on
// behalf of other initiator/target endpoints on the device

/////////////////////////////////////////////////////////////////////////////
// forward references

class CZigBeeDeviceObjectBinding;


/////////////////////////////////////////////////////////////////////////////
// CZigBeeClusterManagementIdentify

class CZigBeeClusterManagementIdentify : public CZigBeeClusterIdentifyClient
{
	// typedefs & enums
	public:
		// Cluster command identifiers (server to client)
		enum { commandQueryIdentifyResponse };

	// Construction
	public:
		CZigBeeClusterManagementIdentify
			(CZigBeeFoundationApplication &application);

	// Operations
	public:
		// Performs finding and binding for the specified endpoint, which must
		// be either an initiator or target (or both). Use the flags parameter
		// to specify which kind of commissioning procedure is allowed, e.g.
		// to force initiator commissioning on endpoints that can act as, both,
		// target and initiator
		void RunFindingAndBinding(CZigBeeFoundationApplication &application,
			const unsigned char nFlags =
				CZigBeeFoundationApplication::commissioningInitiator |
				CZigBeeFoundationApplication::commissioningTarget);

		// Done identify query
		void OnQueryIdentifyComplete(const uint_fast8_t nStatus,
			CZigBeeClusterRequest &request);

	// Overridables
	public:
		// Enters finding and binding for the specified target application. The
		// default is to put the endpoint in identify mode
		virtual void EnterFindingAndBindingAsTarget
			(CZigBeeFoundationApplication &application);

		// Called when a binding has been created or removed as a result of
		// finding and binding
		virtual void AfterManageBinding(CZigBeeDeviceObjectBinding &binding,
			const bool bBind = true);

	// Implementation
	public:
		// The application endpoint for finding & binding. This can be either
		// a push-button commissioning initiator or target
		CZigBeeFoundationApplication *m_pApplication;

		// Performs finding and binding for the specified initiator endpoint
		void FindAndBindTargets(CZigBeeFoundationApplication &application);

		// Handles incoming simple descriptor responses
		static void OnSimpleDescriptorRequestComplete
			(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Finds commissionable clusters that match the local and remote set
		// of clusters provided. The cluster sets must be mating, i.e. the
		// local outbound set and the remote inbound set (or vice versa)
		void FindMatchingOperationalClusters(std::set<unsigned short> &matches,
			const std::set<unsigned short> &local,
			const std::set<unsigned short> &remote) const;

		// Extended address lookup for a binding target has completed
		static void OnExtendedAddressRequestComplete
			(const unsigned char nStatus,
			const CZigBeeDeviceObjectRequest &request);

		// Either creates or removes the specified binding
		void ManageBinding(CZigBeeDeviceObjectBinding &binding,
			const bool bBind = true);

		// Enters identify mode for the specified target application
		void EnterIdentify(CZigBeeFoundationApplication &application);
};
