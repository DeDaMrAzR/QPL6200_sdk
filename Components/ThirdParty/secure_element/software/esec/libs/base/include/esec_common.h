/**
 * @file libs/base/include/esec_common.h
 * @brief Common functions and types used by drivers and by cryptolib
 * @copyright Copyright (c) 2016-2018 Silex Insight. All Rights reserved
 */

#ifndef ESEC_COMMON_H
#define ESEC_COMMON_H

/* Define 'uint8_t', 'uint32_t'... */
#include <stdint.h>

/* Define 'bool' */
#include <stdbool.h>

/* Define 'NULL' */
#include <stddef.h>

/* Define supported features */
#include "esec_features.h"

/* Define address map */
#include "esec_map.h"

/* Define 'CHECK_RESULT' macro (from cryptolib) */
#include "compiler_extentions.h"

/* Define 'block_t' (from cryptolib) */
#include "cryptolib_types.h"

/* Define 'memcpy_*' functions (from cryptolib) */
#include "sx_memcpy.h"

/* Define 'memcmp*' functions (from cryptolib) */
#include "sx_memcmp.h"

/* Define 'SX_PRINTF'... */
#include "debug.h"

/* Return error codes */
#include "sx_errors.h"

/** @brief Command status */
#define OKAY            CRYPTOLIB_SUCCESS
#define UNSUPPORTED_CMD CRYPTOLIB_UNSUPPORTED_ERR  /**< Unsupported command */
#define AUTH_ERROR      2                          /**< Unauthorized command */
#define INVALID_SIGN    CRYPTOLIB_INVALID_SIGN_ERR /**< Invalid signature */
#define BUS_ERROR       CRYPTOLIB_DMA_ERR          /**< Bus Error (in case of DMA usage) */
#define TEST_FAILED     5                          /**< Test failed */
#define CRYPTO_ERROR    CRYPTOLIB_CRYPTO_ERR       /**< Error in crypto function (cryptolib) */
#define INVALID_PARAM   CRYPTOLIB_INVALID_PARAM    /**< Parameters are invalid */
typedef uint32_t esec_status_t;

#if defined(SEUC_HARDEN_MB_RETURN_CODES)
/* Hardened mailbox return codes */
#define MB_OKAY            1  /**< Command successful */
#define MB_UNSUPPORTED_CMD 2  /**< Unsupported command */
#define MB_AUTH_ERROR      4  /**< Unauthorized command */
#define MB_INVALID_SIGN    7  /**< Invalid signature */
#define MB_BUS_ERROR       8  /**< Bus Error (in case of DMA usage) */
#define MB_TEST_FAILED     11 /**< Test failed */
#define MB_CRYPTO_ERROR    13 /**< Error in crypto function (cryptolib) */
#define MB_INVALID_PARAM   14 /**< Parameters are invalid */
#endif

/** @brief Macro for routines returning status */
#define ESEC_STATUS esec_status_t CHECK_RESULT

/** @brief Inline function for maximal value between 2 numbers */

/* This function is used for example as following:
 * SX_MAX(tamper_get_CM_level(i, seccfg_cm),tamper_get_SM_level(i, seccfg_sm)).
 * This involves function call and reading the security configuration.
 * That's why it's an inline function instead of a macro.
 * Using a macro would mean that one argument would be evaluated twice */
static inline uint32_t SX_MAX(uint32_t a, uint32_t b)
{
    if(a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/**
 * @brief Keep CPU in sleep mode until event occurs
 * @param eventmask Mask indicating which events should wake the CPU
 */
void wait_for_events(uint32_t eventmask); // No check unused result because it's never used

/* Security counters */
#define CRYPTO_SECURITY_COUNTER_INIT_VALUE           0x10101010
#define CRYPTO_SECURITY_COUNTER_INCREMENT(x, status) (((x) << 1) + (status))
#define CRYPTO_SECURITY_COUNTER_TOTAL(x)             ((x * (x + 1)) + CRYPTO_SECURITY_COUNTER_INIT_VALUE)

#endif
