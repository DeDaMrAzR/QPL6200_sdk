/**
 * @file libs/base/include/esec_map.h
 * @brief Physical mapping of eSecure
 * @copyright Copyright (c) 2016-2018 Silex Insight. All Rights reserved
 */

#ifndef ESECURE_MAP_H
#define ESECURE_MAP_H

#include "global.h"
#include "gpHal_kx_mm.h"
#include "gpHal_kx_defs.h"

#ifdef GP_COMP_CHIPEMU
#include "gpChipEmu_regaccess_xp400x_esecure.h"
#else
#define ESEC_WRITE_REGISTER_U32(register, value) (register = value)
#define ESEC_READ_REGISTER_U32(register)         (register)
#endif // GP_COMP_CHIPEMU

#include "esec_addr.h"

// Interrupts
#define IRQ_RTC          (1 << 0)
#define IRQ_TAMPER       (1 << 1)
#define IRQ_EXTDMA       (1 << 2)
#define IRQ_WATCHDOG     (1 << 3)
#define IRQ_PUF          (1 << 4)
#define IRQ_TAMPERDONE   (1 << 5)
#define IRQ_TIMER0       (1 << 6)
#define IRQ_TIMER1       (1 << 7)
#define IRQ_TIMER2       (1 << 8)
#define IRQ_TIMER3       (1 << 9)
#define IRQ_TIMER4       (1 << 10)
#define IRQ_PWRDOWN      (1 << 11)
#define IRQ_CHALLENGE_TX (1 << 12)
#define IRQ_CHALLENGE_RX (1 << 13)
#define IRQ_MAILBOX_TX   (1 << 14)
#define IRQ_MAILBOX_RX   (1 << 15)
#define IRQ_CRYPTODMA    (1 << 16)
#define IRQ_RNG          (1 << 17)
#define IRQ_PK           (1 << 18)
#define IRQ_WATCHDOG_L2  (1 << 31)

// Tamper sources
#define TAMPER_SRC_NRSTPOR              0
#define TAMPER_SRC_FILTERCNT            1
#define TAMPER_SRC_WATCHDOG             2
#define TAMPER_SRC_IKG                  3
#define TAMPER_SRC_RAM_FAILURE          4
#define TAMPER_SRC_RUNTIME_ERROR        5
#define TAMPER_SRC_ASSERT               5
#define TAMPER_SRC_RAM_CORRECTION       7
#define TAMPER_SRC_ESEC_BOOT_ERROR      8
#define TAMPER_SRC_HOST_BOOT_ERROR      9
#define TAMPER_SRC_MAILBOX_AUTH_ERROR   10
#define TAMPER_SRC_CHALLENGE_AUTH_ERROR 11
#define TAMPER_SRC_SECCFG_ERROR         12
#define TAMPER_SRC_INTEGRITY_ERROR      13
#define TAMPER_SRC_SELFTEST_ERROR       14
#define TAMPER_SRC_RNG_ERROR            15
#define TAMPER_SRC_EXT_0                16
#define TAMPER_SRC_EXT_1                17
#define TAMPER_SRC_EXT_2                18
#define TAMPER_SRC_EXT_3                19
#define TAMPER_SRC_EXT_4                20
#define TAMPER_SRC_EXT_5                21
#define TAMPER_SRC_EXT_6                22
#define TAMPER_SRC_EXT_7                23
#define TAMPER_SRC_EXT_8                24
#define TAMPER_SRC_EXT_9                25
#define TAMPER_SRC_EXT_10               26
#define TAMPER_SRC_EXT_11               27
#define TAMPER_SRC_EXT_12               28
#define TAMPER_SRC_EXT_13               29
#define TAMPER_SRC_EXT_14               30
#define TAMPER_SRC_EXT_15               31

#endif
