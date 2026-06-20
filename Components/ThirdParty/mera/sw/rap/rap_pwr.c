// -------------------------------------------------------------------
// rap_pwr_por_to_active
//   * configures initially the power state after POR
//   * the function will return with the chip in the requested active_power_mode
//   * prerequisite:
//     * main clocks are enabled
// -------------------------------------------------------------------
void rap_pwr_por_to_active(UInt8 active_power_mode)
{
    Bool hps_active = \
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_HPS) ||
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    if (hps_active)
    {
        rap_pmu_config_hps();
        rap_xo32m_enable();
        rap_bbpll_start();
        GP_WB_WRITE_STANDBY_CLK_MAIN_SELECT(GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF192M_CLOCK);
        GP_WB_WRITE_PMUD_RO64M_PUP_IN_ACTIVE(0);
    }
    else
    {
        rap_pmu_config_mps();
    }
    if (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS)
    {
        rap_dcdc_enable_emps();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
    }
    if (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS)
    {
        rap_dcdc_enable_ehps();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    }
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, active_power_mode);
}

// -------------------------------------------------------------------
// rap_pwr_active_to_standby
//   * prepares the system for standby in the requested standby_power_mode
//   * the function will return with the chip in a special power mode
//       which is only meant to go the standby
//   * note that the pmu_ctrl.active_power_mode will not reflect this
//       special power mode.
//   * this function only prepares the system to go to standby, the actual
//       going to standby is not part of the function.
//   * prerequisite:
//     * main clocks are still enabled
//   * postrequisite:
//     * the system should go to standby
// -------------------------------------------------------------------
void rap_pwr_active_to_standby(void)
{
    UInt8 active_power_mode = GP_WB_READ_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR);
    UInt8 standby_power_mode = GP_WB_READ_PMU_CTRL_STANDBY_POWER_MODE(RAP_H_PMU_CTRL_PTR);
    Bool hps_active = \
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_HPS) ||
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    Bool dcdc_active = \
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS) ||
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    Bool dcdc_standby = \
        (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS) ||
        (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS_RET);

    if (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET)
        rap_h_assert_sys(hps_active); // HPS_RET mode can only be entered from (E)HPS.

    if ((active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS) && dcdc_standby)
    {
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
        rap_dcdc_zcd_calibration(); // when in EHPS, do the switch to retain now, to avoid needing to go to EMPS later
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_HPS);
        rap_dcdc_enable_ret();
    }
    if (dcdc_active && !dcdc_standby)
    {
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_HPS);
        rap_dcdc_disable();
    }
    if (hps_active)
    {
        GP_WB_WRITE_PMUD_RO64M_PUP_IN_ACTIVE(1);
        rap_h_wait_us(10);
        GP_WB_WRITE_STANDBY_CLK_MAIN_SELECT (GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF64M_CLOCK);
        rap_bbpll_stop();
        if (standby_power_mode != GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET)
        {
            rap_xo32m_disable();
            rap_pmu_config_mps();
        }
    }
    if (!dcdc_active && dcdc_standby)
    {
        rap_dcdc_enable_emps();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
    }
    if ((active_power_mode != GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS) && dcdc_standby)
    {
        rap_dcdc_zcd_calibration();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_MPS);
        rap_dcdc_enable_ret();
    }
    rap_pmu_config_standby(standby_power_mode);

    //Need to correctly disable VCORE_RDY_PUP, since analog will be off when ELPS_RET
    if (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS_RET)
    {
        GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(1);
        GP_WB_WRITE_STANDBY_VCORE_RDY_PUP(0);
        GP_WB_WRITE_STANDBY_VCORE_RDY_OVERRIDE(0);
    }
}

// -------------------------------------------------------------------
// rap_pwr_standby_to_active
//   * configures the power state after wakeup from standby
//   * the function will return with the chip in the requested active_power_mode
//   * prerequisite:
//     * regmap restore has been done
//     * main clocks are enabled
// -------------------------------------------------------------------
void rap_pwr_standby_to_active(UInt8 active_power_mode)
{
    UInt8 standby_power_mode = GP_WB_READ_PMU_CTRL_STANDBY_POWER_MODE(RAP_H_PMU_CTRL_PTR);
    Bool hps_active = \
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_HPS) ||
        (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    Bool dcdc_standby = \
        (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS) ||
        (standby_power_mode == GP_WB_ENUM_STANDBY_POWER_MODE_ELPS_RET);
    if (hps_active)
    {
        if (standby_power_mode != GP_WB_ENUM_STANDBY_POWER_MODE_HPS_RET)
        {
            rap_pmu_config_hps();
            if (dcdc_standby)
            {
                rap_dcdc_disable();
            }
            rap_xo32m_enable();
        }
        rap_bbpll_resume();
        GP_WB_WRITE_STANDBY_CLK_MAIN_SELECT(GP_WB_ENUM_SYS_MAIN_CLOCK_SOURCE_HF192M_CLOCK);
        GP_WB_WRITE_PMUD_RO64M_PUP_IN_ACTIVE(0);
    }
    else
    {
        rap_pmu_config_mps();
        if (dcdc_standby)
        {
            rap_dcdc_disable();
        }
    }
    if (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS)
    {
        rap_dcdc_enable_emps();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EMPS);
    }
    if (active_power_mode == GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS)
    {
        rap_dcdc_enable_ehps();
        rap_pwr_temp_batt_update(GP_WB_ENUM_ACTIVE_POWER_MODE_EHPS);
    }
    GP_WB_WRITE_PMU_CTRL_ACTIVE_POWER_MODE(RAP_H_PMU_CTRL_PTR, active_power_mode);
}
