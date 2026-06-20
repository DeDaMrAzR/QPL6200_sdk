#!/bin/bash
set -e

INSTALL_PATH_ARM=/opt/gcc
INSTALL_PATH_ZEPHYR_SDK=/opt/zephyr
INSTALL_PATH_RISC=/opt/gcc

export DEBIAN_FRONTEND=noninteractive
export CI_API_TOKEN=glpat-G6oE9YDSXu17auSgh-R6

OS_ID=$(awk -F "=" '/^ID=/ {print $2}' /etc/os-release)
OS_CODENAME=$(awk -F "=" '/^VERSION_CODENAME=/ {print $2}' /etc/os-release)
HOSTTYPE="$(uname -m)"

SUPPORTED_OS="ubuntu-jammy|ubuntu-noble"

if [[ ! "${OS_ID}-${OS_CODENAME}" =~ ^(${SUPPORTED_OS})$ || ${HOSTTYPE} != "x86_64" ]]; then
    printf 'Your distribution %s %s (%s) is not supported\
' "$OS_ID" "$OS_CODENAME" "${HOSTTYPE}" >&2
    echo "Supported distributions: ${SUPPORTED_OS}"
    exit 1
fi

#=================================#
#          Installation           #
#=================================#

echo -e "
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports ${OS_CODENAME} main restricted universe multiverse\n
deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports ${OS_CODENAME}-updates main restricted universe multiverse\n" >/etc/apt/sources.list.d/arm64.list

dpkg --add-architecture arm64
apt update

apt install -y --no-install-recommends \
    ccache \
    curl \
    git \
    libelf-dev \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    ninja-build \
    clang-format-14 \
    lcov \
    gcovr \
    build-essential \
    wget \
    gdb-multiarch \
    crossbuild-essential-arm64 \
    automake \
    pkg-config \
    pipx

packages=("pre-commit==2.18.1"
    "cmakelang==0.6.13"
    "kconfiglib==14.1.0"
)

for package in "${packages[@]}"; do
    pipx install "$package"
done

local_wget() {
    wget --show-progress --progress=bar:force:noscroll "$@"
}

#
# Install CMake
#
echo "Installing CMake"
CMAKE_VERSION=3.31.6
WGET_TMP_FILE="/tmp/cmake-${CMAKE_VERSION}-Linux-${HOSTTYPE}.sh"
local_wget -O "${WGET_TMP_FILE}" \
    "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-Linux-${HOSTTYPE}.sh"
chmod +x "${WGET_TMP_FILE}"
"${WGET_TMP_FILE}" --skip-license --prefix=/usr/local
rm -f "${WGET_TMP_FILE}"

#
# Install Include What You Use
#
echo "Installing IWYU"
echo "
    deb http://apt.llvm.org/${OS_CODENAME}/ llvm-toolchain-${OS_CODENAME}-20 main
    deb-src http://apt.llvm.org/${OS_CODENAME}/ llvm-toolchain-${OS_CODENAME}-20 main
    " >/etc/apt/sources.list.d/llvm-20.list
wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc

apt update
apt install -y --no-install-recommends \
    llvm-20-dev \
    libclang-20-dev \
    clang-20 \
    clang-format-20 \
    clang-tidy-20

git clone https://github.com/include-what-you-use/include-what-you-use.git --branch clang_20 /tmp/iwyu
mkdir /tmp/iwyu/build
cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-20 -B /tmp/iwyu/build -S /tmp/iwyu
make -C /tmp/iwyu/build -j "$(nproc)" install
rm -rf /tmp/iwyu

#
# Install toolchains
#

# Courtesy of https://stackoverflow.com/questions/4023830/how-to-compare-two-strings-in-dot-separated-version-format-in-bash
vercomp() {
    if [[ $1 == "$2" ]]; then
        echo "=="
    fi
    local IFS=.
    # shellcheck disable=SC2206
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i = ${#ver1[@]}; i < ${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i = 0; i < ${#ver1[@]}; i++)); do
        if ((10#${ver1[i]:=0} > 10#${ver2[i]:=0})); then
            echo ">"
            return
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]})); then
            echo "<"
            return
        fi
    done
    echo "=="
}

parse_toolchains() {
    local -n output_pairs="$1"
    local input_string="$2"
    IFS=',' read -ra elements <<<"${input_string}"
    for element in "${elements[@]}"; do
        IFS=':' read -ra pair <<<"${element}"
        # shellcheck disable=SC2034
        output_pairs["${pair[0]}"]="${pair[1]} ${output_pairs[${pair[0]}]}"
    done
}

install_toolchain() {
    local INSTALL_PATH="$1"
    local URL="$2"
    local ARCHIVE="${URL##*/}"

    mkdir -p "${INSTALL_PATH}"
    cd "${INSTALL_PATH}"
    local_wget "${URL}"
    tar xf "${ARCHIVE}"
    rm "${ARCHIVE}"
}

install_arm_toolchain() {
    local URL="$1"
    local IS_DEFAULT="$2"
    local ARCHIVE="${URL##*/}"

    install_toolchain "${INSTALL_PATH_ARM}" "${URL}"

    if [[ ${IS_DEFAULT} == true ]]; then
        echo "export ARM_COMPILER_PATH=${INSTALL_PATH_ARM}/${ARCHIVE%.tar.*}/bin" >>/etc/profile
    fi
}

install_arm-gnu() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local URL="https://developer.arm.com/-/media/Files/downloads/gnu/${VERSION}/binrel/arm-gnu-toolchain-${VERSION}-${HOSTTYPE}-arm-none-eabi.tar.xz"

    echo "Installing official ARM GNU toolchain ${VERSION}"
    install_arm_toolchain "${URL}" "${IS_DEFAULT}"
}

install_arm-gnu-a() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local URL="https://developer.arm.com/-/media/Files/downloads/gnu-a/${VERSION}/binrel/gcc-arm-${VERSION}-${HOSTTYPE}-arm-linux-gnueabihf.tar.xz"

    echo "Installing official ARM GNU-A toolchain ${VERSION}"
    install_arm_toolchain "${URL}" "${IS_DEFAULT}"
}

install_zephyr-sdk() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local ZEXT="xz"
    if [[ $(vercomp "${VERSION}" "0.16.0") == "<" ]]; then
        local ZEXT="gz"
    fi
    local URL="https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${VERSION}/zephyr-sdk-${VERSION}_linux-${HOSTTYPE}_minimal.tar.${ZEXT}"

    echo "Installing Zephyr sdk ${VERSION}"
    install_toolchain "${INSTALL_PATH_ZEPHYR_SDK}" "${URL}"

    if [[ ${IS_DEFAULT} == true ]]; then
        echo "ZEPHYR_SDK_INSTALL_DIR=${INSTALL_PATH_ZEPHYR_SDK}/zephyr-sdk-${VERSION}" >>/etc/profile
        echo "ZEPHYR_TOOLCHAIN_VARIANT=zephyr" >>/etc/profile
    fi
}

install_arm-xpack() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local HOSTTYPE=x64
    local URL="https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v${VERSION}/xpack-arm-none-eabi-gcc-${VERSION}-linux-${HOSTTYPE}.tar.gz"

    echo "Installing xPack ARM toolchain ${VERSION}"
    install_arm_toolchain "${URL}" "${IS_DEFAULT}"
}

install_risc-qorvo() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local RISC_PROJECT=67532148
    local RISC_NAME=riscv-gnu-multilib
    local RISC_PACKAGE="${RISC_NAME}-release"
    local ARCHIVE="${RISC_PACKAGE}.tar.gz"
    local URL="https://gitlab.com/api/v4/projects/${RISC_PROJECT}/packages/generic/${RISC_PACKAGE}/${VERSION}/${ARCHIVE}"

    echo "Installing Qorvo RISCV toolchain ${VERSION}"
    mkdir -p "${INSTALL_PATH_RISC}"
    cd ${INSTALL_PATH_RISC}

    if [[ -n $CI_API_TOKEN ]]; then
        curl --header "PRIVATE-TOKEN: $CI_API_TOKEN" --location "$URL" --output "$ARCHIVE"
    elif [[ -n $USER_API_TOKEN ]]; then
        curl --header "PRIVATE-TOKEN: $USER_API_TOKEN" --location "$URL" --output "$ARCHIVE"
    else
        echo "Installation of Qorvo multilib RISC-V toochain requires CI_JOB_TOKEN or USER_API_TOKEN set in the env"
        exit 1
    fi
    tar xf "${ARCHIVE}"
    rm "${ARCHIVE}"
    mv "${RISC_NAME}" "${RISC_NAME}-${VERSION}"

    if [[ ${IS_DEFAULT} == true ]]; then
        echo "export RISC_COMPILER_PATH=${INSTALL_PATH_RISC}/${RISC_NAME}-${VERSION}/bin" >>/etc/profile
    fi
}

install_risc-xpack() {
    local VERSION="$1"
    local IS_DEFAULT="$2"
    local HOSTTYPE=x64
    local URL="https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v${VERSION}/xpack-riscv-none-elf-gcc-${VERSION}-linux-${HOSTTYPE}.tar.gz"

    echo "Installing xPack RISC-V toolchain ${VERSION}"

    install_toolchain "${INSTALL_PATH_RISC}" "${URL}"
    if [[ ${IS_DEFAULT} == true ]]; then
        echo "export RISC_COMPILER_PATH=${INSTALL_PATH_RISC}/xpack-riscv-none-elf-gcc-${VERSION}/bin" >>/etc/profile
    fi
}

declare -A parsed_arm_pairs
parse_toolchains parsed_arm_pairs "$ARM_TOOLCHAINS"
default_arm_toolchain_set=false

for key in "${!parsed_arm_pairs[@]}"; do
    for version in ${parsed_arm_pairs[${key}]}; do
        is_default=$([[ "${key}:${version}" == "${DEFAULT_ARM_TOOLCHAIN}" ]] && echo true || echo false)
        default_arm_toolchain_set=$([[ ${default_arm_toolchain_set} == true || ${is_default} == true ]] && echo true || echo false)

        case ${key} in
        arm-gnu)
            install_arm-gnu "${version}" "${is_default}"
            ;;
        arm-gnu-a)
            install_arm-gnu-a "${version}" "${is_default}"
            ;;
        xpack)
            install_arm-xpack "${version}" "${is_default}"
            ;;
        zephyr-sdk)
            install_zephyr-sdk "${version}" "${is_default}"
            ;;
        *)
            echo "Unknown toolchain ${key}"
            exit 1
            ;;
        esac
    done
done

if ! ${default_arm_toolchain_set}; then
    echo "Invalid default toolchain ${DEFAULT_ARM_TOOLCHAIN}"
    exit 1
fi

if [[ ${DEFAULT_RISC_TOOLCHAIN} != "no-install" ]]; then
    declare -A parsed_risc_pairs
    parse_toolchains parsed_risc_pairs "$RISC_TOOLCHAINS"
    default_risc_toolchain_set=false

    for key in "${!parsed_risc_pairs[@]}"; do
        for version in ${parsed_risc_pairs[${key}]}; do
            is_default=$([[ "${key}:${version}" == "${DEFAULT_RISC_TOOLCHAIN}" ]] && echo true || echo false)
            default_risc_toolchain_set=$([[ ${default_risc_toolchain_set} == true || ${is_default} == true ]] && echo true || echo false)

            case ${key} in
            risc-qorvo)
                install_risc-qorvo "${version}" "${is_default}"
                ;;
            xpack)
                install_risc-xpack "${version}" "${is_default}"
                ;;
            *)
                echo "Unknown toolchain ${key}"
                exit 1
                ;;
            esac
        done
    done

    if ! ${default_risc_toolchain_set}; then
        echo "Invalid default toolchain ${DEFAULT_RISC_TOOLCHAIN}"
        exit 1
    fi
fi

update-ccache-symlinks
# shellcheck disable=SC2016
echo 'export PATH="/usr/lib/ccache/:$PATH"' >>/etc/profile

#=================================#
#            Cleaning             #
#=================================#

# Clean up APT when done.
apt clean && rm -rf /var/lib/apt/lists/* /var/tmp/*
