/**
 * @brief eSecure registers
 * @copyright Copyright (c) 2017-2018 Silex Insight. All Rights reserved
 */

#ifndef ESEC_MB_REGS_H
#define ESEC_MB_REGS_H

#define ESEC_MB_REG_FIFO_DATA        (ADDR_MAILBOX + 0x00)
#define ESEC_MB_REG_TX_STATUS        (ADDR_MAILBOX + 0x40)
#define ESEC_MB_REG_RX_STATUS        (ADDR_MAILBOX + 0x44)
#define ESEC_MB_REG_TX_PROTECTION    (ADDR_MAILBOX + 0x48)
#define ESEC_MB_REG_RX_PROTECTION    (ADDR_MAILBOX + 0x4C)
#define ESEC_MB_REG_TX_HEADER        (ADDR_MAILBOX + 0x50)
#define ESEC_MB_REG_RX_HEADER        (ADDR_MAILBOX + 0x54)
#define ESEC_MB_REG_CONFIGURATION    (ADDR_MAILBOX + 0x58)

#define ESEC_MB_CONFIG_TXINTEN       0x00000001
#define ESEC_MB_CONFIG_RXINTEN       0x00000002

#endif
