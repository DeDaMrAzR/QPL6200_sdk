/*****************************************************************************
 *                    time/delay
 *****************************************************************************/
void rap_appuc_lpwait_us(UInt32 delay);


/*****************************************************************************
 *                    watchdog
 *****************************************************************************/
void rap_appuc_enable_watchdog(UInt16 timeout, UInt8 watchdog_function, Bool freeze_when_appuc_asleep, Bool freeze_when_appuc_indeepsleep, Bool freeze_when_appuc_break_point); /*timeout in 16us*/
void rap_appuc_disable_watchdog(void);
void rap_appuc_reset_watchdog(void);
void rap_appuc_backup_watchdog(UInt32 * backup_address);
void rap_appuc_restore_watchdog(const UInt32 * backup_address);

/*****************************************************************************
 *                    sync down bridge bypass
 *****************************************************************************/
void rap_appuc_enable_sync_down_bridge_bypass(Bool enable);
void rap_appuc_set_cpu_speed(UInt32 new_clk_speed);
