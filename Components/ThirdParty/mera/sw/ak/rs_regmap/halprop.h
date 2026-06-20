#ifndef GP_WB_RS_REGMAP_PROP_H
#define GP_WB_RS_REGMAP_PROP_H

/***************************
 * regmap: ak_cal_mgr
 ***************************/

/* Version of the cal_mgr software */
#define GP_WB_READ_AK_CAL_MGR_VERSION() \
        ((UInt32)(AK_CAL_MGR_REGMAP->version.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_VERSION(val) do { \
          AK_CAL_MGR_REGMAP->version.reg32[0] = (val); \
        } while (0)


/* Pointer to the channel definition LUT. channel_lut_ptr=0 will use the DEFAULT_CHANNEL_LUT in ROM. */
#define GP_WB_READ_AK_CAL_MGR_CHANNEL_LUT_PTR() \
        ((UInt32)(AK_CAL_MGR_REGMAP->channel_lut_ptr.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_CHANNEL_LUT_PTR(val) do { \
          AK_CAL_MGR_REGMAP->channel_lut_ptr.reg32[0] = (val); \
        } while (0)


#define GP_WB_READ_AK_CAL_MGR_DEBUG() \
        ((UInt8)(AK_CAL_MGR_REGMAP->debug.reg8[0]))

#define GP_WB_WRITE_AK_CAL_MGR_DEBUG(val) do { \
          AK_CAL_MGR_REGMAP->debug.reg8[0] = (val); \
        } while (0)


#define GP_WB_READ_AK_CAL_MGR_DEBUG_VAL() \
        ((UInt32)(AK_CAL_MGR_REGMAP->debug_val.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_DEBUG_VAL(val) do { \
          AK_CAL_MGR_REGMAP->debug_val.reg32[0] = (val); \
        } while (0)


/* register access functions for AK_CAL_MGR_ZB_CHANNEL_TABLE_CONFIG */
#define GP_WB_READ_AK_CAL_MGR_ZB_CHANNEL_TABLE_CONFIG() \
        ((UInt8)AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0])

#define GP_WB_WRITE_AK_CAL_MGR_ZB_CHANNEL_TABLE_CONFIG(val) \
      AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0] = (val)

#define GP_WB_READ_AK_CAL_MGR_ZB_CSF_BW_WIDE_RX() \
        ((UInt8)((AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_ZB_CSF_BW_WIDE_RX_FROM_ZB_CHANNEL_TABLE_CONFIG(tmp) \
        ((tmp) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_ZB_CSF_BW_WIDE_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x01)) | (((val)) & (0x01)); \
          AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_ZB_CSF_BW_WIDE_RX_TO_ZB_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0E; \
          (tmp) |= ((UInt8)(val)); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_ZB_DOWNMIX_FREQ_SEL_RX() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]) >> 1) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_ZB_DOWNMIX_FREQ_SEL_RX_FROM_ZB_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 1) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_ZB_DOWNMIX_FREQ_SEL_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x02)) | (((val) << 1) & (0x02)); \
          AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_ZB_DOWNMIX_FREQ_SEL_RX_TO_ZB_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0D; \
          (tmp) |= (((UInt8)(val)) << 1); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_ZB_LOW_AAF_BW_RX0() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]) >> 2) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_ZB_LOW_AAF_BW_RX0_FROM_ZB_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 2) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_ZB_LOW_AAF_BW_RX0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x04)) | (((val) << 2) & (0x04)); \
          AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_ZB_LOW_AAF_BW_RX0_TO_ZB_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0B; \
          (tmp) |= (((UInt8)(val)) << 2); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_ZB_LOW_AAF_BW_RX1() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]) >> 3) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_ZB_LOW_AAF_BW_RX1_FROM_ZB_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 3) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_ZB_LOW_AAF_BW_RX1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x08)) | (((val) << 3) & (0x08)); \
          AK_CAL_MGR_REGMAP->zb_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_ZB_LOW_AAF_BW_RX1_TO_ZB_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x07; \
          (tmp) |= (((UInt8)(val)) << 3); \
        } while (0)


/* register access functions for AK_CAL_MGR_BLE_CHANNEL_TABLE_CONFIG */
#define GP_WB_READ_AK_CAL_MGR_BLE_CHANNEL_TABLE_CONFIG() \
        ((UInt8)AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0])

#define GP_WB_WRITE_AK_CAL_MGR_BLE_CHANNEL_TABLE_CONFIG(val) \
      AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0] = (val)

#define GP_WB_READ_AK_CAL_MGR_BLE_CSF_BW_WIDE_RX() \
        ((UInt8)((AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_CSF_BW_WIDE_RX_FROM_BLE_CHANNEL_TABLE_CONFIG(tmp) \
        ((tmp) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_CSF_BW_WIDE_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x01)) | (((val)) & (0x01)); \
          AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_CSF_BW_WIDE_RX_TO_BLE_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0E; \
          (tmp) |= ((UInt8)(val)); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_DOWNMIX_FREQ_SEL_RX() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]) >> 1) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_DOWNMIX_FREQ_SEL_RX_FROM_BLE_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 1) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_DOWNMIX_FREQ_SEL_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x02)) | (((val) << 1) & (0x02)); \
          AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_DOWNMIX_FREQ_SEL_RX_TO_BLE_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0D; \
          (tmp) |= (((UInt8)(val)) << 1); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_LOW_AAF_BW_RX0() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]) >> 2) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LOW_AAF_BW_RX0_FROM_BLE_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 2) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LOW_AAF_BW_RX0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x04)) | (((val) << 2) & (0x04)); \
          AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LOW_AAF_BW_RX0_TO_BLE_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0B; \
          (tmp) |= (((UInt8)(val)) << 2); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_LOW_AAF_BW_RX1() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]) >> 3) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LOW_AAF_BW_RX1_FROM_BLE_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 3) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LOW_AAF_BW_RX1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x08)) | (((val) << 3) & (0x08)); \
          AK_CAL_MGR_REGMAP->ble_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LOW_AAF_BW_RX1_TO_BLE_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x07; \
          (tmp) |= (((UInt8)(val)) << 3); \
        } while (0)


/* register access functions for AK_CAL_MGR_BLE_HDR_CHANNEL_TABLE_CONFIG */
#define GP_WB_READ_AK_CAL_MGR_BLE_HDR_CHANNEL_TABLE_CONFIG() \
        ((UInt8)AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0])

#define GP_WB_WRITE_AK_CAL_MGR_BLE_HDR_CHANNEL_TABLE_CONFIG(val) \
      AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0] = (val)

#define GP_WB_READ_AK_CAL_MGR_BLE_HDR_CSF_BW_WIDE_RX() \
        ((UInt8)((AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_HDR_CSF_BW_WIDE_RX_FROM_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp) \
        ((tmp) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_HDR_CSF_BW_WIDE_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x01)) | (((val)) & (0x01)); \
          AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_HDR_CSF_BW_WIDE_RX_TO_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0E; \
          (tmp) |= ((UInt8)(val)); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_HDR_DOWNMIX_FREQ_SEL_RX() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]) >> 1) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_HDR_DOWNMIX_FREQ_SEL_RX_FROM_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 1) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_HDR_DOWNMIX_FREQ_SEL_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x02)) | (((val) << 1) & (0x02)); \
          AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_HDR_DOWNMIX_FREQ_SEL_RX_TO_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0D; \
          (tmp) |= (((UInt8)(val)) << 1); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX0() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]) >> 2) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX0_FROM_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 2) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x04)) | (((val) << 2) & (0x04)); \
          AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX0_TO_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0B; \
          (tmp) |= (((UInt8)(val)) << 2); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX1() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]) >> 3) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX1_FROM_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 3) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x08)) | (((val) << 3) & (0x08)); \
          AK_CAL_MGR_REGMAP->ble_hdr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_HDR_LOW_AAF_BW_RX1_TO_BLE_HDR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x07; \
          (tmp) |= (((UInt8)(val)) << 3); \
        } while (0)


/* register access functions for AK_CAL_MGR_BLE_LR_CHANNEL_TABLE_CONFIG */
#define GP_WB_READ_AK_CAL_MGR_BLE_LR_CHANNEL_TABLE_CONFIG() \
        ((UInt8)AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0])

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LR_CHANNEL_TABLE_CONFIG(val) \
      AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0] = (val)

#define GP_WB_READ_AK_CAL_MGR_BLE_LR_CSF_BW_WIDE_RX() \
        ((UInt8)((AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LR_CSF_BW_WIDE_RX_FROM_BLE_LR_CHANNEL_TABLE_CONFIG(tmp) \
        ((tmp) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LR_CSF_BW_WIDE_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x01)) | (((val)) & (0x01)); \
          AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LR_CSF_BW_WIDE_RX_TO_BLE_LR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0E; \
          (tmp) |= ((UInt8)(val)); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_LR_DOWNMIX_FREQ_SEL_RX() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]) >> 1) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LR_DOWNMIX_FREQ_SEL_RX_FROM_BLE_LR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 1) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LR_DOWNMIX_FREQ_SEL_RX(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x02)) | (((val) << 1) & (0x02)); \
          AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LR_DOWNMIX_FREQ_SEL_RX_TO_BLE_LR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0D; \
          (tmp) |= (((UInt8)(val)) << 1); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX0() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]) >> 2) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX0_FROM_BLE_LR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 2) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x04)) | (((val) << 2) & (0x04)); \
          AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX0_TO_BLE_LR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x0B; \
          (tmp) |= (((UInt8)(val)) << 2); \
        } while (0)

#define GP_WB_READ_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX1() \
        ((UInt8)(((AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]) >> 3) & 0x01))

#define GP_WB_GET_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX1_FROM_BLE_LR_CHANNEL_TABLE_CONFIG(tmp) \
        (((tmp) >> 3) & 0x01)

#define GP_WB_WRITE_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01); \
          UInt8 halprop_h_tmp = (AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x08)) | (((val) << 3) & (0x08)); \
          AK_CAL_MGR_REGMAP->ble_lr_channel_table_config.reg8[0] = halprop_h_tmp; \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_BLE_LR_LOW_AAF_BW_RX1_TO_BLE_LR_CHANNEL_TABLE_CONFIG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U8(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01); \
          (tmp) &= 0x07; \
          (tmp) |= (((UInt8)(val)) << 3); \
        } while (0)


/* Calibrated Coarse value */
#define GP_WB_READ_AK_CAL_MGR_CB_COARSE() \
        ((UInt8)(AK_CAL_MGR_REGMAP->cb_coarse.reg8[0]))

#define GP_WB_WRITE_AK_CAL_MGR_CB_COARSE(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          AK_CAL_MGR_REGMAP->cb_coarse.reg8[0] = (val); \
        } while (0)


/* register access functions for AK_CAL_MGR_FG_CONF */
/* fine gain calibration config */
#define GP_WB_READ_AK_CAL_MGR_FG_CONF() \
        GP_WB_READ_U24(GP_WB_AK_CAL_MGR_FG_CONF_ADDRESS)

#define GP_WB_WRITE_AK_CAL_MGR_FG_CONF(val) \
      GP_WB_WRITE_U24(GP_WB_AK_CAL_MGR_FG_CONF_ADDRESS, (val))

/* cb value for the lower measurement point */
#define GP_WB_READ_AK_CAL_MGR_FG_CB_0() \
        GP_WB_READ_U8(GP_WB_AK_CAL_MGR_FG_CB_0_ADDRESS)

#define GP_WB_GET_AK_CAL_MGR_FG_CB_0_FROM_FG_CONF(tmp) \
        ((tmp) & 0x7F)

#define GP_WB_WRITE_AK_CAL_MGR_FG_CB_0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_WRITE_U8(GP_WB_AK_CAL_MGR_FG_CB_0_ADDRESS, (val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_FG_CB_0_TO_FG_CONF(tmp, val) do { \
          GP_WB_SIZE_CHECK_U24(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x0F7F00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* cb value for the higher measurement point */
#define GP_WB_READ_AK_CAL_MGR_FG_CB_1() \
        GP_WB_READ_U8(GP_WB_AK_CAL_MGR_FG_CB_1_ADDRESS+1)

#define GP_WB_GET_AK_CAL_MGR_FG_CB_1_FROM_FG_CONF(tmp) \
        (((tmp) >> 8) & 0x7F)

#define GP_WB_WRITE_AK_CAL_MGR_FG_CB_1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_WRITE_U8(GP_WB_AK_CAL_MGR_FG_CB_1_ADDRESS+1, (val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_FG_CB_1_TO_FG_CONF(tmp, val) do { \
          GP_WB_SIZE_CHECK_U24(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x0F007F; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* rfcounter measurement duration 9 = 16us */
#define GP_WB_READ_AK_CAL_MGR_FG_DUR() \
        GP_WB_READ_U8(GP_WB_AK_CAL_MGR_FG_DUR_ADDRESS+2)

#define GP_WB_GET_AK_CAL_MGR_FG_DUR_FROM_FG_CONF(tmp) \
        (((tmp) >> 16) & 0x0F)

#define GP_WB_WRITE_AK_CAL_MGR_FG_DUR(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          GP_WB_WRITE_U8(GP_WB_AK_CAL_MGR_FG_DUR_ADDRESS+2, (val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_FG_DUR_TO_FG_CONF(tmp, val) do { \
          GP_WB_SIZE_CHECK_U24(tmp); \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          (tmp) &= 0x007F7F; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* measured rfcounter value for fg_cb_0 and lowest channels */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_0_0() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_0_0.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_0_0(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_0_0.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_0 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_0_1() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_0_1.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_0_1(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_0_1.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_0 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_0_2() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_0_2.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_0_2(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_0_2.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_0 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_0_3() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_0_3.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_0_3(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_0_3.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_0 and highest channels */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_0_4() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_0_4.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_0_4(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_0_4.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_1 and lowest channels */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_1_0() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_1_0.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_1_0(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_1_0.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_1 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_1_1() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_1_1.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_1_1(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_1_1.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_1 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_1_2() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_1_2.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_1_2(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_1_2.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_1 */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_1_3() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_1_3.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_1_3(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_1_3.reg32[0] = (val); \
        } while (0)


/* measured rfcounter value for fg_cb_1 and highest channels */
#define GP_WB_READ_AK_CAL_MGR_FG_CNT_1_4() \
        ((UInt32)(AK_CAL_MGR_REGMAP->fg_cnt_1_4.reg32[0]))

#define GP_WB_WRITE_AK_CAL_MGR_FG_CNT_1_4(val) do { \
          AK_CAL_MGR_REGMAP->fg_cnt_1_4.reg32[0] = (val); \
        } while (0)


/* rfcounter measurement duration 10 = 32us */
#define GP_WB_READ_AK_CAL_MGR_LG_DUR() \
        ((UInt8)(AK_CAL_MGR_REGMAP->lg_conf_0.reg8[0]))

#define GP_WB_WRITE_AK_CAL_MGR_LG_DUR(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          AK_CAL_MGR_REGMAP->lg_conf_0.reg8[0] = (val); \
        } while (0)


/* register access functions for AK_CAL_MGR_LG_CONF_1 */
/* loop gain calibration config */
#define GP_WB_READ_AK_CAL_MGR_LG_CONF_1() \
        ((UInt32)AK_CAL_MGR_REGMAP->lg_conf_1.reg32[0])

#define GP_WB_WRITE_AK_CAL_MGR_LG_CONF_1(val) \
      AK_CAL_MGR_REGMAP->lg_conf_1.reg32[0] = (val)

/* cb value for the lower measurement point */
#define GP_WB_READ_AK_CAL_MGR_LG_CB_0() \
        ((UInt16)((AK_CAL_MGR_REGMAP->lg_conf_1.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_MGR_LG_CB_0_FROM_LG_CONF_1(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_MGR_LG_CB_0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          AK_CAL_MGR_REGMAP->lg_conf_1.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LG_CB_0_TO_LG_CONF_1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* cb value for the higher measurement point */
#define GP_WB_READ_AK_CAL_MGR_LG_CB_1() \
        ((UInt16)(((AK_CAL_MGR_REGMAP->lg_conf_1.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_MGR_LG_CB_1_FROM_LG_CONF_1(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_MGR_LG_CB_1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          AK_CAL_MGR_REGMAP->lg_conf_1.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LG_CB_1_TO_LG_CONF_1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_MGR_LGG_CONF */
/* loop gain gain calibration config */
#define GP_WB_READ_AK_CAL_MGR_LGG_CONF() \
        ((UInt32)AK_CAL_MGR_REGMAP->lgg_conf.reg32[0])

#define GP_WB_WRITE_AK_CAL_MGR_LGG_CONF(val) \
      AK_CAL_MGR_REGMAP->lgg_conf.reg32[0] = (val)

/* frequency lower measurement point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_LGG_F_0() \
        ((UInt16)((AK_CAL_MGR_REGMAP->lgg_conf.reg32[0]) & 0x0FFF))

#define GP_WB_GET_AK_CAL_MGR_LGG_F_0_FROM_LGG_CONF(tmp) \
        ((tmp) & 0x0FFF)

#define GP_WB_WRITE_AK_CAL_MGR_LGG_F_0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->lgg_conf.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LGG_F_0_TO_LGG_CONF(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          (tmp) &= 0x0FFF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* frequency lower measurement point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_LGG_F_1() \
        ((UInt16)(((AK_CAL_MGR_REGMAP->lgg_conf.reg32[0]) >> 16) & 0x0FFF))

#define GP_WB_GET_AK_CAL_MGR_LGG_F_1_FROM_LGG_CONF(tmp) \
        (((tmp) >> 16) & 0x0FFF)

#define GP_WB_WRITE_AK_CAL_MGR_LGG_F_1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->lgg_conf.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LGG_F_1_TO_LGG_CONF(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          (tmp) &= 0x00000FFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_MGR_LGG_LG */
/* measured loopgain values */
#define GP_WB_READ_AK_CAL_MGR_LGG_LG() \
        ((UInt32)AK_CAL_MGR_REGMAP->lgg_lg.reg32[0])

#define GP_WB_WRITE_AK_CAL_MGR_LGG_LG(val) \
      AK_CAL_MGR_REGMAP->lgg_lg.reg32[0] = (val)

/* measured loopgain value for lgg_f_0 */
#define GP_WB_READ_AK_CAL_MGR_LGG_LG_0() \
        ((UInt16)((AK_CAL_MGR_REGMAP->lgg_lg.reg32[0]) & 0x01FF))

#define GP_WB_GET_AK_CAL_MGR_LGG_LG_0_FROM_LGG_LG(tmp) \
        ((tmp) & 0x01FF)

#define GP_WB_WRITE_AK_CAL_MGR_LGG_LG_0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01FF); \
          AK_CAL_MGR_REGMAP->lgg_lg.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LGG_LG_0_TO_LGG_LG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01FF); \
          (tmp) &= 0x01FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* measured loopgain value for lgg_f_1 */
#define GP_WB_READ_AK_CAL_MGR_LGG_LG_1() \
        ((UInt16)(((AK_CAL_MGR_REGMAP->lgg_lg.reg32[0]) >> 16) & 0x01FF))

#define GP_WB_GET_AK_CAL_MGR_LGG_LG_1_FROM_LGG_LG(tmp) \
        (((tmp) >> 16) & 0x01FF)

#define GP_WB_WRITE_AK_CAL_MGR_LGG_LG_1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x01FF); \
          AK_CAL_MGR_REGMAP->lgg_lg.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_MGR_LGG_LG_1_TO_LGG_LG(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x01FF); \
          (tmp) &= 0x000001FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* lowest channels calibrated coarse capbank switching point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_CBSF_F_0() \
        ((UInt16)((AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0054.reg32[0]) & 0x0FFF))

#define GP_WB_WRITE_AK_CAL_MGR_CBSF_F_0(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0054.reg16[0] = (val); \
        } while (0)

/* calibrated coarse capbank switching point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_CBSF_F_1() \
        ((UInt16)(((AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0054.reg32[0]) >> 16) & 0x0FFF))

#define GP_WB_WRITE_AK_CAL_MGR_CBSF_F_1(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0054.reg16[1] = (val); \
        } while (0)


/* calibrated coarse capbank switching point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_CBSF_F_2() \
        ((UInt16)((AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0058.reg32[0]) & 0x0FFF))

#define GP_WB_WRITE_AK_CAL_MGR_CBSF_F_2(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0058.reg16[0] = (val); \
        } while (0)

/* highest channels calibrated coarse capbank switching point [MHz] */
#define GP_WB_READ_AK_CAL_MGR_CBSF_F_3() \
        ((UInt16)(((AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0058.reg32[0]) >> 16) & 0x0FFF))

#define GP_WB_WRITE_AK_CAL_MGR_CBSF_F_3(val) do { \
          GP_WB_RANGE_CHECK(val, 0x0FFF); \
          AK_CAL_MGR_REGMAP->unnamed_reg_ak_cal_mgr_0x0058.reg16[1] = (val); \
        } while (0)

/***************************
 * regmap: ak_rt_cmd
 ***************************/

/* Command type for the BLE COMMAND interface. Only 1 command can be run at a time. The user must wait for completion before initiating a new command. */
#define GP_WB_READ_AK_RT_CMD_COMMAND() \
        ((UInt8)(AK_RT_CMD_REGMAP->command.reg8[0]))

#define GP_WB_WRITE_AK_RT_CMD_COMMAND(val) do { \
          AK_RT_CMD_REGMAP->command.reg8[0] = (val); \
        } while (0)


/* The status of the ran command. If the contents of this register is equal to the rt_cmd_command type number, it indicates success. If otherwise, an error has occurred during the command execution. */
#define GP_WB_READ_AK_RT_CMD_RESULT() \
        ((UInt16)(AK_RT_CMD_REGMAP->result.reg16[0]))

#define GP_WB_WRITE_AK_RT_CMD_RESULT(val) do { \
          AK_RT_CMD_REGMAP->result.reg16[0] = (val); \
        } while (0)


/* Command arguments. The content of the arguments is dependent of the command type. */
#define GP_WB_READ_AK_RT_CMD_ARGS() \
        ((UInt32)(AK_RT_CMD_REGMAP->args.reg32[0]))

#define GP_WB_WRITE_AK_RT_CMD_ARGS(val) do { \
          AK_RT_CMD_REGMAP->args.reg32[0] = (val); \
        } while (0)

/***************************
 * regmap: ak_cal_nvm
 ***************************/

/* Version on the structure definition */
#define GP_WB_READ_AK_CAL_NVM_VERSION(offset) \
        ((UInt32)(((AK_CAL_NVM_REGMAP_T *) (offset))->version.reg32[0]))

#define GP_WB_WRITE_AK_CAL_NVM_VERSION(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->version.reg32[0] = (val); \
        } while (0)


/* radio radio_rx0_rxfe_rfdet_gain RF detector gain to select threshold -> 170 (typ) */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_RXFE_RFDET_GAIN(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]) & 0xFF))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_RXFE_RFDET_GAIN(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg8[0] = (val); \
        } while (0)

/* radio radio_rx1_rxfe_rfdet_gain RF detector gain to select threshold -> 170 (typ) */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_RXFE_RFDET_GAIN(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]) >> 8) & 0xFF))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_RXFE_RFDET_GAIN(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg8[1] = (val); \
        } while (0)

/* radio radio_rx0_rxfe_rfdet_offset RF detector offset trim offset (impacts lower threshold) -> 0=no correction */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_RXFE_RFDET_OFFSET(offset) \
        (GP_WB_S32(((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]) >> 16) & 0x0000003F)), 6))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_RXFE_RFDET_OFFSET(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -32, 31); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x003f0000)) | ((((UInt32)((UInt32)(val) << 16)) << 0) & (0x003f0000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx1_rxfe_rfdet_offset RF detector offset trim offset (impacts lower threshold) -> 0=no correction */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_RXFE_RFDET_OFFSET(offset) \
        (GP_WB_S32(((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]) >> 22) & 0x0000003F)), 6))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_RXFE_RFDET_OFFSET(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -32, 31); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x0fc00000)) | ((((UInt32)((UInt32)(val) << 22)) << 0) & (0x0fc00000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx0_adc_ldo_refbits ADC LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_ADC_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]) >> 28) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_ADC_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0xf0000000)) | ((((UInt32)(val) << 28) << 0) & (0xf0000000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0004.reg32[0] = halprop_h_tmp; \
        } while (0)


/* radio radio_rx0_bbgm_ldo_refbits Baseband amplifier LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_BBGM_LDO_REFBITS(offset) \
        ((UInt32)((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_BBGM_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x0000000f)) | ((((val)) << 0) & (0x0000000f)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx0_rxfe_ldo_refbits RXFE LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_RXFE_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 4) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_RXFE_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x000000f0)) | ((((UInt32)(val) << 4) << 0) & (0x000000f0)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx0_rxfe_lna_ldo_refbits RXFE-LNA LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_RXFE_LNA_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 8) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_RXFE_LNA_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00000f00)) | ((((UInt32)(val) << 8) << 0) & (0x00000f00)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx1_adc_ldo_refbits ADC LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_ADC_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 12) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_ADC_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x0000f000)) | ((((UInt32)(val) << 12) << 0) & (0x0000f000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx1_bbgm_ldo_refbits Baseband amplifier LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_BBGM_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 16) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_BBGM_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x000f0000)) | ((((UInt32)(val) << 16) << 0) & (0x000f0000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx1_rxfe_ldo_refbits RXFE LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_RXFE_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 20) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_RXFE_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00f00000)) | ((((UInt32)(val) << 20) << 0) & (0x00f00000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0] = halprop_h_tmp; \
        } while (0)

/* radio radio_rx1_rxfe_lna_ldo_refbits RXFE-LNA LDO trim value for RX#. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_RXFE_LNA_LDO_REFBITS(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg32[0]) >> 24) & 0x0F))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_RXFE_LNA_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x0008.reg8[3] = (val); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_BBGM_DC_TUNE */
#define GP_WB_READ_AK_CAL_NVM_RX_BBGM_DC_TUNE(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_BBGM_DC_TUNE(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0] = (val)

/* radio radio_rx0_bbgm_i_dc_tune Adjust I channel baseband amp DC offset */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0]) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE_FROM_RX_BBGM_DC_TUNE(tmp) \
        ((tmp) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX0_BBGM_I_DC_TUNE_TO_RX_BBGM_DC_TUNE(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* radio radio_rx0_bbgm_q_dc_tune Adjust Q channel baseband amp DC offset */
#define GP_WB_READ_AK_CAL_NVM_RX_RX0_BBGM_Q_DC_TUNE(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0]) >> 8) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX0_BBGM_Q_DC_TUNE_FROM_RX_BBGM_DC_TUNE(tmp) \
        (((tmp) >> 8) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX0_BBGM_Q_DC_TUNE(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX0_BBGM_Q_DC_TUNE_TO_RX_BBGM_DC_TUNE(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* radio radio_rx1_bbgm_i_dc_tune Adjust I channel baseband amp DC offset */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_BBGM_I_DC_TUNE(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0]) >> 16) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX1_BBGM_I_DC_TUNE_FROM_RX_BBGM_DC_TUNE(tmp) \
        (((tmp) >> 16) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_BBGM_I_DC_TUNE(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX1_BBGM_I_DC_TUNE_TO_RX_BBGM_DC_TUNE(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* radio radio_rx1_bbgm_q_dc_tune Adjust Q channel baseband amp DC offset */
#define GP_WB_READ_AK_CAL_NVM_RX_RX1_BBGM_Q_DC_TUNE(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg32[0]) >> 24) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX1_BBGM_Q_DC_TUNE_FROM_RX_BBGM_DC_TUNE(tmp) \
        (((tmp) >> 24) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX1_BBGM_Q_DC_TUNE(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bbgm_dc_tune.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX1_BBGM_Q_DC_TUNE_TO_RX_BBGM_DC_TUNE(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_IQCOMP0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_IQCOMP0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_IQCOMP0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 0, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP0_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP0_REAL0_FROM_RX_SIGC0_IQCOMP0(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP0_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP0_REAL0_TO_RX_SIGC0_IQCOMP0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 0, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP0_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP0_IMAG0_FROM_RX_SIGC0_IQCOMP0(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP0_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp0.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP0_IMAG0_TO_RX_SIGC0_IQCOMP0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_IQCOMP1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_IQCOMP1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_IQCOMP1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP1_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP1_REAL0_FROM_RX_SIGC0_IQCOMP1(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP1_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP1_REAL0_TO_RX_SIGC0_IQCOMP1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP1_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP1_IMAG0_FROM_RX_SIGC0_IQCOMP1(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP1_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp1.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP1_IMAG0_TO_RX_SIGC0_IQCOMP1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_IQCOMP2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_IQCOMP2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_IQCOMP2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP2_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP2_REAL0_FROM_RX_SIGC0_IQCOMP2(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP2_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP2_REAL0_TO_RX_SIGC0_IQCOMP2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP2_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP2_IMAG0_FROM_RX_SIGC0_IQCOMP2(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP2_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp2.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP2_IMAG0_TO_RX_SIGC0_IQCOMP2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_IQCOMP3 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_IQCOMP3(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_IQCOMP3(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP3_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP3_REAL0_FROM_RX_SIGC0_IQCOMP3(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP3_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP3_REAL0_TO_RX_SIGC0_IQCOMP3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP3_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP3_IMAG0_FROM_RX_SIGC0_IQCOMP3(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP3_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp3.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP3_IMAG0_TO_RX_SIGC0_IQCOMP3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_IQCOMP4 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_IQCOMP4(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_IQCOMP4(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP4_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP4_REAL0_FROM_RX_SIGC0_IQCOMP4(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP4_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP4_REAL0_TO_RX_SIGC0_IQCOMP4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP4_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP4_IMAG0_FROM_RX_SIGC0_IQCOMP4(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP4_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_iqcomp4.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP4_IMAG0_TO_RX_SIGC0_IQCOMP4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_IQCOMP0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_IQCOMP0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_IQCOMP0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 0, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP0_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP0_REAL1_FROM_RX_SIGC1_IQCOMP0(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP0_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP0_REAL1_TO_RX_SIGC1_IQCOMP0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 0, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP0_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP0_IMAG1_FROM_RX_SIGC1_IQCOMP0(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP0_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp0.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP0_IMAG1_TO_RX_SIGC1_IQCOMP0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_IQCOMP1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_IQCOMP1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_IQCOMP1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP1_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP1_REAL1_FROM_RX_SIGC1_IQCOMP1(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP1_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP1_REAL1_TO_RX_SIGC1_IQCOMP1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP1_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP1_IMAG1_FROM_RX_SIGC1_IQCOMP1(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP1_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp1.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP1_IMAG1_TO_RX_SIGC1_IQCOMP1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_IQCOMP2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_IQCOMP2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_IQCOMP2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP2_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP2_REAL1_FROM_RX_SIGC1_IQCOMP2(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP2_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP2_REAL1_TO_RX_SIGC1_IQCOMP2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP2_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP2_IMAG1_FROM_RX_SIGC1_IQCOMP2(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP2_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp2.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP2_IMAG1_TO_RX_SIGC1_IQCOMP2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_IQCOMP3 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_IQCOMP3(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_IQCOMP3(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP3_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP3_REAL1_FROM_RX_SIGC1_IQCOMP3(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP3_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP3_REAL1_TO_RX_SIGC1_IQCOMP3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP3_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP3_IMAG1_FROM_RX_SIGC1_IQCOMP3(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP3_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp3.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP3_IMAG1_TO_RX_SIGC1_IQCOMP3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_IQCOMP4 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_IQCOMP4(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_IQCOMP4(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg32[0] = (val)

/* IQ-imbalance compensation, real, at BB-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP4_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg32[0]) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP4_REAL1_FROM_RX_SIGC1_IQCOMP4(tmp) \
        (GP_WB_S16(((tmp) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP4_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP4_REAL1_TO_RX_SIGC1_IQCOMP4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x3FFF0000; \
          (tmp) |= (((Int32)(val)) & 0x00003FFF); \
        } while (0)

/* IQ-imbalance compensation, imag, at BB-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_IQCOMP4_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg32[0]) >> 16) & 0x3FFF)), 14))

#define GP_WB_GET_AK_CAL_NVM_RX_IQCOMP4_IMAG1_FROM_RX_SIGC1_IQCOMP4(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x3FFF), 14))

#define GP_WB_WRITE_AK_CAL_NVM_RX_IQCOMP4_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_iqcomp4.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_IQCOMP4_IMAG1_TO_RX_SIGC1_IQCOMP4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -8192, 8191); \
          (tmp) &= 0x00003FFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x3FFF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_BB_GAIN1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_BB_GAIN1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_BB_GAIN1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0] = (val)

/* BB-gain 0, SIGC0, IQ-avg, 0.5dB resolution */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_0(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0]) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_0_FROM_RX_SIGC0_BB_GAIN1(tmp) \
        ((tmp) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_0_TO_RX_SIGC0_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F7F7F00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* BB-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0]) >> 8) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_0_FROM_RX_SIGC0_BB_GAIN1(tmp) \
        (((tmp) >> 8) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_0_TO_RX_SIGC0_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F7F007F; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* BB-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0]) >> 16) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_0_FROM_RX_SIGC0_BB_GAIN1(tmp) \
        (((tmp) >> 16) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_0_TO_RX_SIGC0_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F007F7F; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* BB-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg32[0]) >> 24) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_0_FROM_RX_SIGC0_BB_GAIN1(tmp) \
        (((tmp) >> 24) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_0_TO_RX_SIGC0_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x007F7F7F; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* BB-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN4_0(offset) \
        ((UInt8)(((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bb_gain4_0.reg8[0]))

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN4_0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bb_gain4_0.reg8[0] = (val); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_BB_GAIN1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_BB_GAIN1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_BB_GAIN1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0] = (val)

/* BB-gain 0, SIGC1, IQ-avg, 0.5dB resolution */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_1(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0]) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_1_FROM_RX_SIGC1_BB_GAIN1(tmp) \
        ((tmp) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_1_TO_RX_SIGC1_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F7F7F00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* BB-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0]) >> 8) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_1_FROM_RX_SIGC1_BB_GAIN1(tmp) \
        (((tmp) >> 8) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_1_TO_RX_SIGC1_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F7F007F; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* BB-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0]) >> 16) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_1_FROM_RX_SIGC1_BB_GAIN1(tmp) \
        (((tmp) >> 16) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_1_TO_RX_SIGC1_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x7F007F7F; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* BB-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg32[0]) >> 24) & 0x7F))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_1_FROM_RX_SIGC1_BB_GAIN1(tmp) \
        (((tmp) >> 24) & 0x7F)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_1_TO_RX_SIGC1_BB_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          (tmp) &= 0x007F7F7F; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* BB-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN4_1(offset) \
        ((UInt8)(((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bb_gain4_1.reg8[0]))

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN4_1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x7F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_bb_gain4_1.reg8[0] = (val); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0] = (val)

/* RF-gain 0, SIGC0, IQ-avg, mixer+ATT+LNA, 0.5dB resolution */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN0_0(offset) \
        (GP_WB_S8(((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0]) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN0_0_FROM_RX_SIGC0_RF_GAIN1(tmp) \
        (GP_WB_S8(((tmp) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN0_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg8[0] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN0_0_TO_RX_SIGC0_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= (((Int32)(val)) & 0x000000FF); \
        } while (0)

/* RF-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN1_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0]) >> 8) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN1_0_FROM_RX_SIGC0_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 8) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN1_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg8[1] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN1_0_TO_RX_SIGC0_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= ((((Int32)(val)) << 8) & 0x0000FF00); \
        } while (0)

/* RF-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN2_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0]) >> 16) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN2_0_FROM_RX_SIGC0_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 16) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN2_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg8[2] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN2_0_TO_RX_SIGC0_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x00FF0000); \
        } while (0)

/* RF-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg32[0]) >> 24) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_0_FROM_RX_SIGC0_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 24) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain1.reg8[3] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_0_TO_RX_SIGC0_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= ((((Int32)(val)) << 24) & 0xFF000000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0] = (val)

/* RF-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_0(offset) \
        (GP_WB_S8(((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0]) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_0_FROM_RX_SIGC0_RF_GAIN2(tmp) \
        (GP_WB_S8(((tmp) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg8[0] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_0_TO_RX_SIGC0_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= (((Int32)(val)) & 0x000000FF); \
        } while (0)

/* RF-gain 5, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0]) >> 8) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_0_FROM_RX_SIGC0_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 8) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg8[1] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_0_TO_RX_SIGC0_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= ((((Int32)(val)) << 8) & 0x0000FF00); \
        } while (0)

/* RF-gain 6, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0]) >> 16) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_0_FROM_RX_SIGC0_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 16) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg8[2] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_0_TO_RX_SIGC0_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x00FF0000); \
        } while (0)

/* RF-gain 7, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN7_0(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg32[0]) >> 24) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN7_0_FROM_RX_SIGC0_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 24) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN7_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain2.reg8[3] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN7_0_TO_RX_SIGC0_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= ((((Int32)(val)) << 24) & 0xFF000000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0] = (val)

/* RF-gain 0, SIGC1, IQ-avg, mixer+ATT+LNA, 0.5dB resolution */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN0_1(offset) \
        (GP_WB_S8(((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0]) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN0_1_FROM_RX_SIGC1_RF_GAIN1(tmp) \
        (GP_WB_S8(((tmp) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN0_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg8[0] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN0_1_TO_RX_SIGC1_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= (((Int32)(val)) & 0x000000FF); \
        } while (0)

/* RF-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN1_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0]) >> 8) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN1_1_FROM_RX_SIGC1_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 8) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN1_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg8[1] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN1_1_TO_RX_SIGC1_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= ((((Int32)(val)) << 8) & 0x0000FF00); \
        } while (0)

/* RF-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN2_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0]) >> 16) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN2_1_FROM_RX_SIGC1_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 16) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN2_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg8[2] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN2_1_TO_RX_SIGC1_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x00FF0000); \
        } while (0)

/* RF-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg32[0]) >> 24) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_1_FROM_RX_SIGC1_RF_GAIN1(tmp) \
        (GP_WB_S8((((tmp) >> 24) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain1.reg8[3] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_1_TO_RX_SIGC1_RF_GAIN1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= ((((Int32)(val)) << 24) & 0xFF000000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0] = (val)

/* RF-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_1(offset) \
        (GP_WB_S8(((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0]) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_1_FROM_RX_SIGC1_RF_GAIN2(tmp) \
        (GP_WB_S8(((tmp) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg8[0] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_1_TO_RX_SIGC1_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= (((Int32)(val)) & 0x000000FF); \
        } while (0)

/* RF-gain 5, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0]) >> 8) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_1_FROM_RX_SIGC1_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 8) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg8[1] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_1_TO_RX_SIGC1_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= ((((Int32)(val)) << 8) & 0x0000FF00); \
        } while (0)

/* RF-gain 6, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0]) >> 16) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_1_FROM_RX_SIGC1_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 16) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg8[2] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_1_TO_RX_SIGC1_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x00FF0000); \
        } while (0)

/* RF-gain 7, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN7_1(offset) \
        (GP_WB_S8(((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg32[0]) >> 24) & 0xFF)), 8))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN7_1_FROM_RX_SIGC1_RF_GAIN2(tmp) \
        (GP_WB_S8((((tmp) >> 24) & 0xFF), 8))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN7_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain2.reg8[3] = (UInt8)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN7_1_TO_RX_SIGC1_RF_GAIN2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= ((((Int32)(val)) << 24) & 0xFF000000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0] = (val)

/* lna_bias at RF-gain 0, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0]) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT0(tmp) \
        ((tmp) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_0_TO_RX_SIGC0_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* lna_bias at RF-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0]) >> 8) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 8) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_0_TO_RX_SIGC0_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* lna_bias at RF-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0]) >> 16) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 16) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_0_TO_RX_SIGC0_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* lna_bias at RF-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg32[0]) >> 24) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 24) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut0.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_0_TO_RX_SIGC0_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_AGC_FE_LNA_LUT1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0] = (val)

/* lna_bias at RF-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_0(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0]) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT1(tmp) \
        ((tmp) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_0_TO_RX_SIGC0_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* lna_bias at RF-gain 5, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0]) >> 8) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 8) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_0_TO_RX_SIGC0_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* lna_bias at RF-gain 6, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0]) >> 16) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 16) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_0_TO_RX_SIGC0_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* lna_bias at RF-gain 7, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_0(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg32[0]) >> 24) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_0_FROM_RX_SIGC0_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 24) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_0(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_agc_fe_lna_lut1.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_0_TO_RX_SIGC0_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0] = (val)

/* lna_bias at RF-gain 0, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_1(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0]) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT0(tmp) \
        ((tmp) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS0_1_TO_RX_SIGC1_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* lna_bias at RF-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0]) >> 8) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 8) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS1_1_TO_RX_SIGC1_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* lna_bias at RF-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0]) >> 16) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 16) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS2_1_TO_RX_SIGC1_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* lna_bias at RF-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg32[0]) >> 24) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT0(tmp) \
        (((tmp) >> 24) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut0.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS3_1_TO_RX_SIGC1_AGC_FE_LNA_LUT0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_AGC_FE_LNA_LUT1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0] = (val)

/* lna_bias at RF-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_1(offset) \
        ((UInt8)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0]) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT1(tmp) \
        ((tmp) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg8[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS4_1_TO_RX_SIGC1_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFFFF00; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* lna_bias at RF-gain 5, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0]) >> 8) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 8) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg8[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS5_1_TO_RX_SIGC1_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFFFF00FF; \
          (tmp) |= (((UInt32)(val)) << 8); \
        } while (0)

/* lna_bias at RF-gain 6, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0]) >> 16) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 16) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg8[2] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS6_1_TO_RX_SIGC1_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0xFF00FFFF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)

/* lna_bias at RF-gain 7, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_1(offset) \
        ((UInt8)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg32[0]) >> 24) & 0xFF))

#define GP_WB_GET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_1_FROM_RX_SIGC1_AGC_FE_LNA_LUT1(tmp) \
        (((tmp) >> 24) & 0xFF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_1(offset, val) do { \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_agc_fe_lna_lut1.reg8[3] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RX_RXFE_LNA_BIAS7_1_TO_RX_SIGC1_AGC_FE_LNA_LUT1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          (tmp) &= 0x00FFFFFF; \
          (tmp) |= (((UInt32)(val)) << 24); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 0, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0_FROM_RX_SIGC0_ADC_DC_OFFSET0(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL0_TO_RX_SIGC0_ADC_DC_OFFSET0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 0, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG0_FROM_RX_SIGC0_ADC_DC_OFFSET0(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset0.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG0_TO_RX_SIGC0_ADC_DC_OFFSET0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL0_FROM_RX_SIGC0_ADC_DC_OFFSET1(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL0_TO_RX_SIGC0_ADC_DC_OFFSET1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 1, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG0_FROM_RX_SIGC0_ADC_DC_OFFSET1(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset1.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG0_TO_RX_SIGC0_ADC_DC_OFFSET1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL0_FROM_RX_SIGC0_ADC_DC_OFFSET2(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL0_TO_RX_SIGC0_ADC_DC_OFFSET2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 2, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG0_FROM_RX_SIGC0_ADC_DC_OFFSET2(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset2.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG0_TO_RX_SIGC0_ADC_DC_OFFSET2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET3 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET3(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET3(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL0_FROM_RX_SIGC0_ADC_DC_OFFSET3(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL0_TO_RX_SIGC0_ADC_DC_OFFSET3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 3, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG0_FROM_RX_SIGC0_ADC_DC_OFFSET3(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset3.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG0_TO_RX_SIGC0_ADC_DC_OFFSET3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET4 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET4(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_ADC_DC_OFFSET4(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL0_FROM_RX_SIGC0_ADC_DC_OFFSET4(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL0_TO_RX_SIGC0_ADC_DC_OFFSET4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 4, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG0_FROM_RX_SIGC0_ADC_DC_OFFSET4(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_adc_dc_offset4.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG0_TO_RX_SIGC0_ADC_DC_OFFSET4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET0 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET0(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET0(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 0, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL1_FROM_RX_SIGC1_ADC_DC_OFFSET0(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_REAL1_TO_RX_SIGC1_ADC_DC_OFFSET0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 0, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG1_FROM_RX_SIGC1_ADC_DC_OFFSET0(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset0.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET0_IMAG1_TO_RX_SIGC1_ADC_DC_OFFSET0(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET1 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET1(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET1(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL1_FROM_RX_SIGC1_ADC_DC_OFFSET1(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_REAL1_TO_RX_SIGC1_ADC_DC_OFFSET1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 1, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG1_FROM_RX_SIGC1_ADC_DC_OFFSET1(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset1.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET1_IMAG1_TO_RX_SIGC1_ADC_DC_OFFSET1(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET2 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET2(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET2(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL1_FROM_RX_SIGC1_ADC_DC_OFFSET2(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_REAL1_TO_RX_SIGC1_ADC_DC_OFFSET2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 2, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG1_FROM_RX_SIGC1_ADC_DC_OFFSET2(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset2.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET2_IMAG1_TO_RX_SIGC1_ADC_DC_OFFSET2(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET3 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET3(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET3(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL1_FROM_RX_SIGC1_ADC_DC_OFFSET3(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_REAL1_TO_RX_SIGC1_ADC_DC_OFFSET3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 3, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG1_FROM_RX_SIGC1_ADC_DC_OFFSET3(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset3.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET3_IMAG1_TO_RX_SIGC1_ADC_DC_OFFSET3(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET4 */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET4(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_ADC_DC_OFFSET4(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg32[0] = (val)

/* ADC+BBGM DC-offset, real, at BB-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL1_FROM_RX_SIGC1_ADC_DC_OFFSET4(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_REAL1_TO_RX_SIGC1_ADC_DC_OFFSET4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* ADC+BBGM DC-offset, imag, at BB-gain 4, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG1_FROM_RX_SIGC1_ADC_DC_OFFSET4(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_adc_dc_offset4.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_ADC_DC_OFFSET4_IMAG1_TO_RX_SIGC1_ADC_DC_OFFSET4(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_BB_GAIN0_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_BB_GAIN0_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_BB_GAIN0_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg32[0] = (val)

/* Ratio of BB-gain 0 to BB-gain 4, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0_FROM_RX_SIGC0_BB_GAIN0_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL0_TO_RX_SIGC0_BB_GAIN0_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 0 to BB-gain 4, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG0(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG0_FROM_RX_SIGC0_BB_GAIN0_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain0_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG0_TO_RX_SIGC0_BB_GAIN0_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_BB_GAIN1_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_BB_GAIN1_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_BB_GAIN1_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg32[0] = (val)

/* Ratio of BB-gain 1 to BB-gain 4, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL0(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL0_FROM_RX_SIGC0_BB_GAIN1_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL0_TO_RX_SIGC0_BB_GAIN1_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 1 to BB-gain 4, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG0(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG0_FROM_RX_SIGC0_BB_GAIN1_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain1_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG0_TO_RX_SIGC0_BB_GAIN1_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_BB_GAIN2_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_BB_GAIN2_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_BB_GAIN2_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg32[0] = (val)

/* Ratio of BB-gain 2 to BB-gain 4, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL0(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL0_FROM_RX_SIGC0_BB_GAIN2_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL0_TO_RX_SIGC0_BB_GAIN2_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 2 to BB-gain 4, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG0(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG0_FROM_RX_SIGC0_BB_GAIN2_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain2_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG0_TO_RX_SIGC0_BB_GAIN2_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_BB_GAIN3_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_BB_GAIN3_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_BB_GAIN3_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg32[0] = (val)

/* Ratio of BB-gain 3 to BB-gain 4, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL0(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL0_FROM_RX_SIGC0_BB_GAIN3_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL0_TO_RX_SIGC0_BB_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 3 to BB-gain 4, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG0(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG0_FROM_RX_SIGC0_BB_GAIN3_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_bb_gain3_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG0_TO_RX_SIGC0_BB_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_BB_GAIN0_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_BB_GAIN0_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_BB_GAIN0_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg32[0] = (val)

/* Ratio of BB-gain 0 to BB-gain 4, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL1(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL1_FROM_RX_SIGC1_BB_GAIN0_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_COMP_REAL1_TO_RX_SIGC1_BB_GAIN0_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 0 to BB-gain 4, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG1(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG1_FROM_RX_SIGC1_BB_GAIN0_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain0_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN0_COMP_IMAG1_TO_RX_SIGC1_BB_GAIN0_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_BB_GAIN1_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_BB_GAIN1_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_BB_GAIN1_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg32[0] = (val)

/* Ratio of BB-gain 1 to BB-gain 4, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL1(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL1_FROM_RX_SIGC1_BB_GAIN1_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_COMP_REAL1_TO_RX_SIGC1_BB_GAIN1_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 1 to BB-gain 4, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG1(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG1_FROM_RX_SIGC1_BB_GAIN1_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain1_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN1_COMP_IMAG1_TO_RX_SIGC1_BB_GAIN1_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_BB_GAIN2_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_BB_GAIN2_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_BB_GAIN2_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg32[0] = (val)

/* Ratio of BB-gain 2 to BB-gain 4, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL1(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL1_FROM_RX_SIGC1_BB_GAIN2_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_COMP_REAL1_TO_RX_SIGC1_BB_GAIN2_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 2 to BB-gain 4, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG1(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG1_FROM_RX_SIGC1_BB_GAIN2_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain2_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN2_COMP_IMAG1_TO_RX_SIGC1_BB_GAIN2_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_BB_GAIN3_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_BB_GAIN3_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_BB_GAIN3_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg32[0] = (val)

/* Ratio of BB-gain 3 to BB-gain 4, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL1(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg32[0]) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL1_FROM_RX_SIGC1_BB_GAIN3_COMP(tmp) \
        ((tmp) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg16[0] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_COMP_REAL1_TO_RX_SIGC1_BB_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x03FF0000; \
          (tmp) |= ((UInt32)(val)); \
        } while (0)

/* Ratio of BB-gain 3 to BB-gain 4, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG1(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg32[0]) >> 16) & 0x03FF))

#define GP_WB_GET_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG1_FROM_RX_SIGC1_BB_GAIN3_COMP(tmp) \
        (((tmp) >> 16) & 0x03FF)

#define GP_WB_WRITE_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_bb_gain3_comp.reg16[1] = (val); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_BB_GAIN3_COMP_IMAG1_TO_RX_SIGC1_BB_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK(val, 0x03FF); \
          (tmp) &= 0x000003FF; \
          (tmp) |= (((UInt32)(val)) << 16); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN3_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN3_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN3_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 0,1,2,3, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0_FROM_RX_SIGC0_RF_GAIN3_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL0_TO_RX_SIGC0_RF_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 0,1,2,3, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG0_FROM_RX_SIGC0_RF_GAIN3_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain3_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG0_TO_RX_SIGC0_RF_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN4_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN4_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN4_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 4, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL0_FROM_RX_SIGC0_RF_GAIN4_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL0_TO_RX_SIGC0_RF_GAIN4_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 4, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG0_FROM_RX_SIGC0_RF_GAIN4_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain4_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG0_TO_RX_SIGC0_RF_GAIN4_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN5_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN5_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN5_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 5, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL0_FROM_RX_SIGC0_RF_GAIN5_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL0_TO_RX_SIGC0_RF_GAIN5_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 5, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG0_FROM_RX_SIGC0_RF_GAIN5_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain5_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG0_TO_RX_SIGC0_RF_GAIN5_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC0_RF_GAIN6_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC0_RF_GAIN6_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC0_RF_GAIN6_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 6, real, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL0(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL0_FROM_RX_SIGC0_RF_GAIN6_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL0_TO_RX_SIGC0_RF_GAIN6_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 6, imag, SIGC0 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG0(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG0_FROM_RX_SIGC0_RF_GAIN6_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG0(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc0_rf_gain6_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG0_TO_RX_SIGC0_RF_GAIN6_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN3_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN3_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN3_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 0,1,2,3, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL1_FROM_RX_SIGC1_RF_GAIN3_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_COMP_REAL1_TO_RX_SIGC1_RF_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 0,1,2,3, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG1_FROM_RX_SIGC1_RF_GAIN3_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain3_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN3_COMP_IMAG1_TO_RX_SIGC1_RF_GAIN3_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN4_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN4_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN4_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 4, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL1_FROM_RX_SIGC1_RF_GAIN4_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_COMP_REAL1_TO_RX_SIGC1_RF_GAIN4_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 4, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG1_FROM_RX_SIGC1_RF_GAIN4_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain4_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN4_COMP_IMAG1_TO_RX_SIGC1_RF_GAIN4_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN5_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN5_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN5_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 5, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL1_FROM_RX_SIGC1_RF_GAIN5_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_COMP_REAL1_TO_RX_SIGC1_RF_GAIN5_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 5, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG1_FROM_RX_SIGC1_RF_GAIN5_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain5_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN5_COMP_IMAG1_TO_RX_SIGC1_RF_GAIN5_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* register access functions for AK_CAL_NVM_RX_SIGC1_RF_GAIN6_COMP */
#define GP_WB_READ_AK_CAL_NVM_RX_SIGC1_RF_GAIN6_COMP(offset) \
        ((UInt32)((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg32[0])

#define GP_WB_WRITE_AK_CAL_NVM_RX_SIGC1_RF_GAIN6_COMP(offset, val) \
      ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg32[0] = (val)

/* DC offset compensation for RF-gain 6, real, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL1(offset) \
        (GP_WB_S16(((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg32[0]) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL1_FROM_RX_SIGC1_RF_GAIN6_COMP(tmp) \
        (GP_WB_S16(((tmp) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg16[0] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_COMP_REAL1_TO_RX_SIGC1_RF_GAIN6_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x07FF0000; \
          (tmp) |= (((Int32)(val)) & 0x000007FF); \
        } while (0)

/* DC offset compensation for RF-gain 6, imag, SIGC1 */
#define GP_WB_READ_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG1(offset) \
        (GP_WB_S16(((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg32[0]) >> 16) & 0x07FF)), 11))

#define GP_WB_GET_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG1_FROM_RX_SIGC1_RF_GAIN6_COMP(tmp) \
        (GP_WB_S16((((tmp) >> 16) & 0x07FF), 11))

#define GP_WB_WRITE_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG1(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->rx_sigc1_rf_gain6_comp.reg16[1] = (UInt16)((val)); \
        } while (0)

#define GP_WB_SET_AK_CAL_NVM_RX_RF_GAIN6_COMP_IMAG1_TO_RX_SIGC1_RF_GAIN6_COMP(tmp, val) do { \
          GP_WB_SIZE_CHECK_U32(tmp); \
          GP_WB_RANGE_CHECK_SIGNED(val, -1024, 1023); \
          (tmp) &= 0x000007FF; \
          (tmp) |= ((((Int32)(val)) << 16) & 0x07FF0000); \
        } while (0)


/* DTC pull-down resistor calibration, will be used to calibrate total delay to 416ps within +/- 1ps, Software will take care */
#define GP_WB_READ_AK_CAL_NVM_SX_DTC_RES_CAL(offset) \
        ((UInt32)((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) & 0x000001FF))

#define GP_WB_WRITE_AK_CAL_NVM_SX_DTC_RES_CAL(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x000001FF); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x000001ff)) | ((((val)) << 0) & (0x000001ff)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* DTC bias current for the vdd_dtc generator, programmable, will be used to calibrate remaining total delay of 416ps, software will update this when needed */
#define GP_WB_READ_AK_CAL_NVM_SX_DTC_BIAS_CAL(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 9) & 0x0000003F))

#define GP_WB_WRITE_AK_CAL_NVM_SX_DTC_BIAS_CAL(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000003F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00007e00)) | ((((UInt32)(val) << 9) << 0) & (0x00007e00)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* Sx (dco) LDO trim value. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_SX_DCO_LDO_REFBITS(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 15) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_SX_DCO_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00078000)) | ((((UInt32)(val) << 15) << 0) & (0x00078000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* DCO bias resistor calibration, value determined during BIST SW calibration */
#define GP_WB_READ_AK_CAL_NVM_SX_DCO_RBIAS_SEL(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 19) & 0x0000000F))

#define GP_WB_WRITE_AK_CAL_NVM_SX_DCO_RBIAS_SEL(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00780000)) | ((((UInt32)(val) << 19) << 0) & (0x00780000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* TX LO duty cycle adjust (DCA) n-side - MSB = adj. direction, 2 LSBs: steps for 0-5% duty cycle */
#define GP_WB_READ_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_N(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 23) & 0x00000007))

#define GP_WB_WRITE_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_N(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x00000007); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x03800000)) | ((((UInt32)(val) << 23) << 0) & (0x03800000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* TX LO duty cycle adjust (DCA) p-side - MSB = adj. direction, 2 LSBs: steps for 0-5% duty cycle */
#define GP_WB_READ_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_P(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 26) & 0x00000007))

#define GP_WB_WRITE_AK_CAL_NVM_SX_LO_TX_DUTYCYCLE_P(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x00000007); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x1c000000)) | ((((UInt32)(val) << 26) << 0) & (0x1c000000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)

/* TX LO phase rotator adjustment (~25deg/LSB @ 5GHz) */
#define GP_WB_READ_AK_CAL_NVM_SX_LO_TX_PHASE(offset) \
        ((UInt32)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]) >> 29) & 0x00000007))

#define GP_WB_WRITE_AK_CAL_NVM_SX_LO_TX_PHASE(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x00000007); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt32 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0xe0000000)) | ((((UInt32)(val) << 29) << 0) & (0xe0000000)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d0.reg32[0] = halprop_h_tmp; \
        } while (0)


/* divider by 2 (and pharot/dca) LDO trim value. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_SX_DIV2_LDO_REFBITS(offset) \
        ((UInt16)((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0]) & 0x000F))

#define GP_WB_WRITE_AK_CAL_NVM_SX_DIV2_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt16 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x000F)) | (((val)) & (0x000F)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0] = halprop_h_tmp; \
        } while (0)

/* RX-ADC references for both RX0 and RX1, Used to be SX LOGEN reference, but is not used anymore. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_SX_LOGEN_LDO_REFBITS(offset) \
        ((UInt16)(((((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0]) >> 4) & 0x000F))

#define GP_WB_WRITE_AK_CAL_NVM_SX_LOGEN_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          UInt16 halprop_h_tmp = (((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0]); \
          halprop_h_tmp = (halprop_h_tmp & ~(0x00F0)) | (((UInt16)(val) << 4) & (0x00F0)); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg16[0] = halprop_h_tmp; \
        } while (0)

/* Sx sampler LDO trim value. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_SX_SAMPLER_LDO_REFBITS(offset) \
        ((UInt8)(((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg8[1]))

#define GP_WB_WRITE_AK_CAL_NVM_SX_SAMPLER_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          ((AK_CAL_NVM_REGMAP_T *) (offset))->unnamed_reg_ak_cal_nvm_0x00d4.reg8[1] = (val); \
        } while (0)


/* Radio macro 1uA trim value, default = 19 (decimal), with approx 4% adjustment of current around center */
#define GP_WB_READ_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS(offset) \
        (GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_ADDRESS) & 0x0000001F)

#define GP_WB_WRITE_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000001F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_IREF0_REFBITS_ADDRESS, 0x0000001F, (val)); \
        } while (0)

/* Radio macro 1uA trim value, default = 19 (decimal), with approx 4% adjustment of current around center */
#define GP_WB_READ_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS(offset) \
        ((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS_ADDRESS) >> 5) & 0x0000001F)

#define GP_WB_WRITE_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000001F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_IREF1_REFBITS_ADDRESS, 0x000003E0, (UInt32)(val) << 5); \
        } while (0)

/* Radio macro reference resistor calibration.Valid for RX0/RX1/TX/RADIO: Default = 0, Step is 2.5mV, Range is -40mV to + 37.5mV */
#define GP_WB_READ_AK_CAL_NVM_TRX_MACRO_RES_CAL(offset) \
        (GP_WB_S32(((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_RES_CAL_ADDRESS) >> 10) & 0x0000001F), 5))

#define GP_WB_WRITE_AK_CAL_NVM_TRX_MACRO_RES_CAL(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -16, 15); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TRX_MACRO_RES_CAL_ADDRESS, 0x00007C00, (UInt32)((UInt32)(val) << 10)); \
        } while (0)

/* Bias generator output current-voltage programming */
#define GP_WB_READ_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR(offset) \
        (GP_WB_S32(((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR_ADDRESS) >> 15) & 0x0000000F), 4))

#define GP_WB_WRITE_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -8, 7); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TRX_RFIO_BIASGEN_CUR_ADDRESS, 0x00078000, (UInt32)((UInt32)(val) << 15)); \
        } while (0)


/* Tx LDO vref calibration word, both for 10dBm/20dBm PA */
#define GP_WB_READ_AK_CAL_NVM_TX_RES_CAL(offset) \
        (GP_WB_S32((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TX_RES_CAL_ADDRESS) & 0x0000001F), 5))

#define GP_WB_WRITE_AK_CAL_NVM_TX_RES_CAL(offset, val) do { \
          GP_WB_RANGE_CHECK_SIGNED(val, -16, 15); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TX_RES_CAL_ADDRESS, 0x0000001F, (UInt32)((val))); \
        } while (0)

/* Block LDO vref tune word. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_TX_LDO_REFBITS(offset) \
        ((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TX_LDO_REFBITS_ADDRESS) >> 5) & 0x0000000F)

#define GP_WB_WRITE_AK_CAL_NVM_TX_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TX_LDO_REFBITS_ADDRESS, 0x000001E0, (UInt32)(val) << 5); \
        } while (0)

/* PA LDO vref tune word. Trim value: default = 10 (decimal) / 1010 (binary): Step = 10mV, Range is 700mV + value(dec)*10mV, max 850mV. */
#define GP_WB_READ_AK_CAL_NVM_TX_PA_LDO_REFBITS(offset) \
        ((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TX_PA_LDO_REFBITS_ADDRESS) >> 9) & 0x0000000F)

#define GP_WB_WRITE_AK_CAL_NVM_TX_PA_LDO_REFBITS(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TX_PA_LDO_REFBITS_ADDRESS, 0x00001E00, (UInt32)(val) << 9); \
        } while (0)

/* Tx Spares registers. */
#define GP_WB_READ_AK_CAL_NVM_TX_SPARE(offset) \
        ((GP_WB_READ_U32((offset) + GP_WB_AK_CAL_NVM_TX_SPARE_ADDRESS) >> 13) & 0x0000000F)

#define GP_WB_WRITE_AK_CAL_NVM_TX_SPARE(offset, val) do { \
          GP_WB_RANGE_CHECK(val, 0x0000000F); \
          GP_WB_CHECK_OFFSET_AK_CAL_NVM(offset); \
          GP_WB_MWRITE_U32((offset) + GP_WB_AK_CAL_NVM_TX_SPARE_ADDRESS, 0x0001E000, (UInt32)(val) << 13); \
        } while (0)

#endif //GP_WB_RS_REGMAP_PROP_H
