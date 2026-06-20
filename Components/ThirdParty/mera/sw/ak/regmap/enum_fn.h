#ifndef GPHAL_AK_ENUM_H
#define GPHAL_AK_ENUM_H

/***************************
 * Global types layout: ak_general_enum
 ***************************/
/* ak_standby_state  */
const char * ak_standby_state2str(unsigned int n);

/* ak_ssp_mode  */
const char * ak_ssp_mode2str(unsigned int n);

/* ak_ssp_key_len  */
const char * ak_ssp_key_len2str(unsigned int n);

/* ak_watchdog_function  */
const char * ak_watchdog_function2str(unsigned int n);

/* ak_parity  */
const char * ak_parity2str(unsigned int n);

/* ak_generic_io_drive  */
const char * ak_generic_io_drive2str(unsigned int n);

/* ak_phy_state_transition  */
const char * ak_phy_state_transition2str(unsigned int n);

/* ak_dma_scb_trigger_src_select  */
const char * ak_dma_scb_trigger_src_select2str(unsigned int n);

/* ak_dma_scb_word_mode  */
const char * ak_dma_scb_word_mode2str(unsigned int n);

/* ak_dma_scb_buffer_complete_mode  */
const char * ak_dma_scb_buffer_complete_mode2str(unsigned int n);

/* ak_dma_dcb_word_mode  */
const char * ak_dma_dcb_word_mode2str(unsigned int n);

/* ak_antsel_int  */
const char * ak_antsel_int2str(unsigned int n);

/* ak_antsel_ext  */
const char * ak_antsel_ext2str(unsigned int n);

/* ak_pa_slope  */
const char * ak_pa_slope2str(unsigned int n);

/* ak_circular_buffer  */
const char * ak_circular_buffer2str(unsigned int n);

/* ak_coex_sw_overrule_type  */
const char * ak_coex_sw_overrule_type2str(unsigned int n);

/* ak_rxnb_rfcalgen_mod_clk_freq  */
const char * ak_rxnb_rfcalgen_mod_clk_freq2str(unsigned int n);

/* ak_ipc_action  */
const char * ak_ipc_action2str(unsigned int n);

/* ak_obm_state  */
const char * ak_obm_state2str(unsigned int n);


/***************************
 * Global types layout: ak_phy_enum
 ***************************/
/* ak_lff_int_coupling_factor  */
const char * ak_lff_int_coupling_factor2str(unsigned int n);

/* ak_phy_state  */
const char * ak_phy_state2str(unsigned int n);

/* ak_rssi_offset_sel  */
const char * ak_rssi_offset_sel2str(unsigned int n);

/* ak_multi_std_mode  */
const char * ak_multi_std_mode2str(unsigned int n);

/* ak_receiver_mode  */
const char * ak_receiver_mode2str(unsigned int n);

/* ak_zb_receiver_mode  */
const char * ak_zb_receiver_mode2str(unsigned int n);

/* ak_ble_receiver_mode  */
const char * ak_ble_receiver_mode2str(unsigned int n);

/* ak_phy_mode SPONG ... this one should be replacing the transmitter_mode types */
const char * ak_phy_mode2str(unsigned int n);

/* ak_transmitter_mode  */
const char * ak_transmitter_mode2str(unsigned int n);

/* ak_ble_transmitter_mode  */
const char * ak_ble_transmitter_mode2str(unsigned int n);

/* ak_ble_phy_mode  */
const char * ak_ble_phy_mode2str(unsigned int n);

/* ak_crc_mode  */
const char * ak_crc_mode2str(unsigned int n);

/* ak_fe_period  */
const char * ak_fe_period2str(unsigned int n);

/* ak_fe_sampler  */
const char * ak_fe_sampler2str(unsigned int n);

/* ak_sx_state  */
const char * ak_sx_state2str(unsigned int n);

/* ak_pll_state  */
const char * ak_pll_state2str(unsigned int n);

/* ak_est_trigger_mode  */
const char * ak_est_trigger_mode2str(unsigned int n);

/* ak_slot_start_select  */
const char * ak_slot_start_select2str(unsigned int n);

/* ak_integrator_duration  */
const char * ak_integrator_duration2str(unsigned int n);

/* ak_supplemental_mod  */
const char * ak_supplemental_mod2str(unsigned int n);

/* ak_phy_sequencer_triggers  */
const char * ak_phy_sequencer_triggers2str(unsigned int n);

/* ak_attenuator_sel  */
const char * ak_attenuator_sel2str(unsigned int n);


/***************************
 * Global types layout: ak_dbg_asic_enum
 ***************************/
/* ak_dbg_asic_sel  */
const char * ak_dbg_asic_sel2str(unsigned int n);


/***************************
 * Global types layout: ak_rtm_enum
 ***************************/
/* ak_trc_state  */
const char * ak_trc_state2str(unsigned int n);

/* ak_trc_statereq_status  */
const char * ak_trc_statereq_status2str(unsigned int n);

/* ak_par_state  */
const char * ak_par_state2str(unsigned int n);

/* ak_pat_state  */
const char * ak_pat_state2str(unsigned int n);

/* ak_whitening_mode  */
const char * ak_whitening_mode2str(unsigned int n);

/* ak_coex_packet_rx_det  */
const char * ak_coex_packet_rx_det2str(unsigned int n);

#endif //GPHAL_AK_ENUM_H

