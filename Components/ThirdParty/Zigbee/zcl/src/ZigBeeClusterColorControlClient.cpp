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

#include "QorvoClusterColorControlClient.h"

#define GP_COMPONENT_ID GP_COMPONENT_ID_UBIZPRO
#include "gpLog.h"

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControlClient
const unsigned short CQorvoClusterColorControlClient::nClusterRevision =
    CQorvoClusterColorControlClient::revision;

CQorvoClusterColorControlClient::CQorvoClusterColorControlClient
	(CZigBeeFoundationApplication &application) :
CZigBeeCluster(application, id, flagOutbound | flagOperational)
{
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControlClient client to server command handlers

void CQorvoClusterColorControlClient::MoveToColor(const CZigBeeAddress &destination,
    unsigned short colorX,
    unsigned short colorY,
    unsigned short transitionTime)
{
    CPacket moveToColorPacket;
    unsigned char pData[2+2+2];
    unsigned short index;

    index = 0;
    UInt16_api2buf(pData, &colorX, 1, &index);
    UInt16_api2buf(pData, &colorY, 1, &index);
    UInt16_api2buf(pData, &transitionTime, 1, &index);

    CreateRequest(moveToColorPacket,
                  static_cast<unsigned char>(commandMoveToColor),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    moveToColorPacket.Append(pData, index);

    OnRequestData(destination, moveToColorPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::MoveColor(const CZigBeeAddress &destination,
    unsigned short colorX,
    unsigned short colorY)
{
    CPacket moveColorPacket;
    unsigned char pData[2+2];
    unsigned short index;

    index = 0;
    UInt16_api2buf(pData, &colorX, 1, &index);
    UInt16_api2buf(pData, &colorY, 1, &index);

    CreateRequest(moveColorPacket,
                  static_cast<unsigned char>(commandMoveColor),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    moveColorPacket.Append(pData, index);

    OnRequestData(destination, moveColorPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::StepColor(const CZigBeeAddress &destination,
    signed short rateX,
    signed short rateY)
{
    CPacket stepColorPacket;
    unsigned char pData[2+2];
    unsigned short index;

    index = 0;
    Int16_api2buf(pData, &rateX, 1, &index);
    Int16_api2buf(pData, &rateY, 1, &index);

    CreateRequest(stepColorPacket,
                  static_cast<unsigned char>(commandStepColor),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    stepColorPacket.Append(pData, index);

    OnRequestData(destination, stepColorPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::MoveToHueSaturation(const CZigBeeAddress &destination,
    unsigned char hue,
    unsigned char saturation,
    unsigned short transitionTime)
{
    CPacket moveToHueSaturationPacket;
    unsigned char pData[1+1+2];
    unsigned short index;

    index = 0;
    UInt8_api2buf(pData, &hue, 1, &index);
    UInt8_api2buf(pData, &saturation, 1, &index);
    UInt16_api2buf(pData, &transitionTime, 1, &index);

    CreateRequest(moveToHueSaturationPacket,
                  static_cast<unsigned char>(commandMoveToHueAndSaturation),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    moveToHueSaturationPacket.Append(pData, index);

    OnRequestData(destination, moveToHueSaturationPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::ColorLoopSet(const CZigBeeAddress &destination,
    unsigned char updateFlags,
    unsigned char action,
    unsigned char direction,
    unsigned short time,
    unsigned short startHue)
{
    CPacket colorLoopSetPacket;
    unsigned char pData[1+1+1+2+2];
    unsigned short index;

    index = 0;
    UInt8_api2buf(pData, &updateFlags, 1, &index);
    UInt8_api2buf(pData, &action, 1, &index);
    UInt8_api2buf(pData, &direction, 1, &index);
    UInt16_api2buf(pData, &time, 1, &index);
    UInt16_api2buf(pData, &startHue, 1, &index);

    CreateRequest(colorLoopSetPacket,
                  static_cast<unsigned char>(commandColorLoopSet),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    colorLoopSetPacket.Append(pData, index);

    OnRequestData(destination, colorLoopSetPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::MoveHue(const CZigBeeAddress &destination,
            unsigned char mode,
            unsigned char rate)
{
    CPacket moveHuePacket;
    unsigned char pData[1+1];
    unsigned short index;

    index = 0;
    UInt8_api2buf(pData, &mode, 1, &index);
    UInt8_api2buf(pData, &rate, 1, &index);

    CreateRequest(moveHuePacket,
                  static_cast<unsigned char>(commandMoveHue),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    moveHuePacket.Append(pData, index);

    OnRequestData(destination, moveHuePacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::MoveToSaturation(const CZigBeeAddress &destination,
    unsigned char saturation,
    unsigned short transitionTime)
{
    CPacket moveToSaturationPacket;
    unsigned char pData[1+2];
    unsigned short index;

    index = 0;
    UInt8_api2buf(pData, &saturation, 1, &index);
    UInt16_api2buf(pData, &transitionTime, 1, &index);

    CreateRequest(moveToSaturationPacket,
                  static_cast<unsigned char>(commandMoveToSaturation),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    moveToSaturationPacket.Append(pData, index);

    OnRequestData(destination, moveToSaturationPacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

void CQorvoClusterColorControlClient::StepHue(const CZigBeeAddress &destination,
    unsigned char mode,
    unsigned char size,
    unsigned short transitionTime)
{
    CPacket stepHuePacket;
    unsigned char pData[1+1+1];
    unsigned short index;

    index = 0;
    UInt8_api2buf(pData, &mode, 1, &index);
    UInt8_api2buf(pData, &size, 1, &index);
    UInt8_api2buf(pData, &transitionTime, 1, &index);

    CreateRequest(stepHuePacket,
                  static_cast<unsigned char>(commandStepHue),
                  CZigBeeClusterLibraryHeader::typeCluster|CZigBeeClusterLibraryHeader::disableDefaultResponse,
                  0);
    stepHuePacket.Append(pData, index);

    OnRequestData(destination, stepHuePacket, CZigBeeApplicationSupport::transmitAcknowledged);
}

/////////////////////////////////////////////////////////////////////////////
// CQorvoClusterColorControlClient attribute map

__COMPACT7B_IMPLEMENT_ATTRIBUTE_MAP(CQorvoClusterColorControlClient,
	CZigBeeCluster)

__COMPACT7B_BEGIN_ATTRIBUTE_MAP(CQorvoClusterColorControlClient)
	__COMPACT7B_CLUSTERREVISION(CQorvoClusterColorControlClient)
__COMPACT7B_END_ATTRIBUTE_MAP()

