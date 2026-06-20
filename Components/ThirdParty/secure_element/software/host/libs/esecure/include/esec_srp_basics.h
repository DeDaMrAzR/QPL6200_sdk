/**
 * @brief eSecure Library: SRP key, credentials and state definition
 * @copyright Copyright (c) 2019 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SRP_BASICS_HEADER
#define ESEC_SRP_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_keys_basics.h"
#include "esec_hash_basics.h"

#ifndef ESEC_SRP_MAX_NUM_SIZE
/** The maximum size in bytes of numbers in the domain.
 *
 * See also esec_srp_config::num_size.
 *
 * The maximum size can be adapted at compile time to more precisely match
 * the needs of the application.
 **/
#define ESEC_SRP_MAX_NUM_SIZE (3 * 128)
#endif

enum ESEC_SRP_VERSION { ESEC_SRP_VERSION_SRP6 = 0, ESEC_SRP_VERSION_SRP6A = 1 };

/** The configuration for the SRP protocol.
 *
 * The whole structure should be filled in with the parameters used by both
 * parties of the SRP protocol.
 **/
struct esec_srp_config {
    /** Size in bytes of numbers in the domain.
     *
     * This not only gives the size of the modulus and generator, but also
     * of the intermediate values. Obviously this should be smaller than
     * ESEC_SRP_MAX_NUM_SIZE.
     **/
    uint32_t num_size;

    /** Domain modulus **/
    uint8_t* modulus;

    /** Generator big number **/
    uint8_t* generator;

    /** SRP protocol version.
     *
     * Currently, only SRPv6 and SRPv6a are allowed.
     **/
    enum ESEC_SRP_VERSION version;

    /** Hash algorithm used in the SRP computations. **/
    uint32_t hash_algo;
};

/** User credentials used in SRP protocol.
 *
 * The credentials never leave the user environment. They are never sent to
 * the server.
 *
 * The whole structure is filled in with the credentials obtained from the
 * user.
 **/
struct esec_srp_user_credentials {
    /** Name or login of the user trying to log in. **/
    uint8_t* username;

    /** Size of the username string. **/
    size_t username_len;

    /** User password **/
    uint8_t* password;

    /** Size in bytes of the password string **/
    size_t password_len;
};

/** Connection establishment state on the user side.
 *
 * Filled in by esec_srp_user_gen_public().
 **/
struct esec_srp_user_session_state {
    /** user session secret value **/
    uint8_t a[ESEC_SRP_MAX_NUM_SIZE];

    /** user session public value **/
    uint8_t A[ESEC_SRP_MAX_NUM_SIZE];
};

/** Connection establishment state on the server side.
 *
 * Filled in by esec_srp_server_gen_public().
 **/
struct esec_srp_server_session_state {
    /** server session secret value**/
    uint8_t b[ESEC_SRP_MAX_NUM_SIZE];

    /** server public session key **/
    uint8_t B[ESEC_SRP_MAX_NUM_SIZE];
};

#endif
