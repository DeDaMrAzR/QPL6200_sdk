#!/bin/bash

set -e
set -x

SCRIPT_PATH="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
QMATTER_ROOT_PATH=$(realpath "${SCRIPT_PATH}/..")
VENV_PATH=$(realpath "${SCRIPT_PATH}/../.python_venv")
SCRIPT_TMP_PATH=$(realpath /tmp)
DEVCONTAINER=false
# Project CHIP repository information for spake2p build in case of devcontainer setup
PROJECT_CHIP_REPO_URL="https://github.com/project-chip/connectedhomeip.git"
PROJECT_CHIP_BRANCH=v1.4-branch

trap 'on_error $? $LINENO' ERR

on_error() {
  echo "!!! $SCRIPT_PATH failed with error $1 on line $2"
  exit "$1"
}

DEFAULT_TOOLCHAIN_DIR=/opt/TOOL_ARMGCCEMB/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi

export PATH=$PATH:$DEFAULT_TOOLCHAIN_DIR/bin:${SCRIPT_PATH}/../Tools/QorvoPlatformTools/Matter
# TOOLCHAIN is used by Makefiles, so append it to PATH variable to find arm toolchain
if [[ -n ${TOOLCHAIN} ]]; then
    export PATH="${PATH}:${TOOLCHAIN}/bin"
fi
export MAKEFLAGS=-s

/proc/self/exe --version 2>/dev/null | grep -q 'GNU bash' ||  (\
    echo "!!!!! This is a BASH script !!!!!"; \
    echo "The shell you are running is $(/proc/self/exe --version)"; \
    echo "Please start bash first by running the 'bash' command"; \
    echo "Press Ctrl-C to abort"; \
    read -r \
)

if ! VALID_ARGS=$(getopt -o d --long devcontainer -- "$@"); then
    exit 1
fi

eval set -- "$VALID_ARGS"

while true; do
    case "$1" in
    -d | --devcontainer)
        DEVCONTAINER=true
        shift
        ;;
    --)
        shift
        break
        ;;
    *)
        echo "Unknown option $1"
        exit 1
        ;;
    esac
done

log() {
    echo "$(realpath "${BASH_SOURCE[0]}") ========= ${1} ============"
}
failure() {
    echo "========= ${1} ============"
}
check_installed_dependency ()
{
    $1 --version foo >/dev/null 2>&1 || {
        echo >&2 "$1 not installed. installing........"
        return 1
    }
    return 0
}

install_zap_dependencies ()
{
    sudo apt-get update
    sudo apt-get install -y clang-format

    sudo apt-get install -y --fix-missing libpixman-1-dev libcairo-dev libsdl-pango-dev libjpeg-dev libgif-dev
}

install_arm_gcc_emb ()
{
    sudo apt install -y bzip2 tar xz-utils
    wget -P /tmp --progress=dot:giga https://developer.arm.com/-/media/Files/downloads/gnu/12.2.mpacbti-rel1/binrel/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi.tar.xz
    sudo mkdir -p /opt/TOOL_ARMGCCEMB
    sudo tar -xf /tmp/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi.tar.xz -C /opt/TOOL_ARMGCCEMB
    rm /tmp/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi.tar.xz
}

install_gn ()
{
    ID="9dj0o2GBjGObfhKM_71Kxq0o1LsXEF6l0U9IWnOX1coC"
	sudo apt install unzip
	local curDir="${PWD}"
	mkdir -p "$SCRIPT_TMP_PATH/gn"
	cd "$SCRIPT_TMP_PATH/gn" || (failure "chdir to $SCRIPT_TMP_PATH/gn failed"; exit 1)
	wget --content-disposition "https://chrome-infra-packages.appspot.com/dl/gn/gn/linux-amd64/+/$ID"
	unzip gn-linux-amd64.zip
	sudo cp gn /usr/local/bin/gn
	sudo chmod +x /usr/local/bin/gn
	rm -rf "$SCRIPT_TMP_PATH/gn"
	cd "$curDir"
}

install_git ()
{
    sudo apt -y install git git-lfs
    git lfs install --skip-repo
}

install_rsync ()
{
    sudo apt-get install -y --no-install-recommends rsync
}

setup_venv ()
{
    Var=$(lsb_release -r)
    NumOnly=$(cut -f2 <<< "$Var")
    IntOnly=$(echo "$NumOnly" | cut -d '.' -f 1)
	local curDir="${PWD}"

    if [[ "$IntOnly" -lt 20 ]];
    then
        echo "Ubuntu version is less than 20, please update to 20 or later"
        exit 1
    fi

    sudo apt-get update
    sudo apt-get install -y zip build-essential libssl-dev zlib1g-dev libncurses5-dev \
        libncursesw5-dev libreadline-dev libsqlite3-dev libgdbm-dev libdb5.3-dev libffi-dev \
        libbz2-dev libexpat1-dev liblzma-dev tk-dev wget python3-dev

    if [[ "$IntOnly" -lt 24 ]]; then
        cd /usr/local/bin
        sudo wget https://www.python.org/ftp/python/3.11.0/Python-3.11.0.tgz
        sudo tar xzf Python-3.11.0.tgz
        cd Python-3.11.0
        ./configure --enable-optimizations
        sudo make -j"$(nproc)"
        sudo make altinstall
        sudo rm -rf /var/lib/apt/lists/* /usr/local/bin/Python-3.11.0 /usr/local/bin/Python-3.11.0.tgz
        cd "$curDir"
    fi

    # required for gn exec_script
    sudo apt update
    sudo apt install -y python-is-python3
    #check openssl minversion
    export openssl_minversion=1.1.1
    # if minversion is not the first in the result, install the deb file
    if ! echo -e "$(openssl version|awk '{print $2}')\n${openssl_minversion}" | sort -V | head -1 | grep -q ${openssl_minversion}
    then
        rm libssl1.1_1.1.1f-1ubuntu2.16_amd64.deb || true
        wget http://nz2.archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.1f-1ubuntu2.16_amd64.deb
        sudo dpkg -i libssl1.1_1.1.1f-1ubuntu2.16_amd64.deb
        rm libssl1.1_1.1.1f-1ubuntu2.16_amd64.deb
    fi

    if ! ${DEVCONTAINER}; then
        python3.11 -m venv --help >/dev/null 2>&1 || sudo apt-get install -y python3.11-venv
        # python3 -m ensurepip --help >/dev/null 2>&1 || sudo apt-get install -y python3.11-venv

        if [[ ! -d ${VENV_PATH} ]]; then
            mkdir -p "${VENV_PATH}"
        fi
        python3.11 -m venv "${VENV_PATH}"
        export VENV_PATH
        # shellcheck source=/dev/null
        source "${VENV_PATH}"/bin/activate
    fi
    log "$(python -V)"
    # Install additional modules
    pip3 install wheel dataclasses intelhex click ecdsa cryptography lark jinja2 stringcase pigweed PrettyTable Cheetah3 pylzma packaging
    # clang-format version needs pinning due to matter requirements
    pip3 install clang-format==16.0.6
    # Install qorvo-platform-tools.
    pip3 install qorvo-platform-tools --index-url https://gitlab.com/api/v4/projects/66835490/packages/pypi/simple

    # update oscrypto
    # There is a problem with detecting the version of libcrypto in the current oscrypto version.
    # This is a workaround to install the below patched version of oscrypto
    python3 -m pip install --force-reinstall git+https://github.com/wbond/oscrypto.git@1547f535001ba568b239b8797465536759c742a3
}


install_spake2p ()
{
    BACKUP_DIR=$(pwd)
    sudo apt-get update
    log "Installing spake2p build requirements"

    if ${DEVCONTAINER}; then
        # For devcontainer, spake2p is built from the upstream matter repository
        git clone --depth 1 --branch $PROJECT_CHIP_BRANCH $PROJECT_CHIP_REPO_URL /tmp/matter
        cd /tmp/matter
        # Initialize only required submodules for spake2p build to save time and space
        for module_path in  \
            third_party/nlfaultinjection \
            third_party/jsoncpp \
            third_party/libwebsockets \
            third_party/editline \
            third_party/nlassert \
            third_party/nlio \
            third_party/perfetto
        do
            git submodule update --init --depth=1 -- "${module_path}"
        done
    else
        cd "${QMATTER_ROOT_PATH}/Components/ThirdParty/Matter/repo" || (echo chdir to matter repo failed; exit 1)
    fi

    # shellcheck source=/dev/null
    source "${SCRIPT_PATH}/build_install_spake2p.sh"

    cd "$BACKUP_DIR"

    if ${DEVCONTAINER}; then
        # cleanup /tmp to save space
        rm -rf /tmp/matter
    fi
}
install_zap()
{
    if ${DEVCONTAINER}; then
        # For devcontainer, read the zap version from zap-version.txt file
        # WARNING: This file must be updated manually when ZAP version changes
        ZAP_VERSION_FILE="/${SCRIPT_PATH}/zap-version.txt"
    else
        ZAP_VERSION_FILE="${QMATTER_ROOT_PATH}/Components/ThirdParty/Matter/repo/scripts/setup/zap.json"
    fi
    ZAP_VERSION=$(grep -E "v[0-9]+\.[0-9]+\.[0-9]+-nightly" -o "$ZAP_VERSION_FILE" |head -n 1)
    echo "found version: ${ZAP_VERSION}"
    ZAP_INSTALL_PATH="/opt/zap-${ZAP_VERSION}"

    if test -e "${ZAP_INSTALL_PATH}"
    then
        return
    fi

    sudo mkdir -p "${ZAP_INSTALL_PATH}"
    cd "${ZAP_INSTALL_PATH}"
    sudo wget --progress=dot:giga "https://github.com/project-chip/zap/releases/download/${ZAP_VERSION}/zap-linux-x64.zip"
    sudo apt-get update
    sudo apt install unzip
    sudo unzip -o zap-linux-x64.zip
    sudo rm zap-linux-x64.zip
    # keep zap UI (don't delete it)
    sudo rm /usr/bin/zap-cli || true
    sudo ln -s "${ZAP_INSTALL_PATH}/zap-cli" /usr/bin/
    # additional symlink to do the version check
    sudo rm "/usr/bin/zap-cli-${ZAP_VERSION}" || true
    sudo ln -s "${ZAP_INSTALL_PATH}/zap-cli" "/usr/bin/zap-cli-${ZAP_VERSION}"
}
command -v sudo || (
    echo "Please enter your root password to install sudo."
    su -c 'apt-get update; apt-get install -y sudo'
)
command sudo apt-get update

for tool_name in  \
    lsb-release \
    git \
    clang \
    make \
    ninja \
    curl \
    wget
do
    command -v "$tool_name" || sudo apt-get install -y "${tool_name}" || sudo apt-get install -y "${tool_name}-build"
done

if check_installed_dependency arm-none-eabi-gcc
then
    if ! arm-none-eabi-gcc --version | grep -F "12.2.1 20230214" >/dev/null
    then
        echo "Invalid armgcc version detected"
        exit 1
    fi
else
    install_arm_gcc_emb
fi

install_gn

install_git

install_zap_dependencies

if ! check_installed_dependency rsync; then
    install_rsync
fi

setup_venv


# Installation of Zap and Spake2P requires submodules in case of non-devcontainer setup
if ! ${DEVCONTAINER}; then
    bash "${SCRIPT_PATH}/setup_submodules.sh"
fi

install_zap

if test ! -e /usr/bin/spake2p
then
    install_spake2p
fi
