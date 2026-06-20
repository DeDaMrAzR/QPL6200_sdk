/**
 * @brief Additional types and constants shared by both the platform functions
 *        and the eSecure library and used for mailbox.
 * @copyright Copyright (c) 2017-2021 Silex Insight. All Rights reserved
 *
 * @file
 */

#ifndef ESEC_MAILBOX_HEADER
#define ESEC_MAILBOX_HEADER

#include <stdint.h>

/** Maximum of numbers of a descriptors list for a mail and its response */
#ifndef ESEC_MAX_DESCS
#define ESEC_MAX_DESCS 16
#endif

/** DMA source and destination address. **/
#ifndef DMA_ADDR_64BIT
typedef uint32_t dmaaddr_t;
#else
typedef uint64_t dmaaddr_t;
#endif

#endif
