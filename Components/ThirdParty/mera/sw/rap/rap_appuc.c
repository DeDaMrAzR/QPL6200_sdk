/*****************************************************************************
 *                    time/delay
 *****************************************************************************/
// -------------------------------------------------------------------
// rap_appuc_lpwait_us_base
//   * Internal helper function
//   * Used by the rap_appuc_lpwait_us to allow for sleeps higher than 15ms
// -------------------------------------------------------------------
static void rap_appuc_lpwait_us_base(UInt32 delay)
{
    UInt32 lpwait_status;
    UInt16 t_end_backup, t_end;
    Bool active;

    //read all status in one go, to improve mem access
    lpwait_status = GP_WB_READ_APPUC_LPWAIT_STATUS();

    //get info if lpwait is currently active, for nested waits
    t_end_backup = GP_WB_GET_APPUC_LPWAIT_ACTIVE_END_TIME_US_FROM_LPWAIT_STATUS(lpwait_status);
    active = GP_WB_GET_APPUC_LPWAIT_ACTIVE_FROM_LPWAIT_STATUS(lpwait_status);

    //start lpwait, end time is current time + delay
    t_end = GP_WB_GET_APPUC_LPWAIT_CURRENT_TIME_US_FROM_LPWAIT_STATUS(lpwait_status) + delay + 1;
    GP_WB_WRITE_APPUC_LPWAIT_END(t_end);

    while (!GP_WB_READ_APPUC_LPWAIT_EXPIRED())
    {
        hal_wait_for_event(); // will fall through this if there is an exception, or an RXEV
    }

    if(active)
    {
        GP_WB_WRITE_APPUC_LPWAIT_END(t_end_backup);
    }
}

// -------------------------------------------------------------------
// rap_appuc_lpwait_us
//   * Implements a low power wait for a fixed (minimal) duration
//   * The low power wait is interruptable and will continue after return from interrupt
//   * The low power wait is re-entranble.  The interrupt handler can also use low power wait.  The original low power wait will continue after return from interrupt
//   * No prerequisites required, can be used without valid ES timebase
// -------------------------------------------------------------------
void rap_appuc_lpwait_us(UInt32 delay)
{
    //max delay is the full mask of LPWAIT_END register size if one extra bit, because of time wrap around
    const UInt32 max_possible_delay = GP_WB_APPUC_LPWAIT_END_MASK >> (GP_WB_APPUC_LPWAIT_END_LSB + 1);

    while(delay >= max_possible_delay)
    {
        //do delay with max value
        rap_appuc_lpwait_us_base(max_possible_delay-1);
        delay -= (max_possible_delay-1);
    }

    //do delay with the rest
    rap_appuc_lpwait_us_base(delay);
}


/*****************************************************************************
 *                    watchdog
 *****************************************************************************/
// -------------------------------------------------------------------
// rap_appuc_enable_watchdog
//   * Configures and enables the watchdog
//   * args:
//      * timeout: watchdog timeout in 16us increments (up to 1 second)
//      * watchdog_function: function to be triggered when the watchdog times out
//      * freeze_when_appuc_asleep: pause the watchdog when the uC is asleep, resume when it is active, sleep flag is set also when appuc is using lpwait
//      * freeze_when_appuc_indeepsleep: pause the watchdog when the uC is in deep sleep, resume when it is active, deep sleep is triggered when appuc is ready for full system sleep
//      * freeze_when_appuc_break_point: pause the watchdog when the uC hit a breakpoint
// -------------------------------------------------------------------
void rap_appuc_enable_watchdog(UInt16 timeout, UInt8 watchdog_function, Bool freeze_when_appuc_asleep, Bool freeze_when_appuc_indeepsleep, Bool freeze_when_appuc_break_point) /*timeout in 16us*/
{
    UInt32 control = 0;

    //prepare control to be written
    GP_WB_SET_WATCHDOG_TIMEOUT_TO_CONTROL                      (control, timeout);
    GP_WB_SET_WATCHDOG_WATCHDOG_FUNCTION_TO_CONTROL            (control, watchdog_function);
    GP_WB_SET_WATCHDOG_FREEZE_WHEN_APPUC_ASLEEP_TO_CONTROL     (control, freeze_when_appuc_asleep);
    GP_WB_SET_WATCHDOG_FREEZE_WHEN_APPUC_INDEEPSLEEP_TO_CONTROL(control, freeze_when_appuc_indeepsleep);
    GP_WB_SET_WATCHDOG_FREEZE_WHEN_APPUC_BREAK_POINT_TO_CONTROL(control, freeze_when_appuc_break_point);
    GP_WB_SET_WATCHDOG_ENABLE_TO_CONTROL                       (control, 1);

    GP_WB_WRITE_WATCHDOG_KEY(0x55);         //Open change window
    GP_WB_WRITE_WATCHDOG_CONTROL(control);  //Full control needs to be written in one go, window closes after writing to any field
}

// -------------------------------------------------------------------
// rap_appuc_disable_watchdog
//   * Disables the watchdog
// -------------------------------------------------------------------
void rap_appuc_disable_watchdog(void)
{
    if (GP_WB_READ_WATCHDOG_ENABLE())
    {
        GP_WB_WRITE_WATCHDOG_KEY(0x55); //Open change window
        GP_WB_WRITE_WATCHDOG_ENABLE(0); //window closes after writing to any field of control register
    }
}

// -------------------------------------------------------------------
// rap_appuc_reset_watchdog
//   * Kicks the watchdog for another period
// -------------------------------------------------------------------
void rap_appuc_reset_watchdog(void)
{
    GP_WB_WRITE_WATCHDOG_KEY(0xAA);
}

// -------------------------------------------------------------------
// rap_appuc_backup_watchdog
//   * Return via pointer a 32 bit var with the current watchdog config
// -------------------------------------------------------------------
void rap_appuc_backup_watchdog(UInt32 * backup_address)
{
    *backup_address = GP_WB_READ_WATCHDOG_CONTROL();
    rap_appuc_disable_watchdog();
}

// -------------------------------------------------------------------
// rap_appuc_restore_watchdog
//   * Restore via 32 bit var pointer the watchdog config
// -------------------------------------------------------------------
void rap_appuc_restore_watchdog(const UInt32 * backup_address)
{
    GP_WB_WRITE_WATCHDOG_KEY(0x55);                 //Open change window
    GP_WB_WRITE_WATCHDOG_CONTROL(*backup_address);  //Write backed up config
}


/*****************************************************************************
 *                    sync down bridge bypass
 *****************************************************************************/
// -------------------------------------------------------------------
// rap_appuc_enable_sync_down_bridge_bypass
//   * Internal helper function
//   * Enables or disables the sync down bridge between the APPUC and the SYSAHB
//   * Disabling reduces significantly the wait states when the APPUC accesses something outside APPUC
//   * This ONLY works when the APPUC runs at 32MHz (same speed as the SYSAHB)
//   * prerequisite:
//      * APPUC is running at 32MHz (same speed as the SYSAHB)
// -------------------------------------------------------------------
void rap_appuc_enable_sync_down_bridge_bypass(Bool enable)
{
    if(GP_WB_READ_APPUC_ENABLE_SYNC_DOWN_BYPASS() != enable)
    {
        //we can only enable/disable bypass while on m32 frequency
        rap_h_assert_sys(GP_WB_READ_STANDBY_PRESCALE_UCCORE() == GP_WB_ENUM_CLOCK_SPEED_M32);

        GP_WB_WRITE_APPUC_ENABLE_SYNC_DOWN_BYPASS(enable);

        while(GP_WB_READ_APPUC_SYNC_DOWN_IN_BYPASS() != enable)
        {
            nop();
            nop();
            nop();
        }
    }
}

// -------------------------------------------------------------------
// rap_appuc_set_cpu_speed
//   * Sets the APPUC clock speed
//   * The function will automatically bypass the sync down bridge when possible
//   * args:
//      * new_clk_speed: requested clock speed
// -------------------------------------------------------------------
void rap_appuc_set_cpu_speed(UInt32 new_clk_speed)
{
    if (new_clk_speed == GP_WB_ENUM_CLOCK_SPEED_M32)
    {
        // if the new clk is lower than the current
        // first lower the clock then disable the bridge
        GP_WB_WRITE_STANDBY_PRESCALE_UCCORE(new_clk_speed);
        rap_appuc_enable_sync_down_bridge_bypass(1);
    }
    else
    {
        // If the new clk is higher than the current clk
        // First enable the bride en then increase clk speed
        rap_appuc_enable_sync_down_bridge_bypass(0);
        GP_WB_WRITE_STANDBY_PRESCALE_UCCORE(new_clk_speed);
    }
}