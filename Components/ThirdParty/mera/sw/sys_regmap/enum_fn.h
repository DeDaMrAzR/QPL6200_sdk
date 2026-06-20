#ifndef GPHAL_ENUM_H
#define GPHAL_ENUM_H

/***************************
 * Global typemap: general_enum
 ***************************/
/* gpio_interrupt_type  */
const char * gpio_interrupt_type2str(unsigned int n);

/* parity  */
const char * parity2str(unsigned int n);

/* watchdog_function  */
const char * watchdog_function2str(unsigned int n);

/* nvm_size  */
const char * nvm_size2str(unsigned int n);

/* serial_itf_select  */
const char * serial_itf_select2str(unsigned int n);

/* qspi_cmd_mode  */
const char * qspi_cmd_mode2str(unsigned int n);

/* qspi_cmd_type  */
const char * qspi_cmd_type2str(unsigned int n);

/* qspi_io_default  */
const char * qspi_io_default2str(unsigned int n);

/* qspi_sample_mode  */
const char * qspi_sample_mode2str(unsigned int n);

/* keypad_action  */
const char * keypad_action2str(unsigned int n);

/* dma_dcb_action  */
const char * dma_dcb_action2str(unsigned int n);

/* rtz_action  */
const char * rtz_action2str(unsigned int n);

/* rtz_buffer_mode  */
const char * rtz_buffer_mode2str(unsigned int n);

/* pwmxl_count_mode  */
const char * pwmxl_count_mode2str(unsigned int n);

/* pwmxl_action  */
const char * pwmxl_action2str(unsigned int n);

/* pwmxl_phase_trigger_mode  */
const char * pwmxl_phase_trigger_mode2str(unsigned int n);

/* pwmxl_config_value_type  */
const char * pwmxl_config_value_type2str(unsigned int n);

/* timerxl_count_mode  */
const char * timerxl_count_mode2str(unsigned int n);

/* timerxl_operating_mode  */
const char * timerxl_operating_mode2str(unsigned int n);

/* timerxl_action  */
const char * timerxl_action2str(unsigned int n);

/* cp_threshold voltage threshold for the CP (Charge Pump) comparator */
const char * cp_threshold2str(unsigned int n);

/* cp_switch_wait_time CP (Charge Pump) delay time between enabling CP until enabling switch */
const char * cp_switch_wait_time2str(unsigned int n);

/* cp_comp_out_wait_time CP (Charge Pump) delay time to consider the output of comp_out valid again after changing the voltage threshold */
const char * cp_comp_out_wait_time2str(unsigned int n);

/* bist_action  */
const char * bist_action2str(unsigned int n);

/* led_src_clk_freq_sel  */
const char * led_src_clk_freq_sel2str(unsigned int n);

/* led_action  */
const char * led_action2str(unsigned int n);

/* adc_trigger_mode  */
const char * adc_trigger_mode2str(unsigned int n);

/* gp_adc_channel  */
const char * gp_adc_channel2str(unsigned int n);

/* hr_adc_channel  */
const char * hr_adc_channel2str(unsigned int n);

/* lpcomp_channel  */
const char * lpcomp_channel2str(unsigned int n);

/* adc_buffer_update_mode  */
const char * adc_buffer_update_mode2str(unsigned int n);

/* gp_adc_action  */
const char * gp_adc_action2str(unsigned int n);

/* hr_adc_action  */
const char * hr_adc_action2str(unsigned int n);

/* i2s_clk_sel  */
const char * i2s_clk_sel2str(unsigned int n);

/* asp_clk_sel  */
const char * asp_clk_sel2str(unsigned int n);

/* asp_datastream_mode  */
const char * asp_datastream_mode2str(unsigned int n);

/* ir_modulation_mode  */
const char * ir_modulation_mode2str(unsigned int n);

/* ir_time_unit  */
const char * ir_time_unit2str(unsigned int n);

/* ir_action  */
const char * ir_action2str(unsigned int n);

/* dma_dcb_word_mode  */
const char * dma_dcb_word_mode2str(unsigned int n);

/* generic_io_drive  */
const char * generic_io_drive2str(unsigned int n);

/* clock_speed  */
const char * clock_speed2str(unsigned int n);

/* sys_main_clock_source  */
const char * sys_main_clock_source2str(unsigned int n);

/* obm_src  */
const char * obm_src2str(unsigned int n);

/* obm_state  */
const char * obm_state2str(unsigned int n);

/* por_reason  */
const char * por_reason2str(unsigned int n);

/* wkup_reason  */
const char * wkup_reason2str(unsigned int n);

/* rtc_timer_src  */
const char * rtc_timer_src2str(unsigned int n);

/* wakeup_pin_mode  */
const char * wakeup_pin_mode2str(unsigned int n);

/* ssp_mode  */
const char * ssp_mode2str(unsigned int n);

/* ssp_key_len  */
const char * ssp_key_len2str(unsigned int n);

/* event_state  */
const char * event_state2str(unsigned int n);

/* event_result  */
const char * event_result2str(unsigned int n);

/* standby_power_mode  */
const char * standby_power_mode2str(unsigned int n);

/* active_power_mode  */
const char * active_power_mode2str(unsigned int n);

/* tsens_hanning_cycles  */
const char * tsens_hanning_cycles2str(unsigned int n);

/* hr_adc_hanning_cycles  */
const char * hr_adc_hanning_cycles2str(unsigned int n);

/* key_select  */
const char * key_select2str(unsigned int n);

/* power_state  */
const char * power_state2str(unsigned int n);

/* dcdc_rx_mode  */
const char * dcdc_rx_mode2str(unsigned int n);

/* dcdc_tx_mode  */
const char * dcdc_tx_mode2str(unsigned int n);


/***************************
 * Global typemap: iob_enum
 ***************************/
/* gpio_pull_mode  */
const char * gpio_pull_mode2str(unsigned int n);

/* gpio_slew_rate  */
const char * gpio_slew_rate2str(unsigned int n);

/* gpio_drive_strength  */
const char * gpio_drive_strength2str(unsigned int n);

/* gpio_0_alternates  */
const char * gpio_0_alternates2str(unsigned int n);

/* gpio_1_alternates  */
const char * gpio_1_alternates2str(unsigned int n);

/* gpio_2_alternates  */
const char * gpio_2_alternates2str(unsigned int n);

/* gpio_3_alternates  */
const char * gpio_3_alternates2str(unsigned int n);

/* gpio_4_alternates  */
const char * gpio_4_alternates2str(unsigned int n);

/* gpio_5_alternates  */
const char * gpio_5_alternates2str(unsigned int n);

/* gpio_6_alternates  */
const char * gpio_6_alternates2str(unsigned int n);

/* gpio_7_alternates  */
const char * gpio_7_alternates2str(unsigned int n);

/* gpio_8_alternates  */
const char * gpio_8_alternates2str(unsigned int n);

/* gpio_9_alternates  */
const char * gpio_9_alternates2str(unsigned int n);

/* gpio_10_alternates  */
const char * gpio_10_alternates2str(unsigned int n);

/* gpio_11_alternates  */
const char * gpio_11_alternates2str(unsigned int n);

/* gpio_12_alternates  */
const char * gpio_12_alternates2str(unsigned int n);

/* gpio_13_alternates  */
const char * gpio_13_alternates2str(unsigned int n);

/* gpio_14_alternates  */
const char * gpio_14_alternates2str(unsigned int n);

/* gpio_15_alternates  */
const char * gpio_15_alternates2str(unsigned int n);

/* gpio_16_alternates  */
const char * gpio_16_alternates2str(unsigned int n);

/* gpio_17_alternates  */
const char * gpio_17_alternates2str(unsigned int n);

/* gpio_18_alternates  */
const char * gpio_18_alternates2str(unsigned int n);

/* gpio_19_alternates  */
const char * gpio_19_alternates2str(unsigned int n);

/* gpio_20_alternates  */
const char * gpio_20_alternates2str(unsigned int n);

/* gpio_21_alternates  */
const char * gpio_21_alternates2str(unsigned int n);

/* gpio_22_alternates  */
const char * gpio_22_alternates2str(unsigned int n);

/* gpio_23_alternates  */
const char * gpio_23_alternates2str(unsigned int n);

/* gpio_24_alternates  */
const char * gpio_24_alternates2str(unsigned int n);

/* gpio_25_alternates  */
const char * gpio_25_alternates2str(unsigned int n);

/* gpio_26_alternates  */
const char * gpio_26_alternates2str(unsigned int n);

/* gpio_27_alternates  */
const char * gpio_27_alternates2str(unsigned int n);

/* gpio_28_alternates  */
const char * gpio_28_alternates2str(unsigned int n);

/* gpio_29_alternates  */
const char * gpio_29_alternates2str(unsigned int n);


/***************************
 * Global typemap: cma_enum
 ***************************/
/* nvm_cmd  */
const char * nvm_cmd2str(unsigned int n);

/* nvm_controller_state  */
const char * nvm_controller_state2str(unsigned int n);

/* nvm_tmc_register  */
const char * nvm_tmc_register2str(unsigned int n);

/* nvm_bist_register  */
const char * nvm_bist_register2str(unsigned int n);

/* nvm_bist_mode  */
const char * nvm_bist_mode2str(unsigned int n);

/* nvm_pbi_cmd  */
const char * nvm_pbi_cmd2str(unsigned int n);


/***************************
 * Global typemap: quic_enum
 ***************************/
/* channel_clock_speed  */
const char * channel_clock_speed2str(unsigned int n);

/* channel_edge_sel  */
const char * channel_edge_sel2str(unsigned int n);

/* channel_trigger_src_sel  */
const char * channel_trigger_src_sel2str(unsigned int n);


/***************************
 * Global typemap: dma_scb_enum
 ***************************/
/* dma_scb_word_mode  */
const char * dma_scb_word_mode2str(unsigned int n);

/* dma_scb_buffer_complete_mode  */
const char * dma_scb_buffer_complete_mode2str(unsigned int n);

/* dma_scb_action  */
const char * dma_scb_action2str(unsigned int n);

/* circular_buffer  */
const char * circular_buffer2str(unsigned int n);

/* dma_scb_trigger_type  */
const char * dma_scb_trigger_type2str(unsigned int n);

/* dma_scb_trigger_src_select  */
const char * dma_scb_trigger_src_select2str(unsigned int n);


/***************************
 * Global typemap: gpio_enum
 ***************************/
/* sampling_clock_speed  */
const char * sampling_clock_speed2str(unsigned int n);

/* gpio_action  */
const char * gpio_action2str(unsigned int n);


/***************************
 * Global typemap: debug_asic_enum
 ***************************/
/* debug_sel  */
const char * debug_sel2str(unsigned int n);

#endif //GPHAL_ENUM_H
