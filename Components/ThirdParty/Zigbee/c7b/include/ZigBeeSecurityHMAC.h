// ZigBeeSecurityHMAC.h : header file
//
// Copyright(C) 2011-2012 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// This is an implementation of ZigBee's HMAC cryptogaphic hash message
// authentication code, based on the Matyas-Meyer-Oseas cryptographic hash

/////////////////////////////////////////////////////////////////////////////
// CZigBeeSecurityHMAC

class CZigBeeSecurityHMAC : public CZigBeeSecurityMMO
{
	// Operations
	public:
		static void CalculateHashMessageAuthenticationCode
			(unsigned char abMAC[blockSize], const unsigned char *const pbKey,
			const size_t cbKey, const void *const pData, const size_t cbData);
};
