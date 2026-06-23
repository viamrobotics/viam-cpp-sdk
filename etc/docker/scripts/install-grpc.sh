#!/usr/bin/env bash
#
# grpc (and its bundled protobuf) >= ${GRPC_APT_VERSION_MIN}.
# Version-gated: take the distro grpc when it's recent enough, else build grpc
# (and its bundled protobuf) from source against the system zlib/c-ares/re2/openssl.
# A GRPC_SOURCE_VERSION_OVERRIDE greater than the apt floor forces a source build
# at that version even where the distro grpc would otherwise be accepted.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# Resolve the source-build version. A valid override forces a source build at that
# version; otherwise we may take apt, falling back to the pinned default. An
# override at or below the apt floor is a usage error -- fail loudly.
override="${GRPC_SOURCE_VERSION_OVERRIDE:-}"
if [[ -n "${override}" ]]; then
    if [[ "${override}" == "${GRPC_APT_VERSION_MIN}" ]] \
            || ! version_ge "${override}" "${GRPC_APT_VERSION_MIN}"; then
        echo "GRPC_SOURCE_VERSION_OVERRIDE=${override} must be greater than the" \
             "minimum grpc version ${GRPC_APT_VERSION_MIN}" >&2
        exit 1
    fi
    grpc_source_version="${override}"
    force_source=1
else
    grpc_source_version="${GRPC_SOURCE_VERSION_DEFAULT}"
    force_source=
fi

# Version apt would install, e.g. "1.51.1". Empty if no installable candidate.
apt-get update
candidate_ver="$(apt_candidate libgrpc++-dev)"
rm -rf /var/lib/apt/lists/*

if [[ -z "${force_source}" && -n "${candidate_ver}" ]] \
        && version_ge "${candidate_ver}" "${GRPC_APT_VERSION_MIN}"; then
    # apt grpc is good enough: take the stack from packages.
    apt_install \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler-grpc
    exit 0
fi

# apt grpc missing/too old, or override forces it: build the grpc stack from source.
SRC_DIR="${HOME}/opt/src"
mkdir -p "${SRC_DIR}"

cd "${SRC_DIR}"
git clone --recurse-submodules -b "v${grpc_source_version}" \
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
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DCMAKE_INSTALL_RPATH=/usr/local/lib
cmake --build build --target install

cd "${SRC_DIR}"
rm -rf grpc
