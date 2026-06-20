#ifndef GP_WB_ENUM_SW_H
#define GP_WB_ENUM_SW_H

/***************************
 * typemap: general_enum
 ***************************/
/* pbm_return_code  */
#define GP_WB_ENUM_PBM_RETURN_CODE_FREE                 0x0
#define GP_WB_ENUM_PBM_RETURN_CODE_NOT_DONE             0x1
#define GP_WB_ENUM_PBM_RETURN_CODE_TX_SUCCESS           0x2
#define GP_WB_ENUM_PBM_RETURN_CODE_RX_SUCCESS           0x3
#define GP_WB_ENUM_PBM_RETURN_CODE_CCA_FAILURE          0x4
#define GP_WB_ENUM_PBM_RETURN_CODE_NO_ACK               0x5
#define GP_WB_ENUM_PBM_RETURN_CODE_TX_BUSY              0x6
#define GP_WB_ENUM_PBM_RETURN_CODE_TX_ABORTED           0x7
#define GP_WB_ENUM_PBM_RETURN_CODE_RESCHEDULE_FAILED    0x8
#define GP_WB_ENUM_PBM_RETURN_CODE_NO_GRANT             0x9
#define GP_WB_ENUM_PBM_RETURN_CODE_RX_WIN_SUCCESS       0xA
#define GP_WB_ENUM_PBM_RETURN_CODE_RX_WIN_ABORTED       0xB

/* subev_type  */
#define GP_WB_ENUM_SUBEV_TYPE_LA_TX_PRI                  0x00
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_PRI                  0x01
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_SEC_CONN             0x02
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_SEC_SCAN             0x03
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_SEC_DATA             0x04
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_SEC_PERIODIC_SYNC    0x05
#define GP_WB_ENUM_SUBEV_TYPE_EA_TX_SEC_PERIODIC_DATA    0x06
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_PRI_SCAN             0x07
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_PRI_INIT_SCAN        0x08
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_SEC_CONN             0x09
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_SEC_SCAN             0x0A
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_SEC_DATA             0x0B
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_SEC_PERIODIC_SYNC    0x0C
#define GP_WB_ENUM_SUBEV_TYPE_EA_RX_SEC_PERIODIC_DATA    0x0D
#define GP_WB_ENUM_SUBEV_TYPE_DBG_GPIO_TOGGLE            0x0E
#define GP_WB_ENUM_SUBEV_TYPE_NOT_VALID                  0xFF

/* subev_drop_reason  */
#define GP_WB_ENUM_SUBEV_DROP_REASON_NO_DROP                              0x00
#define GP_WB_ENUM_SUBEV_DROP_REASON_NOT_VALID                            0x01
#define GP_WB_ENUM_SUBEV_DROP_REASON_EVENT_BUSY                           0x02
#define GP_WB_ENUM_SUBEV_DROP_REASON_TOO_LATE                             0x03
#define GP_WB_ENUM_SUBEV_DROP_REASON_DROP_FLAG_SET                        0x04
#define GP_WB_ENUM_SUBEV_DROP_REASON_NRT_HALT_REQUEST_RESULTED_IN_SKIP    0x05
#define GP_WB_ENUM_SUBEV_DROP_REASON_FAILED_TO_SCHEDULE_SUBSEQUENT_SED    0x06
#define GP_WB_ENUM_SUBEV_DROP_REASON_WRONG_CRC                            0x07
#define GP_WB_ENUM_SUBEV_DROP_REASON_ILLEGAL                              0xFF

/* blefilt_state  */
#define GP_WB_ENUM_BLEFILT_STATE_RESET            0x00
#define GP_WB_ENUM_BLEFILT_STATE_ADV_SRC_ADDR     0x01
#define GP_WB_ENUM_BLEFILT_STATE_ADV_DST_ADDR     0x02
#define GP_WB_ENUM_BLEFILT_STATE_ADV_EXT_HDR      0x03
#define GP_WB_ENUM_BLEFILT_STATE_CTE_INFO         0x04
#define GP_WB_ENUM_BLEFILT_STATE_NOTUSED_STATE    0xFF

/* macfilt_state  */
#define GP_WB_ENUM_MACFILT_STATE_RESET               0x00
#define GP_WB_ENUM_MACFILT_STATE_FRM_CTRL            0x01
#define GP_WB_ENUM_MACFILT_STATE_SEQ_NR              0x02
#define GP_WB_ENUM_MACFILT_STATE_DST_ADDR            0x03
#define GP_WB_ENUM_MACFILT_STATE_SRC_ADDR            0x04
#define GP_WB_ENUM_MACFILT_STATE_SEC_HDR_SEC_CTRL    0x05
#define GP_WB_ENUM_MACFILT_STATE_SEC_HDR             0x06
#define GP_WB_ENUM_MACFILT_STATE_IE_HDR              0x07
#define GP_WB_ENUM_MACFILT_STATE_IE_CONTENT          0x08
#define GP_WB_ENUM_MACFILT_STATE_CMD_BYTE            0x09
#define GP_WB_ENUM_MACFILT_STATE_NOTUSED_STATE       0xFF

/* rt_cmd_type  */
/* Calibrate the channel independant FLL parameters (DTC, coarse, fine_gain). No parameters. */
#define GP_WB_ENUM_RT_CMD_TYPE_SX_CAL_NRT                    0x09
/* Calibrate a specific channel on a specific channel index. */
#define GP_WB_ENUM_RT_CMD_TYPE_SX_CAL_RT                     0x0A
/* refreshes the rt_active_format_r_ptr, by quering for a new buffer in the list. User is responsible for releasing the previous pointer (if any) */
#define GP_WB_ENUM_RT_CMD_TYPE_RENEW_ACTIVE_FORMAT_R_PTR     0x10
/* Start a BLE event. Paramter is structure that describes the event to be started. */
#define GP_WB_ENUM_RT_CMD_TYPE_START_EVENT                   0x11
/* Stop an ongoing event and disable it. If the event was not ongoing (currently executed), it will only be disabled. Param: ES event number (UInt8) of the event to be stopped. */
#define GP_WB_ENUM_RT_CMD_TYPE_STOP_EVENT                    0x12
/* stops active RX windows for the given channel_idx's */
#define GP_WB_ENUM_RT_CMD_TYPE_STOP_RX_WD                    0x13
/* Backup of all retained fields into RAM */
#define GP_WB_ENUM_RT_CMD_TYPE_RETAIN_BACKUP                 0x14
/* Restore of all retained fields from RAM */
#define GP_WB_ENUM_RT_CMD_TYPE_RETAIN_RESTORE                0x15
/* Trigger all required calibrations in Akuma for proper radio communication */
#define GP_WB_ENUM_RT_CMD_TYPE_TRIGGER_RADIO_CALIBRATIONS    0x17
/* Trigger to RT system to apply RX_ON_WHEN_IDLE for the specified channel indices */
#define GP_WB_ENUM_RT_CMD_TYPE_UPDATE_RX_ON_WHEN_IDLE        0x18
/* Wake up an event in case it went to sleep due to slave latency. Sleeping events can only happen on connection slave events with slave latency and no_wakeup feature enabled. Param: ES event number (UInt8) of the event to be woken up. */
#define GP_WB_ENUM_RT_CMD_TYPE_BLE_CONN_S_WAKEUP             0x20
/* Start the BLE direct test mode. This can only be done when there are no BLE event scheduled or active. This command can return unsuccessful when the RT processor was unable to start the direct test mode. Param: info_ptr (UInt16) : pointer in the GPMicro address space to the ble_test_info structure. */
#define GP_WB_ENUM_RT_CMD_TYPE_BLE_DIRECT_TEST_MODE_START    0x21
/* Stop the BLE direct test mode. No parameters. */
#define GP_WB_ENUM_RT_CMD_TYPE_BLE_DIRECT_TEST_MODE_STOP     0x22
/* Schedule a SubEvDsc. RT will add the de SED into the time-sorted PDL. Param: sed_idx (UInt8): index of the SED to be scheduled. */
#define GP_WB_ENUM_RT_CMD_TYPE_SCHEDULE_SED                  0x23
/* Release a SubEvDsc claimed by RT. Param: sed_idx (UInt8): index of the SED to be released. should be an index claimed by RT. (ie sed_idx is in BLE_MGR_SUBEV_DSC_ENTRY_RT_MASK ) */
#define GP_WB_ENUM_RT_CMD_TYPE_RELEASE_RT_SED                0x24
/* Inserts a CIS in a CIG */
#define GP_WB_ENUM_RT_CMD_TYPE_CIS_ADD                       0x25
/* Removes a CIS from a CIG */
#define GP_WB_ENUM_RT_CMD_TYPE_CIS_REMOVE                    0x26
#define GP_WB_ENUM_RT_CMD_TYPE_ZB_TX                         0x30
#define GP_WB_ENUM_RT_CMD_TYPE_ZB_TIMED_TX                   0x31
#define GP_WB_ENUM_RT_CMD_TYPE_ZB_TIMED_RX                   0x32
/* init coex statistics, 4 bytes arg, each byte is the gpio num of the coex pins in this order (grant/req/prio0/prio1) setting all to zero disable statistics */
#define GP_WB_ENUM_RT_CMD_TYPE_COEX_INIT_STATISTICS          0x40
/* To halt RTUC for nvm write */
#define GP_WB_ENUM_RT_CMD_TYPE_HALT_RTUC                     0x50
/* Does nothing, handy if we just want to wake up the RT processor to service the main loop once. */
#define GP_WB_ENUM_RT_CMD_TYPE_DUMMY                         0xFE
#define GP_WB_ENUM_RT_CMD_TYPE_NOTUSED                       0xFF

/* rt_error_type  */
/* Indicates a PARBLE_PACKET_DONE interrupt was received while the RT system was not in an active BLE state. Param: rx_pbm_nr (UInt8) : number of the BLE PBM that was received in the non-active state.  The NRT software is responsible for releasing this PBM and handling this error. */
#define GP_WB_ENUM_RT_ERROR_TYPE_PACKET_DONE    0x01
/* Indicate a BLE RIB_RX_WD_DONE interrupt is received while the RT system was not in an active BLE state. No parameters. */
#define GP_WB_ENUM_RT_ERROR_TYPE_RX_WD_DONE     0x02
/* Indicates the M0 has triggered an unexpected (nmi) interrupt and execution has halted. */
#define GP_WB_ENUM_RT_ERROR_TYPE_NMI            0x03
/* Indicates the M0 has triggered an unexpected hardfault and execution has halted. */
#define GP_WB_ENUM_RT_ERROR_TYPE_HARDFAULT      0x04
/* Indicates the M0 has triggered a SW assert */
#define GP_WB_ENUM_RT_ERROR_TYPE_ASSERT         0x05
#define GP_WB_ENUM_RT_ERROR_TYPE_NOTUSED        0xFF

/* rt_stat_type  */
/* indicate an ES event is executed too late by the ES hardwar. Returns es_trigger_too_late info record. */
#define GP_WB_ENUM_RT_STAT_TYPE_ES_TRIGGER_TOO_LATE    0x01
#define GP_WB_ENUM_RT_STAT_TYPE_NOTUSED                0xFF

/* trigger_type  */
#define GP_WB_ENUM_TRIGGER_TYPE_CLEANUP    0x00
#define GP_WB_ENUM_TRIGGER_TYPE_EVENT      0x01
#define GP_WB_ENUM_TRIGGER_TYPE_NOTUSED    0xFF

/* zb_rt_state  */
#define GP_WB_ENUM_ZB_RT_STATE_INIT                0x00
#define GP_WB_ENUM_ZB_RT_STATE_CAL                 0x01
#define GP_WB_ENUM_ZB_RT_STATE_ED                  0x02
#define GP_WB_ENUM_ZB_RT_STATE_ED_DECIDE           0x03
#define GP_WB_ENUM_ZB_RT_STATE_CCA                 0x04
#define GP_WB_ENUM_ZB_RT_STATE_CCA_DECIDE          0x05
#define GP_WB_ENUM_ZB_RT_STATE_BACKOFF             0x06
#define GP_WB_ENUM_ZB_RT_STATE_TX                  0x07
#define GP_WB_ENUM_ZB_RT_STATE_TX_SIGNIFY_PRE      0x08
#define GP_WB_ENUM_ZB_RT_STATE_TX_SIGNIFY          0x09
#define GP_WB_ENUM_ZB_RT_STATE_TX_ACK_RX_START     0x0A
#define GP_WB_ENUM_ZB_RT_STATE_TX_ACK_RX           0x0B
#define GP_WB_ENUM_ZB_RT_STATE_TX_ACK_RX_DECIDE    0x0C
#define GP_WB_ENUM_ZB_RT_STATE_RX_WINDOW_DECIDE    0x0D
#define GP_WB_ENUM_ZB_RT_STATE_RX_WINDOW           0x0E
#define GP_WB_ENUM_ZB_RT_STATE_RX_WINDOW_END       0x0F
#define GP_WB_ENUM_ZB_RT_STATE_FLOW_END            0x10
#define GP_WB_ENUM_ZB_RT_STATE_CAL_POST            0x11
#define GP_WB_ENUM_ZB_RT_STATE_DONE                0x12
#define GP_WB_ENUM_ZB_RT_STATE_WAIT_FOR_GRANT      0x13
#define GP_WB_ENUM_ZB_RT_STATE_NOTUSED             0xFF

/* rt_state  */
#define GP_WB_ENUM_RT_STATE_STANDBY                  0x00
#define GP_WB_ENUM_RT_STATE_CLEANUP                  0x01
#define GP_WB_ENUM_RT_STATE_ABORT                    0x02
#define GP_WB_ENUM_RT_STATE_RECURRING_EV             0x03
#define GP_WB_ENUM_RT_STATE_BLE_ADVERTISING          0x10
#define GP_WB_ENUM_RT_STATE_BLE_SCANNING             0x11
#define GP_WB_ENUM_RT_STATE_BLE_INITIATING           0x12
#define GP_WB_ENUM_RT_STATE_BLE_CONNECTION_M         0x13
#define GP_WB_ENUM_RT_STATE_BLE_CONNECTION_S         0x14
#define GP_WB_ENUM_RT_STATE_BLE_BG_SCANNING          0x15
#define GP_WB_ENUM_RT_STATE_BLE_CIG                  0x16
#define GP_WB_ENUM_RT_STATE_BLE_BIG_TX               0x17
#define GP_WB_ENUM_RT_STATE_BLE_BIG_RX               0x18
#define GP_WB_ENUM_RT_STATE_BLE_TEST_MODE            0x19
#define GP_WB_ENUM_RT_STATE_BLE_SUBEVENT             0x1A
#define GP_WB_ENUM_RT_STATE_BLE_RX                   0x1B
#define GP_WB_ENUM_RT_STATE_BLE_PRIMARY_RXTX         0x1C
#define GP_WB_ENUM_RT_STATE_ZB_TX                    0x20
#define GP_WB_ENUM_RT_STATE_ZB_RX                    0x21
#define GP_WB_ENUM_RT_STATE_ZB_INDIRECT_RX           0x22
#define GP_WB_ENUM_RT_STATE_ZB_RX_FROM_BGSC          0x23
#define GP_WB_ENUM_RT_STATE_ZB_UNTIMED_EV            0x24
#define GP_WB_ENUM_RT_STATE_ZB_TIMED_RX              0x25
#define GP_WB_ENUM_RT_STATE_ZB_TIMED_RX_PACKET_RX    0x26
#define GP_WB_ENUM_RT_STATE_TRX_TX                   0x30
#define GP_WB_ENUM_RT_STATE_CALIBRATION              0x40
/* Special state - to indicate an event type does not need to be updated */
#define GP_WB_ENUM_RT_STATE_DONT_UPDATE              0xFE
#define GP_WB_ENUM_RT_STATE_NOTUSED                  0xFF

/* rx_drop_reason  */
#define GP_WB_ENUM_RX_DROP_REASON_NO_DROP                                0x0
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_FRAME_TYPE                       0x1
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_VERSION                          0x2
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_SRC_PAN_ID                       0x3
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_DST_PAN_ID                       0x4
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_SRC_ADDRESS                      0x5
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_DST_ADDRESS                      0x6
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_COMMAND                          0x7
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_CRC                              0x8
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_SRC_ADDRESSING_MODE              0x9
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_DST_ADDRESSING_MODE              0xA
#define GP_WB_ENUM_RX_DROP_REASON_PACKET_ENDED_BEFORE_PROCESSING_DONE    0xB
#define GP_WB_ENUM_RX_DROP_REASON_NO_FREE_PBM                            0xC
#define GP_WB_ENUM_RX_DROP_REASON_WRONG_ACCESS_CODE                      0xD
#define GP_WB_ENUM_RX_DROP_REASON_RT_PROC_DROP                           0xE
#define GP_WB_ENUM_RX_DROP_REASON_PHY_DROP                               0xF
#define GP_WB_ENUM_RX_DROP_REASON_FRAME_LEN_TOO_LONG                     0x10

/* cca_mode  */
#define GP_WB_ENUM_CCA_MODE_ENERGY_ONLY      0x0
#define GP_WB_ENUM_CCA_MODE_ENERGY_AND_CS    0x1

/* antsel_int  */
#define GP_WB_ENUM_ANTSEL_INT_USE_PORT_FROM_DESIGN    0x0
#define GP_WB_ENUM_ANTSEL_INT_USE_PORT_0              0x1
#define GP_WB_ENUM_ANTSEL_INT_USE_PORT_1              0x2

/* antsel_ext  */
#define GP_WB_ENUM_ANTSEL_EXT_USE_PORT_FROM_DESIGN    0x0
#define GP_WB_ENUM_ANTSEL_EXT_USE_EXT_PORT_0          0x1
#define GP_WB_ENUM_ANTSEL_EXT_USE_EXT_PORT_1          0x2

/* cal_status  */
#define GP_WB_ENUM_CAL_STATUS_UNCALIBRATED          0x0
#define GP_WB_ENUM_CAL_STATUS_REQUESTED             0x1
#define GP_WB_ENUM_CAL_STATUS_IN_PROGRESS           0x2
#define GP_WB_ENUM_CAL_STATUS_CALIBRATED            0x3
#define GP_WB_ENUM_CAL_STATUS_WAIT_FOR_INTERRUPT    0x4

/* cal_type  */
#define GP_WB_ENUM_CAL_TYPE_NRT        0x00
#define GP_WB_ENUM_CAL_TYPE_RT         0x01
#define GP_WB_ENUM_CAL_TYPE_MAX        0x02
#define GP_WB_ENUM_CAL_TYPE_NOTUSED    0xFF

/* cal_steps  */
#define GP_WB_ENUM_CAL_STEPS_NRT    0x1
#define GP_WB_ENUM_CAL_STEPS_RT     0x38

/* cal_step_time_us  */
#define GP_WB_ENUM_CAL_STEP_TIME_US_NRT    0x32
#define GP_WB_ENUM_CAL_STEP_TIME_US_RT     0x3

/* antenna_select  */
#define GP_WB_ENUM_ANTENNA_SELECT_ANT0    0x0
#define GP_WB_ENUM_ANTENNA_SELECT_ANT1    0x1
#define GP_WB_ENUM_ANTENNA_SELECT_AUTO    0x2

/* zb_transmitter_mode  */
#define GP_WB_ENUM_ZB_TRANSMITTER_MODE_ZB               0x0
#define GP_WB_ENUM_ZB_TRANSMITTER_MODE_SIGNIFY          0x1
#define GP_WB_ENUM_ZB_TRANSMITTER_MODE_SIGNIFY_CODED    0x2

#endif //GP_WB_ENUM_SW_H
