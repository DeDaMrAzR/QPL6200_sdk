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

#ifdef QORVO_APPLICATION_MBEDTLS_CONFIG
#include QORVO_APPLICATION_MBEDTLS_CONFIG
#endif // QORVO_APPLICATION_MBEDTLS_CONFIG

#ifndef QORVO_MBEDTLS_DEBUG
#define QORVO_MBEDTLS_DEBUG 0
#endif // QORVO_MBEDTLS_DEBUG

#if QORVO_MBEDTLS_DEBUG
#ifndef MBEDTLS_DEBUG_C
#define MBEDTLS_DEBUG_C
#endif // MBEDTLS_DEBUG_C
#endif // QORVO_MBEDTLS_DEBUG

// Enables secured CoAP, not used by Thread nor Matter
#ifndef QORVO_COAP_SECURE_API_ENABLE
#define QORVO_COAP_SECURE_API_ENABLE 1
#endif // QORVO_COAP_SECURE_API_ENABLE

// Required for SRP client and server and thus in Thread 1.2 and Matter
#ifndef QORVO_ECDSA_ENABLE
#define QORVO_ECDSA_ENABLE 1
#endif // QORVO_ECDSA_ENABLE

// Enables the optional Thread Commissioner feature
#ifndef QORVO_COMMISSIONER_ENABLE
#define QORVO_COMMISSIONER_ENABLE 0
#endif // QORVO_COMMISSIONER_ENABLE

// Enables TCP, Mandatory for Thread Components, optional for Thread Products
#ifndef QORVO_TCP_ENABLE
#define QORVO_TCP_ENABLE 0
#endif // QORVO_TCP_ENABLE

// Enables the Thread Joiner role, Mandatory for Thread Components, optional for Thread Products
#ifndef QORVO_JOINER_ENABLE
#define QORVO_JOINER_ENABLE 0
#endif // QORVO_JOINER_ENABLE

// Enables the Thread Border Agent functionality, Mandatory on Border Routers
#ifndef QORVO_BORDER_AGENT_ENABLE
#define QORVO_BORDER_AGENT_ENABLE 0
#endif // QORVO_BORDER_AGENT_ENABLE

// Enables BLE TCAT support, used by the Thread Commercial Commissioning (Certificates)
#ifndef QORVO_BLE_TCAT_ENABLE
#define QORVO_BLE_TCAT_ENABLE 0
#endif // QORVO_BLE_TCAT_ENABLE

// Enables TLS 1.3 support
#ifndef QORVO_MBEDTLS_TLS1_3_ENABLE
#define QORVO_MBEDTLS_TLS1_3_ENABLE 0
#endif // QORVO_MBEDTLS_TLS1_3_ENABLE
