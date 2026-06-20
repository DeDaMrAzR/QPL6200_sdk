#ifndef GP_WB_RS_REGMAP_PROP_STRUCT_H
#define GP_WB_RS_REGMAP_PROP_STRUCT_H

/***************************
 * regmap: ak_cal_mgr
 ***************************/
typedef struct AK_CAL_MGR_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 version;
    __IO halprop_reg32 channel_lut_ptr;
    __IO halprop_reg8 debug;
    halprop_reg8 reserved_0[3];
    __IO halprop_reg32 debug_val;
    __IO halprop_reg8 zb_channel_table_config;
    __IO halprop_reg8 ble_channel_table_config;
    __IO halprop_reg8 ble_hdr_channel_table_config;
    __IO halprop_reg8 ble_lr_channel_table_config;
    __IO halprop_reg8 cb_coarse;
    halprop_reg8 reserved_1[3];
    __IO UInt8 fg_conf [3];
    halprop_reg8 reserved_2[1];
    __IO halprop_reg32 fg_cnt_0_0;
    __IO halprop_reg32 fg_cnt_0_1;
    __IO halprop_reg32 fg_cnt_0_2;
    __IO halprop_reg32 fg_cnt_0_3;
    __IO halprop_reg32 fg_cnt_0_4;
    __IO halprop_reg32 fg_cnt_1_0;
    __IO halprop_reg32 fg_cnt_1_1;
    __IO halprop_reg32 fg_cnt_1_2;
    __IO halprop_reg32 fg_cnt_1_3;
    __IO halprop_reg32 fg_cnt_1_4;
    __IO halprop_reg8 lg_conf_0;
    halprop_reg8 reserved_3[3];
    __IO halprop_reg32 lg_conf_1;
    __IO halprop_reg32 lgg_conf;
    __IO halprop_reg32 lgg_lg;
    __IO halprop_reg32 unnamed_reg_ak_cal_mgr_0x0054;
    __IO halprop_reg32 unnamed_reg_ak_cal_mgr_0x0058;
} PACKED_POST AK_CAL_MGR_REGMAP_T;

#define AK_CAL_MGR_REGMAP ((AK_CAL_MGR_REGMAP_T *) GP_WB_AK_CAL_MGR_BASE_ADDRESS)

/***************************
 * regmap: ak_rt_cmd
 ***************************/
typedef struct AK_RT_CMD_REGMAP_S PACKED_PRE
{
    __IO halprop_reg8 command;
    halprop_reg8 reserved_0[1];
    __IO halprop_reg16 result;
    __IO halprop_reg32 args;
} PACKED_POST AK_RT_CMD_REGMAP_T;

#define AK_RT_CMD_REGMAP ((AK_RT_CMD_REGMAP_T *) GP_WB_AK_RT_CMD_BASE_ADDRESS)

/***************************
 * regmap: ak_cal_nvm
 ***************************/
typedef struct AK_CAL_NVM_REGMAP_S PACKED_PRE
{
    __IO halprop_reg32 version;
    __IO halprop_reg32 unnamed_reg_ak_cal_nvm_0x0004;
    __IO halprop_reg32 unnamed_reg_ak_cal_nvm_0x0008;
    __IO halprop_reg32 rx_bbgm_dc_tune;
    __IO halprop_reg32 rx_sigc0_iqcomp0;
    __IO halprop_reg32 rx_sigc0_iqcomp1;
    __IO halprop_reg32 rx_sigc0_iqcomp2;
    __IO halprop_reg32 rx_sigc0_iqcomp3;
    __IO halprop_reg32 rx_sigc0_iqcomp4;
    __IO halprop_reg32 rx_sigc1_iqcomp0;
    __IO halprop_reg32 rx_sigc1_iqcomp1;
    __IO halprop_reg32 rx_sigc1_iqcomp2;
    __IO halprop_reg32 rx_sigc1_iqcomp3;
    __IO halprop_reg32 rx_sigc1_iqcomp4;
    __IO halprop_reg32 rx_sigc0_bb_gain1;
    __IO halprop_reg8 rx_bb_gain4_0;
    halprop_reg8 reserved_0[3];
    __IO halprop_reg32 rx_sigc1_bb_gain1;
    __IO halprop_reg8 rx_bb_gain4_1;
    halprop_reg8 reserved_1[3];
    __IO halprop_reg32 rx_sigc0_rf_gain1;
    __IO halprop_reg32 rx_sigc0_rf_gain2;
    __IO halprop_reg32 rx_sigc1_rf_gain1;
    __IO halprop_reg32 rx_sigc1_rf_gain2;
    __IO halprop_reg32 rx_sigc0_agc_fe_lna_lut0;
    __IO halprop_reg32 rx_sigc0_agc_fe_lna_lut1;
    __IO halprop_reg32 rx_sigc1_agc_fe_lna_lut0;
    __IO halprop_reg32 rx_sigc1_agc_fe_lna_lut1;
    __IO halprop_reg32 rx_sigc0_adc_dc_offset0;
    __IO halprop_reg32 rx_sigc0_adc_dc_offset1;
    __IO halprop_reg32 rx_sigc0_adc_dc_offset2;
    __IO halprop_reg32 rx_sigc0_adc_dc_offset3;
    __IO halprop_reg32 rx_sigc0_adc_dc_offset4;
    __IO halprop_reg32 rx_sigc1_adc_dc_offset0;
    __IO halprop_reg32 rx_sigc1_adc_dc_offset1;
    __IO halprop_reg32 rx_sigc1_adc_dc_offset2;
    __IO halprop_reg32 rx_sigc1_adc_dc_offset3;
    __IO halprop_reg32 rx_sigc1_adc_dc_offset4;
    __IO halprop_reg32 rx_sigc0_bb_gain0_comp;
    __IO halprop_reg32 rx_sigc0_bb_gain1_comp;
    __IO halprop_reg32 rx_sigc0_bb_gain2_comp;
    __IO halprop_reg32 rx_sigc0_bb_gain3_comp;
    __IO halprop_reg32 rx_sigc1_bb_gain0_comp;
    __IO halprop_reg32 rx_sigc1_bb_gain1_comp;
    __IO halprop_reg32 rx_sigc1_bb_gain2_comp;
    __IO halprop_reg32 rx_sigc1_bb_gain3_comp;
    __IO halprop_reg32 rx_sigc0_rf_gain3_comp;
    __IO halprop_reg32 rx_sigc0_rf_gain4_comp;
    __IO halprop_reg32 rx_sigc0_rf_gain5_comp;
    __IO halprop_reg32 rx_sigc0_rf_gain6_comp;
    __IO halprop_reg32 rx_sigc1_rf_gain3_comp;
    __IO halprop_reg32 rx_sigc1_rf_gain4_comp;
    __IO halprop_reg32 rx_sigc1_rf_gain5_comp;
    __IO halprop_reg32 rx_sigc1_rf_gain6_comp;
    __IO halprop_reg32 unnamed_reg_ak_cal_nvm_0x00d0;
    __IO halprop_reg16 unnamed_reg_ak_cal_nvm_0x00d4;
    halprop_reg8 reserved_2[2];
    __IO UInt8 unnamed_reg_ak_cal_nvm_0x00d8 [3];
    halprop_reg8 reserved_3[1];
    __IO UInt8 unnamed_reg_ak_cal_nvm_0x00dc [3];
} PACKED_POST AK_CAL_NVM_REGMAP_T;


#endif //GP_WB_RS_REGMAP_PROP_STRUCT_H
