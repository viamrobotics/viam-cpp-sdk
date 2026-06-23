#!/usr/bin/env bash
#
# Sourced helpers shared by the install-*.sh scripts.
# Distro-conditional logic lives in these scripts; the Dockerfile stays structural.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

# ---- pinned toolchain versions (not configurable at docker build time) ----

# LLVM_VERSION: the clang/clang-tidy/clang-format major version installed by
# install-llvm.sh.
LLVM_VERSION=19
export LLVM_VERSION

# CMAKE_MIN_VERSION: minimum cmake install-cmake.sh accepts before pulling a
# newer one from Kitware (Ubuntu) or Debian backports.
CMAKE_MIN_VERSION=3.25
export CMAKE_MIN_VERSION

# GRPC_APT_VERSION_MIN: minimum apt libgrpc++-dev install-grpc.sh accepts before
# falling back to a source build. Doubles as the grpc tag (sans leading "v") built
# from source when no valid GRPC_SOURCE_VERSION_OVERRIDE is supplied.
GRPC_APT_VERSION_MIN=1.51.1
export GRPC_APT_VERSION_MIN

# ---- distro identity (from /etc/os-release, never from build args) ----

# shellcheck disable=SC1091
. /etc/os-release

# DISTRO_ID:   debian | ubuntu
# DISTRO_CODENAME: bookworm | bullseye | sid | focal | jammy | ...
DISTRO_ID="${ID}"
DISTRO_CODENAME="${VERSION_CODENAME:-}"

# Debian sid carries no VERSION_CODENAME; pin it explicitly.
if [[ -z "${DISTRO_CODENAME}" && "${DISTRO_ID}" == "debian" ]]; then
    DISTRO_CODENAME="sid"
fi

export DISTRO_ID DISTRO_CODENAME

is_debian() { [[ "${DISTRO_ID}" == "debian" ]]; }
is_ubuntu() { [[ "${DISTRO_ID}" == "ubuntu" ]]; }

# ---- apt ----

# apt_install <pkg>...  Atomic update-install-clean in one shot.
apt_install() {
    apt-get update
    apt-get install --no-install-recommends -y "$@"
    rm -rf /var/lib/apt/lists/*
}

# apt_install_from <repo> <pkg>...  Same, but -t <repo> for a pinned suite.
apt_install_from() {
    local repo="$1"
    shift
    apt-get update
    apt-get install --no-install-recommends -y -t "${repo}" "$@"
    rm -rf /var/lib/apt/lists/*
}

# apt_candidate <pkg>  Print the version apt would install for <pkg>, with the
# Debian revision suffix stripped (e.g. "3.25.1" from "3.25.1-1"). Prints
# nothing when there is no installable candidate ("(none)" or unknown package).
#
# `apt-cache policy` is the only query that reports the candidate apt actually
# picks by pin priority, and reports "(none)" for a name that is known but
# uninstallable ("referred to by another package") -- which `apt-cache show`
# wrongly reports as present. `apt list -a` / `apt show` use the unstable apt
# CLI and list every version, not the chosen one.
#
# Assumes an apt cache is present (apt-get update already run).
apt_candidate() {
    local line ver
    while IFS= read -r line; do
        [[ "${line}" == *Candidate:* ]] || continue
        ver="${line##*Candidate: }"
        [[ "${ver}" == "(none)" ]] && return 0
        printf '%s' "${ver%%-*}"
        return 0
    done < <(apt-cache policy "$1" 2>/dev/null)
}

# add_apt_repo <name> <key-url> <repo-line>
# Fetches the key to /usr/share/keyrings/<name>.gpg and writes a signed-by source.
# No apt-key add.
add_apt_repo() {
    local name="$1" key_url="$2" repo_line="$3"
    local keyring="/usr/share/keyrings/${name}.gpg"
    wget -qO - "${key_url}" | gpg --dearmor -o "${keyring}"
    echo "deb [signed-by=${keyring}] ${repo_line}" \
        > "/etc/apt/sources.list.d/${name}.list"
}

# ---- version comparison ----

# version_ge <a> <b>  True when a >= b (dotted numeric compare).
version_ge() {
    [[ "$1" == "$2" ]] && return 0
    local lowest
    lowest="$(printf '%s\n%s\n' "$1" "$2" | sort -V | head -n1)"
    [[ "${lowest}" == "$2" ]]
}
