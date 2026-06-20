/**
 * @brief eSecure Library: Symmetric keys representation
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SYMM_BASICS_HEADER
#define ESEC_SYMM_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"

/** eSecure symmetric key.
 *
 * Represents any kind of eSecure symmetric key. The key can be protected,
 * volatile or unprotected.
 **/
struct esec_skey {
    struct esec_key_storage storage;
    uint16_t keyspec;
};

#endif
