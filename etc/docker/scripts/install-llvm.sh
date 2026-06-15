#!/usr/bin/env bash
#
# LLVM toolchain (clang, clang-tidy, clang-format) at ${LLVM_VERSION}.
# Version-gated: use the distro's own clang-${LLVM_VERSION} when apt offers it,
# else add apt.llvm.org for the current distro codename.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# shellcheck disable=SC1091
. "$(dirname "$0")/lib/common.sh"

: "${LLVM_VERSION:?LLVM_VERSION must be set}"

# All three packages are version-suffixed; bare names are wired up via
# update-alternatives below.
LLVM_PKGS=(
    "clang-${LLVM_VERSION}"
    "clang-tidy-${LLVM_VERSION}"
    "clang-format-${LLVM_VERSION}"
)

# Point the unversioned tool names at this LLVM_VERSION so callers that invoke
# `clang` / `clang-tidy` / `clang-format` get the pinned toolchain.
register_alternatives() {
    update-alternatives --install /usr/bin/clang clang \
        "/usr/bin/clang-${LLVM_VERSION}" 100 \
        --slave /usr/bin/clang++ clang++ "/usr/bin/clang++-${LLVM_VERSION}"
    update-alternatives --install /usr/bin/clang-tidy clang-tidy \
        "/usr/bin/clang-tidy-${LLVM_VERSION}" 100
    update-alternatives --install /usr/bin/clang-format clang-format \
        "/usr/bin/clang-format-${LLVM_VERSION}" 100
}

apt-get update
if apt-cache show "clang-${LLVM_VERSION}" > /dev/null 2>&1; then
    # Distro ships it directly.
    rm -rf /var/lib/apt/lists/*
    apt_install "${LLVM_PKGS[@]}"
    register_alternatives
else
    rm -rf /var/lib/apt/lists/*
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
    register_alternatives
fi
