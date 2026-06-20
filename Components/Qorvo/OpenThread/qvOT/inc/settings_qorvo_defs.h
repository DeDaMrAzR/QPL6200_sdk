/*
 *  Copyright (c) 2024, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file includes the Qorvo Tag interface nmv layout.
 *
 */

#pragma once

#ifdef GP_DIVERSITY_GPHAL_XP4002
#define QVOT_SECURE_STORAGE
#endif

/*****************************************************************************
 *                    Includes Definitions
 *****************************************************************************/
#include "openthread/platform/settings.h"
#ifdef QVOT_SECURE_STORAGE
#include "esecure.h"
#endif // def QVOT_SECURE_STORAGE

/*****************************************************************************
 *                    Macros and Types Definitions
 *****************************************************************************/
#define OT_SETTINGS_KEY_ENCRYPTION_KEY 0 // Not used by ot, not really used, more a placeholder

#ifdef QVOT_SECURE_STORAGE
#define QVOT_SKEY_SIZE             32 // bytes
#define QVOT_TAG_SIZE              4  // bytes
#define QVOT_IV_SIZE               12 // bytes
#define QVOT_CRYPTO_META_DATA_SIZE (QVOT_TAG_SIZE + QVOT_IV_SIZE)
#define QVOT_SKEY_META_DATA_SIZE   (ESEC_STOR_PROT_SIZE + ESEC_STOR_AUTH_SIZE + 1)
#else
#define QVOT_SKEY_SIZE             0
#define QVOT_CRYPTO_META_DATA_SIZE 0
#define QVOT_SKEY_META_DATA_SIZE   0
#endif // QVOT_SECURE_STORAGE

#define QVOT_MAX_SUPPORTED_CHILDREN 130 // Hard maximum, due to implementation
#define QVOT_MIN_SUPPORTED_CHILDREN 10  // Minimum required by Thread specification

#define NVM_TAG_QVOT_NROFCHILDRENSTORED 0x00
#define NVM_TAG_QVOT_ACTIVEDATASET      0x01
#define NVM_TAG_QVOT_PENDINGDATASET     0x02
#define NVM_TAG_QVOT_NETWORKINFO        0x03
#define NVM_TAG_QVOT_PARENTINFO         0x04
// Move NVM_TAG_QVOT_CHILDINFO_BASE  (was 0x05) to last entry
#define NVM_TAG_QVOT_EOL_AUTOSTART     0x06
#define NVM_TAG_QVOT_SLAACIIDSECRETKEY 0x07
#define NVM_TAG_QVOT_DADINFO           0x08
#define NVM_TAG_QVOT_EOL_OMRPREFIX     0x09
#define NVM_TAG_QVOT_EOL_ONLINKPREFIX  0x0a
#define NVM_TAG_QVOT_SRPKEY            0x0b
#define NVM_TAG_QVOT_SRPCLIENTINFO     0x0c
#define NVM_TAG_QVOT_SRPSERVERINFO     0x0d
#define NVM_TAG_QVOT_EOL_NAT64PREFIX   0x0e
#define NVM_TAG_QVOT_BRULAPREFIX       0x0f
#define NVM_TAG_QVOT_BRONLINKPREFIXES  0x10
#define NVM_TAG_QVOT_BORDERAGENTID     0x11
#define NVM_TAG_QVOT_KEY_MATERIAL      0x12

#define NVM_TAG_QORVO_LAST NVM_TAG_QVOT_KEY_MATERIAL

// Must be last
#define NVM_TAG_QVOT_CHILDINFO_BASE (NVM_TAG_QORVO_LAST + 1)

#define NVM_TAG_QVOT_SIZEOF_KEY_MATERIAL       (QVOT_SKEY_SIZE + QVOT_SKEY_META_DATA_SIZE)
#define NVM_TAG_QVOT_SIZEOF_NROFCHILDRENSTORED (1)
#define NVM_TAG_QVOT_SIZEOF_ACTIVEDATASET      (120) /* bytes. 255 by the spec, 120 in practice */
#define NVM_TAG_QVOT_SIZEOF_PENDINGDATASET     (120) /* bytes. pending = active dataset + pending ts and timer */
#define NVM_TAG_QVOT_SIZEOF_NETWORKINFO        (38)  /* bytes. */
#define NVM_TAG_QVOT_SIZEOF_PARENTINFO         (10)  /* bytes. */
#define NVM_TAG_QVOT_SIZEOF_SLAACIIDSECRETKEY  (32)  /* bytes. */
#define NVM_TAG_QVOT_SIZEOF_DADINFO            (1)   /* bytes. uint8 counter */
#define NVM_TAG_QVOT_SIZEOF_SRPKEY             (126) /* bytes. */
#define NVM_TAG_QVOT_SIZEOF_SRPCLIENTINFO      (18)  /* bytes. OT_IP6_ADDRESS_SIZE + port(uint16) */
#define NVM_TAG_QVOT_SIZEOF_SRPSERVERINFO      (18)  /* bytes. OT_IP6_ADDRESS_SIZE + port(uint16) */
#define NVM_TAG_QVOT_SIZEOF_BRULAPREFIX        (17)  /* bytes. OT_IP6_ADDRESS_SIZE + length(uint8) */
#define NVM_TAG_QVOT_SIZEOF_CHILDINFO          (17)  /* bytes. One for each child */
#define NVM_TAG_QVOT_SIZEOF_BRONLINKPREFIXES   (16)  /* bytes. number of prefixes x 16 byres, currently 1 prefix */
#define NVM_TAG_QVOT_SIZEOF_BORDERAGENTID      (16)  /* bytes. OT_BORDER_AGENT_ID_LENGTH*/

#define NVM_TAG_QVOT_MAX_SIZEOF_KEY_MATERIAL       NVM_TAG_QVOT_SIZEOF_KEY_MATERIAL
#define NVM_TAG_QVOT_MAX_SIZEOF_NROFCHILDRENSTORED NVM_TAG_QVOT_SIZEOF_NROFCHILDRENSTORED
#define NVM_TAG_QVOT_MAX_SIZEOF_ACTIVEDATASET      (NVM_TAG_QVOT_SIZEOF_ACTIVEDATASET + QVOT_CRYPTO_META_DATA_SIZE)
#define NVM_TAG_QVOT_MAX_SIZEOF_PENDINGDATASET     (NVM_TAG_QVOT_SIZEOF_PENDINGDATASET + QVOT_CRYPTO_META_DATA_SIZE)
#define NVM_TAG_QVOT_MAX_SIZEOF_NETWORKINFO        NVM_TAG_QVOT_SIZEOF_NETWORKINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_PARENTINFO         NVM_TAG_QVOT_SIZEOF_PARENTINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_SLAACIIDSECRETKEY  NVM_TAG_QVOT_SIZEOF_SLAACIIDSECRETKEY
#define NVM_TAG_QVOT_MAX_SIZEOF_DADINFO            NVM_TAG_QVOT_SIZEOF_DADINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_SRPKEY             (NVM_TAG_QVOT_SIZEOF_SRPKEY + QVOT_CRYPTO_META_DATA_SIZE)
#define NVM_TAG_QVOT_MAX_SIZEOF_SRPCLIENTINFO      NVM_TAG_QVOT_SIZEOF_SRPCLIENTINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_SRPSERVERINFO      NVM_TAG_QVOT_SIZEOF_SRPSERVERINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_BRULAPREFIX        NVM_TAG_QVOT_SIZEOF_BRULAPREFIX
#define NVM_TAG_QVOT_MAX_SIZEOF_CHILDINFO          NVM_TAG_QVOT_SIZEOF_CHILDINFO
#define NVM_TAG_QVOT_MAX_SIZEOF_BRONLINKPREFIXES   NVM_TAG_QVOT_SIZEOF_BRONLINKPREFIXES
#define NVM_TAG_QVOT_MAX_SIZEOF_BORDERAGENTID      NVM_TAG_QVOT_SIZEOF_BORDERAGENTID
