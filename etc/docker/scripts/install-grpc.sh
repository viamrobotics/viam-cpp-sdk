#!/usr/bin/env bash
#
# grpc (and its bundled protobuf) >= ${GRPC_MIN_VERSION}.
# Version-gated: take the distro grpc when it's recent enough, else build grpc
# (and its bundled protobuf) from source at ${GRPC_VERSION}, against the system
# zlib/c-ares/re2/openssl.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# Minimum apt grpc we accept before falling back to a source build.
GRPC_MIN_VERSION="${GRPC_MIN_VERSION:-1.51}"

# Candidate version apt would install, e.g. "1.51.1-3". Empty if no candidate.
apt-get update
candidate="$(apt-cache policy libgrpc++-dev 2>/dev/null | awk '/Candidate:/ {print $2}')"
rm -rf /var/lib/apt/lists/*

# Strip the Debian revision suffix for a clean dotted compare.
candidate_ver="${candidate%%-*}"

if [[ -n "${candidate_ver}" && "${candidate_ver}" != "(none)" ]] \
        && version_ge "${candidate_ver}" "${GRPC_MIN_VERSION}"; then
    # apt grpc is good enough: take the stack from packages.
    apt_install \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc
    exit 0
fi

# apt grpc missing/too old: build the grpc stack from source.
: "${GRPC_VERSION:?GRPC_VERSION must be set}"

SRC_DIR="${HOME}/opt/src"
mkdir -p "${SRC_DIR}"

cd "${SRC_DIR}"
git clone --recurse-submodules -b "${GRPC_VERSION}" \
    --depth 1 --shallow-submodules \
    https://github.com/grpc/grpc
cd grpc

cmake -S . -B build -G Ninja \
    -DgRPC_ZLIB_PROVIDER=package \
    -DgRPC_CARES_PROVIDER=package \
    -DgRPC_RE2_PROVIDER=package \
    -DgRPC_SSL_PROVIDER=package \
    -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DCMAKE_INSTALL_RPATH=/usr/local/lib
ninja -C build install

cd "${SRC_DIR}"
rm -rf grpc
ldconfig
