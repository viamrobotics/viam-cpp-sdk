#!/usr/bin/env bash
#
# System -dev packages: boost, grpc, protobuf, ssl, c-ares, re2, zlib.
# On distros where the apt grpc or boost is missing or too old, it is built
# from source instead. See install-grpc.sh and install-boost.sh.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# Libraries needed regardless of how grpc arrives.
COMMON_DEV_PKGS=(
    libc-ares-dev
    libre2-dev
    libssl-dev
    zlib1g-dev
)

apt_install "${COMMON_DEV_PKGS[@]}"

# boost: apt when recent enough, else built from source.
"$(dirname "$0")/install-boost.sh"

# grpc + protobuf: apt when recent enough, else built from source.
"$(dirname "$0")/install-grpc.sh"
