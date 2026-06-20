#!/bin/bash

set -e
set -x

SCRIPT_PATH="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
QMATTER_ROOT_PATH=$(realpath "${SCRIPT_PATH}/..")

trap 'on_error $? $LINENO' ERR

on_error() {
  echo "!!! $SCRIPT_PATH failed with error $1 on line $2"
  exit "$1"
}

log() {
    echo "$(realpath "${BASH_SOURCE[0]}") ========= ${1} ============"
}
failure() {
    echo "========= ${1} ============"
}

setup_submodules ()
{
    # QMatter '-libs' variant lacks the matter submodule to avoid
    # a recursive dependency in project-chip/connectedhomeip
    # For Qorvo-internal CI testing, we add this at validation time
    if test -e "${SCRIPT_PATH}/git_add_submodules.sh"  && test ! -e "${QMATTER_ROOT_PATH}/Components/ThirdParty/Matter/repo/.gitmodules"
    then
        log "Adding submodules to allow package validation"
        # shellcheck source=/dev/null
        source "${SCRIPT_PATH}/git_add_submodules.sh"
    fi

    log "Updating submodules"
    git submodule update --init --depth=1 --progress
    cd Components/ThirdParty/OpenThread/ot-qorvo || (failure "chdir to ot-qorvo repo failed"; exit 1)
    git submodule update --init --depth=1 -- openthread
    cd -
    cd Components/ThirdParty/Matter/repo || (failure "chdir to matter repo failed"; exit 1)
    # TODO: use Components/ThirdParty/Matter/repo/scripts/checkout_submodules.py --platform qpg
    for module_path in  \
        third_party/nlfaultinjection \
        third_party/jsoncpp \
        third_party/libwebsockets \
        third_party/editline \
        third_party/mbedtls \
        third_party/nlassert \
        third_party/nlio \
        third_party/freertos \
        third_party/openthread \
        third_party/pigweed \
        third_party/perfetto \
        third_party/qpg_sdk
    do
        git submodule update --init --depth=1 -- "${module_path}"
    done
    log "submodules successfully initialized"
}

setup_submodules
