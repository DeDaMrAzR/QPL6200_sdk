/*
 * Copyright (c) 2017-2024, Qorvo Inc
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

#pragma once

/* Verify Configuration */
#define VERIFY_CONFIG(flag) (OPENTHREAD_CONFIG_##flag##_ENABLE) && !(QORVO_##flag##_ENABLE)

#if defined(OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE) && VERIFY_CONFIG(COAP_SECURE_API)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_COAP_SECURE_API_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_ECDSA_ENABLE) && VERIFY_CONFIG(ECDSA)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_ECDSA_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_COMMISSIONER_ENABLE) && VERIFY_CONFIG(COMMISSIONER)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_COMMISSIONER_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_JOINER_ENABLE) && VERIFY_CONFIG(JOINER)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_JOINER_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_TCP_ENABLE) && VERIFY_CONFIG(TCP)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_TCP_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE) && VERIFY_CONFIG(BORDER_AGENT)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_BORDER_AGENT_ENABLE not supported"
#endif

#if defined(OPENTHREAD_CONFIG_BLE_TCAT_ENABLE) && VERIFY_CONFIG(BLE_TCAT)
#error "MbedTls configuration mismatch: OPENTHREAD_CONFIG_BLE_TCAT_ENABLE not supported"
#endif
