/*
 * Copyright (c) 2013-2016, GreenPeak Technologies
 * Copyright (c) 2017-2019, Qorvo Inc
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

#ifndef _GP_MACDISPATCHER_MULTISTACK_H_
#define _GP_MACDISPATCHER_MULTISTACK_H_

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

#include "global.h"
#include "gpMacCore.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


/*****************************************************************************
 *                    Single/Dual Stack common definitions
 *****************************************************************************/

/**
 * @file gpMacDispatcher.h
 * @brief The MacDispatcher locks the lower layer before processing any request, after the cbConfirm the lock is released.
 *
 * @defgroup INIT INIT primitives
 * This module groups the initialization routines.
 *
 * @defgroup DATA DATA primitives
 * This module groups the primitives for data transmission.
 * - gpMacDispatcher_DataRequest + cbConfirm, cbIndication
 * - gpMacDispatcher_PollRequest + cbConfirm, cbIndication
 *
 * @defgroup MANAGEMENT MANAGEMENT primitives
 * - gpMacDispatcher_ScanRequest
 * - gpMacDispatcher_AssociateRequest
 * - gpMacDispatcher_AssociateResponse
 *
 * @defgroup ATTRIBUTE ATTRIBUTE primitives
 * - gpMacDispatcher_ScanRequest
 * - gpMacDispatcher_AssociateRequest
 * - gpMacDispatcher_AssociateResponse
 */

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
#include "gpMacDispatcher_CodeJumpTableFlash_Defs_MultiStack.h"
#endif // defined(GP_DIVERSITY_JUMPTABLES) && defined(GP_DIVERSITY_ROM_CODE)
/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_START */

/** @ingroup INIT
 * @brief This function initializes the MacCore layer. It should be called before calling any other request function.
 *
 */
GP_API void gpMacDispatcher_Init(void);

/** @ingroup INIT
 * @brief This function deinitializes the MacCore layer.
 *
 */
GP_API void gpMacDispatcher_DeInit(void);

/** @ingroup INIT
 * @brief This function resets the MAC layer.
 *
 * This function resets the MAC layer. If 'setDefaultPib' is set to true, it also resets the PIB variables, including
 * the IEEE802.15.4-2006 standard values. The transceiver will be switched off. All the buffered packets will be
 * removed. The callback functions (specified using the gpMacDispatcher_RegisterCallbackFunctions function) are
 * registered. The interrupts are enabled.
 *
 * @param setDefaultPib     If true the PIB values are reset to their default value (as specified in the
 *                          IEEE802.15.4-2006 specification).
 * @param stackId           The identifier of the stack
 *
 * @note A prerequisite; the invoker of this function should first acquire the Mac Dispatcher lock before a reset is
 *       allowed
 *
 * @return Whether this stack has successfully issued a reset command on the MAC network layer access (true) or not
 *         (false).
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_Reset(Bool setDefaultPib, gpMacDispatcher_StackId_t stackId);

/** @ingroup LOCK
 * This function is used to request the lock for the MAC network layer access. This lock must be acquired by a stack
 * before it may access the MAC network layer.
 *
 * @param stackId           The identifier of the stack that requests the lock.
 *
 * @return Whether this stack has successfully acquired the lock on the MAC network layer access (true) or not (false).
 */
GP_API Bool gpMacDispatcher_LockClaim(gpMacDispatcher_StackId_t stackId);

/** @ingroup LOCK
 * This function is used to release the lock for the MAC network layer access.
 *
 * @param stackId           The identifier of the stack.
 */
GP_API void gpMacDispatcher_LockRelease(gpMacDispatcher_StackId_t stackId);

/** @ingroup LOCK
 * This function can be used to check whether the stack id already has the lock on the MAC network layer access.
 *
 * @param stackId           The identifier of the stack
 *
 * @return Whether this stack has the lock on the MAC network layer access (true) or not (false).
 */
GP_API Bool gpMacDispatcher_LockedByThisStack(gpMacDispatcher_StackId_t stackId);

/** @ingroup LOCK
 * This function can be used to check whether any stack has the lock on the MAC network layer access.
 *
 * @return  Whether a stack has the lock on the MAC network layer access (true) or not (false).
 */
GP_API Bool gpMacDispatcher_Locked( void );

/** @ingroup INIT
 * @brief This function is used to register a network stack to the dispatcher.
 *
 * @return The id that is allocated by the MacCore for this stack
 */
GP_API gpMacDispatcher_StackId_t gpMacDispatcher_RegisterNetworkStack(gpMacDispatcher_StringIdentifier_t* stringIdentifier);

/** @brief Unregisters a network stack to the MAC layer.
 *
 * @param stackId           the stack ID of the stack that needs to be removed from the MAC layer
 *
 * @return Result of the unregistering. Possible results:
 *         - gpMacDispatcher_ResultSuccess
 *         - gpMacDispatcher_ResultInvalidParameter - stackId is not registered or not in supported range
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_UnRegisterNetworkStack(gpMacDispatcher_StackId_t stackId);

/** @brief Returns if a stackId is in range and registered
 *
 * @param stackId           the stack ID to check
 *
 * @return true if stackId is registered and in range, false if stackId is not registered or not in supported range
 */
GP_API Bool gpMacDispatcher_IsValidStack(gpMacDispatcher_StackId_t stackId);

/** @ingroup DATA
 * @brief This function is used to send a data packet to another device.
 *
 * This function is used to send a data packet conform the IEEE802.15.4-2006 MAC specification.
 *
 * @param srcAddrMode       The address modes to be used for source address. This parameter is used as in the Frame
 *                          Control Field of an IEEE802.15.4-2006 packet. We refer to the definition
 *                          gpMacCore_AddressMode_t.
 * @param pDstAddrInfo      All the information about the destination (address mode, address and panId).
 * @param txOptions         Tx Options byte (see IEEE802.15.4-2006).
 * @param pSecOptions       The security options - maybe NULL to be used (see IEEE802.15.4-2006).
 * @param p_PdLoh           The PD Length/Offset/Handle of the data payload.
 * @param stackId           The stack doing this data request
 */
GP_API void gpMacDispatcher_DataRequest(gpMacCore_AddressMode_t srcAddrMode, gpMacCore_AddressInfo_t* pDstAddrInfo,
                                        UInt8 txOptions, gpMacCore_Security_t* pSecOptions,
                                        gpMacCore_MultiChannelOptions_t multiChannelOptions, gpPd_Loh_t pdLoh,
                                        gpMacDispatcher_StackId_t stackId);

/** @ingroup DATA
 * @brief Schedule a packet for timed transmission.
 *
 * To send a packet via timed transmission, perform two steps:
 * - Call gpMacDispatcher_DataRequest() with option GP_MACCORE_TX_OPT_TIMEDTX to queue the packet;
 * - Call gpMacDispatcher_ScheduleTimedTx() to schedule transmission
 * of the queued packet.
 *
 * This function is only available on hardware that supports a timed transmission queue.
 *
 * @param pdHandle          Handle of TX packet to schedule for TX; must have been previously submitted via
 *                          gpMacDispatcher_DataRequest().
 * @param timingOptions     Specification of scheduled transmit time.
 * @param stackId           The identifier of the stack
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_ScheduleTimedTx(
    gpPd_Handle_t pdHandle, gpMacCore_TxTimingOptions_t timingOptions, gpMacDispatcher_StackId_t stackId);

/** @ingroup MANAGEMENT
 * @brief This function is used to start a scan.
 *
 * This function starts a scan conform the IEEE802.15.4-2006 spec. A scan can be issued to detect a channel with the
 * least amount of interference, to find networks, etc.
 *
 * @param scanType          This parameter defines which of the 3 implemented scans needs to be performed: ED, active or
 *                          orphan scan.
 * @param scanChannels      This parameter defines which channels need to be scanned. It is a bitmask where bit 0 must
 *                          be '1' to select channel 0, bit 26 '1' to select channel 26, etc. Since the chip works in
 *                          the 2.4GHz band, only channels 11 till 26 can be selected. If other channels are selected,
 *                          they are ignored and only the selected channels between channel 11 and channel 26 are
 *                          scanned.
 * @param scanDuration      This parameter specifies how long a channel will be scanned. The scantime equals
 *                          [GP_MACCORE_BASE_SUPERFRAME_DURATION *  (2^scanDuration + 1)]symbols, where 1 symbol is
 *                          16us.
 * @param resultListSize    The length of the resultList (in bytes). If an ED scan is done, this should equal the amount
 *                          of channels. If resultListSize is higher than the amount of channels that needs to be
 *                          scanned there is no problem. If the value is lower, the scan is terminated when the list is
 *                          full. If an active scan is selected, resultListSize is the amount of PANDescriptors that can
 *                          be saved. If it is an orphan scan, resultListSize is 0.
 * @param pResultList       This is a pointer to an array where the result of the scan can be saved. If an ED scan is
 *                          done, the size is resultListSize. If an active scan is issued the size is
 *                          resultListSize*sizeof(gpMacCore_PanDescriptor_t).
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_ScanRequest(gpMacCore_ScanType_t scanType, UInt32 scanChannels, UInt8 scanDuration,
                                        UInt8 resultListSize, UInt8* pResultList, gpMacDispatcher_StackId_t stackId);

/** @ingroup MANAGEMENT
 * @brief This function is issued by a device who wishes to join a network.
 *
 * This function starts the associate procedure. It is used to join a network. When called, the AssociateRequest
 * function will construct an associate request command packet, and send it to the coordinator.
 *
 * @param logicalChannel            The channel that will be used to do the association attempt
 * @param pCoordAddrInfo            The coordinator addressing info (address mode, panId, address)
 * @param capabilityInformation     Specifies the operational capabilities of the associating device.
 * @param stackId                   The identifier of the stack
 */
GP_API void gpMacDispatcher_AssociateRequest(UInt8 logicalChannel, gpMacCore_AddressInfo_t* pCoordAddrInfo,
                                             UInt8 capabilityInformation, gpMacDispatcher_StackId_t stackId);

/** @ingroup MANAGEMENT
 * @brief This function is used to send an associate response command packet to a device that is trying to join. The
 * associate response command packet contains the results of the join procedure.
 *
 * This function is used to respond to a previously received associate request command packet. Upon reception of an
 * associate request command packet, the AssociateIndication function is called to inform the higher layer. The next
 * higher layer then decides to allow the joining device in the network or not. Its decision is sent back to the joining
 * device through the associate response packet.
 *
 * @param pDeviceAddress            A pointer to the address of the device that is trying to join.
 * @param associateShortAddress     If the joining device is allowed (successful association), this parameter contains
 *                                  the short address that was allocated for the device. If the association was
 *                                  unsuccessful, this parameter is set to 0xffff.
 * @param status                    This parameter contains the result of the association attempt. Possible values are:
 *                                  - 0x0: Association successful.
 *                                  - 0x1: PAN at capacity.
 *                                  - 0x2: PAN access denied.
 * @param stackId                   The identifier of the stack
 */
GP_API void gpMacDispatcher_AssociateResponse(MACAddress_t* pDeviceAddress, UInt16 associateShortAddress,
                                              gpMacDispatcher_Result_t status, gpMacDispatcher_StackId_t stackId);

/** @ingroup DATA
 * @brief This function is used to poll a coordinator for data by sending a data request command. If the coordinator has
 * data pending for the device that issued the poll request, it will forward the data.
 *
 * This function starts a poll. This function is used to request packets that are pending at the coordinator (by sending
 * a data request command to the coordinator). If the coordinator has data pending for the device that issued the poll
 * request, it will forward the data. If a short address is allocated to the device, and it was written into the
 * gpMacCore_AttributeShortAddress PIB attribute, the short address will be used as source address of the data
 * request command. If the gpMacCore_AttributeShortAddress PIB attribute is set to 0xffff or 0xfffe, the device will use
 * its extended address as source address in the data request command packet.
 *
 * @param pCoordAddrInfo    All the details regarding the coordinator
 * @param pSecOptions       structure with security options, if NULL no security is used.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_PollRequest(gpMacCore_AddressInfo_t* pCoordAddrInfo, gpMacCore_Security_t* pSecOptions,
                                        gpMacDispatcher_StackId_t stackId);

/** @ingroup DATA
 * @brief This function will remove the specified transmit request form the indirect transmission queue
 *
 * @param handle            The handle to be purged
 * @param stackId           The identifier of the stack request
 */
GP_API void gpMacDispatcher_PurgeRequest(gpPd_Handle_t pdHandle, gpMacDispatcher_StackId_t stackId);

/** @ingroup MANAGEMENT
 * @brief This function is used to send an orphan response command packet.
 *
 * @ param pOrphanAddress
 * @ param shortAddress
 * @ param associatedMember
 * @ param stackId          The identifier of the stack
 */
GP_API void gpMacDispatcher_OrphanResponse(MACAddress_t* pOrphanAddress, UInt16 shortAddress, Bool associatedMember,
                                           gpMacDispatcher_StackId_t stackId);

/** @brief This function is used to start a new superframe specification. As this is a non-beacon enabled MAC, it is
 *  mostly used when starting a coordinator.
 *
 * This function is used to start a new superframe specification. As this is a non-beacon enabled MAC, it is mostly
 * used when starting a coordinator. It will set the values for the superframe.
 *
 * @param panId             Sets the panId that will be used.
 * @param logicalChannel    Sets the logicalChannel (the channel the network will operate on).
 * @param panCoordinator    Sets panCoordinator TRUE or FALSE. If the device is a PAN coordinator, this parameter must
 *                          be set to TRUE.
 * @param stackId           The identifier of the stack
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_Start(gpMacCore_PanId_t panId, UInt8 logicalChannel,
                                                      Bool panCoordinator, gpMacDispatcher_StackId_t stackId);

// getters and setters
/** @ingroup ATTRIBUTE
 * @brief This function set the current channel.
 *
 * @param channel           The channel to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetCurrentChannel(UInt8 channel, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function set the 3 Rx channels.
 *
 * @param pChannel          The pointer of channel list.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMultipleChannelTable(UInt8* pChannel, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the current channel.
 *
 * @param stackId           The identifier of the stack
 *
 * @return current channel
 */
GP_API UInt8 gpMacDispatcher_GetCurrentChannel(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This functions set the default transmit power for each channel
 *
 * @param pDefaultTransmitPowerTable    pointer to 16 byte array with default transmit power (in dBm) for each
 *                                      IEEE channel (11..26).
 *
 */
GP_API void gpMacDispatcher_SetDefaultTransmitPowers(Int8* pDefaultTransmitPowerTable);

/** @ingroup ATTRIBUTE
 * @brief This function set the transmission power.
 *
 * @param transmitPower     The transmission power to set in dBm, or one of gpMacDispatcher_TxPowerMin, TxPowerMax,
 *                          TxPowerDefault.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetTransmitPower(gpMacCore_TxPower_t transmitPower, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the transmission power.
 *
 * @param stackId           The identifier of the stack
 *
 * @return transmit power
 */
GP_API Int8 gpMacDispatcher_GetTransmitPower(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function set the CCA mode.
 *
 * @param cCAMode           The CCA mode to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetCCAMode(UInt8 cCAMode, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the CCA mode.
 *
 * @param stackId           The identifier of the stack
 *
 * @return CCA mode
 */
GP_API UInt8 gpMacDispatcher_GetCCAMode(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the coordinator's extended MAC address.
 *
 * @param pCoordExtendedAddress     A pointer to the address to set.
 * @param stackId                   The identifier of the stack
 */
GP_API void gpMacDispatcher_SetCoordExtendedAddress(MACAddress_t* pCoordExtendedAddress,
                                                    gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the coordinator's extended MAC address.
 *
 * @param pCoordExtendedAddress     A pointer to the address to read.
 * @param stackId                   The identifier of the stack
 */
GP_API void gpMacDispatcher_GetCoordExtendedAddress(MACAddress_t* pCoordExtendedAddress,
                                                    gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the coordinator's short MAC address.
 *
 * @param addr              A pointer to the address to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetCoordShortAddress(UInt16 addr, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the coordinator's short MAC address.
 *
 * @param stackId           The identifier of the stack
 *
 * @return The Coordinator's short address
 */
GP_API UInt16 gpMacDispatcher_GetCoordShortAddress(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the panCoordinator property for a stack.
 *
 * @param panCoordinator    True if the stack is pan coordinator, false if not.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetPanCoordinator(Bool panCoordinator, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the panCoordinator property of a stack.
 *
 * @param stackId           The identifier of the stack
 *
 * @return true if the device is the pan coordinator, else false
 */
GP_API Bool gpMacDispatcher_GetPanCoordinator(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the MAC DSN.
 *
 * @param DSN               The DSN value to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetDsn(UInt8 dsn, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the MAC DSN.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the MAC DSN
 */
GP_API UInt8 gpMacDispatcher_GetDsn(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the maximum CSMA backoffs.
 *
 * @param maxCsmaBackoffs   The maximum CSMA backoffs to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMaxCsmaBackoffs(UInt8 maxCsmaBackoffs, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the maximum CSMA backoffs
 *
 * @param stackId           The identifier of the stack
 *
 * @return the maximum number of CSMA backoffs
 */
GP_API UInt8 gpMacDispatcher_GetMaxCsmaBackoffs(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the minimum BE.
 *
 * @param minBE             The minimum BE to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMinBE(UInt8 minBE, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the minimum BE.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the minimum BE
 */
GP_API UInt8 gpMacDispatcher_GetMinBE(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the maximum BE.
 *
 * @param maxBE             The maximum BE to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMaxBE(UInt8 maxBE, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the maximum BE.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the maximum BE
 */
GP_API UInt8 gpMacDispatcher_GetMaxBE(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the CSMA mode used when transmitting
 *
 * @param csmaMode          The CSMA mode to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetCsmaMode(UInt8 csmaMode, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the CSMA mode.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the CSMA mode
 */
GP_API UInt8 gpMacDispatcher_GetCsmaMode(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the PanId.
 *
 * @param panId             The PanId to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetPanId(UInt16 panId, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the PanId.
 *
 * @param stackId           The identifier of the stack
 *
 * @return The network PanId
 */
GP_API UInt16 gpMacDispatcher_GetPanId(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the RxOnWhenIdle attribute.
 *
 * @param rxOnWhenIdle      The RxOnWhenIdle attribute to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetRxOnWhenIdle(Bool rxOnWhenIdle, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the RxOnWhenIdle attribute.
 *
 * @param stackId           The identifier of the stack
 *
 * @return true when rx-on-when-idle is enabled, else false
 */
GP_API Bool gpMacDispatcher_GetRxOnWhenIdle(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the short MAC address of the device.
 *
 * @param shortAddress      The address to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetShortAddress(UInt16 shortAddress, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the short MAC address of the device.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the device's short address
 */
GP_API UInt16 gpMacDispatcher_GetShortAddress(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the association permit attribute.
 *
 * @param associationPermit The association permit value to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetAssociationPermit(Bool associationPermit, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the association permit attribute.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the value of the association permit attribute
 */
GP_API Bool gpMacDispatcher_GetAssociationPermit(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the beacon payload pointer.
 *
 * @param pBeaconPayload    The beacon payload pointer to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetBeaconPayload(UInt8* pBeaconPayload, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the beacon payload pointer.
 *
 * @param pBeaconPayload    The beacon payload pointer to get.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_GetBeaconPayload(UInt8* pBeaconPayload, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the beacon payload length.
 *
 * @param length            The beacon payload length to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetBeaconPayloadLength(UInt8 length, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the beacon payload length.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the beacon payload length
 */
GP_API UInt8 gpMacDispatcher_GetBeaconPayloadLength(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the promiscuous mode.
 *
 * @param promiscuousMode   The promiscuous mode value to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetPromiscuousMode(UInt8 promiscuousMode, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the promiscuous mode.
 *
 * @param stackId           The identifier of the stack
 *
 * @return true if the stack is in promiscuous mode, else false
 */
GP_API UInt8 gpMacDispatcher_GetPromiscuousMode(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 *
 * @param transactionPersistentTime     The transaction persistence time to set in superframe periods.
 * @param stackId                       The identifier of the stack
 */
GP_API void gpMacDispatcher_SetTransactionPersistenceTime(UInt16 time, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the transaction persistence time.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the transaction persistence time in superframe periods
 */
GP_API UInt16 gpMacDispatcher_GetTransactionPersistenceTime(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the extended MAC address of the device.
 *
 * @param pExtendedAddress  The pointer to the address to read.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetExtendedAddress(MACAddress_t* pExtendedAddress, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the extended MAC address of the device.
 *
 * @param pExtendedAddress  The pointer to the address to read.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_GetExtendedAddress(MACAddress_t* pExtendedAddress, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the number of transmission retries.
 *
 * @param numberOfRetries   The number of transmission retries to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetNumberOfRetries(UInt8 numberOfRetries, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the number of transmission retries.
 *
 * @param stackId           The identifier of the stack
 *
 * @return The number of Mac Retries that has to be attempted before a NO_ACK is generated
 */
GP_API UInt8 gpMacDispatcher_GetNumberOfRetries(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 *
 * @param securityEnabled   enable or disable stack security
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetSecurityEnabled(Bool securityEnabled, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 *
 * @param stackId           The identifier of the stack
 *
 * @return true if security is enabled, else false
 */
GP_API Bool gpMacDispatcher_GetSecurityEnabled(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the BeaconStarted attribute.
 *
 * @param BeaconStarted     The BeaconStarted attribute value to set.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetBeaconStarted(Bool BeaconStarted, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the BeaconStarted attribute.
 *
 * @param stackId           The identifier of the stack
 *
 * @return the value of the BeaconStarted attribute
 */
GP_API Bool gpMacDispatcher_GetBeaconStarted(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the tx antenna.
 *
 * @param txAntenna         The tx antenna to be used.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetTxAntenna(UInt8 txAntenna, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function gets the tx antenna.
 *
 * @param stackId           The identifier of the stack
 *
 * @return antenna index
 */
GP_API UInt8 gpMacDispatcher_GetTxAntenna(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function manipulates the indication of beacons by the MAC
 *
 * @param enable            When enabled, beacons will be indicated by the MAC to the specific stack
 * @param stackId           The identifier of the stack.
 */
GP_API void gpMacDispatcher_SetIndicateBeaconNotifications(Bool enable, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns IndicateBeaconNotifications
 *
 * @param stackId           The identifier of the stack.
 *
 * @return true if beacons are indicated to the stack, else false
 */
GP_API Bool gpMacDispatcher_GetIndicateBeaconNotifications(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 *
 * @param enable            When enable is true, poll indication will be forwarded by the MAC to the specific stack
 * @param stackId           The identifier of the stack.
 */
GP_API void gpMacDispatcher_SetForwardPollIndications(Bool enable, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 *
 * @param stackId           The identifier of the stack.
 *
 * @return true if Forward Poll indications are enabled, else false
 */
GP_API Bool gpMacDispatcher_GetForwardPollIndications(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Set LQI threshold for testing purposes
 *
 * @param lqiThreshold      threshold in dBm
 * @param stackId           The identifier of the stack.
 */
GP_API void gpMacDispatcher_SetLqiThresholdForTest(UInt8 lqiThreshold, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Change the default IEEE 802.15.4 addressmode for beacons
 *
 * @param addressMode       new address mode for beacons
 * @param stackId           The identifier of the stack.
 */
GP_API void gpMacDispatcher_SetAddressModeOverrideForBeacons(UInt8 addressMode, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function sets the mac version.
 *
 * @param MacVersion        The mac version of the stack.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMacVersion(gpMacCore_MacVersion_t macVersion, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function gets the mac version.
 *
 * @param stackId           The identifier of the stack.
 *
 * @return the stack's mac version
 */
GP_API gpMacCore_MacVersion_t gpMacDispatcher_GetMacVersion(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function Adds element to DataPending Queue
 *
 * @param pAddrInfo         Device to queue a packet for
 * @param stackId           The identifier of the stack.
 *
 * @return gpMacDispatcher_ResultSuccess on success
           gpMacDispatcher_ResultInvalidParameter if @pAddrInfo or @stackId is invalid
           gpMacDispatcher_ResultTransactionOverflow if no frame can be queued
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueAdd(gpMacCore_AddressInfo_t* pAddrInfo,
                                                                     gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function Removes element to DataPending Queue
 *
 * @param pAddrInfo         Device to queue a packet for
 * @param stackId           The identifier of the stack.
 *
 * @return gpMacDispatcher_ResultSuccess on success
           gpMacDispatcher_ResultInvalidParameter if @pAddrInfo or @stackId is invalid
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueRemove(gpMacCore_AddressInfo_t* pAddrInfo,
                                                                        gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function Removes all elements from the DataPending Queue
 *
 * @param stackId           The identifier of the stack.
 *
 * @return gpMacDispatcher_ResultSuccess on success
           gpMacDispatcher_ResultInvalidParameter on failure
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_DataPending_QueueClear(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function adds an address to the neighbour list (determines how to handle a poll request).
 *
 * @param pAddrInfo         Address information of the device to add
 * @param stackId           The identifier of the stack.
 *
 * @return Bool indicating success or fail
 */
GP_API Bool gpMacDispatcher_AddNeighbour(gpMacCore_AddressInfo_t* pAddrInfo, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function removes an address to the neighbour list (determines how to handle a poll request).
 *
 * @param pAddrInfo         Address information of the device to remove
 * @param stackId           The identifier of the stack.
 *
 * @return Bool indicating success or fail
 */
GP_API Bool gpMacDispatcher_RemoveNeighbour(gpMacCore_AddressInfo_t* pAddrInfo, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function removes all neighbours from neighbour list.
 *
 * @param stackId           The identifier of the stack.
 *
 * @return Bool indicating success or fail
 */
GP_API void gpMacDispatcher_ClearNeighbours(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access This function sets the mode to be used for datapending.
 *
 * @param dataPendingMode   mode from gpMacCore_DataPendingMode_t enumeration to set.
 * @param stackId           The identifier of the stack.
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_SetDataPendingMode(gpMacCore_DataPendingMode_t dataPendingMode,
                                                                   gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetFrameCounter(UInt32 frameCounter, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API UInt32 gpMacDispatcher_GetFrameCounter(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_SetKeyDescriptor(
    gpMacCore_KeyDescriptor_t* pKeyDescriptor, gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_GetKeyDescriptor(
    gpMacCore_KeyDescriptor_t* pKeyDescriptor, gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetKeyTableEntries(gpMacCore_KeyTablesEntries_t keyTableEntries,
                                               gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacCore_KeyTablesEntries_t gpMacDispatcher_GetKeyTableEntries(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_SetDeviceDescriptor(
    gpMacCore_DeviceDescriptor_t* pDeviceDescriptor, gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_GetDeviceDescriptor(
    gpMacCore_DeviceDescriptor_t* pDeviceDescriptor, gpMacCore_Index_t index, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetDeviceTableEntries(gpMacCore_DeviceTablesEntries_t deviceTableEntries,
                                                  gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacCore_DeviceTablesEntries_t gpMacDispatcher_GetDeviceTableEntries(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_SetSecurityLevelDescriptor(
    gpMacCore_SecurityLevelDescriptor_t* pSecurityLevelDescriptor, gpMacCore_Index_t index,
    gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacDispatcher_Result_t gpMacDispatcher_GetSecurityLevelDescriptor(
    gpMacCore_SecurityLevelDescriptor_t* pSecurityLevelDescriptor, gpMacCore_Index_t index,
    gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API gpMacCore_SecurityLevelTableEntries_t gpMacDispatcher_GetSecurityLevelTableEntries(
    gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetSecurityLevelTableEntries(
    gpMacCore_SecurityLevelTableEntries_t securityLevelTableEntries, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetDefaultKeySource(UInt8* pDefaultKeySource, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_GetDefaultKeySource(UInt8* pDefaultKeySource, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetPanCoordExtendedAddress(MACAddress_t* pPanCoordExtendedAddress,
                                                       gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_GetPanCoordExtendedAddress(MACAddress_t* pPanCoordExtendedAddress,
                                                       gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API void gpMacDispatcher_SetPanCoordShortAddress(UInt16 PanCoordShortAddress, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief Access method for the corresponding PIB attribute
 */
GP_API UInt16 gpMacDispatcher_GetPanCoordShortAddress(gpMacDispatcher_StackId_t stackId);

/** @brief Set Regional Domain Settings (RDS) for the specified stack.
 *
 * The RDS values consist of a list of blocked channels and a table of maximum power levels for non-blocked channels.
 * Any attempt to transmit/listen on a blocked channel will fail with gpMacDispatcher_ResultInvalidParameter. Transmit
 * power levels above maximum will be clipped to the maximum level.
 *
 * @param blockedChannelMask        Bitmask of blocked channels (IEEE numbers). E.g. set bit 11 to block channel 11.
 * @param pMaxTransmitPowerTable    Pointer to 16-byte table of maximum power levels for channels 11 .. 26.
 * @param stackId                   The identifier of the stack.
 */
GP_API void gpMacDispatcher_SetRegionalDomainSettings(
    UInt32 blockedChannelMask, gpMacCore_TxPower_t* pMaxTransmitPowerTable, gpMacDispatcher_StackId_t stackId);

/** @brief Get Regional Domain Settings (RDS) for the specified stack.
 *
 * The RDS values consist of a list of blocked channels and a table of maximum power levels for non-blocked channels.
 *
 * @param pBlockedChannelMask       Bitmask of blocked channels (IEEE numbers). E.g. set bit 11 to block channel 11.
 * @param pMaxTransmitPowerTable    Pointer to 16-byte table of maximum power levels for channels 11 .. 26.
 * @param stackId                   The identifier of the stack.
 */
GP_API void gpMacDispatcher_GetRegionalDomainSettings(
    UInt32* pBlockedChannelMask, gpMacCore_TxPower_t* pMaxTransmitPowerTable, gpMacDispatcher_StackId_t stackId);

/** @brief This function enables the Enhanced FramePending behavior for a specific stack.
 *
 * @param enableEnhancedFramePending    Enables or disables the Enhanced FramePending behavior for a specific stack.
 * @param stackId                       The identifier of the stack.
 */
void gpMacDispatcher_EnableEnhancedFramePending(Bool enableEnhancedFramePending, gpMacDispatcher_StackId_t stackId);

#ifdef GP_MACCORE_DIVERSITY_RAW_FRAMES
/** @brief This function enables the raw mode of the stack.
 *
 * This mode allows the upper stack to create the MAC header while transmitting a packets, and to stop the MacCore from
 * processing MAC headers, and instead passing them to the upper stack.
 *
 * @param rawModeEnabled    Enables or disables the raw mode for a specific stack.
 * @param stackId           The identifier of the stack.
 */
void gpMacDispatcher_SetStackInRawMode(Bool rawModeEnabled, gpMacDispatcher_StackId_t stackId);

/** @brief This function returns if the raw mode is enabled for a specific stack.
 *
 * @param stackId           The identifier of the stack.

 * @return true if raw mode is enabled, false when disabled for a specific stack.
 */
Bool gpMacDispatcher_GetStackInRawMode(gpMacDispatcher_StackId_t stackId);

/** @brief This function sets the encryption key and keyIndex to be used for the Tread raw encryption.
 *
 * @param encryptionKeyIdMode   Key ID mode. Only mode '1' is supported for Thread raw encryption.
 * @param encryptionKeyId       Current key index.
 * @param pCurrKey              Current key.
 * @param stackId               The identifier of the stack.
 */
void gpMacDispatcher_SetRawModeEncryptionKeys(gpMacCore_KeyIdMode_t encryptionKeyIdMode,
                                              gpMacCore_KeyIndex_t encryptionKeyId, UInt8* pCurrKey,
                                              gpMacDispatcher_StackId_t stackId);

/** @brief This function sets the fields used in the Nonce for the Thread raw encryption.
 *
 * @param frameCounter      Initial value of the frame pointer. Will be auto-incremented by the lower layers.
 * @param pExtendedAddress  Pointer to the local(source) extended address.
 * @param seclevel          Security Level. Needs to be '5' for Thread raw encryption.
 * @param stackId           The identifier of the stack.
 */
void gpMacDispatcher_SetRawModeNonceFields(UInt32 frameCounter, MACAddress_t* pExtendedAddress, UInt8 seclevel,
                                           gpMacDispatcher_StackId_t stackId);

/** @brief This function configures the insertion of a VS IE header in the Enh Ack frames containing probing data.
 *
 * @param linkMetrics       Bitmask of link metrics which should be reported.
 * @param pExtendedAddress  The extended address of the probing initiator.
 * @param shortAddress      The short address of the probing initiator.
 * @param stackId           The stack id.
 *
 * @return Success if the parameter are accepted, InvalidParameters otherwise.
 */
gpMacDispatcher_Result_t gpMacDispatcher_ConfigureEnhAckProbing(UInt8 linkMetrics, MACAddress_t* pExtendedAddress,
                                                                UInt16 shortAddress, gpMacDispatcher_StackId_t stackId);

#endif //GP_MACCORE_DIVERSITY_RAW_FRAMES

/** @ingroup ATTRIBUTE
 * @brief Enable or disable automatic toggling of TX antenna after transmission failure.
 *
 * @param enable            True to enable auto-toggling, false to disable.
 * @param stackId           The identifier of the stack.
 */
#ifdef GP_DIVERSITY_TX_ANTENNA_DIVERSITY
GP_API void gpMacDispatcher_SetAutoTxAntennaToggling(Bool enable, gpMacDispatcher_StackId_t stackId);
#else
#define gpMacDispatcher_SetAutoTxAntennaToggling(Bool, gpMacDispatcher_StackId_t)
#endif //GP_DIVERSITY_TX_ANTENNA_DIVERSITY
/** @ingroup ATTRIBUTE
 * @brief Return the current state of automatic toggling of TX antenna.
 *
 * @param stackId           The identifier of the stack.
 *
 * @return true when enabled, false when disabled.
 */
GP_API Bool gpMacDispatcher_GetAutoTxAntennaToggling(gpMacDispatcher_StackId_t stackId);

/** @brief Get the current time
 *
 * Retrieves the current local time on the chip.
 *
 * @return the current local time on chip in us.
 */
GP_API UInt32 gpMacDispatcher_GetCurrentTimeUs(void);

/** @brief Enables or disables the Retransmits on CCA failures.
 *
 * This function enables or disables the Retransmits on CCA failures. The actual amount of retransmits is set through
 * gpMacDispatcher_SetNumberOfRetries. As such this function only changes the default MAC 802.15.4-2015 behavior to also
 * retransmit on a CCA fail.
 *
 * @param enable            Enables or disables the Retransmits.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetRetransmitOnCcaFail(Bool enable, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief returns if the feature "Retransmits on CCA fail" is enabled for a stackId.
 *
 * This function returns if the feature "Retransmits on CCA fail" is enabled for a stackId.
 *
 * @param stackId           The identifier of the stack
 *
 * @return returns true if "Retransmits on CCA fail" is enabled, else false
 */
GP_API Bool gpMacDispatcher_GetRetransmitOnCcaFail(gpMacDispatcher_StackId_t stackId);

/** @brief Enables or disables the random delays between subsequent retransmits.
 *
 * This function enables or disables the random delays between subsequent retransmits. The actual amount of retransmits
 * is set through gpMacDispatcher_SetNumberOfRetries. As such this function only changes the default MAC 802.15.4-2015
 * behavior to add random delays between retransmits.
 *
 * @param enable            Enables or disables the random delays between subsequent retransmits.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetRetransmitRandomBackoff(Bool enable, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief returns if the feature "random delays between subsequent retransmits" is enabled for a stackId.
 *
 * This function returns if the feature "random delays between subsequent retransmits" is enabled for a stackId.
 *
 * @param stackId           The identifier of the stack
 *
 * @return true if "random delays between subsequent retransmits" is enabled, else false
 */
GP_API Bool gpMacDispatcher_GetRetransmitRandomBackoff(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function configures the minimum backoff exponent for random delays between retransmits.
 *
 * This function configures the minimum backoff exponent for random delays between retransmits due to NoAck failures.
 * See Thread v1.2.1 paragraph 3.2.7.4: Mac layer retries.
 * Not to be confused with the backoff exponent for random delays between CCA failures!
 *
 * @param minBERetransmit   The minimum backoff exponent.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMinBeRetransmit(UInt8 minBERetransmit, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the minimum backoff exponent for random delays between retransmits.
 *
 * This function returns the minimum backoff exponent for random delays between retransmits.
 *
 * @param stackId           The identifier of the stack
 *
 *@return the minimum backoff exponent
 */
GP_API UInt8 gpMacDispatcher_GetMinBeRetransmit(gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function configures the maximum backoff exponent for random delays between retransmits.
 *
 * This function configures the maximum backoff exponent for random delays between retransmits due to NoAck failures.
 * See Thread v1.2.1 paragraph 3.2.7.4: Mac layer retries.
 * Not to be confused with the backoff exponent for random delays between CCA failures!
 *
 * @param maxBERetransmit   The maximum backoff exponent.
 * @param stackId           The identifier of the stack
 */
GP_API void gpMacDispatcher_SetMaxBeRetransmit(UInt8 maxBERetransmit, gpMacDispatcher_StackId_t stackId);

/** @ingroup ATTRIBUTE
 * @brief This function returns the maximum backoff exponent for random delays between retransmits.
 *
 * This function returns the maximum backoff exponent for random delays between retransmits.
 *
 * @param stackId           The identifier of the stack
 *
 * @return maximum backoff exponent
 */
GP_API UInt8 gpMacDispatcher_GetMaxBeRetransmit(gpMacDispatcher_StackId_t stackId);

/* JUMPTABLE_FLASH_FUNCTION_DEFINITIONS_END */

#ifdef GP_MACCORE_DIVERSITY_RX_WINDOWS
/** @brief Enables Duty Cycled Rx window
 *
 * Enable a duty cycled Rx window
 *
 * @param dutyCycleOnTime   The amount of time the radio will be on and in receive mode (in us)
 * @param dutyCyclePeriod   Period of the Rx Window (in us)
 * @param recurrenceAmount  Number of occurrences that needs to be scheduled
 * @param startTime         Absolute start time (in us) of the rx windows
 * @param stackId           Stack id.
 */
GP_API void gpMacDispatcher_EnableRxWindows(UInt32 dutyCycleOnTime, UInt32 dutyCyclePeriod, UInt16 recurrenceAmount,
                                            UInt32 startTime, gpMacDispatcher_StackId_t stackId);

/** @brief Disables Duty Cycled Rx Windows
 *
 * Disable a periodic Rx window
 *
 * @param stackId           The stack id for which the rx windows will be disabled.
 */
GP_API void gpMacDispatcher_DisableRxWindows(gpMacDispatcher_StackId_t stackId);

/** @brief Enables CSL IE insertion in Enhanced Acks
 *
 * Enables CSL IE insertion in Enhanced Acks
 *
 * @param dutyCyclePeriod   The period in multiples of 160us that needs to be inserted in the CSL IE.
 * @param stackId           The stackId
 */
GP_API void gpMacDispatcher_EnableCsl(UInt16 dutyCyclePeriod, gpMacDispatcher_StackId_t stackId);

/** @brief Update the CSL samples time
 *
 * Update the CSL samples time at which the next Rx window should occur. This will be used in the phase calculation for
 * the CSL IE.
 *
 * @param nextCslSampleTime     The next CSL sample time in us (the rx window start without extra margin)
 * @param stackId               The stackId
 */
GP_API void gpMacDispatcher_UpdateCslSampleTime(UInt32 nextCslSampleTime, gpMacDispatcher_StackId_t stackId);
#endif // GP_MACCORE_DIVERSITY_RX_WINDOWS

/** @brief This function returns radio sensitivity.
 *
 * This function returns the radio sensitivity in dBm.
 *
 * @return radio sensitivity in dBm.
 */
GP_API Int8 gpMacDispatcher_GetRadioSensitivity(void);

/** @brief This function returns the worst case CSL accuracy.
 *
 * Get the current estimated worst case accuracy (maximum ± deviation from the nominal frequency) of radio clock in
 * units of PPM. This is the clock used to schedule CSL operations.
 *
 * @return The current CSL rx/tx scheduling drift, in PPM.
 */
GP_API UInt8 gpMacDispatcher_GetCslAccuracy(void);

/** @brief This function returns the CSL Uncertainty of the arrival time of CSL transmissions.
 *
 * This designates the worst case constant positive or negative deviation of the actual arrival time of a transmission
 * from the transmission time calculated relative to the local radio clock independent of elapsed time. In addition to
 * uncertainty accumulated over elapsed time, the CSL channel sample ("RX window") must be extended by twice this
 * deviation such that an actual transmission is guaranteed to be detected by the local receiver in the presence of
 * random arrival time jitter.
 *
 * @return The CSL Uncertainty in units of 10 us.
 */
GP_API UInt8 gpMacDispatcher_GetCslUncertainty(void);

#ifdef __cplusplus
}
#endif

#endif // _GP_MACDISPATCHER_MULTISTACK_H_
