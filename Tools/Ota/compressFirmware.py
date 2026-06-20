"""
This tool will compress a binary format firmware file with lzma after applying padding up to a page
size multiple. .
This tool will also calculate the CRC over the relevant part of the binary file and patch it
into the Loaded User License. Alternatively, if secure boot is targetted, a signature will be
created and injected into the Extended User License
"""
import argparse
import os
import struct
import codecs
import sys
import logging
import hashlib
import pylzma
from typing import Tuple
from dataclasses import dataclass, field
from enum import IntEnum
from pathlib import Path

from binascii import crc32
from ecdsa import NIST256p, NIST192p
from qorvo.security_toolbox.firmware_packer import file_operations as fw_file_ops
from qorvo.security_toolbox.firmware_packer.key_operations import Signature
from qorvo.security_toolbox.bundles.in_memory import EccSignature

if os.path.isfile(os.path.join(os.path.dirname(__file__), "crypto_utils.py")):
    # In the Matter DK, all python modules are exported to this script's directory
    import crypto_utils as crypto_utils
else:
    # When running from the Qorvo codebase, use the dependencies from original paths
    moduleroot = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
    sys.path.append(os.path.join(moduleroot, "comps"))
    from crypto_utils import crypto_utils
    sys.path.remove(os.path.join(moduleroot, "comps"))

    # Determine if we are an .exe or not
    if not getattr(sys, 'frozen', False):
        current_dir = os.path.dirname(os.path.realpath(__file__))
        parent_dir = os.path.dirname(current_dir)
        sys.path.append(os.path.join(parent_dir, "..", "..", "..", "..", "Env", "vless", "gppy_vless", "inf"))
        # pylint: disable-next=import-error
        from getEnvVersion import getEnvVersion

        try:
            envVersion = getEnvVersion()
            envPath = os.path.join(parent_dir, "..", "..", "..", "..", "Env", envVersion)
        except Exception as e:
            # Fallback to ENV_PATH
            logging.warning("getEnvVersion() failed, falling back to ENV_PATH")
            envPath = os.path.abspath(os.environ.get('ENV_PATH', ""))

        sec_deps_path = os.path.join(envPath, "gppy", "tools", "sec")
        sys.path.append(sec_deps_path)


# CONSTANTS
USER_LICENSE_CRC_VALUE_OFFSET = 0x10
USER_LICENSE_VPP_OFFSET = 0x1C

USER_LICENSE_LOAD_COMPLETED_OFFSET = 0x78
USER_LICENSE_FRESHNESS_COUNTER_OFFSET = 0x7F

EXTENDED_USER_LICENSE_OFFSET = 0x80
EXTENDED_USER_LICENSE_SIGNATURE_OFFSET = (EXTENDED_USER_LICENSE_OFFSET + 0x28)
EXTENDED_USER_LICENSE_SIGNATURE_SIZE = 64

EXTENDED_USER_LICENSE_SECTION_1_ADDRESS_OFFSET = 0x98
EXTENDED_USER_LICENSE_SECTION_1_SIZE_OFFSET = (EXTENDED_USER_LICENSE_SECTION_1_ADDRESS_OFFSET + 4)
EXTENDED_USER_LICENSE_SECTION_2_ADDRESS_OFFSET = (EXTENDED_USER_LICENSE_SECTION_1_SIZE_OFFSET + 4)
EXTENDED_USER_LICENSE_SECTION_2_SIZE_OFFSET = (EXTENDED_USER_LICENSE_SECTION_2_ADDRESS_OFFSET + 4)

LICENSE_SIZE = 0x100
APPUC_FW_HEADER_SIZE = 0x200

CRC_START_ADDRESS_MSB_OOB = 0xFFFF
CRC_START_ADDRESS_LSB_OOB = 0xFF
CRC_TYPE_32BIT_CRC = 1
CRC_SIZE_OOB = 0X00000000

EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE = 0xFFFFFFFF


class ImageType(IntEnum):
    App = 0x0000000
    BootloaderStage1 = 0xC960730D
    BootloaderStage2 = 0x78454A9E
    CompressedApplicationUpdate = 0x8000000


@dataclass
class CompressFirmwareArguments:
    """helper to enforce type checking on argparse output"""
    input: str
    output: str
    add_crc: bool
    license_offset: int
    page_size: int
    sector_size: int
    pem: str
    pem_password: str
    x25519: bool
    x25519_private_key_binfile: str
    compression: str
    prune_only: bool
    ota_offset: int
    no_extended_user_license: bool
    xp400x: bool
    developer_key: str
    signature_algorithm: str
    get_hash: bool
    inject_signature: str
    inject_signature_format: str
    # application namespace variables
    license_sector_file: str = field(init=False)
    raw_app_content_file: str = field(init=False)
    jumptableFile: str = field(init=False)
    paddedBinFile: str = field(init=False)
    compressed_payload_file: str = field(init=False)
    hash_file: str = field(init=False)
    license_sector: bytearray = field(init=False)
    section1_addr: int = EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE
    section1_size: int = field(init=False)
    section2_addr: int = EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE
    section2_size: int = field(init=False)
    jumptables_data: bytes = field(init=False)
    raw_app_contents: bytes = field(init=False)
    padded_data: bytes = field(init=False)
    compressed_app_data: bytes = field(init=False)
    hash_data: bytes = field(init=False)


def read_input_file(args: CompressFirmwareArguments):
    """read the input file"""
    # Read file contents
    with open(args.input, 'rb') as binary_input_file:
        binary_input = binary_input_file.read(-1)

    if args.xp400x:
        args.license_sector = extract_license_sector_xp4002(args, binary_input)
    else:
        args.license_sector = extract_license_sector(args, binary_input)

        if not args.no_extended_user_license:
            parse_section_sizes(args)

        if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
            args.jumptables_data = extract_jumptables_contents(args, binary_input)

    args.raw_app_contents = extract_app_contents(args, binary_input)


def extract_license_sector(args: CompressFirmwareArguments, binary_input) -> bytearray:
    """Return the license sector"""
    return bytearray(binary_input[args.license_offset:
                                  args.license_offset + LICENSE_SIZE])


def extract_license_sector_xp4002(args: CompressFirmwareArguments, binary_input) -> bytearray:
    """Return the license sector"""
    return unpack_appuc_header(bytearray(binary_input[args.license_offset:
                                                      args.license_offset + APPUC_FW_HEADER_SIZE]))


def parse_section_sizes(args: CompressFirmwareArguments) -> None:
    """parse section values from the license sector"""
    args.section1_addr = struct.unpack_from('I', args.license_sector, EXTENDED_USER_LICENSE_SECTION_1_ADDRESS_OFFSET)[0]
    args.section1_size = struct.unpack_from('I', args.license_sector, EXTENDED_USER_LICENSE_SECTION_1_SIZE_OFFSET)[0]
    args.section2_addr = struct.unpack_from('I', args.license_sector, EXTENDED_USER_LICENSE_SECTION_2_ADDRESS_OFFSET)[0]
    args.section2_size = struct.unpack_from('I', args.license_sector, EXTENDED_USER_LICENSE_SECTION_2_SIZE_OFFSET)[0]


def extract_jumptables_contents(args: CompressFirmwareArguments, binary_input: bytes) -> bytearray:
    """Extract the jumptables content from the input data."""
    # section1 starts after the userlicense, compensate for in offset calculation
    section1_user_license_skip_length = 0x100
    start_offset = args.section1_addr - args.license_offset - section1_user_license_skip_length
    jumptables_start_offset = 0x800 - start_offset
    return bytearray(binary_input[jumptables_start_offset:
                                  jumptables_start_offset + args.section2_size])


def extract_app_contents(args: CompressFirmwareArguments, binary_input: bytes) -> bytearray:
    """Extract the application section from the input data."""
    return bytearray(binary_input[args.license_offset:len(binary_input)])


def write_intermediate_parts(args: CompressFirmwareArguments):
    """write out individual steps for debugging purposes"""
    logging.info("Dumping intemediate parts")

    if args.get_hash:
        with open(args.hash_file, 'wb') as hash_file:
            hash_file.write(args.hash_data)
            logging.info("Written hash to %s", args.hash_file)
    else:
        if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
            # If jumptables are present
            with open(args.jumptableFile, 'wb') as jump_tables_file:
                jump_tables_file.write(bytes(args.jumptables_data))
            logging.info("Written jumptables to %s", args.jumptableFile)

        with open(args.license_sector_file, 'wb') as license_file:  # obsolete intermediate
            license_file.write(bytes(args.license_sector))
            logging.info("Written license sector to %s", args.license_sector_file)

        with open(args.raw_app_content_file, 'wb') as app_bin_file:
            app_bin_file.write(bytes(args.raw_app_contents))
            logging.info("Written application to %s", args.raw_app_content_file)

        with open(args.paddedBinFile, 'wb') as padded_bin_file:
            padded_bin_file.write(bytes(args.padded_data))
            logging.info("Written padded file to %s", args.paddedBinFile)

        if args.compression != 'none':
            with open(args.compressed_payload_file, 'wb') as compressed_payload_file:
                compressed_payload_file.write(bytes(args.compressed_app_data))
                logging.info("Written compressed payload to %s", args.compressed_payload_file)


def add_crc(args: CompressFirmwareArguments):
    """Calculate crc over data"""
    data = bytearray()

    # Add application part section 1
    data.extend(args.compressed_app_data)

    # Add application part section 2
    if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
        data.extend(args.jumptables_data)

    # Add license part
    for i in range(USER_LICENSE_VPP_OFFSET, LICENSE_SIZE):
        # Mask out Load Complete MW
        if i in range(USER_LICENSE_LOAD_COMPLETED_OFFSET, USER_LICENSE_LOAD_COMPLETED_OFFSET + 4):
            # Mask out data
            data.append(0x00)
        elif i is USER_LICENSE_FRESHNESS_COUNTER_OFFSET:
            # Mask out data
            data.append(0x00)
        else:
            data.append(args.license_sector[i])

    crcvalue = (~crc32(memoryview(data)) ^ 0xFFFFFFFF) & 0xFFFFFFFF

    # Write checksum to license area
    struct.pack_into('IHBBI', args.license_sector, USER_LICENSE_CRC_VALUE_OFFSET, crcvalue,
                     CRC_START_ADDRESS_MSB_OOB, CRC_START_ADDRESS_LSB_OOB, CRC_TYPE_32BIT_CRC, CRC_SIZE_OOB)
    logging.info("Packed CRC %#x", crcvalue)


def calculate_signature(image: bytes, pemfile_path: str, password):
    """Calculate the signature over a specified image using the private key
    contained in the specified file.
    """
    # Retrieve information from PEM file
    logging.info("Reading PEM file: %s", os.path.basename(pemfile_path))

    (pem_curve, private_key, public_key) = crypto_utils.getPrivatePublicKeysFromPEMFile(pemfile_path,
                                                                                        password)

    # Assure correct curves and hashes are being used together

    if pem_curve == "secp192r1":
        curve = NIST192p
        hash_function = hashlib.sha1
    elif pem_curve == "secp256r1":
        curve = NIST256p
        hash_function = hashlib.sha256
    else:
        assert False

    logging.info("Using signing curve: %s", curve.name)
    logging.info("PEM file has curve: %s", pem_curve)

    logging.info("Hashing and signing image")
    signature = crypto_utils.signMessage(image, private_key, curve=curve, hashfunc=hash_function)

    logging.info("=====================================")
    logging.info("Signature to be put in user license:")
    logging.info(crypto_utils.getCCodeBuffer(signature, "signature"))
    # crypto_utils.printCCodeBuffer(private_key, "private_key")
    logging.info("=====================================")
    logging.info("Public key to be put in bootloader:")
    logging.info(crypto_utils.getCCodeBuffer(public_key, "public_key"))
    logging.info("=====================================")

    return signature, public_key, curve, hash_function


def add_signature(args: CompressFirmwareArguments):
    """Add a signature over a specified image to the license sector."""
    # Gather data to calculate signature
    image = b''

    # Add application part section 1
    for byte in args.compressed_app_data:
        image += struct.pack("B", byte)

    if args.x25519:
        """ The ROM_aes_mmo_update function, which is used to hash the section, requires a multiple of 16-bytes as
        data to be hashed, so if the size of the section is not a multiple of 16 bytes, we add zero padding to get
        to this multiple"""

        remainder = len(image) % 16
        if remainder != 0:
            for i in range(0, 16 - remainder):
                image += struct.pack("B", 0x00)

            logging.info("Section size not a multiple of 16 as required by ROM_aes_mmo_update, adding %d zero bytes",
                         (16 - remainder))

        assert len(image) % 16 == 0

    # Add application part section 2 if needed
    if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
        for byte in args.jumptables_data:
            image += struct.pack("B", byte)

    if args.x25519:
        """ The ROM_aes_mmo_update function, which is used to hash the section, requires a multiple of 16-bytes as
        data to be hashed, so if the size of the section is not a multiple of 16 bytes, we add zero padding to get
        to this multiple"""

        remainder = len(image) % 16
        if remainder != 0:
            for i in range(0, 16 - remainder):
                image += struct.pack("B", 0x00)

            logging.info("Section size not a multiple of 16 as required by ROM_aes_mmo_update, adding %d zero bytes",
                         (16 - remainder))

        assert len(image) % 16 == 0

    # Add license part
    start_load_completed_mw_offset = USER_LICENSE_LOAD_COMPLETED_OFFSET
    end_load_completed_mw_offset = USER_LICENSE_LOAD_COMPLETED_OFFSET + 4
    start_freshness_counter_offset = USER_LICENSE_FRESHNESS_COUNTER_OFFSET - 3
    stop_freshness_counter_offset = USER_LICENSE_FRESHNESS_COUNTER_OFFSET + 1
    for i in range(USER_LICENSE_VPP_OFFSET, EXTENDED_USER_LICENSE_SIGNATURE_OFFSET):
        if (start_load_completed_mw_offset <= i < end_load_completed_mw_offset) or \
           (start_freshness_counter_offset <= i < stop_freshness_counter_offset):
            pass
        else:
            image += struct.pack("B", args.license_sector[i])

    if args.x25519:
        # pylint: disable-next=import-error
        from aes_mmo import aes_mmo_hash
        # pylint: disable-next=import-error
        from x25519 import x25519_sign_and_return_response

        aes_mmo_hash_buf = bytearray(image)

        logging.info("AES-MMO hashing %d bytes total", len(aes_mmo_hash_buf))

        aes_mmo_result = aes_mmo_hash(aes_mmo_hash_buf)

        logging.info("aes_mmo_hash:")
        logging.info(' '.join(f'{byte_value:#02x}' for byte_value in aes_mmo_result))

        with open(args.x25519_private_key_binfile, "rb") as file_contents:
            private_key = file_contents.read(-1)

        signature = x25519_sign_and_return_response(aes_mmo_result, bytearray(private_key))
    else:
        signature, public_key, curve, hash_function = calculate_signature(
            image,
            args.pem,
            args.pem_password
        )

        if crypto_utils.verifyMessage(image, public_key, signature, curve=curve, hashfunc=hash_function):
            logging.info("SUCCESS: Message OK")
        else:
            logging.error("Message NOK")

    args.license_sector[
        EXTENDED_USER_LICENSE_SIGNATURE_OFFSET:
        EXTENDED_USER_LICENSE_SIGNATURE_OFFSET + EXTENDED_USER_LICENSE_SIGNATURE_SIZE
    ] = bytearray(signature)


def fill_license(args: CompressFirmwareArguments):
    """Transform the user license into the compressed user license by adjusting content"""
    logging.info("Filling the compressed license")

    # Organize ota image as follows:
    # 0x1000 bytes before ota_space is reserved for jumptables
    # first sector will be used for user license
    # the actual compressed image starts afterwards and length should be set to compressed image size
    section1_addr = args.ota_offset + LICENSE_SIZE
    section1_size = len(args.compressed_app_data)
    section2_size = args.section2_size
    if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
        section2_addr = args.ota_offset - section2_size
    else:
        section2_addr = args.section2_addr

    # pack the calculated values into the binary data + set vpp to ota_area location offset
    struct.pack_into('IIII', args.license_sector, EXTENDED_USER_LICENSE_SECTION_1_ADDRESS_OFFSET,
                     section1_addr, section1_size, section2_addr, section2_size)
    logging.info("Packed sections in compressed license: section1 %#x:%#x, section2 %#x:%#x",
                 section1_addr, section1_size, section2_addr, section2_size)


def byte_factory(length: int):
    return bytes(length)


@dataclass
class VersionInfo:
    @staticmethod
    def default_version_tuple():
        return (0, 0, 0, 0)

    __formatstr__ = "<BBBBIII"
    app_version: Tuple[int, int, int, int] = field(default_factory=default_version_tuple)
    app_descriptor: int = 0
    app_changelist_specifier: int = 0
    reserved1: int = field(default=0, repr=False)

    def pack(self):
        data = struct.pack(self.__formatstr__,
                           self.app_version[0],
                           self.app_version[1],
                           self.app_version[2],
                           self.app_version[3],
                           self.app_descriptor,
                           self.app_changelist_specifier,
                           self.reserved1
                           )
        assert len(data) == 16
        return data

    @classmethod
    def unpack(cls, data: bytes):
        (version_1,
            version_2,
            version_3,
            version_4,
            app_descriptor,
            app_changelist_specifier,
            reserved1
         ) = struct.unpack(cls.__formatstr__, data)
        return VersionInfo(
            (version_1, version_2, version_3, version_4),
            app_descriptor,
            app_changelist_specifier,
            reserved1
        )

    def __bytes__(self):
        return self.pack()


@dataclass
class AuthenticatedHeader:
    __formatstr__ = "<IIBB2sIIIB3sI32s"
    firmware_size: int = 0
    anti_rollback_firmware_version: int = 0
    encrypt_flag: bool = 0
    firmware_root_key_index: int = 0
    reserved1: int = field(default=0, repr=False)
    vector_pointer_pointer: int = 0
    image_type: ImageType = field(default_factory=ImageType.App)
    load_completed_magic_word: int = 0
    header_version: int = 0
    reserved2: int = field(default=0, repr=False)
    image_destination_address: int = 0
    encrypted_firmware_encryption_key: bytes = field(default_factory=byte_factory(32), repr=False)

    def pack(self):
        data = struct.pack(self.__formatstr__,
                           self.firmware_size,
                           self.anti_rollback_firmware_version,
                           1 if self.encrypt_flag else 0,
                           self.firmware_root_key_index,
                           bytes(2),  # reserved1
                           self.vector_pointer_pointer,
                           self.image_type,
                           self.load_completed_magic_word,
                           self.header_version,
                           bytes(3),  # reserved2
                           self.image_destination_address,
                           self.encrypted_firmware_encryption_key
                           )
        assert len(data) == 64
        return data

    @classmethod
    def unpack(cls, data: bytes):
        (firmware_size,
            anti_rollback_firmware_version,
            encrypt_flag,
            firmware_root_key_index,
            reserved1,
            vector_pointer_pointer,
            image_type,
            load_completed_magic_word,
            header_version,
            reserved2,
            image_destination_address,
            encrypted_firmware_encryption_key
         ) = struct.unpack(cls.__formatstr__, data)
        return AuthenticatedHeader(
            firmware_size,
            anti_rollback_firmware_version,
            encrypt_flag,
            firmware_root_key_index,
            reserved1,
            vector_pointer_pointer,
            ImageType(image_type),
            load_completed_magic_word,
            header_version,
            reserved2,
            image_destination_address,
            encrypted_firmware_encryption_key
        )

    def __bytes__(self):
        return self.pack()


@dataclass
class AuthenticatedAppucOtaHeader:
    __formatstr__ = "244s64s64s16s112s12s"
    developer_certificate: bytes = field(default_factory=byte_factory(244))
    signature: bytes = field(default_factory=byte_factory(64))
    authenticated_header: bytes = field(default_factory=AuthenticatedHeader)
    version_info: bytes = field(default_factory=VersionInfo)
    reserved2: bytes = field(default_factory=byte_factory(112), repr=False)
    padding: bytes = field(default_factory=byte_factory(12), repr=False)

    def pack(self):
        data = self.developer_certificate + self.signature + \
            bytes(self.authenticated_header) + bytes(self.version_info) + self.reserved2 + self.padding
        assert len(data) == 512
        return data

    @classmethod
    def unpack(cls, data: bytes):
        (developer_certificate,
            signature,
            authenticated_header,
            version_info,
            reserved2,
            padding
         ) = struct.unpack(cls.__formatstr__, data)
        return AuthenticatedAppucOtaHeader(
            developer_certificate,
            signature,
            AuthenticatedHeader.unpack(authenticated_header),
            VersionInfo.unpack(version_info),
            reserved2,
            padding
        )

    def __bytes__(self):
        return self.pack()


@dataclass
class UnAuthenticatedAppucOtaHeader:
    __formatstr__ = "<I304s64s16s112s12s"
    magic_word: int = 0x5733416B
    reserved1: bytes = field(default_factory=byte_factory(304), repr=False)
    authenticated_header: bytes = field(default_factory=AuthenticatedHeader)
    version_info: bytes = field(default_factory=VersionInfo)
    reserved2: bytes = field(default_factory=byte_factory(124), repr=False)
    padding: bytes = field(default_factory=byte_factory(12), repr=False)

    def pack(self):
        data = struct.pack("<I", self.magic_word) \
            + self.reserved1 \
            + bytes(self.authenticated_header) \
            + bytes(self.version_info) \
            + self.reserved2 \
            + self.padding
        assert len(data) == 512
        return data

    @classmethod
    def unpack(cls, data: bytes):
        (magic_word,
            reserved1,
            authenticated_header,
            version_info,
            reserved2,
            padding
         ) = struct.unpack(cls.__formatstr__, data)
        return UnAuthenticatedAppucOtaHeader(
            magic_word,
            reserved1,
            AuthenticatedHeader.unpack(authenticated_header),
            VersionInfo.unpack(version_info),
            reserved2,
            padding
        )

    def __bytes__(self):
        return self.pack()


def unpack_appuc_header(data: bytes):
    if bytes(data[0:4]) == struct.pack("I", UnAuthenticatedAppucOtaHeader.magic_word):
        return UnAuthenticatedAppucOtaHeader.unpack(data)
    else:
        return AuthenticatedAppucOtaHeader.unpack(data)


def add_appuc_ota_header(args: CompressFirmwareArguments):
    args.license_sector.authenticated_header.firmware_size = len(args.compressed_app_data)
    args.license_sector.authenticated_header.image_type = ImageType.CompressedApplicationUpdate
    logging.error(args.license_sector.version_info)
    message = (bytes(args.license_sector.authenticated_header) +
               bytes(args.license_sector.version_info) +
               args.license_sector.reserved2 +
               args.compressed_app_data)

    if args.get_hash:
        args.hash_data = hashlib.sha256(message).digest()
    elif args.developer_key:
        signing_key = fw_file_ops.read_key("ecdsa_p256", Path(args.developer_key))
        logging.info("Add signature to appuc OTA header")
        args.license_sector.signature = Signature.sign(args.signature_algorithm, signing_key, message)
    elif args.inject_signature:
        with open(args.inject_signature, 'rb') as sig_file:
            args.license_sector.signature = sig_file.read()
            if args.inject_signature_format == "asn1":
                # Convert ASN.1 signature to raw format
                args.license_sector.signature = EccSignature.from_asn1(args.license_sector.signature).to_bytes()
    else:
        logging.warning("WARNING: Neither developer key (--developer-key) nor (--get-hash) set!")


def write_combined_output_image(args: CompressFirmwareArguments):
    """Combine all binary files to one output file"""
    logging.info("Combining all binary files to one output file")

    # Overwrite file with compressed data with data in the right order
    with open(args.output, 'wb') as combined_file:
        if args.section2_addr != EXTENDED_USER_LICENSE_SECTION_NOT_IN_USE:
            combined_file.write(bytes(args.jumptables_data))
        if args.license_offset is not None and not args.prune_only:
            combined_file.write(bytes(args.license_sector))
        combined_file.write(args.compressed_app_data)


def parse_command_line_arguments() -> CompressFirmwareArguments:
    """Parse the command line arguments of the application."""
    def base_16_int(string):
        assert string.startswith("0x"), f"expected hexadecimal string does not start with 0x: {string}"
        return int(string, 16)

    def str_as_bytes(string):
        return codecs.encode(string, 'UTF-8')

    parser = argparse.ArgumentParser()

    parser.add_argument("--input",
                        help="path to bin file to be compressed")
    parser.add_argument("--output",
                        help="output bin file to be written")

    parser.add_argument("--add_crc",
                        help="add crc calculation",
                        action='store_true')

    parser.add_argument("--license_offset",
                        type=base_16_int,
                        help="offset relative to the start of the file where the user license begins")

    parser.add_argument("--ota_offset",
                        type=base_16_int,
                        help="offset of the ota area relative to the start of the flash")

    parser.add_argument("--page_size",
                        type=base_16_int,
                        help="the page size used in the target device flash")
    parser.add_argument("--sector_size",
                        type=base_16_int,
                        help="the sector size used in the target device flash")

    parser.add_argument("--pem",
                        help="path to PEM file to be signed")
    parser.add_argument("--pem_password",
                        type=str_as_bytes,
                        help="optional PEM file password")

    parser.add_argument("--x25519",
                        help="use AES-MMO + x25519 signing",
                        action='store_true')

    parser.add_argument("--x25519_private_key_binfile",
                        help="private key used for signing")

    parser.add_argument("--compression",
                        choices=['lzma', 'none'],
                        default="lzma",
                        help="compression type (default to lzma)")

    parser.add_argument("--prune_only",
                        help="prune unneeded sections; don't add an upgrade user license (external storage scenario)",
                        action='store_true')
    parser.add_argument("--no-extended-user-license",
                        help="no extended user license in use",
                        default=False,
                        action='store_true')

    parser.add_argument("--xp400x",
                        help="Use XP400x data structures",
                        default=False,
                        action='store_true')

    parser.add_argument("--get_hash",
                        help="Get a SHA256 hash (in binary format) of the OTA firmware image to sign externally",
                        default=False,
                        action='store_true')

    parser.add_argument("--inject_signature",
                        help="Inject a signature into the OTA firmware image",
                        default=None)

    parser.add_argument("--inject_signature_format",
                        choices=["raw", "asn1"],
                        default="asn1",
                        help="The format of signature to inject (default to ASN.1).")

    # xp400x (appuc header) signing options
    parser.set_defaults(signature_algorithm="ecdsa_p256")
    parser.add_argument("--developer-key",
                        help="sign firmware using developer's signing key",
                        metavar="FILE.sk")
    args = parser.parse_args()

    if args.developer_key and args.get_hash:
        raise ValueError("Cannot use both --developer-key and --get-hash options simultaneously.")

    if args.get_hash and args.output:
        raise ValueError("Cannot use both --get-hash and --output options simultaneously.")

    if args.developer_key and args.inject_signature:
        raise ValueError("Cannot use both --developer-key and --inject-signature options simultaneously.")

    if args.inject_signature and args.get_hash:
        raise ValueError("Cannot use both --inject-signature and --get_hash options simultaneously.")

    return CompressFirmwareArguments(**vars(args))


def validate_command_line_arguments(args: CompressFirmwareArguments) -> None:
    if not args.input:
        logging.error("Supply BIN file path")
        sys.exit(-1)

    if args.pem:
        if not args.pem_password:
            logging.error("Supply PEM file password path")
            sys.exit(-1)

    if not args.output and not args.get_hash:
        input_fn_parts = os.path.splitext(args.input)
        args.output = input_fn_parts[0] + ".compressed" + input_fn_parts[1]
        logging.warning("Setting default output file = %s", args.output)
    if args.license_offset is None:
        logging.info("Not using license approach")

    # Set file variables
    args.license_sector_file = os.path.splitext(args.input)[0] + ".license.bin"
    args.raw_app_content_file = os.path.splitext(args.input)[0] + ".application-extracted.bin"
    args.jumptableFile = os.path.splitext(args.input)[0] + ".jumptables.bin"
    args.paddedBinFile = os.path.splitext(args.input)[0] + ".application-after-padding.bin"
    args.compressed_payload_file = os.path.splitext(args.input)[0] + ".application-compressed-out.bin.lzma"
    if args.get_hash:
        args.hash_file = os.path.splitext(args.input)[0] + ".hash.bin"


def lzma_compress(input_data: bytes) -> bytes:
    """Compress with lzma and add a header containing the uncompressed size."""

    dictionary_size = 1 << 16
    lc_value = 3  # -lc3 : set number of literal context bits : [0, 8] : default = 3
    lp_value = 0  # -lp0 : set number of literal pos bits : [0, 4] : default = 0
    pb_value = 2  # -pb2 : set number of pos bits : [0, 4] : default = 2

    properties = (pb_value * 5 + lp_value) * 9 + lc_value
    assert properties == 0x5d, f"properties is {properties:x} instead"

    header = struct.pack("<BIQ",            # little-endian
                         properties,        # byte
                         dictionary_size,   # unsigned int
                         len(input_data),   # unsigned long long
                         )

    compressed_data = b''
    compressed_data += pylzma.compress(input_data, dictionary=16, fastBytes=128,
                                       literalContextBits=lc_value, literalPosBits=lp_value, posBits=pb_value, eos=0)

    decompressed_data = pylzma.decompress(compressed_data, maxlength=len(input_data))

    if decompressed_data == input_data:
        logging.info("LZMA compressor verify OK")
    else:
        logging.info("LZMA verification failed")
        # return 0

    logging.info("input data length: %#x output data length: %#x", len(decompressed_data), len(compressed_data))

    # Please note that the compressed data is not compatible to the lzma.exe command
    # line utility!  To get compatible data, you can use the following
    output = b''
    output += header
    output += compressed_data[5:]
    return output


def pad_to_page_size(page_size, input_data: bytes) -> bytes:
    """Pad the file to a page size multiple by adding 0's."""
    output = b''
    logging.info("length input data: %#x", len(input_data))
    output += input_data
    bytes_in_incomplete_page = len(input_data) % page_size
    if bytes_in_incomplete_page != 0:
        padding_size = page_size - bytes_in_incomplete_page
        output += b'\x00' * padding_size
    return output


def main(args: CompressFirmwareArguments):
    """the entry point of the application."""
    logging.basicConfig(level=logging.INFO)

    validate_command_line_arguments(args)

    if args.license_offset is not None:
        logging.info(f"license_offset is {args.license_offset:#x}")
        # Split off the jumptables and user license if needed
        read_input_file(args)
        data_to_pad = bytes(args.raw_app_contents)
    else:
        with open(args.input, 'rb') as input_file:
            data_to_pad = bytes(input_file.read(-1))

    args.padded_data = pad_to_page_size(args.page_size, data_to_pad)

    if args.compression == 'lzma':
        logging.info(f"Compressing {len(args.padded_data)} bytes with LZMA")
        args.compressed_app_data = lzma_compress(args.padded_data)
    elif args.compression == 'none':
        args.compressed_app_data = args.padded_data
    else:
        raise RuntimeError(f'Unsupported compression {args.compression}')

    if args.license_offset is None:
        with open(args.output, 'wb') as output_file:
            output_file.write(args.compressed_app_data)
    else:
        if not args.prune_only:
            if args.xp400x:
                add_appuc_ota_header(args)
            else:
                fill_license(args)

        if args.add_crc:
            add_crc(args)

        if args.pem or args.x25519:
            if not args.xp400x:
                add_signature(args)

        write_intermediate_parts(args)
        if not args.get_hash:
            write_combined_output_image(args)


if __name__ == "__main__":
    args = parse_command_line_arguments()
    main(args)
