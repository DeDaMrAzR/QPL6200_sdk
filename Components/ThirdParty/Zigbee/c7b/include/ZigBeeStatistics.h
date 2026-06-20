// ZigBeeStatistics.h : header file
//
// Copyright(C) 2017 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) Compact7B(TM) ZigBee class library.
//
// Diagnostics cluster attributes (elements shared with core stack)

/////////////////////////////////////////////////////////////////////////////
// CZigBeeApplicationSupportStatistics

class CZigBeeApplicationSupportStatistics
{
	// Attributes
	public:
		// A counter that is incremented each time the APS layer receives a
		// broadcast
		uint16_t m_nBroadcastsReceived;

		// A counter that is incremented each time the APS layer transmits a
		// broadcast
		uint16_t m_nBroadcastsTransmitted;

		// A counter that is incremented each time the APS layer receives a
		// unicast
		uint16_t m_nUnicastsReceived;

		// A counter that is incremented each time the APS layer successfully
		// transmits a unicast
		uint16_t m_nUnicastsSuccessful;

		// A counter that is incremented each time the APS layer retries the
		// sending of a unicast
		uint16_t m_nUnicastsRetried;

		// A counter that is incremented each time the APS layer fails to send
		// a unicast
		uint16_t m_nUnicastsFailed;

		// A counter that is incremented each time a message is dropped at the
		// APS layer because the APS frame counter was not higher than the last
		// message seen from that source
		uint16_t m_nFrameCounterFailures;

		// A counter that is incremented each time an APS encrypted message was
		// received but dropped because decryption failed
		uint16_t m_nDecryptionFailures;

		// A counter that is incremented each time a message is dropped at the
		// APS layer because it had APS encryption but the key associated with
		// the sender has not been authenticated, and thus the key is not
		// authorized for use in APS data messages
		uint16_t m_nUnauthorizedKeys;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeNetworkStatistics

class CZigBeeNetworkStatistics
{
	// Attributes
	public:
		// A counter that is incremented each time a route request is initiated
		uint16_t m_nRouteDiscoveriesInitiated;

		// A counter that is incremented each time an entry is added to the
		// neighbor table
		uint16_t m_nNeighborAdditions;

		// A counter that is incremented each time an entry is removed from the
		// neighbor table
		uint16_t m_nNeighborRemovals;

		// A counter that is incremented each time a neighbor table entry
		// becomes stale because the neighbor has not been heard from
		uint16_t m_nNeighborExpirations;

		// A counter that is incremented each time a node joins or rejoins the
		// network via this node
		uint16_t m_nJoinIndications;

		// A counter that is incremented each time an entry is removed from the
		// child table
		uint16_t m_nChildHandovers;

		// A counter that is incremented each time a unicast packet is relayed
		uint16_t m_nUnicastsRelayed;

		// A counter that is incremented each time a message is dropped at the
		// network layer because the network frame counter was not higher than
		// the last message seen from that source
		uint16_t m_nFrameCounterFailures;

		// A counter that is incremented each time a NWK encrypted message was
		// received but dropped because decryption failed
		uint16_t m_nDecryptionFailures;
};


/////////////////////////////////////////////////////////////////////////////
// CZigBeeStatistics

class CZigBeeStatistics
{
	// Attributes
	public:
		// Statistics related to the IEEE 802.15.4 medium access control layer
		CIEEE802154MACStatistics m_mac;

		// Statistics related to the application support layer
		CZigBeeApplicationSupportStatistics m_aps;

		// Statistics related to the network layer
		CZigBeeNetworkStatistics m_nwk;

		// A counter that is incremented each time the stack failed to allocate
		// a packet buffer
		uint16_t m_nPacketBufferAllocationFailures;

		// A counter that is incremented each time a packet was dropped due to
		// a packet validation error. This could be due to length or other
		// formatting problems in the packet
		uint16_t m_nPacketValidationErrors;
};

extern CZigBeeStatistics theZigBeeStatistics;
