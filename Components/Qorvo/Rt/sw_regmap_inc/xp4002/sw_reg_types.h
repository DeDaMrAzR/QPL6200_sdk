
#ifndef GP_WB_RAM_TYPES_H
#define GP_WB_RAM_TYPES_H

/***************************
 * regmap: macfilt
 ***************************/
struct PACKED_PRE gp_macfilt_regmap
{
    union PACKED_PRE {
        volatile UInt8 channel_idx; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator; // Set to 2 if device if pan coordinator for primary pan. Set to 1 if not
        volatile UInt8 p_pan_coordinator[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address; // Short address for the device within its primary PAN
        volatile UInt8 p_short_address[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id; // primary pan id the device operates in
        volatile UInt8 p_pan_id[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address; // 64 bit mac address for the device
        volatile UInt8 p_extended_address[64/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        volatile UInt8 channel_idx_alt_a; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx_alt_a[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator_alt_a; // Set to 2 if device if pan coordinator for secondary pan. Set to 1 if not
        volatile UInt8 p_pan_coordinator_alt_a[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address_alt_a; // Short address for the device within its secondary PAN
        volatile UInt8 p_short_address_alt_a[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id_alt_a; // secondary pan id the device operates in
        volatile UInt8 p_pan_id_alt_a[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address_alt_a; // 64 bit secondary mac address for the device
        volatile UInt8 p_extended_address_alt_a[64/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt8 channel_idx_alt_b; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx_alt_b[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator_alt_b; // Set to 2 if device if pan coordinator for tertiary pan.. Set to 1 if not
        volatile UInt8 p_pan_coordinator_alt_b[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address_alt_b; // Short address for the device within third PAN
        volatile UInt8 p_short_address_alt_b[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id_alt_b; // third pan id the device operates in
        volatile UInt8 p_pan_id_alt_b[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address_alt_b; // 64 bit tertiary mac address for the device
        volatile UInt8 p_extended_address_alt_b[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 dst_pan_id_table_size; // size of the destination address PAN ID table (in nr of entries, 1-3). Can be overruled by license
        volatile UInt8 p_dst_pan_id_table_size[8/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_3 : 2;
            volatile UInt8 ack_power_h : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_4 : 2;
            volatile UInt8 ack_power_rssi_thr; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan;
        volatile UInt8 p_ack_power_cfg_pan[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_alt_a : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_5 : 2;
            volatile UInt8 ack_power_h_alt_a : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_6 : 2;
            volatile UInt8 ack_power_rssi_thr_alt_a; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing_alt_a; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan_alt_a;
        volatile UInt8 p_ack_power_cfg_pan_alt_a[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_alt_b : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_7 : 2;
            volatile UInt8 ack_power_h_alt_b : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_8 : 2;
            volatile UInt8 ack_power_rssi_thr_alt_b; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing_alt_b; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan_alt_b;
        volatile UInt8 p_ack_power_cfg_pan_alt_b[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_default : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does not belong to a pan/short/ch_idx combo
            UInt8 padding_9 : 2;
            volatile UInt8 ack_power_h_default : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does not belong to a pan/short/ch_idx combo
            UInt8 padding_10 : 2;
            volatile UInt8 ack_power_rssi_thr_default; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does not belong to a pan/short/ch_idx combo
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_default : 24;
        volatile UInt8 p_ack_power_cfg_default[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ext_addr_table_size; // Nr of  ext address table entries
        volatile UInt8 p_ext_addr_table_size[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 short_src_address_table_base_address; // short source address table base address ( = offset added to SM_linear start address). Resolution 1 byte - Caution must be 16 bit alligned!
            volatile UInt8 short_src_address_table_size; // Indicates the nr of short src address table entries
        } PACKED_POST;
        volatile UInt32 short_src_address_table_config : 24;
        volatile UInt8 p_short_src_address_table_config[24/8];
    } PACKED_POST;
    UInt8 padding_11[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 long_src_address_table_base_address; // long source address table base address ( = offset added to SM_linear start address). Resolution 1 byte - Caution must be 16 bit alligned!
            volatile UInt8 long_src_address_table_size; // Indicates the nr of long src address table entries
        } PACKED_POST;
        volatile UInt32 long_src_address_table_config : 24;
        volatile UInt8 p_long_src_address_table_config[24/8];
    } PACKED_POST;
    UInt8 padding_12[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool fp_value_when_no_match_found : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool fp_value_when_no_match_found_alt_a : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool fp_value_when_no_match_found_alt_b : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool enable_sniffing : 1; // When enabled, all packets will be queued even if the macfilter would normally drop the frame. The pbm will indicate in the drop_reason field if the frame was dropped or not
            volatile Bool disable : 1; // When 1 the macfilter will be disabled and will accept every packet that comes in (needed for BLE)
            volatile UInt8 every_frame_is_datareq : 3; // When enabled, the src address search will be done for every frame. Can be set per PAN ID. Index 0 corresponds to pan_id, index 1 -> pan_id_alt_a, index 2 -> pan_id_alt_b.
            volatile Bool every_frame_is_datareq_default : 1; // When enabled, the src address search will be done for every frame. Default setting, used when a frame cannot be matched with one of the PAN IDs, e.g. broadcast PAN ID.
            volatile Bool dont_ack_if_not_in_src_list : 1; // When enabled, no ack will be sent if the src address is not in the src list
            volatile Bool drop_if_not_in_src_list : 1; // When enabled, frames will be dropped when the src address is not in the src list
            volatile Bool claim_radio_on_every_rx : 1; // when enabled the macfilter will claim the radio for zigbee as soon as the header is received. When disabled, the radio is only claimed for packets on which an ACK respose is to be sent.
            volatile Bool enh_ack_insert_csl_ie : 1; // when enabled, the CSL IE will be inserted into the Enhanced-Ack
            volatile Bool enable_2015_support : 1; // When enabled, the macfilter will support 2015 spec frames. Not all features are implemented yet.
            volatile Bool disable_crc_check : 1; // 
        } PACKED_POST;
        volatile UInt16 generic_control;
        volatile UInt8 p_generic_control[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cmd_type; // Cmd type that can be filtered on
        volatile UInt8 p_cmd_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_bcn : 1; // When 1, frames of type beacon will be accepted by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_data : 1; // When 1, frames of type data will be accepted by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_ack : 1; // When 1, frames of type ack will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_cmd : 1; // When 1, frames of type beacon will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_rsv_4 : 1; // When 1, frames of reserved frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_mp : 1; // When 1, frames of Multipurpose frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_fra : 1; // When 1, frames of fragment or frak frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_ext : 1; // When 1, frames of extended frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
        } PACKED_POST;
        volatile UInt8 frame_type_accept;
        volatile UInt8 p_frame_type_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_fv_2003 : 1; // When 1, frames of frames using the 2003 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_2006 : 1; // When 1, frames of frames using the 2006 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_2015 : 1; // When 1, frames of frames using the 2010 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_rsv_3 : 1; // When 1, frames of frames using the unknown mac version will be accepted and acked if needed and other filters also match
        } PACKED_POST;
        volatile UInt8 frame_version_accept;
        volatile UInt8 p_frame_version_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_bcn_src_addr_mode_00 : 1; // When 1, beacon frames with src address mode set to 0 will be accepted. Default is 0, used to filter out all zero packets
            volatile Bool accept_bcast_daddr : 1; // When 1 (=default), broadcast destination addressing will be accepted. Use this setting to filter out broadcast packets
            volatile Bool accept_bcast_pan_id : 1; // When 1 (=default), broadcast pan id-s will be accepted. Use this setting to filter out packets using broadcast
        } PACKED_POST;
        volatile UInt8 frame_addressing_accept;
        volatile UInt8 p_frame_addressing_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool src_pan_id_beacon_check_on : 1; // When 1, the src pan id is checked for beacon frames, and the frame is accepted when the pan id matches
            volatile Bool src_pan_id_data_command_check_on : 1; // When 1, the src pan id is checked for data/cmd frames, and the frame is accepted when the pan id matches
            volatile Bool dst_pan_id_check_on : 1; // When 1, the destination pan id is checked and the frame is accepted when the pan id matches
            volatile Bool dst_addr_check_on : 1; // When 1, the src addr is checked and the frame is accepted when the src address matches the address set by short_addr (or extended addresss)
            volatile Bool cmd_type_check_on : 1; // When 1, the cmd type is checked and the frame is accepted when the command type matches
        } PACKED_POST;
        volatile UInt8 frame_checks;
        volatile UInt8 p_frame_checks[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_bcn_to_queue : 1; // When 1, beacon frames will be passed to the queue
            volatile Bool ft_data_to_queue : 1; // When 1, data frames will be passed to the queue
            volatile Bool ft_ack_to_queue : 1; // When 1, ack frames will be passed to the queue
            volatile Bool ft_cmd_to_queue : 1; // When 1, cmd frames will be passed to the queue
            volatile Bool ft_rsv_4_to_queue : 1; // When 1, reserved frames will be passed to the queue
            volatile Bool ft_mp_to_queue : 1; // When 1, Multipurpose frames will be passed to the queue
            volatile Bool ft_fra_to_queue : 1; // When 1, fragment or frak frames will be passed to the queue
            volatile Bool ft_ext_to_queue : 1; // When 1, extended frames will be passed to the queue
        } PACKED_POST;
        volatile UInt8 frame_to_queue_enables;
        volatile UInt8 p_frame_to_queue_enables[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ack_request_enable_ch_idx_0 : 1; // enable ack request for channel index 0
            volatile Bool ack_request_enable_ch_idx_1 : 1; // enable ack request for channel index 1
            volatile Bool ack_request_enable_ch_idx_2 : 1; // enable ack request for channel index 2
            volatile Bool ack_request_enable_ch_idx_3 : 1; // enable ack request for channel index 3
            volatile Bool ack_request_enable_ch_idx_4 : 1; // enable ack request for channel index 4
            volatile Bool ack_request_enable_ch_idx_5 : 1; // enable ack request for channel index 5
        } PACKED_POST;
        volatile UInt8 ack_request_enables;
        volatile UInt8 p_ack_request_enables[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_seq_number; // Sequence number place holder register for the sequence number of ongoing transmission
        volatile UInt8 p_tx_seq_number[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 broadcast_mask; // placeholder to put a broadcast mask (so 0xFFFF) - needs to be inited
        volatile UInt8 p_broadcast_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 debug; // 
        volatile UInt8 p_debug[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 state; // 
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 drop_reason : 5; // Reason why frame was dropped
            volatile Bool drop_reason_valid : 1; // 
        } PACKED_POST;
        volatile UInt8 drop_status;
        volatile UInt8 p_drop_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 csl_ev_nr; // number of ES event that shows the timing of the CSL wake ups (used to insert timing into enh-ack)
        volatile UInt8 p_csl_ev_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 ack_key_ptr; // pointer to the ACK key pointer
        volatile UInt8 p_ack_key_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 ack_nonce_ptr; // pointer to the ACK nonce
        volatile UInt8 p_ack_nonce_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool crc_ok : 1; // 
            volatile Bool is_matching_ack : 1; // 
            volatile Bool frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 frame_status;
        volatile UInt8 p_frame_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool suc_exp_crc_ok : 1; // 
            volatile Bool suc_exp_is_matching_ack : 1; // 
            volatile Bool suc_exp_frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 success_exp_flags;
        volatile UInt8 p_success_exp_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool suc_msk_crc_ok : 1; // 
            volatile Bool suc_msk_is_matching_ack : 1; // 
            volatile Bool suc_msk_frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 success_mask_flags;
        volatile UInt8 p_success_mask_flags[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_macfilt_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_macfilt_regmap)==99));
}

/***************************
 * regmap: ble_mgr
 ***************************/
struct PACKED_PRE gp_ble_mgr_regmap
{
    union PACKED_PRE {
        volatile UInt64 device_address : 48; // The BLE MAC device address of the chip
        volatile UInt8 p_device_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 state; // 
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool abort_current_event : 1; // 
            volatile Bool rx_scan_wd_busy : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_ble_mgr_0x0007;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0007[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool cal_started_int : 1; // 
            volatile Bool cal_done_int : 1; // 
            volatile Bool tx_started_int : 1; // 
            volatile Bool tx_done_int : 1; // 
            volatile Bool rx_wd_started_int : 1; // 
            volatile Bool rx_wd_done_int : 1; // 
        } PACKED_POST;
        volatile UInt8 int_cfg;
        volatile UInt8 p_int_cfg[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 max_allowed_pdu_payload_length; // The maximum allowed length field for BLE PDUs. In case we receive a longer packet, we need to drop it.
        volatile UInt8 p_max_allowed_pdu_payload_length[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 cis_admin_config_ptr; // Location to `cis_admin_config` instance, used for tmp scratchpad during execution of a CIG.
        volatile UInt8 p_cis_admin_config_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rx_preamble_thresh; // Threshold for BLE preamble detect
        volatile UInt8 p_rx_preamble_thresh[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 own_clock_accuracy_ppm; // The accuracy of the current clock source that is used (in ppm)
        volatile UInt8 p_own_clock_accuracy_ppm[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 response_wd_offset; // Additional offset for opening the rx window after a tx. Resolution 1 us
            volatile UInt8 response_wd_extra_duration; // Additional duration of the response listen window. Resolution 1 us
        } PACKED_POST;
        volatile UInt16 unnamed_reg_ble_mgr_0x0010;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0010[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 total_jitter_us; // The total jitter (in us) RT needs to take into account when scheduling relative RX windows
        volatile UInt8 p_total_jitter_us[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool disable_rx_whitening : 1; // 
            volatile Bool disable_tx_whitening : 1; // 
            volatile Bool claim_radio_on_cleanup : 1; // 
            volatile Bool shutdown_during_cleanup : 1; // 
            volatile Bool ms_enabled : 1; // flag to enable multi-standard listening, if 1 events using the ADV access address and that have the ms_allowed flag to 1, can listen in multi-standard
            volatile Bool ad_enabled : 1; // flag to enable antenna diversity
            volatile Bool hw_ad_enabled : 1; // flag to enable hardware(fast) BLE RX antenna diversity
        } PACKED_POST;
        volatile UInt8 unnamed_reg_ble_mgr_0x0013;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0013[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 empty_tx_ptr; // Pointer (offset from RAM_LINEAR_START) to the data packet format_t that can be used by RT for all transmissions
        volatile UInt8 p_empty_tx_ptr[16/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 preamble_len; // The number of preambles the RT system should take into account for 1 Mbit frames
            volatile UInt8 hdr_preamble_len; // The number of preambles the RT system should take into account for 2 Mbit frames
            volatile UInt8 lr_preamble_len; // The number of preambles the RT system should take into account for long range frames
        } PACKED_POST;
        volatile UInt32 preamble_lengths : 24;
        volatile UInt8 p_preamble_lengths[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 t_ifs; // Tmp debug setting to override T_IFS. Resolution 1 us
        volatile UInt8 p_t_ifs[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 preamble_symbol_aa_odd; // The preamble symbol for the 1M/2M PHY where the lsb of the access address is odd
            volatile UInt8 preamble_symbol_aa_even; // The preamble symbol for the 1M/2M PHY where the lsb of the access address is even (e.g. the advertising access address)
            volatile UInt8 preamble_symbol_lr; // The preamble symbol for the LE coded PHY
        } PACKED_POST;
        volatile UInt32 preamble_symbols : 24;
        volatile UInt8 p_preamble_symbols[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 bgscan_ev_nr; // special RT event number used for background scanning
        volatile UInt8 p_bgscan_ev_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 adv_access_address; // Access address for this activity
        volatile UInt8 p_adv_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 adv_validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 adv_validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_1 : 3;
            volatile UInt8 adv_fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_2 : 2;
            volatile Bool adv_fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool adv_validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 adv_validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool adv_validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool adv_validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 adv_validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool adv_validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool adv_validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 adv_validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool adv_validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool adv_validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 adv_crc_init : 24; // The crc init for the advertising channels
        volatile UInt8 p_adv_crc_init[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ble_channel_idx; // The channel index / stack index to use for all BLE events
        volatile UInt8 p_ble_channel_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 debug_log_ptr; // pointer to a an area of 256 bytes where debug loging has to be written out. if 0 no logging will be written out
        volatile UInt8 p_debug_log_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 aod_tx_ant_sw_delay; // gets copied over to tx.aoa_aod_tx_ant_sw_delay for BLE 1Mb TX mode
            volatile UInt8 hdr_aod_tx_ant_sw_delay; // gets copied over to tx.aoa_aod_tx_ant_sw_delay for BLE 2Mb TX mode
        } PACKED_POST;
        volatile UInt16 unnamed_reg_ble_mgr_0x0032;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0032[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 res_pr_local_irk_list_base_address : 24; // resolvable private address table compressed address (16 byte aligned). Contains IRKs.
        volatile UInt8 p_res_pr_local_irk_list_base_address[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pdl_head_idx; // first (in time) PDL to be processed next, i.e. the first PDL from the linked list
        volatile UInt8 p_pdl_head_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 res_pr_prand : 24; // Random part of the to-be generated RPA address
        volatile UInt8 p_res_pr_prand[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 dma_channel_cte_samples : 3; // DMA channel allocated by NRT, to be used exclusively by RT for receiving CTE samples
            volatile UInt8 dma_channel_cte_antennas : 3; // DMA channel allocated by NRT, to be used exclusively by RT for configuring the antenna sequence pattern
        } PACKED_POST;
        volatile UInt8 dma_channels;
        volatile UInt8 p_dma_channels[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ch_conv_ble_channel_0; // 
            volatile UInt8 ch_conv_ble_channel_1; // 
            volatile UInt8 ch_conv_ble_channel_2; // 
            volatile UInt8 ch_conv_ble_channel_3; // 
            volatile UInt8 ch_conv_ble_channel_4; // 
            volatile UInt8 ch_conv_ble_channel_5; // 
            volatile UInt8 ch_conv_ble_channel_6; // 
            volatile UInt8 ch_conv_ble_channel_7; // 
        } PACKED_POST;
        volatile UInt64 ch_conversion_map;
        volatile UInt8 p_ch_conversion_map[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ch_conv_ble_channel_8; // 
            volatile UInt8 ch_conv_ble_channel_9; // 
            volatile UInt8 ch_conv_ble_channel_10; // 
            volatile UInt8 ch_conv_ble_channel_11; // 
            volatile UInt8 ch_conv_ble_channel_12; // 
            volatile UInt8 ch_conv_ble_channel_13; // 
            volatile UInt8 ch_conv_ble_channel_14; // 
            volatile UInt8 ch_conv_ble_channel_15; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ble_mgr_0x0044;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0044[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ch_conv_ble_channel_16; // 
            volatile UInt8 ch_conv_ble_channel_17; // 
            volatile UInt8 ch_conv_ble_channel_18; // 
            volatile UInt8 ch_conv_ble_channel_19; // 
            volatile UInt8 ch_conv_ble_channel_20; // 
            volatile UInt8 ch_conv_ble_channel_21; // 
            volatile UInt8 ch_conv_ble_channel_22; // 
            volatile UInt8 ch_conv_ble_channel_23; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ble_mgr_0x004c;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x004c[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ch_conv_ble_channel_24; // 
            volatile UInt8 ch_conv_ble_channel_25; // 
            volatile UInt8 ch_conv_ble_channel_26; // 
            volatile UInt8 ch_conv_ble_channel_27; // 
            volatile UInt8 ch_conv_ble_channel_28; // 
            volatile UInt8 ch_conv_ble_channel_29; // 
            volatile UInt8 ch_conv_ble_channel_30; // 
            volatile UInt8 ch_conv_ble_channel_31; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ble_mgr_0x0054;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x0054[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ch_conv_ble_channel_32; // 
            volatile UInt8 ch_conv_ble_channel_33; // 
            volatile UInt8 ch_conv_ble_channel_34; // 
            volatile UInt8 ch_conv_ble_channel_35; // 
            volatile UInt8 ch_conv_ble_channel_36; // 
            volatile UInt8 ch_conv_ble_channel_37; // 
            volatile UInt8 ch_conv_ble_channel_38; // 
            volatile UInt8 ch_conv_ble_channel_39; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ble_mgr_0x005c;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x005c[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pdl_first_free_idx; // the first available PDL entry: top of the stack of available PDL entries
        volatile UInt8 p_pdl_first_free_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 active_subev; // 
        volatile UInt8 p_active_subev[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 subev_ev_nr; // number of the RT event that handles the subevents
        volatile UInt8 p_subev_ev_nr[8/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        volatile UInt16 pdl_base_ptr; // offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_pdl_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 subev_dsc_base_ptr; // offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_subev_dsc_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 subev_completed_base_ptr; // offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_subev_completed_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 subev_dsc_entry_rt_claimed; // mask that shows the claimed subev_dsc indexes.
        volatile UInt8 p_subev_dsc_entry_rt_claimed[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 nr_rt_subev_dscs; // the maximum number of subev_dsc available for the RT
        volatile UInt8 p_nr_rt_subev_dscs[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 nr_tot_subev_dscs; // the maximum number of subev_dsc in total
        volatile UInt8 p_nr_tot_subev_dscs[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 subev_completed_wr_ptr; // 
            volatile UInt8 subev_completed_rd_ptr; // 
        } PACKED_POST;
        volatile UInt16 subev_completed_ptrs;
        volatile UInt8 p_subev_completed_ptrs[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 adv_asc_list_base_ptr; // offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_adv_asc_list_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 scan_asc_list_base_ptr; // offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_scan_asc_list_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 init_ext_info_base_ptr; // linear ram offset for the init_ext_info structure
        volatile UInt8 p_init_ext_info_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 nr_scan_asc; // the maximum number of scan_asc
        volatile UInt8 p_nr_scan_asc[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 item_offset_syncc; // 
        volatile UInt8 p_item_offset_syncc[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 item_offset_subev_dsc; // 
            volatile UInt8 item_offset_pdl; // 
            volatile UInt8 item_offset_adv_asc; // 
            volatile UInt8 item_offset_scan_asc; // 
            volatile UInt8 item_offset_per_asc; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ble_mgr_0x007c : 40;
        volatile UInt8 p_unnamed_reg_ble_mgr_0x007c[40/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 item_offset_chmap; // 
        volatile UInt8 p_item_offset_chmap[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 per_asc_list_base_ptr; // 
        volatile UInt8 p_per_asc_list_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 syncc_list_base_ptr; // pointer to the sync context list
        volatile UInt8 p_syncc_list_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 chmap_list_base_ptr; // 
        volatile UInt8 p_chmap_list_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 scan_ext_info_base_ptr; // Linear ram offset for the scan_common_ext_info structure.
        volatile UInt8 p_scan_ext_info_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 current_ble_channel; // The current BLE channel being used
        volatile UInt8 p_current_ble_channel[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ble_mgr_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ble_mgr_regmap)==139));
}

/***************************
 * regmap: blefilt
 ***************************/
struct PACKED_PRE gp_blefilt_regmap
{
    union PACKED_PRE {
        volatile UInt8 state; // 
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 debug; // 
        volatile UInt8 p_debug[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_adv_ind : 1; // 
            volatile Bool ft_adv_direct_ind : 1; // 
            volatile Bool ft_adv_nonconn_ind : 1; // 
            volatile Bool ft_scan_req : 1; // 
            volatile Bool ft_scan_rsp : 1; // 
            volatile Bool ft_connect_req : 1; // 
            volatile Bool ft_adv_scan_ind : 1; // 
            volatile Bool ft_adv_ext_ind : 1; // 
            volatile Bool ft_aux_connect_rsp : 1; // 
            volatile Bool ft_reserved_9 : 1; // 
            volatile Bool ft_reserved_10 : 1; // 
            volatile Bool ft_reserved_11 : 1; // 
            volatile Bool ft_reserved_12 : 1; // 
            volatile Bool ft_reserved_13 : 1; // 
            volatile Bool ft_reserved_14 : 1; // 
            volatile Bool ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 frame_type_mask;
        volatile UInt8 p_frame_type_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool is_adv_ext_frame : 1; // 
            volatile Bool is_cp_bit_present : 1; // 
        } PACKED_POST;
        volatile UInt8 packet_info;
        volatile UInt8 p_packet_info[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cte_time; // 
        volatile UInt8 p_cte_time[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_ext_payload_hdr_len : 6; // 
            volatile UInt8 adv_ext_payload_hdr_mode : 2; // 
        } PACKED_POST;
        volatile UInt8 adv_ext_payload_hdr;
        volatile UInt8 p_adv_ext_payload_hdr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool adv_ext_hdr_flag_src_addr : 1; // 
            volatile Bool adv_ext_hdr_flag_dst_addr : 1; // 
            volatile Bool adv_ext_hdr_flag_cte_info : 1; // 
            volatile Bool adv_ext_hdr_flag_data_info : 1; // 
            volatile Bool adv_ext_hdr_flag_aux_ptr : 1; // 
            volatile Bool adv_ext_hdr_flag_sync_info : 1; // 
            volatile Bool adv_ext_hdr_flag_tx_power : 1; // 
        } PACKED_POST;
        volatile UInt8 adv_ext_hdr_flags;
        volatile UInt8 p_adv_ext_hdr_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 drop_reason : 5; // 
            volatile Bool drop_reason_valid : 1; // 
        } PACKED_POST;
        volatile UInt8 packet_status;
        volatile UInt8 p_packet_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 irk_idx; // Provides the resolving list idx, for the use case where AdvA and TargetA are not in the same pdu
        volatile UInt8 p_irk_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_0_accept_ft_adv_ind : 1; // 
            volatile Bool subfilt_0_accept_ft_adv_direct_ind : 1; // 
            volatile Bool subfilt_0_accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool subfilt_0_accept_ft_scan_req : 1; // 
            volatile Bool subfilt_0_accept_ft_scan_rsp : 1; // 
            volatile Bool subfilt_0_accept_ft_connect_req : 1; // 
            volatile Bool subfilt_0_accept_ft_adv_scan_ind : 1; // 
            volatile Bool subfilt_0_accept_ft_adv_ext_ind : 1; // 
            volatile Bool subfilt_0_accept_ft_aux_connect_rsp : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_9 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_10 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_11 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_12 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_13 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_14 : 1; // 
            volatile Bool subfilt_0_accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 subfilt_0_frame_type_accept;
        volatile UInt8 p_subfilt_0_frame_type_accept[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_0_mask_nonconn_nonscan : 1; // 
            volatile Bool subfilt_0_mask_connectable : 1; // 
            volatile Bool subfilt_0_mask_scannable : 1; // 
            volatile Bool subfilt_0_mask_rfu : 1; // 
        } PACKED_POST;
        volatile UInt8 subfilt_0_adv_mode_mask;
        volatile UInt8 p_subfilt_0_adv_mode_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_0_adv_ext_hdr_flag_src_addr : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_dst_addr : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_cte_info : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_data_info : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_aux_ptr : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_sync_info : 1; // 
            volatile Bool subfilt_0_adv_ext_hdr_flag_tx_power : 1; // 
        } PACKED_POST;
        volatile UInt8 subfilt_0_adv_ext_hdr_flags;
        volatile UInt8 p_subfilt_0_adv_ext_hdr_flags[8/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt64 subfilt_0_dst_device_address : 48; // The current device address, used by the RX filter to accept/reject a frame with a destination device address in it
            volatile Bool subfilt_0_dst_device_address_type : 1; // The current device address type, used by the RX filter to accept/reject a frame with a destination device address in it
            volatile Bool subfilt_0_enable_dst_addr_check : 1; // Wether to enable/disable the frame destination address check
            volatile Bool subfilt_0_accept_unresolved_rpa_dst : 1; // when 1, don't drop packets with an unresolved dst address.
            volatile Bool subfilt_0_dst_rpa_required : 1; // when 1, the destination address must be an RPA address, an public or static random address will be dropped
            volatile Bool subfilt_0_resolved_rpa_must_match : 1; // when 1, even successfully resolved RPA addresses must still match dst_device_address (used for advertising).
            volatile Bool subfilt_0_accept_unresolved_rpa_src : 1; // when 1, don't drop packets with an unresolved src address.
            volatile Bool subfilt_0_accept_unresolved_id_src : 1; // when 1, don't drop packets with an unresolved identity (non-rpa) address.
        } PACKED_POST;
        volatile UInt64 subfilt_0_addr_config : 56;
        volatile UInt8 p_subfilt_0_addr_config[56/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_0_ft_adv_ind_wl_en : 1; // 
            volatile Bool subfilt_0_ft_adv_direct_ind_wl_en : 1; // 
            volatile Bool subfilt_0_ft_adv_nonconn_ind_wl_en : 1; // 
            volatile Bool subfilt_0_ft_scan_req_wl_en : 1; // 
            volatile Bool subfilt_0_ft_scan_rsp_wl_en : 1; // 
            volatile Bool subfilt_0_ft_connect_req_wl_en : 1; // 
            volatile Bool subfilt_0_ft_adv_scan_ind_wl_en : 1; // 
            volatile Bool subfilt_0_ft_adv_ext_ind_wl_en : 1; // 
            volatile Bool subfilt_0_ft_aux_connect_rsp_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_9_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_10_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_11_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_12_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_13_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_14_wl_en : 1; // 
            volatile Bool subfilt_0_ft_reserved_15_wl_en : 1; // 
        } PACKED_POST;
        volatile UInt16 subfilt_0_ft_whitelist_enable;
        volatile UInt8 p_subfilt_0_ft_whitelist_enable[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_1_accept_ft_adv_ind : 1; // 
            volatile Bool subfilt_1_accept_ft_adv_direct_ind : 1; // 
            volatile Bool subfilt_1_accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool subfilt_1_accept_ft_scan_req : 1; // 
            volatile Bool subfilt_1_accept_ft_scan_rsp : 1; // 
            volatile Bool subfilt_1_accept_ft_connect_req : 1; // 
            volatile Bool subfilt_1_accept_ft_adv_scan_ind : 1; // 
            volatile Bool subfilt_1_accept_ft_adv_ext_ind : 1; // 
            volatile Bool subfilt_1_accept_ft_aux_connect_rsp : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_9 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_10 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_11 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_12 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_13 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_14 : 1; // 
            volatile Bool subfilt_1_accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 subfilt_1_frame_type_accept;
        volatile UInt8 p_subfilt_1_frame_type_accept[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_1_mask_nonconn_nonscan : 1; // 
            volatile Bool subfilt_1_mask_connectable : 1; // 
            volatile Bool subfilt_1_mask_scannable : 1; // 
            volatile Bool subfilt_1_mask_rfu : 1; // 
        } PACKED_POST;
        volatile UInt8 subfilt_1_adv_mode_mask;
        volatile UInt8 p_subfilt_1_adv_mode_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_1_adv_ext_hdr_flag_src_addr : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_dst_addr : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_cte_info : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_data_info : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_aux_ptr : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_sync_info : 1; // 
            volatile Bool subfilt_1_adv_ext_hdr_flag_tx_power : 1; // 
        } PACKED_POST;
        volatile UInt8 subfilt_1_adv_ext_hdr_flags;
        volatile UInt8 p_subfilt_1_adv_ext_hdr_flags[8/8];
    } PACKED_POST;
    UInt8 padding_2[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt64 subfilt_1_dst_device_address : 48; // The current device address, used by the RX filter to accept/reject a frame with a destination device address in it
            volatile Bool subfilt_1_dst_device_address_type : 1; // The current device address type, used by the RX filter to accept/reject a frame with a destination device address in it
            volatile Bool subfilt_1_enable_dst_addr_check : 1; // Wether to enable/disable the frame destination address check
            volatile Bool subfilt_1_accept_unresolved_rpa_dst : 1; // when 1, don't drop packets with an unresolved dst address.
            volatile Bool subfilt_1_dst_rpa_required : 1; // when 1, the destination address must be an RPA address, an public or static random address will be dropped
            volatile Bool subfilt_1_resolved_rpa_must_match : 1; // when 1, even successfully resolved RPA addresses must still match dst_device_address (used for advertising).
            volatile Bool subfilt_1_accept_unresolved_rpa_src : 1; // when 1, don't drop packets with an unresolved src address.
            volatile Bool subfilt_1_accept_unresolved_id_src : 1; // when 1, don't drop packets with an unresolved identity (non-rpa) address.
        } PACKED_POST;
        volatile UInt64 subfilt_1_addr_config : 56;
        volatile UInt8 p_subfilt_1_addr_config[56/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_1_ft_adv_ind_wl_en : 1; // 
            volatile Bool subfilt_1_ft_adv_direct_ind_wl_en : 1; // 
            volatile Bool subfilt_1_ft_adv_nonconn_ind_wl_en : 1; // 
            volatile Bool subfilt_1_ft_scan_req_wl_en : 1; // 
            volatile Bool subfilt_1_ft_scan_rsp_wl_en : 1; // 
            volatile Bool subfilt_1_ft_connect_req_wl_en : 1; // 
            volatile Bool subfilt_1_ft_adv_scan_ind_wl_en : 1; // 
            volatile Bool subfilt_1_ft_adv_ext_ind_wl_en : 1; // 
            volatile Bool subfilt_1_ft_aux_connect_rsp_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_9_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_10_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_11_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_12_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_13_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_14_wl_en : 1; // 
            volatile Bool subfilt_1_ft_reserved_15_wl_en : 1; // 
        } PACKED_POST;
        volatile UInt16 subfilt_1_ft_whitelist_enable;
        volatile UInt8 p_subfilt_1_ft_whitelist_enable[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            UInt8 padding_4 : 2;
            volatile Bool enable_coex_sw_overrule_on_accept : 1; // when 1, the rx filter will enable the coex SW overrule in RIB, This makes it possible to keep the coex_req alive.
            volatile Bool disable : 1; // when 1 the ble rx filter will accept all packets
            volatile Bool resolve_res_pr_src : 1; // resolve the src address and use the resolved address in filtering
            volatile Bool whitelist_skip_rpa_check : 1; // If 1, and if src addr is RPA don't check the whitelist in parallel with RPA resolving during.
            volatile Bool enable_crc_check : 1; // If 1, frames with an invalid CRC will be dropped.
        } PACKED_POST;
        volatile UInt8 unnamed_reg_blefilt_0x002a;
        volatile UInt8 p_unnamed_reg_blefilt_0x002a[8/8];
    } PACKED_POST;
    UInt8 padding_5[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 whitelist_base_address; // whitelist table base address ( = offset added to SM_linear start address). Resolution 1 byte - Caution must be 16 bit alligned!
            volatile UInt8 whitelist_length; // length of the whitelist in nr of entries
        } PACKED_POST;
        volatile UInt32 whitelist_config : 24;
        volatile UInt8 p_whitelist_config[24/8];
    } PACKED_POST;
    UInt8 padding_6[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 res_pr_identity_list_base_address; // resolvable private address table base address. Contains local entries of type ble_res_pr_local_entry.
            volatile UInt8 res_pr_list_length; // length of the whitelist in nr of entries
        } PACKED_POST;
        volatile UInt32 res_pr_list_config : 24;
        volatile UInt8 p_res_pr_list_config[24/8];
    } PACKED_POST;
    UInt8 padding_7[1];
    union PACKED_PRE {
        volatile UInt32 res_pr_peer_irk_list_base_address : 24; // resolvable private address table compressed address (16 byte alligned). Contains IRKs.
        volatile UInt8 p_res_pr_peer_irk_list_base_address[24/8];
    } PACKED_POST;
    UInt8 padding_8[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 whitelist_rpa_offset; // Offset to start the search from
            volatile UInt8 whitelist_rpa_length; // Length to search.
        } PACKED_POST;
        volatile UInt16 whitelist_rpa;
        volatile UInt8 p_whitelist_rpa[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 subfilt_0_wl_valid_state_mask; // whitelist state mask
        volatile UInt8 p_subfilt_0_wl_valid_state_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 subfilt_0_phy_mask; // phy mask
        volatile UInt8 p_subfilt_0_phy_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 subfilt_1_wl_valid_state_mask; // whitelist state mask
        volatile UInt8 p_subfilt_1_wl_valid_state_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 subfilt_1_phy_mask; // phy mask
        volatile UInt8 p_subfilt_1_phy_mask[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_blefilt_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_blefilt_regmap)==62));
}

/***************************
 * regmap: rt
 ***************************/
struct PACKED_PRE gp_rt_regmap
{
    union PACKED_PRE {
        volatile UInt8 state; // 
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        volatile UInt32 debug; // 
        volatile UInt8 p_debug[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 shutdown_pending; // Indicates wheter a gpmicro shutdown is pending.
        volatile UInt8 p_shutdown_pending[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_current_nr; // current event number being executed
        volatile UInt8 p_ev_current_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_current_abort; // 
        volatile UInt8 p_ev_current_abort[8/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt16 ev_cleanup_time; // 
        volatile UInt8 p_ev_cleanup_time[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ev_processing_delay; // 
        volatile UInt8 p_ev_processing_delay[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_untimed_nr; // ES event to handle the ZigBee events
        volatile UInt8 p_ev_untimed_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_timed_tx_nr; // RT event to handle timed 15.4 Tx events
        volatile UInt8 p_ev_timed_tx_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_untimed_busy; // indicates if the current untimed event is being executed.
        volatile UInt8 p_ev_untimed_busy[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ev_untimed_list_halt; // indicates if the current untimed event is being executed.
        volatile UInt8 p_ev_untimed_list_halt[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ev_untimed_list_head_ptr; // internal RT pointer, just here for observability. DO NOT WRITE!
        volatile UInt8 p_ev_untimed_list_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ev_timed_list_head_ptr; // internal RT pointer, just here for observability. DO NOT WRITE!
        volatile UInt8 p_ev_timed_list_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 zbev_list_head_ptr; // internal RT pointer, just here for observability. DO NOT WRITE!
        volatile UInt8 p_zbev_list_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ev_list_free_ptr; // pointer (offset from RAM_LINEAR_START)
        volatile UInt8 p_ev_list_free_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 event_base_ptr; // pointer (offset from RAM_LINEAR_START)
        volatile UInt8 p_event_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool rx_wd_abort_pending : 1; // internal RT data, do not write!
            volatile Bool rx_wd_busy : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_rt_0x001e;
        volatile UInt8 p_unnamed_reg_rt_0x001e[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool rx_on_when_idle_ch0 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 0
            volatile Bool rx_on_when_idle_ch1 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 1
            volatile Bool rx_on_when_idle_ch2 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 2
            volatile Bool rx_on_when_idle_ch3 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 3
            volatile Bool rx_on_when_idle_ch4 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 4
            volatile Bool rx_on_when_idle_ch5 : 1; // Indicates that Akuma should configure RX_ON_WHEN_IDLE on channel index 5
        } PACKED_POST;
        volatile UInt8 rx_on_when_idle_ch;
        volatile UInt8 p_rx_on_when_idle_ch[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 rx_ch0; // Indicates the (FLL) RX channel belonging to channel index 0
            volatile UInt8 rx_ch1; // Indicates the (FLL) RX channel belonging to channel index 1
            volatile UInt8 rx_ch2; // Indicates the (FLL) RX channel belonging to channel index 2
            volatile UInt8 rx_ch3; // Indicates the (FLL) RX channel belonging to channel index 3
            volatile UInt8 rx_ch4; // Indicates the (FLL) RX channel belonging to channel index 4
            volatile UInt8 rx_ch5; // Indicates the (FLL) RX channel belonging to channel index 5
        } PACKED_POST;
        volatile UInt64 rx_ch : 48;
        volatile UInt8 p_rx_ch[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_entries_base_ptr; // pointer (offset from RAM_LINEAR_START) to the space where the RCI entries are allocated
        volatile UInt8 p_rci_entries_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 rci_entries_nr; // number of entries in use
        volatile UInt8 p_rci_entries_nr[8/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt16 rci_data_ind_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the data_ind linked list
        volatile UInt8 p_rci_data_ind_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_data_cnf_0_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the data_cnf_0 linked list
        volatile UInt8 p_rci_data_cnf_0_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_data_cnf_1_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the data_cnf_1 linked list
        volatile UInt8 p_rci_data_cnf_1_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_data_cnf_2_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the data_cnf_2 linked list
        volatile UInt8 p_rci_data_cnf_2_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_data_cnf_3_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the data_cnf_3 linked list
        volatile UInt8 p_rci_data_cnf_3_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_ble_data_ind_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the ble_data_ind linked list
        volatile UInt8 p_rci_ble_data_ind_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_ble_adv_ind_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the ble_adv_ind linked list
        volatile UInt8 p_rci_ble_adv_ind_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_ble_conn_req_ind_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the ble_conn_req_ind linked list
        volatile UInt8 p_rci_ble_conn_req_ind_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_ble_conn_rsp_ind_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the ble_conn_rsp_ind linked list
        volatile UInt8 p_rci_ble_conn_rsp_ind_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rci_ble_data_cnf_head_ptr; // pointer (offset from RAM_LINEAR_START) to the head of the ble_data_cnf linked list
        volatile UInt8 p_rci_ble_data_cnf_head_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 format_r_ptrs_base_ptr; // pointer (offset from RAM_LINEAR_START) to the start of avaiable format_r pointer list, assumes an offset of 0x2
        volatile UInt8 p_format_r_ptrs_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 format_r_ptrs_nr; // number of format_r pointers in the list
        volatile UInt8 p_format_r_ptrs_nr[8/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        volatile UInt16 format_r_max_frame_len; // 
        volatile UInt8 p_format_r_max_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 active_format_r_ptr; // current format R buffer used by the receiver
        volatile UInt8 p_active_format_r_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ack_zb_format_t_ptr; // pointer (offset from RAM_LINEAR_START) to the ACK format_t
        volatile UInt8 p_ack_zb_format_t_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 rx_dma_channel : 2; // 
            UInt8 padding_4 : 2;
            volatile UInt8 tx_dma_channel : 2; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_rt_0x0048;
        volatile UInt8 p_unnamed_reg_rt_0x0048[8/8];
    } PACKED_POST;
    UInt8 padding_5[3];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_preamble_len; // 
            volatile UInt8 ble_preamble_len; // The number of preambles the RT system should take into account for 1 Mbit frames
            volatile UInt8 ble_hdr_preamble_len; // The number of preambles the RT system should take into account for 2 Mbit frames
            volatile UInt8 ble_lr_preamble_len; // The number of preambles the RT system should take into account for long range frames
        } PACKED_POST;
        volatile UInt32 unnamed_reg_rt_0x004c;
        volatile UInt8 p_unnamed_reg_rt_0x004c[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_crc_start_byte : 4; // 
            volatile UInt8 ble_crc_start_byte : 4; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_rt_0x0050;
        volatile UInt8 p_unnamed_reg_rt_0x0050[8/8];
    } PACKED_POST;
    UInt8 padding_6[1];
    union PACKED_PRE {
        volatile UInt16 zb_rx_timestamp_offset; // value to be subtracted from the PAR timestamp to correct for timestamping delays
        volatile UInt8 p_zb_rx_timestamp_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ble_rx_timestamp_offset; // value to be subtracted from the PAR timestamp to correct for timestamping delays
        volatile UInt8 p_ble_rx_timestamp_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ble_hdr_rx_timestamp_offset; // value to be subtracted from the PAR timestamp to correct for timestamping delays
        volatile UInt8 p_ble_hdr_rx_timestamp_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ble_lr125_rx_timestamp_offset; // value to be subtracted from the PAR timestamp to correct for timestamping delays
        volatile UInt8 p_ble_lr125_rx_timestamp_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ble_lr500_rx_timestamp_offset; // value to be subtracted from the PAR timestamp to correct for timestamping delays
        volatile UInt8 p_ble_lr500_rx_timestamp_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ble_whitening_enable; // 
        volatile UInt8 p_ble_whitening_enable[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_timestamp_correction; // 
        volatile UInt8 p_tx_timestamp_correction[8/8];
    } PACKED_POST;
    UInt8 padding_7[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 t_cal_warmup; // 
            volatile UInt8 t_rx_warmup; // 
            volatile UInt8 t_tx_warmup; // 
            volatile UInt8 t_off_warmup; // 
        } PACKED_POST;
        volatile UInt32 unnamed_reg_rt_0x0060;
        volatile UInt8 p_unnamed_reg_rt_0x0060[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 t_cal_duration; // 
        volatile UInt8 p_t_cal_duration[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 t_tx_pa_warmup; // amount of time to give the PA to settle before starting the modulation
        volatile UInt8 p_t_tx_pa_warmup[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool zb_csma_force_fail : 1; // 
            volatile Bool zb_dont_disable_radio_for_tx_to_rxack : 1; // 
            volatile Bool zb_dont_disable_radio_for_rx_to_txack : 1; // 
            volatile Bool zb_ifs_skip : 1; // 
            volatile Bool zb_ifs_after_tx_ack_skip : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_rt_0x0066;
        volatile UInt8 p_unnamed_reg_rt_0x0066[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_lifs; // resolution 16 us # long-packet interframe spacing
        volatile UInt8 p_zb_lifs[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_cca_ed_thresh; // 
            volatile UInt8 zb_cca_measurement_time; // duration of the CCA measurment in us
            volatile Bool zb_cca_mode : 1; // Sets the cca mode to either energy only or energy combined with carrier sense (carrier sense only can be obtained by choosing the combined mode and lowering the cca_ed_threshold)
        } PACKED_POST;
        volatile UInt32 unnamed_reg_rt_0x0068 : 24;
        volatile UInt8 p_unnamed_reg_rt_0x0068[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_ack_turnaround; // turnaround time for ACK transmission. Note that this denotes the the start of the TX state, TX warmup time and PA startup time still need to be added to this value for the real start of TX
        volatile UInt8 p_zb_ack_turnaround[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_ack_wd_offset; // AIFS-t_rx_warmup-t_wd_margin
            volatile UInt8 zb_ack_wd_duration; // t_rx_warmup+pre_det_dur + 2*t_wd_margin
        } PACKED_POST;
        volatile UInt16 zb_ack_wd_settings;
        volatile UInt8 p_zb_ack_wd_settings[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_max_frame_len; // 
            volatile UInt8 zb_ack_max_frame_len; // 
        } PACKED_POST;
        volatile UInt16 unnamed_reg_rt_0x006e;
        volatile UInt8 p_unnamed_reg_rt_0x006e[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool zb_coex_ack_tx_req_en : 1; // ZB ack TX coex request enabled
            volatile UInt8 zb_coex_ack_tx_prio : 2; // ZB ack TX coex priority
        } PACKED_POST;
        volatile UInt8 zb_coex_ack_tx;
        volatile UInt8 p_zb_coex_ack_tx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool zb_coex_packet_tx_disable_pa : 1; // ZB packet TX disable PA if grant is missing or not given, fully ties PA enable with grant status. Only used if ZB_FORMAT_T_COEX_PACKET_TX_REQ_EN and ZB_FORMAT_T_COEX_GRANT_AWARE are set
            volatile Bool zb_coex_packet_tx_abort : 1; // ZB packet TX abort TX if grant is lost during a transaction, if set, RX is immediately resumed after grant is lost. Only used if ZB_FORMAT_T_COEX_PACKET_TX_REQ_EN and ZB_FORMAT_T_COEX_GRANT_AWARE are set
            volatile Bool zb_coex_packet_tx_wait_for_grant : 1; // ZB packet TX request and wait for grant before starting TX, zb_coex_wait_timeout defines max time waiting for the grant. Only used if ZB_FORMAT_T_COEX_PACKET_TX_REQ_EN and ZB_FORMAT_T_COEX_GRANT_AWARE are set
            volatile Bool zb_coex_ack_tx_disable_pa : 1; // ZB ack TX disable PA if grant is missing or not given, fully ties PA enable with grant status. Only used if zb_coex_ack_tx_req_en is set
            volatile Bool zb_coex_ack_tx_skip : 1; // ZB ack TX skip sending ack if there is no grant at the start time. Only used if zb_coex_ack_tx_req_en AND RTM_TRC_ZB_COEX_PACKET_RX_REQ_EN are set
            volatile Bool ble_coex_packet_tx_disable_pa : 1; // BLE packet TX disable PA if grant is missing or not given, fully ties PA enable with grant status. Only used if request_en and grant aware are set
        } PACKED_POST;
        volatile UInt8 coex_no_grant_actions;
        volatile UInt8 p_coex_no_grant_actions[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_coex_packet_tx_wait_timeout; // Timeout in ms to abort a TX attempt if grant is not given while waiting for it. Used when zb_coex_packet_wait_for_grant is set
        volatile UInt8 p_zb_coex_packet_tx_wait_timeout[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_coex_packet_rx_ext_prio : 2; // Set coex priority when rx request extension is enabled
            volatile Bool zb_coex_packet_rx_ext_req_en : 1; // Enable/disable coex rx request extension feature. zb_coex_packet_rx_ext_ev_nr meeds to be correctly configured for rx req ext to work.
            volatile Bool zb_coex_packet_rx_ext_req_after_phy_drop : 1; // Enables extension feature after a packet was somehow dropped by the phy (incomplete/corrupted/high RF interference)
            volatile Bool zb_coex_packet_rx_ext_req_after_packet_abort : 1; // Enables extension feature after a packet was aborted by mac filter (f.i. wrong addressing...)
            volatile Bool zb_coex_packet_rx_ext_req_after_fcs_err : 1; // Enables extension feature after a packet was dropped because of bad CRC
            volatile Bool zb_coex_packet_rx_ext_req_after_valid : 1; // Enables extension feature after a packet was correctly received
        } PACKED_POST;
        volatile UInt8 zb_coex_packet_rx_ext_settings;
        volatile UInt8 p_zb_coex_packet_rx_ext_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_coex_packet_rx_ext_timeout; // Configure the timeout for the coex rx packet extension feature in ms. This means a coex request can be extended by up to 127 ms after a packet was received
        volatile UInt8 p_zb_coex_packet_rx_ext_timeout[8/8];
    } PACKED_POST;
    UInt8 padding_8[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 zb_coex_packet_rx_ext_timeout_cnt; // Number of timedout coex extensions. Only increments when zb_coex_packet_rx_ext_cnt!=255.
            volatile UInt8 zb_coex_packet_rx_ext_cnt; // Number of coex extensions that have been triggered. stops incrementing when 255.
        } PACKED_POST;
        volatile UInt16 zb_coex_packet_rx_ext_counters;
        volatile UInt8 p_zb_coex_packet_rx_ext_counters[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_coex_packet_rx_ext_ev_nr; // ES event to handle the ZigBee packet RX ext timeout, if it has na invalid event number(f.i. 0xFF), RX ext cannot be used
        volatile UInt8 p_zb_coex_packet_rx_ext_ev_nr[8/8];
    } PACKED_POST;
    UInt8 padding_9[3];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_req_toggle_prio0; // Number of rising edges on coex request with prio 0
            volatile UInt8 coex_req_toggle_prio1; // Number of rising edges on coex request with prio 1
            volatile UInt8 coex_req_toggle_prio2; // Number of rising edges on coex request with prio 2
            volatile UInt8 coex_req_toggle_prio3; // Number of rising edges on coex request with prio 3
        } PACKED_POST;
        volatile UInt32 coex_req_toggle_cnt;
        volatile UInt8 p_coex_req_toggle_cnt[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_denied_prio0; // Number of times coex request went to active and back to inactive while grant stayed inactive (medium denied) with prio 0
            volatile UInt8 coex_denied_prio1; // Number of times coex request went to active and back to inactive while grant stayed inactive (medium denied) with prio 1
            volatile UInt8 coex_denied_prio2; // Number of times coex request went to active and back to inactive while grant stayed inactive (medium denied) with prio 2
            volatile UInt8 coex_denied_prio3; // Number of times coex request went to active and back to inactive while grant stayed inactive (medium denied) with prio 3
        } PACKED_POST;
        volatile UInt32 coex_denied_cnt;
        volatile UInt8 p_coex_denied_cnt[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_aborted_prio0; // Number of times grant was lost while request was still active (ongoing request was aborted) with prio 0
            volatile UInt8 coex_aborted_prio1; // Number of times grant was lost while request was still active (ongoing request was aborted) with prio 1
            volatile UInt8 coex_aborted_prio2; // Number of times grant was lost while request was still active (ongoing request was aborted) with prio 2
            volatile UInt8 coex_aborted_prio3; // Number of times grant was lost while request was still active (ongoing request was aborted) with prio 3
        } PACKED_POST;
        volatile UInt32 coex_aborted_cnt;
        volatile UInt8 p_coex_aborted_cnt[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rf_pre_calibration_table_ptr; // Pointer to a table with calibration information for all fll channels
        volatile UInt8 p_rf_pre_calibration_table_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 cal_info_base_ptr; // Offset from GP_MM_RAM_LINEAR_START address
        volatile UInt8 p_cal_info_base_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cal_info_status; // 
        volatile UInt8 p_cal_info_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cal_info_entry_idx; // 
        volatile UInt8 p_cal_info_entry_idx[8/8];
    } PACKED_POST;
    UInt8 padding_10[2];
    union PACKED_PRE {
        volatile UInt32 cal_req_time; // 
        volatile UInt8 p_cal_req_time[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 cal_threshold; // 
        volatile UInt8 p_cal_threshold[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 zb_sifs; // resolution 16 us # short-packet interframe spacing
        volatile UInt8 p_zb_sifs[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_regmap)==153));
}

/***************************
 * regmap: rap
 ***************************/
struct PACKED_PRE gp_rap_regmap
{
    union PACKED_PRE {
        volatile UInt8 zb_rx_mode; // Lists the last configured ZB rx_mode, written by the rap_rx_zb_set_mode(), 0xff means not configured yet
        volatile UInt8 p_zb_rx_mode[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ble_rx_mode; // Lists the last configured BLE rx_mode, written by the rap_rx_ble_set_mode(), 0xff means not configured yet
        volatile UInt8 p_ble_rx_mode[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rap_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rap_regmap)==2));
}

/***************************
 * regmap: zbfilt
 ***************************/
struct PACKED_PRE gp_zbfilt_regmap
{
    union PACKED_PRE {
        volatile UInt8 channel_idx; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator; // Set to 2 if device if pan coordinator for primary pan. Set to 1 if not
        volatile UInt8 p_pan_coordinator[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address; // Short address for the device within its primary PAN
        volatile UInt8 p_short_address[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id; // primary pan id the device operates in
        volatile UInt8 p_pan_id[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address; // 64 bit mac address for the device
        volatile UInt8 p_extended_address[64/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        volatile UInt8 channel_idx_alt_a; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx_alt_a[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator_alt_a; // Set to 2 if device if pan coordinator for secondary pan. Set to 1 if not
        volatile UInt8 p_pan_coordinator_alt_a[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address_alt_a; // Short address for the device within its secondary PAN
        volatile UInt8 p_short_address_alt_a[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id_alt_a; // secondary pan id the device operates in
        volatile UInt8 p_pan_id_alt_a[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address_alt_a; // 64 bit secondary mac address for the device
        volatile UInt8 p_extended_address_alt_a[64/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt8 channel_idx_alt_b; // Channel idx mask for which the pan_id/src_addr pair is is valid
        volatile UInt8 p_channel_idx_alt_b[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_coordinator_alt_b; // Set to 2 if device if pan coordinator for tertiary pan.. Set to 1 if not
        volatile UInt8 p_pan_coordinator_alt_b[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 short_address_alt_b; // Short address for the device within third PAN
        volatile UInt8 p_short_address_alt_b[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id_alt_b; // third pan id the device operates in
        volatile UInt8 p_pan_id_alt_b[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 extended_address_alt_b; // 64 bit tertiary mac address for the device
        volatile UInt8 p_extended_address_alt_b[64/8];
    } PACKED_POST;
    UInt8 padding_2[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 short_src_address_table_base_address; // short source address table base address ( = offset added to SM_linear start address). Resolution 1 byte - Caution must be 16 bit alligned!
            volatile UInt8 short_src_address_table_size; // Indicates the nr of short src address table entries
        } PACKED_POST;
        volatile UInt32 short_src_address_table_config : 24;
        volatile UInt8 p_short_src_address_table_config[24/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 long_src_address_table_base_address; // long source address table base address ( = offset added to SM_linear start address). Resolution 1 byte - Caution must be 16 bit alligned!
            volatile UInt8 long_src_address_table_size; // Indicates the nr of long src address table entries
        } PACKED_POST;
        volatile UInt32 long_src_address_table_config : 24;
        volatile UInt8 p_long_src_address_table_config[24/8];
    } PACKED_POST;
    UInt8 padding_4[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_5 : 2;
            volatile UInt8 ack_power_h : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_6 : 2;
            volatile UInt8 ack_power_rssi_thr; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan;
        volatile UInt8 p_ack_power_cfg_pan[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_alt_a : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_7 : 2;
            volatile UInt8 ack_power_h_alt_a : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_8 : 2;
            volatile UInt8 ack_power_rssi_thr_alt_a; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing_alt_a; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan_alt_a;
        volatile UInt8 p_ack_power_cfg_pan_alt_a[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_alt_b : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_9 : 2;
            volatile UInt8 ack_power_h_alt_b : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does belongs to this pan/short/ch_idx combo
            UInt8 padding_10 : 2;
            volatile UInt8 ack_power_rssi_thr_alt_b; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does belongs to this pan/short/ch_idx combo
            volatile UInt8 ack_power_spacing_alt_b; // this has no meaning, just a placeholder for this address so the table is correctly structured
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_pan_alt_b;
        volatile UInt8 p_ack_power_cfg_pan_alt_b[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_power_l_default : 6; // ack tx power if rssi is lower than threshold(thr), this is used when incomming frame does not belong to a pan/short/ch_idx combo
            UInt8 padding_11 : 2;
            volatile UInt8 ack_power_h_default : 6; // ack tx power if rssi is higher or equal to threshold(thr), this is used when incomming frame does not belong to a pan/short/ch_idx combo
            UInt8 padding_12 : 2;
            volatile UInt8 ack_power_rssi_thr_default; // threshold which will be compared to the rssi of the incomming frame, this is used when incomming frame does not belong to a pan/short/ch_idx combo
        } PACKED_POST;
        volatile UInt32 ack_power_cfg_default : 24;
        volatile UInt8 p_ack_power_cfg_default[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool enable_sniffing : 1; // When enabled, all packets will be queued even if the macfilter would normally drop the frame. The pbm will indicate in the drop_reason field if the frame was dropped or not
            volatile Bool disable : 1; // When 1 the macfilter will be disabled and will accept every packet that comes in (needed for BLE)
            volatile UInt8 every_frame_is_datareq : 3; // When enabled, the src address search will be done for every frame. Can be set per PAN ID. Index 0 corresponds to pan_id, index 1 -> pan_id_alt_a, index 2 -> pan_id_alt_b.
            volatile Bool every_frame_is_datareq_default : 1; // When enabled, the src address search will be done for every frame. Default setting, used when a frame cannot be matched with one of the PAN IDs, e.g. broadcast PAN ID.
            volatile Bool claim_radio_on_every_rx : 1; // when enabled the macfilter will claim the radio for zigbee as soon as the header is received. When disabled, the radio is only claimed for packets on which an ACK respose is to be sent.
            volatile Bool enable_2015_support : 1; // When enabled, the macfilter will support 2015 spec frames. Not all features are implemented yet.
        } PACKED_POST;
        volatile UInt8 generic_control;
        volatile UInt8 p_generic_control[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cmd_type; // Cmd type that can be filtered on
        volatile UInt8 p_cmd_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_bcn : 1; // When 1, frames of type beacon will be accepted by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_data : 1; // When 1, frames of type data will be accepted by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_ack : 1; // When 1, frames of type ack will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_cmd : 1; // When 1, frames of type beacon will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_rsv_4 : 1; // When 1, frames of reserved frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_mp : 1; // When 1, frames of Multipurpose frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_fra : 1; // When 1, frames of fragment or frak frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
            volatile Bool accept_ft_ext : 1; // When 1, frames of extended frame type(s) will not be dropped by mac filter, and will be acked if needed and other filters also match
        } PACKED_POST;
        volatile UInt8 frame_type_accept;
        volatile UInt8 p_frame_type_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_fv_2003 : 1; // When 1, frames of frames using the 2003 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_2006 : 1; // When 1, frames of frames using the 2006 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_2015 : 1; // When 1, frames of frames using the 2010 mac version will be accepted and acked if needed and other filters also match
            volatile Bool accept_fv_rsv_3 : 1; // When 1, frames of frames using the unknown mac version will be accepted and acked if needed and other filters also match
        } PACKED_POST;
        volatile UInt8 frame_version_accept;
        volatile UInt8 p_frame_version_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_bcn_src_addr_mode_00 : 1; // When 1, beacon frames with src address mode set to 0 will be accepted. Default is 0, used to filter out all zero packets
            volatile Bool accept_bcast_daddr : 1; // When 1 (=default), broadcast destination addressing will be accepted. Use this setting to filter out broadcast packets
            volatile Bool accept_bcast_pan_id : 1; // When 1 (=default), broadcast pan id-s will be accepted. Use this setting to filter out packets using broadcast
        } PACKED_POST;
        volatile UInt8 frame_addressing_accept;
        volatile UInt8 p_frame_addressing_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool src_pan_id_beacon_check_on : 1; // When 1, the src pan id is checked for beacon frames, and the frame is accepted when the pan id matches
            volatile Bool src_pan_id_data_command_check_on : 1; // When 1, the src pan id is checked for data/cmd frames, and the frame is accepted when the pan id matches
            volatile Bool dst_pan_id_check_on : 1; // When 1, the destination pan id is checked and the frame is accepted when the pan id matches
            volatile Bool dst_addr_check_on : 1; // When 1, the src addr is checked and the frame is accepted when the src address matches the address set by short_addr (or extended addresss)
            volatile Bool cmd_type_check_on : 1; // When 1, the cmd type is checked and the frame is accepted when the command type matches
        } PACKED_POST;
        volatile UInt8 frame_checks;
        volatile UInt8 p_frame_checks[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 allow_zb_rx_subev; // 
        volatile UInt8 p_allow_zb_rx_subev[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 max_frame_len; // 
        volatile UInt8 p_max_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool fp_value_when_no_match_found : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool fp_value_when_no_match_found_alt_a : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool fp_value_when_no_match_found_alt_b : 1; // This sets the polarity of the src address search. When set to 0, the fp in the ack will be set to 0 when no match is found, and to 1 when a match is found. When set to 1, the opposite behavior is obtained
            volatile Bool dont_ack_if_not_in_src_list : 1; // When enabled, no ack will be sent if the src address is not in the src list
            volatile Bool drop_if_not_in_src_list : 1; // When enabled, frames will be dropped when the src address is not in the src list
            volatile Bool enh_ack_insert_csl_ie : 1; // when enabled, the CSL IE will be inserted into the Enhanced-Ack
            volatile Bool disable_crc_check : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_zbfilt_0x0050;
        volatile UInt8 p_unnamed_reg_zbfilt_0x0050[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_bcn_to_queue : 1; // When 1, beacon frames will be passed to the queue
            volatile Bool ft_data_to_queue : 1; // When 1, data frames will be passed to the queue
            volatile Bool ft_ack_to_queue : 1; // When 1, ack frames will be passed to the queue
            volatile Bool ft_cmd_to_queue : 1; // When 1, cmd frames will be passed to the queue
            volatile Bool ft_rsv_4_to_queue : 1; // When 1, reserved frames will be passed to the queue
            volatile Bool ft_mp_to_queue : 1; // When 1, Multipurpose frames will be passed to the queue
            volatile Bool ft_fra_to_queue : 1; // When 1, fragment or frak frames will be passed to the queue
            volatile Bool ft_ext_to_queue : 1; // When 1, extended frames will be passed to the queue
        } PACKED_POST;
        volatile UInt8 frame_to_queue_enables;
        volatile UInt8 p_frame_to_queue_enables[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ack_request_enable_ch_idx_0 : 1; // enable ack request for channel index 0
            volatile Bool ack_request_enable_ch_idx_1 : 1; // enable ack request for channel index 1
            volatile Bool ack_request_enable_ch_idx_2 : 1; // enable ack request for channel index 2
            volatile Bool ack_request_enable_ch_idx_3 : 1; // enable ack request for channel index 3
            volatile Bool ack_request_enable_ch_idx_4 : 1; // enable ack request for channel index 4
            volatile Bool ack_request_enable_ch_idx_5 : 1; // enable ack request for channel index 5
        } PACKED_POST;
        volatile UInt8 ack_request_enables;
        volatile UInt8 p_ack_request_enables[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_bcn : 1; // 
            volatile Bool ft_data : 1; // 
            volatile Bool ft_ack : 1; // 
            volatile Bool ft_cmd : 1; // 
            volatile Bool ft_rsv_4 : 1; // 
            volatile Bool ft_mp : 1; // 
            volatile Bool ft_fra : 1; // 
            volatile Bool ft_ext : 1; // 
        } PACKED_POST;
        volatile UInt8 frame_type_mask;
        volatile UInt8 p_frame_type_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool fv_2003 : 1; // 
            volatile Bool fv_2006 : 1; // 
            volatile Bool fv_2015 : 1; // 
            volatile Bool fv_rsv_3 : 1; // 
        } PACKED_POST;
        volatile UInt8 frame_version_mask;
        volatile UInt8 p_frame_version_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 last_drop_reason; // 
        volatile UInt8 p_last_drop_reason[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ack_request : 1; // 
            volatile Bool seq_nr_suppr : 1; // 
            volatile Bool is_datareq : 1; // 
            volatile Bool security_enabled : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_zbfilt_0x0056;
        volatile UInt8 p_unnamed_reg_zbfilt_0x0056[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pan_idx; // # index that matches with the destination address
        volatile UInt8 p_pan_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 seq_nr; // 
        volatile UInt8 p_seq_nr[8/8];
    } PACKED_POST;
    UInt8 padding_13[3];
    union PACKED_PRE {
        volatile UInt32 security_ctrl_ptr; // 
        volatile UInt8 p_security_ctrl_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 src_list_idx; // 0xFF means no match is found, 0x100 means there was no source address, value less than 0xFF is the match index
            volatile UInt16 dst_match_idx; // 0xFF means no match is found, 0x100 means there was no destination address, value less than 0xFF is the table match index
        } PACKED_POST;
        volatile UInt32 unnamed_reg_zbfilt_0x0060;
        volatile UInt8 p_unnamed_reg_zbfilt_0x0060[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_seq_number; // Sequence number place holder register for the sequence number of ongoing transmission
        volatile UInt8 p_tx_seq_number[8/8];
    } PACKED_POST;
    UInt8 padding_14[1];
    union PACKED_PRE {
        volatile UInt16 broadcast_mask; // placeholder to put a broadcast mask (so 0xFFFF) - needs to be inited
        volatile UInt8 p_broadcast_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 debug; // 
        volatile UInt8 p_debug[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 state; // 
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 drop_reason : 5; // Reason why frame was dropped
            volatile Bool drop_reason_valid : 1; // 
        } PACKED_POST;
        volatile UInt8 drop_status;
        volatile UInt8 p_drop_status[8/8];
    } PACKED_POST;
    UInt8 padding_15[1];
    union PACKED_PRE {
        volatile UInt32 last_rx_opts_ptr; // last rx opts pointer, will be used during ACK construction to store TX ACK info in the Rx opts.
        volatile UInt8 p_last_rx_opts_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 csl_sampletime; // value in microseconds of the next sample time.
        volatile UInt8 p_csl_sampletime[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 csl_period; // value in 10*symbol units of the CSL period.
        volatile UInt8 p_csl_period[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool crc_ok : 1; // 
            volatile Bool is_matching_ack : 1; // 
            volatile Bool frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 frame_status;
        volatile UInt8 p_frame_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool suc_exp_crc_ok : 1; // 
            volatile Bool suc_exp_is_matching_ack : 1; // 
            volatile Bool suc_exp_frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 success_exp_flags;
        volatile UInt8 p_success_exp_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool suc_msk_crc_ok : 1; // 
            volatile Bool suc_msk_is_matching_ack : 1; // 
            volatile Bool suc_msk_frame_pending : 1; // 
        } PACKED_POST;
        volatile UInt8 success_mask_flags;
        volatile UInt8 p_success_mask_flags[8/8];
    } PACKED_POST;
    UInt8 padding_16[1];
    union PACKED_PRE {
        volatile UInt16 rawmode_nonce_ptr; // A pointer to a struct with security info be used in the transmit frames that are encrypted by the RT code (for a stack in Raw mode).
        volatile UInt8 p_rawmode_nonce_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rawmode_key_info_ptr; // A pointer to a struct containing the key identifier and key values to be used in the transmit frames that are encrypted by the RT code (for a stack in Raw mode).
        volatile UInt8 p_rawmode_key_info_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rawmode_vsie_ptr; // A pointer to the vsie struct which containst the info to insert the VS IE in enhanced ack frames.
        volatile UInt8 p_rawmode_vsie_ptr[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zbfilt_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zbfilt_regmap)==128));
}

/***************************
 * regmap: rt_error
 ***************************/
struct PACKED_PRE gp_rt_error_regmap
{
    union PACKED_PRE {
        volatile UInt8 type; // The error or unexpected condition interface is an unidirectional interface from the GPMicro to NRT Software. It is used to signal error or unexpected conditions in the RT software. The GPMicro asserts the IPC_UNMASKED_GPM2X_UNEXPECTED_COND_INTERUPT when such a situation occurs. This is an one hit interface. As long as the IPC_UNMASKED_GPM2X_UNEXPECTED_COND_INTERUPT is asserted, the RT processor will not generate new messages. The NRT software is responsible for clearing the interrupt.
        volatile UInt8 p_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 info; // Contains additional info. (which is dependent on the type of condition)
        volatile UInt8 p_info[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_error_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_error_regmap)==2));
}

/***************************
 * regmap: rt_stat
 ***************************/
struct PACKED_PRE gp_rt_stat_regmap
{
    union PACKED_PRE {
        volatile UInt8 type; // The STATUS interface is a general information output channel from RT processor. It is used to send out internal state/debug information. When there is a info message the IPC_UNMASKED_GPM2X_COMM_INTERRUPT is asserted. This is an one hit interface. As long as the IPC_UNMASKED_GPM2X_STAT_INTERRUPT is asserted, the RT processor will not generate new messages. The NRT software is responsible for clearing the interrupt.
        volatile UInt8 p_type[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 es_trigger_too_late_trigger_type; // Which trigger type was executed too late.
            volatile UInt8 es_trigger_too_late_event_type; // Which event type was executed too late.
            volatile UInt16 es_trigger_too_late_t_too_late; // Amount of time in microseconds the trigger was too late. Accuracy of this value is not that good.  Because this value is calculated by sampling the symbol counter at ES interrupt entry, and this might be blocked by a higher priority interrupt (which may take up to 200 us).
        } PACKED_POST;
        volatile UInt32 info;
        volatile UInt8 p_info[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_stat_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_stat_regmap)==6));
}

/***************************
 * regmap: rt_cmd
 ***************************/
struct PACKED_PRE gp_rt_cmd_regmap
{
    union PACKED_PRE {
        volatile UInt8 command; // Command type for the BLE COMMAND interface. Only 1 command can be run at a time. The user must wait for completion before initiating a new command.
        volatile UInt8 p_command[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 result; // The status of the ran command. If the contents of this register is equal to the rt_cmd_command type number, it indicates success. If otherwise, an error has occurred during the command execution.
        volatile UInt8 p_result[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 args; // Command arguments. The content of the arguments is dependent of the command type.
        volatile UInt8 p_args[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_cmd_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_cmd_regmap)==8));
}

/***************************
 * regmap: adv_asc
 ***************************/
struct PACKED_PRE gp_adv_asc_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 channel_map0; // channel map byte0
            volatile UInt8 channel_map1; // channel map byte1
            volatile UInt8 channel_map2; // channel map byte2
            volatile UInt8 channel_map3; // channel map byte3
            volatile UInt8 channel_map4; // channel map byte4
        } PACKED_POST;
        volatile UInt64 channel_map : 40;
        volatile UInt8 p_channel_map[40/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 channel_map_size; // number of valid channel in the channel map
        volatile UInt8 p_channel_map_size[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ft_ptr; // ptr to transmit for the AUX_ADV_IND PDU (without AuxPtr) to be transmitted as the first packet in a TxEASecScan or TxEASecConn
        volatile UInt8 p_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 own_address : 48; // AdvA When own address is RPA, then prand consists of 3 MSBs
        volatile UInt8 p_own_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool own_address_type : 1; // 0 = public 1 = random address stored in own_address
            volatile Bool generate_rpa_sourceaddr : 1; // 
        } PACKED_POST;
        volatile UInt8 own_address_flags;
        volatile UInt8 p_own_address_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 peer_address_type; // 0 = public 1 = random address
        volatile UInt8 p_peer_address_type[8/8];
    } PACKED_POST;
    UInt8 padding_0[4];
    union PACKED_PRE {
        volatile UInt64 peer_address : 48; // AdvA When own address is RPA, then prand consists of 3 MSBs
        volatile UInt8 p_peer_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 aux_ptr_offset; // indicates the position of the AuxPtr in the pbm
        volatile UInt8 p_aux_ptr_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_adv_ind : 1; // 
            volatile Bool accept_ft_adv_direct_ind : 1; // 
            volatile Bool accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool accept_ft_scan_req : 1; // 
            volatile Bool accept_ft_scan_rsp : 1; // 
            volatile Bool accept_ft_connect_req : 1; // 
            volatile Bool accept_ft_adv_scan_ind : 1; // 
            volatile Bool accept_ft_adv_ext_ind : 1; // 
            volatile Bool accept_ft_aux_connect_rsp : 1; // 
            volatile Bool accept_ft_reserved_9 : 1; // 
            volatile Bool accept_ft_reserved_10 : 1; // 
            volatile Bool accept_ft_reserved_11 : 1; // 
            volatile Bool accept_ft_reserved_12 : 1; // 
            volatile Bool accept_ft_reserved_13 : 1; // 
            volatile Bool accept_ft_reserved_14 : 1; // 
            volatile Bool accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 frame_type_accept_mask;
        volatile UInt8 p_frame_type_accept_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool wl_en_adv_ind : 1; // 
            volatile Bool wl_en_adv_direct_ind : 1; // 
            volatile Bool wl_en_adv_nonconn_ind : 1; // 
            volatile Bool wl_en_scan_req : 1; // 
            volatile Bool wl_en_scan_rsp : 1; // 
            volatile Bool wl_en_connect_req : 1; // 
            volatile Bool wl_en_adv_scan_ind : 1; // 
            volatile Bool wl_en_adv_ext_ind : 1; // 
            volatile Bool wl_en_aux_connect_rsp : 1; // 
            volatile Bool wl_en_reserved_9 : 1; // 
            volatile Bool wl_en_reserved_10 : 1; // 
            volatile Bool wl_en_reserved_11 : 1; // 
            volatile Bool wl_en_reserved_12 : 1; // 
            volatile Bool wl_en_reserved_13 : 1; // 
            volatile Bool wl_en_reserved_14 : 1; // 
            volatile Bool wl_en_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 whitelist_enable_mask;
        volatile UInt8 p_whitelist_enable_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 intra_evt_tx_spacing; // The time in microseconds between 2 consecutive advertising packets in a single (primary) advertising event. Should not exceed 10ms.
        volatile UInt8 p_intra_evt_tx_spacing[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool queue_scan_reqs : 1; // queue AUX_SCAN_REQs
            volatile Bool directed_adv : 1; // 
            volatile Bool scan_req : 1; // Cleared by NRT when starting scannable advertising. Set by RT upon receiving a scan request
            volatile Bool conn_req : 1; // Cleared by NRT when starting connectable advertising. Set by RT upon receiving an AUX_CONNECT_REQ. Never cleared by RT
            volatile Bool rand_order_in_channel_map : 1; // RT does not randomize the Channel Map; 1 = RT does randomize the order of channels in the Channel Map
        } PACKED_POST;
        volatile UInt8 context_flags;
        volatile UInt8 p_context_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 advertising_handle; // used by NRT to identify the advertising set
        volatile UInt8 p_advertising_handle[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_adv_asc_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_adv_asc_regmap)==40));
}

/***************************
 * regmap: adv_ev_info
 ***************************/
struct PACKED_PRE gp_adv_ev_info_regmap
{
    union PACKED_PRE {
        volatile UInt32 guard_time : 24; // Time an event needs to be soft aborted before next event trigger of the same event (in us), should be minumum the event_processing delay
        volatile UInt8 p_guard_time[24/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 intra_ev_tx_spacing; // the time between advertising transmissions (on different channels), has to be a value between 0 and 10 ms. Resolution 1 us
        volatile UInt8 p_intra_ev_tx_spacing[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 adv_delay_mask; // the mask used for generating the random adv delay, the resulting random number has a resolution of 625 us. Set to 15 according to BLE spec
        volatile UInt8 p_adv_delay_mask[8/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt16 adv_ft_ptr; // pbm to transmit as advertising frame
        volatile UInt8 p_adv_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 scan_rsp_ft_ptr; // pbm to transmit as scan response
        volatile UInt8 p_scan_rsp_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 adv_ch0; // channel for first  adv transmission. if 255, advertising event will stop.
            volatile UInt8 adv_ch1; // channel for second adv transmission. if 255, advertising event will stop.
            volatile UInt8 adv_ch2; // channel for third  adv transmission. if 255, advertising event will stop.
        } PACKED_POST;
        volatile UInt32 adv_ch_map : 24;
        volatile UInt8 p_adv_ch_map[24/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_adv_ind : 1; // 
            volatile Bool accept_ft_adv_direct_ind : 1; // 
            volatile Bool accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool accept_ft_scan_req : 1; // 
            volatile Bool accept_ft_scan_rsp : 1; // 
            volatile Bool accept_ft_connect_req : 1; // 
            volatile Bool accept_ft_adv_scan_ind : 1; // 
            volatile Bool accept_ft_adv_ext_ind : 1; // 
            volatile Bool accept_ft_aux_connect_rsp : 1; // 
            volatile Bool accept_ft_reserved_9 : 1; // 
            volatile Bool accept_ft_reserved_10 : 1; // 
            volatile Bool accept_ft_reserved_11 : 1; // 
            volatile Bool accept_ft_reserved_12 : 1; // 
            volatile Bool accept_ft_reserved_13 : 1; // 
            volatile Bool accept_ft_reserved_14 : 1; // 
            volatile Bool accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 frame_type_accept_mask;
        volatile UInt8 p_frame_type_accept_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_adv_ind_wl_en : 1; // 
            volatile Bool ft_adv_direct_ind_wl_en : 1; // 
            volatile Bool ft_adv_nonconn_ind_wl_en : 1; // 
            volatile Bool ft_scan_req_wl_en : 1; // 
            volatile Bool ft_scan_rsp_wl_en : 1; // 
            volatile Bool ft_connect_req_wl_en : 1; // 
            volatile Bool ft_adv_scan_ind_wl_en : 1; // 
            volatile Bool ft_adv_ext_ind_wl_en : 1; // 
            volatile Bool ft_aux_connect_rsp_wl_en : 1; // 
            volatile Bool ft_reserved_9_wl_en : 1; // 
            volatile Bool ft_reserved_10_wl_en : 1; // 
            volatile Bool ft_reserved_11_wl_en : 1; // 
            volatile Bool ft_reserved_12_wl_en : 1; // 
            volatile Bool ft_reserved_13_wl_en : 1; // 
            volatile Bool ft_reserved_14_wl_en : 1; // 
            volatile Bool ft_reserved_15_wl_en : 1; // 
        } PACKED_POST;
        volatile UInt16 ft_whitelist_enable_mask;
        volatile UInt8 p_ft_whitelist_enable_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool rt0_forward_res_pr_src : 1; // when 1, unresolved RPA src addresses will not be dropped
            volatile Bool rt0_forward_res_pr_dst : 1; // when 1, unresolved RPA dst addresses will not be dropped
            volatile Bool accept_unresolved_rpa_src : 1; // when 1, unresolved RPA src addresses will not be dropped
            volatile Bool accept_unresolved_rpa_dst : 1; // when 1, unresolved RPA dst addresses will not be dropped
            volatile Bool accept_unresolved_id_src : 1; // when 1, unresolved identity (non-RPA) src addresses will not be dropped
        } PACKED_POST;
        volatile UInt8 unnamed_reg_adv_ev_info_0x0014;
        volatile UInt8 p_unnamed_reg_adv_ev_info_0x0014[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_settings;
        volatile UInt8 p_coex_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_antenna; // Indicates the antenna currently being used (or next to be used when not during the event)
        volatile UInt8 p_curr_antenna[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_adv_ev_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_adv_ev_info_regmap)==23));
}

/***************************
 * regmap: ant_switch_config
 ***************************/
struct PACKED_PRE gp_ant_switch_config_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 sequence_length; // The length of the switch sequence (max = 74)
            volatile UInt8 slot_duration; // The slot duration
        } PACKED_POST;
        volatile UInt16 unnamed_reg_ant_switch_config_0x0000;
        volatile UInt8 p_unnamed_reg_ant_switch_config_0x0000[16/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slots_0; // The first 2 antenna id (2 antenna ids per byte, 4 lsbs = slot 0)
            volatile UInt8 slots_1; // The next 2 antenna ids
            volatile UInt8 slots_2; // The next 2 antenna ids
            volatile UInt8 slots_3; // The next 2 antenna ids
            volatile UInt8 slots_4; // The next 2 antenna ids
            volatile UInt8 slots_5; // The next 2 antenna ids
            volatile UInt8 slots_6; // The next 2 antenna ids
            volatile UInt8 slots_7; // The next 2 antenna ids
        } PACKED_POST;
        volatile UInt64 ant_switch_slots;
        volatile UInt8 p_ant_switch_slots[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slots_8; // The next 2 antenna ids
            volatile UInt8 slots_9; // The next 2 antenna ids
            volatile UInt8 slots_10; // The next 2 antenna ids
            volatile UInt8 slots_11; // The next 2 antenna ids
            volatile UInt8 slots_12; // The next 2 antenna ids
            volatile UInt8 slots_13; // The next 2 antenna ids
            volatile UInt8 slots_14; // The next 2 antenna ids
            volatile UInt8 slots_15; // The next 2 antenna ids
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ant_switch_config_0x000c;
        volatile UInt8 p_unnamed_reg_ant_switch_config_0x000c[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slots_16; // The next 2 antenna ids
            volatile UInt8 slots_17; // The next 2 antenna ids
            volatile UInt8 slots_18; // The next 2 antenna ids
            volatile UInt8 slots_19; // The next 2 antenna ids
            volatile UInt8 slots_20; // The next 2 antenna ids
            volatile UInt8 slots_21; // The next 2 antenna ids
            volatile UInt8 slots_22; // The next 2 antenna ids
            volatile UInt8 slots_23; // The next 2 antenna ids
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ant_switch_config_0x0014;
        volatile UInt8 p_unnamed_reg_ant_switch_config_0x0014[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slots_24; // The next 2 antenna ids
            volatile UInt8 slots_25; // The next 2 antenna ids
            volatile UInt8 slots_26; // The next 2 antenna ids
            volatile UInt8 slots_27; // The next 2 antenna ids
            volatile UInt8 slots_28; // The next 2 antenna ids
            volatile UInt8 slots_29; // The next 2 antenna ids
            volatile UInt8 slots_30; // The next 2 antenna ids
            volatile UInt8 slots_31; // The next 2 antenna ids
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ant_switch_config_0x001c;
        volatile UInt8 p_unnamed_reg_ant_switch_config_0x001c[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slots_32; // The next 2 antenna ids
            volatile UInt8 slots_33; // The next 2 antenna ids
            volatile UInt8 slots_34; // The next 2 antenna ids
            volatile UInt8 slots_35; // The next 2 antenna ids
            volatile UInt8 slots_36; // The next 2 antenna ids
        } PACKED_POST;
        volatile UInt64 unnamed_reg_ant_switch_config_0x0024 : 40;
        volatile UInt8 p_unnamed_reg_ant_switch_config_0x0024[40/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ant_switch_config_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ant_switch_config_regmap)==41));
}

/***************************
 * regmap: bgsc_ev_info
 ***************************/
struct PACKED_PRE gp_bgsc_ev_info_regmap
{
    union PACKED_PRE {
        volatile UInt32 guard_time : 24; // Time an event needs to be soft aborted before next event trigger of the same event (in us), should be minumum the event_processing delay
        volatile UInt8 p_guard_time[24/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt32 window_duration : 24; // the duration of the RX window. Resolution 16 us
        volatile UInt8 p_window_duration[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 current_ch_map_idx; // current index within the channel map
        volatile UInt8 p_current_ch_map_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 scan_ch0; // channel for first  scan RX window. Always used
            volatile UInt8 scan_ch1; // channel for second scan RX window. if 255, channel is skipped
            volatile UInt8 scan_ch2; // channel for third  scan RX window. if 255, channel is skipped
            volatile UInt8 scan_ch3; // channel for fourth scan RX window. if 255, channel is skipped
            volatile UInt8 scan_ch4; // channel for fifth  scan RX window. if 255, channel is skipped
        } PACKED_POST;
        volatile UInt64 scan_ch_map : 40;
        volatile UInt8 p_scan_ch_map[40/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool enable_scan : 1; // enable legacy and extended scanning
            volatile Bool enable_init : 1; // enable legacy and extended initiating
        } PACKED_POST;
        volatile UInt8 scan_flags;
        volatile UInt8 p_scan_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 scan_ev_info_ptr; // pointer to (legacy) scan_even_info event structure
        volatile UInt8 p_scan_ev_info_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 init_ev_info_ptr; // pointer to (legacy) init_even_info event structure
        volatile UInt8 p_init_ev_info_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_prio : 2; // priority of the coex packet-rx request (if enabled by req_en)
            volatile Bool coex_rx_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_settings;
        volatile UInt8 p_coex_rx_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_wd_prio : 2; // priority of the coex rx-on request (if enabled by req_en)
            volatile Bool coex_rx_wd_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_wd_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_wd_settings;
        volatile UInt8 p_coex_rx_wd_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_antenna; // Indicates the antenna currently being used (or next to be used when not during the event)
        volatile UInt8 p_curr_antenna[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ms_allowed; // indicates if multistandard listening is allowed for this subevent.
        volatile UInt8 p_ms_allowed[8/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt32 elapsed_scan_time : 24; // Internal state of the RT. Indicates the elapsed scan time (in us) during the current event (= effective RX on time).
        volatile UInt8 p_elapsed_scan_time[24/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt32 last_scan_start_time; // Internal state of the RT. Indicates the timestamp when the last scan started.
        volatile UInt8 p_last_scan_start_time[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_bgsc_ev_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_bgsc_ev_info_regmap)==32));
}

/***************************
 * regmap: big_context
 ***************************/
struct PACKED_PRE gp_big_context_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool allocated : 1; // Indicates if this BIG was allocated (done by NRT)
            volatile Bool enabled : 1; // Indicates if this BIG was enabled (done by NRT)
            volatile Bool started : 1; // Indicates if this BIG was started (done by RT)
            volatile Bool transmitter : 1; // Indicates if this BIG acts in the broadcaster/transmitter (1) or receiver role (0)
            volatile Bool new_control_pdu_queued : 1; // Indicates if NRT has queued a new control PDU for this BIG
            volatile Bool encryption_enabled : 1; // Indicates whether encryption is enabled (needed for RT for encryption of BIG control PDUs)
        } PACKED_POST;
        volatile UInt8 state;
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 bis_count; // 
        volatile UInt8 p_bis_count[8/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        volatile UInt32 bis_list; // List of all BISes currently active in this BIG
        volatile UInt8 p_bis_list[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 ec; // 
        volatile UInt8 p_ec[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 iso_interval_us : 24; // The time between two events
        volatile UInt8 p_iso_interval_us[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cssn; // 
        volatile UInt8 p_cssn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 sub_interval_us; // The time between two subevents
        volatile UInt8 p_sub_interval_us[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 bis_spacing_us; // The time between corresponding events of adjacent bises
        volatile UInt8 p_bis_spacing_us[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 worst_case_sca; // 
        volatile UInt8 p_worst_case_sca[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 packing; // 
        volatile UInt8 p_packing[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 framing; // 
        volatile UInt8 p_framing[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 phy; // 
        volatile UInt8 p_phy[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 bn; // 
        volatile UInt8 p_bn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pto; // 
        volatile UInt8 p_pto[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 irc; // 
        volatile UInt8 p_irc[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 nse; // 
        volatile UInt8 p_nse[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 last_scheduled_ec_valid; // 
        volatile UInt8 p_last_scheduled_ec_valid[8/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt32 sync_delay; // 
        volatile UInt8 p_sync_delay[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 channel_map_ptr; // Pointer to the 38 byte hopping remap table
        volatile UInt8 p_channel_map_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 control_pdu_ft_ptr; // 
        volatile UInt8 p_control_pdu_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 last_scheduled_ec; // 
        volatile UInt8 p_last_scheduled_ec[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 event_id; // RT event id (from rt_event regmap) that is driving this BIG
        volatile UInt8 p_event_id[8/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt16 base_crc_init; // BaseCRCInit value from BIGInfo
        volatile UInt8 p_base_crc_init[16/8];
    } PACKED_POST;
    UInt8 padding_3[4];
    union PACKED_PRE {
        volatile UInt64 giv; // Init vector used for constructing the nonce
        volatile UInt8 p_giv[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 gsk; // Session key used for encrypting/decrypting BIS packets (part 1)
        volatile UInt8 p_gsk[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 gsk_1; // Session key used for encrypting/decrypting BIS packets (part 2)
        volatile UInt8 p_gsk_1[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 nonce; // nonce used for encrypting/decrypting BIS packets (part 1)
        volatile UInt8 p_nonce[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 nonce_1; // nonce used for encrypting/decrypting BIS packets (part 2)
        volatile UInt8 p_nonce_1[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_4 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_5 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_big_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_big_context_regmap)==116));
}

/***************************
 * regmap: bis_common_context
 ***************************/
struct PACKED_PRE gp_bis_common_context_regmap
{
    union PACKED_PRE {
        volatile UInt64 pc; // 
        volatile UInt8 p_pc[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 sync_delay; // 
        volatile UInt8 p_sync_delay[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_subevent; // 
        volatile UInt8 p_next_subevent[8/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        volatile UInt32 next_subevent_ts; // 
        volatile UInt8 p_next_subevent_ts[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 last_used_prn; // 
            volatile UInt8 last_used_channel_index; // index of channel in remapping table (not BLE channel index)
        } PACKED_POST;
        volatile UInt32 chan_sel_context : 24;
        volatile UInt8 p_chan_sel_context[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 internal_id; // Serves as index of this BIS in the BIG. This is also the base for BIS_Number (the spec starts counting from 1, so we need to do +1 here to get the BIS_Number)
        volatile UInt8 p_internal_id[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool allocated : 1; // Indicates if this BIG was allocated (done by NRT)
            volatile Bool closed : 1; // Indicates if the current BIS event has been closed
        } PACKED_POST;
        volatile UInt8 state;
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 empty_pdu_queued_for_tx; // Bitmap to cache for which pcs we already have transmitted a padding PDU
        volatile UInt8 p_empty_pdu_queued_for_tx[8/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt32 ts_next_exp_anchor_point; // The timestamp of the next expected anchor point.
        volatile UInt8 p_ts_next_exp_anchor_point[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_bis_common_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_bis_common_context_regmap)==32));
}

/***************************
 * regmap: bis_rx_context
 ***************************/
struct PACKED_PRE gp_bis_rx_context_regmap
{
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_0 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_1 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_rx_pdu_list_size; // Max number of pointers that can be stored on the iso_rx_pdu_list
        volatile UInt8 p_iso_rx_pdu_list_size[8/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt16 iso_rx_pdu_list; // Queue of pointers to iso_rx_descriptors
        volatile UInt8 p_iso_rx_pdu_list[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_bis_rx_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_bis_rx_context_regmap)==16));
}

/***************************
 * regmap: bis_tx_context
 ***************************/
struct PACKED_PRE gp_bis_tx_context_regmap
{
    union PACKED_PRE {
        volatile UInt32 access_address; // For TX, we don't need the full validation context, an access address is sufficient
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_queue_length_tx; // Max number of pointers that can be stored on the iso_tx queue.
        volatile UInt8 p_iso_queue_length_tx[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 iso_queue_tx; // Queue of pointers to iso_tx descriptors
        volatile UInt8 p_iso_queue_tx[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_queue_rd_idx_tx; // Index of next queue item that can be read
            volatile UInt8 iso_queue_wr_idx_tx; // Index of next queue item that can be written
        } PACKED_POST;
        volatile UInt16 unnamed_reg_bis_tx_context_0x0008;
        volatile UInt8 p_unnamed_reg_bis_tx_context_0x0008[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_bis_tx_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_bis_tx_context_regmap)==10));
}

/***************************
 * regmap: ble_format_t
 ***************************/
struct PACKED_PRE gp_ble_format_t_regmap
{
    union PACKED_PRE {
        volatile UInt32 frame_ptr; // Start address of the packet data buffer
        volatile UInt8 p_frame_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 timestamp; // Timestamp at which the frame was sent/received in us
        volatile UInt8 p_timestamp[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_len; // Length of the sent/received frame in bytes
        volatile UInt8 p_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 return_code; // Return/status code
        volatile UInt8 p_return_code[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 tx_power : 6; // Desired Power for transmitted packet
            volatile UInt8 pa_mode : 2; // 
        } PACKED_POST;
        volatile UInt8 tx_power_config;
        volatile UInt8 p_tx_power_config[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 mode_ctrl : 4; // 4 general purpose mode bits that are output to the iob and can be put on pins during the TX phase. This allows for controlling external PA/FEM
            volatile UInt8 ext_antenna : 4; // External antenna to be used
        } PACKED_POST;
        volatile UInt8 external_pa_fem_settings;
        volatile UInt8 p_external_pa_fem_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool antenna : 1; // Internal antenna
            volatile Bool fcs_insert_dis : 1; // If set, no CRC is added in the end of the packet, it allows the SW to send without CRC or to create a custom CRC
            volatile Bool frame_end_with_nibble : 1; // When set to 1, only the first 4 bits of the last byte are transmitted. Feature mainly for HADM.
        } PACKED_POST;
        volatile UInt8 unnamed_reg_ble_format_t_0x000d;
        volatile UInt8 p_unnamed_reg_ble_format_t_0x000d[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 supplemental_len; // Supplemental length in us. Includes Guard and Reference periods
            volatile UInt8 supplemental_mod : 2; // 
        } PACKED_POST;
        volatile UInt16 supplemental_cfg;
        volatile UInt8 p_supplemental_cfg[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool tx_interrupt_rx : 1; // Start tx even when a frame is being received
            volatile Bool tx_if_too_late : 1; // Perform tx even if timestamp has already passed (eg due to calibration taking too long)
            volatile Bool tx_on_timestamp : 1; // Do tx on given timestamp
        } PACKED_POST;
        volatile UInt8 flow_ctrl;
        volatile UInt8 p_flow_ctrl[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pa_power_table_index; // Index to entry in global PA table (used to identify the original tx power setting that was requested)
        volatile UInt8 p_pa_power_table_index[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 related_event; // The nr of the event that is related to this PBM
        volatile UInt8 p_related_event[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ble_format_t_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ble_format_t_regmap)==19));
}

/***************************
 * regmap: ble_res_pr_identity_entry
 ***************************/
struct PACKED_PRE gp_ble_res_pr_identity_entry_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool is_public_peer_address : 1; // Is public device address (cannot be both public and random!). To enable entry matching with mem_part_match.
            volatile Bool is_random_peer_address : 1; // Is random device address (cannot be both public and random!). To enable entry matching with mem_part_match.
            volatile Bool privacy_mode : 1; // 0==network privacy, 1==device privacy
            volatile Bool all_zeros_local_irk : 1; // Indicates whether this device address is valid for scan_rsp frames
            volatile Bool all_zeros_peer_irk : 1; // Indicates whether this device address is valid for scan_rsp frames
            volatile Bool wl_idx_valid : 1; // if 1, wl_idx is valid
        } PACKED_POST;
        volatile UInt8 flags;
        volatile UInt8 p_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 wl_idx; // index of the corresponding entry in the whitelist table
        volatile UInt8 p_wl_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 peer_identity_address : 48; // Indicates whether this device address is valid for adv_direct_ind frames
        volatile UInt8 p_peer_identity_address[48/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ble_res_pr_identity_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ble_res_pr_identity_entry_regmap)==8));
}

/***************************
 * regmap: ble_test_info
 ***************************/
struct PACKED_PRE gp_ble_test_info_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 tx_phy_mode : 2; // the type of long range mode, 125 or 500
            volatile Bool rx_not_tx : 1; // The test scenario, 0 for transmitter test, 1 for receiver test
            volatile UInt8 antenna : 2; // antenna on which to listen/transmit during the test
            volatile Bool whitening_enable : 1; // turns on/off the channel based whitening
            volatile Bool forward_rx_pdus : 1; // Enable/Disable forwarding of received packets to NRT. Use for debug only!!
        } PACKED_POST;
        volatile UInt8 unnamed_reg_ble_test_info_0x0000;
        volatile UInt8 p_unnamed_reg_ble_test_info_0x0000[8/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_1 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_2 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 crc_init : 24; // CRC init value for RX/TX
        volatile UInt8 p_crc_init[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 whitening_init_rev; // if whitening is enabled, this init value will be used. (bit reversed)
        volatile UInt8 p_whitening_init_rev[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 channel; // BLE channel on which to listen/transmit, CAUTION! this is a PHY channel f=(2*phy_channel+2402) MHz
        volatile UInt8 p_channel[8/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        volatile UInt16 tx_packet_interval; // time between 2 transmission starts in us
        volatile UInt8 p_tx_packet_interval[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 tx_packet_count; // number of packets to be transmitted, 0xFFFF means never ending
        volatile UInt8 p_tx_packet_count[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ft_ptr; // The pbm to be transmitted during TX test mode
        volatile UInt8 p_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rx_packet_count_ant0; // number of packets correctly received on antenna0, wrap around, initialize to 0
        volatile UInt8 p_rx_packet_count_ant0[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rx_packet_count_ant1; // number of packets correctly received on antenna1, wrap around, initialize to 0
        volatile UInt8 p_rx_packet_count_ant1[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 raw_phase_measurement_armed; // if set, a single measurement will be taken on the first received packet that includes a CTE.
        volatile UInt8 p_raw_phase_measurement_armed[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ant_switch_ena : 1; // 0 = no antenna switching (for AoA transmitter); 1 = enable antenna switching (for AoD transmitter)
            volatile Bool afifo_enabled : 1; // 0 to enable debug mode (no dma), 1 to enable ant fifo mode (use DMA)
            volatile UInt8 slot_time : 2; // Slot time for AoA/AoD. Can be 1,2,3,4 us
        } PACKED_POST;
        volatile UInt8 direction_finding_config;
        volatile UInt8 p_direction_finding_config[8/8];
    } PACKED_POST;
    UInt8 padding_4[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt32 ant_switch_seq_ptr : 19; // A pointer to the antenna switching sequence (compressed address)
            UInt8 padding_5 : 5;
            volatile UInt8 ant_switch_length; // Length of the antenna switching sequence
        } PACKED_POST;
        volatile UInt32 ant_switch_config;
        volatile UInt8 p_ant_switch_config[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 raw_phase_sample_ptr : 24; // Pointer to buffer for storing raw phase samples (compressed address)
        volatile UInt8 p_raw_phase_sample_ptr[24/8];
    } PACKED_POST;
    UInt8 padding_6[1];
    union PACKED_PRE {
        volatile UInt16 raw_phase_sample_max_words; // Max number of (16bit) words that fit inside the raw phase samples buffer
        volatile UInt8 p_raw_phase_sample_max_words[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool rx_mode_1mb : 1; // rx ( concurrent ) listening modes 1 Mb
            volatile Bool rx_mode_2mb : 1; // rx ( concurrent ) listening modes 2 Mb
            volatile Bool rx_mode_long_range : 1; // rx ( concurrent ) listening modes 125 or 512 Kb
        } PACKED_POST;
        volatile UInt8 rx_allphy_mask;
        volatile UInt8 p_rx_allphy_mask[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ble_test_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ble_test_info_regmap)==47));
}

/***************************
 * regmap: ble_whitelist_entry
 ***************************/
struct PACKED_PRE gp_ble_whitelist_entry_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_adv_ind_valid : 1; // Indicates whether this device address is valid for adv_ind frames
            volatile Bool ft_adv_direct_ind_valid : 1; // Indicates whether this device address is valid for adv_direct_ind frames
            volatile Bool ft_adv_nonconn_ind_valid : 1; // Indicates whether this device address is valid for adv_nonconn_ind frames
            volatile Bool ft_scan_req_valid : 1; // Indicates whether this device address is valid for scan_req frames
            volatile Bool ft_scan_rsp_valid : 1; // Indicates whether this device address is valid for scan_rsp frames
            volatile Bool ft_connect_req_valid : 1; // Indicates whether this device address is valid for connect_req frames
            volatile Bool ft_adv_scan_ind_valid : 1; // Indicates whether this device address is valid for adv_scan_ind frames
            volatile Bool ft_adv_ext_ind_valid : 1; // 
        } PACKED_POST;
        volatile UInt8 valid_frame_type_mask;
        volatile UInt8 p_valid_frame_type_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool s_entry_type : 1; // Indicates whether this entry is regular whitelist entry or controller specific: 0b0 = controller specific, 0b1 = regular entry
            volatile Bool s_advertising_valid : 1; // Indicates whether this device address valid when the BLE manager is in the advertising state
            volatile Bool s_scanning_valid : 1; // Indicates whether this device address valid when the BLE manager is in the scanning state
            volatile Bool s_intiating_valid : 1; // Indicates whether this device address valid when the BLE manager is in the initiating state
            volatile Bool s_persync_valid : 1; // Indicates whether this device address valid when the BLE manager is in the periodic sync state
            Bool padding_0 : 1;
            volatile UInt8 address_type : 2; // 0b01 = (0)public, 0b10 = (1)random, or just trx_add+1
        } PACKED_POST;
        volatile UInt8 valid_state_mask_address_type;
        volatile UInt8 p_valid_state_mask_address_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 device_address : 48; // 6 byte device address
        volatile UInt8 p_device_address[48/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ble_whitelist_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ble_whitelist_entry_regmap)==8));
}

/***************************
 * regmap: cal_entry
 ***************************/
struct PACKED_PRE gp_cal_entry_regmap
{
    union PACKED_PRE {
        volatile UInt8 type; // 
        volatile UInt8 p_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 steps; // 
        volatile UInt8 p_steps[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 status; // 
        volatile UInt8 p_status[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_cal_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_cal_entry_regmap)==3));
}

/***************************
 * regmap: cig_context
 ***************************/
struct PACKED_PRE gp_cig_context_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool allocated : 1; // Indicates if this CIG was allocated (done by NRT)
            volatile Bool enabled : 1; // Indicates if this CIG was enabled (done by NRT)
            volatile Bool started : 1; // Indicates if this CIG was started (done by RT)
            volatile Bool skipped : 1; // Indicates if this CIG needs to skip its next event
        } PACKED_POST;
        volatile UInt8 state;
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 active_cis_count; // How many CISes are currently added by NRT to this CIG
        volatile UInt8 p_active_cis_count[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 active_cis_list; // List of all CISes currently active in this CIG
        volatile UInt8 p_active_cis_list[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 iso_interval_us : 24; // The time between two events (in us)
        volatile UInt8 p_iso_interval_us[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 event_id; // RT event id (from rt_event regmap) that is driving this CIG
        volatile UInt8 p_event_id[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 worst_case_sca; // Worst-case sleep clock accuracy (of all CISes) in this CIG
        volatile UInt8 p_worst_case_sca[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 packing; // How CISes in this CIG are serviced (0 = sequential, 1 = interleaved)
        volatile UInt8 p_packing[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt32 cig_sync_delay; // Time between CIG reference point and CIG anchor point
        volatile UInt8 p_cig_sync_delay[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 next_cig_reference_point; // Next timestamp for this CIG (CIG reference point as per the spec)
        volatile UInt8 p_next_cig_reference_point[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_cig_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_cig_context_regmap)==20));
}

/***************************
 * regmap: cis_admin_config
 ***************************/
struct PACKED_PRE gp_cis_admin_config_regmap
{
    union PACKED_PRE {
        volatile UInt8 cis_execution_list_max_size; // 
        volatile UInt8 p_cis_execution_list_max_size[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cis_execution_list_cur_size; // 
        volatile UInt8 p_cis_execution_list_cur_size[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 cis_execution_list; // 
        volatile UInt8 p_cis_execution_list[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_cis_index; // 
        volatile UInt8 p_next_cis_index[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_rx_desc_list_size; // 
        volatile UInt8 p_iso_rx_desc_list_size[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 iso_rx_desc_list; // 
        volatile UInt8 p_iso_rx_desc_list[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 cis_context_list_size; // Size of CIS context list (e.g. how many CISes can be allocated simultaneously)
        volatile UInt8 p_cis_context_list_size[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 cis_context_list; // List of pointers to CIS contexts
        volatile UInt8 p_cis_context_list[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_rx_desc_rd_idx; // 
            volatile UInt8 iso_rx_desc_wr_idx; // 
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_admin_config_0x000c;
        volatile UInt8 p_unnamed_reg_cis_admin_config_0x000c[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_cis_admin_config_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_cis_admin_config_regmap)==14));
}

/***************************
 * regmap: cis_context
 ***************************/
struct PACKED_PRE gp_cis_context_regmap
{
    union PACKED_PRE {
        volatile UInt64 ec; // 
        volatile UInt8 p_ec[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 cig_ptr; // Pointer to the CIG where this CIS belongs to.
        volatile UInt8 p_cig_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 phy_tx : 2; // 
            volatile UInt8 phy_rx : 2; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_cis_context_0x000a;
        volatile UInt8 p_unnamed_reg_cis_context_0x000a[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt32 sub_interval; // 
        volatile UInt8 p_sub_interval[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 se_length; // 
        volatile UInt8 p_se_length[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 nse; // 
        volatile UInt8 p_nse[8/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt32 cis_sync_delay; // 
        volatile UInt8 p_cis_sync_delay[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_2 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_3 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool allocated : 1; // Indicates if this CIS was allocated (done by NRT)
            volatile Bool registered : 1; // Indicates if this CIS was registered to RT
            volatile Bool ec_reached : 1; // Indicates if this CIS has reached its ACL start event (set by RT)
            volatile Bool started : 1; // Indicates if this CIS was started (done by by RT)
            volatile Bool rf_paused : 1; // Indicates if the air interface activities of this CIS are paused (but the CIS keeps running in the RT)
            volatile Bool central : 1; // Indicates if this CIS belongs to a central or peripheral ACL connection
            volatile Bool can_be_closed : 1; // Indicates if this CIS can be closed before the end of the CIS event
            volatile Bool closed : 1; // Indicates if the current CIS event can be closed
            volatile Bool winwidening_limit_reached : 1; // indicates if window widening on this CIS has reached the limit
        } PACKED_POST;
        volatile UInt16 state;
        volatile UInt8 p_state[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 acl_event_nr; // RT event number for the associated ACL connection
        volatile UInt8 p_acl_event_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool tx_sn : 1; // 
            volatile Bool tx_nesn : 1; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_cis_context_0x0027;
        volatile UInt8 p_unnamed_reg_cis_context_0x0027[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 metrics_update_busy; // Flag indicating that RT is busy updating the metrics, read-only for NRT, rw for RT
        volatile UInt8 p_metrics_update_busy[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 metrics_update_change; // flag set by RT at the end of a metrics update, cleared by NRT before reading metrics and checked by NRT after reading metrics
        volatile UInt8 p_metrics_update_change[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 max_pdu_rx; // max allowed length for ISO Rx PDUs
        volatile UInt8 p_max_pdu_rx[8/8];
    } PACKED_POST;
    UInt8 padding_4[1];
    union PACKED_PRE {
        volatile UInt8 next_subevent; // 
        volatile UInt8 p_next_subevent[8/8];
    } PACKED_POST;
    UInt8 padding_5[3];
    union PACKED_PRE {
        volatile UInt32 next_subevent_ts; // 
        volatile UInt8 p_next_subevent_ts[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 last_used_prn; // 
            volatile UInt8 last_used_channel_index; // index of channel in remapping table (not BLE channel index)
        } PACKED_POST;
        volatile UInt32 chan_sel_context : 24;
        volatile UInt8 p_chan_sel_context[24/8];
    } PACKED_POST;
    UInt8 padding_6[1];
    union PACKED_PRE {
        volatile UInt32 ts_last_packet_rx; // Timestamp of the last received packet.
        volatile UInt8 p_ts_last_packet_rx[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 ts_last_valid_packet_rx; // Timestamp of the last valid received packet.
        volatile UInt8 p_ts_last_valid_packet_rx[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 anchor_point_diff; // (Peripheral only). The (last known) difference between the expected and actual anchor point.
        volatile UInt8 p_anchor_point_diff[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 ts_next_exp_anchor_point; // The timestamp of the next expected anchor point.
        volatile UInt8 p_ts_next_exp_anchor_point[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 current_iso_tx_desc; // The Iso tx descriptor currently being transmitted (zero when a CIS NULL PDU was sent)
        volatile UInt8 p_current_iso_tx_desc[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ch_map_ptr; // Pointer to the 38 byte hopping remap table
        volatile UInt8 p_ch_map_ptr[16/8];
    } PACKED_POST;
    UInt8 padding_7[4];
    union PACKED_PRE {
        volatile UInt64 last_scheduled_ec; // The last EC RT will schedule, before stopping the CIS
        volatile UInt8 p_last_scheduled_ec[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 last_scheduled_ec_valid; // Whether or not the value of last_scheduled_ec is valid
        volatile UInt8 p_last_scheduled_ec_valid[8/8];
    } PACKED_POST;
    UInt8 padding_8[3];
    union PACKED_PRE {
        volatile UInt32 tx_unacked_packets; // counter field used to quantify the link quality
        volatile UInt8 p_tx_unacked_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 tx_flushed_packets; // counter field used to quantify the link quality
        volatile UInt8 p_tx_flushed_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 tx_last_subevent_packets; // counter field used to quantify the link quality
        volatile UInt8 p_tx_last_subevent_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 retransmitted_packets; // counter field used to quantify the link quality
        volatile UInt8 p_retransmitted_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 crc_error_packets; // counter field used to quantify the link quality
        volatile UInt8 p_crc_error_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 rx_unreceived_packets; // counter field used to quantify the link quality
        volatile UInt8 p_rx_unreceived_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 duplicate_packets; // counter field used to quantify the link quality
        volatile UInt8 p_duplicate_packets[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 offset_from_acl_ec_start; // Offset (in us) from start of ACL event with event count acl_ec_start where this CIS should start
        volatile UInt8 p_offset_from_acl_ec_start[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 acl_ec_start; // The event counter of the ACL connection after which this CIS can be started.
        volatile UInt8 p_acl_ec_start[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 nr_of_events_before_start; // The number of CIG events we need to wait before we can start this CIS.
        volatile UInt8 p_nr_of_events_before_start[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 pc_tx; // Current payload counter for iso_tx
        volatile UInt8 p_pc_tx[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 next_flush_event_tx; // CIS event counter where the flush timeout for iso_tx occurs
        volatile UInt8 p_next_flush_event_tx[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_flush_sub_event_tx; // The subevent after which the flush timeout for iso_tx occurs
        volatile UInt8 p_next_flush_sub_event_tx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 initial_flush_subevent_tx; // The first subevent in an event where an FP occurs (this corresponds to 'U' in the spec)
        volatile UInt8 p_initial_flush_subevent_tx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 fp_increment_tx; // The number of subevents to add after the first FP to get to the next FP.
        volatile UInt8 p_fp_increment_tx[8/8];
    } PACKED_POST;
    UInt8 padding_9[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 bn_tx; // 
            volatile UInt8 ft_tx; // 
        } PACKED_POST;
        volatile UInt16 config_tx;
        volatile UInt8 p_config_tx[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 current_payload_ec_offset_tx; // Indicates the offset to the EC where this payload could be processed for the first time
            volatile UInt8 nr_of_pcs_before_ec_upd_tx; // Indicates the number of payloads we need to process before we can update (decrement) current_payload_ec_offset_tx
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_context_0x0096;
        volatile UInt8 p_unnamed_reg_cis_context_0x0096[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_queue_length_tx; // Max number of pointers that can be stored on the iso_tx queue.
        volatile UInt8 p_iso_queue_length_tx[8/8];
    } PACKED_POST;
    UInt8 padding_10[1];
    union PACKED_PRE {
        volatile UInt16 iso_queue_tx; // Queue of pointers to iso_tx descriptors
        volatile UInt8 p_iso_queue_tx[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_queue_rd_idx_tx; // Index of next queue item that can be read
            volatile UInt8 iso_queue_wr_idx_tx; // Index of next queue item that can be written
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_context_0x009c;
        volatile UInt8 p_unnamed_reg_cis_context_0x009c[16/8];
    } PACKED_POST;
    UInt8 padding_11[2];
    union PACKED_PRE {
        volatile UInt64 pc_rx; // Current payload counter for iso_rx
        volatile UInt8 p_pc_rx[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 next_flush_event_rx; // CIS event counter where the flush timeout for iso_rx occurs
        volatile UInt8 p_next_flush_event_rx[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_flush_sub_event_rx; // The subevent after which the flush timeout for iso_rx occurs
        volatile UInt8 p_next_flush_sub_event_rx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 initial_flush_subevent_rx; // The first subevent in an event where an FP occurs (this corresponds to 'U' in the spec)
        volatile UInt8 p_initial_flush_subevent_rx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 fp_increment_rx; // The number of subevents to add after the first FP to get to the next FP.
        volatile UInt8 p_fp_increment_rx[8/8];
    } PACKED_POST;
    UInt8 padding_12[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 bn_rx; // 
            volatile UInt8 ft_rx; // 
        } PACKED_POST;
        volatile UInt16 config_rx;
        volatile UInt8 p_config_rx[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 current_payload_ec_offset_rx; // Indicates the offset to the EC where this payload could be processed for the first time
            volatile UInt8 nr_of_pcs_before_ec_upd_rx; // Indicates the number of payloads we need to process before we can update (decrement) current_payload_ec_offset_rx
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_context_0x00b6;
        volatile UInt8 p_unnamed_reg_cis_context_0x00b6[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_queue_length_rx; // Max number of pointers that can be stored on the iso_rx queue.
        volatile UInt8 p_iso_queue_length_rx[8/8];
    } PACKED_POST;
    UInt8 padding_13[1];
    union PACKED_PRE {
        volatile UInt16 iso_queue_rx; // Queue of pointers to iso_rx descriptors
        volatile UInt8 p_iso_queue_rx[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_queue_rd_idx_rx; // Index of next queue item that can be read
            volatile UInt8 iso_queue_wr_idx_rx; // Index of next queue item that can be written
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_context_0x00bc;
        volatile UInt8 p_unnamed_reg_cis_context_0x00bc[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 padding; // Only used to align cis_context size on multiple of 4. Change when needed
        volatile UInt8 p_padding[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_cis_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_cis_context_regmap)==192));
}

/***************************
 * regmap: cis_direction_context
 ***************************/
struct PACKED_PRE gp_cis_direction_context_regmap
{
    union PACKED_PRE {
        volatile UInt64 pc; // Current payload counter for iso
        volatile UInt8 p_pc[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 next_flush_event; // CIS event counter where the flush timeout for iso occurs
        volatile UInt8 p_next_flush_event[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_flush_sub_event; // The subevent after which the flush timeout for iso occurs
        volatile UInt8 p_next_flush_sub_event[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 initial_flush_subevent; // The first subevent in an event where an FP occurs (this corresponds to 'U' in the spec)
        volatile UInt8 p_initial_flush_subevent[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 fp_increment; // The number of subevents to add after the first FP to get to the next FP.
        volatile UInt8 p_fp_increment[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 bn; // 
            volatile UInt8 ft; // 
        } PACKED_POST;
        volatile UInt16 config;
        volatile UInt8 p_config[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 current_payload_ec_offset; // Indicates the offset to the EC where this payload could be processed for the first time
            volatile UInt8 nr_of_pcs_before_ec_upd; // Indicates the number of payloads we need to process before we can update (decrement) current_payload_ec_offset
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_direction_context_0x0016;
        volatile UInt8 p_unnamed_reg_cis_direction_context_0x0016[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 iso_queue_length; // Max number of pointers that can be stored on the iso queue.
        volatile UInt8 p_iso_queue_length[8/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt16 iso_queue; // Queue of pointers to iso descriptors
        volatile UInt8 p_iso_queue[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_queue_rd_idx; // Index of next queue item that can be read
            volatile UInt8 iso_queue_wr_idx; // Index of next queue item that can be written
        } PACKED_POST;
        volatile UInt16 unnamed_reg_cis_direction_context_0x001c;
        volatile UInt8 p_unnamed_reg_cis_direction_context_0x001c[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_cis_direction_context_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_cis_direction_context_regmap)==30));
}

/***************************
 * regmap: conn_ch_map
 ***************************/
struct PACKED_PRE gp_conn_ch_map_regmap
{
    union PACKED_PRE {
        volatile UInt64 used_ch_idxs : 40; // The channel indexes to be used for the connection event
        volatile UInt8 p_used_ch_idxs[40/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 hop_remap_0; // 
            volatile UInt8 hop_remap_1; // 
            volatile UInt8 hop_remap_2; // 
            volatile UInt8 hop_remap_3; // 
            volatile UInt8 hop_remap_4; // 
            volatile UInt8 hop_remap_5; // 
            volatile UInt8 hop_remap_6; // 
            volatile UInt8 hop_remap_7; // 
        } PACKED_POST;
        volatile UInt64 hop_remap_table;
        volatile UInt8 p_hop_remap_table[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 hop_remap_8; // 
            volatile UInt8 hop_remap_9; // 
            volatile UInt8 hop_remap_10; // 
            volatile UInt8 hop_remap_11; // 
            volatile UInt8 hop_remap_12; // 
            volatile UInt8 hop_remap_13; // 
            volatile UInt8 hop_remap_14; // 
            volatile UInt8 hop_remap_15; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_conn_ch_map_0x0010;
        volatile UInt8 p_unnamed_reg_conn_ch_map_0x0010[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 hop_remap_16; // 
            volatile UInt8 hop_remap_17; // 
            volatile UInt8 hop_remap_18; // 
            volatile UInt8 hop_remap_19; // 
            volatile UInt8 hop_remap_20; // 
            volatile UInt8 hop_remap_21; // 
            volatile UInt8 hop_remap_22; // 
            volatile UInt8 hop_remap_23; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_conn_ch_map_0x0018;
        volatile UInt8 p_unnamed_reg_conn_ch_map_0x0018[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 hop_remap_24; // 
            volatile UInt8 hop_remap_25; // 
            volatile UInt8 hop_remap_26; // 
            volatile UInt8 hop_remap_27; // 
            volatile UInt8 hop_remap_28; // 
            volatile UInt8 hop_remap_29; // 
            volatile UInt8 hop_remap_30; // 
            volatile UInt8 hop_remap_31; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_conn_ch_map_0x0020;
        volatile UInt8 p_unnamed_reg_conn_ch_map_0x0020[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 hop_remap_32; // 
            volatile UInt8 hop_remap_33; // 
            volatile UInt8 hop_remap_34; // 
            volatile UInt8 hop_remap_35; // 
            volatile UInt8 hop_remap_36; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_conn_ch_map_0x0028 : 40;
        volatile UInt8 p_unnamed_reg_conn_ch_map_0x0028[40/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 hop_remap_table_len; // length of the hopping remaping table, used for relaying unmapped channels
        volatile UInt8 p_hop_remap_table_len[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_conn_ch_map_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_conn_ch_map_regmap)==46));
}

/***************************
 * regmap: conn_ev_info
 ***************************/
struct PACKED_PRE gp_conn_ev_info_regmap
{
    union PACKED_PRE {
        volatile UInt32 guard_time : 24; // Time an event needs to be soft aborted before next event trigger of the same event (in us), should be minumum the event_processing delay
        volatile UInt8 p_guard_time[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 hop_increment; // increase to use when jumping to the next channel
        volatile UInt8 p_hop_increment[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ch_map_ptr; // pointer to the 38 byte hopping remap table
        volatile UInt8 p_ch_map_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 tx_phy_mode : 2; // phy mode when transmitting connection packets
            volatile UInt8 rx_phy_mode : 2; // phy mode when receiving connection packets
        } PACKED_POST;
        volatile UInt8 unnamed_reg_conn_ev_info_0x0006;
        volatile UInt8 p_unnamed_reg_conn_ev_info_0x0006[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 combined_sca_worst; // Absolute worst-case combined sleep clock accuracy of master and slave. Note that this field is only used on slave links. NRT should add an extra 5% compensation (this will allow RT to make simpler calculations).
        volatile UInt8 p_combined_sca_worst[16/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_2 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_3 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 crc_init : 24; // crc init for this connection
        volatile UInt8 p_crc_init[24/8];
    } PACKED_POST;
    UInt8 padding_4[1];
    union PACKED_PRE {
        volatile UInt32 window_duration; // duration the slave should listen for first synchronisation with the master
        volatile UInt8 p_window_duration[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 combined_sca; // sleep clock accury of master and slave combined in ppm
        volatile UInt8 p_combined_sca[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 slave_latency; // Number of events that can be skipped due to slave latency
        volatile UInt8 p_slave_latency[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 event_cnt; // the number of connection events passed since the start. Initialize to 0. Wraps around at 0xFFFF
        volatile UInt8 p_event_cnt[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 unmapped_ch_ptr; // The current unmapped channel pointer, initialize to 0
        volatile UInt8 p_unmapped_ch_ptr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_sn; // The sequence number used to identify transmitted packets. Initialize to 0
        volatile UInt8 p_tx_sn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_nesn; // The Next Expected Sequence Number to indicate an ack or not. Initialize to 0
        volatile UInt8 p_tx_nesn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 rx_sn; // The Sequence Number of the the incomming packet. Indicates if this is a retransmit or not. Initialize to 1
        volatile UInt8 p_rx_sn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 rx_nesn; // The Next Expected Sequence Number of the the incomming packet. indicates if we need to resend Tx_pbm or not. Initialize to 0
        volatile UInt8 p_rx_nesn[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool slave_latency_en : 1; // Indicates whether the slave latency should be enabled for the next (few) events. This accounts for the MD bits and acks of the previous packets. Initialize to 0
            volatile Bool slave_latency_wd_too_large : 1; // represents if the next event will be skipped because the current window duration is too large >(interval-guard_time-processing_delay)
            volatile Bool slave_latency_sleep_enable : 1; // Indicates wheter the slave will wakeup each event interval to check for new data in the queue. If this bit is 1, slave_latency events will not be executed at all (chip will not wakeup) and event will be time warped to the next non slave latency event.
            volatile Bool slave_latency_active : 1; // Internal state of the event. indicates that an event is being skipped due to slave latency
            volatile Bool slave_latency_asleep : 1; // State that indicates if an event is asleep. if this is 1 and a frame needs to be sent asap, the event needs to be woken up with a WAKEUP_EVENT ipc command.
        } PACKED_POST;
        volatile UInt8 slave_latency_state;
        volatile UInt8 p_slave_latency_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 current_tx_ptr; // Indicates what the current tx pbm is. This is needed to ensure correct data_cnf signals. Initialize to 255
        volatile UInt8 p_current_tx_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 nr_no_rx_events; // Indicates the number of events there was no rx activity, used for slave latency
        volatile UInt8 p_nr_no_rx_events[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 nr_missed_rx_events; // Indicates the number of events there was no rx activity, used for slave latency
        volatile UInt8 p_nr_missed_rx_events[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 winwidening_limit_reached; // Indicates that RT has reached the window widening spec limit. Written (set/clear) by RT, read by NRT.
        volatile UInt8 p_winwidening_limit_reached[8/8];
    } PACKED_POST;
    UInt8 padding_5[1];
    union PACKED_PRE {
        volatile UInt16 fixed_wd_threshold; // threshold (in nr_missed_rx_events) from where the fixed window duration should be used
        volatile UInt8 p_fixed_wd_threshold[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 last_sch_event_cnt; // The last connection event count to be executed from this connection event, after execution is done the event is disabled by the ble_mgr. This count is only valid when last_sch_event_cnt_valid is set
        volatile UInt8 p_last_sch_event_cnt[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 last_sch_event_cnt_valid; // Enables the last connection event count mechanism
        volatile UInt8 p_last_sch_event_cnt_valid[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 rx_flow_ctrl_flag; // If set, the BLE Event Manager will generate a NACK-condition upon every received PDU. If not set, a normal ack is generated
        volatile UInt8 p_rx_flow_ctrl_flag[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_queue_wr_ptr; // pointer for NRT that indicates where the queue entry in the tx_queue can be written. increase this AFTER write
        volatile UInt8 p_tx_queue_wr_ptr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 tx_queue_rd_ptr; // pointer for RT that indicates where the next queue entry can be read.
        volatile UInt8 p_tx_queue_rd_ptr[8/8];
    } PACKED_POST;
    UInt8 padding_6[4];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 tx_ptr0; // 
            volatile UInt16 tx_ptr1; // 
            volatile UInt16 tx_ptr2; // 
            volatile UInt16 tx_ptr3; // 
        } PACKED_POST;
        volatile UInt64 tx_queue;
        volatile UInt8 p_tx_queue[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 tx_ptr4; // 
            volatile UInt16 tx_ptr5; // 
            volatile UInt16 tx_ptr6; // 
            volatile UInt16 tx_ptr7; // 
        } PACKED_POST;
        volatile UInt64 unnamed_reg_conn_ev_info_0x0048;
        volatile UInt8 p_unnamed_reg_conn_ev_info_0x0048[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_settings;
        volatile UInt8 p_coex_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool metrics_update_busy : 1; // Read only flag that indicates that the ble_mgr is busy updating the metrics
            volatile Bool metrics_update_change : 1; // one shot flag is set by the ble_mgr at the start and end of a metrics update, so is set at a change in the metrics_update_busy flag. NRT may clear this (using atomic bit write!)
        } PACKED_POST;
        volatile UInt8 metrics_update_status;
        volatile UInt8 p_metrics_update_status[8/8];
    } PACKED_POST;
    UInt8 padding_7[2];
    union PACKED_PRE {
        volatile UInt32 t_next_exp_anchor_point; // When the next anchor point is expected
        volatile UInt8 p_t_next_exp_anchor_point[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 t_last_correlation; // Indicates the last timestamp on which a packet was received (with correct crc)
        volatile UInt8 p_t_last_correlation[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 t_last_peer_packet; // Indicates the last timestamp on which a packet was received (irrelevant of it being a correct packet)
        volatile UInt8 p_t_last_peer_packet[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ant_sw_use_crc; // If antenna switching is enabled, crc errors are accounted for in PER
        volatile UInt8 p_ant_sw_use_crc[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ant_sw_use_nack; // If antenna switching is enabled, nack frames are accounted for in PER
        volatile UInt8 p_ant_sw_use_nack[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ant_sw_use_missed; // If antenna switching is enabled, missed frames are accounted for in PER
        volatile UInt8 p_ant_sw_use_missed[8/8];
    } PACKED_POST;
    UInt8 padding_8[1];
    union PACKED_PRE {
        volatile UInt16 ant_sw_threshold; // Threshold on per, on which the antenna has to be switched (fixed point Q1.15)
        volatile UInt8 p_ant_sw_threshold[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ant_sw_iir_factor; // Factor (=2**value) of the iir filter (that filters the per)
        volatile UInt8 p_ant_sw_iir_factor[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ant_sw_nr_backoff; // Nr of connection events before per_other_ant is reset
        volatile UInt8 p_ant_sw_nr_backoff[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_antenna; // Indicates the antenna currently being used (or next to be used when not during the event)
        volatile UInt8 p_curr_antenna[8/8];
    } PACKED_POST;
    UInt8 padding_9[1];
    union PACKED_PRE {
        volatile UInt16 per_curr_ant; // PER of the antenna currently being used (fixed point Q1.15)
        volatile UInt8 p_per_curr_ant[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 per_other_ant; // PER of the antenna currently not in use (fixed point Q1.15)
        volatile UInt8 p_per_other_ant[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_backoff_cnt; // Reflects the remaining nr of connection events before per_other_ant is reset
        volatile UInt8 p_curr_backoff_cnt[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 limit_to_1_cycle; // if set, a connection event will be limited to 1 (req and response) cycle, DEBUG feature, only implemented for connection master
        volatile UInt8 p_limit_to_1_cycle[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt32 ant_switch_seq_ptr_rx : 19; // A pointer to the antenna switching sequence (compressed address)
            UInt8 padding_10 : 5;
            volatile UInt8 ant_switch_length_rx; // Length of the antenna switching sequence
        } PACKED_POST;
        volatile UInt32 ant_switch_config_rx;
        volatile UInt8 p_ant_switch_config_rx[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt32 ant_switch_seq_ptr_tx : 19; // A pointer to the antenna switching sequence (compressed address)
            UInt8 padding_11 : 5;
            volatile UInt8 ant_switch_length_tx; // Length of the antenna switching sequence
        } PACKED_POST;
        volatile UInt32 ant_switch_config_tx;
        volatile UInt8 p_ant_switch_config_tx[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ant_switch_ena_rx : 1; // 0 = no antenna switching (for AoD receiver); 1 = enable antenna switching (for AoA receiver)
            volatile Bool afifo_enabled_rx : 1; // 0 to enable debug mode (no dma), 1 to enable ant fifo mode (use DMA)
            volatile UInt8 slot_time_rx : 2; // Slot time for AoA/AoD. Can be 1,2,3,4 us
        } PACKED_POST;
        volatile UInt8 direction_finding_config_rx;
        volatile UInt8 p_direction_finding_config_rx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ant_switch_ena_tx : 1; // 0 = no antenna switching (for AoD receiver); 1 = enable antenna switching (for AoA receiver)
            volatile Bool afifo_enabled_tx : 1; // 0 to enable debug mode (no dma), 1 to enable ant fifo mode (use DMA)
            volatile UInt8 slot_time_tx : 2; // Slot time for AoA/AoD. Can be 1,2,3,4 us
        } PACKED_POST;
        volatile UInt8 direction_finding_config_tx;
        volatile UInt8 p_direction_finding_config_tx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 raw_phase_sample_max_words; // Max number of (16bit) words that fit inside the raw phase samples buffer
        volatile UInt8 p_raw_phase_sample_max_words[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 raw_phase_sample_ptr : 24; // Pointer to buffer for storing raw phase samples (compressed address)
        volatile UInt8 p_raw_phase_sample_ptr[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 raw_phase_measurement_armed; // if set, a single measurement will be taken on the first received packet that includes a CTE.
        volatile UInt8 p_raw_phase_measurement_armed[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 use_chan_sel_algo2; // Enable channel selection algorithm #2
        volatile UInt8 p_use_chan_sel_algo2[8/8];
    } PACKED_POST;
    UInt8 padding_12[3];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 event_cnt_last_rx; // The connection event counter where the last packet was received from the remote
            volatile UInt16 event_cnt_prev_rx_diff; // The number of connection events that have been elapsed since the previous correlation point.
        } PACKED_POST;
        volatile UInt32 unnamed_reg_conn_ev_info_0x0084;
        volatile UInt8 p_unnamed_reg_conn_ev_info_0x0084[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 t_anchor_offset; // The offset between the actual anchor point and the expected anchor point
        volatile UInt8 p_t_anchor_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 rx_window_shift_ppm; // If nonzero, indicates a shift of the rx window around the anchor point (expressed in ppm). NRT should add 5% margin to this (makes it easier for RT to work with).
        volatile UInt8 p_rx_window_shift_ppm[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 t_anchor_point_last_rx_event; // The anchor point of the connection event where the last packet was received
        volatile UInt8 p_t_anchor_point_last_rx_event[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_conn_ev_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_conn_ev_info_regmap)==144));
}

/***************************
 * regmap: ev_list_entry
 ***************************/
struct PACKED_PRE gp_ev_list_entry_regmap
{
    union PACKED_PRE {
        volatile UInt32 execution_time; // 
        volatile UInt8 p_execution_time[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 event_type; // 
        volatile UInt8 p_event_type[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 info_ptr; // 
        volatile UInt8 p_info_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 next_ptr; // points to the next entry in the list
        volatile UInt8 p_next_ptr[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_ev_list_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_ev_list_entry_regmap)==10));
}

/***************************
 * regmap: format_r
 ***************************/
struct PACKED_PRE gp_format_r_regmap
{
    union PACKED_PRE {
        volatile UInt32 timestamp; // Timestamp at which the frame was sent/received in us
        volatile UInt8 p_timestamp[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 frame_ptr; // Start address of the packet data buffer
        volatile UInt8 p_frame_ptr[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_len; // Length of the sent/received frame in bytes
        volatile UInt8 p_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 return_code; // Return/status code
        volatile UInt8 p_return_code[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 drop_reason : 5; // Reason why frame was dropped. Should always be E_NO_DROP, except in sniffing mode where this will indicate if the frame is ok or not
            volatile Bool ack_info_valid : 1; // ack frame info below is valid
        } PACKED_POST;
        volatile UInt8 unnamed_reg_format_r_0x000b;
        volatile UInt8 p_unnamed_reg_format_r_0x000b[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 slot_idx : 3; // Time Slot on which the frame was received. Corresponds to the channel idx when all slots use different channel
            volatile Bool antenna : 1; // Antenna on which packet is being received
            volatile Bool crc_ok : 1; // Indicates whether the received packet has a valid crc or not
            volatile Bool cte_present : 1; // Indicates whether the packet contains a CTE info field
            volatile Bool phy_drop : 1; // Indicates if the packet was dropped by the PHY
            volatile Bool attenuated : 1; // Indicates whether attenuation was enabled during rx of the frame
            volatile UInt8 phy_mode : 3; // Indicates if it is a ZB or BLE (HDR, LR,...) packet
            volatile Bool exp_mode : 1; // Indicates whether the packet was received in experimental mode or not
            volatile Bool invalid_level_trigger : 1; // indicates if the RX filter has set a level trigger further than the packet length. (mostly point to a corrupt header)
            UInt8 padding_0 : 3;
            volatile UInt8 rf_gain_state : 3; // RF gain at preamble detect
            Bool padding_1 : 1;
            volatile UInt8 bb_gain_state : 3; // BB gain at preamble detect
        } PACKED_POST;
        volatile UInt32 rx_status : 24;
        volatile UInt8 p_rx_status[24/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 foffset; // 
            volatile UInt8 lqi; // Quality of the link during the frame reception
            volatile UInt8 rssi; // Signal strength of the received frame
        } PACKED_POST;
        volatile UInt32 measurements;
        volatile UInt8 p_measurements[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 channel : 6; // Channel number of the received frame
            UInt8 padding_3 : 2;
            volatile UInt8 channel_idx : 3; // Channel bank the packet was received on
            UInt8 padding_4 : 5;
            volatile UInt8 matching_ch_idx_mask : 6; // Indicates what indexes where configured to use the same channel as the channel this packet is received on
        } PACKED_POST;
        volatile UInt32 channel_info : 24;
        volatile UInt8 p_channel_info[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 related_event; // The nr of the event that is related to this PBM, only used for ble!
        volatile UInt8 p_related_event[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 wl_idx; // whitelist entry index that was used for packet filtering
        volatile UInt8 p_wl_idx[8/8];
    } PACKED_POST;
    UInt8 padding_5[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 irk_idx; // Index in both local and peer IRK resolving lists, 0xFF is invalid
            volatile Bool irk_idx_is_valid : 1; // irk_idx is valid, i.e. peer is in resolving list
            volatile Bool src_is_resolved : 1; // src is RPA and was resolved
            volatile Bool target_resolved : 1; // Target RPA was resolved flag
        } PACKED_POST;
        volatile UInt16 ble_res_pr;
        volatile UInt8 p_ble_res_pr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 event_context; // Contains the AdvASC, ScanASC index associated with the sED during which this packet was received
        volatile UInt8 p_event_context[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 event_subtype; // Contains the sED EventType during which this packet was received
        volatile UInt8 p_event_subtype[8/8];
    } PACKED_POST;
    UInt8 padding_6[2];
    union PACKED_PRE {
        volatile UInt32 raw_phase_sample_ptr : 24; // Pointer to buffer with raw phase samples (= offset added to SM_linear start address)
        volatile UInt8 p_raw_phase_sample_ptr[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool subfilt_0_accept : 1; // Subfilter 0 accepted the pdu
            volatile Bool subfilt_1_accept : 1; // Subfilter 1 accepted the pdu
        } PACKED_POST;
        volatile UInt8 subfilt_accept;
        volatile UInt8 p_subfilt_accept[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 event_counter; // Event counter when this pbm is received
        volatile UInt8 p_event_counter[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 ack_frame_type : 3; // Frame Type of the transmitted ACK frame
            volatile Bool ack_sec_enabled : 1; // Security enabled or not on the transmitted ACK frame
            volatile Bool ack_fp : 1; // Frame Pending but for the transmitted ACK frame
            volatile Bool ack_ar : 1; // AR for the transmitted ACK frame
            volatile Bool ack_panid_compression : 1; // panid compression for the transmitted ACK frame
            Bool padding_7 : 1;
            volatile Bool ack_seq_nr_suppression : 1; // seq nr suppression for the transmitted ACK frame
            volatile Bool ack_ie_present : 1; // Set if ie elements are present in the transmitted ACK frame
            volatile UInt8 ack_dest_addr_mode : 2; // Destination address mode for the transmitted ACK frame
            volatile UInt8 ack_frame_version : 2; // Frame version for the transmitted ACK frame
            volatile UInt8 ack_source_addr_mode : 2; // Source address mode for the transmitted ACK frame
        } PACKED_POST;
        volatile UInt16 ack_frame_ctrl;
        volatile UInt8 p_ack_frame_ctrl[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ack_keyid; // keyid from the ack frame that was transmitted
        volatile UInt8 p_ack_keyid[8/8];
    } PACKED_POST;
    UInt8 padding_8[3];
    union PACKED_PRE {
        volatile UInt32 ack_framecounter; // framecounter of the ack frame that was transmitted
        volatile UInt8 p_ack_framecounter[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_format_r_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_format_r_regmap)==48));
}

/***************************
 * regmap: init_ev_info
 ***************************/
struct PACKED_PRE gp_init_ev_info_regmap
{
    union PACKED_PRE {
        volatile UInt32 guard_time : 24; // Time an event needs to be soft aborted before next event trigger of the same event (in us), should be minumum the event_processing delay
        volatile UInt8 p_guard_time[24/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt32 interval : 24; // The time between two events (in us)
        volatile UInt8 p_interval[24/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt32 init_window_duration : 24; // the duration of the RX window. Resolution 1 us
        volatile UInt8 p_init_window_duration[24/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt16 conn_req_ft_ptr; // pbm to transmit as connection request frame
        volatile UInt8 p_conn_req_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 current_ch_map_idx; // current index within the channel map
        volatile UInt8 p_current_ch_map_idx[8/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 init_ch0; // channel for first  initiating RX window. Always used
            volatile UInt8 init_ch1; // channel for second initiating RX window. Always used
            volatile UInt8 init_ch2; // channel for third  initiating RX window. Always used
        } PACKED_POST;
        volatile UInt32 init_ch_map : 24;
        volatile UInt8 p_init_ch_map[24/8];
    } PACKED_POST;
    UInt8 padding_4[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_adv_ind : 1; // 
            volatile Bool accept_ft_adv_direct_ind : 1; // 
            volatile Bool accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool accept_ft_scan_req : 1; // 
            volatile Bool accept_ft_scan_rsp : 1; // 
            volatile Bool accept_ft_connect_req : 1; // 
            volatile Bool accept_ft_adv_scan_ind : 1; // 
            volatile Bool accept_ft_adv_ext_ind : 1; // 
            volatile Bool accept_ft_aux_connect_rsp : 1; // 
            volatile Bool accept_ft_reserved_9 : 1; // 
            volatile Bool accept_ft_reserved_10 : 1; // 
            volatile Bool accept_ft_reserved_11 : 1; // 
            volatile Bool accept_ft_reserved_12 : 1; // 
            volatile Bool accept_ft_reserved_13 : 1; // 
            volatile Bool accept_ft_reserved_14 : 1; // 
            volatile Bool accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 frame_type_accept_mask;
        volatile UInt8 p_frame_type_accept_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_adv_ind_wl_en : 1; // 
            volatile Bool ft_adv_direct_ind_wl_en : 1; // 
            volatile Bool ft_adv_nonconn_ind_wl_en : 1; // 
            volatile Bool ft_scan_req_wl_en : 1; // 
            volatile Bool ft_scan_rsp_wl_en : 1; // 
            volatile Bool ft_connect_req_wl_en : 1; // 
            volatile Bool ft_adv_scan_ind_wl_en : 1; // 
            volatile Bool ft_adv_ext_ind_wl_en : 1; // 
            volatile Bool ft_aux_connect_rsp_wl_en : 1; // 
            volatile Bool ft_reserved_9_wl_en : 1; // 
            volatile Bool ft_reserved_10_wl_en : 1; // 
            volatile Bool ft_reserved_11_wl_en : 1; // 
            volatile Bool ft_reserved_12_wl_en : 1; // 
            volatile Bool ft_reserved_13_wl_en : 1; // 
            volatile Bool ft_reserved_14_wl_en : 1; // 
            volatile Bool ft_reserved_15_wl_en : 1; // 
        } PACKED_POST;
        volatile UInt16 ft_whitelist_enable_mask;
        volatile UInt8 p_ft_whitelist_enable_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 own_device_address : 48; // The address to check against for directed advertising packets
        volatile UInt8 p_own_device_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool own_device_address_type : 1; // The address_type to check against for directed advertising packets
            volatile Bool generate_res_pr : 1; // generate resolvable private source address for outgoing packets
            volatile Bool chan_sel_bit_set_by_advertiser : 1; // Indicates if the received ADV PDU has the ChSel bit set or not
            volatile Bool accept_unresolved_rpa_src : 1; // when 1, unresolved RPA src addresses will not be dropped
            volatile Bool accept_unresolved_id_src : 1; // when 1, unresolved identity (non-RPA) src addresses will not be dropped
        } PACKED_POST;
        volatile UInt8 unnamed_reg_init_ev_info_0x001e;
        volatile UInt8 p_unnamed_reg_init_ev_info_0x001e[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 virt_ev_nr; // Pointer to the virtual event to be used for connection parameters. When this is value is invalid (greater than 15), the window offset is not calculated and not written to the frame
        volatile UInt8 p_virt_ev_nr[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 conn_dst_address : 48; // Holds the address of the device a connection was just initiated with
        volatile UInt8 p_conn_dst_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_tx_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_tx_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_tx_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_tx_settings;
        volatile UInt8 p_coex_tx_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_rx_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_settings;
        volatile UInt8 p_coex_rx_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_wd_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_rx_wd_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_wd_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_wd_settings;
        volatile UInt8 p_coex_rx_wd_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_antenna; // Indicates the antenna currently being used (or next to be used when not during the event)
        volatile UInt8 p_curr_antenna[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ms_allowed; // indicates if multistandard listening is allowed for this subevent.
        volatile UInt8 p_ms_allowed[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_init_ev_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_init_ev_info_regmap)==43));
}

/***************************
 * regmap: init_ext_info
 ***************************/
struct PACKED_PRE gp_init_ext_info_regmap
{
    union PACKED_PRE {
        volatile UInt8 ble_hdr_virt_ev_nr; // Pointer to the virtual event to be used for connection parameters. When this is value is invalid (greater than 15), the window offset is not calculated and not written to the frame
        volatile UInt8 p_ble_hdr_virt_ev_nr[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 ble_hdr_lldata_latency; // Latency to be copied to the LLData field of the connect_req_pdu
            volatile UInt16 ble_hdr_lldata_timeout; // Timeout to be copied to the LLData field of the connect_req_pdu
        } PACKED_POST;
        volatile UInt32 ble_hdr_lldata;
        volatile UInt8 p_ble_hdr_lldata[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ble_lr_virt_ev_nr; // Pointer to the virtual event to be used for connection parameters. When this is value is invalid (greater than 15), the window offset is not calculated and not written to the frame
        volatile UInt8 p_ble_lr_virt_ev_nr[8/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 ble_lr_lldata_latency; // Latency to be copied to the LLData field of the connect_req_pdu
            volatile UInt16 ble_lr_lldata_timeout; // Timeout to be copied to the LLData field of the connect_req_pdu
        } PACKED_POST;
        volatile UInt32 ble_lr_lldata;
        volatile UInt8 p_ble_lr_lldata[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool pri_one : 1; // 1Mbit
            volatile Bool pri_rfu : 1; // reserved for future use
            volatile Bool pri_lr : 1; // long range
        } PACKED_POST;
        volatile UInt8 pri_phy_mask;
        volatile UInt8 p_pri_phy_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool sec_one : 1; // 1Mbit
            volatile Bool sec_two : 1; // 2Mbit
            volatile Bool sec_lr : 1; // long range
        } PACKED_POST;
        volatile UInt8 sec_phy_mask;
        volatile UInt8 p_sec_phy_mask[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_init_ext_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_init_ext_info_regmap)==14));
}

/***************************
 * regmap: init_ext_virt_conn_info
 ***************************/
struct PACKED_PRE gp_init_ext_virt_conn_info_regmap
{
    union PACKED_PRE {
        volatile UInt8 virt_ev_nr; // Pointer to the virtual event to be used for connection parameters. When this is value is invalid (greater than 15), the window offset is not calculated and not written to the frame
        volatile UInt8 p_virt_ev_nr[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 lldata_latency; // Latency to be copied to the LLData field of the connect_req_pdu
            volatile UInt16 lldata_timeout; // Timeout to be copied to the LLData field of the connect_req_pdu
        } PACKED_POST;
        volatile UInt32 lldata;
        volatile UInt8 p_lldata[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_init_ext_virt_conn_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_init_ext_virt_conn_info_regmap)==6));
}

/***************************
 * regmap: iso_queue
 ***************************/
struct PACKED_PRE gp_iso_queue_regmap
{
    union PACKED_PRE {
        volatile UInt8 iso_queue_length; // Max number of pointers that can be stored on the iso queue.
        volatile UInt8 p_iso_queue_length[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 iso_queue; // Queue of pointers to iso descriptors
        volatile UInt8 p_iso_queue[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 iso_queue_rd_idx; // Index of next queue item that can be read
            volatile UInt8 iso_queue_wr_idx; // Index of next queue item that can be written
        } PACKED_POST;
        volatile UInt16 unnamed_reg_iso_queue_0x0004;
        volatile UInt8 p_unnamed_reg_iso_queue_0x0004[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_iso_queue_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_iso_queue_regmap)==6));
}

/***************************
 * regmap: iso_rx_descriptor
 ***************************/
struct PACKED_PRE gp_iso_rx_descriptor_regmap
{
    union PACKED_PRE {
        volatile UInt64 pc; // CIS payload counter of the associated PDU
        volatile UInt8 p_pc[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool claimed : 1; // Whether the descriptor is claimed or not
            volatile Bool nothing_received : 1; // Whether actual data was received or not (Null PDUs)
        } PACKED_POST;
        volatile UInt8 state;
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        volatile UInt32 cis_reference_anchor_point; // Anchor point of the earliest CIS where the PDU could have been received
        volatile UInt8 p_cis_reference_anchor_point[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 fr_ptr; // Format_r pointer to packet options
        volatile UInt8 p_fr_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 cis_ptr; // Pointer to CIS context
        volatile UInt8 p_cis_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 my_padding_0; // Padding (for alignment) - remove when needed
        volatile UInt8 p_my_padding_0[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_iso_rx_descriptor_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_iso_rx_descriptor_regmap)==24));
}

/***************************
 * regmap: iso_tx_descriptor
 ***************************/
struct PACKED_PRE gp_iso_tx_descriptor_regmap
{
    union PACKED_PRE {
        volatile UInt64 pc; // CIS payload counter of the associated PDU
        volatile UInt8 p_pc[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool claimed : 1; // Whether the descriptor is claimed or not
            volatile Bool transmitted : 1; // Whether the PDU in this descriptor is already transmitted at least once or not.
            volatile Bool can_be_freed : 1; // Whether the descriptor can be freed by NRT or not.
        } PACKED_POST;
        volatile UInt8 state;
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 isoal_ref; // ISOAL reference - transparent to RT
        volatile UInt8 p_isoal_ref[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 ft_ptr; // Format_t pointer to packet options
        volatile UInt8 p_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 final_segment_count; // Num final/completed (framed) ISOAL segments in PDU
        volatile UInt8 p_final_segment_count[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 pad_0; // Dummy byte (only for padding)
        volatile UInt8 p_pad_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pad_1; // Dummy bytes (only for padding)
        volatile UInt8 p_pad_1[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_iso_tx_descriptor_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_iso_tx_descriptor_regmap)==16));
}

/***************************
 * regmap: long_src_entry
 ***************************/
struct PACKED_PRE gp_long_src_entry_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool channel_idx_0_valid : 1; // Indicates whether this long src address entry is valid for channel index 0
            volatile Bool channel_idx_1_valid : 1; // Indicates whether this long src address entry is valid for channel index 1
            volatile Bool channel_idx_2_valid : 1; // Indicates whether this long src address entry is valid for channel index 2
            volatile Bool channel_idx_3_valid : 1; // Indicates whether this long src address entry is valid for channel index 3
            volatile Bool channel_idx_4_valid : 1; // Indicates whether this long src address entry is valid for channel index 4
            volatile Bool channel_idx_5_valid : 1; // Indicates whether this long src address entry is valid for channel index 5
        } PACKED_POST;
        volatile UInt8 channel_idx_valid_mask;
        volatile UInt8 p_channel_idx_valid_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 valid; // Indicates whether this long src address is valid
        volatile UInt8 p_valid[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id; // The pan id for this long src address entry
        volatile UInt8 p_pan_id[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 address; // the long address for this long src address entry
        volatile UInt8 p_address[64/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_long_src_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_long_src_entry_regmap)==12));
}

/***************************
 * regmap: pdl_entry
 ***************************/
struct PACKED_PRE gp_pdl_entry_regmap
{
    union PACKED_PRE {
        volatile UInt8 subev_dsc_idx; // 
        volatile UInt8 p_subev_dsc_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 next_free_idx; // PDL index of the next available PDL entry - 0xFF is the invalid index
        volatile UInt8 p_next_free_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 forward_idx; // PDL index of the next prescheduled entry - 0xFF is the invalid index
        volatile UInt8 p_forward_idx[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_pdl_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_pdl_entry_regmap)==3));
}

/***************************
 * regmap: per_asc
 ***************************/
struct PACKED_PRE gp_per_asc_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 adv_access_address; // The access address for periodic advertising packets.
        volatile UInt8 p_adv_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 adv_crc_init : 24; // The crc init for the periodic advertising channels
        volatile UInt8 p_adv_crc_init[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ch_map_idx; // index to the channelmap and remap table
        volatile UInt8 p_ch_map_idx[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_per_asc_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_per_asc_regmap)==12));
}

/***************************
 * regmap: pre_rx_entry
 ***************************/
struct PACKED_PRE gp_pre_rx_entry_regmap
{
    union PACKED_PRE {
        volatile UInt16 address; // address to be written
        volatile UInt8 p_address[16/8];
    } PACKED_POST;
    UInt8 padding_0[2];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 value_ble; // value to write if the RX mode is BLE
            volatile UInt8 value_ble_hdr; // value to write if the RX mode is BLE_HDR
            volatile UInt8 value_ble_lr; // value to write if the RX mode is BLE_LR
            volatile UInt8 value_reserved; // 
        } PACKED_POST;
        volatile UInt32 unnamed_reg_pre_rx_entry_0x0004;
        volatile UInt8 p_unnamed_reg_pre_rx_entry_0x0004[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_pre_rx_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_pre_rx_entry_regmap)==8));
}

/***************************
 * regmap: rt_event
 ***************************/
struct PACKED_PRE gp_rt_event_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 interval_us; // Event interval expressed in microseconds.
        volatile UInt8 p_interval_us[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ttb_handle; // Reference to underlying TTB index (NRT should initialize to invalid)
        volatile UInt8 p_ttb_handle[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 type; // 
        volatile UInt8 p_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 info_ptr; // Pointer to event-specific info structure
        volatile UInt8 p_info_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool cleanup : 1; // Indicates if the event is in the cleanup or event state.
            volatile Bool update_pending : 1; // Set to 1 in case there is an anchor-point / window widening udpate pending, else set to 0.
        } PACKED_POST;
        volatile UInt8 unnamed_reg_rt_event_0x000c;
        volatile UInt8 p_unnamed_reg_rt_event_0x000c[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 cleanup_time; // Latched version of the cleanup time to make sure we updating the execution time consistently
        volatile UInt8 p_cleanup_time[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 correction; // signed correction to the next anchor-point (not taking intou account window widening)
        volatile UInt8 p_correction[16/8];
    } PACKED_POST;
    UInt8 padding_1[2];
    union PACKED_PRE {
        volatile UInt32 last_rx_timestamp; // timestamp of the last synchronization point (not necessarily provided by a service running on this event)
        volatile UInt8 p_last_rx_timestamp[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_event_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_event_regmap)==24));
}

/***************************
 * regmap: rt_rci_entry
 ***************************/
struct PACKED_PRE gp_rt_rci_entry_regmap
{
    union PACKED_PRE {
        volatile UInt16 format_ptr; // pointer (offset from RAM_LINEAR_START) to the format containing the options
        volatile UInt8 p_format_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 next_ptr; // pointer (offset from RAM_LINEAR_START) to the next element in the linked list
        volatile UInt8 p_next_ptr[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_rci_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_rci_entry_regmap)==4));
}

/***************************
 * regmap: rt_rec_event
 ***************************/
struct PACKED_PRE gp_rt_rec_event_regmap
{
    union PACKED_PRE {
        volatile UInt8 type; // Event Type field
        volatile UInt8 p_type[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 amount; // number of recurrences. decreases with every recurrence. 0xFFFF is keep running forever.
        volatile UInt8 p_amount[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 interval; // pointer to the further event info
        volatile UInt8 p_interval[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_rt_rec_event_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_rt_rec_event_regmap)==8));
}

/***************************
 * regmap: scan_asc
 ***************************/
struct PACKED_PRE gp_scan_asc_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool in_use : 1; // free or not , set by RT when claimed, cleared by NRT when released
            volatile Bool halted : 1; // by NRT, this flag is cleared by RT and set by NRT
            volatile Bool valid_adi : 1; // by RT, this flag is used by RT only - ADI valid for comparrison
            volatile Bool prim_phy_active_scan : 1; // set/read by RT: indicates that primary phy is an active scan phy
        } PACKED_POST;
        volatile UInt8 flags;
        volatile UInt8 p_flags[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 irk_idx; // when a peer address is resolved write the found index in the scan context for later use (use case: AdvA and TargetA not in the same pdu)
        volatile UInt8 p_irk_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 adi; // ADI as received in ADV_EXT_IND or AUX_ADV_IND
        volatile UInt8 p_adi[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_scan_asc_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_scan_asc_regmap)==8));
}

/***************************
 * regmap: scan_ev_info
 ***************************/
struct PACKED_PRE gp_scan_ev_info_regmap
{
    union PACKED_PRE {
        volatile UInt32 guard_time : 24; // Time an event needs to be soft aborted before next event trigger of the same event (in us), should be minumum the event_processing delay
        volatile UInt8 p_guard_time[24/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt32 interval : 24; // The time between two events (in us)
        volatile UInt8 p_interval[24/8];
    } PACKED_POST;
    UInt8 padding_1[1];
    union PACKED_PRE {
        volatile UInt32 scan_window_duration : 24; // the duration of the RX window. Resolution 16 us
        volatile UInt8 p_scan_window_duration[24/8];
    } PACKED_POST;
    UInt8 padding_2[1];
    union PACKED_PRE {
        volatile UInt16 scan_req_ft_ptr; // pbm to transmit in response to a scan-able advertisement
        volatile UInt8 p_scan_req_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 current_ch_map_idx; // current index within the channel map
        volatile UInt8 p_current_ch_map_idx[8/8];
    } PACKED_POST;
    UInt8 padding_3[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 scan_ch0; // channel for first  scan RX window. Always used
            volatile UInt8 scan_ch1; // channel for second scan RX window. if 255, channel is skipped
            volatile UInt8 scan_ch2; // channel for third  scan RX window. if 255, channel is skipped
        } PACKED_POST;
        volatile UInt32 scan_ch_map : 24;
        volatile UInt8 p_scan_ch_map[24/8];
    } PACKED_POST;
    UInt8 padding_4[1];
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool accept_ft_adv_ind : 1; // 
            volatile Bool accept_ft_adv_direct_ind : 1; // 
            volatile Bool accept_ft_adv_nonconn_ind : 1; // 
            volatile Bool accept_ft_scan_req : 1; // 
            volatile Bool accept_ft_scan_rsp : 1; // 
            volatile Bool accept_ft_connect_req : 1; // 
            volatile Bool accept_ft_adv_scan_ind : 1; // 
            volatile Bool accept_ft_adv_ext_ind : 1; // 
            volatile Bool accept_ft_aux_connect_rsp : 1; // 
            volatile Bool accept_ft_reserved_9 : 1; // 
            volatile Bool accept_ft_reserved_10 : 1; // 
            volatile Bool accept_ft_reserved_11 : 1; // 
            volatile Bool accept_ft_reserved_12 : 1; // 
            volatile Bool accept_ft_reserved_13 : 1; // 
            volatile Bool accept_ft_reserved_14 : 1; // 
            volatile Bool accept_ft_reserved_15 : 1; // 
        } PACKED_POST;
        volatile UInt16 frame_type_accept_mask;
        volatile UInt8 p_frame_type_accept_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ft_adv_ind_wl_en : 1; // 
            volatile Bool ft_adv_direct_ind_wl_en : 1; // 
            volatile Bool ft_adv_nonconn_ind_wl_en : 1; // 
            volatile Bool ft_scan_req_wl_en : 1; // 
            volatile Bool ft_scan_rsp_wl_en : 1; // 
            volatile Bool ft_connect_req_wl_en : 1; // 
            volatile Bool ft_adv_scan_ind_wl_en : 1; // 
            volatile Bool ft_adv_ext_ind_wl_en : 1; // 
            volatile Bool ft_aux_connect_rsp_wl_en : 1; // 
            volatile Bool ft_reserved_9_wl_en : 1; // 
            volatile Bool ft_reserved_10_wl_en : 1; // 
            volatile Bool ft_reserved_11_wl_en : 1; // 
            volatile Bool ft_reserved_12_wl_en : 1; // 
            volatile Bool ft_reserved_13_wl_en : 1; // 
            volatile Bool ft_reserved_14_wl_en : 1; // 
            volatile Bool ft_reserved_15_wl_en : 1; // 
        } PACKED_POST;
        volatile UInt16 ft_whitelist_enable_mask;
        volatile UInt8 p_ft_whitelist_enable_mask[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt64 own_device_address : 48; // The address to check against for directed advertising packets
        volatile UInt8 p_own_device_address[48/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool own_device_address_type : 1; // The address_type to check against for directed advertising packets
            volatile Bool generate_res_pr : 1; // generate resolvable private source address for outgoing packets
        } PACKED_POST;
        volatile UInt8 unnamed_reg_scan_ev_info_0x001e;
        volatile UInt8 p_unnamed_reg_scan_ev_info_0x001e[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool rt0_forward_res_pr_src : 1; // when 1, unresolved RPA src addresses will not be dropped
            volatile Bool rt0_forward_res_pr_dst : 1; // when 1, unresolved RPA dst addresses will not be dropped
            volatile Bool active_scanning : 1; // when 1, an active scan will be executed and the scan_req_ft_ptr may be sent out
            volatile Bool accept_unresolved_rpa_src : 1; // when 1, unresolved RPA src addresses will not be dropped
            volatile Bool accept_unresolved_rpa_dst : 1; // when 1, unresolved RPA dst addresses will not be dropped (filter policy 0x02|0x03)
            volatile Bool accept_unresolved_id_src : 1; // when 1, unresolved identity (non-RPA) src addresses will not be dropped
            volatile Bool active_scanning_lr : 1; // when 1, an active scan will be executed on coded phy and the scan_req_pbm may be sent out
        } PACKED_POST;
        volatile UInt8 extra_options;
        volatile UInt8 p_extra_options[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 backoff_cnt; // backoff count used by ble_ev_mgr to know when to send a SCAN_REQ. Only used for active scanning. Initialize to 0
        volatile UInt8 p_backoff_cnt[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 upper_limit_mask; // upper limit, used by ble_ev_mgr to generate a new backoff count. Only used for active scanning. Initialize to 0
        volatile UInt8 p_upper_limit_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 success_cnt; // number off consecutive successes upon receiving a SCAN_RSP. Only used for active scanning. Initialize to 0
        volatile UInt8 p_success_cnt[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 failure_cnt; // number off consecutive failures upon receiving a SCAN_RSP. Only used for active scanning. Initialize to 0
        volatile UInt8 p_failure_cnt[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_tx_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_tx_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_tx_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_tx_settings;
        volatile UInt8 p_coex_tx_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_rx_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_settings;
        volatile UInt8 p_coex_rx_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_rx_wd_prio : 2; // priority of the coex request (if enabled by req_en)
            volatile Bool coex_rx_wd_req_en : 1; // if 1, a coex request will be launched upon radio action
            volatile Bool coex_rx_wd_grant_aware : 1; // If 1, the PA will be disabled if no coex grant is received
        } PACKED_POST;
        volatile UInt8 coex_rx_wd_settings;
        volatile UInt8 p_coex_rx_wd_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 curr_antenna; // Indicates the antenna currently being used (or next to be used when not during the event)
        volatile UInt8 p_curr_antenna[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ms_allowed; // indicates if multistandard listening is allowed for this subevent
        volatile UInt8 p_ms_allowed[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_scan_ev_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_scan_ev_info_regmap)==41));
}

/***************************
 * regmap: scan_ext_info
 ***************************/
struct PACKED_PRE gp_scan_ext_info_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool pri_one : 1; // 1Mbit
            volatile Bool pri_rfu : 1; // reserved for future use
            volatile Bool pri_lr : 1; // long range
        } PACKED_POST;
        volatile UInt8 pri_phy_mask;
        volatile UInt8 p_pri_phy_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool sec_one : 1; // 1Mbit
            volatile Bool sec_two : 1; // 2Mbit
            volatile Bool sec_lr : 1; // long range
        } PACKED_POST;
        volatile UInt8 sec_phy_mask;
        volatile UInt8 p_sec_phy_mask[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_scan_ext_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_scan_ext_info_regmap)==2));
}

/***************************
 * regmap: short_src_entry
 ***************************/
struct PACKED_PRE gp_short_src_entry_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool channel_idx_0_valid : 1; // Indicates wheter this short src address entry is valid for channel index 0
            volatile Bool channel_idx_1_valid : 1; // Indicates wheter this short src address entry is valid for channel index 1
            volatile Bool channel_idx_2_valid : 1; // Indicates wheter this short src address entry is valid for channel index 2
            volatile Bool channel_idx_3_valid : 1; // Indicates wheter this short src address entry is valid for channel index 3
            volatile Bool channel_idx_4_valid : 1; // Indicates wheter this short src address entry is valid for channel index 4
            volatile Bool channel_idx_5_valid : 1; // Indicates wheter this short src address entry is valid for channel index 5
        } PACKED_POST;
        volatile UInt8 channel_idx_valid_mask;
        volatile UInt8 p_channel_idx_valid_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 valid; // Indicates wheter this short src address is valid
        volatile UInt8 p_valid[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 pan_id; // The pan id for this short src address entry
        volatile UInt8 p_pan_id[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 address; // the short address for this short src address entry
        volatile UInt8 p_address[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_short_src_entry_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_short_src_entry_regmap)==6));
}

/***************************
 * regmap: subev_dsc
 ***************************/
struct PACKED_PRE gp_subev_dsc_regmap
{
    union PACKED_PRE {
        volatile UInt8 event_type; // 
        volatile UInt8 p_event_type[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 channel; // ble_channel to use for the subevent
        volatile UInt8 p_channel[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 tx_phy : 2; // Tx phy mode to use for the subevent
            volatile UInt8 rx_phy : 2; // Rx phy mode to use for the subevent (no coding specified)
            volatile Bool coding_follow_remote : 1; // Overrule, when set and rx PHY was coded, use the same coding for TX
            volatile Bool ms_allowed : 1; // Indicates if multistandard listening is allowed for this subevent
        } PACKED_POST;
        volatile UInt8 phy_options;
        volatile UInt8 p_phy_options[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 context_idx; // for TX ADV_ASC for RX SCAN_ASC
        volatile UInt8 p_context_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 execution_time; // Execution time (1us base)
        volatile UInt8 p_execution_time[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt32 rx_wd_size; // RX SEDs only: receive window size
        } PACKED_POST;
        volatile UInt32 unnamed_reg_subev_dsc_0x0008;
        volatile UInt8 p_unnamed_reg_subev_dsc_0x0008[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 drop_reason; // 
        volatile UInt8 p_drop_reason[8/8];
    } PACKED_POST;
    UInt8 padding_0[1];
    union PACKED_PRE {
        volatile UInt16 ft_ptr; // TX SEDs only: tx ptr
        volatile UInt8 p_ft_ptr[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 event_count; // 
        volatile UInt8 p_event_count[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 duration; // Time (1us base)
        volatile UInt8 p_duration[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool current_antenna : 1; // 
            volatile Bool event_valid : 1; // 
            volatile Bool event_done : 1; // 
            volatile Bool event_dropped : 1; // 
            volatile Bool scan_requested : 1; // 
            volatile Bool connection_requested : 1; // 
            volatile Bool exec_conditional : 1; // 
            volatile Bool unknown_adva : 1; // 
            volatile Bool update_aux_ptr : 1; // 
            volatile Bool nothing_received : 1; // 
            volatile Bool primary_phy_was_coded : 1; // 
        } PACKED_POST;
        volatile UInt16 flags;
        volatile UInt8 p_flags[16/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_subev_dsc_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_subev_dsc_regmap)==22));
}

/***************************
 * regmap: sync_c
 ***************************/
struct PACKED_PRE gp_sync_c_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 adv_crc_init : 24; // CRC init value for the periodic advertising channels.
        volatile UInt8 p_adv_crc_init[24/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ch_map_idx; // Index to the channel map and remap table.
        volatile UInt8 p_ch_map_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 validation_thresh; // Threshold for BLE preamble validation
            volatile UInt8 validation_start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_0 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_1 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_0 : 3; // BLE LP validation address 0
            volatile Bool validation_sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 validation_addr_1 : 3; // BLE LP validation address 1
            volatile Bool validation_sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_2 : 3; // BLE LP validation address 2
            volatile Bool validation_sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 validation_addr_3 : 3; // BLE LP validation address 3
            volatile Bool validation_sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_4 : 3; // BLE LP validation address 4
            volatile Bool validation_sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 validation_addr_5 : 3; // BLE LP validation address 5
            volatile Bool validation_sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 validation_addr_6 : 3; // BLE LP validation address 6
            volatile Bool validation_sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 halted; // The halted_by_nrt flag.
        volatile UInt8 p_halted[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_sync_c_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_sync_c_regmap)==21));
}

/***************************
 * regmap: validation_rx
 ***************************/
struct PACKED_PRE gp_validation_rx_regmap
{
    union PACKED_PRE {
        volatile UInt32 access_address; // Access address for this activity
        volatile UInt8 p_access_address[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt16 thresh; // Threshold for BLE preamble validation
            volatile UInt8 start_idx : 5; // Amount of symbols inside the access code the validation is going to start
            UInt8 padding_0 : 3;
            volatile UInt8 fake_preamble_start_idx : 5; // The index inside the access code where a fake preamble starts
            UInt8 padding_1 : 2;
            volatile Bool fake_preamble_present : 1; // Indicates wheter a fake preamble is present in the access code or not (valid flag for fake_preamble_start_idx)
        } PACKED_POST;
        volatile UInt32 validation_settings;
        volatile UInt8 p_validation_settings[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 addr_0 : 3; // BLE LP validation address 0
            volatile Bool sign_0 : 1; // BLE LP validation sign 0
            volatile UInt8 addr_1 : 3; // BLE LP validation address 1
            volatile Bool sign_1 : 1; // BLE LP validation sign 1
        } PACKED_POST;
        volatile UInt8 validation_settings_0;
        volatile UInt8 p_validation_settings_0[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 addr_2 : 3; // BLE LP validation address 2
            volatile Bool sign_2 : 1; // BLE LP validation sign 2
            volatile UInt8 addr_3 : 3; // BLE LP validation address 3
            volatile Bool sign_3 : 1; // BLE LP validation sign 3
        } PACKED_POST;
        volatile UInt8 validation_settings_1;
        volatile UInt8 p_validation_settings_1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 addr_4 : 3; // BLE LP validation address 4
            volatile Bool sign_4 : 1; // BLE LP validation sign 4
            volatile UInt8 addr_5 : 3; // BLE LP validation address 5
            volatile Bool sign_5 : 1; // BLE LP validation sign 5
        } PACKED_POST;
        volatile UInt8 validation_settings_2;
        volatile UInt8 p_validation_settings_2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 addr_6 : 3; // BLE LP validation address 6
            volatile Bool sign_6 : 1; // BLE LP validation sign 6
        } PACKED_POST;
        volatile UInt8 validation_settings_3;
        volatile UInt8 p_validation_settings_3[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_validation_rx_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_validation_rx_regmap)==12));
}

/***************************
 * regmap: zb_format_t
 ***************************/
struct PACKED_PRE gp_zb_format_t_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 timestamp; // Timestamp at which the frame was sent/received in us
        volatile UInt8 p_timestamp[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_len; // Length of the frame data in bytes
        volatile UInt8 p_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 return_code; // Return/status code
        volatile UInt8 p_return_code[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 tx_power : 6; // Desired Power for transmitted packet
            volatile UInt8 pa_mode : 2; // 
        } PACKED_POST;
        volatile UInt8 tx_power_config;
        volatile UInt8 p_tx_power_config[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 mode_ctrl : 4; // 4 general purpose mode bits that can be put on pins during the TX phase. This allows for controlling external PA/FEM
            volatile UInt8 ext_antenna : 4; // External antenna to be used
        } PACKED_POST;
        volatile UInt8 external_pa_fem_settings;
        volatile UInt8 p_external_pa_fem_settings[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 antenna; // Internal antenna
        volatile UInt8 p_antenna[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_offset; // Offset to the frame data in the packet data buffer
        volatile UInt8 p_frame_offset[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 frame_buffer_start; // Start address of the packet data buffer
        volatile UInt8 p_frame_buffer_start[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_buffer_size; // Size of the packet data buffer in bytes
        volatile UInt8 p_frame_buffer_size[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ed_cs_detect; // Indicates if during the ED scan the carrier senses circuit triggered (meaning that modulated carrier is present)
        volatile UInt8 p_ed_cs_detect[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ed_result; // Holds the result of an ED scan
        volatile UInt8 p_ed_result[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 retry_cnt : 4; // The number of reties that were done, (so in total, retry_cnt+1 transmits are done)
            volatile UInt8 cca_cnt : 4; // The number of CCA-s that were done for transmitting the packet - Only valid when csma-cca is enabled on the packet. If csma-cca is disabled, this field needs to be ignored as it will hold the value of the previous csma-cca run
        } PACKED_POST;
        volatile UInt8 flow_status;
        volatile UInt8 p_flow_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool ack_fp : 1; // Value of the Frame Pending bit of the ack
            volatile Bool ack_antenna : 1; // Antenna the ack was received on
        } PACKED_POST;
        volatile UInt8 ack_status;
        volatile UInt8 p_ack_status[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ack_rssi; // The rssi of the ACK received on this transmit, only valid when ack_request and an ack is received
        volatile UInt8 p_ack_rssi[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 channel_idx : 3; // Indicates to which stack, the pbm belongs. In a multichannel environment this will determine what channel in the channel list will be changed during pre and post channel change
            volatile Bool channel_ch0a_en : 1; // Before each packet transmission (including retransmission), the MAC will check the status of this bits in the sequence CH0A_EN (first transmission), CH0B_EN (first retransmission), CH0C_EN (second retransmission), CH0A_EN (third retransmission) etc. If the bit being analyzed is set, the PHY channel will be set to the corresponding value (MM_CHANNEL_CH0[A/B/C])
            volatile Bool channel_ch0b_en : 1; // see channel_ch0a_en
            volatile Bool channel_ch0c_en : 1; // see channel_ch0a_en
            volatile Bool channel_ch1_en : 1; // If this bit is set, at the end of the transmission process the PHY channel will be set to the corresponding value
        } PACKED_POST;
        volatile UInt8 channel_flow;
        volatile UInt8 p_channel_flow[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool csma_ca_enable : 1; // When set, enable CSMA-CA algorithm for the TX, if disabled, CCA is skipped
            volatile UInt8 max_csma_backoffs : 3; // Max backoffs to be performed - only valid when csma_ca mode enabled
            volatile Bool first_boff_is_0 : 1; // The length of first backoff slot will be forced to zero.
            volatile Bool treat_csma_fail_as_no_ack : 1; // When set, a CCA failure will be treated as a no-ACK failure and use the same retry mechanism as if no ACK has been received, the failure will be reported as a no-ACK.
            volatile Bool use_tx_antenna_for_ed_and_cca : 1; // When set to ED scans and cca measurements will be done on the antenna specified by antenna. Otherwise the global antenna diversity setting is used to either toggle the antenna or use the rx antenna.
            volatile Bool retransmit_on_cca_fail : 1; // When set, a CCA failure will be treated as a no-ACK failure and use the same retry mechanism as if no ACK has been received, the failure will be reported as a cca-fail.
            volatile UInt8 min_be : 4; // Min BE setting for the CSMA CA cycle - only valid when csma_ca mode enabled
            volatile UInt8 max_be : 4; // Min BE setting for the CSMA CA cycle - only valid when csma_ca mode enabled
        } PACKED_POST;
        volatile UInt16 csma_options;
        volatile UInt8 p_csma_options[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 channel_ch0a : 4; // see channel_ch0a_en
            volatile UInt8 channel_ch0b : 4; // see channel_ch0a_en
            volatile UInt8 channel_ch0c : 4; // see channel_ch0a_en
            volatile UInt8 channel_ch1 : 4; // channel_ch1_en
        } PACKED_POST;
        volatile UInt16 channels;
        volatile UInt8 p_channels[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 coex_packet_tx_prio : 2; // If CoEx (Coexistence interface) is enabled, this indicates which is the COEX priority of this request
            volatile Bool coex_packet_tx_req_en : 1; // If CoEx (Coexistence interface) is enabled, this indicates if this request should request COEX or not
            Bool padding_0 : 1;
            volatile UInt8 coex_ack_rx_prio : 2; // If CoEx (Coexistence interface) is enabled, this indicates the priority of the received ACK, only valid if on transmit and ack_request is set
            volatile Bool coex_ack_rx_req_en : 1; // If CoEx (Coexistence interface) is enabled, this indicates if the ACK should request COEX or not, only valid if on transmit and ack_request is set
            volatile Bool coex_grant_aware : 1; // If CoEx (Coexistence interface) is enabled, this indicates if the transmission allows enabled RIB COEX no-grant setting to be applied (f.i. disable PA, delayed start,...)
        } PACKED_POST;
        volatile UInt8 coex_options;
        volatile UInt8 p_coex_options[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 state; // Current state of the ZB state machine running on RT
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool skip_cal_tx : 1; // When set to the SX frequency and dac calibration will be skipped. Not advised to do this though ...
            volatile Bool acked_mode : 1; // When set, transmission will request an ACK
            volatile Bool rx_wd_enable : 1; // When set, the value in rx_duration will be taken into account.
            volatile Bool rx_wd_start_on_fp : 1; // The optional RX listening window will be started only if the expected ACK has the pending bit set
            volatile Bool rx_wd_stop_on_pbm_full : 1; // The RX window is stopped when the PBM is full (from the TRC point of view this means it is not granted any PBM entry after reception of the last packet)
            volatile Bool ed_scan : 1; // If set, request is an ED scan, if false, it is a transmission
            volatile UInt8 num_channel_slots : 2; // The number of active channel slots, should be a number between 1 and 3, used for the retransmissions to determine which channel_change setting to use
            volatile UInt8 max_frame_retries : 4; // Max number of retries when  - always valid
            volatile UInt8 confirm_queue : 2; // Selection of which data confirm queue will be used for confirm the request (0, 1, 2 or 3)
            volatile UInt8 attenuator_sel : 2; // Selects the attenuator mode to use (only relevant for ED scan)
        } PACKED_POST;
        volatile UInt16 flow_ctrl;
        volatile UInt8 p_flow_ctrl[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool fcs_insert_dis : 1; // If set, no CRC is added in the end of the packet, it allows the SW to send without CRC or to create a custom CRC
            volatile Bool whitening_enable : 1; // If set the PHY payload will be whitened with the ZB PN9 sequence
            volatile Bool whitening_late : 1; // If set the whitening will happen after FEC encoding and interleaving
            Bool padding_1 : 1;
            volatile UInt8 phy_mode : 2; // 
        } PACKED_POST;
        volatile UInt8 unnamed_reg_zb_format_t_0x0024;
        volatile UInt8 p_unnamed_reg_zb_format_t_0x0024[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 preamble_len; // length of the preamble in case of signify phy
        volatile UInt8 p_preamble_len[8/8];
    } PACKED_POST;
    UInt8 padding_2[2];
    union PACKED_PRE {
        volatile UInt32 rx_wd_duration; // If rx_wd_enable=True, number of us the receiver will be enabled after a successful packet transmission (if a packet needed to be transmitted). If transmission was not succesfull force_rx_on_after_tx_fail has to be set to use this feature. In case of an ED scan this is has to be interpreted as the scan length. If it is equal to zero a single ED will be performed.
        volatile UInt8 p_rx_wd_duration[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile Bool enc_enable : 1; // When set, the frame will be encrypted by the RT using the raw mode settings.
            volatile Bool enc_keep_framecounter : 1; // When set, the framecounter will not be updated.
            volatile Bool enc_done : 1; // To be set to 0. Will be set to 1 by the RT when encryption is completed.
            volatile Bool ack_sec : 1; // To be set to 0. Will be set to 1 by the RT if a secured ACK frame is received.
            volatile UInt8 ack_frame_vers : 2; // To be set to 0. Will be set to the frame version of the ACK by the RT.
        } PACKED_POST;
        volatile UInt8 raw_mode_cfg;
        volatile UInt8 p_raw_mode_cfg[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 ack_frame_vers_mask; // To be set to 0. Will be set to the frame version mask (!) of the ACK by the RT.
        volatile UInt8 p_ack_frame_vers_mask[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 aux_offset; // Offset of the Aux header in the frame to be used by the RT to fill in the framecounter.
        volatile UInt8 p_aux_offset[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 encr_offset; // Offset of the data which is to be encrypted inside the packet.
        volatile UInt8 p_encr_offset[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 csl_ie_offset; // Offset of the csl ie which needs to be filled at transmit time.
        volatile UInt8 p_csl_ie_offset[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zb_format_t_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zb_format_t_regmap)==49));
}

/***************************
 * regmap: zb_format_timed_rx
 ***************************/
struct PACKED_PRE gp_zb_format_timed_rx_regmap
{
    union PACKED_PRE {
        struct PACKED_PRE {
            volatile UInt8 priority; // 0 is lowest priority; 255 is highest priority
            volatile Bool suspend_event : 1; // When enabled, the event will not do any radio activity and the event will be skipped. The event state will still be updated though.
            volatile Bool extended_prio_en : 1; // When 1, the round robin mechanism will also take into account the nr_consec_skipped_events for this event
            volatile UInt8 rfu : 6; // Reserved for future use
            volatile UInt16 nr_consec_skipped_events; // Indicates the number of events that are skipped in a row
        } PACKED_POST;
        volatile UInt32 priority_fields;
        volatile UInt8 p_priority_fields[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 timestamp; // Timestamp at which the frame was sent/received in us
        volatile UInt8 p_timestamp[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt16 frame_len; // Length of the frame data in bytes
        volatile UInt8 p_frame_len[16/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 return_code; // Return/status code
        volatile UInt8 p_return_code[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 channel_idx; // Indicates to which stack, the pbm belongs. In a multichannel environment this will determine what channel in the channel list will be changed during pre and post channel change
        volatile UInt8 p_channel_idx[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 state; // Current state of the ZB state machine running on RT
        volatile UInt8 p_state[8/8];
    } PACKED_POST;
    UInt8 padding_0[3];
    union PACKED_PRE {
        volatile UInt32 rx_wd_starttime; // absolute system time at which the rx window will be started.
        volatile UInt8 p_rx_wd_starttime[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 rx_wd_duration; // If rx_wd_enable=True, number of us the receiver will be enabled after a successful packet transmission (if a packet needed to be transmitted). If transmission was not succesfull force_rx_on_after_tx_fail has to be set to use this feature. In case of an ED scan this is has to be interpreted as the scan length. If it is equal to zero a single ED will be performed.
        volatile UInt8 p_rx_wd_duration[32/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zb_format_timed_rx_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zb_format_timed_rx_regmap)==24));
}

/***************************
 * regmap: zb_rawmode_key_info
 ***************************/
struct PACKED_PRE gp_zb_rawmode_key_info_regmap
{
    union PACKED_PRE {
        volatile UInt8 currkey; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey1; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey1[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey2; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey2[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey3; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey3[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey4; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey4[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey5; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey5[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey6; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey6[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey7; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey7[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey8; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey8[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey9; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey9[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey10; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey10[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey11; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey11[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey12; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey12[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey13; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey13[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey14; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey14[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 currkey15; // 8 bits of the current key to be used in raw mode for tx frames
        volatile UInt8 p_currkey15[8/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 keyid; // key index corresponding to the current key to be used in raw mode for tx frames
        volatile UInt8 p_keyid[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zb_rawmode_key_info_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zb_rawmode_key_info_regmap)==17));
}

/***************************
 * regmap: zb_rawmode_nonce
 ***************************/
struct PACKED_PRE gp_zb_rawmode_nonce_regmap
{
    union PACKED_PRE {
        volatile UInt64 macaddress; // macaddress to be used in the nonce for the tx encryption
        volatile UInt8 p_macaddress[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt32 framecounter; // framecounter to be used in raw mode for tx frames
        volatile UInt8 p_framecounter[32/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 seclevel; // seclevel to be used in the nonce for the tx encryption
        volatile UInt8 p_seclevel[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zb_rawmode_nonce_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zb_rawmode_nonce_regmap)==13));
}

/***************************
 * regmap: zb_rawmode_vsie
 ***************************/
struct PACKED_PRE gp_zb_rawmode_vsie_regmap
{
    union PACKED_PRE {
        volatile UInt64 vsiedata; // content of the VS IE which needs to be added to EnhAck frames.
        volatile UInt8 p_vsiedata[64/8];
    } PACKED_POST;
    union PACKED_PRE {
        volatile UInt8 vsielen; // length of the VS IE which needs to be added to EnhAck frames. Use 0 to disable.
        volatile UInt8 p_vsielen[8/8];
    } PACKED_POST;
} PACKED_POST;

// Sanity check
ALWAYS_INLINE void gp_zb_rawmode_vsie_regmap_size_check(void) {
    COMPILE_TIME_ASSERT((sizeof(struct gp_zb_rawmode_vsie_regmap)==9));
}

#endif //GP_WB_RAM_TYPES_H
