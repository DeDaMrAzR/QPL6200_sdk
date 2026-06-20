#ifndef RS_REGMAP_INIT
#define RS_REGMAP_INIT


/***************************
 * regmap: ak_cal_mgr
 ***************************/
ALWAYS_INLINE void init_ak_cal_mgr_rs_regmap(void)
{
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->version.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_VERSION_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->channel_lut_ptr.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_CHANNEL_LUT_PTR_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->debug.reg8 [0] = \
        (0 << GP_WB_AK_CAL_MGR_DEBUG_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->debug_val.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_DEBUG_VAL_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->zb_channel_table_config.reg8 [0] = \
        (1 << GP_WB_AK_CAL_MGR_ZB_CSF_BW_WIDE_RX_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_ZB_DOWNMIX_FREQ_SEL_RX_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_ZB_LOW_AAF_BW_RX0_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_ZB_LOW_AAF_BW_RX1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->ble_channel_table_config.reg8 [0] = \
        (0 << GP_WB_AK_CAL_MGR_BLE_CSF_BW_WIDE_RX_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_BLE_DOWNMIX_FREQ_SEL_RX_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_BLE_LOW_AAF_BW_RX0_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_BLE_LOW_AAF_BW_RX1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->ble_hdr_channel_table_config.reg8 [0] = \
        (1 << GP_WB_AK_CAL_MGR_BLE_HDR_CSF_BW_WIDE_RX_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_BLE_HDR_DOWNMIX_FREQ_SEL_RX_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX0_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->ble_lr_channel_table_config.reg8 [0] = \
        (0 << GP_WB_AK_CAL_MGR_BLE_LR_CSF_BW_WIDE_RX_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_BLE_LR_DOWNMIX_FREQ_SEL_RX_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX0_LSB) + \
        (1 << GP_WB_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->cb_coarse.reg8 [0] = \
        (0 << GP_WB_AK_CAL_MGR_CB_COARSE_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_conf[0] = ((\
        (32 << GP_WB_AK_CAL_MGR_FG_CB_0_LSB) + \
        (96 << GP_WB_AK_CAL_MGR_FG_CB_1_LSB) + \
        (9 << GP_WB_AK_CAL_MGR_FG_DUR_LSB) + \
        0) >> 0) & 0xFF;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_conf[1] = ((\
        (32 << GP_WB_AK_CAL_MGR_FG_CB_0_LSB) + \
        (96 << GP_WB_AK_CAL_MGR_FG_CB_1_LSB) + \
        (9 << GP_WB_AK_CAL_MGR_FG_DUR_LSB) + \
        0) >> 8) & 0xFF;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_conf[2] = ((\
        (32 << GP_WB_AK_CAL_MGR_FG_CB_0_LSB) + \
        (96 << GP_WB_AK_CAL_MGR_FG_CB_1_LSB) + \
        (9 << GP_WB_AK_CAL_MGR_FG_DUR_LSB) + \
        0) >> 16) & 0xFF;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_0_0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_0_0_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_0_1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_0_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_0_2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_0_2_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_0_3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_0_3_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_0_4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_0_4_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_1_0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_1_0_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_1_1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_1_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_1_2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_1_2_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_1_3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_1_3_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->fg_cnt_1_4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_FG_CNT_1_4_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->lg_conf_0.reg8 [0] = \
        (10 << GP_WB_AK_CAL_MGR_LG_DUR_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->lg_conf_1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_LG_CB_0_LSB) + \
        (1023 << GP_WB_AK_CAL_MGR_LG_CB_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->lgg_conf.reg32 [0] = \
        (2420 << GP_WB_AK_CAL_MGR_LGG_F_0_LSB) + \
        (2460 << GP_WB_AK_CAL_MGR_LGG_F_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->lgg_lg.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_LGG_LG_0_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_LGG_LG_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->unnamed_reg_ak_cal_mgr_0x0054.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_CBSF_F_0_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_CBSF_F_1_LSB) + \
        0;
    ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)->unnamed_reg_ak_cal_mgr_0x0058.reg32 [0] = \
        (0 << GP_WB_AK_CAL_MGR_CBSF_F_2_LSB) + \
        (0 << GP_WB_AK_CAL_MGR_CBSF_F_3_LSB) + \
        0;
}

/***************************
 * regmap: ak_rt_cmd
 ***************************/
ALWAYS_INLINE void init_ak_rt_cmd_rs_regmap(void)
{
    ((AK_RT_CMD_REGMAP_T *) GP_WB_AK_RT_CMD_BASE_ADDRESS)->command.reg8 [0] = \
        (0 << GP_WB_AK_RT_CMD_COMMAND_LSB) + \
        0;
    ((AK_RT_CMD_REGMAP_T *) GP_WB_AK_RT_CMD_BASE_ADDRESS)->result.reg16 [0] = \
        (0 << GP_WB_AK_RT_CMD_RESULT_LSB) + \
        0;
    ((AK_RT_CMD_REGMAP_T *) GP_WB_AK_RT_CMD_BASE_ADDRESS)->args.reg32 [0] = \
        (0 << GP_WB_AK_RT_CMD_ARGS_LSB) + \
        0;
}

/***************************
 * regmap: ak_cal_nvm
 ***************************/
ALWAYS_INLINE void init_ak_cal_nvm_rs_regmap(UInt32 offset)
{
    ((AK_CAL_NVM_REGMAP_T *) offset)->version.reg32 [0] = \
        (539230753 << GP_WB_AK_CAL_NVM_VERSION_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x0004.reg32 [0] = \
        (170 << GP_WB_AK_CAL_NVM_RX_RX0_RXFE_RFDET_GAIN_LSB) + \
        (170 << GP_WB_AK_CAL_NVM_RX_RX1_RXFE_RFDET_GAIN_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RX0_RXFE_RFDET_OFFSET_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RX1_RXFE_RFDET_OFFSET_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX0_ADC_LDO_REFBITS_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x0008.reg32 [0] = \
        (10 << GP_WB_AK_CAL_NVM_RX_RX0_BBGM_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX0_RXFE_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX0_RXFE_LNA_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX1_ADC_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX1_BBGM_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX1_RXFE_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_RX_RX1_RXFE_LNA_LDO_REFBITS_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_bbgm_dc_tune.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RX0_BBGM_Q_DC_TUNE_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RX1_BBGM_I_DC_TUNE_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RX1_BBGM_Q_DC_TUNE_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_iqcomp0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP0_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP0_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_iqcomp1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP1_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP1_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_iqcomp2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP2_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP2_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_iqcomp3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP3_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP3_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_iqcomp4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP4_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP4_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_iqcomp0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP0_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP0_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_iqcomp1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP1_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP1_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_iqcomp2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP2_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP2_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_iqcomp3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP3_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP3_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_iqcomp4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP4_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_IQCOMP4_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_bb_gain1.reg32 [0] = \
        (16 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_0_LSB) + \
        (28 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_0_LSB) + \
        (40 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_0_LSB) + \
        (52 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_bb_gain4_0.reg8 [0] = \
        (64 << GP_WB_AK_CAL_NVM_RX_BB_GAIN4_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_bb_gain1.reg32 [0] = \
        (16 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_1_LSB) + \
        (28 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_1_LSB) + \
        (40 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_1_LSB) + \
        (52 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_bb_gain4_1.reg8 [0] = \
        (64 << GP_WB_AK_CAL_NVM_RX_BB_GAIN4_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain1.reg32 [0] = \
        (226 << GP_WB_AK_CAL_NVM_RX_RF_GAIN0_0_LSB) + \
        (238 << GP_WB_AK_CAL_NVM_RX_RF_GAIN1_0_LSB) + \
        (250 << GP_WB_AK_CAL_NVM_RX_RF_GAIN2_0_LSB) + \
        (6 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain2.reg32 [0] = \
        (18 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_0_LSB) + \
        (30 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_0_LSB) + \
        (42 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_0_LSB) + \
        (54 << GP_WB_AK_CAL_NVM_RX_RF_GAIN7_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain1.reg32 [0] = \
        (226 << GP_WB_AK_CAL_NVM_RX_RF_GAIN0_1_LSB) + \
        (238 << GP_WB_AK_CAL_NVM_RX_RF_GAIN1_1_LSB) + \
        (250 << GP_WB_AK_CAL_NVM_RX_RF_GAIN2_1_LSB) + \
        (6 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain2.reg32 [0] = \
        (18 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_1_LSB) + \
        (30 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_1_LSB) + \
        (42 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_1_LSB) + \
        (54 << GP_WB_AK_CAL_NVM_RX_RF_GAIN7_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_agc_fe_lna_lut0.reg32 [0] = \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_agc_fe_lna_lut1.reg32 [0] = \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_0_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_agc_fe_lna_lut0.reg32 [0] = \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_agc_fe_lna_lut1.reg32 [0] = \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_1_LSB) + \
        (208 << GP_WB_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_adc_dc_offset0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_adc_dc_offset1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_adc_dc_offset2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_adc_dc_offset3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_adc_dc_offset4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_adc_dc_offset0.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_adc_dc_offset1.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_adc_dc_offset2.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_adc_dc_offset3.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_adc_dc_offset4.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_bb_gain0_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_bb_gain1_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_bb_gain2_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_bb_gain3_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_bb_gain0_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_bb_gain1_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_bb_gain2_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_bb_gain3_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain3_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain4_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain5_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc0_rf_gain6_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL0_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG0_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain3_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain4_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain5_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->rx_sigc1_rf_gain6_comp.reg32 [0] = \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL1_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG1_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00d0.reg32 [0] = \
        (185 << GP_WB_AK_CAL_NVM_SX_DTC_RES_CAL_LSB) + \
        (16 << GP_WB_AK_CAL_NVM_SX_DTC_BIAS_CAL_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_SX_DCO_LDO_REFBITS_LSB) + \
        (3 << GP_WB_AK_CAL_NVM_SX_DCO_RBIAS_SEL_LSB) + \
        (2 << GP_WB_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_N_LSB) + \
        (5 << GP_WB_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_P_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_SX_LO_TX_PHASE_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00d4.reg16 [0] = \
        (10 << GP_WB_AK_CAL_NVM_SX_DIV2_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_SX_LOGEN_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_SX_SAMPLER_LDO_REFBITS_LSB) + \
        0;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00d8[0] = ((\
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_LSB) + \
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_MACRO_RES_CAL_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR_LSB) + \
        0) >> 0) & 0xFF;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00d8[1] = ((\
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_LSB) + \
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_MACRO_RES_CAL_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR_LSB) + \
        0) >> 8) & 0xFF;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00d8[2] = ((\
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_LSB) + \
        (19 << GP_WB_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_MACRO_RES_CAL_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR_LSB) + \
        0) >> 16) & 0xFF;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00dc[0] = ((\
        (0 << GP_WB_AK_CAL_NVM_TX_RES_CAL_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_PA_LDO_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TX_SPARE_LSB) + \
        0) >> 0) & 0xFF;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00dc[1] = ((\
        (0 << GP_WB_AK_CAL_NVM_TX_RES_CAL_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_PA_LDO_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TX_SPARE_LSB) + \
        0) >> 8) & 0xFF;
    ((AK_CAL_NVM_REGMAP_T *) offset)->unnamed_reg_ak_cal_nvm_0x00dc[2] = ((\
        (0 << GP_WB_AK_CAL_NVM_TX_RES_CAL_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_LDO_REFBITS_LSB) + \
        (10 << GP_WB_AK_CAL_NVM_TX_PA_LDO_REFBITS_LSB) + \
        (0 << GP_WB_AK_CAL_NVM_TX_SPARE_LSB) + \
        0) >> 16) & 0xFF;
}


ALWAYS_INLINE void init_all_rs_regmaps(void)
{
    init_ak_cal_mgr_rs_regmap();
    init_ak_rt_cmd_rs_regmap();
}


#endif //RS_REGMAP_INIT
