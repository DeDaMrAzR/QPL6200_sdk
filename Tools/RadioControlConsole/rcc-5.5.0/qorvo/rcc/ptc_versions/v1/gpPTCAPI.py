#   Copyright (c) 2017, Qorvo Inc

#   gpPTC
#   Dictionary of the API of gpPTC.


# *****************************************************************************
# *                   Import files
# *****************************************************************************/

GP_MODULE_ID_PTC = 193

# *****************************************************************************
# *                    Enum Definitions
# *****************************************************************************/
# @enum gpPTC_AttributeId_t - UInt8
# @{
# @brief Channel that the device will transmit or listen on (for all PHYs).
gpPTC_AttributeChannel = 0x01  # noqa E221
# @brief Antenna on which the device will transmit or listen.
gpPTC_AttributeAntenna = 0x02  # noqa E221
# @brief Boolean flag controlling the antenna diversity feature.
gpPTC_AttributeAntennaDiversity = 0x03  # noqa E221
gpPTC_AttributeSleepMode = 0x04  # noqa E221
gpPTC_AttributeCSMAMode = 0x05  # noqa E221
gpPTC_AttributeMAXBE = 0x06  # noqa E221
gpPTC_AttributeMINBE = 0x07  # noqa E221
gpPTC_AttributeMaxCSMABackoff = 0x08  # noqa E221
gpPTC_AttributeMaxMACRetries = 0x09  # noqa E221
gpPTC_AttributeTXPower = 0x10  # noqa E221
gpPTC_AttributePanID = 0x11  # noqa E221
gpPTC_AttributeShortAddress = 0x12  # noqa E221
gpPTC_AttributeCWMode = 0x13  # noqa E221
gpPTC_AttributePowerControlLoop = 0x14  # noqa E221
# @brief Value representing the address that will be used with the ReadRegister mode.
gpPTC_AttributeReadRegisterAddress = 0x15  # noqa E221
# @brief Value representing the address that will be used with the WriteRegister mode.
gpPTC_AttributeWriteRegisterAddress = 0x16  # noqa E221
# @brief Value that will be written to selected register with the WrigeRegister mode.
gpPTC_AttributeWriteRegisterValue = 0x17  # noqa E221
# @brief Special case to set the byte array as the data payload for tx
gpPTC_AttributeSetTxData = 0x18  # noqa E221
# @brief Sets the interval between packets in a packet train when transmitting multiple packets.
gpPTC_AttributePacketIntervalInMS = 0x19  # noqa E221
# @brief Number of scans to be done by ED Scan
gpPTC_AttributeScanCount = 0x20  # noqa E221
# @brief Number of packets that will be transmitted in a packet train.
gpPTC_AttributePacketCount = 0x21  # noqa E221
gpPTC_AttributePacketInPacket = 0x22  # noqa E221
gpPTC_AttributePromiscuousMode = 0x23  # noqa E221
gpPTC_AttributePacketLength = 0x24  # noqa E221
# @brief Write register values only for the specified bitmask
gpPTC_AttributeWriteRegisterBitsMask = 0x25  # noqa E221
gpPTC_AttributeWriteRegisterBitsValue = 0x26  # noqa E221
gpPTC_AttributePhyMode = 0x27  # noqa E221
gpPTC_BLE_DTM_PacketType = 0x28  # noqa E221
gpPTC_AttributeScanIntervalInMS = 0x29  # noqa E221
gpPTC_BLE_DataRate = 0x30  # noqa E221
gpPTC_AttributeMapClockType = 0x31  # noqa E221
gpPTC_AttributeMapClockIOPin = 0x32  # noqa E221
gpPTC_AttributeRxLnaAttDuringTimeout = 0x33  # noqa E221
gpPTC_AttributeRxMultiStandard = 0x34  # noqa E221
gpPTC_AttributeRxHighSensitivity = 0x35  # noqa E221
gpPTC_AttributeRxMultiChannel = 0x36  # noqa E221
gpPTC_AttributePDMClkSrc = 0x37  # noqa E221
gpPTC_AttributePDMClkFreq = 0x38  # noqa E221
gpPTC_AttributePDMClkOutPin = 0x39  # noqa E221
# @brief This defines the number of elements defined in this enum. This can be used as the size of the buffer array. The value needs to be increased when a new attribute is defined.Not to be used as attribute !
PTC_ATTRIBUTES_MAX_NUMBER = 0x3A  # noqa E221
gpPTC_AttributeId_t = {
    gpPTC_AttributeChannel: "gpPTC_AttributeChannel",  # noqa E221
    gpPTC_AttributeAntenna: "gpPTC_AttributeAntenna",  # noqa E221
    gpPTC_AttributeAntennaDiversity: "gpPTC_AttributeAntennaDiversity",  # noqa E221
    gpPTC_AttributeSleepMode: "gpPTC_AttributeSleepMode",  # noqa E221
    gpPTC_AttributeCSMAMode: "gpPTC_AttributeCSMAMode",  # noqa E221
    gpPTC_AttributeMAXBE: "gpPTC_AttributeMAXBE",  # noqa E221
    gpPTC_AttributeMINBE: "gpPTC_AttributeMINBE",  # noqa E221
    gpPTC_AttributeMaxCSMABackoff: "gpPTC_AttributeMaxCSMABackoff",  # noqa E221
    gpPTC_AttributeMaxMACRetries: "gpPTC_AttributeMaxMACRetries",  # noqa E221
    gpPTC_AttributeTXPower: "gpPTC_AttributeTXPower",  # noqa E221
    gpPTC_AttributePanID: "gpPTC_AttributePanID",  # noqa E221
    gpPTC_AttributeShortAddress: "gpPTC_AttributeShortAddress",  # noqa E221
    gpPTC_AttributeCWMode: "gpPTC_AttributeCWMode",  # noqa E221
    gpPTC_AttributePowerControlLoop: "gpPTC_AttributePowerControlLoop",  # noqa E221
    gpPTC_AttributeReadRegisterAddress: "gpPTC_AttributeReadRegisterAddress",  # noqa E221
    gpPTC_AttributeWriteRegisterAddress: "gpPTC_AttributeWriteRegisterAddress",  # noqa E221
    gpPTC_AttributeWriteRegisterValue: "gpPTC_AttributeWriteRegisterValue",  # noqa E221
    gpPTC_AttributeSetTxData: "gpPTC_AttributeSetTxData",  # noqa E221
    gpPTC_AttributePacketIntervalInMS: "gpPTC_AttributePacketIntervalInMS",  # noqa E221
    gpPTC_AttributeScanCount: "gpPTC_AttributeScanCount",  # noqa E221
    gpPTC_AttributePacketCount: "gpPTC_AttributePacketCount",  # noqa E221
    gpPTC_AttributePacketInPacket: "gpPTC_AttributePacketInPacket",  # noqa E221
    gpPTC_AttributePromiscuousMode: "gpPTC_AttributePromiscuousMode",  # noqa E221
    gpPTC_AttributePacketLength: "gpPTC_AttributePacketLength",  # noqa E221
    gpPTC_AttributeWriteRegisterBitsMask: "gpPTC_AttributeWriteRegisterBitsMask",  # noqa E221
    gpPTC_AttributeWriteRegisterBitsValue: "gpPTC_AttributeWriteRegisterBitsValue",  # noqa E221
    gpPTC_AttributePhyMode: "gpPTC_AttributePhyMode",  # noqa E221
    gpPTC_BLE_DTM_PacketType: "gpPTC_BLE_DTM_PacketType",  # noqa E221
    gpPTC_AttributeScanIntervalInMS: "gpPTC_AttributeScanIntervalInMS",  # noqa E221
    gpPTC_BLE_DataRate: "gpPTC_BLE_DataRate",  # noqa E221
    gpPTC_AttributeMapClockType: "gpPTC_AttributeMapClockType",  # noqa E221
    gpPTC_AttributeMapClockIOPin: "gpPTC_AttributeMapClockIOPin",  # noqa E221
    gpPTC_AttributeRxLnaAttDuringTimeout: "gpPTC_AttributeRxLnaAttDuringTimeout",  # noqa E221
    gpPTC_AttributeRxMultiStandard: "gpPTC_AttributeRxMultiStandard",  # noqa E221
    gpPTC_AttributeRxHighSensitivity: "gpPTC_AttributeRxHighSensitivity",  # noqa E221
    gpPTC_AttributeRxMultiChannel: "gpPTC_AttributeRxMultiChannel",  # noqa E221
    gpPTC_AttributePDMClkSrc: "gpPTC_AttributePDMClkSrc",  # noqa E221
    gpPTC_AttributePDMClkFreq: "gpPTC_AttributePDMClkFreq",  # noqa E221
    gpPTC_AttributePDMClkOutPin: "gpPTC_AttributePDMClkOutPin",  # noqa E221
    PTC_ATTRIBUTES_MAX_NUMBER: "PTC_ATTRIBUTES_MAX_NUMBER",  # noqa E221
}
# @}

# @enum gpPTC_CSMAMode_t - UInt8
# @{
gpPTC_CSMAMode_NoCCA = 0x0  # noqa E221
gpPTC_CSMAMode_CCA = 0x1  # noqa E221
gpPTC_CSMAMode_CSMA = 0x2  # noqa E221
gpPTC_CSMAMode_t = {
    gpPTC_CSMAMode_NoCCA: "gpPTC_CSMAMode_NoCCA",  # noqa E221
    gpPTC_CSMAMode_CCA: "gpPTC_CSMAMode_CCA",  # noqa E221
    gpPTC_CSMAMode_CSMA: "gpPTC_CSMAMode_CSMA",  # noqa E221
}
# @}

# @enum gpPTC_CWMode_t - UInt8
# @{
gpPTC_CWUnModulated = 0x1  # noqa E221
gpPTC_CWModulated = 0x0  # noqa E221
gpPTC_CWMode_t = {
    gpPTC_CWUnModulated: "gpPTC_CWUnModulated",  # noqa E221
    gpPTC_CWModulated: "gpPTC_CWModulated",  # noqa E221
}
# @}

# @enum gpPTC_HighSensitivity_t - UInt8
# @{
gpPTC_Enabled = 0x1  # noqa E221
gpPTC_Disabled = 0x0  # noqa E221
gpPTC_HighSensitivity_t = {
    gpPTC_Enabled: "gpPTC_Enabled",  # noqa E221
    gpPTC_Disabled: "gpPTC_Disabled",  # noqa E221
}
# @}

# @enum gpPTC_ModeExecution_t - UInt8
# @{
gpPTC_ModeExecution_On = 0x1  # noqa E221
gpPTC_ModeExecution_Off = 0x0  # noqa E221
gpPTC_ModeExecution_Single = 0xFF  # noqa E221
gpPTC_ModeExecution_t = {
    gpPTC_ModeExecution_On: "gpPTC_ModeExecution_On",  # noqa E221
    gpPTC_ModeExecution_Off: "gpPTC_ModeExecution_Off",  # noqa E221
    gpPTC_ModeExecution_Single: "gpPTC_ModeExecution_Single",  # noqa E221
}
# @}

# @enum gpPTC_ModeId_t - UInt8
# @{
gpPTC_ModeInfo = 0x01  # noqa E221
gpPTC_ModeCarrierWave = 0x02  # noqa E221
gpPTC_ModeRx = 0x03  # noqa E221
gpPTC_ModeReadRegister = 0x04  # noqa E221
gpPTC_ModeWriteRegister = 0x05  # noqa E221
gpPTC_ModeSleep = 0x06  # noqa E221
gpPTC_ModeWakeUp = 0x07  # noqa E221
gpPTC_ModeEDScan = 0x08  # noqa E221
gpPTC_ModeTransmitPacket = 0x09  # noqa E221
gpPTC_ModePrintPacketCount = 0x10  # noqa E221
gpPTC_ModeResetCounters = 0x11  # noqa E221
gpPTC_ModePrintReceivedPackets = 0x12  # noqa E221
gpPTC_ModeTransmitRandomPackets = 0x13  # noqa E221
gpPTC_ModeWriteRegisterBits = 0x14  # noqa E221
gpPTC_ModeSetClockToGPIO = 0x15  # noqa E221
gpPTC_ModeXtalSelfTest = 0x16  # noqa E221
gpPTC_ModeSetPdmClock = 0x17  # noqa E221
gpPTC_ModeId_t = {
    gpPTC_ModeInfo: "gpPTC_ModeInfo",  # noqa E221
    gpPTC_ModeCarrierWave: "gpPTC_ModeCarrierWave",  # noqa E221
    gpPTC_ModeRx: "gpPTC_ModeRx",  # noqa E221
    gpPTC_ModeReadRegister: "gpPTC_ModeReadRegister",  # noqa E221
    gpPTC_ModeWriteRegister: "gpPTC_ModeWriteRegister",  # noqa E221
    gpPTC_ModeSleep: "gpPTC_ModeSleep",  # noqa E221
    gpPTC_ModeWakeUp: "gpPTC_ModeWakeUp",  # noqa E221
    gpPTC_ModeEDScan: "gpPTC_ModeEDScan",  # noqa E221
    gpPTC_ModeTransmitPacket: "gpPTC_ModeTransmitPacket",  # noqa E221
    gpPTC_ModePrintPacketCount: "gpPTC_ModePrintPacketCount",  # noqa E221
    gpPTC_ModeResetCounters: "gpPTC_ModeResetCounters",  # noqa E221
    gpPTC_ModePrintReceivedPackets: "gpPTC_ModePrintReceivedPackets",  # noqa E221
    gpPTC_ModeTransmitRandomPackets: "gpPTC_ModeTransmitRandomPackets",  # noqa E221
    gpPTC_ModeWriteRegisterBits: "gpPTC_ModeWriteRegisterBits",  # noqa E221
    gpPTC_ModeSetClockToGPIO: "gpPTC_ModeSetClockToGPIO",  # noqa E221
    gpPTC_ModeXtalSelfTest: "gpPTC_ModeXtalSelfTest",  # noqa E221
    gpPTC_ModeSetPdmClock: "gpPTC_ModeSetPdmClock",  # noqa E221
}
# @}

# @enum gpPTC_MultiChannel_t - UInt8
# @{
gpPTC_Enabled = 0x1  # noqa E221
gpPTC_Disabled = 0x0  # noqa E221
gpPTC_MultiChannel_t = {
    gpPTC_Enabled: "gpPTC_Enabled",  # noqa E221
    gpPTC_Disabled: "gpPTC_Disabled",  # noqa E221
}
# @}

# @enum gpPTC_MultiStandard_t - UInt8
# @{
gpPTC_Enabled = 0x1  # noqa E221
gpPTC_Disabled = 0x0  # noqa E221
gpPTC_MultiStandard_t = {
    gpPTC_Enabled: "gpPTC_Enabled",  # noqa E221
    gpPTC_Disabled: "gpPTC_Disabled",  # noqa E221
}
# @}

# @enum gpPTC_PDMClkSrc_t - UInt8
# @{
gpPTC_PDMClkSrc_None = 0x00  # noqa E221
gpPTC_PDMClkSrc_2M = 0x01  # noqa E221
gpPTC_PDMClkSrc_PLL = 0x02  # noqa E221
gpPTC_PDMClkSrc_t = {
    gpPTC_PDMClkSrc_None: "gpPTC_PDMClkSrc_None",  # noqa E221
    gpPTC_PDMClkSrc_2M: "gpPTC_PDMClkSrc_2M",  # noqa E221
    gpPTC_PDMClkSrc_PLL: "gpPTC_PDMClkSrc_PLL",  # noqa E221
}
# @}

# @enum gpPTC_PhyMode_t - UInt8
# @{
# @brief Enum value for 802.15.4 PHY
gpPTC_RF4CE = 0x00  # noqa E221
# @brief Enum value for BLE PHY
gpPTC_BLE = 0x01  # noqa E221
gpPTC_PhyMode_t = {
    gpPTC_RF4CE: "gpPTC_RF4CE",  # noqa E221
    gpPTC_BLE: "gpPTC_BLE",  # noqa E221
}
# @}

# @enum gpPTC_Result_t - UInt8
# @{
# @brief The function returned successful.
gpPTC_ResultSuccess = 0x0  # noqa E221
# @brief The connected PC client is not the expected client. Last connected client takes ownership of the board, rendering previously connected clients invalid.
gpPTC_ResultClientIDMismatch = 0x1  # noqa E221
# @brief Received an invalid argument.
gpPTC_ResultInvalidArgument = 0x02  # noqa E221
# @brief Setting an attribute on the target failed.
gpPTC_ResultSetAttributeFailed = 0x03  # noqa E221
# @brief Setting a mode on the target failed.
gpPTC_ResultSetModeFailed = 0x04  # noqa E221
# @brief Received an invalid argument.
gpPTC_ResultInvalidParameter = 0x05  # noqa E221
# @brief Component is busy.
gpPTC_ResultBusy = 0x06  # noqa E221
# @brief Indicates unsupported attribute or mode (e.g. differences between 15.4 and BLE)
gpPTC_ResultUnsupported = 0x07  # noqa E221
# @brief Indicates that a wrong MF version is used
gpPTC_ResultInvalidChip = 0x08  # noqa E221
# @brief Indicates that a wrong combination was selected while enabling the current RX mode
gpPTC_ResultInvalidRXMode = 0x09  # noqa E221
gpPTC_Result_t = {
    gpPTC_ResultSuccess: "gpPTC_ResultSuccess",  # noqa E221
    gpPTC_ResultClientIDMismatch: "gpPTC_ResultClientIDMismatch",  # noqa E221
    gpPTC_ResultInvalidArgument: "gpPTC_ResultInvalidArgument",  # noqa E221
    gpPTC_ResultSetAttributeFailed: "gpPTC_ResultSetAttributeFailed",  # noqa E221
    gpPTC_ResultSetModeFailed: "gpPTC_ResultSetModeFailed",  # noqa E221
    gpPTC_ResultInvalidParameter: "gpPTC_ResultInvalidParameter",  # noqa E221
    gpPTC_ResultBusy: "gpPTC_ResultBusy",  # noqa E221
    gpPTC_ResultUnsupported: "gpPTC_ResultUnsupported",  # noqa E221
    gpPTC_ResultInvalidChip: "gpPTC_ResultInvalidChip",  # noqa E221
    gpPTC_ResultInvalidRXMode: "gpPTC_ResultInvalidRXMode",  # noqa E221
}
# @}

# @enum gpPTC_SleepModes_t - UInt8
# @{
gpPTC_SleepModeRC = 0x0  # noqa E221
gpPTC_SleepMode32KHz = 0x1  # noqa E221
gpPTC_SleepMode32MHz = 0x2  # noqa E221
gpPTC_SleepModes_t = {
    gpPTC_SleepModeRC: "gpPTC_SleepModeRC",  # noqa E221
    gpPTC_SleepMode32KHz: "gpPTC_SleepMode32KHz",  # noqa E221
    gpPTC_SleepMode32MHz: "gpPTC_SleepMode32MHz",  # noqa E221
}
# @}


# *****************************************************************************
# *                    Functional Macro Definitions
# *****************************************************************************/

# *****************************************************************************
# *                    Macro Definitions
# *****************************************************************************/
GP_PTC_API_VERSION = 1  # noqa E221


# *****************************************************************************
# *                    Structure Definitions
# *****************************************************************************/
class gpPTC_Attribute_t(object):
    """
    @brief gpPTC_Attribute_t:
    @member id
    @member value
    """

    def __init__(self, id, value):
        self.id = id  # gpPTC_AttributeId_t                   # noqa E221
        self.value = value  # Int32                                 # noqa E221


class gpPTC_MACAddress_t(object):
    """
    @brief gpPTC_MACAddress_t:
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
        self.byte0 = byte0  # UInt8                                 # noqa E221
        self.byte1 = byte1  # UInt8                                 # noqa E221
        self.byte2 = byte2  # UInt8                                 # noqa E221
        self.byte3 = byte3  # UInt8                                 # noqa E221
        self.byte4 = byte4  # UInt8                                 # noqa E221
        self.byte5 = byte5  # UInt8                                 # noqa E221
        self.byte6 = byte6  # UInt8                                 # noqa E221
        self.byte7 = byte7  # UInt8                                 # noqa E221


class gpPTC_DeviceAddress_t(object):
    """
    @brief gpPTC_DeviceAddress_t:
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    @member byte5
    """

    def __init__(self, byte0, byte1, byte2, byte3, byte4, byte5):
        self.byte0 = byte0  # UInt8                                 # noqa E221
        self.byte1 = byte1  # UInt8                                 # noqa E221
        self.byte2 = byte2  # UInt8                                 # noqa E221
        self.byte3 = byte3  # UInt8                                 # noqa E221
        self.byte4 = byte4  # UInt8                                 # noqa E221
        self.byte5 = byte5  # UInt8                                 # noqa E221


class gpPTC_chipSerial_t(object):
    """
    @brief gpPTC_chipSerial_t:
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
        self.byte0 = byte0  # UInt8                                 # noqa E221
        self.byte1 = byte1  # UInt8                                 # noqa E221
        self.byte2 = byte2  # UInt8                                 # noqa E221
        self.byte3 = byte3  # UInt8                                 # noqa E221
        self.byte4 = byte4  # UInt8                                 # noqa E221
        self.byte5 = byte5  # UInt8                                 # noqa E221
        self.byte6 = byte6  # UInt8                                 # noqa E221
        self.byte7 = byte7  # UInt8                                 # noqa E221


class gpPTC_serialNumber_t(object):
    """
    @brief gpPTC_serialNumber_t:
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

    def __init__(
        self,
        location,
        year0,
        year1,
        week0,
        week1,
        batch0,
        batch1,
        index0,
        index1,
        index2,
        index3,
    ):
        self.location = location  # UInt8                                 # noqa E221
        self.year0 = year0  # UInt8                                 # noqa E221
        self.year1 = year1  # UInt8                                 # noqa E221
        self.week0 = week0  # UInt8                                 # noqa E221
        self.week1 = week1  # UInt8                                 # noqa E221
        self.batch0 = batch0  # UInt8                                 # noqa E221
        self.batch1 = batch1  # UInt8                                 # noqa E221
        self.index0 = index0  # UInt8                                 # noqa E221
        self.index1 = index1  # UInt8                                 # noqa E221
        self.index2 = index2  # UInt8                                 # noqa E221
        self.index3 = index3  # UInt8                                 # noqa E221


class gpPTC_swVersionNumber_t(object):
    """
    @brief gpPTC_swVersionNumber_t:
    @member major
    @member minor
    @member revision
    @member patch
    @member changelist
    """

    def __init__(self, major, minor, revision, patch, changelist):
        self.major = major  # UInt8                                 # noqa E221
        self.minor = minor  # UInt8                                 # noqa E221
        self.revision = revision  # UInt8                                 # noqa E221
        self.patch = patch  # UInt8                                 # noqa E221
        self.changelist = changelist  # UInt32                                # noqa E221


class gpPTC_partNumber_t(object):
    """
    @brief gpPTC_partNumber_t:
    @member header0
    @member header1
    @member byte0
    @member byte1
    @member byte2
    @member byte3
    @member byte4
    """

    def __init__(self, header0, header1, byte0, byte1, byte2, byte3, byte4):
        self.header0 = header0  # UInt8                                 # noqa E221
        self.header1 = header1  # UInt8                                 # noqa E221
        self.byte0 = byte0  # UInt8                                 # noqa E221
        self.byte1 = byte1  # UInt8                                 # noqa E221
        self.byte2 = byte2  # UInt8                                 # noqa E221
        self.byte3 = byte3  # UInt8                                 # noqa E221
        self.byte4 = byte4  # UInt8                                 # noqa E221


class gpPTC_Parameter_t(object):
    """
    @brief gpPTC_Parameter_t:
    @member value
    """

    def __init__(self, value):
        self.value = value  # Int32                                 # noqa E221


class gpPTC_DiscoveryInfo_t(object):
    """
    @brief gpPTC_DiscoveryInfo_t:
    @member senderMacAddress
    @member DUTMacAddress
    @member RSSI
    """

    def __init__(self, senderMacAddress, DUTMacAddress, RSSI):
        self.senderMacAddress = senderMacAddress  # gpPTC_MACAddress_t                    # noqa E221
        self.DUTMacAddress = DUTMacAddress  # gpPTC_MACAddress_t                    # noqa E221
        self.RSSI = RSSI  # Int16                                 # noqa E221


class gpPTC_ProductName_t(object):
    """
    @brief gpPTC_ProductName_t: The name that uniquely identifies the firmware and ptc dll. The RCC application will look for matching names so that the PTC dll and the firmware can communicate. Limited to 20 chars
    @member name
    """

    def __init__(self, name):
        self.name = name  # char_list                             # noqa E221


class gpPTC_ProductID_t(object):
    """
    @brief gpPTC_ProductID_t:
    @member name
    """

    def __init__(self, name):
        self.name = name  # productIdchar_list                    # noqa E221


# *****************************************************************************
# *                    Public Function Prototypes
# *****************************************************************************/


gpPTC_Subcommands = {}

# Requests
gpPTC_SetClientIDRequest_CmdId = 2  # 0x02  # noqa E221
gpPTC_Discover_CmdId = 3  # 0x03  # noqa E221
gpPTC_ResetDUT_CmdId = 6  # 0x06  # noqa E221
gpPTC_GetDUTApiVersion_CmdId = 7  # 0x07  # noqa E221
gpPTC_GetDUTInfoRequest_CmdId = 8  # 0x08  # noqa E221
gpPTC_SetAttributeRequest_CmdId = 10  # 0x0A  # noqa E221
gpPTC_GetAttributeRequest_CmdId = 11  # 0x0B  # noqa E221
gpPTC_SetModeRequest_CmdId = 12  # 0x0C  # noqa E221
gpPTC_SetByteDataForAttributeRequest_CmdId = 13  # 0x0D  # noqa E221
gpPTC_GetModeRequest_CmdId = 14  # 0x0E  # noqa E221
gpPTC_ExecuteCustomCommand_CmdId = 18  # 0x12  # noqa E221
# Indications
gpPTC_DiscoverIndication_CmdId = 4  # 0x04  # noqa E221
gpPTC_RXPacketIndication_CmdId = 15  # 0x0F  # noqa E221
gpPTC_DataConfirm_CmdId = 16  # 0x10  # noqa E221
gpPTC_EDConfirm_CmdId = 17  # 0x11  # noqa E221

gpPTC_Commands = {
    "gpAPIInfo": [GP_MODULE_ID_PTC, "gpPTCApi", True],
    # Requests
    gpPTC_SetClientIDRequest_CmdId: [
        "gpPTC_SetClientIDRequest",
        0,
        [["clientID", 1]],
        [],
        True,
    ],
    gpPTC_Discover_CmdId: [
        "gpPTC_Discover",
        0,
        [
            ["clientID", 1],
            ["senderMacAddress_byte0", 1],
            ["senderMacAddress_byte1", 1],
            ["senderMacAddress_byte2", 1],
            ["senderMacAddress_byte3", 1],
            ["senderMacAddress_byte4", 1],
            ["senderMacAddress_byte5", 1],
            ["senderMacAddress_byte6", 1],
            ["senderMacAddress_byte7", 1],
        ],
        [],
        True,
    ],
    gpPTC_ResetDUT_CmdId: ["gpPTC_ResetDUT", 0, [], [], True],
    gpPTC_GetDUTApiVersion_CmdId: [
        "gpPTC_GetDUTApiVersion",
        0,
        [],
        [["version", 4]],
        True,
    ],
    gpPTC_GetDUTInfoRequest_CmdId: [
        "gpPTC_GetDUTInfoRequest",
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
            ["ptcVersion_IsNullPtr", 1],
            ["ptcVersion_major", 1],
            ["ptcVersion_minor", 1],
            ["ptcVersion_revision", 1],
            ["ptcVersion_patch", 1],
            ["ptcVersion_changelist", 4],
            ["productID_IsNullPtr", 1],
            ["productID_name", 10],
        ],
        True,
    ],
    gpPTC_SetAttributeRequest_CmdId: [
        "gpPTC_SetAttributeRequest",
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
    gpPTC_GetAttributeRequest_CmdId: [
        "gpPTC_GetAttributeRequest",
        0,
        [
            ["clientID", 1],
            ["numberOfAttr", 1],
            ["attributes_IsNullPtr", 1],
            ["attributes", "numberOfAttr", lambda len: len * 5],
        ],
        [
            ["result", 1],
            ["numberOfAttr", 1],
            ["attributes_IsNullPtr", 1],
            ["attributes", "numberOfAttr", lambda len: len * 5],
        ],
        True,
    ],
    gpPTC_SetModeRequest_CmdId: [
        "gpPTC_SetModeRequest",
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
        [
            ["result", 1],
            ["numberOfExtraParameters", 1],
            ["parameters_IsNullPtr", 1],
            ["parameters", "numberOfExtraParameters", lambda len: len * 4],
        ],
        True,
    ],
    gpPTC_SetByteDataForAttributeRequest_CmdId: [
        "gpPTC_SetByteDataForAttributeRequest",
        0,
        [
            ["clientID", 1],
            ["attributeID", 1],
            ["dataLen", 1],
            ["pData_IsNullPtr", 1],
            ["pData", "dataLen"],
        ],
        [["result", 1]],
        True,
    ],
    gpPTC_GetModeRequest_CmdId: [
        "gpPTC_GetModeRequest",
        0,
        [["clientID", 1], ["modeID", 1]],
        [["OnOff", 1]],
        True,
    ],
    gpPTC_ExecuteCustomCommand_CmdId: [
        "gpPTC_ExecuteCustomCommand",
        0,
        [
            ["clientID", 1],
            ["dataLenIn", 1],
            ["pDataIn_IsNullPtr", 1],
            ["pDataIn", "dataLenIn"],
        ],
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
    gpPTC_DiscoverIndication_CmdId: [
        "gpPTC_DiscoverIndication",
        0,
        [["pDiscoveryInfo_IsNullPtr", 1], ["pDiscoveryInfo", 1]],
        [],
        False,
    ],
    gpPTC_RXPacketIndication_CmdId: [
        "gpPTC_RXPacketIndication",
        0,
        [["datalength", 1], ["payload_IsNullPtr", 1], ["payload", "datalength"]],
        [],
        False,
    ],
    gpPTC_DataConfirm_CmdId: [
        "gpPTC_DataConfirm",
        0,
        [["status", 1], ["packetsSentOK", 2], ["packetsSentError", 2]],
        [],
        False,
    ],
    gpPTC_EDConfirm_CmdId: [
        "gpPTC_EDConfirm",
        0,
        [["result", 1], ["finished", 1], ["EDValue", 2]],
        [],
        False,
    ],
}

gpPTC_CommandName = {
    "setclientidrequest": gpPTC_SetClientIDRequest_CmdId,
    "discover": gpPTC_Discover_CmdId,
    "resetdut": gpPTC_ResetDUT_CmdId,
    "getdutapiversion": gpPTC_GetDUTApiVersion_CmdId,
    "getdutinforequest": gpPTC_GetDUTInfoRequest_CmdId,
    "setattributerequest": gpPTC_SetAttributeRequest_CmdId,
    "getattributerequest": gpPTC_GetAttributeRequest_CmdId,
    "setmoderequest": gpPTC_SetModeRequest_CmdId,
    "setbytedataforattributerequest": gpPTC_SetByteDataForAttributeRequest_CmdId,
    "getmoderequest": gpPTC_GetModeRequest_CmdId,
    "executecustomcommand": gpPTC_ExecuteCustomCommand_CmdId,
}
