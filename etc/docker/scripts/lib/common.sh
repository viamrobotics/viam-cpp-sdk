#!/usr/bin/env bash
#
# Sourced helpers shared by the install-*.sh scripts.
# Distro-conditional logic lives in these scripts; the Dockerfile stays structural.

set -euo pipefail
[[ "${DEBUG:-}" ]] && set -x

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
