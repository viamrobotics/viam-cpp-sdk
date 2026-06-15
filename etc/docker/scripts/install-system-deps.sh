#!/usr/bin/env bash
#
# System -dev packages: boost, grpc, protobuf, ssl, c-ares, re2, zlib.
# On distros where the apt grpc is missing or too old, the grpc stack is built
# from source at ${GRPC_VERSION} instead (against the system zlib/c-ares/re2/ssl).

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# Minimum apt grpc we accept before falling back to a source build.
GRPC_MIN_VERSION="${GRPC_MIN_VERSION:-1.51}"

# Libraries needed regardless of how grpc arrives.
COMMON_DEV_PKGS=(
    libboost-all-dev
    libc-ares-dev
    libre2-dev
    libssl-dev
    zlib1g-dev
)

apt-get update
grpc_candidate="$(apt-cache policy libgrpc++-dev 2>/dev/null | awk '/Candidate:/ {print $2}')"
rm -rf /var/lib/apt/lists/*
grpc_candidate_ver="${grpc_candidate%%-*}"

if [[ -n "${grpc_candidate_ver}" && "${grpc_candidate_ver}" != "(none)" ]] \
        && version_ge "${grpc_candidate_ver}" "${GRPC_MIN_VERSION}"; then
    # apt grpc is good enough: take the whole stack from packages.
    apt_install \
        "${COMMON_DEV_PKGS[@]}" \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc
else
    # apt grpc missing/too old: system deps from apt, grpc from source.
    apt_install "${COMMON_DEV_PKGS[@]}"
    "$(dirname "$0")/install-grpc-from-source.sh"
fi
