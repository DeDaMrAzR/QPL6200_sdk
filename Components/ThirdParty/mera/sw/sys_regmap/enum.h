#ifndef GP_WB_ENUM_H
#define GP_WB_ENUM_H

/***************************
 * typemap: general_enum
 ***************************/
/* gpio_interrupt_type  */
#define GP_WB_ENUM_GPIO_INTERRUPT_TYPE_EDGE_DETECT    0x0
#define GP_WB_ENUM_GPIO_INTERRUPT_TYPE_LEVEL          0x1

/* parity  */
#define GP_WB_ENUM_PARITY_EVEN    0x0
#define GP_WB_ENUM_PARITY_ODD     0x1
#define GP_WB_ENUM_PARITY_OFF     0x2

/* watchdog_function  */
#define GP_WB_ENUM_WATCHDOG_FUNCTION_IDLE             0x0
#define GP_WB_ENUM_WATCHDOG_FUNCTION_GEN_INTERRUPT    0x1
#define GP_WB_ENUM_WATCHDOG_FUNCTION_SOFT_POR         0x2
#define GP_WB_ENUM_WATCHDOG_FUNCTION_RESET_APPUC      0x3

/* nvm_size  */
#define GP_WB_ENUM_NVM_SIZE_KB2048    0x0
#define GP_WB_ENUM_NVM_SIZE_KB1792    0x1
#define GP_WB_ENUM_NVM_SIZE_KB1536    0x2
#define GP_WB_ENUM_NVM_SIZE_KB1280    0x3
#define GP_WB_ENUM_NVM_SIZE_KB1024    0x4
#define GP_WB_ENUM_NVM_SIZE_KB768     0x5
#define GP_WB_ENUM_NVM_SIZE_KB512     0x6
#define GP_WB_ENUM_NVM_SIZE_KB256     0x7

/* serial_itf_select  */
#define GP_WB_ENUM_SERIAL_ITF_SELECT_USE_SPI         0x0
#define GP_WB_ENUM_SERIAL_ITF_SELECT_USE_I2C         0x1
#define GP_WB_ENUM_SERIAL_ITF_SELECT_USE_UART        0x2
#define GP_WB_ENUM_SERIAL_ITF_SELECT_NO_INTERFACE    0x3

/* qspi_cmd_mode  */
#define GP_WB_ENUM_QSPI_CMD_MODE_SINGLE    0x0
#define GP_WB_ENUM_QSPI_CMD_MODE_DUAL      0x1
#define GP_WB_ENUM_QSPI_CMD_MODE_QUAD      0x2

/* qspi_cmd_type  */
#define GP_WB_ENUM_QSPI_CMD_TYPE_RX    0x0
#define GP_WB_ENUM_QSPI_CMD_TYPE_TX    0x1

/* qspi_io_default  */
#define GP_WB_ENUM_QSPI_IO_DEFAULT_DRIVE0      0x0
#define GP_WB_ENUM_QSPI_IO_DEFAULT_DRIVE1      0x1
#define GP_WB_ENUM_QSPI_IO_DEFAULT_LAST        0x2
#define GP_WB_ENUM_QSPI_IO_DEFAULT_UNDRIVEN    0x3

/* qspi_sample_mode  */
#define GP_WB_ENUM_QSPI_SAMPLE_MODE_EARLY    0x0
#define GP_WB_ENUM_QSPI_SAMPLE_MODE_MID_1    0x1
#define GP_WB_ENUM_QSPI_SAMPLE_MODE_MID_2    0x2
#define GP_WB_ENUM_QSPI_SAMPLE_MODE_LATE     0x3

/* keypad_action  */
#define GP_WB_ENUM_KEYPAD_ACTION_NONE       0x0
#define GP_WB_ENUM_KEYPAD_ACTION_DO_SCAN    0x1

/* dma_dcb_action  */
#define GP_WB_ENUM_DMA_DCB_ACTION_NONE              0x0
#define GP_WB_ENUM_DMA_DCB_ACTION_RESET_SRC_PTR     0x1
#define GP_WB_ENUM_DMA_DCB_ACTION_RESET_DEST_PTR    0x2
#define GP_WB_ENUM_DMA_DCB_ACTION_RESET_BOTH_PTR    0x3

/* rtz_action  */
#define GP_WB_ENUM_RTZ_ACTION_NONE                0x0
#define GP_WB_ENUM_RTZ_ACTION_TRIGGER_SEQUENCE    0x1

/* rtz_buffer_mode  */
#define GP_WB_ENUM_RTZ_BUFFER_MODE_DATA8BIT     0x0
#define GP_WB_ENUM_RTZ_BUFFER_MODE_DATA16BIT    0x1
#define GP_WB_ENUM_RTZ_BUFFER_MODE_DATA32BIT    0x2

/* pwmxl_count_mode  */
#define GP_WB_ENUM_PWMXL_COUNT_MODE_PRESCALED    0x0
#define GP_WB_ENUM_PWMXL_COUNT_MODE_EXTERNAL     0x1

/* pwmxl_action  */
#define GP_WB_ENUM_PWMXL_ACTION_NONE                    0x0
#define GP_WB_ENUM_PWMXL_ACTION_INCREMENT_MAIN_COUNT    0x1
#define GP_WB_ENUM_PWMXL_ACTION_START_WIDTH_COUNT       0x2
#define GP_WB_ENUM_PWMXL_ACTION_LATCH_PHASE             0x3
#define GP_WB_ENUM_PWMXL_ACTION_INCREMENT_WIDTH         0x4
#define GP_WB_ENUM_PWMXL_ACTION_DECREMENT_WIDTH         0x5

/* pwmxl_phase_trigger_mode  */
#define GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_PHASE_MATCH                   0x0
#define GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_EXTERNAL                      0x1
#define GP_WB_ENUM_PWMXL_PHASE_TRIGGER_MODE_EXTERNAL_AFTER_PHASE_MATCH    0x2

/* pwmxl_config_value_type  */
#define GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_PHASE       0x0
#define GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_WIDTH       0x1
#define GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_DITHER      0x2
#define GP_WB_ENUM_PWMXL_CONFIG_VALUE_TYPE_TOP_WRAP    0x3

/* timerxl_count_mode  */
#define GP_WB_ENUM_TIMERXL_COUNT_MODE_PRESCALED    0x0
#define GP_WB_ENUM_TIMERXL_COUNT_MODE_EXTERNAL     0x1

/* timerxl_operating_mode  */
#define GP_WB_ENUM_TIMERXL_OPERATING_MODE_DISABLED         0x0
#define GP_WB_ENUM_TIMERXL_OPERATING_MODE_SINGLE_32BITS    0x1
#define GP_WB_ENUM_TIMERXL_OPERATING_MODE_DUAL_16BITS      0x2

/* timerxl_action  */
#define GP_WB_ENUM_TIMERXL_ACTION_NONE               0x0
#define GP_WB_ENUM_TIMERXL_ACTION_INCREMENT_TIMER    0x1
#define GP_WB_ENUM_TIMERXL_ACTION_START_TIMER        0x2
#define GP_WB_ENUM_TIMERXL_ACTION_STOP_TIMER         0x3
#define GP_WB_ENUM_TIMERXL_ACTION_TOGGLE_TIMER       0x4
#define GP_WB_ENUM_TIMERXL_ACTION_RESET_TIMER        0x5
#define GP_WB_ENUM_TIMERXL_ACTION_PRESET_TIMER       0x6
#define GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_0          0x7
#define GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_1          0x8
#define GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_2          0x9
#define GP_WB_ENUM_TIMERXL_ACTION_CAPTURE_3          0xA

/* cp_threshold voltage threshold for the CP (Charge Pump) comparator */
#define GP_WB_ENUM_CP_THRESHOLD_VAL_2_4V    0x0
#define GP_WB_ENUM_CP_THRESHOLD_VAL_2_6V    0x1
#define GP_WB_ENUM_CP_THRESHOLD_VAL_2_8V    0x2
#define GP_WB_ENUM_CP_THRESHOLD_VAL_3_0V    0x3

/* cp_switch_wait_time CP (Charge Pump) delay time between enabling CP until enabling switch */
#define GP_WB_ENUM_CP_SWITCH_WAIT_TIME_VAL_1_5US    0x0
#define GP_WB_ENUM_CP_SWITCH_WAIT_TIME_VAL_3US      0x1
#define GP_WB_ENUM_CP_SWITCH_WAIT_TIME_VAL_5US      0x2
#define GP_WB_ENUM_CP_SWITCH_WAIT_TIME_VAL_7_5US    0x3

/* cp_comp_out_wait_time CP (Charge Pump) delay time to consider the output of comp_out valid again after changing the voltage threshold */
#define GP_WB_ENUM_CP_COMP_OUT_WAIT_TIME_VAL_0US      0x0
#define GP_WB_ENUM_CP_COMP_OUT_WAIT_TIME_VAL_1US      0x1
#define GP_WB_ENUM_CP_COMP_OUT_WAIT_TIME_VAL_2US      0x2
#define GP_WB_ENUM_CP_COMP_OUT_WAIT_TIME_VAL_3_5US    0x3

/* bist_action  */
#define GP_WB_ENUM_BIST_ACTION_NONE                0x0
/* Start a single Temperature Sensor measurement */
#define GP_WB_ENUM_BIST_ACTION_TSENS_DO_SINGLE     0x1
/* Start Temperature Sensor continuous measurement */
#define GP_WB_ENUM_BIST_ACTION_TSENS_CONT_START    0x2
/* Stop Temperature Sensor continuous measurement */
#define GP_WB_ENUM_BIST_ACTION_TSENS_CONT_STOP     0x3
/* Start BIST ADC measurement - same as firing bist_adc_start */
#define GP_WB_ENUM_BIST_ACTION_ADC_START           0x4
/* Start BIST ADC continuous measurement */
#define GP_WB_ENUM_BIST_ACTION_ADC_CONT_START      0x5
/* Stop BIST ADC continuous measurement */
#define GP_WB_ENUM_BIST_ACTION_ADC_CONT_STOP       0x6

/* led_src_clk_freq_sel  */
#define GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M32    0x0
#define GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M8     0x1
#define GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_M2     0x2
#define GP_WB_ENUM_LED_SRC_CLK_FREQ_SEL_K62    0x3

/* led_action  */
#define GP_WB_ENUM_LED_ACTION_NONE      0x0
#define GP_WB_ENUM_LED_ACTION_SET       0x1
#define GP_WB_ENUM_LED_ACTION_CLR       0x2
#define GP_WB_ENUM_LED_ACTION_TOGGLE    0x3

/* adc_trigger_mode  */
/* the adc is never triggered or triggered by a quic channel */
#define GP_WB_ENUM_ADC_TRIGGER_MODE_NEVER_OR_QUIC    0x0
/* adc keep re-triggering continuously */
#define GP_WB_ENUM_ADC_TRIGGER_MODE_CONTINUOUS       0x1

/* gp_adc_channel  */
#define GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_0    0x0
#define GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_1    0x1
#define GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_2    0x2
#define GP_WB_ENUM_GP_ADC_CHANNEL_ANIO_3    0x3

/* hr_adc_channel  */
#define GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_0    0x0
#define GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_1    0x1
#define GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_2    0x2
#define GP_WB_ENUM_HR_ADC_CHANNEL_ANIO_3    0x3

/* lpcomp_channel  */
#define GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_0    0x0
#define GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_1    0x1
#define GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_2    0x2
#define GP_WB_ENUM_LPCOMP_CHANNEL_ANIO_3    0x3

/* adc_buffer_update_mode  */
#define GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_NORMAL          0x0
#define GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_MIN_HOLD        0x1
#define GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_MAX_HOLD        0x2
#define GP_WB_ENUM_ADC_BUFFER_UPDATE_MODE_OUT_OF_RANGE    0x3

/* gp_adc_action  */
#define GP_WB_ENUM_GP_ADC_ACTION_NONE              0x0
#define GP_WB_ENUM_GP_ADC_ACTION_DO_MEASUREMENT    0x1

/* hr_adc_action  */
#define GP_WB_ENUM_HR_ADC_ACTION_NONE                     0x0
#define GP_WB_ENUM_HR_ADC_ACTION_DO_SINGLE_MEASUREMENT    0x1
#define GP_WB_ENUM_HR_ADC_ACTION_START_ACCU               0x2

/* i2s_clk_sel  */
#define GP_WB_ENUM_I2S_CLK_SEL_MAIN_CLK              0x0
#define GP_WB_ENUM_I2S_CLK_SEL_FRACT_CLK_CLKGEN_0    0x1
#define GP_WB_ENUM_I2S_CLK_SEL_FRACT_CLK_CLKGEN_1    0x2
#define GP_WB_ENUM_I2S_CLK_SEL_IOB_CLK               0x3

/* asp_clk_sel  */
#define GP_WB_ENUM_ASP_CLK_SEL_CLK_2M                0x0
#define GP_WB_ENUM_ASP_CLK_SEL_FRACT_CLK_CLKGEN_0    0x1
#define GP_WB_ENUM_ASP_CLK_SEL_FRACT_CLK_CLKGEN_1    0x2

/* asp_datastream_mode  */
#define GP_WB_ENUM_ASP_DATASTREAM_MODE_OFF        0x0
#define GP_WB_ENUM_ASP_DATASTREAM_MODE_RISING     0x1
#define GP_WB_ENUM_ASP_DATASTREAM_MODE_FALLING    0x2
/* The first sample will always be rising edge */
#define GP_WB_ENUM_ASP_DATASTREAM_MODE_BOTH       0x3

/* ir_modulation_mode  */
#define GP_WB_ENUM_IR_MODULATION_MODE_MODULATION_OFF    0x0
#define GP_WB_ENUM_IR_MODULATION_MODE_REGISTER_BASED    0x1
#define GP_WB_ENUM_IR_MODULATION_MODE_PATTERN_BASED     0x2
#define GP_WB_ENUM_IR_MODULATION_MODE_TIME_BASED        0x3
#define GP_WB_ENUM_IR_MODULATION_MODE_EXT_BASED         0x4

/* ir_time_unit  */
#define GP_WB_ENUM_IR_TIME_UNIT_TU_500NS    0x0
#define GP_WB_ENUM_IR_TIME_UNIT_TU_1US      0x1
#define GP_WB_ENUM_IR_TIME_UNIT_TU_2US      0x2
#define GP_WB_ENUM_IR_TIME_UNIT_TU_4US      0x3

/* ir_action  */
#define GP_WB_ENUM_IR_ACTION_NONE                   0x0
#define GP_WB_ENUM_IR_ACTION_START_RAM_SEQUENCER    0x1
#define GP_WB_ENUM_IR_ACTION_ALT_CARRIER_SET        0x2
#define GP_WB_ENUM_IR_ACTION_ALT_CARRIER_UNSET      0x3

/* dma_dcb_word_mode  */
#define GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA8BIT     0x0
#define GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA16BIT    0x1
#define GP_WB_ENUM_DMA_DCB_WORD_MODE_DATA32BIT    0x2

/* generic_io_drive  */
#define GP_WB_ENUM_GENERIC_IO_DRIVE_PUSH_PULL     0x0
#define GP_WB_ENUM_GENERIC_IO_DRIVE_OPEN_DRAIN    0x1

/* clock_speed  */
#define GP_WB_ENUM_CLOCK_SPEED_M32     0x0
#define GP_WB_ENUM_CLOCK_SPEED_M64     0x1
#define GP_WB_ENUM_CLOCK_SPEED_M96     0x2
#define GP_WB_ENUM_CLOCK_SPEED_M192    0x3

/* sys_main_clock_source  */
#define GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF64M_CLOCK     0x0
#define GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF192M_CLOCK    0x1

/* obm_src  */
#define GP_WB_ENUM_OBM_SRC_OFF                 0x0
#define GP_WB_ENUM_OBM_SRC_RC64K_CLOCK         0x1
#define GP_WB_ENUM_OBM_SRC_XT32K_CLOCK         0x2
#define GP_WB_ENUM_OBM_SRC_DTC_CLOCK           0x3
#define GP_WB_ENUM_OBM_SRC_XT32M_CLOCK         0x4
#define GP_WB_ENUM_OBM_SRC_BIST_RINGOSC        0x5
#define GP_WB_ENUM_OBM_SRC_BIST_XO32M_CLOCK    0x6
#define GP_WB_ENUM_OBM_SRC_EXTERNAL            0x7
#define GP_WB_ENUM_OBM_SRC_BIST_RCMEAS         0x8
#define GP_WB_ENUM_OBM_SRC_RC64M_CLOCK         0x9

/* obm_state  */
#define GP_WB_ENUM_OBM_STATE_IDLE                    0x0
#define GP_WB_ENUM_OBM_STATE_RESYNC_INIT0            0x1
#define GP_WB_ENUM_OBM_STATE_RESYNC_INIT1            0x2
#define GP_WB_ENUM_OBM_STATE_RESYNC_INIT2            0x3
#define GP_WB_ENUM_OBM_STATE_SEARCHING_FIRST_EDGE    0x4
#define GP_WB_ENUM_OBM_STATE_MEASURING               0x5

/* por_reason  */
#define GP_WB_ENUM_POR_REASON_HW_POR                          0x0
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_REGMAP              0x1
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_ISO_TX              0x2
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_ES                  0x3
#define GP_WB_ENUM_POR_REASON_POR_BY_VDDB_CUTOFF              0x4
#define GP_WB_ENUM_POR_REASON_POR_BY_VDDDIG_NOK               0x5
#define GP_WB_ENUM_POR_REASON_POR_BY_OVD                      0x6
#define GP_WB_ENUM_POR_REASON_SOFT_POR_RAM_MW_INVALID         0x7
#define GP_WB_ENUM_POR_REASON_SOFT_POR_RAM_CRC_INVALID        0x8
#define GP_WB_ENUM_POR_REASON_SOFT_POR_RRAM_BL_CRC_INVALID    0x9
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BOOTLOADER             0xA
#define GP_WB_ENUM_POR_REASON_SOFT_POR_PRESERVE_DBG_ITF       0xB
#define GP_WB_ENUM_POR_REASON_SOFT_POR_DEBUG_MODE             0xC
#define GP_WB_ENUM_POR_REASON_SOFT_POR_DIAGNOSTIC_MODE        0xD
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_WATCHDOG            0xE
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_BBPLL               0xF
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_TAMPER              0x10
#define GP_WB_ENUM_POR_REASON_SOFT_POR_BY_TAMPER_OTP          0x11

/* wkup_reason  */
#define GP_WB_ENUM_WKUP_REASON_POR                0x0
#define GP_WB_ENUM_WKUP_REASON_TIMER_EVENT        0x1
#define GP_WB_ENUM_WKUP_REASON_LPCOMP_EVENT       0x2
#define GP_WB_ENUM_WKUP_REASON_GPIO_EVENT         0x3
#define GP_WB_ENUM_WKUP_REASON_DCDC_EVENT         0x4
#define GP_WB_ENUM_WKUP_REASON_IMMEDIATE_EVENT    0x5
#define GP_WB_ENUM_WKUP_REASON_LPR_OVERLOAD       0x6
#define GP_WB_ENUM_WKUP_REASON_OVD_EVENT          0x7

/* rtc_timer_src  */
/* this is the 64 kHz RC clock divided down to 32 khz */
#define GP_WB_ENUM_RTC_TIMER_SRC_LFRC     0x0
/* This is the 32 kHz Xtal osc */
#define GP_WB_ENUM_RTC_TIMER_SRC_XT32K    0x1
/* this is the 32MHz xtal clock divided down to 32 KHz */
#define GP_WB_ENUM_RTC_TIMER_SRC_XT32M    0x2

/* wakeup_pin_mode  */
#define GP_WB_ENUM_WAKEUP_PIN_MODE_NO_EDGE         0x0
#define GP_WB_ENUM_WAKEUP_PIN_MODE_RISING_EDGE     0x1
#define GP_WB_ENUM_WAKEUP_PIN_MODE_FALLING_EDGE    0x2
#define GP_WB_ENUM_WAKEUP_PIN_MODE_BOTH_EDGES      0x3

/* ssp_mode  */
#define GP_WB_ENUM_SSP_MODE_ENCRYPT    0x0
#define GP_WB_ENUM_SSP_MODE_DECRYPT    0x1
#define GP_WB_ENUM_SSP_MODE_AES        0x2

/* ssp_key_len  */
#define GP_WB_ENUM_SSP_KEY_LEN_KEY_128    0x0
#define GP_WB_ENUM_SSP_KEY_LEN_KEY_192    0x1
#define GP_WB_ENUM_SSP_KEY_LEN_KEY_256    0x2

/* event_state  */
#define GP_WB_ENUM_EVENT_STATE_INVALID                              0x0
#define GP_WB_ENUM_EVENT_STATE_SCHEDULED                            0x1
#define GP_WB_ENUM_EVENT_STATE_SCHEDULED_FOR_IMMEDIATE_EXECUTION    0x2
#define GP_WB_ENUM_EVENT_STATE_RESCHEDULED                          0x3
#define GP_WB_ENUM_EVENT_STATE_DONE                                 0x4

/* event_result  */
#define GP_WB_ENUM_EVENT_RESULT_UNKNOWN              0x0
#define GP_WB_ENUM_EVENT_RESULT_EXECUTED_ON_TIME     0x1
#define GP_WB_ENUM_EVENT_RESULT_EXECUTED_TOO_LATE    0x2
#define GP_WB_ENUM_EVENT_RESULT_MISSED_TOO_LATE      0x3

/* standby_power_mode  */
/* This is the LPS power mode, it uses the low power regulator and switches off the digital core */
#define GP_WB_ENUM_STANDBY_POWER_MODE_LPS         0x0
/* This is the LPS power mode, with DCDC enabled it uses the DCDCregulator and switches off the digital core */
#define GP_WB_ENUM_STANDBY_POWER_MODE_ELPS        0x1
/* In this mode the digital is not switched off, and the higher power regulator is used */
#define GP_WB_ENUM_STANDBY_POWER_MODE_LPS_RET     0x2
/* In this mode the digital is not switched off, and the DCDC convertor is used */
#define GP_WB_ENUM_STANDBY_POWER_MODE_ELPS_RET    0x3
/* In this mode, both analog as digital supplies are on, in order to support the 32MHz based sleep clock */
#define GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET     0x4

/* active_power_mode  */
/* This is the Medium Power Mode = uses 64MHz RC osc, analog voltages are off */
#define GP_WB_ENUM_ACTIVE_POWER_MODE_MPS     0x0
/* This is te Medium Power Mode with DCDC convertor enabled */
#define GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS    0x1
/* This is the High power Mode, uses the 192MHz BBL clock, locked to the xo32m and has analog voltages enabled */
#define GP_WB_ENUM_ACTIVE_POWER_MODE_HPS     0x2
/* This is the High Power Mode with DCDC convertor enabled */
#define GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS    0x3

/* tsens_hanning_cycles  */
#define GP_WB_ENUM_TSENS_HANNING_CYCLES_N_32     0x0
#define GP_WB_ENUM_TSENS_HANNING_CYCLES_N_64     0x1
#define GP_WB_ENUM_TSENS_HANNING_CYCLES_N_128    0x2

/* hr_adc_hanning_cycles  */
#define GP_WB_ENUM_HR_ADC_HANNING_CYCLES_N_32     0x0
#define GP_WB_ENUM_HR_ADC_HANNING_CYCLES_N_64     0x1
#define GP_WB_ENUM_HR_ADC_HANNING_CYCLES_N_128    0x2
#define GP_WB_ENUM_HR_ADC_HANNING_CYCLES_N_256    0x3

/* key_select  */
#define GP_WB_ENUM_KEY_SELECT_REGMAP_KEY    0x0
#define GP_WB_ENUM_KEY_SELECT_KRTL          0x1

/* power_state  */
/* eSecure is powered up, but idle */
#define GP_WB_ENUM_POWER_STATE_IDLE         0x0
/* eSecure is powered up and active */
#define GP_WB_ENUM_POWER_STATE_ACTIVE       0x1
/* eSecure is powered down (low-power state) */
#define GP_WB_ENUM_POWER_STATE_POWERDOWN    0x2

/* dcdc_rx_mode  */
#define GP_WB_ENUM_DCDC_RX_MODE_DISABLED       0x0
#define GP_WB_ENUM_DCDC_RX_MODE_FORCED         0x1
#define GP_WB_ENUM_DCDC_RX_MODE_AUTO_PHY_RX    0x2
#define GP_WB_ENUM_DCDC_RX_MODE_AUTO_RX_ON     0x3

/* dcdc_tx_mode  */
#define GP_WB_ENUM_DCDC_TX_MODE_DISABLED       0x0
#define GP_WB_ENUM_DCDC_TX_MODE_FORCED         0x1
#define GP_WB_ENUM_DCDC_TX_MODE_AUTO_PHY_TX    0x2
#define GP_WB_ENUM_DCDC_TX_MODE_AUTO_TX_ON     0x3
#define GP_WB_ENUM_DCDC_TX_MODE_AUTO_PA_ON     0x4

/***************************
 * typemap: iob_enum
 ***************************/
/* gpio_pull_mode  */
#define GP_WB_ENUM_GPIO_PULL_MODE_PULL_DOWN    0x0
#define GP_WB_ENUM_GPIO_PULL_MODE_PULL_UP      0x1

/* gpio_slew_rate  */
#define GP_WB_ENUM_GPIO_SLEW_RATE_FAST    0x0
#define GP_WB_ENUM_GPIO_SLEW_RATE_SLOW    0x1

/* gpio_drive_strength  */
/* 2mA of gpio drive strength */
#define GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_2MA     0x0
/* 4mA of gpio drive strength */
#define GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_4MA     0x1
/* 8mA of gpio drive strength */
#define GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_8MA     0x2
/* 12mA of gpio drive strength */
#define GP_WB_ENUM_GPIO_DRIVE_STRENGTH_MAX_12MA    0x3

/* gpio_0_alternates  */
#define GP_WB_ENUM_GPIO_0_ALTERNATES_CSSYS_TRACE_CLK                     0x0
#define GP_WB_ENUM_GPIO_0_ALTERNATES_QSPI_M_SCLK                         0x1
#define GP_WB_ENUM_GPIO_0_ALTERNATES_ASP_1_CLK                           0x2
#define GP_WB_ENUM_GPIO_0_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1    0x3
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_COEX_MS_GRANTB                0x4
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_COEX_SL_REQ                   0x5
#define GP_WB_ENUM_GPIO_0_ALTERNATES_ASP_0_DATA                          0x6
#define GP_WB_ENUM_GPIO_0_ALTERNATES_IR_DOUT                             0x7
#define GP_WB_ENUM_GPIO_0_ALTERNATES_EXT_INT_EXT_INT                     0x8
#define GP_WB_ENUM_GPIO_0_ALTERNATES_ANALOG_IRLED                        0x9
#define GP_WB_ENUM_GPIO_0_ALTERNATES_LED_LED_0                           0xA
#define GP_WB_ENUM_GPIO_0_ALTERNATES_SPI_M_MOSI                          0xB
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_PHY_MODE_3                    0xC
#define GP_WB_ENUM_GPIO_0_ALTERNATES_PWMXL_PWMXL_0                       0xD
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_RTUC_SWDIO                    0xF
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_RTUC_UART_TX                  0x10
#define GP_WB_ENUM_GPIO_0_ALTERNATES_UART_1_RX                           0x11
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_RX_ADC0_CLK                   0x12
#define GP_WB_ENUM_GPIO_0_ALTERNATES_AKUMA_RX_ADC1_CLK                   0x13
#define GP_WB_ENUM_GPIO_0_ALTERNATES_I2C_M_0_SDA                         0x14
#define GP_WB_ENUM_GPIO_0_ALTERNATES_I2C_SL_SDA                          0x15
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_0_ALTERNATES_I2S_0_SDO                           0x16
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_0_ALTERNATES_KEYPAD_SENSES_7                     0x17
#define GP_WB_ENUM_GPIO_0_ALTERNATES_SECURE_ELEMENT_SEUC_SWDIO           0x18
#define GP_WB_ENUM_GPIO_0_ALTERNATES_RTZ_1_TX                            0x19
#define GP_WB_ENUM_GPIO_0_ALTERNATES_UART_0_TX                           0x1A
#define GP_WB_ENUM_GPIO_0_ALTERNATES_UART_2_TX                           0x1B
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_0_ALTERNATES_I2S_0_WS_IN                         0x1C
#endif //['itwosperipheral']

/* gpio_1_alternates  */
#define GP_WB_ENUM_GPIO_1_ALTERNATES_CSSYS_TRACE_DATA_0           0x0
#define GP_WB_ENUM_GPIO_1_ALTERNATES_CSSYS_SWDIO_TMS              0x1
#define GP_WB_ENUM_GPIO_1_ALTERNATES_QSPI_M_IO_0                  0x2
#define GP_WB_ENUM_GPIO_1_ALTERNATES_ASP_0_CLK                    0x3
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_COEX_MS_REQB           0x4
#define GP_WB_ENUM_GPIO_1_ALTERNATES_ASP_1_DATA                   0x5
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_DBG_3                  0x6
#define GP_WB_ENUM_GPIO_1_ALTERNATES_DEBUG_DEBUG_3                0x7
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_GPIO_3                 0x8
#define GP_WB_ENUM_GPIO_1_ALTERNATES_LED_LED_1                    0x9
#define GP_WB_ENUM_GPIO_1_ALTERNATES_SPI_M_MISO                   0xA
#define GP_WB_ENUM_GPIO_1_ALTERNATES_SPI_SL_MISO                  0xB
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_PHY_MODE_0             0xC
#define GP_WB_ENUM_GPIO_1_ALTERNATES_PWMXL_PWMXL_1                0xD
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2S_0_REF_IN                 0xF
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2S_1_REF_IN                 0x10
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_RTUC_SWCLK             0x11
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_RTUC_UART_RX           0x12
#define GP_WB_ENUM_GPIO_1_ALTERNATES_UART_0_RX                    0x13
#define GP_WB_ENUM_GPIO_1_ALTERNATES_UART_2_RX                    0x14
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_RX_ADC0_OUT_2          0x15
#define GP_WB_ENUM_GPIO_1_ALTERNATES_AKUMA_RX_ADC1_OUT_2          0x16
#define GP_WB_ENUM_GPIO_1_ALTERNATES_KEYPAD_SCANS_0               0x17
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2C_M_0_SCL                  0x18
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2C_SL_SCL                   0x19
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2S_0_SDI                    0x1A
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_1_ALTERNATES_I2S_1_SDI                    0x1B
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_1_ALTERNATES_KEYPAD_SENSES_0              0x1C
#define GP_WB_ENUM_GPIO_1_ALTERNATES_SECURE_ELEMENT_SEUC_SWCLK    0x1D
#define GP_WB_ENUM_GPIO_1_ALTERNATES_CMA_TMS                      0x1E
#define GP_WB_ENUM_GPIO_1_ALTERNATES_UART_1_TX                    0x1F

/* gpio_2_alternates  */
#define GP_WB_ENUM_GPIO_2_ALTERNATES_CSSYS_TRACE_DATA_1           0x0
#define GP_WB_ENUM_GPIO_2_ALTERNATES_CSSYS_SWCLK_TCK              0x1
#define GP_WB_ENUM_GPIO_2_ALTERNATES_QSPI_M_IO_2                  0x2
#define GP_WB_ENUM_GPIO_2_ALTERNATES_ASP_1_CLK                    0x3
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_COEX_MS_PRIOB          0x4
#define GP_WB_ENUM_GPIO_2_ALTERNATES_UART_2_CTS                   0x5
#define GP_WB_ENUM_GPIO_2_ALTERNATES_ASP_0_DATA                   0x6
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_DBG_4                  0x7
#define GP_WB_ENUM_GPIO_2_ALTERNATES_DEBUG_DEBUG_4                0x8
#define GP_WB_ENUM_GPIO_2_ALTERNATES_USB_DP                       0x9
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_GPIO_4                 0xA
#define GP_WB_ENUM_GPIO_2_ALTERNATES_LED_LED_2                    0xB
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_PHY_MODE_1             0xC
#define GP_WB_ENUM_GPIO_2_ALTERNATES_PWMXL_PWMXL_2                0xD
#define GP_WB_ENUM_GPIO_2_ALTERNATES_UART_1_RX                    0xF
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_RX_ADC0_OUT_3          0x10
#define GP_WB_ENUM_GPIO_2_ALTERNATES_AKUMA_RX_ADC1_OUT_3          0x11
#define GP_WB_ENUM_GPIO_2_ALTERNATES_KEYPAD_SCANS_1               0x12
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2C_M_0_SDA                  0x13
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2C_SL_SDA                   0x14
#define GP_WB_ENUM_GPIO_2_ALTERNATES_KEYPAD_SENSES_1              0x15
#define GP_WB_ENUM_GPIO_2_ALTERNATES_SECURE_ELEMENT_SEUC_SWDIO    0x16
#define GP_WB_ENUM_GPIO_2_ALTERNATES_SPI_M_SSN                    0x17
#define GP_WB_ENUM_GPIO_2_ALTERNATES_SPI_SL_SSN                   0x18
#define GP_WB_ENUM_GPIO_2_ALTERNATES_CMA_TCK                      0x19
#define GP_WB_ENUM_GPIO_2_ALTERNATES_UART_0_TX                    0x1A
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2S_0_WS                     0x1B
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2S_1_WS                     0x1C
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2S_0_WS_IN                  0x1D
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_2_ALTERNATES_I2S_1_WS_IN                  0x1E
#endif //['itwosperipheral']

/* gpio_3_alternates  */
#define GP_WB_ENUM_GPIO_3_ALTERNATES_CSSYS_TRACE_DATA_2           0x0
#define GP_WB_ENUM_GPIO_3_ALTERNATES_CSSYS_TDI                    0x1
#define GP_WB_ENUM_GPIO_3_ALTERNATES_QSPI_M_IO_1                  0x2
#define GP_WB_ENUM_GPIO_3_ALTERNATES_ASP_0_CLK                    0x3
#define GP_WB_ENUM_GPIO_3_ALTERNATES_AKUMA_DBG_5                  0x4
#define GP_WB_ENUM_GPIO_3_ALTERNATES_DEBUG_DEBUG_5                0x5
#define GP_WB_ENUM_GPIO_3_ALTERNATES_USB_DM                       0x6
#define GP_WB_ENUM_GPIO_3_ALTERNATES_AKUMA_GPIO_5                 0x7
#define GP_WB_ENUM_GPIO_3_ALTERNATES_LED_LED_3                    0x8
#define GP_WB_ENUM_GPIO_3_ALTERNATES_PWMXL_PWMXL_3                0x9
#define GP_WB_ENUM_GPIO_3_ALTERNATES_UART_0_RX                    0xB
#define GP_WB_ENUM_GPIO_3_ALTERNATES_AKUMA_RX_ADC0_OUT_4          0xC
#define GP_WB_ENUM_GPIO_3_ALTERNATES_AKUMA_RX_ADC1_OUT_4          0xD
#define GP_WB_ENUM_GPIO_3_ALTERNATES_KEYPAD_SCANS_2               0xE
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2S_0_SCK                    0xF
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2S_1_SCK                    0x10
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2S_0_SCK_IN                 0x11
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2S_1_SCK_IN                 0x12
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2C_M_0_SCL                  0x13
#define GP_WB_ENUM_GPIO_3_ALTERNATES_I2C_SL_SCL                   0x14
#define GP_WB_ENUM_GPIO_3_ALTERNATES_SPI_M_SCLK                   0x15
#define GP_WB_ENUM_GPIO_3_ALTERNATES_SPI_SL_SCLK                  0x16
#define GP_WB_ENUM_GPIO_3_ALTERNATES_KEYPAD_SENSES_2              0x17
#define GP_WB_ENUM_GPIO_3_ALTERNATES_SECURE_ELEMENT_SEUC_SWCLK    0x18
#define GP_WB_ENUM_GPIO_3_ALTERNATES_CMA_TDI                      0x19
#define GP_WB_ENUM_GPIO_3_ALTERNATES_UART_1_TX                    0x1A

/* gpio_4_alternates  */
#define GP_WB_ENUM_GPIO_4_ALTERNATES_CSSYS_TRACE_DATA_3                  0x0
#define GP_WB_ENUM_GPIO_4_ALTERNATES_CSSYS_SWV_TDO                       0x1
#define GP_WB_ENUM_GPIO_4_ALTERNATES_QSPI_M_IO_3                         0x2
#define GP_WB_ENUM_GPIO_4_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1    0x3
#define GP_WB_ENUM_GPIO_4_ALTERNATES_AKUMA_COEX_MS_REQA                  0x4
#define GP_WB_ENUM_GPIO_4_ALTERNATES_AKUMA_COEX_SL_REQ_NOT               0x5
#define GP_WB_ENUM_GPIO_4_ALTERNATES_UART_0_CTS                          0x6
#define GP_WB_ENUM_GPIO_4_ALTERNATES_UART_1_CTS                          0x7
#define GP_WB_ENUM_GPIO_4_ALTERNATES_ASP_0_DATA                          0x8
#define GP_WB_ENUM_GPIO_4_ALTERNATES_IR_DOUT                             0x9
#define GP_WB_ENUM_GPIO_4_ALTERNATES_SPI_M_MOSI                          0xA
#define GP_WB_ENUM_GPIO_4_ALTERNATES_SPI_SL_MOSI                         0xB
#define GP_WB_ENUM_GPIO_4_ALTERNATES_BIST_OBM_OUT                        0xC
#define GP_WB_ENUM_GPIO_4_ALTERNATES_AKUMA_PHY_ANTSW_2                   0xD
#define GP_WB_ENUM_GPIO_4_ALTERNATES_PWMXL_PWMXL_6                       0xE
#define GP_WB_ENUM_GPIO_4_ALTERNATES_AKUMA_RX_ADC0_OUT_5                 0x10
#define GP_WB_ENUM_GPIO_4_ALTERNATES_AKUMA_RX_ADC1_OUT_5                 0x11
#define GP_WB_ENUM_GPIO_4_ALTERNATES_KEYPAD_SCANS_3                      0x12
#define GP_WB_ENUM_GPIO_4_ALTERNATES_I2C_M_0_SDA                         0x13
#define GP_WB_ENUM_GPIO_4_ALTERNATES_I2C_SL_SDA                          0x14
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_4_ALTERNATES_I2S_0_SDO                           0x15
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_4_ALTERNATES_I2S_1_SDO                           0x16
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_4_ALTERNATES_KEYPAD_SENSES_3                     0x17
#define GP_WB_ENUM_GPIO_4_ALTERNATES_CMA_TDO                             0x18

/* gpio_5_alternates  */
#define GP_WB_ENUM_GPIO_5_ALTERNATES_CSSYS_TRACE_CLK                     0x0
#define GP_WB_ENUM_GPIO_5_ALTERNATES_QSPI_M_SCLK                         0x1
#define GP_WB_ENUM_GPIO_5_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x2
#define GP_WB_ENUM_GPIO_5_ALTERNATES_AKUMA_COEX_MS_PRIOA                 0x3
#define GP_WB_ENUM_GPIO_5_ALTERNATES_AKUMA_COEX_SL_PRIO_0                0x4
#define GP_WB_ENUM_GPIO_5_ALTERNATES_SPI_M_MISO                          0x5
#define GP_WB_ENUM_GPIO_5_ALTERNATES_SPI_SL_MISO                         0x6
#define GP_WB_ENUM_GPIO_5_ALTERNATES_PWMXL_PWMXL_7                       0x7
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_5_ALTERNATES_I2S_0_REF_IN                        0x9
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_5_ALTERNATES_AKUMA_RX_ADC0_OUT_6                 0xA
#define GP_WB_ENUM_GPIO_5_ALTERNATES_AKUMA_RX_ADC1_OUT_6                 0xB
#define GP_WB_ENUM_GPIO_5_ALTERNATES_KEYPAD_SCANS_4                      0xC
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_5_ALTERNATES_I2S_0_SDI                           0xD
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_5_ALTERNATES_KEYPAD_SENSES_4                     0xE

/* gpio_6_alternates  */
#define GP_WB_ENUM_GPIO_6_ALTERNATES_CSSYS_SWDIO_TMS              0x0
#define GP_WB_ENUM_GPIO_6_ALTERNATES_QSPI_M_IO_0                  0x1
#define GP_WB_ENUM_GPIO_6_ALTERNATES_AKUMA_PHY_CHANNEL_0          0x2
#define GP_WB_ENUM_GPIO_6_ALTERNATES_AKUMA_PHY_MODE_0             0x3
#define GP_WB_ENUM_GPIO_6_ALTERNATES_PWMXL_PWMXL_0                0x4
#define GP_WB_ENUM_GPIO_6_ALTERNATES_AKUMA_RTUC_SWDIO             0x6
#define GP_WB_ENUM_GPIO_6_ALTERNATES_AKUMA_RX_ADC1_OUT_0          0x7
#define GP_WB_ENUM_GPIO_6_ALTERNATES_KEYPAD_SCANS_5               0x8
#define GP_WB_ENUM_GPIO_6_ALTERNATES_KEYPAD_SENSES_5              0x9
#define GP_WB_ENUM_GPIO_6_ALTERNATES_SECURE_ELEMENT_SEUC_SWDIO    0xA
#define GP_WB_ENUM_GPIO_6_ALTERNATES_SPI_M_SSN                    0xB
#define GP_WB_ENUM_GPIO_6_ALTERNATES_SPI_SL_SSN                   0xC
#define GP_WB_ENUM_GPIO_6_ALTERNATES_CMA_TMS                      0xD
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_6_ALTERNATES_I2S_0_WS                     0xE
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_6_ALTERNATES_I2S_0_WS_IN                  0xF
#endif //['itwosperipheral']

/* gpio_7_alternates  */
#define GP_WB_ENUM_GPIO_7_ALTERNATES_CSSYS_SWCLK_TCK              0x0
#define GP_WB_ENUM_GPIO_7_ALTERNATES_QSPI_M_IO_2                  0x1
#define GP_WB_ENUM_GPIO_7_ALTERNATES_AKUMA_PHY_CHANNEL_1          0x2
#define GP_WB_ENUM_GPIO_7_ALTERNATES_AKUMA_PHY_MODE_1             0x3
#define GP_WB_ENUM_GPIO_7_ALTERNATES_PWMXL_PWMXL_1                0x4
#define GP_WB_ENUM_GPIO_7_ALTERNATES_AKUMA_RTUC_SWCLK             0x6
#define GP_WB_ENUM_GPIO_7_ALTERNATES_AKUMA_RX_ADC1_OUT_1          0x7
#define GP_WB_ENUM_GPIO_7_ALTERNATES_KEYPAD_SCANS_6               0x8
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_7_ALTERNATES_I2S_0_SCK                    0x9
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_7_ALTERNATES_I2S_0_SCK_IN                 0xA
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_7_ALTERNATES_SPI_M_SCLK                   0xB
#define GP_WB_ENUM_GPIO_7_ALTERNATES_SPI_SL_SCLK                  0xC
#define GP_WB_ENUM_GPIO_7_ALTERNATES_KEYPAD_SENSES_6              0xD
#define GP_WB_ENUM_GPIO_7_ALTERNATES_SECURE_ELEMENT_SEUC_SWCLK    0xE
#define GP_WB_ENUM_GPIO_7_ALTERNATES_CMA_TCK                      0xF

/* gpio_8_alternates  */
#define GP_WB_ENUM_GPIO_8_ALTERNATES_CSSYS_TDI              0x0
#define GP_WB_ENUM_GPIO_8_ALTERNATES_QSPI_M_IO_1            0x1
#define GP_WB_ENUM_GPIO_8_ALTERNATES_SPI_M_MOSI             0x2
#define GP_WB_ENUM_GPIO_8_ALTERNATES_SPI_SL_MOSI            0x3
#define GP_WB_ENUM_GPIO_8_ALTERNATES_AKUMA_PHY_CHANNEL_2    0x4
#define GP_WB_ENUM_GPIO_8_ALTERNATES_AKUMA_PHY_MODE_2       0x5
#define GP_WB_ENUM_GPIO_8_ALTERNATES_PWMXL_PWMXL_2          0x6
#define GP_WB_ENUM_GPIO_8_ALTERNATES_UART_1_RX              0x8
#define GP_WB_ENUM_GPIO_8_ALTERNATES_AKUMA_RX_ADC1_OUT_2    0x9
#define GP_WB_ENUM_GPIO_8_ALTERNATES_KEYPAD_SCANS_7         0xA
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_8_ALTERNATES_I2S_0_SDO              0xB
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_8_ALTERNATES_KEYPAD_SENSES_7        0xC
#define GP_WB_ENUM_GPIO_8_ALTERNATES_CMA_TDI                0xD
#define GP_WB_ENUM_GPIO_8_ALTERNATES_RTZ_0_TX               0xE
#define GP_WB_ENUM_GPIO_8_ALTERNATES_UART_0_TX              0xF

/* gpio_9_alternates  */
#define GP_WB_ENUM_GPIO_9_ALTERNATES_CSSYS_SWV_TDO          0x0
#define GP_WB_ENUM_GPIO_9_ALTERNATES_QSPI_M_IO_3            0x1
#define GP_WB_ENUM_GPIO_9_ALTERNATES_LED_LED_3              0x2
#define GP_WB_ENUM_GPIO_9_ALTERNATES_SPI_M_MISO             0x3
#define GP_WB_ENUM_GPIO_9_ALTERNATES_SPI_SL_MISO            0x4
#define GP_WB_ENUM_GPIO_9_ALTERNATES_AKUMA_PHY_ANTSW_2      0x5
#define GP_WB_ENUM_GPIO_9_ALTERNATES_AKUMA_PHY_CHANNEL_3    0x6
#define GP_WB_ENUM_GPIO_9_ALTERNATES_AKUMA_PHY_MODE_3       0x7
#define GP_WB_ENUM_GPIO_9_ALTERNATES_PWMXL_PWMXL_3          0x8
#define GP_WB_ENUM_GPIO_9_ALTERNATES_UART_0_RX              0xA
#define GP_WB_ENUM_GPIO_9_ALTERNATES_AKUMA_RX_ADC1_OUT_3    0xB
#define GP_WB_ENUM_GPIO_9_ALTERNATES_KEYPAD_SCANS_0         0xC
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_9_ALTERNATES_I2S_0_SDI              0xD
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_9_ALTERNATES_KEYPAD_SENSES_0        0xE
#define GP_WB_ENUM_GPIO_9_ALTERNATES_CMA_TDO                0xF
#define GP_WB_ENUM_GPIO_9_ALTERNATES_RTZ_1_TX               0x10
#define GP_WB_ENUM_GPIO_9_ALTERNATES_UART_1_TX              0x11

/* gpio_10_alternates  */
#define GP_WB_ENUM_GPIO_10_ALTERNATES_QSPI_M_SCLK                         0x0
#define GP_WB_ENUM_GPIO_10_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x1
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_DBG_0                         0x2
#define GP_WB_ENUM_GPIO_10_ALTERNATES_DEBUG_DEBUG_0                       0x3
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_GPIO_0                        0x4
#define GP_WB_ENUM_GPIO_10_ALTERNATES_LED_LED_2                           0x5
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_PHY_ANTSW_3                   0x6
#define GP_WB_ENUM_GPIO_10_ALTERNATES_PWMXL_PWMXL_4                       0x7
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_RTUC_UART_TX                  0x9
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_RX_ADC0_OUT_7                 0xA
#define GP_WB_ENUM_GPIO_10_ALTERNATES_AKUMA_RX_ADC1_OUT_7                 0xB
#define GP_WB_ENUM_GPIO_10_ALTERNATES_KEYPAD_SCANS_1                      0xC
#define GP_WB_ENUM_GPIO_10_ALTERNATES_I2C_M_1_SDA                         0xD
#define GP_WB_ENUM_GPIO_10_ALTERNATES_KEYPAD_SENSES_1                     0xE
#define GP_WB_ENUM_GPIO_10_ALTERNATES_SPI_M_SSN                           0xF
#define GP_WB_ENUM_GPIO_10_ALTERNATES_SPI_SL_SSN                          0x10
#define GP_WB_ENUM_GPIO_10_ALTERNATES_RTZ_0_TX                            0x11
#define GP_WB_ENUM_GPIO_10_ALTERNATES_UART_0_TX                           0x12
#define GP_WB_ENUM_GPIO_10_ALTERNATES_UART_1_TX                           0x13
#define GP_WB_ENUM_GPIO_10_ALTERNATES_UART_2_TX                           0x14
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_10_ALTERNATES_I2S_0_WS                            0x15
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_10_ALTERNATES_I2S_0_WS_IN                         0x16
#endif //['itwosperipheral']

/* gpio_11_alternates  */
#define GP_WB_ENUM_GPIO_11_ALTERNATES_CSSYS_TRACE_CLK                     0x0
#define GP_WB_ENUM_GPIO_11_ALTERNATES_QSPI_M_IO_0                         0x1
#define GP_WB_ENUM_GPIO_11_ALTERNATES_ASP_1_CLK                           0x2
#define GP_WB_ENUM_GPIO_11_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1    0x3
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_COEX_MS_GRANTB                0x4
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_COEX_SL_PRIO_0                0x5
#define GP_WB_ENUM_GPIO_11_ALTERNATES_ASP_0_DATA                          0x6
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_DBG_1                         0x7
#define GP_WB_ENUM_GPIO_11_ALTERNATES_DEBUG_DEBUG_1                       0x8
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_GPIO_1                        0x9
#define GP_WB_ENUM_GPIO_11_ALTERNATES_LED_LED_1                           0xA
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_PHY_ANTSW_0                   0xB
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_PHY_MODE_0                    0xC
#define GP_WB_ENUM_GPIO_11_ALTERNATES_PWMXL_PWMXL_5                       0xD
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_RTUC_UART_RX                  0xF
#define GP_WB_ENUM_GPIO_11_ALTERNATES_UART_0_RX                           0x10
#define GP_WB_ENUM_GPIO_11_ALTERNATES_UART_1_RX                           0x11
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_RX_ADC0_OUT_8                 0x12
#define GP_WB_ENUM_GPIO_11_ALTERNATES_AKUMA_RX_ADC1_OUT_8                 0x13
#define GP_WB_ENUM_GPIO_11_ALTERNATES_KEYPAD_SCANS_2                      0x14
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_11_ALTERNATES_I2S_0_SCK                           0x15
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_11_ALTERNATES_I2S_0_SCK_IN                        0x16
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_11_ALTERNATES_I2C_M_0_SCL                         0x17
#define GP_WB_ENUM_GPIO_11_ALTERNATES_I2C_M_1_SCL                         0x18
#define GP_WB_ENUM_GPIO_11_ALTERNATES_SPI_M_SCLK                          0x19
#define GP_WB_ENUM_GPIO_11_ALTERNATES_SPI_SL_SCLK                         0x1A
#define GP_WB_ENUM_GPIO_11_ALTERNATES_KEYPAD_SENSES_2                     0x1B
#define GP_WB_ENUM_GPIO_11_ALTERNATES_RTZ_1_TX                            0x1C

/* gpio_12_alternates  */
#define GP_WB_ENUM_GPIO_12_ALTERNATES_CSSYS_TRACE_DATA_0                  0x0
#define GP_WB_ENUM_GPIO_12_ALTERNATES_CSSYS_SWDIO_TMS                     0x1
#define GP_WB_ENUM_GPIO_12_ALTERNATES_QSPI_M_IO_2                         0x2
#define GP_WB_ENUM_GPIO_12_ALTERNATES_ASP_0_CLK                           0x3
#define GP_WB_ENUM_GPIO_12_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x4
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_COEX_MS_GRANTA                0x5
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_COEX_SL_REQ_NOT               0x6
#define GP_WB_ENUM_GPIO_12_ALTERNATES_UART_0_CTS                          0x7
#define GP_WB_ENUM_GPIO_12_ALTERNATES_ASP_1_DATA                          0x8
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_DBG_2                         0x9
#define GP_WB_ENUM_GPIO_12_ALTERNATES_DEBUG_DEBUG_2                       0xA
#define GP_WB_ENUM_GPIO_12_ALTERNATES_IR_DOUT                             0xB
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_GPIO_2                        0xC
#define GP_WB_ENUM_GPIO_12_ALTERNATES_LED_LED_0                           0xD
#define GP_WB_ENUM_GPIO_12_ALTERNATES_SPI_M_MOSI                          0xE
#define GP_WB_ENUM_GPIO_12_ALTERNATES_SPI_SL_MOSI                         0xF
#define GP_WB_ENUM_GPIO_12_ALTERNATES_BIST_OBM_OUT                        0x10
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_PHY_ANTSW_1                   0x11
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_PHY_MODE_1                    0x12
#define GP_WB_ENUM_GPIO_12_ALTERNATES_PWMXL_PWMXL_0                       0x13
#define GP_WB_ENUM_GPIO_12_ALTERNATES_UART_2_RX                           0x15
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_RX_ADC0_OUT_9                 0x16
#define GP_WB_ENUM_GPIO_12_ALTERNATES_AKUMA_RX_ADC1_OUT_9                 0x17
#define GP_WB_ENUM_GPIO_12_ALTERNATES_KEYPAD_SCANS_3                      0x18
#define GP_WB_ENUM_GPIO_12_ALTERNATES_I2C_M_0_SDA                         0x19
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_12_ALTERNATES_I2S_0_SDO                           0x1A
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_12_ALTERNATES_I2S_1_SDO                           0x1B
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_12_ALTERNATES_KEYPAD_SENSES_3                     0x1C
#define GP_WB_ENUM_GPIO_12_ALTERNATES_RTZ_0_TX                            0x1D
#define GP_WB_ENUM_GPIO_12_ALTERNATES_UART_1_TX                           0x1E

/* gpio_13_alternates  */
#define GP_WB_ENUM_GPIO_13_ALTERNATES_CSSYS_TRACE_DATA_1     0x0
#define GP_WB_ENUM_GPIO_13_ALTERNATES_CSSYS_SWCLK_TCK        0x1
#define GP_WB_ENUM_GPIO_13_ALTERNATES_QSPI_M_IO_1            0x2
#define GP_WB_ENUM_GPIO_13_ALTERNATES_AKUMA_GPIO_3           0x3
#define GP_WB_ENUM_GPIO_13_ALTERNATES_SPI_M_MISO             0x4
#define GP_WB_ENUM_GPIO_13_ALTERNATES_SPI_SL_MISO            0x5
#define GP_WB_ENUM_GPIO_13_ALTERNATES_AKUMA_PHY_MODE_2       0x6
#define GP_WB_ENUM_GPIO_13_ALTERNATES_PWMXL_PWMXL_1          0x7
#define GP_WB_ENUM_GPIO_13_ALTERNATES_AKUMA_RX_ADC1_OUT_4    0x8
#define GP_WB_ENUM_GPIO_13_ALTERNATES_KEYPAD_SCANS_4         0x9
#define GP_WB_ENUM_GPIO_13_ALTERNATES_I2C_M_0_SCL            0xA
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_13_ALTERNATES_I2S_0_SDI              0xB
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_13_ALTERNATES_I2S_1_SDI              0xC
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_13_ALTERNATES_KEYPAD_SENSES_4        0xD
#define GP_WB_ENUM_GPIO_13_ALTERNATES_RTZ_1_TX               0xE

/* gpio_14_alternates  */
#define GP_WB_ENUM_GPIO_14_ALTERNATES_CSSYS_TRACE_DATA_2     0x0
#define GP_WB_ENUM_GPIO_14_ALTERNATES_CSSYS_TDI              0x1
#define GP_WB_ENUM_GPIO_14_ALTERNATES_QSPI_M_IO_3            0x2
#define GP_WB_ENUM_GPIO_14_ALTERNATES_AKUMA_GPIO_4           0x3
#define GP_WB_ENUM_GPIO_14_ALTERNATES_AKUMA_PHY_MODE_3       0x4
#define GP_WB_ENUM_GPIO_14_ALTERNATES_PWMXL_PWMXL_2          0x5
#define GP_WB_ENUM_GPIO_14_ALTERNATES_AKUMA_RX_ADC1_OUT_5    0x6
#define GP_WB_ENUM_GPIO_14_ALTERNATES_KEYPAD_SCANS_5         0x7
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_14_ALTERNATES_I2S_0_SCK              0x8
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_14_ALTERNATES_I2S_1_SCK              0x9
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_14_ALTERNATES_I2S_0_SCK_IN           0xA
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_14_ALTERNATES_I2S_1_SCK_IN           0xB
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_14_ALTERNATES_I2C_M_0_SCL            0xC
#define GP_WB_ENUM_GPIO_14_ALTERNATES_SPI_M_SCLK             0xD
#define GP_WB_ENUM_GPIO_14_ALTERNATES_SPI_SL_SCLK            0xE
#define GP_WB_ENUM_GPIO_14_ALTERNATES_KEYPAD_SENSES_5        0xF
#define GP_WB_ENUM_GPIO_14_ALTERNATES_RTZ_0_TX               0x10

/* gpio_15_alternates  */
#define GP_WB_ENUM_GPIO_15_ALTERNATES_CSSYS_TRACE_DATA_3     0x0
#define GP_WB_ENUM_GPIO_15_ALTERNATES_CSSYS_SWV_TDO          0x1
#define GP_WB_ENUM_GPIO_15_ALTERNATES_QSPI_M_SCLK            0x2
#define GP_WB_ENUM_GPIO_15_ALTERNATES_AKUMA_GPIO_5           0x3
#define GP_WB_ENUM_GPIO_15_ALTERNATES_AKUMA_PHY_ANTSW_3      0x4
#define GP_WB_ENUM_GPIO_15_ALTERNATES_PWMXL_PWMXL_3          0x5
#define GP_WB_ENUM_GPIO_15_ALTERNATES_AKUMA_RX_ADC1_OUT_6    0x6
#define GP_WB_ENUM_GPIO_15_ALTERNATES_KEYPAD_SCANS_6         0x7
#define GP_WB_ENUM_GPIO_15_ALTERNATES_I2C_M_0_SDA            0x8
#define GP_WB_ENUM_GPIO_15_ALTERNATES_KEYPAD_SENSES_6        0x9
#define GP_WB_ENUM_GPIO_15_ALTERNATES_SPI_M_SSN              0xA
#define GP_WB_ENUM_GPIO_15_ALTERNATES_SPI_SL_SSN             0xB
#define GP_WB_ENUM_GPIO_15_ALTERNATES_RTZ_1_TX               0xD
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_15_ALTERNATES_I2S_0_WS               0xE
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_15_ALTERNATES_I2S_1_WS               0xF
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_15_ALTERNATES_I2S_0_WS_IN            0x10
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_15_ALTERNATES_I2S_1_WS_IN            0x11
#endif //['itwosperipheral']

/* gpio_16_alternates  */
#define GP_WB_ENUM_GPIO_16_ALTERNATES_ASP_1_CLK                0x0
#define GP_WB_ENUM_GPIO_16_ALTERNATES_AKUMA_COEX_ATT_CTRL_0    0x1
#define GP_WB_ENUM_GPIO_16_ALTERNATES_AKUMA_COEX_MS_GRANTA     0x2
#define GP_WB_ENUM_GPIO_16_ALTERNATES_AKUMA_COEX_SL_PRIO_0     0x3
#define GP_WB_ENUM_GPIO_16_ALTERNATES_IR_DIN                   0x4
#define GP_WB_ENUM_GPIO_16_ALTERNATES_LED_LED_1                0x5
#define GP_WB_ENUM_GPIO_16_ALTERNATES_SPI_M_MOSI               0x6
#define GP_WB_ENUM_GPIO_16_ALTERNATES_AKUMA_PHY_ANTSW_0        0x7
#define GP_WB_ENUM_GPIO_16_ALTERNATES_PWMXL_PWMXL_6            0x8
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2S_0_REF_IN             0xA
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2S_1_REF_IN             0xB
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_16_ALTERNATES_UART_1_RX                0xC
#define GP_WB_ENUM_GPIO_16_ALTERNATES_AKUMA_RX_ADC1_OUT_7      0xD
#define GP_WB_ENUM_GPIO_16_ALTERNATES_KEYPAD_SCANS_7           0xE
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2C_M_0_SCL              0xF
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2C_M_1_SCL              0x10
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2C_SL_SCL               0x11
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2S_0_SDO                0x12
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_16_ALTERNATES_I2S_1_SDO                0x13
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_16_ALTERNATES_KEYPAD_SENSES_7          0x14
#define GP_WB_ENUM_GPIO_16_ALTERNATES_RTZ_0_TX                 0x15
#define GP_WB_ENUM_GPIO_16_ALTERNATES_UART_0_TX                0x16
#define GP_WB_ENUM_GPIO_16_ALTERNATES_UART_2_TX                0x17

/* gpio_17_alternates  */
#define GP_WB_ENUM_GPIO_17_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_1    0x0
#define GP_WB_ENUM_GPIO_17_ALTERNATES_AKUMA_COEX_ATT_CTRL_1               0x1
#define GP_WB_ENUM_GPIO_17_ALTERNATES_AKUMA_COEX_MS_GRANTB                0x2
#define GP_WB_ENUM_GPIO_17_ALTERNATES_AKUMA_COEX_SL_REQ_NOT               0x3
#define GP_WB_ENUM_GPIO_17_ALTERNATES_ASP_1_DATA                          0x4
#define GP_WB_ENUM_GPIO_17_ALTERNATES_IR_DOUT                             0x5
#define GP_WB_ENUM_GPIO_17_ALTERNATES_LED_LED_2                           0x6
#define GP_WB_ENUM_GPIO_17_ALTERNATES_AKUMA_PHY_ANTSW_1                   0x7
#define GP_WB_ENUM_GPIO_17_ALTERNATES_PWMXL_PWMXL_7                       0x8
#define GP_WB_ENUM_GPIO_17_ALTERNATES_UART_0_RX                           0xA
#define GP_WB_ENUM_GPIO_17_ALTERNATES_UART_2_RX                           0xB
#define GP_WB_ENUM_GPIO_17_ALTERNATES_AKUMA_RX_ADC1_OUT_8                 0xC
#define GP_WB_ENUM_GPIO_17_ALTERNATES_KEYPAD_SCANS_0                      0xD
#define GP_WB_ENUM_GPIO_17_ALTERNATES_I2C_M_0_SDA                         0xE
#define GP_WB_ENUM_GPIO_17_ALTERNATES_I2C_M_1_SDA                         0xF
#define GP_WB_ENUM_GPIO_17_ALTERNATES_I2C_SL_SDA                          0x10
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_17_ALTERNATES_I2S_0_SDI                           0x11
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_17_ALTERNATES_KEYPAD_SENSES_0                     0x12
#define GP_WB_ENUM_GPIO_17_ALTERNATES_RTZ_1_TX                            0x13
#define GP_WB_ENUM_GPIO_17_ALTERNATES_UART_1_TX                           0x14

/* gpio_18_alternates  */
#define GP_WB_ENUM_GPIO_18_ALTERNATES_ASP_1_CLK                           0x0
#define GP_WB_ENUM_GPIO_18_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x1
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_COEX_ATT_CTRL_2               0x2
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_COEX_MS_GRANTB                0x3
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_COEX_SL_GRANT                 0x4
#define GP_WB_ENUM_GPIO_18_ALTERNATES_UART_1_CTS                          0x5
#define GP_WB_ENUM_GPIO_18_ALTERNATES_IR_DOUT                             0x6
#define GP_WB_ENUM_GPIO_18_ALTERNATES_LED_LED_3                           0x7
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_PHY_ANTSW_2                   0x8
#define GP_WB_ENUM_GPIO_18_ALTERNATES_PWMXL_PWMXL_0                       0x9
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_RX_ADC0_OUT_1                 0xB
#define GP_WB_ENUM_GPIO_18_ALTERNATES_AKUMA_RX_ADC1_OUT_1                 0xC
#define GP_WB_ENUM_GPIO_18_ALTERNATES_KEYPAD_SCANS_1                      0xD
#define GP_WB_ENUM_GPIO_18_ALTERNATES_KEYPAD_SCANS_4                      0xE
#define GP_WB_ENUM_GPIO_18_ALTERNATES_I2C_M_0_SCL                         0xF
#define GP_WB_ENUM_GPIO_18_ALTERNATES_I2C_M_1_SCL                         0x10
#define GP_WB_ENUM_GPIO_18_ALTERNATES_I2C_SL_SCL                          0x11
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_18_ALTERNATES_I2S_0_SDI                           0x12
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_18_ALTERNATES_KEYPAD_SENSES_1                     0x13
#define GP_WB_ENUM_GPIO_18_ALTERNATES_RTZ_0_TX                            0x14
#define GP_WB_ENUM_GPIO_18_ALTERNATES_UART_0_TX                           0x15
#define GP_WB_ENUM_GPIO_18_ALTERNATES_UART_2_TX                           0x16

/* gpio_19_alternates  */
#define GP_WB_ENUM_GPIO_19_ALTERNATES_QSPI_M_IO_1             0x0
#define GP_WB_ENUM_GPIO_19_ALTERNATES_AKUMA_COEX_SL_PRIO_1    0x1
#define GP_WB_ENUM_GPIO_19_ALTERNATES_ANALOG_LXH              0x2
#define GP_WB_ENUM_GPIO_19_ALTERNATES_SPI_M_MOSI              0x3
#define GP_WB_ENUM_GPIO_19_ALTERNATES_AKUMA_PHY_ANTSW_2       0x4
#define GP_WB_ENUM_GPIO_19_ALTERNATES_AKUMA_PHY_MODE_2        0x5
#define GP_WB_ENUM_GPIO_19_ALTERNATES_PWMXL_PWMXL_5           0x6
#define GP_WB_ENUM_GPIO_19_ALTERNATES_UART_2_RX               0x7
#define GP_WB_ENUM_GPIO_19_ALTERNATES_AKUMA_RX_ADC0_OUT_0     0x8
#define GP_WB_ENUM_GPIO_19_ALTERNATES_AKUMA_RX_ADC1_OUT_0     0x9
#define GP_WB_ENUM_GPIO_19_ALTERNATES_I2C_M_0_SCL             0xA
#define GP_WB_ENUM_GPIO_19_ALTERNATES_I2C_SL_SCL              0xB
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_19_ALTERNATES_I2S_0_SDO               0xC
#endif //['itwosperipheral']

/* gpio_20_alternates  */
#define GP_WB_ENUM_GPIO_20_ALTERNATES_QSPI_M_IO_3            0x0
#define GP_WB_ENUM_GPIO_20_ALTERNATES_AKUMA_COEX_SL_REQ      0x1
#define GP_WB_ENUM_GPIO_20_ALTERNATES_ANALOG_LXL             0x2
#define GP_WB_ENUM_GPIO_20_ALTERNATES_SPI_M_MISO             0x3
#define GP_WB_ENUM_GPIO_20_ALTERNATES_AKUMA_PHY_ANTSW_3      0x4
#define GP_WB_ENUM_GPIO_20_ALTERNATES_PWMXL_PWMXL_0          0x5
#define GP_WB_ENUM_GPIO_20_ALTERNATES_AKUMA_RX_ADC0_OUT_1    0x6
#define GP_WB_ENUM_GPIO_20_ALTERNATES_AKUMA_RX_ADC1_OUT_1    0x7
#define GP_WB_ENUM_GPIO_20_ALTERNATES_I2C_M_0_SDA            0x8
#define GP_WB_ENUM_GPIO_20_ALTERNATES_I2C_SL_SDA             0x9
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_20_ALTERNATES_I2S_0_SDI              0xA
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_20_ALTERNATES_UART_2_TX              0xB

/* gpio_21_alternates  */
#define GP_WB_ENUM_GPIO_21_ALTERNATES_CSSYS_TRACE_DATA_0                  0x0
#define GP_WB_ENUM_GPIO_21_ALTERNATES_QSPI_M_IO_3                         0x1
#define GP_WB_ENUM_GPIO_21_ALTERNATES_ASP_0_CLK                           0x2
#define GP_WB_ENUM_GPIO_21_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x3
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_COEX_MS_GRANTA                0x4
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_COEX_SL_PRIO_1                0x5
#define GP_WB_ENUM_GPIO_21_ALTERNATES_UART_2_CTS                          0x6
#define GP_WB_ENUM_GPIO_21_ALTERNATES_IR_DOUT                             0x7
#define GP_WB_ENUM_GPIO_21_ALTERNATES_LED_LED_3                           0x8
#define GP_WB_ENUM_GPIO_21_ALTERNATES_ANALOG_MICBIAS                      0x9
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_PHY_CHANNEL_0                 0xA
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_PHY_MODE_2                    0xB
#define GP_WB_ENUM_GPIO_21_ALTERNATES_PWMXL_PWMXL_7                       0xC
#define GP_WB_ENUM_GPIO_21_ALTERNATES_UART_0_RX                           0xE
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_RX_ADC0_OUT_10                0xF
#define GP_WB_ENUM_GPIO_21_ALTERNATES_AKUMA_RX_ADC1_OUT_10                0x10
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_21_ALTERNATES_I2S_0_SCK                           0x11
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_21_ALTERNATES_I2C_M_0_SCL                         0x12
#define GP_WB_ENUM_GPIO_21_ALTERNATES_I2C_SL_SCL                          0x13
#define GP_WB_ENUM_GPIO_21_ALTERNATES_SPI_M_SCLK                          0x14
#define GP_WB_ENUM_GPIO_21_ALTERNATES_KEYPAD_SENSES_6                     0x15
#define GP_WB_ENUM_GPIO_21_ALTERNATES_RTZ_0_TX                            0x16
#define GP_WB_ENUM_GPIO_21_ALTERNATES_UART_1_TX                           0x17

/* gpio_22_alternates  */
#define GP_WB_ENUM_GPIO_22_ALTERNATES_CSSYS_TRACE_DATA_3                  0x0
#define GP_WB_ENUM_GPIO_22_ALTERNATES_QSPI_M_IO_0                         0x1
#define GP_WB_ENUM_GPIO_22_ALTERNATES_ANALOG_ANIO_2                       0x2
#define GP_WB_ENUM_GPIO_22_ALTERNATES_ASP_0_CLK                           0x3
#define GP_WB_ENUM_GPIO_22_ALTERNATES_FRACT_CLK_GEN_CLK_FRACT_CLKGEN_0    0x4
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_COEX_MS_GRANTA                0x5
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_COEX_SL_REQ                   0x6
#define GP_WB_ENUM_GPIO_22_ALTERNATES_UART_1_CTS                          0x7
#define GP_WB_ENUM_GPIO_22_ALTERNATES_IR_DOUT                             0x8
#define GP_WB_ENUM_GPIO_22_ALTERNATES_LED_LED_0                           0x9
#define GP_WB_ENUM_GPIO_22_ALTERNATES_SPI_M_MOSI                          0xA
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_PHY_CHANNEL_1                 0xB
#define GP_WB_ENUM_GPIO_22_ALTERNATES_PWMXL_PWMXL_2                       0xC
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_RX_ADC0_EOC                   0xE
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_RX_ADC1_EOC                   0xF
#define GP_WB_ENUM_GPIO_22_ALTERNATES_AKUMA_RX_ADC1_OUT_9                 0x10
#define GP_WB_ENUM_GPIO_22_ALTERNATES_I2C_M_0_SCL                         0x11
#define GP_WB_ENUM_GPIO_22_ALTERNATES_I2C_SL_SCL                          0x12
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_22_ALTERNATES_I2S_0_SDO                           0x13
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_22_ALTERNATES_KEYPAD_SENSES_3                     0x14
#define GP_WB_ENUM_GPIO_22_ALTERNATES_RTZ_1_TX                            0x15
#define GP_WB_ENUM_GPIO_22_ALTERNATES_UART_0_TX                           0x16
#define GP_WB_ENUM_GPIO_22_ALTERNATES_UART_2_TX                           0x17
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_22_ALTERNATES_I2S_0_WS_IN                         0x18
#endif //['itwosperipheral']

/* gpio_23_alternates  */
#define GP_WB_ENUM_GPIO_23_ALTERNATES_CSSYS_TRACE_DATA_2       0x0
#define GP_WB_ENUM_GPIO_23_ALTERNATES_QSPI_M_IO_2              0x1
#define GP_WB_ENUM_GPIO_23_ALTERNATES_ANALOG_ANIO_3            0x2
#define GP_WB_ENUM_GPIO_23_ALTERNATES_ASP_0_CLK                0x3
#define GP_WB_ENUM_GPIO_23_ALTERNATES_ASP_1_CLK                0x4
#define GP_WB_ENUM_GPIO_23_ALTERNATES_AKUMA_COEX_MS_GRANTB     0x5
#define GP_WB_ENUM_GPIO_23_ALTERNATES_AKUMA_COEX_SL_REQ_NOT    0x6
#define GP_WB_ENUM_GPIO_23_ALTERNATES_IR_DOUT                  0x7
#define GP_WB_ENUM_GPIO_23_ALTERNATES_LED_LED_1                0x8
#define GP_WB_ENUM_GPIO_23_ALTERNATES_SPI_M_MISO               0x9
#define GP_WB_ENUM_GPIO_23_ALTERNATES_AKUMA_PHY_CHANNEL_2      0xA
#define GP_WB_ENUM_GPIO_23_ALTERNATES_PWMXL_PWMXL_3            0xB
#define GP_WB_ENUM_GPIO_23_ALTERNATES_UART_0_RX                0xD
#define GP_WB_ENUM_GPIO_23_ALTERNATES_UART_2_RX                0xE
#define GP_WB_ENUM_GPIO_23_ALTERNATES_AKUMA_RX_ADC1_OUT_10     0xF
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_23_ALTERNATES_I2S_0_SCK_IN             0x10
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_23_ALTERNATES_I2C_M_0_SDA              0x11
#define GP_WB_ENUM_GPIO_23_ALTERNATES_I2C_SL_SDA               0x12
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_23_ALTERNATES_I2S_0_SDI                0x13
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_23_ALTERNATES_KEYPAD_SENSES_4          0x14
#define GP_WB_ENUM_GPIO_23_ALTERNATES_RTZ_0_TX                 0x15
#define GP_WB_ENUM_GPIO_23_ALTERNATES_UART_1_TX                0x16

/* gpio_24_alternates  */
#define GP_WB_ENUM_GPIO_24_ALTERNATES_CSSYS_TRACE_DATA_1      0x0
#define GP_WB_ENUM_GPIO_24_ALTERNATES_QSPI_M_IO_1             0x1
#define GP_WB_ENUM_GPIO_24_ALTERNATES_AKUMA_COEX_MS_PRIOB     0x2
#define GP_WB_ENUM_GPIO_24_ALTERNATES_AKUMA_COEX_SL_PRIO_0    0x3
#define GP_WB_ENUM_GPIO_24_ALTERNATES_ASP_0_DATA              0x4
#define GP_WB_ENUM_GPIO_24_ALTERNATES_ASP_1_DATA              0x5
#define GP_WB_ENUM_GPIO_24_ALTERNATES_IR_DOUT                 0x6
#define GP_WB_ENUM_GPIO_24_ALTERNATES_LED_LED_2               0x7
#define GP_WB_ENUM_GPIO_24_ALTERNATES_AKUMA_PHY_CHANNEL_3     0x8
#define GP_WB_ENUM_GPIO_24_ALTERNATES_PWMXL_PWMXL_4           0x9
#define GP_WB_ENUM_GPIO_24_ALTERNATES_UART_1_RX               0xB
#define GP_WB_ENUM_GPIO_24_ALTERNATES_AKUMA_RX_ADC0_OUT_0     0xC
#define GP_WB_ENUM_GPIO_24_ALTERNATES_AKUMA_RX_ADC1_OUT_0     0xD
#define GP_WB_ENUM_GPIO_24_ALTERNATES_I2C_M_0_SCL             0xE
#define GP_WB_ENUM_GPIO_24_ALTERNATES_I2C_SL_SCL              0xF
#define GP_WB_ENUM_GPIO_24_ALTERNATES_KEYPAD_SENSES_5         0x10
#define GP_WB_ENUM_GPIO_24_ALTERNATES_SPI_M_SSN               0x11
#define GP_WB_ENUM_GPIO_24_ALTERNATES_RTZ_1_TX                0x12
#define GP_WB_ENUM_GPIO_24_ALTERNATES_UART_0_TX               0x13
#define GP_WB_ENUM_GPIO_24_ALTERNATES_UART_2_TX               0x14
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_24_ALTERNATES_I2S_0_WS                0x15
#endif //['itwosperipheral']

/* gpio_25_alternates  */
#define GP_WB_ENUM_GPIO_25_ALTERNATES_AKUMA_COEX_ATT_CTRL_1    0x0
#define GP_WB_ENUM_GPIO_25_ALTERNATES_AKUMA_COEX_MS_REQB       0x1
#define GP_WB_ENUM_GPIO_25_ALTERNATES_AKUMA_COEX_SL_GRANT      0x2
#define GP_WB_ENUM_GPIO_25_ALTERNATES_UART_2_CTS               0x3
#define GP_WB_ENUM_GPIO_25_ALTERNATES_SPI_M_MISO               0x4
#define GP_WB_ENUM_GPIO_25_ALTERNATES_UART_0_RX                0x5
#define GP_WB_ENUM_GPIO_25_ALTERNATES_KEYPAD_SENSES_2          0x6
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_25_ALTERNATES_I2S_0_WS_IN              0x7
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_25_ALTERNATES_ANALOG_XP32KP            0x8

/* gpio_26_alternates  */
#define GP_WB_ENUM_GPIO_26_ALTERNATES_AKUMA_COEX_ATT_CTRL_2    0x0
#define GP_WB_ENUM_GPIO_26_ALTERNATES_AKUMA_COEX_MS_PRIOB      0x1
#define GP_WB_ENUM_GPIO_26_ALTERNATES_UART_2_CTS               0x2
#define GP_WB_ENUM_GPIO_26_ALTERNATES_ASP_1_DATA               0x3
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_26_ALTERNATES_I2S_0_SCK_IN             0x4
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_26_ALTERNATES_KEYPAD_SENSES_3          0x5
#define GP_WB_ENUM_GPIO_26_ALTERNATES_ANALOG_XP32KN            0x7

/* gpio_27_alternates  */
#define GP_WB_ENUM_GPIO_27_ALTERNATES_AKUMA_COEX_ATT_CTRL_2    0x0
#define GP_WB_ENUM_GPIO_27_ALTERNATES_AKUMA_COEX_MS_REQB       0x1
#define GP_WB_ENUM_GPIO_27_ALTERNATES_AKUMA_COEX_SL_GRANT      0x2
#define GP_WB_ENUM_GPIO_27_ALTERNATES_UART_0_CTS               0x3
#define GP_WB_ENUM_GPIO_27_ALTERNATES_UART_1_CTS               0x4
#define GP_WB_ENUM_GPIO_27_ALTERNATES_ASP_0_DATA               0x5
#define GP_WB_ENUM_GPIO_27_ALTERNATES_IR_DIN                   0x6
#define GP_WB_ENUM_GPIO_27_ALTERNATES_SPI_M_MISO               0x7
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_27_ALTERNATES_I2S_0_SDI                0x8
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_27_ALTERNATES_KEYPAD_SENSES_0          0x9
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_27_ALTERNATES_I2S_0_WS_IN              0xB
#endif //['itwosperipheral']

/* gpio_28_alternates  */
#define GP_WB_ENUM_GPIO_28_ALTERNATES_ANALOG_ANIO_0            0x0
#define GP_WB_ENUM_GPIO_28_ALTERNATES_AKUMA_COEX_ATT_CTRL_0    0x1
#define GP_WB_ENUM_GPIO_28_ALTERNATES_AKUMA_COEX_MS_PRIOA      0x2
#define GP_WB_ENUM_GPIO_28_ALTERNATES_ASP_1_DATA               0x3
#define GP_WB_ENUM_GPIO_28_ALTERNATES_IR_DIN                   0x4
#define GP_WB_ENUM_GPIO_28_ALTERNATES_BIST_OBM_IN              0x5
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_28_ALTERNATES_I2S_0_SCK_IN             0x6
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_28_ALTERNATES_KEYPAD_SENSES_1          0x7

/* gpio_29_alternates  */
#define GP_WB_ENUM_GPIO_29_ALTERNATES_ANALOG_ANIO_1            0x0
#define GP_WB_ENUM_GPIO_29_ALTERNATES_AKUMA_COEX_ATT_CTRL_1    0x1
#define GP_WB_ENUM_GPIO_29_ALTERNATES_AKUMA_COEX_MS_REQA       0x2
#define GP_WB_ENUM_GPIO_29_ALTERNATES_AKUMA_COEX_SL_GRANT      0x3
#define GP_WB_ENUM_GPIO_29_ALTERNATES_UART_0_CTS               0x4
#define GP_WB_ENUM_GPIO_29_ALTERNATES_UART_1_CTS               0x5
#define GP_WB_ENUM_GPIO_29_ALTERNATES_ASP_0_DATA               0x6
#define GP_WB_ENUM_GPIO_29_ALTERNATES_SPI_M_MISO               0x7
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_29_ALTERNATES_I2S_0_REF_IN             0x8
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_29_ALTERNATES_I2S_1_REF_IN             0x9
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_29_ALTERNATES_I2S_0_SDI                0xA
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_GPIO_29_ALTERNATES_I2S_1_SDI                0xB
#endif //['itwosperipheral']
#define GP_WB_ENUM_GPIO_29_ALTERNATES_KEYPAD_SENSES_2          0xC

/***************************
 * typemap: cma_enum
 ***************************/
/* nvm_cmd  */
#define GP_WB_ENUM_NVM_CMD_IDLE            0x0
#define GP_WB_ENUM_NVM_CMD_READ            0x1
#define GP_WB_ENUM_NVM_CMD_LOAD            0x2
#define GP_WB_ENUM_NVM_CMD_WRITE           0x3
#define GP_WB_ENUM_NVM_CMD_CLEAR_LOAD      0x4
#define GP_WB_ENUM_NVM_CMD_WRITE_CONFIG    0x5
#define GP_WB_ENUM_NVM_CMD_READ_CONFIG     0x6
#define GP_WB_ENUM_NVM_CMD_RECALL          0x7
#define GP_WB_ENUM_NVM_CMD_RESET_REG       0x8
#define GP_WB_ENUM_NVM_CMD_RUN_DMA         0x9
#define GP_WB_ENUM_NVM_CMD_REWRITE         0xA
#define GP_WB_ENUM_NVM_CMD_WRITE1          0xB
#define GP_WB_ENUM_NVM_CMD_WRITE0          0xC
#define GP_WB_ENUM_NVM_CMD_SW_R_CFG        0xD
#define GP_WB_ENUM_NVM_CMD_WRITE_EOTP      0xE
#define GP_WB_ENUM_NVM_CMD_DESTROY_EOTP    0xF

/* nvm_controller_state  */
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_INIT                   0x0
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_PD                 0x1
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_POC_IO             0x2
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_VDD                0x3
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD                    0x4
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_VDD_RECOVERY       0x5
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_POC_IO_RECOVERY    0x6
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_PORN_RECOVERY      0x7
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_DPD_PD_RECOVERY        0x8
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_NAP                    0x9
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_NAP_PRE_RECOVERY       0xA
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_NAP_RECOVERY           0xB
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_BLOCKED                0xC
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_PRE_IDLE               0xD
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_IDLE                   0xE
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_READ_BUSY              0xF
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_WRITE_BUSY             0x10
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_HIDDEN_BUSY            0x11
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_FAST_READ              0x12
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_READ                   0x13
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_LOAD                   0x14
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_HIDDEN_LOAD            0x15
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_WRITE                  0x16
#define GP_WB_ENUM_NVM_CONTROLLER_STATE_HIDDEN_WRITE           0x17

/* nvm_tmc_register  */
#define GP_WB_ENUM_NVM_TMC_REGISTER_TEST                          0x00
#define GP_WB_ENUM_NVM_TMC_REGISTER_MAP                           0x02
#define GP_WB_ENUM_NVM_TMC_REGISTER_RW                            0x04
#define GP_WB_ENUM_NVM_TMC_REGISTER_DEFAULT_CFG_63_0              0x06
#define GP_WB_ENUM_NVM_TMC_REGISTER_DEFAULT_CFG_127_64            0x08
#define GP_WB_ENUM_NVM_TMC_REGISTER_DEFAULT_CFG_191_128           0x0A
#define GP_WB_ENUM_NVM_TMC_REGISTER_DEFAULT_CFG_209_192           0x0C
#define GP_WB_ENUM_NVM_TMC_REGISTER_SET_GLOBAL_0                  0x10
#define GP_WB_ENUM_NVM_TMC_REGISTER_SET_GLOBAL_1                  0x12
#define GP_WB_ENUM_NVM_TMC_REGISTER_RESET_GLOBAL_0                0x14
#define GP_WB_ENUM_NVM_TMC_REGISTER_RESET_GLOBAL_1                0x16
#define GP_WB_ENUM_NVM_TMC_REGISTER_MASK_CFG_63_0                 0x18
#define GP_WB_ENUM_NVM_TMC_REGISTER_CONSTANT_CFG_63_0             0x20
#define GP_WB_ENUM_NVM_TMC_REGISTER_CONSTANT_CFG_127_64           0x22
#define GP_WB_ENUM_NVM_TMC_REGISTER_CONSTANT_CFG_191_128          0x24
#define GP_WB_ENUM_NVM_TMC_REGISTER_CONSTANT_CFG_209_192          0x26
#define GP_WB_ENUM_NVM_TMC_REGISTER_MAXPULSE                      0x28
#define GP_WB_ENUM_NVM_TMC_REGISTER_CLK_INFO                      0x2A
#define GP_WB_ENUM_NVM_TMC_REGISTER_PGM_WRITE_ALPHA               0x2C
#define GP_WB_ENUM_NVM_TMC_REGISTER_PGM_WRITE_BETA                0x2E
#define GP_WB_ENUM_NVM_TMC_REGISTER_PGM_READ                      0x30
#define GP_WB_ENUM_NVM_TMC_REGISTER_VFORM_OPTION                  0x3E
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG0_63_0           0x40
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG0_127_64         0x42
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG1_63_0           0x44
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG1_127_64         0x46
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG2_63_0           0x48
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_SET_CFG2_127_64         0x4A
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG0_63_0         0x5C
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG0_127_64       0x5E
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG1_63_0         0x60
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG1_127_64       0x62
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG2_63_0         0x64
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRAIN_RESET_CFG2_127_64       0x66
#define GP_WB_ENUM_NVM_TMC_REGISTER_FIRST_TRIM_TEMPCO_RES_TRIM    0x68
#define GP_WB_ENUM_NVM_TMC_REGISTER_USE_FIRST_TRIM_DATA           0x6A
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIMMING_STATUS               0x6C
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_VBG_OPT                  0xB6
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_PV                       0xB8
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_NOR                      0xBA
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_EV                       0xBC
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_IRD_STEP                 0xBE
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_IWD_STEP                 0xC0
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_IRD_STEP_VCH             0xC2
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_TEMPCO_RES_TRIM          0xC4
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_TEMPCO_RES_TRIM_RST      0xC6
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_VCH_TEMPCO_SLOPE         0xC8
#define GP_WB_ENUM_NVM_TMC_REGISTER_TRIM_VCH_TEMPCO_SLOPE_RST     0xCA
#define GP_WB_ENUM_NVM_TMC_REGISTER_FORCE_CFG                     0xCC
#define GP_WB_ENUM_NVM_TMC_REGISTER_ENABLE_TRIM                   0xCE
#define GP_WB_ENUM_NVM_TMC_REGISTER_DEBUG                         0xD0
#define GP_WB_ENUM_NVM_TMC_REGISTER_CR                            0xD2
#define GP_WB_ENUM_NVM_TMC_REGISTER_RR_BK0                        0xD4
#define GP_WB_ENUM_NVM_TMC_REGISTER_RR_BK1                        0xD6
#define GP_WB_ENUM_NVM_TMC_REGISTER_RR_BK2                        0xD8
#define GP_WB_ENUM_NVM_TMC_REGISTER_RR_BK3                        0xDA

/* nvm_bist_register  */
#define GP_WB_ENUM_NVM_BIST_REGISTER_IP_CONFIG                     0x1
#define GP_WB_ENUM_NVM_BIST_REGISTER_TRC_DATA                      0x2
#define GP_WB_ENUM_NVM_BIST_REGISTER_PATTERN                       0x3
#define GP_WB_ENUM_NVM_BIST_REGISTER_ADR_CTRL                      0x4
#define GP_WB_ENUM_NVM_BIST_REGISTER_TRC_DATA_RW_BIT_EABLE         0x5
#define GP_WB_ENUM_NVM_BIST_REGISTER_CMD                           0x6
#define GP_WB_ENUM_NVM_BIST_REGISTER_PIN_CTRL                      0x7
#define GP_WB_ENUM_NVM_BIST_REGISTER_ADR_CTRL_SETTING_1            0x8
#define GP_WB_ENUM_NVM_BIST_REGISTER_GRPSEL                        0x9
#define GP_WB_ENUM_NVM_BIST_REGISTER_STATUS                        0xB
#define GP_WB_ENUM_NVM_BIST_REGISTER_STATUS_EN                     0xC
#define GP_WB_ENUM_NVM_BIST_REGISTER_STATUS_CLR                    0xD
#define GP_WB_ENUM_NVM_BIST_REGISTER_STATUS_ARRYINFO_IP0           0xE
#define GP_WB_ENUM_NVM_BIST_REGISTER_STATUS_EN_ARRYINFO_IP0        0xF
#define GP_WB_ENUM_NVM_BIST_REGISTER_LOOP_CTRL                     0x10
#define GP_WB_ENUM_NVM_BIST_REGISTER_VERSION                       0x11
#define GP_WB_ENUM_NVM_BIST_REGISTER_FBC_SETTING                   0x12
#define GP_WB_ENUM_NVM_BIST_REGISTER_TRIM_COMPARE                  0x13
#define GP_WB_ENUM_NVM_BIST_REGISTER_DBG_SETTING                   0x14
#define GP_WB_ENUM_NVM_BIST_REGISTER_DBG_BIST_DOUT                 0x15
#define GP_WB_ENUM_NVM_BIST_REGISTER_ADR_CTRL_SETTING_0            0x16
#define GP_WB_ENUM_NVM_BIST_REGISTER_REPAIR_SECTOR_SEL             0x18
#define GP_WB_ENUM_NVM_BIST_REGISTER_COL_REPAIR_SEC                0x19
#define GP_WB_ENUM_NVM_BIST_REGISTER_ROW_REPAIR_SEC_TOPBOT_BANK    0x1A
#define GP_WB_ENUM_NVM_BIST_REGISTER_MULTI_XSIZE_PROGRAM           0x20
#define GP_WB_ENUM_NVM_BIST_REGISTER_MULTI_XSIZE_PROGRAM_1         0x21
#define GP_WB_ENUM_NVM_BIST_REGISTER_MULTI_XSIZE_PROGRAM_2         0x22
#define GP_WB_ENUM_NVM_BIST_REGISTER_COUNTER_SETTING               0x23
#define GP_WB_ENUM_NVM_BIST_REGISTER_BLG_TARGET_STATUS_EN          0x34
#define GP_WB_ENUM_NVM_BIST_REGISTER_BLG_TARGET_STATUS             0x35
#define GP_WB_ENUM_NVM_BIST_REGISTER_BLG_DUMMY_STATUS_EN           0x36
#define GP_WB_ENUM_NVM_BIST_REGISTER_BLG_DUMMY_STATUS              0x37
#define GP_WB_ENUM_NVM_BIST_REGISTER_BLG_IP_CONFIG                 0x3C

/* nvm_bist_mode  */
#define GP_WB_ENUM_NVM_BIST_MODE_DISABLED        0x0
#define GP_WB_ENUM_NVM_BIST_MODE_JTAG_ONLY       0x1
#define GP_WB_ENUM_NVM_BIST_MODE_JTAG_AND_PBI    0x2

/* nvm_pbi_cmd  */
#define GP_WB_ENUM_NVM_PBI_CMD_IDLE     0x0
#define GP_WB_ENUM_NVM_PBI_CMD_READ     0x1
#define GP_WB_ENUM_NVM_PBI_CMD_WRITE    0x2

/***************************
 * typemap: quic_enum
 ***************************/
/* channel_clock_speed  */
#define GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M32    0x0
#define GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M16    0x1
#define GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M8     0x2
#define GP_WB_ENUM_CHANNEL_CLOCK_SPEED_M4     0x3

/* channel_edge_sel  */
#define GP_WB_ENUM_CHANNEL_EDGE_SEL_RISING            0x0
#define GP_WB_ENUM_CHANNEL_EDGE_SEL_FALLING           0x1
#define GP_WB_ENUM_CHANNEL_EDGE_SEL_LEVEL             0x2
#define GP_WB_ENUM_CHANNEL_EDGE_SEL_LEVEL_INVERTED    0x3
#define GP_WB_ENUM_CHANNEL_EDGE_SEL_RISE_AND_FALL     0x4

/* channel_trigger_src_sel  */
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_NO_TRIGGER                   0x00
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_0                  0x01
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_1                  0x02
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_2                  0x03
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_IPC_3                  0x04
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PA_ON                  0x05
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PHY_RX                 0x06
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_PHY_TX                 0x07
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_RX_ON                  0x08
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_AKUMA_TX_ON                  0x09
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_BIST_TSENS_ABOVE_MAXIMUM     0x0A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_BIST_TSENS_BELOW_MINIMUM     0x0B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP1                   0x0C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DCDC_CLKP2                   0x0D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_DMA_DCB_BUFFER_COMPLETE_0    0x0E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_0                   0x0F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_1                   0x10
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_2                   0x11
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_3                   0x12
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_4                   0x13
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_5                   0x14
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_6                   0x15
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_7                   0x16
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_8                   0x17
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_9                   0x18
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_10                  0x19
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_11                  0x1A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_12                  0x1B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_13                  0x1C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_14                  0x1D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_ES_EVENT_15                  0x1E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_0                  0x1F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_1                  0x20
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_2                  0x21
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_3                  0x22
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_4                  0x23
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_5                  0x24
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_6                  0x25
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_7                  0x26
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_8                  0x27
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_9                  0x28
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_10                 0x29
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_11                 0x2A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_12                 0x2B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_13                 0x2C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_14                 0x2D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_GPIO_GPIO_15                 0x2E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PMUD_LPCMP_MATCH             0x2F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_MATCH    0x30
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_OFF      0x31
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_0_PHASE_ON       0x32
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_MATCH    0x33
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_OFF      0x34
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_1_PHASE_ON       0x35
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_MATCH    0x36
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_OFF      0x37
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_2_PHASE_ON       0x38
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_MATCH    0x39
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_OFF      0x3A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_3_PHASE_ON       0x3B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_MATCH    0x3C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_OFF      0x3D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_4_PHASE_ON       0x3E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_MATCH    0x3F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_OFF      0x40
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_5_PHASE_ON       0x41
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_MATCH    0x42
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_OFF      0x43
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_6_PHASE_ON       0x44
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_MATCH    0x45
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_OFF      0x46
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_PWMXL_7_PHASE_ON       0x47
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_PWMXL_TOP_OVERFLOW           0x48
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR0_WRAP              0x49
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR1_WRAP              0x4A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR2_WRAP              0x4B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMER_TMR3_WRAP              0x4C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_0_MATCH_0       0x4D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_0_MATCH_1       0x4E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_1_MATCH_0       0x4F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_1_MATCH_1       0x50
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_2_MATCH_0       0x51
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_2_MATCH_1       0x52
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_3_MATCH_0       0x53
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_CC_3_MATCH_1       0x54
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_OVERFLOW_0         0x55
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_0_OVERFLOW_1         0x56
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_0_MATCH_0       0x57
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_0_MATCH_1       0x58
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_1_MATCH_0       0x59
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_1_MATCH_1       0x5A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_2_MATCH_0       0x5B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_2_MATCH_1       0x5C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_3_MATCH_0       0x5D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_CC_3_MATCH_1       0x5E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_OVERFLOW_0         0x5F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_1_OVERFLOW_1         0x60
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_0_MATCH_0       0x61
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_0_MATCH_1       0x62
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_1_MATCH_0       0x63
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_1_MATCH_1       0x64
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_2_MATCH_0       0x65
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_2_MATCH_1       0x66
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_3_MATCH_0       0x67
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_CC_3_MATCH_1       0x68
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_OVERFLOW_0         0x69
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_2_OVERFLOW_1         0x6A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_0_MATCH_0       0x6B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_0_MATCH_1       0x6C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_1_MATCH_0       0x6D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_1_MATCH_1       0x6E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_2_MATCH_0       0x6F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_2_MATCH_1       0x70
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_3_MATCH_0       0x71
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_CC_3_MATCH_1       0x72
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_OVERFLOW_0         0x73
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_3_OVERFLOW_1         0x74
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_0_MATCH_0       0x75
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_0_MATCH_1       0x76
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_1_MATCH_0       0x77
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_1_MATCH_1       0x78
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_2_MATCH_0       0x79
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_2_MATCH_1       0x7A
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_3_MATCH_0       0x7B
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_CC_3_MATCH_1       0x7C
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_OVERFLOW_0         0x7D
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_4_OVERFLOW_1         0x7E
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_0_MATCH_0       0x7F
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_0_MATCH_1       0x80
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_1_MATCH_0       0x81
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_1_MATCH_1       0x82
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_2_MATCH_0       0x83
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_2_MATCH_1       0x84
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_3_MATCH_0       0x85
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_CC_3_MATCH_1       0x86
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_OVERFLOW_0         0x87
#define GP_WB_ENUM_CHANNEL_TRIGGER_SRC_SEL_TIMERXL_5_OVERFLOW_1         0x88

/***************************
 * typemap: dma_scb_enum
 ***************************/
/* dma_scb_word_mode  */
#define GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA8BIT     0x0
#define GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA16BIT    0x1
#define GP_WB_ENUM_DMA_SCB_WORD_MODE_DATA32BIT    0x2

/* dma_scb_buffer_complete_mode  */
#define GP_WB_ENUM_DMA_SCB_BUFFER_COMPLETE_MODE_ERROR_MODE     0x0
#define GP_WB_ENUM_DMA_SCB_BUFFER_COMPLETE_MODE_STATUS_MODE    0x1

/* dma_scb_action  */
#define GP_WB_ENUM_DMA_SCB_ACTION_NONE       0x0
#define GP_WB_ENUM_DMA_SCB_ACTION_DO_COPY    0x1

/* circular_buffer  */
#define GP_WB_ENUM_CIRCULAR_BUFFER_SRC_BUFFER     0x0
#define GP_WB_ENUM_CIRCULAR_BUFFER_DEST_BUFFER    0x1

/* dma_scb_trigger_type  */
#define GP_WB_ENUM_DMA_SCB_TRIGGER_TYPE_LEVEL_TRIGGER    0x0
#define GP_WB_ENUM_DMA_SCB_TRIGGER_TYPE_EVENT_TRIGGER    0x1

/* dma_scb_trigger_src_select  */
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_NO_TRIGGER_SRC                      0x00
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_0_DATASTREAM_0_VALID            0x01
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_0_DATASTREAM_1_VALID            0x02
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_1_DATASTREAM_0_VALID            0x03
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_ASP_1_DATASTREAM_1_VALID            0x04
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_BIST_ADC_RESULT_FIFO_NOT_EMPTY      0x05
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_BIST_TSENS_RESULT_FIFO_NOT_EMPTY    0x06
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GP_ADC_FIFO_NOT_EMPTY               0x07
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GPIO_TIMESTAMP_FIFO_0_NOT_EMPTY     0x08
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_GPIO_TIMESTAMP_FIFO_1_NOT_EMPTY     0x09
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_HR_ADC_FIFO_NOT_EMPTY               0x0A
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_0_RX_NOT_EMPTY                  0x0B
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_0_TX_NOT_FULL                   0x0C
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_1_RX_NOT_EMPTY                  0x0D
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_I2S_1_TX_NOT_FULL                   0x0E
#endif //['itwosperipheral']
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_NEXT_BUFFER_NOT_FULL          0x12
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_0_PHASE_MATCH           0x13
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_0_PHASE_OFF             0x14
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_0_PHASE_ON              0x15
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_1_PHASE_MATCH           0x16
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_1_PHASE_OFF             0x17
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_1_PHASE_ON              0x18
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_2_PHASE_MATCH           0x19
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_2_PHASE_OFF             0x1A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_2_PHASE_ON              0x1B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_3_PHASE_MATCH           0x1C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_3_PHASE_OFF             0x1D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_3_PHASE_ON              0x1E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_4_PHASE_MATCH           0x1F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_4_PHASE_OFF             0x20
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_4_PHASE_ON              0x21
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_5_PHASE_MATCH           0x22
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_5_PHASE_OFF             0x23
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_5_PHASE_ON              0x24
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_6_PHASE_MATCH           0x25
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_6_PHASE_OFF             0x26
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_6_PHASE_ON              0x27
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_7_PHASE_MATCH           0x28
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_7_PHASE_OFF             0x29
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_PWMXL_7_PHASE_ON              0x2A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_PWMXL_TOP_OVERFLOW                  0x2B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_QSPI_M_RX_NOT_EMPTY                 0x2C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_QSPI_M_TX_NOT_FULL                  0x2D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_RTZ_0_TX_NOT_FULL                   0x2E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_RTZ_1_TX_NOT_FULL                   0x2F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_M_RX_NOT_EMPTY                  0x30
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_M_TX_NOT_FULL                   0x31
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_SL_RX_NOT_EMPTY                 0x32
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_SPI_SL_TX_NOT_FULL                  0x33
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_0_MATCH_0              0x34
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_0_MATCH_1              0x35
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_1_MATCH_0              0x36
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_1_MATCH_1              0x37
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_2_MATCH_0              0x38
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_2_MATCH_1              0x39
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_3_MATCH_0              0x3A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_CC_3_MATCH_1              0x3B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_OVERFLOW_0                0x3C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_0_OVERFLOW_1                0x3D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_0_MATCH_0              0x3E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_0_MATCH_1              0x3F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_1_MATCH_0              0x40
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_1_MATCH_1              0x41
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_2_MATCH_0              0x42
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_2_MATCH_1              0x43
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_3_MATCH_0              0x44
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_CC_3_MATCH_1              0x45
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_OVERFLOW_0                0x46
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_1_OVERFLOW_1                0x47
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_0_MATCH_0              0x48
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_0_MATCH_1              0x49
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_1_MATCH_0              0x4A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_1_MATCH_1              0x4B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_2_MATCH_0              0x4C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_2_MATCH_1              0x4D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_3_MATCH_0              0x4E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_CC_3_MATCH_1              0x4F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_OVERFLOW_0                0x50
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_2_OVERFLOW_1                0x51
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_0_MATCH_0              0x52
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_0_MATCH_1              0x53
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_1_MATCH_0              0x54
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_1_MATCH_1              0x55
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_2_MATCH_0              0x56
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_2_MATCH_1              0x57
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_3_MATCH_0              0x58
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_CC_3_MATCH_1              0x59
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_OVERFLOW_0                0x5A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_3_OVERFLOW_1                0x5B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_0_MATCH_0              0x5C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_0_MATCH_1              0x5D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_1_MATCH_0              0x5E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_1_MATCH_1              0x5F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_2_MATCH_0              0x60
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_2_MATCH_1              0x61
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_3_MATCH_0              0x62
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_CC_3_MATCH_1              0x63
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_OVERFLOW_0                0x64
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_4_OVERFLOW_1                0x65
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_0_MATCH_0              0x66
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_0_MATCH_1              0x67
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_1_MATCH_0              0x68
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_1_MATCH_1              0x69
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_2_MATCH_0              0x6A
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_2_MATCH_1              0x6B
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_3_MATCH_0              0x6C
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_CC_3_MATCH_1              0x6D
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_OVERFLOW_0                0x6E
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_TIMERXL_5_OVERFLOW_1                0x6F
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_0_RX_NOT_EMPTY                 0x70
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_0_TX_NOT_FULL                  0x71
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_1_RX_NOT_EMPTY                 0x72
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_1_TX_NOT_FULL                  0x73
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_2_RX_NOT_EMPTY                 0x74
#define GP_WB_ENUM_DMA_SCB_TRIGGER_SRC_SELECT_UART_2_TX_NOT_FULL                  0x75

/***************************
 * typemap: gpio_enum
 ***************************/
/* sampling_clock_speed  */
#define GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M32    0x0
#define GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M8     0x1
#define GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M4     0x2
#define GP_WB_ENUM_SAMPLING_CLOCK_SPEED_M2     0x3

/* gpio_action  */
#define GP_WB_ENUM_GPIO_ACTION_NONE      0x0
#define GP_WB_ENUM_GPIO_ACTION_SET       0x1
#define GP_WB_ENUM_GPIO_ACTION_CLR       0x2
#define GP_WB_ENUM_GPIO_ACTION_TOGGLE    0x3

/***************************
 * typemap: debug_asic_enum
 ***************************/
/* debug_sel  */
#define GP_WB_ENUM_DEBUG_SEL_APPUC_ASLEEP                             0x00
#define GP_WB_ENUM_DEBUG_SEL_APPUC_CACHE_HIT0                         0x01
#define GP_WB_ENUM_DEBUG_SEL_APPUC_CACHE_HIT1                         0x02
#define GP_WB_ENUM_DEBUG_SEL_APPUC_CACHE_MISS                         0x03
#define GP_WB_ENUM_DEBUG_SEL_APPUC_CORE_AHB_ERROR                     0x04
#define GP_WB_ENUM_DEBUG_SEL_APPUC_HALT                               0x05
#define GP_WB_ENUM_DEBUG_SEL_APPUC_INDEEPSLEEP                        0x06
#define GP_WB_ENUM_DEBUG_SEL_APPUC_LOCKUP                             0x07
#define GP_WB_ENUM_DEBUG_SEL_APPUC_LPWAIT_ACTIVE                      0x08
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_0_READ_ACCESS                  0x09
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_0_WRITE_ACCESS                 0x0A
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_1_READ_ACCESS                  0x0B
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_1_WRITE_ACCESS                 0x0C
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_2_READ_ACCESS                  0x0D
#define GP_WB_ENUM_DEBUG_SEL_APPUC_RAM_2_WRITE_ACCESS                 0x0E
#define GP_WB_ENUM_DEBUG_SEL_BBPLL_LOCKED                             0x0F
#define GP_WB_ENUM_DEBUG_SEL_BBPLL_LOCKED_RAW                         0x10
#define GP_WB_ENUM_DEBUG_SEL_BBPLL_SMPL_OUT                           0x11
#define GP_WB_ENUM_DEBUG_SEL_BIST_ADC_EOC                             0x12
#define GP_WB_ENUM_DEBUG_SEL_BIST_ADC_SCALER_RESET                    0x13
#define GP_WB_ENUM_DEBUG_SEL_BIST_CP_COMP_OUT                         0x14
#define GP_WB_ENUM_DEBUG_SEL_BIST_CP_TO_VDDT                          0x15
#define GP_WB_ENUM_DEBUG_SEL_BIST_OBM                                 0x16
#define GP_WB_ENUM_DEBUG_SEL_BIST_TSENS_BS                            0x17
#define GP_WB_ENUM_DEBUG_SEL_BIST_TSENS_BS_DIG                        0x18
#define GP_WB_ENUM_DEBUG_SEL_BIST_TSENS_CLK_EVAL                      0x19
#define GP_WB_ENUM_DEBUG_SEL_CMA_AHB_AK_RTUC_READ_ACCESS              0x1A
#define GP_WB_ENUM_DEBUG_SEL_CMA_AHB_APPUC_READ_ACCESS                0x1B
#define GP_WB_ENUM_DEBUG_SEL_CMA_AHB_MAIN_READ_ACCESS                 0x1C
#define GP_WB_ENUM_DEBUG_SEL_CMA_AHB_SEC_READ_ACCESS                  0x1D
#define GP_WB_ENUM_DEBUG_SEL_CMA_ROM_MACRO_READ_ACCESS                0x1E
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_LOAD_BUFFER_EMPTY    0x1F
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_STATE_0              0x20
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_STATE_1              0x21
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_STATE_2              0x22
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_STATE_3              0x23
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_CONTROLLER_STATE_4              0x24
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_MACRO_NAP_MODE                  0x25
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_MACRO_PD_MODE                   0x26
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_MACRO_READ_ACCESS               0x27
#define GP_WB_ENUM_DEBUG_SEL_CMA_RRAM_MACRO_WRITE_ACCESS              0x28
#define GP_WB_ENUM_DEBUG_SEL_DCDC_CLKDET1                             0x29
#define GP_WB_ENUM_DEBUG_SEL_DCDC_CLKDET2                             0x2A
#define GP_WB_ENUM_DEBUG_SEL_DCDC_CLKN_END                            0x2B
#define GP_WB_ENUM_DEBUG_SEL_DCDC_CLKP                                0x2C
#define GP_WB_ENUM_DEBUG_SEL_DCDC_S1                                  0x2D
#define GP_WB_ENUM_DEBUG_SEL_DCDC_S1_SKIP                             0x2E
#define GP_WB_ENUM_DEBUG_SEL_DCDC_S2                                  0x2F
#define GP_WB_ENUM_DEBUG_SEL_DCDC_S2_SKIP                             0x30
#define GP_WB_ENUM_DEBUG_SEL_DCDC_VFB1                                0x31
#define GP_WB_ENUM_DEBUG_SEL_DCDC_VFB2                                0x32
#define GP_WB_ENUM_DEBUG_SEL_DCDC_VFB_EDGE                            0x33
#define GP_WB_ENUM_DEBUG_SEL_DCDC_ZCD                                 0x34
#define GP_WB_ENUM_DEBUG_SEL_DCDC_ZCD_OFFSET_INC                      0x35
#define GP_WB_ENUM_DEBUG_SEL_ES_ABS_EV_0                              0x36
#define GP_WB_ENUM_DEBUG_SEL_ES_ABS_EV_1                              0x37
#define GP_WB_ENUM_DEBUG_SEL_ES_ABS_EV_2                              0x38
#define GP_WB_ENUM_DEBUG_SEL_ES_ABS_EV_3                              0x39
#define GP_WB_ENUM_DEBUG_SEL_ES_RTC_CLK                               0x3A
#define GP_WB_ENUM_DEBUG_SEL_GP_ADC_EOC                               0x3B
#define GP_WB_ENUM_DEBUG_SEL_GP_ADC_SCALER_RESET                      0x3C
#define GP_WB_ENUM_DEBUG_SEL_HR_ADC_BS                                0x3D
#define GP_WB_ENUM_DEBUG_SEL_HR_ADC_BS_DIG                            0x3E
#define GP_WB_ENUM_DEBUG_SEL_HR_ADC_CLK_EVAL                          0x3F
#define GP_WB_ENUM_DEBUG_SEL_HR_ADC_DITHER_OUT                        0x40
#define GP_WB_ENUM_DEBUG_SEL_IR_CARRIER                               0x41
#define GP_WB_ENUM_DEBUG_SEL_IR_ENVELOPE                              0x42
#define GP_WB_ENUM_DEBUG_SEL_MM_AKUMA_M_READ_ACCESS                   0x43
#define GP_WB_ENUM_DEBUG_SEL_MM_AKUMA_M_WRITE_ACCESS                  0x44
#define GP_WB_ENUM_DEBUG_SEL_MM_AKUMA_S_READ_ACCESS                   0x45
#define GP_WB_ENUM_DEBUG_SEL_MM_AKUMA_S_WRITE_ACCESS                  0x46
#define GP_WB_ENUM_DEBUG_SEL_MM_APPUC_M_READ_ACCESS                   0x47
#define GP_WB_ENUM_DEBUG_SEL_MM_APPUC_M_WRITE_ACCESS                  0x48
#define GP_WB_ENUM_DEBUG_SEL_MM_APPUC_S_READ_ACCESS                   0x49
#define GP_WB_ENUM_DEBUG_SEL_MM_APPUC_S_WRITE_ACCESS                  0x4A
#define GP_WB_ENUM_DEBUG_SEL_MM_DMA_DCB_READ_ACCESS                   0x4B
#define GP_WB_ENUM_DEBUG_SEL_MM_DMA_DCB_WRITE_ACCESS                  0x4C
#define GP_WB_ENUM_DEBUG_SEL_MM_DMA_SCB_READ_ACCESS                   0x4D
#define GP_WB_ENUM_DEBUG_SEL_MM_DMA_SCB_WRITE_ACCESS                  0x4E
#define GP_WB_ENUM_DEBUG_SEL_MM_REGMAP_GPM_READ_ACCESS                0x4F
#define GP_WB_ENUM_DEBUG_SEL_MM_REGMAP_GPM_WRITE_ACCESS               0x50
#define GP_WB_ENUM_DEBUG_SEL_MM_SE_MB_READ_ACCESS                     0x51
#define GP_WB_ENUM_DEBUG_SEL_MM_SE_MB_WRITE_ACCESS                    0x52
#define GP_WB_ENUM_DEBUG_SEL_MM_SYSRAM_READ_ACCESS                    0x53
#define GP_WB_ENUM_DEBUG_SEL_MM_SYSRAM_WRITE_ACCESS                   0x54
#define GP_WB_ENUM_DEBUG_SEL_SECURE_ELEMENT_ASLEEP                    0x55
#define GP_WB_ENUM_DEBUG_SEL_SECURE_ELEMENT_CORE_AHB_ERROR            0x56
#define GP_WB_ENUM_DEBUG_SEL_SECURE_ELEMENT_HALT                      0x57
#define GP_WB_ENUM_DEBUG_SEL_SECURE_ELEMENT_INDEEPSLEEP               0x58
#define GP_WB_ENUM_DEBUG_SEL_SECURE_ELEMENT_LOCKUP                    0x59
#define GP_WB_ENUM_DEBUG_SEL_STANDBY_RTGTS_ALL                        0x5A
#define GP_WB_ENUM_DEBUG_SEL_STANDBY_STATE_ACTIVE                     0x5B
#define GP_WB_ENUM_DEBUG_SEL_STANDBY_STATE_SLEEP                      0x5C

#endif //GP_WB_ENUM_H
