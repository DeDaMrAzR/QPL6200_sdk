/**
 * @brief eSecure Library: Elliptic Curve JPAKE key definition
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_JPAKE_BASICS_HEADER
#define ESEC_JPAKE_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_keys_basics.h"

/** J-PAKE configuration
 *
 * The user ids and asymmetric key domain definition for the EC J-JPAKE
 * steps.
 *
 * There is no need to specify the curve to use as only the ECC P256 is valid
 * as defined in the Thread ("Thread Commissioning" ).
 **/
struct esec_jpake_cfg {
    const uint8_t* user_id1;
    size_t idlen1;
    const uint8_t* user_id2;
    size_t idlen2;
};

#endif
