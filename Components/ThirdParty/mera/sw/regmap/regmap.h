#ifndef __COMMON_H__
#define __COMMON_H__

#include "global.h"

//Define base FROM_SYS and TO_SYS if not defined
#if !(defined(GP_MM_ADDR_FROM_SYS) || defined(GP_MM_ADDR_TO_SYS))
#if defined(GP_MM_ADDR_FROM_SYS) || defined(GP_MM_ADDR_TO_SYS)
#error "either define both or none"
#endif
#define GP_MM_ADDR_FROM_SYS(addr) (addr)
#define GP_MM_ADDR_TO_SYS(addr)   (addr)
#endif

// ***************************************************
// ****  base regmap types used by the halprop_struct.h file
// ***************************************************
typedef union {
    UInt64 reg64[1];
    UInt32 reg32[2];
    UInt16 reg16[4];
    UInt8  reg8[8];
} halprop_reg64;

typedef union {
    UInt32 reg32[1];
    UInt16 reg16[2];
    UInt8  reg8[4];
} halprop_reg32;

typedef union {
    UInt16 reg16[1];
    UInt8  reg8[2];
} halprop_reg16;

typedef union {
    UInt8  reg8[1];
} halprop_reg8;

#include "regmap_headers.h"


// ***************************************************
// ****  check retention size fits all retained fields
// ***************************************************
#if defined(GP_WB_REGMAP_RETAIN_SIZE) && defined(GP_MM_SYSRAM_RETENTION_SIZE)
#if GP_WB_REGMAP_RETAIN_SIZE > (GP_MM_SYSRAM_RETENTION_SIZE)
#error "retention size too small"
#endif
#endif

// ***************************************************
// ****  Relative regmaps intervals definitions
// ***************************************************
#define TIMERXL_INTERVAL (GP_WB_TIMERXL_1_TIMERXL32_PRESCALER_DIV_ADDRESS - \
                          GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS)
#define TIMERXL_BASE     (GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS)

#define TIMER32_INTERVAL (GP_WB_TIMERXL_1_TIMERXL32_PRESCALER_DIV_ADDRESS - \
                          GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS)
#define TIMER32_BASE     (GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS)

#define TIMER16_INTERVAL (GP_WB_TIMERXL_0_TIMERXL16_1_PRESCALER_DIV_ADDRESS - \
                          GP_WB_TIMERXL_0_TIMERXL16_0_PRESCALER_DIV_ADDRESS)
#define TIMER16_BASE     (GP_WB_TIMERXL_0_TIMERXL16_0_PRESCALER_DIV_ADDRESS)

#define PWMXLS_INTERVAL (GP_WB_PWMXLS_PWMXL_1_NEXT_PHASE_ADDRESS - \
                         GP_WB_PWMXLS_PWMXL_0_NEXT_PHASE_ADDRESS)
#define PWMXLS_BASE     (GP_WB_PWMXLS_PWMXL_0_NEXT_PHASE_ADDRESS)

#define QUIC_INTERVAL (GP_WB_QUIC_CHANNEL_1_TRIGGER_SRC_ADDRESS - \
                       GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_ADDRESS)
#define QUIC_BASE     (GP_WB_QUIC_CHANNEL_0_TRIGGER_SRC_ADDRESS)

#define QUIC_GPIO_INTERVAL (GP_WB_GPIO_QUIC_TRIGGER_GPIO_1_ADDRESS - \
                            GP_WB_GPIO_QUIC_TRIGGER_GPIO_0_ADDRESS)
#define QUIC_GPIO_BASE     (GP_WB_GPIO_QUIC_TRIGGER_GPIO_0_ADDRESS)

#define I2C_M_INTERVAL (GP_WB_I2C_M_1_PRESCALER_ADDRESS - \
                        GP_WB_I2C_M_0_PRESCALER_ADDRESS)
#define I2C_M_BASE     (GP_WB_I2C_M_0_PRESCALER_ADDRESS)

#define I2S_INTERVAL (GP_WB_I2S_1_TX_DATA_ADDRESS - \
                      GP_WB_I2S_0_TX_DATA_ADDRESS)
#define I2S_BASE     (GP_WB_I2S_0_TX_DATA_ADDRESS)

#define LED_INTERVAL (GP_WB_LEDS_LED1_ENABLE_ADDRESS - \
                      GP_WB_LEDS_LED0_ENABLE_ADDRESS)
#define LED_BASE     (GP_WB_LEDS_LED0_ENABLE_ADDRESS)

#define TIMERS_INTERVAL (GP_WB_TIMERS_TMR1_PRESCALER_DIV_ADDRESS - \
                         GP_WB_TIMERS_TMR0_PRESCALER_DIV_ADDRESS)
#define TIMERS_BASE     (GP_WB_TIMERS_TMR0_PRESCALER_DIV_ADDRESS)

#define PWMS_INTERVAL (GP_WB_PWMS_PWM1_UP_DOWN_ENABLE_ADDRESS - \
                       GP_WB_PWMS_PWM0_UP_DOWN_ENABLE_ADDRESS)
#define PWMS_BASE     (GP_WB_PWMS_PWM0_UP_DOWN_ENABLE_ADDRESS)

#define PWM_TIMESTAMP_INTERVAL (GP_WB_PWMS_TIMESTAMP1_ENABLE_ADDRESS - \
                                GP_WB_PWMS_TIMESTAMP0_ENABLE_ADDRESS)
#define PWM_TIMESTAMP_BASE     (GP_WB_PWMS_TIMESTAMP0_ENABLE_ADDRESS)

#define DMA_SCB_INTERVAL (GP_WB_DMAS_SCB_SRC_ADDR_1_ADDRESS - \
                          GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS)
#define DMA_SCB_BASE     (GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS)

#define AK_DMA_SCB_INTERVAL (GP_WB_AK_DMAS_SCB_SRC_ADDR_1_ADDRESS - \
                             GP_WB_AK_DMAS_SCB_SRC_ADDR_0_ADDRESS)
#define AK_DMA_SCB_BASE     (GP_WB_AK_DMAS_SCB_SRC_ADDR_0_ADDRESS)

// #define DMA_DCB_INTERVAL (GP_WB_DMAS_DCB_SRC_ADDR_1_ADDRESS -
                        //   GP_WB_DMAS_DCB_SRC_ADDR_0_ADDRESS)
#define DMA_DCB_INTERVAL 0xFFFF //invalid, since we only have one channel available for now
#define DMA_DCB_BASE     (GP_WB_DMAS_DCB_SRC_ADDR_0_ADDRESS)

// #define AK_DMA_DCB_INTERVAL (GP_WB_AK_DMAS_DCB_SRC_ADDR_1_ADDRESS -
                            //  GP_WB_AK_DMAS_DCB_SRC_ADDR_0_ADDRESS)
#define AK_DMA_DCB_INTERVAL 0xFFFF //invalid, since we only have one channel available for now
#define AK_DMA_DCB_BASE     (GP_WB_AK_DMAS_DCB_SRC_ADDR_0_ADDRESS)

// ***************************************************
// ****  OFFSET CHECKERS
// ***************************************************
#define GP_WB_CHECK_OFFSET_UART(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_UART_0_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_UART_1_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_UART_2_BASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_I2C_M(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_I2C_M_0_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_I2C_M_1_BASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_I2S(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_I2S_0_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_I2S_1_BASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_LED(offset) do {                              \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_LEDS_LED0_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_LEDS_LED1_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_LEDS_LED2_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_LEDS_LED3_ENABLE_ADDRESS));   \
} while (false)


#define GP_WB_CHECK_OFFSET_RTZ(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_RTZ_0_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_RTZ_1_BASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_ASP(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_ASP_0_BASE_ADDRESS) ||  \
                        ((offset) == GP_WB_ASP_1_BASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_TIMER(offset) do {                                     \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_TIMERS_TMR0_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERS_TMR1_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERS_TMR2_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERS_TMR3_PRESCALER_DIV_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_PWM(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_PWMS_PWM0_UP_DOWN_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMS_PWM1_UP_DOWN_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMS_PWM2_UP_DOWN_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMS_PWM3_UP_DOWN_ENABLE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_PWMXL(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_PWMXLS_PWMXL_0_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_1_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_2_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_3_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_4_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_5_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_6_NEXT_PHASE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMXLS_PWMXL_7_NEXT_PHASE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_TIMERXL(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_1_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_2_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_3_TIMERXL32_PRESCALER_DIV_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_TIMERXL32(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_TIMERXL_0_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_1_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_2_TIMERXL32_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_3_TIMERXL32_PRESCALER_DIV_ADDRESS));   \
} while (false)


#define GP_WB_CHECK_OFFSET_TIMERXL16(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_TIMERXL_0_TIMERXL16_0_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_1_TIMERXL16_0_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_2_TIMERXL16_0_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_3_TIMERXL16_0_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_0_TIMERXL16_1_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_1_TIMERXL16_1_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_2_TIMERXL16_1_PRESCALER_DIV_ADDRESS) ||  \
                        ((offset) == GP_WB_TIMERXL_3_TIMERXL16_1_PRESCALER_DIV_ADDRESS));   \
} while (false)


#define GP_WB_CHECK_OFFSET_TIMESTAMP(offset) do {                        \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_PWMS_TIMESTAMP0_ENABLE_ADDRESS) ||  \
                        ((offset) == GP_WB_PWMS_TIMESTAMP1_ENABLE_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_ABS_EV(offset) do { \
} while (false)

#define GP_WB_CHECK_OFFSET_QUIC_CHANNEL(offset) do { \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_QUIC_CHANNEL_0_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_1_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_2_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_3_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_4_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_5_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_6_CONF_ADDRESS) ||  \
                        ((offset) == GP_WB_QUIC_CHANNEL_7_CONF_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_GPIO_QUIC(offset) do { \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_0_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_1_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_2_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_3_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_4_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_5_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_6_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_TRIGGER_GPIO_7_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_0_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_1_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_2_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_3_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_4_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_5_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_6_ADDRESS) ||  \
                        ((offset) == GP_WB_GPIO_QUIC_ACTION_CHANNEL_7_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_DMA_SCB(offset) do {                             \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_DMAS_SCB_SRC_ADDR_0_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_1_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_2_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_3_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_4_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_5_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_6_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_7_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_8_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_9_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_10_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_11_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_12_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_13_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_14_ADDRESS) ||  \
                        ((offset) == GP_WB_DMAS_SCB_SRC_ADDR_15_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_AK_DMA_SCB(offset) do {                             \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_0_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_1_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_2_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_3_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_4_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_DMAS_SCB_SRC_ADDR_5_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_DMA_DCB(offset) do {                             \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_DMAS_DCB_SRC_ADDR_0_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_AK_DMA_DCB(offset) do {                             \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_AK_DMAS_DCB_SRC_ADDR_0_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_AK_SX_CHIDX(offset) do { \
    COMPILE_TIME_ASSERT(((offset) == GP_WB_AK_PLME_SX_CHIDX0_LF_PRELOAD_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_PLME_SX_CHIDX1_LF_PRELOAD_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_PLME_SX_CHIDX2_LF_PRELOAD_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_PLME_SX_CHIDX3_LF_PRELOAD_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_PLME_SX_CHIDX4_LF_PRELOAD_ADDRESS) ||  \
                        ((offset) == GP_WB_AK_PLME_SX_CHIDX5_LF_PRELOAD_ADDRESS));   \
} while (false)

#define GP_WB_CHECK_OFFSET_AK_RT_RCI_ENTRY(offset)

#define GP_WB_CHECK_OFFSET_AK_FORMAT_R(offset)

#define GP_WB_CHECK_OFFSET_CAL_NVM(offset)

#define GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset)

#define GP_WB_CHECK_OFFSET_SE_CAL_RAM(offset)

#define GP_WB_CHECK_OFFSET_PMU_CTRL(offset)

// ***************************************************
// ****  Some GPIOs helpers
// ***************************************************
static inline void write_gpio_field(const UInt32 gpio0_base_address, const UInt8 gpio_nr, const Bool value)
{
    UInt32 address = gpio0_base_address + (gpio_nr/32)*4;
    UInt32 val = GP_WB_READ_U32(address);
    if (value)
    {
        BIT_SET(val, gpio_nr%32);
    }
    else
    {
        BIT_CLR(val, gpio_nr%32);
    }
    GP_WB_WRITE_U32(address, val);
}

static inline Bool read_gpio_field(const UInt32 gpio0_base_address, const UInt8 gpio_nr)
{
    UInt32 address = gpio0_base_address + (gpio_nr/32)*4;
    UInt32 val = GP_WB_READ_U32(address);
    return BIT_TST(val, gpio_nr%32);
}

//input value, read only
#define READ_GPIO_INPUT_VALUE(gpio_nr) read_gpio_field(GP_WB_GPIO_GPIO_0_INPUT_VALUE_ADDRESS, gpio_nr)

//direction, read/write
#define  READ_GPIO_DIRECTION(gpio_nr)       read_gpio_field(GP_WB_GPIO_GPIO_0_DIRECTION_ADDRESS, gpio_nr)
#define WRITE_GPIO_DIRECTION(gpio_nr, val) write_gpio_field(GP_WB_GPIO_GPIO_0_DIRECTION_ADDRESS, gpio_nr, val)

//output value, read/write
#define  READ_GPIO_OUTPUT_VALUE(gpio_nr)       read_gpio_field(GP_WB_GPIO_GPIO_0_OUTPUT_VALUE_ADDRESS, gpio_nr)
#define WRITE_GPIO_OUTPUT_VALUE(gpio_nr, val) write_gpio_field(GP_WB_GPIO_GPIO_0_OUTPUT_VALUE_ADDRESS, gpio_nr, val)

//interrupt level, read/write
#define  READ_GPIO_INT_LEVEL(gpio_nr)       read_gpio_field(GP_WB_GPIO_GPIO_0_INTERRUPT_LEVEL_ADDRESS, gpio_nr)
#define WRITE_GPIO_INT_LEVEL(gpio_nr, val) write_gpio_field(GP_WB_GPIO_GPIO_0_INTERRUPT_LEVEL_ADDRESS, gpio_nr, val)

//interrupt type, read/write
#define  READ_GPIO_INT_TYPE(gpio_nr)       read_gpio_field(GP_WB_GPIO_GPIO_0_INTERRUPT_TYPE_ADDRESS, gpio_nr)
#define WRITE_GPIO_INT_TYPE(gpio_nr, val) write_gpio_field(GP_WB_GPIO_GPIO_0_INTERRUPT_TYPE_ADDRESS, gpio_nr, val)

//interrupt clr is One-Shot register, so it can be written directly
#define CLR_GPIO_INT(gpio_nr) GP_WB_WRITE_U32((GP_WB_GPIO_CLR_GPIO_0_INTERRUPT_ADDRESS + (gpio_nr/32)*4), (1 << (gpio_nr%32)))

//interrupt set is One-Shot register, so it can be written directly
#define SET_GPIO_INT(gpio_nr) GP_WB_WRITE_U32((GP_WB_GPIO_SET_GPIO_0_INTERRUPT_ADDRESS + (gpio_nr/32)*4), (1 << (gpio_nr%32)))

//unmasked interrupt, read only
#define READ_UNMASKED_GPIO_INT(gpio_nr) read_gpio_field(GP_WB_GPIO_UNMASKED_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)

//input buffer enable, read/write
#define  READ_GPIO_INPUT_BUFFER_ENABLE(gpio_nr)       read_gpio_field(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_ADDRESS, gpio_nr)
#define WRITE_GPIO_INPUT_BUFFER_ENABLE(gpio_nr, val) write_gpio_field(GP_WB_IOB_GPIO_INPUT_BUFFER_ENABLE_ADDRESS, gpio_nr, val)

//input buffer enable, read/write
#define  READ_GPIO_PULL_ENABLE(gpio_nr)       read_gpio_field(GP_WB_IOB_GPIO_PULL_ENABLE_ADDRESS, gpio_nr)
#define WRITE_GPIO_PULL_ENABLE(gpio_nr, val) write_gpio_field(GP_WB_IOB_GPIO_PULL_ENABLE_ADDRESS, gpio_nr, val)

//input buffer enable, read/write
#define  READ_GPIO_PULL_MODE(gpio_nr)       read_gpio_field(GP_WB_IOB_GPIO_PULL_MODE_ADDRESS, gpio_nr)
#define WRITE_GPIO_PULL_MODE(gpio_nr, val) write_gpio_field(GP_WB_IOB_GPIO_PULL_MODE_ADDRESS, gpio_nr, val)

// ***************************************************
// ****  Some GPIOs interrupt controller helpers
// ***************************************************
//mask is read/write
#define WRITE_APPUC_GPIO_0_INT_MASK(gpio_nr, val) write_gpio_field(GP_WB_INT_CTRL_MASK_APPUC_GPIO_0_GPIO_0_INTERRUPT_ADDRESS, gpio_nr, val)
#define  READ_APPUC_GPIO_0_INT_MASK(gpio_nr)       read_gpio_field(GP_WB_INT_CTRL_MASK_APPUC_GPIO_0_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)
//masked is read only
#define READ_APPUC_GPIO_0_INT_MASKED(gpio_nr)      read_gpio_field(GP_WB_INT_CTRL_MASKED_APPUC_GPIO_0_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)
//mask is read/write
#define WRITE_APPUC_GPIO_1_INT_MASK(gpio_nr, val) write_gpio_field(GP_WB_INT_CTRL_MASK_APPUC_GPIO_1_GPIO_0_INTERRUPT_ADDRESS, gpio_nr, val)
#define  READ_APPUC_GPIO_1_INT_MASK(gpio_nr)       read_gpio_field(GP_WB_INT_CTRL_MASK_APPUC_GPIO_1_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)
//masked is read only
#define READ_APPUC_GPIO_1_INT_MASKED(gpio_nr)      read_gpio_field(GP_WB_INT_CTRL_MASKED_APPUC_GPIO_1_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)


//mask is read/write
#define WRITE_SEUC_GPIO_INT_MASK(gpio_nr, val) write_gpio_field(GP_WB_INT_CTRL_SEC_MASK_SEUC_GPIO_GPIO_0_INTERRUPT_ADDRESS, gpio_nr, val)
#define  READ_SEUC_GPIO_INT_MASK(gpio_nr)       read_gpio_field(GP_WB_INT_CTRL_SEC_MASK_SEUC_GPIO_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)
//masked is read only
#define READ_SEUC_GPIO_INT_MASKED(gpio_nr)      read_gpio_field(GP_WB_INT_CTRL_SEC_MASKED_SEUC_GPIO_GPIO_0_INTERRUPT_ADDRESS, gpio_nr)


// ***************************************************
// ****  Some Alternate helpers
// ***************************************************
static inline void disable_gpio_alternate(const UInt8 gpio_nr)
{
    //each gpio is 4 bytes aligned
    UInt32 address = GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_ADDRESS + gpio_nr*4;

    //first read value in there
    UInt32 val = GP_WB_READ_U32(address);

    //clear enable and write
    BIT_CLR(val, GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_LSB);
    GP_WB_WRITE_U32(address, val);
}

static inline void enable_gpio_alternate(const UInt8 gpio_nr, const UInt8 alternate_function)
{
    //each gpio is 4 bytes aligned
    UInt32 address = GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_ADDRESS + gpio_nr*4;

    //first read value in there
    UInt32 val = GP_WB_READ_U32(address);

    //if alternate is enabled, disable it
    if (val & GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_MASK)
    {
        BIT_CLR(val, GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_LSB);
        GP_WB_WRITE_U32(address, val);
    }

    //now assign val with the alternate function and write it
    val = alternate_function << GP_WB_IOB_GPIO_0_ALTERNATE_LSB;
    GP_WB_WRITE_U32(address, val);

    //now enable
    BIT_SET(val, GP_WB_IOB_GPIO_0_ALTERNATE_ENABLE_LSB);
    GP_WB_WRITE_U32(address, val);
}


// ***************************************************
// ****  Some ASIC Debug helpers
// ***************************************************
static inline void enable_debug(const UInt8 debug_nr, const UInt8 debug_function)
{
    //each debug is 4 bytes aligned
    UInt32 address = GP_WB_DEBUG_DEBUG_SEL_0_ADDRESS + debug_nr*4;
    GP_WB_WRITE_U32(address, debug_function);
}


// ***************************************************
// ****  Some QUIC helpers
// ***************************************************
#define WRITE_QUIC_CONF(quic_nr, conf)       GP_WB_WRITE_U16((GP_WB_QUIC_CHANNEL_0_CONF_ADDRESS      + quic_nr*4), (conf))
#define WRITE_PWMXL_QUIC_CONF(quic_nr, conf) GP_WB_WRITE_U16((GP_WB_PWMXLS_QUIC_ACTION_CFG_0_ADDRESS + quic_nr*4), (conf))

// ***************************************************
// ****  Some field helpers
// ***************************************************

// works for eaxample in combination with the qbuild decorate_c_field.py
#define FIELD_TYPE_UNSIGNED 0
#define FIELD_TYPE_SIGNED   1
typedef struct {
    UInt32 addr;
    UInt8  len;
    UInt8  lsb;
    UInt16 type;
} FIELD_T;

// if addr is zero, the field is considered None
ALWAYS_INLINE Bool FIELD_IS_NONE(const FIELD_T *field)
{
    return field->addr == 0;
}

// field is a pointer to a field!
ALWAYS_INLINE UInt32 GP_WB_READ_FIELD(const FIELD_T *field)
{
    UInt32 mask = ((1 << (field->len)) - 1) << (field->lsb);
    UInt32 val = (GP_WB_READ_U32(field->addr) & mask) >> (field->lsb);
    if (field->type == FIELD_TYPE_SIGNED)
        return GP_WB_S32(val, field->len);
    return val;
}

ALWAYS_INLINE void GP_WB_WRITE_FIELD(const FIELD_T *field, UInt32 val)
{
    // GP_ASSERT(field->addr >= GP_MM_CMA_NVM_LINEAR_START); // 0x10000000
    UInt32 mask = ((1 << (field->len)) - 1) << (field->lsb);
    GP_WB_MWRITE_U32(field->addr, mask, val << (field->lsb));
}

// Macro that simplifies the way a FIELD_T struct can be created
#define GET_FIELD(reg) {   \
    GP_WB_##reg##_ADDRESS, \
    GP_WB_##reg##_NBITS,   \
    GP_WB_##reg##_LSB,     \
    GP_WB_##reg##_SIGNED   \
}

#define GET_FIELD_PTR(reg) &(FIELD_T){GP_WB_##reg##_ADDRESS, GP_WB_##reg##_NBITS, GP_WB_##reg##_LSB, GP_WB_##reg##_SIGNED}

#endif  /* __COMMON_H__ */
