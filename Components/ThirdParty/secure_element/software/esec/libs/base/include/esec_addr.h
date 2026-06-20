/**
 * @file libs/base/include/esec_addr.h
 * @brief addresses of eSecure
 * @copyright Copyright (c) 2016-2018 Silex Insight. All Rights reserved
 */

#ifndef ESECURE_ADDR_H
#define ESECURE_ADDR_H

#ifdef __arm__
#define PERIPH_BASE_ADDR 0x20000000
#else
#define PERIPH_BASE_ADDR 0x00000000
#endif

// Address map and interrupt lines, as defined in Hardware Architecture document.
#define ADDR_RAM           0x10000000
#define ADDR_ROM           0x20000000
#define ADDR_MAILBOX       (0x30000000 + PERIPH_BASE_ADDR)
#define ADDR_CRYPTOWRAP    (0x31000000 + PERIPH_BASE_ADDR)
#define ADDR_INTCTRL       (0x32000400 + PERIPH_BASE_ADDR)
#define ADDR_EXTDMA        (0x32000800 + PERIPH_BASE_ADDR)
#define ADDR_WATCHDOG      (0x32000C00 + PERIPH_BASE_ADDR)
#define ADDR_PUF           (0x32001000 + PERIPH_BASE_ADDR)
#define ADDR_CHALLENGE     (0x32001400 + PERIPH_BASE_ADDR)
#define ADDR_TIMER         (0x32001800 + PERIPH_BASE_ADDR)
#define ADDR_SYSREG        (0x32001C00 + PERIPH_BASE_ADDR)
#define ADDR_BOOTREGS      (0x33000000 + PERIPH_BASE_ADDR)
#define ADDR_BATTREGS      (0x33800000 + PERIPH_BASE_ADDR)
#define ADDR_DBG_GRANT     (0x36000000 + PERIPH_BASE_ADDR)
#define ADDR_REMAP         (0x36000400 + PERIPH_BASE_ADDR)
#define ADDR_APBM_CORE     (0x37000000 + PERIPH_BASE_ADDR)
#define ADDR_APBM_PDON     (0x38000000 + PERIPH_BASE_ADDR)
#define ADDR_RTC           (0x39000000 + PERIPH_BASE_ADDR)
#define ADDR_TAMPER        (0x39000400 + PERIPH_BASE_ADDR)
#define ADDR_DMAFIFO_FETCH 0xAF9DEE01 /* MW disguised as address to indicate fetcher address */
#define ADDR_DMAFIFO_PUSH  0xAF9DEE02 /* MW disguised as address to indicate pusher address */
#define ADDR_QSPI_REGS     (ADDR_APBM_CORE + 0x00000000)

#define ADDR_OTP     GP_MM_ADDR_FROM_SYS(Q_MM_OTP_START)
#define ADDR_NVM     GP_MM_ADDR_FROM_SYS(GP_MM_CMA_NVM_LINEAR_START)
#define ADDR_PRINTER GP_MM_ADDR_FROM_SYS(GP_MM_SYSRAM_END - 4)
#define ADDR_QSPI    GP_MM_ADDR_FROM_SYS(0x70000000)

#endif
