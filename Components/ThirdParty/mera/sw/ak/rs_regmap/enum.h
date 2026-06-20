#ifndef GP_WB_RS_REGMAP_ENUM_H
#define GP_WB_RS_REGMAP_ENUM_H

/***************************
 * typemap: ak_sw_enum
 ***************************/
/* ak_rt_cmd_type  */
/* Calibrate the channel independant FLL parameters (DTC, coarse, fine_gain). No parameters. */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_SX_CAL_NRT                   0x09
/* Calibrate a specific channel on a specific channel index. */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_SX_CAL_RT                    0x0A
/* set the receiver on or off during idle state, param: bitmask of channel_idx's that should enable RX */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_RX_ON_WHEN_IDLE_SET          0x50
/* set the receiver on or off during idle state for a specific channel_idx, params: channel_idx (UInt8), rx_on (Bool) */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_RX_ON_WHEN_IDLE_SET_CHIDX    0x51
/* Does nothing, handy if we just want to wake up the RT processor to service the main loop once. */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_DUMMY                        0x63
/* Backup of all retained fields into RAM */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_RETAIN_BACKUP                0xC0
/* Restore of all retained fields from RAM */
#define GP_WB_ENUM_AK_RT_CMD_TYPE_RETAIN_RESTORE               0xC1
#define GP_WB_ENUM_AK_RT_CMD_TYPE_NOTUSED                      0xFF

#endif //GP_WB_RS_REGMAP_ENUM_H
