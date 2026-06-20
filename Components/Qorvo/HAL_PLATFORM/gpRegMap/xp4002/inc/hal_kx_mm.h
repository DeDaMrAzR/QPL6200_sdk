#include "mm.h"

/*******************************************************************************
 *                      Defines
 ******************************************************************************/

#define Q_MM_OTP_SIZE          (16*512)
#ifndef GP_COMP_CHIPEMU
#define Q_MM_OTP_START         (GP_MM_CMA_NVM_LINEAR_END - Q_MM_OTP_SIZE)
#else
extern UInt8 otp_emu[Q_MM_OTP_SIZE];
#define Q_MM_OTP_START         ((UIntPtr)otp_emu)
#endif

#define SYSRAM_MW_NO_CRC       0x33BC0000
#define SYSRAM_MW_CRC_RETAIN   0xA4C30000
#define SYSRAM_MW_CRC_ALL      0x4A560000
#define SYSRAM_MW_MASK         0xFFFF0000
#define SYSRAM_RET_MAP1_MASK   0x00001FFF
#define SYSRAM_RET_MAP1_OFFSET 0

#define SYSRAM_DCO_COARSE_MASK 0x000000FF
#define SYSRAM_CLK_MODE_MASK   0x0000FF00
#define SYSRAM_CLK_MODE_OFFSET 8

extern uint8_t __sysram_retain_header_start[];
extern uint8_t __sysram_retain_body_start[];
extern uint8_t __sysram_esevents_start[];
extern uint8_t __sysram_esevents_end[];

#define SYSRAM_MW_ADDRESS             GP_MM_ADDR_FROM_SYS((uint32_t)&__sysram_retain_header_start)
#define SYSRAM_RET_MAP0_ADDRESS       GP_MM_ADDR_FROM_SYS((uint32_t)&__sysram_retain_header_start + 0x4)
#define SYSRAM_CRC_ADDRESS            GP_MM_ADDR_FROM_SYS((uint32_t)&__sysram_retain_header_start + 0x8)
#define SYSRAM_OPT_OFFSET_BYTES       (0xC)
#define SYSRAM_OPT_ADDRESS            GP_MM_ADDR_FROM_SYS((uint32_t)&__sysram_retain_header_start + SYSRAM_OPT_OFFSET_BYTES)
#define SYSRAM_OPT_SIZE_BYTES         (4)

#if((SYSRAM_OPT_OFFSET_BYTES + SYSRAM_OPT_SIZE_BYTES) > (GP_MM_SYSRAM_RAM_REGMAP_START - GP_MM_SYSRAM_START))
#error "SYSRAM overlap detected!"
#endif

#define SYSRAM_RETENTION_OFFSET_BYTES ((uint32_t)&__sysram_retain_body_start - GP_MM_SYSRAM_START)
#define SYSRAM_RETENTION_START        GP_MM_ADDR_FROM_SYS((uint32_t)&__sysram_retain_body_start)
#define SYSRAM_RETENTION_END          (SYSRAM_RETENTION_START + GP_WB_REGMAP_RETAIN_SIZE)
#define SYSRAM_RETENTION_SIZE         (SYSRAM_RETENTION_END - SYSRAM_RETENTION_START)

#ifndef GP_COMP_CHIPEMU
#define SYSRAM_ES_EVENTS_START        ((uint32_t)&__sysram_esevents_start)
#define SYSRAM_ES_EVENTS_END          ((uint32_t)&__sysram_esevents_end)
#else
#define SYSRAM_ES_EVENTS_START        (0x40000600)
#define SYSRAM_ES_EVENTS_END          (0x40000700)
#endif
#define SYSRAM_ES_EVENTS_SIZE         (SYSRAM_ES_EVENTS_END - SYSRAM_ES_EVENTS_START)
#define SYSRAM_ES_EVENT_OFFSET        (0x00000010)

#define SYSRAM_WIPE_INIT_START        (0)
#define SYSRAM_WIPE_INIT_END_ALL      ((GP_MM_SYSRAM_SIZE - 4))

#define SYSRAM_CRC_INIT_START         (SYSRAM_OPT_OFFSET_BYTES)
#define SYSRAM_CRC_INIT_END_RETAIN    (SYSRAM_RETENTION_OFFSET_BYTES + GP_WB_REGMAP_RETAIN_SIZE - 4)
#define SYSRAM_CRC_INIT_END_ALL       ((GP_MM_SYSRAM_SIZE - 4))

#define SYSRAM_RETAIN_INIT_START      (SYSRAM_RETENTION_OFFSET_BYTES)

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define SYSRAM_GET_MW() ((*(UInt32*)SYSRAM_MW_ADDRESS) & SYSRAM_MW_MASK)

#define SYSRAM_SET_MW(mw)                                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        UInt32 temp = *(UInt32*)SYSRAM_MW_ADDRESS;                                                                     \
        *(UInt32*)SYSRAM_MW_ADDRESS = ((temp & ~SYSRAM_MW_MASK) | (mw & SYSRAM_MW_MASK));                              \
    } while(0)

#define SYSRAM_GET_MAP1_RETAIN() (((*(UInt32*)SYSRAM_MW_ADDRESS) & SYSRAM_RET_MAP1_MASK) >> SYSRAM_RET_MAP1_OFFSET)

#define SYSRAM_SET_MAP1_RETAIN(value)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        UInt32 temp = *(UInt32*)SYSRAM_MW_ADDRESS;                                                                     \
        UInt32 value_shifted = ((UInt32)value << SYSRAM_RET_MAP1_OFFSET);                                              \
        *(UInt32*)SYSRAM_MW_ADDRESS = ((temp & ~SYSRAM_RET_MAP1_MASK) | (value_shifted & SYSRAM_RET_MAP1_MASK));       \
    } while(0)

#define SYSRAM_GET_MAP0_RETAIN() (*(UInt32*)SYSRAM_RET_MAP0_ADDRESS)

#define SYSRAM_SET_MAP0_RETAIN(value)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        *(UInt32*)SYSRAM_RET_MAP0_ADDRESS = value;                                                                     \
    } while(0)

#define SYSRAM_GET_CRC() (*(UInt32*)SYSRAM_CRC_ADDRESS)

#define SYSRAM_SET_CRC(value)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        *(UInt32*)SYSRAM_CRC_ADDRESS = value;                                                                          \
    } while(0)

#define SYSRAM_GET_DCO_COARSE() ((*(UInt32*)SYSRAM_OPT_ADDRESS) & SYSRAM_DCO_COARSE_MASK)

#define SYSRAM_SET_DCO_COARSE(value)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        UInt32 temp = *(UInt32*)SYSRAM_OPT_ADDRESS;                                                                    \
        *(UInt32*)SYSRAM_OPT_ADDRESS = ((temp & ~SYSRAM_DCO_COARSE_MASK) | (value & SYSRAM_DCO_COARSE_MASK));          \
    } while(0)

#define SYSRAM_GET_CLK_MODE() (((*(UInt32*)SYSRAM_OPT_ADDRESS) & SYSRAM_CLK_MODE_MASK) >> SYSRAM_CLK_MODE_OFFSET)

#define SYSRAM_SET_CLK_MODE(value)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        UInt32 temp = *(UInt32*)SYSRAM_OPT_ADDRESS;                                                                    \
        UInt32 value_shifted = ((UInt32)value << SYSRAM_CLK_MODE_OFFSET);                                              \
        *(UInt32*)SYSRAM_OPT_ADDRESS = ((temp & ~SYSRAM_CLK_MODE_MASK) | (value_shifted & SYSRAM_CLK_MODE_MASK));      \
    } while(0)
