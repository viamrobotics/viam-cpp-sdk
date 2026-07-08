#!/usr/bin/env bash
#
# cmake >= ${CMAKE_MIN_VERSION}.
# Version-gated: take the distro cmake when it's recent enough, else pull a newer
# one from Kitware (Ubuntu) or the Debian backports archive (Debian).

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# CMAKE_MIN_VERSION is pinned in lib/common.sh.
# Version apt would install, e.g. "3.25.1". Empty if no installable candidate.
apt-get update
candidate_ver="$(apt_candidate cmake)"
rm -rf /var/lib/apt/lists/*

if [[ -n "${candidate_ver}" ]] \
        && version_ge "${candidate_ver}" "${CMAKE_MIN_VERSION}"; then
    apt_install cmake
    exit 0
fi

if is_ubuntu; then
    # Kitware apt repo carries current cmake for Ubuntu.
    add_apt_repo "kitware-archive" \
        "https://apt.kitware.com/keys/kitware-archive-latest.asc" \
        "https://apt.kitware.com/ubuntu/ ${DISTRO_CODENAME} main"
    apt_install cmake
elif is_debian; then
    # Backports carries a newer cmake than the base suite.
    # Older releases live on archive.debian.org.
    add_apt_repo "debian-backports" \
        "https://ftp-master.debian.org/keys/archive-key-11.asc" \
        "https://archive.debian.org/debian ${DISTRO_CODENAME}-backports main"
    # Backports cmake hard-depends on a newer libcurl4 than the base suite, so
    # installing it upgrades libcurl4 to the backports version. That splits the
    # curl source packages across suites: a later `apt install curl` (or a
    # libcurl4-*-dev) picks the base-suite build, which pins the old libcurl4 and
    # fails with "held broken packages". Pin the whole curl family to backports
    # so those installs resolve against the libcurl4 that's actually installed,
    # and preinstall curl so the common case needs no apt work at all.
    cat > /etc/apt/preferences.d/99-curl-backports <<EOF
Package: curl libcurl3-gnutls libcurl3-nss libcurl4 libcurl4-doc libcurl4-gnutls-dev libcurl4-nss-dev libcurl4-openssl-dev
Pin: release n=${DISTRO_CODENAME}-backports
Pin-Priority: 500
EOF
    apt_install_from "${DISTRO_CODENAME}-backports" cmake curl
else
    echo "install-cmake.sh: unsupported distro '${DISTRO_ID}'" >&2
    exit 1
fi
