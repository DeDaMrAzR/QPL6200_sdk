/**
 * @brief eSecure Library: Asynchronous SRP functions
 * @copyright Copyright (c) 2019Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_ASYNC_SRP_HEADER
#define ESEC_ASYNC_SRP_HEADER

#include <stdint.h>
#include <stddef.h>

#include "esec_errors.h"
#include "esec_srp_basics.h"

/** Create asynchronously a SRP verifier from the user credentials.
 *
 * 'salt' is a random value. The created SRP verifier and the salt are sent to
 * the server when registering the user.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_srp_gen_verifier_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                  struct esec_srp_user_credentials* creds, uint8_t* salt, size_t salt_len,
                                  uint8_t* verifier);

/** Create asynchronously a user session state to establish a connection with
 *  a server via SRP
 *
 * The user session public value ('A') in session should be sent to the server.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_srp_user_gen_public_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                     struct esec_srp_user_session_state* session);

/** Create asynchronously a session state to establish a connection with a user
 *  via SRP
 *
 * The caller should provide the same 'verifier' value that the user
 * sent at registration time.
 * The server session public value ('B') in session should be sent to the user.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_srp_server_gen_public_go(struct esec_mb* mb, struct esec_srp_config* cfg, uint8_t* verifier,
                                       struct esec_srp_server_session_state* session);

/** Generate asynchronously a shared key at the user side
 *
 * The server session public value ('B') was received from the server. The
 * server normally also sent the 'salt' the user provided at registration
 * time.
 *
 * The buffer 'sharedkey' should be large enough to hold a digest of the
 * configured hash algorithm.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_srp_user_gen_key_go(struct esec_mb* mb, struct esec_srp_config* cfg,
                                  struct esec_srp_user_credentials* creds, uint8_t* B, uint8_t* salt, size_t salt_len,
                                  struct esec_srp_user_session_state* session, uint8_t* sharedkey);

/** Generate asynchronously a shared key at the server side
 *
 * The user session public value ('A') was received from the user.
 *
 * The buffer 'sharedkey' should be large enough to hold a digest of the
 * configured hash algorithm.
 *
 * After issuing this command, caller must wait a response becomes available
 * by calling ::esec_pop_cmd_response or polling ::esec_pop_cmd_response_if_any
 **/
uint32_t esec_srp_server_gen_key_go(struct esec_mb* mb, struct esec_srp_config* cfg, uint8_t* A, uint8_t* verifier,
                                    struct esec_srp_server_session_state* session, uint8_t* sharedkey);

#endif
