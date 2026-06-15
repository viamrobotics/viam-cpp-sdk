#!/usr/bin/env bash
#
# cmake >= ${CMAKE_MIN_VERSION}.
# Version-gated: take the distro cmake when it's recent enough, else pull a newer
# one from Kitware (Ubuntu) or the Debian backports archive (Debian).

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

: "${CMAKE_MIN_VERSION:?CMAKE_MIN_VERSION must be set}"

# Candidate version apt would install, e.g. "3.25.1-1". Empty if no candidate.
apt-get update
candidate="$(apt-cache policy cmake 2>/dev/null | awk '/Candidate:/ {print $2}')"
rm -rf /var/lib/apt/lists/*

# Strip the Debian revision suffix for a clean dotted compare.
candidate_ver="${candidate%%-*}"

if [[ -n "${candidate_ver}" && "${candidate_ver}" != "(none)" ]] \
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
    apt_install_from "${DISTRO_CODENAME}-backports" cmake
else
    echo "install-cmake.sh: unsupported distro '${DISTRO_ID}'" >&2
    exit 1
fi
