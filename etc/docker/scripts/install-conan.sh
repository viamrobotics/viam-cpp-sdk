#!/usr/bin/env bash
#
# conan strategy: conan in the system python, plus the jinja2 profile
# `viam-system-gcc-release` and a `default` that include()s it.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

apt_install python3 python3-pip

# PEP-668 blocks pip on bookworm+/noble+. Grant break-system-packages for the
# install only; older distros carry no marker and ignore the unknown key.
pip_conf=/etc/pip.conf
trap 'rm -f "${pip_conf}"' EXIT
printf '[global]\nbreak-system-packages = true\n' > "${pip_conf}"

# bullseye's pip 20.3.4 dies building conan's deps (toml decoder IndexError);
# fails identically without the override.
pip3 install --no-cache-dir --upgrade pip
pip3 install --no-cache-dir "conan~=2.0"

rm -f "${pip_conf}"
trap - EXIT

# Install the jinja2 toolchain profile plus the `default` that include()s it.
profile=viam-system-gcc-release
conan_home="$(conan config home)"
mkdir -p "${conan_home}/profiles"
install -m 0644 "$(dirname "$0")/conan/${profile}.jinja" "${conan_home}/profiles/${profile}"
install -m 0644 "$(dirname "$0")/conan/default" "${conan_home}/profiles/default"

CONAN_REMOTE_URL="https://viam.jfrog.io/artifactory/api/conan/viamconan"
conan remote add viamconan "${CONAN_REMOTE_URL}"

conan --version
