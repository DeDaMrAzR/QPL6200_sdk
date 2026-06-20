#!/bin/bash
set -e
set -x

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

# SDK use TOOLCHAIN variable while cpp feature install toolchain in ARM_COMPILER_PATH
echo 'export TOOLCHAIN=${ARM_COMPILER_PATH%/bin}' >>/etc/profile
echo 'export PATH=${PATH}:${ARM_COMPILER_PATH}:~/.local/bin' >>/etc/profile
echo 'export MAKEFLAGS=-s' >>/etc/profile

./bootstrap.sh --devcontainer

#=================================#
#            Cleaning             #
#=================================#

# Clean up APT when done.
apt clean && rm -rf /var/lib/apt/lists/* /var/tmp/*
