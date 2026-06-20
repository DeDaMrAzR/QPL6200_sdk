#   Copyright (c) 2021, Qorvo Inc

#   gpCTC
#   Dictionary of the API of gpCTC.


# *****************************************************************************
# *                   Import files
# *****************************************************************************/

GP_MODULE_ID_CTC = 196

# *****************************************************************************
# *                    Enum Definitions
# *****************************************************************************/

# @enum gpCTC_AttributeId_t - UInt8
# @{
gpCTC_AttributePriority = 0x01
gpCTC_AttributeRequest = 0x02
gpCTC_AttributeTxNotGrantedAction = 0x03
gpCTC_AttributeTxAckNotGrantedAction = 0x04
gpCTC_AttributeCount = 0x05
gpCTC_AttributeRxReqExtTrigger = 0x06
gpCTC_AttributeBLE_NotGrantedAction = 0x07
gpCTC_AttributeATTLNA_Low = 0x08
gpCTC_AttributeATTLNA_High = 0x09
gpCTC_AttributeBLE_Rx_window_Prio = 0x0A
gpCTC_AttributeBLE_Rx_window_Req = 0x0B
gpCTC_AttributeBLE_Rx_packet_Prio = 0x0C
gpCTC_AttributeBLE_Rx_packet_Req = 0x0D
gpCTC_AttributeBLE_Tx_packet_Prio = 0x0E
gpCTC_AttributeBLE_Tx_packet_Req = 0x0F
gpCTC_AttributeBLE_TxRx_packet_Prio = 0x10
gpCTC_AttributeBLE_TxRx_packet_Req = 0x11
gpCTC_AttributeExtTimeOut = 0x12
gpCTC_AttributeGainControlMode = 0x13
gpCTC_AttributeRxReqExtPriority = 0x14
# @brief This defines the number of elements defined in this enum. This can be used as the size of the buffer array. The value needs to be increased when a new attribute is defined.Not to be used as attribute !
CTC_ATTRIBUTES_MAX_NUMBER = 0x15
gpCTC_AttributeId_t = {
    gpCTC_AttributePriority: "gpCTC_AttributePriority",
    gpCTC_AttributeRequest: "gpCTC_AttributeRequest",
    gpCTC_AttributeTxNotGrantedAction: "gpCTC_AttributeTxNotGrantedAction",
    gpCTC_AttributeTxAckNotGrantedAction: "gpCTC_AttributeTxAckNotGrantedAction",
    gpCTC_AttributeCount: "gpCTC_AttributeCount",
    gpCTC_AttributeRxReqExtTrigger: "gpCTC_AttributeRxReqExtTrigger",
    gpCTC_AttributeBLE_NotGrantedAction: "gpCTC_AttributeBLE_NotGrantedAction",
    gpCTC_AttributeATTLNA_Low: "gpCTC_AttributeATTLNA_Low",
    gpCTC_AttributeATTLNA_High: "gpCTC_AttributeATTLNA_High",
    gpCTC_AttributeBLE_Rx_window_Prio: "gpCTC_AttributeBLE_Rx_window_Prio",
    gpCTC_AttributeBLE_Rx_window_Req: "gpCTC_AttributeBLE_Rx_window_Req",
    gpCTC_AttributeBLE_Rx_packet_Prio: "gpCTC_AttributeBLE_Rx_packet_Prio",
    gpCTC_AttributeBLE_Rx_packet_Req: "gpCTC_AttributeBLE_Rx_packet_Req",
    gpCTC_AttributeBLE_Tx_packet_Prio: "gpCTC_AttributeBLE_Tx_packet_Prio",
    gpCTC_AttributeBLE_Tx_packet_Req: "gpCTC_AttributeBLE_Tx_packet_Req",
    gpCTC_AttributeBLE_TxRx_packet_Prio: "gpCTC_AttributeBLE_TxRx_packet_Prio",
    gpCTC_AttributeBLE_TxRx_packet_Req: "gpCTC_AttributeBLE_TxRx_packet_Req",
    gpCTC_AttributeExtTimeOut: "gpCTC_AttributeExtTimeOut",
    gpCTC_AttributeGainControlMode: "gpCTC_AttributeGainControlMode",
    gpCTC_AttributeRxReqExtPriority: "gpCTC_AttributeRxReqExtPriority",
    CTC_ATTRIBUTES_MAX_NUMBER: "CTC_ATTRIBUTES_MAX_NUMBER",
}
# @}

# @enum gpCTC_ModeExecution_t - UInt8
# @{
gpCTC_ModeExecution_On = 0x1
gpCTC_ModeExecution_Off = 0x0
gpCTC_ModeExecution_Single = 0xFF
gpCTC_ModeExecution_t = {
    gpCTC_ModeExecution_On: "gpCTC_ModeExecution_On",
    gpCTC_ModeExecution_Off: "gpCTC_ModeExecution_Off",
    gpCTC_ModeExecution_Single: "gpCTC_ModeExecution_Single",
}
# @}

# @enum gpCTC_ModeId_t - UInt8
# @{
gpCTC_ModeInfo = 0x01
gpCTC_Mode_Set_MAC_RX_Packet = 0x02
gpCTC_Mode_Set_MAC_TX_Packet = 0x03
gpCTC_Mode_Set_MAC_TX_ACK = 0x04
gpCTC_Mode_Set_MAC_RX_ACK = 0x05
gpCTC_Mode_Set_MAC_EarlyPreambleDetect = 0x06
gpCTC_Mode_Set_MAC_RX_ReqExt = 0x07
gpCTC_Mode_Set_BLE_Scan = 0x08
gpCTC_Mode_Set_BLE_Adv = 0x09
gpCTC_Mode_Set_BLE_Init = 0x0A
gpCTC_Mode_Set_BLE_ConnSla = 0x0B
gpCTC_Mode_Set_BLE_ConnMas = 0x0C
gpCTC_Mode_Set_GainControl = 0x0D
gpCTC_Mode_Set_CoexStatistics = 0x0E
gpCTC_Mode_Set_MAC_ExtensionTimeout = 0x0F
gpCTC_ModeId_t = {
    gpCTC_ModeInfo: "gpCTC_ModeInfo",
    gpCTC_Mode_Set_MAC_RX_Packet: "gpCTC_Mode_Set_MAC_RX_Packet",
    gpCTC_Mode_Set_MAC_TX_Packet: "gpCTC_Mode_Set_MAC_TX_Packet",
    gpCTC_Mode_Set_MAC_TX_ACK: "gpCTC_Mode_Set_MAC_TX_ACK",
    gpCTC_Mode_Set_MAC_RX_ACK: "gpCTC_Mode_Set_MAC_RX_ACK",
    gpCTC_Mode_Set_MAC_EarlyPreambleDetect: "gpCTC_Mode_Set_MAC_EarlyPreambleDetect",
    gpCTC_Mode_Set_MAC_RX_ReqExt: "gpCTC_Mode_Set_MAC_RX_ReqExt",
    gpCTC_Mode_Set_BLE_Scan: "gpCTC_Mode_Set_BLE_Scan",
    gpCTC_Mode_Set_BLE_Adv: "gpCTC_Mode_Set_BLE_Adv",
    gpCTC_Mode_Set_BLE_Init: "gpCTC_Mode_Set_BLE_Init",
    gpCTC_Mode_Set_BLE_ConnSla: "gpCTC_Mode_Set_BLE_ConnSla",
    gpCTC_Mode_Set_BLE_ConnMas: "gpCTC_Mode_Set_BLE_ConnMas",
    gpCTC_Mode_Set_GainControl: "gpCTC_Mode_Set_GainControl",
    gpCTC_Mode_Set_CoexStatistics: "gpCTC_Mode_Set_CoexStatistics",
    gpCTC_Mode_Set_MAC_ExtensionTimeout: "gpCTC_Mode_Set_MAC_ExtensionTimeout",
}
# @}

# @enum gpCTC_Result_t - UInt8
# @{
# @brief The function returned successful.
gpCTC_ResultSuccess = 0x0
# @brief The connected PC client is not the expected client. Last connected client takes ownership of the board, rendering previously connected clients invalid.
gpCTC_ResultClientIDMismatch = 0x1
# @brief Received an invalid argument.
gpCTC_ResultInvalidArgument = 0x02
# @brief Setting an attribute on the target failed.
gpCTC_ResultSetAttributeFailed = 0x03
# @brief Setting a mode on the target failed.
gpCTC_ResultSetModeFailed = 0x04
# @brief Received an invalid argument.
gpCTC_ResultInvalidParameter = 0x05
# @brief Component is busy.
gpCTC_ResultBusy = 0x06
# @brief Indicates unsupported attribute or mode (e.g. differences between 15.4 and BLE)
gpCTC_ResultUnsupported = 0x07
# @brief  Indicates that a wrong MF version is used
gpCTC_ResultInvalidChip = 0x08
gpCTC_Result_t = {
    gpCTC_ResultSuccess: "gpCTC_ResultSuccess",
    gpCTC_ResultClientIDMismatch: "gpCTC_ResultClientIDMismatch",
    gpCTC_ResultInvalidArgument: "gpCTC_ResultInvalidArgument",
    gpCTC_ResultSetAttributeFailed: "gpCTC_ResultSetAttributeFailed",
    gpCTC_ResultSetModeFailed: "gpCTC_ResultSetModeFailed",
    gpCTC_ResultInvalidParameter: "gpCTC_ResultInvalidParameter",
    gpCTC_ResultBusy: "gpCTC_ResultBusy",
    gpCTC_ResultUnsupported: "gpCTC_ResultUnsupported",
    gpCTC_ResultInvalidChip: "gpCTC_ResultInvalidChip",
}
# @}


# *****************************************************************************
# *                    Functional Macro Definitions
# *****************************************************************************/


# *****************************************************************************
# *                    Macro Definitions
# *****************************************************************************/


# *****************************************************************************
# *                    Structure Definitions
# *****************************************************************************/
class gpCTC_Attribute_t:
    """
    @brief gpCTC_Attribute_t:
    @member id
    @member value
    """

    def __init__(self, id, value):
        self.id = id  # gpCTC_AttributeId_t
        self.value = value  # Int32


class gpCTC_MACAddress_t:
    """
    @brief gpCTC_MACAddress_t:
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    @member byte5
    @member byte6
    @member byte7
    """

    def __init__(self, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7):
        self.byte0 = byte0  # UInt8
        self.byte1 = byte1  # UInt8
        self.byte2 = byte2  # UInt8
        self.byte3 = byte3  # UInt8
        self.byte4 = byte4  # UInt8
        self.byte5 = byte5  # UInt8
        self.byte6 = byte6  # UInt8
        self.byte7 = byte7  # UInt8


class gpCTC_DeviceAddress_t:
    """
    @brief gpCTC_DeviceAddress_t:
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    @member byte5
    """

    def __init__(self, byte0, byte1, byte2, byte3, byte4, byte5):
        self.byte0 = byte0  # UInt8
        self.byte1 = byte1  # UInt8
        self.byte2 = byte2  # UInt8
        self.byte3 = byte3  # UInt8
        self.byte4 = byte4  # UInt8
        self.byte5 = byte5  # UInt8


class gpCTC_chipSerial_t:
    """
    @brief gpCTC_chipSerial_t:
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    @member byte5
    @member byte6
    @member byte7
    """

    def __init__(self, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7):
        self.byte0 = byte0  # UInt8
        self.byte1 = byte1  # UInt8
        self.byte2 = byte2  # UInt8
        self.byte3 = byte3  # UInt8
        self.byte4 = byte4  # UInt8
        self.byte5 = byte5  # UInt8
        self.byte6 = byte6  # UInt8
        self.byte7 = byte7  # UInt8


class gpCTC_serialNumber_t:
    """
    @brief gpCTC_serialNumber_t:
    @member location
    @member year0
    @member year1
    @member week0
    @member week1
    @member batch0
    @member batch1
    @member index0
    @member index1
    @member index2
    @member index3
    """

    def __init__(self, location, year0, year1, week0, week1, batch0, batch1, index0, index1, index2, index3):
        self.location = location  # UInt8
        self.year0 = year0  # UInt8
        self.year1 = year1  # UInt8
        self.week0 = week0  # UInt8
        self.week1 = week1  # UInt8
        self.batch0 = batch0  # UInt8
        self.batch1 = batch1  # UInt8
        self.index0 = index0  # UInt8
        self.index1 = index1  # UInt8
        self.index2 = index2  # UInt8
        self.index3 = index3  # UInt8


class gpCTC_swVersionNumber_t:
    """
    @brief gpCTC_swVersionNumber_t:
    @member major
    @member minor
    @member revision
    @member patch
    @member changelist
    """

    def __init__(self, major, minor, revision, patch, changelist):
        self.major = major  # UInt8
        self.minor = minor  # UInt8
        self.revision = revision  # UInt8
        self.patch = patch  # UInt8
        self.changelist = changelist  # UInt32


class gpCTC_partNumber_t:
    """
    @brief gpCTC_partNumber_t:
    @member header0
    @member header1
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    """

    def __init__(self, header0, header1, byte0, byte1, byte2, byte3, byte4):
        self.header0 = header0  # UInt8
        self.header1 = header1  # UInt8
        self.byte0 = byte0  # UInt8
        self.byte1 = byte1  # UInt8
        self.byte2 = byte2  # UInt8
        self.byte3 = byte3  # UInt8
        self.byte4 = byte4  # UInt8


class gpCTC_Parameter_t:
    """
    @brief gpCTC_Parameter_t:
    @member value
    """

    def __init__(self, value):
        self.value = value  # Int32


class gpCTC_DiscoveryInfo_t:
    """
    @brief gpCTC_DiscoveryInfo_t:
    @member senderMacAddress
    @member DUTMacAddress
    @member RSSI
    """

    def __init__(self, senderMacAddress, DUTMacAddress, RSSI):
        self.senderMacAddress = senderMacAddress  # gpCTC_MACAddress_t
        self.DUTMacAddress = DUTMacAddress  # gpCTC_MACAddress_t
        self.RSSI = RSSI  # Int16


class gpCTC_ProductName_t:
    """
    @brief gpCTC_ProductName_t: The name that uniquely identifies the firmware and ptc dll. The RCC application will look for matching names so that the CTC dll and the firmware can communicate. Limited to 20 chars
    @member name
    """

    def __init__(self, name):
        self.name = name  # char_list


class gpCTC_ProductID_t:
    """
    @brief gpCTC_ProductID_t:
    @member name
    """

    def __init__(self, name):
        self.name = name  # productIdchar_list


# *****************************************************************************
# *                    Public Function Prototypes
# *****************************************************************************/

gpCTC_Subcommands = {}

# Requests
gpCTC_setClientIDRequest_CmdId = 2  # 0x02
gpCTC_getDUTInfoRequest_CmdId = 8  # 0x08
gpCTC_SetAttributeRequest_CmdId = 10  # 0x0A
gpCTC_GetAttributeRequest_CmdId = 11  # 0x0B
gpCTC_SetModeRequest_CmdId = 12  # 0x0C
gpCTC_SetByteDataForAttributeRequest_CmdId = 13  # 0x0D
gpCTC_GetModeRequest_CmdId = 14  # 0x0E
gpCTC_ExecuteCustomCommand_CmdId = 18  # 0x12
# Indications

gpCTC_Commands = {
    "gpAPIInfo": [GP_MODULE_ID_CTC, "gpCTCApi", True],
    # Requests
    gpCTC_setClientIDRequest_CmdId: ["gpCTC_setClientIDRequest", 0, [["clientID", 1]], [], True],
    gpCTC_getDUTInfoRequest_CmdId: [
        "gpCTC_getDUTInfoRequest",
        0,
        [["clientID", 1]],
        [
            ["result", 1],
            ["version_IsNullPtr", 1],
            ["version", 1],
            ["macAddr_IsNullPtr", 1],
            ["macAddr_byte0", 1],
            ["macAddr_byte1", 1],
            ["macAddr_byte2", 1],
            ["macAddr_byte3", 1],
            ["macAddr_byte4", 1],
            ["macAddr_byte5", 1],
            ["macAddr_byte6", 1],
            ["macAddr_byte7", 1],
            ["bleAddr_IsNullPtr", 1],
            ["bleAddr_byte0", 1],
            ["bleAddr_byte1", 1],
            ["bleAddr_byte2", 1],
            ["bleAddr_byte3", 1],
            ["bleAddr_byte4", 1],
            ["bleAddr_byte5", 1],
            ["appVersion_IsNullPtr", 1],
            ["appVersion_major", 1],
            ["appVersion_minor", 1],
            ["appVersion_revision", 1],
            ["appVersion_patch", 1],
            ["appVersion_changelist", 4],
            ["partNumber_IsNullPtr", 1],
            ["partNumber_header0", 1],
            ["partNumber_header1", 1],
            ["partNumber_byte0", 1],
            ["partNumber_byte1", 1],
            ["partNumber_byte2", 1],
            ["partNumber_byte3", 1],
            ["partNumber_byte4", 1],
            ["productName_IsNullPtr", 1],
            ["productName_name", 40],
            ["ctcVersion_IsNullPtr", 1],
            ["ctcVersion_major", 1],
            ["ctcVersion_minor", 1],
            ["ctcVersion_revision", 1],
            ["ctcVersion_patch", 1],
            ["ctcVersion_changelist", 4],
            ["productID_IsNullPtr", 1],
            ["productID_name", 10],
        ],
        True,
    ],
    gpCTC_SetAttributeRequest_CmdId: [
        "gpCTC_SetAttributeRequest",
        0,
        [
            ["clientID", 1],
            ["numberOfAttr", 1],
            ["attributes_IsNullPtr", 1],
            ["attributes", "numberOfAttr", lambda len: len * 5],
        ],
        [["result", 1]],
        True,
    ],
    gpCTC_GetAttributeRequest_CmdId: [
        "gpCTC_GetAttributeRequest",
        0,
        [
            ["clientID", 1],
            ["numberOfAttr", 1],
            ["attributes_IsNullPtr", 1],
            ["attributes", "numberOfAttr", lambda len: len * 5],
        ],
        [["result", 1], ["attributes_IsNullPtr", 1], ["attributes", "numberOfAttr", lambda len: len * 5]],
        True,
    ],
    gpCTC_SetModeRequest_CmdId: [
        "gpCTC_SetModeRequest",
        0,
        [
            ["clientID", 1],
            ["modeID", 1],
            ["exectime", 4],
            ["OnOff", 1],
            ["numberOfExtraParameters", 1],
            ["parameters_IsNullPtr", 1],
            ["parameters", "numberOfExtraParameters", lambda len: len * 4],
        ],
        [["result", 1]],
        True,
    ],
    gpCTC_SetByteDataForAttributeRequest_CmdId: [
        "gpCTC_SetByteDataForAttributeRequest",
        0,
        [["clientID", 1], ["attributeID", 1], ["dataLen", 1], ["pData_IsNullPtr", 1], ["pData", "dataLen"]],
        [["result", 1]],
        True,
    ],
    gpCTC_GetModeRequest_CmdId: ["gpCTC_GetModeRequest", 0, [["clientID", 1], ["modeID", 1]], [["OnOff", 1]], True],
    gpCTC_ExecuteCustomCommand_CmdId: [
        "gpCTC_ExecuteCustomCommand",
        0,
        [["clientID", 1], ["dataLenIn", 1], ["pDataIn_IsNullPtr", 1], ["pDataIn", "dataLenIn"]],
        [
            ["result", 1],
            ["dataLenOut_IsNullPtr", 1],
            ["dataLenOut", 1],
            ["pDataOut_IsNullPtr", 1],
            ["pDataOut", "dataLenOut"],
        ],
        True,
    ],
    # Indications
}

gpCTC_CommandName = {
    "setclientidrequest": gpCTC_setClientIDRequest_CmdId,
    "getdutinforequest": gpCTC_getDUTInfoRequest_CmdId,
    "setattributerequest": gpCTC_SetAttributeRequest_CmdId,
    "getattributerequest": gpCTC_GetAttributeRequest_CmdId,
    "setmoderequest": gpCTC_SetModeRequest_CmdId,
    "setbytedataforattributerequest": gpCTC_SetByteDataForAttributeRequest_CmdId,
    "getmoderequest": gpCTC_GetModeRequest_CmdId,
    "executecustomcommand": gpCTC_ExecuteCustomCommand_CmdId,
}
