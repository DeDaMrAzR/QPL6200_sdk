#!/bin/bash
set -e

export DEBIAN_FRONTEND=noninteractive

OS_ID=$(awk -F "=" '/^ID=/ {print $2}' /etc/os-release)
OS_CODENAME=$(awk -F "=" '/^VERSION_CODENAME=/ {print $2}' /etc/os-release)
OS="${OS_ID}-${OS_CODENAME}"

SUPPORTED_OS="ubuntu-jammy|ubuntu-noble"

if [[ ! ${OS} =~ ^(${SUPPORTED_OS})$ || ${HOSTTYPE} != "x86_64" ]]; then
    echo "Your distribution ${OS} ${HOSTTYPE} is not supported" >&2
    echo "Supported distributions: ${SUPPORTED_OS}"
    exit 1
fi

#=================================#
#          Installation           #
#=================================#

{
    echo "export TZ=Europe/Paris"
    echo "export LANG=en_US.UTF-8"
    echo "export LANGUAGE=en_US:en"
    echo "export LC_ALL=en_US.UTF-8"
    echo "export LC_CTYPE=en_US.UTF-8"
} >>/etc/profile

# shellcheck source=/dev/null
source /etc/profile

# Can use snapshots without installing first ca-certificates
apt update
apt install -y --no-install-recommends \
    ca-certificates

# Force snapshot for all repositories that support snapshots
if [[ -n ${UBUNTU_SNAPSHOT} ]]; then
    echo "APT::Snapshot \"${UBUNTU_SNAPSHOT}\";" >/etc/apt/apt.conf.d/50snapshot
fi

# Force archive.ubuntu.com to support only amd64 architecture. Any other architecture would need to
# add the right repositories with associated architecture.
if [[ ${OS} == "ubuntu-jammy" ]]; then
    echo -e "
deb [arch=amd64] http://archive.ubuntu.com/ubuntu ${OS_CODENAME} main restricted universe multiverse\n
deb [arch=amd64] http://archive.ubuntu.com/ubuntu ${OS_CODENAME}-updates main restricted universe multiverse\n
deb-src http://archive.ubuntu.com/ubuntu/ ${OS_CODENAME} main restricted universe multiverse\n
deb-src http://archive.ubuntu.com/ubuntu/ ${OS_CODENAME}-updates main restricted universe multiverse\n" >/etc/apt/sources.list
else
    sed -i /etc/apt/sources.list.d/ubuntu.sources -e '/Components/a Architectures: amd64'
fi

apt update

apt install -y --no-install-recommends \
    git \
    sudo \
    locales \
    ssh \
    pipx \
    python3-pip \
    curl \
    zsh \
    less

apt upgrade -y

sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen
locale-gen

# Ubuntu noble images are adding the ubuntu user, remove it if existsing to avoid any issue with
# devcontaineruser.
if id ubuntu >/dev/null 2>&1; then
    deluser --remove-home ubuntu
fi

useradd -m -N -g users devcontaineruser
passwd --delete devcontaineruser
usermod -aG sudo devcontaineruser

mkdir -p /etc/zsh
cp "$(dirname "$0")"/newuser.zshrc.recommended /etc/zsh/newuser.zshrc.recommended

sed -i 's|/bin/sh|/bin/zsh|g' /etc/passwd

echo ". /etc/profile" >>/etc/zsh/zprofile

getent passwd | while IFS=: read -r USERNAME _ _ _ _ USER_HOME USER_SHELL; do
    if [[ -d $USER_HOME && $USER_SHELL == *"zsh" ]]; then
        cp /etc/zsh/newuser.zshrc.recommended "$USER_HOME/.zshrc"
        chown "$USERNAME:" "$USER_HOME/.zshrc"
    fi
done

# PIPX installation

export PIPX_BIN_DIR=/usr/local/bin
export PIPX_HOME=/opt/pipx
echo "export PIPX_BIN_DIR=/usr/local/bin" >>/etc/profile
echo "export PIPX_HOME=/opt/pipx" >>/etc/profile

# Avoid pip --break-system-packages
echo -e "
[global]\n
break-system-packages = true" >/etc/pip.conf

packages=("cruft==2.15.0")

for package in "${packages[@]}"; do
    pipx install "$package"
done

pip install west==1.2.0
cp "$(dirname "$0")"/update-python-packages.py /usr/local/bin/update-python-packages
chmod +x /usr/local/bin/update-python-packages

#=================================#
#            Cleaning             #
#=================================#

# Clean up APT when done.
apt clean && rm -rf /var/lib/apt/lists/* /var/tmp/*
