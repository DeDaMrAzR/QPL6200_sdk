#ifndef GP_WB_PROP_STRUCT_H
#define GP_WB_PROP_STRUCT_H

/***************************
 * regmap: cal_ram
 ***************************/
typedef struct CAL_RAM_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 cal_nvm_ptr;
    __IO halprop_reg32 vbatt_uv;
    __IO halprop_reg32 temp_deg;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x000c;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x0010;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x0014;
    __IO halprop_reg32 xo32m_cload_comp_lut;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x001c;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x0020;
    __IO halprop_reg32 unnamed_reg_cal_ram_0x0024;
    __IO halprop_reg32 xo32m_cload_comp_16;
    __IO halprop_reg32 bist_adc_mode;
    __IO halprop_reg32 cp_users;
} PACKED_POST CAL_RAM_REGMAP_T;

#define CAL_RAM_REGMAP ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)

/***************************
 * regmap: appuc
 ***************************/
typedef struct APPUC_REGMAP_S
{
    __IO halprop_reg32 vector_table_offset;
    __IO halprop_reg32 systick_calib;
    __IO halprop_reg32 status;
    __IO halprop_reg32 cfg;
    __IO halprop_reg32 nvm_cfg;
    __IO halprop_reg32 cache_statistics_clr;
    __IO halprop_reg32 enable_sync_down_bypass;
    __IO halprop_reg32 rtgts_sw;
    __IO halprop_reg32 unnamed_reg_appuc_0x0020;
    __IO halprop_reg32 lpwait_end;
    __IO halprop_reg32 lpwait_status;
    __IO halprop_reg32 clear_interrupts;
    __IO halprop_reg32 interrupt_status;
    __IO halprop_reg32 cache_hit0_cnt;
    __IO halprop_reg32 cache_hit1_cnt;
    __IO halprop_reg32 cache_miss_cnt;
    __IO halprop_reg32 all_rams_bist_enable;
    __IO halprop_reg32 all_rams_bist_start;
    __IO halprop_reg32 all_rams_bist_status;
    __IO halprop_reg32 dci_enable;
    __IO halprop_reg32 debug_status;
    __IO halprop_reg32 debug_cssys;
} APPUC_REGMAP_T;

#define APPUC_REGMAP ((APPUC_REGMAP_T *) GP_WB_APPUC_BASE_ADDRESS)

/***************************
 * regmap: int_ctrl
 ***************************/
typedef struct INT_CTRL_REGMAP_S
{
    __IO halprop_reg32 unmasked_appuc_interrupts_0;
    __IO halprop_reg32 unmasked_appuc_interrupts_1;
    __IO halprop_reg32 unmasked_ext_interrupts;
    __IO halprop_reg32 masked_appuc_interrupts_0;
    __IO halprop_reg32 masked_appuc_interrupts_1;
    __IO halprop_reg32 masked_ext_interrupts;
    __IO halprop_reg32 mask_appuc_interrupts_0;
    __IO halprop_reg32 mask_appuc_interrupts_1;
    __IO halprop_reg32 mask_ext_interrupts;
    __IO halprop_reg32 masked_appuc_akuma_main_interrupt;
    __IO halprop_reg32 masked_appuc_asp_0_interrupts;
    __IO halprop_reg32 masked_appuc_asp_1_interrupts;
    __IO halprop_reg32 masked_appuc_bbpll_interrupts;
    __IO halprop_reg32 masked_appuc_bist_interrupts;
    __IO halprop_reg32 masked_appuc_dcdc_interrupts;
    __IO halprop_reg32 masked_appuc_debug_debug_interrupt;
    __IO halprop_reg32 masked_appuc_dma_dcb_interrupts;
    __IO halprop_reg32 masked_appuc_dma_scb_interrupts_0;
    __IO halprop_reg32 masked_appuc_dma_scb_cpy_err_interrupt;
    __IO halprop_reg32 masked_appuc_es_interrupts;
    __IO halprop_reg32 masked_appuc_gp_adc_interrupts;
    __IO halprop_reg32 masked_appuc_gpio_0_interrupts;
    __IO halprop_reg32 masked_appuc_gpio_1_interrupts;
    __IO halprop_reg32 masked_appuc_hr_adc_interrupts;
    __IO halprop_reg32 masked_appuc_i2c_m_0_interrupts;
    __IO halprop_reg32 masked_appuc_i2c_m_1_interrupts;
    __IO halprop_reg32 masked_appuc_i2c_sl_interrupts;
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
    __IO halprop_reg32 masked_appuc_i2s_0_interrupts;
#else //['itwosperipheral']
    halprop_reg32 reserved_0[1];
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
    __IO halprop_reg32 masked_appuc_i2s_1_interrupts;
#else //['itwosperipheral']
    halprop_reg32 reserved_1[1];
#endif //['itwosperipheral']
    __IO halprop_reg32 masked_appuc_ipc_interrupts;
    __IO halprop_reg32 masked_appuc_ir_interrupts;
    __IO halprop_reg32 masked_appuc_keypad_scan_done_interrupt;
    __IO halprop_reg32 masked_appuc_pmud_lpcmp_match_interrupt;
    halprop_reg32 reserved_2[1];
    __IO halprop_reg32 masked_appuc_pwmxl_interrupts;
    __IO halprop_reg32 masked_appuc_qspi_m_interrupts;
    __IO halprop_reg32 masked_appuc_rtz_0_interrupts;
    __IO halprop_reg32 masked_appuc_rtz_1_interrupts;
    __IO halprop_reg32 masked_appuc_secure_element_interrupts;
    __IO halprop_reg32 masked_appuc_spi_m_interrupts;
    __IO halprop_reg32 masked_appuc_spi_sl_interrupts;
    __IO halprop_reg32 masked_appuc_ssp_done_interrupt;
    __IO halprop_reg32 masked_appuc_standby_interrupts;
    halprop_reg32 reserved_3[1];
    __IO halprop_reg32 masked_appuc_timerxl_0_interrupts;
    __IO halprop_reg32 masked_appuc_timerxl_1_interrupts;
    __IO halprop_reg32 masked_appuc_timerxl_2_interrupts;
    __IO halprop_reg32 masked_appuc_timerxl_3_interrupts;
    __IO halprop_reg32 masked_appuc_timerxl_4_interrupts;
    __IO halprop_reg32 masked_appuc_timerxl_5_interrupts;
    __IO halprop_reg32 masked_appuc_uart_0_interrupts;
    __IO halprop_reg32 masked_appuc_uart_1_interrupts;
    __IO halprop_reg32 masked_appuc_uart_2_interrupts;
    __IO halprop_reg32 masked_appuc_usb_interrupts;
    __IO halprop_reg32 masked_appuc_watchdog_timer_expired_interrupt;
    __IO halprop_reg32 masked_ext_akuma_main_interrupt;
    __IO halprop_reg32 masked_ext_ipc_interrupts;
    __IO halprop_reg32 masked_ext_standby_interrupts;
    __IO halprop_reg32 mask_appuc_akuma_main_interrupt;
    __IO halprop_reg32 mask_appuc_asp_0_interrupts;
    __IO halprop_reg32 mask_appuc_asp_1_interrupts;
    __IO halprop_reg32 mask_appuc_bbpll_interrupts;
    __IO halprop_reg32 mask_appuc_bist_interrupts;
    __IO halprop_reg32 mask_appuc_dcdc_interrupts;
    __IO halprop_reg32 mask_appuc_debug_debug_interrupt;
    __IO halprop_reg32 mask_appuc_dma_dcb_interrupts;
    __IO halprop_reg32 mask_appuc_dma_scb_interrupts_0;
    __IO halprop_reg32 mask_appuc_dma_scb_cpy_err_interrupt;
    __IO halprop_reg32 mask_appuc_es_interrupts;
    __IO halprop_reg32 mask_appuc_gp_adc_interrupts;
    __IO halprop_reg32 mask_appuc_gpio_0_interrupts;
    __IO halprop_reg32 mask_appuc_gpio_1_interrupts;
    __IO halprop_reg32 mask_appuc_hr_adc_interrupts;
    __IO halprop_reg32 mask_appuc_i2c_m_0_interrupts;
    __IO halprop_reg32 mask_appuc_i2c_m_1_interrupts;
    __IO halprop_reg32 mask_appuc_i2c_sl_interrupts;
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
    __IO halprop_reg32 mask_appuc_i2s_0_interrupts;
#else //['itwosperipheral']
    halprop_reg32 reserved_4[1];
#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
    __IO halprop_reg32 mask_appuc_i2s_1_interrupts;
#else //['itwosperipheral']
    halprop_reg32 reserved_5[1];
#endif //['itwosperipheral']
    __IO halprop_reg32 mask_appuc_ipc_interrupts;
    __IO halprop_reg32 mask_appuc_ir_interrupts;
    __IO halprop_reg32 mask_appuc_keypad_scan_done_interrupt;
    __IO halprop_reg32 mask_appuc_pmud_lpcmp_match_interrupt;
    halprop_reg32 reserved_6[1];
    __IO halprop_reg32 mask_appuc_pwmxl_interrupts;
    __IO halprop_reg32 mask_appuc_qspi_m_interrupts;
    __IO halprop_reg32 mask_appuc_rtz_0_interrupts;
    __IO halprop_reg32 mask_appuc_rtz_1_interrupts;
    __IO halprop_reg32 mask_appuc_secure_element_interrupts;
    __IO halprop_reg32 mask_appuc_spi_m_interrupts;
    __IO halprop_reg32 mask_appuc_spi_sl_interrupts;
    __IO halprop_reg32 mask_appuc_ssp_done_interrupt;
    __IO halprop_reg32 mask_appuc_standby_interrupts;
    halprop_reg32 reserved_7[1];
    __IO halprop_reg32 mask_appuc_timerxl_0_interrupts;
    __IO halprop_reg32 mask_appuc_timerxl_1_interrupts;
    __IO halprop_reg32 mask_appuc_timerxl_2_interrupts;
    __IO halprop_reg32 mask_appuc_timerxl_3_interrupts;
    __IO halprop_reg32 mask_appuc_timerxl_4_interrupts;
    __IO halprop_reg32 mask_appuc_timerxl_5_interrupts;
    __IO halprop_reg32 mask_appuc_uart_0_interrupts;
    __IO halprop_reg32 mask_appuc_uart_1_interrupts;
    __IO halprop_reg32 mask_appuc_uart_2_interrupts;
    __IO halprop_reg32 mask_appuc_usb_interrupts;
    __IO halprop_reg32 mask_appuc_watchdog_timer_expired_interrupt;
    __IO halprop_reg32 mask_ext_akuma_main_interrupt;
    __IO halprop_reg32 mask_ext_ipc_interrupts;
    __IO halprop_reg32 mask_ext_standby_interrupts;
} INT_CTRL_REGMAP_T;

#define INT_CTRL_REGMAP ((INT_CTRL_REGMAP_T *) GP_WB_INT_CTRL_BASE_ADDRESS)

/***************************
 * regmap: gpio
 ***************************/
typedef struct GPIO_REGMAP_S
{
    __IO halprop_reg32 cfg;
    __IO halprop_reg32 gpio_directions;
    __IO halprop_reg32 gpio_input_values;
    __IO halprop_reg32 gpio_output_values;
    __IO halprop_reg32 clear_interrupts;
    __IO halprop_reg32 interrupt_levels;
    __IO halprop_reg32 interrupt_status;
    __IO halprop_reg32 interrupt_types;
    __IO halprop_reg32 set_interrupts;
    __IO halprop_reg32 timestamp_0;
    __IO halprop_reg32 timestamp_0_gpio_src;
    __IO halprop_reg32 timestamp_1;
    __IO halprop_reg32 timestamp_1_gpio_src;
    __IO halprop_reg32 timestamp_fifo_status;
    __IO halprop_reg32 quic_trigger_gpio_0;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_trigger_gpio_1;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 quic_trigger_gpio_2;
    __IO halprop_reg32 quic_cfg_2;
    __IO halprop_reg32 quic_trigger_gpio_3;
    __IO halprop_reg32 quic_cfg_3;
    __IO halprop_reg32 quic_trigger_gpio_4;
    __IO halprop_reg32 quic_cfg_4;
    __IO halprop_reg32 quic_trigger_gpio_5;
    __IO halprop_reg32 quic_cfg_5;
    __IO halprop_reg32 quic_trigger_gpio_6;
    __IO halprop_reg32 quic_cfg_6;
    __IO halprop_reg32 quic_trigger_gpio_7;
    __IO halprop_reg32 quic_cfg_7;
    __IO halprop_reg32 quic_trigger_gpio_8;
    __IO halprop_reg32 quic_cfg_8;
    __IO halprop_reg32 quic_trigger_gpio_9;
    __IO halprop_reg32 quic_cfg_9;
    __IO halprop_reg32 quic_trigger_gpio_10;
    __IO halprop_reg32 quic_cfg_10;
    __IO halprop_reg32 quic_trigger_gpio_11;
    __IO halprop_reg32 quic_cfg_11;
    __IO halprop_reg32 quic_trigger_gpio_12;
    __IO halprop_reg32 quic_cfg_12;
    __IO halprop_reg32 quic_trigger_gpio_13;
    __IO halprop_reg32 quic_cfg_13;
    __IO halprop_reg32 quic_trigger_gpio_14;
    __IO halprop_reg32 quic_cfg_14;
    __IO halprop_reg32 quic_trigger_gpio_15;
    __IO halprop_reg32 quic_cfg_15;
} GPIO_REGMAP_T;

#define GPIO_REGMAP ((GPIO_REGMAP_T *) GP_WB_GPIO_BASE_ADDRESS)

/***************************
 * regmap: iob
 ***************************/
typedef struct IOB_REGMAP_S
{
    __IO halprop_reg32 gpio_drive_strength_0;
    __IO halprop_reg32 gpio_drive_strength_1;
    __IO halprop_reg32 gpio_input_buffer_enable;
    __IO halprop_reg32 gpio_pull_enable;
    __IO halprop_reg32 gpio_pull_mode;
    __IO halprop_reg32 gpio_schmitt_trigger;
    __IO halprop_reg32 gpio_slew_rate;
    __IO halprop_reg32 gpio_0_alternate_cfg;
    __IO halprop_reg32 gpio_1_alternate_cfg;
    __IO halprop_reg32 gpio_2_alternate_cfg;
    __IO halprop_reg32 gpio_3_alternate_cfg;
    __IO halprop_reg32 gpio_4_alternate_cfg;
    __IO halprop_reg32 gpio_5_alternate_cfg;
    __IO halprop_reg32 gpio_6_alternate_cfg;
    __IO halprop_reg32 gpio_7_alternate_cfg;
    __IO halprop_reg32 gpio_8_alternate_cfg;
    __IO halprop_reg32 gpio_9_alternate_cfg;
    __IO halprop_reg32 gpio_10_alternate_cfg;
    __IO halprop_reg32 gpio_11_alternate_cfg;
    __IO halprop_reg32 gpio_12_alternate_cfg;
    __IO halprop_reg32 gpio_13_alternate_cfg;
    __IO halprop_reg32 gpio_14_alternate_cfg;
    __IO halprop_reg32 gpio_15_alternate_cfg;
    __IO halprop_reg32 gpio_16_alternate_cfg;
    __IO halprop_reg32 gpio_17_alternate_cfg;
    __IO halprop_reg32 gpio_18_alternate_cfg;
    __IO halprop_reg32 gpio_19_alternate_cfg;
    __IO halprop_reg32 gpio_20_alternate_cfg;
    __IO halprop_reg32 gpio_21_alternate_cfg;
    __IO halprop_reg32 gpio_22_alternate_cfg;
    __IO halprop_reg32 gpio_23_alternate_cfg;
    __IO halprop_reg32 gpio_24_alternate_cfg;
    __IO halprop_reg32 gpio_25_alternate_cfg;
    __IO halprop_reg32 gpio_26_alternate_cfg;
    __IO halprop_reg32 gpio_27_alternate_cfg;
    __IO halprop_reg32 gpio_28_alternate_cfg;
    __IO halprop_reg32 gpio_29_alternate_cfg;
    __IO halprop_reg32 irled_cfg;
} IOB_REGMAP_T;

#define IOB_REGMAP ((IOB_REGMAP_T *) GP_WB_IOB_BASE_ADDRESS)

/***************************
 * regmap: uart_0
 ***************************/
typedef struct UART_0_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 rx_not_busy;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} UART_0_REGMAP_T;

#define UART_0_REGMAP ((UART_0_REGMAP_T *) GP_WB_UART_0_BASE_ADDRESS)

/***************************
 * regmap: uart_1
 ***************************/
typedef struct UART_1_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 rx_not_busy;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} UART_1_REGMAP_T;

#define UART_1_REGMAP ((UART_1_REGMAP_T *) GP_WB_UART_1_BASE_ADDRESS)

/***************************
 * regmap: uart_2
 ***************************/
typedef struct UART_2_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 rx_not_busy;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} UART_2_REGMAP_T;

#define UART_2_REGMAP ((UART_2_REGMAP_T *) GP_WB_UART_2_BASE_ADDRESS)

/***************************
 * regmap: rtz_0
 ***************************/
typedef struct RTZ_0_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 init_random_seed;
    __IO halprop_reg32 cfg;
    __IO halprop_reg32 zero_timing;
    __IO halprop_reg32 one_timing;
    __IO halprop_reg32 stop_timing;
    __IO halprop_reg32 reset_timing;
    __IO halprop_reg32 jitter_cfg;
    __IO halprop_reg32 data_bits;
    __IO halprop_reg32 pixels;
    __IO halprop_reg32 trailing_data_bits;
    __IO halprop_reg32 sequence_busy;
    __IO halprop_reg32 repeat_sequence;
    __IO halprop_reg32 triggers;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 quic_cfg;
} RTZ_0_REGMAP_T;

#define RTZ_0_REGMAP ((RTZ_0_REGMAP_T *) GP_WB_RTZ_0_BASE_ADDRESS)

/***************************
 * regmap: rtz_1
 ***************************/
typedef struct RTZ_1_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 init_random_seed;
    __IO halprop_reg32 cfg;
    __IO halprop_reg32 zero_timing;
    __IO halprop_reg32 one_timing;
    __IO halprop_reg32 stop_timing;
    __IO halprop_reg32 reset_timing;
    __IO halprop_reg32 jitter_cfg;
    __IO halprop_reg32 data_bits;
    __IO halprop_reg32 pixels;
    __IO halprop_reg32 trailing_data_bits;
    __IO halprop_reg32 sequence_busy;
    __IO halprop_reg32 repeat_sequence;
    __IO halprop_reg32 triggers;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 quic_cfg;
} RTZ_1_REGMAP_T;

#define RTZ_1_REGMAP ((RTZ_1_REGMAP_T *) GP_WB_RTZ_1_BASE_ADDRESS)

/***************************
 * regmap: asp_0
 ***************************/
typedef struct ASP_0_REGMAP_S
{
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 pcm_conf;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 unnamed_reg_asp_0_0x0010;
    __IO halprop_reg32 unnamed_reg_asp_0_0x0014;
    __IO halprop_reg32 clk_2m_enable;
    __IO halprop_reg32 cic_diagnostics_clr;
    __IO halprop_reg32 cic_diagnostics_status;
} ASP_0_REGMAP_T;

#define ASP_0_REGMAP ((ASP_0_REGMAP_T *) GP_WB_ASP_0_BASE_ADDRESS)

/***************************
 * regmap: asp_1
 ***************************/
typedef struct ASP_1_REGMAP_S
{
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 pcm_conf;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 unnamed_reg_asp_1_0x0010;
    __IO halprop_reg32 unnamed_reg_asp_1_0x0014;
    __IO halprop_reg32 clk_2m_enable;
    __IO halprop_reg32 cic_diagnostics_clr;
    __IO halprop_reg32 cic_diagnostics_status;
} ASP_1_REGMAP_T;

#define ASP_1_REGMAP ((ASP_1_REGMAP_T *) GP_WB_ASP_1_BASE_ADDRESS)

/***************************
 * regmap: spi_sl
 ***************************/
typedef struct SPI_SL_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 clr_fifos;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} SPI_SL_REGMAP_T;

#define SPI_SL_REGMAP ((SPI_SL_REGMAP_T *) GP_WB_SPI_SL_BASE_ADDRESS)

/***************************
 * regmap: spi_m
 ***************************/
typedef struct SPI_M_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_rx_overrun_interrupt;
} SPI_M_REGMAP_T;

#define SPI_M_REGMAP ((SPI_M_REGMAP_T *) GP_WB_SPI_M_BASE_ADDRESS)

/***************************
 * regmap: qspi_m
 ***************************/
typedef struct QSPI_M_REGMAP_S
{
    __IO halprop_reg32 cmd_repetitions;
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 cmd_config;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 sclk_config;
    __IO halprop_reg32 rx_config;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_rx_overrun_interrupt;
    __IO halprop_reg32 io_default;
} QSPI_M_REGMAP_T;

#define QSPI_M_REGMAP ((QSPI_M_REGMAP_T *) GP_WB_QSPI_M_BASE_ADDRESS)

/***************************
 * regmap: ir
 ***************************/
typedef struct IR_REGMAP_S
{
    __IO halprop_reg32 prescale_div;
    __IO halprop_reg32 threshold;
    __IO halprop_reg32 wrap;
    __IO halprop_reg32 alt_threshold;
    __IO halprop_reg32 alt_wrap;
    __IO halprop_reg32 carrier_tu;
    __IO halprop_reg32 timer_tu;
    __IO halprop_reg32 modulation_control_0;
    __IO halprop_reg32 modulation_control_1;
    __IO halprop_reg32 ram_sequence_start_ptr;
    __IO halprop_reg32 ram_sequence_len;
    __IO halprop_reg32 ram_sequence_match_index;
    __IO halprop_reg32 modulation_triggers;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 modulation_status;
    __IO halprop_reg32 sequence_idx;
    __IO halprop_reg32 io_control;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 quic_cfg_2;
} IR_REGMAP_T;

#define IR_REGMAP ((IR_REGMAP_T *) GP_WB_IR_BASE_ADDRESS)

/***************************
 * regmap: i2c_m_0
 ***************************/
typedef struct I2C_M_0_REGMAP_S
{
    __IO halprop_reg32 prescaler;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 control;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
} I2C_M_0_REGMAP_T;

#define I2C_M_0_REGMAP ((I2C_M_0_REGMAP_T *) GP_WB_I2C_M_0_BASE_ADDRESS)

/***************************
 * regmap: i2c_m_1
 ***************************/
typedef struct I2C_M_1_REGMAP_S
{
    __IO halprop_reg32 prescaler;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 control;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
} I2C_M_1_REGMAP_T;

#define I2C_M_1_REGMAP ((I2C_M_1_REGMAP_T *) GP_WB_I2C_M_1_BASE_ADDRESS)

/***************************
 * regmap: i2c_sl
 ***************************/
typedef struct I2C_SL_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 slave_address;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 status;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} I2C_SL_REGMAP_T;

#define I2C_SL_REGMAP ((I2C_SL_REGMAP_T *) GP_WB_I2C_SL_BASE_ADDRESS)

#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
/***************************
 * regmap: i2s_0
 ***************************/
typedef struct I2S_0_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 config_0;
    __IO halprop_reg32 config_1;
    __IO halprop_reg32 unmasked_interrupts;
} I2S_0_REGMAP_T;

#define I2S_0_REGMAP ((I2S_0_REGMAP_T *) GP_WB_I2S_0_BASE_ADDRESS)

#endif //['itwosperipheral']
#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
/***************************
 * regmap: i2s_1
 ***************************/
typedef struct I2S_1_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 config_0;
    __IO halprop_reg32 config_1;
    __IO halprop_reg32 unmasked_interrupts;
} I2S_1_REGMAP_T;

#define I2S_1_REGMAP ((I2S_1_REGMAP_T *) GP_WB_I2S_1_BASE_ADDRESS)

#endif //['itwosperipheral']
/***************************
 * regmap: dmas_scb
 ***************************/
typedef struct DMAS_SCB_REGMAP_S
{
    __IO halprop_reg32 src_addr_0;
    __IO halprop_reg32 dest_addr_0;
    __IO halprop_reg32 buffer_size_0;
    __IO halprop_reg32 buffer_almost_complete_threshold_0;
    __IO halprop_reg32 buffer_ptr_value_0;
    __IO halprop_reg32 buffer_ptr_wrap_value_0;
    __IO halprop_reg32 conf_0;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_0;
    __IO halprop_reg32 circular_buffer_select_0;
    __IO halprop_reg32 buffer_ptr_control_0;
    __IO halprop_reg32 clr_buffer_complete_interrupt_0;
    __IO halprop_reg32 internal_ptr_status_0;
    __IO halprop_reg32 status_0;
    __IO halprop_reg32 src_addr_1;
    __IO halprop_reg32 dest_addr_1;
    __IO halprop_reg32 buffer_size_1;
    __IO halprop_reg32 buffer_almost_complete_threshold_1;
    __IO halprop_reg32 buffer_ptr_value_1;
    __IO halprop_reg32 buffer_ptr_wrap_value_1;
    __IO halprop_reg32 conf_1;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_1;
    __IO halprop_reg32 circular_buffer_select_1;
    __IO halprop_reg32 buffer_ptr_control_1;
    __IO halprop_reg32 clr_buffer_complete_interrupt_1;
    __IO halprop_reg32 internal_ptr_status_1;
    __IO halprop_reg32 status_1;
    __IO halprop_reg32 src_addr_2;
    __IO halprop_reg32 dest_addr_2;
    __IO halprop_reg32 buffer_size_2;
    __IO halprop_reg32 buffer_almost_complete_threshold_2;
    __IO halprop_reg32 buffer_ptr_value_2;
    __IO halprop_reg32 buffer_ptr_wrap_value_2;
    __IO halprop_reg32 conf_2;
    __IO halprop_reg32 quic_cfg_2;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_2;
    __IO halprop_reg32 circular_buffer_select_2;
    __IO halprop_reg32 buffer_ptr_control_2;
    __IO halprop_reg32 clr_buffer_complete_interrupt_2;
    __IO halprop_reg32 internal_ptr_status_2;
    __IO halprop_reg32 status_2;
    __IO halprop_reg32 src_addr_3;
    __IO halprop_reg32 dest_addr_3;
    __IO halprop_reg32 buffer_size_3;
    __IO halprop_reg32 buffer_almost_complete_threshold_3;
    __IO halprop_reg32 buffer_ptr_value_3;
    __IO halprop_reg32 buffer_ptr_wrap_value_3;
    __IO halprop_reg32 conf_3;
    __IO halprop_reg32 quic_cfg_3;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_3;
    __IO halprop_reg32 circular_buffer_select_3;
    __IO halprop_reg32 buffer_ptr_control_3;
    __IO halprop_reg32 clr_buffer_complete_interrupt_3;
    __IO halprop_reg32 internal_ptr_status_3;
    __IO halprop_reg32 status_3;
    __IO halprop_reg32 src_addr_4;
    __IO halprop_reg32 dest_addr_4;
    __IO halprop_reg32 buffer_size_4;
    __IO halprop_reg32 buffer_almost_complete_threshold_4;
    __IO halprop_reg32 buffer_ptr_value_4;
    __IO halprop_reg32 buffer_ptr_wrap_value_4;
    __IO halprop_reg32 conf_4;
    __IO halprop_reg32 quic_cfg_4;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_4;
    __IO halprop_reg32 circular_buffer_select_4;
    __IO halprop_reg32 buffer_ptr_control_4;
    __IO halprop_reg32 clr_buffer_complete_interrupt_4;
    __IO halprop_reg32 internal_ptr_status_4;
    __IO halprop_reg32 status_4;
    __IO halprop_reg32 src_addr_5;
    __IO halprop_reg32 dest_addr_5;
    __IO halprop_reg32 buffer_size_5;
    __IO halprop_reg32 buffer_almost_complete_threshold_5;
    __IO halprop_reg32 buffer_ptr_value_5;
    __IO halprop_reg32 buffer_ptr_wrap_value_5;
    __IO halprop_reg32 conf_5;
    __IO halprop_reg32 quic_cfg_5;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_5;
    __IO halprop_reg32 circular_buffer_select_5;
    __IO halprop_reg32 buffer_ptr_control_5;
    __IO halprop_reg32 clr_buffer_complete_interrupt_5;
    __IO halprop_reg32 internal_ptr_status_5;
    __IO halprop_reg32 status_5;
    __IO halprop_reg32 src_addr_6;
    __IO halprop_reg32 dest_addr_6;
    __IO halprop_reg32 buffer_size_6;
    __IO halprop_reg32 buffer_almost_complete_threshold_6;
    __IO halprop_reg32 buffer_ptr_value_6;
    __IO halprop_reg32 buffer_ptr_wrap_value_6;
    __IO halprop_reg32 conf_6;
    __IO halprop_reg32 quic_cfg_6;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_6;
    __IO halprop_reg32 circular_buffer_select_6;
    __IO halprop_reg32 buffer_ptr_control_6;
    __IO halprop_reg32 clr_buffer_complete_interrupt_6;
    __IO halprop_reg32 internal_ptr_status_6;
    __IO halprop_reg32 status_6;
    __IO halprop_reg32 src_addr_7;
    __IO halprop_reg32 dest_addr_7;
    __IO halprop_reg32 buffer_size_7;
    __IO halprop_reg32 buffer_almost_complete_threshold_7;
    __IO halprop_reg32 buffer_ptr_value_7;
    __IO halprop_reg32 buffer_ptr_wrap_value_7;
    __IO halprop_reg32 conf_7;
    __IO halprop_reg32 quic_cfg_7;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_7;
    __IO halprop_reg32 circular_buffer_select_7;
    __IO halprop_reg32 buffer_ptr_control_7;
    __IO halprop_reg32 clr_buffer_complete_interrupt_7;
    __IO halprop_reg32 internal_ptr_status_7;
    __IO halprop_reg32 status_7;
    __IO halprop_reg32 src_addr_8;
    __IO halprop_reg32 dest_addr_8;
    __IO halprop_reg32 buffer_size_8;
    __IO halprop_reg32 buffer_almost_complete_threshold_8;
    __IO halprop_reg32 buffer_ptr_value_8;
    __IO halprop_reg32 buffer_ptr_wrap_value_8;
    __IO halprop_reg32 conf_8;
    __IO halprop_reg32 quic_cfg_8;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_8;
    __IO halprop_reg32 circular_buffer_select_8;
    __IO halprop_reg32 buffer_ptr_control_8;
    __IO halprop_reg32 clr_buffer_complete_interrupt_8;
    __IO halprop_reg32 internal_ptr_status_8;
    __IO halprop_reg32 status_8;
    __IO halprop_reg32 src_addr_9;
    __IO halprop_reg32 dest_addr_9;
    __IO halprop_reg32 buffer_size_9;
    __IO halprop_reg32 buffer_almost_complete_threshold_9;
    __IO halprop_reg32 buffer_ptr_value_9;
    __IO halprop_reg32 buffer_ptr_wrap_value_9;
    __IO halprop_reg32 conf_9;
    __IO halprop_reg32 quic_cfg_9;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_9;
    __IO halprop_reg32 circular_buffer_select_9;
    __IO halprop_reg32 buffer_ptr_control_9;
    __IO halprop_reg32 clr_buffer_complete_interrupt_9;
    __IO halprop_reg32 internal_ptr_status_9;
    __IO halprop_reg32 status_9;
    __IO halprop_reg32 src_addr_10;
    __IO halprop_reg32 dest_addr_10;
    __IO halprop_reg32 buffer_size_10;
    __IO halprop_reg32 buffer_almost_complete_threshold_10;
    __IO halprop_reg32 buffer_ptr_value_10;
    __IO halprop_reg32 buffer_ptr_wrap_value_10;
    __IO halprop_reg32 conf_10;
    __IO halprop_reg32 quic_cfg_10;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_10;
    __IO halprop_reg32 circular_buffer_select_10;
    __IO halprop_reg32 buffer_ptr_control_10;
    __IO halprop_reg32 clr_buffer_complete_interrupt_10;
    __IO halprop_reg32 internal_ptr_status_10;
    __IO halprop_reg32 status_10;
    __IO halprop_reg32 src_addr_11;
    __IO halprop_reg32 dest_addr_11;
    __IO halprop_reg32 buffer_size_11;
    __IO halprop_reg32 buffer_almost_complete_threshold_11;
    __IO halprop_reg32 buffer_ptr_value_11;
    __IO halprop_reg32 buffer_ptr_wrap_value_11;
    __IO halprop_reg32 conf_11;
    __IO halprop_reg32 quic_cfg_11;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_11;
    __IO halprop_reg32 circular_buffer_select_11;
    __IO halprop_reg32 buffer_ptr_control_11;
    __IO halprop_reg32 clr_buffer_complete_interrupt_11;
    __IO halprop_reg32 internal_ptr_status_11;
    __IO halprop_reg32 status_11;
    __IO halprop_reg32 src_addr_12;
    __IO halprop_reg32 dest_addr_12;
    __IO halprop_reg32 buffer_size_12;
    __IO halprop_reg32 buffer_almost_complete_threshold_12;
    __IO halprop_reg32 buffer_ptr_value_12;
    __IO halprop_reg32 buffer_ptr_wrap_value_12;
    __IO halprop_reg32 conf_12;
    __IO halprop_reg32 quic_cfg_12;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_12;
    __IO halprop_reg32 circular_buffer_select_12;
    __IO halprop_reg32 buffer_ptr_control_12;
    __IO halprop_reg32 clr_buffer_complete_interrupt_12;
    __IO halprop_reg32 internal_ptr_status_12;
    __IO halprop_reg32 status_12;
    __IO halprop_reg32 src_addr_13;
    __IO halprop_reg32 dest_addr_13;
    __IO halprop_reg32 buffer_size_13;
    __IO halprop_reg32 buffer_almost_complete_threshold_13;
    __IO halprop_reg32 buffer_ptr_value_13;
    __IO halprop_reg32 buffer_ptr_wrap_value_13;
    __IO halprop_reg32 conf_13;
    __IO halprop_reg32 quic_cfg_13;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_13;
    __IO halprop_reg32 circular_buffer_select_13;
    __IO halprop_reg32 buffer_ptr_control_13;
    __IO halprop_reg32 clr_buffer_complete_interrupt_13;
    __IO halprop_reg32 internal_ptr_status_13;
    __IO halprop_reg32 status_13;
    __IO halprop_reg32 src_addr_14;
    __IO halprop_reg32 dest_addr_14;
    __IO halprop_reg32 buffer_size_14;
    __IO halprop_reg32 buffer_almost_complete_threshold_14;
    __IO halprop_reg32 buffer_ptr_value_14;
    __IO halprop_reg32 buffer_ptr_wrap_value_14;
    __IO halprop_reg32 conf_14;
    __IO halprop_reg32 quic_cfg_14;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_14;
    __IO halprop_reg32 circular_buffer_select_14;
    __IO halprop_reg32 buffer_ptr_control_14;
    __IO halprop_reg32 clr_buffer_complete_interrupt_14;
    __IO halprop_reg32 internal_ptr_status_14;
    __IO halprop_reg32 status_14;
    __IO halprop_reg32 src_addr_15;
    __IO halprop_reg32 dest_addr_15;
    __IO halprop_reg32 buffer_size_15;
    __IO halprop_reg32 buffer_almost_complete_threshold_15;
    __IO halprop_reg32 buffer_ptr_value_15;
    __IO halprop_reg32 buffer_ptr_wrap_value_15;
    __IO halprop_reg32 conf_15;
    __IO halprop_reg32 quic_cfg_15;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete_15;
    __IO halprop_reg32 circular_buffer_select_15;
    __IO halprop_reg32 buffer_ptr_control_15;
    __IO halprop_reg32 clr_buffer_complete_interrupt_15;
    __IO halprop_reg32 internal_ptr_status_15;
    __IO halprop_reg32 status_15;
    __IO halprop_reg32 channel_switch_latency;
    __IO halprop_reg32 clr_cpy_err_interrupt;
    __IO halprop_reg32 unmasked_cpy_err_interrupt;
} DMAS_SCB_REGMAP_T;

#define DMAS_SCB_REGMAP ((DMAS_SCB_REGMAP_T *) GP_WB_DMAS_SCB_BASE_ADDRESS)

/***************************
 * regmap: dmas_dcb
 ***************************/
typedef struct DMAS_DCB_REGMAP_S
{
    __IO halprop_reg32 src_addr_0;
    __IO halprop_reg32 dest_addr_0;
    __IO halprop_reg32 buffer_size_0;
    __IO halprop_reg32 src_write_ptr_0;
    __IO halprop_reg32 src_read_ptr_0;
    __IO halprop_reg32 dest_write_ptr_0;
    __IO halprop_reg32 dest_read_ptr_0;
    __IO halprop_reg32 conf_0;
    __IO halprop_reg32 reset_pointers_0;
    __IO halprop_reg32 unmasked_buffer_complete_0_interrupt;
    __IO halprop_reg32 quic_0_cfg_0;
    __IO halprop_reg32 quic_1_cfg_0;
    __IO halprop_reg32 quic_2_cfg_0;
    __IO halprop_reg32 channel_switch_latency;
    __IO halprop_reg32 clr_cpy_err_interrupt;
    __IO halprop_reg32 unmasked_cpy_err_interrupt;
} DMAS_DCB_REGMAP_T;

#define DMAS_DCB_REGMAP ((DMAS_DCB_REGMAP_T *) GP_WB_DMAS_DCB_BASE_ADDRESS)

/***************************
 * regmap: bist
 ***************************/
typedef struct BIST_REGMAP_S
{
    __IO halprop_reg32 adc_enable_continuous_measurement;
    __IO halprop_reg32 tsens_enable_continuous_measurement;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 scaler_wait_time;
    __IO halprop_reg32 scaler_ck_div;
    __IO halprop_reg32 adc_skip_samples;
    __IO halprop_reg32 adc_count;
    __IO halprop_reg32 adc_eoc_time;
    __IO halprop_reg32 adc_tsens_triggers;
    __IO halprop_reg32 adc_accu_result;
    __IO halprop_reg32 adc_tsens_status;
    __IO halprop_reg32 tsens_phase_length_cfg;
    __IO halprop_reg32 tsens_max_threshold;
    __IO halprop_reg32 tsens_min_threshold;
    __IO halprop_reg32 adc_result;
    __IO halprop_reg32 tsens_result;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 quic_cfg_2;
    __IO halprop_reg32 cp_ctrl_main_config;
    __IO halprop_reg32 cp_ctrl_time_config;
    __IO halprop_reg32 cp_ctrl_enabled;
    __IO halprop_reg32 obm_config;
    __IO halprop_reg32 obm_src_invert;
    __IO halprop_reg32 obm_thr_config;
    __IO halprop_reg32 obm_period_meas;
    __IO halprop_reg32 obm_total_meas;
    __IO halprop_reg32 obm_high_meas;
    __IO halprop_reg32 obm_status;
    __IO halprop_reg32 obm_triggers;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unnamed_reg_bist_0x0080;
    __IO halprop_reg32 unnamed_reg_bist_0x0084;
    __IO halprop_reg32 unnamed_reg_bist_0x0088;
    __IO halprop_reg32 unnamed_reg_bist_0x008c;
    __IO halprop_reg32 unnamed_reg_bist_0x0090;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_bist_0x009c;
    __IO halprop_reg32 unnamed_reg_bist_0x00a0;
} BIST_REGMAP_T;

#define BIST_REGMAP ((BIST_REGMAP_T *) GP_WB_BIST_BASE_ADDRESS)

/***************************
 * regmap: keypad
 ***************************/
typedef struct KEYPAD_REGMAP_S
{
    __IO halprop_reg32 keypad_config;
    __IO halprop_reg32 active_columns_charge_period;
    __IO halprop_reg32 passive_columns_charge_period;
    __IO halprop_reg32 row_drive_period;
    __IO halprop_reg32 keypad_state;
    __IO halprop_reg32 keypad_status;
    __IO halprop_reg32 keypad_scans;
    __IO halprop_reg32 keypad_senses_active_charge;
    __IO halprop_reg32 keypad_senses;
    __IO halprop_reg32 keypad_control;
    __IO halprop_reg32 results_stable_counter;
    __IO halprop_reg32 keypad_result_summary;
    __IO halprop_reg32 key_vector_0;
    __IO halprop_reg32 key_vector_1;
    __IO halprop_reg32 key_vector_2;
    __IO halprop_reg32 key_vector_3;
    __IO halprop_reg32 key_vector_4;
    __IO halprop_reg32 key_vector_5;
    __IO halprop_reg32 key_vector_6;
    __IO halprop_reg32 key_vector_7;
    __IO halprop_reg32 unmasked_scan_done_interrupt;
    __IO halprop_reg32 unnamed_reg_keypad_0x0054;
    __IO halprop_reg32 quic_cfg_0;
} KEYPAD_REGMAP_T;

#define KEYPAD_REGMAP ((KEYPAD_REGMAP_T *) GP_WB_KEYPAD_BASE_ADDRESS)

/***************************
 * regmap: gp_adc
 ***************************/
typedef struct GP_ADC_REGMAP_S
{
    __IO halprop_reg32 general_config;
    __IO halprop_reg32 input_mux_time_cfg;
    __IO halprop_reg32 eoc_time;
    __IO halprop_reg32 slot_enable;
    __IO halprop_reg32 slot_a_config;
    __IO halprop_reg32 slot_b_config;
    __IO halprop_reg32 slot_c_config;
    __IO halprop_reg32 slot_d_config;
    __IO halprop_reg32 preset_max_value;
    __IO halprop_reg32 preset_min_value;
    __IO halprop_reg32 mailbox_config;
    __IO halprop_reg32 action_triggers;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 fifo_result;
    __IO halprop_reg32 buffer_a_result;
    __IO halprop_reg32 buffer_b_result;
    __IO halprop_reg32 buffer_c_result;
    __IO halprop_reg32 general_status;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 unnamed_reg_gp_adc_0x0054;
    __IO halprop_reg32 result;
    __IO halprop_reg32 eoc;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_gp_adc_0x0068;
} GP_ADC_REGMAP_T;

#define GP_ADC_REGMAP ((GP_ADC_REGMAP_T *) GP_WB_GP_ADC_BASE_ADDRESS)

/***************************
 * regmap: hr_adc
 ***************************/
typedef struct HR_ADC_REGMAP_S
{
    __IO halprop_reg32 conf;
    __IO halprop_reg32 accu_count;
    __IO halprop_reg32 enable_continuous_measurement;
    __IO halprop_reg32 phase_length_cfg;
    __IO halprop_reg32 start_accu;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 result;
    __IO halprop_reg32 channel_conf;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_cfg_1;
    __IO halprop_reg32 unnamed_reg_hr_adc_0x0030;
    __IO halprop_reg32 unnamed_reg_hr_adc_0x0034;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_hr_adc_0x0040;
} HR_ADC_REGMAP_T;

#define HR_ADC_REGMAP ((HR_ADC_REGMAP_T *) GP_WB_HR_ADC_BASE_ADDRESS)

/***************************
 * regmap: cma
 ***************************/
typedef struct CMA_REGMAP_S
{
    __IO halprop_reg32 nvm_ctrl_config;
    __IO halprop_reg32 nvm_ctrl_pd_config;
    __IO halprop_reg32 nvm_ctrl_nap_config;
    __IO halprop_reg32 dpd_min_duration_cfg_0;
    __IO halprop_reg32 dpd_min_duration_cfg_1;
    __IO halprop_reg32 dpd_min_duration_cfg_2;
    __IO halprop_reg32 nvm_ctrl_force_on_cfg;
    __IO halprop_reg32 nvm_ctrl_words_to_write;
    __IO halprop_reg32 nvm_ctrl_status;
    __IO halprop_reg32 nvm_macro_ecc_max_err_cnt;
    __IO halprop_reg32 nvm_macro_clk_force_on;
    __IO halprop_reg32 nvm_macro_clr_ecc_max_err_cnt;
    __IO halprop_reg32 mri_nvm_address;
    __IO halprop_reg32 mri_nvm_write_data_0;
    __IO halprop_reg32 mri_nvm_write_data_1;
    __IO halprop_reg32 mri_nvm_write_data_2;
    __IO halprop_reg32 mri_nvm_write_data_3;
    __IO halprop_reg32 mri_nvm_write_data_ecc;
    __IO halprop_reg32 mri_nvm_read_data_0;
    __IO halprop_reg32 mri_nvm_read_data_1;
    __IO halprop_reg32 mri_nvm_read_data_2;
    __IO halprop_reg32 mri_nvm_read_data_3;
    __IO halprop_reg32 mri_nvm_read_data_ecc;
    __IO halprop_reg32 mri_nvm_status;
    __IO halprop_reg32 mri_nvm_cmd_trigger_ctrl;
    __IO halprop_reg32 mri_nvm_config_0;
    __IO halprop_reg32 mri_nvm_cmd;
    __IO halprop_reg32 nvm_bist_mode;
    __IO halprop_reg32 nvm_pbi_cmd;
    __IO halprop_reg32 nvm_pbi_addr;
    __IO halprop_reg32 unnamed_reg_cma_0x0078;
    __IO halprop_reg32 nvm_pbi_read_data_4;
    __IO halprop_reg32 nvm_pbi_access_status;
    __IO halprop_reg32 unnamed_reg_cma_0x0084;
    __IO halprop_reg32 nvm_pbi_write_data_4;
    __IO halprop_reg32 nvm_pbi_clr_interrupt;
    __IO halprop_reg32 nvm_power_ctrl_state;
    __IO halprop_reg32 nvm_port_buffer_invalidate;
    __IO halprop_reg32 nvm_access_err;
    __IO halprop_reg32 access_err_clr;
    __IO halprop_reg32 rom_cfg;
    __IO halprop_reg32 rom_bist_start;
    __IO halprop_reg32 rom_status;
    __IO halprop_reg32 rom_bist_crc;
    __IO halprop_reg32 unnamed_reg_cma_0x00b0;
} CMA_REGMAP_T;

#define CMA_REGMAP ((CMA_REGMAP_T *) GP_WB_CMA_BASE_ADDRESS)

/***************************
 * regmap: msi
 ***************************/
typedef struct MSI_REGMAP_S
{
    __IO halprop_reg32 conf;
} MSI_REGMAP_T;

#define MSI_REGMAP ((MSI_REGMAP_T *) GP_WB_MSI_BASE_ADDRESS)

/***************************
 * regmap: debug
 ***************************/
typedef struct DEBUG_REGMAP_S
{
    __IO halprop_reg32 interrupt;
    __IO halprop_reg32 debug_sel_0;
    __IO halprop_reg32 debug_sel_1;
    __IO halprop_reg32 debug_sel_2;
    __IO halprop_reg32 debug_sel_3;
    __IO halprop_reg32 debug_sel_4;
    __IO halprop_reg32 debug_sel_5;
    __IO halprop_reg32 debug_status;
} DEBUG_REGMAP_T;

#define DEBUG_REGMAP ((DEBUG_REGMAP_T *) GP_WB_DEBUG_BASE_ADDRESS)

/***************************
 * regmap: bbpll
 ***************************/
typedef struct BBPLL_REGMAP_S
{
    __IO halprop_reg32 f_estimation_duration;
    __IO halprop_reg32 dco_coarse_new;
    __IO halprop_reg32 dco_tune_during_open_loop;
    __IO halprop_reg32 dco_tune_overrule_settings;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0010;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0014;
    __IO halprop_reg32 freq_out_of_range_fe_min;
    __IO halprop_reg32 freq_out_of_range_fe_max;
    __IO halprop_reg32 freq_out_of_range_timeout;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0024;
    __IO halprop_reg32 phase_detect_nochange_decrement_value;
    __IO halprop_reg32 out_of_range_timeout_value;
    __IO halprop_reg32 increased_bandwidth_timeout_limit_1;
    __IO halprop_reg32 increased_bandwidth_timeout_limit_2;
    __IO halprop_reg32 increased_bandwidth_timeout_limit_3;
    __IO halprop_reg32 mask_bandwidth_increase_during_foor;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0040;
    __IO halprop_reg32 loop_and_fe_config;
    __IO halprop_reg32 loop_enable;
    __IO halprop_reg32 unnamed_reg_bbpll_0x004c;
    __IO halprop_reg32 clk_phase;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0054;
    __IO halprop_reg32 fe_result;
    __IO halprop_reg32 loop_is_locked_raw_status;
    __IO halprop_reg32 freq_out_of_range_cnt;
    __IO halprop_reg32 tune_out_of_range_cnt;
    __IO halprop_reg32 dco_coarse;
    __IO halprop_reg32 dco_tune;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0074;
    __IO halprop_reg32 unmasked_interrupt_status;
    __IO halprop_reg32 unnamed_reg_bbpll_0x007c;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0080;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0084;
    __IO halprop_reg32 unnamed_reg_bbpll_0x0088;
} BBPLL_REGMAP_T;

#define BBPLL_REGMAP ((BBPLL_REGMAP_T *) GP_WB_BBPLL_BASE_ADDRESS)

/***************************
 * regmap: xo32m
 ***************************/
typedef struct XO32M_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_xo32m_0x0000;
    __IO halprop_reg32 unnamed_reg_xo32m_0x0004;
    __IO halprop_reg32 unnamed_reg_xo32m_0x0008;
    __IO halprop_reg32 unnamed_reg_xo32m_0x000c;
} XO32M_REGMAP_T;

#define XO32M_REGMAP ((XO32M_REGMAP_T *) GP_WB_XO32M_BASE_ADDRESS)

/***************************
 * regmap: fract_clk_gen
 ***************************/
typedef struct FRACT_CLK_GEN_REGMAP_S
{
    __IO halprop_reg32 t_target_0;
    __IO halprop_reg32 enable_fract_clk_0;
    __IO halprop_reg32 t_target_1;
    __IO halprop_reg32 enable_fract_clk_1;
} FRACT_CLK_GEN_REGMAP_T;

#define FRACT_CLK_GEN_REGMAP ((FRACT_CLK_GEN_REGMAP_T *) GP_WB_FRACT_CLK_GEN_BASE_ADDRESS)

/***************************
 * regmap: dcdc
 ***************************/
typedef struct DCDC_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_dcdc_0x0000;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0004;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0008;
    __IO halprop_reg32 unnamed_reg_dcdc_0x000c;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0010;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0014;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0018;
    __IO halprop_reg32 unnamed_reg_dcdc_0x001c;
    __IO halprop_reg32 y1_min;
    __IO halprop_reg32 y2_min;
    __IO halprop_reg32 y1_max;
    __IO halprop_reg32 y2_max;
    __IO halprop_reg32 y2_cal;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0034;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0038;
    __IO halprop_reg32 n1_skip;
    __IO halprop_reg32 n2_skip;
    __IO halprop_reg32 n1_max;
    __IO halprop_reg32 n2_max;
    __IO halprop_reg32 n_count;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0050;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0054;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0058;
    __IO halprop_reg32 unnamed_reg_dcdc_0x005c;
    __IO halprop_reg32 zcd_reset_offset_value;
    __IO halprop_reg32 y1_preload_ena;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0068;
    __IO halprop_reg32 vfb2_count_max;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0070;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0074;
    __IO halprop_reg32 sw_width_during_cal;
    __IO halprop_reg32 ton_dac_overrule;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0080;
    __IO halprop_reg32 fw_time;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0088;
    __IO halprop_reg32 unnamed_reg_dcdc_0x008c;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0090;
    __IO halprop_reg32 unnamed_reg_dcdc_0x0094;
    __IO halprop_reg32 start_zcd_cal;
    __IO halprop_reg32 clkp_length;
    __IO halprop_reg32 unnamed_reg_dcdc_0x00a0;
    __IO halprop_reg32 vfb_stable;
    __IO halprop_reg32 zcd_cal_ready;
    __IO halprop_reg32 dcdc_zcd_offset;
    __IO halprop_reg32 unnamed_reg_dcdc_0x00b0;
    __IO halprop_reg32 unnamed_reg_dcdc_0x00b4;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 mode_cfg;
    __IO halprop_reg32 default_mode_cfg;
    __IO halprop_reg32 rx_mode_cfg;
    __IO halprop_reg32 tx_mode_cfg;
    __IO halprop_reg32 unnamed_reg_dcdc_0x00d0;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_dcdc_0x00dc;
} DCDC_REGMAP_T;

#define DCDC_REGMAP ((DCDC_REGMAP_T *) GP_WB_DCDC_BASE_ADDRESS)

/***************************
 * regmap: pmud
 ***************************/
typedef struct PMUD_REGMAP_S
{
    __IO halprop_reg32 standby_ctrl_settings;
    __IO halprop_reg32 standby_power_mode_settings;
    __IO halprop_reg32 active_power_mode_settings;
    __IO halprop_reg32 soft_por_triggers;
    __IO halprop_reg32 unnamed_reg_pmud_0x0010;
    __IO halprop_reg32 rtc_timer_src;
    __IO halprop_reg32 rtc_compare_level_set;
    __IO halprop_reg32 unnamed_reg_pmud_0x001c;
    __IO halprop_reg32 rtc_timer;
    __IO halprop_reg32 rtc_compare_event;
    __IO halprop_reg32 rtc_active_compare_level;
    __IO halprop_reg32 unnamed_reg_pmud_0x002c;
    __IO halprop_reg32 dcdc_ret_en;
    __IO halprop_reg32 dcdc_ton_dac_default;
    __IO halprop_reg32 unnamed_reg_pmud_0x0038;
    __IO halprop_reg32 dcdc_wakeup_delay;
    __IO halprop_reg32 dcdc_ret_reduce_clock_dis;
    __IO halprop_reg32 dcdc_ret_en_i;
    __IO halprop_reg32 unnamed_reg_pmud_0x0048;
    __IO halprop_reg32 bootreg_write_val;
    __IO halprop_reg32 bootreg;
    __IO halprop_reg32 tamper_por_src;
    __IO halprop_reg32 wakeup_pin_modes_0;
    __IO halprop_reg32 wakeup_pin_modes_1;
    __IO halprop_reg32 unnamed_reg_pmud_0x0060;
    __IO halprop_reg32 gpio_rising_edge_events_0;
    __IO halprop_reg32 gpio_falling_edge_events_0;
    __IO halprop_reg32 vddram_retain;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_pmud_0x0078;
    __IO halprop_reg32 unnamed_reg_pmud_0x007c;
    __IO halprop_reg32 unnamed_reg_pmud_0x0080;
    __IO halprop_reg32 unnamed_reg_pmud_0x0084;
    __IO halprop_reg32 vcore_ldo_refbits;
    __IO halprop_reg32 vddram_lpr_tune_settings;
} PMUD_REGMAP_T;

#define PMUD_REGMAP ((PMUD_REGMAP_T *) GP_WB_PMUD_BASE_ADDRESS)

/***************************
 * regmap: sens_ao
 ***************************/
typedef struct SENS_AO_REGMAP_S
{
    __IO halprop_reg32 lpcmp_config_a_slot_0;
    __IO halprop_reg32 lpcmp_config_a_slot_1;
    __IO halprop_reg32 lpcmp_config_b_slot_0_1;
    __IO halprop_reg32 lpcmp_config_a_slot_2;
    __IO halprop_reg32 lpcmp_config_a_slot_3;
    __IO halprop_reg32 lpcmp_config_b_slot_2_3;
    __IO halprop_reg32 lpcmp_status;
    __IO halprop_reg32 lpcmp_general_config;
    __IO halprop_reg32 unnamed_reg_sens_ao_0x0020;
    __IO halprop_reg32 unnamed_reg_sens_ao_0x0024;
    __IO halprop_reg32 sens_lpcmp_result;
} SENS_AO_REGMAP_T;

#define SENS_AO_REGMAP ((SENS_AO_REGMAP_T *) GP_WB_SENS_AO_BASE_ADDRESS)

/***************************
 * regmap: standby
 ***************************/
typedef struct STANDBY_REGMAP_S
{
    __IO halprop_reg32 version;
    __IO halprop_reg32 dummy_placeholder;
    __IO halprop_reg32 clk_control;
    __IO halprop_reg32 unnamed_reg_standby_0x000c;
    __IO halprop_reg32 xt32m_sleep_clk_enable;
    __IO halprop_reg32 unnamed_reg_standby_0x0014;
    __IO halprop_reg32 unnamed_reg_standby_0x0018;
    __IO halprop_reg32 nvm_cfg_during_active;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 unnamed_reg_standby_0x0028;
    __IO halprop_reg32 rtgts_threshold;
    __IO halprop_reg32 sp_ram_config;
    __IO halprop_reg32 rf_ram_config;
    __IO halprop_reg32 rtgts_sw_overrule;
    __IO halprop_reg32 rtgts_status;
    __IO halprop_reg32 clk_ena_0;
    __IO halprop_reg32 clk_ena_1;
    __IO halprop_reg32 clk_ena_2;
    __IO halprop_reg32 clk_ena_3;
    __IO halprop_reg32 fon_clk_0;
    __IO halprop_reg32 fon_clk_1;
    __IO halprop_reg32 fon_clk_2;
    __IO halprop_reg32 fon_clk_3;
    __IO halprop_reg32 reset_0;
    __IO halprop_reg32 reset_1;
    __IO halprop_reg32 reset_2;
    __IO halprop_reg32 reset_3;
    __IO halprop_reg32 unnamed_reg_standby_0x0070;
    __IO halprop_reg32 unnamed_reg_standby_0x0074;
    __IO halprop_reg32 bod_tune;
} STANDBY_REGMAP_T;

#define STANDBY_REGMAP ((STANDBY_REGMAP_T *) GP_WB_STANDBY_BASE_ADDRESS)

/***************************
 * regmap: leds
 ***************************/
typedef struct LEDS_REGMAP_S
{
    __IO halprop_reg32 control_0;
    __IO halprop_reg32 led0_threshold;
    __IO halprop_reg32 led0_io_control;
    __IO halprop_reg32 led0_quic_cfg_0;
    __IO halprop_reg32 led0_quic_cfg_1;
    __IO halprop_reg32 control_1;
    __IO halprop_reg32 led1_threshold;
    __IO halprop_reg32 led1_io_control;
    __IO halprop_reg32 led1_quic_cfg_0;
    __IO halprop_reg32 led1_quic_cfg_1;
    __IO halprop_reg32 control_2;
    __IO halprop_reg32 led2_threshold;
    __IO halprop_reg32 led2_io_control;
    __IO halprop_reg32 led2_quic_cfg_0;
    __IO halprop_reg32 led2_quic_cfg_1;
    __IO halprop_reg32 control_3;
    __IO halprop_reg32 led3_threshold;
    __IO halprop_reg32 led3_io_control;
    __IO halprop_reg32 led3_quic_cfg_0;
    __IO halprop_reg32 led3_quic_cfg_1;
    __IO halprop_reg32 timer_config;
    __IO halprop_reg32 slope_time_unit;
} LEDS_REGMAP_T;

#define LEDS_REGMAP ((LEDS_REGMAP_T *) GP_WB_LEDS_BASE_ADDRESS)

/***************************
 * regmap: pwmxls
 ***************************/
typedef struct PWMXLS_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0000;
    __IO halprop_reg32 pwmxl_0_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0008;
    __IO halprop_reg32 pwmxl_0_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0010;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0014;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0018;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x001c;
    __IO halprop_reg32 pwmxl_1_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0024;
    __IO halprop_reg32 pwmxl_1_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x002c;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0030;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0034;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0038;
    __IO halprop_reg32 pwmxl_2_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0040;
    __IO halprop_reg32 pwmxl_2_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0048;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x004c;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0050;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0054;
    __IO halprop_reg32 pwmxl_3_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x005c;
    __IO halprop_reg32 pwmxl_3_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0064;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0068;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x006c;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0070;
    __IO halprop_reg32 pwmxl_4_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0078;
    __IO halprop_reg32 pwmxl_4_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0080;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0084;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0088;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x008c;
    __IO halprop_reg32 pwmxl_5_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x0094;
    __IO halprop_reg32 pwmxl_5_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x009c;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00a0;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00a4;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00a8;
    __IO halprop_reg32 pwmxl_6_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00b0;
    __IO halprop_reg32 pwmxl_6_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00b8;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00bc;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00c0;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00c4;
    __IO halprop_reg32 pwmxl_7_next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00cc;
    __IO halprop_reg32 pwmxl_7_active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00d4;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00d8;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00dc;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00e0;
    __IO halprop_reg32 next_top_wrap;
    __IO halprop_reg32 active_top_wrap;
    __IO halprop_reg32 pipelined_config;
    __IO halprop_reg32 main_counter;
    __IO halprop_reg32 unnamed_reg_pwmxls_0x00f4;
    __IO halprop_reg32 clr_top_overflow_interrupt;
    __IO halprop_reg32 quic_action_cfg_0;
    __IO halprop_reg32 quic_action_cfg_1;
    __IO halprop_reg32 quic_action_cfg_2;
    __IO halprop_reg32 quic_action_cfg_3;
    __IO halprop_reg32 quic_action_cfg_4;
    __IO halprop_reg32 quic_action_cfg_5;
    __IO halprop_reg32 quic_action_cfg_6;
    __IO halprop_reg32 quic_action_cfg_7;
} PWMXLS_REGMAP_T;

#define PWMXLS_REGMAP ((PWMXLS_REGMAP_T *) GP_WB_PWMXLS_BASE_ADDRESS)

/***************************
 * regmap: timerxl_0
 ***************************/
typedef struct TIMERXL_0_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_0_0x0100;
} TIMERXL_0_REGMAP_T;

#define TIMERXL_0_REGMAP ((TIMERXL_0_REGMAP_T *) GP_WB_TIMERXL_0_BASE_ADDRESS)

/***************************
 * regmap: timerxl_1
 ***************************/
typedef struct TIMERXL_1_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_1_0x0100;
} TIMERXL_1_REGMAP_T;

#define TIMERXL_1_REGMAP ((TIMERXL_1_REGMAP_T *) GP_WB_TIMERXL_1_BASE_ADDRESS)

/***************************
 * regmap: timerxl_2
 ***************************/
typedef struct TIMERXL_2_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_2_0x0100;
} TIMERXL_2_REGMAP_T;

#define TIMERXL_2_REGMAP ((TIMERXL_2_REGMAP_T *) GP_WB_TIMERXL_2_BASE_ADDRESS)

/***************************
 * regmap: timerxl_3
 ***************************/
typedef struct TIMERXL_3_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_3_0x0100;
} TIMERXL_3_REGMAP_T;

#define TIMERXL_3_REGMAP ((TIMERXL_3_REGMAP_T *) GP_WB_TIMERXL_3_BASE_ADDRESS)

/***************************
 * regmap: timerxl_4
 ***************************/
typedef struct TIMERXL_4_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_4_0x0100;
} TIMERXL_4_REGMAP_T;

#define TIMERXL_4_REGMAP ((TIMERXL_4_REGMAP_T *) GP_WB_TIMERXL_4_BASE_ADDRESS)

/***************************
 * regmap: timerxl_5
 ***************************/
typedef struct TIMERXL_5_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_5_0x0100;
} TIMERXL_5_REGMAP_T;

#define TIMERXL_5_REGMAP ((TIMERXL_5_REGMAP_T *) GP_WB_TIMERXL_5_BASE_ADDRESS)

/***************************
 * regmap: sysram
 ***************************/
typedef struct SYSRAM_REGMAP_S
{
    __IO halprop_reg32 start_ptr;
    __IO halprop_reg32 end_ptr;
    __IO halprop_reg32 bist_enable;
    __IO halprop_reg32 triggers;
    __IO halprop_reg32 status;
    __IO halprop_reg32 crc_result;
    __IO halprop_reg32 retention_map_0;
    __IO halprop_reg32 retention_map_1;
} SYSRAM_REGMAP_T;

#define SYSRAM_REGMAP ((SYSRAM_REGMAP_T *) GP_WB_SYSRAM_BASE_ADDRESS)

/***************************
 * regmap: usb_ctrl
 ***************************/
typedef struct USB_CTRL_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x0000;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x0004;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x0008;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 bist_enable;
    __IO halprop_reg32 bist_start;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x0018;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x001c;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x0020;
    __IO halprop_reg32 overrule_0;
    __IO halprop_reg32 overrule_ena_0;
    __IO halprop_reg32 unnamed_reg_usb_ctrl_0x002c;
} USB_CTRL_REGMAP_T;

#define USB_CTRL_REGMAP ((USB_CTRL_REGMAP_T *) GP_WB_USB_CTRL_BASE_ADDRESS)

/***************************
 * regmap: usb_sfr
 ***************************/
typedef struct USB_SFR_REGMAP_S
{
    halprop_reg8 reserved_0[1088];
    __IO halprop_reg8 out5buf;
    halprop_reg8 reserved_1[63];
    __IO halprop_reg8 in5buf;
    halprop_reg8 reserved_2[63];
    __IO halprop_reg8 out4buf;
    halprop_reg8 reserved_3[63];
    __IO halprop_reg8 in4buf;
    halprop_reg8 reserved_4[63];
    __IO halprop_reg8 out3buf;
    halprop_reg8 reserved_5[63];
    __IO halprop_reg8 in3buf;
    halprop_reg8 reserved_6[63];
    __IO halprop_reg8 out2buf;
    halprop_reg8 reserved_7[63];
    __IO halprop_reg8 in2buf;
    halprop_reg8 reserved_8[63];
    __IO halprop_reg8 out1buf;
    halprop_reg8 reserved_9[63];
    __IO halprop_reg8 in1buf;
    halprop_reg8 reserved_10[63];
    __IO halprop_reg8 out0buf;
    halprop_reg8 reserved_11[63];
    __IO halprop_reg8 in0buf;
    halprop_reg8 reserved_12[63];
    __IO halprop_reg8 dmabufaddr0;
    __IO halprop_reg8 dmabufaddr1;
    __IO halprop_reg8 dmaramadr0;
    __IO halprop_reg8 dmaramadr1;
    __IO halprop_reg8 dmaramadr2;
    __IO halprop_reg8 dmaramadr3;
    __IO halprop_reg8 dmalen;
    __IO halprop_reg8 dmactrl;
    __IO halprop_reg8 dmatransfersize0;
    __IO halprop_reg8 dmatransfersize1;
    halprop_reg8 reserved_13[22];
    __IO halprop_reg8 out8data;
    halprop_reg8 reserved_14[7];
    __IO halprop_reg8 in8data;
    halprop_reg8 reserved_15[7];
    __IO halprop_reg8 out8bch;
    __IO halprop_reg8 out8bcl;
    halprop_reg8 reserved_16[15];
    __IO halprop_reg8 bout1addr;
    __IO halprop_reg8 bout2addr;
    __IO halprop_reg8 bout3addr;
    __IO halprop_reg8 bout4addr;
    __IO halprop_reg8 bout5addr;
    halprop_reg8 reserved_17[2];
    __IO halprop_reg8 binstaddr;
    __IO halprop_reg8 bin1addr;
    __IO halprop_reg8 bin2addr;
    __IO halprop_reg8 bin3addr;
    __IO halprop_reg8 bin4addr;
    __IO halprop_reg8 bin5addr;
    halprop_reg8 reserved_18[2];
    __IO halprop_reg8 clkgate;
    halprop_reg8 reserved_19[15];
    __IO halprop_reg8 isoerr;
    halprop_reg8 reserved_20[1];
    __IO halprop_reg8 zbcout;
    halprop_reg8 reserved_21[5];
    __IO halprop_reg8 ivec;
    __IO halprop_reg8 in07irq;
    __IO halprop_reg8 out07irq;
    __IO halprop_reg8 usbirq;
    __IO halprop_reg8 in07ien;
    __IO halprop_reg8 out07ien;
    __IO halprop_reg8 usbien;
    __IO halprop_reg8 usbbav;
    halprop_reg8 reserved_22[4];
    __IO halprop_reg8 ep0cs;
    __IO halprop_reg8 in0bc;
    __IO halprop_reg8 in1cs;
    __IO halprop_reg8 in1bc;
    __IO halprop_reg8 in2cs;
    __IO halprop_reg8 in2bc;
    __IO halprop_reg8 in3cs;
    __IO halprop_reg8 in3bc;
    __IO halprop_reg8 in4cs;
    __IO halprop_reg8 in4bc;
    __IO halprop_reg8 in5cs;
    __IO halprop_reg8 in5bc;
    halprop_reg8 reserved_23[5];
    __IO halprop_reg8 out0bc;
    __IO halprop_reg8 out1cs;
    __IO halprop_reg8 out1bc;
    __IO halprop_reg8 out2cs;
    __IO halprop_reg8 out2bc;
    __IO halprop_reg8 out3cs;
    __IO halprop_reg8 out3bc;
    __IO halprop_reg8 out4cs;
    __IO halprop_reg8 out4bc;
    __IO halprop_reg8 out5cs;
    __IO halprop_reg8 out5bc;
    halprop_reg8 reserved_24[5];
    __IO halprop_reg8 lpmst;
    __IO halprop_reg8 usbcs;
    __IO halprop_reg8 togctl;
    __IO halprop_reg8 usbframel;
    __IO halprop_reg8 usbframeh;
    halprop_reg8 reserved_25[1];
    __IO halprop_reg8 fnaddr;
    halprop_reg8 reserved_26[1];
    __IO halprop_reg8 usbpair;
    __IO halprop_reg8 in07val;
    __IO halprop_reg8 out07val;
    __IO halprop_reg8 inisoval;
    __IO halprop_reg8 outisoval;
    __IO halprop_reg8 isostaddr;
    __IO halprop_reg8 isosize;
    halprop_reg8 reserved_27[4];
    __IO halprop_reg8 setupbuf0;
    __IO halprop_reg8 setupbuf1;
    __IO halprop_reg8 setupbuf2;
    __IO halprop_reg8 setupbuf3;
    __IO halprop_reg8 setupbuf4;
    __IO halprop_reg8 setupbuf5;
    __IO halprop_reg8 setupbuf6;
    __IO halprop_reg8 setupbuf7;
    __IO halprop_reg8 out8addr;
    halprop_reg8 reserved_28[7];
    __IO halprop_reg8 in8addr;
} USB_SFR_REGMAP_T;

#define USB_SFR_REGMAP ((USB_SFR_REGMAP_T *) GP_WB_USB_SFR_BASE_ADDRESS)

/***************************
 * regmap: watchdog
 ***************************/
typedef struct WATCHDOG_REGMAP_S
{
    __IO halprop_reg32 control;
    __IO halprop_reg32 key;
    __IO halprop_reg32 clr_timer_expired_interrupt;
    __IO halprop_reg32 status;
    __IO halprop_reg32 current_time;
} WATCHDOG_REGMAP_T;

#define WATCHDOG_REGMAP ((WATCHDOG_REGMAP_T *) GP_WB_WATCHDOG_BASE_ADDRESS)

/***************************
 * regmap: ipc
 ***************************/
typedef struct IPC_REGMAP_S
{
    __IO halprop_reg32 set_channel_interrupts;
    __IO halprop_reg32 clr_channel_interrupts;
    __IO halprop_reg32 unmasked_channel_interrupts;
    __IO halprop_reg32 mutex_0;
    __IO halprop_reg32 mutex_1;
    __IO halprop_reg32 mutex_2;
    __IO halprop_reg32 mutex_3;
    __IO halprop_reg32 mutex_4;
    __IO halprop_reg32 mutex_5;
    __IO halprop_reg32 mutex_6;
    __IO halprop_reg32 mutex_7;
} IPC_REGMAP_T;

#define IPC_REGMAP ((IPC_REGMAP_T *) GP_WB_IPC_BASE_ADDRESS)

/***************************
 * regmap: ssp
 ***************************/
typedef struct SSP_REGMAP_S
{
    __IO halprop_reg32 a_ptr;
    __IO halprop_reg32 msg_ptr;
    __IO halprop_reg32 mic_ptr;
    __IO halprop_reg32 key_ptr;
    __IO halprop_reg32 msg_out_ptr;
    __IO halprop_reg32 nonce_ptr;
    __IO halprop_reg32 a_len;
    __IO halprop_reg32 msg_len;
    __IO halprop_reg32 unnamed_reg_ssp_0x0020;
    __IO halprop_reg32 circ_wd_start_ptr;
    __IO halprop_reg32 circ_wd_len;
    __IO halprop_reg32 mode_and_int_control;
    __IO halprop_reg32 encryption_control;
    __IO halprop_reg32 encryption_status;
} SSP_REGMAP_T;

#define SSP_REGMAP ((SSP_REGMAP_T *) GP_WB_SSP_BASE_ADDRESS)

/***************************
 * regmap: quic
 ***************************/
typedef struct QUIC_REGMAP_S
{
    __IO halprop_reg32 channel_0_conf;
    __IO halprop_reg32 channel_1_conf;
    __IO halprop_reg32 channel_2_conf;
    __IO halprop_reg32 channel_3_conf;
    __IO halprop_reg32 channel_4_conf;
    __IO halprop_reg32 channel_5_conf;
    __IO halprop_reg32 channel_6_conf;
    __IO halprop_reg32 channel_7_conf;
    __IO halprop_reg32 channel_8_conf;
    __IO halprop_reg32 channel_9_conf;
    __IO halprop_reg32 channel_10_conf;
    __IO halprop_reg32 channel_11_conf;
    __IO halprop_reg32 channel_12_conf;
    __IO halprop_reg32 channel_13_conf;
    __IO halprop_reg32 channel_14_conf;
    __IO halprop_reg32 channel_15_conf;
    __IO halprop_reg32 channel_0_mask;
    __IO halprop_reg32 channel_1_mask;
} QUIC_REGMAP_T;

#define QUIC_REGMAP ((QUIC_REGMAP_T *) GP_WB_QUIC_BASE_ADDRESS)

/***************************
 * regmap: es
 ***************************/
typedef struct ES_REGMAP_S
{
    __IO halprop_reg32 time_reference_setting;
    __IO halprop_reg32 first_cycle_control;
    __IO halprop_reg32 time_reference_control;
    __IO halprop_reg32 time_reference_state;
    __IO halprop_reg32 system_time_us;
    __IO halprop_reg32 next_event_time;
    __IO halprop_reg32 next_es_event_time;
    __IO halprop_reg32 next_akuma_event_time;
    __IO halprop_reg32 rtc_time_sampled;
    __IO halprop_reg32 system_time_sampled;
    __IO halprop_reg32 max_wakeup_duration;
    __IO halprop_reg32 min_sleep_duration;
    __IO halprop_reg32 rtc_time_reference;
    __IO halprop_reg32 system_time_reference;
    __IO halprop_reg32 calibration_factor;
    __IO halprop_reg32 phase_compensation;
    __IO halprop_reg32 enable_clk_time_reference_base_b_by_uc;
    __IO halprop_reg32 valid_events;
    __IO halprop_reg32 set_valid_events;
    __IO halprop_reg32 clr_valid_events;
    __IO halprop_reg32 buffer_time;
    __IO halprop_reg32 priority_time;
    __IO halprop_reg32 guard_time_sel;
    __IO halprop_reg32 overwrite_execution_time_on_final_execution;
    __IO halprop_reg32 reevaluate_events;
    __IO halprop_reg32 event_handler_state;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 calibration_factor_adjustment;
    __IO halprop_reg32 events_base_ptr;
} ES_REGMAP_T;

#define ES_REGMAP ((ES_REGMAP_T *) GP_WB_ES_BASE_ADDRESS)

/***************************
 * regmap: secure_element
 ***************************/
typedef struct SECURE_ELEMENT_REGMAP_S
{
    __IO halprop_reg32 hw_key_word_0;
    __IO halprop_reg32 hw_key_word_1;
    __IO halprop_reg32 hw_key_word_2;
    __IO halprop_reg32 hw_key_word_3;
    __IO halprop_reg32 hw_key_word_4;
    __IO halprop_reg32 hw_key_word_5;
    __IO halprop_reg32 hw_key_word_6;
    __IO halprop_reg32 hw_key_word_7;
    __IO halprop_reg32 core_config;
    __IO halprop_reg32 disable_countermeasures;
    __IO halprop_reg32 clk_rst_ctrl;
    __IO halprop_reg32 dbg_grant;
    __IO halprop_reg32 seuc_status;
    __IO halprop_reg32 lock_sysram;
    __IO halprop_reg32 restricted_nvm_info_user;
    __IO halprop_reg32 restricted_nvm_main;
    __IO halprop_reg32 restricted_cma_rom;
    __IO halprop_reg32 restricted_regmaps;
    __IO halprop_reg32 restricted_gpios;
    __IO halprop_reg32 disable_cma_nvm_bist;
    __IO halprop_reg32 enable_cma_mri;
    __IO halprop_reg32 cma_otp_eotp_cfg;
    __IO halprop_reg32 enable_cma_eotp_cfg_0;
    __IO halprop_reg32 sysram_bist_enable;
    __IO halprop_reg32 sysram_bist_start;
    __IO halprop_reg32 unnamed_reg_secure_element_0x0064;
    __IO halprop_reg32 pkdataram_bist_enable;
    __IO halprop_reg32 pkdataram_bist_start;
    __IO halprop_reg32 unnamed_reg_secure_element_0x0070;
    __IO halprop_reg32 sysrom_bist_enable;
    __IO halprop_reg32 sysrom_bist_start;
    __IO halprop_reg32 unnamed_reg_secure_element_0x007c;
    __IO halprop_reg32 sysrom_bist_crc;
    __IO halprop_reg32 pkcoderom_bist_enable;
    __IO halprop_reg32 pkcoderom_bist_start;
    __IO halprop_reg32 unnamed_reg_secure_element_0x008c;
    __IO halprop_reg32 pkcoderom_bist_crc;
    __IO halprop_reg32 sp_ram_config;
    __IO halprop_reg32 rf_ram_config;
    __IO halprop_reg32 tbus_lock_cfg;
    __IO halprop_reg32 power_ctrl;
    __IO halprop_reg32 power_status;
} SECURE_ELEMENT_REGMAP_T;

#define SECURE_ELEMENT_REGMAP ((SECURE_ELEMENT_REGMAP_T *) GP_WB_SECURE_ELEMENT_BASE_ADDRESS)

/***************************
 * regmap: appuc_cache
 ***************************/
typedef struct APPUC_CACHE_REGMAP_S
{
    __IO halprop_reg32 cachectrl;
} APPUC_CACHE_REGMAP_T;

#define APPUC_CACHE_REGMAP ((APPUC_CACHE_REGMAP_T *) GP_WB_APPUC_CACHE_BASE_ADDRESS)

/***************************
 * regmap: esec_mb
 ***************************/
typedef struct ESEC_MB_REGMAP_S
{
    __IO halprop_reg32 fifo_data;
    halprop_reg32 reserved_0[15];
    __IO halprop_reg32 tx_status;
    __IO halprop_reg32 rx_status;
    __IO halprop_reg32 tx_protection;
    __IO halprop_reg32 rx_protection;
    __IO halprop_reg32 tx_header;
    __IO halprop_reg32 rx_header;
    __IO halprop_reg32 configuration;
} ESEC_MB_REGMAP_T;

#define ESEC_MB_REGMAP ((ESEC_MB_REGMAP_T *) GP_WB_ESEC_MB_BASE_ADDRESS)

/***************************
 * regmap: abs_ev
 ***************************/
typedef struct ABS_EV_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 execution_time;
    __IO halprop_reg32 recurrence_period;
    __IO halprop_reg32 recurrence_amount;
    __IO halprop_reg32 options;
} PACKED_POST ABS_EV_REGMAP_T;

/***************************
 * regmap: asp
 ***************************/
typedef struct ASP_REGMAP_S
{
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 pcm_conf;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 unnamed_reg_asp_0x0010;
    __IO halprop_reg32 unnamed_reg_asp_0x0014;
    __IO halprop_reg32 clk_2m_enable;
    __IO halprop_reg32 cic_diagnostics_clr;
    __IO halprop_reg32 cic_diagnostics_status;
} ASP_REGMAP_T;

/***************************
 * regmap: cal_nvm
 ***************************/
typedef struct CAL_NVM_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 magic_word;
    __IO halprop_reg32 version;
    __IO halprop_reg32 bist_adc_lsb_high_diff_uv;
    __IO halprop_reg32 bist_adc_lsb_high_se_uv;
    __IO halprop_reg32 bist_adc_lsb_low_diff_uv;
    __IO halprop_reg32 bist_adc_lsb_low_se_uv;
    __IO halprop_reg32 bist_adc_offset_high_diff_uv;
    __IO halprop_reg32 bist_adc_offset_high_se_uv;
    __IO halprop_reg32 bist_adc_offset_low_diff_uv;
    __IO halprop_reg32 bist_adc_offset_low_se_uv;
    __IO halprop_reg32 gp_adc_lsb_high_diff_uv;
    __IO halprop_reg32 gp_adc_lsb_high_se_uv;
    __IO halprop_reg32 gp_adc_lsb_low_diff_uv;
    __IO halprop_reg32 gp_adc_lsb_low_se_uv;
    __IO halprop_reg32 gp_adc_offset_high_diff_uv;
    __IO halprop_reg32 gp_adc_offset_high_se_uv;
    __IO halprop_reg32 gp_adc_offset_low_diff_uv;
    __IO halprop_reg32 gp_adc_offset_low_se_uv;
    __IO halprop_reg32 hr_adc_lsb_gain0p25_uv;
    __IO halprop_reg32 hr_adc_lsb_gain1_uv;
    __IO halprop_reg32 hr_adc_lsb_gain4_uv;
    __IO halprop_reg32 hr_adc_lsb_gain8_uv;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0058;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x005c;
    __IO halprop_reg32 bist_resload_alpha_1000k;
    __IO halprop_reg32 bist_resload_alpha_100k;
    __IO halprop_reg32 bist_resload_alpha_250k;
    __IO halprop_reg32 bist_resload_alpha_33k;
    __IO halprop_reg32 bist_tsens_offset;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0074;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0078;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x007c;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0080;
    __IO halprop_reg32 pmud_vcore_ldo_refbits;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0088;
    __IO halprop_reg32 standby_bod_tune;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0090;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0094;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x0098;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x009c;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x00a0;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x00a4;
    __IO halprop_reg32 unnamed_reg_cal_nvm_0x00a8;
    __IO halprop_reg32 xo32m_x_cap_factor;
} PACKED_POST CAL_NVM_REGMAP_T;

/***************************
 * regmap: dma_dcb
 ***************************/
typedef struct DMA_DCB_REGMAP_S
{
    __IO halprop_reg32 src_addr;
    __IO halprop_reg32 dest_addr;
    __IO halprop_reg32 buffer_size;
    __IO halprop_reg32 src_write_ptr;
    __IO halprop_reg32 src_read_ptr;
    __IO halprop_reg32 dest_write_ptr;
    __IO halprop_reg32 dest_read_ptr;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 reset_pointers;
    __IO halprop_reg32 unmasked_buffer_complete_interrupt;
    __IO halprop_reg32 quic_0_cfg;
    __IO halprop_reg32 quic_1_cfg;
    __IO halprop_reg32 quic_2_cfg;
} DMA_DCB_REGMAP_T;

/***************************
 * regmap: dma_scb
 ***************************/
typedef struct DMA_SCB_REGMAP_S
{
    __IO halprop_reg32 src_addr;
    __IO halprop_reg32 dest_addr;
    __IO halprop_reg32 buffer_size;
    __IO halprop_reg32 buffer_almost_complete_threshold;
    __IO halprop_reg32 buffer_ptr_value;
    __IO halprop_reg32 buffer_ptr_wrap_value;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 quic_cfg;
    __IO halprop_reg32 cpy_trigger_ignore_buffer_complete;
    __IO halprop_reg32 circular_buffer_select;
    __IO halprop_reg32 buffer_ptr_control;
    __IO halprop_reg32 clr_buffer_complete_interrupt;
    __IO halprop_reg32 internal_ptr_status;
    __IO halprop_reg32 status;
} DMA_SCB_REGMAP_T;

/***************************
 * regmap: gpio_quic
 ***************************/
typedef struct GPIO_QUIC_REGMAP_S
{
    __IO halprop_reg32 trigger_gpio;
    __IO halprop_reg32 quic_cfg;
} GPIO_QUIC_REGMAP_T;

/***************************
 * regmap: i2c_m
 ***************************/
typedef struct I2C_M_REGMAP_S
{
    __IO halprop_reg32 prescaler;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 control;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
} I2C_M_REGMAP_T;

#if defined(GP_DIGITAL_ITWOSPERIPHERAL)
/***************************
 * regmap: i2s
 ***************************/
typedef struct I2S_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 select_clk;
    __IO halprop_reg32 config_0;
    __IO halprop_reg32 config_1;
    __IO halprop_reg32 unmasked_interrupts;
} I2S_REGMAP_T;

#endif //['itwosperipheral']
/***************************
 * regmap: led
 ***************************/
typedef struct LED_REGMAP_S
{
    __IO halprop_reg32 control;
    __IO halprop_reg32 threshold;
    __IO halprop_reg32 io_control;
    __IO halprop_reg32 quic_cfg_0;
    __IO halprop_reg32 quic_cfg_1;
} LED_REGMAP_T;

/***************************
 * regmap: pmu_ctrl
 ***************************/
typedef struct PMU_CTRL_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 unnamed_reg_pmu_ctrl_0x0000;
} PACKED_POST PMU_CTRL_REGMAP_T;

/***************************
 * regmap: pwmxl
 ***************************/
typedef struct PWMXL_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_pwmxl_0x0000;
    __IO halprop_reg32 next_dither;
    __IO halprop_reg32 unnamed_reg_pwmxl_0x0008;
    __IO halprop_reg32 active_dither;
    __IO halprop_reg32 unnamed_reg_pwmxl_0x0010;
    __IO halprop_reg32 unnamed_reg_pwmxl_0x0014;
    __IO halprop_reg32 unnamed_reg_pwmxl_0x0018;
} PWMXL_REGMAP_T;

/***************************
 * regmap: quic_channel
 ***************************/
typedef struct QUIC_CHANNEL_REGMAP_S
{
    __IO halprop_reg32 conf;
} QUIC_CHANNEL_REGMAP_T;

/***************************
 * regmap: rtz
 ***************************/
typedef struct RTZ_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 init_random_seed;
    __IO halprop_reg32 cfg;
    __IO halprop_reg32 zero_timing;
    __IO halprop_reg32 one_timing;
    __IO halprop_reg32 stop_timing;
    __IO halprop_reg32 reset_timing;
    __IO halprop_reg32 jitter_cfg;
    __IO halprop_reg32 data_bits;
    __IO halprop_reg32 pixels;
    __IO halprop_reg32 trailing_data_bits;
    __IO halprop_reg32 sequence_busy;
    __IO halprop_reg32 repeat_sequence;
    __IO halprop_reg32 triggers;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 quic_cfg;
} RTZ_REGMAP_T;

/***************************
 * regmap: se_cal_ram
 ***************************/
typedef struct SE_CAL_RAM_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 unnamed_reg_se_cal_ram_0x0000;
    __IO halprop_reg32 unnamed_reg_se_cal_ram_0x0004;
    __IO halprop_reg32 unnamed_reg_se_cal_ram_0x0008;
} PACKED_POST SE_CAL_RAM_REGMAP_T;

/***************************
 * regmap: timerxl
 ***************************/
typedef struct TIMERXL_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0008;
    __IO halprop_reg32 timerxl32_counter;
    __IO halprop_reg32 timerxl32_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0018;
    __IO halprop_reg32 unmasked_timerxl32_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0028;
    __IO halprop_reg32 unmasked_timerxl32_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0038;
    __IO halprop_reg32 unmasked_timerxl32_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl32_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0048;
    __IO halprop_reg32 unmasked_timerxl32_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0050;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0054;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0058;
    __IO halprop_reg32 timerxl16_0_counter;
    __IO halprop_reg32 timerxl16_0_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0064;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0068;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0074;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0078;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0084;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0088;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_0_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0094;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0098;
    __IO halprop_reg32 unmasked_timerxl16_0_cc_3_match_interrupt;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00a0;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00a4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00a8;
    __IO halprop_reg32 timerxl16_1_counter;
    __IO halprop_reg32 timerxl16_1_cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00b4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00b8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_0_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00c4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00c8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_1_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00d4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00d8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_2_match_interrupt;
    __IO halprop_reg32 timerxl16_1_cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00e4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00e8;
    __IO halprop_reg32 unmasked_timerxl16_1_cc_3_match_interrupt;
    __IO halprop_reg32 mode;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00f4;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00f8;
    __IO halprop_reg32 unnamed_reg_timerxl_0x00fc;
    __IO halprop_reg32 unnamed_reg_timerxl_0x0100;
} TIMERXL_REGMAP_T;

/***************************
 * regmap: timerxl16
 ***************************/
typedef struct TIMERXL16_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0008;
    __IO halprop_reg32 counter;
    __IO halprop_reg32 cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0018;
    __IO halprop_reg32 unmasked_cc_0_match_interrupt;
    __IO halprop_reg32 cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0028;
    __IO halprop_reg32 unmasked_cc_1_match_interrupt;
    __IO halprop_reg32 cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0038;
    __IO halprop_reg32 unmasked_cc_2_match_interrupt;
    __IO halprop_reg32 cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl16_0x0048;
    __IO halprop_reg32 unmasked_cc_3_match_interrupt;
} TIMERXL16_REGMAP_T;

/***************************
 * regmap: timerxl32
 ***************************/
typedef struct TIMERXL32_REGMAP_S
{
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0000;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0004;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0008;
    __IO halprop_reg32 counter;
    __IO halprop_reg32 cc_0_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0014;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0018;
    __IO halprop_reg32 unmasked_cc_0_match_interrupt;
    __IO halprop_reg32 cc_1_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0024;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0028;
    __IO halprop_reg32 unmasked_cc_1_match_interrupt;
    __IO halprop_reg32 cc_2_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0034;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0038;
    __IO halprop_reg32 unmasked_cc_2_match_interrupt;
    __IO halprop_reg32 cc_3_comparecapture;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0044;
    __IO halprop_reg32 unnamed_reg_timerxl32_0x0048;
    __IO halprop_reg32 unmasked_cc_3_match_interrupt;
} TIMERXL32_REGMAP_T;

/***************************
 * regmap: uart
 ***************************/
typedef struct UART_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 rx_not_busy;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} UART_REGMAP_T;


#endif //GP_WB_PROP_STRUCT_H
