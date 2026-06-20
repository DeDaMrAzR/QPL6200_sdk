#!/bin/bash

set -e

OS_ID=$(awk -F "=" '/^ID=/ {print $2}' /etc/os-release)
OS_CODENAME=$(awk -F "=" '/^VERSION_CODENAME=/ {print $2}' /etc/os-release)
HOSTTYPE="$(uname -m)"

if [[ ${OS_ID} != "ubuntu" || ${HOSTTYPE} != "x86_64" ]]; then
    printf 'Your distribution %s %s (%s) is not supported\
' "$OS_ID" "$OS_CODENAME" "${HOSTTYPE}" >&2
    echo "Supported distributions: ${SUPPORTED_OS}"
    exit 1
fi

#=================================#
#          Installation           #
#=================================#

apt update

ARCHIVE="JLink_Linux_V${VERSION/\./}_${HOSTTYPE}.deb"

curl --location "https://www.segger.com/downloads/jlink/${ARCHIVE}" \
    -X POST \
    --data-raw 'accept_license_agreement=accepted&submit=Download+software' \
    --output "${ARCHIVE}"

# Installing J-Link requires udevadm to run but available in container
# So steps are:
# 1. unpack jlink (installation prepared but not yet done)
dpkg --unpack "${ARCHIVE}"
# 2. remove post installation script
rm /var/lib/dpkg/info/jlink.postinst -f
# 3. finish installation including J-Link dependencies
apt install -y --no-install-recommends --fix-broken "./${ARCHIVE}"

rm -f "${ARCHIVE}"

#=================================#
#            Cleaning             #
#=================================#

# Clean up APT when done.
apt clean && rm -rf /var/lib/apt/lists/* /var/tmp/*
