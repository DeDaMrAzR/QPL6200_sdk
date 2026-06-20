#!/bin/sh

set -e

SCRIPT_DIR="$(dirname "$(realpath "$0")")"

# determine python interpreter path
if [ -f "`which python3`" ]; then
    PYTHON="`which python3`"
else
    if [ -f "`which python`" ]; then
        PYTHON="`which python`"
    else
        echo "No python interpreter found."
        exit 1
    fi
fi

# check python interpreter version
PYTHON_VERSION=`"${PYTHON}" --version | cut -d\  -f 2`
PYTHON_MAJOR=`echo ${PYTHON_VERSION} | cut -d. -f 1`
PYTHON_MINOR=`echo ${PYTHON_VERSION} | cut -d. -f 2`

if [ ! \( ${PYTHON_MAJOR} -eq 3 -o ${PYTHON_MINOR} -lt 6 \) ]; then
    echo "Python 3.x (at least 3.6) is required, you have ${PYTHON_VERSION}."
fi

# seed random source

RANDOM=`date +%s`$$

# set variables

OLD_CWD=`pwd`
TEMP_DIR=/tmp
UNIQUE_ID=${RANDOM}
PROJECT_PATH="$1"
TARGET_PATH="$2"
TARGET_BASEPATH="`echo ${TARGET_PATH} | sed -E 's/\.[^.]+$//g'`"
TARGET_BASENAME="`basename ${TARGET_BASEPATH}`"
TARGET_DIR="`dirname ${TARGET_BASEPATH}`"

trap 'cd ${OLD_CWD}' EXIT

# Build steps

cp "${TARGET_BASEPATH}.hex" "${TARGET_BASEPATH}_before_signing.hex_"

appuc-firmware-packer --appuc 1 --version 1 --input ${TARGET_BASEPATH}_before_signing.hex_ --sign "${SCRIPT_DIR}"/../../../Tools/SecureBoot/developer_key_private.der --cert "${SCRIPT_DIR}"/../../../Tools/SecureBoot/developer_certificate_signed.cert --output ${TARGET_BASEPATH}.hex

"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Ota/generate_ota_img.py --bl_image_file "${SCRIPT_DIR}"/../../../Work/Bootloader_qpg6200/Bootloader_qpg6200.hex --chip_config_header "${SCRIPT_DIR}"/../../../Applications/Matter/DoorWindow/include/CHIPProjectConfig.h --chip_root "${SCRIPT_DIR}"/../../../Components/ThirdParty/Matter/repo --compression lzma --developer-key "${SCRIPT_DIR}"/../../../Tools/SecureBoot/developer_key_private.der --factory_data_config "${SCRIPT_DIR}"/../../../Tools/QorvoPlatformTools/Matter/Credentials/test_doorWindow_qpg6200.factory_data_config --flash_app_start_offset 0xc400 --flash_start 0x10013000 --header_version 1 --in_file "${SCRIPT_DIR}"/../../../Work//doorWindow_qpg6200_development_thread_certified/doorWindow_qpg6200_development_thread_certified.hex --out_file "${SCRIPT_DIR}"/../../../Work//doorWindow_qpg6200_development_thread_certified/doorWindow_qpg6200_development_thread_certified.ota --se_image_file "${SCRIPT_DIR}"/../../..//Binaries/SEFW/se_firmware_release_xp4002_seuc_upgrade_FKS_0004_production_410.hex --xp400x

cp "${TARGET_BASEPATH}.hex" "${TARGET_BASEPATH}_before_hexmerge.hex_"

"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Hex/hexmerge.py ${TARGET_BASEPATH}.hex ${TARGET_BASEPATH}_before_hexmerge.hex_ "${SCRIPT_DIR}"/../../../Work/FactoryData_doorWindow/FactoryData_doorWindow.hex "${SCRIPT_DIR}"/../../../Work/Bootloader_qpg6200/Bootloader_qpg6200.hex --ignore_start_execution_addr --overlap keep_last

cp "${TARGET_BASEPATH}.hex" "${TARGET_BASEPATH}_before_hexfill.hex_"

"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/Hex/hexfill.py --endex 0x1017a800 --start 0x10172800 --value 0x0 ${TARGET_BASEPATH}.hex ${TARGET_BASEPATH}.hex --ignore_start_execution_addr

"$PYTHON" "${SCRIPT_DIR}"/../../../Tools/MemoryOverview/memoryoverview.py --logfile "${SCRIPT_DIR}"/../../../Work//doorWindow_qpg6200_development_thread_certified/doorWindow_qpg6200_development_thread_certified.memoryoverview --only-this "${SCRIPT_DIR}"/../../../Work//doorWindow_qpg6200_development_thread_certified/doorWindow_qpg6200_development_thread_certified.map
