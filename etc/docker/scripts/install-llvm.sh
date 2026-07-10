#!/usr/bin/env bash
#
# LLVM toolchain (clang, clang-tidy, clang-format) at ${LLVM_VERSION}.
# Version-gated: use the distro's own clang-${LLVM_VERSION} when apt offers it,
# else add apt.llvm.org for the current distro codename.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

# LLVM_VERSION is pinned in lib/common.sh.
LLVM_PKGS=(
    "clang-${LLVM_VERSION}"
    "clang-tidy-${LLVM_VERSION}"
    "clang-format-${LLVM_VERSION}"
)

apt-get update
clang_candidate="$(apt_candidate "clang-${LLVM_VERSION}")"
rm -rf /var/lib/apt/lists/*

if [[ -n "${clang_candidate}" ]]; then
    # Distro ships it directly.
    apt_install "${LLVM_PKGS[@]}"
elif [[ $(dpkg --print-architecture) != "armhf" ]]; then
    # apt.llvm.org suite name: llvm-toolchain-<codename>-<version>, except
    # Debian sid which drops the codename segment (llvm-toolchain-<version>).
    if [[ "${DISTRO_CODENAME}" == "sid" ]]; then
        repo_path="unstable"
        suite="llvm-toolchain-${LLVM_VERSION}"
    else
        repo_path="${DISTRO_CODENAME}"
        suite="llvm-toolchain-${DISTRO_CODENAME}-${LLVM_VERSION}"
    fi
    add_apt_repo "llvm" \
        "https://apt.llvm.org/llvm-snapshot.gpg.key" \
        "http://apt.llvm.org/${repo_path}/ ${suite} main"
    apt_install_from "${suite}" "${LLVM_PKGS[@]}"
fi
