/*
 * Copyright (c) 2017-2021, Qorvo Inc
 *
 * settings_qorvo.c
 *   This file contains the implementation of the qorvo settings api for openthread.
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

#define GP_COMPONENT_ID GP_COMPONENT_ID_QVOT

// #define GP_LOCAL_LOG
#define LOG_PREFIX "[Q] Nvm-----------: "

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/

// Openthread headers
#include "openthread/platform/settings.h"

// Qorvo Headers
#include "gpNvm.h"
#include "gpNvm_NvmProtect.h"
#include "gpAssert.h"
#include "gpLog.h"
#include "settings_qorvo.h"
#include "settings_qorvo_defs.h"

#ifdef QVOT_SECURE_STORAGE
#include "gpRandom.h"
#include "random_qorvo.h"
#include "esecure.h"
#endif // QVOT_SECURE_STORAGE

/*****************************************************************************
 *                    Macros and Types Definitions
 *****************************************************************************/

/* Overall debug flag, required to enable one of the below, more specific features
 */
// #define QVOT_DEBUG // (uncomment or set on the cmd line when building)

#ifdef QVOT_SECURE_STORAGE
// Sensitive tags as provided by openthread
#define QVOT_KEY_IS_SENSITIVE(key) BIT_TST(qorvoSensitiveTags, (key))
// Sensitive tags that we support to be sensitive
#define QVOT_KEY_CAN_BE_SENSITIVE(key) BIT_TST(qorvoSupportedSensitiveTags, (key))
// Keys that are actually encrypted
#define QVOT_KEY_CAN_BE_ENCRYPTED(key) ((QVOT_KEY_IS_SENSITIVE(key)) && (QVOT_KEY_CAN_BE_SENSITIVE(key)))
#endif // QVOT_SECURE_STORAGE

// Thread Routers MUST support 10 children, but SHOULD support 64 children
#ifndef QVOT_MAX_CHILDREN
#define QVOT_MAX_CHILDREN 0
#else
// "FTD (REEDs) have to support at least QVOT_MIN_SUPPORTED_CHILDREN children"
GP_COMPILE_TIME_VERIFY(QVOT_MAX_CHILDREN >= QVOT_MIN_SUPPORTED_CHILDREN);
// "FTD (REEDs) can support maximum QVOT_MAX_SUPPORTED_CHILDREN children"
GP_COMPILE_TIME_VERIFY(QVOT_MAX_CHILDREN <= QVOT_MAX_SUPPORTED_CHILDREN);
// Ensure that we do not exceed the available NVM tags
GP_COMPILE_TIME_VERIFY(QVOT_MAX_CHILDREN + NVM_TAG_QORVO_LAST <= 255);
#endif

#define GP_LOG_SYSTEM_PRINTF_DEBUG(...)                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
    } while(0);
#define GP_LOG_PRINTF_DEBUG(...)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
    } while(0);
#define gpLog_PrintBuffer_debug(...)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
    } while(0);

#define NVM_TAG_QVOT_HEADERSIZE (offsetof(NvmTagsBuffer, NvmData))

typedef struct {
    uint8_t dataValid;
    uint8_t dataSize; /* Note: dataSize type needs to be adapted if any tag size would be larger than 255 ! */
    union {
        uint8_t activeDataSet[NVM_TAG_QVOT_MAX_SIZEOF_ACTIVEDATASET];
        uint8_t pendingDataSet[NVM_TAG_QVOT_MAX_SIZEOF_PENDINGDATASET];
        uint8_t networkInfo[NVM_TAG_QVOT_MAX_SIZEOF_NETWORKINFO];
        uint8_t parentInfo[NVM_TAG_QVOT_MAX_SIZEOF_PARENTINFO];
        uint8_t slaacIidSecretKey[NVM_TAG_QVOT_MAX_SIZEOF_SLAACIIDSECRETKEY];
        uint8_t dadInfo[NVM_TAG_QVOT_MAX_SIZEOF_DADINFO];
        uint8_t srpKey[NVM_TAG_QVOT_MAX_SIZEOF_SRPKEY];
        uint8_t srpClientInfo[NVM_TAG_QVOT_MAX_SIZEOF_SRPCLIENTINFO];
        uint8_t srpServerInfo[NVM_TAG_QVOT_MAX_SIZEOF_SRPSERVERINFO];
        uint8_t brUlaPrefix[NVM_TAG_QVOT_MAX_SIZEOF_BRULAPREFIX];
        uint8_t childInfo[NVM_TAG_QVOT_MAX_SIZEOF_CHILDINFO];
        uint8_t brOnLinkPrefixes[NVM_TAG_QVOT_MAX_SIZEOF_BRONLINKPREFIXES];
        uint8_t borderAgentId[NVM_TAG_QVOT_MAX_SIZEOF_BORDERAGENTID];
        uint8_t keyMaterial[NVM_TAG_QVOT_MAX_SIZEOF_KEY_MATERIAL];
    } NvmData;
} NvmTagsBuffer;

typedef struct NvmTag_ {
    uint8_t otTagId;
    uint8_t nvmTagId;
    uint8_t maxTagSize;
} NvmTag_t;

/*****************************************************************************
 *                    Static Function Prototypes
 *****************************************************************************/

static bool qorvoSettings_DefaultInitializer(const ROM void* pTag, uint8_t* pBuffer);

/*****************************************************************************
 *                    Static Data Definitions
 *****************************************************************************/

#ifdef QVOT_SECURE_STORAGE
// bitmask for all sensitive tags based on OT_SETTINGS keys
uint32_t qorvoSensitiveTags = 0; // initialized at startup
static const uint32_t qorvoSupportedSensitiveTags =
    BM(NVM_TAG_QVOT_ACTIVEDATASET) | BM(NVM_TAG_QVOT_PENDINGDATASET) | BM(NVM_TAG_QVOT_SRPKEY);
typedef struct esec_skey skey_t;
static uint8_t key_buffer[NVM_TAG_QVOT_MAX_SIZEOF_KEY_MATERIAL];
#endif // QVOT_SECURE_STORAGE

static uint8_t qorvoSettings_NrOfChildrenStored;

// clang-format off
static const NvmTag_t NvmLookupTable[] =
{
    /* OpenThread key tag ID*/              /* Internal NVM tag ID*/          /* Max size for internal tag */
    {OT_SETTINGS_KEY_ACTIVE_DATASET,        NVM_TAG_QVOT_ACTIVEDATASET,       NVM_TAG_QVOT_MAX_SIZEOF_ACTIVEDATASET},
    {OT_SETTINGS_KEY_PENDING_DATASET,       NVM_TAG_QVOT_PENDINGDATASET,      NVM_TAG_QVOT_MAX_SIZEOF_PENDINGDATASET},
    {OT_SETTINGS_KEY_NETWORK_INFO,          NVM_TAG_QVOT_NETWORKINFO,         NVM_TAG_QVOT_MAX_SIZEOF_NETWORKINFO},
    {OT_SETTINGS_KEY_PARENT_INFO,           NVM_TAG_QVOT_PARENTINFO,          NVM_TAG_QVOT_MAX_SIZEOF_PARENTINFO},
    {OT_SETTINGS_KEY_SLAAC_IID_SECRET_KEY,  NVM_TAG_QVOT_SLAACIIDSECRETKEY,   NVM_TAG_QVOT_MAX_SIZEOF_SLAACIIDSECRETKEY},
    {OT_SETTINGS_KEY_DAD_INFO,              NVM_TAG_QVOT_DADINFO,             NVM_TAG_QVOT_MAX_SIZEOF_DADINFO},
    {OT_SETTINGS_KEY_SRP_ECDSA_KEY,         NVM_TAG_QVOT_SRPKEY,              NVM_TAG_QVOT_MAX_SIZEOF_SRPKEY},
    {OT_SETTINGS_KEY_SRP_CLIENT_INFO,       NVM_TAG_QVOT_SRPCLIENTINFO,       NVM_TAG_QVOT_MAX_SIZEOF_SRPCLIENTINFO},
    {OT_SETTINGS_KEY_SRP_SERVER_INFO,       NVM_TAG_QVOT_SRPSERVERINFO,       NVM_TAG_QVOT_MAX_SIZEOF_SRPSERVERINFO},
    {OT_SETTINGS_KEY_BR_ULA_PREFIX,         NVM_TAG_QVOT_BRULAPREFIX,         NVM_TAG_QVOT_MAX_SIZEOF_BRULAPREFIX},
    {OT_SETTINGS_KEY_BR_ON_LINK_PREFIXES,   NVM_TAG_QVOT_BRONLINKPREFIXES,    NVM_TAG_QVOT_MAX_SIZEOF_BRONLINKPREFIXES},
    {OT_SETTINGS_KEY_BORDER_AGENT_ID,       NVM_TAG_QVOT_BORDERAGENTID,       NVM_TAG_QVOT_MAX_SIZEOF_BORDERAGENTID},
    {OT_SETTINGS_KEY_ENCRYPTION_KEY,        NVM_TAG_QVOT_KEY_MATERIAL,        NVM_TAG_QVOT_MAX_SIZEOF_KEY_MATERIAL},
    {OT_SETTINGS_KEY_CHILD_INFO,            NVM_TAG_QVOT_CHILDINFO_BASE,      NVM_TAG_QVOT_MAX_SIZEOF_CHILDINFO},
};
#define QVOT_NVM_MAX_SUPPORTED_KEYS number_of_elements(NvmLookupTable)

#define QVOT_NVM_BASE_TAG_ID ((uint16_t)(GP_COMPONENT_ID << 8))
#define QVOT_NVM_MINIMAL_TAG_COUNT (QVOT_NVM_MAX_SUPPORTED_KEYS)
#define QVOT_NVM_MINIMAL_TAGS_DATA \
     {QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_ACTIVEDATASET,        NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_ACTIVEDATASET,      gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_PENDINGDATASET,       NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_PENDINGDATASET,     gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_NETWORKINFO,          NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_NETWORKINFO,        gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_PARENTINFO,           NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_PARENTINFO,         gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_SLAACIIDSECRETKEY,    NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_SLAACIIDSECRETKEY,  gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_DADINFO,              NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_DADINFO,            gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_SRPKEY,               NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_SRPKEY,             gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_SRPCLIENTINFO,        NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_SRPCLIENTINFO,      gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_SRPSERVERINFO,        NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_SRPSERVERINFO,      gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_BRULAPREFIX,          NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_BRULAPREFIX,        gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_BRONLINKPREFIXES,     NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_BRONLINKPREFIXES,   gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_BORDERAGENTID,        NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_BORDERAGENTID,      gpNvm_UpdateFrequencyLow, NULL, NULL} \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_KEY_MATERIAL,         NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_MAX_SIZEOF_KEY_MATERIAL,       gpNvm_UpdateFrequencyLow, NULL, NULL}
#define QVOT_NVM_CHILD_ENTRY(X) \
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_CHILDINFO_BASE + (X), NULL,  NVM_TAG_QVOT_HEADERSIZE + NVM_TAG_QVOT_SIZEOF_CHILDINFO,          gpNvm_UpdateFrequencyLow, NULL, NULL}

const gpNvm_IdentifiableTag_t ROM qorvoSettings_NvmElements[QVOT_NVM_MINIMAL_TAG_COUNT + QVOT_MAX_CHILDREN] FLASH_PROGMEM = {
/* note that the sizes in this table are 1 byte more then the actual data to be able to easily generate the "Not Found" error in the platform Api */
    QVOT_NVM_MINIMAL_TAGS_DATA
/* Child info storage */
    ,{QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_NROFCHILDRENSTORED, (uint8_t*)&(qorvoSettings_NrOfChildrenStored), NVM_TAG_QVOT_MAX_SIZEOF_NROFCHILDRENSTORED, gpNvm_UpdateFrequencyLow, (gpNvm_cbDefaultValueInitializer_t)qorvoSettings_DefaultInitializer, NULL}

#if(QVOT_MAX_CHILDREN > 0)
    QVOT_NVM_CHILD_ENTRY(0)
    QVOT_NVM_CHILD_ENTRY(1)
    QVOT_NVM_CHILD_ENTRY(2)
    QVOT_NVM_CHILD_ENTRY(3)
    QVOT_NVM_CHILD_ENTRY(4)
    QVOT_NVM_CHILD_ENTRY(5)
    QVOT_NVM_CHILD_ENTRY(6)
    QVOT_NVM_CHILD_ENTRY(7)
    QVOT_NVM_CHILD_ENTRY(8)
    QVOT_NVM_CHILD_ENTRY(9)
#endif

#if(QVOT_MAX_CHILDREN > 10)
    QVOT_NVM_CHILD_ENTRY(10)
#endif
#if(QVOT_MAX_CHILDREN > 11)
    QVOT_NVM_CHILD_ENTRY(11)
#endif
#if(QVOT_MAX_CHILDREN > 12)
    QVOT_NVM_CHILD_ENTRY(12)
#endif
#if(QVOT_MAX_CHILDREN > 13)
    QVOT_NVM_CHILD_ENTRY(13)
#endif
#if(QVOT_MAX_CHILDREN > 14)
    QVOT_NVM_CHILD_ENTRY(14)
#endif
#if(QVOT_MAX_CHILDREN > 15)
    QVOT_NVM_CHILD_ENTRY(15)
#endif
#if(QVOT_MAX_CHILDREN > 16)
    QVOT_NVM_CHILD_ENTRY(16)
#endif
#if(QVOT_MAX_CHILDREN > 17)
    QVOT_NVM_CHILD_ENTRY(17)
#endif
#if(QVOT_MAX_CHILDREN > 18)
    QVOT_NVM_CHILD_ENTRY(18)
#endif
#if(QVOT_MAX_CHILDREN > 19)
    QVOT_NVM_CHILD_ENTRY(19)
#endif


#if(QVOT_MAX_CHILDREN > 20)
    QVOT_NVM_CHILD_ENTRY(20)
#endif
#if(QVOT_MAX_CHILDREN > 21)
    QVOT_NVM_CHILD_ENTRY(21)
#endif
#if(QVOT_MAX_CHILDREN > 22)
    QVOT_NVM_CHILD_ENTRY(22)
#endif
#if(QVOT_MAX_CHILDREN > 23)
    QVOT_NVM_CHILD_ENTRY(23)
#endif
#if(QVOT_MAX_CHILDREN > 24)
    QVOT_NVM_CHILD_ENTRY(24)
#endif
#if(QVOT_MAX_CHILDREN > 25)
    QVOT_NVM_CHILD_ENTRY(25)
#endif
#if(QVOT_MAX_CHILDREN > 26)
    QVOT_NVM_CHILD_ENTRY(26)
#endif
#if(QVOT_MAX_CHILDREN > 27)
    QVOT_NVM_CHILD_ENTRY(27)
#endif
#if(QVOT_MAX_CHILDREN > 28)
    QVOT_NVM_CHILD_ENTRY(28)
#endif
#if(QVOT_MAX_CHILDREN > 29)
    QVOT_NVM_CHILD_ENTRY(29)
#endif


#if(QVOT_MAX_CHILDREN > 30)
    QVOT_NVM_CHILD_ENTRY(30)
#endif
#if(QVOT_MAX_CHILDREN > 31)
    QVOT_NVM_CHILD_ENTRY(31)
#endif
#if(QVOT_MAX_CHILDREN > 32)
    QVOT_NVM_CHILD_ENTRY(32)
#endif
#if(QVOT_MAX_CHILDREN > 33)
    QVOT_NVM_CHILD_ENTRY(33)
#endif
#if(QVOT_MAX_CHILDREN > 34)
    QVOT_NVM_CHILD_ENTRY(34)
#endif
#if(QVOT_MAX_CHILDREN > 35)
    QVOT_NVM_CHILD_ENTRY(35)
#endif
#if(QVOT_MAX_CHILDREN > 36)
    QVOT_NVM_CHILD_ENTRY(36)
#endif
#if(QVOT_MAX_CHILDREN > 37)
    QVOT_NVM_CHILD_ENTRY(37)
#endif
#if(QVOT_MAX_CHILDREN > 38)
    QVOT_NVM_CHILD_ENTRY(38)
#endif
#if(QVOT_MAX_CHILDREN > 39)
    QVOT_NVM_CHILD_ENTRY(39)
#endif


#if(QVOT_MAX_CHILDREN > 40)
    QVOT_NVM_CHILD_ENTRY(40)
#endif
#if(QVOT_MAX_CHILDREN > 41)
    QVOT_NVM_CHILD_ENTRY(41)
#endif
#if(QVOT_MAX_CHILDREN > 42)
    QVOT_NVM_CHILD_ENTRY(42)
#endif
#if(QVOT_MAX_CHILDREN > 43)
    QVOT_NVM_CHILD_ENTRY(43)
#endif
#if(QVOT_MAX_CHILDREN > 44)
    QVOT_NVM_CHILD_ENTRY(44)
#endif
#if(QVOT_MAX_CHILDREN > 45)
    QVOT_NVM_CHILD_ENTRY(45)
#endif
#if(QVOT_MAX_CHILDREN > 46)
    QVOT_NVM_CHILD_ENTRY(46)
#endif
#if(QVOT_MAX_CHILDREN > 47)
    QVOT_NVM_CHILD_ENTRY(47)
#endif
#if(QVOT_MAX_CHILDREN > 48)
    QVOT_NVM_CHILD_ENTRY(48)
#endif
#if(QVOT_MAX_CHILDREN > 49)
    QVOT_NVM_CHILD_ENTRY(49)
#endif


#if(QVOT_MAX_CHILDREN > 50)
    QVOT_NVM_CHILD_ENTRY(50)
#endif
#if(QVOT_MAX_CHILDREN > 51)
    QVOT_NVM_CHILD_ENTRY(51)
#endif
#if(QVOT_MAX_CHILDREN > 52)
    QVOT_NVM_CHILD_ENTRY(52)
#endif
#if(QVOT_MAX_CHILDREN > 53)
    QVOT_NVM_CHILD_ENTRY(53)
#endif
#if(QVOT_MAX_CHILDREN > 54)
    QVOT_NVM_CHILD_ENTRY(54)
#endif
#if(QVOT_MAX_CHILDREN > 55)
    QVOT_NVM_CHILD_ENTRY(55)
#endif
#if(QVOT_MAX_CHILDREN > 56)
    QVOT_NVM_CHILD_ENTRY(56)
#endif
#if(QVOT_MAX_CHILDREN > 57)
    QVOT_NVM_CHILD_ENTRY(57)
#endif
#if(QVOT_MAX_CHILDREN > 58)
    QVOT_NVM_CHILD_ENTRY(58)
#endif
#if(QVOT_MAX_CHILDREN > 59)
    QVOT_NVM_CHILD_ENTRY(59)
#endif


#if(QVOT_MAX_CHILDREN > 60)
    QVOT_NVM_CHILD_ENTRY(60)
#endif
#if(QVOT_MAX_CHILDREN > 61)
    QVOT_NVM_CHILD_ENTRY(61)
#endif
#if(QVOT_MAX_CHILDREN > 62)
    QVOT_NVM_CHILD_ENTRY(62)
#endif
#if(QVOT_MAX_CHILDREN > 63)
    QVOT_NVM_CHILD_ENTRY(63)
#endif
#if(QVOT_MAX_CHILDREN > 64)
    QVOT_NVM_CHILD_ENTRY(64)
#endif
#if(QVOT_MAX_CHILDREN > 65)
    QVOT_NVM_CHILD_ENTRY(65)
#endif
#if(QVOT_MAX_CHILDREN > 66)
    QVOT_NVM_CHILD_ENTRY(66)
#endif
#if(QVOT_MAX_CHILDREN > 67)
    QVOT_NVM_CHILD_ENTRY(67)
#endif
#if(QVOT_MAX_CHILDREN > 68)
    QVOT_NVM_CHILD_ENTRY(68)
#endif
#if(QVOT_MAX_CHILDREN > 69)
    QVOT_NVM_CHILD_ENTRY(69)
#endif

#if(QVOT_MAX_CHILDREN > 70)
    QVOT_NVM_CHILD_ENTRY(70)
#endif
#if(QVOT_MAX_CHILDREN > 71)
    QVOT_NVM_CHILD_ENTRY(71)
#endif
#if(QVOT_MAX_CHILDREN > 72)
    QVOT_NVM_CHILD_ENTRY(72)
#endif
#if(QVOT_MAX_CHILDREN > 73)
    QVOT_NVM_CHILD_ENTRY(73)
#endif
#if(QVOT_MAX_CHILDREN > 74)
    QVOT_NVM_CHILD_ENTRY(74)
#endif
#if(QVOT_MAX_CHILDREN > 75)
    QVOT_NVM_CHILD_ENTRY(75)
#endif
#if(QVOT_MAX_CHILDREN > 76)
    QVOT_NVM_CHILD_ENTRY(76)
#endif
#if(QVOT_MAX_CHILDREN > 77)
    QVOT_NVM_CHILD_ENTRY(77)
#endif
#if(QVOT_MAX_CHILDREN > 78)
    QVOT_NVM_CHILD_ENTRY(78)
#endif
#if(QVOT_MAX_CHILDREN > 79)
    QVOT_NVM_CHILD_ENTRY(79)
#endif

#if(QVOT_MAX_CHILDREN > 80)
    QVOT_NVM_CHILD_ENTRY(80)
#endif
#if(QVOT_MAX_CHILDREN > 81)
    QVOT_NVM_CHILD_ENTRY(81)
#endif
#if(QVOT_MAX_CHILDREN > 82)
    QVOT_NVM_CHILD_ENTRY(82)
#endif
#if(QVOT_MAX_CHILDREN > 83)
    QVOT_NVM_CHILD_ENTRY(83)
#endif
#if(QVOT_MAX_CHILDREN > 84)
    QVOT_NVM_CHILD_ENTRY(84)
#endif
#if(QVOT_MAX_CHILDREN > 85)
    QVOT_NVM_CHILD_ENTRY(85)
#endif
#if(QVOT_MAX_CHILDREN > 86)
    QVOT_NVM_CHILD_ENTRY(86)
#endif
#if(QVOT_MAX_CHILDREN > 87)
    QVOT_NVM_CHILD_ENTRY(87)
#endif
#if(QVOT_MAX_CHILDREN > 88)
    QVOT_NVM_CHILD_ENTRY(88)
#endif
#if(QVOT_MAX_CHILDREN > 89)
    QVOT_NVM_CHILD_ENTRY(89)
#endif

#if(QVOT_MAX_CHILDREN > 90)
    QVOT_NVM_CHILD_ENTRY(90)
#endif
#if(QVOT_MAX_CHILDREN > 91)
    QVOT_NVM_CHILD_ENTRY(91)
#endif
#if(QVOT_MAX_CHILDREN > 92)
    QVOT_NVM_CHILD_ENTRY(92)
#endif
#if(QVOT_MAX_CHILDREN > 93)
    QVOT_NVM_CHILD_ENTRY(93)
#endif
#if(QVOT_MAX_CHILDREN > 94)
    QVOT_NVM_CHILD_ENTRY(94)
#endif
#if(QVOT_MAX_CHILDREN > 95)
    QVOT_NVM_CHILD_ENTRY(95)
#endif
#if(QVOT_MAX_CHILDREN > 96)
    QVOT_NVM_CHILD_ENTRY(96)
#endif
#if(QVOT_MAX_CHILDREN > 97)
    QVOT_NVM_CHILD_ENTRY(97)
#endif
#if(QVOT_MAX_CHILDREN > 98)
    QVOT_NVM_CHILD_ENTRY(98)
#endif
#if(QVOT_MAX_CHILDREN > 99)
    QVOT_NVM_CHILD_ENTRY(99)
#endif

#if(QVOT_MAX_CHILDREN > 100)
    QVOT_NVM_CHILD_ENTRY(100)
#endif
#if(QVOT_MAX_CHILDREN > 101)
    QVOT_NVM_CHILD_ENTRY(101)
#endif
#if(QVOT_MAX_CHILDREN > 102)
    QVOT_NVM_CHILD_ENTRY(102)
#endif
#if(QVOT_MAX_CHILDREN > 103)
    QVOT_NVM_CHILD_ENTRY(103)
#endif
#if(QVOT_MAX_CHILDREN > 104)
    QVOT_NVM_CHILD_ENTRY(104)
#endif
#if(QVOT_MAX_CHILDREN > 105)
    QVOT_NVM_CHILD_ENTRY(105)
#endif
#if(QVOT_MAX_CHILDREN > 106)
    QVOT_NVM_CHILD_ENTRY(106)
#endif
#if(QVOT_MAX_CHILDREN > 107)
    QVOT_NVM_CHILD_ENTRY(107)
#endif
#if(QVOT_MAX_CHILDREN > 108)
    QVOT_NVM_CHILD_ENTRY(108)
#endif
#if(QVOT_MAX_CHILDREN > 109)
    QVOT_NVM_CHILD_ENTRY(109)
#endif

#if(QVOT_MAX_CHILDREN > 110)
    QVOT_NVM_CHILD_ENTRY(110)
#endif
#if(QVOT_MAX_CHILDREN > 111)
    QVOT_NVM_CHILD_ENTRY(111)
#endif
#if(QVOT_MAX_CHILDREN > 112)
    QVOT_NVM_CHILD_ENTRY(112)
#endif
#if(QVOT_MAX_CHILDREN > 113)
    QVOT_NVM_CHILD_ENTRY(113)
#endif
#if(QVOT_MAX_CHILDREN > 114)
    QVOT_NVM_CHILD_ENTRY(114)
#endif
#if(QVOT_MAX_CHILDREN > 115)
    QVOT_NVM_CHILD_ENTRY(115)
#endif
#if(QVOT_MAX_CHILDREN > 116)
    QVOT_NVM_CHILD_ENTRY(116)
#endif
#if(QVOT_MAX_CHILDREN > 117)
    QVOT_NVM_CHILD_ENTRY(117)
#endif
#if(QVOT_MAX_CHILDREN > 118)
    QVOT_NVM_CHILD_ENTRY(118)
#endif
#if(QVOT_MAX_CHILDREN > 119)
    QVOT_NVM_CHILD_ENTRY(119)
#endif

#if(QVOT_MAX_CHILDREN > 120)
    QVOT_NVM_CHILD_ENTRY(120)
#endif
#if(QVOT_MAX_CHILDREN > 121)
    QVOT_NVM_CHILD_ENTRY(121)
#endif
#if(QVOT_MAX_CHILDREN > 122)
    QVOT_NVM_CHILD_ENTRY(122)
#endif
#if(QVOT_MAX_CHILDREN > 123)
    QVOT_NVM_CHILD_ENTRY(123)
#endif
#if(QVOT_MAX_CHILDREN > 124)
    QVOT_NVM_CHILD_ENTRY(124)
#endif
#if(QVOT_MAX_CHILDREN > 125)
    QVOT_NVM_CHILD_ENTRY(125)
#endif
#if(QVOT_MAX_CHILDREN > 126)
    QVOT_NVM_CHILD_ENTRY(126)
#endif
#if(QVOT_MAX_CHILDREN > 127)
    QVOT_NVM_CHILD_ENTRY(127)
#endif
#if(QVOT_MAX_CHILDREN > 128)
    QVOT_NVM_CHILD_ENTRY(128)
#endif
#if(QVOT_MAX_CHILDREN > 129)
    QVOT_NVM_CHILD_ENTRY(129)
#endif
};
// clang-format on

/*****************************************************************************
 *                    Static Function Definitions
 *****************************************************************************/
static NvmTag_t* qorvoSettings_GetTagStructByKey(uint16_t aKey)
{
    uint8_t i;

    for(i = 0; i < QVOT_NVM_MAX_SUPPORTED_KEYS; i++)
    {
        if(NvmLookupTable[i].otTagId == aKey)
        {
            return (NvmTag_t*)(&NvmLookupTable[i]);
        }
    }

    return NULL;
}

#ifdef QVOT_SECURE_STORAGE
static NvmTag_t* qorvoSettings_GetKeyStruct(void)
{
    uint8_t i;

    for(i = 0; i < QVOT_NVM_MAX_SUPPORTED_KEYS; i++)
    {
        if(NvmLookupTable[i].nvmTagId == NVM_TAG_QVOT_KEY_MATERIAL)
        {
            return (NvmTag_t*)(&NvmLookupTable[i]);
        }
    }

    return NULL;
}
#endif // QVOT_SECURE_STORAGE

/* @brief Get the max length of a Tag
 *
 * @param pTag: Pointer to the Tag structure
 * @return: Max length of the Tag, depending on it being sensitive
 */
uint8_t qorvoSettings_GetTagLength(NvmTag_t* pTag)
{
#ifdef QVOT_SECURE_STORAGE
    uint8_t tag_length = pTag->maxTagSize;
    // If the key is sized to be sensitive, but sensitivity is not enabled for it
    if(QVOT_KEY_CAN_BE_SENSITIVE(pTag->nvmTagId) && !QVOT_KEY_IS_SENSITIVE(pTag->nvmTagId))
    {
        tag_length -= QVOT_CRYPTO_META_DATA_SIZE;
    }
    return tag_length;
#else
    return pTag->maxTagSize;
#endif // QVOT_SECURE_STORAGE
}

static bool qorvoSettings_DefaultInitializer(const ROM void* pTag, uint8_t* pBuffer)
{
    gpNvm_IdentifiableTag_t tag;
    MEMCPY_P((uint8_t*)&tag, pTag, sizeof(gpNvm_IdentifiableTag_t));
    if(NULL == pBuffer)
    {
        pBuffer = tag.pRamLocation;
        if(NULL == pBuffer)
        {
            return false;
        }
    }

    if(QVOT_NVM_BASE_TAG_ID + NVM_TAG_QVOT_NROFCHILDRENSTORED == tag.uniqueTagId)
    {
        MEMSET(pBuffer, 0x00, sizeof(uint8_t));
    }
    else
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "CRIT: Did not find tag id 0x%4x", 0, tag.uniqueTagId);
        GP_ASSERT_DEV_INT(false);
        return false; // Signal NVM init failure
    }

    return true;
}

/* Special functionality to delete a ChildInfo tag
 * In case a single entry is deleted from the childInfo list, we shift all the subsequent childInfo entries
 * This has the advantage that the ChildInfo entries are always in a list without gaps, making the code much easier
 * the disadvantage is that we need to perform multiple NVM accesses to shift all entries, but we assume this
 * scenario does not occur often
 * We also assume that the OpenThread code does not remember the indexes in the NVM
 * and that if it iterates of the ChildInfo entries, it will only delete one entry during a full loop over all entries
 */
static otError qorvoSettings_DeleteChild(int childOffset)
{
    GP_LOG_PRINTF(LOG_PREFIX "delete child %i/%u", 0, childOffset, qorvoSettings_NrOfChildrenStored);

    if(childOffset == -1)
    {
        // Delete all childs
        for(uint8_t i = 0; i < qorvoSettings_NrOfChildrenStored; i++)
        {
            gpNvm_ClearProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_CHILDINFO_BASE + i);
        }
        qorvoSettings_NrOfChildrenStored = 0;
    }
    else if((childOffset < 0) || (childOffset >= qorvoSettings_NrOfChildrenStored))
    {
        // Out of bounds of stored entries
        GP_LOG_PRINTF(LOG_PREFIX "WARN: Child not found", 0);
        return OT_ERROR_NOT_FOUND;
    }
    else
    {
        NvmTagsBuffer buffer;

        // shift all entries after the current one one index up.
        // Last entry 'frees up'

        for(uint8_t i = childOffset; i < (qorvoSettings_NrOfChildrenStored - 1); i++)
        {
            GP_LOG_PRINTF(LOG_PREFIX "shifting tag:%u >> %u", 0, i + 1, i);
            gpNvm_RestoreProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_CHILDINFO_BASE + i + 1, (uint8_t*)&buffer);
            gpNvm_BackupProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_CHILDINFO_BASE + i, (uint8_t*)&buffer);
        }

        // Remove the last entry
        gpNvm_ClearProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_CHILDINFO_BASE + (qorvoSettings_NrOfChildrenStored - 1));

        qorvoSettings_NrOfChildrenStored--;
    }

    // Update number of stored in NVM
    gpNvm_BackupProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_NROFCHILDRENSTORED,
                          (uint8_t*)(&qorvoSettings_NrOfChildrenStored));

    return OT_ERROR_NONE;
}

#ifdef QVOT_SECURE_STORAGE
static void print_key(skey_t* key)
{
    GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "DEBUG: Encryption key size: %u:", 0, key->keyspec);
    GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "DEBUG: * Encryption key:", 0);
    gpLog_PrintBuffer_debug(QVOT_SKEY_SIZE, (uint8_t*)key->storage.material);
    GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "DEBUG: * Encryption key protection:", 0);
    gpLog_PrintBuffer_debug(ESEC_STOR_PROT_SIZE, (uint8_t*)&key->storage.material[QVOT_SKEY_SIZE]);
    GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "DEBUG: * Encryption key authentication:", 0);
    gpLog_PrintBuffer_debug(ESEC_STOR_AUTH_SIZE, (uint8_t*)key->storage.auth);
}
#endif // QVOT_SECURE_STORAGE

#ifdef QVOT_SECURE_STORAGE
static otError qorvoGetKey(skey_t* skey, bool do_extract_key)
{
    NvmTagsBuffer buffer;
    NvmTag_t* pKeyTag;

    if(skey == NULL && do_extract_key)
    {
        return OT_ERROR_NOT_FOUND;
    }

    pKeyTag = qorvoSettings_GetKeyStruct();
    if(pKeyTag == NULL)
    {
        return OT_ERROR_NOT_FOUND;
    }
    gpNvm_RestoreProtected(GP_COMPONENT_ID, pKeyTag->nvmTagId, (uint8_t*)(&buffer));
    if(buffer.dataValid != 1)
    {
        GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] nvm tag for encryption key is invalid", 0);
        return OT_ERROR_NOT_FOUND;
    }
    if(!do_extract_key)
    {
        // We are only interested if the key storage is valid, no need to actually extract the key
        GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] valid encryption key present", 0);
        return OT_ERROR_NONE;
    }
    MEMCPY(key_buffer, &buffer.NvmData, buffer.dataSize);
    skey->storage.material = key_buffer;
    skey->storage.auth = &key_buffer[QVOT_SKEY_SIZE + ESEC_STOR_PROT_SIZE];
    skey->keyspec = key_buffer[buffer.dataSize - 1];
    GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] encryption key found: len = %u", 0, skey->keyspec);

    return OT_ERROR_NONE;
}
#endif // QVOT_SECURE_STORAGE

#ifdef QVOT_SECURE_STORAGE
static otError qorvoAddKey(void)
{
    NvmTagsBuffer buffer;
    NvmTag_t* pKeyTag;
    skey_t skey = {}; // empty skey_t struct, will be filled out by esec_import_skey
    uint32_t result;

    if(OT_ERROR_NONE == qorvoGetKey(NULL, false))
    {
        // This shouldn't happen because we only call this function when there is no key
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "WARN: Encryption key already present", 0);
        return OT_ERROR_NONE; // Key already present
    }

    pKeyTag = qorvoSettings_GetKeyStruct();
    if(pKeyTag == NULL)
    {
        return OT_ERROR_NOT_FOUND;
    }

    MEMSET(&buffer, 0x00, sizeof(NvmTagsBuffer));
    uint8_t* key_storage = (uint8_t*)&buffer.NvmData;

    // Random nonce
    uint8_t auth[ESEC_STOR_AUTH_SIZE] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xED, 0xBE, 0xEF};
    qorvoRandomGet(auth, ESEC_STOR_AUTH_SIZE);

    result = esec_create_skey(QVOT_SKEY_SIZE, (struct esec_key_storage)ESEC_STORAGE_WRAPPED(key_storage, auth),
                              ESEC_KEY_EXPORTABLE, &skey);

    if(result != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "WARN: Encryption key creation failed: %lu", 0, result);
        return OT_ERROR_SECURITY;
    }
    buffer.dataValid = 1;
    buffer.dataSize = QVOT_SKEY_SIZE + ESEC_STOR_PROT_SIZE;
    uint8_t* auth_dst = (uint8_t*)(&key_storage[buffer.dataSize]);
    MEMCPY(auth_dst, auth, ESEC_STOR_AUTH_SIZE);
    buffer.dataSize += ESEC_STOR_AUTH_SIZE;
    key_storage[buffer.dataSize] = QVOT_SKEY_SIZE;
    buffer.dataSize++;
    gpNvm_BackupProtected(GP_COMPONENT_ID, pKeyTag->nvmTagId, (uint8_t*)(&buffer));

    print_key(&skey);
    MEMSET(auth, 0x00, ESEC_STOR_AUTH_SIZE); // clearing the memory
    GP_LOG_PRINTF(LOG_PREFIX "Encryption key created", 0);
    return OT_ERROR_NONE;
}
#endif // QVOT_SECURE_STORAGE

static otError qorvoDeleteKey(void)
{
#ifdef QVOT_SECURE_STORAGE
    NvmTag_t* pKeyTag = qorvoSettings_GetKeyStruct();
    if(pKeyTag != NULL)
    {
        gpNvm_ClearProtected(GP_COMPONENT_ID, pKeyTag->nvmTagId);
    }
    MEMSET(key_buffer, 0x00, sizeof(key_buffer)); // clearing the memory
    GP_LOG_PRINTF(LOG_PREFIX "Encryption key deleted", 0);
#endif // QVOT_SECURE_STORAGE
    return OT_ERROR_NONE;
}

#ifdef QVOT_SECURE_STORAGE
static otError qorvoEncrypt(uint8_t* cipher, const uint8_t* plain, uint8_t length)
{
    uint8_t buffer[256];
    skey_t skey = {};
    uint32_t status;
    otError err;

    if((err = qorvoGetKey(&skey, true)) != OT_ERROR_NONE)
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "WARN: No encryption key present, cannot encrypt: %d", 0, err);
        return OT_ERROR_NOT_FOUND;
    }

    uint8_t* iv = (uint8_t*)&buffer[length];
    qorvoRandomGet(iv, QVOT_IV_SIZE);
    uint8_t* tag = (uint8_t*)&buffer[length + QVOT_IV_SIZE];
    status = esec_aes_gcm_encrypt(skey, plain, iv, buffer, length, NULL, 0, tag, QVOT_TAG_SIZE);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "ERROR: Encryption failed: %lu", 0, status);
        return OT_ERROR_SECURITY;
    }

    GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "[Debg] Encrypting %u bytes, successful", 0, length);
    MEMCPY(cipher, buffer, length + QVOT_CRYPTO_META_DATA_SIZE);
    return OT_ERROR_NONE;
}
#endif // QVOT_SECURE_STORAGE

#ifdef QVOT_SECURE_STORAGE
static otError qorvoDecrypt(uint8_t* plain, const uint8_t* cipher, uint8_t length)
{
    uint8_t buffer[256];
    skey_t skey = {};
    uint32_t status;

    if(qorvoGetKey(&skey, true) != OT_ERROR_NONE)
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "WARN: No encryption key present, cannot decrypt", 0);
        return OT_ERROR_NOT_FOUND; // No key present, cannot decrypt
    }

    uint8_t* iv = (uint8_t*)&cipher[length];
    uint8_t* tag = (uint8_t*)&cipher[length + QVOT_IV_SIZE];
    status = esec_aes_gcm_decrypt(skey, cipher, iv, buffer, length, NULL, 0, tag, QVOT_TAG_SIZE);
    if(status != ESEC_OKAY)
    {
        GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "ERROR: Decryption failed: %lu", 0, status);
        return OT_ERROR_SECURITY;
    }
    GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] decryption successful", 0);

    MEMCPY(plain, buffer, length);
    return OT_ERROR_NONE;
}
#endif // QVOT_SECURE_STORAGE

#ifdef QVOT_SECURE_STORAGE
static bool qorvoIsEncryptionEnabled(void)
{
    return (qorvoGetKey(NULL, false) == OT_ERROR_NONE);
}
#endif // QVOT_SECURE_STORAGE

/*****************************************************************************
 *                    Public Function Definitions
 *****************************************************************************/
void qorvoSettingsInit()
{
    // Register the NVM storage
    gpNvm_RegisterElementsProtected(qorvoSettings_NvmElements, number_of_elements(qorvoSettings_NvmElements));

    gpNvm_RestoreProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_NROFCHILDRENSTORED, NULL);
    /* safety mechanism to ensure number of children is always initialized */
    if(qorvoSettings_NrOfChildrenStored > QVOT_MAX_CHILDREN)
    {
        qorvoSettings_NrOfChildrenStored = 0;
        gpNvm_BackupProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_NROFCHILDRENSTORED,
                              (uint8_t*)(&qorvoSettings_NrOfChildrenStored));
    }
}

void qorvoSettingsSetSensitiveKeys(uint8_t aLength, const uint16_t* aKeys)
{
#ifdef QVOT_SECURE_STORAGE
    // We currently assume this function will always be called and we don't need to store
    // `qorvoSensitiveTags` in NVM as well
    otError status = OT_ERROR_NONE;
    NvmTag_t* pKeyTag;

    if(aLength == 0 || aKeys == NULL)
    {
        // This happens when the settings are wiped and or no keys are marked as sensitive
        return;
    }

    GP_LOG_PRINTF(LOG_PREFIX "Set %u sensitive keys", 0, aLength);
    qorvoSensitiveTags = 0;

    bool encryptionEnabled = qorvoIsEncryptionEnabled();
    if(!encryptionEnabled)
    {
        GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] encryption not enabled, creating encryption key", 0);
        if((status = qorvoAddKey()) != OT_ERROR_NONE) // Try to create a key
        {
            GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "CRIT: Encryption key creation failed: %u", 0, status);
            qorvoDeleteKey(); // Ensure no key is present
            return;
        }
        skey_t tmpKey = {};
        if((status = qorvoGetKey(&tmpKey, true)) != OT_ERROR_NONE)
        {
            GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "CRIT: Encryption key retrieval failed: %u", 0, status);
            qorvoDeleteKey(); // Ensure no key is present
            return;
        }
        else
        {
            GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] Security Key creation verified", 0);
            print_key(&tmpKey);
        }
    }

    for(uint8_t i = 0; i < aLength; i++)
    {
        GP_LOG_PRINTF_DEBUG(LOG_PREFIX "[Debg] Processing key %u", 0, aKeys[i]);
        pKeyTag = qorvoSettings_GetTagStructByKey(aKeys[i]);
        if(pKeyTag == NULL)
        {
            continue; // If we get an unknown key we ignore it
        }
        if(!QVOT_KEY_CAN_BE_SENSITIVE(pKeyTag->nvmTagId))
        {
            GP_LOG_PRINTF(LOG_PREFIX "WARN: Key %u cannot be stored as sensitive, ignoring", 0, aKeys[i]);
            continue; // Only allow the key if we support it to be sensitive
        }
        if(!encryptionEnabled)
        {
            GP_LOG_PRINTF(LOG_PREFIX "INFO: Key %u will be stored as sensitive from now on", 0, aKeys[i]);
            // by delaying the setting of the bit in qorvoSensitiveTags until after the get, we trick the system
            // into believing the key is not encrypted (which it isn't yet) and thus we can read it properly.
            uint8_t buffer[256];
            uint16_t length = 0;
            status = qorvoSettingsGet(aKeys[i], 0, (uint8_t*)buffer, (uint16_t*)&length);
            BIT_SET(qorvoSensitiveTags, pKeyTag->nvmTagId);
            if(status != OT_ERROR_NONE)
            {
                // If no data found, or data is not valid, we ignore the data and assume we can convert
                GP_LOG_PRINTF(LOG_PREFIX "INFO: Key %u not found: err:%u, continuing", 0, aKeys[i], status);
                continue;
            }
            if((status = qorvoSettingsAdd(aKeys[i], true, (uint8_t*)buffer, length)) != OT_ERROR_NONE)
            {
                // There was valid data, but we could not encrypt it, so we clear the bit again
                GP_LOG_SYSTEM_PRINTF(LOG_PREFIX "WARN: Key %u conversion to sensitive failed: %u", 0, aKeys[i], status);
                BIT_CLR(qorvoSensitiveTags, pKeyTag->nvmTagId);
                continue;
            }
            GP_LOG_PRINTF(LOG_PREFIX "INFO: Key %u is now stored encrypted", 0, aKeys[i]);
        }
        else
        {
            BIT_SET(qorvoSensitiveTags, pKeyTag->nvmTagId);
        }
    }
#else
    NOT_USED(aLength);
    NOT_USED(aKeys);
#endif // QVOT_SECURE_STORAGE
}

otError qorvoSettingsGet(uint16_t aKey, int aChildIndex, uint8_t* aValue, uint16_t* aValueLength)
{
    NvmTagsBuffer buffer;
    NvmTag_t* pKeyTag;
    uint8_t tagId;

    if((aValue == NULL) || (aValueLength == NULL))
    {
        return OT_ERROR_NOT_FOUND; // OT_ERROR_INVALID_ARGS is not a valid return code
    }

    pKeyTag = qorvoSettings_GetTagStructByKey(aKey);
    if(pKeyTag == NULL)
    {
        return OT_ERROR_NOT_FOUND;
    }
    tagId = pKeyTag->nvmTagId;

    if(aKey == OT_SETTINGS_KEY_CHILD_INFO)
    {
        if((aChildIndex < 0) || (aChildIndex >= qorvoSettings_NrOfChildrenStored))
        {
            return OT_ERROR_NOT_FOUND;
        }
        tagId += aChildIndex;
    }

    gpNvm_RestoreProtected(GP_COMPONENT_ID, tagId, (uint8_t*)(&buffer));

    if(aKey == OT_SETTINGS_KEY_CHILD_INFO)
    {
        GP_LOG_PRINTF(LOG_PREFIX "get child:%d ind:%d tag:%d valid=%d", 0, aKey, aChildIndex, tagId, buffer.dataValid);
    }
    else
    {
        GP_LOG_PRINTF(LOG_PREFIX "get key:%d tag:%d valid=%d, sensitive=%d", 0, aKey, tagId, buffer.dataValid,
                      QVOT_KEY_IS_SENSITIVE(tagId));
    }

    if(buffer.dataValid == 1) // 0xFF will be set after NVM clearing
    {
        uint8_t maxTagSize = qorvoSettings_GetTagLength(pKeyTag);
        // Should never be stored with a higher length
        if(buffer.dataSize > maxTagSize)
        {
            return OT_ERROR_NOT_FOUND; // OT_ERROR_INVALID_ARGS is not a valid return code
        }

#ifdef QVOT_SECURE_STORAGE
        if(QVOT_KEY_IS_SENSITIVE(tagId))
        {
            *aValueLength = (uint16_t)(buffer.dataSize - QVOT_CRYPTO_META_DATA_SIZE);
            if(OT_ERROR_NONE != qorvoDecrypt(aValue, (uint8_t*)&buffer.NvmData, *aValueLength))
            {
                GP_LOG_SYSTEM_PRINTF_DEBUG(LOG_PREFIX "[Debg] decryption failed", 0);
                return OT_ERROR_NOT_FOUND;
            }
        }
        else
#endif // QVOT_SECURE_STORAGE
        {
            *aValueLength = buffer.dataSize;
            MEMCPY((void*)aValue, (void*)&buffer.NvmData, *aValueLength);
        }
#ifdef GP_LOCAL_LOG
        gpLog_PrintBuffer(*aValueLength, aValue);
#endif // GP_LOCAL_LOG

        return OT_ERROR_NONE;
    }

    return OT_ERROR_NOT_FOUND;
}

otError qorvoSettingsAdd(uint16_t aKey, bool isFlatTag, const uint8_t* aValue, uint16_t aValueLength)
{
    NvmTagsBuffer buffer;
    NvmTag_t* pKeyTag;
    uint8_t tagId;
    uint8_t maxTagSize;

    pKeyTag = qorvoSettings_GetTagStructByKey(aKey);
    if(pKeyTag == NULL)
    {
        return OT_ERROR_NO_BUFS; // OT_ERROR_NOT_FOUND is not a valid return code
    }

    tagId = pKeyTag->nvmTagId;
    maxTagSize = qorvoSettings_GetTagLength(pKeyTag);

    // for ChildInfo entries, the nvm entries are added on top of the existing entries
    // for all other entries, the nvm entries are overwriting the existing entry
    if(((aKey == OT_SETTINGS_KEY_CHILD_INFO) && (isFlatTag == true)) ||
       ((aKey != OT_SETTINGS_KEY_CHILD_INFO) && (isFlatTag == false)) || (aValue == NULL) ||
       (aValueLength > maxTagSize))
    {
        return OT_ERROR_NO_BUFS; // OT_ERROR_INVALID_ARGS is not a valid return code
    }

    // if not a MTD and a child is stored: check that we have enough space to store more data
    if((aKey == OT_SETTINGS_KEY_CHILD_INFO) && (qorvoSettings_NrOfChildrenStored >= QVOT_MAX_CHILDREN))
    {
        return OT_ERROR_NO_BUFS;
    }

    if(isFlatTag == false) // this means aKey must be OT_SETTINGS_KEY_CHILD_INFO - address the end of the children list
    {
        tagId += qorvoSettings_NrOfChildrenStored;
    }

    // Fill buffer
    MEMSET(&buffer, 0x00, sizeof(NvmTagsBuffer));
    buffer.dataSize = (uint8_t)(aValueLength & 0xFF);
#ifdef QVOT_SECURE_STORAGE
    if(QVOT_KEY_IS_SENSITIVE(tagId))
    {
        if(OT_ERROR_NONE != qorvoEncrypt((uint8_t*)&buffer.NvmData, aValue, buffer.dataSize))
        {
            return OT_ERROR_NO_BUFS;
        }
        buffer.dataSize += QVOT_CRYPTO_META_DATA_SIZE;
    }
    else
#endif // QVOT_SECURE_STORAGE
    {
        MEMCPY((void*)&buffer.NvmData, aValue, aValueLength);
    }
    buffer.dataValid = 1;

    if(isFlatTag == false)
    {
        GP_LOG_PRINTF(LOG_PREFIX "add child key:%d ind:%d tag:%d: stored %u/%u", 0, aKey,
                      qorvoSettings_NrOfChildrenStored, tagId, buffer.dataSize, maxTagSize);
    }
    else
    {
        GP_LOG_PRINTF(LOG_PREFIX "add key:%d tag:%d stored %u/%u, sensitive: %d", 0, aKey, tagId, buffer.dataSize,
                      maxTagSize, QVOT_KEY_IS_SENSITIVE(tagId));
    }
    gpNvm_BackupProtected(GP_COMPONENT_ID, tagId, (uint8_t*)(&buffer));
#ifdef GP_LOCAL_LOG
    gpLog_PrintBuffer(buffer.dataSize, (uint8_t*)&buffer.NvmData);
#endif // GP_LOCAL_LOG

    // Update children stored variable
    if(aKey == OT_SETTINGS_KEY_CHILD_INFO)
    {
        qorvoSettings_NrOfChildrenStored++;
        gpNvm_BackupProtected(GP_COMPONENT_ID, NVM_TAG_QVOT_NROFCHILDRENSTORED,
                              (uint8_t*)(&qorvoSettings_NrOfChildrenStored));
    }

    return OT_ERROR_NONE;
}

otError qorvoSettingsDelete(uint16_t aKey, int aChildIndex)
{
    NvmTag_t* pKeyTag;

    pKeyTag = qorvoSettings_GetTagStructByKey(aKey);
    if(pKeyTag == NULL)
    {
        GP_LOG_PRINTF(LOG_PREFIX "WARN: key:%d NOT FOUND", 0, aKey);
        return OT_ERROR_NOT_FOUND;
    }
    if(aKey == NVM_TAG_QVOT_KEY_MATERIAL)
    {
        // Not deleting key via the settings interface, as this could keep encrypted entries unencryptable
        return OT_ERROR_NONE;
    }
    if(aKey == OT_SETTINGS_KEY_CHILD_INFO)
    {
        return qorvoSettings_DeleteChild(aChildIndex);
    }
    else
    {
        GP_LOG_PRINTF(LOG_PREFIX "del key:%d tag:%d", 0, aKey, pKeyTag->nvmTagId);
        gpNvm_ClearProtected(GP_COMPONENT_ID, pKeyTag->nvmTagId);
    }

    return OT_ERROR_NONE;
}

void qorvoSettingsWipe(void)
{
    GP_LOG_PRINTF(LOG_PREFIX "Wipe all", 0);
    qorvoDeleteKey();
    gpNvm_ClearProtected(GP_COMPONENT_ID, gpNvm_AllTags);
}
