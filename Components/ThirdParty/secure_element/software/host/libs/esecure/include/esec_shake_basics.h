/**
 * @brief eSecure Library: Asynchronous SHA hash (SHAKE) algorithms definition
 * @copyright Copyright (c) 2018 Silex Insight. All Rights reserved
 * @file
 */

#ifndef ESEC_SHA3_BASICS_HEADER
#define ESEC_SHA3_BASICS_HEADER

#include <stdint.h>
#include <stddef.h>

/** SHAKE_128 hash algorithm **/
#define ESEC_HASH_ALGO_SHAKE_128 0x01

/** SHAKE_256 hash algorithm **/
#define ESEC_HASH_ALGO_SHAKE_256 0x02

/** Message size granularity for SHAKE_128 with esec_hash_update(). **/
#define ESEC_HASH_BLOCK_SIZE_SHAKE_128 168

/** Message size granularity for SHAKE_256 with esec_hash_update(). **/
#define ESEC_HASH_BLOCK_SIZE_SHAKE_256 136

#endif
