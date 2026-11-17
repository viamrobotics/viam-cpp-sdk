#!/usr/bin/env bash
#
# Boost >= ${BOOST_APT_VERSION_MIN}.
# Version-gated: take the distro boost when it's recent enough, else build the
# libraries the SDK needs from source into /usr/local.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

apt-get update
candidate_ver="$(apt_candidate libboost-dev)"
rm -rf /var/lib/apt/lists/*

if [[ -n "${candidate_ver}" ]] && version_ge "${candidate_ver}" "${BOOST_APT_VERSION_MIN}"; then
    apt_install libboost-all-dev
    exit 0
fi

# apt boost missing/too old: build from source.
SRC_DIR="${HOME}/opt/src"
mkdir -p "${SRC_DIR}"

cd "${SRC_DIR}"
tarball="boost_${BOOST_APT_VERSION_MIN//./_}"
wget -q "https://archives.boost.io/release/${BOOST_APT_VERSION_MIN}/source/${tarball}.tar.gz"
tar xzf "${tarball}.tar.gz"
rm "${tarball}.tar.gz"
cd "${tarball}"

./bootstrap.sh --prefix=/usr/local --with-libraries=log,program_options,test
./b2 -j"$(nproc)" variant=release link=shared threading=multi \
    linkflags=-Wl,-rpath,/usr/local/lib install

cd "${SRC_DIR}"
rm -rf "${tarball}"
