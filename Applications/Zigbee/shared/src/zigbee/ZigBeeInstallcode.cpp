/*
 * Copyright (c) 2017, Qorvo Inc
 *
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
 *
 */

/** @file "ZigBeeInstallcode.cpp"
 *
 * Source file for handling Zigbee install code.
*/

#define GP_COMPONENT_ID GP_COMPONENT_ID_APP

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "ZigBee.h"
#include "gpLog.h"
#include "gpSched.h"

#include "ZigBeeInstallcode.h"

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
/** @brief Marco to convert MACAddress_t into unsigned long long(ULL)
*
*   @param msb_lsb          MACAddress_t structure for MAC address
*   @return ull             unsigned long long(ULL) for MAC address
*
*/
#define MSB_LSB_TO_ULL(msb_lsb, ull)                    \
    do                                                  \
    {                                                   \
        (ull) = (msb_lsb).LSB;                          \
        (ull) += (unsigned long long)msb_lsb.MSB << 32; \
    } while(false);

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *                    External data structures
 *****************************************************************************/
extern CZigBeeApplicationSupport aps;
//defined in linker script, IAR compiler only
extern UInt32 Factory_Start;

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/
/** @brief Macro defines for Zigbee 3.0 install code address from flash */
const UInt32 INSTALL_CODE_ADDRESS = (UInt32)&Factory_Start + FACTORYBLOCK_INSTALLCODE_OFFSET;

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// Install code API
/** @brief Apply the Install code from persistent memory
*   After passing the CRC check, the perconfigured link key will be generated from install code
*   and applied into system
*/
void Application_ZigBeeApplyInstallCode(void)
{
    CStandardISO13239CRC crc;

    if(aps.m_apsme.IsStartupInProgress())
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Commissioning is in progress, unable to load install code", 0);
        return;
    }

    // The install code from flash
    unsigned char* installCode = (UInt8*)INSTALL_CODE_ADDRESS;
    // The generated link key from the install code
    unsigned char perconfiguredLinkKey[LINK_KEY_SIZE];

    // print to install code
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Load App Install Code(address: 0x%08lx)", 0, (UInt32)installCode);
    gpLog_PrintBuffer(INSTALL_CODE_SIZE, installCode);

    // Validate the App install code with CRC checking
    bool isRawValid = crc.Check(installCode, INSTALL_CODE_SIZE);

    if(isRawValid)
    {
        // Calculate the MMO Hash value of the install code
        CZigBeeSecurityMMO::CalculateHash(perconfiguredLinkKey,
                                          installCode,
                                          INSTALL_CODE_SIZE);

        // // Print the generated Link key
        // GP_LOG_SYSTEM_PRINTF("LinkKey:",0);
        // gpLog_PrintBuffer(sizeof(perconfiguredLinkKey),perconfiguredLinkKey);

        // Apply the new link key into startup attributes
        CZigBeeCommissioningAttributesEx attributes;

        attributes.Load();
        memcpy(attributes.m_abPreconfiguredLinkKey, perconfiguredLinkKey, LINK_KEY_SIZE);

        // store updated attributes
        attributes.Store();
        attributes.Commit(aps);
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF("ZIGBEE: Warning, Invalid Install Code", 0);
    }
}

/** @brief Dump the Install code from persistent memory
*/
void Application_ZigBeeDumpInstallCode(void)
{
#ifdef GP_DIVERSITY_LOG
    // The install code from flash
    unsigned char* installCode = (UInt8*)INSTALL_CODE_ADDRESS;
    // Debug print to install code
    GP_LOG_SYSTEM_PRINTF("ZIGBEE: Device Install Code:", 0);
    gpLog_PrintBuffer(INSTALL_CODE_SIZE, installCode);
#endif // GP_DIVERSITY_LOG
}
