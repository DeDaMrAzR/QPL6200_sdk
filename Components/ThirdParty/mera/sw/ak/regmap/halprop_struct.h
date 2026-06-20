#ifndef GP_WB_AK_REGMAP_PROP_STRUCT_H
#define GP_WB_AK_REGMAP_PROP_STRUCT_H

/***************************
 * regmap: ak_standby
 ***************************/
typedef struct AK_STANDBY_REGMAP_S
{
    halprop_reg32 reserved_0[512];
    __IO halprop_reg32 version;
    __IO halprop_reg32 snapshot_ctrl;
    __IO halprop_reg32 powerup_and_isolation_ctrl;
    __IO halprop_reg32 unnamedpb_standby_0x080c;
    __IO halprop_reg32 rtgts_sw_overrule;
    __IO halprop_reg32 unnamedpb_standby_0x0814;
    __IO halprop_reg32 rtgts_status;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 standby_status;
    __IO halprop_reg32 reset_0;
    __IO halprop_reg32 reset_regmap_0;
    __IO halprop_reg32 clk_ena_0;
    __IO halprop_reg32 clk_ena_regmap_0;
    __IO halprop_reg32 fon_clk_0;
} AK_STANDBY_REGMAP_T;

#define AK_STANDBY_REGMAP ((AK_STANDBY_REGMAP_T *) GP_WB_AK_STANDBY_BASE_ADDRESS)

/***************************
 * regmap: ak_int_ctrl
 ***************************/
typedef struct AK_INT_CTRL_REGMAP_S
{
    __IO halprop_reg32 global_interrupts_enable;
    __IO halprop_reg32 unmasked_interrupts_0;
    __IO halprop_reg32 masked_ext_interrupts_0;
    __IO halprop_reg32 mask_ext_interrupts_0;
    __IO halprop_reg32 masked_rtuc_interrupts_0;
    __IO halprop_reg32 mask_rtuc_interrupts_0;
    __IO halprop_reg32 mask_ttb_interrupts;
    __IO halprop_reg32 mask_dmadcb_interrupts;
    __IO halprop_reg32 mask_dmascb_interrupts;
    __IO halprop_reg32 mask_par_interrupts;
    __IO halprop_reg32 mask_stbc_interrupts;
    __IO halprop_reg32 mask_pat_interrupts;
    __IO halprop_reg32 mask_phy_interrupts_0;
    __IO halprop_reg32 mask_phy_interrupts_1;
    __IO halprop_reg32 mask_phy_dc_offset_interrupts;
    __IO halprop_reg32 mask_rtm_trc_interrupts;
    __IO halprop_reg32 mask_ipcrt2x_interrupts;
    __IO halprop_reg32 mask_ipcx2rt_interrupts;
    __IO halprop_reg32 mask_uart_0_interrupts;
    __IO halprop_reg32 mask_coex_interrupts;
    __IO halprop_reg32 mask_single_interrupts;
    __IO halprop_reg32 mask_gpio_interrupts;
    __IO halprop_reg32 masked_ttb_interrupts;
    __IO halprop_reg32 masked_dmadcb_interrupts;
    __IO halprop_reg32 masked_dmascb_interrupts;
    __IO halprop_reg32 masked_par_interrupts;
    __IO halprop_reg32 masked_stbc_interrupts;
    __IO halprop_reg32 masked_pat_interrupts;
    __IO halprop_reg32 masked_phy_interrupts_0;
    __IO halprop_reg32 masked_phy_interrupts_1;
    __IO halprop_reg32 masked_phy_dc_offset_interrupts;
    __IO halprop_reg32 masked_rtm_trc_interrupts;
    __IO halprop_reg32 masked_ipcrt2x_interrupts;
    __IO halprop_reg32 masked_ipcx2rt_interrupts;
    __IO halprop_reg32 masked_uart_0_interrupts;
    __IO halprop_reg32 masked_coex_interrupts;
    __IO halprop_reg32 masked_single_interrupts;
    __IO halprop_reg32 masked_gpio_interrupts;
} AK_INT_CTRL_REGMAP_T;

#define AK_INT_CTRL_REGMAP ((AK_INT_CTRL_REGMAP_T *) GP_WB_AK_INT_CTRL_BASE_ADDRESS)

/***************************
 * regmap: ak_rtuc
 ***************************/
typedef struct AK_RTUC_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_rtuc_0x0000;
    __IO halprop_reg32 unnamedpb_rtuc_0x0004;
    __IO halprop_reg32 unnamedpb_rtuc_0x0008;
    __IO halprop_reg32 unnamedpb_rtuc_0x000c;
    __IO halprop_reg32 unnamedpb_rtuc_0x0010;
    __IO halprop_reg32 lpwait_status;
    __IO halprop_reg32 nvm_cfg;
    __IO halprop_reg32 cache_cfg;
    __IO halprop_reg32 unnamedpb_rtuc_0x0020;
    __IO halprop_reg32 unnamedpb_rtuc_0x0024;
    __IO halprop_reg32 unnamedpb_rtuc_0x0028;
    __IO halprop_reg32 unnamedpb_rtuc_0x002c;
    __IO halprop_reg32 cache_ram_test;
    __IO halprop_reg32 cache_bist_start;
    __IO halprop_reg32 cache_bist_status;
} AK_RTUC_REGMAP_T;

#define AK_RTUC_REGMAP ((AK_RTUC_REGMAP_T *) GP_WB_AK_RTUC_BASE_ADDRESS)

/***************************
 * regmap: ak_mm
 ***************************/
typedef struct AK_MM_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_mm_0x0000;
    __IO halprop_reg32 unnamedpb_mm_0x0004;
    __IO halprop_reg32 unnamedpb_mm_0x0008;
    __IO halprop_reg32 unnamedpb_mm_0x000c;
    __IO halprop_reg32 unnamedpb_mm_0x0010;
    __IO halprop_reg32 ram_operation_triggers;
    __IO halprop_reg32 ram_operation_status;
    __IO halprop_reg32 access_errors;
    __IO halprop_reg32 unnamedpb_mm_0x0020;
    __IO halprop_reg32 sp_ram_config;
    __IO halprop_reg32 rf_ram_config;
} AK_MM_REGMAP_T;

#define AK_MM_REGMAP ((AK_MM_REGMAP_T *) GP_WB_AK_MM_BASE_ADDRESS)

/***************************
 * regmap: ak_rtm
 ***************************/
typedef struct AK_RTM_REGMAP_S
{
    __IO halprop_reg32 trc_status;
    __IO halprop_reg32 unnamedpb_rtm_0x0004;
    __IO halprop_reg32 trc_statereq_done_info;
    __IO halprop_reg32 unnamedpb_rtm_0x000c;
    __IO halprop_reg32 unnamedpb_rtm_0x0010;
    __IO halprop_reg32 unnamedpb_rtm_0x0014;
    __IO halprop_reg32 unnamedpb_rtm_0x0018;
    __IO halprop_reg32 unnamedpb_rtm_0x001c;
    __IO halprop_reg32 trc_statereq_opts;
    __IO halprop_reg32 unnamedpb_rtm_0x0024;
    __IO halprop_reg32 unnamedpb_rtm_0x0028;
    __IO halprop_reg32 unnamedpb_rtm_0x002c;
    __IO halprop_reg32 trc_statereq0_opts;
    __IO halprop_reg32 unnamedpb_rtm_0x0034;
    __IO halprop_reg32 unnamedpb_rtm_0x0038;
    __IO halprop_reg32 unnamedpb_rtm_0x003c;
    __IO halprop_reg32 trc_statereq1_opts;
    __IO halprop_reg32 unnamedpb_rtm_0x0044;
    __IO halprop_reg32 unnamedpb_rtm_0x0048;
    __IO halprop_reg32 unnamedpb_rtm_0x004c;
    __IO halprop_reg32 trc_statereq2_opts;
    __IO halprop_reg32 unnamedpb_rtm_0x0054;
    __IO halprop_reg32 unnamedpb_rtm_0x0058;
    __IO halprop_reg32 unnamedpb_rtm_0x005c;
    __IO halprop_reg32 trc_statereq3_opts;
    __IO halprop_reg32 unnamedpb_rtm_0x0064;
    __IO halprop_reg32 unnamedpb_rtm_0x0068;
    __IO halprop_reg32 unnamedpb_rtm_0x006c;
    __IO halprop_reg32 trc_rx_on_when_idle_ch;
    __IO halprop_reg32 trc_is_ble_ch;
    __IO halprop_reg32 unnamedpb_rtm_0x0078;
    __IO halprop_reg32 unnamedpb_rtm_0x007c;
    __IO halprop_reg32 trc_config;
    __IO halprop_reg32 trc_zb_coex_packet_rx_config;
    __IO halprop_reg32 trc_ble_coex_packet_rx_config;
    __IO halprop_reg32 trc_control;
    __IO halprop_reg32 trc_unmasked_interrupts;
    __IO halprop_reg32 trc_clr_interrupts;
    __IO halprop_reg32 trc_coex_status;
} AK_RTM_REGMAP_T;

#define AK_RTM_REGMAP ((AK_RTM_REGMAP_T *) GP_WB_AK_RTM_BASE_ADDRESS)

/***************************
 * regmap: ak_pat
 ***************************/
typedef struct AK_PAT_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_pat_0x0000;
    __IO halprop_reg32 tx_power_cfg;
    __IO halprop_reg32 external_pa_fem_settings;
    __IO halprop_reg32 supplemental_cfg;
    __IO halprop_reg32 unnamedpb_pat_0x0010;
    __IO halprop_reg32 unnamedpb_pat_0x0014;
    __IO halprop_reg32 unnamedpb_pat_0x0018;
    __IO halprop_reg32 pat_start_bytes;
    __IO halprop_reg32 whitening_cfg;
    __IO halprop_reg32 unnamedpb_pat_0x0024;
    __IO halprop_reg32 unnamedpb_pat_0x0028;
    __IO halprop_reg32 unnamedpb_pat_0x002c;
    __IO halprop_reg32 unnamedpb_pat_0x0030;
    __IO halprop_reg32 unnamedpb_pat_0x0034;
    __IO halprop_reg32 unnamedpb_pat_0x0038;
    __IO halprop_reg32 unnamedpb_pat_0x003c;
} AK_PAT_REGMAP_T;

#define AK_PAT_REGMAP ((AK_PAT_REGMAP_T *) GP_WB_AK_PAT_BASE_ADDRESS)

/***************************
 * regmap: ak_par
 ***************************/
typedef struct AK_PAR_REGMAP_S
{
    __IO halprop_reg32 options;
    __IO halprop_reg32 unnamedpb_par_0x0004;
    __IO halprop_reg32 unnamedpb_par_0x0008;
    __IO halprop_reg32 unnamedpb_par_0x000c;
    __IO halprop_reg32 unnamedpb_par_0x0010;
    __IO halprop_reg32 unnamedpb_par_0x0014;
    __IO halprop_reg32 unnamedpb_par_0x0018;
    __IO halprop_reg32 unnamedpb_par_0x001c;
    __IO halprop_reg32 unnamedpb_par_0x0020;
    __IO halprop_reg32 unnamedpb_par_0x0024;
    __IO halprop_reg32 unnamedpb_par_0x0028;
    __IO halprop_reg32 unnamedpb_par_0x002c;
    __IO halprop_reg32 unnamedpb_par_0x0030;
    __IO halprop_reg32 unnamedpb_par_0x0034;
    __IO halprop_reg32 unnamedpb_par_0x0038;
    __IO halprop_reg32 unnamedpb_par_0x003c;
    __IO halprop_reg32 unnamedpb_par_0x0040;
    __IO halprop_reg32 rx_status;
    __IO halprop_reg32 measurements;
    __IO halprop_reg32 channel_info;
    __IO halprop_reg32 unnamedpb_par_0x0050;
    __IO halprop_reg32 unnamedpb_par_0x0054;
    __IO halprop_reg32 unnamedpb_par_0x0058;
    __IO halprop_reg32 unnamedpb_par_0x005c;
    __IO halprop_reg32 unmasked_interrupts;
} AK_PAR_REGMAP_T;

#define AK_PAR_REGMAP ((AK_PAR_REGMAP_T *) GP_WB_AK_PAR_BASE_ADDRESS)

/***************************
 * regmap: ak_plme
 ***************************/
typedef struct AK_PLME_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_plme_0x0000;
    __IO halprop_reg32 unnamedpb_plme_0x0004;
    __IO halprop_reg32 unnamedpb_plme_0x0008;
    __IO halprop_reg32 sig_gen_transition_control;
    __IO halprop_reg32 sig_gen_sequence_triggers;
    __IO halprop_reg32 sig_gen_sequence_status;
    __IO halprop_reg32 unnamedpb_plme_0x0018;
    __IO halprop_reg32 unnamedpb_plme_0x001c;
    __IO halprop_reg32 unnamedpb_plme_0x0020;
    __IO halprop_reg32 unnamedpb_plme_0x0024;
    __IO halprop_reg32 apply_channel;
    __IO halprop_reg32 channel_preamble_detect_disable;
    __IO halprop_reg32 channel_config_update;
    __IO halprop_reg32 sx_actions;
    __IO halprop_reg32 sx_overrules;
    __IO halprop_reg32 unnamedpb_plme_0x003c;
    __IO halprop_reg32 unnamedpb_plme_0x0040;
    __IO halprop_reg32 unnamedpb_plme_0x0044;
    __IO halprop_reg32 clr_rssi_for_channel;
    __IO halprop_reg32 unnamedpb_plme_0x004c;
    __IO halprop_reg32 unnamedpb_plme_0x0050;
    __IO halprop_reg32 unnamedpb_plme_0x0054;
    __IO halprop_reg32 unnamedpb_plme_0x0058;
    __IO halprop_reg32 unnamedpb_plme_0x005c;
    __IO halprop_reg32 unnamedpb_plme_0x0060;
    __IO halprop_reg32 rssi_valid;
    __IO halprop_reg32 lf_preload;
    __IO halprop_reg32 capbank_crs;
    __IO halprop_reg32 capbank_fine_tx;
    __IO halprop_reg32 capbank_fine_rx;
    __IO halprop_reg32 loop_gain;
    __IO halprop_reg32 unnamedpb_plme_0x007c;
    __IO halprop_reg32 unnamedpb_plme_0x0080;
    __IO halprop_reg32 unnamedpb_plme_0x0084;
    __IO halprop_reg32 bbprx_phy_seq_triggers_en;
    __IO halprop_reg32 unnamedpb_plme_0x008c;
    __IO halprop_reg32 unnamedpb_plme_0x0090;
    __IO halprop_reg32 sx_chidx0_tx_cfg;
    __IO halprop_reg32 sx_chidx0_rx_cfg;
    __IO halprop_reg32 sx_chidx0_rx_cfg2;
    __IO halprop_reg32 unnamedpb_plme_0x00a0;
    __IO halprop_reg32 unnamedpb_plme_0x00a4;
    __IO halprop_reg32 sx_chidx1_tx_cfg;
    __IO halprop_reg32 sx_chidx1_rx_cfg;
    __IO halprop_reg32 sx_chidx1_rx_cfg2;
    __IO halprop_reg32 unnamedpb_plme_0x00b4;
    __IO halprop_reg32 unnamedpb_plme_0x00b8;
    __IO halprop_reg32 sx_chidx2_tx_cfg;
    __IO halprop_reg32 sx_chidx2_rx_cfg;
    __IO halprop_reg32 sx_chidx2_rx_cfg2;
    __IO halprop_reg32 unnamedpb_plme_0x00c8;
    __IO halprop_reg32 unnamedpb_plme_0x00cc;
    __IO halprop_reg32 sx_chidx3_tx_cfg;
    __IO halprop_reg32 sx_chidx3_rx_cfg;
    __IO halprop_reg32 sx_chidx3_rx_cfg2;
    __IO halprop_reg32 unnamedpb_plme_0x00dc;
    __IO halprop_reg32 unnamedpb_plme_0x00e0;
    __IO halprop_reg32 sx_chidx4_tx_cfg;
    __IO halprop_reg32 sx_chidx4_rx_cfg;
    __IO halprop_reg32 sx_chidx4_rx_cfg2;
    __IO halprop_reg32 unnamedpb_plme_0x00f0;
    __IO halprop_reg32 unnamedpb_plme_0x00f4;
    __IO halprop_reg32 sx_chidx5_tx_cfg;
    __IO halprop_reg32 sx_chidx5_rx_cfg;
    __IO halprop_reg32 sx_chidx5_rx_cfg2;
} AK_PLME_REGMAP_T;

#define AK_PLME_REGMAP ((AK_PLME_REGMAP_T *) GP_WB_AK_PLME_BASE_ADDRESS)

/***************************
 * regmap: ak_radioitf
 ***************************/
typedef struct AK_RADIOITF_REGMAP_S
{
    __IO halprop_reg32 rx_overrule_control;
    __IO halprop_reg32 agc_mux_control;
    __IO halprop_reg32 ana_overrule_ena_0;
    __IO halprop_reg32 ana_overrule_ena_1;
    __IO halprop_reg32 ana_overrule_ena_2;
    __IO halprop_reg32 ana_overrule_ena_3;
    __IO halprop_reg32 ana_overrule_ena_4;
    __IO halprop_reg32 ana_overrule_ena_5;
    __IO halprop_reg32 ana_overrule_ena_6;
    __IO halprop_reg32 ana_overrule_ena_7;
    __IO halprop_reg32 ana_overrule_0;
    __IO halprop_reg32 ana_overrule_1;
    __IO halprop_reg32 ana_overrule_2;
    __IO halprop_reg32 ana_overrule_3;
    __IO halprop_reg32 ana_overrule_4;
    __IO halprop_reg32 ana_overrule_5;
    __IO halprop_reg32 ana_overrule_6;
    __IO halprop_reg32 ana_overrule_7;
    __IO halprop_reg32 ana_status_0;
    __IO halprop_reg32 ana_status_1;
    __IO halprop_reg32 ana_status_2;
    __IO halprop_reg32 ana_status_3;
    __IO halprop_reg32 ana_status_4;
    __IO halprop_reg32 ana_status_5;
    __IO halprop_reg32 ana_status_6;
    __IO halprop_reg32 ana_status_7;
    __IO halprop_reg32 ana_status_8;
} AK_RADIOITF_REGMAP_T;

#define AK_RADIOITF_REGMAP ((AK_RADIOITF_REGMAP_T *) GP_WB_AK_RADIOITF_BASE_ADDRESS)

/***************************
 * regmap: ak_tx
 ***************************/
typedef struct AK_TX_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_tx_0x0000;
    __IO halprop_reg32 unnamedpb_tx_0x0004;
    __IO halprop_reg32 unnamedpb_tx_0x0008;
    __IO halprop_reg32 bbptx_config;
    __IO halprop_reg32 unnamedpb_tx_0x0010;
    __IO halprop_reg32 unnamedpb_tx_0x0014;
    __IO halprop_reg32 unnamedpb_tx_0x0018;
    __IO halprop_reg32 unnamedpb_tx_0x001c;
    __IO halprop_reg32 unnamedpb_tx_0x0020;
    __IO halprop_reg32 aoa_aod_tx_config_settings_1;
    __IO halprop_reg32 unnamedpb_tx_0x0028;
    __IO halprop_reg32 unnamedpb_tx_0x002c;
    __IO halprop_reg32 unnamedpb_tx_0x0030;
    __IO halprop_reg32 aoa_aod_tx_config_settings_3;
    __IO halprop_reg32 unnamedpb_tx_0x0038;
    __IO halprop_reg32 unnamedpb_tx_0x003c;
    __IO halprop_reg32 unnamedpb_tx_0x0040;
    __IO halprop_reg32 unnamedpb_tx_0x0044;
    __IO halprop_reg32 tx_antsel;
    __IO halprop_reg32 tx_ant_ext;
    __IO halprop_reg32 unnamedpb_tx_0x0050;
    __IO halprop_reg32 unnamedpb_tx_0x0054;
    __IO halprop_reg32 unnamedpb_tx_0x0058;
    __IO halprop_reg32 unnamedpb_tx_0x005c;
    __IO halprop_reg32 unnamedpb_tx_0x0060;
    __IO halprop_reg32 unnamedpb_tx_0x0064;
    __IO halprop_reg32 unnamedpb_tx_0x0068;
    __IO halprop_reg32 unnamedpb_tx_0x006c;
    __IO halprop_reg32 unnamedpb_tx_0x0070;
} AK_TX_REGMAP_T;

#define AK_TX_REGMAP ((AK_TX_REGMAP_T *) GP_WB_AK_TX_BASE_ADDRESS)

/***************************
 * regmap: ak_trx
 ***************************/
typedef struct AK_TRX_REGMAP_S
{
    __IO halprop_reg32 external_mode_control;
    __IO halprop_reg32 rx_a0_cfg;
    __IO halprop_reg32 rx_a1_cfg;
    __IO halprop_reg32 rx_ad_cfg;
    __IO halprop_reg32 tx_a0_cfg;
    __IO halprop_reg32 tx_a1_cfg;
    __IO halprop_reg32 trx_off_cfg;
    __IO halprop_reg32 sx_dco_ibias_sel_cfg;
    __IO halprop_reg32 unnamedpb_trx_0x0020;
    __IO halprop_reg32 unnamedpb_trx_0x0024;
    __IO halprop_reg32 unnamedpb_trx_0x0028;
    __IO halprop_reg32 unnamedpb_trx_0x002c;
} AK_TRX_REGMAP_T;

#define AK_TRX_REGMAP ((AK_TRX_REGMAP_T *) GP_WB_AK_TRX_BASE_ADDRESS)

/***************************
 * regmap: ak_sx
 ***************************/
typedef struct AK_SX_REGMAP_S
{
    __IO halprop_reg32 states;
    __IO halprop_reg32 mi_adjust;
    __IO halprop_reg32 dtc_mi_adjust;
    __IO halprop_reg32 mod_delay;
    __IO halprop_reg32 one_bit_1;
    __IO halprop_reg32 sx_channel_invalid_duration;
    __IO halprop_reg32 sx_rx_out_of_range;
    __IO halprop_reg32 sx_tx_out_of_range;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 gfsk_filter_coeffs_0;
    __IO halprop_reg32 gfsk_filter_coeffs_1;
    __IO halprop_reg32 gfsk_filter_coeffs_2;
    __IO halprop_reg32 msk_filter_coeffs_0;
    __IO halprop_reg32 msk_filter_coeffs_1;
    __IO halprop_reg32 msk_filter_coeffs_2;
    __IO halprop_reg32 freq_offset;
    __IO halprop_reg32 cb_tune_read;
    __IO halprop_reg32 lf_read;
    __IO halprop_reg32 tune_stats_count;
    __IO halprop_reg32 tune_stats;
    __IO halprop_reg32 one_bit_2;
    __IO halprop_reg32 fe_results;
    __IO halprop_reg32 cal_cfg;
    __IO halprop_reg32 meas_cntr;
    __IO halprop_reg32 dtc_meas_control;
    __IO halprop_reg32 dtc_measurements;
    __IO halprop_reg32 tune_value;
    __IO halprop_reg32 fine_value;
    __IO halprop_reg32 coarse_value;
    __IO halprop_reg32 dtc_step_value;
    __IO halprop_reg32 dtc_frac;
    __IO halprop_reg32 f_beat_control;
    __IO halprop_reg32 phase_control;
    __IO halprop_reg32 f_err_mod_delay;
    __IO halprop_reg32 pa_count;
    __IO halprop_reg32 err_gain_one_bit;
    __IO halprop_reg32 prg_update;
    __IO halprop_reg32 prg_mask;
    __IO halprop_reg32 prg_oset;
    __IO halprop_reg32 lfsr_value;
    __IO halprop_reg32 pll_control;
    __IO halprop_reg32 pll_state_counters_rx;
    __IO halprop_reg32 pll_state_counters_tx;
    __IO halprop_reg32 pll_parameters;
    __IO halprop_reg32 rfcounter_read;
    __IO halprop_reg32 rfcounter_control;
    __IO halprop_reg32 qsampled_sh_time;
    __IO halprop_reg32 qsampled_sum_reg;
    __IO halprop_reg32 unnamedpb_sx_0x00c4;
    __IO halprop_reg32 unnamedpb_sx_0x00c8;
    __IO halprop_reg32 unnamedpb_sx_0x00cc;
    __IO halprop_reg32 hadm_event_counter;
    __IO halprop_reg32 unnamedpb_sx_0x00d4;
    __IO halprop_reg32 unnamedpb_sx_0x00d8;
    __IO halprop_reg32 obm_status;
    __IO halprop_reg32 obm_triggers;
    __IO halprop_reg32 unnamedpb_sx_0x00e4;
    __IO halprop_reg32 ana_overrule_ena;
    __IO halprop_reg32 unnamedpb_sx_0x00ec;
    __IO halprop_reg32 unnamedpb_sx_0x00f0;
    __IO halprop_reg32 unnamedpb_sx_0x00f4;
    __IO halprop_reg32 unnamedpb_sx_0x00f8;
} AK_SX_REGMAP_T;

#define AK_SX_REGMAP ((AK_SX_REGMAP_T *) GP_WB_AK_SX_BASE_ADDRESS)

/***************************
 * regmap: ak_rx
 ***************************/
typedef struct AK_RX_REGMAP_S
{
    __IO halprop_reg32 rx_antsel;
    __IO halprop_reg32 rx_ant_ext;
    __IO halprop_reg32 bbprx_config;
    __IO halprop_reg32 rssi_based_training_cfg;
    __IO halprop_reg32 unnamedpb_rx_0x0010;
    __IO halprop_reg32 unnamedpb_rx_0x0014;
    __IO halprop_reg32 unnamedpb_rx_0x0018;
    __IO halprop_reg32 unnamedpb_rx_0x001c;
    __IO halprop_reg32 unnamedpb_rx_0x0020;
    __IO halprop_reg32 unnamedpb_rx_0x0024;
    __IO halprop_reg32 unnamedpb_rx_0x0028;
    __IO halprop_reg32 unnamedpb_rx_0x002c;
    __IO halprop_reg32 unnamedpb_rx_0x0030;
    __IO halprop_reg32 unnamedpb_rx_0x0034;
    __IO halprop_reg32 unnamedpb_rx_0x0038;
    __IO halprop_reg32 unnamedpb_rx_0x003c;
    __IO halprop_reg32 unnamedpb_rx_0x0040;
    __IO halprop_reg32 unnamedpb_rx_0x0044;
    __IO halprop_reg32 rssi_overrule_settings;
    __IO halprop_reg32 bbprx_event;
    __IO halprop_reg32 ble_timeout_events;
    __IO halprop_reg32 ble_timeout_set_events;
    __IO halprop_reg32 bbprx_state;
    __IO halprop_reg32 unnamedpb_rx_0x005c;
    __IO halprop_reg32 unnamedpb_rx_0x0060;
    __IO halprop_reg32 unnamedpb_rx_0x0064;
    __IO halprop_reg32 unnamedpb_rx_0x0068;
    __IO halprop_reg32 unnamedpb_rx_0x006c;
    __IO halprop_reg32 sfd_thresh;
    __IO halprop_reg32 freq_track;
    __IO halprop_reg32 frequency_compensation;
    __IO halprop_reg32 rssi_threshold_cfg;
    __IO halprop_reg32 rssi_ref_cfg;
    __IO halprop_reg32 rssi_config;
    __IO halprop_reg32 unnamedpb_rx_0x0088;
    __IO halprop_reg32 unnamedpb_rx_0x008c;
    __IO halprop_reg32 unnamedpb_rx_0x0090;
    __IO halprop_reg32 unnamedpb_rx_0x0094;
    __IO halprop_reg32 unnamedpb_rx_0x0098;
    __IO halprop_reg32 unnamedpb_rx_0x009c;
    __IO halprop_reg32 unnamedpb_rx_0x00a0;
    __IO halprop_reg32 unnamedpb_rx_0x00a4;
    __IO halprop_reg32 cw_config;
    __IO halprop_reg32 lr_cw_config;
    __IO halprop_reg32 ble_lr_frame_sync_config;
    __IO halprop_reg32 validation_cfg;
    __IO halprop_reg32 unnamedpb_rx_0x00b8;
    __IO halprop_reg32 validation_data_phase_thresh;
    __IO halprop_reg32 dbg_0;
    __IO halprop_reg32 bt_data_cfg_0;
    __IO halprop_reg32 bt_data_cfg_1;
    __IO halprop_reg32 dyn_validation_cfg;
    __IO halprop_reg32 dyn_validation_cfg_0;
    __IO halprop_reg32 unnamedpb_rx_0x00d4;
    __IO halprop_reg32 dbg_1;
    __IO halprop_reg32 dbg_2;
    __IO halprop_reg32 dbg_3;
    __IO halprop_reg32 dbg_3_dp;
    __IO halprop_reg32 dbg_4;
    __IO halprop_reg32 dbg_5;
    __IO halprop_reg32 dbg_6;
    __IO halprop_reg32 dbg_7;
    __IO halprop_reg32 dbg_8;
    __IO halprop_reg32 dbg_9;
    __IO halprop_reg32 dbg_10;
    __IO halprop_reg32 dbg_11;
    __IO halprop_reg32 dbg_13;
    __IO halprop_reg32 unnamedpb_rx_0x010c;
    __IO halprop_reg32 unnamedpb_rx_0x0110;
    __IO halprop_reg32 ram_operation_triggers;
    __IO halprop_reg32 ram_operation_status;
    __IO halprop_reg32 aoa_aod_rx_config_settings_1;
    __IO halprop_reg32 aoa_aod_rx_config_settings_2;
    __IO halprop_reg32 unnamedpb_rx_0x0124;
    __IO halprop_reg32 unnamedpb_rx_0x0128;
    __IO halprop_reg32 aoa_aod_rx_config_settings_5;
    __IO halprop_reg32 aoa_aod_rx_config_settings_6;
    __IO halprop_reg32 aoa_aod_rx_eop_adj_cfg;
    __IO halprop_reg32 aoa_aod_rx_eop_hdr_adj_cfg;
    __IO halprop_reg32 aoa_aod_rx_eop_zb_adj_cfg;
    __IO halprop_reg32 unnamedpb_rx_0x0140;
    __IO halprop_reg32 unnamedpb_rx_0x0144;
    __IO halprop_reg32 unnamedpb_rx_0x0148;
    __IO halprop_reg32 unnamedpb_rx_0x014c;
    __IO halprop_reg32 unnamedpb_rx_0x0150;
    __IO halprop_reg32 aoa_aod_rx_sfifo;
    __IO halprop_reg32 aoa_aod_rx_afifo;
    __IO halprop_reg32 unnamedpb_rx_0x015c;
    __IO halprop_reg32 unnamedpb_rx_0x0160;
    __IO halprop_reg32 unnamedpb_rx_0x0164;
    __IO halprop_reg32 unnamedpb_rx_0x0168;
    __IO halprop_reg32 gpio_rx_adc_settings;
    __IO halprop_reg32 rtt_config;
    __IO halprop_reg32 unnamedpb_rx_0x0174;
    __IO halprop_reg32 rtt0_peak_samples;
    __IO halprop_reg32 rtt0_timestamp;
    __IO halprop_reg32 rtt1_peak_samples;
    __IO halprop_reg32 rtt1_timestamp;
    __IO halprop_reg32 unnamedpb_rx_0x0188;
    __IO halprop_reg32 unnamedpb_rx_0x018c;
    __IO halprop_reg32 unnamedpb_rx_0x0190;
    __IO halprop_reg32 unnamedpb_rx_0x0194;
    __IO halprop_reg32 unnamedpb_rx_0x0198;
    __IO halprop_reg32 unnamedpb_rx_0x019c;
    __IO halprop_reg32 unnamedpb_rx_0x01a0;
    __IO halprop_reg32 unnamedpb_rx_0x01a4;
    __IO halprop_reg32 unnamedpb_rx_0x01a8;
    __IO halprop_reg32 unnamedpb_rx_0x01ac;
    __IO halprop_reg32 unnamedpb_rx_0x01b0;
    __IO halprop_reg32 unnamedpb_rx_0x01b4;
    __IO halprop_reg32 unnamedpb_rx_0x01b8;
    __IO halprop_reg32 unnamedpb_rx_0x01bc;
    __IO halprop_reg32 unnamedpb_rx_0x01c0;
    __IO halprop_reg32 unnamedpb_rx_0x01c4;
    __IO halprop_reg32 unnamedpb_rx_0x01c8;
    __IO halprop_reg32 unnamedpb_rx_0x01cc;
    __IO halprop_reg32 unnamedpb_rx_0x01d0;
    __IO halprop_reg32 unnamedpb_rx_0x01d4;
    __IO halprop_reg32 unnamedpb_rx_0x01d8;
    __IO halprop_reg32 unnamedpb_rx_0x01dc;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 unnamedpb_rx_0x01e8;
    __IO halprop_reg32 dc_offset_ctrl;
    __IO halprop_reg32 unnamedpb_rx_0x01f0;
    __IO halprop_reg32 dc_offset_mov_avg_en;
    __IO halprop_reg32 dc_offset_mov_avg_config_0;
    __IO halprop_reg32 dc_offset_mov_avg_config_1;
    __IO halprop_reg32 dc_offset_mov_avg_config_2;
    __IO halprop_reg32 dc_offset_mov_avg_config_3;
    __IO halprop_reg32 dc_offset_mov_avg_config_4;
    __IO halprop_reg32 dc_offset_mov_avg_config_5;
    __IO halprop_reg32 dc_offset_integrator_config_0;
    __IO halprop_reg32 dc_offset_integrator_config_1;
    __IO halprop_reg32 unnamedpb_rx_0x0218;
    __IO halprop_reg32 unnamedpb_rx_0x021c;
    __IO halprop_reg32 unnamedpb_rx_0x0220;
    __IO halprop_reg32 unnamedpb_rx_0x0224;
    __IO halprop_reg32 unnamedpb_rx_0x0228;
    __IO halprop_reg32 unnamedpb_rx_0x022c;
    __IO halprop_reg32 unnamedpb_rx_0x0230;
    __IO halprop_reg32 unnamedpb_rx_0x0234;
    __IO halprop_reg32 unnamedpb_rx_0x0238;
    __IO halprop_reg32 unnamedpb_rx_0x023c;
    __IO halprop_reg32 unnamedpb_rx_0x0240;
    __IO halprop_reg32 unnamedpb_rx_0x0244;
    __IO halprop_reg32 unnamedpb_rx_0x0248;
    __IO halprop_reg32 unnamedpb_rx_0x024c;
    __IO halprop_reg32 unnamedpb_rx_0x0250;
    __IO halprop_reg32 unnamedpb_rx_0x0254;
    __IO halprop_reg32 unnamedpb_rx_0x0258;
    __IO halprop_reg32 unnamedpb_rx_0x025c;
    __IO halprop_reg32 unnamedpb_rx_0x0260;
    __IO halprop_reg32 unnamedpb_rx_0x0264;
    __IO halprop_reg32 unnamedpb_rx_0x0268;
    __IO halprop_reg32 unnamedpb_rx_0x026c;
    __IO halprop_reg32 unnamedpb_rx_0x0270;
    __IO halprop_reg32 unnamedpb_rx_0x0274;
    __IO halprop_reg32 unnamedpb_rx_0x0278;
    __IO halprop_reg32 unnamedpb_rx_0x027c;
    __IO halprop_reg32 unnamedpb_rx_0x0280;
    __IO halprop_reg32 unnamedpb_rx_0x0284;
    __IO halprop_reg32 unnamedpb_rx_0x0288;
    __IO halprop_reg32 unnamedpb_rx_0x028c;
    __IO halprop_reg32 unnamedpb_rx_0x0290;
    __IO halprop_reg32 unnamedpb_rx_0x0294;
    __IO halprop_reg32 unnamedpb_rx_0x0298;
    __IO halprop_reg32 unnamedpb_rx_0x029c;
    __IO halprop_reg32 unnamedpb_rx_0x02a0;
    __IO halprop_reg32 unnamedpb_rx_0x02a4;
    __IO halprop_reg32 unnamedpb_rx_0x02a8;
    __IO halprop_reg32 unnamedpb_rx_0x02ac;
    __IO halprop_reg32 unnamedpb_rx_0x02b0;
    __IO halprop_reg32 unnamedpb_rx_0x02b4;
    __IO halprop_reg32 unnamedpb_rx_0x02b8;
    __IO halprop_reg32 unnamedpb_rx_0x02bc;
    __IO halprop_reg32 unnamedpb_rx_0x02c0;
    __IO halprop_reg32 unnamedpb_rx_0x02c4;
    __IO halprop_reg32 unnamedpb_rx_0x02c8;
    __IO halprop_reg32 unnamedpb_rx_0x02cc;
    __IO halprop_reg32 unnamedpb_rx_0x02d0;
    __IO halprop_reg32 unnamedpb_rx_0x02d4;
    __IO halprop_reg32 unnamedpb_rx_0x02d8;
    __IO halprop_reg32 unnamedpb_rx_0x02dc;
    __IO halprop_reg32 unnamedpb_rx_0x02e0;
    __IO halprop_reg32 unnamedpb_rx_0x02e4;
    __IO halprop_reg32 unnamedpb_rx_0x02e8;
    __IO halprop_reg32 unnamedpb_rx_0x02ec;
    __IO halprop_reg32 unnamedpb_rx_0x02f0;
    __IO halprop_reg32 unnamedpb_rx_0x02f4;
    __IO halprop_reg32 unnamedpb_rx_0x02f8;
    __IO halprop_reg32 unnamedpb_rx_0x02fc;
    __IO halprop_reg32 unnamedpb_rx_0x0300;
    __IO halprop_reg32 unnamedpb_rx_0x0304;
    __IO halprop_reg32 unnamedpb_rx_0x0308;
    __IO halprop_reg32 unnamedpb_rx_0x030c;
    __IO halprop_reg32 unnamedpb_rx_0x0310;
    __IO halprop_reg32 unnamedpb_rx_0x0314;
    __IO halprop_reg32 unnamedpb_rx_0x0318;
    __IO halprop_reg32 unnamedpb_rx_0x031c;
    __IO halprop_reg32 unnamedpb_rx_0x0320;
    __IO halprop_reg32 unnamedpb_rx_0x0324;
    __IO halprop_reg32 unnamedpb_rx_0x0328;
    __IO halprop_reg32 unnamedpb_rx_0x032c;
    __IO halprop_reg32 unnamedpb_rx_0x0330;
    __IO halprop_reg32 unnamedpb_rx_0x0334;
    __IO halprop_reg32 dc_offset_clr_interrupts_0;
    __IO halprop_reg32 dc_offset_clr_interrupts_1;
    __IO halprop_reg32 dc_offset_clr_interrupts_2;
    __IO halprop_reg32 dc_offset_clr_interrupts_3;
    __IO halprop_reg32 dc_offset_clr_interrupts_4;
    __IO halprop_reg32 dc_offset_clr_interrupts_5;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_0;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_1;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_2;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_3;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_4;
    __IO halprop_reg32 unmasked_dc_offset_interrupts_5;
    __IO halprop_reg32 unnamedpb_rx_0x0368;
    __IO halprop_reg32 unnamedpb_rx_0x036c;
    __IO halprop_reg32 unnamedpb_rx_0x0370;
    __IO halprop_reg32 unnamedpb_rx_0x0374;
    __IO halprop_reg32 dc_offset_cfg_0;
    __IO halprop_reg32 dbg_17;
    __IO halprop_reg32 dbg_19;
    __IO halprop_reg32 dbg_20;
    __IO halprop_reg32 dbg_21;
    __IO halprop_reg32 dbg_22;
    __IO halprop_reg32 dbg_25;
    __IO halprop_reg32 dbg_26;
    __IO halprop_reg32 sigc_config;
    __IO halprop_reg32 sigc_iqimb;
    __IO halprop_reg32 sigc_combiner;
    __IO halprop_reg32 sigc_config2;
    __IO halprop_reg32 sigc_slot_start;
    __IO halprop_reg32 sigc_config3;
    __IO halprop_reg32 sigc_rssi_config1;
    __IO halprop_reg32 sigc_rssi_config2;
    __IO halprop_reg32 sigc_rssi_config3;
    __IO halprop_reg32 sigc_agc_config0;
    __IO halprop_reg32 sigc_agc_config1;
    __IO halprop_reg32 sigc_agc_dc_adj_0;
    __IO halprop_reg32 sigc_agc_dc_adj_1;
    __IO halprop_reg32 sigc_agc_config2;
    __IO halprop_reg32 sigc_agc_config_rfd;
    __IO halprop_reg32 sigc_agc_config3;
    __IO halprop_reg32 sigc_agc_config4;
    __IO halprop_reg32 sigc_agc_config5;
    __IO halprop_reg32 sigc_agc_config6;
    __IO halprop_reg32 sigc_agc_slot;
    __IO halprop_reg32 sigc_agc_fe_lut0_1;
    __IO halprop_reg32 sigc_agc_fe_lut2_3;
    __IO halprop_reg32 sigc_agc_fe_lut4_5;
    __IO halprop_reg32 sigc_agc_fe_lut6_7;
    __IO halprop_reg32 sigc_agc_enable;
    __IO halprop_reg32 chopper_control;
    __IO halprop_reg32 sigc_status;
    __IO halprop_reg32 sigc_rssi_status;
    __IO halprop_reg32 sigc_gain_status;
    __IO halprop_reg32 sigc_icos_meas0;
    __IO halprop_reg32 sigc_isin_meas0;
    __IO halprop_reg32 sigc_qcos_meas0;
    __IO halprop_reg32 sigc_qsin_meas0;
    __IO halprop_reg32 sigc_icos_meas1;
    __IO halprop_reg32 sigc_isin_meas1;
    __IO halprop_reg32 sigc_qcos_meas1;
    __IO halprop_reg32 sigc_qsin_meas1;
    __IO halprop_reg32 unnamedpb_rx_0x042c;
    __IO halprop_reg32 unnamedpb_rx_0x0430;
    __IO halprop_reg32 rxnb_rfcalgen_mod_clk_config;
    __IO halprop_reg32 rxnb_rfcalgen_count_config_0;
    __IO halprop_reg32 rxnb_rfcalgen_count_config_1;
    __IO halprop_reg32 rxnb_rfcalgen_count_status;
    __IO halprop_reg32 rxfe_iir_path_sel_config;
    __IO halprop_reg32 dbg_27;
    __IO halprop_reg32 dbg_28;
    __IO halprop_reg32 dbg_29;
    __IO halprop_reg32 unnamedpb_rx_0x0454;
    __IO halprop_reg32 unnamedpb_rx_0x0458;
    __IO halprop_reg32 unnamedpb_rx_0x045c;
    __IO halprop_reg32 unnamedpb_rx_0x0460;
    __IO halprop_reg32 unnamedpb_rx_0x0464;
    __IO halprop_reg32 unnamedpb_rx_0x0468;
    __IO halprop_reg32 unnamedpb_rx_0x046c;
    __IO halprop_reg32 unnamedpb_rx_0x0470;
    __IO halprop_reg32 unnamedpb_rx_0x0474;
    __IO halprop_reg32 unnamedpb_rx_0x0478;
    __IO halprop_reg32 unnamedpb_rx_0x047c;
    __IO halprop_reg32 unnamedpb_rx_0x0480;
    __IO halprop_reg32 unnamedpb_rx_0x0484;
    __IO halprop_reg32 unnamedpb_rx_0x0488;
    __IO halprop_reg32 unnamedpb_rx_0x048c;
    __IO halprop_reg32 unnamedpb_rx_0x0490;
    __IO halprop_reg32 unnamedpb_rx_0x0494;
    __IO halprop_reg32 unnamedpb_rx_0x0498;
    __IO halprop_reg32 unnamedpb_rx_0x049c;
    __IO halprop_reg32 unnamedpb_rx_0x04a0;
    __IO halprop_reg32 unnamedpb_rx_0x04a4;
    __IO halprop_reg32 unnamedpb_rx_0x04a8;
    __IO halprop_reg32 unnamedpb_rx_0x04ac;
    __IO halprop_reg32 unnamedpb_rx_0x04b0;
    __IO halprop_reg32 unnamedpb_rx_0x04b4;
    __IO halprop_reg32 unnamedpb_rx_0x04b8;
    __IO halprop_reg32 unnamedpb_rx_0x04bc;
    __IO halprop_reg32 unnamedpb_rx_0x04c0;
    __IO halprop_reg32 unnamedpb_rx_0x04c4;
    __IO halprop_reg32 unnamedpb_rx_0x04c8;
    __IO halprop_reg32 dc_offset_cfg_1;
    __IO halprop_reg32 sfsk_cfg0;
    __IO halprop_reg32 sfsk_cfg1;
    __IO halprop_reg32 sfsk_cfg3;
    __IO halprop_reg32 sfsk_cfg4;
    __IO halprop_reg32 sfsk_zb_sfd;
    __IO halprop_reg32 unnamedpb_rx_0x04e4;
    __IO halprop_reg32 unnamedpb_rx_0x04e8;
    __IO halprop_reg32 unnamedpb_rx_0x04ec;
    __IO halprop_reg32 unnamedpb_rx_0x04f0;
    __IO halprop_reg32 unnamedpb_rx_0x04f4;
    __IO halprop_reg32 unnamedpb_rx_0x04f8;
    __IO halprop_reg32 unnamedpb_rx_0x04fc;
    __IO halprop_reg32 sfsk_cfg6;
    __IO halprop_reg32 sfsk_cfg7;
    __IO halprop_reg32 sfsk_cfg8;
    __IO halprop_reg32 sfsk_cfg9;
    __IO halprop_reg32 eoced_cfg0;
    __IO halprop_reg32 bt_2m_data_validation_cfg;
    __IO halprop_reg32 bt_ant_sel_rssi_ctrl;
    __IO halprop_reg32 eoced_clr;
    __IO halprop_reg32 unmasked_eoced_interrupts;
    __IO halprop_reg32 unnamedpb_rx_0x0524;
    __IO halprop_reg32 unnamedpb_rx_0x0528;
    __IO halprop_reg32 unnamedpb_rx_0x052c;
    __IO halprop_reg32 unnamedpb_rx_0x0530;
    __IO halprop_reg32 unnamedpb_rx_0x0534;
    __IO halprop_reg32 unnamedpb_rx_0x0538;
    __IO halprop_reg32 unnamedpb_rx_0x053c;
    __IO halprop_reg32 unnamedpb_rx_0x0540;
    __IO halprop_reg32 unnamedpb_rx_0x0544;
    __IO halprop_reg32 sigc0_iqcomp0;
    __IO halprop_reg32 sigc0_iqcomp1;
    __IO halprop_reg32 sigc0_iqcomp2;
    __IO halprop_reg32 sigc0_iqcomp3;
    __IO halprop_reg32 sigc0_iqcomp4;
    __IO halprop_reg32 sigc1_iqcomp0;
    __IO halprop_reg32 sigc1_iqcomp1;
    __IO halprop_reg32 sigc1_iqcomp2;
    __IO halprop_reg32 sigc1_iqcomp3;
    __IO halprop_reg32 sigc1_iqcomp4;
    __IO halprop_reg32 sigc0_bb_gain1;
    __IO halprop_reg32 sigc0_bb_gain2;
    __IO halprop_reg32 sigc1_bb_gain1;
    __IO halprop_reg32 sigc1_bb_gain2;
    __IO halprop_reg32 sigc0_rf_gain1;
    __IO halprop_reg32 sigc0_rf_gain2;
    __IO halprop_reg32 sigc1_rf_gain1;
    __IO halprop_reg32 sigc1_rf_gain2;
    __IO halprop_reg32 sigc0_agc_fe_lna_lut0;
    __IO halprop_reg32 sigc0_agc_fe_lna_lut1;
    __IO halprop_reg32 sigc1_agc_fe_lna_lut0;
    __IO halprop_reg32 sigc1_agc_fe_lna_lut1;
    __IO halprop_reg32 sigc0_adc_dc_offset0;
    __IO halprop_reg32 sigc0_adc_dc_offset1;
    __IO halprop_reg32 sigc0_adc_dc_offset2;
    __IO halprop_reg32 sigc0_adc_dc_offset3;
    __IO halprop_reg32 sigc0_adc_dc_offset4;
    __IO halprop_reg32 sigc1_adc_dc_offset0;
    __IO halprop_reg32 sigc1_adc_dc_offset1;
    __IO halprop_reg32 sigc1_adc_dc_offset2;
    __IO halprop_reg32 sigc1_adc_dc_offset3;
    __IO halprop_reg32 sigc1_adc_dc_offset4;
    __IO halprop_reg32 sigc0_bb_gain0_comp;
    __IO halprop_reg32 sigc0_bb_gain1_comp;
    __IO halprop_reg32 sigc0_bb_gain2_comp;
    __IO halprop_reg32 sigc0_bb_gain3_comp;
    __IO halprop_reg32 sigc1_bb_gain0_comp;
    __IO halprop_reg32 sigc1_bb_gain1_comp;
    __IO halprop_reg32 sigc1_bb_gain2_comp;
    __IO halprop_reg32 sigc1_bb_gain3_comp;
    __IO halprop_reg32 sigc0_rf_gain3_comp;
    __IO halprop_reg32 sigc0_rf_gain4_comp;
    __IO halprop_reg32 sigc0_rf_gain5_comp;
    __IO halprop_reg32 sigc0_rf_gain6_comp;
    __IO halprop_reg32 sigc1_rf_gain3_comp;
    __IO halprop_reg32 sigc1_rf_gain4_comp;
    __IO halprop_reg32 sigc1_rf_gain5_comp;
    __IO halprop_reg32 sigc1_rf_gain6_comp;
    __IO halprop_reg32 sigc0_bb_margin;
    __IO halprop_reg32 sigc1_bb_margin;
} AK_RX_REGMAP_T;

#define AK_RX_REGMAP ((AK_RX_REGMAP_T *) GP_WB_AK_RX_BASE_ADDRESS)

/***************************
 * regmap: ak_rx_misc
 ***************************/
typedef struct AK_RX_MISC_REGMAP_S
{
    __IO halprop_reg32 iq_logger_config;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0004;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0008;
    __IO halprop_reg32 unnamedpb_rx_misc_0x000c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0010;
    __IO halprop_reg32 iq_logger_sfifo_0;
    __IO halprop_reg32 iq_logger_sfifo_1;
    __IO halprop_reg32 unnamedpb_rx_misc_0x001c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0020;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0024;
    __IO halprop_reg32 pbr_config;
    __IO halprop_reg32 unnamedpb_rx_misc_0x002c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0030;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0034;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0038;
    __IO halprop_reg32 unnamedpb_rx_misc_0x003c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0040;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0044;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0048;
    __IO halprop_reg32 unnamedpb_rx_misc_0x004c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0050;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0054;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0058;
    __IO halprop_reg32 unnamedpb_rx_misc_0x005c;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0060;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0064;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0068;
    __IO halprop_reg32 pbr0_sfifo;
    __IO halprop_reg32 pbr1_sfifo;
    __IO halprop_reg32 unnamedpb_rx_misc_0x0074;
} AK_RX_MISC_REGMAP_T;

#define AK_RX_MISC_REGMAP ((AK_RX_MISC_REGMAP_T *) GP_WB_AK_RX_MISC_BASE_ADDRESS)

/***************************
 * regmap: ak_ssp
 ***************************/
typedef struct AK_SSP_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_ssp_0x0000;
    __IO halprop_reg32 unnamedpb_ssp_0x0004;
    __IO halprop_reg32 unnamedpb_ssp_0x0008;
    __IO halprop_reg32 unnamedpb_ssp_0x000c;
    __IO halprop_reg32 unnamedpb_ssp_0x0010;
    __IO halprop_reg32 unnamedpb_ssp_0x0014;
    __IO halprop_reg32 unnamedpb_ssp_0x0018;
    __IO halprop_reg32 unnamedpb_ssp_0x001c;
    __IO halprop_reg32 unnamedpb_ssp_0x0020;
    __IO halprop_reg32 unnamedpb_ssp_0x0024;
    __IO halprop_reg32 unnamedpb_ssp_0x0028;
    __IO halprop_reg32 mode_and_int_control;
    __IO halprop_reg32 encryption_control;
    __IO halprop_reg32 encryption_status;
} AK_SSP_REGMAP_T;

#define AK_SSP_REGMAP ((AK_SSP_REGMAP_T *) GP_WB_AK_SSP_BASE_ADDRESS)

/***************************
 * regmap: ak_uart_0
 ***************************/
typedef struct AK_UART_0_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
} AK_UART_0_REGMAP_T;

#define AK_UART_0_REGMAP ((AK_UART_0_REGMAP_T *) GP_WB_AK_UART_0_BASE_ADDRESS)

/***************************
 * regmap: ak_watchdog
 ***************************/
typedef struct AK_WATCHDOG_REGMAP_S
{
    __IO halprop_reg32 control;
    __IO halprop_reg32 timeout;
    __IO halprop_reg32 key;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 status;
    __IO halprop_reg32 current_time;
} AK_WATCHDOG_REGMAP_T;

#define AK_WATCHDOG_REGMAP ((AK_WATCHDOG_REGMAP_T *) GP_WB_AK_WATCHDOG_BASE_ADDRESS)

/***************************
 * regmap: ak_coex
 ***************************/
typedef struct AK_COEX_REGMAP_S
{
    __IO halprop_reg32 arbiter_config;
    __IO halprop_reg32 priority_expansion_config;
    __IO halprop_reg32 exta_io_config;
    __IO halprop_reg32 extb_io_config;
    __IO halprop_reg32 int_io_config;
    __IO halprop_reg32 att_ctrl_config;
    __IO halprop_reg32 att_protection_time;
    __IO halprop_reg32 status_0;
    __IO halprop_reg32 status_1;
    __IO halprop_reg32 re_request_config;
    __IO halprop_reg32 re_request_off_time;
    __IO halprop_reg32 re_request_on_time;
    __IO halprop_reg32 sw_control_config;
    __IO halprop_reg32 delayed_sw_request_time;
    __IO halprop_reg32 unmasked_interrupts;
    __IO halprop_reg32 clr_interrupts;
} AK_COEX_REGMAP_T;

#define AK_COEX_REGMAP ((AK_COEX_REGMAP_T *) GP_WB_AK_COEX_BASE_ADDRESS)

/***************************
 * regmap: ak_ipc
 ***************************/
typedef struct AK_IPC_REGMAP_S
{
    __IO halprop_reg32 set_rt2x_interrupts_0;
    __IO halprop_reg32 set_rt2x_interrupts_1;
    __IO halprop_reg32 set_rt2x_rci_interrupts;
    __IO halprop_reg32 set_rt2x_interrupts_2;
    __IO halprop_reg32 set_x2rt_interrupts;
    __IO halprop_reg32 clr_rt2x_interrupts_0;
    __IO halprop_reg32 clr_rt2x_interrupts_1;
    __IO halprop_reg32 clr_rt2x_rci_interrupts;
    __IO halprop_reg32 clr_rt2x_interrupts_2;
    __IO halprop_reg32 clr_x2rt_interrupts;
    __IO halprop_reg32 unmasked_rt2x_interrupts_0;
    __IO halprop_reg32 unmasked_rt2x_interrupts_1;
    __IO halprop_reg32 unmasked_rt2x_rci_interrupts;
    __IO halprop_reg32 unmasked_rt2x_interrupts_2;
    __IO halprop_reg32 unmasked_x2rt_interrupts;
    __IO halprop_reg32 unnamedpb_ipc_0x003c;
    __IO halprop_reg32 unnamedpb_ipc_0x0040;
    __IO halprop_reg32 unnamedpb_ipc_0x0044;
    __IO halprop_reg32 unnamedpb_ipc_0x0048;
    __IO halprop_reg32 unnamedpb_ipc_0x004c;
    __IO halprop_reg32 unnamedpb_ipc_0x0050;
    __IO halprop_reg32 unnamedpb_ipc_0x0054;
    __IO halprop_reg32 unnamedpb_ipc_0x0058;
} AK_IPC_REGMAP_T;

#define AK_IPC_REGMAP ((AK_IPC_REGMAP_T *) GP_WB_AK_IPC_BASE_ADDRESS)

/***************************
 * regmap: ak_dmas_scb
 ***************************/
typedef struct AK_DMAS_SCB_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0000;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0004;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0008;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x000c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0010;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0014;
    __IO halprop_reg32 conf_0;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x001c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0020;
    __IO halprop_reg32 buffer_ptr_control_0;
    __IO halprop_reg32 clr_interrupts_0;
    __IO halprop_reg32 internal_ptr_status_0;
    __IO halprop_reg32 status_0;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0034;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0038;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x003c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0040;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0044;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0048;
    __IO halprop_reg32 conf_1;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0050;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0054;
    __IO halprop_reg32 buffer_ptr_control_1;
    __IO halprop_reg32 clr_interrupts_1;
    __IO halprop_reg32 internal_ptr_status_1;
    __IO halprop_reg32 status_1;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0068;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x006c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0070;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0074;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0078;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x007c;
    __IO halprop_reg32 conf_2;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0084;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0088;
    __IO halprop_reg32 buffer_ptr_control_2;
    __IO halprop_reg32 clr_interrupts_2;
    __IO halprop_reg32 internal_ptr_status_2;
    __IO halprop_reg32 status_2;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x009c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00a0;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00a4;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00a8;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00ac;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00b0;
    __IO halprop_reg32 conf_3;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00b8;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00bc;
    __IO halprop_reg32 buffer_ptr_control_3;
    __IO halprop_reg32 clr_interrupts_3;
    __IO halprop_reg32 internal_ptr_status_3;
    __IO halprop_reg32 status_3;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00d0;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00d4;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00d8;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00dc;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00e0;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00e4;
    __IO halprop_reg32 conf_4;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00ec;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x00f0;
    __IO halprop_reg32 buffer_ptr_control_4;
    __IO halprop_reg32 clr_interrupts_4;
    __IO halprop_reg32 internal_ptr_status_4;
    __IO halprop_reg32 status_4;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0104;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0108;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x010c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0110;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0114;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0118;
    __IO halprop_reg32 conf_5;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0120;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0124;
    __IO halprop_reg32 buffer_ptr_control_5;
    __IO halprop_reg32 clr_interrupts_5;
    __IO halprop_reg32 internal_ptr_status_5;
    __IO halprop_reg32 status_5;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0138;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x013c;
    __IO halprop_reg32 unnamedpb_dmas_scb_0x0140;
} AK_DMAS_SCB_REGMAP_T;

#define AK_DMAS_SCB_REGMAP ((AK_DMAS_SCB_REGMAP_T *) GP_WB_AK_DMAS_SCB_BASE_ADDRESS)

/***************************
 * regmap: ak_dmas_dcb
 ***************************/
typedef struct AK_DMAS_DCB_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x0000;
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x0004;
    __IO halprop_reg32 buffer_size_0;
    __IO halprop_reg32 src_write_ptr_0;
    __IO halprop_reg32 src_read_ptr_0;
    __IO halprop_reg32 dest_write_ptr_0;
    __IO halprop_reg32 dest_read_ptr_0;
    __IO halprop_reg32 conf_0;
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x0020;
    __IO halprop_reg32 status_0;
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x0028;
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x002c;
    __IO halprop_reg32 unnamedpb_dmas_dcb_0x0030;
} AK_DMAS_DCB_REGMAP_T;

#define AK_DMAS_DCB_REGMAP ((AK_DMAS_DCB_REGMAP_T *) GP_WB_AK_DMAS_DCB_BASE_ADDRESS)

/***************************
 * regmap: ak_rpa_acc
 ***************************/
typedef struct AK_RPA_ACC_REGMAP_S
{
    __IO halprop_reg32 key_table_ptr;
    __IO halprop_reg32 num_key_entries;
    __IO halprop_reg32 prand_data;
    __IO halprop_reg32 ah_data;
    __IO halprop_reg32 control;
    __IO halprop_reg32 search_status;
    __IO halprop_reg32 unnamedpb_rpa_acc_0x0018;
    __IO halprop_reg32 hash_data_out;
} AK_RPA_ACC_REGMAP_T;

#define AK_RPA_ACC_REGMAP ((AK_RPA_ACC_REGMAP_T *) GP_WB_AK_RPA_ACC_BASE_ADDRESS)

/***************************
 * regmap: ak_prg
 ***************************/
typedef struct AK_PRG_REGMAP_S
{
    __IO halprop_reg32 prg_setting;
    __IO halprop_reg32 prg_control;
    __IO halprop_reg32 random_seed;
    __IO halprop_reg32 random_value;
} AK_PRG_REGMAP_T;

#define AK_PRG_REGMAP ((AK_PRG_REGMAP_T *) GP_WB_AK_PRG_BASE_ADDRESS)

/***************************
 * regmap: ak_gpio
 ***************************/
typedef struct AK_GPIO_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_gpio_0x0000;
    __IO halprop_reg32 gpio_directions;
    __IO halprop_reg32 gpio_input_values;
    __IO halprop_reg32 gpio_output_values;
    __IO halprop_reg32 interrupt_types;
    __IO halprop_reg32 interrupt_levels;
    __IO halprop_reg32 clear_interrupts;
    __IO halprop_reg32 set_interrupts;
    __IO halprop_reg32 interrupt_status;
} AK_GPIO_REGMAP_T;

#define AK_GPIO_REGMAP ((AK_GPIO_REGMAP_T *) GP_WB_AK_GPIO_BASE_ADDRESS)

/***************************
 * regmap: ak_debug
 ***************************/
typedef struct AK_DEBUG_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_debug_0x0000;
    __IO halprop_reg32 unnamedpb_debug_0x0004;
    __IO halprop_reg32 unnamedpb_debug_0x0008;
    __IO halprop_reg32 unnamedpb_debug_0x000c;
    __IO halprop_reg32 unnamedpb_debug_0x0010;
    __IO halprop_reg32 unnamedpb_debug_0x0014;
    __IO halprop_reg32 unnamedpb_debug_0x0018;
} AK_DEBUG_REGMAP_T;

#define AK_DEBUG_REGMAP ((AK_DEBUG_REGMAP_T *) GP_WB_AK_DEBUG_BASE_ADDRESS)

/***************************
 * regmap: ak_ttb
 ***************************/
typedef struct AK_TTB_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_ttb_0x0000;
    __IO halprop_reg32 unnamedpb_ttb_0x0004;
    __IO halprop_reg32 unnamedpb_ttb_0x0008;
    __IO halprop_reg32 unnamedpb_ttb_0x000c;
    __IO halprop_reg32 unnamedpb_ttb_0x0010;
    __IO halprop_reg32 unnamedpb_ttb_0x0014;
    __IO halprop_reg32 unnamedpb_ttb_0x0018;
    __IO halprop_reg32 unnamedpb_ttb_0x001c;
    __IO halprop_reg32 unnamedpb_ttb_0x0020;
    __IO halprop_reg32 unnamedpb_ttb_0x0024;
    __IO halprop_reg32 unnamedpb_ttb_0x0028;
    __IO halprop_reg32 unnamedpb_ttb_0x002c;
    __IO halprop_reg32 unnamedpb_ttb_0x0030;
    __IO halprop_reg32 unnamedpb_ttb_0x0034;
    __IO halprop_reg32 unnamedpb_ttb_0x0038;
    __IO halprop_reg32 unnamedpb_ttb_0x003c;
    __IO halprop_reg32 unnamedpb_ttb_0x0040;
    __IO halprop_reg32 unnamedpb_ttb_0x0044;
    __IO halprop_reg32 unnamedpb_ttb_0x0048;
    __IO halprop_reg32 unnamedpb_ttb_0x004c;
    __IO halprop_reg32 unnamedpb_ttb_0x0050;
    __IO halprop_reg32 unnamedpb_ttb_0x0054;
    __IO halprop_reg32 unnamedpb_ttb_0x0058;
    __IO halprop_reg32 unnamedpb_ttb_0x005c;
} AK_TTB_REGMAP_T;

#define AK_TTB_REGMAP ((AK_TTB_REGMAP_T *) GP_WB_AK_TTB_BASE_ADDRESS)

/***************************
 * regmap: ak_rtuc_cache
 ***************************/
typedef struct AK_RTUC_CACHE_REGMAP_S
{
    __IO halprop_reg32 cachectrl;
} AK_RTUC_CACHE_REGMAP_T;

#define AK_RTUC_CACHE_REGMAP ((AK_RTUC_CACHE_REGMAP_T *) GP_WB_AK_RTUC_CACHE_BASE_ADDRESS)

/***************************
 * regmap: ak_dma_dcb
 ***************************/
typedef struct AK_DMA_DCB_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_dma_dcb_0x0000;
    __IO halprop_reg32 unnamedpb_dma_dcb_0x0004;
    __IO halprop_reg32 buffer_size;
    __IO halprop_reg32 src_write_ptr;
    __IO halprop_reg32 src_read_ptr;
    __IO halprop_reg32 dest_write_ptr;
    __IO halprop_reg32 dest_read_ptr;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 unnamedpb_dma_dcb_0x0020;
    __IO halprop_reg32 status;
} AK_DMA_DCB_REGMAP_T;

/***************************
 * regmap: ak_dma_scb
 ***************************/
typedef struct AK_DMA_SCB_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_dma_scb_0x0000;
    __IO halprop_reg32 unnamedpb_dma_scb_0x0004;
    __IO halprop_reg32 unnamedpb_dma_scb_0x0008;
    __IO halprop_reg32 unnamedpb_dma_scb_0x000c;
    __IO halprop_reg32 unnamedpb_dma_scb_0x0010;
    __IO halprop_reg32 unnamedpb_dma_scb_0x0014;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 unnamedpb_dma_scb_0x001c;
    __IO halprop_reg32 unnamedpb_dma_scb_0x0020;
    __IO halprop_reg32 buffer_ptr_control;
    __IO halprop_reg32 clr_interrupts;
    __IO halprop_reg32 internal_ptr_status;
    __IO halprop_reg32 status;
} AK_DMA_SCB_REGMAP_T;

/***************************
 * regmap: ak_sx_chidx
 ***************************/
typedef struct AK_SX_CHIDX_REGMAP_S
{
    __IO halprop_reg32 unnamedpb_sx_chidx_0x0000;
    __IO halprop_reg32 unnamedpb_sx_chidx_0x0004;
    __IO halprop_reg32 tx_cfg;
    __IO halprop_reg32 rx_cfg;
    __IO halprop_reg32 rx_cfg2;
} AK_SX_CHIDX_REGMAP_T;

/***************************
 * regmap: ak_uart
 ***************************/
typedef struct AK_UART_REGMAP_S
{
    __IO halprop_reg32 tx_data;
    __IO halprop_reg32 rx_data;
    __IO halprop_reg32 baud_rate;
    __IO halprop_reg32 conf;
    __IO halprop_reg32 status;
    __IO halprop_reg32 clr_interrupts;
} AK_UART_REGMAP_T;


#endif //GP_WB_AK_REGMAP_PROP_STRUCT_H
