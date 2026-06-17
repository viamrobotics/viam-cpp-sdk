#!/usr/bin/env bash
#
# System -dev packages: boost, grpc, protobuf, ssl, c-ares, re2, zlib.
# On distros where the apt grpc is missing or too old, the grpc stack is built
# from source at ${GRPC_VERSION} instead (against the system zlib/c-ares/re2/ssl).

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# Libraries needed regardless of how grpc arrives.
COMMON_DEV_PKGS=(
    libboost-all-dev
    libc-ares-dev
    libre2-dev
    libssl-dev
    zlib1g-dev
)

apt_install "${COMMON_DEV_PKGS[@]}"

# grpc + protobuf: apt when recent enough, else built from source.
"$(dirname "$0")/install-grpc.sh"
