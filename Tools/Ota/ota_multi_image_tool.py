import argparse
import hashlib
import sys
import struct
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding, ec, rsa
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from intelhex import IntelHex

from qorvo.security_toolbox.bundles.in_memory import EccSignature

# Constants
HEADER_VERSION_BYTES = 2
HEADER_SIZE_BYTES = 2
SEGMENT_DESCRIPTOR_BYTES = 4 + 4 + 4 + 4 + 32
HEADER_SIGNATURE_BYTES = 256


def parse_args():
    parser = argparse.ArgumentParser(description='Pack or unpack binary images with a header')
    subparsers = parser.add_subparsers(dest='command', required=True, help='Sub-command to execute')

    # Sub-parser for the "create" command
    parser_create = subparsers.add_parser('create', help='Pack binary images with a header')
    parser_create.add_argument('-app', '--app-input-file', type=str, help='Path to app binary image')
    parser_create.add_argument('-sec', '--secondary-input-file', type=str, help='Path to secondary app binary image')
    parser_create.add_argument('-se', '--se-input-file', type=str, help='Path to SE binary image')
    parser_create.add_argument('-bl', '--bl-input-file', type=str, help='Path to BL binary image')
    parser_create.add_argument('--header_version', type=int, required=True, help='Header version')
    parser_create.add_argument('--key', type=str, required=True, help='Path to the private key for signing')
    parser_create.add_argument('--password', type=str, help='Password for the private key')
    parser_create.add_argument('--output', type=str, required=True, help='Output file path')
    parser_create.add_argument("--get_hash",
                               help="Get a SHA256 hash (in binary format) of the OTA header to sign externally",
                               action='store_true')
    parser_create.add_argument("--inject_signature", type=str, help="Inject a signature into the OTA header")

    # Sub-parser for the "show" command
    parser_show = subparsers.add_parser('show', help='Show contents of a packed binary file or an image header')
    parser_show.add_argument('--packed-file', type=str, help='Path to the packed binary file')
    parser_show.add_argument('--key', type=str, required=True, help='Path to the private key PEM file')
    parser_show.add_argument('--password', type=str, help='Password for the private key')
    parser_show.add_argument('--developer-key', type=str, required=True, help='Path to the developer private key file')

    return parser.parse_args()


def load_private_key(key_path, password, key_format='PEM'):
    with open(key_path, 'rb') as key_file:
        if key_format == 'DER':
            private_key = serialization.load_der_private_key(
                key_file.read(),
                password=password.encode() if password else None,
                backend=default_backend()
            )
        else:
            private_key = serialization.load_pem_private_key(
                key_file.read(),
                password=password.encode() if password else None,
                backend=default_backend()
            )
    return private_key


def verify_signature(public_key, key_type, header, signature):
    if key_type == 'rsa':
        public_key.verify(
            signature,
            header,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
    else:
        public_key.verify(
            signature,
            header,
            ec.ECDSA(hashes.SHA256())
        )

    return True


class SignatureVerificationError(RuntimeError):
    pass


class HashVerificationError(RuntimeError):
    pass


def verify_content_hash(data, expected_hash):
    actual_hash = hashlib.sha256(data).digest()
    if actual_hash != expected_hash:
        raise HashVerificationError(f"expected {expected_hash} but got {actual_hash}")


class InputFile:
    def __init__(self, filename: str):
        self.filename = filename
        if filename.endswith(".hex") or filename.endswith(".hex_"):
            ih = IntelHex(filename)
            self.data = ih.tobinarray()
        elif filename.endswith(".bin"):
            with open(filename, 'rb') as f:
                self.data = f.read()
        else:
            raise RuntimeError(f"unsupported file extension: {filename}")

    @property
    def description(self) -> str:
        return self.__class__.__name__

    @property
    def size(self) -> int:
        return len(self.data)

    def get_hash(self):
        return hashlib.sha256(self.data).digest()


class SeFile(InputFile):
    @property
    def version(self) -> int:
        # Version is in big endian
        # Version format: major | minor | patch | header_version
        return struct.unpack_from('>I', self.data, 0x13E)[0] >> 8

    @property
    def identifier(self) -> int:
        return 0xF1


class AppucFile(InputFile):
    @property
    def version(self) -> int:
        # Version is in little endian
        return struct.unpack_from('>I', self.data, 0x174)[0]


class BootloaderFile(AppucFile):
    @property
    def identifier(self) -> int:
        return 0xF2


class ApplicationFile(AppucFile):
    @property
    def identifier(self) -> int:
        return 0xF0


class SecondaryApplicationFile(AppucFile):
    @property
    def identifier(self) -> int:
        return 0xF3


def pack_images(app_input_file=None, se_input_file=None, bl_input_file=None, header_version=1, key=None, password=None, developer_key=None, get_hash=False,
                inject_signature=None, inject_signature_format="raw", output=None, secondary_input_file=None):
    if not (app_input_file or se_input_file or bl_input_file or secondary_input_file):
        print("Error: At least one image (--app-input-file, --se-input-file, --secondary-input-file, or --bl-input-file) must be provided.")
        sys.exit(1)

    ota_parts = []
    if se_input_file:
        ota_parts.append(SeFile(se_input_file))
    if bl_input_file:
        ota_parts.append(BootloaderFile(bl_input_file))
    if secondary_input_file:
        ota_parts.append(SecondaryApplicationFile(secondary_input_file))
    if app_input_file:
        ota_parts.append(ApplicationFile(app_input_file))

    header_version_b = header_version.to_bytes(HEADER_VERSION_BYTES, byteorder='little')
    total_header_size = HEADER_VERSION_BYTES + HEADER_SIZE_BYTES + \
        SEGMENT_DESCRIPTOR_BYTES * len(ota_parts) + HEADER_SIGNATURE_BYTES
    header_number_of_images_b = len(ota_parts).to_bytes(HEADER_SIZE_BYTES, byteorder='little')
    print(f"image count: {len(ota_parts)} {header_number_of_images_b.hex()}")

    segment_descriptors_array = b''
    payload = b''
    print(f"header length: {total_header_size}")
    offset_in_image = total_header_size
    for ota_part in ota_parts:
        segment_descriptor = struct.pack('<IIII32s',
                                         ota_part.identifier,
                                         ota_part.size,
                                         ota_part.version,
                                         offset_in_image,
                                         ota_part.get_hash())
        assert len(segment_descriptor) == SEGMENT_DESCRIPTOR_BYTES
        segment_descriptors_array += segment_descriptor
        print(f"{offset_in_image:#x}: {ota_part.description} {ota_part.size} bytes / {ota_part.identifier:#x} - {ota_part.filename} - {ota_part.version:#x}")
        offset_in_image += ota_part.size
        payload += ota_part.data

    header = header_version_b + header_number_of_images_b + segment_descriptors_array

    if get_hash:
        header_hash = hashlib.sha256(header).digest()
        with open(output, 'wb') as f:
            f.write(header_hash)
        return

    if inject_signature:
        with open(inject_signature, 'rb') as sig_file:
            sig_bytes = sig_file.read()
            if inject_signature_format == 'asn1':
                sig_bytes = EccSignature.from_asn1(sig_bytes).to_bytes()
    else:
        if developer_key is not None:
            private_key = load_private_key(developer_key, None, 'DER')
        else:
            private_key = load_private_key(key, password, 'PEM')

        header_signature_der = private_key.sign(
            header,
            ec.ECDSA(hashes.SHA256())
        )
        int_r, int_s = decode_dss_signature(header_signature_der)
        sig_bytes = int_r.to_bytes(32, 'big') + int_s.to_bytes(32, 'big')

    # Signature padding to 256 bytes
    sig_bytes += b'\x00' * (256 - len(sig_bytes))

    with open(output, 'wb') as f:
        f.write(header)
        f.write(sig_bytes)
        f.write(payload)


def read_packed_file(packed_file_path):
    with open(packed_file_path, 'rb') as f:
        return f.read()


def unpack_images(key=None, password=None, packed_file=None):
    private_key, key_type = load_private_key(key, password)
    public_key = private_key.public_key()

    packed_data = read_packed_file(packed_file)

    header_version_b = packed_data[:HEADER_VERSION_BYTES]
    header_number_of_images_b = packed_data[HEADER_VERSION_BYTES:HEADER_VERSION_BYTES + HEADER_SIZE_BYTES]
    header_length = int.from_bytes(header_number_of_images_b, byteorder='little') * \
        SEGMENT_DESCRIPTOR_BYTES + HEADER_VERSION_BYTES + HEADER_SIZE_BYTES
    header = packed_data[:header_length]

    signature_start = header_length
    signature_end = signature_start + 256
    signature = packed_data[signature_start:signature_end].rstrip(b'\x00')

    if not verify_signature(public_key, key_type, header, signature):
        sys.exit(1)
    else:
        print(f"Signature verification successful. Signature: {signature.hex()} / len: {len(signature)}")

    offset = 4
    images = []
    while offset < header_length:
        identifier = packed_data[offset]
        size = int.from_bytes(packed_data[offset + 1:offset + 4], byteorder='little')
        version = int.from_bytes(packed_data[offset + 4:offset + 8], byteorder='little')
        data_offset = int.from_bytes(packed_data[offset + 8:offset + 12], byteorder='little')
        print(data_offset, size)
        content_hash = packed_data[offset + 12:offset + 44]
        images.append((identifier, size, version, data_offset, content_hash))
        offset += 44

    for identifier, size, version, data_offset, content_hash in images:
        image_data = packed_data[data_offset:data_offset + size]
        actual_hash = hashlib.sha256(image_data).digest()
        if actual_hash != content_hash:
            print(f"Content hash mismatch for image with identifier 0x{identifier:x}.")
            continue

        print(f"Image Identifier: {hex(identifier)}")
        print(f"Size: {size} bytes")
        print(f"Version: {version} (0x{version:x})")
        print(f"Offset: {data_offset} (0x{data_offset:x})")
        print(f"Content Hash: {content_hash.hex()}")
        print(f"Image Data (first 64 bytes): {image_data[:64].hex()}\n")


if __name__ == '__main__':
    args = parse_args()
    if args.command == 'create':
        pack_images(app_input_file=args.app_input_file, se_input_file=args.se_input_file, bl_input_file=args.bl_input_file,
                    header_version=args.header_version, key=args.key, password=args.password, developer_key=args.developer_key, output=args.output, secondary_input_file=args.secondary_input_file)
    elif args.command == 'show':
        unpack_images(key=args.developer_key if args.developer_key is not None else args.key,
                      password=args.password, packed_file=args.packed_file)
    else:
        raise RuntimeError("invalid command given")
