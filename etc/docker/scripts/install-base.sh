#!/usr/bin/env bash
#
# Universal devkit: compilers, build tools, and the apt machinery the later
# scripts lean on (wget/gnupg for signed-by repo adds, software-properties-common
# for add-apt-repository on distros that still want it).

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

apt-get update
apt-get -y dist-upgrade
rm -rf /var/lib/apt/lists/*

apt_install \
    build-essential \
    ca-certificates \
    curl \
    doxygen \
    g++ \
    gcc \
    gdb \
    git \
    gnupg \
    gpg \
    less \
    ninja-build \
    pkg-config \
    software-properties-common \
    sudo \
    wget
