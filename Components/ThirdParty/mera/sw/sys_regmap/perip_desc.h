#ifndef PERIP_DESC_H
#define PERIP_DESC_H

    #define NR_UARTS 3

    #define NR_RTZS              2
    #define NR_RTZS_QUIC_ACTIONS 1

    #define NR_DMA_SCB_CHANNELS 16

    #define NR_DMA_DCB_CHANNELS     1
    #define NR_DMA_DCB_QUIC_ACTIONS 3

    #define NR_TIMERS             4
    #define BITS_TIMERS_PRESCALER 12
    #define BITS_TIMERS_TIMER     16

    #define NR_LEDS              4
    #define NR_LEDS_QUIC_ACTIONS 2

    #define NR_PWMS           4
    #define NR_PWM_TIMESTAMPS 2

    #define NR_PWMXLS              8
    #define NR_PWMXLS_QUIC_ACTIONS 8

    #define NR_TIMERXLS             6
    #define NR_TIMERXL_COMPARES     4
    #define NR_TIMERXL_QUIC_ACTIONS 4

    #define NR_KEYPAD_SCANS        8
    #define NR_KEYPAD_SENSES       8
    #define NR_KEYPAD_QUIC_ACTIONS 1

    #define NR_ASPS 2

    #define NR_CMA_NVM_INFO_USER_READ_LOCKS 4
    #define NR_CMA_NVM_MAIN_READ_LOCKS      32
    #define NR_CMA_EOTP_ENABLE_BITS         16
    #define BYTES_CMA_EOTP_SLOT_SIZE        512
    #define CMA_ROM_PRESENT                 1
    #define NR_CMA_ROM_READ_LOCKS           32

    #define NR_IR_QUIC_ACTIONS 3

    #define NR_I2SS 2

    #define NR_I2C_MS 2

    #define NR_IPC_CHANNELS 8
    #define NR_IPC_MUTEXES  8

    #define NR_GPIO_TIMESTAMPS   2
    #define NR_GPIO_QUIC_ACTIONS 16

    #define NR_QUIC_CHANNELS 16

    #define NR_HR_ADC_QUIC_ACTIONS 2

    #define NR_GP_ADC_QUIC_ACTIONS 2

    #define NR_BIST_QUIC_ACTIONS 3

    #define NR_ES_EVENTS 16

#endif // PERIP_DESC_H
