// PacketQueue.h : header file
//
// Copyright(c) 2008 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Part of the ubisys(R) Compact15.4(TM) class library
//
// Provides a packet queue implementation that can be used to store incoming
// or outgoing frames

/////////////////////////////////////////////////////////////////////////////
// CPacketCookie

class CPacketCookie
{
	// Implementation
	public:
		virtual ~CPacketCookie();
};


/////////////////////////////////////////////////////////////////////////////
// CPacketEx

class CPacketEx : public CPacket
{
	// Construction
	public:
		CPacketEx(const CPacket &packet, CPacketCookie *const pCookie = 0);

	// Attributes
	public:
		// Holds information that can be stored together with the packet in a
		// packet queue
		CPacketCookie *m_pCookie;

	// Implementaton
	public:
		virtual ~CPacketEx();

	private:
		// CPacketEx objects cannot be copied (cookie is allocated dynamically)
		CPacketEx(const CPacketEx &packet);
		CPacketEx &operator=(const CPacketEx &packet);
};


/////////////////////////////////////////////////////////////////////////////
// CPacketQueue

typedef std::list<CPacketEx *> CPacketQueue;
