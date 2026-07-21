#!/usr/bin/env bash
#
# Universal devkit: compilers, build tools, and the apt machinery the later
# scripts lean on (wget/gnupg for signed-by repo adds, software-properties-common
# for add-apt-repository on distros that still want it).
#
# The LLVM toolchain (clang, clang-tidy, clang-format) comes from the distro's own
# default version. Consumers needing a specific major install it themselves.

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
    clang \
    clang-format \
    clang-tidy \
    curl \
    g++ \
    gcc \
    git \
    gnupg \
    gpg \
    ninja-build \
    pkg-config \
    sudo \
    wget
