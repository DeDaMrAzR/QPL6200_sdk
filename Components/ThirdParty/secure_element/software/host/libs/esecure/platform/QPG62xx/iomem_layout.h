/**
 * @brief Physical mapping of host
 * @copyright Copyright (c) 2024, Qorvo Inc
 */


#ifndef HOST_MAP_H
#define HOST_MAP_H

#include "gpRegMap.h"

// xp4001 backwards compatibility

// Address map and interrupt lines, as defined in Hardware Architecture document.
#define ADDR_RAM         GP_MM_APPUC_RAM_START
#define ADDR_MAILBOX     GP_MM_SECURE_ELEMENT_START

// External address map
#ifndef ESEC_TEST_ADDR
#define ESEC_TEST_ADDR   0xa4000000
#endif
#ifndef ADDR_RAM_HOST
#define ADDR_RAM_HOST    (ESEC_TEST_ADDR+0x00100000)
#endif

#endif
