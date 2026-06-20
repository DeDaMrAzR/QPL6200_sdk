"""CTC driver API"""

import logging

import six

from . import gpCTCAPI
from .ctcdriver import CTC

logger = logging.getLogger(__name__)

from ...rcc_types.generic import RccArgumentTypeDict, RCCDriverGeneric


class CTCDriver(RCCDriverGeneric, CTC):
    """RCC driver class for CTC devices."""

    _api_version = 1

    # datatypes
    low_high_t = RccArgumentTypeDict({"LOW": 0x00, "HIGH": 0x01})
    tx_notgranted_action_t = RccArgumentTypeDict(
        {"IGNORE": 0x00, "DISABLE_PA": 0x01, "CCA_FAILURE": 0x02, "CCA_HOLD": 0x04, "DELAYED_START": 0x08}
    )
    tx_acknotgranted_action_t = RccArgumentTypeDict({"IGNORE": 0x00, "DISABLE_PA": 0x01, "SKIP_TX": 0x02})
    att_lna_t = RccArgumentTypeDict(
        {"LNA0": 0x00, "LNA1": 0x01, "LNA2": 0x02, "LNA3": 0x03, "LNA4": 0x04, "LNA5": 0x05, "IGNORE": 0xFF}
    )
    gain_control_mode_t = RccArgumentTypeDict(
        {"DEFAULT": 0x00, "FIXED": 0x01, "RSSIBASEDAGC": 0x02, "GPIOBASEDAGC": 0x03}
    )
    rx_req_ext_trigger_t = RccArgumentTypeDict(
        {"NONE": 0x00, "PREAMBLE": 0x01, "SFD": 0x02, "PACKETABORT": 0x04, "FCSERR": 0x08}
    )
    ble_notgranted_action_t = RccArgumentTypeDict({"IGNORE": 0x00, "DISABLE_PA": 0x01})

    def __init__(self, connection_str, user_commands=None, debug_level=0):
        RCCDriverGeneric.__init__(self)
        CTC.__init__(self, connection_str, debug_level)

        if user_commands:
            for cmd, func in six.iteritems(user_commands):
                self.bind_user_command(cmd, func)
        else:
            # Default user commands
            self._user_commands = {
                "PRIORITY": self.set_priority,
                "REQUEST": self.set_request,
                "COUNT": self.set_count,
                "TRIGGER": self.set_rxreqexttrigger,
                "ATTLNA_LOW": self.set_attlna_low,
                "ATTLNA_HIGH": self.set_attlna_high,
                "TX_NOTGRANTED_ACTION": self.set_txnotgrantedaction,
                "TX_ACK_NOTGRANTED_ACTION": self.set_txacknotgrantedaction,
                "BLE_NOTGRANTED_ACTION": self.set_ble_notgrantedaction,
                "BLE_RX_WINDOW_PRIO": self.set_ble_rx_window_prio,
                "BLE_RX_WINDOW_REQ": self.set_ble_rx_window_req,
                "BLE_RX_PACKET_PRIO": self.set_ble_rx_packet_prio,
                "BLE_RX_PACKET_REQ": self.set_ble_rx_packet_req,
                "BLE_TX_PACKET_PRIO": self.set_ble_tx_packet_prio,
                "BLE_TX_PACKET_REQ": self.set_ble_tx_packet_req,
                "BLE_TX_RX_PACKET_PRIO": self.set_ble_txrx_packet_prio,
                "BLE_TX_RX_PACKET_REQ": self.set_ble_txrx_packet_req,
                "INFO": self.get_device_info,
                "SET_MAC_RX_PACKET": self.set_mode_mac_rx_packet,
                "SET_MAC_TX_PACKET": self.set_mode_mac_tx_packet,
                "SET_MAC_RX_ACK": self.set_mode_mac_rx_ack,
                "SET_MAC_TX_ACK": self.set_mode_mac_tx_ack,
                "SET_MAC_EARLY_PREAMBLE_DETECT": self.set_mode_mac_earlypreambledetect,
                "SET_MAC_RX_REQEXT": self.set_mode_mac_rx_request,
                "SET_BLE_ADV": self.set_mode_ble_adv,
                "SET_BLE_SCAN": self.set_mode_ble_scan,
                "SET_BLE_CONNSLA": self.set_mode_ble_connsla,
                "SET_BLE_CONNMAS": self.set_mode_ble_connmas,
                "SET_BLE_INIT": self.set_mode_ble_int,
            }

            self._user_getter_commands = {
                "PRIORITY": self.get_priority,
                "REQUEST": self.get_request,
                "COUNT": self.get_count,
                "TRIGGER": self.get_rxreqexttrigger,
                "ATTLNA_LOW": self.get_attlna_low,
                "ATTLNA_HIGH": self.get_attlna_high,
                "TX_NOTGRANTED_ACTION": self.get_txnotgrantedaction,
                "TX_ACK_NOTGRANTED_ACTION": self.get_txacknotgrantedaction,
                "BLE_NOTGRANTED_ACTION": self.get_ble_notgrantedaction,
                "BLE_RX_WINDOW_PRIO": self.get_ble_rx_window_prio,
                "BLE_RX_WINDOW_REQ": self.get_ble_rx_window_req,
                "BLE_RX_PACKET_PRIO": self.get_ble_rx_packet_prio,
                "BLE_RX_PACKET_REQ": self.get_ble_rx_packet_req,
                "BLE_TX_PACKET_PRIO": self.get_ble_tx_packet_prio,
                "BLE_TX_PACKET_REQ": self.get_ble_tx_packet_req,
                "BLE_TX_RX_PACKET_PRIO": self.get_ble_txrx_packet_prio,
                "BLE_TX_RX_PACKET_REQ": self.get_ble_txrx_packet_req,
                "INFO": self.get_device_info,
                "SET_MAC_RX_PACKET": None,
                "SET_MAC_TX_PACKET": None,
                "SET_MAC_RX_ACK": None,
                "SET_MAC_TX_ACK": None,
                "SET_MAC_EARLY_PREAMBLE_DETECT": self.get_mode_mac_earlypreambledetect,
                "SET_MAC_RX_REQEXT": None,
                "SET_BLE_ADV": None,
                "SET_BLE_SCAN": None,
                "SET_BLE_CONNSLA": None,
                "SET_BLE_CONNMAS": None,
                "SET_BLE_INIT": None,
            }

        # Modes with linked attributes
        self._mode_attributes = {
            self.set_mode_gaincontrol: [self.set_attlna_high, self.set_attlna_low, self.set_gaincontrolmode],
            self.set_mode_mac_rx_packet: [self.set_priority, self.set_request],
            self.set_mode_mac_tx_packet: [self.set_priority, self.set_request, self.set_txnotgrantedaction],
            self.set_mode_mac_tx_ack: [self.set_priority, self.set_request, self.set_txacknotgrantedaction],
            self.set_mode_mac_rx_ack: [self.set_priority, self.set_request],
            self.set_mode_mac_rx_request: [self.set_rxreqexttrigger],
            self.set_mode_ble_adv: [
                self.set_ble_notgrantedaction,
                self.set_ble_txrx_packet_prio,
                self.set_ble_txrx_packet_req,
            ],
            self.set_mode_ble_scan: [
                self.set_ble_notgrantedaction,
                self.set_ble_rx_packet_prio,
                self.set_ble_rx_packet_req,
                self.set_ble_rx_window_prio,
                self.set_ble_rx_window_req,
                self.set_ble_tx_packet_prio,
                self.set_ble_tx_packet_req,
            ],
            self.set_mode_ble_connsla: [
                self.set_ble_notgrantedaction,
                self.set_ble_txrx_packet_prio,
                self.set_ble_txrx_packet_req,
            ],
            self.set_mode_ble_connmas: [
                self.set_ble_notgrantedaction,
                self.set_ble_txrx_packet_prio,
                self.set_ble_txrx_packet_req,
            ],
            self.set_mode_ble_int: [
                self.set_ble_notgrantedaction,
                self.set_ble_rx_packet_prio,
                self.set_ble_rx_packet_req,
                self.set_ble_rx_window_prio,
                self.set_ble_rx_window_req,
                self.set_ble_tx_packet_prio,
                self.set_ble_tx_packet_req,
            ],
        }

        # Indicates whether a mode has an ON/OFF state
        self._mode_list_has_state_false = [
            self.get_device_info,
            self.set_mode_gaincontrol,
            self.set_mode_mac_rx_packet,
            self.set_mode_mac_tx_packet,
            self.set_mode_mac_tx_ack,
            self.set_mode_mac_rx_ack,
            self.set_mode_mac_rx_request,
            self.set_mode_ble_adv,
            self.set_mode_ble_connsla,
            self.set_mode_ble_connmas,
            self.set_mode_ble_int,
        ]
        self._mode_list_has_state_true = [self.set_mode_mac_earlypreambledetect]

        # The command syntax is automatically generated except for some complex cases
        self._command_syntax = {}

        # Some command values have special types
        self._command_value_types = {
            self.set_priority: self.low_high_t,
            self.set_request: self.off_on_t,
            self.set_txnotgrantedaction: self.tx_notgranted_action_t,
            self.set_txacknotgrantedaction: self.tx_acknotgranted_action_t,
            self.set_attlna_low: self.att_lna_t,
            self.set_attlna_high: self.att_lna_t,
            self.set_gaincontrolmode: self.gain_control_mode_t,
            self.set_rxreqexttrigger: self.rx_req_ext_trigger_t,
            self.set_ble_notgrantedaction: self.ble_notgranted_action_t,
            self.set_ble_rx_packet_prio: self.low_high_t,
            self.set_ble_rx_packet_req: self.off_on_t,
            self.set_ble_tx_packet_prio: self.low_high_t,
            self.set_ble_tx_packet_req: self.off_on_t,
            self.set_ble_rx_window_prio: self.low_high_t,
            self.set_ble_rx_window_req: self.off_on_t,
            self.set_ble_txrx_packet_prio: self.low_high_t,
            self.set_ble_txrx_packet_req: self.off_on_t,
        }

    # ------ CTC modes
    def set_mode_gaincontrol(self, att_lna_low=None, att_lna_high=None, gain_control_mode=None):
        """Sets the gain control for the selected mode."""
        if att_lna_low is not None:
            mode_value = self._get_int_value(att_lna_low, self.att_lna_t)
            self.set_attlna_low(mode_value)
        if att_lna_high is not None:
            mode_value = self._get_int_value(att_lna_high, self.att_lna_t)
            self.set_attlna_high(mode_value)
        if gain_control_mode is not None:
            mode_value = self._get_int_value(gain_control_mode, self.gain_control_mode_t)
            self.set_gaincontrolmode(mode_value)

    def set_mode_mac_rx_packet(self, priority=None, request=None):
        """Sets the coexistence parameters for 802.15.4 Packet RX."""
        if priority is not None:
            mode_value = self._get_int_value(priority, self.low_high_t)
            self.set_priority(mode_value)
        if request is not None:
            mode_value = self._get_int_value(request, self.off_on_t)
            self.set_request(mode_value)

    def set_mode_mac_tx_packet(self, priority=None, request=None, tx_notgranted_action=None):
        """Sets the coexistence parameters for 802.15.4 Packet TX."""
        if priority is not None:
            mode_value = self._get_int_value(priority, self.low_high_t)
            self.set_priority(mode_value)
        if request is not None:
            mode_value = self._get_int_value(request, self.off_on_t)
            self.set_request(mode_value)
        if tx_notgranted_action is not None:
            mode_value = self._get_int_value(tx_notgranted_action, self.tx_notgranted_action_t)
            self.set_txnotgrantedaction(mode_value)

    def set_mode_mac_rx_ack(self, priority=None, request=None):
        """Sets the coexistence parameters for 802.15.4 ACK RX."""
        if priority is not None:
            mode_value = self._get_int_value(priority, self.low_high_t)
            self.set_priority(mode_value)
        if request is not None:
            mode_value = self._get_int_value(request, self.off_on_t)
            self.set_request(mode_value)

    def set_mode_mac_tx_ack(self, priority=None, request=None, tx_ack_notgranted_action=None):
        """Sets the coexistence parameter for 802.15.4 ACK TX."""
        if priority is not None:
            mode_value = self._get_int_value(priority, self.low_high_t)
            self.set_priority(mode_value)
        if request is not None:
            mode_value = self._get_int_value(request, self.off_on_t)
            self.set_request(mode_value)
        if tx_ack_notgranted_action is not None:
            mode_value = self._get_int_value(tx_ack_notgranted_action, self.tx_acknotgranted_action_t)
            self.set_txacknotgrantedaction(mode_value)

    def set_mode_mac_earlypreambledetect(self, exec_mode):
        """Enables early preamble detect."""
        exec_mode = self._get_int_value(exec_mode, self.exec_mode_t)
        self.set_mode(gpCTCAPI.gpCTC_Mode_Set_MAC_EarlyPreambleDetect, 0, exec_mode, 1, 0x00)

    def set_mode_mac_rx_request(self, trigger):
        """Configures the RX request extension, that will keep RX high
        for extra time after receiving a corrupted packet."""
        if trigger is not None:
            mode_value = self._get_int_value(trigger, self.rx_req_ext_trigger_t)
            self.set_rxreqexttrigger(mode_value)

    def set_mode_ble_adv(self, ble_notgranted_action=None, ble_tx_rx_packet_prio=None, ble_tx_rx_packet_req=None):
        """Sets the advertisement coex configuration."""
        if ble_notgranted_action is not None:
            mode_value = self._get_int_value(ble_notgranted_action, self.ble_notgranted_action_t)
            self.set_ble_notgrantedaction(mode_value)
        if ble_tx_rx_packet_prio is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_prio, self.low_high_t)
            self.set_ble_txrx_packet_prio(mode_value)
        if ble_tx_rx_packet_req is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_req, self.off_on_t)
            self.set_ble_txrx_packet_req(mode_value)

    def set_mode_ble_scan(
        self,
        ble_notgranted_action=None,
        ble_rx_packet_prio=None,
        ble_rx_packet_req=None,
        ble_rx_window_prio=None,
        ble_rx_window_req=None,
        ble_tx_packet_prio=None,
        ble_tx_packet_req=None,
    ):
        """Set the scanning coex configuration."""
        if ble_notgranted_action is not None:
            mode_value = self._get_int_value(ble_notgranted_action, self.ble_notgranted_action_t)
            self.set_ble_notgrantedaction(mode_value)
        if ble_rx_packet_prio is not None:
            mode_value = self._get_int_value(ble_rx_packet_prio, self.low_high_t)
            self.set_ble_rx_packet_prio(mode_value)
        if ble_rx_packet_req is not None:
            mode_value = self._get_int_value(ble_rx_packet_req, self.off_on_t)
            self.set_ble_rx_packet_req(mode_value)
        if ble_rx_window_prio is not None:
            mode_value = self._get_int_value(ble_rx_window_prio, self.low_high_t)
            self.set_ble_rx_window_prio(mode_value)
        if ble_rx_window_req is not None:
            mode_value = self._get_int_value(ble_rx_window_req, self.off_on_t)
            self.set_ble_rx_window_req(mode_value)
        if ble_tx_packet_prio is not None:
            mode_value = self._get_int_value(ble_rx_window_prio, self.low_high_t)
            self.set_ble_tx_packet_prio(mode_value)
        if ble_tx_packet_req is not None:
            mode_value = self._get_int_value(ble_rx_window_req, self.off_on_t)
            self.set_ble_tx_packet_req(mode_value)

    def set_mode_ble_connsla(self, ble_notgranted_action=None, ble_tx_rx_packet_prio=None, ble_tx_rx_packet_req=None):
        """Sets the slave connection coex configuration."""
        if ble_notgranted_action is not None:
            mode_value = self._get_int_value(ble_notgranted_action, self.ble_notgranted_action_t)
            self.set_ble_notgrantedaction(mode_value)
        if ble_tx_rx_packet_prio is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_prio, self.low_high_t)
            self.set_ble_txrx_packet_prio(mode_value)
        if ble_tx_rx_packet_req is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_req, self.off_on_t)
            self.set_ble_txrx_packet_req(mode_value)

    def set_mode_ble_connmas(self, ble_notgranted_action=None, ble_tx_rx_packet_prio=None, ble_tx_rx_packet_req=None):
        """Sets the master connection coex configuration."""
        if ble_notgranted_action is not None:
            mode_value = self._get_int_value(ble_notgranted_action, self.ble_notgranted_action_t)
            self.set_ble_notgrantedaction(mode_value)
        if ble_tx_rx_packet_prio is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_prio, self.low_high_t)
            self.set_ble_txrx_packet_prio(mode_value)
        if ble_tx_rx_packet_req is not None:
            mode_value = self._get_int_value(ble_tx_rx_packet_req, self.off_on_t)
            self.set_ble_txrx_packet_req(mode_value)

    def set_mode_ble_int(
        self,
        ble_notgranted_action=None,
        ble_rx_packet_prio=None,
        ble_rx_packet_req=None,
        ble_rx_window_prio=None,
        ble_rx_window_req=None,
        ble_tx_packet_prio=None,
        ble_tx_packet_req=None,
    ):
        """Sets the initiating coex configuration."""
        if ble_notgranted_action is not None:
            mode_value = self._get_int_value(ble_notgranted_action, self.ble_notgranted_action_t)
            self.set_ble_notgrantedaction(mode_value)
        if ble_rx_packet_prio is not None:
            mode_value = self._get_int_value(ble_rx_packet_prio, self.low_high_t)
            self.set_ble_rx_packet_prio(mode_value)
        if ble_rx_packet_req is not None:
            mode_value = self._get_int_value(ble_rx_packet_req, self.off_on_t)
            self.set_ble_rx_packet_req(mode_value)
        if ble_rx_window_prio is not None:
            mode_value = self._get_int_value(ble_rx_window_prio, self.low_high_t)
            self.set_ble_rx_window_prio(mode_value)
        if ble_rx_window_req is not None:
            mode_value = self._get_int_value(ble_rx_window_req, self.off_on_t)
            self.set_ble_rx_window_req(mode_value)
        if ble_tx_packet_prio is not None:
            mode_value = self._get_int_value(ble_rx_window_prio, self.low_high_t)
            self.set_ble_tx_packet_prio(mode_value)
        if ble_tx_packet_req is not None:
            mode_value = self._get_int_value(ble_rx_window_req, self.off_on_t)
            self.set_ble_tx_packet_req(mode_value)
