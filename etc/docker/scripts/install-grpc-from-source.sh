#!/usr/bin/env bash
#
# Build and install grpc (and its bundled protobuf) at ${GRPC_VERSION} from
# source, against the system zlib/c-ares/re2/openssl. Used on distros whose apt
# grpc is missing or too old.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

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
