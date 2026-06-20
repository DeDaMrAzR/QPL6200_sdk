#ifndef RAM_REGMAP_INIT
#define RAM_REGMAP_INIT


/***************************
 * regmap: cal_ram
 ***************************/
ALWAYS_INLINE void init_cal_ram_ram_regmap(void)
{
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->cal_nvm_ptr.reg32 [0] = \
        (0 << GP_WB_CAL_RAM_CAL_NVM_PTR_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->vbatt_uv.reg32 [0] = \
        (0 << GP_WB_CAL_RAM_VBATT_UV_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->temp_deg.reg32 [0] = \
        (0 << GP_WB_CAL_RAM_TEMP_DEG_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x000c.reg32 [0] = \
        (10 << GP_WB_CAL_RAM_DCDC_Y2_SKIP_EHPS_LSB) + \
        (2 << GP_WB_CAL_RAM_DCDC_Y2_SKIP_EMPS_LSB) + \
        (10 << GP_WB_CAL_RAM_DCDC_Y2_MIN_EHPS_LSB) + \
        (2 << GP_WB_CAL_RAM_DCDC_Y2_MIN_EMPS_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x0010.reg16 [0] = \
        (10 << GP_WB_CAL_RAM_DCDC_Y1_SKIP_LSB) + \
        (10 << GP_WB_CAL_RAM_DCDC_Y1_MIN_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x0014.reg32 [0] = \
        (0 << GP_WB_CAL_RAM_PMUD_VCORE_LDO_TEMP_OFFSET_LSB) + \
        (0 << GP_WB_CAL_RAM_PMUD_GLOBAL_LDO_TEMP_OFFSET_LSB) + \
        (0 << GP_WB_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET_LSB) + \
        (0 << GP_WB_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->xo32m_cload_comp_lut.reg32 [0] = \
        (15 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_0_LSB) + \
        (15 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_1_LSB) + \
        (15 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_2_LSB) + \
        (15 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_3_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x001c.reg32 [0] = \
        (0 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_4_LSB) + \
        (0 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_5_LSB) + \
        (0 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_6_LSB) + \
        (0 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_7_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x0020.reg32 [0] = \
        (241 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_8_LSB) + \
        (241 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_9_LSB) + \
        (236 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_10_LSB) + \
        (236 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_11_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->unnamed_reg_cal_ram_0x0024.reg32 [0] = \
        (241 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_12_LSB) + \
        (241 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_13_LSB) + \
        (251 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_14_LSB) + \
        (10 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_15_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->xo32m_cload_comp_16.reg8 [0] = \
        (25 << GP_WB_CAL_RAM_XO32M_CLOAD_COMP_16_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->bist_adc_mode.reg8 [0] = \
        (0 << GP_WB_CAL_RAM_BIST_ADC_MODE_LSB) + \
        0;
    ((CAL_RAM_REGMAP_T *) GP_WB_CAL_RAM_BASE_ADDRESS)->cp_users.reg8 [0] = \
        (0 << GP_WB_CAL_RAM_CP_USERS_LSB) + \
        0;
}

/***************************
 * regmap: abs_ev
 ***************************/
ALWAYS_INLINE void init_abs_ev_ram_regmap(UInt32 offset)
{
    ((ABS_EV_REGMAP_T *) offset)->execution_time.reg32 [0] = \
        (0 << GP_WB_ABS_EV_EXECUTION_TIME_LSB) + \
        0;
    ((ABS_EV_REGMAP_T *) offset)->recurrence_period.reg32 [0] = \
        (0 << GP_WB_ABS_EV_RECURRENCE_PERIOD_LSB) + \
        0;
    ((ABS_EV_REGMAP_T *) offset)->recurrence_amount.reg16 [0] = \
        (0 << GP_WB_ABS_EV_RECURRENCE_AMOUNT_LSB) + \
        0;
    ((ABS_EV_REGMAP_T *) offset)->options.reg32 [0] = \
        (0 << GP_WB_ABS_EV_ADD_GUARD_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_SUBTRACT_GUARD_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_EXECUTE_IF_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_PROHIBIT_STANDBY_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_FIRST_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_OTHERS_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_LAST_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_FIRST_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_OTHERS_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_INTERRUPT_ON_LAST_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_FIRST_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_OTHERS_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_LAST_ON_TIME_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_FIRST_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_OTHERS_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_QUIC_TRIGGER_ON_LAST_TOO_LATE_LSB) + \
        (0 << GP_WB_ABS_EV_STATE_LSB) + \
        (0 << GP_WB_ABS_EV_RESULT_LSB) + \
        0;
}

/***************************
 * regmap: cal_nvm
 ***************************/
ALWAYS_INLINE void init_cal_nvm_ram_regmap(UInt32 offset)
{
    ((CAL_NVM_REGMAP_T *) offset)->magic_word.reg32 [0] = \
        (305419896 << GP_WB_CAL_NVM_MAGIC_WORD_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->version.reg32 [0] = \
        (539232263 << GP_WB_CAL_NVM_VERSION_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_lsb_high_diff_uv.reg32 [0] = \
        (255983616 << GP_WB_CAL_NVM_BIST_ADC_LSB_HIGH_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_lsb_high_se_uv.reg32 [0] = \
        (127991808 << GP_WB_CAL_NVM_BIST_ADC_LSB_HIGH_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_lsb_low_diff_uv.reg32 [0] = \
        (64028672 << GP_WB_CAL_NVM_BIST_ADC_LSB_LOW_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_lsb_low_se_uv.reg32 [0] = \
        (31981568 << GP_WB_CAL_NVM_BIST_ADC_LSB_LOW_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_offset_high_diff_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_BIST_ADC_OFFSET_HIGH_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_offset_high_se_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_BIST_ADC_OFFSET_HIGH_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_offset_low_diff_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_BIST_ADC_OFFSET_LOW_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_adc_offset_low_se_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_BIST_ADC_OFFSET_LOW_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_lsb_high_diff_uv.reg32 [0] = \
        (242483200 << GP_WB_CAL_NVM_GP_ADC_LSB_HIGH_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_lsb_high_se_uv.reg32 [0] = \
        (121241600 << GP_WB_CAL_NVM_GP_ADC_LSB_HIGH_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_lsb_low_diff_uv.reg32 [0] = \
        (62586880 << GP_WB_CAL_NVM_GP_ADC_LSB_LOW_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_lsb_low_se_uv.reg32 [0] = \
        (31260672 << GP_WB_CAL_NVM_GP_ADC_LSB_LOW_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_offset_high_diff_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_GP_ADC_OFFSET_HIGH_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_offset_high_se_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_GP_ADC_OFFSET_HIGH_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_offset_low_diff_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_GP_ADC_OFFSET_LOW_DIFF_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->gp_adc_offset_low_se_uv.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_GP_ADC_OFFSET_LOW_SE_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->hr_adc_lsb_gain0p25_uv.reg32 [0] = \
        (10157248 << GP_WB_CAL_NVM_HR_ADC_LSB_GAIN0P25_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->hr_adc_lsb_gain1_uv.reg32 [0] = \
        (2539312 << GP_WB_CAL_NVM_HR_ADC_LSB_GAIN1_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->hr_adc_lsb_gain4_uv.reg32 [0] = \
        (634828 << GP_WB_CAL_NVM_HR_ADC_LSB_GAIN4_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->hr_adc_lsb_gain8_uv.reg32 [0] = \
        (317414 << GP_WB_CAL_NVM_HR_ADC_LSB_GAIN8_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0058.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_HR_ADC_OFFSET_GAIN0P25_UV_LSB) + \
        (0 << GP_WB_CAL_NVM_HR_ADC_OFFSET_GAIN1_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x005c.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_HR_ADC_OFFSET_GAIN4_UV_LSB) + \
        (0 << GP_WB_CAL_NVM_HR_ADC_OFFSET_GAIN8_UV_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_resload_alpha_1000k.reg32 [0] = \
        (940 << GP_WB_CAL_NVM_BIST_RESLOAD_ALPHA_1000K_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_resload_alpha_100k.reg32 [0] = \
        (940 << GP_WB_CAL_NVM_BIST_RESLOAD_ALPHA_100K_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_resload_alpha_250k.reg32 [0] = \
        (940 << GP_WB_CAL_NVM_BIST_RESLOAD_ALPHA_250K_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_resload_alpha_33k.reg32 [0] = \
        (940 << GP_WB_CAL_NVM_BIST_RESLOAD_ALPHA_33K_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->bist_tsens_offset.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_BIST_TSENS_OFFSET_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0074.reg32 [0] = \
        (19 << GP_WB_CAL_NVM_BIST_IREF_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_BIST_RES_CAL_LSB) + \
        (0 << GP_WB_CAL_NVM_BIST_TSENS_VBE_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_BIST_ADC_LDO_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_BIST_ADC_REF_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_BIST_CMP_REF_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_BIST_DAC_OVP_REFBITS_LSB) + \
        (1 << GP_WB_CAL_NVM_BIST_TSENS_BIAS_TUNE_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0078.reg16 [0] = \
        (10 << GP_WB_CAL_NVM_BIST_CP_LDO_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_BIST_MON_LDO_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_BIST_TSENS_LDO_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_BIST_TSENS_VCM_REFBITS_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x007c.reg32 [0] = \
        (157 << GP_WB_CAL_NVM_PMUD_RC64K_TUNE_F_LSB) + \
        (20 << GP_WB_CAL_NVM_PMUD_IBIASGEN_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_RES_CAL_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_RO64M_IDAC_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_VCORE_LPR_TUNE_LSB) + \
        (7 << GP_WB_CAL_NVM_PMUD_DCDC_RC_TUNE_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0080.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_PMUD_VMT_TUNE_LSB) + \
        (23 << GP_WB_CAL_NVM_PMUD_XT32K_CURRENT_LSB) + \
        (5 << GP_WB_CAL_NVM_PMUD_GLOBAL_LDO_REFBITS_LSB) + \
        (1 << GP_WB_CAL_NVM_PMUD_RC64K_TUNE_TC_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_CUTOFF_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_LPR_PMUA_TUNE_LSB) + \
        (3 << GP_WB_CAL_NVM_PMUD_LPR_RC64K_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_RO64M_RESTUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_PMUD_VMT_CAL_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->pmud_vcore_ldo_refbits.reg8 [0] = \
        (12 << GP_WB_CAL_NVM_PMUD_VCORE_LDO_REFBITS_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0088.reg16 [0] = \
        (20 << GP_WB_CAL_NVM_PMUD_VDDRAM_LPR_TUNE_IN_STANDBY_LSB) + \
        (6 << GP_WB_CAL_NVM_PMUD_VDDRAM_LPR_TUNE_IN_ACTIVE_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->standby_bod_tune.reg8 [0] = \
        (0 << GP_WB_CAL_NVM_STANDBY_BOD_TUNE_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0090.reg8 [0] = \
        (5 << GP_WB_CAL_NVM_DCDC_REFBITS1_LSB) + \
        (10 << GP_WB_CAL_NVM_DCDC_REFBITS2_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0094.reg32 [0] = \
        (32 << GP_WB_CAL_NVM_XO32M_CLOADN_SEL_LSB) + \
        (32 << GP_WB_CAL_NVM_XO32M_CLOADP_SEL_LSB) + \
        (0 << GP_WB_CAL_NVM_XO32M_SPARE_LSB) + \
        (10 << GP_WB_CAL_NVM_XO32M_LDO_REFBITS_LSB) + \
        (2 << GP_WB_CAL_NVM_XO32M_AGC_LVL_LSB) + \
        (2 << GP_WB_CAL_NVM_XO32M_AGC_LVL_LSB_LSB) + \
        (0 << GP_WB_CAL_NVM_XO32M_DELAY_RADIO_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x0098.reg32 [0] = \
        (19 << GP_WB_CAL_NVM_BBPLL_IREF_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_BBPLL_RES_CAL_LSB) + \
        (12 << GP_WB_CAL_NVM_BBPLL_LDO_REFBITS_LSB) + \
        (5 << GP_WB_CAL_NVM_BBPLL_BIAS_VT_SEL_LSB) + \
        (0 << GP_WB_CAL_NVM_BBPLL_BIAS_FREQ_SEL_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x009c.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_GP_ADC_IREF_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_GP_ADC_RES_CAL_LSB) + \
        (10 << GP_WB_CAL_NVM_GP_ADC_LDO_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_GP_ADC_REF_REFBITS_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x00a0.reg16 [0] = \
        (10 << GP_WB_CAL_NVM_HR_ADC_LDO_REFBITS_LSB) + \
        (10 << GP_WB_CAL_NVM_HR_ADC_REF_REFBITS_LSB) + \
        (4 << GP_WB_CAL_NVM_HR_ADC_VCM_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_HR_ADC_SPARE_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x00a4.reg8 [0] = \
        (7 << GP_WB_CAL_NVM_USB_CTRL_DRIVER_FALL_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_USB_CTRL_DRIVER_RES_TRIM_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->unnamed_reg_cal_nvm_0x00a8.reg32 [0] = \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_TBUS_ADDR_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_BIST_SEL_IN_ANIO0_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_MICBIAS_REFBITS_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_LPREF_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_MICBIAS_HI_CURLIM_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_MICBIAS_LO_CURLIM_TUNE_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_BIST_SEL_IN_ANIO1_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_BIST_SEL_IN_ANIO2_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_BIST_SEL_IN_ANIO3_LSB) + \
        (0 << GP_WB_CAL_NVM_SENS_AO_SENS_MICBIAS_SEL_LSB) + \
        0;
    ((CAL_NVM_REGMAP_T *) offset)->xo32m_x_cap_factor.reg32 [0] = \
        (65536 << GP_WB_CAL_NVM_XO32M_X_CAP_FACTOR_LSB) + \
        0;
}

/***************************
 * regmap: pmu_ctrl
 ***************************/
ALWAYS_INLINE void init_pmu_ctrl_ram_regmap(UInt32 offset)
{
    ((PMU_CTRL_REGMAP_T *) offset)->unnamed_reg_pmu_ctrl_0x0000.reg8 [0] = \
        (0 << GP_WB_PMU_CTRL_STANDBY_POWER_MODE_LSB) + \
        (0 << GP_WB_PMU_CTRL_ACTIVE_POWER_MODE_LSB) + \
        0;
}

/***************************
 * regmap: se_cal_ram
 ***************************/
ALWAYS_INLINE void init_se_cal_ram_ram_regmap(UInt32 offset)
{
    ((SE_CAL_RAM_REGMAP_T *) offset)->unnamed_reg_se_cal_ram_0x0000.reg32 [0] = \
        (10 << GP_WB_SE_CAL_RAM_DCDC_Y2_SKIP_EHPS_LSB) + \
        (2 << GP_WB_SE_CAL_RAM_DCDC_Y2_SKIP_EMPS_LSB) + \
        (10 << GP_WB_SE_CAL_RAM_DCDC_Y2_MIN_EHPS_LSB) + \
        (2 << GP_WB_SE_CAL_RAM_DCDC_Y2_MIN_EMPS_LSB) + \
        0;
    ((SE_CAL_RAM_REGMAP_T *) offset)->unnamed_reg_se_cal_ram_0x0004.reg16 [0] = \
        (10 << GP_WB_SE_CAL_RAM_DCDC_Y1_SKIP_LSB) + \
        (10 << GP_WB_SE_CAL_RAM_DCDC_Y1_MIN_LSB) + \
        0;
    ((SE_CAL_RAM_REGMAP_T *) offset)->unnamed_reg_se_cal_ram_0x0008.reg32 [0] = \
        (0 << GP_WB_SE_CAL_RAM_PMUD_VCORE_LDO_TEMP_OFFSET_LSB) + \
        (0 << GP_WB_SE_CAL_RAM_PMUD_GLOBAL_LDO_TEMP_OFFSET_LSB) + \
        (0 << GP_WB_SE_CAL_RAM_PMUD_VCORE_LDO_DCDC_BATT_OFFSET_LSB) + \
        (0 << GP_WB_SE_CAL_RAM_PMUD_GLOBAL_LDO_DCDC_BATT_OFFSET_LSB) + \
        0;
}


ALWAYS_INLINE void init_all_ram_regmaps(void)
{
    init_cal_ram_ram_regmap();
}


#endif //RAM_REGMAP_INIT
